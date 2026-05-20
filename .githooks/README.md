# Repo-managed git hooks

Tracked hooks that mirror CI checks. Cross-platform: Linux, macOS, Windows (Git Bash).

## Enable (once per clone)

```sh
git config core.hooksPath .githooks
```

Reverts via `git config --unset core.hooksPath`. Skip once with `git push --no-verify`.

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

- **pre-push** — formatter (`--verify` on the whole tree) + lint on pushed `.das` files. Mirrors `.github/workflows/extended_checks.yml`.

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
