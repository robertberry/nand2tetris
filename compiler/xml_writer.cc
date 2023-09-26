#include "compiler/xml_writer.h"

#include <string_view>

namespace jack {

constexpr int kSpacesPerIndent = 2;

void XmlWriter::OpenTag(std::string_view name) {
  if (open_tags_.size() != 0) {
    output_ << std::endl;
  }
  PrintIndent();
  output_ << '<' << name << '>';
  open_tags_.push_back(name);
}

void XmlWriter::AddTagWithContent(std::string_view name, std::string_view content) {
  OpenTag(name);
  output_ << content;
  std::string_view tag = open_tags_.back();
  open_tags_.pop_back();
  output_ << "</" << tag << '>';
}

void XmlWriter::CloseTag() {
  std::string_view tag = open_tags_.back();
  open_tags_.pop_back();
  output_ << std::endl;
  PrintIndent();
  output_ << "</" << tag << '>';
}

void XmlWriter::PrintIndent() {
  int spaces = kSpacesPerIndent * open_tags_.size();
  for (int i = 0; i < spaces; i++) {
    output_ << ' ';
  }
}

}  // namespace
