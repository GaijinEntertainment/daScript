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

//! the child's environment, as a command prefix: the scan trace on, so the trace names each
//! descriptor's verdict and each shared-module load
def trace_prefix() : string {
    return get_platform_name() == "windows" ? "set \"DAS_TRACE_MODULE_LOAD=1\"&& " : "DAS_TRACE_MODULE_LOAD=1 "
}

//! stderr joins stdout: the scan trace goes through the logger
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

//! the whole scan trace line for a module's descriptor, timing clause included
def descriptor_line(out : string; mod : string) : string {
    let marker = "{mod}/.das_module: "
    let at = find(out, marker)
    if (at < 0) {
        return ""
    }
    let rest = slice(out, at + length(marker))
    let eol = find(rest, "\n")
    return eol < 0 ? rest : slice(rest, 0, eol)
}

let UNIT_TEST_MODULE = "UnitTest"
let UNIT_TEST_CLASS = "Module_UnitTest"
let UNIT_TEST_FOLDER = "dasUnitTest"
let UNIT_TEST_ARTIFACT = "dasModuleUnitTest"

//! the tree's UnitTest shared module, in whichever configuration this binary was built - absent
//! in a static build, where nothing is deferred and this test has nothing to observe
def unit_test_artifact() : string {
    let dir = path_join(path_join(get_das_root(), "modules"), UNIT_TEST_FOLDER)
    for (name in ["{UNIT_TEST_ARTIFACT}.shared_module", "{UNIT_TEST_ARTIFACT}_debug.shared_module"]) {
        let candidate = path_join(dir, name)
        if (stat(candidate).is_valid) {
            return candidate
        }
    }
    return ""
}

