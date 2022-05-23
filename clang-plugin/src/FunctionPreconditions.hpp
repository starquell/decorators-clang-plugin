#pragma once

namespace clang {
    class DiagnosticsEngine;
    class Attr;
    class FunctionDecl;
}

bool DoesAppliesToFunction(clang::DiagnosticsEngine& D, const clang::Attr& DecoAttr, const clang::FunctionDecl& Func);