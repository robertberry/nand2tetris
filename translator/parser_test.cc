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

TEST(ParserTest, HasMoreLinesOnlyCommentIsFalse) {
  std::istringstream input(R"pcode(
// This is a comment
)pcode");
  Parser p(input);

  EXPECT_FALSE(p.HasMoreLines());
}

TEST(ParserTest, HasMoreLinesNonEmptyStringIsTrue) {
  std::istringstream input(R"pcode(
push constant 2145
)pcode");
  Parser p(input);

  EXPECT_TRUE(p.HasMoreLines());
}

}  // namespace
}  // namespace translator
