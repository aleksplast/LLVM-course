#include <CLI/CLI.hpp>
#include <filesystem>
#include <memory>

#include "ASMParser.hpp"
#include "CPU.hpp"
#include "EmuIR.hpp"
#include "FullIR.hpp"
#include "IRGen.hpp"

int main(int argc, char **argv) {
    CLI::App app{"ASM2IR"};

    std::filesystem::path asm_path;
    app.add_option("--a", asm_path, "Path to asm file")->required();

    std::filesystem::path ir_out_path;
    app.add_option("--o", ir_out_path, "IR out path");

    bool ir_emulate = false;
    app.add_flag("--ir-emulate", ir_emulate, "Should ir be emulated");

    bool ir_only = false;
    app.add_flag("--ir-only", ir_only, "Should it be full functional ir");

    CLI11_PARSE(app, argc, argv);

    ASM2IR::AsmParser parser(asm_path);
    std::unique_ptr<ASM2IR::IRGen> gen;

    if (ir_emulate) {
        gen = std::make_unique<ASM2IR::EmuIRGen>();
    } else {
        gen = std::make_unique<ASM2IR::FullIRGen>();
    }

    ASM2IR::CPU cpu;
    gen->build_ir(parser);
    gen->exec(cpu);
}
