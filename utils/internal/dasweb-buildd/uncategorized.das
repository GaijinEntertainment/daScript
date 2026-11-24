options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require strings

//! The green predicate of roll_toolchain.sh through its --green-check
//! seam - one recorded actions/runs shape per branch of the rule -
//! and the worktree guard through --dirty-check, on a scratch
//! repository; and the exit contract of run_build.sh against a stub
//! podman. Needs bash, jq and git on the host, which the ubuntu image
//! of the extended-checks pull-request lane's `modules` job carries;
//! elsewhere the suite skips.

let SCRIPT = "utils/internal/dasweb-buildd/roll_toolchain.sh"
let BUILD_SCRIPT = "utils/internal/dasweb-buildd/run_build.sh"

def private tool_answers(tool : string) : bool {
    let rc = unsafe(popen_argv([tool, "--version"], 30.0) $(_f) {
    })
    return rc == 0
}

//! WSL's bash, which a Windows process finds first, answers --version
//! but cannot open this process's paths.
def private bash_opens_host_paths : bool {
    var err = ""
    return unsafe(
                  popen_argv(["bash", "-c", "test -d \"$1\"", "_", temp_directory(err)],
                             30.0) $(_f) {
    }) == 0
}

def private tools_present(tool : string) : bool =>
    stat(SCRIPT).is_valid && tool_answers("bash") && bash_opens_host_paths() && tool_answers(tool)

