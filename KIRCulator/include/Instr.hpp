#pragma once

#include <inttypes.h>
#include <unordered_map>
#include <string>
#include "RISC_VI_VIIUtils.hpp"

namespace KIRCulator {

using Opcode = uint64_t;

struct Instr {
    enum IDs {
#define ISA(Opcode_, Name_, SkipArgs, ReadArgs, WriteArgs, Execute, IRGenExec)    \
        Name_ = Opcode_,
#include "ISA.hpp"
#undef ISA
    };

    Opcode opcode;
    long long r1;
    long long r2;
    long long r3imm;
    std::string call_target;
};

struct InstrInfo {
    InstrInfo();
    std::unordered_map<std::string, uint64_t> name2op;
    std::unordered_map<uint64_t, std::string> op2name;

    Opcode get_opcode(const std::string &instr_name) const {
        auto it = name2op.find(instr_name);
        return (it == name2op.end()) ? ~Opcode{0} : it->second;
    }
};

} // namespace KIRCulator
