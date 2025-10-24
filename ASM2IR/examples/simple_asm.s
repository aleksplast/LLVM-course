init
    ALLOCA_NEIGHBOURS x0 // %1
    ALLOCA_GRID x1  // %3, %6
    ALLOCA_GRID x2 // %5
    LIFETIME_START x1
    MEMSET x1
    MOVi x3, 0

label_7
    // x3 = %8 = phi i64 [ 0, %0 ], [ %29, %28 ]
    BRANCH label_31

label_9
    NEIGHBOURS



