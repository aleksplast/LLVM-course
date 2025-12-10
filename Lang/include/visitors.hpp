#pragma once

#include "SilverLangLexer.h"
#include "SilverLangParser.h"
#include "SilverLangVisitor.h"
#include "antlr4-runtime.h"
#include "llvm-18/llvm/IR/IRBuilder.h"
#include "llvm-18/llvm/IR/IRBuilder.h"
#include "llvm-18/llvm/IR/LLVMContext.h"
#include "llvm-18/llvm/IR/Module.h"
#include "llvm-18/llvm/IR/Verifier.h"
#include <utility>
#include <vector>
#include <map>


namespace SilverLang {

using namespace llvm;

struct TreeLLVMWalker : public SilverLangVisitor {
  IRBuilder<> *builder;
  std::vector<std::map<std::string, Value *>> vars;
  std::vector<std::map<std::string, std::pair<Value *, ArrayType *>>> arrs;
  Function *currFunc;
  LLVMContext *ctxLLVM;
  Module *module;
  bool interpretMode;
  Type *int32Type;
  Type *voidType;
  TreeLLVMWalker(LLVMContext *ctxLLVM, IRBuilder<> *builder, Module *module,
                 bool interpretMode)
      : ctxLLVM(ctxLLVM), builder(builder), module(module),
        interpretMode(interpretMode), vars(1), arrs(1) {
    int32Type = Type::getInt32Ty(*ctxLLVM);
    voidType = Type::getVoidTy(*ctxLLVM);
  }

  Value *registerVar(const std::string &name) {
    auto *currBB = builder->GetInsertBlock();
    auto &entryBB = currFunc->getEntryBlock();
    if (!entryBB.empty()) {
      auto *lastInsn = &entryBB.back();
      builder->SetInsertPoint(lastInsn);
    } else {
      builder->SetInsertPoint(&entryBB);
    }
    auto ptr = builder->CreateAlloca(int32Type);
    vars.back()[name] = ptr;
    builder->SetInsertPoint(currBB);
    return ptr;
  }

  Value *registerArr(const std::string &name, int xSize, int ySize) {
    auto *currBB = builder->GetInsertBlock();
    auto &entryBB = currFunc->getEntryBlock();
    ArrayType *ArrTy = ArrayType::get(ArrayType::get(int32Type, ySize), xSize);
    if (!entryBB.empty()) {
      auto *lastInsn = &entryBB.back();
      builder->SetInsertPoint(lastInsn);
    } else {
      builder->SetInsertPoint(&entryBB);
    }
    auto ptr = builder->CreateAlloca(ArrTy);
    arrs.back()[name] = std::pair{(Value *)ptr, ArrTy};
    builder->SetInsertPoint(currBB);
    return ptr;
  }

  Value *searchVar(const std::string &name) {
    for (auto it = vars.rbegin(); it != vars.rend(); ++it) {
      if (auto find = it->find(name); find != it->end()) {
        return find->second;
      }
    }
    Function *func = module->getFunction(name);
    if (!func || func->arg_size() > 0) {
      return nullptr;
    }
    return nullptr;
  }

  std::pair<Value *, ArrayType *> searchArr(const std::string &name) {
    for (auto it = arrs.rbegin(); it != arrs.rend(); ++it) {
      if (auto find = it->find(name); find != it->end()) {
        return find->second;
      }
    }

    Function *func = module->getFunction(name);
    if (!func || func->arg_size() > 0) {
      return {nullptr, nullptr};
    }
    return {nullptr, nullptr};
  }

  void regGraphicFuncsIntr() {
    {
      FunctionType *FuncType = FunctionType::get(builder->getInt32Ty(), true);
      Function *SimRand = Function::Create(FuncType, Function::ExternalLinkage, "sim_rand", module);
    }

    {
      FunctionType *FuncType = FunctionType::get(builder->getVoidTy(), true);
      Function *ScreenFlush = Function::Create(FuncType, Function::ExternalLinkage, "screen_flush", module);
    }

    {
      ArrayRef<Type *> ArgTypes = {builder->getInt32Ty(), builder->getInt32Ty(), builder->getInt32Ty()};
      FunctionType *FuncType = FunctionType::get(builder->getVoidTy(), ArgTypes, false);
      Function *ScreenPutPixel = Function::Create(FuncType, Function::ExternalLinkage, "screen_put_pixel", module);
    }
  }

  antlrcpp::Any visitProgramm(SilverLangParser::ProgrammContext *ctx) override {
    regGraphicFuncsIntr();
    visitStatements(ctx->statements());
    return nullptr;
  }

