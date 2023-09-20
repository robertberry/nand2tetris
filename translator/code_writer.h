#ifndef TRANSLATOR_CODE_WRITER_H_
#define TRANSLATOR_CODE_WRITER_H_

#include <iostream>
#include <string>
#include <string_view>

namespace translator {

// Translates p-code into Hack assembly code.
class CodeWriter final {
 public:
  // Returns a new CodeWriter, writing its output to the provided stream.
  CodeWriter(std::string_view static_name, std::ostream& output)
      : static_name_(static_name), output_(output) {}

  // Writes an arithmetic expression as assembly code.
  void WriteArithmetic(std::string_view command);

  // Pushes a 16-bit int from the given memory location onto the stack.
  void WritePush(std::string_view segment, int offset);

  // Pops a 16-bit int from the top of the stack into the given memory location.
  void WritePop(std::string_view segment, int offset);

  // Closes the stream, flushes it.
  void Close();

 private:
  std::string static_name_;

  std::ostream& output_;

  static std::string_view SegmentNameToAssemblySymbol(std::string_view segment_name);

  void WriteOpOnTopTwoElements(std::string_view op);
};

}  // namespace translator

#endif  // TRANSLATOR_CODE_WRITER_H_
