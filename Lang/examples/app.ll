; ModuleID = 'top'
source_filename = "top"

declare void @llvm.riscvivii.putpixel(i32, i32, i32)

define i32 @screen_put_pixel(i32 %0, i32 %1, i32 %2) {
entry:
  call void @llvm.riscvivii.putpixel(i32 %0, i32 %1, i32 %2)
  ret i32 0
}

declare void @llvm.riscvivii.flush()

define i32 @screen_flush() {
entry:
  call void @llvm.riscvivii.flush()
  ret i32 0
}

declare i32 @llvm.riscvivii.simrand()

define i32 @sim_rand() {
entry:
  %0 = call i32 @llvm.riscvivii.simrand()
  ret i32 %0
}

define i32 @get_cell_color(i32 %0) {
entry:
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 %0, ptr %1, align 4
  %3 = alloca i32, align 4
  store i32 0, ptr %2, align 4
  %4 = alloca i32, align 4
  store i32 65280, ptr %3, align 4
  %5 = alloca i32, align 4
  store i32 16711680, ptr %4, align 4
  %6 = alloca i32, align 4
  store i32 255, ptr %5, align 4
  store i32 16777215, ptr %6, align 4
  br label %if_cond

if_cond:                                          ; preds = %entry
  %7 = load i32, ptr %1, align 4
  %8 = icmp sle i32 %7, 0
  br i1 %8, label %if_body, label %fallthrought

if_body:                                          ; preds = %if_cond
  %9 = load i32, ptr %2, align 4
  ret i32 %9

fallthrought:                                     ; preds = %if_body, %if_cond
  br label %else_if_cond

else_if_cond:                                     ; preds = %fallthrought
  %10 = load i32, ptr %1, align 4
  %11 = icmp eq i32 %10, 1
  br i1 %11, label %else_if_body, label %fallthrought1

else_if_body:                                     ; preds = %else_if_cond
  %12 = load i32, ptr %3, align 4
  ret i32 %12

fallthrought1:                                    ; preds = %else_if_body, %else_if_cond
  br label %else_if_cond2

else_if_cond2:                                    ; preds = %fallthrought1
  %13 = load i32, ptr %1, align 4
  %14 = icmp eq i32 %13, 2
  br i1 %14, label %else_if_body3, label %fallthrought4

else_if_body3:                                    ; preds = %else_if_cond2
  %15 = load i32, ptr %5, align 4
  ret i32 %15

fallthrought4:                                    ; preds = %else_if_body3, %else_if_cond2
  br label %else_if_cond5

else_if_cond5:                                    ; preds = %fallthrought4
  %16 = load i32, ptr %1, align 4
  %17 = icmp eq i32 %16, 3
  br i1 %17, label %else_if_body6, label %fallthrought7

else_if_body6:                                    ; preds = %else_if_cond5
  %18 = load i32, ptr %4, align 4
  ret i32 %18

fallthrought7:                                    ; preds = %else_if_body6, %else_if_cond5
  br label %else_if_cond8

else_if_cond8:                                    ; preds = %fallthrought7
  %19 = load i32, ptr %1, align 4
  %20 = icmp sge i32 %19, 4
  br i1 %20, label %else_if_body9, label %fallthrought10

else_if_body9:                                    ; preds = %else_if_cond8
  %21 = load i32, ptr %6, align 4
  ret i32 %21

fallthrought10:                                   ; preds = %else_if_body9, %else_if_cond8
  %22 = load i32, ptr %2, align 4
  ret i32 %22
}

