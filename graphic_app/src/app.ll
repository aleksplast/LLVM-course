; ModuleID = 'app.c'
source_filename = "app.c"
target datalayout = "e-m:e-p:32:32-i8:8:32-i16:16:32-i64:64-n32"
target triple = "riscvivii"

@switch.table.app = private unnamed_addr constant [3 x i32] [i32 -16711936, i32 -16776961, i32 -65536], align 4

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read)
define dso_local range(i32 0, 9) i32 @get_alive_neighbours(ptr nocapture noundef readonly %grid, i32 noundef %x, i32 noundef %y) local_unnamed_addr #0 {
entry:
  %sub = add nsw i32 %x, -1
  %sub1 = add nsw i32 %y, -1
  %add = add nsw i32 %y, 1
  %add15 = add nsw i32 %x, 1
  %or.cond = icmp ult i32 %sub, 1000
  %0 = icmp ult i32 %sub1, 900
  %or.cond39 = select i1 %or.cond, i1 %0, i1 false
  br i1 %or.cond39, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %arrayidx35 = getelementptr inbounds nuw [1000 x [900 x i32]], ptr %grid, i32 0, i32 %sub, i32 %sub1
  %1 = load i32, ptr %arrayidx35, align 4, !tbaa !3
  %tobool = icmp ne i32 %1, 0
  %lnot.ext = zext i1 %tobool to i32
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %alive_num.1 = phi i32 [ %lnot.ext, %if.then ], [ 0, %entry ]
  %2 = icmp ult i32 %y, 900
  %or.cond39.1 = and i1 %or.cond, %2
  br i1 %or.cond39.1, label %if.then.1, label %if.end.1

if.then.1:                                        ; preds = %if.end
  %arrayidx35.1 = getelementptr inbounds nuw [1000 x [900 x i32]], ptr %grid, i32 0, i32 %sub, i32 %y
  %3 = load i32, ptr %arrayidx35.1, align 4, !tbaa !3
  %tobool.1 = icmp ne i32 %3, 0
  %lnot.ext.1 = zext i1 %tobool.1 to i32
  %add37.1 = add nuw nsw i32 %alive_num.1, %lnot.ext.1
  br label %if.end.1

if.end.1:                                         ; preds = %if.then.1, %if.end
  %alive_num.1.1 = phi i32 [ %add37.1, %if.then.1 ], [ %alive_num.1, %if.end ]
  %4 = icmp ult i32 %add, 900
  %or.cond39.2 = select i1 %or.cond, i1 %4, i1 false
  br i1 %or.cond39.2, label %if.then.2, label %if.end.2

if.then.2:                                        ; preds = %if.end.1
  %arrayidx35.2 = getelementptr inbounds nuw [1000 x [900 x i32]], ptr %grid, i32 0, i32 %sub, i32 %add
  %5 = load i32, ptr %arrayidx35.2, align 4, !tbaa !3
  %tobool.2 = icmp ne i32 %5, 0
  %lnot.ext.2 = zext i1 %tobool.2 to i32
  %add37.2 = add nuw nsw i32 %alive_num.1.1, %lnot.ext.2
  br label %if.end.2

if.end.2:                                         ; preds = %if.then.2, %if.end.1
  %alive_num.1.2 = phi i32 [ %add37.2, %if.then.2 ], [ %alive_num.1.1, %if.end.1 ]
  %or.cond.3 = icmp ult i32 %x, 1000
  %or.cond39.3 = select i1 %or.cond.3, i1 %0, i1 false
  br i1 %or.cond39.3, label %if.then.3, label %if.end.3

if.then.3:                                        ; preds = %if.end.2
  %arrayidx35.3 = getelementptr inbounds nuw [1000 x [900 x i32]], ptr %grid, i32 0, i32 %x, i32 %sub1
  %6 = load i32, ptr %arrayidx35.3, align 4, !tbaa !3
  %tobool.3 = icmp ne i32 %6, 0
  %lnot.ext.3 = zext i1 %tobool.3 to i32
  %add37.3 = add nuw nsw i32 %alive_num.1.2, %lnot.ext.3
  br label %if.end.3

