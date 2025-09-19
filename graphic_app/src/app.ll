; ModuleID = 'app.c'
source_filename = "app.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@switch.table.app = private unnamed_addr constant [3 x i32] [i32 -16711936, i32 -16776961, i32 -65536], align 4

; Function Attrs: nofree nosync nounwind readonly uwtable
define dso_local i32 @get_alive_neighbours([1000 x [900 x i32]]* nocapture noundef readonly %0, i32 noundef %1, i32 noundef %2) local_unnamed_addr #0 {
  %4 = alloca [8 x [2 x i32]], align 16
  %5 = bitcast [8 x [2 x i32]]* %4 to i8*
  call void @llvm.lifetime.start.p0i8(i64 64, i8* nonnull %5) #7
  %6 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 0, i64 0
  %7 = add nsw i32 %1, -1
  store i32 %7, i32* %6, align 16, !tbaa !5
  %8 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 0, i64 1
  %9 = add nsw i32 %2, -1
  store i32 %9, i32* %8, align 4, !tbaa !5
  %10 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 1, i64 0
  store i32 %7, i32* %10, align 8, !tbaa !5
  %11 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 1, i64 1
  store i32 %2, i32* %11, align 4, !tbaa !5
  %12 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 2, i64 0
  store i32 %7, i32* %12, align 16, !tbaa !5
  %13 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 2, i64 1
  %14 = add nsw i32 %2, 1
  store i32 %14, i32* %13, align 4, !tbaa !5
  %15 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 3, i64 0
  store i32 %1, i32* %15, align 8, !tbaa !5
  %16 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 3, i64 1
  store i32 %9, i32* %16, align 4, !tbaa !5
  %17 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 4, i64 0
  store i32 %1, i32* %17, align 16, !tbaa !5
  %18 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 4, i64 1
  store i32 %14, i32* %18, align 4, !tbaa !5
  %19 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 5, i64 0
  %20 = add nsw i32 %1, 1
  store i32 %20, i32* %19, align 8, !tbaa !5
  %21 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 5, i64 1
  store i32 %9, i32* %21, align 4, !tbaa !5
  %22 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 6, i64 0
  store i32 %20, i32* %22, align 16, !tbaa !5
  %23 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 6, i64 1
  store i32 %2, i32* %23, align 4, !tbaa !5
  %24 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 7, i64 0
  store i32 %20, i32* %24, align 8, !tbaa !5
  %25 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 7, i64 1
  store i32 %14, i32* %25, align 4, !tbaa !5
  br label %27

26:                                               ; preds = %47
  call void @llvm.lifetime.end.p0i8(i64 64, i8* nonnull %5) #7
  ret i32 %48

27:                                               ; preds = %3, %47
  %28 = phi i64 [ 0, %3 ], [ %49, %47 ]
  %29 = phi i32 [ 0, %3 ], [ %48, %47 ]
  %30 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 %28, i64 0
  %31 = load i32, i32* %30, align 8, !tbaa !5
  %32 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %4, i64 0, i64 %28, i64 1
  %33 = load i32, i32* %32, align 4, !tbaa !5
  %34 = icmp ult i32 %31, 1000
  %35 = icmp sgt i32 %33, -1
  %36 = select i1 %34, i1 %35, i1 false
  %37 = icmp slt i32 %33, 900
  %38 = select i1 %36, i1 %37, i1 false
  br i1 %38, label %39, label %47

39:                                               ; preds = %27
  %40 = sext i32 %31 to i64
  %41 = sext i32 %33 to i64
  %42 = getelementptr inbounds [1000 x [900 x i32]], [1000 x [900 x i32]]* %0, i64 0, i64 %40, i64 %41
  %43 = load i32, i32* %42, align 4, !tbaa !5
  %44 = icmp ne i32 %43, 0
  %45 = zext i1 %44 to i32
  %46 = add nsw i32 %29, %45
  br label %47

47:                                               ; preds = %39, %27
  %48 = phi i32 [ %46, %39 ], [ %29, %27 ]
  %49 = add nuw nsw i64 %28, 1
  %50 = icmp eq i64 %49, 8
  br i1 %50, label %26, label %27, !llvm.loop !9
}

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: argmemonly mustprogress nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #1

; Function Attrs: mustprogress nofree norecurse nosync nounwind readnone uwtable willreturn
define dso_local i32 @get_cell_color(i32 noundef %0) local_unnamed_addr #2 {
  %2 = icmp slt i32 %0, 1
  br i1 %2, label %10, label %3

