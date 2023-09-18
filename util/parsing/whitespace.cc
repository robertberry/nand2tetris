#include "util/parsing/whitespace.h"

#include <ctype.h>
#include <iostream>

namespace util_parsing {

void SkipWhitespaceAndComments(std::istream& input) {
  int ch;
  bool consuming_line = false;
  while ((ch = input.get()) != EOF) {
    if (consuming_line && ch == '\n') {
      consuming_line = false;
      continue;
    }
    if (consuming_line || isspace(ch)) {
      continue;
    }
    if (ch == '/' && input.peek() == '/') {
      consuming_line = true;
      continue;
    }
    break;
  }
  input.unget();
}

}  // namespace util_parsing
