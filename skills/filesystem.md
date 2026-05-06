# Filenames & filesystem in daslang

Read this skill before writing or editing `.das` code that builds, splits, or normalizes a file path, or that touches the filesystem (existence checks, listing directories, copy/rename/remove, temp files). Path & filename operations **MUST** use `fio` helpers — never hand-rolled `rfind`/`slice` on `/` and `\`. The C++ implementations are platform-aware; manual splitting silently breaks on Windows separators, trailing slashes, and the no-separator edge case.

Runnable examples: [tests/fio/fio_utils.das](tests/fio/fio_utils.das), [tutorials/dasAudio/02_playing_files.das](tutorials/dasAudio/02_playing_files.das).

## `require`

```das
require daslib/fio
```

Always require `daslib/fio` — never `fio_core` directly. `daslib/fio` does `require fio_core public` internally, so this single require pulls in both layers transitively.

### `fio_core` vs `daslib/fio`

Two layers, one require:

- **`fio_core`** (C++ builtins) — the path primitives that are basically free function calls into `std::filesystem`: `path_join`, `is_absolute`, `normalize`, `relative` (3-arg with error out-param), `dir_name`, `base_name`, `parent`, `extension`, `stem`, `replace_extension`, `fexist`, `stat`, `dir`, `dir_rec` (builtin), `fopen` (2-arg returning `FILE?`), `mkdir`/`rmdir`, `remove`/`rename`, `copy_file`, temp files/dirs, env vars, `popen`/`system`. Listed via `mcp__daslang__list_module_api fio_core`.
- **`daslib/fio`** (the daslang wrapper) — adds RAII `fopen` with block, `dir_rec` generic, `FStat` daslib overload (sets `is_valid`), `disk_space` struct return, whole-file `fread(path)`/`fwrite(path, text)`, `mkdir_rec`, the `_result` variants that wrap error out-params into a `fs_result_string`/`fs_result_bool` variant, and the **glob bundle**: `match_glob`, `glob`, `glob_filtered`, `is_glob_pattern`, `to_generic_path`, `expand_glob`, `parse_file_list`. Source: [daslib/fio.das](daslib/fio.das).

The "Pick the right tool first" table below mixes both — it doesn't matter which layer a function lives in, only that you use the right one. Hand-rolling any of these (typically with `find`/`rfind`/`slice` on `/` or `\\`) is wrong on Windows or in edge cases.

## Pick the right tool first

Fixed order of preference. If the row on the left fits, the row on the right is wrong.

| If you want to | Use | Don't |
|---|---|---|
| Get the filename out of a path | `base_name(p)` | `let i = max(rfind(p, "/"), rfind(p, "\\")); slice(p, i+1)` |
| Get the directory out of a path | `dir_name(p)` (alias `parent(p)`) | manual `rfind` / `slice` on separators |
| Get the extension (with dot) | `extension(p)` | slicing on the last `.` |
| Get the filename without extension | `stem(p)` | basename + slice-off-extension |
| Replace the extension | `replace_extension(p, ".new")` | string concat with stem |
| Join two path components | `path_join(a, b)` | `"{a}/{b}"` string interp |
| Resolve `..`, `.`, fix separators | `normalize(p)` (uses platform-preferred separator) | hand-rolled normalizer |
| Force forward slashes regardless of host | `to_generic_path(p)` | `replace(p, "\\", "/")` |
| Is this path absolute? | `is_absolute(p)` | check leading `/` or drive letter |
| Compute a relative path | `relative(p, base, error)` or `relative_result(p, base)` | manual prefix strip with `find`/`slice` |
| Find a known component anywhere in a path (e.g. `/modules/...`) | `to_generic_path(p)` then `find`/`rfind("/<name>/")` on the result | scanning for both `/<name>/` AND `\<name>\` |
| Expand a single glob into a sorted file list | `expand_glob(pattern, var result)` | manual `dir_rec` + `match_glob` walk |
| Parse user `paths` arg (comma/newline list of files/dirs/globs) | `parse_file_list(file, var result)` | manual split + per-entry dispatch |
| Check existence | `fexist(p)` | open-and-check |
| Get file size / mtime / type | `stat(p) : FStat` (daslib overload) | platform `stat` syscall |
| Walk a directory | `dir(p) $(name) { ... }` | platform `opendir` |
| Walk recursively | `dir_rec(p) $(name, is_dir) { ... }` | manual recursion |
| Open a file with auto-close | `fopen(p, mode) $(f) { ... }` (3-arg block form) | 2-arg `fopen` + manual `fclose` |
| Read whole file as string | `fread(path) : string` (daslib overload) | open + read + close by hand |
| Write whole string to file | `fwrite(path, text) : bool` (daslib overload) | open + write + close by hand |

## Path manipulation cookbook

```das
let p = "/usr/local/share/daslang/main.das"

