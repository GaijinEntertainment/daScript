options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio
require daslib/strings_boost

//! the daslang binary to spawn - dastest runs as `daslang(.exe) dastest/dastest.das ...`,
//! so argv[0] is the interpreter, not the test script
def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

//! stderr joins stdout: the scan trace goes through the logger. Commands stay unquoted: a leading
//! quote breaks Windows _popen (cmd /c quote stripping)
def run(cmd : string; var output : string&) : int {
    return unsafe(popen_timeout("{cmd} 2>&1", 300.0, $(f) {
        if (f != null) {
            output = fread(f)
        }
    }))
}

def report_child(t : T?; phase : string; rc : int; out : string; marker : string) : bool {
    let ok = rc == 0 && find(out, marker) >= 0
    if (!ok) {
        t |> failure("{phase} child: rc={rc}, marker '{marker}' {find(out, marker) >= 0 ? "found" : "MISSING"}")
        t |> failure("{phase} child output follows:\n{out}")
    }
    return ok
}

let DESCRIPTOR = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    register_native_path(\"manifest_probe\", \"hello\", \"\{project_path\}/hello.das\")\n\}\n"
let DESCRIPTOR_OPT_OUT = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    no_manifest()\n    register_native_path(\"manifest_probe\", \"hello\", \"\{project_path\}/hello.das\")\n\}\n"

//! the scan trace line for the probe descriptor: `[module] descriptor <path>/.das_module: <verdict>`
def probe_line(out : string) : string {
    let at = find(out, "manifest_probe/.das_module: ")
    if (at < 0) {
        return ""
    }
    let rest = slice(out, at + length("manifest_probe/.das_module: "))
    let eol = find(rest, "\n")
    return eol < 0 ? rest : slice(rest, 0, eol)
}

//! a project root holding one pure-das module whose descriptor registers one require path,
//! and a driver that requires through it; the child runs with the scan trace on
[test]
def test_descriptor_manifest(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_dm_manifest", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let modDir = path_join(path_join(tmp, "modules"), "manifest_probe")
    var merr : string
    mkdir_rec(modDir, merr)
    let descriptor = path_join(modDir, ".das_module")
    let manifest = path_join(modDir, ".das_module.manifest")
    fwrite(descriptor, DESCRIPTOR)
    fwrite(path_join(modDir, "hello.das"), "options gen2\nmodule hello\ndef hello_value() : int \{\n    return 42\n\}\n")
    fwrite(path_join(tmp, "use.das"), "options gen2\nrequire manifest_probe/hello\n[export]\ndef main \{\n    print(\"PROBE \{hello_value()\}\\n\")\n\}\n")
    let cmd = "{das_exe()} -dasroot {get_das_root()} -no-module-cache -project_root {tmp} {path_join(tmp, "use.das")}"
    let prevTrace = get_env_variable("DAS_TRACE_MODULE_LOAD")
    set_env_variable("DAS_TRACE_MODULE_LOAD", "1")   //! inherited by the child; the trace names each descriptor's verdict
    t |> run("the first start compiles the descriptor and writes its manifest") @(t : T?) {
        var out : string
        t |> success(report_child(t, "cold", run(cmd, out), out, "PROBE 42"), "the require resolves through the compiled descriptor")
        t |> equal("compiled (no manifest), manifest written (1 row(s))", probe_line(out), "the trace says compiled and written:\n{out}")
        t |> success(stat(manifest).is_valid, "the manifest sits beside the descriptor")
        let text = fread(manifest)
        t |> success(text |> starts_with("das_module_manifest\t1\n"), "the manifest opens with its format version: {text}")
        t |> success(find(text, "\nnp\tmanifest_probe\thello\t") >= 0, "the manifest carries the recorded row: {text}")
        t |> success(find(text, "\nend\t1\n") >= 0, "the manifest closes with its row count: {text}")
    }
    t |> run("the second start replays the manifest and never compiles the descriptor") @(t : T?) {
        var out : string
        t |> success(report_child(t, "warm", run(cmd, out), out, "PROBE 42"), "the require resolves through the replayed rows")
        t |> equal("replayed 1 row(s)", probe_line(out), "the trace says replayed:\n{out}")
    }
    t |> run("an edited descriptor recompiles once and is replayed again after") @(t : T?) {
        fwrite(descriptor, "{DESCRIPTOR}// edited\n")
        var out : string
        t |> success(report_child(t, "edited", run(cmd, out), out, "PROBE 42"), "the edited descriptor still resolves the require")
        t |> equal("compiled (descriptor changed), manifest written (1 row(s))", probe_line(out), "the stamp mismatch recompiles and rewrites:\n{out}")
        var again : string
        t |> success(report_child(t, "edited warm", run(cmd, again), again, "PROBE 42"), "the rewritten manifest resolves the require")
        t |> equal("replayed 1 row(s)", probe_line(again), "the rewritten manifest replays:\n{again}")
    }
    t |> run("a damaged manifest recompiles and is rewritten") @(t : T?) {
        let text = fread(manifest)
        let endAt = find(text, "\nend\t")
        fwrite(manifest, slice(text, 0, endAt + 1))   //! drop the end line - a write that died before its rename
        var out : string
        t |> success(report_child(t, "damaged", run(cmd, out), out, "PROBE 42"), "the damaged manifest is not replayed")
        t |> equal("compiled (no end line), manifest written (1 row(s))", probe_line(out), "the trace names the damage:\n{out}")
    }
    t |> run("a cross-compile target is part of the key: a --jit-target run never replays a native manifest") @(t : T?) {
        //! dasOpenGL's descriptor registers its module only when get_cross_platform_name() says emscripten -
        //! an argv input, so a native manifest must not serve a --jit-target run, nor the other way round
        var out : string
        t |> success(report_child(t, "target", run("{cmd} -- --jit-target=wasm32-unknown-emscripten", out), out, "PROBE 42"), "the require resolves under the target")
        t |> equal("compiled (compile target), manifest written (1 row(s))", probe_line(out), "the target keys the manifest apart:\n{out}")
        var back : string
        t |> success(report_child(t, "native again", run(cmd, back), back, "PROBE 42"), "the native run resolves the require")
        t |> equal("compiled (compile target), manifest written (1 row(s))", probe_line(back), "the native run keys apart from the target's manifest:\n{back}")
    }
    t |> run("no_manifest() inside the descriptor makes it run on every start") @(t : T?) {
        fwrite(descriptor, DESCRIPTOR_OPT_OUT)
        var out : string
        t |> success(report_child(t, "opt-out", run(cmd, out), out, "PROBE 42"), "the opted-out descriptor resolves the require")
        t |> equal("compiled (descriptor changed), no_manifest recorded", probe_line(out), "the first run records the opt-out:\n{out}")
        let text = fread(manifest)
        t |> success(find(text, "\nno_manifest\n") >= 0 && find(text, "\nnp\t") < 0, "the manifest carries the flag and no rows: {text}")
        var again : string
        t |> success(report_child(t, "opt-out warm", run(cmd, again), again, "PROBE 42"), "the opted-out descriptor resolves the require again")
        t |> equal("compiled (no_manifest)", probe_line(again), "every later start compiles it:\n{again}")
    }
    set_env_variable("DAS_TRACE_MODULE_LOAD", prevTrace)
    var err : string
    rmdir_rec(tmp, err)
}
