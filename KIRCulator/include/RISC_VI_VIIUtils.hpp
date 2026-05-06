#pragma once

#include <cstdint>
#include <string>

namespace RISC_VI_VII {

using RawInstruction = uint64_t;
using Address = uint32_t;
const uint64_t INSTR_SIZE = sizeof(RawInstruction);

inline const char* const REG_NAMES[] = {
    "TRALALERO_TRALALA",        // 0
    "BOMBARDIRO_CROCODILO",     // 1
    "BRR_BRR_PATAPIM",          // 2  (ra)
    "BALLERINA_CAPPUCCINA",     // 3
    "CAPPUCINO_ASSASINO",       // 4
    "BOMBOMBINI_GUSINI",        // 5
    "LIRILI_LARILA",            // 6
    "FRIGO_CAMELO",             // 7
    "BONECA_AMBALABU",          // 8
    "TUNG_TUNG_TUNG_SAHUR",     // 9  (sp)
    "TRIPPI_TROPPI",            // 10
    "TROPPA_TRIPPA",            // 11
    "CHIMPANZINI_BANANINI",     // 12
    "BOBRITO_BANDITO",          // 13
    "LAVACA_SATURNA_SATURNITA", // 14
    "TRULIMERA_TRULICINA",      // 15
};
const int NUM_REGS = 16;

inline int reg_name_to_idx(const std::string &name) {
    for (int i = 0; i < NUM_REGS; ++i)
        if (REG_NAMES[i] == name) return i;
    return -1;
}

} // namespace RISC_VI_VII