3:                                                ; preds = %1
  %4 = add i32 %0, -1
  %5 = icmp ult i32 %4, 3
  br i1 %5, label %6, label %10

6:                                                ; preds = %3
  %7 = sext i32 %4 to i64
  %8 = getelementptr inbounds [3 x i32], [3 x i32]* @switch.table.app, i64 0, i64 %7
  %9 = load i32, i32* %8, align 4
  br label %10

10:                                               ; preds = %3, %6, %1
  %11 = phi i32 [ -16777216, %1 ], [ %9, %6 ], [ -1, %3 ]
  ret i32 %11
}

; Function Attrs: nounwind uwtable
define dso_local void @app() local_unnamed_addr #3 {
  %1 = alloca [8 x [2 x i32]], align 16
  %2 = alloca [1000 x [900 x i32]], align 16
  %3 = bitcast [1000 x [900 x i32]]* %2 to i8*
  %4 = alloca [1000 x [900 x i32]], align 16
  %5 = bitcast [1000 x [900 x i32]]* %4 to i8*
  %6 = bitcast [1000 x [900 x i32]]* %2 to i8*
  call void @llvm.lifetime.start.p0i8(i64 3600000, i8* nonnull %6) #7
  call void @llvm.memset.p0i8.i64(i8* noundef nonnull align 16 dereferenceable(3600000) %6, i8 0, i64 3600000, i1 false)
  br label %7

7:                                                ; preds = %0, %28
  %8 = phi i64 [ 0, %0 ], [ %29, %28 ]
  br label %31

9:                                                ; preds = %28
  %10 = bitcast [1000 x [900 x i32]]* %4 to i8*
  %11 = bitcast [8 x [2 x i32]]* %1 to i8*
  %12 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 0, i64 0
  %13 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 0, i64 1
  %14 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 1, i64 0
  %15 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 1, i64 1
  %16 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 2, i64 0
  %17 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 2, i64 1
  %18 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 3, i64 0
  %19 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 3, i64 1
  %20 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 4, i64 0
  %21 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 4, i64 1
  %22 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 5, i64 0
  %23 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 5, i64 1
  %24 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 6, i64 0
  %25 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 6, i64 1
  %26 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 7, i64 0
  %27 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 7, i64 1
  br label %41

28:                                               ; preds = %31
  %29 = add nuw nsw i64 %8, 1
  %30 = icmp eq i64 %29, 1000
  br i1 %30, label %9, label %7, !llvm.loop !12

31:                                               ; preds = %7, %31
  %32 = phi i64 [ 0, %7 ], [ %38, %31 ]
  %33 = call i32 (...) @sim_rand() #7
  %34 = srem i32 %33, 3
  %35 = icmp eq i32 %34, 0
  %36 = getelementptr inbounds [1000 x [900 x i32]], [1000 x [900 x i32]]* %2, i64 0, i64 %8, i64 %32
  %37 = zext i1 %35 to i32
  store i32 %37, i32* %36, align 4, !tbaa !5
  %38 = add nuw nsw i64 %32, 1
  %39 = icmp eq i64 %38, 900
  br i1 %39, label %28, label %31, !llvm.loop !13

40:                                               ; preds = %121
  call void @llvm.lifetime.end.p0i8(i64 3600000, i8* nonnull %6) #7
  ret void

41:                                               ; preds = %9, %121
  %42 = phi i32 [ 0, %9 ], [ %122, %121 ]
  call void @llvm.lifetime.start.p0i8(i64 3600000, i8* nonnull %10) #7
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* noundef nonnull align 16 dereferenceable(3600000) %5, i8* noundef nonnull align 16 dereferenceable(3600000) %3, i64 3600000, i1 false), !tbaa !5
  br label %43

43:                                               ; preds = %41, %55
  %44 = phi i64 [ 0, %41 ], [ %46, %55 ]
  %45 = add nsw i64 %44, -1
  %46 = add nuw nsw i64 %44, 1
  %47 = trunc i64 %45 to i32
  %48 = trunc i64 %45 to i32
  %49 = trunc i64 %45 to i32
  %50 = trunc i64 %44 to i32
  %51 = trunc i64 %44 to i32
  %52 = trunc i64 %46 to i32
  %53 = trunc i64 %46 to i32
  %54 = trunc i64 %46 to i32
  br label %58

