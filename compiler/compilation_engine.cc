#include "compiler/compilation_engine.h"

#include <iostream>

namespace jack {

void CompilationEngine::CompileClass() {
  // TODO
}

void CompilationEngine::CompileClassVarDec() {
  // TODO
}

void CompilationEngine::CompileSubroutine() {
  // TODO
}

void CompilationEngine::CompileParameterList() {
  // TODO
}

void CompilationEngine::CompileSubroutineBody() {
  // TODO
}

void CompilationEngine::CompileVarDec() {
  // TODO
}

void CompilationEngine::CompileStatements() {
  // TODO
}

void CompilationEngine::CompileLet() {
  // TODO
}

void CompilationEngine::CompileIf() {
  // TODO
}

void CompilationEngine::CompileWhile() {
  ExpectKeyWord(KeyWord::kWhile);
  ExpectSymbol('(');
  CompileExpression();
  ExpectSymbol(')');
  ExpectSymbol('{');
  CompileStatements();
  ExpectSymbol('}');
}

void CompilationEngine::CompileDo() {
  // TODO
}

void CompilationEngine::CompileReturn() {
  // TODO
}

void CompilationEngine::CompileExpression() {
  // TODO
}

void CompilationEngine::CompileTerm() {
  // TODO
}

int CompilationEngine::CompileExpressionList() {
  // TODO
  return 0;
}

void CompilationEngine::ExpectKeyWord(KeyWord key_word) {
  if (tokenizer_.GetTokenType() != TokenType::kKeyWord ||
      tokenizer_.GetKeyWord() != key_word) {
    std::cerr << "Expected key word " << key_word << std::endl;
    exit(1);
  }
  tokenizer_.Advance();
}

void CompilationEngine::ExpectSymbol(char symbol) {
  if (tokenizer_.GetTokenType() != TokenType::kSymbol ||
      tokenizer_.GetSymbol() != symbol) {
    std::cerr << "Expected symbol " << symbol << std::endl;
    exit(1);
  }
  tokenizer_.Advance();
}

}  // namespace jack
