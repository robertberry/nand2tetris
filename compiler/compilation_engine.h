#ifndef COMPILER_COMPILATION_ENGINE_H_
#define COMPILER_COMPILATION_ENGINE_H_

#include <iostream>

#include "compiler/jack_tokenizer.h"
#include "compiler/xml_writer.h"

namespace jack {

class CompilationEngine final {
 public:
  CompilationEngine(JackTokenizer& tokenizer, std::ostream& output) : 
      tokenizer_(tokenizer), xml_writer_(output) {}

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

  XmlWriter xml_writer_;

  void ExpectKeyWord(KeyWord keyWord);

  void ExpectSymbol(char symbol);

  void ExpectIntConst();

  void ExpectStringConst();

  void ExpectIdentifier();

  void CompileVarName();

  void CompileSubroutineName();

  void CompileSubroutineCall();

  void CompileType();

  std::string AssertIdentifier();

  KeyWord AssertKeyWord();

  bool IsClassVarDecKeyWord(KeyWord key_word);

  static bool IsOp(char ch);

  static std::string EscapeForXml(char ch);
};

}  // namespace jack

#endif  // COMPILER_COMPILATION_ENGINE_H_
