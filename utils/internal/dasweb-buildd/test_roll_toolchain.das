options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require strings

//! The green predicate of roll_toolchain.sh through its --green-check seam - one recorded
//! actions/runs shape per branch of the rule - and the worktree guard through --dirty-check,
//! on a scratch repository. Needs bash, jq and git on the host, which the extended-checks
//! pull-request lane's macos-15 image carries; elsewhere the suite skips.

let SCRIPT = "utils/internal/dasweb-buildd/roll_toolchain.sh"

def private tool_answers(tool : string) : bool {
    let rc = unsafe(popen_argv([tool, "--version"], 30.0) $(_f) {
    })
    return rc == 0
}

// The script from the repo root, bash, and the one tool the seam under test spawns.
def private tools_present(tool : string) : bool {
    return stat(SCRIPT).is_valid && tool_answers("bash") && tool_answers(tool)
}

def private git(dir : string; args : array<string>) : int {
    var argv <- ["git", "-C", dir, "-c", "core.autocrlf=false", "-c", "user.name=t", "-c", "user.email=t@t"]
    argv |> push_from(args)
    return unsafe(popen_argv(argv, 30.0) $(_f) {
    })
}

// Runs `blk` in a scratch repository whose tracked files, gen.inc and "gen two.inc", are committed
// with CRLF endings - the shape a build's regenerator then rewrites with LF - and removes it after.
def private with_crlf_repo(t : T?; blk : block<(dir : string) : void>) {
    var err = ""
    let dir = create_temp_directory("buildd_dirty_", err)
    if (empty(dir)) {
        t |> failure("no scratch directory: {err}")
        return
    }
    fwrite(path_join(dir, "gen.inc"), "a\r\nb\r\n")
    fwrite(path_join(dir, "gen two.inc"), "a\r\nb\r\n")
    var ready = true
    for (args in [["init", "-q"], ["config", "core.autocrlf", "false"], ["add", "gen.inc", "gen two.inc"], ["commit", "-qm", "gen"]]) {
        let rc = git(dir, args)
        if (rc != 0) {
            t |> failure("git {join(args, " ")} exited {rc} in {dir}")
            ready = false
            break
        }
    }
    if (ready) {
        invoke(blk, dir)
    }
    rmdir_rec(dir)
}

def private dirty_check(dir : string) : int {
    return unsafe(popen_argv(["bash", SCRIPT, "--dirty-check", dir], 30.0) $(_f) {
    })
}

def private green_check(runs_json : string) : int {
    var err = ""
    let dir = create_temp_directory("buildd_green_", err)
    let file = path_join(dir, "runs.json")
    fwrite(file, runs_json)
    let rc = unsafe(popen_argv(["bash", SCRIPT, "--green-check", file], 30.0) $(_f) {
    })
    rmdir_rec(dir)
    return rc
}

def private run_of(name : string; conclusion : string; event : string = "push") : string {
    let c = conclusion == "null" ? "null" : "\"{conclusion}\""
    return "\{\"name\":\"{name}\",\"event\":\"{event}\",\"conclusion\":{c}\}"
}

def private runs_of(items : array<string>) : string {
    return "\{\"workflow_runs\":[" + join(items, ",") + "]\}"
}

def private all_required : array<string> {
    return <- [run_of("wasm_build", "success"), run_of("build", "success"),
        run_of("build_eastl", "success"), run_of("extended checks", "success")]
}

[test]
def test_green_predicate(t : T?) {
    if (!tools_present("jq")) {
        t |> skip("bash + jq not on this host, or not run from the repo root")
        return
    }
    t |> run("all required workflows passed") @(tt : T?) {
        tt |> equal(green_check(runs_of(all_required())), 0)
    }
    t |> run("a required workflow missing is not green") @(tt : T?) {
        tt |> equal(green_check(runs_of([run_of("wasm_build", "success"), run_of("build", "success")])), 1)
    }
    t |> run("a required workflow still running is not green") @(tt : T?) {
        var items <- all_required()
        items[0] = run_of("wasm_build", "null")
        tt |> equal(green_check(runs_of(items)), 1)
    }
    t |> run("any other workflow failing is not green") @(tt : T?) {
        var items <- all_required()
        items |> push(run_of("CodeQL", "failure"))
        tt |> equal(green_check(runs_of(items)), 1)
    }
    t |> run("a skipped extra workflow and a failed pull_request run are ignored") @(tt : T?) {
        var items <- all_required()
        items |> push(run_of("Build doc", "skipped"))
        items |> push(run_of("build", "failure", "pull_request"))
        tt |> equal(green_check(runs_of(items)), 0)
    }
}

[test]
def test_dirty_guard(t : T?) {
    if (!tools_present("git")) {
        t |> skip("bash + git not on this host, or not run from the repo root")
        return
    }
    t |> run("a tracked file a build rewrote with the other line ending is put back, and the roll goes on") @(tt : T?) {
        with_crlf_repo(tt) $(dir) {
            let gen = path_join(dir, "gen.inc")
            let gen_two = path_join(dir, "gen two.inc")
            fwrite(gen, "a\nb\n")
            fwrite(gen_two, "a\nb\n")
            tt |> equal(dirty_check(dir), 0)
            tt |> equal(fread(gen), "a\r\nb\r\n")
            tt |> equal(fread(gen_two), "a\r\nb\r\n", "a path with a space is restored too")
        }
    }
    t |> run("a tracked file whose content changed refuses the roll") @(tt : T?) {
        with_crlf_repo(tt) $(dir) {
            fwrite(path_join(dir, "gen.inc"), "a\nc\n")
            tt |> equal(dirty_check(dir), 12)
        }
    }
    t |> run("a content change in a path with a space refuses the roll") @(tt : T?) {
        with_crlf_repo(tt) $(dir) {
            fwrite(path_join(dir, "gen two.inc"), "a\nc\n")
            tt |> equal(dirty_check(dir), 12)
        }
    }
    t |> run("a staged change refuses the roll") @(tt : T?) {
        with_crlf_repo(tt) $(dir) {
            fwrite(path_join(dir, "new.txt"), "z\n")
            git(dir, ["add", "new.txt"])
            tt |> equal(dirty_check(dir), 12)
        }
    }
    t |> run("a clean worktree passes") @(tt : T?) {
        with_crlf_repo(tt) $(dir) {
            tt |> equal(dirty_check(dir), 0)
        }
    }
}