if.end.3:                                         ; preds = %if.then.3, %if.end.2
  %alive_num.1.3 = phi i32 [ %add37.3, %if.then.3 ], [ %alive_num.1.2, %if.end.2 ]
  %or.cond39.4 = select i1 %or.cond.3, i1 %4, i1 false
  br i1 %or.cond39.4, label %if.then.4, label %if.end.4

if.then.4:                                        ; preds = %if.end.3
  %arrayidx35.4 = getelementptr inbounds nuw [1000 x [900 x i32]], ptr %grid, i32 0, i32 %x, i32 %add
  %7 = load i32, ptr %arrayidx35.4, align 4, !tbaa !3
  %tobool.4 = icmp ne i32 %7, 0
  %lnot.ext.4 = zext i1 %tobool.4 to i32
  %add37.4 = add nuw nsw i32 %alive_num.1.3, %lnot.ext.4
  br label %if.end.4

if.end.4:                                         ; preds = %if.then.4, %if.end.3
  %alive_num.1.4 = phi i32 [ %add37.4, %if.then.4 ], [ %alive_num.1.3, %if.end.3 ]
  %or.cond.5 = icmp ult i32 %add15, 1000
  %or.cond39.5 = select i1 %or.cond.5, i1 %0, i1 false
  br i1 %or.cond39.5, label %if.then.5, label %if.end.5

if.then.5:                                        ; preds = %if.end.4
  %arrayidx35.5 = getelementptr inbounds nuw [1000 x [900 x i32]], ptr %grid, i32 0, i32 %add15, i32 %sub1
  %8 = load i32, ptr %arrayidx35.5, align 4, !tbaa !3
  %tobool.5 = icmp ne i32 %8, 0
  %lnot.ext.5 = zext i1 %tobool.5 to i32
  %add37.5 = add nuw nsw i32 %alive_num.1.4, %lnot.ext.5
  br label %if.end.5

if.end.5:                                         ; preds = %if.then.5, %if.end.4
  %alive_num.1.5 = phi i32 [ %add37.5, %if.then.5 ], [ %alive_num.1.4, %if.end.4 ]
  %or.cond39.6 = and i1 %or.cond.5, %2
  br i1 %or.cond39.6, label %if.then.6, label %if.end.6

if.then.6:                                        ; preds = %if.end.5
  %arrayidx35.6 = getelementptr inbounds nuw [1000 x [900 x i32]], ptr %grid, i32 0, i32 %add15, i32 %y
  %9 = load i32, ptr %arrayidx35.6, align 4, !tbaa !3
  %tobool.6 = icmp ne i32 %9, 0
  %lnot.ext.6 = zext i1 %tobool.6 to i32
  %add37.6 = add nuw nsw i32 %alive_num.1.5, %lnot.ext.6
  br label %if.end.6

if.end.6:                                         ; preds = %if.then.6, %if.end.5
  %alive_num.1.6 = phi i32 [ %add37.6, %if.then.6 ], [ %alive_num.1.5, %if.end.5 ]
  %or.cond39.7 = select i1 %or.cond.5, i1 %4, i1 false
  br i1 %or.cond39.7, label %if.then.7, label %if.end.7

if.then.7:                                        ; preds = %if.end.6
  %arrayidx35.7 = getelementptr inbounds nuw [1000 x [900 x i32]], ptr %grid, i32 0, i32 %add15, i32 %add
  %10 = load i32, ptr %arrayidx35.7, align 4, !tbaa !3
  %tobool.7 = icmp ne i32 %10, 0
  %lnot.ext.7 = zext i1 %tobool.7 to i32
  %add37.7 = add nuw nsw i32 %alive_num.1.6, %lnot.ext.7
  br label %if.end.7

