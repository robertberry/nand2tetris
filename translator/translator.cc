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

void HandleFile(CodeWriter& code_writer, std::filesystem::path file_path) {
  std::ifstream input_stream(file_path.string());
  if (!input_stream.is_open()) {
    std::cerr << "Could not open '" << file_path << "'" << std::endl;
    exit(2);
  }
  Parser parser(input_stream);

  code_writer.SetFileName(file_path.filename().string());

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

      case CommandType::kCCall:
        code_writer.WriteCall(instruction.arg1, instruction.arg2);
        break;

      case CommandType::kCFunction:
        code_writer.WriteFunction(instruction.arg1, instruction.arg2);
        break;

      case CommandType::kCReturn:
        code_writer.WriteReturn();
        break;

      default:
        std::cerr << "Unexpected instruction type" << std::endl;
        exit(3);
    }
  }
}

std::filesystem::path GetOutputPath(std::filesystem::path input_path) {
  if (!std::filesystem::is_directory(input_path)) {
    input_path.replace_extension(kHackAssemblyExtension);
    return input_path;
  }

  // Use same name for file as directory.
  std::filesystem::path file_name = input_path.filename();
  file_name.replace_extension(kHackAssemblyExtension);
  return input_path / file_name;
}

bool HasVmExtension(const std::filesystem::path& path) {
  return path.extension() == ".vm";
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: translator <file>" << std::endl;
    return 1;
  }

  std::filesystem::path absolute_path = std::filesystem::absolute(argv[1]);
  std::filesystem::path output_path = GetOutputPath(absolute_path);
  std::ofstream output_stream(output_path.string());
  CodeWriter code_writer(output_stream);
  
  if (!output_stream.is_open()) {
    std::cerr << "Could not open '" << output_path << "' for writing" 
              << std::endl;
    return 2;  
  }

  code_writer.WriteBootstrap();
  if (std::filesystem::is_directory(absolute_path)) {
    bool saw_at_least_one_vm_file = false;
    for (const auto& entry : std::filesystem::directory_iterator(absolute_path)) {
      if (entry.is_regular_file() && HasVmExtension(entry.path())) {
        saw_at_least_one_vm_file = true;
        HandleFile(code_writer, entry.path());
      }
    }
    if (!saw_at_least_one_vm_file) {
      std::cerr << "No .vm files in '" << absolute_path << "'" << std::endl;
      exit(4);
    }
  } else {
    HandleFile(code_writer, absolute_path);
  }
  code_writer.Close();

  return 0;
}