let name = base_name(p)            // "main.das"
let dir  = dir_name(p)             // "/usr/local/share/daslang"
let ext  = extension(p)            // ".das"
let base = stem(p)                 // "main"
let cpp  = replace_extension(p, ".cpp")  // "/usr/local/share/daslang/main.cpp"

let full = path_join("assets", "sprites/hero.png")  // platform-correct
let norm = normalize("a/b/../c/./d")                // "a/c/d"
let abs  = is_absolute(p)                           // true
```

Walk a directory and pick `.das` files using `extension`, not string suffix matching:

```das
dir_rec("daslib") $(name, isdir) {
    return if (isdir)
    return if (extension(name) != ".das")
    process(name)
}
```

`dir(path) $(name) { ... }` yields raw entries — on POSIX you also get `.` and `..`, so guard:

```das
dir(some_dir) $(name) {
    return if (name == "." || name == "..")
    let full = path_join(some_dir, name)   // not "{some_dir}/{name}"
    handle(full)
}
```

## Filesystem queries — `stat` and `FStat`

Prefer the daslib `stat(path) : FStat` overload — it sets `is_valid` for you:

```das
let st = stat(path)
return if (!st.is_valid)
if (st.is_reg) {
    let sz = st.size
}
if (st.is_dir) {
    recurse(path)
}
let when = st.mtime
```

`FStat` has one bool field (`is_valid`) plus six **property** accessors — call them WITHOUT parens: `st.is_reg`, `st.is_dir`, `st.size : uint64`, `st.atime : clock`, `st.ctime : clock`, `st.mtime : clock`. The C++ definition lives in [include/daScript/simulate/aot_builtin_fio.h](include/daScript/simulate/aot_builtin_fio.h).

`fexist(p)` is the cheap "does this exist at all" probe — it doesn't tell you reg vs dir, just whether `stat` would succeed. If you'll need fields later, jump straight to `stat`.

## File I/O

Always use the **3-arg block form** of `fopen` for auto-close. The 2-arg form (`fopen(path, mode) : FILE?`) requires explicit `fclose` and is leak-prone:

```das
fopen(path, "rb") $(f) {
    return if (f == null)
    let contents = fread(f)         // requires binary mode
    process(contents)
}                                   // file closed here
```

Whole-file convenience overloads from `daslib/fio` skip the boilerplate:

```das
let text = fread(path)              // returns "" on failure
let ok   = fwrite(path, text)       // bool
```

Binary memory-mapped read:

```das
fopen(path, "rb") $(f) {
    fmap(f) $(data : array<uint8>) {
        decode(data)
    }
}
```

POD struct / array I/O via the generic `fread(f, var buf)` / `fwrite(f, buf)` overloads — see [daslib/fio.das:109-139](daslib/fio.das#L109-L139). They wrap `_builtin_read`/`_builtin_write` with a `concept_assert` that the type is raw POD.

## Mutating filesystem operations

| Op | Plain | With `error` out-param | `_result` variant |
|---|---|---|---|
| Create dir | `mkdir(p)` | `mkdir(p, error)` | `mkdir_result(p)` |
| Create dir tree | `mkdir_rec(p)` | `mkdir_rec(p, error)` | — |
| Remove empty dir | `rmdir(p)` | `rmdir(p, error)` | `rmdir_result(p)` |
| Remove dir tree | `rmdir_rec(p)` | `rmdir_rec(p, error)` | `rmdir_rec_result(p)` |
| Delete file | `remove(p)` | `remove(p, error)` | `remove_result(p)` |
| Move/rename | `rename(a, b)` | `rename(a, b, error)` | `rename_result(a, b)` |
| Copy | — | `copy_file(src, dst, overwrite, error)` | `copy_file_result(src, dst, overwrite)` |
| Set mtime | — | `set_mtime(p, t, error)` | `set_mtime_result(p, t)` |

## Error handling — three forms

Pick the form by what the caller does with failures:

- **Plain bool / no error** — `mkdir(p)`, `remove(p)`. Fire-and-forget; caller only cares about success.
- **`error` out-param** — `mkdir(p, error)`. C++-`error_code` style. Use when you want to log or surface the message but stay imperative.
- **`_result` variant** — returns `variant fs_result_bool { value : bool; error : string }` (or `_int64`/`_string`). Functional style; pairs with `is _value` / `as _value` checks. Use when chaining or returning to a caller that wants exhaustive matching.

Full `_result` set in [daslib/fio.das:234-371](daslib/fio.das#L234-L371): `file_size_result`, `equivalent_result`, `is_symlink_result`, `copy_file_result`, `set_mtime_result`, `temp_directory_result`, `create_temp_file_result`, `create_temp_directory_result`, `relative_result`, `remove_result`, `rename_result`, `mkdir_result`, `rmdir_result`, `rmdir_rec_result`.

```das
let r = mkdir_result("output/cache")
if (r is error) {
    to_log(LOG_ERROR, "mkdir failed: {r as error}")
    return
}
```

## Temp files & dirs

```das
let dir = create_temp_directory("build_") ?? ""    // null/empty on failure
let f   = create_temp_file("dump_", ".log") ?? ""
let td  = temp_directory() ?? ""
```

All three return absolute paths; the `_result` variants surface a real error string if creation failed (disk full, permission denied, etc.).

## Disk space

```das
let info = disk_space(path)        // daslib overload returns DiskSpaceInfo
let free_gb = info.free / (1ul << 30ul)
```

Fields: `capacity`, `free`, `available` (all `uint64`).

## Finding bundled asset files at runtime

For loading data files (fonts, SVGs, configs) that ship next to your `.das` source, use `get_this_module_dir()` from `daslib/module_path` — **never** `dir_name(get_module_file_name(name))`. The latter returns the path baked at compile time, which is wrong in a relocated daspkg-release standalone bundle (`get_module_file_name` returns empty for some standalone exe lookups).

```das
require daslib/module_path

