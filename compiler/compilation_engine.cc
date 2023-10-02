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
  xml_writer_.OpenTag("classVarDec");
  switch (AssertKeyWord()) {
    case KeyWord::kStatic:
      xml_writer_.AddTagWithContent("keyword", "static");
      break;

    case KeyWord::kField:
      xml_writer_.AddTagWithContent("keyword", "field");
      break;

    default: {
      std::cerr << "Expected static or field" << std::endl;
      exit(1);
    }
  }
  CompileType();

  // get the identifier list
  do {
    // hacky, to add the comma.
    if (tokenizer_.GetTokenType() == TokenType::kSymbol) {
      xml_writer_.AddTagWithContent("symbol", ",");
      tokenizer_.Advance();
    }
    xml_writer_.AddTagWithContent("identifier", AssertIdentifier());
  } while (tokenizer_.NextIsSymbol(','));

  ExpectSymbol(';');

  xml_writer_.CloseTag();
}

void CompilationEngine::CompileSubroutine() {
  xml_writer_.OpenTag("subroutineDec");
  switch (AssertKeyWord()) {
    case KeyWord::kConstructor: {
      xml_writer_.AddTagWithContent("keyword", "constructor");
      break;
    }

    case KeyWord::kFunction: {
      xml_writer_.AddTagWithContent("keyword", "function");
      break;
    }

    case KeyWord::kMethod: {
      xml_writer_.AddTagWithContent("keyword", "method");
      break;
    }

    default: {
      std::cerr << "Expected constructor|function|method" << std::endl;
      exit(1);
    }
  }
  if (tokenizer_.NextIsKeyWord(KeyWord::kVoid)) {
    xml_writer_.AddTagWithContent("keyword", "void");
    tokenizer_.Advance();
  } else {
    CompileType();
  }
  xml_writer_.AddTagWithContent("subroutineName", AssertIdentifier());
  CompileParameterList();
  CompileSubroutineBody();
  xml_writer_.CloseTag();
}

void CompilationEngine::CompileParameterList() {
  xml_writer_.OpenTag("parameterList");
  ExpectSymbol('(');
  if (!tokenizer_.NextIsSymbol(')')) {
    bool more = true;
    do {
      CompileType();
      xml_writer_.AddTagWithContent("identifier", AssertIdentifier());
      more = tokenizer_.NextIsSymbol(',');;
      if (more) {
        ExpectSymbol(',');
      }
    } while (more);
  }
  ExpectSymbol(')');
  xml_writer_.CloseTag();
}

void CompilationEngine::CompileSubroutineBody() {
  xml_writer_.OpenTag("subroutineBody");
  ExpectSymbol('{');
  while (tokenizer_.NextIsKeyWord(KeyWord::kVar)) {
    CompileVarDec();
  }
  CompileStatements();
  ExpectSymbol('}');
  xml_writer_.CloseTag();
}

void CompilationEngine::CompileVarDec() {
  ExpectKeyWord(KeyWord::kVar);
  xml_writer_.OpenTag("varDec");
  CompileType();

  bool more = true;
  do {
    xml_writer_.AddTagWithContent("varName", AssertIdentifier());
    more = tokenizer_.NextIsSymbol(',');
    if (more) {
      ExpectSymbol(',');
    }
  } while (more);

  ExpectSymbol(';');
  xml_writer_.CloseTag();
}

void CompilationEngine::CompileStatements() {
  xml_writer_.OpenTag("statements");
  while (tokenizer_.GetTokenType() == TokenType::kKeyWord) {
    switch (tokenizer_.GetKeyWord()) {
      case KeyWord::kLet: {
        CompileLet();
        break;
      }
      case KeyWord::kDo: {
        CompileDo();
        break;
      }
      case KeyWord::kIf: {
        CompileIf();
        break;
      }
      case KeyWord::kWhile: {
        CompileWhile();
        break;
      }
      case KeyWord::kReturn: {
        CompileReturn();
        break;
      }
      default: {
        xml_writer_.CloseTag();
        return;
      }
    }
  }
  xml_writer_.CloseTag();
}

void CompilationEngine::CompileLet() {
  ExpectKeyWord(KeyWord::kLet);
  xml_writer_.OpenTag("letStatement");
  CompileVarName();

  // TODO: Support array indexing.
  ExpectSymbol('=');
  CompileExpression();
  ExpectSymbol(';');
  xml_writer_.CloseTag();
}

void CompilationEngine::CompileIf() {
  ExpectKeyWord(KeyWord::kIf);
  xml_writer_.OpenTag("ifStatement");
  ExpectSymbol('(');
  CompileExpression();
  ExpectSymbol(')');
  ExpectSymbol('{');
  CompileStatements();
  ExpectSymbol('}');
  xml_writer_.CloseTag();
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
  ExpectKeyWord(KeyWord::kDo);
  xml_writer_.OpenTag("doStatement");
  CompileSubroutineCall();
  ExpectSymbol(';');
  xml_writer_.CloseTag();
}

