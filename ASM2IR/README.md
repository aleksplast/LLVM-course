## ASM2IR

### Description

This directory contains implementation of ASM2IR tool, which includes:
1) ASM parser
2) Generation of emulating IR, where instructions of ASM is translated into calls of emulating functions, which executes operations on simple CPU model
3) Generation of full-IR, which is evolution of previous case, now each ASM instruction is translated into LLVM-IR instruction, working with some set of registers

## Usage

1. Build
```
mkdir build && cd build
cmake .. && make
```

2. Run ASM2IR on examples:
```
./asm2ir --a ../examples/simple_asm.s --ir-emulate
```

or

```
./asm2ir --a ../examples/simple_asm.s --ir-only
```

## ISA desciption

Here, `rd` is the destination register, where result will be stored, `rs#` is source register to be read, `imm` is some immediate value. `[reg]` refers to memory by address, held in `reg`

| Instruction      | Description |
| ---------------- | ----------- |
| ALLOCA_NEIGHBOURS  rd         | Allocate array 8x2 and store pointer in rd       |
| ALLOCA_GRID rd        | Allocate array 1000x900 and store pointer in rd        |
| LIFETIME_START rs        | Hint, translated into LLVM-IR lifetime_start equivalent   |
| MEMSET rs        | Set bytes [rs, rs + 3600000] to 0 |
| Neighbours rs        | Store pointers to 16 ints from [rs, rs + 16] into registers from x48 to x63 |
| INC_EQ rd rs imm        | rs += 1; rd = (rs == imm) |
| SIM_RAND rd        | Call external sim_rand() function, store result in rd |
| SREMi rd rs imm        | rd = rs % imm |
| SET_CELL rs1 rs2 rs3 rs4        | rs1[900 * rs2 + rs3] = rs4 |
| LIFETIME_END        | Hint, translated into LLVM-IR lifetime_end equivalent |
| MEMCPY rd rs        | Copy bytes from [rs, rs + 3600000] to [rd, rd + 3600000] |
| MOV rd rs        | rd = rs |
| ADDi rd rs imm        | rd = rs + imm |
| GET_CELL rs1 rs2 rs3 rd        | rd = rs1[900 * rs2 + rs3] |
| STORE rs1 rs2        | [rs2] = rs1 |
| GET_NEIGHBOUR rs1 rs2 rs3 rd        | rd = rs1[2 * rs2 + rs3] |
| CMP_LT rd rs imm        | rd = (rs < imm) |
| CMP_GT rd rs imm        | rd = (rs > imm) |
| CMP_EQ rd rs imm        | rd = (rs == imm) |
| CMP_NE rd rs imm        | rd = (rs != imm) |
| SELECT rd rs1 rs2 imm        | rd = rs1 ? rs2 : imm |
| SCREEN_FLUSH       | call external func screen_flush() |
| GET_SWITCH_TBL_ELEM rd rs       | if (rs1 == 0) {rd = -16711936;} else if (rs1 == 1) { rd = -16776961;} else { rd = -65536;} |
|  SCREEN_PUT_PIXEL rs1 rs2 rs3     | call external func screen_put_pixel(rs1, rs1, rs2) |
|  ANDi rd rs imm    | rd = rs & imm |
|  DUMP_GRID rs    | Debug command, dumps grid (1000x900 array) on [rs] into stdout |
|  DUMP_NEIGHBOURS rs    | Debug command, dumps neighbours (8x2 array) on [rs] into stdout |
|  DUMP_REG rs    | Debug command, dumps rs into stdout |
|  EXIT    | Create ret from func |
|  MOVi rd imm    | rd = imm |
|  BR_COND rs label_1 label_2   | if rs != 0 branch on label_1, branch on label_2 otherwise |
|  BRANCH label   | branch on label |
