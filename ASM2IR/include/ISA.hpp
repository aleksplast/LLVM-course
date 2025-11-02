
// ISA format:
// Opcode
// Name
// SkipArgs: string -> 0 [ifstream input]
// ReadArgs: string -> args [ifstream input, Instr instr, map bb2pc]
// WriteArgs: args -> string [stringstream out, Instr instr, map PC2BBName]
// Execute: args -> EXECUTION [uint32_t rd, uint32_t rs1, uint32_t rs2imm, CPU *cpu]
// IRGenExecute = args -> IR [IRBuilder Builder, regFileType regFile, Instr instr,
//                            map BBMap, uint32_t PC, GraphicalFuncs]

// ISA(Opcode_, Name_, SkipArgs_, ReadArgs_, WriteArgs_, Execute_,
// IRGenExecute_)

// SkipArgs
#include <cstring>

#define SKIP_4ARGS                                                             \
  { input >> arg >> arg >> arg >> arg; }
#define SKIP_3ARGS                                                             \
  { input >> arg >> arg >> arg; }
#define SKIP_2ARGS                                                             \
  { input >> arg >> arg; }
#define SKIP_1ARGS                                                             \
  { input >> arg; }
#define SKIP_0ARGS {}

// ReadArgs
#define READ_REG(reg)                                                          \
  {                                                                            \
    input >> arg;                                                          \
    instr.reg = std::stoi(arg.substr(1));                                      \
  }
#define READ_IMM                                                               \
  {                                                                            \
    input >> arg;                                                          \
    instr.rs2imm = std::stoi(arg);                                             \
  }
#define READ_IMM3                                                              \
  {                                                                            \
    input >> arg;                                                          \
    instr.rs3imm = std::stoi(arg);                                             \
  }
#define READ_LABEL                                                             \
  {                                                                            \
    input >> arg;                                                               \
    std::cout << "In read label: arg = " << arg << '\n';                         \
    if (bb2pc.find(arg) == bb2pc.end()) {                                      \
      return true;                                                             \
    }                                                                          \
    std::cout << "rs2imm = " << bb2pc[arg] << '\n';                             \
    instr.rs2imm = bb2pc[arg];                                                  \
  }
#define READ_LABEL3                                                             \
  {                                                                            \
    input >> arg;                                                               \
    std::cout << "In read label: arg = " << arg << '\n';                         \
    if (bb2pc.find(arg) == bb2pc.end()) {                                      \
      return true;                                                             \
    }                                                                          \
    std::cout << "rs2imm = " << bb2pc[arg] << '\n';                             \
    instr.rs3imm = bb2pc[arg];                                                  \
  }


#define READ_4REGS                                                              \
  { READ_REG(rd) READ_REG(rs1) READ_REG(rs2imm) READ_REG(rs3imm) }
#define READ_3REGS                                                             \
  { READ_REG(rd) READ_REG(rs1) READ_REG(rs2imm) }
#define READ_3REGS_IMM                                                             \
  { READ_3REGS READ_IMM3 }
#define READ_2REGS_IMM                                                         \
  { READ_REG(rd) READ_REG(rs1) READ_IMM }
#define READ_2REGS                                                             \
  { READ_REG(rd) READ_REG(rs1) }
#define READ_1REG                                                              \
  { READ_REG(rd) }
#define READ_REG_LABEL                                                         \
  { READ_REG(rd) READ_LABEL }
#define READ_REG_2LABEL                                                         \
  { READ_REG_LABEL READ_LABEL3 }
#define READ_REG_IMM                                                           \
  { READ_REG(rd) READ_IMM }
#define READ_2REGS_IMM_REG                                                     \
  { READ_2REGS_IMM READ_REG(rs3imm)}
#define READ_0ARGS {}


// WriteArgs
#define WRITE_REG(reg)                                                         \
  { Stream << " x" << instr.reg; }
#define WRITE_IMM                                                              \
  { Stream << " " << instr.rs2imm; }
#define WRITE_IMM3                                                              \
  { Stream << " " << instr.rs3imm; }
#define WRITE_LABEL                                                            \
  { Stream << " " << pc2bb[instr.rs2imm]; }
#define WRITE_LABEL3                                                            \
  { Stream << " " << pc2bb[instr.rs3imm]; }
#define WRITE_4REGS                                                            \
  { WRITE_REG(rd) WRITE_REG(rs1) WRITE_REG(rs2imm) WRITE_REG(rs3imm)}
#define WRITE_3REGS                                                            \
  { WRITE_REG(rd) WRITE_REG(rs1) WRITE_REG(rs2imm) }