55:                                               ; preds = %116
  %56 = icmp eq i64 %46, 1000
  br i1 %56, label %57, label %43, !llvm.loop !14

57:                                               ; preds = %55
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* noundef nonnull align 16 dereferenceable(3600000) %3, i8* noundef nonnull align 16 dereferenceable(3600000) %5, i64 3600000, i1 false), !tbaa !5
  br label %118

58:                                               ; preds = %43, %116
  %59 = phi i64 [ 0, %43 ], [ %65, %116 ]
  %60 = getelementptr inbounds [1000 x [900 x i32]], [1000 x [900 x i32]]* %2, i64 0, i64 %44, i64 %59
  %61 = load i32, i32* %60, align 4, !tbaa !5
  call void @llvm.lifetime.start.p0i8(i64 64, i8* nonnull %11) #7
  store i32 %47, i32* %12, align 16, !tbaa !5
  %62 = add nsw i64 %59, -1
  %63 = trunc i64 %62 to i32
  store i32 %63, i32* %13, align 4, !tbaa !5
  store i32 %48, i32* %14, align 8, !tbaa !5
  %64 = trunc i64 %59 to i32
  store i32 %64, i32* %15, align 4, !tbaa !5
  store i32 %49, i32* %16, align 16, !tbaa !5
  %65 = add nuw nsw i64 %59, 1
  %66 = trunc i64 %65 to i32
  store i32 %66, i32* %17, align 4, !tbaa !5
  store i32 %50, i32* %18, align 8, !tbaa !5
  %67 = trunc i64 %62 to i32
  store i32 %67, i32* %19, align 4, !tbaa !5
  store i32 %51, i32* %20, align 16, !tbaa !5
  %68 = trunc i64 %65 to i32
  store i32 %68, i32* %21, align 4, !tbaa !5
  store i32 %52, i32* %22, align 8, !tbaa !5
  %69 = trunc i64 %62 to i32
  store i32 %69, i32* %23, align 4, !tbaa !5
  store i32 %53, i32* %24, align 16, !tbaa !5
  %70 = trunc i64 %59 to i32
  store i32 %70, i32* %25, align 4, !tbaa !5
  store i32 %54, i32* %26, align 8, !tbaa !5
  %71 = trunc i64 %65 to i32
  store i32 %71, i32* %27, align 4, !tbaa !5
  br label %72

72:                                               ; preds = %92, %58
  %73 = phi i64 [ 0, %58 ], [ %94, %92 ]
  %74 = phi i32 [ 0, %58 ], [ %93, %92 ]
  %75 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 %73, i64 0
  %76 = load i32, i32* %75, align 8, !tbaa !5
  %77 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 %73, i64 1
  %78 = load i32, i32* %77, align 4, !tbaa !5
  %79 = icmp ult i32 %76, 1000
  %80 = icmp sgt i32 %78, -1
  %81 = select i1 %79, i1 %80, i1 false
  %82 = icmp slt i32 %78, 900
  %83 = select i1 %81, i1 %82, i1 false
  br i1 %83, label %84, label %92

84:                                               ; preds = %72
  %85 = sext i32 %76 to i64
  %86 = sext i32 %78 to i64
  %87 = getelementptr inbounds [1000 x [900 x i32]], [1000 x [900 x i32]]* %2, i64 0, i64 %85, i64 %86
  %88 = load i32, i32* %87, align 4, !tbaa !5
  %89 = icmp ne i32 %88, 0
  %90 = zext i1 %89 to i32
  %91 = add nsw i32 %74, %90
  br label %92

92:                                               ; preds = %84, %72
  %93 = phi i32 [ %91, %84 ], [ %74, %72 ]
  %94 = add nuw nsw i64 %73, 1
  %95 = icmp eq i64 %94, 8
  br i1 %95, label %96, label %72, !llvm.loop !9

96:                                               ; preds = %92
  call void @llvm.lifetime.end.p0i8(i64 64, i8* nonnull %11) #7
  %97 = icmp ne i32 %61, 0
  %98 = icmp slt i32 %93, 2
  %99 = select i1 %97, i1 %98, i1 false
  br i1 %99, label %113, label %100

100:                                              ; preds = %96
  %101 = and i32 %93, -2
  %102 = icmp eq i32 %101, 2
  %103 = select i1 %97, i1 %102, i1 false
  br i1 %103, label %104, label %106

