#include "translator/parser.h"

#include <sstream>
#include <gtest/gtest.h>

namespace translator {
namespace {

TEST(ParserTest, HasMoreLinesEmptyStringIsFalse) {
  std::istringstream input("");
  Parser p(input);

  EXPECT_FALSE(p.HasMoreLines());
}

}  // namespace
}  // namespace translator
