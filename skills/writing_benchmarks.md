# Writing and running benchmarks

Benchmarks run on the same `dastest` framework as tests — read `skills/writing_tests.md`
for the framework itself; this file is what benchmarks add.

## Running

```bash
bin/daslang dastest/dastest.das -- --bench --test path/to/benchmark.das
bin/daslang -jit dastest/dastest.das -- --bench --test path/to/directory/
```

- `--bench` — without it, `[benchmark]` functions are skipped
- `-jit` goes **before** `dastest.das`: it puts dastest itself in JIT mode, which is what
  gets the benchmark code JIT-compiled. Use it for any performance number you intend to
  believe
- `--bench-names name1,name2` — run only those benchmark functions

Dropping `--bench` turns the same command into a fast compile check: it reports 0 tests but
surfaces every compile error.

Each sub-benchmark prints one line, tagged `[INTERP]` or `[JIT]`:

```
insert/600000         60 ns/op          84 B/op       1 allocs/op     0 SB/op       0 strings/op
```

Fields: name, nanoseconds per operation, heap bytes per op, heap allocations per op, string
heap bytes per op, string allocations per op.

## File structure

```das
options gen2
options persistent_heap

require dastest/testing_boost

[benchmark]
def my_benchmark(b : B?) {
    b |> run("sub_name", CHUNK_SIZE) {
        // measured — this block runs in a loop
    }
}
```

`options persistent_heap` matters: benchmarks allocate, and the persistent heap keeps GC
out of the measurement. `b : B?` is the benchmark context, the counterpart of `t : T?`.

## `run`

```das
b |> run("name", op : block)                      // one block call = one operation
b |> run("name", chunk_size : int, op : block)    // one block call = chunk_size operations
```

`run` calls the block once as warmup, estimates timing, picks an iteration count for a
stable measurement, unrolls blocks faster than 100 ns, and reports CPU time plus heap and
string allocations per operation. Pass `chunk_size` when the block loops internally, so
ns/op stays per logical operation; names take interpolation (`"insert/{HASH_SIZE}"`).

`B` and `T` share the `Asserter` base, so the test assertions work — prefer
`b |> equal(x, y)` over a hand-rolled `if (x != y) { b->failNow() }`. `b->failNow()` aborts
the benchmark, `b->fail()` marks it failed and continues; a correctness helper takes
`b : B?` so it can do either.

## Example

```das
options gen2
options persistent_heap

require dastest/testing_boost

let HASH_SIZE = 600000

def run_read_bench(b : B?; hmap : auto(HashMapType)) {
    let m = fill_map(hmap, HASH_SIZE)          // setup is outside run — not measured
    b |> run("read/{HASH_SIZE}", HASH_SIZE) {
        for (i in range(HASH_SIZE)) {
            b |> equal(m?[i] ?? 0, -i)
        }
    }
}

[benchmark]
def builtin_table(b : B?) {
    run_read_bench(b, default<table<int; int>>)
}
```

## Tips

- **Setup outside `run`.** Pre-generate data before the call; only the block is measured
- One `[benchmark]` function may call `run` several times for related operations
  (insert + read)
- `auto(HashMapType)`-style generic helpers let one body benchmark several implementations
- `options unsafe_table_lookup = false` when benchmarking table `[]` without `unsafe`
- Custom containers (`TCuckooHashTable`, `TFlatHashTable`, `SlotMap`) declare `erase`,
  `key_exists`, `emplace` as `def static` — call them with dot syntax
  (`hashMap.erase(key)`); pipe syntax resolves a free function and fails
- Files starting with `_` are skipped by dastest — use that for shared helper modules
- Format benchmark files like any other `.das`

This repo's own benchmark tree — layout, the index file, output capture:
`skills/internal/benchmarks_in_repo.md` (repo-only).
