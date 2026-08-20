# detect-dupe --- Cross-file similar-function detector

`detect-dupe` normalises every user function in a corpus of `.das` files into an
alpha-renamed token stream (identifiers, types and literals collapsed) and reports
near-identical functions across it. One engine behind two interfaces: the CLI
`utils/detect-dupe/main.das` and the MCP tools `export_corpus` / `detect_duplicates`.

```{contents}
:local:
:depth: 2
```

## What it reports

* **Exact-clone clusters** --- canonical token streams byte-identical.
* **Fuzzy near-duplicates** --- pairs scored `sqrt(jaccard × len_ratio)` over a
  64-slot MinHash signature, gated on `len_ratio >= threshold`. The geometric mean
  admits a Jaccard somewhat below `threshold` when lengths match closely --- a
  deliberate recall bias.


## Flags

Invocation is `bin/daslang utils/detect-dupe/main.das -- <flags>`.

| Flag | Default | Meaning |
|---|---|---|
| `-p / --path` | required† | File or directory to scan; repeatable |
| `--paths-from` | off | Newline-delimited path list from a file (`#`-comments and blanks skipped); composes with `-p`; entries may be directories |
| `--paths-stdin` | off | Same, from stdin. Mutually exclusive with `--against-from-stdin` (one stdin reader per run) |
| `-j / --workers` | 0 (auto) | Workers for `--export-functions`; 0 = hardware threads, 1 = sequential. Ignored without `--export-functions` |
| `-t / --threshold` | 0.7 | Fuzzy floor (0..1) on `sqrt(jaccard × len_ratio)`, plus a hard `len_ratio >= threshold` gate |
| `-n / --top` | 20 | Top-N entries in the stdout summary |
| `--json` | off | Path for the full JSON report |
| `-x / --no-fuzzy` | off | Skip the MinHash pass --- exact clusters only |
| `--min-tokens` | 8 | Drop functions with fewer than N tokens |
| `-L / --lambdas-only` | off | Skip top-level functions, keep only lambdas |
| `--export-functions` | off | Write all extracted functions to a JSON file and exit before clustering |
| `--import-functions` | off | Load functions from an `--export-functions` JSON instead of compiling. Mutually exclusive with `--path` and `--export-functions` |
| `--baseline` | off | B1: load corpus JSON; tag records whose member identity (`file:line:name`) is absent from it as candidates, and filter to those |
| `--baseline-strict` | off | B1 modifier: also drop clusters whose canonical exists in the baseline (only fully-new clusters survive) |
| `--against` | off | B2 candidate path (file or directory), repeatable. Compiled in-process, tagged candidates, report filtered |
| `--against-from-stdin` | off | B2 candidate paths from stdin, newline-delimited |
| `--check` | off | Exit non-zero when the post-filter report contains any clusters/pairs (CI gate) |
| `--flat` | off | In `--against` mode, force the flat clusters/pairs writer instead of the default per-candidate rollup |
| `-k / --keep` | off | Pattern name to KEEP despite default skip (repeatable); `all` disables pattern filtering entirely |
| `-v / --verbose` | off | Per-file progress |
| `-?` | | Show help |

† one of `-p`, `--paths-from`, `--paths-stdin`, `--import-functions` or `--against`
is required.

`builtin.das`, `daslib/debugger.das`, `daslib/profiler.das`, and any path containing
`ast-fuzz/selftest/` (deliberately-broken AST fixtures) are skipped automatically ---
the two daslib files install thread-local debug agents at compile time, which abort
the scanner on the second use.


(utils_detect_dupe_patterns)=
## Pattern filter

A "pattern" is a structural shape whose canonical token stream carries no signal
beyond its repetition count. Matched functions are dropped from clustering by
default.

