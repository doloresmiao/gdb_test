#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
using namespace llvm;

#define DEBUG_PRINT 0

namespace {
struct TraceDiffPass : public FunctionPass {
	static char ID;
	TraceDiffPass() : FunctionPass(ID) {}

	StringRef getFunctionName(CallInst *call) {
		Function *fun = call->getCalledFunction();
		if (fun)
			return fun->getName();
		else
			return StringRef("indirect call");
	}

	CallInst *injectFPProfileCall(Instruction &point, Instruction &I,
																BasicBlock &BB, IRBuilder<> &builder,
																Module *module, bool after) {
		// Declare C standard library printf
		std::string printStr = "ins ";
		if (after)
			printStr += " after ";
		else
			printStr += "before ";
		printStr += I.getOpcodeName();

		builder.SetInsertPoint(&point);
		Value *str = nullptr;
		std::vector<Value *> argsV;
		FunctionCallee printfFunc;
		if (ReturnInst* retI = dyn_cast<ReturnInst>(&I)) {
			return nullptr;
		}
		else if (CallInst *callI = dyn_cast<CallInst>(&I)) {
			printStr += getFunctionName(callI);
			if (after) {
				Value *result = dyn_cast<Value>(&I);
				if (result->getType()->isX86_FP80Ty()) {
					printStr += " %.17g\n";
					Type *intType = Type::getInt32Ty(module->getContext());
					std::vector<Type *> printfArgsTypes(
							{Type::getInt8PtrTy(module->getContext()),
							Type::getX86_FP80Ty(module->getContext())});
					FunctionType *printfType =
							FunctionType::get(intType, printfArgsTypes, true);
					printfFunc = module->getOrInsertFunction("printf", printfType);
					str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
					
					argsV.push_back(str);
					argsV.push_back(result);					
				}
				else if (result->getType()->isFPOrFPVectorTy()) {
					printStr += " %.17g\n";
					Type *intType = Type::getInt32Ty(module->getContext());
					std::vector<Type *> printfArgsTypes(
							{Type::getInt8PtrTy(module->getContext()),
							Type::getDoubleTy(module->getContext())});
					FunctionType *printfType =
							FunctionType::get(intType, printfArgsTypes, true);
					printfFunc = module->getOrInsertFunction("printf", printfType);
					str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
					
					argsV.push_back(str);
					argsV.push_back(result);
				} else {
					printStr += "\n";
					Type *intType = Type::getInt32Ty(module->getContext());
					std::vector<Type *> printfArgsTypes(
							{Type::getInt8PtrTy(module->getContext())});
					FunctionType *printfType =
							FunctionType::get(intType, printfArgsTypes, true);
					printfFunc = module->getOrInsertFunction("printf", printfType);
					str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
					argsV.push_back(str);
				}
			}
			else {
				std::vector<Type *> printfArgsTypes(
						{Type::getInt8PtrTy(module->getContext())});
				Type *intType = Type::getInt32Ty(module->getContext());
				FunctionType *printfType =
						FunctionType::get(intType, printfArgsTypes, true);
				printfFunc = module->getOrInsertFunction("printf", printfType);
				str = nullptr;
				argsV.push_back(str);
				for (unsigned int opI = 0; opI < callI->getNumOperands(); opI++) {
					Value *op = I.getOperand(opI);
					if (op->getType()->isFPOrFPVectorTy()) {
						printStr += " %.17g";
						if (op->getType()->isX86_FP80Ty()) {
							printfArgsTypes.push_back(Type::getX86_FP80Ty(module->getContext()));
						}
						else {
							printfArgsTypes.push_back(Type::getDoubleTy(module->getContext()));
						}
						argsV.push_back(callI->getOperand(opI));
					}
					else {
						printStr += " (nf)";
					}
				}				
				printStr += "\n";
				str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
				argsV[0] = str;
			}
		} else if (StoreInst *storeI = after ? nullptr : dyn_cast<StoreInst>(&I)) {
			Value *value_to_store = storeI->getOperand(0);
			if (value_to_store->getType()->isVectorTy()) {
				printStr += " (vector)\n";
				Type *intType = Type::getInt32Ty(module->getContext());
				std::vector<Type *> printfArgsTypes(
						{Type::getInt8PtrTy(module->getContext())});
				FunctionType *printfType =
						FunctionType::get(intType, printfArgsTypes, true);
				printfFunc = module->getOrInsertFunction("printf", printfType);
				str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
				argsV.push_back(str);
			}
			else {
				printStr += " %.17g %x\n";
				Type *intType = Type::getInt32Ty(module->getContext());
				std::vector<Type *> printfArgsTypes(
						{Type::getInt8PtrTy(module->getContext()),
						Type::getDoubleTy(module->getContext()),
					 	Type::getDoublePtrTy(module->getContext())});
				FunctionType *printfType =
						FunctionType::get(intType, printfArgsTypes, true);
				printfFunc = module->getOrInsertFunction("printf", printfType);
				str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
				Value *address_of_store = storeI->getOperand(1);
				argsV.push_back(str);
				argsV.push_back(value_to_store);
				argsV.push_back(address_of_store);
			}
		} else if (LoadInst *loadI = after ? dyn_cast<LoadInst>(&I) : nullptr) {
			printStr += " %.17g\n";
			Type *intType = Type::getInt32Ty(module->getContext());
			std::vector<Type *> printfArgsTypes(
					{Type::getInt8PtrTy(module->getContext()),
					 Type::getDoubleTy(module->getContext())});
			FunctionType *printfType =
					FunctionType::get(intType, printfArgsTypes, true);
			printfFunc = module->getOrInsertFunction("printf", printfType);
			str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
			Value *result = dyn_cast<Value>(loadI);
			argsV.push_back(str);
			argsV.push_back(result);
		} else if (isa<FPMathOperator>(&I) && I.isUnaryOp()) {
			if (after) {
				printStr += " %.17g\n";
				Type *intType = Type::getInt32Ty(module->getContext());
				std::vector<Type *> printfArgsTypes(
						{Type::getInt8PtrTy(module->getContext()),
						 Type::getDoubleTy(module->getContext())});
				FunctionType *printfType =
						FunctionType::get(intType, printfArgsTypes, true);
				printfFunc = module->getOrInsertFunction("printf", printfType);
				str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
				Value *result = dyn_cast<Value>(&I);
				argsV.push_back(str);
				argsV.push_back(result);
			} else {
				printStr += " %.17g\n";
				Type *intType = Type::getInt32Ty(module->getContext());
				std::vector<Type *> printfArgsTypes(
						{Type::getInt8PtrTy(module->getContext()),
						 Type::getDoubleTy(module->getContext())});
				FunctionType *printfType =
						FunctionType::get(intType, printfArgsTypes, true);
				printfFunc = module->getOrInsertFunction("printf", printfType);
				str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
				Value *num1 = I.getOperand(0);
				argsV.push_back(str);
				argsV.push_back(num1);
			}
		} else if (isa<FPMathOperator>(&I) && I.isBinaryOp()) {
			if (after) {
				printStr += " %.17g\n";
				Type *intType = Type::getInt32Ty(module->getContext());
				std::vector<Type *> printfArgsTypes(
						{Type::getInt8PtrTy(module->getContext()),
						 Type::getDoubleTy(module->getContext())});
				FunctionType *printfType =
						FunctionType::get(intType, printfArgsTypes, true);
				printfFunc = module->getOrInsertFunction("printf", printfType);
				str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
				Value *result = dyn_cast<Value>(&I);
				argsV.push_back(str);
				argsV.push_back(result);
			} else {
				printStr += " %.17g %.17g\n";
				Type *intType = Type::getInt32Ty(module->getContext());
				std::vector<Type *> printfArgsTypes(
						{Type::getInt8PtrTy(module->getContext()),
						 Type::getDoubleTy(module->getContext()),
						 Type::getDoubleTy(module->getContext())});
				FunctionType *printfType =
						FunctionType::get(intType, printfArgsTypes, true);
				printfFunc = module->getOrInsertFunction("printf", printfType);
				str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
				Value *num1 = I.getOperand(0);
				Value *num2 = I.getOperand(1);
				argsV.push_back(str);
				argsV.push_back(num1);
				argsV.push_back(num2);
			}
		} else {
			printStr += "\n";
			Type *intType = Type::getInt32Ty(module->getContext());
			std::vector<Type *> printfArgsTypes(
					{Type::getInt8PtrTy(module->getContext())});
			FunctionType *printfType =
					FunctionType::get(intType, printfArgsTypes, true);
			printfFunc = module->getOrInsertFunction("printf", printfType);
			str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
			argsV.push_back(str);
		}
		CallInst *call = builder.CreateCall(printfFunc, argsV, "calltmp");
		errs() << "injected " << printStr;
		return call;
	}

