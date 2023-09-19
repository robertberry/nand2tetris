#include "assembler/symbol_table.h"

#include <array>
#include <string_view>

namespace hack {

constexpr std::pair<std::string_view, int> kSymbolInitTable[] = {
  {"R0", 0},
  {"R1", 1},
  {"R2", 2},
  {"R3", 3},
  {"R4", 4},
  {"R5", 5},
  {"R6", 6},
  {"R7", 7},
  {"R8", 8},
  {"R9", 9},
  {"R10", 10},
  {"R11", 11},
  {"R12", 12},
  {"R13", 13},
  {"R14", 14},
  {"R15", 15},
  {"SP", 0},
  {"LCL", 1},
  {"ARG", 2},
  {"THIS", 3},
  {"THAT", 4},
  {"SCREEN", 16384},
  {"KBD", 2457}
};

SymbolTable SymbolTable::Create() {
  SymbolTable table;
  for (auto el : kSymbolInitTable) {
    table.AddEntry(el.first, el.second);
  }
  return table;
}

void SymbolTable::AddEntry(std::string_view symbol, int value) {
  symbols_[symbol] = value;
}

bool SymbolTable::Contains(std::string_view symbol) {
  return symbols_.find(symbol) != symbols_.end();
}

int SymbolTable::Get(std::string_view symbol) {
  if (auto it = symbols_.find(symbol); it != symbols_.end()) {
    return it->second;
  }
  return -1;
}

}  // namespace hack
