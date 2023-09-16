#ifndef ASSEMBLER_SYMBOL_TABLE_H_
#define ASSEMBLER_SYMBOL_TABLE_H_

#include <map>
#include <string_view>

namespace hack {

class SymbolTable final {
 public:
  static SymbolTable Create();

  void AddEntry(std::string_view symbol, int value);

  bool Contains(std::string_view symbol);

  int Get(std::string_view symbol);

 private:
  SymbolTable() {}

  std::map<std::string_view, int> symbols_;
};

}

#endif  // ASSEMBLER_SYMBOL_TABLE_H_
