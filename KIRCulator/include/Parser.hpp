#pragma once

#include "Instr.hpp"
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>

namespace KIRCulator {

struct FuncInfo {
    std::string name;
    std::vector<Instr> insns;
    std::vector<std::string> bb_names;
    std::unordered_map<std::string, uint64_t> bb2pc;
    std::unordered_map<uint64_t, std::string> pc2bb;
};

struct Parser {
    virtual ~Parser() = default;
    virtual void parse(const std::filesystem::path &path) = 0;

    InstrInfo instr_info;
    std::vector<FuncInfo> funcs; // [0] is the program entry point
};

} // namespace KIRCulator
