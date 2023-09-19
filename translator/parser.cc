#include "translator/parser.h"

#include <ctype.h>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include "util/parsing/whitespace.h"

namespace translator {

using ::util_parsing::SkipWhitespaceAndComments;

// Command types by the string name of the command as used in the p-code.
constexpr std::pair<std::string_view, CommandType> kCommandTable[] = {
  {"add", CommandType::kCArithmetic},
  {"sub", CommandType::kCArithmetic},
  {"neg", CommandType::kCArithmetic},
  {"eq", CommandType::kCArithmetic},
  {"gt", CommandType::kCArithmetic},
  {"lt", CommandType::kCArithmetic},
  {"and", CommandType::kCArithmetic},
  {"or", CommandType::kCArithmetic},
  {"not", CommandType::kCArithmetic},
  {"push", CommandType::kCPush},
  {"pop", CommandType::kCPop},
  {"goto", CommandType::kCGoto},
  {"if", CommandType::kCIf},
  {"return", CommandType::kCReturn}
};

bool Parser::HasMoreLines() {
  SkipWhitespaceAndComments(input_);
  return input_.peek() != EOF;
}

void Parser::Advance() {
  std::string command = ExpectWord("Expected command");

  std::optional<CommandType> command_type = CommandTypeFromString(command);
  if (!command_type) {
    ReportError("Invalid command");
  }

  current_instruction_.command_type = *command_type;
  switch (*command_type) {
    case CommandType::kCArithmetic:
      current_instruction_.arg1 = command;
      break;

    case CommandType::kCPush:
    case CommandType::kCPop:
      current_instruction_.arg1 = ExpectWord("Expected memory segment");
      current_instruction_.arg2 = ExpectNumber("Expected memory offset");
      break;

    default:
      ReportError("Unsupported operation");
      break;
  }

  ExpectEndOfLine("Expected end of line");
}

std::optional<CommandType> Parser::CommandTypeFromString(std::string_view s) {
  for (auto pair : kCommandTable) {
    if (s == pair.first) {
      return pair.second;
    }
  }
  return {};
}

std::string Parser::ExpectWord(std::string_view error_message) {
  std::string s;
  if (!(input_ >> s)) {
    ReportError(error_message);
  }
  return s;
}

int Parser::ExpectNumber(std::string_view error_message) {
  int n;
  if (!(input_ >> n)) {
    ReportError(error_message);
  }
  return n;
}

void Parser::ExpectEndOfLine(std::string_view error_message) {
  int ch;
  while ((ch = input_.get()) != EOF) {
    if (ch == '\n') {
      return;
    }
    if (!isspace(ch)) {
      ReportError(error_message);
    }
  }
  // Consider EOF to be equivalent to end of line, so do not report error.
}

void Parser::ReportError(std::string_view error_message) {
  std::cerr << error_message << std::endl;
  exit(1);
}

}  // namespace translator
