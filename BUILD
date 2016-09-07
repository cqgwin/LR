cc_binary(
    name = 'lr',
    extra_cppflags = [
        '-Wl,--no-as-needed',
        '-pthread',
        '-std=c++0x',],
    srcs = [
    'main.cc',],
    deps = [
        '//mmbiz/mmbizdatacomm/ftrl/ftrl_lr_64/common:common',
        '//mmbiz/mmbizdatacomm/ftrl/ftrl_lr_64/io:io',
        '//mmbiz/mmbizdatacomm/ftrl/ftrl_lr_64/ftrl:ftrl',
    ],
)
