# lint

The lint suite runner: compiles each target file and applies the rule modules
(`daslib/perf_lint.das`, `daslib/style_lint.das`, `daslib/lint.das`) over its AST.

Run:

    daslang utils/lint/main.das -- <files or dirs> --quiet

Exit 2 on any warning - CI's changed-files gate and the nightly full sweep both key on it.
The SDK also ships a prebuilt `bin/lint.exe`.

One rule is the runner's own, because it is about folders rather than code: **LINT025**. A
directory whose `.lint_config` carries `[docs] rule_docs_only = true` may hold only rule
documents (`REVIEW*.md`, `ARCHITECTURE*.md`, `LAWS.md`); any other `.md` beside the sources
is reported once per invocation, from a walk of the directory roots the run was given. The
key is a folder property - it never cascades, unlike `[format]`. Fixture: `tests/lint025_*`,
which drives the CLI over a planted tree.

Design: the runner stays thin - rules live in the daslib modules (authoring rails:
`skills/internal/perf_lint_authoring.md`, `skills/internal/style_lint_authoring.md`);
suppression policy is `skills/perf_lint.md` / `skills/style_lint.md`. `tests/` here are the
runner's own fixtures, run by `run_utils_tests`.

Every rule id has a fixture whose file name carries the id - `tests/<id>_*.das` here, or a
`tests/lint/` (repo root) file for the rules whose test needs the dastest harness - and a
section in `doc/source/reference/language/lint.rst`; `REVIEW.das` beside this file checks the
triple. Fixtures whose names do not carry the id: LINT019 -> `tests/lint/test_stale_nolint.das`.
