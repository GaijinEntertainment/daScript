options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio
require daslib/strings_boost
require _mc_common

let DESCRIPTOR = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    register_native_path(\"manifest_probe\", \"hello\", \"\{project_path\}/hello.das\")\n\}\n"
let DESCRIPTOR_OPT_OUT = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    no_manifest()\n    register_native_path(\"manifest_probe\", \"hello\", \"\{project_path\}/hello.das\")\n\}\n"
let DESCRIPTOR_TABBED = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    register_native_path(\"manifest_probe\", \"hello\", \"\{project_path\}/hello.das\")\n    register_native_path(\"manifest_probe\", \"tab\\ted\", \"\{project_path\}/tabbed.das\")\n\}\n"
//! a C++ module row on an artifact that is not there: the Quiet overload defers the dlopen
//! failure and never fails the descriptor
let DESCRIPTOR_DM = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    register_dynamic_module(\"\{project_path\}/absent.shared_module\", \"Module_Absent\")\n\}\n"
//! the require path comes from a module the descriptor requires: its content is part of the key
let DESCRIPTOR_DEP = "options gen2\nrequire daslib/fio\nrequire ./helper\n\n[export]\ndef initialize(project_path : string) \{\n    register_native_path(\"manifest_probe\", helper_name(), \"\{project_path\}/hello.das\")\n\}\n"
let HELPER = "options gen2\nmodule helper\ndef helper_name() : string \{\n    return \"hello\"\n\}\n"
let DESCRIPTOR_BROKEN = "options gen2\n\n[export]\ndef initialize(project_path : string) \{\n    this_is_not_a_function()\n\}\n"

//! the scan trace line for a module's descriptor: `[module] descriptor <path>/.das_module: <verdict>`;
//! a replay verdict keeps its deferred count and drops the two timings
def probe_line(out : string; mod : string = "manifest_probe") : string {
    let marker = "{mod}/.das_module: "
    let at = find(out, marker)
    if (at < 0) {
        return ""
    }
    let rest = slice(out, at + length(marker))
    let eol = find(rest, "\n")
    let line = eol < 0 ? rest : slice(rest, 0, eol)
    let timing = find(line, " in ")
    if (timing < 0) {
        return line
    }
    let deferred = find(line, ", deferred ")
    return "{slice(line, 0, timing)} ({slice(line, deferred + 2)}"
}

def count_of(text : string; needle : string) : int {
    var n = 0
    var at = find(text, needle)
    while (at >= 0) {
        n++
        at = find(text, needle, at + length(needle))
    }
    return n
}

//! the key lines every manifest opens with; dep lines, rows and the end line follow
let LINE_HEADER = 0
let LINE_STAMP = 1
let LINE_DLL = 2
let LINE_ROOT = 3
let LINE_DASROOT = 4
let LINE_TARGET = 5

def manifest_lines(manifest : string) : array<string> {
    var lines <- split(fread(manifest), "\n")
    if (!empty(lines) && empty(lines[length(lines) - 1])) {
        lines |> pop()
    }
    return <- lines
}

def index_of_prefix(lines : array<string>; prefix : string) : int {
    for (i in range(length(lines))) {
        if (lines[i] |> starts_with(prefix)) return i
    }
    return -1
}

def with_line(lines : array<string>; idx : int; text : string) : string {
    var edited := lines
    edited[idx] = text
    return join(edited, "\n") + "\n"
}

def with_line_inserted(lines : array<string>; idx : int; text : string) : string {
    var edited : array<string>
    edited |> reserve(length(lines) + 1)
    for (i in range(length(lines))) {
        if (i == idx) {
            edited |> push(text)
        }
        edited |> push(lines[i])
    }
    return join(edited, "\n") + "\n"
}

//! writes `text` as the manifest, runs the child, and expects the scan to reject it for `why`
//! and rewrite it; the require resolves either way
def expect_rewrite(t : T?; cmd : string; manifest : string; text : string; why : string) {
    fwrite(manifest, text)
    var out : string
    t |> success(report_child(t, why, run_child(cmd, out), out, "PROBE 42"), "'{why}': the require resolves without the manifest")
    t |> equal("compiled ({why}), manifest written (1 row(s))", probe_line(out), "'{why}' recompiles and rewrites:\n{out}")
}

//! the project root the arms share: the probe module's paths and the child command line
struct Fixture {
    tmp : string
    modDir : string
    dmDir : string
    brokenDir : string
    descriptor : string
    manifest : string
    manifestTmp : string
    dmManifest : string
    cmd : string
}

//! a project root holding one pure-das module whose descriptor registers one require path,
//! a module whose descriptor registers an absent C++ artifact, a module whose descriptor does
//! not compile, and a driver that requires through the first
def make_fixture(tmp : string) : Fixture {
    let modules = path_join(tmp, "modules")
    let modDir = path_join(modules, "manifest_probe")
    let dmDir = path_join(modules, "dm_probe")
    let brokenDir = path_join(modules, "broken_probe")
    var merr : string
    mkdir_rec(modDir, merr)
    mkdir_rec(dmDir, merr)
    mkdir_rec(brokenDir, merr)
    let manifest = path_join(modDir, ".das_module.manifest")
    let fx = Fixture(
        tmp = tmp,
        modDir = modDir,
        dmDir = dmDir,
        brokenDir = brokenDir,
        descriptor = path_join(modDir, ".das_module"),
        manifest = manifest,
        manifestTmp = "{manifest}.tmp",
        dmManifest = path_join(dmDir, ".das_module.manifest"),
        cmd = "{trace_prefix()}\"{das_exe()}\" -dasroot \"{get_das_root()}\" -no-module-cache -project_root \"{tmp}\" \"{path_join(tmp, "use.das")}\""
    )
    fwrite(fx.descriptor, DESCRIPTOR)
    fwrite(path_join(fx.modDir, "hello.das"), "options gen2\nmodule hello\ndef hello_value() : int \{\n    return 42\n\}\n")
    fwrite(path_join(fx.dmDir, ".das_module"), DESCRIPTOR_DM)
    fwrite(path_join(fx.brokenDir, ".das_module"), DESCRIPTOR_BROKEN)
    fwrite(path_join(tmp, "use.das"), "options gen2\nrequire manifest_probe/hello\n[export]\ndef main \{\n    print(\"PROBE \{hello_value()\}\\n\")\n\}\n")
    return fx
}

def arms_record_and_replay(t : T?; fx : Fixture) {
    let cmd = fx.cmd
    let manifest = fx.manifest
    t |> run("the first start compiles the descriptor and writes its manifest") @(t : T?) {
        var out : string
        t |> success(report_child(t, "cold", run_child(cmd, out), out, "PROBE 42"), "the require resolves through the compiled descriptor")
        t |> equal("compiled (no manifest), manifest written (1 row(s))", probe_line(out), "the trace says compiled and written:\n{out}")
        t |> success(stat(manifest).is_valid, "the manifest sits beside the descriptor")
        let text = fread(manifest)
        t |> success(text |> starts_with("das_module_manifest\t3\n"), "the manifest opens with its format version: {text}")
        t |> success(find(text, "\ndep\t") >= 0 && find(text, "/daslib/fio.das\t") >= 0, "the manifest stamps the files the descriptor's compile read: {text}")
        t |> success(find(text, "\nnp\tmanifest_probe\thello\t") >= 0, "the manifest carries the recorded row: {text}")
        t |> success(find(text, "\nend\t1\n") >= 0, "the manifest closes with its row count: {text}")
    }
    t |> run("a dynamic-module row records the artifact, the class, the on_error and the (empty) module name") @(t : T?) {
        let text = fread(fx.dmManifest)
        t |> success(find(text, "\ndm\t") >= 0 && find(text, "absent.shared_module\tModule_Absent\t0\t\n") >= 0,
            "the dm row carries five fields, the last empty for an artifact that did not load: {text}")
        t |> success(find(text, "\nend\t1\n") >= 0, "the dm manifest closes with its row count: {text}")
    }
    t |> run("a descriptor that does not compile gets no manifest, and the run goes on") @(t : T?) {
        var out : string
        t |> success(report_child(t, "broken", run_child(cmd, out), out, "PROBE 42"), "the other module's require still resolves")
        t |> equal("compiled with errors, no manifest", probe_line(out, "broken_probe"), "the trace names the failure:\n{out}")
        t |> success(!stat(path_join(fx.brokenDir, ".das_module.manifest")).is_valid, "nothing was written for it")
    }
    t |> run("the second start replays the manifest and never compiles the descriptor") @(t : T?) {
        var out : string
        t |> success(report_child(t, "warm", run_child(cmd, out), out, "PROBE 42"), "the require resolves through the replayed rows")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(out), "the trace says replayed:\n{out}")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(out, "dm_probe"), "the dm row replays too:\n{out}")
        t |> success(count_of(out, "Module_Absent <- ") >= 1, "the replayed dm row retries its dlopen:\n{out}")
    }
    t |> run("an edited descriptor recompiles once and is replayed again after") @(t : T?) {
        fwrite(fx.descriptor, "{DESCRIPTOR}// edited\n")
        var out : string
        t |> success(report_child(t, "edited", run_child(cmd, out), out, "PROBE 42"), "the edited descriptor still resolves the require")
        t |> equal("compiled (descriptor changed), manifest written (1 row(s))", probe_line(out), "the stamp mismatch recompiles and rewrites:\n{out}")
        var again : string
        t |> success(report_child(t, "edited warm", run_child(cmd, again), again, "PROBE 42"), "the rewritten manifest resolves the require")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(again), "the rewritten manifest replays:\n{again}")
    }
}

def arms_rejections(t : T?; fx : Fixture) {
    let cmd = fx.cmd
    let manifest = fx.manifest
    t |> run("a damaged manifest recompiles and is rewritten") @(t : T?) {
        let text = fread(manifest)
        let endAt = find(text, "\nend\t")
        fwrite(manifest, slice(text, 0, endAt + 1))   //! drop the end line - a write that died before its rename
        var out : string
        t |> success(report_child(t, "damaged", run_child(cmd, out), out, "PROBE 42"), "the damaged manifest is not replayed")
        t |> equal("compiled (no end line), manifest written (1 row(s))", probe_line(out), "the trace names the damage:\n{out}")
    }
    t |> run("every other rejection: no row of a manifest the reader cannot vouch for is replayed") @(t : T?) {
        let lines <- manifest_lines(manifest)
        let np = index_of_prefix(lines, "np\t")
        let dep = index_of_prefix(lines, "dep\t")
        let end = length(lines) - 1
        t |> success(np > 6 && dep > 5 && dep < np && (lines[end] |> starts_with("end\t")), "the good manifest is six key lines, dep lines, the np row and end: {join(lines, " | ")}")
        return if (np < 0 || dep < 0)
        expect_rewrite(t, cmd, manifest, join([for (i in range(LINE_DASROOT)); lines[i]], "\n") + "\n", "fewer than 7 lines")
        expect_rewrite(t, cmd, manifest, with_line(lines, LINE_HEADER, "das_module_manifest\t9"), "format version")
        expect_rewrite(t, cmd, manifest, with_line(lines, LINE_STAMP, "stamp\t1"), "stamp line")
        expect_rewrite(t, cmd, manifest, with_line(lines, LINE_DLL, lines[LINE_DLL] == "dll\t0" ? "dll\t1" : "dll\t0"), "binary kind")
        expect_rewrite(t, cmd, manifest, with_line(lines, LINE_ROOT, "rooted\t{fx.modDir}"), "root line")
        expect_rewrite(t, cmd, manifest, with_line(lines, LINE_ROOT, "root\t{fx.modDir}/elsewhere"), "module folder moved")
        expect_rewrite(t, cmd, manifest, with_line(lines, LINE_DASROOT, "dasroot"), "dasroot line")
        expect_rewrite(t, cmd, manifest, with_line(lines, LINE_DASROOT, "dasroot\t{fx.tmp}"), "das root moved")
        expect_rewrite(t, cmd, manifest, with_line(lines, LINE_TARGET, "target"), "target line")
        expect_rewrite(t, cmd, manifest, with_line_inserted(lines, np, ""), "empty line")
        expect_rewrite(t, cmd, manifest, with_line(lines, dep, "dep\t{fx.modDir}/nope.das\t1"), "dep field count")
        expect_rewrite(t, cmd, manifest, with_line(lines, dep, "dep\t{fx.modDir}/nope.das\t1\t0000000000000000"), "dependency missing")
        expect_rewrite(t, cmd, manifest, with_line(lines, dep, "dep\t{fx.modDir}/hello.das\t1\t0000000000000000"), "dependency changed")
        expect_rewrite(t, cmd, manifest, with_line(lines, np, "np\tmanifest_probe\thello"), "np field count")
        expect_rewrite(t, cmd, manifest, with_line(lines, np, "dm\ta\tb"), "dm field count")
        expect_rewrite(t, cmd, manifest, with_line(lines, np, "dm\ta\tb\tx\tc"), "dm on_error field")
        expect_rewrite(t, cmd, manifest, with_line(lines, np, "dm\ta\tb\t3\tc"), "dm on_error field")
        expect_rewrite(t, cmd, manifest, with_line(lines, np, "no_manifest\tx"), "no_manifest field count")
        expect_rewrite(t, cmd, manifest, with_line(lines, np, "xx\ta\tb\tc"), "unknown row kind")
        expect_rewrite(t, cmd, manifest, with_line(lines, end, "end\t2"), "end row count")
        expect_rewrite(t, cmd, manifest, join(lines, "\n") + "\njunk\n", "content after end")
        expect_rewrite(t, cmd, manifest, with_line_inserted(lines, end, "no_manifest"), "no_manifest with rows")
        t |> equal(join(lines, "\n") + "\n", fread(manifest), "the last rewrite restored the good manifest")
    }
    t |> run("a module the descriptor requires is part of the key: editing it recompiles the descriptor") @(t : T?) {
        let helper = path_join(fx.modDir, "helper.das")
        fwrite(helper, HELPER)
        fwrite(fx.descriptor, DESCRIPTOR_DEP)
        var out : string
        t |> success(report_child(t, "dep", run_child(cmd, out), out, "PROBE 42"), "the require resolves through the helper's answer")
        t |> equal("compiled (descriptor changed), manifest written (1 row(s))", probe_line(out), "the dep descriptor compiles once:\n{out}")
        t |> success(find(fread(manifest), "/helper.das\t") >= 0, "the helper is stamped in the manifest, by the path the require resolved: {fread(manifest)}")
        var warm : string
        t |> success(report_child(t, "dep warm", run_child(cmd, warm), warm, "PROBE 42"), "the dep descriptor replays")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(warm), "an unchanged helper replays:\n{warm}")
        fwrite(helper, "{HELPER}// edited\n")
        var edited : string
        t |> success(report_child(t, "dep edited", run_child(cmd, edited), edited, "PROBE 42"), "the require resolves after the helper edit")
        t |> equal("compiled (dependency changed), manifest written (1 row(s))", probe_line(edited), "the helper's stamp mismatch recompiles the descriptor:\n{edited}")
        var again : string
        t |> success(report_child(t, "dep edited warm", run_child(cmd, again), again, "PROBE 42"), "the rewritten manifest resolves the require")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(again), "the rewritten manifest replays:\n{again}")
    }
    t |> run("a cross-compile target is part of the key: a --jit-target run never replays a native manifest") @(t : T?) {
        //! dasOpenGL's descriptor registers its module only when get_cross_platform_name() says emscripten -
        //! an argv input, so a native manifest must not serve a --jit-target run, nor the other way round
        var out : string
        t |> success(report_child(t, "target", run_child("{cmd} -- --jit-target=wasm32-unknown-emscripten", out), out, "PROBE 42"), "the require resolves under the target")
        t |> equal("compiled (compile target), manifest written (1 row(s))", probe_line(out), "the target keys the manifest apart:\n{out}")
        var back : string
        t |> success(report_child(t, "native again", run_child(cmd, back), back, "PROBE 42"), "the native run resolves the require")
        t |> equal("compiled (compile target), manifest written (1 row(s))", probe_line(back), "the native run keys apart from the target's manifest:\n{back}")
    }
}

