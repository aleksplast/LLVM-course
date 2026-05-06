#pragma once

#include "IRGen.hpp"

namespace KIRCulator {

class FullIRGen final : public IRGen {
public:
    void build_ir(const Parser &parser) override;
    void exec(CPU &cpu) override;
private:
    std::vector<std::unique_ptr<GlobalVariable>> globals;
};

} // namespace KIRCulator
