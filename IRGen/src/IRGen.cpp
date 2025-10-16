#include "IRGen.hpp"
#include <cassert>
#include <filesystem>
#include <llvm-18/llvm/ADT/ArrayRef.h>
#include <llvm-18/llvm/IR/Attributes.h>
#include <llvm-18/llvm/IR/BasicBlock.h>
#include <llvm-18/llvm/IR/Constants.h>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Function.h>
#include <llvm-18/llvm/Transforms/Utils/Cloning.h>
#include <llvm-18/llvm/IR/GlobalObject.h>
#include <llvm-18/llvm/IR/GlobalValue.h>
#include <llvm-18/llvm/IR/GlobalVariable.h>
#include <llvm-18/llvm/IR/LLVMContext.h>
#include "llvm/IR/IRBuilder.h"
#include <llvm-18/llvm/IR/Module.h>
#include <llvm-18/llvm/IR/Value.h>
#include "llvm-18/llvm/IR/Verifier.h"
#include <llvm-18/llvm/Support/Alignment.h>
#include <llvm-18/llvm/Support/raw_ostream.h>
#include <llvm-18/llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm-18/llvm/ExecutionEngine/GenericValue.h>

#include <memory>

extern "C" {
    void sim_init();
    void screen_flush();
    void screen_put_pixel(int x, int y, int argb);
    int sim_rand();
}