  antlrcpp::Any visitStatement(SilverLangParser::StatementContext *ctx) override {
    if (ctx->vardecl()) {
      return visitVardecl(ctx->vardecl());
    } else if (ctx->funcdecl()) {
      return visitFuncdecl(ctx->funcdecl());
    } else if (ctx->assignment()) {
      return visitAssignment(ctx->assignment());
    } else if (ctx->ifstatement()) {
      return visitIfstatement(ctx->ifstatement());
    } else if (ctx->whilestatement()) {
      return visitWhilestatement(ctx->whilestatement());
    } else if (ctx->retstatement()) {
      return visitRetstatement(ctx->retstatement());
    } else if (ctx->arrdecl()) {
      return visitArrdecl(ctx->arrdecl());
    } else if (ctx->call()) {
      return visitCall(ctx->call());
    }
    return nullptr;
  }

  antlrcpp::Any visitAssignment(SilverLangParser::AssignmentContext *ctx) override {
    Value *ptr = nullptr;
    if (ctx->arraccess()) {
      ptr = std::any_cast<Value *>(visitArraccess(ctx->arraccess()));
    } else {
      std::string name = ctx->NAME()->getText();
      ptr = searchVar(name);
    }
    auto *val = std::any_cast<Value *>(visitExpr(ctx->expr()));
    builder->CreateStore(val, ptr);
    return nullptr;
  }

  antlrcpp::Any visitStatements(SilverLangParser::StatementsContext *ctx) override {
    for (auto statement: ctx->statement()) {
      visitStatement(statement);
    }
    return nullptr;
  }

  antlrcpp::Any visitFuncdecl(SilverLangParser::FuncdeclContext *ctx) override {
    std::string name = ctx->NAME()[0]->getText();
    vars.emplace_back();

    std::vector<Type *> funcParamTypes(ctx->NAME().size() - 1, int32Type);

    FunctionType *funcType =
        FunctionType::get(int32Type, funcParamTypes, false);
    Function *func = Function::Create(funcType, Function::ExternalLinkage,
                                      ctx->NAME()[0]->getText(), module);
    BasicBlock *entryBB = BasicBlock::Create(*ctxLLVM, "entry", func);
    builder->SetInsertPoint(entryBB);
    currFunc = func;
    for (int arg = 1; arg < ctx->NAME().size(); arg++) {
      auto ptr = registerVar(ctx->NAME()[arg]->getText());
      builder->CreateStore(func->getArg(arg - 1), ptr);
    }

    Value *res = nullptr;
    visitStatements(ctx->statements());
    vars.pop_back();
    return nullptr;
  }

  antlrcpp::Any visitVardecl(SilverLangParser::VardeclContext *ctx) override {
    std::string name = ctx->NAME()->getText();
    auto ptr = registerVar(name);
    builder->CreateStore(std::any_cast<Value *>(visitExpr(ctx->expr())), ptr);
    return ptr;
  }

  antlrcpp::Any visitArrdecl(SilverLangParser::ArrdeclContext *ctx) override {
    std::string name = ctx->NAME()->getText();
    int xSize = std::stoi(ctx->INT()[0]->getText());
    int ySize = std::stoi(ctx->INT()[1]->getText());
    auto ptr = registerArr(name, xSize, ySize);
    return ptr;
  }

  antlrcpp::Any visitExpr(SilverLangParser::ExprContext *ctx) override {
    if (ctx->arraccess()) {
      auto ptr = std::any_cast<Value *>(visitArraccess(ctx->arraccess()));
      return (Value *)(builder->CreateLoad(int32Type, ptr));
    }
    if (ctx->NAME()) {
      auto ptr = searchVar(ctx->NAME()->getText());
      return (Value *)(builder->CreateLoad(int32Type, ptr));
    }
    if (ctx->call()) {
      return visitCall(ctx->call());
    }
    if (ctx->INT()) {
      return (Value *)(builder->getInt32(std::stoi(ctx->INT()->getText())));
    }
    if (ctx->children.size() == 2) {
      return builder->CreateNeg(std::any_cast<Value *>(visitExpr(ctx->expr()[0])));
    }
    if (ctx->BRA()) {
      return visit(ctx->children[1]);
    }
    Value *lhs = std::any_cast<Value *>(visit(ctx->children[0]));
    Value *rhs = std::any_cast<Value *>(visit(ctx->children[2]));
    auto arith_op = ctx->children[1]->getText();
    if (ctx->ADD()) {
      return builder->CreateAdd(lhs, rhs);
    } else if (ctx->SUB()) {
      return builder->CreateSub(lhs, rhs);
    } else if (ctx->MUL()) {
      return builder->CreateMul(lhs, rhs);
    } else if (ctx->DIV()) {
      return builder->CreateSDiv(lhs, rhs);
    } else if (ctx->EQ()) {
      return builder->CreateICmpEQ(lhs, rhs);
    } else if (ctx->NE()) {
      return builder->CreateICmpNE(lhs, rhs);
    } else if (ctx->LE()) {
      return builder->CreateICmpSLE(lhs, rhs);
    } else if (ctx->LT()) {
      return builder->CreateICmpSLT(lhs, rhs);
    } else if (ctx->GE()) {
      return builder->CreateICmpSGE(lhs, rhs);
    } else if (ctx->GT()) {
      return builder->CreateICmpSGT(lhs, rhs);
    } else if (ctx->REM()) {
      return builder->CreateSRem(lhs, rhs);
    }
    return nullptr;
  }

