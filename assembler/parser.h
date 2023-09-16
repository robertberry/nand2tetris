#ifndef ASSEMBLER_PARSER_H_
#define ASSEMBLER_PARSER_H_

#include <istream>
#include <string>

namespace hack {

enum class InstructionType {
  kAInstruction,
  kCInstruction,
  kLInstruction
};

struct Instruction {
  InstructionType instruction_type;

  // If the current instruction is (xxx), returns xxx. If the current
  // instruction is @xxx, returns xxx. Should only be called for A instructions
  // and labels.
  std::string symbol;

  // Returns the symbol destination part of the current C-instruction. Should
  // be called only if InstructionType is a C instruction.
  std::string destination;

  // Returns the symbolic comparison part of the current C-instruction. Should
  // only be called if InstructionType is a C instruction.  
  std::string comparison;

  // Returns the symbolic jump part of the current C-instruction. Should only be
  // called if InstructionType is a C instruction.
  std::string jump;
};

// Parses a .asm file.
class Parser final {
 public:
  explicit Parser(std::istream& input_stream) : input_stream_(input_stream) {}

  // Skips over whitespace and comments as necessary. Reads the next instruction
  // from the input and makes it the current instruction.
  void Advance();

  // Returns true if there are more lines in the input.
  bool HasMoreLines();

  // Gets the current instruction. Should only be called after Advance() has
  // been invoked at least once.
  Instruction CurrentInstruction() {
    return current_instruction_;
  }

 private:
  std::istream& input_stream_;

  Instruction current_instruction_;

  void SkipWhitespaceAndComments();

  std::string ConsumeSymbolOrConstant();

  std::string ConsumeSymbol();

  std::string ConsumeConstant();

  void ConsumeRestOfLine();

  static bool IsSymbolChar(char ch);

  static std::string Trim(char* start, char* end);
};

}  // namespace hack

#endif  // ASSEMBLER_PARSER_H_
