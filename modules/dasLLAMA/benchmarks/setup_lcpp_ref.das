options gen2

require daslib/fio
require daslib/clargs
require strings
require daslib/strings_boost

// The reference supply chain (history/dasLLAMA/PUBLIC_BENCH_PLAN.md): build pinned llama-bench
// binaries per build flavor, so every box — and any community user who wants our exact reference —
// benches against the identical binary. One git worktree at the pinned sha, one build dir per flavor:
//   clean-cpu: -DGGML_ACCELERATE=OFF -DGGML_BLAS=OFF -DGGML_METAL=OFF — kernel-vs-kernel fair
//              (on Apple Silicon, Accelerate routes GEMM through the AMX coprocessor; das's own
//              kernels don't use it)
//   stock:     release defaults — llama.cpp as people actually run it (Accelerate + Metal on
//              macOS); one build serves CPU rows (-ngl 0) and Metal rows (-ngl 99)
// Usage: bin/daslang modules/dasLLAMA/benchmarks/setup_lcpp_ref.das -- --src ~/Work/llama.cpp
// Point lcpp_bench --ref (or LLAMA_BENCH) at the reported binaries.

// bump deliberately; the sha a record carries is what pins its ref, this is just the default
let DEFAULT_REF_SHA = "ebd048fc5e4b43ec4e0b4abe0b9bf66e1724dad0"

[CommandLineArgs]
struct Args {
    @clarg_required
    @clarg_doc = "Path to a llama.cpp git checkout (worktrees and builds are created NEXT to it)"
    src : string

    @clarg_doc = "llama.cpp commit to pin (default: the standing reference sha)"
    sha : string

    @clarg_doc = "Build flavor, repeatable: clean-cpu | stock (default: both)"
    flavor : array<string>

    @clarg_short = "j"
    @clarg_doc = "Parallel build jobs (default: 8)"
    jobs : int = 8

    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

// Run `cmd`, return its trimmed stdout (pipe-safe fread; stderr stays on the console).
def private capture_out(cmd : string) : string {
    var out = ""
    unsafe {
        popen(cmd) $(f) {
            if (f != null) {   // popen returns null on spawn failure (missing git / bad PATH) — don't deref
                out = fread(f)
            }
        }
    }
    return strip(out)
}

// Run a command with inherited stdio so git/cmake progress streams live.
def private run_visible(cmd : string) : int {
    print("$ {cmd}\n")
    return unsafe(system(cmd))
}

// Reject paths that could break out of the double-quoted shell commands below — the quote itself,
// POSIX command substitution, cmd.exe variable expansion, and newlines (the mcp/setup.das guard).
def private shell_unsafe(s : string) : bool {
    return (find(s, "\"") >= 0 || find(s, "`") >= 0 || find(s, "$") >= 0 ||
        find(s, "%") >= 0 || find(s, "\n") >= 0 || find(s, "\r") >= 0)
}

def private flavor_flags(flavor : string) : string {
    if (flavor == "clean-cpu") {
        return " -DGGML_ACCELERATE=OFF -DGGML_BLAS=OFF -DGGML_METAL=OFF"
    }
    return ""   // stock: release defaults
}

// The built llama-bench, wherever this platform's generator put it; "" when the build failed.
def private locate_bench(build_dir : string) : string {
    let candidates = ["bin/llama-bench", "bin/Release/llama-bench.exe", "bin/llama-bench.exe"]
    for (rel in candidates) {
        let p = path_join(build_dir, rel)
        if (fexist(p)) {
            return p
        }
    }
    return ""
}

// create the ref worktree at `full`, or verify an existing one sits there. "" = error (printed).
def private ensure_ref_worktree(src, full, short_sha : string) : string {
    let dest = "{src}-ref-{short_sha}"
    if (!stat(dest).is_valid) {
        print("creating worktree {dest} @ {short_sha}\n")
        if (run_visible("git -C \"{src}\" worktree add \"{dest}\" {full}") != 0) {
            print("error: worktree add failed\n")
            return ""
        }
        return dest
    }
    let head = capture_out("git -C \"{dest}\" rev-parse HEAD")
    if (head != full) {
        print("error: {dest} exists but is at {head}, not {full} — remove it (git -C \"{src}\" worktree remove \"{dest}\") and re-run\n")
        return ""
    }
    print("worktree {dest} already at {short_sha}\n")
    return dest
}

[export]
def main : int {
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "setup_lcpp_ref")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help) {
        print_help(get_command_info(type<Args>), "setup_lcpp_ref")
        return 0
    }
    var flavors := cfg.flavor
    if (empty(flavors)) {
        flavors <- ["clean-cpu", "stock"]   // nolint:PERF030 — the enclosing empty(flavors) guard proves the target holds nothing
    }
    for (f in flavors) {
        if (f != "clean-cpu" && f != "stock") {
            print("error: unknown flavor `{f}` — expected clean-cpu | stock\n")
            return 1
        }
    }
    let src = cfg.src
    if (shell_unsafe(src) || shell_unsafe(cfg.sha)) {
        print("error: --src / --sha contain shell-special characters\n")
        return 1
    }
    if (!stat(src).is_valid) {
        print("error: --src `{src}` does not exist\n")
        return 1
    }
    let want = empty(cfg.sha) ? DEFAULT_REF_SHA : cfg.sha
    let full = capture_out("git -C \"{src}\" rev-parse --verify --quiet \"{want}^\{commit\}\"")
    if (empty(full)) {
        print("error: `{want}` is not a commit in {src} — fetch it first (git -C \"{src}\" fetch origin)\n")
        return 1
    }
    let short_sha = slice(full, 0, 7)
    let dest = ensure_ref_worktree(src, full, short_sha)
    if (empty(dest)) {
        return 1
    }
    var rc = 0
    for (f in flavors) {
        if (build_flavor(dest, f, short_sha, cfg.jobs) != 0) {
            rc = 1
        }
    }
    return rc
}

def private build_flavor(dest, f, short_sha : string; jobs : int) : int {
    let build_dir = path_join(dest, "build-{f}")
    print("=== {f} -> {build_dir} (minutes-class: configure + build llama-bench)\n")
    if (!fexist(path_join(build_dir, "CMakeCache.txt"))) {
        let cfg_cmd = "cmake -S \"{dest}\" -B \"{build_dir}\" -DCMAKE_BUILD_TYPE=Release -DLLAMA_CURL=OFF{flavor_flags(f)}"
        if (run_visible(cfg_cmd) != 0) {
            print("error: cmake configure failed for {f}\n")
            return 1
        }
    }
    if (run_visible("cmake --build \"{build_dir}\" --config Release --target llama-bench -j {jobs}") != 0) {
        print("error: build failed for {f}\n")
        return 1
    }
    let bench = locate_bench(build_dir)
    if (empty(bench)) {
        print("error: {f} built but llama-bench not found under {build_dir}\n")
        return 1
    }
    print("{f}: {bench} (llama.cpp {short_sha})\n")
    print("    lcpp_bench: --ref \"{bench}\" --ref-flavor {f}\n")
    return 0
}
