options gen2
options indenting = 4

module _mc_common shared public

require dastest/testing_boost public

require strings
require daslib/fio

//! the daslang binary to spawn - dastest runs as `daslang(.exe) dastest/dastest.das ...`,
//! so argv[0] is the interpreter, not the test script
def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

//! the manifest file this binary's kind writes beside a descriptor
def manifest_file_name() : string =>
    das_is_dll_build() ? ".das_module.manifest" : ".das_module.static.manifest"

def twin_manifest_file_name() : string =>
    das_is_dll_build() ? ".das_module.static.manifest" : ".das_module.manifest"

//! the manifest a `--jit-target` run of this kind writes: the target's platform name sits before `.manifest`
def target_manifest_file_name(target : string) : string =>
    das_is_dll_build() ? ".das_module.{target}.manifest" : ".das_module.static.{target}.manifest"

//! the other binary kind's daslang beside this one - `daslang_static` beside `daslang` and back; "" when absent
def twin_exe() : string {
    let exe = das_exe()
    let dir = dir_name(exe)
    let ext = get_platform_name() == "windows" ? ".exe" : ""
    let twin = path_join(dir, (das_is_dll_build() ? "daslang_static" : "daslang") + ext)
    return stat(twin).is_valid ? twin : ""
}

//! the child's environment, as a command prefix: the scan trace on, so the trace names each
//! descriptor's verdict and each shared-module load; the prefix also keeps the command from
//! opening with a quote, which cmd.exe would strip
def trace_prefix() : string {
    let trace_env = "DAS_TRACE_MODULE_LOAD=1"
    return get_platform_name() == "windows" ? "set \"{trace_env}\"&& " : "{trace_env} "
}

//! stderr joins stdout: the scan trace goes through the logger
def run_child(cmd : string; var output : string&) : int {
    return unsafe(popen_timeout("{cmd} 2>&1", 300.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

def run_argv(args : array<string>; var output : string&) : int =>
    run_and_capture(args, output, 300.0)

def report_child(t : T?; phase : string; rc : int; out : string; marker : string) : bool {
    let ok = rc == 0 && find(out, marker) >= 0
    if (!ok) {
        t |> failure("{phase} child: rc={rc}, marker '{marker}' {find(out, marker) >= 0 ? "found" : "MISSING"}")
        t |> failure("{phase} child output follows:\n{out}")
    }
    return ok
}

//! runs `cmd` and reports it as report_child does; `out` keeps the child's output for the
//! caller's own reads. The run is a statement of its own, since the order a call's arguments
//! are evaluated in is not defined (LINT030).
def run_child_reported(t : T?; phase : string; cmd : string; var out : string&; marker : string) : bool {
    let rc = run_child(cmd, out)
    return report_child(t, phase, rc, out, marker)
}

//! the command prefix that runs a child inside `folder`, so the
//! default cache it keys lands there
def enter_dir(folder : string) : string =>
    get_platform_name() == "windows" ? "cd /d {folder} &&" : "cd {folder} &&"

//! copies the files of `from` that `keep` accepts into `to`
def copy_files(from, to : string; keep : block<(name : string) : bool>) {
    var err : string
    mkdir_rec(to, err)
    dir(from) $(name) {
        if (invoke(keep, name)) {
            copy_file(path_join(from, name), path_join(to, name), true, err)
        }
    }
}

//! copies only daslib's top-level .das files: enough for a
//! descriptor's requires
def copy_tree_daslib(root : string) {
    let daslib = path_join(get_das_root(), "daslib")
    copy_files(daslib, path_join(root, "daslib")) $(name) => extension(name) == ".das"
}

//! a copy of one of the tree's module folders - descriptor and the
//! artifacts that exist, in either configuration's name - under
//! `root`'s modules, where it shadows the tree's
def copy_module(root, folder : string; artifacts : array<string>) : string {
    let src = path_join(path_join(get_das_root(), "modules"), folder)
    let dst = path_join(path_join(root, "modules"), folder)
    var err : string
    mkdir_rec(dst, err)
    copy_file(path_join(src, ".das_module"), path_join(dst, ".das_module"), true, err)
    for (name in artifacts) {
        for (suffix in [".shared_module", "_debug.shared_module"]) {
            let f = path_join(src, "{name}{suffix}")
            if (stat(f).is_valid) {
                copy_file(f, path_join(dst, "{name}{suffix}"), true, err)
            }
        }
    }
    copy_files(src, dst) $(name) {              // a library an artifact loads from beside itself
        let ext = extension(name)
        return ext == ".dylib" || ext == ".so" || ext == ".dll"
    }
    return dst
}
