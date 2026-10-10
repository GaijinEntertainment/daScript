# lint

lint ships in the SDK as `bin/lint.exe`, a copy of daslang that runs this tool from source by its
name - the `DAS_UTILS_SHIPPED_EXES` entry in `utils/CMakeLists.txt` (repo root); `utils/REVIEW.das`
reads this line as the record of that decision.

The lint suite runner: compiles each target file and applies the rule modules
(`daslib/perf_lint.das`, `daslib/style_lint.das`, `daslib/lint.das`) over its AST.

Run:

    daslang -tool lint <files or dirs> --quiet

Exit 2 on any warning - CI's whole-tree sweep (`check_lint_tree`, in the extended checks) keys on it.
`lint <files>` is the same run through the SDK's `bin/lint.exe`. The directory it runs in is the
project root, so the modules a project installs resolve.

Clean results are cached in `.cache/daslang/lint_cache/` (`--cache-dir` moves it, `--no-cache` skips
it). A file has one entry per flag and rule-filter set, named by their hash and rewritten in place,
so the folder never outgrows the tree. The entry opens with the `DAS_LINT_CONFIG_PATH` in force,
then lists the content hash of every module source the file's compile read and of every
`.lint_config` that can steer it. A run that finds all of it unchanged prints `CACHED` for the file
and does not compile it. A file with findings, or one that fails to compile, is never recorded. The
folder is versioned by the semantic hash of the runner's `[lint_tool_entry]` `main`, every function
it reaches included: the `lint_tool_hash` simulate macro (`daslib/lint_config.das`) compares it with
`lint_hash.txt` while the runner compiles and empties the folder when it differs, so a changed rule
re-lints everything while a comment or an unreached helper re-lints nothing. A standalone `-exe`
build of the runner never compiles, cannot check that version, and runs without the cache. The cache does not track
`include`d files, macro-pinned environment and command-line inputs, or the C++ build: a change to
those alone re-lints nothing until `--no-cache`. CI keeps the folder between runs, one per platform
(`actions/cache` in `.github/workflows/extended_checks.yml`).

Five rules are the runner's own, because they are about folders rather than code. Each runs
once per invocation, over a walk of the directory roots the run was given. A directory whose
`.lint_config` carries `[docs] rule_docs_only = true` may hold only rule documents
(`REVIEW*.md`, `ARCHITECTURE*.md`); any other `.md` beside the sources is
**LINT025**. **LINT026**'s reverse direction needs no tag: every `{#anchor}` in any `.md` under
the run's roots must be cited - by an `[arch]` in a `.das` there, a `// <doc>.md#<anchor>`
pointer in a C++ source, or a `<doc>.md#<anchor>` anywhere in any markdown file under the roots
(a rule document, a ledger, an architecture document's routing alike) - and a markdown citation
that resolves to no section is a forward finding like a code one; a markdown citation's path is
joined to the citing file's folder (so `./` and `../` resolve against that folder), then tried at
each ancestor up to the root as a C++ pointer's is, and the folder-tree rule does not bind it. An
`arch(at="...")` written behind a `//` in a `.das` source is a finding when it resolves - a citation
pasted into a comment annotates nothing and is no citer - and silent when it does not, which is
prose about the annotation. **LINT027** caps each
`REVIEW*.md` / `ARCHITECTURE*.md` at 300 lines in every folder that holds one. Two more read
the checklists' text: **LINT032** reports a `REVIEW*.md` citing a rule by position ("the rule
above", "see below"), and **LINT033** a `REVIEW*.md` naming a path (a backticked token with a
source extension) or a test cell (a backticked `test_<subject>_<claim>`) the tree no longer has -
a path resolves against the checklist's folder, its ancestors and the working directory, a bare
basename anywhere in the repo, a cell as a `def test_*` under the checklist's folder. The
`rule_docs_only` key is a folder property - it never cascades, unlike `[format]`. Fixtures:
`tests/lint025_*`, `tests/lint026_*`, `tests/lint027_*` and `tests/lint033_*`, each driving the
CLI over a planted tree.

Design: the runner stays thin - rules live in the daslib modules (authoring rails:
`skills/internal/perf_lint_authoring.md`, `skills/internal/style_lint_authoring.md`);
suppression policy is `skills/perf_lint.md` / `skills/style_lint.md`. `tests/` here are the
runner's own fixtures, run by `run_utils_tests`.

Every rule id has a fixture whose file name carries the id - `tests/<id>_*.das` here, or a
`tests/lint/` (repo root) file for the rules whose test needs the dastest harness - and a
section in `doc/source/reference/language/lint.rst`; `REVIEW.das` beside this file checks the
triple. Fixtures whose names do not carry the id: LINT019 -> `tests/lint/test_stale_nolint.das`, PERF033 ->
`tests/lint/test_lint_fix.das` (its before/after fixture pair).