if.end.7:                                         ; preds = %if.then.7, %if.end.6
  %alive_num.1.7 = phi i32 [ %add37.7, %if.then.7 ], [ %alive_num.1.6, %if.end.6 ]
  ret i32 %alive_num.1.7
}

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none)
define dso_local range(i32 -16777216, 0) i32 @get_cell_color(i32 noundef %cell_val) local_unnamed_addr #2 {
entry:
  %cmp = icmp slt i32 %cell_val, 1
  br i1 %cmp, label %return, label %if.else

if.else:                                          ; preds = %entry
  %0 = icmp ult i32 %cell_val, 4
  br i1 %0, label %switch.lookup, label %return

switch.lookup:                                    ; preds = %if.else
  %switch.tableidx = add nsw i32 %cell_val, -1
  %switch.gep = getelementptr inbounds [3 x i32], ptr @switch.table.app, i32 0, i32 %switch.tableidx
  %switch.load = load i32, ptr %switch.gep, align 4
  br label %return

return:                                           ; preds = %if.else, %switch.lookup, %entry
  %retval.0 = phi i32 [ -16777216, %entry ], [ %switch.load, %switch.lookup ], [ -1, %if.else ]
  ret i32 %retval.0
}

; Function Attrs: nounwind
define dso_local void @app() local_unnamed_addr #3 {
entry:
  %grid = alloca [1000 x [900 x i32]], align 4
  %new_grid = alloca [1000 x [900 x i32]], align 4
  call void @llvm.lifetime.start.p0(i64 3600000, ptr nonnull %grid) #6
  call void @llvm.memset.p0.i32(ptr noundef nonnull align 4 dereferenceable(3600000) %grid, i8 0, i32 3600000, i1 false)
  %0 = tail call i32 @llvm.riscvivii.simrand()
  %rem = urem i32 %0, 3
  %tobool.not = icmp eq i32 %rem, 0
  %spec.select = zext i1 %tobool.not to i32
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %entry, %for.cond.cleanup3
  %i.0178 = phi i32 [ 0, %entry ], [ %inc9, %for.cond.cleanup3 ]
  br label %for.body4

for.cond.cleanup3:                                ; preds = %for.body4
  %inc9 = add nuw nsw i32 %i.0178, 1
  %exitcond188.not = icmp eq i32 %inc9, 1000
  br i1 %exitcond188.not, label %for.body14, label %for.cond1.preheader, !llvm.loop !7

for.body4:                                        ; preds = %for.cond1.preheader, %for.body4
  %j.0177 = phi i32 [ 0, %for.cond1.preheader ], [ %inc, %for.body4 ]
  %1 = getelementptr inbounds nuw [1000 x [900 x i32]], ptr %grid, i32 0, i32 %i.0178, i32 %j.0177
  store i32 %spec.select, ptr %1, align 4, !tbaa !3
  %inc = add nuw nsw i32 %j.0177, 1
  %exitcond.not = icmp eq i32 %inc, 900
  br i1 %exitcond.not, label %for.cond.cleanup3, label %for.body4, !llvm.loop !9

for.cond.cleanup13:                               ; preds = %for.cond.cleanup107
  call void @llvm.lifetime.end.p0(i64 3600000, ptr nonnull %grid) #6
  ret void

for.body14:                                       ; preds = %for.cond.cleanup3, %for.cond.cleanup107
  %step.0187 = phi i32 [ %inc124, %for.cond.cleanup107 ], [ 0, %for.cond.cleanup3 ]
  call void @llvm.lifetime.start.p0(i64 3600000, ptr nonnull %new_grid) #6
  call void @llvm.memcpy.p0.p0.i32(ptr noundef nonnull align 4 dereferenceable(3600000) %new_grid, ptr noundef nonnull align 4 dereferenceable(3600000) %grid, i32 3600000, i1 false), !tbaa !3
  br label %for.cond41.preheader

