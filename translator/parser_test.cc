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

TEST(ParserTest, Arg1PushIsMemorySegment) {
  std::istringstream input("push constant 16");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.arg1, "constant");
}

TEST(ParserTest, Arg2PushIsOffset) {
  std::istringstream input("push constant 16");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.arg2, 16);
}

TEST(ParserTest, Arg1PopIsMemorySegment) {
  std::istringstream input("pop local 32");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.arg1, "local");
}

TEST(ParserTest, Arg2PopIsOffset) {
  std::istringstream input("pop local 32");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.arg2, 32);
}

TEST(ParserTest, Call) {
  std::istringstream input("call sqrt");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.command_type, CommandType::kCCall);
  EXPECT_EQ(instruction.arg1, "sqrt");
}

TEST(ParserTest, Label) {
  std::istringstream input("label WHILE_LOOP");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.command_type, CommandType::kCLabel);
  EXPECT_EQ(instruction.arg1, "WHILE_LOOP");
}

TEST(ParserTest, Goto) {
  std::istringstream input("goto LOOP");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.command_type, CommandType::kCGoto);
  EXPECT_EQ(instruction.arg1, "LOOP");
}

TEST(ParserTest, If) {
  std::istringstream input("if-goto WHILE_END");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.command_type, CommandType::kCIf);
  EXPECT_EQ(instruction.arg1, "WHILE_END");
}

TEST(ParserTest, Return) {
  std::istringstream input("return");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.command_type, CommandType::kCReturn);
}

TEST(ParserTest, Function) {
  std::istringstream input("function hypot(x,y)");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.command_type, CommandType::kCFunction);
  EXPECT_EQ(instruction.arg1, "hypot(x,y)");
}

}  // namespace
}  // namespace translator
