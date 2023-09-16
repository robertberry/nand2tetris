#include "assembler/code.h"

#include <array>
#include <string_view>

namespace hack {

constexpr std::string_view kJumpTable[] = {
  "", "000",
  "JGT", "001",
  "JEQ", "010",
  "JGE", "011",
  "JLT", "100",
  "JNE", "101",
  "JLE", "110",
  "JMP", "111"
};

constexpr std::string_view kDestTable[] = {
  "", "000",
  "M", "001",
  "D", "010",
  "DM", "011",
  "MD", "011",
  "A", "100",
  "AM", "101",
  "MA", "101",
  "AD", "110",
  "DA", "110",
  "ADM", "111",
  "AMD", "111",
  "MDA", "111",
  "MAD", "111",
  "DAM", "111",
  "DMA", "111"
};

constexpr std::string_view kCompTable[] = {
  "0", "0101010",
  "1", "0111111",
  "-1", "0111010",
  "D", "0001100",
  "A", "0110000",
  "M", "1110000",
  "!D", "0001101",
  "!A", "0110001",
  "!M", "1110001",
  "-D", "0001111",
  "-A", "0110011",
  "-M", "1110011",
  "D+1", "0011111",
  "A+1", "0110111",
  "M+1", "1110111",
  "D-1", "0001110",
  "A-1", "0110010",
  "M-1", "1110010",
  "D+A", "0000010",
  "D+M", "1000010",
  "D-A", "0010011",
  "D-M", "1010011",
  "A-D", "0000111",
  "M-D", "1000111",
  "D&A", "0000000",
  "D&M", "1000000",
  "D|A", "0010101",
  "D|M", "1010101"
};

std::string DestToBinary(std::string_view dest) {
  size_t len = std::size(kDestTable);
  for (size_t i = 0; i < len; i += 2) {
    if (dest == kDestTable[i]) {
      return std::string(kDestTable[i + 1]);
    }
  }
  return "000";
}

std::string CompToBinary(std::string_view comp) {
  size_t len = std::size(kCompTable);
  for (size_t i = 0; i < len; i += 2) {
    if (comp == kCompTable[i]) {
      return std::string(kCompTable[i + 1]);
    }
  }
  // Return error instead?
  return "0101010";
}

std::string JumpToBinary(std::string_view jump) {
  size_t len = std::size(kJumpTable);
  for (size_t i = 0; i < len; i += 2) {
    if (jump == kJumpTable[i]) {
      return std::string(kJumpTable[i + 1]);
    }
  }
  return "000";
}

}  // namespace hack
