#include "compiler/jack_tokenizer.h"

#include <sstream>
#include <gtest/gtest.h>

namespace jack {
namespace {

TEST(JackTokenizerTest, HasMoreTokensEmptyStringIsFalse) {
  std::istringstream input("");
  JackTokenizer tokenizer(input);

  EXPECT_FALSE(tokenizer.HasMoreTokens());
}

TEST(JackTokenizerTest, HasMoreTokensSpacesOnlyStringIsFalse) {
  std::istringstream input(R"jack(  

  )jack");
  JackTokenizer tokenizer(input);

  EXPECT_FALSE(tokenizer.HasMoreTokens());
}

TEST(JackTokenizerTest, HasMoreTokensSingleLineCommentOnlyIsFalse) {
  std::istringstream input("// Bonchon is a fantastic doge");
  JackTokenizer tokenizer(input);

  EXPECT_FALSE(tokenizer.HasMoreTokens());
}

TEST(JackTokenizerTest, HasMoreTokensMultiLineCommentOnlyIsFalse) {
  std::istringstream input(R"jack(
/*****************
 * B O N C H O N *
 *****************/

)jack");
  JackTokenizer tokenizer(input);

  EXPECT_FALSE(tokenizer.HasMoreTokens());
}

TEST(JackTokenizerTest, HasMoreTokensATokenIsTrue) {
  std::istringstream input(R"jack(
/*****************
 * B O N C H O N *
 *****************/

int
)jack");
  JackTokenizer tokenizer(input);

  EXPECT_TRUE(tokenizer.HasMoreTokens());
}

}  // namespace
}  // namespace jack