def private git(dir : string; args : array<string>) : int {
    var argv <- ["git", "-C", dir, "-c", "core.autocrlf=false", "-c", "user.name=t", "-c",
                 "user.email=t@t"]
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
    for (
         args in [["init", "-q"], ["config", "core.autocrlf", "false"],
                  ["add", "gen.inc", "gen two.inc"], ["commit", "-qm", "gen"]]) {
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

def private runs_of(items : array<string>) : string =>
    "\{\"workflow_runs\":[" + join(items, ",") + "]\}"

def private all_required : array<string> {
    return <- [run_of("wasm_build", "success"), run_of("build", "success"),
        run_of("build_eastl", "success"), run_of("extended checks", "success")]
}

[test(description = "roll_toolchain.sh calls a commit green only when every required workflow passed on push, ignoring skipped extras and pull_request runs", timeout = 1200)]
def test_green_predicate(t : T?) {
    if (!tools_present("jq")) {
        t |> skip("no bash that opens this host's paths, no jq, or not run from the repo root")
        return
    }
    t |> run("all required workflows passed") @(tt : T?) {
        tt |> equal(green_check(runs_of(all_required())), 0)
    }
    t |> run("a required workflow missing is not green") @(tt : T?) {
        tt |> equal(
                    green_check(
                                runs_of(
                                        [run_of("wasm_build", "success"),
                                         run_of("build", "success")])), 1)
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

[test(description = "the roll's worktree guard restores line-ending and regenerated-lexer rewrites and refuses a real content or staged change with exit 12", timeout = 1200)]
def test_dirty_guard(t : T?) {
    if (!tools_present("git")) {
        t |> skip("no bash that opens this host's paths, no git, or not run from the repo root")
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
    t |> run("a committed flex output the build regenerated with other content is put back, and the roll goes on") @(tt : T?) {
        with_crlf_repo(tt) $(dir) {
            let lexer = path_join(dir, "src/parser/ds2_lexer.cpp")
            mkdir_rec(path_join(dir, "src/parser"))
            fwrite(lexer, "committed\n")
            git(dir, ["add", "src/parser/ds2_lexer.cpp"])
            git(dir, ["commit", "-qm", "lexer"])
            fwrite(lexer, "regenerated by another flex\n")
            tt |> equal(dirty_check(dir), 0)
            tt |> equal(fread(lexer), "committed\n")
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

struct private BuildStage {
    root : string
    worktree : string
    stub_podman_dir : string
}

//! A scratch worktree run_build.sh accepts and a stub podman whose
//! `run` exits $FAKE_SANDBOX_RC; removed after `blk`.
def private with_build_stage(t : T?; blk : block<(stage : BuildStage) : void>) {
    var err = ""
    let root = create_temp_directory("buildd_recipe_", err)
    if (empty(root)) {
        t |> failure("no scratch directory: {err}")
        return
    }
    let stage = BuildStage(root = root, worktree = path_join(root, "wt"),
                           stub_podman_dir = path_join(root, "stub"))
    let host = path_join(path_join(stage.worktree, "bin"), "daslang")
    let podman = path_join(stage.stub_podman_dir, "podman")
    mkdir_rec(path_join(stage.worktree, "bin"))
    mkdir_rec(path_join(stage.worktree, "web/output64/lib"))
    mkdir_rec(stage.stub_podman_dir)
    mkdir_rec(path_join(root, "src"))
    mkdir_rec(path_join(root, "out"))
    fwrite(host, "#!/usr/bin/env bash\n")
    fwrite(path_join(stage.worktree, "web/output64/lib/liblibDaScript_runtime.a"), "archive")
    fwrite(podman, ("#!/usr/bin/env bash\nif [ \"$1\" = image ]; then exit 0; fi\n" +
        "exit \"$FAKE_SANDBOX_RC\"\n"))
    let chmod_rc = unsafe(popen_argv(["chmod", "+x", host, podman], 30.0) $(_f) {
    })
    if (chmod_rc == 0) {
        invoke(blk, stage)
    } else {
        t |> failure("chmod exited {chmod_rc}")
    }
    rmdir_rec(root)
}

def private recipe_env(stage : BuildStage; sandbox_rc : int; worktree : string;
                       emsdk : string) : array<string> {
    let host_path = get_env_variable("PATH")
    let list_separator = find(host_path, ";") >= 0 ? ";" : ":"
    return <- ["PATH={stage.stub_podman_dir}{list_separator}{host_path}",
               "DASWEB_WASM_WORKTREE={worktree}",
        "EMSDK={emsdk}", "FAKE_SANDBOX_RC={sandbox_rc}"]
}

def private run_build_exit_status(stage : BuildStage; sandbox_rc : int; mode : string = "module";
                                  emsdk : string = "/emsdk") : int {
    let argv <- ["bash", BUILD_SCRIPT, path_join(stage.root, "src"), path_join(stage.root, "out"),
                 mode, "main.das"]
    return unsafe(
                  popen_argv(argv, "", recipe_env(stage, sandbox_rc, stage.worktree, emsdk),
                             30.0) $(_f) {
    })
}

[test(description = "run_build.sh exits 0 for a build that passed, 65 for one that failed in the sandbox whatever its status or mode, and a fault status - never 65 - when podman, the mode, the environment or the argument list is what failed", timeout = 1200)]
def test_build_exit_contract(t : T?) {
    if (!stat(BUILD_SCRIPT).is_valid || !tool_answers("bash") || !bash_opens_host_paths()) {
        t |> skip("no bash that opens this host's paths, or not run from the repo root")
        return
    }
    with_build_stage(t) $(stage) {
        t |> run("a build that passed") @(tt : T?) {
            tt |> equal(run_build_exit_status(stage, 0), 0)
        }
        t |> run("a build that failed in the sandbox is the verdict, at any status") @(tt : T?) {
            tt |> equal(run_build_exit_status(stage, 1), 65)
            tt |> equal(run_build_exit_status(stage, 2), 65)
            tt |> equal(run_build_exit_status(stage, 137), 65)
        }
        t |> run("podman failing to run the build is a fault") @(tt : T?) {
            tt |> equal(run_build_exit_status(stage, 125), 20)
            tt |> equal(run_build_exit_status(stage, 126), 20)
            tt |> equal(run_build_exit_status(stage, 127), 20)
        }
        t |> run("a page build maps the same way") @(tt : T?) {
            tt |> equal(run_build_exit_status(stage, 0, "page"), 0)
            tt |> equal(run_build_exit_status(stage, 1, "page"), 65)
        }
        t |> run("an unknown mode and a missing environment are faults") @(tt : T?) {
            tt |> equal(run_build_exit_status(stage, 0, "bogus"), 14)
            tt |> equal(run_build_exit_status(stage, 0, "module", ""), 17)
        }
        t |> run("a missing worktree variable and a missing argument are faults") @(tt : T?) {
            let no_worktree = unsafe(popen_argv(["bash", BUILD_SCRIPT, path_join(stage.root, "src"),
                path_join(stage.root, "out"), "module", "main.das"], "",
                recipe_env(stage, 0, "", "/emsdk"), 30.0) $(_f) {
            })
            tt |> equal(no_worktree, 17)
            let three_arguments = unsafe(
                                         popen_argv(
                                                    ["bash", BUILD_SCRIPT,
                                                     path_join(stage.root, "src"),
                path_join(stage.root, "out"), "module"], "",
                recipe_env(stage, 0, stage.worktree, "/emsdk"), 30.0) $(_f) {
            })
            tt |> equal(three_arguments, 17)
        }
        t |> run("a worktree with no runtime archive is a fault") @(tt : T?) {
            remove(path_join(stage.worktree, "web/output64/lib/liblibDaScript_runtime.a"))
            tt |> equal(run_build_exit_status(stage, 0), 11)
        }
    }
}
