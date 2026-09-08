options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio
require daslib/strings_boost
require _mc_common

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

//! the `DOLLAR N` line the dollar.das child prints: the builtin module's function count
def dollar_count(out : string) : int {
    let marker = "DOLLAR "
    let at = find(out, marker)
    if (at < 0) {
        return -1
    }
    let rest = slice(out, at + length(marker))
    let eol = find(rest, "\n")
    return to_int(eol < 0 ? rest : slice(rest, 0, eol))
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
    exists : string
    hasModule : string
    dollar : string
    nested : string
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
        exists = path_join(tmp, "exists.das"),
        hasModule = path_join(tmp, "has_module.das"),
        dollar = path_join(tmp, "dollar.das"),
        nested = path_join(tmp, "nested.das"),
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
    fwrite(fx.exists, "options gen2\n[export]\ndef main \{\n    print(\"EXISTS \{typeinfo builtin_module_exists({UNIT_TEST_MODULE})\}\\n\")\n\}\n")
    fwrite(fx.hasModule, "options gen2\nrequire daslib/rtti\n[export]\ndef main \{\n    print(\"HAS \{has_module(\"{UNIT_TEST_MODULE}\")\}\\n\")\n\}\n")
    fwrite(fx.dollar, "options gen2\nrequire daslib/rtti\n[export]\ndef main \{\n    program_for_each_registered_module() $(mod) \{\n        if (mod.name == \"$\") \{\n            var n = 0\n            module_for_each_function(mod) $(fn) \{\n                n++\n            \}\n            print(\"DOLLAR \{n\}\\n\")\n        \}\n    \}\n\}\n")
    fwrite(fx.nested, "options gen2\nrequire daslib/ast\n[export]\ndef main \{\n    compile(\"nested\", \"require {UNIT_TEST_MODULE}\\n[export]\\ndef main \\\{\\n\\\}\\n\", CodeOfPolicies()) $(ok; _prog; errors) \{\n        print(\"NESTED \{ok\}\\n\{errors\}\\n\")\n    \}\n\}\n")
    fwrite(fx.guarded, "options gen2\n{guardLine}{guardMain}")
    fwrite(fx.guardedAfter, "options gen2\nrequire {UNIT_TEST_MODULE}\n{guardLine}{guardMain}")
    fwrite(fx.guardedBefore, "options gen2\n{guardLine}require {UNIT_TEST_MODULE}\n{guardMain}")
    fwrite(fx.guardedDep, "options gen2\nrequire deferred_probe/guarded_dep\nrequire {UNIT_TEST_MODULE}\n{guardMain}")
    fwrite(fx.app, "options gen2\nrequire imgui_app\n[export]\ndef main \{\n    print(\"APP\\n\")\n\}\n")
    return fx
}

//! a copy of one of the tree's module folders - descriptor and the artifacts that exist, in
//! either configuration's name - under the fixture's modules, where it shadows the tree's
def copy_module(fx : Fixture; folder : string; artifacts : array<string>) : string {
    let src = path_join(path_join(get_das_root(), "modules"), folder)
    let dst = path_join(path_join(fx.tmp, "modules"), folder)
    var merr : string
    mkdir_rec(dst, merr)
    var cerr : string
    copy_file(path_join(src, ".das_module"), path_join(dst, ".das_module"), true, cerr)
    for (name in artifacts) {
        for (suffix in [".shared_module", "_debug.shared_module"]) {
            let f = path_join(src, "{name}{suffix}")
            if (stat(f).is_valid) {
                copy_file(f, path_join(dst, "{name}{suffix}"), true, cerr)
            }
        }
    }
    dir(src) $(name) {                          // a library an artifact loads from beside itself
        let ext = extension(name)
        if (ext == ".dylib" || ext == ".so" || ext == ".dll") {
            copy_file(path_join(src, name), path_join(dst, name), true, cerr)
        }
    }
    return dst
}

//! whether the cold run loaded the copied artifact - a build whose artifacts find their
//! libraries relative to the tree cannot load a copy, and the arms on copies say so and return
def copy_loaded(out : string; dir : string; artifact : string) : bool {
    for (suffix in [".shared_module", "_debug.shared_module"]) {
        if (find(out, "{path_join(dir, artifact)}{suffix} : loaded") >= 0) {
            return true
        }
    }
    return false
}

