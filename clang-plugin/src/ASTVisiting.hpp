#pragma once

#include "clang/Frontend/CompilerInstance.h"

#include <memory>

namespace clang {
    class Rewriter;
}

class DecorableFuncsFinder;

class FuncDeclConsumer : public clang::ASTConsumer {
  public:
    explicit FuncDeclConsumer(clang::CompilerInstance& CI, clang::Rewriter& Wr);

    bool HandleTopLevelDecl(clang::DeclGroupRef DG) override;

    ~FuncDeclConsumer();

  private:
    std::unique_ptr<DecorableFuncsFinder> DecorableFinder;
};