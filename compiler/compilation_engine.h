#ifndef COMPILER_COMPILATION_ENGINE_H_
#define COMPILER_COMPILATION_ENGINE_H_

#include <iostream>

#include "compiler/jack_tokenizer.h"

namespace jack {

class CompilationEngine final {
 public:
  CompilationEngine(JackTokenizer& tokenizer, std::ostream& output) : 
      tokenizer_(tokenizer), output_(output) {}

  void CompileClass();

  void CompileClassVarDec();

  void CompileSubroutine();

  void CompileParameterList();

  void CompileSubroutineBody();

  void CompileVarDec();

  void CompileStatements();

  void CompileLet();

  void CompileIf();

  void CompileWhile();

  void CompileDo();

  void CompileReturn();

  void CompileExpression();

  void CompileTerm();

  int CompileExpressionList();

 private:
  JackTokenizer& tokenizer_;

  std::ostream& output_;

  void ExpectKeyWord(KeyWord keyWord);

  void ExpectSymbol(char symbol);
};

}  // namespace jack

#endif  // COMPILER_COMPILATION_ENGINE_H_
