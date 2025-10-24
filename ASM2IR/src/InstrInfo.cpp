#include "Instr.hpp"

namespace ASM2IR {

void InstrInfo::construct_info() {
#define ISA(Opcode, Name, SkipArgs, ReadArgs, WriteArgs, Execute,       \
             IRGenExecute)                                              \
  name2op[#Name] = Opcode;                                              \
  op2name[Opcode] = #Name;
#include "ISA.hpp"
#undef ISA
}

} // namespace ASM2IR
