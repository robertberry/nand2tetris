#include "translator/parser.h"

#include <ctype.h>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

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
  {"if-goto", CommandType::kCIf},
  {"return", CommandType::kCReturn},
  {"label", CommandType::kCLabel},
  {"function", CommandType::kCFunction},
  {"call", CommandType::kCCall},
};

constexpr std::string_view kMemorySegments[] = {
  "argument",
  "local",
  "static",
  "constant",
  "this",
  "that",
  "pointer",
  "temp"
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
    case CommandType::kCPop: {
      std::string memory_segment = ExpectWord("Expected memory segment");
      if (!IsMemorySegment(memory_segment)) {
        ReportError("Not a memory segment name");
      }
      current_instruction_.arg1 = std::move(memory_segment);
      current_instruction_.arg2 = ExpectNumber("Expected memory offset");
      break;
    }

    case CommandType::kCReturn: {
      // No args.
      break;
    };

    case CommandType::kCCall:
    case CommandType::kCLabel:
    case CommandType::kCIf:
    case CommandType::kCGoto: {
      current_instruction_.arg1 = ExpectWord("Expected label");
      break;
    }

    case CommandType::kCFunction: {
      current_instruction_.arg1 = ExpectWord("Expected function name");
      break;
    }

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
  bool inside_comment = false;
  while ((ch = input_.get()) != EOF) {
    if (ch == '\n') {
      return;
    }
    if (!inside_comment && ch == '/' && input_.peek() == '/') {
      inside_comment = true;
    }
    if (!inside_comment && !isspace(ch)) {
      ReportError(error_message);
    }
  }
  // Consider EOF to be equivalent to end of line, so do not report error.
}

bool Parser::IsMemorySegment(std::string_view segment_name) {
  for (std::string_view segment : kMemorySegments) {
    if (segment_name == segment) {
      return true;
    }
  }
  return false;
}

void Parser::ReportError(std::string_view error_message) {
  std::cerr << error_message << std::endl;
  exit(1);
}

}  // namespace translator
