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

}  // namespace
}  // namespace jack
