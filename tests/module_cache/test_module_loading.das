options gen2

require dastest/testing_boost public
require strings
require daslib/fio
require daslib/strings_boost
require daslib/module_group
require _mc_common
require daslib/cross_context
require _mc_global_offset_solid

// alpha joins probe_group under a guard the build has and path_group
// under a path guard the fixture has; beta joins probe_group
// unguarded and path_group under a path it lacks; delta joins under a
// guard the build lacks, with no file behind it, and registers
// alpha's row a second time
let DESCRIPTOR_ALPHA = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    register_native_path(\"grp_alpha\", \"alpha\", \"\{project_path\}/alpha.das\")\n    register_module_group(\"probe_group\", \"grp_alpha/alpha\", \"math\")\n    register_module_group(\"path_group\", \"grp_alpha/alpha\", \"grp_beta/beta\")\n\}\n"
let DESCRIPTOR_DELTA = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    register_module_group(\"probe_group\", \"grp_delta/delta\", \"zz_absent_guard\")\n    register_module_group(\"probe_group\", \"grp_alpha/alpha\", \"math\")\n\}\n"
let DESCRIPTOR_BETA = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    register_native_path(\"grp_beta\", \"beta\", \"\{project_path\}/beta.das\")\n    register_module_group(\"probe_group\", \"grp_beta/beta\")\n    register_module_group(\"path_group\", \"grp_beta/beta\", \"grp_nowhere/none\")\n\}\n"
let DESCRIPTOR_EMPTY = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    register_module_group(\"probe_group\", \"\")\n\}\n"
let DESCRIPTOR_BETA_OUTSIDE = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    register_native_path(\"grp_beta\", \"beta\", \"\{project_path\}/beta.das\")\n\}\n"
let DESCRIPTOR_GAMMA = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    register_module_group(\"broken_group\", \"grp_gamma/missing\")\n\}\n"

let ALPHA = "options gen2\nmodule alpha\ndef alpha_value() : int \{\n    return 1\n\}\ndef probe_entry(k : int) \{\n    print(\"ENTRY alpha \{k\}\\n\")\n\}\n"
let BETA = "options gen2\nmodule beta\ndef beta_value() : int \{\n    return 10\n\}\ndef probe_entry(k : int) \{\n    print(\"ENTRY beta \{k\}\\n\")\n\}\n"
let CALL_GROUP = "options gen2\nrequire daslib/module_group\nrequire [probe_group]\n[export]\ndef main \{\n    call_module_group(\"probe_group\", \"probe_entry\", 7)\n    print(\"PROBE done\\n\")\n\}\n"
let CALL_GROUP_MISSING = "options gen2\nrequire daslib/module_group\nrequire [probe_group]\n[export]\ndef main \{\n    call_module_group(\"probe_group\", \"no_such_entry\")\n\}\n"
let CALL_GROUP_BAD_ARGS = "options gen2\nrequire daslib/module_group\nrequire [probe_group]\n[export]\ndef main \{\n    call_module_group(\"probe_group\")\n\}\n"
let CALL_GROUP_SIDE_EFFECT = "options gen2\nrequire daslib/module_group\nrequire [probe_group]\nvar counter = 0\ndef bump() : int \{\n    counter++\n    return counter\n\}\n[export]\ndef main \{\n    call_module_group(\"probe_group\", \"probe_entry\", bump())\n    print(\"PROBE \{counter\}\\n\")\n\}\n"
let CALL_PATH_GROUP = "options gen2\nrequire daslib/module_group\nrequire [path_group]\n[export]\ndef main \{\n    call_module_group(\"path_group\", \"probe_entry\", 3)\n    print(\"PROBE done\\n\")\n\}\n"
let CALL_GROUP_SHADOW = "options gen2\nrequire daslib/module_group\nrequire [probe_group]\nvar counter = 0\ndef bump() : int \{\n    counter++\n    return counter\n\}\n[export]\ndef main \{\n    let _cmg_arg_0 = 5\n    call_module_group(\"probe_group\", \"probe_entry\", bump())\n    print(\"PROBE \{counter\} \{_cmg_arg_0\}\\n\")\n\}\n"
let MID_PUBLIC = "options gen2\nmodule mid\nrequire [probe_group] public\n"
let MID_PUBLIC_TAB = "options gen2\nmodule mid_tab\nrequire [probe_group]\tpublic\n"
let MID_PRIVATE = "options gen2\nmodule mid_private\nrequire [probe_group]\n"

def driver(requires : string; expr : string) : string =>
    "options gen2\n{requires}\n[export]\ndef main \{\n    print(\"PROBE \{{expr}\}\\n\")\n\}\n"

struct GroupsFixture {
    tmp : string
    alphaDir : string
    betaDir : string
    cmdPrefix : string
}

def make_groups_fixture(tmp : string) : GroupsFixture {
    let modules = path_join(tmp, "modules")
    let alphaDir = path_join(modules, "grp_alpha")
    let betaDir = path_join(modules, "grp_beta")
    let gammaDir = path_join(modules, "grp_gamma")
    let deltaDir = path_join(modules, "grp_delta")
    var merr : string
    mkdir_rec(alphaDir, merr)
    mkdir_rec(betaDir, merr)
    mkdir_rec(gammaDir, merr)
    mkdir_rec(deltaDir, merr)
    fwrite(path_join(alphaDir, ".das_module"), DESCRIPTOR_ALPHA)
    fwrite(path_join(alphaDir, "alpha.das"), ALPHA)
    fwrite(path_join(betaDir, ".das_module"), DESCRIPTOR_BETA)
    fwrite(path_join(betaDir, "beta.das"), BETA)
    fwrite(path_join(gammaDir, ".das_module"), DESCRIPTOR_GAMMA)
    fwrite(path_join(deltaDir, ".das_module"), DESCRIPTOR_DELTA)
    fwrite(path_join(tmp, "mid.das"), MID_PUBLIC)
    fwrite(path_join(tmp, "mid_tab.das"), MID_PUBLIC_TAB)
    fwrite(path_join(tmp, "mid_private.das"), MID_PRIVATE)
    return GroupsFixture(
        tmp = tmp,
        alphaDir = alphaDir,
        betaDir = betaDir,
        cmdPrefix = "{trace_prefix()}\"{das_exe()}\" -dasroot \"{get_das_root()}\" -project_root \"{tmp}\""
    )
}

def write_driver(fx : GroupsFixture; name : string; requires : string; expr : string) : string {
    let path = path_join(fx.tmp, name)
    fwrite(path, driver(requires, expr))
    return path
}

def run_driver(fx : GroupsFixture; script : string; var out : string&;
               cache : string = "-no-module-cache") : int =>
    run_child("{fx.cmdPrefix} {cache} \"{script}\"", out)

//! run_driver, reported as run_child_reported reports: the run is its
//! own statement, never an argument beside the `out` it writes
def run_driver_reported(t : T?; phase : string; fx : GroupsFixture; script : string;
                        var out : string&; marker : string;
                        cache : string = "-no-module-cache") : bool {
    let rc = run_driver(fx, script, out, cache)
    return report_child(t, phase, rc, out, marker)
}

//! the scan trace line for a module's descriptor; a replay verdict
//! keeps its deferred count and drops the two timings
def descriptor_verdict(out : string; mod : string) : string {
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


def arms_expand(t : T?; fx : GroupsFixture) {
    let useGroup = write_driver(fx, "use_group.das", "require [probe_group]", "alpha_value() + beta_value()")
    t |> run("a cold start compiles the descriptors and the group brings both members in, a member row's guard the build lacks left out") @(t : T?) {
        var out : string
        t |> success(run_driver_reported(t, "cold", fx, useGroup, out, "PROBE 11"), "both members resolve")
        t |> equal("compiled (no manifest), manifest written (3 row(s))", descriptor_verdict(out, "grp_alpha"), "alpha's manifest carries its path row and its two group rows:\n{out}")
        let manifest = fread(path_join(fx.alphaDir, manifest_file_name()))
        t |> success(find(manifest, "\ngrp\tprobe_group\tgrp_alpha/alpha\tmath\n") >= 0, "the group row is written with its guard:\n{manifest}")
        t |> success(find(out, "grp_delta/delta") < 0, "delta's row, guarded on a module the build lacks, adds nothing and resolves nothing:\n{out}")
    }
    t |> run("a warm start replays the group rows and the group still brings both members in") @(t : T?) {
        var out : string
        t |> success(run_driver_reported(t, "warm", fx, useGroup, out, "PROBE 11"), "both members resolve from the replayed rows")
        t |> equal("replayed 3 row(s) (deferred 0)", descriptor_verdict(out, "grp_alpha"), "alpha's descriptor replays:\n{out}")
        t |> equal("replayed 3 row(s) (deferred 0)", descriptor_verdict(out, "grp_beta"), "beta's descriptor replays:\n{out}")
        t |> equal("replayed 2 row(s) (deferred 0)", descriptor_verdict(out, "grp_delta"), "delta's guarded row and its duplicate of alpha's replay:\n{out}")
    }
    t |> run("`require[group]` with no space, and the group name on its own line, are the same require") @(t : T?) {
        var out : string
        let script = write_driver(fx, "use_nospace.das", "require[probe_group]", "alpha_value() + beta_value()")
        t |> success(run_driver_reported(t, "no space", fx, script, out, "PROBE 11"), "the collector and the parser read the spelling alike")
        let multi = write_driver(fx, "use_multiline.das", "require [\n    probe_group\n]", "alpha_value() + beta_value()")
        t |> success(run_driver_reported(t, "multi-line", fx, multi, out, "PROBE 11"), "a newline inside the brackets is nothing to the walk, as to the parser")
    }
    t |> run("a member row's path guard is resolved as the require's would be, in the walk, the parser and the calls") @(t : T?) {
        var out : string
        let taken = write_driver(fx, "use_path_group.das", "require [path_group]", "alpha_value()")
        t |> success(run_driver_reported(t, "path guard taken", fx, taken, out, "PROBE 1"), "alpha's row, guarded on a path the fixture has, joins")
        let dropped = write_driver(fx, "use_path_group_dropped.das", "require [path_group]", "beta_value()")
        let rc = run_driver(fx, dropped, out)
        t |> success(rc != 0 && find(out, "beta_value") >= 0, "beta's row, guarded on a path the fixture lacks, is dropped: rc={rc}\n{out}")
        let calls = path_join(fx.tmp, "use_path_calls.das")
        fwrite(calls, CALL_PATH_GROUP)
        t |> success(run_driver_reported(t, "path calls", fx, calls, out, "PROBE done"), "call_module_group over the path-guarded group runs")
        t |> success(find(out, "ENTRY alpha 3") >= 0 && find(out, "ENTRY beta") < 0, "the calls take the same members the require took:\n{out}")
    }
    t |> run("a guard the build lacks drops the group; a guard the build has takes it") @(t : T?) {
        var out : string
        let dropped = write_driver(fx, "use_guard_absent.das", "require ?zz_absent_guard [probe_group]", "alpha_value()")
        let rc = run_driver(fx, dropped, out)
        t |> success(rc != 0 && find(out, "alpha_value") >= 0, "with the group dropped nothing declares alpha_value: rc={rc}\n{out}")
        let taken = write_driver(fx, "use_guard_present.das", "require ?math [probe_group]", "alpha_value() + beta_value()")
        t |> success(run_driver_reported(t, "guard taken", fx, taken, out, "PROBE 11"), "the guarded group expands")
    }
    t |> run("a group nothing registered adds nothing") @(t : T?) {
        var out : string
        let script = write_driver(fx, "use_empty.das", "require [nobody_registered_here]", "0")
        t |> success(run_driver_reported(t, "empty", fx, script, out, "PROBE 0"), "the program compiles with no dependency added")
    }
    t |> run("`public` on the group re-exports every member; without it the members stay private") @(t : T?) {
        var out : string
        let viaPublic = write_driver(fx, "use_public.das", "require mid", "alpha_value() + beta_value()")
        t |> success(run_driver_reported(t, "public", fx, viaPublic, out, "PROBE 11"), "a module requiring the group public re-exports the members")
        let viaTab = write_driver(fx, "use_public_tab.das", "require mid_tab", "alpha_value() + beta_value()")
        t |> success(run_driver_reported(t, "public after a tab", fx, viaTab, out, "PROBE 11"), "the collector reads `public` after any whitespace, as the parser does")
        let viaPrivate = write_driver(fx, "use_private.das", "require mid_private", "alpha_value()")
        let rc = run_driver(fx, viaPrivate, out)
        t |> success(rc != 0 && find(out, "alpha_value") >= 0, "a private group require keeps the members out of the requirer's requirer: rc={rc}\n{out}")
    }
    t |> run("a member no require path serves fails as a require spelled by hand would") @(t : T?) {
        var out : string
        let script = write_driver(fx, "use_broken.das", "require [broken_group]", "0")
        let rc = run_driver(fx, script, out)
        t |> success(rc != 0 && find(out, "grp_gamma/missing") >= 0, "the missing prerequisite names the member: rc={rc}\n{out}")
    }
}

def arms_calls(t : T?; fx : GroupsFixture) {
    t |> run("call_module_group calls every member's entry with the arguments, members sorted by path") @(t : T?) {
        var out : string
        let script = path_join(fx.tmp, "use_call.das")
        fwrite(script, CALL_GROUP)
        t |> success(run_driver_reported(t, "call", fx, script, out, "PROBE done"), "the expansion compiles and runs")
        let alphaAt = find(out, "ENTRY alpha 7")
        let betaAt = find(out, "ENTRY beta 7")
        t |> success(alphaAt >= 0 && betaAt >= 0 && alphaAt < betaAt, "both entries ran with the argument, alpha first:\n{out}")
        t |> equal(1, count_of(out, "ENTRY alpha 7"), "a member registered twice is one member:\n{out}")
        let missing = path_join(fx.tmp, "use_call_missing.das")
        fwrite(missing, CALL_GROUP_MISSING)
        var rc = run_driver(fx, missing, out)
        t |> success(rc != 0 && find(out, "alpha::no_such_entry") >= 0, "a member without the entry is a compile error naming the member's call: rc={rc}\n{out}")
        let badArgs = path_join(fx.tmp, "use_call_bad_args.das")
        fwrite(badArgs, CALL_GROUP_BAD_ARGS)
        rc = run_driver(fx, badArgs, out)
        t |> success(rc != 0 && find(out, "call_module_group expects") >= 0, "a call without the entry name is a macro error: rc={rc}\n{out}")
        let sideEffect = path_join(fx.tmp, "use_call_side_effect.das")
        fwrite(sideEffect, CALL_GROUP_SIDE_EFFECT)
        t |> success(run_driver_reported(t, "side effect", fx, sideEffect, out, "PROBE 1"), "an argument with a side effect is evaluated once for every member")
        t |> success(find(out, "ENTRY alpha 1") >= 0 && find(out, "ENTRY beta 1") >= 0, "every member receives the one value:\n{out}")
        let shadow = path_join(fx.tmp, "use_call_shadow.das")
        fwrite(shadow, CALL_GROUP_SHADOW)
        t |> success(run_driver_reported(t, "shadow", fx, shadow, out, "PROBE 1 5"), "the bound argument's name is the expansion's own, so a caller's local of the plain spelling is untouched")
    }
    t |> run("a descriptor registering an empty member is refused by name, and the run goes on") @(t : T?) {
        var out : string
        let emptyDir = path_join(path_join(fx.tmp, "modules"), "grp_empty")
        var merr : string
        mkdir_rec(emptyDir, merr)
        fwrite(path_join(emptyDir, ".das_module"), DESCRIPTOR_EMPTY)
        let script = write_driver(fx, "use_group_beside_empty.das", "require [probe_group]", "alpha_value() + beta_value()")
        let rc = run_driver(fx, script, out)
        t |> success(find(out, "register_module_group needs a group name and a member require path") >= 0, "the descriptor's error names the missing member: rc={rc}\n{out}")
        t |> success(find(out, "PROBE 11") >= 0, "the other descriptors' rows still serve the run:\n{out}")
        var err : string
        rmdir_rec(emptyDir, err)
    }
    t |> run("the lint reads a group require as one unit: a used member keeps it, no used member reports it once") @(t : T?) {
        var out : string
        let lint = "{fx.cmdPrefix} -no-module-cache \"{path_join(get_das_root(), "utils/lint/main.das")}\" -- --quiet"
        let oneUsed = write_driver(fx, "lint_group_used.das", "require [probe_group]", "alpha_value()")
        var rc = run_child("{lint} \"{oneUsed}\"", out)
        t |> equal(0, rc, "beta unused beside a used alpha draws nothing:\n{out}")
        let noneUsed = write_driver(fx, "lint_group_unused.das", "require [probe_group]", "0")
        rc = run_child("{lint} \"{noneUsed}\"", out)
        t |> equal(2, rc, "an unused group is an issue:\n{out}")
        let named = find(out, "grp_alpha/alpha") >= 0 && find(out, "grp_beta/beta") >= 0
        let findings = count_of(out, "STYLE030: require [group] is unused")
        t |> equal(1, findings, "an unused group is one finding:\n{out}")
        t |> success(named, "the finding names the group's members:\n{out}")
    }
}

def arm_cache_stamps_the_expansion(t : T?; fx : GroupsFixture) {
    t |> run("a member joining the group after the record was written cuts the cache off at the requirer") @(t : T?) {
        let cache = "-module-cache \"{path_join(fx.tmp, "groups.dascache")}\""
        let script = write_driver(fx, "use_cache.das", "require grp_beta/beta\nrequire mid", "alpha_value()")
        fwrite(path_join(fx.betaDir, ".das_module"), DESCRIPTOR_BETA_OUTSIDE)
        var out : string
        t |> success(run_driver_reported(t, "cache write", fx, script, out, "PROBE 1", cache), "the cache is written with beta outside the group")
        t |> success(run_driver_reported(t, "cache serve", fx, script, out, "PROBE 1", cache), "the unchanged tree serves")
        t |> success(find(out, "FALLBACK") < 0, "nothing cuts the unchanged tree off:\n{out}")
        fwrite(path_join(fx.betaDir, ".das_module"), DESCRIPTOR_BETA)
        t |> success(run_driver_reported(t, "member joined", fx, script, out, "PROBE 1", cache), "the run with beta in the group still answers")
        t |> success(find(out, "re-parsed from") >= 0 && find(out, "mid.das") >= 0 && find(out, "(require set changed)") >= 0, "the requirer's record is the cutoff, named with its reason:\n{out}")
        t |> success(run_driver_reported(t, "rewritten", fx, script, out, "PROBE 1", cache), "the rewritten cache serves again")
        t |> success(find(out, "FALLBACK") < 0, "the rewritten cache carries the grown expansion:\n{out}")
    }
}

[test(description = "A group member's module name is its last path segment, less the .das a relative spelling carries")]
def test_member_module_name(t : T?) {
    t |> run("a member's module is its last path segment, less the .das a relative spelling carries") @(t : T?) {
        t |> equal(module_group_module_name("sqlite/sqlite_provider"), "sqlite_provider")
        t |> equal(module_group_module_name("./foo.das"), "foo")
        t |> equal(module_group_module_name("bar"), "bar")
    }
}

[test(description = "require [group] over a project root expands to its members, reports an unused group by name and stamps the expansion into the cache", timeout = 1200)]
def test_module_groups(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_module_groups", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let fx = make_groups_fixture(tmp)
    arms_expand(t, fx)
    arms_calls(t, fx)
    arm_cache_stamps_the_expansion(t, fx)
    var err : string
    rmdir_rec(tmp, err)
}

def fixture(name : string) : string => "%/tests/module_cache/_fixtures/{name}.das"

// the in-process arms issue their late requires from this [init]: a
// require after the walk is a compile's, and a test body runs after
// the compile, where the wrapper panics
var late_mod : Module?
var late_second : Module?
var late_answer_out = 0
var late_sum_out = 0
var late_sum3_out = 0
var late_pick_out = 0
var late_pick_refused = false
var late_pick_why : string
var late_nomacro_mod : Module?
var late_nomacro_ctx : Context?
var late_absent_why : string
var late_plain_why : string
var late_broken_why : string
var late_named_why : string
var late_ignoring_why : string

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
                late_pick_refused = true
                // AOT-compiled code need not preserve the text
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
    using() $(var cop : CodeOfPolicies) {
        cop.ignore_shared_modules = true
        let ignoring = fixture("mc_late_ignoring")
        require_module_now(ignoring, no_access, cop) $(var mod : Module?; issues : das_string) {
            late_ignoring_why = mod != null ? "answered {mod.name}" : string(issues)
        }
    }
}

//! a late require from the driver's [init], with the fixture's
//! compile logged per module
def late_driver() : string {
    return ("options gen2\nrequire daslib/cross_context\n\nvar answer = 0\n\n[init]\n"
        + "def late_init \{\n    var inscope no_access : smart_ptr<FileAccess>\n"
        + "    using() $(var cop : CodeOfPolicies) \{\n"
        + "        cop.log_module_compile_time = true\n"
        + "        cop.module_cache = compiling_program().policies.module_cache\n"
        + "        require_module_now(\"{fixture("mc_late_mod")}\", no_access, cop) "
        + "$(var mod : Module?; issues : das_string) \{\n"
        + "            print(\"LATE LOG \{issues\}\\n\")\n"
        + "            if (mod != null) \{\n                unsafe \{\n"
        + "                    invoke_in_context(addr(find_macro_context(mod)), "
        + "\"late_answer\", addr(answer))\n"
        + "                \}\n            \}\n        \}\n    \}\n\}\n\n"
        + "[export]\ndef main \{\n    print(\"LATE \{answer\}\\n\")\n\}\n")
}

def child_cmd(tmp, flags, script : string) : string {
    let enter = get_platform_name() == "windows" ? "cd /d {tmp} &&" : "cd {tmp} &&"
    return "{enter} {get_full_file_name(das_exe())} -dasroot {get_das_root()} {flags} \"{script}\""
}

//! the wrapper called from a running script
def running_driver() : string =>
    "options gen2\nrequire daslib/cross_context\n\n[export]\ndef main \{\n    let m = require_module_now(\"{fixture("mc_late_mod")}\")\n    print(\"LATE \{m != null\}\\n\")\n\}\n"

//! the block form called from a running script - the builtin's own
//! refusal
def running_block_driver() : string =>
    "options gen2\nrequire daslib/ast\n\n[export]\ndef main \{\n    var inscope no_access : smart_ptr<FileAccess>\n    using() $(var cop : CodeOfPolicies) \{\n        require_module_now(\"{fixture("mc_late_mod")}\", no_access, cop) $(var mod : Module?; issues : das_string) \{\n            print(\"LATE \{mod != null\}\\n\")\n        \}\n    \}\n\}\n"

//! a late require issued from a call macro during the requirer's own
//! compile; `tail` follows the call
def macro_driver(tail : string) : string =>
    "options gen2\nrequire {fixture("mc_late_macro_user")}\n\n[export]\ndef main \{\n    print(\"LATE \{late_probe_call()\}\\n\")\n{tail}\}\n"

def count_prefix(dir : string; prefix : string) : int {
    var n = 0
    fio::dir(dir) $(name) {
        if (name |> starts_with(prefix)) {
            n++
        }
    }
    return n
}

[test(description = "A late require compiles a shared module at the call, answers the same module afterwards, and answers null with the reason for an absent, unshared, broken or misnamed one", timeout = 1200)]
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
        t |> success(late_pick_refused, "the shared name is refused: {late_pick_why}")
        t |> success(empty(late_pick_why) || find(late_pick_why, "'late_pick' is ambiguous") >= 0, "the refusal names the ambiguity where the text survives: {late_pick_why}")
        t |> equal(late_pick_out, 0, "neither late_pick ran")
    }
    t |> run("a shared module with no macro context is required, and macro_context_of answers null for it") @(t : T?) {
        t |> success(late_nomacro_mod != null && late_nomacro_mod.name == "mc_late_nomacro", "the late require answers the module")
        t |> success(late_nomacro_ctx == null, "no macro context to answer")
    }
    t |> run("a caller that ignores shared modules still gets a shared module") @(t : T?) {
        t |> equal(late_ignoring_why, "answered mc_late_ignoring")
    }
    t |> run("a file nothing serves, a module that is not shared, one that does not compile, and one whose name is not its file's answer null with the reason") @(t : T?) {
        t |> success(find(late_absent_why, "mc_late_absent") >= 0, "the absent file is named: {late_absent_why}")
        t |> success(find(late_plain_why, "is not shared") >= 0, "the reason names the missing `shared`: {late_plain_why}")
        t |> success(find(late_broken_why, "mc_late_broken") >= 0 && find(late_broken_why, "return type") >= 0, "the compile error is the reason: {late_broken_why}")
        t |> success(find(late_named_why, "declares module 'mc_late_other'") >= 0 && find(late_named_why, "`module mc_late_named`") >= 0, "the declared name is held to the file's: {late_named_why}")
        t |> success(!has_module("mc_late_other"), "the misnamed module never joined the process")
    }
}

[test(description = "A late require keeps its own cache file and a call from a running script panics, as a child process shows", timeout = 1200)]
def test_require_module_now_children(t : T?) {
    t |> run("a late require keeps its own cache file; a running script's call panics") @(t : T?) {
        var terr : string
        let tmp = create_temp_directory("das_late_require", terr)
        if (empty(tmp)) {
            t |> failure("create_temp_directory: {terr}")
            return
        }
        let script = path_join(tmp, "use_late.das")
        fwrite(script, late_driver())
        let cacheFile = path_join(tmp, "late.dascache")
        let cached = child_cmd(tmp, "-module-cache \"{cacheFile}\"", script)
        var out : string
        t |> success(run_child_reported(t, "cold", cached, out, "LATE 42"), "the cold run answers through the late module")
        t |> success(find(out, "compiler took") >= 0 && find(out, "mc_late_mod") >= 0, "the cold run compiles the fixture:\n{out}")
        let lateDir = path_join(tmp, ".cache/daslang/module_cache")
        let lateFiles = count_prefix(lateDir, "mc_late_mod-")
        t |> equal(lateFiles, 1, "the late walk has a cache file of its own")
        let coldSize = long_length(fread(cacheFile))
        t |> success(run_child_reported(t, "warm", cached, out, "LATE 42"), "the warm run answers through the late module")
        let warmServed = (find(out, "cache read took") >= 0 && find(out, "mc_late_mod") >= 0
            && find(out, "compiler took") < 0)
        t |> success(warmServed, "the warm run serves the fixture from its own cache:\n{out}")
        t |> equal(long_length(fread(cacheFile)), coldSize, "the warm run rewrites nothing")
        let uncached = child_cmd(tmp, "-no-module-cache", script)
        t |> success(run_child_reported(t, "no cache", uncached, out, "LATE 42"), "the run with no module cache answers too")
        t |> success(find(out, "compiler took") >= 0 && find(out, "cache read took") < 0, "with no module cache the late walk compiles:\n{out}")
        let running = path_join(tmp, "use_running.das")
        fwrite(running, running_driver())
        var rc = run_child("{das_exe()} -dasroot {get_das_root()} -no-module-cache \"{running}\"", out)
        t |> success(rc != 0 && find(out, "no program is compiling") >= 0, "a running script's late require panics with the reason: rc={rc}\n{out}")
        let runningBlock = path_join(tmp, "use_running_block.das")
        fwrite(runningBlock, running_block_driver())
        rc = run_child("{das_exe()} -dasroot {get_das_root()} -no-module-cache \"{runningBlock}\"", out)
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
        let cached = child_cmd(tmp, "-module-cache \"{cacheFile}\"", script)
        var out : string
        t |> success(run_child_reported(t, "cold", cached, out, "LATE 42"), "the macro's late require answers on the cold run")
        let coldSize = long_length(fread(cacheFile))
        t |> success(run_child_reported(t, "warm", cached, out, "LATE 42"), "the warm run answers")
        t |> success(find(out, "FALLBACK") < 0 && find(out, "module order changed") < 0, "the warm read is not cut by the nested walk's modules:\n{out}")
        t |> equal(long_length(fread(cacheFile)), coldSize, "the warm run rewrites nothing")
        t |> success(run_child_reported(t, "warm again", cached, out, "LATE 42"), "the second warm run answers")
        t |> success(find(out, "FALLBACK") < 0, "the second warm read is clean too:\n{out}")
        fwrite(script, macro_driver("    let y : int = \"not an int\"\n"))
        let rc = run_child(cached, out)
        t |> success(rc != 0, "the broken requirer fails to compile: rc={rc}")
        t |> equal(long_length(fread(cacheFile)), coldSize, "a failed compile after the macro's late require leaves the good cache in place")
        var err : string
        rmdir_rec(tmp, err)
    }
}

//! the whole scan trace line for a module's descriptor, timing clause
//! included
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

def line_count(text : string; prefix : string) : int {
    var n = 0
    for (line in split(text, "\n")) {
        if (line |> starts_with(prefix)) {
            n++
        }
    }
    return n
}

//! the `DOLLAR N` line the dollar.das child prints: the builtin
//! module's function count
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

//! the tree's UnitTest shared module, in whichever configuration this
//! binary was built - absent in a static build, where nothing is
//! deferred and this test has nothing to observe
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

let DEFERRED_DESCRIPTOR = ("options gen2\nrequire daslib/fio\n\n[export]\n" +
    "def initialize(project_path : string) \{\n" +
    "    register_native_path(\"deferred_probe\", \"hello\", \"\{project_path\}/hello.das\")\n" +
    "    register_native_path(\"deferred_probe\", \"guarded_dep\", " +
    "\"\{project_path\}/guarded_dep.das\")\n" +
    "\}\n")

//! a project root holding a copy of the tree's dasUnitTest -
//! descriptor and artifact - so the copy's manifest is the test's to
//! make cold or warm (the copy shadows the tree's), a pure-das
//! module, and the driver scripts the arms spawn
struct DeferredFixture {
    tmp : string
    manifest : string
    utManifest : string
    utDir : string        //! the copied dasUnitTest module folder under the fixture
    utArtifact : string   //! the copied artifact in it: the arms move it out and back
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

def make_deferred_fixture(tmp : string; artifact : string) : DeferredFixture {
    let modules = path_join(tmp, "modules")
    let modDir = path_join(modules, "deferred_probe")
    let utDir = path_join(modules, UNIT_TEST_FOLDER)
    var merr : string
    mkdir_rec(modDir, merr)
    mkdir_rec(utDir, merr)
    fwrite(path_join(modDir, ".das_module"), DEFERRED_DESCRIPTOR)
    fwrite(path_join(modDir, "hello.das"), "options gen2\nmodule hello\ndef hello_value() : int \{\n    return 42\n\}\n")
    fwrite(path_join(modDir, "guarded_dep.das"), "options gen2\nmodule guarded_dep\nrequire ?{UNIT_TEST_MODULE} deferred_probe/hello public\n")
    let treeUt = path_join(path_join(get_das_root(), "modules"), UNIT_TEST_FOLDER)
    var cerr : string
    copy_file(path_join(treeUt, ".das_module"), path_join(utDir, ".das_module"), true, cerr)
    copy_file(artifact, path_join(utDir, base_name(artifact)), true, cerr)
    let fx = DeferredFixture(
        tmp = tmp,
        manifest = path_join(modDir, manifest_file_name()),
        utManifest = path_join(utDir, manifest_file_name()),
        utDir = utDir,
        utArtifact = path_join(utDir, base_name(artifact)),
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

//! a copy of one of the tree's module folders - descriptor and the
//! artifacts that exist, in either configuration's name - under the
//! fixture's modules, where it shadows the tree's
def copy_module(fx : DeferredFixture; folder : string; artifacts : array<string>) : string {
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

//! whether the cold run loaded the copied artifact - a build whose
//! artifacts find their libraries relative to the tree cannot load a
//! copy, and the arms on copies say so and return
def copy_loaded(out : string; dir : string; artifact : string) : bool {
    for (suffix in [".shared_module", "_debug.shared_module"]) {
        if (find(out, "{path_join(dir, artifact)}{suffix} : loaded") >= 0) {
            return true
        }
    }
    return false
}

def child_cmd(fx : DeferredFixture; script : string; flags : string = "";
              cache : string = "-no-module-cache") : string =>
    "{trace_prefix()}\"{das_exe()}\" -dasroot \"{get_das_root()}\" {cache} -project_root \"{fx.tmp}\" {flags} \"{script}\""

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

def arms_deferred(t : T?; fx : DeferredFixture) {
    t |> run("a cold start loads the module to record its name, and a guard reads it available") @(t : T?) {
        var out : string
        t |> success(run_child_reported(t, "cold", child_cmd(fx, fx.guarded), out, "GUARD true"), "the cold start runs, and the guard takes its target")
        t |> equal("compiled (no manifest), manifest written (1 row(s))", descriptor_line(out, UNIT_TEST_FOLDER), "the copied descriptor compiles and records:\n{out}")
        t |> success(find(out, "{UNIT_TEST_CLASS} <- ") >= 0 && find(out, ": loaded") >= 0, "the recording start loads the module:\n{out}")
    }
    t |> run("a warm start defers the module and loads nothing for a program that requires nothing") @(t : T?) {
        var out : string
        t |> success(run_child_reported(t, "warm", child_cmd(fx, fx.plain), out, "PLAIN"), "the warm start runs the program")
        let line = descriptor_line(out, UNIT_TEST_FOLDER)
        t |> success(line |> starts_with("replayed 1 row(s) in ") && find(line, ", deferred 1)") >= 0, "the descriptor replays its one row deferred:\n{out}")
        t |> equal(-1, find(out, "{UNIT_TEST_CLASS} <- "), "nothing loads the shared module:\n{out}")
    }
    t |> run("the first require naming a deferred module loads it, and the program calls into it") @(t : T?) {
        var out : string
        t |> success(run_child_reported(t, "uses", child_cmd(fx, fx.uses), out, "LENGTH 5"), "the program calls the module's function")
        t |> success(find(out, "[module] require {UNIT_TEST_MODULE}: loading the deferred {UNIT_TEST_CLASS}") >= 0, "the trace names the require that loaded it:\n{out}")
        t |> success(find(out, "{UNIT_TEST_CLASS} <- ") >= 0 && find(out, ": loaded") >= 0, "the shared module loads:\n{out}")
    }
    t |> run("builtin_module_exists loads a waiting module, with no require naming it") @(t : T?) {
        var out : string
        t |> success(run_child_reported(t, "exists", child_cmd(fx, fx.exists), out, "EXISTS true"), "the build has the module")
        t |> success(find(out, "[module] require {UNIT_TEST_MODULE}: loading the deferred {UNIT_TEST_CLASS}") >= 0, "the question loaded it:\n{out}")
    }
    t |> run("has_module answers true for a waiting module and loads nothing") @(t : T?) {
        var out : string
        t |> success(run_child_reported(t, "has_module", child_cmd(fx, fx.hasModule), out, "HAS true"), "the build has the module")
        t |> equal(-1, find(out, "{UNIT_TEST_CLASS} <- "), "nothing loads the shared module:\n{out}")
    }
    t |> run("a lazy start and an eager start build the same builtin module") @(t : T?) {
        var lazy : string
        t |> success(run_child_reported(t, "dollar lazy", child_cmd(fx, fx.dollar), lazy, "DOLLAR "), "the lazy start counts")
        var eager : string
        t |> success(run_child_reported(t, "dollar eager", child_cmd(fx, fx.dollar, "-ignore-manifest"), eager, "DOLLAR "), "the eager start counts")
        t |> success(dollar_count(lazy) > 0, "the count is a count:\n{lazy}")
        t |> equal(dollar_count(lazy), dollar_count(eager), "a C++ module's load registers nothing into $:\nlazy:\n{lazy}\neager:\n{eager}")
    }
    t |> run("a compile of a string, which walks no prerequisites, meets the waiting module at its require") @(t : T?) {
        var out : string
        t |> success(run_child_reported(t, "nested", child_cmd(fx, fx.nested), out, "NESTED true"), "the nested program compiles")
        t |> success(find(out, "[module] require {UNIT_TEST_MODULE}: loading the deferred {UNIT_TEST_CLASS}") >= 0, "the parser's require loaded it:\n{out}")
    }
}

def arm_math_container_cache(t : T?; fx : DeferredFixture) {
    t |> run("math caches survive eager and deferred matrix-vector consumers") @(t : T?) {
        let driver = path_join(fx.tmp, "math_cache.das")
        fwrite(driver, "options gen2\nrequire math\n[export]\ndef main \{\n    print(\"MATH \{identity3x4()[0].x\}\\n\")\n\}\n")
        for (eagerFirst in [true, false]) {
            let cache = "-module-cache \"{path_join(fx.tmp, eagerFirst ? "math_eager.bin" : "math_lazy.bin")}\""
            var cold : string
            t |> success(run_child_reported(t, "math cold", child_cmd(fx, driver, eagerFirst ? "-ignore-manifest" : "", cache), cold, "MATH 1"), "the first loading order writes the cache")
            var warm : string
            t |> success(run_child_reported(t, "math warm", child_cmd(fx, driver, eagerFirst ? "" : "-ignore-manifest", cache), warm, "MATH 1"), "the other loading order runs correctly")
            t |> equal(-1, find(warm, "cumulative hash"), "loading matrix-vector consumers does not change math's interface:\n{warm}")
            t |> success(find(warm, "deser: clean") >= 0, "the other loading order serves every cached module:\n{warm}")
        }
    }
}

[arch(at="ARCHITECTURE.md#file-charters")]
def arms_artifact_unloadable_or_gone(t : T?; fx : DeferredFixture) {
    let loading = "[module] require {UNIT_TEST_MODULE}: loading the deferred {UNIT_TEST_CLASS}"
    let missing = "error[20605]: missing prerequisite '{UNIT_TEST_MODULE}'"
    t |> run("a deferred row whose artifact does not load brings every deferred module in, " +
             "and the require fails as on a cold start") @(t : T?) {
        var warm : string
        t |> success(run_child_reported(t, "rewarm", child_cmd(fx, fx.plain), warm, "PLAIN"),
            "a cold start rewrites the manifest")
        let text = fread(fx.utManifest)
        t |> success(find(text, "\ndm\t") >= 0, "the manifest carries the dm row: {text}")
        for (name in ["absent.shared_module", "absent_debug.shared_module"]) {
            fwrite(path_join(fx.utDir, name), "not a shared module\n")
        }
        fwrite(fx.utManifest, replace(text, UNIT_TEST_ARTIFACT, "absent"))
        var out : string
        let rc = run_child(child_cmd(fx, fx.uses), out)
        t |> success(rc != 0 && find(out, "LENGTH") < 0, "the program does not run:\n{out}")
        t |> success(find(out, loading) >= 0, "the require asks for the row:\n{out}")
        t |> success(find(out, "[module] loading every deferred module (") >= 0,
            "the failed load brings the rest in:\n{out}")
        t |> success(find(out, missing) >= 0,
            "the require fails with the missing-module error:\n{out}")
        var disabled = ""
        let modules = path_join(get_das_root(), "modules")
        dir(modules) $(name) {
            let descriptor = path_join(path_join(modules, name), ".das_module")
            if (name != UNIT_TEST_FOLDER && fexist(descriptor)) {
                disabled += " --disable-module \"{name}\""
            }
        }
        let empty_rc = run_child(child_cmd(fx, fx.uses, disabled), out)
        t |> success(empty_rc != 0 && find(out, "LENGTH") < 0,
            "missing artifact still rejects execution with no other deferred rows")
        t |> success(find(out, loading) >= 0,
            "the isolated child requests its deferred module:\n{out}")
        t |> success(find(out, missing) >= 0,
            "the isolated child reports the missing module:\n{out}")
        t |> success(find(out, "[module] loading every deferred module (0)") >= 0,
            "empty fallback attempts remain visible:\n{out}")
        remove_result(fx.utManifest)
        for (name in ["absent.shared_module", "absent_debug.shared_module"]) {
            remove_result(path_join(fx.utDir, name))
        }
    }
    t |> run("a dm row whose artifact is gone makes the manifest stale: " +
             "the descriptor recompiles, and has_module answers absent") @(t : T?) {
        var warm : string
        t |> success(run_child_reported(t, "rewarm", child_cmd(fx, fx.plain), warm, "PLAIN"),
            "a cold start rewrites the manifest")
        with_artifact_parked(t, fx) {
            var out : string
            t |> success(run_child_reported(t, "gone", child_cmd(fx, fx.hasModule), out,
                "HAS false"), "the build does not have the module:\n{out}")
            let line = descriptor_line(out, UNIT_TEST_FOLDER)
            t |> success(line |> starts_with("compiled (artifact missing)"),
                "the stale manifest recompiles the descriptor:\n{out}")
            t |> success(find(out, "{UNIT_TEST_CLASS} <- ") >= 0 && find(out, ": FAILED") >= 0,
                "the register fails quietly, as on a cold start:\n{out}")
        }
        var again : string
        t |> success(run_child_reported(t, "back", child_cmd(fx, fx.hasModule), again, "HAS true"),
            "the next start has the module again:\n{again}")
    }
}

//! `blk` runs with the unit-test module's artifact moved out of its
//! folder, the artifact back in place after it
def private with_artifact_parked(t : T?; fx : DeferredFixture; blk : block<void>) {
    let parked = path_join(fx.tmp, "parked.shared_module")
    var rerr : string
    let left = rename(fx.utArtifact, parked, rerr)
    t |> success(left, "the artifact leaves the module folder: {rerr}")
    invoke(blk)
    let back = rename(parked, fx.utArtifact, rerr)
    t |> success(back, "the artifact comes back: {rerr}")
}

def arm_cache_pins_presence(t : T?; fx : DeferredFixture) {
    t |> run("a module-cache record folded with the module present is refused once it is gone, " +
             "and the other way") @(t : T?) {
        let cache = "-module-cache \"{path_join(fx.tmp, "mc_witness.bin")}\""
        var warm : string
        let cmd = child_cmd(fx, fx.exists, "", cache)
        t |> success(run_child_reported(t, "cache present", cmd, warm, "EXISTS true"),
            "the cold cache run folds the module as present:\n{warm}")
        with_artifact_parked(t, fx) {
            var gone : string
            t |> success(run_child_reported(t, "cache gone", cmd, gone, "EXISTS false"),
                "the record pinned the module's presence, so the run folds it absent:\n{gone}")
        }
        var back : string
        t |> success(run_child_reported(t, "cache back", cmd, back, "EXISTS true"),
            "the record written absent is refused once the module is back:\n{back}")
    }
}

def arms_guards_and_caches(t : T?; fx : DeferredFixture) {
    t |> run("a require guard loads a waiting module, whatever else the compile requires") @(t : T?) {
        var alone : string
        t |> success(run_child_reported(t, "guarded", child_cmd(fx, fx.guarded), alone, "GUARD true"), "the guard alone takes its target")
        t |> success(find(alone, "[module] require {UNIT_TEST_MODULE}: loading the deferred {UNIT_TEST_CLASS}") >= 0, "the guard loaded the module:\n{alone}")
        var after : string
        t |> success(run_child_reported(t, "guarded after", child_cmd(fx, fx.guardedAfter), after, "GUARD true"), "a require above the guard")
        var before : string
        t |> success(run_child_reported(t, "guarded before", child_cmd(fx, fx.guardedBefore), before, "GUARD true"), "a require below the guard")
        var dep : string
        t |> success(run_child_reported(t, "guarded dep", child_cmd(fx, fx.guardedDep), dep, "GUARD true"), "a guard inside a module walked before the entry's own require")
    }
    t |> run("-ignore-manifest compiles every descriptor, loads every C++ module on start, and writes no manifest") @(t : T?) {
        remove_result(fx.manifest)
        remove_result(fx.utManifest)
        var out : string
        t |> success(run_child_reported(t, "ignore", child_cmd(fx, fx.plain, "-ignore-manifest"), out, "PLAIN"), "the program runs")
        t |> equal("compiled (manifests ignored)", descriptor_line(out, UNIT_TEST_FOLDER), "the copied descriptor compiles:\n{out}")
        t |> equal("compiled (manifests ignored)", descriptor_line(out, "deferred_probe"), "the pure-das descriptor compiles:\n{out}")
        t |> success(find(out, "{UNIT_TEST_CLASS} <- ") >= 0 && find(out, ": loaded") >= 0, "the shared module loads on start:\n{out}")
        t |> success(!stat(fx.manifest).is_valid && !stat(fx.utManifest).is_valid, "no manifest was written")
    }
    t |> run("a module cache written by an eager start serves a lazy start, and -log-compile-time shows the reads and the timeline") @(t : T?) {
        let cache = "-module-cache \"{path_join(fx.tmp, "mc.bin")}\""
        var cold : string
        t |> success(run_child_reported(t, "cold cache", child_cmd(fx, fx.plain, "-ignore-manifest -log-compile-time", cache), cold, "PLAIN"), "the eager start writes the cache")
        t |> success(find(cold, "startup: main total") >= 0 && find(cold, "\tmodule scan") >= 0 && find(cold, "\tshutdown ") >= 0, "the timeline prints its laps:\n{cold}")
        var warm : string
        t |> success(run_child_reported(t, "warm cache", child_cmd(fx, fx.plain, "-log-compile-time", cache), warm, "PLAIN"), "the lazy start, which loads no C++ module, runs")
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
        t |> success(run_child_reported(t, "ser", "{child_cmd(fx, dastest)} -- --test \"{testsDir}\" --ser \"{stream}\"", wrote, "Serialized 1 programs"), "the writer serializes the test program")
        var read : string
        t |> success(run_child_reported(t, "deser", "{child_cmd(fx, dastest)} -- --test \"{testsDir}\" --deser \"{stream}\"", read, "1 tests, 0 error(s)"), "the reader runs the test from the stream")
        t |> success(find(read, "[module] require {UNIT_TEST_MODULE}: loading the deferred {UNIT_TEST_CLASS}") >= 0, "the reader loaded the module the stream names:\n{read}")
    }
}

def arm_fallback(t : T?; fx : DeferredFixture) {
    let modules = path_join(get_das_root(), "modules")
    let imgui = path_join(modules, "dasImgui")
    if (!has_shared_modules(imgui, ["imguiApp", "dasModuleImgui"]) || !has_shared_modules(path_join(modules, "dasGlfw"), ["dasModuleGlfw"])) {
        to_log(LOG_INFO, "test_deferred_modules: no imgui_app, imgui and glfw shared modules in {modules} - the fallback arm has nothing to observe\n")
        return
    }
    t |> run("a module whose C++ dependencies are deferred too brings every deferred module in") @(t : T?) {
        var out : string
        t |> success(run_child_reported(t, "app", child_cmd(fx, fx.app), out, "APP"), "the program compiles against imgui_app")
        t |> success(find(out, "[module] require imgui_app: loading the deferred Module_imgui_app") >= 0, "imgui_app loads at its require:\n{out}")
        t |> success(find(out, "[module] loading every deferred module (") >= 0, "its initDependencies asks for glfw and imgui, so the rest loads:\n{out}")
    }
    t |> run("a half-warm tree - a descriptor compiled cold beside replayed ones - initializes by bringing every deferred module in") @(t : T?) {
        let imguiDir = copy_module(fx, "dasImgui", ["dasModuleImgui", "imguiApp", "imguiAppHeadless"])
        copy_module(fx, "dasGlfw", ["dasModuleGlfw"])
        var cold : string
        t |> success(run_child_reported(t, "half-warm cold", child_cmd(fx, fx.plain), cold, "PLAIN"), "the copies record their manifests")
        if (!copy_loaded(cold, imguiDir, "imguiApp")) {
            to_log(LOG_INFO, "test_deferred_modules: the copied imguiApp does not load from {imguiDir} - its libraries sit outside the copy - so the half-warm arm has nothing to observe\n")
            return
        }
        remove_result(path_join(imguiDir, manifest_file_name()))
        var out : string
        t |> success(run_child_reported(t, "half-warm", child_cmd(fx, fx.plain), out, "PLAIN"), "the program runs with imgui_app loaded on start and glfw waiting")
        if (!copy_loaded(out, imguiDir, "imguiApp")) {
            to_log(LOG_INFO, "test_deferred_modules: the copied imguiApp did not load on the half-warm start - its libraries sit outside the copy - so initialize had nothing to bring in:\n{out}\n")
            return
        }
        t |> success(find(out, "[module] loading every deferred module (") >= 0, "initialize brings the deferred glfw in:\n{out}")
    }
}

//! the dm rows a recording start whose load failed leaves - nameless
//! - and what the scan makes of them
def arms_nameless_rows(t : T?; fx : DeferredFixture) {
    let utDir = path_join(path_join(fx.tmp, "modules"), UNIT_TEST_FOLDER)
    t |> run("a replayed row whose artifact exists but does not load brings every deferred module in at the scan") @(t : T?) {
        var warm : string
        t |> success(run_child_reported(t, "rewarm", child_cmd(fx, fx.plain), warm, "PLAIN"), "a cold start rewrites the manifest")
        let text = fread(fx.utManifest)
        // a nameless dm row whose path is a file that exists and is no shared
        // module
        let descriptor = path_join(utDir, ".das_module")
        let doctored = replace(text, "\nend\t1\n", "\ndm\t{descriptor}\tModule_Unloadable\t0\t\nend\t2\n")
        t |> success(doctored != text, "the manifest carries its one row: {text}")
        fwrite(fx.utManifest, doctored)
        var out : string
        t |> success(run_child_reported(t, "pending", child_cmd(fx, fx.plain), out, "PLAIN"), "the program runs")
        t |> success(find(out, "Module_Unloadable <- {descriptor} : FAILED") >= 0, "the row's load fails at the scan:\n{out}")
        t |> success(find(out, "[module] a pending module's artifact exists - loading every deferred module") >= 0, "the present artifact brings the deferred set in:\n{out}")
        // the trace's path is the row's as recorded, so its separators are
        // the descriptor's, not path_join's
        let loaded = find(out, "{UNIT_TEST_ARTIFACT}.shared_module : loaded") >= 0 || find(out, "{UNIT_TEST_ARTIFACT}_debug.shared_module : loaded") >= 0
        t |> success(find(out, "{UNIT_TEST_CLASS} <- ") >= 0 && loaded, "the deferred module loads on start:\n{out}")
        t |> success(find(out, ": manifest rewritten, ") < 0, "a row that never loads stays nameless:\n{out}")
        remove_result(fx.utManifest)
    }
    t |> run("a replayed nameless row whose module loads gets its name written back, and the next start defers it") @(t : T?) {
        var warm : string
        t |> success(run_child_reported(t, "rewarm", child_cmd(fx, fx.plain), warm, "PLAIN"), "a cold start rewrites the manifest")
        let text = fread(fx.utManifest)
        // the real dm row with its name blanked
        let doctored = replace(text, "\t{UNIT_TEST_MODULE}\n", "\t\n")
        t |> success(doctored != text, "the manifest carries the named row: {text}")
        let depLines = line_count(text, "dep\t")
        t |> success(depLines > 0, "the manifest carries dep lines: {text}")
        fwrite(fx.utManifest, doctored)
        var eager : string
        t |> success(run_child_reported(t, "eager", child_cmd(fx, fx.plain), eager, "PLAIN"), "the program runs")
        let loaded = find(eager, "{UNIT_TEST_ARTIFACT}.shared_module : loaded") >= 0 || find(eager, "{UNIT_TEST_ARTIFACT}_debug.shared_module : loaded") >= 0
        t |> success(find(eager, "{UNIT_TEST_CLASS} <- ") >= 0 && loaded, "the nameless row loads at the scan:\n{eager}")
        // the trace joins the descriptor with "/" whatever the platform, as
        // descriptor_line reads it
        t |> success(find(eager, "{UNIT_TEST_FOLDER}/.das_module: manifest rewritten, 1 row(s) named") >= 0, "the scan writes the name back:\n{eager}")
        let healed = fread(fx.utManifest)
        t |> success(find(healed, "\t{UNIT_TEST_MODULE}\n") >= 0, "the manifest carries the name again")
        t |> equal(depLines, line_count(healed, "dep\t"), "and every dep line it had: {healed}")
        var lazy : string
        t |> success(run_child_reported(t, "lazy", child_cmd(fx, fx.plain), lazy, "PLAIN"), "the next start runs")
        t |> equal(-1, find(lazy, "{UNIT_TEST_CLASS} <- "), "the next start defers the row and loads nothing:\n{lazy}")
    }
}

//! the arms share one root and run in this order
[test(description = "A replayed dm row is not loaded by the scan, a deferred module loads on demand, and the cache, artifact and fallback arms behave across cold and warm starts", timeout = 1200)]
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
    let fx = make_deferred_fixture(tmp, artifact)
    arms_deferred(t, fx)
    arms_guards_and_caches(t, fx)
    arms_artifact_unloadable_or_gone(t, fx)
    arm_cache_pins_presence(t, fx)
    arm_math_container_cache(t, fx)
    arms_nameless_rows(t, fx)
    arm_fallback(t, fx)
    var err : string
    rmdir_rec(tmp, err)
}

let DESCRIPTOR = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    register_native_path(\"manifest_probe\", \"hello\", \"\{project_path\}/hello.das\")\n\}\n"
let DESCRIPTOR_OPT_OUT = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    no_manifest()\n    register_native_path(\"manifest_probe\", \"hello\", \"\{project_path\}/hello.das\")\n\}\n"
let DESCRIPTOR_TABBED = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    register_native_path(\"manifest_probe\", \"hello\", \"\{project_path\}/hello.das\")\n    register_native_path(\"manifest_probe\", \"tab\\ted\", \"\{project_path\}/tabbed.das\")\n\}\n"
//! a C++ module row on an artifact that is not there: the Quiet
//! overload defers the dlopen failure and never fails the descriptor
let DESCRIPTOR_DM = "options gen2\nrequire daslib/fio\n\n[export]\ndef initialize(project_path : string) \{\n    register_dynamic_module(\"\{project_path\}/absent.shared_module\", \"Module_Absent\")\n\}\n"
//! the require path comes from a module the descriptor requires: its
//! content is part of the key
let DESCRIPTOR_DEP = "options gen2\nrequire daslib/fio\nrequire ./helper\n\n[export]\ndef initialize(project_path : string) \{\n    register_native_path(\"manifest_probe\", helper_name(), \"\{project_path\}/hello.das\")\n\}\n"
let HELPER = "options gen2\nmodule helper\ndef helper_name() : string \{\n    return \"hello\"\n\}\n"
let DESCRIPTOR_BROKEN = "options gen2\n\n[export]\ndef initialize(project_path : string) \{\n    this_is_not_a_function()\n\}\n"

//! the scan trace line for a module's descriptor: `[module]
//! descriptor <path>/.das_module: <verdict>`; a replay verdict keeps
//! its deferred count and drops the two timings
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

//! the key lines every manifest opens with; dep lines, rows and the
//! end line follow
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

//! writes `text` as the manifest, runs the child, and expects the
//! scan to reject it for `why` and rewrite it; the require resolves
//! either way
def expect_rewrite(t : T?; cmd : string; manifest : string; text : string; why : string) {
    fwrite(manifest, text)
    var out : string
    t |> success(run_child_reported(t, why, cmd, out, "PROBE 42"), "'{why}': the require resolves without the manifest")
    t |> equal("compiled ({why}), manifest written (1 row(s))", probe_line(out), "'{why}' recompiles and rewrites:\n{out}")
}

//! the project root the arms share: the probe module's paths and the
//! child command line
struct ManifestFixture {
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

//! the child that runs the driver under project root `tmp`: `exe`
//! started with `-dasroot root`
def fixture_cmd(tmp, exe, root : string) : string =>
    "{trace_prefix()}\"{exe}\" -dasroot \"{root}\" -no-module-cache -project_root \"{tmp}\" \"{path_join(tmp, "use.das")}\""

//! a project root holding one pure-das module whose descriptor
//! registers one require path, a module whose descriptor registers an
//! absent C++ artifact, a module whose descriptor does not compile,
//! and a driver that requires through the first
def make_manifest_fixture(tmp : string) : ManifestFixture {
    let modules = path_join(tmp, "modules")
    let modDir = path_join(modules, "manifest_probe")
    let dmDir = path_join(modules, "dm_probe")
    let brokenDir = path_join(modules, "broken_probe")
    var merr : string
    mkdir_rec(modDir, merr)
    mkdir_rec(dmDir, merr)
    mkdir_rec(brokenDir, merr)
    let manifest = path_join(modDir, manifest_file_name())
    let fx = ManifestFixture(
        tmp = tmp,
        modDir = modDir,
        dmDir = dmDir,
        brokenDir = brokenDir,
        descriptor = path_join(modDir, ".das_module"),
        manifest = manifest,
        manifestTmp = "{manifest}.tmp",
        dmManifest = path_join(dmDir, manifest_file_name()),
        cmd = fixture_cmd(tmp, das_exe(), get_das_root())
    )
    fwrite(fx.descriptor, DESCRIPTOR)
    fwrite(path_join(fx.modDir, "hello.das"), "options gen2\nmodule hello\ndef hello_value() : int \{\n    return 42\n\}\n")
    fwrite(path_join(fx.dmDir, ".das_module"), DESCRIPTOR_DM)
    fwrite(path_join(fx.brokenDir, ".das_module"), DESCRIPTOR_BROKEN)
    fwrite(path_join(tmp, "use.das"), "options gen2\nrequire manifest_probe/hello\n[export]\ndef main \{\n    print(\"PROBE \{hello_value()\}\\n\")\n\}\n")
    return fx
}

def arms_record_and_replay(t : T?; fx : ManifestFixture) {
    let cmd = fx.cmd
    let manifest = fx.manifest
    t |> run("the first start compiles the descriptor and writes its manifest") @(t : T?) {
        var out : string
        t |> success(run_child_reported(t, "cold", cmd, out, "PROBE 42"), "the require resolves through the compiled descriptor")
        t |> equal("compiled (no manifest), manifest written (1 row(s))", probe_line(out), "the trace says compiled and written:\n{out}")
        t |> success(stat(manifest).is_valid, "the manifest sits beside the descriptor")
        let text = fread(manifest)
        t |> success(text |> starts_with("das_module_manifest\t4\n"), "the manifest opens with its format version: {text}")
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
        t |> success(run_child_reported(t, "broken", cmd, out, "PROBE 42"), "the other module's require still resolves")
        t |> equal("compiled with errors, no manifest", probe_line(out, "broken_probe"), "the trace names the failure:\n{out}")
        t |> success(!stat(path_join(fx.brokenDir, manifest_file_name())).is_valid, "nothing was written for it")
    }
    t |> run("the second start replays the manifest and never compiles the descriptor") @(t : T?) {
        var out : string
        t |> success(run_child_reported(t, "warm", cmd, out, "PROBE 42"), "the require resolves through the replayed rows")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(out), "the trace says replayed:\n{out}")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(out, "dm_probe"), "the dm row replays too:\n{out}")
        t |> success(count_of(out, "Module_Absent <- ") >= 1, "the replayed dm row retries its dlopen:\n{out}")
    }
    t |> run("an edited descriptor recompiles once and is replayed again after") @(t : T?) {
        fwrite(fx.descriptor, "{DESCRIPTOR}// edited\n")
        var out : string
        t |> success(run_child_reported(t, "edited", cmd, out, "PROBE 42"), "the edited descriptor still resolves the require")
        t |> equal("compiled (descriptor changed), manifest written (1 row(s))", probe_line(out), "the stamp mismatch recompiles and rewrites:\n{out}")
        var again : string
        t |> success(run_child_reported(t, "edited warm", cmd, again, "PROBE 42"), "the rewritten manifest resolves the require")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(again), "the rewritten manifest replays:\n{again}")
    }
}

//! the binary kinds keep their manifests apart: the other kind's
//! daslang writes its own file beside this one's, and each replays
//! its own from its second start on - a CI lane running both suites
//! at once never sees a `binary kind` recompile
def arms_binary_kinds(t : T?; fx : ManifestFixture) {
    let twin = twin_exe()
    if (empty(twin)) {
        t |> skip("no {das_is_dll_build() ? "daslang_static" : "daslang"} beside {das_exe()}")
        return
    }
    let twinCmd = fixture_cmd(fx.tmp, twin, get_das_root())
    let twinManifest = path_join(fx.modDir, twin_manifest_file_name())
    t |> run("the other kind's first start writes its own manifest and leaves this kind's alone") @(t : T?) {
        let before = fread(fx.manifest)
        var out : string
        t |> success(run_child_reported(t, "twin cold", twinCmd, out, "PROBE 42"), "the require resolves through the twin's compiled descriptor")
        t |> equal("compiled (no manifest), manifest written (1 row(s))", probe_line(out), "the twin found no manifest of its kind:\n{out}")
        t |> success(stat(twinManifest).is_valid, "the twin's manifest sits beside the descriptor under its own name")
        t |> equal(before, fread(fx.manifest), "this kind's manifest is untouched")
    }
    t |> run("each kind replays its own manifest after the other ran") @(t : T?) {
        var out : string
        t |> success(run_child_reported(t, "this kind warm", fx.cmd, out, "PROBE 42"), "this kind resolves through its rows")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(out), "this kind replays, no binary-kind recompile:\n{out}")
        var again : string
        t |> success(run_child_reported(t, "twin warm", twinCmd, again, "PROBE 42"), "the twin resolves through its rows")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(again), "the twin replays its own:\n{again}")
    }
}

//! the compile targets keep their manifests apart too: a
//! `--jit-target` naming another platform writes its own file beside
//! the native one, and each replays its own - a sweep whose parallel
//! tests mix native and wasm runs never sees a `compile target`
//! recompile
def arms_targets(t : T?; fx : ManifestFixture) {
    let targetCmd = "{fx.cmd} -- --jit-target wasm32-unknown-emscripten"
    let targetManifest = path_join(fx.modDir, target_manifest_file_name("emscripten"))
    t |> run("a wasm target's first start writes its own manifest and leaves the native one alone") @(t : T?) {
        let before = fread(fx.manifest)
        var out : string
        t |> success(run_child_reported(t, "target cold", targetCmd, out, "PROBE 42"), "the require resolves through the target's compiled descriptor")
        t |> equal("compiled (no manifest), manifest written (1 row(s))", probe_line(out), "the target found no manifest of its own:\n{out}")
        t |> success(stat(targetManifest).is_valid, "the target's manifest sits beside the descriptor under its own name")
        t |> success(find(fread(targetManifest), "\ntarget\temscripten\n") >= 0, "the target's manifest names its platform")
        t |> equal(before, fread(fx.manifest), "the native manifest is untouched")
    }
    t |> run("the native run and the target run each replay their own manifest after the other ran") @(t : T?) {
        var out : string
        t |> success(run_child_reported(t, "native warm", fx.cmd, out, "PROBE 42"), "the native run resolves through its rows")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(out), "the native run replays, no compile-target recompile:\n{out}")
        var again : string
        t |> success(run_child_reported(t, "target warm", targetCmd, again, "PROBE 42"), "the target run resolves through its rows")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(again), "the target run replays its own:\n{again}")
    }
}

//! one das root under several spellings is one manifest key
//! (src/misc/ARCHITECTURE_DAS_ROOT.md#das-root-absolute)
def arms_root_spellings(t : T?; fx : ManifestFixture) {
    let root = get_das_root()
    var spellings <- [
        (name = "dot-dot", path = "{root}/modules/.."),
        (name = "trailing slash", path = "{root}/")
    ]
    let fromCwd = relative_result(root, getcwd())
    if (fromCwd is value && !empty(fromCwd as value)) {
        spellings |> push((name = "relative", path = fromCwd as value))
    }
    for (s in spellings) {
        t |> run("a das root spelled with a {s.name} replays the manifest the full path wrote") @(t : T?) {
            let before = fread(fx.manifest)
            var out : string
            t |> success(run_child_reported(t, s.name, fixture_cmd(fx.tmp, das_exe(), s.path), out, "PROBE 42"), "the require resolves under -dasroot {s.path}")
            t |> equal("replayed 1 row(s) (deferred 0)", probe_line(out), "-dasroot {s.path} replays, no das-root recompile:\n{out}")
            t |> equal(before, fread(fx.manifest), "the manifest is untouched")
        }
    }
}

def arms_rejections(t : T?; fx : ManifestFixture) {
    let cmd = fx.cmd
    let manifest = fx.manifest
    t |> run("a damaged manifest recompiles and is rewritten") @(t : T?) {
        let text = fread(manifest)
        let endAt = find(text, "\nend\t")
        // ! drop the end line - a write that died before its rename
        fwrite(manifest, slice(text, 0, endAt + 1))
        var out : string
        t |> success(run_child_reported(t, "damaged", cmd, out, "PROBE 42"), "the damaged manifest is not replayed")
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
        expect_rewrite(t, cmd, manifest, with_line(lines, np, "grp\tg\tm"), "grp field count")
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
        t |> success(run_child_reported(t, "dep", cmd, out, "PROBE 42"), "the require resolves through the helper's answer")
        t |> equal("compiled (descriptor changed), manifest written (1 row(s))", probe_line(out), "the dep descriptor compiles once:\n{out}")
        t |> success(find(fread(manifest), "/helper.das\t") >= 0, "the helper is stamped in the manifest, by the path the require resolved: {fread(manifest)}")
        var warm : string
        t |> success(run_child_reported(t, "dep warm", cmd, warm, "PROBE 42"), "the dep descriptor replays")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(warm), "an unchanged helper replays:\n{warm}")
        fwrite(helper, "{HELPER}// edited\n")
        var edited : string
        t |> success(run_child_reported(t, "dep edited", cmd, edited, "PROBE 42"), "the require resolves after the helper edit")
        t |> equal("compiled (dependency changed), manifest written (1 row(s))", probe_line(edited), "the helper's stamp mismatch recompiles the descriptor:\n{edited}")
        var again : string
        t |> success(run_child_reported(t, "dep edited warm", cmd, again, "PROBE 42"), "the rewritten manifest resolves the require")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(again), "the rewritten manifest replays:\n{again}")
    }
    t |> run("a cross-compile target is part of the key: a native manifest under the target's name is not replayed") @(t : T?) {
        //! dasOpenGL's descriptor registers its module only when
        //! get_cross_platform_name() says emscripten - an argv input, so the
        //! file name keeps the targets apart and the key line still refuses a
        //! manifest written for another target, however it got there
        let targetManifest = path_join(fx.modDir, target_manifest_file_name("emscripten"))
        fwrite(targetManifest, fread(manifest))
        var out : string
        t |> success(run_child_reported(t, "target", "{cmd} -- --jit-target=wasm32-unknown-emscripten", out, "PROBE 42"), "the require resolves under the target")
        t |> equal("compiled (compile target), manifest written (1 row(s))", probe_line(out), "the native rows under the target's name are refused:\n{out}")
        var back : string
        t |> success(run_child_reported(t, "native again", cmd, back, "PROBE 42"), "the native run resolves the require")
        t |> equal("replayed 1 row(s) (deferred 0)", probe_line(back), "the native manifest was never touched:\n{back}")
    }
}

def arms_opt_out_and_write_failures(t : T?; fx : ManifestFixture) {
    let cmd = fx.cmd
    let manifest = fx.manifest
    t |> run("no_manifest() inside the descriptor makes it run on every start") @(t : T?) {
        fwrite(fx.descriptor, DESCRIPTOR_OPT_OUT)
        var out : string
        t |> success(run_child_reported(t, "opt-out", cmd, out, "PROBE 42"), "the opted-out descriptor resolves the require")
        t |> equal("compiled (descriptor changed), no_manifest recorded", probe_line(out), "the first run records the opt-out:\n{out}")
        let text = fread(manifest)
        t |> success(find(text, "\nno_manifest\n") >= 0 && find(text, "\nnp\t") < 0, "the manifest carries the flag and no rows: {text}")
        var again : string
        t |> success(run_child_reported(t, "opt-out warm", cmd, again, "PROBE 42"), "the opted-out descriptor resolves the require again")
        t |> equal("compiled (no_manifest)", probe_line(again), "every later start compiles it:\n{again}")
    }
    t |> run("a recorded argument the format cannot carry leaves the manifest unwritten") @(t : T?) {
        fwrite(fx.descriptor, DESCRIPTOR_TABBED)
        let before = fread(manifest)
        var out : string
        t |> success(run_child_reported(t, "tabbed", cmd, out, "PROBE 42"), "the descriptor still resolves the require")
        t |> equal("compiled (descriptor changed), manifest not written: a recorded argument contains a tab or newline", probe_line(out), "the trace names the field:\n{out}")
        t |> equal(before, fread(manifest), "the stale manifest is left as it was")
    }
    t |> run("a tree the scan cannot write to just compiles, every start") @(t : T?) {
        fwrite(fx.descriptor, DESCRIPTOR)
        remove_result(manifest)
        // ! a directory where the .tmp goes: the create fails on every
        // platform, root or not
        mkdir_result(fx.manifestTmp)
        var out : string
        t |> success(run_child_reported(t, "no tmp", cmd, out, "PROBE 42"), "the require resolves without a manifest")
        t |> success(probe_line(out) |> starts_with("compiled (no manifest), manifest not written: cannot create "), "the trace names the create failure:\n{out}")
        rmdir_result(fx.manifestTmp)
        mkdir_result(manifest)   //! a directory where the manifest goes: the rename fails
        var again : string
        t |> success(run_child_reported(t, "no rename", cmd, again, "PROBE 42"), "the require resolves without a manifest")
        t |> success(find(probe_line(again), ", manifest not written: cannot rename ") >= 0, "the trace names the rename failure:\n{again}")
        t |> success(!stat(fx.manifestTmp).is_valid, "the failed rename left no .tmp behind")
        rmdir_result(manifest)
    }
}

//! the arms share one root and run in this order
[test(description = "The manifest beside each .das_module records and replays descriptors, keeps binary kinds and targets apart, rejects bad manifests and survives opt-out and write failures", timeout = 1200)]
def test_descriptor_manifest(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_dm_manifest", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let fx = make_manifest_fixture(tmp)
    arms_record_and_replay(t, fx)
    arms_binary_kinds(t, fx)
    arms_targets(t, fx)
    arms_root_spellings(t, fx)
    arms_rejections(t, fx)
    arms_opt_out_and_write_failures(t, fx)
    var err : string
    rmdir_rec(tmp, err)
}

var late : Module?

[init]
def load_late() {
    late = require_module_now("%/tests/module_cache/_mc_global_offset_late.das")
}

def read_c() : int => g_c

[test(description = "A module required late from an init keeps the global offsets of the variables around it")]
def test_late_require_keeps_global_offsets(t : T?) {
    t |> success(late != null)
    t |> equal(g_a, 1)
    t |> equal(read_c(), 3)
}
