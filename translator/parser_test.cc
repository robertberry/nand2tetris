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

TEST(ParserTest, AdvanceMovesToFirstInstruction) {
  std::istringstream input(R"pcode(
push constant 2145
)pcode");
  Parser p(input);

  p.Advance();

  EXPECT_EQ(p.CurrentInstruction().command_type, CommandType::kCPush);
}

TEST(ParserTest, AdvanceInvokedTwiceMovesToSecondInstruction) {
  std::istringstream input(R"pcode(
push constant 256
pop local 32
)pcode");
  Parser p(input);

  p.Advance();
  p.Advance();

  EXPECT_EQ(p.CurrentInstruction().command_type, CommandType::kCPop);
}

TEST(ParserTest, Arg1ArithmeticIsCommandName) {
  std::string command = "sub";
  std::istringstream input(command);
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.command_type, CommandType::kCArithmetic);
  EXPECT_EQ(instruction.arg1, command);
}

}  // namespace
}  // namespace translator