namespace IRGen {

Generator::Generator() : Context(), IRModule(std::make_unique<Module>("top", Context)), Builder(Context) {}

void Generator::print(const std::optional<std::filesystem::path> &OutPath) const {
    assert(IRModule);

    if (!OutPath) {
        IRModule->print(outs(), nullptr);
        return;
    }

    std::error_code Err;
    raw_fd_ostream OS(OutPath->c_str(), Err);

    if (Err.value()) {
        return;
    }

    IRModule->print(OS, nullptr);
}

void Generator::interpret() {
    assert(IRModule);

    auto ExecModule = CloneModule(*IRModule);
    auto *App = ExecModule->getFunction(kAppName);

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

    ExecutionEngine *ee = EngineBuilder(std::move(ExecModule)).create();

    ee->InstallLazyFunctionCreator([&](const std::string &fnName) -> void * {
        if (fnName == kScreenFlushName) {
            return reinterpret_cast<void *>(screen_flush);
        } else if (fnName == kSimRandName) {
            return reinterpret_cast<void *>(sim_rand);
        } else if (fnName == kScreenPutPixelName) {
            return reinterpret_cast<void *>(screen_put_pixel);
        }
        return nullptr;
    });

    ee->finalizeObject();

    sim_init();

    ArrayRef<GenericValue> NoArgs;
    GenericValue v = ee->runFunction(App, NoArgs);
}

Function *Generator::declareSimRand() {
    assert(IRModule);

    // declare i32 @sim_rand(...) local_unnamed_addr #5
    FunctionType *FuncType = FunctionType::get(Builder.getInt32Ty(), true);
    Function *SimRand = Function::Create(FuncType, Function::ExternalLinkage, kSimRandName, *IRModule);

    return SimRand;
}

Function *Generator::declareScreenFlush() {
    assert(IRModule);

    // declare void @screen_flush(...) local_unnamed_addr #5
    FunctionType *FuncType = FunctionType::get(Builder.getVoidTy(), true);
    Function *ScreenFlush = Function::Create(FuncType, Function::ExternalLinkage, kScreenFlushName, *IRModule);

    return ScreenFlush;
}

Function *Generator::declareScreenPutPixel() {
    assert(IRModule);

    // declare void @screen_put_pixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #5
    ArrayRef<Type *> ArgTypes = {Builder.getInt32Ty(), Builder.getInt32Ty(), Builder.getInt32Ty()};
    FunctionType *FuncType = FunctionType::get(Builder.getVoidTy(), ArgTypes, false);
    Function *ScreenPutPixel = Function::Create(FuncType, Function::ExternalLinkage, kScreenPutPixelName, *IRModule);

    return ScreenPutPixel;
}

void Generator::generate() {
    FunctionType *FuncType = FunctionType::get(Builder.getVoidTy(), false);
    Function *AppFunc = Function::Create(FuncType, Function::ExternalLinkage, kAppName, *IRModule);
    auto *SimRand = declareSimRand();
    auto *ScreenFlush = declareScreenFlush();
    auto *ScreenPutPixel = declareScreenPutPixel();

    // @switch.table.app = private unnamed_addr constant [3 x i32] [i32 -16711936, i32 -16776961, i32 -65536], align 4
    auto *SwitchTableTy = ArrayType::get(Builder.getInt32Ty(), 3);
    auto &SwitchTable = globals.emplace_back(std::make_unique<GlobalVariable>(
        *IRModule,
        SwitchTableTy,
        true,
        GlobalValue::PrivateLinkage,
        ConstantArray::get(SwitchTableTy, {
            Builder.getInt32(-16711936),
            Builder.getInt32(-16776961),
            Builder.getInt32(-65536)
        }),
        "switch.table.app"
    ));

    SwitchTable->setUnnamedAddr(GlobalValue::UnnamedAddr::Local);
    SwitchTable->setAlignment(Align(4));

    BasicBlock *BB0 = BasicBlock::Create(Context, "BB0", AppFunc);
    BasicBlock *BB7 = BasicBlock::Create(Context, "BB7", AppFunc);
    BasicBlock *BB9 = BasicBlock::Create(Context, "BB9", AppFunc);
    BasicBlock *BB28 = BasicBlock::Create(Context, "BB28", AppFunc);
    BasicBlock *BB31 = BasicBlock::Create(Context, "BB31", AppFunc);
    BasicBlock *BB40 = BasicBlock::Create(Context, "BB40", AppFunc);
    BasicBlock *BB41 = BasicBlock::Create(Context, "BB41", AppFunc);
    BasicBlock *BB43 = BasicBlock::Create(Context, "BB43", AppFunc);
    BasicBlock *BB55 = BasicBlock::Create(Context, "BB55", AppFunc);
    BasicBlock *BB57 = BasicBlock::Create(Context, "BB57", AppFunc);
    BasicBlock *BB58 = BasicBlock::Create(Context, "BB58", AppFunc);
    BasicBlock *BB72 = BasicBlock::Create(Context, "BB72", AppFunc);
    BasicBlock *BB84 = BasicBlock::Create(Context, "BB84", AppFunc);
    BasicBlock *BB92 = BasicBlock::Create(Context, "BB92", AppFunc);
    BasicBlock *BB96 = BasicBlock::Create(Context, "BB96", AppFunc);
    BasicBlock *BB100 = BasicBlock::Create(Context, "BB100", AppFunc);
    BasicBlock *BB104 = BasicBlock::Create(Context, "BB104", AppFunc);
    BasicBlock *BB106 = BasicBlock::Create(Context, "BB106", AppFunc);
    BasicBlock *BB109 = BasicBlock::Create(Context, "BB109", AppFunc);
    BasicBlock *BB113 = BasicBlock::Create(Context, "BB113", AppFunc);
    BasicBlock *BB116 = BasicBlock::Create(Context, "BB116", AppFunc);
    BasicBlock *BB118 = BasicBlock::Create(Context, "BB118", AppFunc);
    BasicBlock *BB121 = BasicBlock::Create(Context, "BB121", AppFunc);
    BasicBlock *BB124 = BasicBlock::Create(Context, "BB124", AppFunc);
    BasicBlock *BB127 = BasicBlock::Create(Context, "BB127", AppFunc);
    BasicBlock *BB132 = BasicBlock::Create(Context, "BB132", AppFunc);
    BasicBlock *BB135 = BasicBlock::Create(Context, "BB135", AppFunc);
    BasicBlock *BB139 = BasicBlock::Create(Context, "BB139", AppFunc);

    ArrayType *NeighbArrTy = ArrayType::get(ArrayType::get(IntegerType::get(Context, 32), 2), 8);
    ArrayType *MemArrTy = ArrayType::get(ArrayType::get(IntegerType::get(Context, 32), 900), 1000);

    Builder.SetInsertPoint(BB0);
    // %1 = alloca [8 x [2 x i32]], align 16
    auto *Val1 = Builder.CreateAlloca(NeighbArrTy);
    // %2 = alloca [1000 x [900 x i32]], align 16
    auto *Val2 = Builder.CreateAlloca(MemArrTy);
    // %3 = bitcast [1000 x [900 x i32]]* %2 to i8*
    auto *Val3 = Builder.CreateBitCast(Val2, Type::getInt8Ty(Context)->getPointerTo());
    // %4 = alloca [1000 x [900 x i32]], align 16
    auto *Val4 = Builder.CreateAlloca(MemArrTy);
    // %5 = bitcast [1000 x [900 x i32]]* %4 to i8*
    auto *Val5 = Builder.CreateBitCast(Val4, Type::getInt8Ty(Context)->getPointerTo());
    // %6 = bitcast [1000 x [900 x i32]]* %2 to i8*
    auto *Val6 = Builder.CreateBitCast(Val2, Type::getInt8Ty(Context)->getPointerTo());
    // call void @llvm.lifetime.start.p0i8(i64 3600000, i8* nonnull %6) #7
    Builder.CreateLifetimeStart(Val6);
    // call void @llvm.memset.p0i8.i64(i8* noundef nonnull align 16 dereferenceable(3600000) %6, i8 0, i64 3600000, i1 false)
    Builder.CreateMemSet(Val6, Builder.getInt8(0), Builder.getInt64(3600000), MaybeAlign(16));
    // br label %7
    Builder.CreateBr(BB7);

    // 7:                                                ; preds = %0, %28
    Builder.SetInsertPoint(BB7);
    // %8 = phi i64 [ 0, %0 ], [ %29, %28 ]
    auto *Val8 = Builder.CreatePHI(Builder.getInt64Ty(), 2);
    // br label %31
    Builder.CreateBr(BB31);

    // 9:                                                ; preds = %28
    Builder.SetInsertPoint(BB9);
    // %10 = bitcast [1000 x [900 x i32]]* %4 to i8*
    auto *Val10 = Builder.CreateBitCast(Val4, Type::getInt8Ty(Context)->getPointerTo());
    // %11 = bitcast [8 x [2 x i32]]* %1 to i8*
    auto *Val11 = Builder.CreateBitCast(Val1, Type::getInt8Ty(Context)->getPointerTo());
    // %12 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 0, i64 0
    auto *Val12 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(0), Builder.getInt64(0)});
    // %13 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 0, i64 1
    auto *Val13 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(0), Builder.getInt64(1)});
    // %14 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 1, i64 0
    auto *Val14 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(1), Builder.getInt64(0)});
    // %15 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 1, i64 1
    auto *Val15 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(1), Builder.getInt64(1)});
    // %16 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 2, i64 0
    auto *Val16 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(2), Builder.getInt64(0)});
    // %17 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 2, i64 1
    auto *Val17 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(2), Builder.getInt64(1)});
    // %18 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 3, i64 0
    auto *Val18 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(3), Builder.getInt64(0)});
    // %19 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 3, i64 1
    auto *Val19 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(3), Builder.getInt64(1)});
    // %20 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 4, i64 0
    auto *Val20 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(4), Builder.getInt64(0)});
    // %21 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 4, i64 1
    auto *Val21 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(4), Builder.getInt64(1)});
    // %22 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 5, i64 0
    auto *Val22 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(5), Builder.getInt64(0)});
    // %23 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 5, i64 1
    auto *Val23 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(5), Builder.getInt64(1)});
    // %24 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 6, i64 0
    auto *Val24 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(6), Builder.getInt64(0)});
    // %25 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 6, i64 1
    auto *Val25 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(6), Builder.getInt64(1)});
    // %26 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 7, i64 0
    auto *Val26 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(7), Builder.getInt64(0)});
    // %27 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 7, i64 1
    auto *Val27 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Builder.getInt64(7), Builder.getInt64(1)});
    // br label %41
    Builder.CreateBr(BB41);

    // 28:                                               ; preds = %31
    Builder.SetInsertPoint(BB28);
    // %29 = add nuw nsw i64 %8, 1
    auto *Val29 = Builder.CreateAdd(Val8, Builder.getInt64(1));
    // %30 = icmp eq i64 %29, 1000
    auto *Val30 = Builder.CreateICmpEQ(Val29, Builder.getInt64(1000));
    // br i1 %30, label %9, label %7, !llvm.loop !12
    Builder.CreateCondBr(Val30, BB9, BB7);

    // 31:                                               ; preds = %7, %31
    Builder.SetInsertPoint(BB31);
    // %32 = phi i64 [ 0, %7 ], [ %38, %31 ]
    auto *Val32 = Builder.CreatePHI(Builder.getInt64Ty(), 2);
    // %33 = call i32 (...) @sim_rand() #7
    auto *Val33 = Builder.CreateCall(SimRand);
    // %34 = srem i32 %33, 3
    auto *Val34 = Builder.CreateSRem(Val33, Builder.getInt32(3));
    // %35 = icmp eq i32 %34, 0
    auto *Val35 = Builder.CreateICmpEQ(Val34, Builder.getInt32(0));
    // %36 = getelementptr inbounds [1000 x [900 x i32]], [1000 x [900 x i32]]* %2, i64 0, i64 %8, i64 %32
    auto *Val36 = Builder.CreateInBoundsGEP(MemArrTy, Val2, {Builder.getInt64(0), Val8, Val32});
    // %37 = zext i1 %35 to i32
    auto *Val37 = Builder.CreateZExt(Val35, Builder.getInt32Ty());
    // store i32 %37, i32* %36, align 4, !tbaa !5
    Builder.CreateStore(Val37, Val36);
    // %38 = add nuw nsw i64 %32, 1
    auto *Val38 = Builder.CreateAdd(Val32, Builder.getInt64(1));
    // %39 = icmp eq i64 %38, 900
    auto *Val39 = Builder.CreateICmpEQ(Val38, Builder.getInt64(900));
    // br i1 %39, label %28, label %31, !llvm.loop !13
    Builder.CreateCondBr(Val39, BB28, BB31);

    // 40:                                               ; preds = %121
    Builder.SetInsertPoint(BB40);
    // call void @llvm.lifetime.end.p0i8(i64 3600000, i8* nonnull %6) #7
    Builder.CreateLifetimeEnd(Val6);
    // ret void
    Builder.CreateRetVoid();

    // 41:                                               ; preds = %9, %121
    Builder.SetInsertPoint(BB41);
    // %42 = phi i32 [ 0, %9 ], [ %122, %121 ]
    auto *Val42 = Builder.CreatePHI(Builder.getInt32Ty(), 2);
    // call void @llvm.lifetime.start.p0i8(i64 3600000, i8* nonnull %10) #7
    Builder.CreateLifetimeStart(Val10);
    // call void @llvm.memcpy.p0i8.p0i8.i64(i8* noundef nonnull align 16 dereferenceable(3600000) %5, i8* noundef nonnull align 16 dereferenceable(3600000) %3, i64 3600000, i1 false), !tbaa !5
    Builder.CreateMemCpy(Val5, MaybeAlign(16), Val3, MaybeAlign(16), Builder.getInt64(3600000));
    // br label %43
    Builder.CreateBr(BB43);

    // 43:                                               ; preds = %41, %55
    Builder.SetInsertPoint(BB43);
    // %44 = phi i64 [ 0, %41 ], [ %46, %55 ]
    auto *Val44 = Builder.CreatePHI(Builder.getInt64Ty(), 2);
    // %45 = add nsw i64 %44, -1
    auto *Val45 = Builder.CreateNSWAdd(Val44, Builder.getInt64(-1));
    // %46 = add nuw nsw i64 %44, 1
    auto *Val46 = Builder.CreateAdd(Val44, Builder.getInt64(1));
    // %47 = trunc i64 %45 to i32
    auto *Val47 = Builder.CreateTrunc(Val45, Builder.getInt32Ty());
    // %48 = trunc i64 %45 to i32
    auto *Val48 = Builder.CreateTrunc(Val45, Builder.getInt32Ty());
    // %49 = trunc i64 %45 to i32
    auto *Val49 = Builder.CreateTrunc(Val45, Builder.getInt32Ty());
    // %50 = trunc i64 %44 to i32
    auto *Val50 = Builder.CreateTrunc(Val44, Builder.getInt32Ty());
    // %51 = trunc i64 %44 to i32
    auto *Val51 = Builder.CreateTrunc(Val44, Builder.getInt32Ty());
    // %52 = trunc i64 %46 to i32
    auto *Val52 = Builder.CreateTrunc(Val46, Builder.getInt32Ty());
    // %53 = trunc i64 %46 to i32
    auto *Val53 = Builder.CreateTrunc(Val46, Builder.getInt32Ty());
    // %54 = trunc i64 %46 to i32
    auto *Val54 = Builder.CreateTrunc(Val46, Builder.getInt32Ty());
    // br label %58
    Builder.CreateBr(BB58);

    // 55:                                               ; preds = %116
    Builder.SetInsertPoint(BB55);
    // %56 = icmp eq i64 %46, 1000
    auto *Val56 = Builder.CreateICmpEQ(Val46, Builder.getInt64(1000));
    // br i1 %56, label %57, label %43, !llvm.loop !14
    Builder.CreateCondBr(Val56, BB57, BB43);

    // 57:                                               ; preds = %55
    Builder.SetInsertPoint(BB57);
    // call void @llvm.memcpy.p0i8.p0i8.i64(i8* noundef nonnull align 16 dereferenceable(3600000) %3, i8* noundef nonnull align 16 dereferenceable(3600000) %5, i64 3600000, i1 false), !tbaa !5
    Builder.CreateMemCpy(Val3, MaybeAlign(16), Val5, MaybeAlign(16), Builder.getInt64(3600000));
    // br label %118
    Builder.CreateBr(BB118);

    // 58:                                               ; preds = %43, %116
    Builder.SetInsertPoint(BB58);
    // %59 = phi i64 [ 0, %43 ], [ %65, %116 ]
    auto *Val59 = Builder.CreatePHI(Builder.getInt64Ty(), 2);
    // %60 = getelementptr inbounds [1000 x [900 x i32]], [1000 x [900 x i32]]* %2, i64 0, i64 %44, i64 %59
    auto *Val60 = Builder.CreateInBoundsGEP(MemArrTy, Val2, {Builder.getInt64(0), Val44, Val59});
    // %61 = load i32, i32* %60, align 4, !tbaa !5
    auto *Val61 = Builder.CreateLoad(Builder.getInt32Ty(), Val60);
    // call void @llvm.lifetime.start.p0i8(i64 64, i8* nonnull %11) #7
    Builder.CreateLifetimeStart(Val11);
    // store i32 %47, i32* %12, align 16, !tbaa !5
    Builder.CreateStore(Val47, Val12);
    // %62 = add nsw i64 %59, -1
    auto *Val62 = Builder.CreateNSWAdd(Val59, Builder.getInt64(-1));
    // %63 = trunc i64 %62 to i32
    auto *Val63 = Builder.CreateTrunc(Val62, Builder.getInt32Ty());
    // store i32 %63, i32* %13, align 4, !tbaa !5
    Builder.CreateStore(Val63, Val13);
    // store i32 %48, i32* %14, align 8, !tbaa !5
    Builder.CreateStore(Val48, Val14);
    // %64 = trunc i64 %59 to i32
    auto *Val64 = Builder.CreateTrunc(Val59, Builder.getInt32Ty());
    // store i32 %64, i32* %15, align 4, !tbaa !5
    Builder.CreateStore(Val64, Val15);
    // store i32 %49, i32* %16, align 16, !tbaa !5
    Builder.CreateStore(Val49, Val16);
    // %65 = add nuw nsw i64 %59, 1
    auto *Val65 = Builder.CreateNUWAdd(Val59, Builder.getInt64(1));
    // %66 = trunc i64 %65 to i32
    auto *Val66 = Builder.CreateTrunc(Val65, Builder.getInt32Ty());
    // store i32 %66, i32* %17, align 4, !tbaa !5
    Builder.CreateStore(Val66, Val17);
    // store i32 %50, i32* %18, align 8, !tbaa !5
    Builder.CreateStore(Val50, Val18);
    // %67 = trunc i64 %62 to i32
    auto *Val67 = Builder.CreateTrunc(Val62, Builder.getInt32Ty());
    // store i32 %67, i32* %19, align 4, !tbaa !5
    Builder.CreateStore(Val67, Val19);
    // store i32 %51, i32* %20, align 16, !tbaa !5
    Builder.CreateStore(Val51, Val20);
    // %68 = trunc i64 %65 to i32
    auto *Val68 = Builder.CreateTrunc(Val65, Builder.getInt32Ty());
    // store i32 %68, i32* %21, align 4, !tbaa !5
    Builder.CreateStore(Val68, Val21);
    // store i32 %52, i32* %22, align 8, !tbaa !5
    Builder.CreateStore(Val52, Val22);
    // %69 = trunc i64 %62 to i32
    auto *Val69 = Builder.CreateTrunc(Val62, Builder.getInt32Ty());
    // store i32 %69, i32* %23, align 4, !tbaa !5
    Builder.CreateStore(Val69, Val23);
    // store i32 %53, i32* %24, align 16, !tbaa !5
    Builder.CreateStore(Val53, Val24);
    // %70 = trunc i64 %59 to i32
    auto *Val70 = Builder.CreateTrunc(Val59, Builder.getInt32Ty());
    // store i32 %70, i32* %25, align 4, !tbaa !5
    Builder.CreateStore(Val70, Val25);
    // store i32 %54, i32* %26, align 8, !tbaa !5
    Builder.CreateStore(Val54, Val26);
    // %71 = trunc i64 %65 to i32
    auto *Val71 = Builder.CreateTrunc(Val65, Builder.getInt32Ty());
    // store i32 %71, i32* %27, align 4, !tbaa !5
    Builder.CreateStore(Val71, Val27);
    // br label %72
    Builder.CreateBr(BB72);

    // 72:                                               ; preds = %92, %58
    Builder.SetInsertPoint(BB72);
    // %73 = phi i64 [ 0, %58 ], [ %94, %92 ]
    auto *Val73 = Builder.CreatePHI(Builder.getInt64Ty(), 2);
    // %74 = phi i32 [ 0, %58 ], [ %93, %92 ]
    auto *Val74 = Builder.CreatePHI(Builder.getInt32Ty(), 2);
    // %75 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 %73, i64 0
    auto *Val75 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Val73, Builder.getInt64(0)});
    // %76 = load i32, i32* %75, align 8, !tbaa !5
    auto *Val76 = Builder.CreateLoad(Builder.getInt32Ty(), Val75);
    // %77 = getelementptr inbounds [8 x [2 x i32]], [8 x [2 x i32]]* %1, i64 0, i64 %73, i64 1
    auto *Val77 = Builder.CreateInBoundsGEP(NeighbArrTy, Val1, {Builder.getInt64(0), Val73, Builder.getInt64(1)});
    // %78 = load i32, i32* %77, align 4, !tbaa !5
    auto *Val78 = Builder.CreateLoad(Builder.getInt32Ty(), Val77);
    // %79 = icmp ult i32 %76, 1000
    auto *Val79 = Builder.CreateICmpULT(Val76, Builder.getInt32(1000));
    // %80 = icmp sgt i32 %78, -1
    auto *Val80 = Builder.CreateICmpSGT(Val78, Builder.getInt32(-1));
    // %81 = select i1 %79, i1 %80, i1 false
    auto *Val81 = Builder.CreateSelect(Val79, Val80, Builder.getFalse());
    // %82 = icmp slt i32 %78, 900
    auto *Val82 = Builder.CreateICmpSLT(Val78, Builder.getInt32(900));
    // %83 = select i1 %81, i1 %82, i1 false
    auto *Val83 = Builder.CreateSelect(Val81, Val82, Builder.getFalse());
    // br i1 %83, label %84, label %92
    Builder.CreateCondBr(Val83, BB84, BB92);

    // 84:                                               ; preds = %72
    Builder.SetInsertPoint(BB84);
    // %85 = sext i32 %76 to i64
    auto *Val85 = Builder.CreateSExt(Val76, Builder.getInt64Ty());
    // %86 = sext i32 %78 to i64
    auto *Val86 = Builder.CreateSExt(Val78, Builder.getInt64Ty());
    // %87 = getelementptr inbounds [1000 x [900 x i32]], [1000 x [900 x i32]]* %2, i64 0, i64 %85, i64 %86
    auto *Val87 = Builder.CreateInBoundsGEP(MemArrTy, Val2, {Builder.getInt64(0), Val85, Val86});
    // %88 = load i32, i32* %87, align 4, !tbaa !5
    auto *Val88 = Builder.CreateLoad(Builder.getInt32Ty(), Val87);
    // %89 = icmp ne i32 %88, 0
    auto *Val89 = Builder.CreateICmpNE(Val88, Builder.getInt32(0));
    // %90 = zext i1 %89 to i32
    auto *Val90 = Builder.CreateZExt(Val89, Builder.getInt32Ty());
    // %91 = add nsw i32 %74, %90
    auto *Val91 = Builder.CreateNSWAdd(Val74, Val90);
    // br label %92
    Builder.CreateBr(BB92);

    // 92:                                               ; preds = %84, %72
    Builder.SetInsertPoint(BB92);
    // %93 = phi i32 [ %91, %84 ], [ %74, %72 ]
    auto *Val93 = Builder.CreatePHI(Builder.getInt32Ty(), 2);
    // %94 = add nuw nsw i64 %73, 1
    auto *Val94 = Builder.CreateAdd(Val73, Builder.getInt64(1));
    // %95 = icmp eq i64 %94, 8
    auto *Val95 = Builder.CreateICmpEQ(Val94, Builder.getInt64(8));
    // br i1 %95, label %96, label %72, !llvm.loop !9
    Builder.CreateCondBr(Val95, BB96, BB72);

    // 96:                                               ; preds = %92
    Builder.SetInsertPoint(BB96);
    // call void @llvm.lifetime.end.p0i8(i64 64, i8* nonnull %11) #7
    Builder.CreateLifetimeEnd(Val11);
    // %97 = icmp ne i32 %61, 0
    auto *Val97 = Builder.CreateICmpNE(Val61, Builder.getInt32(0));
    // %98 = icmp slt i32 %93, 2
    auto *Val98 = Builder.CreateICmpSLT(Val93, Builder.getInt32(2));
    // %99 = select i1 %97, i1 %98, i1 false
    auto *Val99 = Builder.CreateSelect(Val97, Val98, Builder.getFalse());
    // br i1 %99, label %113, label %100
    Builder.CreateCondBr(Val99, BB113, BB100);

    // 100:                                              ; preds = %96
    Builder.SetInsertPoint(BB100);
    // %101 = and i32 %93, -2
    auto *Val101 = Builder.CreateAnd({Val93, Builder.getInt32(-2)});
    // %102 = icmp eq i32 %101, 2
    auto *Val102 = Builder.CreateICmpEQ(Val101, Builder.getInt32(2));
    // %103 = select i1 %97, i1 %102, i1 false
    auto *Val103 = Builder.CreateSelect(Val97, Val102, Builder.getFalse());
    // br i1 %103, label %104, label %106
    Builder.CreateCondBr(Val103, BB104, BB106);

    // 104:                                              ; preds = %100
    Builder.SetInsertPoint(BB104);
    // %105 = add nsw i32 %61, 1
    auto *Val105 = Builder.CreateNSWAdd(Val61, Builder.getInt32(1));
    // br label %113
    Builder.CreateBr(BB113);

    // 106:                                              ; preds = %100
    Builder.SetInsertPoint(BB106);
    // %107 = icmp sgt i32 %93, 3
    auto *Val107 = Builder.CreateICmpSGT(Val93, Builder.getInt32(3));
    // %108 = select i1 %97, i1 %107, i1 false
    auto *Val108 = Builder.CreateSelect(Val97, Val107, Builder.getFalse());
    // br i1 %108, label %113, label %109
    Builder.CreateCondBr(Val108, BB113, BB109);

    // 109:                                              ; preds = %106
    Builder.SetInsertPoint(BB109);
    // %110 = icmp eq i32 %61, 0
    auto *Val110 = Builder.CreateICmpEQ(Val61, Builder.getInt32(0));
    // %111 = icmp eq i32 %93, 3
    auto *Val111 = Builder.CreateICmpEQ(Val93, Builder.getInt32(3));
    // %112 = select i1 %110, i1 %111, i1 false
    auto *Val112 = Builder.CreateSelect(Val110, Val111, Builder.getFalse());
    // br i1 %112, label %113, label %116
    Builder.CreateCondBr(Val112, BB113, BB116);

    // 113:                                              ; preds = %109, %106, %96, %104
    Builder.SetInsertPoint(BB113);
    // %114 = phi i32 [ %105, %104 ], [ 0, %96 ], [ 0, %106 ], [ 1, %109 ]
    auto *Val114 = Builder.CreatePHI(Builder.getInt32Ty(), 4);
    // %115 = getelementptr inbounds [1000 x [900 x i32]], [1000 x [900 x i32]]* %4, i64 0, i64 %44, i64 %59
    auto *Val115 = Builder.CreateInBoundsGEP(MemArrTy, Val4, {Builder.getInt64(0), Val44, Val59});
    // store i32 %114, i32* %115, align 4, !tbaa !5
    Builder.CreateStore(Val114, Val115);
    // br label %116
    Builder.CreateBr(BB116);

    // 116:                                              ; preds = %113, %109
    Builder.SetInsertPoint(BB116);
    // %117 = icmp eq i64 %65, 900
    auto *Val117 = Builder.CreateICmpEQ(Val65, Builder.getInt64(900));
    // br i1 %117, label %55, label %58, !llvm.loop !15
    Builder.CreateCondBr(Val117, BB55, BB58);

    // 118:                                              ; preds = %57, %124
    Builder.SetInsertPoint(BB118);
    // %119 = phi i64 [ 0, %57 ], [ %125, %124 ]
    auto *Val119 = Builder.CreatePHI(Builder.getInt64Ty(), 2);
    // %120 = trunc i64 %119 to i32
    auto *Val120 = Builder.CreateTrunc(Val119, Builder.getInt32Ty());
    // br label %127
    Builder.CreateBr(BB127);

    // 121:                                              ; preds = %124
    Builder.SetInsertPoint(BB121);
    // call void (...) @screen_flush() #7
    Builder.CreateCall(ScreenFlush);
    // call void @llvm.lifetime.end.p0i8(i64 3600000, i8* nonnull %10) #7
    Builder.CreateLifetimeEnd(Val10);
    // %122 = add nuw nsw i32 %42, 1
    auto *Val122 = Builder.CreateAdd(Val42, Builder.getInt32(1));
    // %123 = icmp eq i32 %122, 100
    auto *Val123 = Builder.CreateICmpEQ(Val122, Builder.getInt32(100));
    // br i1 %123, label %40, label %41, !llvm.loop !16
    Builder.CreateCondBr(Val123, BB40, BB41);

    // 124:                                              ; preds = %139
    Builder.SetInsertPoint(BB124);
    // %125 = add nuw nsw i64 %119, 1
    auto *Val125 = Builder.CreateAdd(Val119, Builder.getInt64(1));
    // %126 = icmp eq i64 %125, 1000
    auto *Val126 = Builder.CreateICmpEQ(Val125, Builder.getInt64(1000));
    // br i1 %126, label %121, label %118, !llvm.loop !17
    Builder.CreateCondBr(Val126, BB121, BB118);

    // 127:                                              ; preds = %118, %139
    Builder.SetInsertPoint(BB127);
    // %128 = phi i64 [ 0, %118 ], [ %142, %139 ]
    auto *Val128 = Builder.CreatePHI(Builder.getInt64Ty(), 2);
    // %129 = getelementptr inbounds [1000 x [900 x i32]], [1000 x [900 x i32]]* %2, i64 0, i64 %119, i64 %128
    auto *Val129 = Builder.CreateInBoundsGEP(MemArrTy, Val2, {Builder.getInt64(0), Val119, Val128});
    // %130 = load i32, i32* %129, align 4, !tbaa !5
    auto *Val130 = Builder.CreateLoad(Builder.getInt32Ty(), Val129);
    // %131 = icmp slt i32 %130, 1
    auto *Val131 = Builder.CreateICmpSLT(Val130, Builder.getInt32(1));
    // br i1 %131, label %139, label %132
    Builder.CreateCondBr(Val131, BB139, BB132);

    // 132:                                              ; preds = %127
    Builder.SetInsertPoint(BB132);
    // %133 = add i32 %130, -1
    auto *Val133 = Builder.CreateAdd(Val130, Builder.getInt32(-1));
    // %134 = icmp ult i32 %133, 3
    auto *Val134 = Builder.CreateICmpULT(Val133, Builder.getInt32(3));
    // br i1 %134, label %135, label %139
    Builder.CreateCondBr(Val134, BB135, BB139);

    // 135:                                              ; preds = %132
    Builder.SetInsertPoint(BB135);
    // %136 = sext i32 %133 to i64
    auto *Val136 = Builder.CreateSExt(Val133, Builder.getInt64Ty());
    // %137 = getelementptr inbounds [3 x i32], [3 x i32]* @switch.table.app, i64 0, i64 %136
    auto *Val137 = Builder.CreateInBoundsGEP(SwitchTableTy, SwitchTable.get(), {Builder.getInt64(0), Val136});
    // %138 = load i32, i32* %137, align 4
    auto *Val138 = Builder.CreateLoad(Builder.getInt32Ty(), Val137);
    // br label %139
    Builder.CreateBr(BB139);

    // 139:                                              ; preds = %132, %135, %127
    Builder.SetInsertPoint(BB139);
    // %140 = phi i32 [ -16777216, %127 ], [ %138, %135 ], [ -1, %132 ]
    auto *Val140 = Builder.CreatePHI(Builder.getInt32Ty(), 3);
    // %141 = trunc i64 %128 to i32
    auto *Val141 = Builder.CreateTrunc(Val128, Builder.getInt32Ty());
    // call void @screen_put_pixel(i32 noundef %120, i32 noundef %141, i32 noundef %140) #7
    Builder.CreateCall(ScreenPutPixel, {Val120, Val141, Val140});
    // %142 = add nuw nsw i64 %128, 1
    auto *Val142 = Builder.CreateAdd(Val128, Builder.getInt64(1));
    // %143 = icmp eq i64 %142, 900
    auto *Val143 = Builder.CreateICmpEQ(Val142, Builder.getInt64(900));
    // br i1 %143, label %124, label %127, !llvm.loop !18
    Builder.CreateCondBr(Val143, BB124, BB127);

    // %8 = phi i64 [ 0, %0 ], [ %29, %28 ]
    Val8->addIncoming(Builder.getInt64(0), BB0);
    Val8->addIncoming(Val29, BB28);

    // %32 = phi i64 [ 0, %7 ], [ %38, %31 ]
    Val32->addIncoming(Builder.getInt64(0), BB7);
    Val32->addIncoming(Val38, BB31);

    // %42 = phi i32 [ 0, %9 ], [ %122, %121 ]
    Val42->addIncoming(Builder.getInt32(0), BB9);
    Val42->addIncoming(Val122, BB121);

    // %44 = phi i64 [ 0, %41 ], [ %46, %55 ]
    Val44->addIncoming(Builder.getInt64(0), BB41);
    Val44->addIncoming(Val46, BB55);

    // %59 = phi i64 [ 0, %43 ], [ %65, %116 ]
    Val59->addIncoming(Builder.getInt64(0), BB43);
    Val59->addIncoming(Val65, BB116);

    // %73 = phi i64 [ 0, %58 ], [ %94, %92 ]
    Val73->addIncoming(Builder.getInt64(0), BB58);
    Val73->addIncoming(Val94, BB92);

    // %74 = phi i32 [ 0, %58 ], [ %93, %92 ]
    Val74->addIncoming(Builder.getInt32(0), BB58);
    Val74->addIncoming(Val93, BB92);

    // %93 = phi i32 [ %91, %84 ], [ %74, %72 ]
    Val93->addIncoming(Val91, BB84);
    Val93->addIncoming(Val74, BB72);

    // %114 = phi i32 [ %105, %104 ], [ 0, %96 ], [ 0, %106 ], [ 1, %109 ]
    Val114->addIncoming(Val105, BB104);
    Val114->addIncoming(Builder.getInt32(0), BB96);
    Val114->addIncoming(Builder.getInt32(0), BB106);
    Val114->addIncoming(Builder.getInt32(1), BB109);

    // %119 = phi i64 [ 0, %57 ], [ %125, %124 ]
    Val119->addIncoming(Builder.getInt64(0), BB57);
    Val119->addIncoming(Val125, BB124);

    // %128 = phi i64 [ 0, %118 ], [ %142, %139 ]
    Val128->addIncoming(Builder.getInt64(0), BB118);
    Val128->addIncoming(Val142, BB139);

    // %140 = phi i32 [ -16777216, %127 ], [ %138, %135 ], [ -1, %132 ]
    Val140->addIncoming(Builder.getInt32(-16777216), BB127);
    Val140->addIncoming(Val138, BB135);
    Val140->addIncoming(Builder.getInt32(-1), BB132);
}

} // namespace IRGen
