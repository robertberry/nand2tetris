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
  explicit CodeWriter(std::ostream& output);

  // Writes an arithmetic expression as assembly code.
  void WriteArithmetic(std::string_view command);

  // Pushes a 16-bit int from the given memory location onto the stack.
  void WritePush(std::string_view segment, int offset);

  // Pops a 16-bit int from the top of the stack into the given memory location.
  void WritePop(std::string_view segment, int offset);

  // Writes a label at this location for use by VM goto or if-goto.
  void WriteLabel(std::string_view label);

  // Writes a goto at this location for the provided label.
  void WriteGoto(std::string_view label);

  // Writes an if-goto at the current location. The if is executed against
  // whatever is at the top of the stack.
  void WriteIf(std::string_view label);

  // Writes assembly code for a function definition.
  void WriteFunction(std::string_view function_name, int n_vars);

  // Writes assembly code that implements calling the given function.
  void WriteCall(std::string_view function_name, int n_args);

  // Writes the return statement for a function.
  void WriteReturn();

  // Sets the file name of the current .vm file being processed. This should be
  // invoked whenever switching files in order to guarantee unique memory
  // layout for statics.
  void SetFileName(std::string_view file_name);

  // Closes the stream, flushes it.
  void Close();

 private:
  std::string file_scope_ = "_NO_FILE_NAME_SET";

  std::string function_scope_;

  std::ostream& output_;

  int next_symbol_ = 1;

  static std::string_view SegmentNameToAssemblySymbol(std::string_view segment_name);

  static std::string ScopeNameFromFileName(std::string_view file_name);

  std::string GenSym();

  void WriteSetAToLocation(std::string_view segment, int offset);

  std::string FullyQualifiedLabelName(std::string_view label);
};

}  // namespace translator

#endif  // TRANSLATOR_CODE_WRITER_H_
