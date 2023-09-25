cc_library(
    name = "compilation_engine",
    hdrs = ["compilation_engine.h"],
    srcs = ["compilation_engine.cc"],
    deps = [
        ":jack_tokenizer"
    ]
)

cc_binary(
    name = "compiler",
    srcs = ["compiler.cc"],
)

cc_library(
    name = "jack_tokenizer",
    hdrs = ["jack_tokenizer.h"],
    srcs = ["jack_tokenizer.cc"],
)

cc_test(
    name = "jack_tokenizer_test",
    srcs = ["jack_tokenizer_test.cc"],
    deps = [
        ":jack_tokenizer",
        "@com_google_googletest//:gtest_main"
    ]
)
