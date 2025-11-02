init
    ALLOCA_NEIGHBOURS x0
    ALLOCA_GRID x1
    ALLOCA_GRID x2
    LIFETIME_START x1
    MEMSET x1
    MOVi x3 0
    BRANCH label_7
label_7
    MOVi x5 0
    BRANCH label_31
label_9
    MOVi x8 0
    NEIGHBOURS x0
    BRANCH label_41
label_28
    INC_EQ x20 x3 1000
    BR_COND x20 label_9 label_7
label_31
    SIM_RAND x4
    SREMi x4 x4 3
    EQi x4 x4 0
    SET_CELL x1 x3 x5 x4
    INC_EQ x7 x5 900
    BR_COND x7 label_28 label_31
label_40
    LIFETIME_END x1
    EXIT
label_41
    MOVi x30 0
    LIFETIME_START x2
    MEMCPY x2 x1
    BRANCH label_43
label_43
    MOV x9 x30
    ADDi x10 x9 -1
    ADDi x11 x9 1
    MOVi x13 0
    BRANCH label_58
label_55
    EQi x12 x11 1000
    MOV x30 x11
    BR_COND x12 label_57 label_43
label_57
    MEMCPY x1 x2
    MOVi x25 0
    BRANCH label_118
label_58
    GET_CELL x1 x9 x13 x12
    LIFETIME_START x0
    STORE x10 x48
    ADDi x14 x13 -1
    STORE x14 x49
    STORE x10 x50
    STORE x13 x51
    STORE x10 x52
    ADDi x15 x13 1
    STORE x15 x53
    STORE x9 x54
    STORE x14 x55
    STORE x9 x56
    STORE x15 x57
    STORE x11 x58
    STORE x14 x59
    STORE x11 x60
    STORE x13 x61
    STORE x11 x62
    STORE x15 x63
    MOVi x16 0
    MOVi x17 0
    BRANCH label_72
label_72
    GET_NEIGHBOUR x0 x16 0 x18
    GET_NEIGHBOUR x0 x16 1 x19
    CMP_LT x20 x18 1000
    CMP_GT x21 x19 -1
    SELECT x22 x20 x21 0
    CMP_LT x23 x19 900
    SELECT x24 x22 x23 0
    BR_COND x24 label_84 label_92
label_84
    GET_CELL x1 x18 x19 x20
    CMP_NE x20 x20 0
    ADD x17 x17 x20
    BRANCH label_92
label_92
    INC_EQ x20 x16 8
    BR_COND x20 label_96 label_72
label_96
    LIFETIME_END x1
    CMP_NE x20 x12 0
    CMP_LT x21 x17 2
    SELECT x22 x20 x21 0
    MOVi x24 0
    BR_COND x22 label_113 label_100
label_100
    ANDi x21 x17 -2
    CMP_EQ x21 x21 2
    SELECT x21 x20 x21 0
    BR_COND x21 label_104 label_106
label_104
    ADDi x24 x12 1
    BRANCH label_113
label_106
    CMP_GT x21 x17 -2
    SELECT x21 x20 x21 0
    MOVi x24 0
    BR_COND x21 label_113 label_109
label_109
    CMP_EQ x21 x12 0
    CMP_EQ x22 x17 3
    SELECT x21 x21 x22 0
    MOVi x24 1
    BR_COND x21 label_113 label_116
label_113
    SET_CELL x2 x9 x13 x24
    BRANCH label_116
label_116
    CMP_EQ x20 x15 900
    MOV x13 x15
    BR_COND x20 label_55 label_58
label_118
    MOVi x26 0
    BRANCH label_127
label_121
    SCREEN_FLUSH
    LIFETIME_END x2
    INC_EQ x20 x8 100
    BR_COND x20 label_40 label_41
label_124
    INC_EQ x20, x25, 1000
    BR_COND x20 label_121 label_118
label_127
    GET_CELL x1 x25 x26 x27
    CMP_LT x20 x27 1
    MOVi x28 -16777216
    BR_COND x20 label_139 label_132
label_132
    ADDi x27 x27 -1
    CMP_LT x20 x27 3
    MOVi x28 -1
    BR_COND x20 label_135 label_139
label_135
    GET_SWITCH_TBL_ELEM x28 x27
    BRANCH label_139
label_139
    SCREEN_PUT_PIXEL x25 x26 x28
    INC_EQ x20, x26, 900
    BR_COND x20 label_124 label_127
