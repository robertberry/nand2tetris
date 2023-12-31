#ifndef COMPILER_JACK_TOKENIZER_H_
#define COMPILER_JACK_TOKENIZER_H_

#include <iostream>
#include <optional>
#include <string>

namespace jack {

enum class TokenType {
  kKeyWord,
  kSymbol,
  kIdentifier,
  kIntConst,
  kStringConst
};

enum class KeyWord {
  kClass,
  kMethod,
  kFunction,
  kConstructor,
  kInt,
  kBoolean,
  kChar,
  kVoid,
  kVar,
  kStatic,
  kField,
  kLet,
  kDo,
  kIf,
  kElse,
  kWhile,
  kReturn,
  kTrue,
  kFalse,
  kNull,
  kThis
};

std::ostream& operator<<(std::ostream& out, const KeyWord key_word);

class JackTokenizer final {
 public:
  explicit JackTokenizer(std::istream& input) : input_(input) {}

  // Returns true if there are more tokens in the input.
  bool HasMoreTokens();

  // Advances to the next token.
  void Advance();

  // Returns the type of the current token.
  TokenType GetTokenType();
  
  // If GetTokenType returns kKeyword, returns the type of the key word.
  KeyWord GetKeyWord();

  // If GetTokenType returns kSymbol, returns the symbol.
  char GetSymbol();

  // If GetTokenType returns kIdentifier, returns the identifier.
  std::string GetIdentifier();

  // If GetTokenType returns kIntConst, returns the integer value of it.
  int GetIntVal();

  // If GetTokenType returns kStringConst, returns the string value of it.
  std::string GetStringVal();

  bool NextIsSymbol(char symbol);

  bool NextIsKeyWord(KeyWord key_word);

 private:
  std::istream& input_;

  TokenType token_type_;

  KeyWord key_word_;

  char symbol_;

  std::string identifier_;

  int int_val_;

  std::string string_val_;

  void SkipWhitespaceAndComments();

  // Returns true if ch is a symbol used in a Jack program.
  bool IsSymbol(char ch);

  // Returns true if ch is a valid character for use in an identifier.
  // Note: although digits are valid characters for an identifier, the
  // first character must not be a digit, so this should be separately
  // checked.
  bool IsIdentifierChar(char ch);

  std::string ExpectStringConst();

  void ExpectChar(char expected);

  std::string ExpectBareWord();

  std::optional<KeyWord> GetKeyWord(std::string_view identifier);
};

}  // namespace jack

#endif  // COMPILER_JACK_TOKENIZER_H_
