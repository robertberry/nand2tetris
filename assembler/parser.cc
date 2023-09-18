#include "assembler/parser.h"

#include <ctype.h>

#include "util/parsing/whitespace.h"

namespace hack {

using ::util_parsing::SkipWhitespaceAndComments;

constexpr int kMaxLineLength = 256;

namespace {

// The state of the parser loop when handling a C-instruction.
enum class CParserState {
  // Expecting the destination OR the comparison.
  kDest,

  // Expecting the comparison.
  kComp,

  // Expecting the jump.
  kJump
};

}  // namespace

void Parser::Advance() {
  SkipWhitespaceAndComments(input_stream_);

  char ch = input_stream_.get();
  if (ch == '@') {
    current_instruction_.instruction_type = InstructionType::kAInstruction;
    current_instruction_.symbol = ConsumeSymbolOrConstant();
    ConsumeRestOfLine();
    return;
  } else if (ch == '(') {
    current_instruction_.instruction_type = InstructionType::kLInstruction;
    current_instruction_.symbol = ConsumeSymbol();
    char ch = input_stream_.get();
    if (ch != ')') {
      // better error handling needed
      exit(1);
    }
    ConsumeRestOfLine();
    return;
  }
  input_stream_.unget();

  current_instruction_.instruction_type = InstructionType::kCInstruction;
  // C-instruction: more complicated parsing. First get whole line.
  char line[kMaxLineLength];
  input_stream_.getline(line, kMaxLineLength);

  current_instruction_.destination = "";
  current_instruction_.jump = "";

  CParserState state = CParserState::kDest;
  int start = 0;
  int i = 0;
  while (line[i] != '\0') {
    if (state == CParserState::kDest && line[i] == '=') {
      current_instruction_.destination = Trim(line, line + i);
      start = i + 1;
    }

    if (line[i] == ';') {
      // For error handling this would throw if already in kJump.
      current_instruction_.comparison = Trim(line + start, line + i);
      start = i + 1;
      state = CParserState::kJump;
    }
    i++;
  }

  if (state == CParserState::kJump) {
    current_instruction_.jump = Trim(line + start, line + i);
  } else {
    current_instruction_.comparison = Trim(line + start, line + i);
  }
}

bool Parser::HasMoreLines() {
  SkipWhitespaceAndComments(input_stream_);
  return input_stream_.peek() != EOF;
}

std::string Parser::ConsumeSymbolOrConstant() {
  if (isdigit(input_stream_.peek())) {
    return ConsumeConstant();
  } else {
    return ConsumeSymbol();
  }
}

std::string Parser::ConsumeSymbol() {
  std::string str;
  char ch = input_stream_.get();
  while (IsSymbolChar(ch)) {
    str += ch;
    ch = input_stream_.get();
  }
  input_stream_.unget();
  return str;
}

std::string Parser::ConsumeConstant() {
  std::string str;
  char ch = input_stream_.get();
  while (isdigit(ch)) {
    str += ch;
    ch = input_stream_.get();
  }
  input_stream_.unget();
  return str;
}

void Parser::ConsumeRestOfLine() {
  // If we were doing error handling we might also want to assert we only see
  // whitespace until the end of the line.
  char ch = input_stream_.get();
  while (ch != '\n') {
    ch = input_stream_.get();
  }
}

bool Parser::IsSymbolChar(char ch) {
  return isdigit(ch) || isalpha(ch) || ch == '_' || ch == '.' ||
    ch == '$' || ch == ':';
}

std::string Parser::Trim(char* start, char* end) {
  while (isspace(*start) && start < end) {
    start++;
  }
  while (isspace(*(end - 1)) && end > start) {
    end--;
  }
  return std::string(start, end);
}

}  // namespace hack
