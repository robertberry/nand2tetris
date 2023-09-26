#ifndef COMPILER_XML_WRITER_H_
#define COMPILER_XML_WRITER_H_

#include <iostream>
#include <string_view>
#include <vector>

namespace jack {

// Simple XML writer class.
class XmlWriter final {
 public:
  explicit XmlWriter(std::ostream& output) : output_(output) {}

  void OpenTag(std::string_view name);

  void AddTagWithContent(std::string_view name, std::string_view content);

  void CloseTag();

 private:
  std::ostream& output_;

  std::vector<std::string_view> open_tags_;

  void PrintIndent();
};

}  // namespace jack

#endif  // COMPILER_XML_WRITER_H_
