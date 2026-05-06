
// RISC_VI_VII ISA
//
// Instruction format (64-bit, little-endian):
//   [63:56] opcode  [55:52] r1  [51:48] r2  [47:32] padding  [31:0] r3/imm
//
// ISA(Opcode_, Name_, SkipArgs_, ReadArgs_, WriteArgs_, Execute_, IRGenExecute_)


#define SKIP_3ARGS { input >> arg >> arg >> arg; }
#define SKIP_2ARGS { input >> arg >> arg; }
#define SKIP_1ARGS { input >> arg; }
#define SKIP_0ARGS { ; }

#define READ_REG(reg)                                                          \
  { input >> arg; instr.reg = RISC_VI_VII::reg_name_to_idx(arg); }
#define READ_IMM                                                               \
  { input >> arg; instr.r3imm = std::stoll(arg); }
#define READ_LABEL                                                             \
  { input >> arg;                                                              \
    if (bb2pc.find(arg) == bb2pc.end()) return true;                               \
    instr.r3imm = bb2pc[arg]; }

#define READ_3REGS { READ_REG(r1) READ_REG(r2) READ_REG(r3imm) }
#define READ_2REGS { READ_REG(r1) READ_REG(r2) }
#define READ_2REGS_IMM { READ_REG(r1) READ_REG(r2) READ_IMM }
#define READ_1REG { READ_REG(r1) }
#define READ_1REG_IMM { READ_REG(r1) READ_IMM }
#define READ_REG_LABEL { READ_REG(r1) READ_LABEL }
#define READ_0ARGS {}

#define WRITE_REG(reg) \
  { Stream << " " << RISC_VI_VII::REG_NAMES[instr.reg]; }
#define WRITE_REG3 \
  { Stream << " " << RISC_VI_VII::REG_NAMES[(int)instr.r3imm]; }
#define WRITE_IMM \
  { Stream << " " << instr.r3imm; }
#define WRITE_LABEL \
  { Stream << " " << pc2bb[instr.r3imm]; }

#define WRITE_3REGS { WRITE_REG(r1) WRITE_REG(r2) WRITE_REG3 }
#define WRITE_2REGS { WRITE_REG(r1) WRITE_REG(r2) }
#define WRITE_2REGS_IMM { WRITE_REG(r1) WRITE_REG(r2) WRITE_IMM }
#define WRITE_1REG { WRITE_REG(r1) }
#define WRITE_1REG_IMM { WRITE_REG(r1) WRITE_IMM }
#define WRITE_REG_LABEL { WRITE_REG(r1) WRITE_LABEL }
#define WRITE_0ARGS {}

#define LOAD_REG(idx) Builder.CreateLoad(int64Type, Builder.CreateConstGEP2_32(regFileType, regFile, 0, (uint32_t)(idx)))
#define STORE_REG(idx, ...) Builder.CreateStore((__VA_ARGS__), Builder.CreateConstGEP2_32(regFileType, regFile, 0, (uint32_t)(idx)))
#define BYTE_PTR(base, off) Builder.CreateGEP(int8Type, stackGlobal, Builder.CreateAdd((base), Builder.getInt64(off)))

ISA(0x10, LW, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[r1] = *(int32_t *)((uint8_t *)cpu->stack.get() + cpu->reg_file[r2] + r3imm); },
    { Value *ld = Builder.CreateLoad(int32Type, BYTE_PTR(LOAD_REG(r2), imm));
      STORE_REG(r1, Builder.CreateSExt(ld, int64Type)); })

ISA(0x11, SW, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { *(int32_t *)((uint8_t *)cpu->stack.get() + cpu->reg_file[r2] + r3imm) = cpu->reg_file[r1]; },
    { Builder.CreateStore(Builder.CreateTrunc(LOAD_REG(r1), int32Type), BYTE_PTR(LOAD_REG(r2), imm)); })


ISA(0x30, CMP_EQ, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = (cpu->reg_file[r2] == cpu->reg_file[r3imm]); },
    { STORE_REG(r1, Builder.CreateZExt(Builder.CreateICmpEQ(LOAD_REG(r2), LOAD_REG(r3)), int64Type)); })

ISA(0x31, CMP_NE, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = (cpu->reg_file[r2] != cpu->reg_file[r3imm]); },
    { STORE_REG(r1, Builder.CreateZExt(Builder.CreateICmpNE(LOAD_REG(r2), LOAD_REG(r3)), int64Type)); })

