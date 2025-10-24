
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
#define SKIP_3ARGS                                                             \
  { input >> arg >> arg >> arg; }
#define SKIP_2ARGS                                                             \
  { input >> arg >> arg; }
#define SKIP_1ARGS                                                             \
  { input >> arg; }

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
#define READ_3REGS                                                             \
  { READ_REG(rd) READ_REG(rs1) READ_REG(rs2imm) }
#define READ_2REGS_IMM                                                         \
  { READ_REG(rd) READ_REG(rs1) READ_IMM }
#define READ_2REGS                                                             \
  { READ_REG(rd) READ_REG(rs1) }
#define READ_REG_LABEL                                                         \
  { READ_REG(rd) READ_LABEL }
#define READ_REG_IMM                                                           \
  { READ_REG(rd) READ_IMM }

// WriteArgs
#define WRITE_REG(reg)                                                         \
  { Stream << " x" << instr.reg; }
#define WRITE_IMM                                                              \
  { Stream << " " << instr.rs2imm; }
#define WRITE_LABEL                                                            \
  { Stream << " " << pc2bb[instr.rs2imm]; }
#define WRITE_3REGS                                                            \
  { WRITE_REG(rd) WRITE_REG(rs1) WRITE_REG(rs2imm) }
#define WRITE_2REGS_IMM                                                        \
  { WRITE_REG(rd) WRITE_REG(rs1) WRITE_IMM }
#define WRITE_2REGS                                                            \
  { WRITE_REG(rd) WRITE_REG(rs1) }
#define WRITE_REG_LABEL                                                        \
  { WRITE_REG(rd) WRITE_LABEL }
#define WRITE_REG_IMM                                                          \
  { WRITE_REG(rd) WRITE_IMM }

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
    0xBC, BR_COND, SKIP_2ARGS, READ_REG_LABEL, WRITE_REG_LABEL,
    {
      if (cpu->reg_file[rd]) {
        cpu->next_pc = rs2imm;
      }
    },
    {
      PC++;
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
      PC++;
      Builder.CreateBr(BBMap[I.R3Imm]);
      Builder.SetInsertPoint(BBMap[PC]);
      continue;
    })
