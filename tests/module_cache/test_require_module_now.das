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

// the in-process arms issue their late requires from this [init]: a require after the walk is
// a compile's, and a test body runs after the compile, where the wrapper panics
var late_mod : Module?
var late_second : Module?
var late_answer_out = 0
var late_sum_out = 0
var late_sum3_out = 0
var late_pick_out = 0
var late_pick_why : string
var late_nomacro_mod : Module?
var late_nomacro_ctx : Context?
var late_absent_why : string
var late_plain_why : string
var late_broken_why : string
var late_named_why : string

[init]
def late_probe {
    late_mod = require_module_now(fixture("mc_late_mod"))
    if (late_mod != null) {
        late_second = require_module_now(fixture("mc_late_mod"))
        let ctx = macro_context_of(fixture("mc_late_mod"))
        if (ctx != null) {
            unsafe {
                invoke_in_context(ctx, "late_answer", addr(late_answer_out))
                invoke_in_context(ctx, "late_add", 20, 22, addr(late_sum_out))
                invoke_in_context(ctx, "late_add3", 20, 20, 2, addr(late_sum3_out))
            }
            try {
                unsafe {
                    invoke_in_context(ctx, "late_pick", 7, addr(late_pick_out))
                }
                late_pick_why = "answered {late_pick_out}"
            } recover {
                late_pick_why = this_context().last_exception
            }
        }
    }
    late_nomacro_mod = require_module_now(fixture("mc_late_nomacro"))
    late_nomacro_ctx = macro_context_of(fixture("mc_late_nomacro"))
    var inscope no_access : smart_ptr<FileAccess>
    using() $(var cop : CodeOfPolicies) {
        require_module_now(fixture("mc_late_absent"), no_access, cop) $(var mod : Module?; issues : das_string) {
            late_absent_why = mod == null ? string(issues) : "answered a module"
        }
        require_module_now(fixture("mc_late_plain"), no_access, cop) $(var mod : Module?; issues : das_string) {
            late_plain_why = mod == null ? string(issues) : "answered a module"
        }
        require_module_now(fixture("mc_late_broken"), no_access, cop) $(var mod : Module?; issues : das_string) {
            late_broken_why = mod == null ? string(issues) : "answered a module"
        }
        require_module_now(fixture("mc_late_named"), no_access, cop) $(var mod : Module?; issues : das_string) {
            late_named_why = mod == null ? string(issues) : "answered a module"
        }
    }
}

//! a late require from the driver's [init], with the fixture's compile logged per module
def late_driver() : string {
    return "options gen2\nrequire daslib/cross_context\n\nvar answer = 0\n\n[init]\ndef late_init \{\n    var inscope no_access : smart_ptr<FileAccess>\n    using() $(var cop : CodeOfPolicies) \{\n        cop.log_module_compile_time = true\n        require_module_now(\"{fixture("mc_late_mod")}\", no_access, cop) $(var mod : Module?; issues : das_string) \{\n            print(\"LATE LOG \{issues\}\\n\")\n            if (mod != null) \{\n                unsafe \{\n                    invoke_in_context(addr(find_macro_context(mod)), \"late_answer\", addr(answer))\n                \}\n            \}\n        \}\n    \}\n\}\n\n[export]\ndef main \{\n    print(\"LATE \{answer\}\\n\")\n\}\n"
}

//! the wrapper called from a running script
def running_driver() : string {
    return "options gen2\nrequire daslib/cross_context\n\n[export]\ndef main \{\n    let m = require_module_now(\"{fixture("mc_late_mod")}\")\n    print(\"LATE \{m != null\}\\n\")\n\}\n"
}

//! the block form called from a running script - the builtin's own refusal
def running_block_driver() : string {
    return "options gen2\nrequire daslib/ast\n\n[export]\ndef main \{\n    var inscope no_access : smart_ptr<FileAccess>\n    using() $(var cop : CodeOfPolicies) \{\n        require_module_now(\"{fixture("mc_late_mod")}\", no_access, cop) $(var mod : Module?; issues : das_string) \{\n            print(\"LATE \{mod != null\}\\n\")\n        \}\n    \}\n\}\n"
}

//! a late require issued from a call macro during the requirer's own compile; `tail` follows the call
def macro_driver(tail : string) : string {
    return "options gen2\nrequire {fixture("mc_late_macro_user")}\n\n[export]\ndef main \{\n    print(\"LATE \{late_probe_call()\}\\n\")\n{tail}\}\n"
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
        t |> success(late_mod != null, "the late require answers a module")
        if (late_mod == null) {
            return
        }
        t |> success(late_mod.name == "mc_late_mod", "the module is the fixture's: {late_mod.name}")
        t |> success(has_module("mc_late_mod"), "the module joined the process")
        t |> success(late_second == late_mod, "a second call answers the module already there")
    }
    t |> run("its macro context answers a call by name, the result through a pointer; a name two functions share is an error, not a pick") @(t : T?) {
        t |> equal(late_answer_out, 42)
        t |> equal(late_sum_out, 42)
        t |> equal(late_sum3_out, 42)
        t |> success(find(late_pick_why, "'late_pick' is ambiguous") >= 0, "the shared name is refused: {late_pick_why}")
        t |> equal(late_pick_out, 0, "neither late_pick ran")
    }
    t |> run("a shared module with no macro context is required, and macro_context_of answers null for it") @(t : T?) {
        t |> success(late_nomacro_mod != null && late_nomacro_mod.name == "mc_late_nomacro", "the late require answers the module")
        t |> success(late_nomacro_ctx == null, "no macro context to answer")
    }
    t |> run("a file nothing serves, a module that is not shared, one that does not compile, and one whose name is not its file's answer null with the reason") @(t : T?) {
        t |> success(find(late_absent_why, "mc_late_absent") >= 0, "the absent file is named: {late_absent_why}")
        t |> success(find(late_plain_why, "is not shared") >= 0, "the reason names the missing `shared`: {late_plain_why}")
        t |> success(find(late_broken_why, "mc_late_broken") >= 0 && find(late_broken_why, "return type") >= 0, "the compile error is the reason: {late_broken_why}")
        t |> success(find(late_named_why, "declares module 'mc_late_other'") >= 0 && find(late_named_why, "`module mc_late_named`") >= 0, "the declared name is held to the file's: {late_named_why}")
        t |> success(!has_module("mc_late_other"), "the misnamed module never joined the process")
    }
}

