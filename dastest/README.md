# daslang testing framework

## Introduction

Inspired by [Golang testing framework](https://pkg.go.dev/testing), this framework provides a simple way to write unit tests for your scripts.

## Usage

In the examples below, `$target` refers to "folderWithScriptsOrSingleScript".

Running all tests:

- `daslang dastest.das -- --test $target [--test anotherPath]`

Running all tests **and** benchmarks:

- `daslang dastest.das -- --bench --test $target`

Running benchmarks **only**:

- `daslang dastest.das -- --bench --test $target --test-names none`

Running only **some** selected benchmarks (uses `vector_alloc` as a filtering prefix):

- `daslang dastest.das -- --bench --bench-names vector_alloc --test $target --test-names none`

### The `[test]` annotation

`[test]` marks a `def f()` or `def f(t : T?)` as a test and exports it. Its arguments, each optional:

- `name = "..."`: the name the run prints and `--test-names` matches, in place of the function's name
- `description = "..."`: one sentence stating what the test proves. It prints under the test's `--- FAIL` line and nowhere else; the file census reads it without compiling the file
- `timeout = seconds` (an int or a float): this one test's budget. A test that finishes past it is reported failed, naming the budget; the file's `--max-file-time` limit and, in isolated mode, its `--test-timeout` deadline grow by the same amount, so one slow test raises its own limit without loosening any other file's

- `standalone = "..."`: why this test's file cannot share a program with any other - a program-wide registry it fills, a deliberate duplicate that breaks other tests. dastest ignores it; a test-tree gate reads it to accept a small file

Any other argument, or a wrong type, is a compile error from the macro.

### dastest.das arguments

- `--test`: Path to the folder with scripts or single script to test
- `--test-names <namePrefix>`: Run top-level tests matching "namePrefix", such as "namePrefix1"
- `--test-list <path>`: Path to a file listing one test path per line; restricts the collected set to those paths
- `--test-only <path>`: Restrict the collected set to this test path (repeatable)
- `--exclude <substring>`: Skip test files whose base name contains this substring (repeatable)
- `--test-project <path.das_project>`: Project file. Will be used to compile given tests
- `--module-cache-dir <dir>`: Keep AST caches per test file and compile
  configuration, including isolated workers. `--verbose` prints each verdict.
  Custom test projects compile from source because their callbacks are
  outside the cache identity.
- `--uri-paths`: Print uri paths instead of file paths (vscode friendly)
- `--color`: Print colored output
- `--headless`: Passed to every test run, including each `--run` worker, so a harness that reads it opens no window
- `--verbose`: Print verbose output
- `--failures-only`: Only print output for failed tests, suppressing successful ones
- `--compile-only`: Compile test files without executing them
- `--json-file <path>`: Path to write the JSON test results report to
- `--cov-path <path>`: Path to write coverage report data to
- `--timing-outliers <n>`: Print the top N slowest test files and highlight statistical timing outliers (above mean + 2x stddev). `0` (default) prints none
- `--timeout <seconds>`: The whole suite's timeout. Past it the process ends at once with exit code 124 and no summary, after one line naming the timeout and, in the sequential mode, the file that was running (`--failures-only` hides the `run N/M` lines, so this line is the one record). `0` disables it. The default is 20 minutes
- `--max-file-time <seconds>`: Fail after the suite finishes if any completed test file exceeded this wall-clock duration plus the `[test(timeout = N)]` budgets it declares. `0` (default) disables the limit; this is independent of the global `--timeout` deadlock guard.
- `--isolated-mode`: Run tests in isolated processes, useful to catch crashes
- `--isolated-mode-threads <n>`: Number of worker threads in isolated mode. `0` uses a conservative JIT rule (`min(32, max(4, logical_threads / 2))`) and 2x hardware threads otherwise. Preflight can override the JIT width per machine through its user config.
- `--batch <n>`: Files per worker subprocess in isolated mode (semi-isolated sharding). `1` (default) is one process per test (full isolation). `>1` amortizes process/compile cold-start across a batch - much faster, especially on Windows. A crash in a batch is auto-recovered: the file that died is reported as crashed and the rest of the batch is re-run one-process-per-file, so isolation is preserved exactly where it is needed.
- `--together <path>` (repeatable): The listed files compiled as ONE program. A generated root (under `--module-cache-dir`, else the temp folder) carries the files' merged `options` lines and requires every file, so the module graph is parsed and JIT'd once for the list; each file's module then runs in a FRESH context of that program - its globals re-initialized, its `[init]`s run anew, the previous file's `[finalize]`s run at its context's drop, the JIT DLL reloaded from its cache in a few seconds - and reports as its own file (`PASS` / `FAIL` line, result counts). The files' `options` must agree key for key, and none may be an expected-failure compile test (`cant_` / `failed_` / `invalid_`). What a file still shares with the files before it is the program: a module one file requires is in every file's context, so a test that reads the state before some module's `[init]` ran holds only alone. Unlike `--batch`, which runs each file as its own program in one worker, this shares the parse and the JIT
- `--test-timeout <seconds>`: Isolated mode: each file's subprocess deadline, plus every `[test(timeout = N)]` the file declares - a test that raises its own budget raises the file's by N. `0` (default) disables it
- `--use-aot`: Compile and run tests using AOT (ahead-of-time) compilation, failing if AOT is unavailable
- `--aot-macros`: Force `quote()`/`qmacro` lowering (`daslib/quote` QuotePass) when compiling test files
- `--ser <path>`: Path to serialize compiled test programs to (for use with `--deser`)
- `--deser <path>`: Path to a previously serialized program file to load and run tests from (see `--ser`)
- `--debugger`: Set `cop.debugger` so `context.debugger` is truthy, exercising `_Debug` SimNode variants
- `--keep-alive`: Set `cop.keep_alive` so `_KeepAlive` SimNode variants get dispatched
- `--persistent-heap`: Run every test on the persistent heap (`cop.persistent_heap`), whose frees and sweeps an address-sanitizer build hands to its own allocator, so a stale pointer into them is reported; a test that pins the default heap's byte counts fails under it
- `--disable-cse`: Compile every test with the CSE (common-subexpression-elimination) pass disabled (`cop.disable_cse`)
- `--disable-dse`: Compile every test with the DSE (dead-store-elimination) pass disabled (`cop.disable_dse`)
- `--disable-temp-string-reclaim`: Compile every test with the temp-string reclaim pass disabled (`cop.disable_temp_string_reclaim`)
- `--disable-inline`: Compile every test with the `[inline]` function inliner disabled (`cop.disable_inline`)
- `--disable-auto-inline`: Compile every test with automatic block-inlining and invoke-of-literal devirtualization disabled (`cop.disable_auto_inline`)
- `--cache-dir <dir>`: Keep a test result cache in `<dir>`. A file whose last run passed is reported as cached instead of run again, as long as dastest's own sources, every module source its compile read, and every argument except the report-only ones (`--test`, `--failures-only`, `--isolated-mode`, `--color`, the timeouts, ...) are unchanged. The cache does not hash the daslang binaries or the files a test reads at run time; whoever rebuilds the binaries empties `<dir>` (in this repo the `dastest_cache_guard` CMake target does it before every test lane). Off by default; `--bench`, `--cov-path`, `--ser` and `--deser` runs never use it.
- `--stack-on-exception`: On a test panic, walk the call stack *at throw time* (frames intact) and print a real `CALL STACK` trace, instead of the default that reports only the panic message and location. Off by default (a test that swallows a panic via `try`/`recover` would also emit a walk); enable for one-iteration debugging of a failing/crashing test. Works in isolated mode too - the trace is folded under the failing test's log.
- `--bench`: Enable benchmark execution (all of them)
- `--bench-names <namePrefix>`: Run top-level benchmark matching "namePrefix"
- `--bench-format <format>`: Specifies the benchmark output format ("native", "go" or "json")
- `--count <n>`: Run all benchmarks this number of times (useful for sample collection)

Note that benchmarks will be executed only if all module tests have already passed. If you want to run benchmarks only, skipping the tests, consider using something like `--test-names none`.

#### Internal arguments

- `--run`: Script file(s) to run in a single subprocess (repeatable). Used by isolated/semi-isolated mode to dispatch a batch to one worker; each file streams its own JSON report so the parent can attribute results and recover from a mid-batch crash
- `--worker-index <n>`: Stable worker index the parent dastest passes to each worker in isolated mode; tests read it from `get_user_args()` to derive per-worker ports and paths. Default `0`
- `--worker-module-cache-dir <base64>`: Internal encoding of
  `--module-cache-dir` for workers, preserving literal path bytes through
  the process shell.

### Benchmark measurement {#benchmark-measurement}

`run` measures the way Go's `testing.B` does. The first batch is one call - the warm-up and the calibration. Every batch that finishes under the 1 s budget grows the count from what that batch measured (1.2x the projection, at most 100x per round), so a JIT that was cold on the first call is measured warm, and the batch that meets the budget is the one reported: `ns/op` is its wall time over its count. Batches of 1000 calls or more run ten-fold unrolled under a single timer, which divides the loop's own cost by ten; growth rounds such counts down to a multiple of ten, so the reported count is exactly the number of calls timed. A shorter batch times each call instead, because there the call dominates the loop. The benchmark's name is announced after the first batch, so a benchmark that fails fast never prints it. `--count` repeats the whole pass for sample collection; the `go` output format is what `benchstat` reads.

## Examples

### Benchmarking

```das
options gen2
require dastest/testing_boost public

[benchmark]
def dyn_array(b : B?) {
    let size = 100

    b |> run("fill_vec") <| $() {
        var vec : array<int>
        for (i in range(size)) {
            vec |> push(i)
        }
    }

    b |> run("fill_vec_reserve") <| $() {
        var vec : array<int>
        vec |> reserve(size)
        for (i in range(size)) {
            vec |> push(i)
        }
    }
}
```

## Files a test writes

`test_temp_dir()` (`testing_boost`) returns a directory private to the current run of a test
file: created on the first call, removed with everything in it when the run ends. Two runs of the
same file never see each other's files there. The directory belongs to the context that asked for
it, so a `new_thread` body or a job - each runs in its own context - gets a different one.

```das
let out = path_join(test_temp_dir(), "out.png")
```

## review_gate.das

Not part of the test runner: `dastest/review_gate.das` is the support library for
`REVIEW.das` gates - the executable half of a folder's `REVIEW.md` review checklist (the
contract lives in `REVIEW_COMMON.md` at the repo root, vendored by repos that adopt it). It
provides finding accumulation and the exit verdict (`gate_finding`, `gate_findings`,
`gate_reset`, `gate_verdict`), the descriptor census (`gate_descriptor_census`, two
overloads), the cookie-and-host gate for a served web tree (`gate_web_third_party`, built
on `html_resource_urls`, `script_urls`, `css_urls` and `url_host`), the home-directory gate
for a published record (`gate_home_paths`), the prefix census (`gate_prefixed_names_defined`,
built on `prefixed_tokens`, `css_selector_tokens` and `html_ids`), the page-source extractors
(`html_code_blocks`, `markdown_fences`), plus tree-analysis
helpers: `das_requires`, `strip_line_comments`, `cmake_command_blocks`,
`cmake_command_targets`, `cmake_words`, `cmake_args`, `cmake_list_entries`,
`cmake_test_labels`, `cmake_test_commands`, `cmake_copy_sources`, `shell_cp_sources`,
`prefixed_tokens`, `markdown_table_rows`, `is_cmake_keyword`, `is_kebab_case`, `find_line`,
`is_ident_byte`, `contains_word`. The CMake helpers match command names case-insensitively, as CMake itself
does. It lives under
`dastest/` so an installed SDK carries it the same way it carries the test framework -
dastest itself ships in the SDK as `bin/dastest.exe`, a copy of daslang that runs `dastest.das` by
its name, the `DAS_UTILS_SHIPPED_EXES` entry in `utils/CMakeLists.txt` (repo root).

### The web gate's scanned set {#web-gate-scanned-set}

`gate_web_third_party` reads one JSON shape and no other: an asset manifest (a
`.assets.json` sidecar) lists what the playground fetches, so every entry in it is a load.
Any other JSON can spell an address as a `link` - a click, not a load - so scanning it would
report hosts no page ever reaches.
