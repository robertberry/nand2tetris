#include "compiler/xml_writer.h"

#include <sstream>
#include <gtest/gtest.h>

namespace jack {
namespace {

TEST(XmlWriterTest, E2e) {
  std::ostringstream output;
  XmlWriter xml_writer(output);

  xml_writer.OpenTag("whileStatement");
  xml_writer.AddTagWithContent("symbol", "}");
  xml_writer.OpenTag("expression");
  xml_writer.OpenTag("term");
  xml_writer.AddTagWithContent("varName", "count");
  xml_writer.CloseTag();
  xml_writer.CloseTag();
  xml_writer.CloseTag();

  EXPECT_EQ(output.str(), R"xml(<whileStatement>
  <symbol>}</symbol>
  <expression>
    <term>
      <varName>count</varName>
    </term>
  </expression>
</whileStatement>)xml");
}

}  // namespace
}  // namespace jack
