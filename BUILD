load("//:build_defs/windows_dll_library.bzl", "windows_dll_library")

cc_binary(
    name = "DINPUT8.dll",
    srcs = glob([
        "**/*.cpp",
        "**/*.h",
        "**/*.hpp",
    ]),
    copts = ["/DCOMPILING_DLL"],
    linkopts = [
        "-DEFAULTLIB:user32",
        "-DEFAULTLIB:kernel32",
        "-DEFAULTLIB:user32",
        "-DEFAULTLIB:gdi32",
        "-DEFAULTLIB:winspool",
        "-DEFAULTLIB:shell32",
        "-DEFAULTLIB:ole32",
        "-DEFAULTLIB:oleaut32",
        "-DEFAULTLIB:uuid",
        "-DEFAULTLIB:comdlg32",
        "-DEFAULTLIB:advapi32",
    ],
    linkshared = 1,
)
