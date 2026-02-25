# Writing & Running Benchmarks

Benchmarks use the same `dastest` framework as tests. Benchmark files live under `benchmarks/` organized by category (e.g. `benchmarks/core/hash/`).

## Running benchmarks

```bash
# Interpreter mode
bin/Release/daslang.exe dastest/dastest.das -- --bench --test path/to/benchmark.das

# JIT mode (recommended for performance benchmarks)
bin/Release/daslang.exe -jit dastest/dastest.das -- --bench --test path/to/benchmark.das

# Run all benchmarks in a directory
bin/Release/daslang.exe -jit dastest/dastest.das -- --bench --test benchmarks/core/hash/
```

**Key flags:**
- `--bench` — enables benchmark execution (without this, `[benchmark]` functions are skipped)
- `-jit` — placed **before** `dastest.das` (before `--`), runs dastest itself in JIT mode, which also JITs the benchmark code
- `--test` — specifies the file or directory to run (same as for tests)
- `--bench-names name1,name2` — filter to run only specific benchmark functions

### Capturing benchmark output reliably

The Copilot terminal tool truncates output at ~16KB and mixes it with prior scrollback. **Always pipe benchmark output to a file**, then read the file:

```powershell
# Run a single benchmark file
Push-Location D:\Work\daScript
D:\Work\daScript\bin\Release\daslang.exe D:\Work\daScript\dastest\dastest.das -- --bench --test D:\Work\daScript\benchmarks\core\hash\test02.das 2>&1 | Out-File D:\Work\daScript\_bench_out.txt -Encoding ascii
Pop-Location

# Run a whole directory — run files one at a time to avoid multi-hour waits
$tests = 2..12
foreach ($t in $tests) {
    $f = "D:\Work\daScript\benchmarks\core\hash\test{0:D2}.das" -f $t
    D:\Work\daScript\bin\Release\daslang.exe D:\Work\daScript\dastest\dastest.das -- --bench --test $f 2>&1 | Out-File D:\Work\daScript\_bench_out.txt -Append -Encoding ascii
}
```

Then read results with `read_file` on `_bench_out.txt` — **not** from terminal output.

**Compilation check without running benchmarks**: run without `--bench` to verify all files compile (fast, ~0.5s):
```bash
bin/Release/daslang.exe dastest/dastest.das -- --test benchmarks/core/hash/
```
This reports 0 tests (benchmark functions are skipped) but shows any compile errors.

**Output format:** Each sub-benchmark prints a line like:
```
insert/600000         60 ns/op          84 B/op       1 allocs/op     0 SB/op       0 strings/op
```
Fields: name, nanoseconds per operation, heap bytes per op, heap allocations per op, string heap bytes per op, string allocations per op.

The output shows `[INTERP]` or `[JIT]` after each benchmark name to indicate the execution mode.

## Benchmark file structure

```das
options gen2
options persistent_heap

require dastest/testing_boost

[benchmark]
def my_benchmark(b : B?) {
    b |> run("sub_name", CHUNK_SIZE) <| $() {
        // code to benchmark — this block runs in a loop
    }
}
```

### Required elements

- `options gen2` — always use gen2 syntax
- `options persistent_heap` — benchmarks often allocate; persistent heap avoids GC during measurement
- `require dastest/testing_boost` — provides `[benchmark]` annotation and `B` class
- `[benchmark]` annotation on each benchmark function
- `b : B?` parameter — the benchmark context (like `t : T?` for tests)

### The `run` method

```das
// Simple: each block invocation = 1 logical operation
b |> run("name", op : block)

// With chunk size: each block invocation does `chunk_size` logical operations
b |> run("name", chunk_size : int, op : block)
```

The `run` method:
1. Runs the block once as warmup and to estimate timing
2. Auto-calculates the number of iterations needed for stable measurement
3. Unrolls fast benchmarks (< 100ns) for accuracy
4. Measures CPU time, heap allocations, and string allocations
5. Reports results as per-operation metrics

**`chunk_size`**: If the benchmarked block performs N logical operations internally, pass N as chunk_size so that ns/op reflects a single logical operation. Use string interpolation for descriptive names: `"insert/{HASH_SIZE}"`.

### Benchmark helpers

