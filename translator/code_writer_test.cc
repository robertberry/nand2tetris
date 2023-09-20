#include "translator/code_writer.h"

#include <sstream>
#include <gtest/gtest.h>

namespace translator {
namespace {

TEST(CodeWriterTest, PushConstant) {
  std::ostringstream output;
  CodeWriter code_writer(output);

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

}  // namespace
}  // namespace translator
