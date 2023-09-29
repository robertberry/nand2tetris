cc_library(
    name = "compilation_engine",
    hdrs = ["compilation_engine.h"],
    srcs = ["compilation_engine.cc"],
    deps = [
        ":jack_tokenizer",
        ":xml_writer"
    ]
)

cc_test(
    name = "compilation_engine_test",
    srcs = ["compilation_engine_test.cc"],
    deps = [
        ":compilation_engine",
        "@com_google_googletest//:gtest_main"
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

cc_library(
    name = "xml_writer",
    hdrs = ["xml_writer.h"],
    srcs = ["xml_writer.cc"],
)

cc_test(
    name = "xml_writer_test",
    srcs = ["xml_writer_test.cc"],
    size = "small",
    deps = [
        ":xml_writer",
        "@com_google_googletest//:gtest_main"
    ]
)
