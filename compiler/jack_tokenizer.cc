#include "compiler/jack_tokenizer.h"

#include <ctype.h>
#include <optional>
#include <string>
#include <string_view>
#include <sstream>
#include <utility>

namespace jack {

// Valid symbols for use in a Jack program.
constexpr char kValidSymbols[] = {
  '>', '=', '<', '+', '-', '{', '}', '(', ')'
};

constexpr std::pair<std::string_view, KeyWord> kKeyWordTable[] = {
  {"class", KeyWord::kClass},
  {"method", KeyWord::kMethod},
  {"function", KeyWord::kFunction},
  {"constructor", KeyWord::kConstructor},
  {"int", KeyWord::kInt},
  {"bool", KeyWord::kBoolean},
  {"char", KeyWord::kChar},
  {"void", KeyWord::kVoid},
  {"var", KeyWord::kVar},
  {"static", KeyWord::kStatic},
  {"field", KeyWord::kField},
  {"let", KeyWord::kLet},
  {"do", KeyWord::kDo},
  {"if", KeyWord::kIf},
  {"else", KeyWord::kElse},
  {"while", KeyWord::kWhile},
  {"return", KeyWord::kReturn},
  {"true", KeyWord::kTrue},
  {"false", KeyWord::kFalse},
  {"null", KeyWord::kNull},
  {"this", KeyWord::kThis}
};

bool JackTokenizer::HasMoreTokens() {
  SkipWhitespaceAndComments();
  return input_.peek() != EOF;
}

void JackTokenizer::Advance() {
  SkipWhitespaceAndComments();

  int next_char = input_.peek();
  if (next_char == EOF) {
    // Should not happen as Advance should not be called if HasMoreTokens
    // returns false.
    return;
  }

  if (isdigit(next_char)) {
    token_type_ = TokenType::kIntConst;
    input_ >> int_val_;
  } else if (next_char == '"') {
    token_type_ = TokenType::kStringConst;
    string_val_ = ExpectStringConst();
  } else if (IsSymbol(next_char)) {
    token_type_ = TokenType::kSymbol;
    symbol_ = (char) next_char;
  } else {
    std::string bare_word = ExpectBareWord();
    std::optional<KeyWord> key_word = GetKeyWord(bare_word);
    if (key_word) {
      token_type_ = TokenType::kKeyWord;
      key_word_ = *key_word;
    } else {
      token_type_ = TokenType::kIdentifier;
      identifier_ = bare_word;
    }
  }
}

TokenType JackTokenizer::GetTokenType() {
  return token_type_;
}

KeyWord JackTokenizer::GetKeyWord() {
  return key_word_;
}

char JackTokenizer::GetSymbol() {
  return symbol_;
}

std::string JackTokenizer::GetIdentifier() {
  return identifier_;
}

int JackTokenizer::GetIntVal() {
  return int_val_;
}

std::string JackTokenizer::GetStringVal() {
  return string_val_;
}

namespace {

enum class SkipState {
  kOutsideComment,
  kInsideLineComment,
  kInsideMultiLineComment,
  kDone
};

}  // namespace

void JackTokenizer::SkipWhitespaceAndComments() {
  int ch;
  SkipState state = SkipState::kOutsideComment;
  while (state != SkipState::kDone && (ch = input_.get()) != EOF) {
    switch (state) {
      case SkipState::kOutsideComment: {
        if (ch == '/') {
          int next = input_.get();
          if (next == '/') {
            state = SkipState::kInsideLineComment;
          } else if (next == '*') {
            state = SkipState::kInsideMultiLineComment;
          } else {
            input_.unget();
            state = SkipState::kDone;
          }
        } else if (!isspace(ch)) {
          state = SkipState::kDone;
        }
        break;
      }
      
      case SkipState::kInsideLineComment: {
        if (ch == '\n') {
          state = SkipState::kOutsideComment;
        }
        break;
      }

      case SkipState::kInsideMultiLineComment: {
        if (ch == '*' && input_.peek() == '/') {
          input_.get();
          state = SkipState::kOutsideComment;
        }
        break;
      }

      case SkipState::kDone: {
        // should be unreachable.
        break;
      }
    }
  }

  // Unconsume the last thing that was not a space or part of a comment.
  input_.unget();
}

bool JackTokenizer::IsSymbol(char ch) {
  for (char valid_symbol : kValidSymbols) {
    if (ch == valid_symbol) {
      return true;
    }
  }
  return false;
}

std::string JackTokenizer::ExpectStringConst() {
  ExpectChar('"');
  std::ostringstream str_const;
  int ch;

  while ((ch = input_.get()) != EOF) {
    if (ch == '"') {
      return str_const.str();
    }
    str_const << ((char) ch);
  }

  // TODO: Better error handling.
  std::cerr << "Expected string const but got EOF" << std::endl;
  exit(1);
}

void JackTokenizer::ExpectChar(char expected) {
  int ch = input_.get();

  // TODO: Better error handling.
  if (ch != expected) {
    std::cerr << "Expected " << expected;
    if (ch == EOF) {
      std::cerr << " but got EOF";
    } else {
      std::cerr << " but got " << ((char) ch);
    }
    std::cerr << std::endl;
    exit(1);
  }
}

std::string JackTokenizer::ExpectBareWord() {
  std::ostringstream bare_word;
  int ch;
  while ((ch = input_.get()) != EOF) {
    
    // TODO: Update to only accept chars that can be used in identifiers.
    if (isspace(ch)) {
      break;
    }
    bare_word << ((char) ch);
  }
  return bare_word.str();
}

std::optional<KeyWord> JackTokenizer::GetKeyWord(std::string_view identifier) {
  for (std::pair<std::string_view, KeyWord> entry : kKeyWordTable) {
    if (identifier == entry.first) {
      return entry.second;
    }
  }
  return {};
}

}  // namespace jack