void CompilationEngine::CompileReturn() {
  ExpectKeyWord(KeyWord::kReturn);
  xml_writer_.OpenTag("returnStatement");
  if (!tokenizer_.NextIsSymbol(';')) {
    CompileExpression();
  }
  ExpectSymbol(';');
  xml_writer_.CloseTag();
}

void CompilationEngine::CompileExpression() {
  xml_writer_.OpenTag("expression");
  CompileTerm();
  if (tokenizer_.GetTokenType() == TokenType::kSymbol && 
      IsOp(tokenizer_.GetSymbol())) {
    xml_writer_.AddTagWithContent("op", EscapeForXml(tokenizer_.GetSymbol()));
    tokenizer_.Advance();
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
      CompileVarName();
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
  xml_writer_.OpenTag("expressionList");
  if (tokenizer_.NextIsSymbol(')')) {
    // zero-length expression list.
    xml_writer_.CloseTag();
    return 0;
  }
  
  CompileExpression();
  int size = 1;
  while (tokenizer_.NextIsSymbol(',')) {
    ExpectSymbol(',');
    CompileExpression();
    size++;
  }
  xml_writer_.CloseTag();
  return size;
}

void CompilationEngine::ExpectKeyWord(KeyWord key_word) {
  if (!tokenizer_.NextIsKeyWord(key_word)) {
    std::cerr << "Expected key word " << key_word << std::endl;
    exit(1);
  }
  tokenizer_.Advance();
}

void CompilationEngine::ExpectSymbol(char symbol) {
  if (!tokenizer_.NextIsSymbol(symbol)) {
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
      "intConst",
      std::to_string(tokenizer_.GetIntVal()));
  tokenizer_.Advance();
}

void CompilationEngine::ExpectStringConst() {
  if (tokenizer_.GetTokenType() != TokenType::kStringConst) {
    std::cerr << "Expected string const" << std::endl;
    exit(1);
  }
  xml_writer_.AddTagWithContent("stringConst", tokenizer_.GetStringVal());
  tokenizer_.Advance();
}

void CompilationEngine::ExpectIdentifier() {
  xml_writer_.AddTagWithContent("identifier", AssertIdentifier());
}

void CompilationEngine::CompileVarName() {
  xml_writer_.AddTagWithContent("varName", AssertIdentifier());
}

void CompilationEngine::CompileSubroutineName() {
  xml_writer_.AddTagWithContent("subroutineName", AssertIdentifier());
}

void CompilationEngine::CompileSubroutineCall() {
  xml_writer_.OpenTag("subroutineCall");
  CompileSubroutineName();
  ExpectSymbol('(');
  CompileExpressionList();
  ExpectSymbol(')');
  xml_writer_.CloseTag();
  // TODO: also support method calls.
}

bool CompilationEngine::IsOp(char ch) {
  for (char op : kOps) {
    if (op == ch) {
      return true;
    }
  }
  return false;
}

std::string CompilationEngine::EscapeForXml(char ch) {
  switch (ch) {
    case '>':
      return "&gt;";
    case '<':
      return "&lt;";
    case '&':
      return "&amp;";
    default:
      std::string s(1, ch);
      return s;
  }
}

void CompilationEngine::CompileType() {
  switch (tokenizer_.GetTokenType()) {
    case TokenType::kKeyWord: {
      switch (tokenizer_.GetKeyWord()) {
        case KeyWord::kInt: {
          xml_writer_.AddTagWithContent("keyword", "int");
          break;
        }

        case KeyWord::kChar: {
          xml_writer_.AddTagWithContent("keyword", "char");
          break;
        }

        case KeyWord::kBoolean: {
          xml_writer_.AddTagWithContent("keyword", "boolean");
          break;
        }

        default: {
          std::cerr << "Expected int, char or boolean" << std::endl;
          break;
        }
      }
      break;
    }

    case TokenType::kIdentifier: {
      xml_writer_.AddTagWithContent("className", tokenizer_.GetIdentifier());
      break;
    }

    default: {
      std::cerr << "Expected int, char, boolean or class name" << std::endl;
      exit(1);
    }
  }
  tokenizer_.Advance();
}

std::string CompilationEngine::AssertIdentifier() {
  if (tokenizer_.GetTokenType() != TokenType::kIdentifier) {
    std::cerr << "Expected identifier" << std::endl;
    exit(1);
  }
  std::string identifier = tokenizer_.GetIdentifier();
  tokenizer_.Advance();
  return identifier;
}

KeyWord CompilationEngine::AssertKeyWord() {
  if (tokenizer_.GetTokenType() != TokenType::kKeyWord) {
    std::cerr << "Expected keyword" << std::endl;
    exit(1);
  }
  KeyWord key_word = tokenizer_.GetKeyWord();
  tokenizer_.Advance();
  return key_word;
}

}  // namespace jack
