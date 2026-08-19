# lint

The lint suite runner: compiles each target file and applies the rule modules
(`daslib/perf_lint.das`, `daslib/style_lint.das`, `daslib/lint.das`) over its AST.

Run:

    daslang utils/lint/main.das -- <files or dirs> --quiet

Exit 2 on any warning — CI's changed-files gate and the nightly full sweep both key on it.
The SDK also ships a prebuilt `bin/lint.exe`.

Design: the runner stays thin — rules live in the daslib modules (authoring rails:
`skills/internal/perf_lint_authoring.md`, `skills/internal/style_lint_authoring.md`);
suppression policy is `skills/perf_lint.md` / `skills/style_lint.md`. `tests/` here are the
runner's own fixtures, run by `run_utils_tests`.

Every rule id has a fixture whose file name carries the id — `tests/<id>_*.das` here, or a
`tests/lint/` (repo root) file for the rules whose test needs the dastest harness — and a
section in `doc/source/reference/language/lint.rst`; `REVIEW.das` beside this file checks the
triple. Fixtures whose names do not carry the id: LINT019 → `tests/lint/test_stale_nolint.das`.
