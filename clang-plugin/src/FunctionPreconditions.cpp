#include "FunctionPreconditions.hpp"

#include "clang/AST/Attr.h"
#include "clang/AST/Attrs.inc"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclCXX.h"
#include "clang/Basic/Diagnostic.h"

using namespace clang;

bool DoesAppliesToFunction(DiagnosticsEngine& D, const Attr& DecoAttr, const FunctionDecl& Func)
{
    if (Func.isPure()) {
        D.Report(Func.getLocation(), D.getCustomDiagID(DiagnosticsEngine::Warning,
                                                        "`decorate` attribute does not "
                                                        "apply to pure functions"));
        return false;
    }
    if (Func.isDeletedAsWritten()) {
        D.Report(Func.getLocation(), D.getCustomDiagID(DiagnosticsEngine::Warning,
                                                        "`decorate` attribute does not "
                                                        "apply to deleted functions"));
        return false;
    }
    if (Func.isExternC()) {
        D.Report(Func.getLocation(), D.getCustomDiagID(DiagnosticsEngine::Warning,
                                                        "`decorate` attribute does not apply to "
                                                        "function with "
                                                        "external C linkage"));
        return false;
    }
    if (llvm::isa<CXXDeductionGuideDecl>(Func)) {
        D.Report(Func.getLocation(), D.getCustomDiagID(DiagnosticsEngine::Warning,
                                                        "`decorate` attribute does not "
                                                        "apply to deduction guides"));
        return false;
    }
    if (&Func != Func.getDefinition()) {
        D.Report(Func.getLocation(), D.getCustomDiagID(DiagnosticsEngine::Warning,
                                                        "`decorate` attribute has effect only "
                                                        "if used with function definition"));
        return false;
    }
    if (llvm::isa<CXXMethodDecl>(Func)) {
        D.Report(Func.getLocation(), D.getCustomDiagID(DiagnosticsEngine::Warning,
                                                        "`decorate` attribute not support member function now, but it will!"));
        return false;
    }
    return true;
}