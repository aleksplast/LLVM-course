#pragma once

#include "ASMParser.hpp"
#include "CPU.hpp"
#include <llvm-18/llvm/IR/Module.h>
#include <llvm-18/llvm/IR/LLVMContext.h>
#include <string_view>
#include "llvm/IR/IRBuilder.h"

namespace ASM2IR {

using namespace llvm;

class IRGen {
private:
    static const constexpr std::string_view kModuleName = "top";

public:
    IRGen() : Context(), IRModule(std::make_unique<Module>(kModuleName, Context)), Builder(Context) {}

    virtual ~IRGen() = default;
    virtual void build_ir(const AsmParser &parser) = 0;
    virtual void exec(CPU &cpu) = 0;

protected:
    static const constexpr std::string_view kSimRandName = "sim_rand";
    static const constexpr std::string_view kScreenFlushName = "screen_flush";
    static const constexpr std::string_view kScreenPutPixelName = "screen_put_pixel";
    static const constexpr std::string_view kAppName = "app";

protected:
    Function *declareSimRand();
    Function *declareScreenFlush();
    Function *declareScreenPutPixel();

protected:
    LLVMContext Context;
    std::unique_ptr<Module> IRModule;
    IRBuilder<> Builder;
};

} // namespace ASM2IR
