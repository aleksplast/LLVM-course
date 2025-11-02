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
label_28
    INC_EQ x20 x3 1000
    SCREEN_FLUSH
    BR_COND x20 label_40 label_7
label_31
    SIM_RAND x4
    SREMi x4 x4 3
    GET_SWITCH_TBL_ELEM x8 x4
    SCREEN_PUT_PIXEL x3 x5 x8
    EQi x4 x4 0
    SET_CELL x1 x3 x5 x4
    INC_EQ x7 x5 900
    BR_COND x7 label_28 label_31
label_40
    LIFETIME_END x1
    EXIT
