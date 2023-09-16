#include "assembler/symbol_table.h"

#include <array>
#include <string_view>

namespace hack {

namespace {

constexpr std::pair<std::string_view, int> sym(std::string_view key, int value) {
  return std::make_pair(key, value);
}

}  // namespace

constexpr std::pair<std::string_view, int> kSymbolInitTable[] = {
  sym("R0", 0),
  sym("R1", 1),
  sym("R2", 2),
  sym("R3", 3),
  sym("R4", 4),
  sym("R5", 5),
  sym("R6", 6),
  sym("R7", 7),
  sym("R8", 8),
  sym("R9", 9),
  sym("R10", 10),
  sym("R11", 11),
  sym("R12", 12),
  sym("R13", 13),
  sym("R14", 14),
  sym("R15", 15),
  sym("SP", 0),
  sym("LCL", 1),
  sym("ARG", 2),
  sym("THIS", 3),
  sym("THAT", 4),
  sym("SCREEN", 16384),
  sym("KBD", 24576)
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
