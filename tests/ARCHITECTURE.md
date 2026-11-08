# tests/ - how the suite is laid out

The architecture doc for the daslang test tree; the common contract is `ARCHITECTURE_COMMON.md`
at the repo root.

## File charters

- `README.md` - the generated index: every run file, its tests and their descriptions. Written
  by `utils/internal/test-audit/main.das --readme`; the `check_tests_index` CI target runs
  `--verify` against it.
- `REVIEW.das` - the mechanical half of the checklist: the write-path scan and the file-shape
  rules, over the census in `dastest/file_census.das` (repo root).
- `.das_test` - per-folder gating dastest consults at the `--test` root: module availability and
  the per-mode skips (`--use-aot`, `-jit`, `--ser`/`--deser`).
- `aot/CMakeLists.txt` - what `test_aot` compiles, folder by folder.
- `<folder>/` - one topic area each; the folder is the first level of categorization, and
  `.das_test` gates on the folder name.

## The run file {#run-file}

dastest runs every `.das` under the `--test` root whose name, and the name of every directory
on its path, does not start with `_`. A `_` name is a data file or a helper module another test
reads or requires; it never runs on its own. The census - `dastest/file_census.das` (repo root),
the one scan that classifies each run file and reads its `[test]` annotations for both the gate
and the index - applies the same rule and also skips `REVIEW.das`: dastest runs it, but it is the
folder's gate, not a test.

## The three classes of run file {#file-classes}

A **suite** holds one or more `[test]` functions. A **compile-fail fixture** carries an `expect`
directive, or a name starting `failed_`, `cant_` or `invalid_`, and proves that a program does
not compile. Nothing in it runs, so any `[test]` it holds never runs. `REVIEW.das` requires the
name on every fixture, so tools that skip files with no compilable AST can tell it apart. One
fixture is one file, because a compile error cascades through the whole file. The census names
a third class, a
**script**: a run file with neither, which passes when it runs to the end. `REVIEW.das` rejects
a script, since its claim has no `[test]` to carry a description and no `run("...")` subtest name
to report what failed.

## Where a test lives {#file-shape}

A folder's tests sit in **category files**, each holding three or more `[test]` functions on
one topic, and in **uncategorized files** - the plain `uncategorized.das` and one
`uncategorized_<signature>.das` per fence signature (the fence signature section below) - which
hold every test the folder has no category for yet. A new test goes into an existing category
file when one fits, and into the uncategorized file of its signature otherwise. When three or
more tests in an uncategorized file, plain or fenced, belong together, they move into a category
file, new or existing, so `uncategorized.das` shrinks as topics get files.

A file holds at most 100 `[test]` functions. An uncategorized file past the cap continues in
`<base>_2.das`, then `_3`, with the same fence signature (`uncategorized_2.das`,
`uncategorized_no_aot_2.das`), and `REVIEW.das` rejects a continuation whose base is missing or
under the cap. A category file past the cap splits into `<base>.das`, `<base>_2.das`, ... of
three or more tests each (`test_linq_from_decs_2.das`).

This shape wins because a run pays for every file it opens, not for every test: compile,
module load, and in isolated mode (dastest `--isolated-mode`, one subprocess per file) a process.
A file per test multiplies the suite's fixed cost by its test count; a file per topic keeps each
file readable and each failure attributable.

## A test that cannot share a program {#standalone}

A few tests cannot share a program with any other, for a reason no `options` line expresses.
Two kinds of reason exist. A declaration of the test changes or breaks every other test
compiled into the same program: a program-wide registry the test fills (`[sql_migration]`), a
deliberate duplicate (a second `[sql_function]` of one name), a global conversion overload, a
check that another test's code in the same program would hide (a fresh `select` instantiation,
which any other select chain in the program compiles first). Or the file itself is the unit another
test or tool consumes: a corpus a fold test compiles by path, a conformance file a provider
copies per capability. Such a test declares `[test(standalone = "why")]`; its file may hold
fewer than three tests, and a reviewer judges the reason. A test that differs from its
neighbours only by its `options` is not standalone: it goes to the fenced file of its signature.

## The fence signature {#fence-signature}

