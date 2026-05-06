#include "Instr.hpp"

namespace KIRCulator {

InstrInfo::InstrInfo() {
#define ISA(Opcode, Name, SkipArgs, ReadArgs, WriteArgs, Execute,       \
             IRGenExecute)                                              \
  name2op[#Name] = Opcode;                                              \
  op2name[Opcode] = #Name;
#include "ISA.hpp"
#undef ISA
}

} // namespace KIRCulator
