#include "translator/parser.h"

#include "util/parsing/whitespace.h"

namespace translator {

using ::util_parsing::SkipWhitespaceAndComments;

bool Parser::HasMoreLines() {
  SkipWhitespaceAndComments(input_);
  return input_.peek() != EOF;
}

void Parser::Advance() {
  // TODO
}

}  // namespace translator