Two files can share one program only when they compile under the same `options`. The **fence
signature** of a file is the sorted set of its `options` that change the program - everything
except the formatting and lint knobs `NON_FENCE_OPTIONS` in `dastest/file_census.das` (repo root)
lists. An option's value is part of the signature: `auto_inline_functions = false` and `= true`
are two programs, and `gen2 = false` (gen1 syntax) fences although `gen2` itself does not. A
module's own `_`-prefixed option (`_flatten_expand_lerp`) changes what the module compiles and
fences like any other.

A test whose signature differs from the folder's `uncategorized.das` lives in the existing file
with its signature, or starts the fenced uncategorized file of that signature. `REVIEW.das`
requires that file to be named by its signature: the options sorted and joined by `_`, every
value other than true joining its option's name, a `.` in a value spelled `_`:
`uncategorized_persistent_heap.das`, `uncategorized_no_aot_rtti_strict_smart_pointers_false.das`,
`uncategorized_stack_4096.das`, `uncategorized_unit_test_1_23.das`. The plain
`uncategorized.das` carries an empty signature, so any plain test can join it.

## Every test states its claim {#test-description}

Every `[test]` in a suite carries `description = "..."`: one sentence stating what the test proves, in
words that are not the function name again. dastest prints it under the test's `--- FAIL` line,
and the index generator reads it without compiling the file, so the whole annotation sits on
one line. A compile-fail fixture carries its claim as a `//!` line above its first declaration
instead.

## A test's own time budget {#test-timeout}

A test may carry `timeout = seconds`: a test that finishes past it is reported failed, naming
the budget, and the file's limits grow by the same amount - the `--max-file-time` cap the lanes
pass and, in isolated mode (dastest `--isolated-mode`, one subprocess per file), the
`--test-timeout` deadline. One slow test raises its own file's limits, not every file's, which is
what lets slow tests merge into a topic file at all. The value is the suite default, `1200`
(CI's dastest `--timeout`): CI has no per-test or per-file cap of its own and runs several times
slower than a workstation, so a budget measured locally fails there and a budget nobody measured
costs nothing - the annotation marks a slow test, it does not time it.

## Files a test writes {#test-writes}

CI's extended job runs the suite under `daslang` and under `daslang_static` at the same time, in
one checkout, so two runs of the same test file can overlap. A test writes only under
`test_temp_dir()` (`dastest/testing_boost`, repo root), a directory private to its run and
removed when the run ends, or under its own `create_temp_directory` (`daslib/fio`, repo root).

`REVIEW.das` finds a write call in a test file - fio's file and directory calls, a writing
`fopen`, `stbi_write_*`, an image `save` - whose path is built from `get_das_root()` directly,
through a helper that returns such a path, or through a variable or field bound to one, and a
writing `fopen` of a relative literal path. The gate treats the result of any call other than
`get_das_root`, `path_join`, `join_path`, `dir_name`, `string` and the file's own root-path
helpers as a path outside the tree, whatever its arguments name. `--root <dir>` points the gate
at another folder, which the gate's own test in `dastest/uncategorized.das` (this folder) uses
to run it over `dastest/_tests_gate_fixture/` and `dastest/_tests_shape_fixture/`.

Ledgered: the tests of `jit_tests/jit_artifacts.das` write under the tree on purpose, because
the JIT's own outputs are what they test. The DLL-cache test names `.cache/daslang/<namespace>/`:
the default JIT DLL cache names each DLL by a hash of its content and cleans its DLLs only at
that default home - an explicitly set `jit_output_path` turns both off - and its probe script
sits under `test_temp_dir()`, so the namespace (a hash of the script's path) is private to the
run and the test removes that directory when it ends. The exe, lib, wasm and cross-target tests
start `-jit` children without a module-cache flag for the same reason: the default caches they
fill are the subject.

## What `test_aot` compiles and runs {#test-aot}

`test_aot` compiles every file not marked `options no_aot` that an `AOT_*_FILES` variable of
`aot/CMakeLists.txt` holds after its filters; a folder `tests/<name>/` that `DAS_AOT_SUITES` lists
is globbed for its top-level `*.das` files, only where no variable already covers it, and a
subfolder's files are compiled only when a variable lists them. It runs every file it compiles unless
`.das_test` skips the file's folder under `--use-aot`. Per-PR CI builds only the subset binary
(`test_aot_subset`, `tests/language` alone); the full `test_aot` runs nightly and under
`preflight --full`.
