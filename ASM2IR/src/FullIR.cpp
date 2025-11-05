#include "FullIR.hpp"
#include "ASMParser.hpp"
#include "CPU.hpp"
#include <llvm-18/llvm/IR/Verifier.h>
#include <llvm-18/llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm-18/llvm/ExecutionEngine/GenericValue.h>
#include <llvm-18/llvm/Transforms/Utils/Cloning.h>

namespace ASM2IR {

extern "C" {
    void sim_init();
    void sim_fini();
}

void FullIRGen::build_ir(const AsmParser &parser) {
    auto voidType = Type::getVoidTy(Context);
    auto int64Type = Type::getInt64Ty(Context);
    auto int32PtrType = Type::getInt32Ty(Context)->getPointerTo();

    // @switch.table.app = private unnamed_addr constant [3 x i32] [i32 -16711936, i32 -16776961, i32 -65536], align 4
    auto *SwitchTableTy = ArrayType::get(Builder.getInt32Ty(), 3);
    auto &SwitchTable = globals.emplace_back(std::make_unique<GlobalVariable>(
        *IRModule,
        SwitchTableTy,
        true,
        GlobalValue::PrivateLinkage,
        ConstantArray::get(SwitchTableTy, {
            Builder.getInt32(-16711936),
            Builder.getInt32(-16776961),
            Builder.getInt32(-65536)
        }),
        "switch.table.app"
    ));

    SwitchTable->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
    SwitchTable->setAlignment(Align(4));

    FunctionType *FuncType = FunctionType::get(voidType, false);
    Function *AppFunc = Function::Create(FuncType, Function::ExternalLinkage, kAppName, *IRModule);

    auto *ScreenFlush = declareScreenFlush();
    auto *ScreenPutPixel = declareScreenPutPixel();
    auto *SimRand = declareSimRand();
    auto *DumpGrid = declareDumpGrid();
    auto *DumpNeighbours = declareDumpNeighbours();
    auto *DumpReg = declareDumpReg();

    std::unordered_map<uint32_t, BasicBlock *> BBMap;
    for (auto &bb_name : parser.bb_names) {
        BBMap[parser.bb2pc.at(bb_name)] = BasicBlock::Create(Context, bb_name, AppFunc);
    }

    uint32_t PC = 0;
    Builder.SetInsertPoint(BBMap[0]);
    // %0 = alloca i64, i64 64, align 8
    ArrayType *regFileType = ArrayType::get(int64Type, CPU::kRegNum);
    Value *regFile = Builder.CreateAlloca(regFileType);


    for (const Instr &instr : parser.insns) {
    switch (instr.opcode) {
    default:
      break;
#define ISA(Opcode_, Name_, SkipArgs_, ReadArgs_, WriteArgs_, Execute_,       \
             IRGenExecute_)                                                    \
  case (Opcode_):                                                              \
    IRGenExecute_;                                                             \
    break;
#include "ISA.hpp"
#undef ISA
    }
    PC++;
  }
}

void FullIRGen::exec(CPU &cpu) {
    IRModule->dump();
    bool verif = verifyModule(*IRModule, &outs());
    auto ExecModule = CloneModule(*IRModule);
    auto *App = ExecModule->getFunction(kAppName);
    auto *simFlush = ExecModule->getFunction(kScreenFlushName);
    auto *simRand = ExecModule->getFunction(kSimRandName);
    auto *screenPutPixel = ExecModule->getFunction(kScreenPutPixelName);

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

  ExecutionEngine *ee = EngineBuilder(std::move(ExecModule)).create();
  ee->InstallLazyFunctionCreator([&](const std::string &fnName) -> void * {
        if (fnName == kScreenFlushName) {
            return reinterpret_cast<void *>(screen_flush);
        } else if (fnName == kSimRandName) {
            return reinterpret_cast<void *>(sim_rand);
        } else if (fnName == kScreenPutPixelName) {
            return reinterpret_cast<void *>(screen_put_pixel);
        } else if (fnName == kDumpGridName) {
            return reinterpret_cast<void *>(dump_grid);
        } else if (fnName == kDumpNeighboursName) {
            return reinterpret_cast<void *>(dump_neighbours);
        } else if (fnName == kDumpRegName) {
            return reinterpret_cast<void *>(dump_reg);
        }
        return nullptr;
    });
  ee->finalizeObject();

  sim_init();
  CPU::set_cpu(&cpu);

  ArrayRef<GenericValue> noargs;
  ee->runFunction(App, noargs);

  sim_fini();
}

} // namespace ASM2IR
