#pragma once

#include <cassert>
#include <memory>
#include <string>

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/CompilerInvocation.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "clang/CodeGen/CodeGenAction.h"

#include "llvm/Support/MemoryBuffer.h"

template <typename IT>
inline void compile(clang::CompilerInstance* CI,
                    const std::string& FileName,
                    IT FileBegin,
                    IT FileEnd)
{
  auto& CodeGenOpts = CI->getCodeGenOpts();
  auto& Target = CI->getTarget();
  auto& Diagnostics =  CI->getDiagnostics();

  // create new compiler instance
  auto NewCInvocation = std::make_shared<clang::CompilerInvocation>();

  const bool NewCICreated = clang::CompilerInvocation::CreateFromArgs(*NewCInvocation, CodeGenOpts.CommandLineArgs, Diagnostics);

  assert(NewCICreated);

  clang::CompilerInstance CINew;
  CINew.setInvocation(NewCInvocation);
  CINew.setTarget(&Target);
  CINew.createDiagnostics();

  auto FileContent  = std::string(FileBegin, FileEnd);
  auto FileMemoryBuffer = llvm::MemoryBuffer::getMemBuffer(FileContent);

  // create "virtual" input file
  auto& PreprocessorOpts = CINew.getPreprocessorOpts();
  PreprocessorOpts.addRemappedFile(FileName, FileMemoryBuffer.get());

  clang::EmitObjAction EmitObj;
  CINew.ExecuteAction(EmitObj);

  FileMemoryBuffer.release();
}