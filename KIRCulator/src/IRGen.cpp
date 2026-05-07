#include "IRGen.hpp"

namespace KIRCulator {

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

} // namespace KIRCulator
