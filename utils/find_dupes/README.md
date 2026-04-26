# find_dupes — cross-file similar-function detector

Walks a directory tree of `.das` files, normalises each user function into
an alpha-renamed token stream (identifiers, types, and literals all
collapsed), and reports:

- **Exact-clone clusters** — functions that hash to the same canonical
  text. Pure structural duplicates, modulo names/types/values.
- **Fuzzy near-duplicates** — pairs scored as `sqrt(jaccard × len_ratio)`
  on top of a 64-slot MinHash signature and a hard `len_ratio ≥ threshold`
  gate. The length gate suppresses MinHash false-positives on highly
  periodic boilerplate (otherwise a 4-statement and a 7-statement copy
  of the same `t |> run(...)` block both look 100% identical to MinHash).
  Note that the geometric mean admits Jaccard somewhat below `threshold`
  when lengths match closely — this is intentional and biases toward
  recall.

Useful for surfacing test-suite boilerplate that could be factored,
near-clones that drifted apart, or copy-pasted helpers that escaped
review.

## Usage

```sh
./bin/daslang utils/find_dupes/main.das -- -p tests --json /tmp/dupes.json
./bin/daslang utils/find_dupes/main.das -- -p tests/strings -t 0.85 -n 20
./bin/daslang utils/find_dupes/main.das -- -p daslib --no-fuzzy --min-tokens 32
./bin/daslang utils/find_dupes/main.das -- -p tests -L --min-tokens 16    # cluster dastest run() bodies
./bin/daslang utils/find_dupes/main.das -- -?
```

`./bin/daslang -jit utils/find_dupes/main.das -- …` works too — find_dupes itself JITs cleanly. Net runtime improvement is modest because per-file cost is dominated by interpreter compilation of the scanned files.

Flags:

| Flag | Default | Meaning |
|---|---|---|
| `-p / --path` | required | File or directory to scan; repeatable |
| `-t / --threshold` | 0.7 | Fuzzy similarity floor (0..1). Score is `sqrt(jaccard × len_ratio)` plus a hard `len_ratio ≥ threshold` gate |
| `-n / --top` | 20 | Top-N entries shown in stdout summary |
| `--json` | (off) | Path for full JSON report |
| `-x / --no-fuzzy` | off | Skip MinHash pass — exact clusters only |
| `--min-tokens` | 8 | Drop functions with fewer than N tokens (filters trivial wrappers) |
| `-L / --lambdas-only` | off | Skip top-level functions, keep only lambdas — useful for clustering dastest `t \|> run("…") @(t) { … }` bodies |
| `--export-functions` | (off) | Write all extracted functions to a JSON file and exit before clustering |
| `--import-functions` | (off) | Load functions from a JSON file (produced by `--export-functions`) instead of compiling. Mutually exclusive with `--path` and `--export-functions` |
| `-v / --verbose` | off | Per-file progress |
| `-?` | | Help |

`builtin.das`, `daslib/debugger.das`, and `daslib/profiler.das` are skipped automatically — the latter two install thread-local debug agents at compile time, which would abort the scanner on second use.

## Canonical form

Each function emits a flat tag stream. Examples:

| Source | Canonical (excerpt) |
|---|---|
| `def add(a,b:int):int { return a+b }` | `FN ARG <var_0> TYP ARG <var_1> TYP TYP BODY BLK STMT RET OP2:+ <var_0> <var_1> ENDBLK ENDFN` |
| `def add(a,b:float):float { return a+b }` | (same — types collapse) |
| `def double(a:int) { return a*2 }` | `FN ARG <var_0> TYP BODY BLK STMT RET OP2:* <var_0> LIT ENDBLK ENDFN` |

User identifiers → `<var_0>`, `<var_1>`, …
All types → `TYP`. All literals → `LIT`. Field/swizzle names → `.FLD` /
`.SWZ`. Called function names are kept (`CALL:push` vs `CALL:emplace`
is real signal).

## Implementation

| File | Role |
|---|---|
| `canonical.das` | `CanonicalVisitor` (extends `daslib/ast` `AstVisitor`) and `tokenize_canonical` |
| `minhash.das` | 64-slot MinHash signatures over 5-grams, Jaccard estimate |
| `cluster.das` | Exact-bucket clustering + fuzzy all-pairs with length gate |
| `report.das` | JSON + stdout summary writer |
| `main.das` | CLI (`daslib/clargs`), file scan, compile-and-collect orchestration |
| `pipeline.das` | `compile_and_collect` / `collect_from_program` — compile-and-extract orchestration, shared by `main.das` and the test suite |
| `exchange.das` | On-disk JSON schema + writer/reader for `--export-functions` / `--import-functions` |
| `fixture/synth.das` | Hand-crafted fixture for smoke-testing the visitor end-to-end |
| `fixture/canonical_cases.das` | Narrowly-targeted fixture (one function per canonicalization concern) for unit-testing `CanonicalVisitor` |
| `test_find_dupes.das` | dastest suite — run with `bin/daslang dastest/dastest.das -- --test utils/find_dupes/test_find_dupes.das` |

Notes:

- Compile policy mirrors `utils/lint`: `ignore_shared_modules`,
  `export_all`. Optimisations and infer-time folding stay ON so dastest
  macros (e.g. `unroll`) compile.
- **Default mode** drops everything `generated` (which includes
  lambdas). The dispatcher already references each lambda via an `ADDR`
  token, so the lambda's structural fingerprint is partially preserved
  in the parent function. Use `-L` to flip this and cluster the lambda
  bodies themselves.
- **Lambda-only mode (`-L`)** is dominated at the top by linq's `each`
  macro emissions (a 100+-token `GOTO/LABEL/_builtin_iterator_first /
  next/close` shell that recurs hundreds of times). Real test-body
  signal starts a few clusters down; sort/grep accordingly.
- Functions whose `at.fileInfo` points outside the compiled file are
  filtered out — without this, reified generics from required modules
  (e.g. `dastest/testing.das`) flood the report.
- Per-source-line dedup: a generic reified for N types becomes N
  FunctionPtrs all pointing at the same `(file, line)`. We keep the
  first to avoid the same source location being counted N times.

## Export / import

Compilation is the expensive step; the canonical-form computation is deterministic. To hand the function list off to an external tool (visualizer, custom clusterer), or to shard compilation across machines and merge later, dump the post-canonicalization records and reload them:

```sh
# compile + extract, write JSON, exit before clustering
./bin/daslang utils/find_dupes/main.das -- -p tests --export-functions /tmp/funcs.json

# skip compilation; cluster + report from JSON (--json still works as before)
./bin/daslang utils/find_dupes/main.das -- --import-functions /tmp/funcs.json --json /tmp/dupes.json
```

`--import-functions` is mutually exclusive with both `--path` and `--export-functions`. `--export-functions` always exits before the clustering pass.

The on-disk schema is a small envelope:

```json
{
  "schema_version": 1,
  "functions": [
    { "name": "add_int", "file": "tests/foo.das", "line": 4,
      "is_lambda": false, "canonical": "FN ARG <var_0> TYP …" }
  ]
}
```

MinHash signatures are not included — they're recomputed on import (deterministic and cheap). On import, `--no-fuzzy` and `--min-tokens` apply just like in the compile path.

## Out of scope

- LSH / banding (4.4K² is fine; revisit at >50K functions).
- Embedding-based similarity (would need an external service).
- Auto-fix or refactor suggestions — discovery only.
- CI integration — first read what it finds; promote findings to lint
  rules later.
