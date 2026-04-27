# Filenames & filesystem in daslang

Read this skill before writing or editing `.das` code that builds, splits, or normalizes a file path, or that touches the filesystem (existence checks, listing directories, copy/rename/remove, temp files). Path & filename operations **MUST** use `fio` helpers — never hand-rolled `rfind`/`slice` on `/` and `\`. The C++ implementations are platform-aware; manual splitting silently breaks on Windows separators, trailing slashes, and the no-separator edge case.

## `require`

```das
require daslib/fio
```

`daslib/fio` does `require fio_core public` internally, so this single require pulls in both the C++ builtins and the daslang-side wrappers (RAII `fopen`, `_result` variants, `mkdir_rec`, `disk_space` struct return, whole-file `fread(path)`/`fwrite(path, text)`).

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
| Resolve `..`, `.`, fix separators | `normalize(p)` | hand-rolled normalizer |
| Is this path absolute? | `is_absolute(p)` | check leading `/` or drive letter |
| Compute a relative path | `relative(p, base, error)` or `relative_result(p, base)` | manual prefix strip |
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

`FStat` has one bool field (`is_valid`) plus six **property** accessors — call them WITHOUT parens: `st.is_reg`, `st.is_dir`, `st.size : uint64`, `st.atime : clock`, `st.ctime : clock`, `st.mtime : clock`.

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

POD struct / array I/O via the generic `fread(f, var buf)` / `fwrite(f, buf)` overloads in `daslib/fio` — they wrap the raw byte primitives with a `concept_assert` that the type is raw POD.

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

Full `_result` set in `daslib/fio.das`: `file_size_result`, `equivalent_result`, `is_symlink_result`, `copy_file_result`, `set_mtime_result`, `temp_directory_result`, `create_temp_file_result`, `create_temp_directory_result`, `relative_result`, `remove_result`, `rename_result`, `mkdir_result`, `rmdir_result`, `rmdir_rec_result`.

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

## Common gotchas

- **Never split paths with `rfind("/")` and `rfind("\\")` followed by `slice`.** Always `base_name` / `dir_name` / `parent`. The manual form misses Windows backslashes, mishandles trailing separators, and returns the wrong slice when no separator is present.
- **Never build paths with string interpolation.** `"{a}/{b}"` produces `"foo//bar"` if `a` has a trailing slash and breaks on Windows when one side uses backslashes. Use `path_join`.
- `fread(file)` requires **binary mode** (`"rb"`). Text mode causes a partial-read error.
- `fopen(path, mode, blk)` (3-arg block form) auto-closes on block exit; the 2-arg `FILE?` form needs explicit `fclose`. Prefer the 3-arg form unless you need the file handle to outlive the call.
- `dir(path)` callback yields `.` and `..` on POSIX — skip them.
- `dir_name`/`base_name` use platform-specific code (POSIX `dirname`/Windows `_splitpath`); `parent`/`stem`/`extension` use C++17 `std::filesystem` and are uniform across platforms. All are exposed; either set is correct.
- `getcwd()` and `chdir()` are no-ops on Emscripten — guard if your code runs there.
- **Out of scope for this skill** (same module, different concerns): `popen`, `popen_timeout`, `system`, `exit`, `get_env_variable`, `has_env_variable`, `sleep`, `get_clock`, `mktime`, `register_dynamic_module`. Use them directly from `fio`; they're not the focus here.

## Reference

- `daslib/fio.das` — daslang wrapper layer (RAII `fopen`, `_result` variants, `mkdir_rec`, `disk_space`, whole-file `fread`/`fwrite` by path)
- `examples/` and `tutorials/` — search for `fopen`, `dir_rec`, `path_join` for live usage
