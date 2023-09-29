#include "compiler/compilation_engine.h"

#include <sstream>
#include <gtest/gtest.h>

#include "compiler/jack_tokenizer.h"

namespace jack {
namespace {

TEST(CompilationEngineTest, IfStatementToXml) {
  std::istringstream input(R"jack(
if (x > 0) {
  return;
}
)jack");
  JackTokenizer tokenizer(input);
  tokenizer.Advance();
  std::ostringstream output;
  CompilationEngine engine(tokenizer, output);
  
  engine.CompileIf();

  EXPECT_EQ(output.str(), R"xml(<ifStatement>
  <symbol>(</symbol>
  <expression>
    <term>
      <varName>x</varName>
    </term>
    <op>&gt;</op>
    <term>
      <intConst>0</intConst>
    </term>
  </expression>
  <symbol>)</symbol>
  <symbol>{</symbol>
  <statements>
    <returnStatement>
      <symbol>;</symbol>
    </returnStatement>
  </statements>
  <symbol>}</symbol>
</ifStatement>)xml");
}

TEST(CompilationEngineTest, WhileStatementToXml) {
  std::istringstream input(R"jack(
while (x) {
  let y = y + 1;
  let x = x - 1;
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
      <varName>x</varName>
    </term>
  </expression>
  <symbol>)</symbol>
  <symbol>{</symbol>
  <statements>
    <letStatement>
      <varName>y</varName>
      <symbol>=</symbol>
      <expression>
        <term>
          <varName>y</varName>
        </term>
        <op>+</op>
        <term>
          <intConst>1</intConst>
        </term>
      </expression>
      <symbol>;</symbol>
    </letStatement>
    <letStatement>
      <varName>x</varName>
      <symbol>=</symbol>
      <expression>
        <term>
          <varName>x</varName>
        </term>
        <op>-</op>
        <term>
          <intConst>1</intConst>
        </term>
      </expression>
      <symbol>;</symbol>
    </letStatement>
  </statements>
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
  <varName>shiba</varName>
  <symbol>=</symbol>
  <expression>
    <term>
      <stringConst>bonchon</stringConst>
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
      <intConst>42</intConst>
    </term>
  </expression>
  <symbol>;</symbol>
</returnStatement>)xml");
}

TEST(CompilationEngineTest, DoStatementToXml) {
  std::istringstream input(R"jack(
do zoomies(bonchon, 3600);
)jack");
  JackTokenizer tokenizer(input);
  tokenizer.Advance();
  std::ostringstream output;
  CompilationEngine engine(tokenizer, output);
  
  engine.CompileDo();

  EXPECT_EQ(output.str(), R"xml(<doStatement>
  <subroutineCall>
    <subroutineName>zoomies</subroutineName>
    <symbol>(</symbol>
    <expressionList>
      <expression>
        <term>
          <varName>bonchon</varName>
        </term>
      </expression>
      <symbol>,</symbol>
      <expression>
        <term>
          <intConst>3600</intConst>
        </term>
      </expression>
    </expressionList>
    <symbol>)</symbol>
  </subroutineCall>
  <symbol>;</symbol>
</doStatement>)xml");
}

}  // namespace
}  // namespace jack