#define WRITE_3REGS_IMM                                                            \
  { WRITE_3REGS WRITE_IMM3 }
#define WRITE_2REGS_IMM                                                        \
  { WRITE_REG(rd) WRITE_REG(rs1) WRITE_IMM }
#define WRITE_2REGS                                                            \
  { WRITE_REG(rd) WRITE_REG(rs1) }
#define WRITE_1REG                                                             \
  { WRITE_REG(rd) }
#define WRITE_REG_LABEL                                                        \
  { WRITE_REG(rd) WRITE_LABEL }
#define WRITE_REG_2LABEL                                                        \
  { WRITE_REG_LABEL WRITE_LABEL3 }
#define WRITE_REG_IMM                                                          \
  { WRITE_REG(rd) WRITE_IMM }
#define WRITE_0ARGS {}

// IRGenExecute_
#define GEP2_32(arg) Builder.CreateConstGEP2_32(regFileType, regFile, 0, arg)
#define LOAD_REG(arg) Builder.CreateLoad(int32Type, GEP2_32(arg))
#define GEN_IMM(arg) Builder.getInt32(arg)

// ISA(Opcode_, Name_, SkipArgs_, ReadArgs_, WriteArgs_, Execute_,
// IRGenExecute_)

//    EXIT (NO ARGS)
ISA(0xBB, EXIT, {}, {}, {}, { cpu->run = false; }, { Builder.CreateRetVoid(); })

//    ADD x3 x2 x1 (3REGS)
ISA(
    0x30, ADD, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[rd] = cpu->reg_file[rs1] + cpu->reg_file[rs2imm]; },
    {
      builder.CreateStore(builder.CreateAdd(LOAD_REG(instr.rs1), LOAD_REG(instr.rs2imm)),
                          GEP2_32(instr.rd));
    })

//    MOVi r2 0 (REG_IMM)
ISA(
    0xAA, MOVi, SKIP_2ARGS, READ_REG_IMM, WRITE_REG_IMM,
    { cpu->reg_file[rd] = rs2imm; },
    { Builder.CreateStore(GEN_IMM(instr.rs2imm), GEP2_32(instr.rs1)); })

//    BR_COND x4 label_13 (REG_IMM)
ISA(
    0xBC, BR_COND, SKIP_3ARGS, READ_REG_2LABEL, WRITE_REG_2LABEL,
    {
      if (cpu->reg_file[rd]) {
        cpu->next_pc = rs2imm;
      } else {
        cpu->next_pc = rs3imm;
      }
    },
    {
      Builder.CreateCondBr(
          Builder.CreateTrunc(LOAD_REG(I.R1), builder.getInt1Ty()),
          BBMap[I.R3Imm], BBMap[PC]);
      Builder.SetInsertPoint(BBMap[PC]);
      continue;
    })

//    B label_13 (IMM)
ISA(
    0xB0, BRANCH, SKIP_1ARGS, READ_LABEL, WRITE_LABEL, { cpu->next_pc = rs2imm; },
    {
      Builder.CreateBr(BBMap[I.R3Imm]);
      Builder.SetInsertPoint(BBMap[PC]);
      continue;
    })

ISA(
    0x1, ALLOCA_NEIGHBOURS, SKIP_1ARGS, READ_1REG, WRITE_1REG,
    { cpu->reg_file[rd] = reinterpret_cast<long long>(new uint32_t[16]); },
    {
    })

ISA(
    0x2, ALLOCA_GRID, SKIP_1ARGS, READ_1REG, WRITE_1REG,
    { cpu->reg_file[rd] = reinterpret_cast<long long>(new uint32_t[1000 * 900]); },
    {
    })

ISA(
    0x3, LIFETIME_START, SKIP_1ARGS, READ_1REG, WRITE_1REG,
    {},
    {
    })

ISA(
    0x4, MEMSET, SKIP_1ARGS, READ_1REG, WRITE_1REG,
    { ::std::memset((void *) cpu->reg_file[rd], false, 3600000); },
    {
    })

ISA(
    0x5, NEIGHBOURS, SKIP_1ARGS, READ_1REG, WRITE_1REG,
    { cpu->reg_file[48] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[0]);
      cpu->reg_file[49] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[1]);
      cpu->reg_file[50] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[2]);
      cpu->reg_file[51] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[3]);
      cpu->reg_file[52] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[4]);
      cpu->reg_file[53] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[5]);
      cpu->reg_file[54] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[6]);
      cpu->reg_file[55] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[7]);
      cpu->reg_file[56] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[8]);
      cpu->reg_file[57] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[9]);
      cpu->reg_file[58] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[10]);
      cpu->reg_file[59] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[11]);
      cpu->reg_file[60] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[12]);
      cpu->reg_file[61] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[13]);
      cpu->reg_file[62] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[14]);
      cpu->reg_file[63] = reinterpret_cast<long long>(&((uint32_t*)cpu->reg_file[rd])[15]); },
    {
    })

