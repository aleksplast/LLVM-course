#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include <llvm-18/llvm/IR/Instruction.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/PassManager.h>
#include <llvm-18/llvm/IR/Value.h>
#include <unordered_set>
using namespace llvm;

struct TracerPass : public PassInfoMixin<TracerPass> {
  Type *voidType;
  Type *int8PtrTy;

  const StringRef LOG_FUNC_NAME = "insn_logger";

  bool isFuncLogger(StringRef name) {
    return name == LOG_FUNC_NAME;
  }

  std::vector<Value *> getOperandsNames(Instruction &I, IRBuilder<> &Builder, std::unordered_set<Instruction *> Visited) {
    if (Visited.find(&I) != Visited.end()) {
      return {};
    }

    std::vector<Value *> Names;
    Visited.emplace(&I);

    for (auto &Op : I.operands()) {
      if (!isa<Instruction>(Op)) {
        continue;
      }

      auto *OpInsn = cast<Instruction>(Op);

      if (isa<PHINode>(*OpInsn)) {
        auto PHIOpNames = getOperandsNames(*OpInsn, Builder, Visited);
        Names.insert(Names.end(), PHIOpNames.begin(), PHIOpNames.end());
      } else {
        Value *OpName = Builder.CreateGlobalStringPtr(OpInsn->getOpcodeName());
        Names.emplace_back(OpName);
      }
    }

    return Names;
  }

  bool insertInsnLog(Module &M, Instruction &I, IRBuilder<> &Builder) {
    if (isa<PHINode>(I)) {
      return false;
    }

    ArrayRef<Type *> logFuncParamTypes = {int8PtrTy, int8PtrTy};
    FunctionType *logFuncType =
        FunctionType::get(voidType, logFuncParamTypes, false);
    FunctionCallee funcStartLogFunc =
        M.getOrInsertFunction(LOG_FUNC_NAME, logFuncType);

    Builder.SetInsertPoint(&I);
    Value *InstrName = Builder.CreateGlobalStringPtr(I.getOpcodeName());

    std::unordered_set<Instruction *> Visited;
    auto OpNames = getOperandsNames(I, Builder, Visited);
    for (auto *OpName: OpNames) {
      Value *args[] = {InstrName, OpName};
      Builder.CreateCall(funcStartLogFunc, args);
    }

    return true;
  }

  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    LLVMContext &Ctx = M.getContext();
    IRBuilder<> builder(Ctx);
    voidType = Type::getVoidTy(Ctx);
    int8PtrTy = Type::getInt8Ty(Ctx)->getPointerTo();

    bool Changed = false;

    for (auto &F: M) {
      for (auto &BB : F) {
        if (isFuncLogger(F.getName()) || F.isDeclaration()) {
          continue;
        }

        for (auto &I: BB) {
          bool ChangedOnIter = insertInsnLog(M, I, builder);
          Changed = Changed ? Changed : ChangedOnIter;
        }
      }
    }
    return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
  };
};

PassPluginLibraryInfo getPassPluginInfo() {
  const auto callback = [](PassBuilder &PB) {
    PB.registerOptimizerLastEPCallback([](ModulePassManager &MPM, auto) {
      MPM.addPass(TracerPass{});
      return true;
    });
  };

  return {LLVM_PLUGIN_API_VERSION, "TracePlugin", "0.0.1", callback};
};

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
  return getPassPluginInfo();
}