	virtual bool runOnFunction(Function &F) {
		errs() << "I saw a function called " << F.getName() << "!\n";
#if DEBUG_PRINT
		errs() << "Instruction count " << F.getInstructionCount() << "\n";
#endif
		Module *module = F.getParent();
		IRBuilder<> builder(module->getContext());

		for (BasicBlock &BB : F) {
#if DEBUG_PRINT
			errs() << "Basic block (name=" << BB.getName() << ") has " << BB.size()
						 << " instructions.\n";
#endif
			Instruction *PrevI = nullptr;
			bool prevHasFloat = false;
			for (Instruction &I : BB) {
				CallInst *call = nullptr;
				if (prevHasFloat) {
					prevHasFloat = false;
					call = injectFPProfileCall(I, *PrevI, BB, builder, module, true);
				}
				if (I.getOpcode() == Instruction::PHI)
					continue;
				bool hasFloat = false;
				for (unsigned int opI = 0; opI < I.getNumOperands(); opI++) {
					Value *op = I.getOperand(opI);
					if (op->getType()->isFPOrFPVectorTy()) {
						hasFloat = true;
					}
					if (LoadInst *LMemI = dyn_cast<LoadInst>(&I)) {
						Value *PtrValue = LMemI->getPointerOperand();
						Type *PointerElementType = LMemI->getType();
						if (PointerElementType->isFPOrFPVectorTy()) {
							hasFloat = true;
						}
					}
				}
				if (hasFloat) {
#if DEBUG_PRINT
					errs() << "has float\n";
#endif
					injectFPProfileCall(I, I, BB, builder, module, false);
					prevHasFloat = true;
				}
				PrevI = &I;
			}
		}
		errs() << "end of function " << F.getName() << "!\n";
		return false;
	}

	virtual bool doFinalization(Module &M) {
		std::error_code ec;
		errs() << "filename1:" << M.getSourceFileName() << "\n";
		std::string base_filename = M.getSourceFileName().substr(
				M.getSourceFileName().find_last_of("/\\") + 1);
		std::string bcFilename =
				base_filename.substr(0, base_filename.find_last_of('.')) + ".bc";
		errs() << "filename2:" << bcFilename << "\n";
		ToolOutputFile result(bcFilename, ec, sys::fs::OF_None);
		WriteBitcodeToFile(M, result.os());
		result.keep();
		return false;
	}
};
} // namespace

char TraceDiffPass::ID = 0;

static RegisterPass<TraceDiffPass> X("tracediff", "TraceDiffPass", false,
																		 false);

// Automatically enable the pass. http://adriansampson.net/blog/clangpass.html
static void registerTraceDiffPass(const PassManagerBuilder &,
																	legacy::PassManagerBase &PM) {
	PM.add(new TraceDiffPass());
}
static RegisterStandardPasses
		RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
									 registerTraceDiffPass);