---
slug: daslang-script-flags-need-dash-dash-separator
title: When daslang.exe runs a script with `daslang FILE.das --flag`, why doesn't the script's `get_user_args()` see `--flag`?
created: 2026-05-15
last_verified: 2026-05-15
links: []
---

**Cause:** `get_user_args()` in `daslib/clargs` is **mode-aware**:
- Standalone exe (`daslang -exe`-built): returns `argv[1..]`
- Interpreter mode (regular `daslang.exe FILE.das`): returns the slice **after** the `--` separator. No `--` = empty args.

So `daslang.exe FILE.das --headless` → `get_user_args()` returns `[]` because there's no `--` separator. The `--headless` token is consumed by daslang.exe itself (or treated as positional and ignored).

**Fix — invoke with `--` separator:**
```
daslang.exe FILE.das -- --headless
```

`get_user_args()` now returns `["--headless"]` and clargs `find_bool_flag_raw_value(args, "--headless")` resolves correctly.

**For dastest:** `dastest --test FILE.das -- --headless` works the same way; dastest passes everything after its own `--` through to the script.

**Source:** `daslib/clargs.das:74` `get_user_args()` / `:36` `get_cli_arguments()` — `find_index(all_args, "--")` is the gate.

**Related memory:** `feedback_clargs_underscore_hyphen` (clargs field underscores → hyphens). Together they characterize the clargs invocation contract for daslang-script CLI flags.

## Questions
- When daslang.exe runs a script with `daslang FILE.das --flag`, why doesn't the script's `get_user_args()` see `--flag`?