- `b->failNow()` — abort benchmark on correctness failure (e.g. verification step)
- `b->fail()` — mark as failed but continue
- Helper functions that verify correctness should take `b : B?` as a parameter

### Porting from `profile_test` style

Old benchmarks in `examples/profile/` use `_framework.das` with `profile_test()` and a `test()` function. To port:

1. Replace `require _framework` with `require dastest/testing_boost`
2. Replace `[export] def main` with `[benchmark] def benchmark_name(b : B?)`
3. Convert `profile_test("name", default<Type>, args)` → `b |> run("name") <| $() { ... }`
4. Move the benchmark loop body into the `run` block (dastest handles iteration automatically)
5. Extract setup code (data generation, pre-filling) outside `run` — only the measured operation goes inside the block
6. Use `b->failNow()` instead of `assert` for correctness checks

### Example: hash map benchmark

```das
options gen2
options persistent_heap

require dastest/testing_boost
require daslib/flat_hash_table

typedef FlatHashMap_test = $TFlatHashTable < int; int >

let HASH_SIZE = 600000

[sideeffects]
def fill_map(hmap : auto(HashMapType); size : int) : auto(HashMapType) {
    var hashMap : HashMapType
    static_if (!typeinfo is_table(type<HashMapType>)) {
        hashMap <- HashMapType()
    }
    for (j in range(size)) {
        unsafe(hashMap[j]) = -j
    }
    return <- hashMap
}

def run_write_bench(b : B?; hmap : auto(HashMapType)) {
    b |> run("insert/{HASH_SIZE}", HASH_SIZE) <| $() {
        fill_map(hmap, HASH_SIZE)
    }
}

def run_read_bench(b : B?; hmap : auto(HashMapType)) {
    let m = fill_map(hmap, HASH_SIZE)
    b |> run("read/{HASH_SIZE}", HASH_SIZE) <| $() {
        for (i in range(HASH_SIZE)) {
            let v = m?[i] ?? 0
            if (v != -i) {
                b->failNow()
            }
        }
    }
}

[benchmark]
def builtin_table(b : B?) {
    run_write_bench(b, default<table<int; int>>)
    run_read_bench(b, default<table<int; int>>)
}

[benchmark]
def flat_hashmap(b : B?) {
    run_write_bench(b, default<FlatHashMap_test>)
    run_read_bench(b, default<FlatHashMap_test>)
}
```

### Tips

- **Setup outside `run`**: Pre-generate test data (random numbers, pre-filled maps) before calling `b |> run(...)`. Only the code inside the block is measured.
- **Multiple sub-benchmarks**: A single `[benchmark]` function can call `run` multiple times for related operations (e.g. insert + read).
- **Generic helpers**: Use `auto(HashMapType)` generics to benchmark multiple implementations with the same helper functions.
- **`options unsafe_table_lookup = false`**: Add when benchmarking table `[]` access without `unsafe`.
- **Static struct methods need dot-call**: Custom hash map types (`TCuckooHashTable`, `TFlatHashTable`, `SlotMap`, etc.) define methods like `erase`, `key_exists`, `emplace` as `def static`. These must be called with dot syntax (`hashMap.erase(key)`) — **not** pipe syntax (`hashMap |> erase(key)`), which tries to resolve a free function and fails.
- **Files starting with `_` are skipped by dastest**: Use `_` prefix for helper modules (`_common.das`, `_slot_map.das`) that should not be run as standalone benchmarks/tests.
- **Format after writing**: Run the formatter on benchmark files like any other `.das` file.

## Directory structure

```
benchmarks/
  core/           # Core language/runtime benchmarks
    hash/         # Hash map implementations
      test02.das  # Sequential insert + read (600K elements)
      test03.das  # Random insert, clear, re-insert (1M elements)
```

When adding a new benchmark, choose an appropriate category under `benchmarks/`. Create subdirectories as needed.

## Benchmark index (`benchmarks/README.md`)

The file `benchmarks/README.md` is an index of every `.das` benchmark file. **When you add, remove, or rename a benchmark file, update `benchmarks/README.md`** to keep it in sync:

- Add a row to the correct directory table with the filename and a short description.
- If creating a new subdirectory, add a new section header and table.
