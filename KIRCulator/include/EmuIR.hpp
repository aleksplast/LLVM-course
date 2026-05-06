#pragma once

#include "IRGen.hpp"
#include "CPU.hpp"
#include <llvm-18/llvm/IR/GlobalVariable.h>

namespace KIRCulator {

class EmuIRGen final : public IRGen {
public:
    void build_ir(const Parser &parser) override;
    void exec(CPU &cpu) override;

public:
    GlobalVariable *regFile;
};

} // namespace KIRCulator