def load_assets() {
    let dir = get_this_module_dir()
    let svg = path_join(dir, "cards.svg")
    // ...
}
```

The macro captures the call-site source-file path at expansion, then a 3-tier resolver walks `<exe_dir>/<rel>` → `<das_root>/<rel>` → `dir_name(baked)`. `<rel>` is the suffix starting at the last `/modules/` segment.

**Project-local code (no `/modules/` in source path)** — e.g. `get_this_module_dir()` called from a user's `main.das` — skips tiers 1+2 (no `<rel>`) and goes to tier 3, which returns `dir_name(baked)` if it still exists on disk (dev) or `<exe_dir>` if it doesn't (relocated bundle). Either way you get an existing directory. Ship project-local assets next to the exe and `path_join(get_this_module_dir(), "asset.png")` works in both dev and shipped bundles.

See [skills/daspkg.md](skills/daspkg.md#L224) for the bundle-shipping side of the same topic.

**Don't call from a top-level `let` initializer** — `-exe` JIT-bakes function pointers in module-init code and ASLR breaks them in standalone exes (issue #2582). Inside any `def` body it's fine.

## Common gotchas

- **Never split paths with `rfind("/")` and `rfind("\\")` followed by `slice`.** Always `base_name` / `dir_name` / `parent`. The manual form misses Windows backslashes, mishandles trailing separators, and returns the wrong slice when no separator is present. Exception: when the substring you're searching for is a **named component** (e.g. `/modules/`, `/.git/`), normalize once via `to_generic_path(p)` and search for forward slashes only: `rfind(to_generic_path(p), "/modules/")`. The normalize step is what makes the rule "search for `/` only" safe; never search for both `/X/` and `\X\` separately.
- **Never build paths with string interpolation.** `"{a}/{b}"` produces `"foo//bar"` if `a` has a trailing slash and breaks on Windows when one side uses backslashes. Use `path_join`.
- **Never strip prefixes with `find(p, root) == 0` + `slice`.** Use `relative(p, root, err)` (or `relative_result`) — handles the trailing-separator case, refuses partial-component matches (`/foo/bar` vs `/foo/barx`), and produces correct `..`-relative paths when `p` is outside `root`. Pair with `to_generic_path` if the consumer needs forward slashes.
- **`normalize` and `to_generic_path` are NOT the same.** `normalize` uses the platform-preferred separator (`\` on Windows). `to_generic_path` always emits `/`. Pick by what the consumer needs: shells/system calls want `normalize`, JSON/log output and stable test fixtures want `to_generic_path`.
- `fread(file)` requires **binary mode** (`"rb"`). Text mode causes a partial-read error.
- `fopen(path, mode, blk)` (3-arg block form) auto-closes on block exit; the 2-arg `FILE?` form needs explicit `fclose`. Prefer the 3-arg form unless you need the file handle to outlive the call.
- `dir(path)` callback yields `.` and `..` on POSIX — skip them.
- `dir_name`/`base_name` use platform-specific code (POSIX `dirname`/Windows `_splitpath`); `parent`/`stem`/`extension` use C++17 `std::filesystem` and are uniform across platforms. All are exposed; either set is correct.
- `getcwd()` and `chdir()` are no-ops on Emscripten — guard if your code runs there.
- **Out of scope for this skill** (same C++ module, but different concerns): `popen`, `popen_timeout`, `system`, `exit`, `get_env_variable`, `has_env_variable`, `sleep`, `get_clock`, `mktime`, `register_dynamic_module`. Use them directly from `fio`; they're not the focus here.

## Reference

- [daslib/fio.das](daslib/fio.das) — daslang wrapper layer (RAII `fopen`, `_result` variants, `mkdir_rec`, `disk_space`, whole-file `fread`/`fwrite` by path, glob bundle)
- [src/builtin/module_builtin_fio.cpp:1218-1457](src/builtin/module_builtin_fio.cpp#L1218-L1457) — C++ registrations (the `fio_core` layer)
- [include/daScript/simulate/aot_builtin_fio.h](include/daScript/simulate/aot_builtin_fio.h) — `FStat`, `DiskSpaceInfo` structs
- [doc/source/stdlib/handmade/module-fio.rst](doc/source/stdlib/handmade/module-fio.rst) — auto-generated module reference
- [tests/fio/fio_utils.das](tests/fio/fio_utils.das) — read/write by path, recursive `rmdir`, `_result` variants
- [tests/fio/glob_test.das](tests/fio/glob_test.das) and [tests/fio/expand_glob_test.das](tests/fio/expand_glob_test.das) — glob primitives + expander/parser tests
- [tutorials/dasAudio/02_playing_files.das](tutorials/dasAudio/02_playing_files.das) — `fopen` + `fmap` for binary data
- [skills/glob.md](skills/glob.md) — pathname glob matching, when to use `match_glob` vs `glob_match`, full `expand_glob` / `parse_file_list` cookbook
