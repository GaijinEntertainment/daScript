options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio
require daslib/cross_context
require _mc_common

def fixture(name : string) : string {
    return "%/tests/module_cache/_fixtures/{name}.das"
}

//! a late require at runtime, with the fixture's compile logged per module
def late_driver() : string {
    return "options gen2\nrequire daslib/cross_context\n\n[export]\ndef main \{\n    var answer = 0\n    var inscope no_access : smart_ptr<FileAccess>\n    using() $(var cop : CodeOfPolicies) \{\n        cop.log_module_compile_time = true\n        require_module_now(\"{fixture("mc_late_mod")}\", no_access, cop) $(var mod : Module?; issues : das_string) \{\n            print(\"LATE LOG \{issues\}\\n\")\n            if (mod != null) \{\n                call_in_context(unsafe(addr(find_macro_context(mod))), \"late_answer\", answer)\n            \}\n        \}\n    \}\n    print(\"LATE \{answer\}\\n\")\n\}\n"
}

def count_prefix(dir : string; prefix : string) : int {
    var n = 0
    fio::dir(dir) $(name) {
        if (name |> starts_with(prefix)) {
            n++
        }
    }
    return n
}

[test]
def test_require_module_now(t : T?) {
    t |> run("a shared module compiles at the call and answers the same module after") @(t : T?) {
        let mod = require_module_now(fixture("mc_late_mod"))
        t |> success(mod != null, "the late require answers a module")
        if (mod == null) {
            return
        }
        t |> success(mod.name == "mc_late_mod", "the module is the fixture's: {mod.name}")
        t |> success(has_module("mc_late_mod"), "the module joined the process")
        t |> success(require_module_now(fixture("mc_late_mod")) == mod, "a second call answers the module already there")
    }
    t |> run("its macro context answers a call by name, the result through a pointer") @(t : T?) {
        let ctx = macro_context_of(fixture("mc_late_mod"))
        t |> success(ctx != null, "the fixture has a macro context")
        if (ctx == null) {
            return
        }
        var answer = 0
        call_in_context(ctx, "late_answer", answer)
        t |> equal(answer, 42)
        var sum = 0
        call_in_context(ctx, "late_add", 20, 22, sum)
        t |> equal(sum, 42)
    }
    t |> run("a file nothing serves, a module that is not shared, and one that does not compile answer null with the reason") @(t : T?) {
        var why : string
        var inscope no_access : smart_ptr<FileAccess>
        using() $(var cop : CodeOfPolicies) {
            require_module_now(fixture("mc_late_absent"), no_access, cop) $(var mod : Module?; issues : das_string) {
                t |> success(mod == null, "an absent file answers null")
                why = string(issues)
            }
            t |> success(find(why, "mc_late_absent") >= 0, "the absent file is named: {why}")
            require_module_now(fixture("mc_late_plain"), no_access, cop) $(var mod : Module?; issues : das_string) {
                t |> success(mod == null, "a module that is not shared answers null")
                why = string(issues)
            }
            t |> success(find(why, "is not shared") >= 0, "the reason names the missing `shared`: {why}")
            require_module_now(fixture("mc_late_broken"), no_access, cop) $(var mod : Module?; issues : das_string) {
                t |> success(mod == null, "a module that does not compile answers null")
                why = string(issues)
            }
            t |> success(find(why, "mc_late_broken") >= 0 && find(why, "return type") >= 0, "the compile error is the reason: {why}")
        }
    }
    t |> run("an explicit module cache keeps a late cache beside it, the second run serves from it, and no cache means none") @(t : T?) {
        var terr : string
        let tmp = create_temp_directory("das_late_require", terr)
        if (empty(tmp)) {
            t |> failure("create_temp_directory: {terr}")
            return
        }
        let script = path_join(tmp, "use_late.das")
        fwrite(script, late_driver())
        let cached = "\"{das_exe()}\" -dasroot \"{get_das_root()}\" -module-cache \"{path_join(tmp, "late.dascache")}\" \"{script}\""
        var out : string
        t |> success(report_child(t, "cold", run_child(cached, out), out, "LATE 42"), "the cold run answers through the late module")
        t |> success(find(out, "compiler took") >= 0 && find(out, "mc_late_mod") >= 0, "the cold run compiles the fixture:\n{out}")
        t |> equal(count_prefix(tmp, "late~mc_late_mod-"), 1, "the late cache sits beside the explicit cache file")
        t |> success(report_child(t, "warm", run_child(cached, out), out, "LATE 42"), "the warm run answers through the late module")
        t |> success(find(out, "cache read took") >= 0 && find(out, "mc_late_mod") >= 0 && find(out, "compiler took") < 0, "the warm run serves the fixture from the late cache:\n{out}")
        let uncached = "\"{das_exe()}\" -dasroot \"{get_das_root()}\" -no-module-cache \"{script}\""
        t |> success(report_child(t, "no cache", run_child(uncached, out), out, "LATE 42"), "the run with no module cache answers too")
        t |> success(find(out, "compiler took") >= 0 && find(out, "cache read took") < 0, "with no module cache the late walk compiles:\n{out}")
        var err : string
        rmdir_rec(tmp, err)
    }
}