ISA(0x32, CMP_LT, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = (cpu->reg_file[r2] < cpu->reg_file[r3imm]); },
    { STORE_REG(r1, Builder.CreateZExt(Builder.CreateICmpSLT(LOAD_REG(r2), LOAD_REG(r3)), int64Type)); })

ISA(0x33, CMP_GT, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = (cpu->reg_file[r2] > cpu->reg_file[r3imm]); },
    { STORE_REG(r1, Builder.CreateZExt(Builder.CreateICmpSGT(LOAD_REG(r2), LOAD_REG(r3)), int64Type)); })

ISA(0x34, CMP_LE, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = (cpu->reg_file[r2] <= cpu->reg_file[r3imm]); },
    { STORE_REG(r1, Builder.CreateZExt(Builder.CreateICmpSLE(LOAD_REG(r2), LOAD_REG(r3)), int64Type)); })

ISA(0x35, CMP_GE, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = (cpu->reg_file[r2] >= cpu->reg_file[r3imm]); },
    { STORE_REG(r1, Builder.CreateZExt(Builder.CreateICmpSGE(LOAD_REG(r2), LOAD_REG(r3)), int64Type)); })

ISA(0x36, CMP_ULT, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = ((uint32_t)cpu->reg_file[r2] < (uint32_t)cpu->reg_file[r3imm]); },
    { STORE_REG(r1, Builder.CreateZExt(
        Builder.CreateICmpULT(Builder.CreateTrunc(LOAD_REG(r2), int32Type),
                              Builder.CreateTrunc(LOAD_REG(r3), int32Type)), int64Type)); })

ISA(0x37, CMP_UGT, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = ((uint32_t)cpu->reg_file[r2] > (uint32_t)cpu->reg_file[r3imm]); },
    { STORE_REG(r1, Builder.CreateZExt(
        Builder.CreateICmpUGT(Builder.CreateTrunc(LOAD_REG(r2), int32Type),
                              Builder.CreateTrunc(LOAD_REG(r3), int32Type)), int64Type)); })

ISA(0x38, CMP_ULE, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = ((uint32_t)cpu->reg_file[r2] <= (uint32_t)cpu->reg_file[r3imm]); },
    { STORE_REG(r1, Builder.CreateZExt(
        Builder.CreateICmpULE(Builder.CreateTrunc(LOAD_REG(r2), int32Type),
                              Builder.CreateTrunc(LOAD_REG(r3), int32Type)), int64Type)); })

ISA(0x39, CMP_UGE, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = ((uint32_t)cpu->reg_file[r2] >= (uint32_t)cpu->reg_file[r3imm]); },
    { STORE_REG(r1, Builder.CreateZExt(
        Builder.CreateICmpUGE(Builder.CreateTrunc(LOAD_REG(r2), int32Type),
                              Builder.CreateTrunc(LOAD_REG(r3), int32Type)), int64Type)); })

ISA(0x40, ADD,  SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = cpu->reg_file[r2] + cpu->reg_file[r3imm]; },
    { STORE_REG(r1, Builder.CreateAdd(LOAD_REG(r2), LOAD_REG(r3))); })

ISA(0x41, SUB,  SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = cpu->reg_file[r2] - cpu->reg_file[r3imm]; },
    { STORE_REG(r1, Builder.CreateSub(LOAD_REG(r2), LOAD_REG(r3))); })

ISA(0x42, DIV,  SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = cpu->reg_file[r2] / cpu->reg_file[r3imm]; },
    { STORE_REG(r1, Builder.CreateSDiv(LOAD_REG(r2), LOAD_REG(r3))); })

ISA(0x43, UDIV, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = (uint64_t)cpu->reg_file[r2] / (uint64_t)cpu->reg_file[r3imm]; },
    { STORE_REG(r1, Builder.CreateUDiv(LOAD_REG(r2), LOAD_REG(r3))); })

ISA(0x44, MUL,  SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = cpu->reg_file[r2] * cpu->reg_file[r3imm]; },
    { STORE_REG(r1, Builder.CreateMul(LOAD_REG(r2), LOAD_REG(r3))); })

ISA(0x45, AND,  SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = cpu->reg_file[r2] & cpu->reg_file[r3imm]; },
    { STORE_REG(r1, Builder.CreateAnd(LOAD_REG(r2), LOAD_REG(r3))); })

ISA(0x46, OR,   SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = cpu->reg_file[r2] | cpu->reg_file[r3imm]; },
    { STORE_REG(r1, Builder.CreateOr(LOAD_REG(r2), LOAD_REG(r3))); })

