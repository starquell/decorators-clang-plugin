#include "clang/AST/Decl.h"
#include "clang/Basic/LangOptions.h"
#include "clang/Basic/LangStandard.h"
#include "clang/AST/Expr.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include <fmt/format.h>
#include <llvm-12/llvm/ADT/StringRef.h>

using namespace clang;

namespace {

    std::string FormatDecoratorsChain(StringRef UndecoratedName, const std::vector<StringRef>& Decorators)
    {
        auto Res = fmt::format("{0}({1})", Decorators.back(), UndecoratedName);

        for (int i = Decorators.size() - 2; i >= 0; --i) {
            Res = fmt::format("{0}({1})", Decorators[i], Res);
        }
        return Res;
    }

    std::string createDecoratorCode(StringRef UndecoratedName, StringRef DecoratedName, const std::vector<StringRef>& Decorators)
    {
        /// TODO: remove constinit if not supported
        return fmt::format(" static constinit auto {0} = {1};", DecoratedName, FormatDecoratorsChain(UndecoratedName, Decorators));
    }
}

void ApplyDecorator(Rewriter& Wr, const FunctionDecl& Func, std::vector<StringRef> DecoNames)
{
    const auto DecoratedName = Func.getNameInfo().getAsString();
    const auto UndecoratedName = "__undecorated_" + DecoratedName;
    Wr.ReplaceText(Func.getLocation(), DecoratedName.size(), UndecoratedName);

    Wr.InsertTextAfter(Func.getBody()->getEndLoc().getLocWithOffset(1), createDecoratorCode(UndecoratedName, DecoratedName, DecoNames));
}