[test]
def test_require_module_now_children(t : T?) {
    t |> run("a late require's modules are records of the host's module cache; a running script's call panics") @(t : T?) {
        var terr : string
        let tmp = create_temp_directory("das_late_require", terr)
        if (empty(tmp)) {
            t |> failure("create_temp_directory: {terr}")
            return
        }
        let script = path_join(tmp, "use_late.das")
        fwrite(script, late_driver())
        let cacheFile = path_join(tmp, "late.dascache")
        let cached = "\"{das_exe()}\" -dasroot \"{get_das_root()}\" -module-cache \"{cacheFile}\" \"{script}\""
        var out : string
        t |> success(report_child(t, "cold", run_child(cached, out), out, "LATE 42"), "the cold run answers through the late module")
        t |> success(find(out, "compiler took") >= 0 && find(out, "mc_late_mod") >= 0, "the cold run compiles the fixture:\n{out}")
        t |> equal(count_prefix(tmp, "late~"), 0, "no cache of the late walk's own sits beside the host's")
        let coldSize = long_length(fread(cacheFile))
        t |> success(report_child(t, "warm", run_child(cached, out), out, "LATE 42"), "the warm run answers through the late module")
        t |> success(find(out, "cache read took") >= 0 && find(out, "mc_late_mod") >= 0 && find(out, "compiler took") < 0, "the warm run serves the fixture from the host's cache:\n{out}")
        t |> equal(long_length(fread(cacheFile)), coldSize, "the warm run rewrites nothing")
        let uncached = "\"{das_exe()}\" -dasroot \"{get_das_root()}\" -no-module-cache \"{script}\""
        t |> success(report_child(t, "no cache", run_child(uncached, out), out, "LATE 42"), "the run with no module cache answers too")
        t |> success(find(out, "compiler took") >= 0 && find(out, "cache read took") < 0, "with no module cache the late walk compiles:\n{out}")
        let running = path_join(tmp, "use_running.das")
        fwrite(running, running_driver())
        var rc = run_child("\"{das_exe()}\" -dasroot \"{get_das_root()}\" -no-module-cache \"{running}\"", out)
        t |> success(rc != 0 && find(out, "no program is compiling") >= 0, "a running script's late require panics with the reason: rc={rc}\n{out}")
        let runningBlock = path_join(tmp, "use_running_block.das")
        fwrite(runningBlock, running_block_driver())
        rc = run_child("\"{das_exe()}\" -dasroot \"{get_das_root()}\" -no-module-cache \"{runningBlock}\"", out)
        t |> success(rc != 0 && find(out, "no program is compiling") >= 0, "the block form from a running script is refused by the builtin itself: rc={rc}\n{out}")
        var err : string
        rmdir_rec(tmp, err)
    }
    t |> run("a late require nested in a compile leaves the host's stream alone: the warm read stays clean, and a failed compile keeps the cache") @(t : T?) {
        var terr : string
        let tmp = create_temp_directory("das_late_nested", terr)
        if (empty(tmp)) {
            t |> failure("create_temp_directory: {terr}")
            return
        }
        let script = path_join(tmp, "use_macro.das")
        fwrite(script, macro_driver(""))
        let cacheFile = path_join(tmp, "nested.dascache")
        let cached = "\"{das_exe()}\" -dasroot \"{get_das_root()}\" -module-cache \"{cacheFile}\" \"{script}\""
        var out : string
        t |> success(report_child(t, "cold", run_child(cached, out), out, "LATE 42"), "the macro's late require answers on the cold run")
        let coldSize = long_length(fread(cacheFile))
        t |> success(report_child(t, "warm", run_child(cached, out), out, "LATE 42"), "the warm run answers")
        t |> success(find(out, "FALLBACK") < 0 && find(out, "module order changed") < 0, "the warm read is not cut by the nested walk's modules:\n{out}")
        t |> equal(long_length(fread(cacheFile)), coldSize, "the warm run rewrites nothing")
        t |> success(report_child(t, "warm again", run_child(cached, out), out, "LATE 42"), "the second warm run answers")
        t |> success(find(out, "FALLBACK") < 0, "the second warm read is clean too:\n{out}")
        fwrite(script, macro_driver("    let y : int = \"not an int\"\n"))
        let rc = run_child(cached, out)
        t |> success(rc != 0, "the broken requirer fails to compile: rc={rc}")
        t |> equal(long_length(fread(cacheFile)), coldSize, "a failed compile after the macro's late require leaves the good cache in place")
        var err : string
        rmdir_rec(tmp, err)
    }
}
