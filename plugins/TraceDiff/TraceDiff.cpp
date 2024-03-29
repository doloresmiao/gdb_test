#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include <iostream>
#include <sstream>
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
		std::stringstream ss;
		const DebugLoc &location = I.getDebugLoc();
		if (location) {
			unsigned int line = location.getLine();
			unsigned int col = location.getCol();
			auto *scope = cast<DIScope>(location.getScope());
			std::string fileName = scope->getFilename().data();
			ss << "file:" << fileName << " line:" << line << " col:" << col << " | ";
		} else {
			ss << "no debug loc available | ";
		}

		// Declare C standard library printf
		std::string printStr = ss.str() + "ins ";
		if (after)
			printStr += " after ";
		else
			printStr += "before ";
		printStr += I.getOpcodeName();

		builder.SetInsertPoint(&point);
		Value *str = nullptr;
		std::vector<Value *> argsV;
		FunctionCallee printfFunc;
		bool isOpVector = false;
		bool isResultVector = false;
		for (unsigned int opI = 0; opI < I.getNumOperands(); opI++) {
			Value *op = I.getOperand(opI);
			if (op->getType()->isVectorTy()) {
				isOpVector = true;
				break;
			}
		}
		Value *result = dyn_cast<Value>(&I);
		if (result->getType()->isVectorTy()) {
			isResultVector = true;
		}

		if (CallInst *callI = dyn_cast<CallInst>(&I)) {
			printStr += " ";
			printStr += getFunctionName(callI);
		}

		if (after && isa<ReturnInst>(&I)) {
			return nullptr;
		} else if (StoreInst *storeI = dyn_cast<StoreInst>(&I)) {
			return nullptr;
		} else if (LoadInst *loadI = dyn_cast<LoadInst>(&I)) {
			return nullptr;
		} else {
			if (after) {
				Type *intType = Type::getInt32Ty(module->getContext());
				std::vector<Type *> printfArgsTypes(
						{Type::getInt8PtrTy(module->getContext())});
				argsV.push_back(nullptr);
				Value *result = dyn_cast<Value>(&I);
				if (result->getType()->isVectorTy()) {
					VectorType* vt = dyn_cast<VectorType>(result->getType());
					ElementCount ct = vt->getElementCount();
					Type* et = vt->getElementType();
					if (ct.isScalable() || !et->isFPOrFPVectorTy())
						printStr += " (vector)";
					else {
						printStr += " (";

						for (int vi = 0; vi < ct.getFixedValue(); vi++) {
							Value* val = builder.CreateExtractElement(result, vi);
							printfArgsTypes.push_back(et);
							argsV.push_back(val);
							printStr += "%.17g";
							if (vi != ct.getFixedValue() - 1)
								printStr += " ";
						}
						printStr += ")\n";							
					}
				}
				else if (result->getType()->isFPOrFPVectorTy()) {
					printfArgsTypes.push_back(result->getType());
					argsV.push_back(result);
					printStr += " %.17g\n";					
				}
				else if (result->getType()->isIntegerTy()) {
					printfArgsTypes.push_back(result->getType());
					argsV.push_back(result);
					printStr += " %d\n";							
				}
				else {
					printStr += " (nf)\n";
				}
				FunctionType *printfType =
						FunctionType::get(intType, printfArgsTypes, true);
				printfFunc = module->getOrInsertFunction("printf", printfType);
				str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
				argsV[0] = str;
			} else {
				Type *intType = Type::getInt32Ty(module->getContext());
				std::vector<Type *> printfArgsTypes(
						{Type::getInt8PtrTy(module->getContext())});
				argsV.push_back(nullptr);
				for (unsigned int opI = 0; opI < I.getNumOperands(); opI++) {
					Value *op = I.getOperand(opI);
					if (op->getType()->isVectorTy()) {
						VectorType* vt = dyn_cast<VectorType>(op->getType());
						ElementCount ct = vt->getElementCount();
						Type* et = vt->getElementType();
						if (ct.isScalable() || !et->isFPOrFPVectorTy())
							printStr += " (vector)";
						else {
							printStr += " (";

							for (int vi = 0; vi < ct.getFixedValue(); vi++) {
								Value* val = builder.CreateExtractElement(op, vi);
							    printfArgsTypes.push_back(et);
								argsV.push_back(val);
								printStr += "%.17g";
								if (vi != ct.getFixedValue() - 1)
									printStr += " ";
							}
							printStr += ")";							
						}
					}
					else if (op->getType()->isFPOrFPVectorTy()) {
						printfArgsTypes.push_back(op->getType());
						argsV.push_back(op);
						printStr += " %.17g";
					}
					else if (op->getType()->isIntegerTy()) {
						printfArgsTypes.push_back(op->getType());
						argsV.push_back(op);
						printStr += " %d";							
					}					
					else {
						printStr += " (nf)";
					}
				}
				printStr += "\n";
				FunctionType *printfType =
						FunctionType::get(intType, printfArgsTypes, true);
				printfFunc = module->getOrInsertFunction("printf", printfType);
				str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
				argsV[0] = str;
			}	
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

static RegisterPass<TraceDiffPass> X("tracediff", "TraceDiffPass", false, false);