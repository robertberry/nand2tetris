#include "compiler/compilation_engine.h"

#include <iostream>

namespace jack {

constexpr char kOps[] = {
  '+', '-', '=', '>', '<'
};

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
  ExpectKeyWord(KeyWord::kLet);
  xml_writer_.OpenTag("letStatement");
  xml_writer_.OpenTag("varName");
  ExpectIdentifier();
  xml_writer_.CloseTag();

  // TODO: Support array indexing.
  ExpectSymbol('=');
  CompileExpression();
  ExpectSymbol(';');
  xml_writer_.CloseTag();
}

void CompilationEngine::CompileIf() {
  // TODO
}

void CompilationEngine::CompileWhile() {
  ExpectKeyWord(KeyWord::kWhile);
  xml_writer_.OpenTag("whileStatement");
  ExpectSymbol('(');
  CompileExpression();
  ExpectSymbol(')');
  ExpectSymbol('{');
  CompileStatements();
  ExpectSymbol('}');
  xml_writer_.CloseTag();
}

void CompilationEngine::CompileDo() {
  // TODO
}

void CompilationEngine::CompileReturn() {
  // TODO
}

void CompilationEngine::CompileExpression() {
  xml_writer_.OpenTag("expression");
  CompileTerm();
  if (tokenizer_.GetTokenType() == TokenType::kSymbol && 
      IsOp(tokenizer_.GetSymbol())) {
    ExpectSymbol(tokenizer_.GetSymbol());
    CompileTerm();
  }
  xml_writer_.CloseTag();
}

void CompilationEngine::CompileTerm() {
  xml_writer_.OpenTag("term");
  switch (tokenizer_.GetTokenType()) {
    case TokenType::kIntConst: {
      ExpectIntConst();
      break;
    }

    case TokenType::kStringConst: {
      ExpectStringConst();
      break;
    }

    case TokenType::kIdentifier: {
      ExpectIdentifier();
      break;
    }

    default: {
      std::cerr << "Expected constant or var name: " << tokenizer_.GetSymbol() << std::endl;
      exit(1);
    }
  }
  xml_writer_.CloseTag();
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
  std::string symbol_string(1, symbol);
  xml_writer_.AddTagWithContent("symbol", symbol_string);
  tokenizer_.Advance();
}

void CompilationEngine::ExpectIntConst() {
  if (tokenizer_.GetTokenType() != TokenType::kIntConst) {
    std::cerr << "Expected int const" << std::endl;
    exit(1);
  }
  xml_writer_.AddTagWithContent(
      "intConstant",
      std::to_string(tokenizer_.GetIntVal()));
  tokenizer_.Advance();
}

void CompilationEngine::ExpectStringConst() {
  if (tokenizer_.GetTokenType() != TokenType::kStringConst) {
    std::cerr << "Expected string const" << std::endl;
    exit(1);
  }
  xml_writer_.AddTagWithContent("stringConstant", tokenizer_.GetStringVal());
  tokenizer_.Advance();
}

void CompilationEngine::ExpectIdentifier() {
  if (tokenizer_.GetTokenType() != TokenType::kIdentifier) {
    std::cerr << "Expected identifier" << std::endl;
    exit(1);
  }
  xml_writer_.AddTagWithContent("identifier", tokenizer_.GetIdentifier());
  tokenizer_.Advance();
}

bool CompilationEngine::IsOp(char ch) {
  for (char op : kOps) {
    if (op == ch) {
      return true;
    }
  }
  return false;
}

}  // namespace jack
