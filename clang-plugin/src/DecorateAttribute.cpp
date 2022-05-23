////////////////////////////////////////////////////////////////////////////////////////////////
/// There is bug - clang don`t recognize arguments of `decorate` attribute, reason unknown.
/// Some instead of using custom attribute `decorate(args...)`, you can use `clang::annotate("decorate", args...)
/// Code in this file is not compiled in plugin
////////////////////////////////////////////////////////////////////////////////////////////////

#include "clang/AST/ASTContext.h"
#include "clang/AST/Attr.h"
#include "clang/AST/Expr.h"
#include "clang/Basic/AttributeCommonInfo.h"
#include "clang/Sema/ParsedAttr.h"
#include "clang/Sema/Sema.h"
#include "clang/Sema/SemaDiagnostic.h"
#include "llvm/IR/Attributes.h"
#include <llvm/Support/raw_ostream.h>

using namespace clang;

namespace {

struct DecorateAttrInfo : public ParsedAttrInfo {

    DecorateAttrInfo()
    {
        OptArgs = 15;

        static constexpr Spelling S[] = {{ParsedAttr::AS_GNU, "decorate"},
                                         {ParsedAttr::AS_CXX11, "decorate"}};

        Spellings = S;
    }

    bool diagAppertainsToDecl(Sema &S, const ParsedAttr &Attr, const Decl *D) const override {

        const bool isFunctionDecl = isa<FunctionDecl>(D);
        const bool isRecordDecl = isa<CXXRecordDecl>(D); // class, structs and lambdas (lambdas
                                                         // compiles to CXXRecordDecl)

        if (!(isFunctionDecl || isRecordDecl)) {
            S.Diag(Attr.getLoc(), diag::warn_attribute_wrong_decl_type_str)
                << Attr << "functions, classes and lambdas";
            return false;
        }

        return true;
    }

    AttrHandling handleDeclAttribute(Sema &S, Decl *D, const ParsedAttr &Attr) const override {

        const bool isFunctionDecl = isa<FunctionDecl>(D);
        const bool isRecordDecl = isa<CXXRecordDecl>(D); // class, structs and lambdas (lambdas
                                                         // compiles to CXXRecordDecl)

        if (isFunctionDecl) {
            auto *AsFunction = cast<FunctionDecl>(D);
            if (AsFunction->isPure()) {
                /// TODO: doesnt work
                S.Diag(Attr.getLoc(), S.Diags.getCustomDiagID(DiagnosticsEngine::Warning,
                                                              "%0 attribute does not "
                                                              "apply to pure functions"))
                    << Attr;
                return AttributeNotApplied;
            }
            if (AsFunction->isDeletedAsWritten()) {
                S.Diag(Attr.getLoc(), S.Diags.getCustomDiagID(DiagnosticsEngine::Warning,
                                                              "%0 attribute does not "
                                                              "apply to deleted functions"))
                    << Attr;
                return AttributeNotApplied;
            }
            if (AsFunction->isExternC()) {
                S.Diag(Attr.getLoc(), S.Diags.getCustomDiagID(DiagnosticsEngine::Warning,
                                                              "%0 attribute does not apply to "
                                                              "function with "
                                                              "external C linkage"))
                    << Attr;
                return AttributeNotApplied;
            }
            if (isa<CXXDeductionGuideDecl>(AsFunction)) {
                S.Diag(Attr.getLoc(), S.Diags.getCustomDiagID(DiagnosticsEngine::Warning,
                                                              "%0 attribute does not "
                                                              "apply to deduction guides"))
                    << Attr;
                return AttributeNotApplied;
            }
            if (!(AsFunction != AsFunction->getDefinition())) {
                S.Diag(Attr.getLoc(), S.Diags.getCustomDiagID(DiagnosticsEngine::Warning,
                                                              "%0 attribute has effect only "
                                                              "if used with function definition"))
                    << Attr;
                return AttributeNotApplied;
            }
        }
        else if (isRecordDecl) {
            /// TODO
        }
        SmallVector<Expr *, 16> ArgsBuf;
        for (unsigned i = 0; i < Attr.getNumArgs(); i++) {
            ArgsBuf.push_back(Attr.getArgAsExpr(i));
        }
        D->addAttr(AnnotateAttr::Create(S.Context, "decorate", ArgsBuf.data(),
                                      ArgsBuf.size(), Attr.getRange()));
                      
        return AttributeApplied;
    }
};

} // namespace

static ParsedAttrInfoRegistry::Add<DecorateAttrInfo> Y("decorate-attr",
                                                      "implements decorators via attibute");
