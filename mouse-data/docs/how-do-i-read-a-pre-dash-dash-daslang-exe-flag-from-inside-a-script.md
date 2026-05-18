---
slug: how-do-i-read-a-pre-dash-dash-daslang-exe-flag-from-inside-a-script
title: How do I read a pre-`--` daslang.exe flag (like `-project_root`) from inside a running script?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

Verified 2026-05-17 (dasImgui PR #44).

`get_command_line_arguments()` (daslang builtin) returns the **full
original argv**. `get_user_args()` (in `daslib/clargs`) is mode-aware
and in interpreted mode returns ONLY the slice after `--`. So if you
want to read a `daslang.exe` flag like `-project_root` from inside a
script, **use `get_command_line_arguments()`, not `get_user_args()`**.

**Why:** `setCommandLineArguments(argc, argv)` is called at
`utils/daScript/main.cpp:140` (and `:509` for the alternate code path)
**before** any flag parsing. It freezes the full argv into a daslang-
visible array. So `daslang.exe -project_root D:/foo myscript.das`
produces a `get_command_line_arguments()` of
`["daslang.exe", "-project_root", "D:/foo", "myscript.das"]` —
`-project_root` is visible.

`get_user_args()` (`daslib/clargs.das:74`) wraps this with mode logic:
- Standalone exe: `argv[1..]` (skips program name)
- **Interpreter mode: post-`--` slice only** — `-project_root` is invisible

**Scan with `find_flag_raw_value`:**

```daslang
require daslib/clargs

let argv_all <- get_command_line_arguments()
let pr = find_flag_raw_value(argv_all, "-project_root") |> unwrap_or("")
if (!empty(pr)) {
    // … use the project root, e.g. forward to a spawned daslang-live
}
```

`find_flag_raw_value` handles both `-flag value` and `-flag=value`
forms. Same import (`daslib/clargs`) as `get_user_args()`.

**Bug pattern this fixes:** dasImgui's `with_recording_app` initially
scanned `get_user_args()` for `-project_root`. The documented
single-flag invocation

```
daslang.exe -project_root <dasimgui> tests/integration/record_X.das
```

silently dropped the flag at forward time, and the spawned
daslang-live died on `require imgui` with
`error[20605]: missing prerequisite 'imgui'`. Workaround was passing
`-project_root` **twice** (pre- AND post-`--`). The fix swaps the
scanner to `get_command_line_arguments()`; the flag is visible
regardless of position.

**Companion card:** `daslang-script-flags-need-dash-dash-separator`
covers the inverse — when YOU want to pass a flag to the script and
it doesn't show in `get_user_args()`, the fix is to use `--` before
your flag. The two together: pre-`--` flags belong to daslang.exe and
need `get_command_line_arguments()`; post-`--` flags belong to your
script and show up in `get_user_args()`.

## Questions
- How do I read a pre-`--` daslang.exe flag (like `-project_root`) from inside a running script?
