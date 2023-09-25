#include "compiler/jack_tokenizer.h"

#include <ctype.h>
#include <string>

namespace jack {

// Valid symbols for use in a Jack program.
constexpr char kValidSymbols[] = {
  '>', '=', '<', '+', '-'
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
    // Consume string const.
    // TODO
  } else if (IsSymbol(next_char)) {
    token_type_ = TokenType::kSymbol;
    symbol_ = (char) next_char;
  } else {
    // Get word, check if it is keyWord.
    // TODO
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

}  // namespace jack
