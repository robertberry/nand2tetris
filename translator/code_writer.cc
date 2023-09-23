#include "translator/code_writer.h"

#include <array>
#include <sstream>
#include <string>
#include <string_view>

namespace translator {

namespace {

enum class Arity {
  kUnary,
  kBinary
};

struct Op {
  std::string_view command;
  
  std::string_view op;

  Arity arity;

  std::string_view comment;
};

}  // namespace

constexpr std::string_view kFunctionScopeNone = "noFunction";

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

constexpr Op kOps[] = {
  {
    "add",
    "+",
    Arity::kBinary,
    "Add the top two elements of the stack."
  },
  {
    "sub",
    "-",
    Arity::kBinary,
    "Subtract the top element from the second to top element of the stack."
  },
  {
    "and",
    "&",
    Arity::kBinary,
    "Performs bit-wise and on the top two elements of the stack."
  },
  {
    "or",
    "|",
    Arity::kBinary,
    "Performs bit-wise or on the top two elements of the stack."
  },
  {
    "neg",
    "-",
    Arity::kUnary,
    "Negate the top of the stack."
  },
  {
    "not",
    "!",
    Arity::kUnary,
    "Performs bit-wise not on the top element of the stack."
  }
};

constexpr Op kComps[] = {
  {
    "eq",
    "JEQ",
    Arity::kBinary,
    "Performs an equality comparison on the top two elements of the stack."
  },
  {
    "gt",
    "JGT",
    Arity::kBinary,
    "Performs a greater than comparison on the top two elements of the stack."
  },
  {
    "lt",
    "JLT",
    Arity::kBinary,
    "Performs a less than comparison on the top two elements of the stack."
  }
};

CodeWriter::CodeWriter(std::ostream& output) :
    function_scope_(kFunctionScopeNone), output_(output) {}

void CodeWriter::WriteArithmetic(std::string_view command) {
  for (Op op : kOps) {
    if (op.command != command) {
      continue;
    }

    output_ << "// " << op.comment << std::endl;
    if (op.arity == Arity::kUnary) {
      output_ << "@SP" << std::endl
              << "A=M-1" << std::endl
              << "M=" << op.op << "M" << std::endl
              << std::endl;
    } else if (op.arity == Arity::kBinary) {
      output_ << R"asm(@SP
M=M-1
A=M
D=M
A=A-1
M=M)asm" << op.op << R"asm(D

)asm";
    }
  }

  for (Op op : kComps) {
    if (op.command != command) {
      continue;
    }
    output_ << "// " << op.comment << std::endl;

    std::string symbol1 = GenSym();
    std::string symbol2 = GenSym();
    output_ << R"asm(@SP
M=M-1
A=M
D=M
A=A-1
D=M-D
@)asm" << symbol1 << R"asm(
D;)asm" << op.op << R"asm(
@SP
A=M-1
M=0
@)asm" << symbol2 << R"asm(
0;JMP
()asm" << symbol1 << R"asm()
@SP
A=M-1
M=-1
()asm" << symbol2 << R"asm()

)asm";
  }

  // Error handling if op not found?
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
  } else {
    WriteSetAToLocation(segment, offset);
    output_ << "D=M" << std::endl;
  }

  // Write contents of D to the stack.
  output_ << "@SP" << std::endl
          << "A=M" << std::endl
          << "M=D" << std::endl
          << "@SP" << std::endl
          << "M=M+1" << std::endl << std::endl;
}

void CodeWriter::WritePop(std::string_view segment, int offset) {
  if (segment == "constant") {
    // TODO: Better error handling
    std::cerr << "Cannot pop onto a constant" << std::endl;
    exit(1);
  }

  // Add comment explaining what is happening at a high level.
  output_ << "// Pop to " << segment << "[" << offset << "]" << std::endl;

  WriteSetAToLocation(segment, offset);
  output_ << R"asm(D=A
@SP
A=M
M=D
A=A-1
D=M
A=A+1
A=M
M=D
@SP
M=M-1

)asm";
}

void CodeWriter::WriteLabel(std::string_view label) {
  output_ << "// VM label " << label << std::endl
          << "(" << FullyQualifiedLabelName(label) << ")" << std::endl
          << std::endl;
}

void CodeWriter::WriteGoto(std::string_view label) {
  output_ << "// Goto VM label " << label << std::endl
          << "@" << FullyQualifiedLabelName(label) << std::endl
          << "0;JMP" << std::endl << std::endl;
}

void CodeWriter::WriteIf(std::string_view label) {
  output_ << "// If top of stack is true, goto " << label << std::endl
          << "@SP" << std::endl
          << "AM=M-1" << std::endl
          << "D=M" << std::endl
          << "@" << FullyQualifiedLabelName(label) << std::endl
          << "D;JNE" << std::endl << std::endl;
}

void CodeWriter::WriteFunction(std::string_view function_name, int n_vars) {
  function_scope_ = function_name;
  // TODO
}

void CodeWriter::WriteCall(std::string_view function_name, int n_args) {
  // TODO
}

void CodeWriter::WriteReturn() {
  function_scope_ = kFunctionScopeNone;
  // TODO
}

void CodeWriter::SetFileName(std::string_view file_name) {
  file_scope_ = ScopeNameFromFileName(file_name);
}

void CodeWriter::Close() {
  output_ << "// Infinitely loop to end program." << std::endl
          << "(EOP)" << std::endl
          << "@EOP" << std::endl
          << "0; JMP" << std::endl;
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

std::string CodeWriter::GenSym() {
  int i = next_symbol_++;
  std::ostringstream symbol;
  symbol << "G" << i;
  return symbol.str();
}

void CodeWriter::WriteSetAToLocation(std::string_view segment, int offset) {
  if (segment == "this" || segment == "that" || segment == "argument"
             || segment == "local") {
    output_ << "@" << SegmentNameToAssemblySymbol(segment) << std::endl;
    if (offset == 0) {
      output_ << "A=M" << std::endl;
    } else {
      output_ << "D=M" << std::endl
              << "@" << offset << std::endl
              << "A=D+A" << std::endl;
    }
  } else if (segment == "pointer") {
    if (offset < 0 || offset > 1) {
      // TODO: Better error handling.
      std::cerr << "Invalid pointer offset: " << offset << std::endl;
      exit(1);
    }
    std::string_view symbol = kPointerSymbolByOffset[offset];
    output_ << "@" << symbol << std::endl;
  } else if (segment == "temp") {
    if (offset < 0 || offset > 7) {
      // TODO: better error handling.
      std::cerr << "Invalid temp offset: " << offset << std::endl;
      exit(1);
    }
    int memory_location = 5 + offset;
    output_ << "@" << memory_location << std::endl;
  } else if (segment == "static") {
    output_ << "@" << file_scope_ << "." << offset << std::endl;
  }
}

std::string CodeWriter::ScopeNameFromFileName(std::string_view file_name) {
  size_t pos = file_name.rfind(".vm");
  if (pos == std::string::npos) {
    // TODO: Better error handling.
    return "badFileName";
  }
  return std::string(file_name.substr(0, pos));
}

std::string CodeWriter::FullyQualifiedLabelName(std::string_view label) {
  std::ostringstream fq_label;
  fq_label << file_scope_ << "." << function_scope_ << "$" << label;
  return fq_label.str();
}

}  // namespace translator
