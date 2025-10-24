#pragma once

#include "IRGen.hpp"
#include "CPU.hpp"

namespace ASM2IR {

class EmuIRGen final : public IRGen {
public:
    void build_ir(const AsmParser &parser) override;
    void exec(CPU &cpu) override;
};

} // namespace ASM2IR