def child_cmd(fx : Fixture; script : string; flags : string = ""; cache : string = "-no-module-cache") : string {
    return "{trace_prefix()}\"{das_exe()}\" -dasroot \"{get_das_root()}\" {cache} -project_root \"{fx.tmp}\" {flags} \"{script}\""
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
        t |> success(report_child(t, "cold", run_child(child_cmd(fx, fx.guarded), out), out, "GUARD true"), "the cold start runs, and the guard takes its target")
        t |> equal("compiled (no manifest), manifest written (1 row(s))", descriptor_line(out, UNIT_TEST_FOLDER), "the copied descriptor compiles and records:\n{out}")
        t |> success(find(out, "{UNIT_TEST_CLASS} <- ") >= 0 && find(out, ": loaded") >= 0, "the recording start loads the module:\n{out}")
    }
    t |> run("a warm start defers the module and loads nothing for a program that requires nothing") @(t : T?) {
        var out : string
        t |> success(report_child(t, "warm", run_child(child_cmd(fx, fx.plain), out), out, "PLAIN"), "the warm start runs the program")
        let line = descriptor_line(out, UNIT_TEST_FOLDER)
        t |> success(line |> starts_with("replayed 1 row(s) in ") && find(line, ", deferred 1)") >= 0, "the descriptor replays its one row deferred:\n{out}")
        t |> equal(-1, find(out, "{UNIT_TEST_CLASS} <- "), "nothing loads the shared module:\n{out}")
    }
    t |> run("the first require naming a deferred module loads it, and the program calls into it") @(t : T?) {
        var out : string
        t |> success(report_child(t, "uses", run_child(child_cmd(fx, fx.uses), out), out, "LENGTH 5"), "the program calls the module's function")
        t |> success(find(out, "[module] require {UNIT_TEST_MODULE}: loading the deferred {UNIT_TEST_CLASS}") >= 0, "the trace names the require that loaded it:\n{out}")
        t |> success(find(out, "{UNIT_TEST_CLASS} <- ") >= 0 && find(out, ": loaded") >= 0, "the shared module loads:\n{out}")
    }
    t |> run("builtin_module_exists loads a waiting module, with no require naming it") @(t : T?) {
        var out : string
        t |> success(report_child(t, "exists", run_child(child_cmd(fx, fx.exists), out), out, "EXISTS true"), "the build has the module")
        t |> success(find(out, "[module] require {UNIT_TEST_MODULE}: loading the deferred {UNIT_TEST_CLASS}") >= 0, "the question loaded it:\n{out}")
    }
    t |> run("has_module answers true for a waiting module and loads nothing") @(t : T?) {
        var out : string
        t |> success(report_child(t, "has_module", run_child(child_cmd(fx, fx.hasModule), out), out, "HAS true"), "the build has the module")
        t |> equal(-1, find(out, "{UNIT_TEST_CLASS} <- "), "nothing loads the shared module:\n{out}")
    }
    t |> run("a lazy start and an eager start build the same builtin module") @(t : T?) {
        var lazy : string
        t |> success(report_child(t, "dollar lazy", run_child(child_cmd(fx, fx.dollar), lazy), lazy, "DOLLAR "), "the lazy start counts")
        var eager : string
        t |> success(report_child(t, "dollar eager", run_child(child_cmd(fx, fx.dollar, "-ignore-manifest"), eager), eager, "DOLLAR "), "the eager start counts")
        t |> success(dollar_count(lazy) > 0, "the count is a count:\n{lazy}")
        t |> equal(dollar_count(lazy), dollar_count(eager), "a C++ module's load registers nothing into $:\nlazy:\n{lazy}\neager:\n{eager}")
    }
    t |> run("a compile of a string, which walks no prerequisites, meets the waiting module at its require") @(t : T?) {
        var out : string
        t |> success(report_child(t, "nested", run_child(child_cmd(fx, fx.nested), out), out, "NESTED true"), "the nested program compiles")
        t |> success(find(out, "[module] require {UNIT_TEST_MODULE}: loading the deferred {UNIT_TEST_CLASS}") >= 0, "the parser's require loaded it:\n{out}")
    }
}

