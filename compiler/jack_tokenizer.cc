#include "compiler/jack_tokenizer.h"

#include <string>

namespace jack {

bool JackTokenizer::HasMoreTokens() {
  SkipWhitespaceAndComments();
  return input_.peek() != EOF;
}

void JackTokenizer::Advance() {
  // TODO
}

TokenType JackTokenizer::GetTokenType() {
  // TODO
  return TokenType::kKeyword;
}

KeyWord JackTokenizer::GetKeyWord() {
  // TODO
  return KeyWord::kClass;
}

char JackTokenizer::GetSymbol() {
  // TODO
  return '<';
}

std::string JackTokenizer::GetIdentifier() {
  // TODO
  return "";
}

int JackTokenizer::GetIntVal() {
  // TODO
  return 0;
}

std::string JackTokenizer::GetStringVal() {
  // TODO
  return "";
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

}  // namespace jack