104:                                              ; preds = %100
  %105 = add nsw i32 %61, 1
  br label %113

106:                                              ; preds = %100
  %107 = icmp sgt i32 %93, 3
  %108 = select i1 %97, i1 %107, i1 false
  br i1 %108, label %113, label %109

109:                                              ; preds = %106
  %110 = icmp eq i32 %61, 0
  %111 = icmp eq i32 %93, 3
  %112 = select i1 %110, i1 %111, i1 false
  br i1 %112, label %113, label %116

113:                                              ; preds = %109, %106, %96, %104
  %114 = phi i32 [ %105, %104 ], [ 0, %96 ], [ 0, %106 ], [ 1, %109 ]
  %115 = getelementptr inbounds [1000 x [900 x i32]], [1000 x [900 x i32]]* %4, i64 0, i64 %44, i64 %59
  store i32 %114, i32* %115, align 4, !tbaa !5
  br label %116

116:                                              ; preds = %113, %109
  %117 = icmp eq i64 %65, 900
  br i1 %117, label %55, label %58, !llvm.loop !15

118:                                              ; preds = %57, %124
  %119 = phi i64 [ 0, %57 ], [ %125, %124 ]
  %120 = trunc i64 %119 to i32
  br label %127

121:                                              ; preds = %124
  call void (...) @screen_flush() #7
  call void @llvm.lifetime.end.p0i8(i64 3600000, i8* nonnull %10) #7
  %122 = add nuw nsw i32 %42, 1
  %123 = icmp eq i32 %122, 100
  br i1 %123, label %40, label %41, !llvm.loop !16

124:                                              ; preds = %139
  %125 = add nuw nsw i64 %119, 1
  %126 = icmp eq i64 %125, 1000
  br i1 %126, label %121, label %118, !llvm.loop !17

127:                                              ; preds = %118, %139
  %128 = phi i64 [ 0, %118 ], [ %142, %139 ]
  %129 = getelementptr inbounds [1000 x [900 x i32]], [1000 x [900 x i32]]* %2, i64 0, i64 %119, i64 %128
  %130 = load i32, i32* %129, align 4, !tbaa !5
  %131 = icmp slt i32 %130, 1
  br i1 %131, label %139, label %132

132:                                              ; preds = %127
  %133 = add i32 %130, -1
  %134 = icmp ult i32 %133, 3
  br i1 %134, label %135, label %139

135:                                              ; preds = %132
  %136 = sext i32 %133 to i64
  %137 = getelementptr inbounds [3 x i32], [3 x i32]* @switch.table.app, i64 0, i64 %136
  %138 = load i32, i32* %137, align 4
  br label %139

139:                                              ; preds = %132, %135, %127
  %140 = phi i32 [ -16777216, %127 ], [ %138, %135 ], [ -1, %132 ]
  %141 = trunc i64 %128 to i32
  call void @screen_put_pixel(i32 noundef %120, i32 noundef %141, i32 noundef %140) #7
  %142 = add nuw nsw i64 %128, 1
  %143 = icmp eq i64 %142, 900
  br i1 %143, label %124, label %127, !llvm.loop !18
}

; Function Attrs: argmemonly mustprogress nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #4

declare i32 @sim_rand(...) local_unnamed_addr #5

declare void @screen_put_pixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #5

declare void @screen_flush(...) local_unnamed_addr #5

; Function Attrs: argmemonly nofree nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #6

attributes #0 = { nofree nosync nounwind readonly uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { argmemonly mustprogress nofree nosync nounwind willreturn }
attributes #2 = { mustprogress nofree norecurse nosync nounwind readnone uwtable willreturn "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { argmemonly mustprogress nofree nounwind willreturn writeonly }
attributes #5 = { "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { argmemonly nofree nounwind willreturn }
attributes #7 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = distinct !{!9, !10, !11}
!10 = !{!"llvm.loop.mustprogress"}
!11 = !{!"llvm.loop.unroll.disable"}
!12 = distinct !{!12, !10, !11}
!13 = distinct !{!13, !10, !11}
!14 = distinct !{!14, !10, !11}
!15 = distinct !{!15, !10, !11}
!16 = distinct !{!16, !10, !11}
!17 = distinct !{!17, !10, !11}
!18 = distinct !{!18, !10, !11}
