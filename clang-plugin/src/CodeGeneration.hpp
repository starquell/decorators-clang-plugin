#include <string>
#include <vector>

namespace llvm {
    class StringRef;
}

namespace clang {
     class Rewriter;
     class FunctionDecl;
}

void ApplyDecorator(clang::Rewriter& Wr, const clang::FunctionDecl& Func, std::vector<llvm::StringRef> DecoNames);