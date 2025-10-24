#pragma once

#include "Instr.hpp"
#include <filesystem>
#include <unordered_map>
#include <vector>

namespace ASM2IR {

struct AsmParser final {
public:
    AsmParser(const std::filesystem::path &asm_path);

    bool find_bbs(std::ifstream &input);
    bool read_insns(std::ifstream &input);

    InstrInfo instr_info;

    std::vector<Instr> insns;
    std::vector<std::string> bb_names;
    std::unordered_map<std::string, uint64_t> bb2pc;
    std::unordered_map<uint64_t, std::string> pc2bb;
};

} // namespace ASM2IR
