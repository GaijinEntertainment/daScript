options gen2
module _host_fixture public

require dastest/testing_boost
require daslib/fio
require daslib/clargs
require daslib/strings_boost
require strings

def tests_dir() : string => path_join(get_das_root(), "utils/daslang/tests")

def exe_name(name : string) : string => get_platform_name() == "windows" ? "{name}.exe" : name

struct Captured {
    rc : int
    out : string
}

//! `argv` from `cwd` ("" inherits), stdout and stderr merged
def run_daslang(argv : array<string>; cwd : string = "") : Captured {
    var res : Captured
    let noenv : array<string>
    res.rc = run_and_capture(argv, cwd, noenv, res.out, 300.0)
    return res
}

def copy_into(t : T?; src, dst : string) {
    mkdir_rec(dir_name(dst))
    var err : string
    let copied = copy_file(src, dst, true, err)
    t |> success(copied, "copied {src} to {dst}: {err}")
}

def is_daslang_library(f : string) : bool {
    let known_prefix = (starts_with(f, "libdaslang_main") || starts_with(f, "daslang_main")
        || starts_with(f, "libDaScriptDyn") || starts_with(f, "liblibDaScriptDyn"))
    return known_prefix && (ends_with(f, ".dll") || ends_with(f, ".dylib") || ends_with(f, ".so"))
}

//! the libraries a copied daslang loads (Windows: beside it)
def copy_daslang_libraries(t : T?; root : string) {
    let windows = get_platform_name() == "windows"
    let src_dir = windows ? dir_name(get_host_binary()) : path_join(get_das_root(), "lib")
    let dst_dir = path_join(root, windows ? "bin" : "lib")
    var copied = 0
    dir(src_dir) $(f) {
        if (is_daslang_library(f)) {
            copy_into(t, path_join(src_dir, f), path_join(dst_dir, f))
            copied++
        }
    }
    t |> success(copied >= 2, "found {copied} daslang libraries under {src_dir}")
}

//! a scratch das root with builtin.das, the echo tool and `copies`
def make_sdk(t : T?; name : string; copies : array<string>) : string {
    let root = path_join(test_temp_dir(), name)
    copy_into(t, path_join(get_das_root(), "daslib/builtin.das"),
        path_join(root, "daslib/builtin.das"))
    copy_into(t, path_join(tests_dir(), "_echo_tool.das"), path_join(root, "utils/echo/main.das"))
    let daslang = get_host_binary()
    for (c in copies) {
        copy_into(t, daslang, path_join(root, "bin/{c}"))
    }
    if (!empty(copies)) {
        copy_daslang_libraries(t, root)
    }
    return root
}

def lines_of(out : string) : array<string> =>
    <- [for (ln in split(out, "\n")); strip(ln); where !empty(strip(ln))]

def args_of(out : string) : array<string> =>
    <- [for (ln in lines_of(out)); slice(ln, 4); where starts_with(ln, "arg=")]

//! the value after `key` on the first line that starts with it
def field_of(out, key : string) : string {
    for (ln in lines_of(out)) {
        if (starts_with(ln, key)) return slice(ln, length(key))
    }
    return ""
}

def host_of(out : string) : string => to_generic_path(field_of(out, "host="))

def root_of(out : string) : string => field_of(out, "root=")
