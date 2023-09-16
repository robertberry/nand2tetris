#include "assembler/symbol_table.h"

#include <gtest/gtest.h>

namespace hack {
namespace {

TEST(SymbolTableTest, CreateInitsTheTable) {
  SymbolTable table = SymbolTable::Create();

  EXPECT_EQ(table.Get("R0"), 0);
  EXPECT_EQ(table.Get("R15"), 15);
}

}  // namespace
}  // namespace hack
