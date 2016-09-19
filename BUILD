cc_binary(
    name = 'lr',
    extra_cppflags = [
        '-Wl,--no-as-needed',
        '-pthread',
        '-std=c++0x',],
    srcs = [
    'src/main.cc',],
    deps = [
        '//mmbiz/mmbizdatacomm/ftrl/mmbizlr/src/common:common',
        '//mmbiz/mmbizdatacomm/ftrl/mmbizlr/src/io:io',
        '//mmbiz/mmbizdatacomm/ftrl/mmbizlr/src/sgd:sgd',
        '//mmbiz/mmbizdatacomm/ftrl/mmbizlr/src/ftrl:ftrl',
    ],
)
