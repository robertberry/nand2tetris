cc_library(
  name = "code_writer",
  hdrs = ["code_writer.h"],
  srcs = ["code_writer.cc"]
)

cc_test(
  name = "code_writer_test",
  srcs = ["code_writer_test.cc"],
  size = "small",
  deps = [
    ":code_writer",
    "@com_google_googletest//:gtest_main"
  ]
)

cc_library(
  name = "parser",
  hdrs = ["parser.h"],
  srcs = ["parser.cc"],
  deps = [
    "//util/parsing:whitespace"
  ]
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

cc_binary(
  name = "translator",
  srcs = ["translator.cc"],
  deps = [
    ":code_writer",
    ":parser"
  ]
)
