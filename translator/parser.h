#ifndef TRANSLATOR_PARSER_H_
#define TRANSLATOR_PARSER_H_

#include <optional>
#include <iostream>
#include <string>
#include <string_view>

namespace translator {

// The type of command of the p-instruction.
enum class CommandType {
  kCArithmetic,
  kCPush,
  kCPop,
  kCLabel,
  kCGoto,
  kCIf,
  kCFunction,
  kCReturn,
  kCCall
};

// A VM p-code instruction.
struct Instruction {
  CommandType command_type;

  std::string arg1;

  int arg2;
};

// Parses VM p-code.
class Parser {
 public:
  explicit Parser(std::istream& istream) : input_(istream) {}
  
  // Returns true if the file has more lines to process.
  bool HasMoreLines();

  // Advances to the next instruction.
  void Advance();

  // Returns a reference to the current instruction. This updates when Advance is
  // invoked.
  Instruction CurrentInstruction() {
    return current_instruction_;
  }

 private:
  static std::optional<CommandType> CommandTypeFromString(std::string_view s);
  
  std::istream& input_;

  Instruction current_instruction_;
};

}  // namespace translator

#endif  // TRANSLATOR_PARSER_H_
