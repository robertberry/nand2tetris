#include "compiler/compilation_engine.h"

#include <sstream>
#include <gtest/gtest.h>

#include "compiler/jack_tokenizer.h"

namespace jack {
namespace {

TEST(CompilationEngineTest, WhileStatementToXml) {
  std::istringstream input(R"jack(
while (x) {
}
)jack");
  JackTokenizer tokenizer(input);
  tokenizer.Advance();
  std::ostringstream output;
  CompilationEngine engine(tokenizer, output);
  
  engine.CompileWhile();

  EXPECT_EQ(output.str(), R"xml(<whileStatement>
  <symbol>(</symbol>
  <expression>
    <term>
      <identifier>x</identifier>
    </term>
  </expression>
  <symbol>)</symbol>
  <symbol>{</symbol>
  <symbol>}</symbol>
</whileStatement>)xml");
}

TEST(CompilationEngineTest, LetStatementToXml) {
  std::istringstream input(R"jack(
let shiba = "bonchon";
)jack");
  JackTokenizer tokenizer(input);
  tokenizer.Advance();
  std::ostringstream output;
  CompilationEngine engine(tokenizer, output);
  
  engine.CompileLet();

  EXPECT_EQ(output.str(), R"xml(<letStatement>
  <varName>
    <identifier>shiba</identifier>
  </varName>
  <symbol>=</symbol>
  <expression>
    <term>
      <stringConstant>bonchon</stringConstant>
    </term>
  </expression>
  <symbol>;</symbol>
</letStatement>)xml");
}

TEST(CompilationEngineTest, ReturnStatementToXml) {
  std::istringstream input(R"jack(
return 42;
)jack");
  JackTokenizer tokenizer(input);
  tokenizer.Advance();
  std::ostringstream output;
  CompilationEngine engine(tokenizer, output);
  
  engine.CompileReturn();

  EXPECT_EQ(output.str(), R"xml(<returnStatement>
  <expression>
    <term>
      <intConstant>42</intConstant>
    </term>
  </expression>
  <symbol>;</symbol>
</returnStatement>)xml");
}

}  // namespace
}  // namespace jack
