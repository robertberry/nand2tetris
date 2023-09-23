#include <filesystem>
#include <fstream>
#include <iostream>
#include <string_view>

#include "translator/code_writer.h"
#include "translator/parser.h"

using ::translator::CodeWriter;
using ::translator::CommandType;
using ::translator::Instruction;
using ::translator::Parser;

constexpr std::string_view kHackAssemblyExtension = "asm";

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: translator <file>" << std::endl;
    return 1;
  }

  std::filesystem::path absolute_path = std::filesystem::absolute(argv[1]);
  std::ifstream input_stream(absolute_path.string());
  if (!input_stream.is_open()) {
    std::cerr << "Could not open '" << absolute_path << "'" << std::endl;
    return 2;
  }
  Parser parser(input_stream);

  absolute_path.replace_extension(kHackAssemblyExtension);
  std::ofstream output_stream(absolute_path.string());
  if (!output_stream.is_open()) {
    std::cerr << "Could not open '" << absolute_path << "' for writing" 
              << std::endl;
    return 2;
  }
  CodeWriter code_writer(output_stream);
  code_writer.SetFileName(absolute_path.filename().string());

  code_writer.WriteBootstrap();

  while (parser.HasMoreLines()) {
    parser.Advance();
    Instruction instruction = parser.CurrentInstruction();
    CommandType command_type = instruction.command_type;

    switch (command_type) {
      case CommandType::kCArithmetic:
        code_writer.WriteArithmetic(instruction.arg1);
        break;

      case CommandType::kCPush:
        code_writer.WritePush(instruction.arg1, instruction.arg2);
        break;

      case CommandType::kCPop:
        code_writer.WritePop(instruction.arg1, instruction.arg2);
        break;

      case CommandType::kCLabel:
        code_writer.WriteLabel(instruction.arg1);
        break;

      case CommandType::kCGoto:
        code_writer.WriteGoto(instruction.arg1);
        break;

      case CommandType::kCIf:
        code_writer.WriteIf(instruction.arg1);
        break;

      default:
        std::cerr << "Unexpected instruction type" << std::endl;
        return 3;
    }
  }

  code_writer.Close();

  return 0;
}
