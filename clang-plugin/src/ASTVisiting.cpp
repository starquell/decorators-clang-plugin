#include "ASTVisiting.hpp"

#include "clang/AST/DeclCXX.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/AST/Attr.h"
#include <memory>

#include "CodeGeneration.hpp"
#include "FunctionPreconditions.hpp"


using namespace clang;

bool IsDecorateAttr(const Attr* Attr) 
{
    return Attr->getKind() == attr::Kind::Annotate 
        && cast<AnnotateAttr>(Attr)->getAnnotation() == "decorate";
}

class DecorableFuncsFinder : public RecursiveASTVisitor<DecorableFuncsFinder> {
  public:
    explicit DecorableFuncsFinder(CompilerInstance& CI, Rewriter& Wr)
         : _CI(CI), _Wr(Wr) {}

    bool VisitFunctionDecl(FunctionDecl* FunctionDecl) 
    { 
        const AttrVec& Attrs = FunctionDecl->getAttrs();

        const AnnotateAttr* DecorateAttr = nullptr;

        const auto DecorateAttrOccurencies = std::count_if(Attrs.begin(), Attrs.end(), [&] (const Attr* a) 
                                                          { 
                                                               const bool needed = IsDecorateAttr(a);
                                                               if (needed) {
                                                                   DecorateAttr = cast<AnnotateAttr>(a);
                                                               }
                                                               return needed;
                                                          });

        if (DecorateAttrOccurencies == 0) {
             return true;
        }
        if (DecorateAttrOccurencies > 1) {
            getDiag().Report(getDiag().getCustomDiagID(DiagnosticsEngine::Error, 
                                                      "attribute 'decorate' can appear only once in functions attribute list"));
            return true;
        }

        if (DecorateAttr->args_size() == 0) {
             getDiag().Report(getDiag().getCustomDiagID(DiagnosticsEngine::Error, 
                                                      "attribute 'decorate' should accept at least 1 argument"));
            return true;
        }

        if (!DoesAppliesToFunction(getDiag(), *DecorateAttr, *FunctionDecl)) {
            return true;
        }

        std::vector Args = [&]
        {
            /// TODO: analyze if argument actually decorator
            auto Vec = std::vector<StringRef>{};
            for (auto Arg: DecorateAttr->args()) {
                auto AsWritten = Arg->IgnoreImplicitAsWritten();
                if (auto* Lit = dyn_cast<clang::StringLiteral>(AsWritten)) {
                    Vec.push_back(Lit->getString());
                }    
            }
            return Vec;
        }();

        if (Args.empty()) {
            return true;
        }

        ApplyDecorator(_Wr, *FunctionDecl, Args);

        return true;
    }

private:
    DiagnosticsEngine& getDiag() { return _CI.getDiagnostics(); }

public:
    Rewriter& _Wr;
    CompilerInstance& _CI;
};

/// Indented to find decorators templates, not implemented yet
class DecoratorsFinder : public RecursiveASTVisitor<DecorableFuncsFinder> {

public:
    explicit DecoratorsFinder(CompilerInstance& CI) 
        : CI(CI), Diag(CI.getDiagnostics()) {}

    bool VisitFunctionTemplateDecl(FunctionTemplateDecl* Decl) {

        Diag.Report(Diag.getCustomDiagID(DiagnosticsEngine::Warning, "name of template: %0")) << Decl;
        return true;
    }

    CompilerInstance& CI;
    DiagnosticsEngine& Diag;
};


FuncDeclConsumer::FuncDeclConsumer(CompilerInstance& CI, Rewriter& Wr) 
    : DecorableFinder(std::make_unique<DecorableFuncsFinder>(CI, Wr)) 
{}

bool FuncDeclConsumer::HandleTopLevelDecl(DeclGroupRef DG) {
    for (Decl* Decl : DG) {
        DecorableFinder->TraverseDecl(Decl);
    }
    return true;
}

FuncDeclConsumer::~FuncDeclConsumer() = default;