def arms_opt_out_and_write_failures(t : T?; fx : Fixture) {
    let cmd = fx.cmd
    let manifest = fx.manifest
    t |> run("no_manifest() inside the descriptor makes it run on every start") @(t : T?) {
        fwrite(fx.descriptor, DESCRIPTOR_OPT_OUT)
        var out : string
        t |> success(report_child(t, "opt-out", run_child(cmd, out), out, "PROBE 42"), "the opted-out descriptor resolves the require")
        t |> equal("compiled (descriptor changed), no_manifest recorded", probe_line(out), "the first run records the opt-out:\n{out}")
        let text = fread(manifest)
        t |> success(find(text, "\nno_manifest\n") >= 0 && find(text, "\nnp\t") < 0, "the manifest carries the flag and no rows: {text}")
        var again : string
        t |> success(report_child(t, "opt-out warm", run_child(cmd, again), again, "PROBE 42"), "the opted-out descriptor resolves the require again")
        t |> equal("compiled (no_manifest)", probe_line(again), "every later start compiles it:\n{again}")
    }
    t |> run("a recorded argument the format cannot carry leaves the manifest unwritten") @(t : T?) {
        fwrite(fx.descriptor, DESCRIPTOR_TABBED)
        let before = fread(manifest)
        var out : string
        t |> success(report_child(t, "tabbed", run_child(cmd, out), out, "PROBE 42"), "the descriptor still resolves the require")
        t |> equal("compiled (descriptor changed), manifest not written: a recorded argument contains a tab or newline", probe_line(out), "the trace names the field:\n{out}")
        t |> equal(before, fread(manifest), "the stale manifest is left as it was")
    }
    t |> run("a tree the scan cannot write to just compiles, every start") @(t : T?) {
        fwrite(fx.descriptor, DESCRIPTOR)
        remove_result(manifest)
        mkdir_result(fx.manifestTmp)   //! a directory where the .tmp goes: the create fails on every platform, root or not
        var out : string
        t |> success(report_child(t, "no tmp", run_child(cmd, out), out, "PROBE 42"), "the require resolves without a manifest")
        t |> success(probe_line(out) |> starts_with("compiled (no manifest), manifest not written: cannot create "), "the trace names the create failure:\n{out}")
        rmdir_result(fx.manifestTmp)
        mkdir_result(manifest)   //! a directory where the manifest goes: the rename fails
        var again : string
        t |> success(report_child(t, "no rename", run_child(cmd, again), again, "PROBE 42"), "the require resolves without a manifest")
        t |> success(find(probe_line(again), ", manifest not written: cannot rename ") >= 0, "the trace names the rename failure:\n{again}")
        t |> success(!stat(fx.manifestTmp).is_valid, "the failed rename left no .tmp behind")
        rmdir_result(manifest)
    }
}

//! the arms share one root and run in this order
[test]
def test_descriptor_manifest(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_dm_manifest", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let fx = make_fixture(tmp)
    arms_record_and_replay(t, fx)
    arms_rejections(t, fx)
    arms_opt_out_and_write_failures(t, fx)
    var err : string
    rmdir_rec(tmp, err)
}
