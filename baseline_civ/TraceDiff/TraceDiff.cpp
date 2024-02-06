#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Bitcode/BitcodeWriter.h"
using namespace llvm;

#define DEBUG_PRINT 0

namespace {
  struct TraceDiffPass : public FunctionPass {
    static char ID;
    TraceDiffPass() : FunctionPass(ID) {}

    CallInst* injectFPProfileCall(Instruction& point, Instruction& I, BasicBlock& BB, IRBuilder<>& builder, Module* module, bool after) {
      // Declare C standard library printf 
      Type *intType = Type::getInt32Ty(module->getContext());
      std::vector<Type *> printfArgsTypes({Type::getInt8PtrTy(module->getContext())});
      FunctionType *printfType = FunctionType::get(intType, printfArgsTypes, true);
      FunctionCallee printfFunc = module->getOrInsertFunction("printf", printfType);
      std::string printStr = "ins ";
      if (after)
        printStr += " after ";
      else
        printStr += "before ";
      printStr += I.getOpcodeName();
      printStr += "\n";
      builder.SetInsertPoint(&point);
      Value *str = builder.CreateGlobalStringPtr(printStr.c_str(), printStr.c_str());
      std::vector<Value *> argsV({str});
      CallInst* call = builder.CreateCall(printfFunc, argsV, "calltmp");
      errs() << "injected " << printStr;
      return call;
    }

    virtual bool runOnFunction(Function &F) {
      errs() << "I saw a function called " << F.getName() << "!\n";
      #if DEBUG_PRINT
      errs() << "Instruction count " << F.getInstructionCount() << "\n";
      #endif
      Module* module = F.getParent();
      IRBuilder<> builder(module->getContext());

      for (BasicBlock &BB : F) {
        #if DEBUG_PRINT
        errs() << "Basic block (name=" << BB.getName() << ") has "
                  << BB.size() << " instructions.\n";
        #endif
        Instruction* PrevI = nullptr;
        bool prevHasFloat = false;
        for (Instruction &I : BB) {
          CallInst* call = nullptr;
          if (prevHasFloat) {
            prevHasFloat = false;
            call = injectFPProfileCall(I, *PrevI, BB, builder, module, true);
          }
          if (I.getOpcode() == Instruction::PHI)
            continue;
          bool hasFloat = false;
          for (unsigned int opI = 0; opI < I.getNumOperands(); opI++) {
            Value* op = I.getOperand(opI);
            if (op->getType()->isFPOrFPVectorTy()) {
              hasFloat = true;
            }
            if (LoadInst *LMemI = dyn_cast<LoadInst>(&I)) {
              Value* PtrValue = LMemI->getPointerOperand();
              Type* PointerElementType = LMemI->getType();
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

    virtual bool doFinalization(Module& M) {
      std::error_code ec;
      errs() << "filename1:" << M.getSourceFileName() << "\n";     
      std::string base_filename = M.getSourceFileName().substr(M.getSourceFileName().find_last_of("/\\") + 1);     
      std::string bcFilename = base_filename.substr(0, base_filename.find_last_of('.'))+".bc";
      errs() << "filename2:" << bcFilename << "\n";      
      ToolOutputFile result(bcFilename, ec, sys::fs::OF_None);
      WriteBitcodeToFile(M, result.os());
      result.keep();
      return false;
    }
  };
}

char TraceDiffPass::ID = 0;

static RegisterPass<TraceDiffPass> X("tracediff", "TraceDiffPass", false, false);

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerTraceDiffPass(const PassManagerBuilder &,
                         legacy::PassManagerBase &PM) {
  PM.add(new TraceDiffPass());
}
static RegisterStandardPasses
  RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                 registerTraceDiffPass);