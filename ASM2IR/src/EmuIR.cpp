#include "EmuIR.hpp"
#include "CPU.hpp"
#include <iostream>

#include <llvm-18/llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm-18/llvm/ExecutionEngine/GenericValue.h>
#include <llvm-18/llvm/Transforms/Utils/Cloning.h>
#include <llvm-18/llvm/IR/Verifier.h>

extern "C" {
    void sim_init();
    void sim_fini();
}

namespace ASM2IR {

void EmuIRGen::build_ir(const AsmParser &parser) {
    auto voidType = Type::getVoidTy(Context);
    auto int64Type = Type::getInt64Ty(Context);

    //[32 x i32] reg_file = {0, 0, 0, 0}
    ArrayType *regFileType = ArrayType::get(int64Type, CPU::kRegNum);
    IRModule->getOrInsertGlobal("reg_file", regFileType);
    regFile = IRModule->getNamedGlobal("reg_file");

    FunctionType *FuncType = FunctionType::get(voidType, false);
    Function *AppFunc = Function::Create(FuncType, Function::ExternalLinkage, kAppName, *IRModule);

     // Functions types
    FunctionType *voidFuncType = FunctionType::get(voidType, false);
    ArrayRef<Type *> int32x4Types = {int64Type, int64Type, int64Type, int64Type};
    FunctionType *int32x4FuncType = FunctionType::get(voidType, int32x4Types, false);

    // Functions
#define ISA(Opcode_, Name_, SkipArgs_, ReadArgs_, WriteArgs_, Execute_,        \
             IRGenExecute_)                                                    \
  FunctionCallee Callee##Name_ =                                               \
      IRModule->getOrInsertFunction("do_" #Name_, int32x4FuncType);
#include "ISA.hpp"
#undef ISA

    std::unordered_map<uint32_t, BasicBlock *> BBMap;
    for (auto &bb_name : parser.bb_names) {
        BBMap[parser.bb2pc.at(bb_name)] = BasicBlock::Create(Context, bb_name, AppFunc);
    }

    uint32_t PC = 0;
    Builder.SetInsertPoint(BBMap[0]);
    for (const Instr &instr : parser.insns) {
        Value *arg1 = Builder.getInt64(instr.rd);
        Value *arg2 = Builder.getInt64(instr.rs1);
        Value *arg3 = Builder.getInt64(instr.rs2imm);
        Value *arg4 = Builder.getInt64(instr.rs3imm);
        Value *args[] = {arg1, arg2, arg3, arg4};
        switch (instr.opcode) {
        default:
        break;
    #define ISA(Opcode_, Name_, SkipArgs_, ReadArgs_, WriteArgs_, Execute_,       \
                IRGenExecute_)                                                    \
    case (Opcode_):                                                              \
        Builder.CreateCall(Callee##Name_, args);                                   \
        break;
    #include "ISA.hpp"
    #undef ISA
        }
        PC++;
        auto BB = BBMap.find(PC);

        switch (instr.opcode) {
        default:
            if (BB != BBMap.end()) {
                Builder.CreateBr(BB->second);
            }
        break;
        case Instr::BR_COND:
            arg1 = Builder.CreateConstGEP2_32(regFileType, regFile, 0, instr.rd);
            arg2 = Builder.CreateTrunc(Builder.CreateLoad(int64Type, arg1),
                                        Builder.getInt1Ty());
            Builder.CreateCondBr(arg2, BBMap[instr.rs2imm], BBMap[instr.rs3imm]);
        break;
        case Instr::BRANCH:
            Builder.CreateBr(BBMap[instr.rs2imm]);
        break;
        case Instr::EXIT:
            Builder.CreateRetVoid();
        break;
        }
        if (BB != BBMap.end()) {
            Builder.SetInsertPoint(BB->second);
        }
    }
}

void EmuIRGen::exec(CPU &cpu) {
    IRModule->dump();
    bool verif = verifyModule(*IRModule, &outs());
    auto ExecModule = CloneModule(*IRModule);
    auto *App = ExecModule->getFunction(kAppName);

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

    ExecutionEngine *ee = EngineBuilder(std::move(ExecModule)).create();
    ee->InstallLazyFunctionCreator([](const std::string &fnName) -> void * {
#define ISA(Opcode_, Name_, SkipArgs_, ReadArgs_, WriteArgs_, Execute_,        \
             IRGenExecute_)                                                    \
  if (fnName == "do_" #Name_)                                                  \
    return reinterpret_cast<void *>(CPU::do_##Name_);
#include "ISA.hpp"
#undef ISA
    return nullptr;
  });

    ee->addGlobalMapping(regFile, (void *)cpu.reg_file);
    ee->finalizeObject();

    CPU::set_cpu(&cpu);
    sim_init();

    ArrayRef<GenericValue> noargs;
    ee->runFunction(App, noargs);

    sim_fini();
}

} // namespace ASM2IR