define i32 @app() {
entry:
  %0 = alloca i32, align 4
  %1 = alloca [1000 x [900 x i32]], align 4
  %2 = alloca i32, align 4
  store i32 0, ptr %0, align 4
  store i32 0, ptr %2, align 4
  %3 = alloca i32, align 4
  %4 = alloca [1000 x [900 x i32]], align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca [8 x [2 x i32]], align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  br label %while_cond

while_cond:                                       ; preds = %fallthrought3, %entry
  %13 = load i32, ptr %0, align 4
  %14 = icmp slt i32 %13, 1000
  br i1 %14, label %while_body, label %fallthrought

while_body:                                       ; preds = %while_cond
  store i32 0, ptr %2, align 4
  br label %while_cond1

fallthrought:                                     ; preds = %while_cond
  store i32 0, ptr %3, align 4
  br label %while_cond4

while_cond1:                                      ; preds = %while_body2, %while_body
  %15 = load i32, ptr %2, align 4
  %16 = icmp slt i32 %15, 900
  br i1 %16, label %while_body2, label %fallthrought3

while_body2:                                      ; preds = %while_cond1
  %17 = load i32, ptr %0, align 4
  %18 = load i32, ptr %2, align 4
  %19 = getelementptr [1000 x [900 x i32]], ptr %1, i32 0, i32 %17, i32 %18
  %20 = call i32 @sim_rand()
  %21 = srem i32 %20, 3
  store i32 %21, ptr %19, align 4
  %22 = load i32, ptr %2, align 4
  %23 = add i32 %22, 1
  store i32 %23, ptr %2, align 4
  br label %while_cond1

fallthrought3:                                    ; preds = %while_cond1
  %24 = load i32, ptr %0, align 4
  %25 = add i32 %24, 1
  store i32 %25, ptr %0, align 4
  br label %while_cond

while_cond4:                                      ; preds = %fallthrought59, %fallthrought
  %26 = load i32, ptr %3, align 4
  %27 = icmp slt i32 %26, 10
  br i1 %27, label %while_body5, label %fallthrought6

while_body5:                                      ; preds = %while_cond4
  store i32 0, ptr %0, align 4
  store i32 0, ptr %2, align 4
  br label %while_cond7

fallthrought6:                                    ; preds = %while_cond4
  ret i32 0

while_cond7:                                      ; preds = %fallthrought12, %while_body5
  %28 = load i32, ptr %0, align 4
  %29 = icmp slt i32 %28, 1000
  br i1 %29, label %while_body8, label %fallthrought9

while_body8:                                      ; preds = %while_cond7
  store i32 0, ptr %2, align 4
  br label %while_cond10

fallthrought9:                                    ; preds = %while_cond7
  store i32 0, ptr %0, align 4
  store i32 0, ptr %2, align 4
  br label %while_cond13

while_cond10:                                     ; preds = %while_body11, %while_body8
  %30 = load i32, ptr %2, align 4
  %31 = icmp slt i32 %30, 900
  br i1 %31, label %while_body11, label %fallthrought12

while_body11:                                     ; preds = %while_cond10
  %32 = load i32, ptr %0, align 4
  %33 = load i32, ptr %2, align 4
  %34 = getelementptr [1000 x [900 x i32]], ptr %4, i32 0, i32 %32, i32 %33
  %35 = load i32, ptr %0, align 4
  %36 = load i32, ptr %2, align 4
  %37 = getelementptr [1000 x [900 x i32]], ptr %1, i32 0, i32 %35, i32 %36
  %38 = load i32, ptr %37, align 4
  store i32 %38, ptr %34, align 4
  %39 = load i32, ptr %2, align 4
  %40 = add i32 %39, 1
  store i32 %40, ptr %2, align 4
  br label %while_cond10

fallthrought12:                                   ; preds = %while_cond10
  %41 = load i32, ptr %0, align 4
  %42 = add i32 %41, 1
  store i32 %42, ptr %0, align 4
  br label %while_cond7

while_cond13:                                     ; preds = %fallthrought18, %fallthrought9
  %43 = load i32, ptr %0, align 4
  %44 = icmp slt i32 %43, 1000
  br i1 %44, label %while_body14, label %fallthrought15

while_body14:                                     ; preds = %while_cond13
  store i32 0, ptr %2, align 4
  br label %while_cond16

fallthrought15:                                   ; preds = %while_cond13
  store i32 0, ptr %0, align 4
  store i32 0, ptr %2, align 4
  br label %while_cond51

while_cond16:                                     ; preds = %fallthrought50, %while_body14
  %45 = load i32, ptr %2, align 4
  %46 = icmp slt i32 %45, 900
  br i1 %46, label %while_body17, label %fallthrought18

while_body17:                                     ; preds = %while_cond16
  %47 = load i32, ptr %0, align 4
  %48 = load i32, ptr %2, align 4
  %49 = getelementptr [1000 x [900 x i32]], ptr %1, i32 0, i32 %47, i32 %48
  %50 = load i32, ptr %49, align 4
  store i32 %50, ptr %5, align 4
  store i32 0, ptr %6, align 4
  %51 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 0, i32 0
  %52 = load i32, ptr %0, align 4
  %53 = sub i32 %52, 1
  store i32 %53, ptr %51, align 4
  %54 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 0, i32 1
  %55 = load i32, ptr %2, align 4
  %56 = sub i32 %55, 1
  store i32 %56, ptr %54, align 4
  %57 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 1, i32 0
  %58 = load i32, ptr %0, align 4
  %59 = sub i32 %58, 1
  store i32 %59, ptr %57, align 4
  %60 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 1, i32 1
  %61 = load i32, ptr %2, align 4
  store i32 %61, ptr %60, align 4
  %62 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 2, i32 0
  %63 = load i32, ptr %0, align 4
  %64 = sub i32 %63, 1
  store i32 %64, ptr %62, align 4
  %65 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 2, i32 1
  %66 = load i32, ptr %2, align 4
  %67 = add i32 %66, 1
  store i32 %67, ptr %65, align 4
  %68 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 3, i32 0
  %69 = load i32, ptr %0, align 4
  store i32 %69, ptr %68, align 4
  %70 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 3, i32 1
  %71 = load i32, ptr %2, align 4
  %72 = sub i32 %71, 1
  store i32 %72, ptr %70, align 4
  %73 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 4, i32 1
  %74 = load i32, ptr %0, align 4
  store i32 %74, ptr %73, align 4
  %75 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 4, i32 0
  %76 = load i32, ptr %2, align 4
  %77 = add i32 %76, 1
  store i32 %77, ptr %75, align 4
  %78 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 5, i32 0
  %79 = load i32, ptr %0, align 4
  %80 = add i32 %79, 1
  store i32 %80, ptr %78, align 4
  %81 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 5, i32 1
  %82 = load i32, ptr %2, align 4
  %83 = sub i32 %82, 1
  store i32 %83, ptr %81, align 4
  %84 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 6, i32 0
  %85 = load i32, ptr %0, align 4
  %86 = add i32 %85, 1
  store i32 %86, ptr %84, align 4
  %87 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 6, i32 1
  %88 = load i32, ptr %2, align 4
  store i32 %88, ptr %87, align 4
  %89 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 7, i32 0
  %90 = load i32, ptr %0, align 4
  %91 = add i32 %90, 1
  store i32 %91, ptr %89, align 4
  %92 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 7, i32 1
  %93 = load i32, ptr %2, align 4
  %94 = add i32 %93, 1
  store i32 %94, ptr %92, align 4
  store i32 0, ptr %8, align 4
  br label %while_cond19

fallthrought18:                                   ; preds = %while_cond16
  %95 = load i32, ptr %0, align 4
  %96 = add i32 %95, 1
  store i32 %96, ptr %0, align 4
  br label %while_cond13

while_cond19:                                     ; preds = %fallthrought22, %while_body17
  %97 = load i32, ptr %8, align 4
  %98 = icmp slt i32 %97, 8
  br i1 %98, label %while_body20, label %fallthrought21

while_body20:                                     ; preds = %while_cond19
  %99 = load i32, ptr %8, align 4
  %100 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 %99, i32 0
  %101 = load i32, ptr %100, align 4
  store i32 %101, ptr %9, align 4
  %102 = load i32, ptr %8, align 4
  %103 = getelementptr [8 x [2 x i32]], ptr %7, i32 0, i32 %102, i32 1
  %104 = load i32, ptr %103, align 4
  store i32 %104, ptr %10, align 4
  br label %if_cond

fallthrought21:                                   ; preds = %while_cond19
  br label %if_cond35

if_cond:                                          ; preds = %while_body20
  %105 = load i32, ptr %9, align 4
  %106 = icmp sge i32 %105, 0
  br i1 %106, label %if_body, label %fallthrought22

if_body:                                          ; preds = %if_cond
  br label %if_cond23

fallthrought22:                                   ; preds = %fallthrought25, %if_cond
  %107 = load i32, ptr %8, align 4
  %108 = add i32 %107, 1
  store i32 %108, ptr %8, align 4
  br label %while_cond19

if_cond23:                                        ; preds = %if_body
  %109 = load i32, ptr %9, align 4
  %110 = icmp slt i32 %109, 1000
  br i1 %110, label %if_body24, label %fallthrought25

if_body24:                                        ; preds = %if_cond23
  br label %if_cond26

fallthrought25:                                   ; preds = %fallthrought28, %if_cond23
  br label %fallthrought22

if_cond26:                                        ; preds = %if_body24
  %111 = load i32, ptr %10, align 4
  %112 = icmp sge i32 %111, 0
  br i1 %112, label %if_body27, label %fallthrought28

if_body27:                                        ; preds = %if_cond26
  br label %if_cond29

fallthrought28:                                   ; preds = %fallthrought31, %if_cond26
  br label %fallthrought25

if_cond29:                                        ; preds = %if_body27
  %113 = load i32, ptr %10, align 4
  %114 = icmp slt i32 %113, 900
  br i1 %114, label %if_body30, label %fallthrought31

if_body30:                                        ; preds = %if_cond29
  %115 = load i32, ptr %9, align 4
  %116 = load i32, ptr %10, align 4
  %117 = getelementptr [1000 x [900 x i32]], ptr %1, i32 0, i32 %115, i32 %116
  %118 = load i32, ptr %117, align 4
  store i32 %118, ptr %11, align 4
  br label %if_cond32

fallthrought31:                                   ; preds = %fallthrought34, %if_cond29
  br label %fallthrought28

if_cond32:                                        ; preds = %if_body30
  %119 = load i32, ptr %11, align 4
  %120 = icmp sgt i32 %119, 0
  br i1 %120, label %if_body33, label %fallthrought34

if_body33:                                        ; preds = %if_cond32
  %121 = load i32, ptr %6, align 4
  %122 = add i32 %121, 1
  store i32 %122, ptr %6, align 4
  br label %fallthrought34

fallthrought34:                                   ; preds = %if_body33, %if_cond32
  br label %fallthrought31

if_cond35:                                        ; preds = %fallthrought21
  %123 = load i32, ptr %5, align 4
  %124 = icmp sgt i32 %123, 0
  br i1 %124, label %if_body36, label %fallthrought37

if_body36:                                        ; preds = %if_cond35
  br label %if_cond38

fallthrought37:                                   ; preds = %fallthrought47, %if_cond35
  br label %else_if_cond48

if_cond38:                                        ; preds = %if_body36
  %125 = load i32, ptr %6, align 4
  %126 = icmp slt i32 %125, 2
  br i1 %126, label %if_body39, label %fallthrought40

if_body39:                                        ; preds = %if_cond38
  %127 = load i32, ptr %0, align 4
  %128 = load i32, ptr %2, align 4
  %129 = getelementptr [1000 x [900 x i32]], ptr %4, i32 0, i32 %127, i32 %128
  store i32 0, ptr %129, align 4
  br label %fallthrought40

fallthrought40:                                   ; preds = %if_body39, %if_cond38
  br label %else_if_cond

else_if_cond:                                     ; preds = %fallthrought40
  %130 = load i32, ptr %6, align 4
  %131 = icmp eq i32 %130, 3
  br i1 %131, label %else_if_body, label %fallthrought41

else_if_body:                                     ; preds = %else_if_cond
  %132 = load i32, ptr %0, align 4
  %133 = load i32, ptr %2, align 4
  %134 = getelementptr [1000 x [900 x i32]], ptr %4, i32 0, i32 %132, i32 %133
  %135 = load i32, ptr %5, align 4
  %136 = add i32 %135, 1
  store i32 %136, ptr %134, align 4
  br label %fallthrought41

fallthrought41:                                   ; preds = %else_if_body, %else_if_cond
  br label %else_if_cond42

else_if_cond42:                                   ; preds = %fallthrought41
  %137 = load i32, ptr %6, align 4
  %138 = icmp eq i32 %137, 2
  br i1 %138, label %else_if_body43, label %fallthrought44

else_if_body43:                                   ; preds = %else_if_cond42
  %139 = load i32, ptr %0, align 4
  %140 = load i32, ptr %2, align 4
  %141 = getelementptr [1000 x [900 x i32]], ptr %4, i32 0, i32 %139, i32 %140
  %142 = load i32, ptr %5, align 4
  %143 = add i32 %142, 1
  store i32 %143, ptr %141, align 4
  br label %fallthrought44

fallthrought44:                                   ; preds = %else_if_body43, %else_if_cond42
  br label %else_if_cond45

else_if_cond45:                                   ; preds = %fallthrought44
  %144 = load i32, ptr %6, align 4
  %145 = icmp sgt i32 %144, 3
  br i1 %145, label %else_if_body46, label %fallthrought47

else_if_body46:                                   ; preds = %else_if_cond45
  %146 = load i32, ptr %0, align 4
  %147 = load i32, ptr %2, align 4
  %148 = getelementptr [1000 x [900 x i32]], ptr %4, i32 0, i32 %146, i32 %147
  store i32 0, ptr %148, align 4
  br label %fallthrought47

fallthrought47:                                   ; preds = %else_if_body46, %else_if_cond45
  br label %fallthrought37

else_if_cond48:                                   ; preds = %fallthrought37
  %149 = load i32, ptr %6, align 4
  %150 = icmp eq i32 %149, 3
  br i1 %150, label %else_if_body49, label %fallthrought50

else_if_body49:                                   ; preds = %else_if_cond48
  %151 = load i32, ptr %0, align 4
  %152 = load i32, ptr %2, align 4
  %153 = getelementptr [1000 x [900 x i32]], ptr %4, i32 0, i32 %151, i32 %152
  store i32 1, ptr %153, align 4
  br label %fallthrought50

fallthrought50:                                   ; preds = %else_if_body49, %else_if_cond48
  %154 = load i32, ptr %2, align 4
  %155 = add i32 %154, 1
  store i32 %155, ptr %2, align 4
  br label %while_cond16

while_cond51:                                     ; preds = %fallthrought56, %fallthrought15
  %156 = load i32, ptr %0, align 4
  %157 = icmp slt i32 %156, 1000
  br i1 %157, label %while_body52, label %fallthrought53

while_body52:                                     ; preds = %while_cond51
  store i32 0, ptr %2, align 4
  br label %while_cond54

fallthrought53:                                   ; preds = %while_cond51
  store i32 0, ptr %0, align 4
  store i32 0, ptr %2, align 4
  br label %while_cond57

while_cond54:                                     ; preds = %while_body55, %while_body52
  %158 = load i32, ptr %2, align 4
  %159 = icmp slt i32 %158, 900
  br i1 %159, label %while_body55, label %fallthrought56

while_body55:                                     ; preds = %while_cond54
  %160 = load i32, ptr %0, align 4
  %161 = load i32, ptr %2, align 4
  %162 = getelementptr [1000 x [900 x i32]], ptr %1, i32 0, i32 %160, i32 %161
  %163 = load i32, ptr %0, align 4
  %164 = load i32, ptr %2, align 4
  %165 = getelementptr [1000 x [900 x i32]], ptr %4, i32 0, i32 %163, i32 %164
  %166 = load i32, ptr %165, align 4
  store i32 %166, ptr %162, align 4
  %167 = load i32, ptr %2, align 4
  %168 = add i32 %167, 1
  store i32 %168, ptr %2, align 4
  br label %while_cond54

fallthrought56:                                   ; preds = %while_cond54
  %169 = load i32, ptr %0, align 4
  %170 = add i32 %169, 1
  store i32 %170, ptr %0, align 4
  br label %while_cond51

while_cond57:                                     ; preds = %fallthrought62, %fallthrought53
  %171 = load i32, ptr %0, align 4
  %172 = icmp slt i32 %171, 1000
  br i1 %172, label %while_body58, label %fallthrought59

while_body58:                                     ; preds = %while_cond57
  store i32 0, ptr %2, align 4
  br label %while_cond60

fallthrought59:                                   ; preds = %while_cond57
  %173 = call i32 @screen_flush()
  %174 = load i32, ptr %3, align 4
  %175 = add i32 %174, 1
  store i32 %175, ptr %3, align 4
  br label %while_cond4

while_cond60:                                     ; preds = %while_body61, %while_body58
  %176 = load i32, ptr %2, align 4
  %177 = icmp slt i32 %176, 900
  br i1 %177, label %while_body61, label %fallthrought62

while_body61:                                     ; preds = %while_cond60
  %178 = load i32, ptr %0, align 4
  %179 = load i32, ptr %2, align 4
  %180 = getelementptr [1000 x [900 x i32]], ptr %1, i32 0, i32 %178, i32 %179
  %181 = load i32, ptr %180, align 4
  %182 = call i32 @get_cell_color(i32 %181)
  store i32 %182, ptr %12, align 4
  %183 = load i32, ptr %0, align 4
  %184 = load i32, ptr %2, align 4
  %185 = load i32, ptr %12, align 4
  %186 = call i32 @screen_put_pixel(i32 %183, i32 %184, i32 %185)
  %187 = load i32, ptr %2, align 4
  %188 = add i32 %187, 1
  store i32 %188, ptr %2, align 4
  br label %while_cond60

fallthrought62:                                   ; preds = %while_cond60
  %189 = load i32, ptr %0, align 4
  %190 = add i32 %189, 1
  store i32 %190, ptr %0, align 4
  br label %while_cond57
}
