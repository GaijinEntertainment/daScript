# Files, Paths, and Globs

```das
require daslib/fio
```

Path and filename operations **must** use `fio` helpers — never a hand-rolled `rfind` / `slice` on
`/` and `\`, which breaks on Windows separators, trailing separators, and the no-separator case.
Never require `fio_core` directly; `fio` requires it publicly.

## Pick the right tool

| To do this | Use |
|---|---|
| Filename out of a path | `base_name(p)` |
| Directory out of a path | `dir_name(p)` (alias `parent(p)`) |
| Extension (with dot) / without extension | `extension(p)` / `stem(p)` |
| Swap the extension | `replace_extension(p, ".new")` |
| Join components | `path_join(a, b)` — never `"{a}/{b}"` |
| Resolve `..` and `.`, fix separators | `normalize(p)` |
| Force forward slashes on every host | `to_generic_path(p)` |
| Absolute? | `is_absolute(p)` |
| Make a path relative to a base | `relative(p, base, error)` or `relative_result(p, base)` |
| Existence / size / mtime / kind | `fexist(p)` / `stat(p) : FStat` |
| Walk a directory / recursively | `dir(p) $(name) { … }` / `dir_rec(p) $(name, is_dir) { … }` |
| Open with auto-close | `fopen(p, mode) $(f) { … }` (the 3-arg block form) |
| Whole file in / out | `fread(path) : string` (`""` on failure) / `fwrite(path, text) : bool` |
| One glob → a sorted file list | `expand_glob(pattern, var result)` |
| A user `paths` argument — a comma/newline list of files, dirs, globs | `parse_file_list(arg, var result)` |

`normalize` emits the **platform-preferred** separator (`\` on Windows) — use it for shell and
system calls; `to_generic_path` always emits `/` — use it for matching, JSON output, and stable
test fixtures.

Never strip a prefix with `find(p, root) == 0` + `slice`: `relative` handles trailing separators,
refuses partial-component matches (`/foo/bar` vs `/foo/barx`), and yields `..`-relative paths for
`p` outside `root`. To find a named component, `to_generic_path` first, then search `/name/`.

## Walking directories

**`dir_rec` yields paths relative to the walked root**, never absolute: `path_join` with the root
before any `fread` / `stat`, or the call silently resolves against the wrong working directory. Its
separators are native, so `to_generic_path` before matching a `/`-pattern against its output. `dir`
yields `"."` and `".."` on POSIX — skip them.

## Stat, I/O, and processes

`FStat`: bool field `is_valid` — check it first — plus six **property** accessors called without
parens: `is_reg`, `is_dir`, `size` (`uint64`), `atime`, `ctime`, `mtime`.

```das
fopen(path, "rb") $(f) {
    return if (f == null)
    fmap(f) $(data) {               // data : array<uint8># — do not annotate it
        decode(data)
    }
}
```

- The 2-arg `fopen(path, mode) : FILE?` needs an explicit `fclose`. `fread(f)` requires **binary
  mode**; text mode causes a partial-read error.
- POD struct / array I/O goes through the generic `fread(f, var buf)` / `fwrite(f, buf)`, which
  concept-assert raw POD. The array forms **panic above 2 GiB**; `long_fread` / `long_fwrite`
  return `int64` and have no cap.
- `run_and_capture(args, var output, timeout_sec = 0.0) : int` runs a child with no shell, capturing
  merged stdout+stderr. **Windows trap: `args[0]` must use backslashes** — the OS will not resolve a
  forward-slash relative path — so `replace(exe, "/", "\\")` first.

## Mutating operations and their three error forms

| Operation | Plain | With `error` out-param | `_result` variant |
|---|---|---|---|
| Create dir / dir tree | `mkdir(p)` / `mkdir_rec(p)` | `mkdir(p, error)` | `mkdir_result(p)` |
| Remove dir / dir tree | `rmdir(p)` / `rmdir_rec(p)` | `rmdir(p, error)` | `rmdir_result(p)`, `rmdir_rec_result(p)` |
| Delete file / rename | `remove(p)` / `rename(a, b)` | `remove(p, error)` | `remove_result(p)`, `rename_result(a, b)` |
| Copy / set mtime | — | `copy_file(src, dst, overwrite, error)` | `copy_file_result(…)`, `set_mtime_result(p, t)` |

`_result` returns `variant fs_result_bool { value : bool; error : string }`, likewise `_int64` /
`_string`; test it with `r is error` and read the message as `r as error`.

Temp paths take the out-param shape — `create_temp_directory(prefix, err)`,
`create_temp_file(prefix, ext, err)`, `temp_directory(err)` — all absolute, `""` plus a set `err` on
failure, all with `_result` twins. No zero-argument forms: `temp_directory()` is a compile error.
`disk_space(path)` gives `capacity` / `free` / `available` as `uint64`.

## Globs — two flavors

| Function | Module | `*` crosses `/` | `**` | Char classes |
|---|---|---|---|---|
| `match_glob(pattern, path)` | `daslib/fio` | **no** — stops at `/` | **yes** | `[abc]`, `[a-z]`, `[!abc]`, `[^abc]` |
| `glob_match(pattern, text)` | `daslib/strings_boost` | yes | no | none |

**Default to `match_glob`**; `glob_match` only where slashes carry no structural meaning.

```das
def glob(root : string; pattern : string; blk : block<(filename : string; is_dir : bool) : void>)
def glob_filtered(root : string; includes, excludes : array<string>; blk : block<…>)
def is_glob_pattern(pattern : string) : bool
def expand_glob(pattern : string; var result : array<string>)
def parse_file_list(file : string; var result : array<string>)   // `file` is the argument TEXT
```

- `match_glob` is a pure string match — no filesystem access; both sides use `/`. **Whole** path
  against **whole** pattern: an absolute path against a relative pattern silently fails (scope it
  first with `relative_result`).
- `glob` walks recursively; its callback receives `/`-normalized paths on every host.
  `glob_filtered` yields entries matching ≥1 include and 0 excludes — **excludes win** — but
  excluded subtrees are **not pruned**, so a huge excluded tree still costs the I/O; walk with
  `dir_rec` and short-circuit when that matters.
- `is_glob_pattern` is true for `*`, `?`, or `[` — the inline
  `find(s, "*") >= 0 || find(s, "?") >= 0` misses character classes. Tool convention for
  user-supplied patterns: a literal that does not exist is an **error**, a glob matching nothing a
  **warning** — dispatch on `is_glob_pattern`.
- `expand_glob` and `parse_file_list` **append** to `result` and sort each expansion **locally**,
  preserving the order of plain entries around it; a global `sort` over a mixed plain+glob parse
  scrambles the user's order.
- `**` is greedy across `/` wherever it appears, so `a**b` matches `a/x/b`; bash treats `**` as
  special only as a whole component — write `a*b` for the bash meaning.
- **No brace alternation** (`{a,b}`) in either flavor — expand into multiple includes. **No
  backslash escape**: match a literal wildcard with a character class — `[*]`, `[?]`, `[[]`.
- **Don't substring-filter paths** (`find(p, "tests") >= 0`) — `tests` matches `unit_tests/`. Use a
  pattern.
