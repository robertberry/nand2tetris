#include "translator/code_writer.h"

namespace translator {

void CodeWriter::WriteArithmetic(std::string_view command) {
  // TODO
}

void CodeWriter::WritePush(std::string_view segment, int offset) {
  if (segment == "constant") {
    output_ << "// Push " << offset << " onto the stack" << std::endl;
    output_ << "@" << offset << std::endl;
    output_ << "D=A" << std::endl;
    output_ << "@SP" << std::endl;
    output_ << "A=M" << std::endl;
    output_ << "M=D" << std::endl;
    output_ << "@SP" << std::endl;
    output_ << "M=M+1" << std::endl;
    output_ << std::endl;
    return;
  }

  // TODO
}

void CodeWriter::WritePop(std::string_view segment, int offset) {
  // TODO
}

void CodeWriter::Close() {
  // TODO
}

}  // namespace translator
