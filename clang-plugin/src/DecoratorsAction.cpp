#include "clang/AST/Attr.h"
#include "clang/AST/AttrIterator.h"
#include "clang/AST/Attrs.inc"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/TemplateBase.h"
#include "clang/AST/Type.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Basic/AttrKinds.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/Specifiers.h"
#include <algorithm>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Frontend/FrontendPluginRegistry.h>
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/FormatVariadic.h"
#include <llvm-12/llvm/ADT/ArrayRef.h>
#include <llvm-12/llvm/Support/Casting.h>
#include <llvm-12/llvm/Support/raw_ostream.h>
#include <memory>
#include <optional>

#include <fmt/format.h>

#include "Compile.hpp"
#include "ASTVisiting.hpp"

using namespace clang;
using namespace clang::ast_matchers;

namespace {

auto getRewriter() -> std::optional<Rewriter>& 
{
    static std::optional<Rewriter> _instance;
    return _instance;
}

void dumpRewritenToFile(StringRef FromFile, const RewriteBuffer& buf)
{
    auto [Path, Ext] = FromFile.rsplit('.');
    auto [Rest, Name] = Path.rsplit('/');

    const auto& ActualName = Name.empty() ? Rest : Name;
    const auto RewritenFilePath = fmt::format("{}_decorated.{}", ActualName, Ext);
    
     auto err = std::error_code{};
     llvm::raw_fd_ostream fileStream {RewritenFilePath, err};
     fileStream << std::string{buf.begin(), buf.end()};
     fileStream.close();
}


class DecoratorsAction : public PluginASTAction {
  public:
    auto CreateASTConsumer(CompilerInstance &CI, llvm::StringRef InFile)
        -> std::unique_ptr<ASTConsumer> override
    {
        getRewriter().emplace(CI.getSourceManager(), CI.getLangOpts());
        _CI = &CI;
        _filename = InFile.str();
        _currentFile = CI.getSourceManager().getMainFileID();
        return std::make_unique<FuncDeclConsumer>(CI, *getRewriter());
    }

    ActionType getActionType() override { return ActionType::ReplaceAction; }

    bool ParseArgs(const CompilerInstance &CI, const std::vector<std::string> &Args) override {
        return true;
    }

    void EndSourceFileAction() override
    {
        auto* FileRewriteBuffer = getRewriter()->getRewriteBufferFor(_currentFile);
        if (FileRewriteBuffer) 
        {
            dumpRewritenToFile(_filename, *FileRewriteBuffer);

            compile(_CI, _filename, FileRewriteBuffer->begin(), FileRewriteBuffer->end());
        }
        else {
            auto optBuf = _CI->getSourceManager().getBufferDataOrNone(_currentFile);
            assert(optBuf.hasValue());

            compile(_CI, _filename, optBuf->begin(), optBuf->end());
        }
  }


  private:
    CompilerInstance* _CI = nullptr;
    std::string _filename;
    FileID _currentFile;
};

} // namespace

static FrontendPluginRegistry::Add<DecoratorsAction> X("decorators",
                                                       "decorators support via attribute");
