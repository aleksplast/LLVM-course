#include "FullIR.hpp"
#include "CPU.hpp"
#include <iostream>
#include <unordered_map>
#include <llvm-18/llvm/IR/Verifier.h>
#include <llvm-18/llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm-18/llvm/ExecutionEngine/GenericValue.h>

namespace KIRCulator {

extern "C" {
    void sim_init();
    void sim_fini();
}

struct FullExternals {
    Function *ScreenFlush;
    Function *ScreenPutPixel;
    Function *SimRand;
};

static void build_func_full_ir(const FuncInfo &fi, Function *Func,
                               ArrayType *regFileType, GlobalVariable *regFile,
                               GlobalVariable *stackGlobal, IRBuilder<> &Builder,
                               Module *IRModule, const FullExternals &ext) {
    LLVMContext &Ctx = IRModule->getContext();
    Type *int8Type  = Type::getInt8Ty(Ctx);
    Type *int32Type = Type::getInt32Ty(Ctx);
    Type *int64Type = Type::getInt64Ty(Ctx);
    FunctionType *voidFT = FunctionType::get(Type::getVoidTy(Ctx), false);

    std::unordered_map<uint32_t, BasicBlock *> BBMap;
    for (auto &bb_name : fi.bb_names) {
        BBMap[fi.bb2pc.at(bb_name)] = BasicBlock::Create(Ctx, bb_name, Func);
    }

    uint32_t PC = 0;
    Builder.SetInsertPoint(BBMap[0]);

    for (const Instr &instr : fi.insns) {
        uint32_t r1 = instr.r1;
        uint32_t r2 = instr.r2;
        uint32_t r3 = instr.r3imm;
        int64_t imm = (int64_t)instr.r3imm;

        ++PC;
        BasicBlock *nextBB = BBMap.count(PC) ? BBMap.at(PC) : nullptr;

        auto emitFallThrough = [&]() {
            if (nextBB && !Builder.GetInsertBlock()->getTerminator())
                Builder.CreateBr(nextBB);
        };

        switch (instr.opcode) {
        default: break;
#define ISA(Opcode_, Name_, Skip_, Read_, Write_, Exec_, IRGen_) \
        case (Opcode_): { IRGen_; break; }
#include "ISA.hpp"
#undef ISA
        }

        emitFallThrough();

        if (nextBB)
            Builder.SetInsertPoint(nextBB);
    }

    for (auto &[pc, bb] : BBMap) {
        if (!bb->getTerminator()) {
            Builder.SetInsertPoint(bb);
            Builder.CreateRetVoid();
        }
    }
}

void FullIRGen::build_ir(const Parser &parser) {
    auto voidType  = Type::getVoidTy(Context);
    auto int64Type = Type::getInt64Ty(Context);
    auto int8Type  = Type::getInt8Ty(Context);

    ArrayType *regFileType = ArrayType::get(int64Type, CPU::kRegNum);
    IRModule->getOrInsertGlobal("reg_file", regFileType);
    GlobalVariable *regFile = IRModule->getNamedGlobal("reg_file");

    IRModule->getOrInsertGlobal("stack", int8Type);
    GlobalVariable *stackGlobal = IRModule->getNamedGlobal("stack");

    FullExternals ext;
    ext.ScreenFlush = declareScreenFlush();
    ext.ScreenPutPixel = declareScreenPutPixel();
    ext.SimRand = declareSimRand();

    FunctionType *voidFuncType = FunctionType::get(voidType, false);

    for (const FuncInfo &fi : parser.funcs)
        Function::Create(voidFuncType, Function::ExternalLinkage, fi.name, *IRModule);

    for (const FuncInfo &fi : parser.funcs) {
        Function *Func = IRModule->getFunction(fi.name);
        build_func_full_ir(fi, Func, regFileType, regFile, stackGlobal, Builder, IRModule.get(), ext);
    }
}

void FullIRGen::exec(CPU &cpu) {
    IRModule->dump();
    if (verifyModule(*IRModule, &outs())) {
        return;
    }

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

    GlobalVariable *regFile     = IRModule->getNamedGlobal("reg_file");
    GlobalVariable *stackGlobal = IRModule->getNamedGlobal("stack");

    ExecutionEngine *ee = EngineBuilder(std::move(IRModule)).create();
    ee->InstallLazyFunctionCreator([](const std::string &fnName) -> void * {
        if (fnName == "screen_flush")     return reinterpret_cast<void *>(screen_flush);
        if (fnName == "sim_rand")         return reinterpret_cast<void *>(sim_rand);
        if (fnName == "screen_put_pixel") return reinterpret_cast<void *>(screen_put_pixel);
        return nullptr;
    });

    ee->addGlobalMapping(regFile,     (void *)cpu.reg_file);
    ee->addGlobalMapping(stackGlobal, (void *)cpu.stack.get());
    ee->finalizeObject();

    cpu.reg_file[9] = CPU::kStackSize;

    sim_init();

    ArrayRef<GenericValue> noargs;
    auto *App = ee->FindFunctionNamed(kAppName);
    ee->runFunction(App, noargs);

    sim_fini();
}

} // namespace KIRCulator
