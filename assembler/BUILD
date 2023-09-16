

cc_binary(
  name = "assembler",
  srcs = ["assembler.cc"],
  deps = [
    ":code",
    ":parser",
    ":symbol_table",
  ]
)

cc_library(
  name = "code",
  hdrs = ["code.h"],
  srcs = ["code.cc"]
)

cc_test(
  name = "code_test",
  srcs = ["code_test.cc"],
  size = "small",
  deps = [
    ":code",
    "@com_google_googletest//:gtest_main"
  ]
)

cc_library(
  name = "parser",
  hdrs = ["parser.h"],
  srcs = ["parser.cc"]
)

cc_test(
  name = "parser_test",
  srcs = ["parser_test.cc"],
  size = "small",
  deps = [
    ":parser",
    "@com_google_googletest//:gtest_main"
  ]
)

cc_library(
  name = "symbol_table",
  hdrs = ["symbol_table.h"],
  srcs = ["symbol_table.cc"],    
)

cc_test(
  name = "symbol_table_test",
  srcs = ["symbol_table_test.cc"],
  size = "small",
  deps = [
    ":symbol_table",
    "@com_google_googletest//:gtest_main"
  ]
)
