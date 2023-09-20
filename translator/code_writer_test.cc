#include "translator/code_writer.h"

#include <sstream>
#include <gtest/gtest.h>

namespace translator {
namespace {

constexpr std::string_view kStaticName = "Foo";

TEST(CodeWriterTest, PushConstant) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  code_writer.WritePush("constant", 42);

  EXPECT_EQ(output.str(), R"asm(// Push 42 onto the stack
@42
D=A
@SP
A=M
M=D
@SP
M=M+1

)asm");
}

TEST(CodeWriterTest, PushThis) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  code_writer.WritePush("this", 16);

  EXPECT_EQ(output.str(), R"asm(// Push this[16] onto the stack
@THIS
D=M
@16
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

)asm");
}

TEST(CodeWriterTest, PushThat) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  code_writer.WritePush("that", 32);

  EXPECT_EQ(output.str(), R"asm(// Push that[32] onto the stack
@THAT
D=M
@32
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

)asm");
}

TEST(CodeWriterTest, PushArgument) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  code_writer.WritePush("argument", 0);

  EXPECT_EQ(output.str(), R"asm(// Push argument[0] onto the stack
@ARG
A=M
D=M
@SP
A=M
M=D
@SP
M=M+1

)asm");
}

TEST(CodeWriterTest, PushLocal) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  code_writer.WritePush("local", 2);

  EXPECT_EQ(output.str(), R"asm(// Push local[2] onto the stack
@LCL
D=M
@2
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

)asm");
}

TEST(CodeWriterTest, PushPointerThis) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  // pointer[0] is this.
  code_writer.WritePush("pointer", 0);

  EXPECT_EQ(output.str(), R"asm(// Push pointer[0] onto the stack
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1

)asm");
}

TEST(CodeWriterTest, PushPointerThat) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  // pointer[1] is that.
  code_writer.WritePush("pointer", 1);

  EXPECT_EQ(output.str(), R"asm(// Push pointer[1] onto the stack
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1

)asm");
}

TEST(CodeWriterTest, PushTemp) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  code_writer.WritePush("temp", 1);

  EXPECT_EQ(output.str(), R"asm(// Push temp[1] onto the stack
@6
D=M
@SP
A=M
M=D
@SP
M=M+1

)asm");
}

TEST(CodeWriterTest, PushStatic) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  code_writer.WritePush("static", 1);

  EXPECT_EQ(output.str(), R"asm(// Push static[1] onto the stack
@Foo.1
D=M
@SP
A=M
M=D
@SP
M=M+1

)asm");
}

TEST(CodeWriterTest, Neg) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  code_writer.WriteArithmetic("neg");

  EXPECT_EQ(output.str(), R"asm(// Negate the top of the stack.
@SP
A=M
M=-M

)asm");
}

TEST(CodeWriterTest, Add) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  code_writer.WriteArithmetic("add");

  EXPECT_EQ(output.str(), R"asm(// Add the top two elements of the stack.
@SP
A=M-1
D=M
@SP
A=M
D=D+M
@SP
M=M-1
@SP
A=M
M=D

)asm");
}

TEST(CodeWriterTest, Sub) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  code_writer.WriteArithmetic("sub");

  EXPECT_EQ(output.str(), R"asm(// Subtract the top element from the second to top element of the stack.
@SP
A=M-1
D=M
@SP
A=M
D=D-M
@SP
M=M-1
@SP
A=M
M=D

)asm");
}

TEST(CodeWriterTest, And) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  code_writer.WriteArithmetic("and");

  EXPECT_EQ(output.str(), R"asm(// Performs bit-wise and on the top two elements of the stack.
@SP
A=M-1
D=M
@SP
A=M
D=D&M
@SP
M=M-1
@SP
A=M
M=D

)asm");
}

TEST(CodeWriterTest, Or) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  code_writer.WriteArithmetic("or");

  EXPECT_EQ(output.str(), R"asm(// Performs bit-wise or on the top two elements of the stack.
@SP
A=M-1
D=M
@SP
A=M
D=D|M
@SP
M=M-1
@SP
A=M
M=D

)asm");
}

TEST(CodeWriterTest, Not) {
  std::ostringstream output;
  CodeWriter code_writer(kStaticName, output);

  code_writer.WriteArithmetic("not");

  EXPECT_EQ(output.str(), R"asm(// Performs bit-wise not on the top element of the stack.
@SP
A=M
M=!M

)asm");
}

}  // namespace
}  // namespace translator
