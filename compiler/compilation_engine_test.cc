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

TEST(CompilationEngineTest, LetStatementWithIndexToXml) {
  std::istringstream input(R"jack(
let shiba[n + 2] = "bonchon";
)jack");
  JackTokenizer tokenizer(input);
  tokenizer.Advance();
  std::ostringstream output;
  CompilationEngine engine(tokenizer, output);
  
  engine.CompileLet();

  EXPECT_EQ(output.str(), R"xml(<letStatement>
  <varName>shiba</varName>
  <symbol>[</symbol>
  <expression>
    <term>
      <varName>n</varName>
    </term>
    <op>+</op>
    <term>
      <intConst>2</intConst>
    </term>
  </expression>
  <symbol>]</symbol>
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

TEST(CompilationEngineTest, ClassVarDevToXml) {
  std::istringstream input(R"jack(
static int x, y;
)jack");
  JackTokenizer tokenizer(input);
  tokenizer.Advance();
  std::ostringstream output;
  CompilationEngine engine(tokenizer, output);
  
  engine.CompileClassVarDec();

  EXPECT_EQ(output.str(), R"xml(<classVarDec>
  <keyword>static</keyword>
  <keyword>int</keyword>
  <identifier>x</identifier>
  <symbol>,</symbol>
  <identifier>y</identifier>
  <symbol>;</symbol>
</classVarDec>)xml");
}

TEST(CompilationEngineTest, ParameterListToXml) {
  std::istringstream input(R"jack(
(int x, int y, OutputStream outputStream)
)jack");
  JackTokenizer tokenizer(input);
  tokenizer.Advance();
  std::ostringstream output;
  CompilationEngine engine(tokenizer, output);
  
  engine.CompileParameterList();

  EXPECT_EQ(output.str(), R"xml(<parameterList>
  <symbol>(</symbol>
  <keyword>int</keyword>
  <identifier>x</identifier>
  <symbol>,</symbol>
  <keyword>int</keyword>
  <identifier>y</identifier>
  <symbol>,</symbol>
  <className>OutputStream</className>
  <identifier>outputStream</identifier>
  <symbol>)</symbol>
</parameterList>)xml");
}

TEST(CompilationEngineTest, VarDecToXml) {
  std::istringstream input(R"jack(
var int a, b, c, d;
)jack");
  JackTokenizer tokenizer(input);
  tokenizer.Advance();
  std::ostringstream output;
  CompilationEngine engine(tokenizer, output);
  
  engine.CompileVarDec();

  EXPECT_EQ(output.str(), R"xml(<varDec>
  <keyword>int</keyword>
  <varName>a</varName>
  <symbol>,</symbol>
  <varName>b</varName>
  <symbol>,</symbol>
  <varName>c</varName>
  <symbol>,</symbol>
  <varName>d</varName>
  <symbol>;</symbol>
</varDec>)xml");
}

TEST(CompilationEngineTEst, SubroutineBodyToXml) {
  std::istringstream input(R"jack(
{
  var int a, b;
  let a = 1;
  let b = 2;
  do thing(a, b);
}
)jack");
  JackTokenizer tokenizer(input);
  tokenizer.Advance();
  std::ostringstream output;
  CompilationEngine engine(tokenizer, output);
  
  engine.CompileSubroutineBody();

  EXPECT_EQ(output.str(), R"xml(<subroutineBody>
  <symbol>{</symbol>
  <varDec>
    <keyword>int</keyword>
    <varName>a</varName>
    <symbol>,</symbol>
    <varName>b</varName>
    <symbol>;</symbol>
  </varDec>
  <statements>
    <letStatement>
      <varName>a</varName>
      <symbol>=</symbol>
      <expression>
        <term>
          <intConst>1</intConst>
        </term>
      </expression>
      <symbol>;</symbol>
    </letStatement>
    <letStatement>
      <varName>b</varName>
      <symbol>=</symbol>
      <expression>
        <term>
          <intConst>2</intConst>
        </term>
      </expression>
      <symbol>;</symbol>
    </letStatement>
    <doStatement>
      <subroutineCall>
        <subroutineName>thing</subroutineName>
        <symbol>(</symbol>
        <expressionList>
          <expression>
            <term>
              <varName>a</varName>
            </term>
          </expression>
          <symbol>,</symbol>
          <expression>
            <term>
              <varName>b</varName>
            </term>
          </expression>
        </expressionList>
        <symbol>)</symbol>
      </subroutineCall>
      <symbol>;</symbol>
    </doStatement>
  </statements>
  <symbol>}</symbol>
</subroutineBody>)xml");
}

TEST(CompilationEngineTest, SubroutineToXml) {
  std::istringstream input(R"jack(
function void close() {
  do shutdown();
  do release();
}
)jack");
  JackTokenizer tokenizer(input);
  tokenizer.Advance();
  std::ostringstream output;
  CompilationEngine engine(tokenizer, output);
  
  engine.CompileSubroutine();

  EXPECT_EQ(output.str(), R"xml(<subroutineDec>
  <keyword>function</keyword>
  <keyword>void</keyword>
  <subroutineName>close</subroutineName>
  <parameterList>
    <symbol>(</symbol>
    <symbol>)</symbol>
  </parameterList>
  <subroutineBody>
    <symbol>{</symbol>
    <statements>
      <doStatement>
        <subroutineCall>
          <subroutineName>shutdown</subroutineName>
          <symbol>(</symbol>
          <expressionList>
          </expressionList>
          <symbol>)</symbol>
        </subroutineCall>
        <symbol>;</symbol>
      </doStatement>
      <doStatement>
        <subroutineCall>
          <subroutineName>release</subroutineName>
          <symbol>(</symbol>
          <expressionList>
          </expressionList>
          <symbol>)</symbol>
        </subroutineCall>
        <symbol>;</symbol>
      </doStatement>
    </statements>
    <symbol>}</symbol>
  </subroutineBody>
</subroutineDec>)xml");
}

TEST(CompilationEngineTest, ClassToXml) {
  std::istringstream input(R"jack(
class IoStream {

  field int channel;
  static int n_instances;

  method void close() {
    do IoEngine.shutdown();
  }
}
)jack");
  JackTokenizer tokenizer(input);
  tokenizer.Advance();
  std::ostringstream output;
  CompilationEngine engine(tokenizer, output);
  
  engine.CompileClass();

  EXPECT_EQ(output.str(), R"xml(<classDec>
  <className>IoStream</className>
  <symbol>{</symbol>
  <classVarDec>
    <keyword>field</keyword>
    <keyword>int</keyword>
    <identifier>channel</identifier>
    <symbol>;</symbol>
  </classVarDec>
  <classVarDec>
    <keyword>static</keyword>
    <keyword>int</keyword>
    <identifier>n_instances</identifier>
    <symbol>;</symbol>
  </classVarDec>
  <subroutineDec>
    <keyword>method</keyword>
    <keyword>void</keyword>
    <subroutineName>close</subroutineName>
    <parameterList>
      <symbol>(</symbol>
      <symbol>)</symbol>
    </parameterList>
    <subroutineBody>
      <symbol>{</symbol>
      <statements>
        <doStatement>
          <subroutineCall>
            <identifier>IoEngine</identifier>
            <symbol>.</symbol>
            <subroutineName>shutdown</subroutineName>
            <symbol>(</symbol>
            <expressionList>
            </expressionList>
            <symbol>)</symbol>
          </subroutineCall>
          <symbol>;</symbol>
        </doStatement>
      </statements>
      <symbol>}</symbol>
    </subroutineBody>
  </subroutineDec>
  <symbol>}</symbol>
</classDec>)xml");
}

}  // namespace
}  // namespace jack
