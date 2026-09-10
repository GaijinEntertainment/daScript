options gen2
options _lint = "LINT030"   // a child run is never an argument beside the out it writes
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio
require daslib/strings_boost
require daslib/module_group
require _mc_common

// alpha joins probe_group under a guard the build has and path_group under a path guard the
// fixture has; beta joins probe_group unguarded and path_group under a path it lacks; delta joins
// under a guard the build lacks, with no file behind it, and registers alpha's row a second time
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

def driver(requires : string; expr : string) : string {
    return "options gen2\n{requires}\n[export]\ndef main \{\n    print(\"PROBE \{{expr}\}\\n\")\n\}\n"
}

struct Fixture {
    tmp : string
    alphaDir : string
    betaDir : string
    cmdPrefix : string
}

def make_fixture(tmp : string) : Fixture {
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
    return Fixture(
        tmp = tmp,
        alphaDir = alphaDir,
        betaDir = betaDir,
        cmdPrefix = "{trace_prefix()}\"{das_exe()}\" -dasroot \"{get_das_root()}\" -project_root \"{tmp}\""
    )
}

def write_driver(fx : Fixture; name : string; requires : string; expr : string) : string {
    let path = path_join(fx.tmp, name)
    fwrite(path, driver(requires, expr))
    return path
}

def run_driver(fx : Fixture; script : string; var out : string&; cache : string = "-no-module-cache") : int {
    return run_child("{fx.cmdPrefix} {cache} \"{script}\"", out)
}

//! run_driver, reported as run_child_reported reports: the run is its own statement, never an
//! argument beside the `out` it writes
def run_driver_reported(t : T?; phase : string; fx : Fixture; script : string; var out : string&; marker : string; cache : string = "-no-module-cache") : bool {
    let rc = run_driver(fx, script, out, cache)
    return report_child(t, phase, rc, out, marker)
}

//! the scan trace line for a module's descriptor; a replay verdict keeps its deferred count and
//! drops the two timings
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

def count_of(text : string; needle : string) : int {
    var n = 0
    var at = find(text, needle)
    while (at >= 0) {
        n++
        at = find(text, needle, at + length(needle))
    }
    return n
}

def arms_expand(t : T?; fx : Fixture) {
    let useGroup = write_driver(fx, "use_group.das", "require [probe_group]", "alpha_value() + beta_value()")
    t |> run("a cold start compiles the descriptors and the group brings both members in, a member row's guard the build lacks left out") @(t : T?) {
        var out : string
        t |> success(run_driver_reported(t, "cold", fx, useGroup, out, "PROBE 11"), "both members resolve")
        t |> equal("compiled (no manifest), manifest written (3 row(s))", descriptor_verdict(out, "grp_alpha"), "alpha's manifest carries its path row and its two group rows:\n{out}")
        let manifest = fread(path_join(fx.alphaDir, ".das_module.manifest"))
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

def arms_calls(t : T?; fx : Fixture) {
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
        run_child("{lint} \"{oneUsed}\"", out)
        t |> success(find(out, "STYLE030") < 0 && find(out, "STYLE029") < 0 && find(out, "0 issue(s)") >= 0, "beta unused beside a used alpha draws nothing:\n{out}")
        let noneUsed = write_driver(fx, "lint_group_unused.das", "require [probe_group]", "0")
        run_child("{lint} \"{noneUsed}\"", out)
        t |> success(find(out, "STYLE030: require [group] is unused") >= 0 && find(out, "grp_alpha/alpha") >= 0 && find(out, "grp_beta/beta") >= 0 && find(out, "1 issue(s)") >= 0, "an unused group is one finding naming its members:\n{out}")
    }
}

def arm_cache_stamps_the_expansion(t : T?; fx : Fixture) {
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

[test]
def test_member_module_name(t : T?) {
    t |> run("a member's module is its last path segment, less the .das a relative spelling carries") @(t : T?) {
        t |> equal(module_group_module_name("sqlite/sqlite_provider"), "sqlite_provider")
        t |> equal(module_group_module_name("./foo.das"), "foo")
        t |> equal(module_group_module_name("bar"), "bar")
    }
}

[test]
def test_module_groups(t : T?) {
    var terr : string
    let tmp = create_temp_directory("das_module_groups", terr)
    if (empty(tmp)) {
        t |> failure("create_temp_directory: {terr}")
        return
    }
    let fx = make_fixture(tmp)
    arms_expand(t, fx)
    arms_calls(t, fx)
    arm_cache_stamps_the_expansion(t, fx)
    var err : string
    rmdir_rec(tmp, err)
}
