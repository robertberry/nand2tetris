#include "translator/parser.h"

#include <optional>
#include <string_view>

#include "util/parsing/whitespace.h"

namespace translator {

using ::util_parsing::SkipWhitespaceAndComments;

// Command types by the string name of the command as used in the p-code.
constexpr std::pair<std::string_view, CommandType> kCommandTable[] = {
  {"add", CommandType::kCArithmetic},
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
  std::string command;
  input_ >> command;

  std::optional<CommandType> commandType = CommandTypeFromString(command);
  if (!commandType) {
    // TODO: better error handling
    exit(1);
  }
}

std::optional<CommandType> Parser::CommandTypeFromString(std::string_view s) {
  for (auto pair : kCommandTable) {
    if (s == pair.first) {
      return pair.second;
    }
  }
  return {};
}

}  // namespace translator
