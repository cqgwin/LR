cc_binary(
    name = 'mmbizlr',
    extra_cppflags = [
        '-Wl,--no-as-needed',
        '-pthread',
        '-std=c++11',],
    srcs = [
    'main.cc',],
    deps = [
        '//mmbiz/mmbizdatacomm/ftrl/ftrl_lr_64/common:common',
        '//mmbiz/mmbizdatacomm/ftrl/ftrl_lr_64/io:io',
        '//mmbiz/mmbizdatacomm/ftrl/ftrl_lr_64/ftrl:ftrl',
    ],
)
