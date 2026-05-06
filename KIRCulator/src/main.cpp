#include <CLI/CLI.hpp>
#include <filesystem>
#include <memory>

#include "BinParser.hpp"
#include "CPU.hpp"
#include "EmuIR.hpp"
#include "FullIR.hpp"
#include "IRGen.hpp"

int main(int argc, char **argv) {
    CLI::App app{"KIRCulator"};

    auto *input_group = app.add_option_group("input");

    std::filesystem::path bin_path;
    input_group->add_option("--b", bin_path, "RISC_VI_VII binary file path");
    input_group->require_option();

    std::filesystem::path ir_out_path;
    app.add_option("--o", ir_out_path, "IR out path");

    auto *mode_group = app.add_option_group("mode");
    bool ir_emulate = false;
    mode_group->add_flag("--ir-emulate", ir_emulate, "Should ir be emulated");

    bool ir_only = false;
    mode_group->add_flag("--ir-only", ir_only, "Should it be full functional ir");

    bool simulate = false;
    mode_group->add_flag("--simulate", simulate, "Should run on a RISC-VI-VII CPU model");

    mode_group->require_option();

    CLI11_PARSE(app, argc, argv);

    std::unique_ptr<KIRCulator::Parser> parser;
    parser = std::make_unique<KIRCulator::BinParser>();
    parser->parse(bin_path);
    std::unique_ptr<KIRCulator::IRGen> gen;

    if (ir_emulate) {
        gen = std::make_unique<KIRCulator::EmuIRGen>();
    } else if (ir_only) {
        gen = std::make_unique<KIRCulator::FullIRGen>();
    } else if (simulate) {
        KIRCulator::CPU cpu;
        cpu.exec(*parser);
        return 0;
    }

    KIRCulator::CPU cpu;
    gen->build_ir(*parser);
    gen->exec(cpu);
}
