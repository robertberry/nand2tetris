#include "compiler/jack_tokenizer.h"

#include <string>

namespace jack {

bool JackTokenizer::HasMoreTokens() {
  // TODO
  return false;
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

}  // namespace jack
