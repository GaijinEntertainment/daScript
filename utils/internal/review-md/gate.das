options gen2

require strings
require daslib/fio

//! Child-process plumbing: REVIEW.das gates run with the walker's own binary (never another
//! tree's — the worktree trap) and cwd at the repo root, which is the gate contract.

def run_argv(args : array<string>) : tuple<rc : int; out : string> {
    var output : string
    let rc = unsafe(popen_argv(args, 0.0) $(f) {
        if (f != null) {
            output = fread(f)
        }
    })
    return (rc = rc, out = output)
}

//! The running binary, spawn-safe across a chdir: a path-form argv[0] (CI's ./bin/daslang)
//! is absolutized so it survives the cwd change; a bare name stays bare for PATH resolution.
def walker_self_binary : string {
    let argv <- get_command_line_arguments()
    if (find(to_generic_path(argv[0]), "/") < 0) {
        return argv[0]
    }
    return get_full_file_name(normalize(argv[0]))
}

//! Execute each gate (repo-relative path), printing its output. Returns the red-gate count.
def run_gates(root : string; gates : array<string>) : int {
    return 0 if (empty(gates))
    let self = walker_self_binary()
    let prev = getcwd()
    if (!chdir(root)) {
        to_log(LOG_ERROR, "review-md: cannot chdir to {root}\n")
        return length(gates)
    }
    var failed = 0
    for (g in gates) {
        var out : string
        let rc = run_and_capture([self, g], out, 120.0)
        print(out)
        if (rc != 0) {
            failed++
        }
    }
    chdir(prev)
    return failed
}
