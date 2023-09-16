#include "assembler/parser.h"

#include <sstream>
#include <gtest/gtest.h>

namespace hack {
namespace {

TEST(ParserTest, AInstruction) {
  std::istringstream input(R"asm(
@12345
)asm");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.instruction_type, InstructionType::kAInstruction);
  EXPECT_EQ(instruction.symbol, "12345");
}

TEST(ParserTest, LInstruction) {
  std::istringstream input(R"asm(
(LOOP)
)asm");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.instruction_type, InstructionType::kLInstruction);
  EXPECT_EQ(instruction.symbol, "LOOP");
}

TEST(ParserTest, CInstruction) {
  std::istringstream input(R"asm(
D=M+1;JNE
)asm");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.instruction_type, InstructionType::kCInstruction);
  ASSERT_EQ(instruction.destination, "D");
  ASSERT_EQ(instruction.comparison, "M+1");
  ASSERT_EQ(instruction.jump, "JNE");
}

TEST(ParserTest, CInstructionTrimsWhitespace) {
  std::istringstream input(R"asm(
 D = M + 1 ; JNE
)asm");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.instruction_type, InstructionType::kCInstruction);
  ASSERT_EQ(instruction.destination, "D");
  ASSERT_EQ(instruction.comparison, "M + 1");
  ASSERT_EQ(instruction.jump, "JNE");
}

TEST(ParserTest, HasMoreLinesEmptyStringIsFalse) {
  std::istringstream input("");
  Parser p(input);

  EXPECT_FALSE(p.HasMoreLines());
}

TEST(ParserTest, HasMoreLinesSingleLineInitiallyIsTrue) {
  std::istringstream input(R"asm(
D=M+1;JNE
)asm");
  Parser p(input);

  EXPECT_TRUE(p.HasMoreLines());
}

TEST(ParserTest, HasMoreLinesSingleLineAfterAdvanceIsFalse) {
  std::istringstream input(R"asm(
D=M+1;JNE
)asm");
  Parser p(input);
  p.Advance();

  EXPECT_FALSE(p.HasMoreLines());
}

TEST(ParserTest, HasMoreLinesMultiLinesAfterAdvanceIsTrue) {
  std::istringstream input(R"asm(
(LOOP)
D=M+1;JNE
)asm");
  Parser p(input);
  p.Advance();

  EXPECT_TRUE(p.HasMoreLines());
}

TEST(ParserTest, HasMoreLinesMultiLinesAfterAdvanceToEndIsFalse) {
  std::istringstream input(R"asm(
(LOOP)
// Test
D=M+1;JNE
// Done
)asm");
  Parser p(input);
  p.Advance();
  p.Advance();

  EXPECT_FALSE(p.HasMoreLines());
}

TEST(ParserTest, AdvanceSkipsComments) {
  std::istringstream input(R"asm(
// This is where the loop
// starts.
 // ok!
(LOOP)
)asm");
  Parser p(input);

  p.Advance();

  Instruction instruction = p.CurrentInstruction();
  EXPECT_EQ(instruction.instruction_type, InstructionType::kLInstruction);
  EXPECT_EQ(instruction.symbol, "LOOP");
}

}  // namespace
}  // namespace hack
