#include "IRGen.hpp"

namespace ASM2IR {

Function *IRGen::declareSimRand() {
    assert(IRModule);

    // declare i32 @sim_rand(...) local_unnamed_addr #5
    FunctionType *FuncType = FunctionType::get(Builder.getInt32Ty(), true);
    Function *SimRand = Function::Create(FuncType, Function::ExternalLinkage, kSimRandName, *IRModule);

    return SimRand;
}

Function *IRGen::declareScreenFlush() {
    assert(IRModule);

    // declare void @screen_flush(...) local_unnamed_addr #5
    FunctionType *FuncType = FunctionType::get(Builder.getVoidTy(), true);
    Function *ScreenFlush = Function::Create(FuncType, Function::ExternalLinkage, kScreenFlushName, *IRModule);

    return ScreenFlush;
}

Function *IRGen::declareScreenPutPixel() {
    assert(IRModule);

    // declare void @screen_put_pixel(i32 noundef, i32 noundef, i32 noundef) local_unnamed_addr #5
    ArrayRef<Type *> ArgTypes = {Builder.getInt32Ty(), Builder.getInt32Ty(), Builder.getInt32Ty()};
    FunctionType *FuncType = FunctionType::get(Builder.getVoidTy(), ArgTypes, false);
    Function *ScreenPutPixel = Function::Create(FuncType, Function::ExternalLinkage, kScreenPutPixelName, *IRModule);

    return ScreenPutPixel;
}

Function *IRGen::declareDumpGrid() {
    assert(IRModule);

    ArrayRef<Type *> ArgTypes = {Builder.getInt32Ty()->getPointerTo()};
    FunctionType *FuncType = FunctionType::get(Builder.getVoidTy(), ArgTypes, false);
    Function *DumpGrid = Function::Create(FuncType, Function::ExternalLinkage, kDumpGridName, *IRModule);

    return DumpGrid;
}

Function *IRGen::declareDumpNeighbours() {
    assert(IRModule);

    ArrayRef<Type *> ArgTypes = {Builder.getInt32Ty()->getPointerTo(), Builder.getInt32Ty(), Builder.getInt32Ty()};
    FunctionType *FuncType = FunctionType::get(Builder.getVoidTy(), ArgTypes, false);
    Function *DumpNeighbours = Function::Create(FuncType, Function::ExternalLinkage, kDumpNeighboursName, *IRModule);

    return DumpNeighbours;
}

Function *IRGen::declareDumpReg() {
    assert(IRModule);

    ArrayRef<Type *> ArgTypes = {Builder.getInt64Ty()};
    FunctionType *FuncType = FunctionType::get(Builder.getVoidTy(), ArgTypes, false);
    Function *DumpReg = Function::Create(FuncType, Function::ExternalLinkage, kDumpRegName, *IRModule);

    return DumpReg;
}

} // namespace ASM2IR
