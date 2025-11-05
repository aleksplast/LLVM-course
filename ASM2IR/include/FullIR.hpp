#pragma once

#include "IRGen.hpp"

namespace ASM2IR {

class FullIRGen final : public IRGen {
public:
    void build_ir(const AsmParser &parser) override;
    void exec(CPU &cpu) override;
private:
    std::vector<std::unique_ptr<GlobalVariable>> globals;
};

} // namespace ASM2IR
