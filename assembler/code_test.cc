#include "assembler/code.h"

#include <gtest/gtest.h>

namespace hack {
namespace {

TEST(CodeTest, JumpToBinaryEmptyString) {
  EXPECT_EQ(JumpToBinary(""), "000");
}

TEST(CodeTest, JumpToBinaryJGE) {
  EXPECT_EQ(JumpToBinary("JGE"), "011");
}

TEST(CodeTest, DestToBinaryEmptyString) {
  EXPECT_EQ(DestToBinary(""), "000");
}

TEST(CodeTest, DestToBinaryAD) {
  EXPECT_EQ(DestToBinary("AD"), "110");
}

TEST(CodeTest, CompToBinaryM) {
  EXPECT_EQ(CompToBinary("M"), "1110000");
}

}  // namespace
}  // namespace hack

