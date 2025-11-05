#include "ASMParser.hpp"
#include "Instr.hpp"
#include <fstream>
#include <iostream>

namespace ASM2IR {

AsmParser::AsmParser(const std::filesystem::path &asm_path) {
    std::ifstream input{asm_path};

    if (!input.is_open()) {
        return;
    }

    instr_info.construct_info();

    find_bbs(input);

    input.clear();
    input.seekg(0, std::ios::beg);
    read_insns(input);
}

bool AsmParser::find_bbs(std::ifstream &input) {
    std::string name;
    std::string arg;
    uint64_t pc = 0;

    while (input >> name) {
        Opcode op = instr_info.get_opcode(name);

        switch (op) {
        default:
            if (bb2pc.find(name) != bb2pc.end()) {
                return false;
            }
            if (pc2bb.find(pc) != pc2bb.end()) {
                return false;
            }
            bb2pc[name] = pc;
            bb_names.emplace_back(name);
            pc2bb[pc] = name;
            continue;
#define ISA(Opcode, Name, SkipArgs, ReadArgs, WriteArgs, Execute,       \
             IRGenExecute)                                              \
            case (Opcode):                                              \
            SkipArgs;                                                   \
            break;
#include "ISA.hpp"
#undef ISA
        }
        ++pc;
    }

    return true;
}

bool AsmParser::read_insns(std::ifstream &input) {
    std::string name;
    std::string arg;

    while (input >> name) {
        Opcode op = instr_info.get_opcode(name);
        Instr instr;
        instr.opcode = op;

        switch (op) {
        default:
            if (bb2pc.find(name) != bb2pc.end()) {
                continue;
            }
            return false;
#define ISA(Opcode, Name, SkipArgs, ReadArgs, WriteArgs, Execute,       \
             IRGenExecute)                                              \
            case (Opcode):                                              \
            ReadArgs;                                                   \
            break;
#include "ISA.hpp"
#undef ISA
        }
        insns.push_back(instr);
    }

    return true;
}

} // namespace ASM2IR