  antlrcpp::Any visitIfstatement(SilverLangParser::IfstatementContext *ctx) override {
    auto *IfCondBB = BasicBlock::Create(*ctxLLVM, "if_cond", currFunc);
    auto *IfBodyBB = BasicBlock::Create(*ctxLLVM, "if_body", currFunc);
    auto *FallThrBB = BasicBlock::Create(*ctxLLVM, "fallthrought", currFunc);
    builder->CreateBr(IfCondBB);
    builder->SetInsertPoint(IfCondBB);
    auto Cond = std::any_cast<Value *>(visitExpr(ctx->expr()));
    Cond = builder->CreateTrunc(Cond, builder->getInt1Ty());
    builder->CreateCondBr(Cond, IfBodyBB, FallThrBB);
    builder->SetInsertPoint(IfBodyBB);
    visitStatements(ctx->statements());
    builder->CreateBr(FallThrBB);
    builder->SetInsertPoint(FallThrBB);
    if (ctx->elseifstatement()) {
      return visitElseifstatement(ctx->elseifstatement());
    }
    return nullptr;
  }

  antlrcpp::Any visitElseifstatement(SilverLangParser::ElseifstatementContext *ctx) override {
    auto *ElseIfCondBB = BasicBlock::Create(*ctxLLVM, "else_if_cond", currFunc);
    auto *ElseIfBodyBB = BasicBlock::Create(*ctxLLVM, "else_if_body", currFunc);
    auto *FallThrBB = BasicBlock::Create(*ctxLLVM, "fallthrought", currFunc);
    builder->CreateBr(ElseIfCondBB);
    builder->SetInsertPoint(ElseIfCondBB);
    auto Cond = std::any_cast<Value *>(visitExpr(ctx->expr()));
    Cond = builder->CreateTrunc(Cond, builder->getInt1Ty());
    builder->CreateCondBr(Cond, ElseIfBodyBB, FallThrBB);
    builder->SetInsertPoint(ElseIfBodyBB);
    visitStatements(ctx->statements());
    builder->CreateBr(FallThrBB);
    builder->SetInsertPoint(FallThrBB);
    if (ctx->elseifstatement()) {
      return visitElseifstatement(ctx->elseifstatement());
    }
    return nullptr;
  }

  antlrcpp::Any visitWhilestatement(SilverLangParser::WhilestatementContext *ctx) override {
    auto *WhileCondBB = BasicBlock::Create(*ctxLLVM, "while_cond", currFunc);
    auto *WhileBodyBB = BasicBlock::Create(*ctxLLVM, "while_body", currFunc);
    auto *FallThrBB = BasicBlock::Create(*ctxLLVM, "fallthrought", currFunc);
    builder->CreateBr(WhileCondBB);
    builder->SetInsertPoint(WhileCondBB);
    auto Cond = std::any_cast<Value *>(visitExpr(ctx->expr()));
    Cond = builder->CreateTrunc(Cond, builder->getInt1Ty());
    builder->CreateCondBr(Cond, WhileBodyBB, FallThrBB);
    builder->SetInsertPoint(WhileBodyBB);
    visitStatements(ctx->statements());
    builder->CreateBr(WhileCondBB);
    builder->SetInsertPoint(FallThrBB);
    return nullptr;
  }

  antlrcpp::Any visitRetstatement(SilverLangParser::RetstatementContext *ctx) override {
    auto *RetVal = std::any_cast<Value *>(visitExpr(ctx->expr()));
    builder->CreateRet(RetVal);
    return nullptr;
  }

  antlrcpp::Any visitCall(SilverLangParser::CallContext *ctx) override {
    auto FuncName = ctx->NAME()->getText();
    Function *func = module->getFunction(FuncName);
    if (!func) {
      return nullptr;
    }
    int argSize = ctx->expr().size();
    if (argSize != func->arg_size()) {
      return nullptr;
    }
    std::vector<Value *> args;
    args.reserve(argSize);
    for (int i = 0; i < argSize; i++) {
      auto *val = std::any_cast<Value *>(visitExpr(ctx->expr()[i]));
      args.push_back(val);
    }
    return (Value *)builder->CreateCall(func, args);
  }

  antlrcpp::Any visitArraccess(SilverLangParser::ArraccessContext *ctx) override {
    auto *xIdx = std::any_cast<Value *>(visitExpr(ctx->expr()[0]));
    auto *yIdx = std::any_cast<Value *>(visitExpr(ctx->expr()[1]));

    auto [ArrPtr, ArrType] = searchArr(ctx->NAME()->getText());
    return builder->CreateGEP(ArrType, ArrPtr, {builder->getInt32(0), xIdx, yIdx});
  }
};

} // namespace SilverLang
