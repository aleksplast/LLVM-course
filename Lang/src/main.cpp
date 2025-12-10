#include "visitors.hpp"
#include "llvm-18/llvm/IR/IRBuilder.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm-18/llvm/IR/IRBuilder.h"
#include "llvm-18/llvm/IR/LLVMContext.h"
#include "llvm-18/llvm/IR/Module.h"
#include "llvm-18/llvm/IR/Verifier.h"
#include <fstream>
#include <llvm-18/llvm/IR/BasicBlock.h>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <string>

using namespace llvm;

extern "C" {
#include "sim.h"
}

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    return 1;
  }
  std::ifstream stream;
  stream.open(argv[1]);

  antlr4::ANTLRInputStream input(stream);

  SilverLangLexer lexer(&input);

  antlr4::CommonTokenStream tokens(&lexer);

  SilverLangParser parser(&tokens);

  LLVMContext context;
  Module *module = new Module("top", context);
  IRBuilder<> builder(context);

  bool interpretMode = false;
  SilverLang::TreeLLVMWalker walker(&context, &builder, module, interpretMode);
  walker.visitProgramm(parser.programm());

  Function *appFunc = module->getFunction("app");
  if (appFunc == nullptr) {
    return -1;
  }

  LLVMInitializeNativeTarget();
  LLVMInitializeNativeAsmPrinter();

  ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
  ee->InstallLazyFunctionCreator([](const std::string &fnName) -> void * {
    if (fnName == "screen_put_pixel") {
    return reinterpret_cast<void *>(screen_put_pixel);
    }
    if (fnName == "screen_flush") {
      return reinterpret_cast<void *>(screen_flush);
    }
    if (fnName == "sim_rand") {
      return reinterpret_cast<void *>(sim_rand);
    }
    return nullptr;
  });
  ee->finalizeObject();

  sim_init();

  ArrayRef<GenericValue> noargs;
  GenericValue v = ee->runFunction(appFunc, noargs);

  sim_fini();

  return 0;
}
