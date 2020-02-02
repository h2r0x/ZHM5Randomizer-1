load("//:build_defs/windows_dll_library.bzl", "windows_dll_library")

windows_dll_library(
    name = "DINPUT8",
    linkopts = ["-DEFAULTLIB:user32"],
    srcs = glob(["**/*.cpp", "**/*.h", "**/*.hpp"]),
)