| Name | Detects | Why it's boilerplate |
|---|---|---|
| `visitor` | Class-method whose hook name starts with `visit`, `preVisit`, `postVisit`, `before` or `after` (by name, regardless of body) | `AstVisitor` overrides --- one method per AST node type is the dispatch contract, so cross-class duplication is structural, not actionable |
| `dispatch` | Body is N >= 2 byte-identical top-level statement chunks | dastest `t \|> run("X") @(t) { … }` outer functions, `t \|> bench(…)`, repeated-init blocks, any uniform call list. Lambda bodies collapse to `ADDR` upstream, so two `run` calls look identical |
| `test_wrapper` | Name starts `test_`, body is one top-level `CALL:run` statement carrying at least one `ADDR` (a lambda) | dastest `[test]` shells --- one `t \|> run(…) @(t) { … }` per test; the lambda collapses to `ADDR`, so every such wrapper looks identical |
| `emit` | 1..6 top-level statements, each a single trivial `CALL:foo(...)` (literal/var/field args only --- no nested calls, no control flow) or a `RET ...` | Emitter shells like `def visitX(...) { write(*ss, ")") ; return that }` --- free-function variants the name-based `visitor` matcher doesn't cover |

Match order in `classify()` is name-first (`visitor`), then body-shape (`dispatch`,
`test_wrapper`, `emit` --- `test_wrapper` before `emit` because it is the more
specific name+shape match); first match wins.

The stdout summary reports `patterns skipped: N dispatch, N emit, N visitor`;
`--verbose` adds one `pattern-skip [name] file:line func (note)` line per filtered
record.


## Canonical form

Each function emits a flat tag stream:

```
def add(a,b:int):int { return a+b }
  FN ARG <var_0> TYP ARG <var_1> TYP TYP BODY BLK STMT RET OP2:+ <var_0> <var_1> ENDBLK ENDFN
def double(a:int) { return a*2 }
  FN ARG <var_0> TYP BODY BLK STMT RET OP2:* <var_0> LIT ENDBLK ENDFN
```

User identifiers become `<var_0>`, `<var_1>`, …; all types collapse to `TYP`; all
literals to `LIT`; field/swizzle names use `.FLD` / `.SWZ`. Called function names
are kept --- `CALL:push` vs `CALL:emplace` is real signal.


## Modes