def arms_guards_and_caches(t : T?; fx : Fixture) {
    t |> run("a require guard loads a waiting module, whatever else the compile requires") @(t : T?) {
        var alone : string
        t |> success(report_child(t, "guarded", run_child(child_cmd(fx, fx.guarded), alone), alone, "GUARD true"), "the guard alone takes its target")
        t |> success(find(alone, "[module] require {UNIT_TEST_MODULE}: loading the deferred {UNIT_TEST_CLASS}") >= 0, "the guard loaded the module:\n{alone}")
        var after : string
        t |> success(report_child(t, "guarded after", run_child(child_cmd(fx, fx.guardedAfter), after), after, "GUARD true"), "a require above the guard")
        var before : string
        t |> success(report_child(t, "guarded before", run_child(child_cmd(fx, fx.guardedBefore), before), before, "GUARD true"), "a require below the guard")
        var dep : string
        t |> success(report_child(t, "guarded dep", run_child(child_cmd(fx, fx.guardedDep), dep), dep, "GUARD true"), "a guard inside a module walked before the entry's own require")
    }
    t |> run("-ignore-manifest compiles every descriptor, loads every C++ module on start, and writes no manifest") @(t : T?) {
        remove_result(fx.manifest)
        remove_result(fx.utManifest)
        var out : string
        t |> success(report_child(t, "ignore", run_child(child_cmd(fx, fx.plain, "-ignore-manifest"), out), out, "PLAIN"), "the program runs")
        t |> equal("compiled (manifests ignored)", descriptor_line(out, UNIT_TEST_FOLDER), "the copied descriptor compiles:\n{out}")
        t |> equal("compiled (manifests ignored)", descriptor_line(out, "deferred_probe"), "the pure-das descriptor compiles:\n{out}")
        t |> success(find(out, "{UNIT_TEST_CLASS} <- ") >= 0 && find(out, ": loaded") >= 0, "the shared module loads on start:\n{out}")
        t |> success(!stat(fx.manifest).is_valid && !stat(fx.utManifest).is_valid, "no manifest was written")
    }
    t |> run("a deferred row whose artifact is gone brings every deferred module in, and the require fails as on a cold start") @(t : T?) {
        var warm : string
        t |> success(report_child(t, "rewarm", run_child(child_cmd(fx, fx.plain), warm), warm, "PLAIN"), "a cold start rewrites the manifest")
        let text = fread(fx.utManifest)
        t |> success(find(text, "\ndm\t") >= 0, "the manifest carries the dm row: {text}")
        fwrite(fx.utManifest, replace(text, UNIT_TEST_ARTIFACT, "absent"))
        var out : string
        let rc = run_child(child_cmd(fx, fx.uses), out)
        t |> success(rc != 0 && find(out, "LENGTH") < 0, "the program does not run:\n{out}")
        t |> success(find(out, "[module] require {UNIT_TEST_MODULE}: loading the deferred {UNIT_TEST_CLASS}") >= 0, "the require asks for the row:\n{out}")
        t |> success(find(out, "[module] loading every deferred module (") >= 0, "the failed load brings the rest in:\n{out}")
        t |> success(find(out, "error[20605]: missing prerequisite '{UNIT_TEST_MODULE}'") >= 0, "the require fails with the missing-module error:\n{out}")
        remove_result(fx.utManifest)
    }
    t |> run("a module cache written by an eager start serves a lazy start, and -log-compile-time shows the reads and the timeline") @(t : T?) {
        let cache = "-module-cache \"{path_join(fx.tmp, "mc.bin")}\""
        var cold : string
        t |> success(report_child(t, "cold cache", run_child(child_cmd(fx, fx.plain, "-ignore-manifest -log-compile-time", cache), cold), cold, "PLAIN"), "the eager start writes the cache")
        t |> success(find(cold, "startup: main total") >= 0 && find(cold, "\tmodule scan") >= 0 && find(cold, "\tshutdown ") >= 0, "the timeline prints its laps:\n{cold}")
        var warm : string
        t |> success(report_child(t, "warm cache", run_child(child_cmd(fx, fx.plain, "-log-compile-time", cache), warm), warm, "PLAIN"), "the lazy start, which loads no C++ module, runs")
        t |> equal(-1, find(warm, "cumulative hash"), "a builtin module's hash does not depend on the loaded set:\n{warm}")
        t |> success(find(warm, "cache read took") >= 0, "the lazy start reads the eager start's records:\n{warm}")
    }
    t |> run("a dastest stream written where the module had loaded is read where nothing required it yet") @(t : T?) {
        let testsDir = path_join(fx.tmp, "tests")
        var merr : string
        mkdir_rec(testsDir, merr)
        fwrite(path_join(testsDir, "test_uses.das"), "options gen2\nrequire dastest/testing_boost public\nrequire {UNIT_TEST_MODULE}\n[test]\ndef test_uses(t : T?) \{\n    t |> equal(5l, test_string_arg_length(\"hello\"))\n\}\n")
        let stream = path_join(fx.tmp, "tests.ser")
        let dastest = path_join(get_das_root(), "dastest/dastest.das")
        var wrote : string
        t |> success(report_child(t, "ser", run_child("{child_cmd(fx, dastest)} -- --test \"{testsDir}\" --ser \"{stream}\"", wrote), wrote, "Serialized 1 programs"), "the writer serializes the test program")
        var read : string
        t |> success(report_child(t, "deser", run_child("{child_cmd(fx, dastest)} -- --test \"{testsDir}\" --deser \"{stream}\"", read), read, "1 tests, 0 error(s)"), "the reader runs the test from the stream")
        t |> success(find(read, "[module] require {UNIT_TEST_MODULE}: loading the deferred {UNIT_TEST_CLASS}") >= 0, "the reader loaded the module the stream names:\n{read}")
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
        t |> success(report_child(t, "app", run_child(child_cmd(fx, fx.app), out), out, "APP"), "the program compiles against imgui_app")
        t |> success(find(out, "[module] require imgui_app: loading the deferred Module_imgui_app") >= 0, "imgui_app loads at its require:\n{out}")
        t |> success(find(out, "[module] loading every deferred module (") >= 0, "its initDependencies asks for glfw and imgui, so the rest loads:\n{out}")
    }
    t |> run("a half-warm tree - a descriptor compiled cold beside replayed ones - initializes by bringing every deferred module in") @(t : T?) {
        let imguiDir = copy_module(fx, "dasImgui", ["dasModuleImgui", "imguiApp", "imguiAppHeadless"])
        copy_module(fx, "dasGlfw", ["dasModuleGlfw"])
        var cold : string
        t |> success(report_child(t, "half-warm cold", run_child(child_cmd(fx, fx.plain), cold), cold, "PLAIN"), "the copies record their manifests")
        if (!copy_loaded(cold, imguiDir, "imguiApp")) {
            to_log(LOG_INFO, "test_deferred_modules: the copied imguiApp does not load from {imguiDir} - its libraries sit outside the copy - so the half-warm arm has nothing to observe\n")
            return
        }
        remove_result(path_join(imguiDir, ".das_module.manifest"))
        var out : string
        t |> success(report_child(t, "half-warm", run_child(child_cmd(fx, fx.plain), out), out, "PLAIN"), "the program runs with imgui_app loaded on start and glfw waiting")
        if (!copy_loaded(out, imguiDir, "imguiApp")) {
            to_log(LOG_INFO, "test_deferred_modules: the copied imguiApp did not load on the half-warm start - its libraries sit outside the copy - so initialize had nothing to bring in:\n{out}\n")
            return
        }
        t |> success(find(out, "[module] loading every deferred module (") >= 0, "initialize brings the deferred glfw in:\n{out}")
    }
}

//! the arms share one root and run in this order
[test]
def test_deferred_modules(t : T?) {
    if (!das_is_dll_build()) {
        to_log(LOG_INFO, "test_deferred_modules: {das_exe()} is a static build - its descriptors register no shared module, so it defers nothing\n")
        return
    }
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
    arms_guards_and_caches(t, fx)
    arm_fallback(t, fx)
    var err : string
    rmdir_rec(tmp, err)
}