let DESCRIPTOR = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    register_native_path(\"deferred_probe\", \"hello\", \"\{project_path\}/hello.das\")\n    register_native_path(\"deferred_probe\", \"guarded_dep\", \"\{project_path\}/guarded_dep.das\")\n\}\n"

//! a project root holding a copy of the tree's dasUnitTest - descriptor and artifact - so the
//! copy's manifest is the test's to make cold or warm (the copy shadows the tree's), a pure-das
//! module, and the driver scripts the arms spawn
struct Fixture {
    tmp : string
    manifest : string
    utManifest : string
    plain : string
    uses : string
    guarded : string
    guardedAfter : string
    guardedBefore : string
    guardedDep : string
    app : string
}

def make_fixture(tmp : string; artifact : string) : Fixture {
    let modules = path_join(tmp, "modules")
    let modDir = path_join(modules, "deferred_probe")
    let utDir = path_join(modules, UNIT_TEST_FOLDER)
    var merr : string
    mkdir_rec(modDir, merr)
    mkdir_rec(utDir, merr)
    fwrite(path_join(modDir, ".das_module"), DESCRIPTOR)
    fwrite(path_join(modDir, "hello.das"), "options gen2\nmodule hello\ndef hello_value() : int \{\n    return 42\n\}\n")
    fwrite(path_join(modDir, "guarded_dep.das"), "options gen2\nmodule guarded_dep\nrequire ?{UNIT_TEST_MODULE} deferred_probe/hello public\n")
    let treeUt = path_join(path_join(get_das_root(), "modules"), UNIT_TEST_FOLDER)
    var cerr : string
    copy_file(path_join(treeUt, ".das_module"), path_join(utDir, ".das_module"), true, cerr)
    copy_file(artifact, path_join(utDir, base_name(artifact)), true, cerr)
    let fx = Fixture(
        tmp = tmp,
        manifest = path_join(modDir, ".das_module.manifest"),
        utManifest = path_join(utDir, ".das_module.manifest"),
        plain = path_join(tmp, "plain.das"),
        uses = path_join(tmp, "uses.das"),
        guarded = path_join(tmp, "guarded.das"),
        guardedAfter = path_join(tmp, "guarded_after.das"),
        guardedBefore = path_join(tmp, "guarded_before.das"),
        guardedDep = path_join(tmp, "guarded_dep_driver.das"),
        app = path_join(tmp, "app.das")
    )
    let guardLine = "require ?{UNIT_TEST_MODULE} deferred_probe/hello\n"
    let guardMain = "[export]\ndef main \{\n    print(\"GUARD \{typeinfo module_exists(hello)\}\\n\")\n\}\n"
    fwrite(fx.plain, "options gen2\n[export]\ndef main \{\n    print(\"PLAIN\\n\")\n\}\n")
    fwrite(fx.uses, "options gen2\nrequire {UNIT_TEST_MODULE}\n[export]\ndef main \{\n    print(\"LENGTH \{test_string_arg_length(\"hello\")\}\\n\")\n\}\n")
    fwrite(fx.guarded, "options gen2\n{guardLine}{guardMain}")
    fwrite(fx.guardedAfter, "options gen2\nrequire {UNIT_TEST_MODULE}\n{guardLine}{guardMain}")
    fwrite(fx.guardedBefore, "options gen2\n{guardLine}require {UNIT_TEST_MODULE}\n{guardMain}")
    fwrite(fx.guardedDep, "options gen2\nrequire deferred_probe/guarded_dep\nrequire {UNIT_TEST_MODULE}\n{guardMain}")
    fwrite(fx.app, "options gen2\nrequire imgui_app\n[export]\ndef main \{\n    print(\"APP\\n\")\n\}\n")
    return fx
}

def child_cmd(fx : Fixture; script : string; flags : string = "") : string {
    return "{trace_prefix()}\"{das_exe()}\" -dasroot \"{get_das_root()}\" -no-module-cache -project_root \"{fx.tmp}\" {flags} \"{script}\""
}

def has_shared_modules(dir : string; names : array<string>) : bool {
    for (name in names) {
        var found = false
        for (suffix in [".shared_module", "_debug.shared_module"]) {
            found ||= stat(path_join(dir, "{name}{suffix}")).is_valid
        }
        if (!found) {
            return false
        }
    }
    return true
}

def arms_deferred(t : T?; fx : Fixture) {
    t |> run("a cold start loads the module to record its name, and a guard reads it available") @(t : T?) {
        var out : string
        t |> success(report_child(t, "cold", run(child_cmd(fx, fx.guarded), out), out, "GUARD true"), "the cold start runs, and the guard takes its target")
        t |> equal("compiled (no manifest), manifest written (1 row(s))", descriptor_line(out, UNIT_TEST_FOLDER), "the copied descriptor compiles and records:\n{out}")
        t |> success(find(out, "{UNIT_TEST_CLASS} <- ") >= 0 && find(out, ": loaded") >= 0, "the recording start loads the module:\n{out}")
    }
    t |> run("a warm start defers the module and loads nothing for a program that requires nothing") @(t : T?) {
        var out : string
        t |> success(report_child(t, "warm", run(child_cmd(fx, fx.plain), out), out, "PLAIN"), "the warm start runs the program")
        let line = descriptor_line(out, UNIT_TEST_FOLDER)
        t |> success(line |> starts_with("replayed 1 row(s) in ") && find(line, ", deferred 1)") >= 0, "the descriptor replays its one row deferred:\n{out}")
        t |> equal(-1, find(out, "{UNIT_TEST_CLASS} <- "), "nothing loads the shared module:\n{out}")
    }
    t |> run("the first require naming a deferred module loads it, and the program calls into it") @(t : T?) {
        var out : string
        t |> success(report_child(t, "uses", run(child_cmd(fx, fx.uses), out), out, "LENGTH 5"), "the program calls the module's function")
        t |> success(find(out, "[module] require {UNIT_TEST_MODULE}: loading the deferred {UNIT_TEST_CLASS}") >= 0, "the trace names the require that loaded it:\n{out}")
        t |> success(find(out, "{UNIT_TEST_CLASS} <- ") >= 0 && find(out, ": loaded") >= 0, "the shared module loads:\n{out}")
    }
    t |> run("a require guard loads a waiting module, whatever else the compile requires") @(t : T?) {
        var alone : string
        t |> success(report_child(t, "guarded", run(child_cmd(fx, fx.guarded), alone), alone, "GUARD true"), "the guard alone takes its target")
        t |> success(find(alone, "[module] require {UNIT_TEST_MODULE}: loading the deferred {UNIT_TEST_CLASS}") >= 0, "the guard loaded the module:\n{alone}")
        var after : string
        t |> success(report_child(t, "guarded after", run(child_cmd(fx, fx.guardedAfter), after), after, "GUARD true"), "a require above the guard")
        var before : string
        t |> success(report_child(t, "guarded before", run(child_cmd(fx, fx.guardedBefore), before), before, "GUARD true"), "a require below the guard")
        var dep : string
        t |> success(report_child(t, "guarded dep", run(child_cmd(fx, fx.guardedDep), dep), dep, "GUARD true"), "a guard inside a module walked before the entry's own require")
    }
    t |> run("-ignore-manifest compiles every descriptor, loads every C++ module on start, and writes no manifest") @(t : T?) {
        remove_result(fx.manifest)
        remove_result(fx.utManifest)
        var out : string
        t |> success(report_child(t, "ignore", run(child_cmd(fx, fx.plain, "-ignore-manifest"), out), out, "PLAIN"), "the program runs")
        t |> equal("compiled (manifests ignored)", descriptor_line(out, UNIT_TEST_FOLDER), "the copied descriptor compiles:\n{out}")
        t |> equal("compiled (manifests ignored)", descriptor_line(out, "deferred_probe"), "the pure-das descriptor compiles:\n{out}")
        t |> success(find(out, "{UNIT_TEST_CLASS} <- ") >= 0 && find(out, ": loaded") >= 0, "the shared module loads on start:\n{out}")
        t |> success(!stat(fx.manifest).is_valid && !stat(fx.utManifest).is_valid, "no manifest was written")
    }
}

def arm_fallback(t : T?; fx : Fixture) {
    let modules = path_join(get_das_root(), "modules")
    let imgui = path_join(modules, "dasImgui")
    if (!has_shared_modules(imgui, ["imguiApp", "dasModuleImgui"]) || !has_shared_modules(path_join(modules, "dasGlfw"), ["dasModuleGlfw"])) {
        to_log(LOG_INFO, "test_deferred_modules: no imgui_app, imgui and glfw shared modules in {modules} - the fallback arm has nothing to observe\n")
        return
    }
    t |> run("a module whose C++ dependencies are deferred too brings every deferred module in") @(t : T?) {
        var out : string
        t |> success(report_child(t, "app", run(child_cmd(fx, fx.app), out), out, "APP"), "the program compiles against imgui_app")
        t |> success(find(out, "[module] require imgui_app: loading the deferred Module_imgui_app") >= 0, "imgui_app loads at its require:\n{out}")
        t |> success(find(out, "[module] loading every deferred module (") >= 0, "its initDependencies asks for glfw and imgui, so the rest loads:\n{out}")
    }
}

//! the arms share one root and run in this order
[test]
def test_deferred_modules(t : T?) {
    let artifact = unit_test_artifact()
    if (empty(artifact)) {
        to_log(LOG_INFO, "test_deferred_modules: no {UNIT_TEST_ARTIFACT}.shared_module under {get_das_root()}/modules/{UNIT_TEST_FOLDER} - a static build defers nothing\n")
        return
    }
    var terr : string
    let tmp = create_temp_directory("das_deferred_modules", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let fx = make_fixture(tmp, artifact)
    arms_deferred(t, fx)
    arm_fallback(t, fx)
    var err : string
    rmdir_rec(tmp, err)
}
