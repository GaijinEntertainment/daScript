options gen2

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

//! Execute each gate (repo-relative path), printing its output. Returns the red-gate count.
def run_gates(root : string; gates : array<string>) : int {
    return 0 if (empty(gates))
    let argv <- get_command_line_arguments()
    let self = normalize(argv[0])
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
