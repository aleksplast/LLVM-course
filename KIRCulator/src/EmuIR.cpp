#include "EmuIR.hpp"
#include "CPU.hpp"
#include <iostream>

#include <llvm-18/llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm-18/llvm/ExecutionEngine/GenericValue.h>
#include <llvm-18/llvm/IR/Verifier.h>

extern "C" {
    void sim_init();
    void sim_fini();
}

namespace KIRCulator {

static void build_func_ir(const KIRCulator::FuncInfo &fi, Function *LLFunc,
                          ArrayType *regFileType, GlobalVariable *regFile,
                          IRBuilder<> &Builder, Module *IRModule,
                          FunctionType *doFuncType) {
    auto *Context = &IRModule->getContext();
    auto int64Type = Type::getInt64Ty(*Context);

#define ISA(Opcode_, Name_, Skip_, Read_, Write_, Exec_, IRGen_)              \
    FunctionCallee Callee##Name_ =                                            \
        IRModule->getOrInsertFunction("do_" #Name_, doFuncType);
#include "ISA.hpp"
#undef ISA

    std::unordered_map<uint32_t, BasicBlock *> BBMap;
    for (auto &bb_name : fi.bb_names)
        BBMap[fi.bb2pc.at(bb_name)] =
            BasicBlock::Create(*Context, bb_name, LLFunc);

    uint32_t PC = 0;
    Builder.SetInsertPoint(BBMap[0]);

    for (const Instr &instr : fi.insns) {
        Value *arg1 = Builder.getInt64(instr.r1);
        Value *arg2 = Builder.getInt64(instr.r2);
        Value *arg3 = Builder.getInt64(instr.r3imm);
        Value *args[] = {arg1, arg2, arg3};

        switch (instr.opcode) {
        default: break;
#define ISA(Opcode_, Name_, Skip_, Read_, Write_, Exec_, IRGen_)              \
        case (Opcode_): Builder.CreateCall(Callee##Name_, args); break;
#include "ISA.hpp"
#undef ISA
        }

        ++PC;
        auto BB = BBMap.find(PC);

        switch (instr.opcode) {
        default:
            if (BB != BBMap.end()) {
                Builder.CreateBr(BB->second);
            }
            break;
        case Instr::BR_COND: {
            Value *condPtr = Builder.CreateConstGEP2_32(regFileType, regFile, 0, (uint32_t)instr.r1);
            Value *cond = Builder.CreateTrunc(Builder.CreateLoad(int64Type, condPtr), Builder.getInt1Ty());
            Builder.CreateCondBr(cond, BBMap[instr.r3imm], BBMap[PC]);
            break;
        }
        case Instr::B:
            Builder.CreateBr(BBMap[instr.r3imm]);
            break;
        case Instr::BR:
            Builder.CreateRetVoid();
            break;

        case Instr::CALL:
            if (!instr.call_target.empty()) {
                FunctionCallee callee =
                    IRModule->getOrInsertFunction(instr.call_target,
                        FunctionType::get(Type::getVoidTy(*Context), false));
                Builder.CreateCall(callee);
            }
            if (BB != BBMap.end())
                Builder.CreateBr(BB->second);
            break;
        }

        if (BB != BBMap.end())
            Builder.SetInsertPoint(BB->second);
    }

    for (auto &[pc, bb] : BBMap) {
        if (!bb->getTerminator()) {
            Builder.SetInsertPoint(bb);
            Builder.CreateRetVoid();
        }
    }
}

void EmuIRGen::build_ir(const Parser &parser) {
    auto voidType  = Type::getVoidTy(Context);
    auto int64Type = Type::getInt64Ty(Context);

    ArrayType *regFileType = ArrayType::get(int64Type, CPU::kRegNum);
    IRModule->getOrInsertGlobal("reg_file", regFileType);
    regFile = IRModule->getNamedGlobal("reg_file");

    ArrayRef<Type *> argTypes = {int64Type, int64Type, int64Type};
    FunctionType *doFuncType  = FunctionType::get(voidType, argTypes, false);
    FunctionType *voidFuncType = FunctionType::get(voidType, false);

    for (const FuncInfo &fi : parser.funcs)
        Function::Create(voidFuncType, Function::ExternalLinkage,
                         fi.name, *IRModule);

    for (const FuncInfo &fi : parser.funcs) {
        Function *LLFunc = IRModule->getFunction(fi.name);
        build_func_ir(fi, LLFunc, regFileType, regFile,
                      Builder, IRModule.get(), doFuncType);
    }
}

void EmuIRGen::exec(CPU &cpu) {
    IRModule->dump();

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

    ExecutionEngine *ee = EngineBuilder(std::move(IRModule)).create();
    ee->InstallLazyFunctionCreator([](const std::string &fnName) -> void * {
#define ISA(Opcode_, Name_, Skip_, Read_, Write_, Exec_, IRGen_)              \
        if (fnName == "do_" #Name_)                                           \
            return reinterpret_cast<void *>(CPU::do_##Name_);
#include "ISA.hpp"
#undef ISA
        return nullptr;
    });

    ee->addGlobalMapping(regFile, (void *)cpu.reg_file);
    ee->finalizeObject();

    CPU::set_cpu(&cpu);

    cpu.reg_file[9] = CPU::kStackSize;

    sim_init();

    ArrayRef<GenericValue> noargs;
    auto *App = ee->FindFunctionNamed(kAppName);
    ee->runFunction(App, noargs);

    sim_fini();
}

} // namespace KIRCulator