ISA(0x47, XOR,  SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = cpu->reg_file[r2] ^ cpu->reg_file[r3imm]; },
    { STORE_REG(r1, Builder.CreateXor(LOAD_REG(r2), LOAD_REG(r3))); })

ISA(0x48, SREM, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = cpu->reg_file[r2] % cpu->reg_file[r3imm]; },
    { STORE_REG(r1, Builder.CreateSRem(LOAD_REG(r2), LOAD_REG(r3))); })

ISA(0x49, UREM, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = (uint32_t)cpu->reg_file[r2] % (uint32_t)cpu->reg_file[r3imm]; },
    { STORE_REG(r1, Builder.CreateZExt(
        Builder.CreateURem(Builder.CreateTrunc(LOAD_REG(r2), int32Type),
                           Builder.CreateTrunc(LOAD_REG(r3), int32Type)), int64Type)); })

ISA(0x4a, SHL,  SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = cpu->reg_file[r2] << (cpu->reg_file[r3imm] & 63); },
    { STORE_REG(r1, Builder.CreateShl(LOAD_REG(r2),
        Builder.CreateAnd(LOAD_REG(r3), Builder.getInt64(63)))); })

ISA(0x4b, SHA,  SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { cpu->reg_file[r1] = (long long)cpu->reg_file[r2] >> (cpu->reg_file[r3imm] & 63); },
    { STORE_REG(r1, Builder.CreateAShr(LOAD_REG(r2),
        Builder.CreateAnd(LOAD_REG(r3), Builder.getInt64(63)))); })

ISA(0x50, ADDi,  SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[r1] = cpu->reg_file[r2] + r3imm; },
    { STORE_REG(r1, Builder.CreateAdd(LOAD_REG(r2), Builder.getInt64(imm))); })

ISA(0x51, SUBi,  SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[r1] = cpu->reg_file[r2] - r3imm; },
    { STORE_REG(r1, Builder.CreateSub(LOAD_REG(r2), Builder.getInt64(imm))); })

ISA(0x52, DIVi,  SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[r1] = cpu->reg_file[r2] / r3imm; },
    { STORE_REG(r1, Builder.CreateSDiv(LOAD_REG(r2), Builder.getInt64(imm))); })

ISA(0x53, UDIVi, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[r1] = (uint64_t)cpu->reg_file[r2] / (uint64_t)r3imm; },
    { STORE_REG(r1, Builder.CreateUDiv(LOAD_REG(r2), Builder.getInt64(imm))); })

ISA(0x54, MULi,  SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[r1] = cpu->reg_file[r2] * r3imm; },
    { STORE_REG(r1, Builder.CreateMul(LOAD_REG(r2), Builder.getInt64(imm))); })

ISA(0x55, ANDi,  SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[r1] = cpu->reg_file[r2] & r3imm; },
    { STORE_REG(r1, Builder.CreateAnd(LOAD_REG(r2), Builder.getInt64(imm))); })

ISA(0x56, ORi,   SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[r1] = cpu->reg_file[r2] | r3imm; },
    { STORE_REG(r1, Builder.CreateOr(LOAD_REG(r2), Builder.getInt64(imm))); })

ISA(0x57, XORi,  SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[r1] = cpu->reg_file[r2] ^ r3imm; },
    { STORE_REG(r1, Builder.CreateXor(LOAD_REG(r2), Builder.getInt64(imm))); })

ISA(0x58, SREMi, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[r1] = cpu->reg_file[r2] % r3imm; },
    { STORE_REG(r1, Builder.CreateSRem(LOAD_REG(r2), Builder.getInt64(imm))); })

ISA(0x59, UREMi, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[r1] = cpu->reg_file[r2] % r3imm; },
    { STORE_REG(r1, Builder.CreateURem(LOAD_REG(r2), Builder.getInt64(imm))); })

ISA(0x5a, SHLi,  SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[r1] = cpu->reg_file[r2] << (r3imm & 63); },
    { STORE_REG(r1, Builder.CreateShl(LOAD_REG(r2),
        Builder.getInt64((int64_t)(imm & 63)))); })

ISA(0x5b, SHAi,  SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { cpu->reg_file[r1] = (long long)cpu->reg_file[r2] >> (r3imm & 63); },
    { STORE_REG(r1, Builder.CreateAShr(LOAD_REG(r2),
        Builder.getInt64((int64_t)(imm & 63)))); })

ISA(0x66, MOV, SKIP_2ARGS, READ_2REGS, WRITE_2REGS,
    { cpu->reg_file[r1] = cpu->reg_file[r2]; },
    { STORE_REG(r1, LOAD_REG(r2)); })

