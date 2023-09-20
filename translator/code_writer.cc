#include "translator/code_writer.h"

#include <array>
#include <string_view>

namespace translator {

constexpr std::string_view kSegmentNameSymbolTable[] = {
  "this", "THIS",
  "that", "THAT",
  "argument", "ARG",
  "local", "LCL"
};

constexpr std::string_view kPointerSymbolByOffset[] = {
  "THIS",
  "THAT"
};

void CodeWriter::WriteArithmetic(std::string_view command) {
  // TODO
}

void CodeWriter::WritePush(std::string_view segment, int offset) {
  // First prepend a comment explaining the assembly that is to follow.
  if (segment == "constant") {
    output_ << "// Push " << offset << " onto the stack" << std::endl;
  } else {
    output_ << "// Push " << segment << "[" << offset << "] onto the stack"
            << std::endl;
  }

  if (segment == "constant") {
    output_ << "@" << offset << std::endl
            << "D=A" << std::endl;
  } else if (segment == "this" || segment == "that" || segment == "argument"
             || segment == "local") {
    output_ << "@" << SegmentNameToAssemblySymbol(segment) << std::endl;
    if (offset == 0) {
      output_ << "A=M" << std::endl;
    } else {
      output_ << "D=M" << std::endl
              << "@" << offset << std::endl
              << "A=D+A" << std::endl;
    }
    output_ << "D=M" << std::endl;
  } else if (segment == "pointer") {
    if (offset < 0 || offset > 1) {
      // TODO: Better error handling.
      std::cerr << "Invalid pointer offset: " << offset << std::endl;
      exit(1);
    }
    std::string_view symbol = kPointerSymbolByOffset[offset];
    output_ << "@" << symbol << std::endl
            << "D=M" << std::endl;
  } else if (segment == "temp") {
    if (offset < 0 || offset > 7) {
      // TODO: better error handling.
      std::cerr << "Invalid temp offset: " << offset << std::endl;
      exit(1);
    }
    int memory_location = 5 + offset;
    output_ << "@" << memory_location << std::endl
            << "D=M" << std::endl;
  } else if (segment == "static") {
    output_ << "@" << static_name_ << "." << offset << std::endl
            << "D=M" << std::endl;
  }

  // Write contents of D to the stack.
  output_ << "@SP" << std::endl
          << "A=M" << std::endl
          << "M=D" << std::endl
          << "@SP" << std::endl
          << "M=M+1" << std::endl << std::endl;
}

void CodeWriter::WritePop(std::string_view segment, int offset) {
  // TODO
}

void CodeWriter::Close() {
  // TODO
}

std::string_view CodeWriter::SegmentNameToAssemblySymbol(std::string_view segment_name) {
  constexpr int n = std::size(kSegmentNameSymbolTable);
  for (int i = 0; i < n; i += 2) {
    if (kSegmentNameSymbolTable[i] == segment_name) {
      return kSegmentNameSymbolTable[i + 1];
    }
  }
  // TODO: Better error handling.
  exit(1);
}

}  // namespace translator