ISA(
    0x6, INC_EQ, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { ++cpu->reg_file[rs1];
      cpu->reg_file[rd] = (cpu->reg_file[rs1] == rs2imm); },
    {
    })

ISA(
    0x7, SIM_RAND, SKIP_1ARGS, READ_1REG, WRITE_1REG,
    { cpu->reg_file[rd] = sim_rand(); },
    {
    })

ISA(
    0x8, SREMi, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[rd] = cpu->reg_file[rs1] % rs2imm; },
    {
    })

ISA(
    0x9, EQi, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[rd] = (cpu->reg_file[rs1] == rs2imm); },
    {
    })

ISA(
    0xa, SET_CELL, SKIP_4ARGS, READ_4REGS, WRITE_4REGS,
    { ((uint32_t *)cpu->reg_file[rd])[900 * cpu->reg_file[rs1] + cpu->reg_file[rs2imm]] = cpu->reg_file[rs3imm]; },
    {
    })

ISA(
    0xb, LIFETIME_END, SKIP_1ARGS, READ_1REG, WRITE_1REG,
    {},
    {
    })

ISA(
    0xc, MEMCPY, SKIP_2ARGS, READ_2REGS, WRITE_2REGS,
    { ::std::memcpy((void *)cpu->reg_file[rd], (void *)cpu->reg_file[rs1], 3600000); },
    {
    })

ISA(
    0xd, MOV, SKIP_2ARGS, READ_2REGS, WRITE_2REGS,
    { cpu->reg_file[rd] = cpu->reg_file[rs1]; },
    {
    })

ISA(
    0xe, ADDi, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[rd] = cpu->reg_file[rs1] + rs2imm; },
    {
    })

ISA(
    0xf, GET_CELL, SKIP_4ARGS, READ_4REGS, WRITE_4REGS,
    { cpu->reg_file[rs3imm] = ((uint32_t *)cpu->reg_file[rd])[900 * cpu->reg_file[rs1] + cpu->reg_file[rs2imm]]; },
    {
    })

ISA(
    0x10, STORE, SKIP_2ARGS, READ_2REGS, WRITE_2REGS,
    { *((uint32_t *)cpu->reg_file[rs1]) = cpu->reg_file[rd]; },
    {
    })

ISA(
    0x11, GET_NEIGHBOUR, SKIP_4ARGS, READ_2REGS_IMM_REG, WRITE_2REGS_IMM,
    { cpu->reg_file[rs3imm] = ((uint32_t *)cpu->reg_file[rd])[2 * cpu->reg_file[rs1] + rs2imm]; },
    {
    })

ISA(
    0x12, CMP_LT, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[rd] = (cpu->reg_file[rs1] < rs2imm); },
    {
    })

ISA(
    0x13, CMP_GT, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[rd] = (cpu->reg_file[rs1] > rs2imm); },
    {
    })

ISA(
    0x14, CMP_EQ, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[rd] = (cpu->reg_file[rs1] == rs2imm); },
    {
    })

ISA(
    0x15, CMP_NE, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[rd] = (cpu->reg_file[rs1] != rs2imm); },
    {
    })

ISA(
    0x16, SELECT, SKIP_4ARGS, READ_3REGS_IMM, WRITE_3REGS_IMM,
    { cpu->reg_file[rd] = cpu->reg_file[rs1] ? cpu->reg_file[rs2imm] : rs3imm; },
    {
    })

ISA(
    0x17, SCREEN_FLUSH, SKIP_0ARGS, READ_0ARGS, WRITE_0ARGS,
    { screen_flush(); },
    {
    })

ISA(
    0x18, GET_SWITCH_TBL_ELEM, SKIP_2ARGS, READ_2REGS, WRITE_2REGS,
    { if (cpu->reg_file[rs1] == 0) {
      cpu->reg_file[rd] = -16711936;
    } else if (cpu->reg_file[rs1] == 1) {
      cpu->reg_file[rd] = -16776961;
    } else {
      cpu->reg_file[rd] = -65536;
    }},
    {
    })

ISA(
    0x19, SCREEN_PUT_PIXEL, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { screen_put_pixel(cpu->reg_file[rd], cpu->reg_file[rs1], cpu->reg_file[rs2imm]); },
    {
    })

ISA(
    0x1a, ANDi, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[rd] = cpu->reg_file[rs1] & rs2imm; },
    {
    })





