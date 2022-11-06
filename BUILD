cc_library(
    name = "socket_connect",
    hdrs = ["socket_connect.hpp"],
)

cc_library(
    name = "config_util",
    hdrs = ["config_util.hpp"],
)

cc_binary(
    name = "server",
    srcs = [
        "server.cpp",
    ],
    deps = [
        ":config_util",
        ":socket_connect",
    ],
)

cc_binary(
    name = "client",
    srcs = [
        "client.cpp",
    ],
    deps = [
        ":config_util",
        ":socket_connect",
    ],
)
