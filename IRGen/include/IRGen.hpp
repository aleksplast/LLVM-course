#pragma once

#include "llvm-18/llvm/IR/Module.h"
#include "llvm-18/llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include <filesystem>
#include <llvm-18/llvm/IR/GlobalVariable.h>
#include <memory>
#include <string_view>
#include <vector>

using namespace llvm;

namespace IRGen {

class Generator final {
public:
    void generate();
    void print(const std::optional<std::filesystem::path> &OutPath) const;
    void interpret();
    Generator();

private:
    static const constexpr std::string_view kSimRandName = "sim_rand";
    static const constexpr std::string_view kScreenFlushName = "screen_flush";
    static const constexpr std::string_view kScreenPutPixelName = "screen_put_pixel";
    static const constexpr std::string_view kAppName = "app";

private:
    Function *declareSimRand();
    Function *declareScreenFlush();
    Function *declareScreenPutPixel();

private:
    LLVMContext Context;
    std::unique_ptr<Module> IRModule;
    IRBuilder<> Builder;

    std::vector<std::unique_ptr<GlobalVariable>> globals;
};

} // namespace IRGen
