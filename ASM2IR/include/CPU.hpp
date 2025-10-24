#pragma once

#include <cinttypes>

namespace ASM2IR {

struct CPU {
  static constexpr uint32_t kRegNum = 16;
  uint32_t reg_file[kRegNum] = {};
  uint32_t pc;
  uint32_t next_pc;
  bool run = true;

  static CPU *cpu;
  static void set_cpu(CPU *c) { cpu = c; }
#define ISA(Opcode_, Name_, SkipArgs_, ReadArgs_, WriteArgs_, Execute_,        \
             IRGenExecute_)                                                    \
static void do_##Name_(uint32_t rd, uint32_t rs1, uint32_t rs2imm) {           \
    Execute_;                                                                  \
}
#include "ISA.hpp"
#undef ISA
};

} // namespace ASM2IR
