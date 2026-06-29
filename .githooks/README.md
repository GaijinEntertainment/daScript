# Repo-managed git hooks

Tracked hooks that mirror CI checks. Cross-platform: Linux, macOS, Windows (Git Bash).

## Enable (once per clone)

```sh
git config core.hooksPath .githooks
```

Reverts via `git config --unset core.hooksPath`. Skip once with `git push --no-verify`.

## The full-preflight gate

`pre-push` blocks a push unless **both** hold for the pushed commit: its base is
**within `MAX_BEHIND` (3) merge commits of `origin/master`** (the hook fetches to
check; a base further behind than that prints a rebase remedy — the gate catches
a branch tested against a long-stale master, not the normal churn of unrelated
PRs landing mid-run), **and** it has a fresh **full-preflight token**. Tag pushes
(`refs/tags/*`) are not gated. The token is minted only by a clean, complete
full run of preflight:

```sh
daslang utils/preflight/main.das -- --full
```

It is written to `$(git rev-parse --git-path preflight-token)` (per-worktree)
and bound to the HEAD sha. Any later commit, amend, or rebase changes the sha,
the token goes stale, and the hook blocks until you re-run full preflight. A
failed or partial (`--only`/`--skip`) run does not mint a token; a dirty working
tree at the end of a full run does not either (you pushed the commit, not the
tree). This is a forcing function for "forgot to preflight" — not a security
gate; `git push --no-verify` skips it for deliberate WIP pushes.

### Windows notes

- Git for Windows ships Git Bash — the `#!/usr/bin/env bash` shebang works out of the box. No extra install needed.
- If you use PowerShell or `cmd` for `git push`, that's fine — git invokes the hook through its own shell, not yours.
- File-mode permissions are ignored on Windows; just having the file in `.githooks/` is enough.

### Linux / macOS

Hooks must be executable. Cloning preserves the executable bit (it's set in the repo via `git update-index --chmod=+x`). If you ever lose it locally:

```sh
chmod +x .githooks/*
```

## Hooks

- **pre-push** — requires a fresh full-preflight token for the pushed commit (see "The full-preflight gate" above). Full preflight itself runs the format + lint gates that mirror `.github/workflows/extended_checks.yml`, plus the full-tier suites.

## Requirements

A built `daslang` binary. The script auto-detects:

- `bin/daslang` (Linux / macOS, single-config Make/Ninja)
- `bin/daslang.exe` (MSYS / cygwin)
- `bin/Release/daslang.exe` / `bin/Debug/daslang.exe` (Windows MSVC)
- `build/daslang`, `build/bin/daslang`

Build before pushing:

```sh
cmake --build build --target daslang
```

Override the resolved path:

```sh
DASLANG=/custom/path/daslang git push
```
