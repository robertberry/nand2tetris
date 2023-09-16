#include <ctype.h>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "assembler/code.h"
#include "assembler/parser.h"
#include "assembler/symbol_table.h"

using ::hack::Instruction;
using ::hack::InstructionType;
using ::hack::Parser;
using ::hack::SymbolTable;
using ::hack::CompToBinary;
using ::hack::DestToBinary;
using ::hack::JumpToBinary;

SymbolTable PopulateLabels(std::ifstream& input_stream) {
  Parser parser(input_stream);

  auto symbol_table = SymbolTable::Create();
  int line_number = 0;
  while (parser.HasMoreLines()) {
    parser.Advance();
    Instruction instruction = parser.CurrentInstruction();
    if (instruction.instruction_type == InstructionType::kLInstruction) {
      symbol_table.AddEntry(instruction.symbol, line_number + 1);
    } else {
      line_number++;
    }
  }

  return symbol_table;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: assembler <file>" << std::endl;
    return 1;
  }

  std::filesystem::path absolute_path = std::filesystem::absolute(argv[1]);
  std::ifstream input_stream(absolute_path.string());
  if (!input_stream.is_open()) {
    std::cerr << "Could not open '" << absolute_path << "'" << std::endl;
    return 2;
  }

  auto symbol_table = PopulateLabels(input_stream);

  input_stream.clear();
  input_stream.seekg(0);
  Parser parser(input_stream);

  int next_variable_address = 16;
  while (parser.HasMoreLines()) {
    parser.Advance();
    Instruction instruction = parser.CurrentInstruction();
    InstructionType instruction_type = instruction.instruction_type;

    if (instruction_type == InstructionType::kLInstruction) {
      // Ignore pseudo-instruction.
      continue;
    }

    if (instruction_type == InstructionType::kAInstruction) {
      std::string& symbol = instruction.symbol;
      int n;
      if (isdigit(symbol[0])) {
	n = std::atoi(symbol.c_str());
      } else if (symbol_table.Contains(symbol)) {
        n = symbol_table.Get(symbol);
      } else {
	// need to make new symbol - it's a variable.
	n = next_variable_address++;
	symbol_table.AddEntry(symbol, n);
      }
      // If we were error-handling we'd check that n was less than max int.
      
      std::bitset<16> binary(n);
      // This bit signifies it's an A-instruction.
      binary[15] = 0;
      std::cout << binary.to_string() << std::endl;
      continue;
    }

    // Must be C instruction.
    std::cout << "111"
	      << CompToBinary(instruction.comparison)
	      << DestToBinary(instruction.destination)
	      << JumpToBinary(instruction.jump)
	      << std::endl;
  }
  
  return 0;
}