ISA(0x67, MOVi, SKIP_2ARGS, READ_1REG_IMM, WRITE_1REG_IMM,
    { cpu->reg_file[r1] = r3imm; },
    { STORE_REG(r1, Builder.getInt64(imm)); })

ISA(0x90, INC_EQ, SKIP_3ARGS, READ_2REGS_IMM, WRITE_2REGS_IMM,
    { ++cpu->reg_file[r2]; cpu->reg_file[r1] = (cpu->reg_file[r2] == r3imm); },
    { Value *incr = Builder.CreateAdd(LOAD_REG(r2), Builder.getInt64(1));
      STORE_REG(r2, incr);
      STORE_REG(r1, Builder.CreateZExt(Builder.CreateICmpEQ(incr, Builder.getInt64(imm)), int64Type)); })

ISA(0x91, SELECT, SKIP_0ARGS, READ_0ARGS, WRITE_0ARGS,
    {},
    {})

ISA(0xB0, B, SKIP_1ARGS, READ_LABEL, WRITE_LABEL,
    { cpu->next_pc = r3imm; },
    { Builder.CreateBr(BBMap.at((uint32_t)instr.r3imm)); })

ISA(0xB1, CALL, SKIP_1ARGS, READ_LABEL, WRITE_LABEL,
    { cpu->reg_file[2] = cpu->next_pc; cpu->next_pc = 0; },
    { if (!instr.call_target.empty()) {
        FunctionCallee callee = IRModule->getOrInsertFunction(instr.call_target, voidFT);
        Builder.CreateCall(callee);
      } })

ISA(0xB3, BR_COND, SKIP_2ARGS, READ_REG_LABEL, WRITE_REG_LABEL,
    { if (cpu->reg_file[r1]) { cpu->next_pc = r3imm; } },
    { Value *cond = Builder.CreateTrunc(LOAD_REG(r1), Builder.getInt1Ty());
      Builder.CreateCondBr(cond, BBMap.at((uint32_t)instr.r3imm), nextBB); })

ISA(0xB4, BR, SKIP_1ARGS, READ_1REG, WRITE_1REG,
    { cpu->next_pc = cpu->reg_file[2]; },
    { Builder.CreateRetVoid(); })

ISA(0x1e, EXIT, SKIP_0ARGS, READ_0ARGS, WRITE_0ARGS,
    { cpu->run = false; },
    { Builder.CreateRetVoid(); })

ISA(0xE0, MEMSET, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { ::std::memset((void *)(cpu->stack.get() + cpu->reg_file[r1]), (int)cpu->reg_file[r2], (size_t)cpu->reg_file[r3imm]); },
    { Builder.CreateMemSet(BYTE_PTR(LOAD_REG(r1), 0),
        Builder.CreateTrunc(LOAD_REG(r2), Builder.getInt8Ty()),
        LOAD_REG(r3), MaybeAlign()); })

ISA(0xE1, MEMCPY, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { ::std::memcpy((void *)(cpu->stack.get() + cpu->reg_file[r1]), (const void *)cpu->reg_file[r2], (size_t)cpu->reg_file[r3imm]); },
    { Builder.CreateMemCpy(BYTE_PTR(LOAD_REG(r1), 0), MaybeAlign(),
        BYTE_PTR(LOAD_REG(r2), 0), MaybeAlign(),
        LOAD_REG(r3)); })

ISA(0xE2, SCREEN_FLUSH, SKIP_0ARGS, READ_0ARGS, WRITE_0ARGS,
    { screen_flush(); },
    { Builder.CreateCall(ext.ScreenFlush); })

ISA(0xE3, SCREEN_PUT_PIXEL, SKIP_3ARGS, READ_3REGS, WRITE_3REGS,
    { screen_put_pixel((int)cpu->reg_file[r1], (int)cpu->reg_file[r2], (int)cpu->reg_file[r3imm]); },
    { Value *spp_x = Builder.CreateTrunc(LOAD_REG(r1), int32Type);
      Value *spp_y = Builder.CreateTrunc(LOAD_REG(r2), int32Type);
      Value *spp_c = Builder.CreateTrunc(LOAD_REG(r3), int32Type);
      Builder.CreateCall(ext.ScreenPutPixel, ArrayRef<Value*>({spp_x, spp_y, spp_c})); })

ISA(0xE4, SIM_RAND, SKIP_1ARGS, READ_1REG, WRITE_1REG,
    { cpu->reg_file[r1] = sim_rand(); },
    { STORE_REG(r1, Builder.CreateSExt(Builder.CreateCall(ext.SimRand), int64Type)); })