`-p` alone reports flat. Two filtered modes layer on top via a single
`is_candidate` flag inside `FuncRecord`: a cluster or fuzzy pair is **kept** iff at
least one of its members is a candidate. An AI judge
([utils_find_dupe](https://daslang.io/doc/reference/utils/find_dupe.html#utils-find-dupe))
can triage the resulting JSON into real duplicates, partial matches and false
positives.


### B1 --- baseline diff (CI gate)

```das
# one-off: build the baseline (commit this)
bin/daslang utils/detect-dupe/main.das -- -p tests --export-functions tests_baseline.json

# CI: scan again, surface only what isn't in the baseline
bin/daslang utils/detect-dupe/main.das -- -p tests --baseline tests_baseline.json --check
```

Growth counts, not just brand-new canonicals: a new copy of an already-tracked
canonical in a new location surfaces its cluster. `--baseline-strict` drops that
case. Pairs are never strict-filtered (the baseline carries no MinHash signatures),
so strict is cluster-only.

`file:line:name` keying means an unrelated edit that shifts line numbers looks like
a new member and surfaces its cluster --- acceptable for CI, since touched code is
the right default to re-check.


### B2 --- PR-files / interactive

"Did I just write something that already exists?" --- compare a file list against a
pre-built corpus:

```das
bin/daslang utils/detect-dupe/main.das -- \
    --import-functions tests_baseline.json --against tests/strings/new_helper.das

# git pipeline:
git diff --name-only master | grep '\.das$' | \
    bin/daslang utils/detect-dupe/main.das -- \
        --import-functions tests_baseline.json --against-from-stdin
```

With `--against` and `--import-functions` both set, corpus records whose `file`
matches any candidate path are dropped first (look for the
`dropped N corpus records overridden` line), then the candidate is freshly compiled
--- so the file is compared against the rest of the world, never against its own
stale copy in the baseline.


## Export / import

Dump post-canonicalization records to hand off to another tool, or to shard
compilation across machines and merge later:

```das
bin/daslang utils/detect-dupe/main.das -- -p tests --export-functions /tmp/funcs.json
bin/daslang utils/detect-dupe/main.das -- --import-functions /tmp/funcs.json --json /tmp/dupes.json
```

The on-disk schema is a small envelope:

```json
{
  "schema_version": 1,
  "functions": [
    {
      "name": "add_int",
      "file": "tests/foo.das",
      "line": 4,
      "is_lambda": false,
      "canonical": "FN ARG <var_0> TYP ..."
    }
  ]
}
```

MinHash signatures are not included --- they're recomputed on import. On import,
`--no-fuzzy` and `--min-tokens` apply just as in the compile path.

### Parallel export (`-j / --workers`)

`--workers N` fans the export across N child detect-dupe processes: the file list
is sorted, split into N contiguous chunks, and the parent reads the shards back
**in chunk-index order**, so output is byte-identical to a `--workers 1` run on the
same inputs. Below 16 input files the export stays sequential regardless. A compile
failure in any child fails the whole export --- same gate as the sequential path.

### Explicit file-list inputs

`--paths-from <file>` and `--paths-stdin` scope an export to a precomputed list,
typically a PR diff; the file form avoids ARG_MAX on big PRs.

```sh
git diff --name-only master | grep '\.das$' | \
    bin/daslang utils/detect-dupe/main.das -- \
        --paths-stdin --export-functions pr.json
```

For a comma- or newline-separated list of files, directories and globs in a single
argument (the format all MCP file/glob tools take), the canonical expander is
`daslib/fio.parse_file_list` --- it strips whitespace, passes literals through,
expands globs via `expand_glob`, and preserves the order of plain entries across
glob expansions.


## MCP integration

The [utils_mcp](https://daslang.io/doc/reference/utils/mcp.html#utils-mcp) server
wraps the engine end-to-end:

| Tool | Purpose |
|---|---|
| `export_corpus` | Scan `paths` (files / directories / globs), compile each `.das`, write a corpus JSON to `out`. Replaces the CLI `--export-functions` |
| `detect_duplicates` | B2 mode. Pass `paths` (newline- or comma-delimited, or a glob) and `corpus` (a JSON from `export_corpus` / `--export-functions`); receive a per-candidate JSON envelope with corpus stats, pattern-skip counts, and per-candidate top-N exact and fuzzy matches |

Both take a `keep` parameter mirroring the CLI `--keep`. The envelope also reports
`candidate_functions_pre_filter`, distinguishing "no candidates compiled" from "all
candidates pattern-filtered out".


## Implementation

| File | Role |
|---|---|
| `canonical.das` | `CanonicalVisitor` (extends `daslib/ast` `AstVisitor`) and `tokenize_canonical` |
| `minhash.das` | 64-slot MinHash signatures over 5-grams, Jaccard estimate |
| `cluster.das` | Exact-bucket clustering + fuzzy all-pairs with length gate |
| `report.das` | JSON + stdout summary writer |
| `main.das` | CLI (`daslib/clargs`), file scan, compile-and-collect orchestration |
| `pipeline.das` | `compile_and_collect` / `collect_from_program`, shared by `main.das` and the tests; also `apply_pattern_filter` and the filesystem scan helpers |
| `patterns.das` | `classify(name, canonical) → PatternHit` |
| `exchange.das` | On-disk JSON schema + writer/reader for `--export-functions` / `--import-functions` |
| `fixture/synth.das`, `fixture/canonical_cases.das` | End-to-end visitor smoke fixture; one-function-per-concern `CanonicalVisitor` unit fixture |
| `test_detect_dupe.das` | dastest suite --- `bin/daslang dastest/dastest.das -- --test utils/detect-dupe/test_detect_dupe.das` |

## Notes

* Compile policy mirrors `utils/lint`: `ignore_shared_modules`, `export_all`.
  Optimisations and infer-time folding stay ON so dastest macros (e.g. `unroll`)
  compile.
* **Default mode** drops everything `generated`, lambdas included; the dispatcher
  still references each lambda via an `ADDR` token, so the fingerprint partially
  survives in the parent. `-L` clusters the lambda bodies themselves instead.
* **`-L`** is dominated at the top by linq `each` macro emissions (a 100+-token
  `GOTO/LABEL/_builtin_iterator_first/next/close` shell recurring hundreds of
  times); real test-body signal starts a few clusters down.
* Functions whose `at.fileInfo` points outside the compiled file are filtered out
  --- otherwise reified generics from required modules (e.g. `dastest/testing.das`)
  flood the report.
* Per-source-line dedup: a generic reified for N types becomes N `FunctionPtr`
  instances at the same `(file, line)`; only the first is kept.