for.cond41.preheader:                             ; preds = %for.body14, %for.cond.cleanup43
  %i35.0182 = phi i32 [ %inc82, %for.cond.cleanup43 ], [ 0, %for.body14 ]
  br label %for.body44

for.cond.cleanup43:                               ; preds = %if.end77
  %inc82 = add nuw nsw i32 %i35.0182, 1
  %exitcond191.not = icmp eq i32 %inc82, 1000
  br i1 %exitcond191.not, label %for.cond90.preheader.preheader, label %for.cond41.preheader, !llvm.loop !10

for.cond90.preheader.preheader:                   ; preds = %for.cond.cleanup43
  call void @llvm.memcpy.p0.p0.i32(ptr noundef nonnull align 4 dereferenceable(3600000) %grid, ptr noundef nonnull align 4 dereferenceable(3600000) %new_grid, i32 3600000, i1 false), !tbaa !3
  br label %for.cond110.preheader

for.body44:                                       ; preds = %for.cond41.preheader, %if.end77
  %j40.0181 = phi i32 [ 0, %for.cond41.preheader ], [ %inc79, %if.end77 ]
  %arrayidx46 = getelementptr inbounds nuw [1000 x [900 x i32]], ptr %grid, i32 0, i32 %i35.0182, i32 %j40.0181
  %2 = load i32, ptr %arrayidx46, align 4, !tbaa !3
  %call = call i32 @get_alive_neighbours(ptr noundef nonnull %grid, i32 noundef %i35.0182, i32 noundef %j40.0181)
  %tobool47 = icmp ne i32 %2, 0
  %cmp48 = icmp samesign ult i32 %call, 2
  %or.cond = select i1 %tobool47, i1 %cmp48, i1 false
  br i1 %or.cond, label %if.end77.sink.split, label %if.else52

if.else52:                                        ; preds = %for.body44
  %3 = and i32 %call, 14
  %or.cond126 = icmp eq i32 %3, 2
  %or.cond176 = select i1 %tobool47, i1 %or.cond126, i1 false
  br i1 %or.cond176, label %if.then57, label %if.else60

if.then57:                                        ; preds = %if.else52
  %add = add nsw i32 %2, 1
  br label %if.end77.sink.split

if.else60:                                        ; preds = %if.else52
  %cmp63 = icmp samesign ugt i32 %call, 3
  %or.cond127 = select i1 %tobool47, i1 %cmp63, i1 false
  br i1 %or.cond127, label %if.end77.sink.split, label %if.else67

if.else67:                                        ; preds = %if.else60
  %tobool68 = icmp eq i32 %2, 0
  %cmp70 = icmp eq i32 %call, 3
  %or.cond128 = select i1 %tobool68, i1 %cmp70, i1 false
  br i1 %or.cond128, label %if.end77.sink.split, label %if.end77

if.end77.sink.split:                              ; preds = %if.else67, %if.else60, %for.body44, %if.then57
  %add.sink = phi i32 [ %add, %if.then57 ], [ 0, %for.body44 ], [ 0, %if.else60 ], [ 1, %if.else67 ]
  %arrayidx59 = getelementptr inbounds nuw [1000 x [900 x i32]], ptr %new_grid, i32 0, i32 %i35.0182, i32 %j40.0181
  store i32 %add.sink, ptr %arrayidx59, align 4, !tbaa !3
  br label %if.end77

if.end77:                                         ; preds = %if.end77.sink.split, %if.else67
  %inc79 = add nuw nsw i32 %j40.0181, 1
  %exitcond190.not = icmp eq i32 %inc79, 900
  br i1 %exitcond190.not, label %for.cond.cleanup43, label %for.body44, !llvm.loop !11

