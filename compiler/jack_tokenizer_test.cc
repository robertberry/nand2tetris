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

TEST(JackTokenizerTest, AdvanceIntConst) {
  std::istringstream input(R"jack(
// The answer to everything.
42
)jack");
  JackTokenizer tokenizer(input);

  tokenizer.Advance();

  EXPECT_EQ(tokenizer.GetTokenType(), TokenType::kIntConst);
  EXPECT_EQ(tokenizer.GetIntVal(), 42);
}

TEST(JackTokenizerTest, AdvanceSymbol) {
  std::istringstream input(">");
  JackTokenizer tokenizer(input);

  tokenizer.Advance();

  EXPECT_EQ(tokenizer.GetTokenType(), TokenType::kSymbol);
  EXPECT_EQ(tokenizer.GetSymbol(), '>');
}

TEST(JackTokenizerTest, AdvanceStringConst) {
  std::istringstream input(R"jack(
// The answer to everything.
"Forty two"
)jack");
  JackTokenizer tokenizer(input);

  tokenizer.Advance();

  EXPECT_EQ(tokenizer.GetTokenType(), TokenType::kStringConst);
  EXPECT_EQ(tokenizer.GetStringVal(), "Forty two");
}

TEST(JackTokenizerTest, AdvanceClass) {
  std::istringstream input(R"jack(
// The most useful object.
class Bonchon {
}
)jack");
  JackTokenizer tokenizer(input);

  tokenizer.Advance();

  EXPECT_EQ(tokenizer.GetTokenType(), TokenType::kKeyWord);
  EXPECT_EQ(tokenizer.GetKeyWord(), KeyWord::kClass);
}

TEST(JackTokenizerTest, AdvanceMethod) {
  std::istringstream input(R"jack(
/* woofs! */
method bark() {
}
)jack");
  JackTokenizer tokenizer(input);

  tokenizer.Advance();

  EXPECT_EQ(tokenizer.GetTokenType(), TokenType::kKeyWord);
  EXPECT_EQ(tokenizer.GetKeyWord(), KeyWord::kMethod);
}

TEST(JackTokenizerTest, AdvanceFunction) {
  std::istringstream input(R"jack(
/* woofs! */
function bark() {
}
)jack");
  JackTokenizer tokenizer(input);

  tokenizer.Advance();

  EXPECT_EQ(tokenizer.GetTokenType(), TokenType::kKeyWord);
  EXPECT_EQ(tokenizer.GetKeyWord(), KeyWord::kFunction);
}

}  // namespace
}  // namespace jack
