#pragma once

#include "IRGen.hpp"
#include "CPU.hpp"
#include <llvm-14/llvm/IR/GlobalVariable.h>

namespace ASM2IR {

class EmuIRGen final : public IRGen {
public:
    void build_ir(const AsmParser &parser) override;
    void exec(CPU &cpu) override;

public:
    GlobalVariable *regFile;
};

} // namespace ASM2IR