for.cond110.preheader:                            ; preds = %for.cond90.preheader.preheader, %for.cond.cleanup112
  %i104.0186 = phi i32 [ %inc121, %for.cond.cleanup112 ], [ 0, %for.cond90.preheader.preheader ]
  br label %for.body113

for.cond.cleanup107:                              ; preds = %for.cond.cleanup112
  tail call void @llvm.riscvivii.flush()
  call void @llvm.lifetime.end.p0(i64 3600000, ptr nonnull %new_grid) #6
  %inc124 = add nuw nsw i32 %step.0187, 1
  %exitcond195.not = icmp eq i32 %inc124, 100
  br i1 %exitcond195.not, label %for.cond.cleanup13, label %for.body14, !llvm.loop !12

for.cond.cleanup112:                              ; preds = %get_cell_color.exit
  %inc121 = add nuw nsw i32 %i104.0186, 1
  %exitcond194.not = icmp eq i32 %inc121, 1000
  br i1 %exitcond194.not, label %for.cond.cleanup107, label %for.cond110.preheader, !llvm.loop !13

for.body113:                                      ; preds = %for.cond110.preheader, %get_cell_color.exit
  %j109.0185 = phi i32 [ 0, %for.cond110.preheader ], [ %inc118, %get_cell_color.exit ]
  %arrayidx115 = getelementptr inbounds nuw [1000 x [900 x i32]], ptr %grid, i32 0, i32 %i104.0186, i32 %j109.0185
  %4 = load i32, ptr %arrayidx115, align 4, !tbaa !3
  %cmp.i = icmp slt i32 %4, 1
  br i1 %cmp.i, label %get_cell_color.exit, label %if.else.i

if.else.i:                                        ; preds = %for.body113
  %5 = icmp ult i32 %4, 4
  br i1 %5, label %switch.lookup, label %get_cell_color.exit

switch.lookup:                                    ; preds = %if.else.i
  %switch.tableidx = add nsw i32 %4, -1
  %switch.gep = getelementptr inbounds [3 x i32], ptr @switch.table.app, i32 0, i32 %switch.tableidx
  %switch.load = load i32, ptr %switch.gep, align 4
  br label %get_cell_color.exit

get_cell_color.exit:                              ; preds = %if.else.i, %switch.lookup, %for.body113
  %retval.0.i = phi i32 [ -16777216, %for.body113 ], [ %switch.load, %switch.lookup ], [ -1, %if.else.i ]
  tail call void @llvm.riscvivii.putpixel(i32 %i104.0186, i32 %j109.0185, i32 %retval.0.i)
  %inc118 = add nuw nsw i32 %j109.0185, 1
  %exitcond193.not = icmp eq i32 %inc118, 900
  br i1 %exitcond193.not, label %for.cond.cleanup112, label %for.body113, !llvm.loop !14
}

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i32(ptr nocapture writeonly, i8, i32, i1 immarg) #4

; Function Attrs: nofree nosync nounwind memory(none)
declare i32 @llvm.riscvivii.simrand() #5

; Function Attrs: nounwind
declare void @llvm.riscvivii.putpixel(i32, i32, i32) #6

; Function Attrs: nounwind
declare void @llvm.riscvivii.flush() #6

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i32(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i32, i1 immarg) #7

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: read) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #1 = { mustprogress nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" }
attributes #4 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #5 = { nofree nosync nounwind memory(none) }
attributes #6 = { nounwind }
attributes #7 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 1}
!1 = !{i32 7, !"frame-pointer", i32 2}
!2 = !{!"clang version 20.1.0 (git@github.com:aleksplast/llvm-project.git ee2190a69719c70e605511addb70fc532ee10caf)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = distinct !{!7, !8}
!8 = !{!"llvm.loop.mustprogress"}
!9 = distinct !{!9, !8}
!10 = distinct !{!10, !8}
!11 = distinct !{!11, !8}
!12 = distinct !{!12, !8}
!13 = distinct !{!13, !8}
!14 = distinct !{!14, !8}
