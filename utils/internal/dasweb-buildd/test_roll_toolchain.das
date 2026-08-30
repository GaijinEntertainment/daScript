options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require strings

//! The green predicate of roll_toolchain.sh through its --green-check seam:
//! one recorded actions/runs shape per branch of the rule. Needs bash and jq on
//! the host, which the linux CI lane has; elsewhere the suite skips.

let SCRIPT = "utils/internal/dasweb-buildd/roll_toolchain.sh"

def private tools_present : bool {
    if (!stat(SCRIPT).is_valid) {
        return false
    }
    let rc = unsafe(popen_argv(["jq", "--version"], 30.0) $(_f) {
    })
    return rc == 0
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
    if (!tools_present()) {
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
