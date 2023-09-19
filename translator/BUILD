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
  srcs = ["translator.cc"]
)
