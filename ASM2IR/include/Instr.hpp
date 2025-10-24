#pragma once

#include <inttypes.h>
#include <unordered_map>
#include <string>

namespace ASM2IR {

using Opcode = uint64_t;
using RegIndex = uint64_t;

struct Instr {
    enum IDs {
#define ISA(Opcode, Name, SkipArgs, ReadArgs, WriteArgs, Execute, IRGenExec)    \
        Name = Opcode,
#include "ISA.hpp"
#undef ISA
    };

    Opcode opcode;
    RegIndex rs1;
    RegIndex rd;
    RegIndex rs2imm;
};

struct InstrInfo {
    std::unordered_map<std::string, uint64_t> name2op;
    std::unordered_map<uint64_t, std::string> op2name;
    void construct_info();
    Opcode get_opcode(const std::string &instr_name) const {
        auto name_it = name2op.find(instr_name);
        if (name_it == name2op.end()) {
            return ~Opcode{0};
        }
        return name2op.find(instr_name)->second;
    }
};

} // namespace ASM2IR
