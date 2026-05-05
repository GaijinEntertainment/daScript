options gen2
options indenting = 4
options remove_unused_symbols = false

// Make it shared, so other shared modules can include it.
module fio shared

require fio_core public
require strings

typedef file = FILE const?


[generic]
def fopen(name : string; mode : string; blk : block<(f : file) : void>) {
    let f = fopen(name, mode)
    invoke(blk, f)
    if (f != null) {
        fclose(f)
    }
}

[generic]
def dir(path : string; blk : block<(filename : string) : void>) {
    builtin_dir(path, blk)
}

[generic]
def stat(path : string) : FStat {
    var fs : FStat
    if (stat(path, fs)) {
        fs.is_valid = true
    }
    return fs
}

[generic]
def fstat(f : file) : FStat {
    var fs : FStat
    if (fstat(f, fs)) {
        fs.is_valid = true
    }
    return fs
}

[generic]
def fread(f : file; blk : block<(data : string const#)>) {
    var stmp = fread(f)
    unsafe {
        invoke(blk, reinterpret<string#>(stmp))
        delete_string(stmp)
    }
}

let {
    df_magic = 0x12345678
}

struct df_header {
    magic : uint
    size : int
}

[generic]
def fload(file : file; size : int; blk : block<(data : array<uint8>) : void>) : void {
    _builtin_load(file, size, blk)
}

[generic]
def fload(f : file; buf : auto(BufType) -const) {
    var dfh : df_header
    let r1 = _builtin_read(f, dfh, typeinfo sizeof(type<df_header>))
    if (r1 < 0) {
        return false
    }
    if (dfh.magic != df_magic) {
        return false
    }
    var loaded = false
    _builtin_load(f, dfh.size) $(data : array<uint8>) {
        if (length(data) != 0) {
            binary_load(buf, data)
            loaded = true
        }
    }
    return loaded
}

[generic]
def fsave(f : file; buf : auto(BufType) const) {
    var r1, r2 : int
    binary_save(buf) $(data) {
        let len = length(data)
        let dfh = df_header(magic = df_magic, size = len)
        r1 = _builtin_write(f, dfh, typeinfo sizeof(type<df_header>))
        unsafe {
            r2 = _builtin_write(f, addr(data[0]), len)
        }
    }
    if (r1 < 0) {
        return r1
    }
    if (r2 < 0) {
        return r2
    }
    return (r1 + r2)
}

[generic]
def fread(f : file; buf : auto(BufType) const implicit) {
    concept_assert(typeinfo is_raw(buf), "'fread' accepts only raw POD types")
    unsafe {
        return _builtin_read(f, addr(buf), typeinfo sizeof(buf))
    }
}

[generic]
def fread(f : file; buf : array<auto(BufType)> const implicit) {
    concept_assert(typeinfo is_raw(type<BufType>), "'fread' accepts only raw POD type arrays")
    unsafe {
        return _builtin_read(f, addr(buf[0]), length(buf) * (typeinfo sizeof(type<BufType>)))
    }
}

[generic]
def fwrite(f : file; buf : auto(BufType) const implicit) {
    concept_assert(typeinfo is_raw(buf), "'fwrite' accepts only raw POD types")
    unsafe {
        return _builtin_write(f, addr(buf), typeinfo sizeof(buf))
    }
}

[generic]
def fwrite(f : file; buf : array<auto(BufType)> const implicit) {
    concept_assert(typeinfo is_raw(type<BufType>), "'fwrite' accepts only raw POD type arrays")
    unsafe {
        return _builtin_write(f, addr(buf[0]), length(buf) * (typeinfo sizeof(type<BufType>)))
    }
}

[generic]
def mkdir_rec(path : string) : bool {
    return true if (path == "")
    var st : FStat
    return true if (stat(path, st) && st.is_dir)
    mkdir_rec(dir_name(path))
    return mkdir(path)
}

[generic]
def mkdir_rec(path : string; var error : string&) : bool {
    return true if (path == "")
    var st : FStat
    return true if (stat(path, st) && st.is_dir)
    if (!mkdir_rec(dir_name(path), error)) {
        return false
    }
    return mkdir(path, error)
}

// convenience: read entire file by path, returns empty string on failure
[generic]
def fread(path : string) : string {
    var result : string
    fopen(path, "rb") $(f) {
        if (f != null) {
            result = fread(f)
        }
    }
    return result
}

// convenience: write string to file by path, returns true on success
[generic]
def fwrite(path : string; text : string) : bool {
    var ok = false
    fopen(path, "wb") $(f) {
        if (f != null) {
            fwrite(f, text)
            ok = true
        }
    }
    return ok
}

// recursive directory iteration
[generic]
def dir_rec(path : string; blk : block<(filename : string; is_dir : bool) : void>) {
    var error : string
    builtin_dir_rec(path, blk, error)
}

[generic]
def dir_rec(path : string; blk : block<(filename : string; is_dir : bool) : void>; var error : string&) {
    builtin_dir_rec(path, blk, error)
}

// ===== glob =====

// Byte-level pathname-glob matcher. `/` is a hard segment boundary that single
// `*` and `?` may not cross; `**` matches across `/`.
def private _match_glob_bytes(p : array<uint8> const#; var pi : int; s : array<uint8> const#; var si : int) : bool {
    let plen = length(p)
    let slen = length(s)
    while (pi < plen) {
        let pc = p[pi]
        if (pc == uint8('*')) {
            let is_double = (pi + 1 < plen && p[pi + 1] == uint8('*'))
            if (is_double) {
                // `**/` semantics: the `/` is optional only when `**` matches
                // empty. Otherwise `**` must consume one or more segments
                // ending in `/`, so the source must have a `/` boundary.
                if (pi + 2 < plen && p[pi + 2] == uint8('/')) {
                    if (_match_glob_bytes(p, pi + 3, s, si)) {
                        return true
                    }
                    var k = si
                    while (k < slen) {
                        if (s[k] == uint8('/') && _match_glob_bytes(p, pi + 3, s, k + 1)) {
                            return true
                        }
                        k += 1
                    }
                    return false
                }
                pi += 2
                var k = si
                while (k <= slen) {
                    if (_match_glob_bytes(p, pi, s, k)) {
                        return true
                    }
                    k += 1
                }
                return false
            } else {
                pi += 1
                var k = si
                while (k <= slen) {
                    if (_match_glob_bytes(p, pi, s, k)) {
                        return true
                    }
                    return false if (k < slen && s[k] == uint8('/'))
                    k += 1
                }
                return false
            }
        } elif (pc == uint8('?')) {
            return false if (si >= slen)
            return false if (s[si] == uint8('/'))
            pi += 1
            si += 1
        } elif (pc == uint8('[')) {
            // POSIX fnmatch: `[` without a closing `]` matches the literal `[`.
            // Probe ahead before committing to class processing.
            var scan = pi + 1
            if (scan < plen && (p[scan] == uint8('!') || p[scan] == uint8('^'))) {
                scan += 1
            }
            // A `]` immediately after `[` or `[!`/`[^` is a literal class member,
            // not the terminator (POSIX rule for closing brackets in classes).
            if (scan < plen && p[scan] == uint8(']')) {
                scan += 1
            }
            while (scan < plen && p[scan] != uint8(']')) {
                scan += 1
            }
            if (scan >= plen) {
                // No closing `]` — treat the `[` as a literal character.
                return false if (si >= slen)
                return false if (s[si] != pc)
                pi += 1
                si += 1
            } else {
                return false if (si >= slen)
                let sc = s[si]
                return false if (sc == uint8('/'))
                pi += 1
                var negate = false
                if (pi < plen && (p[pi] == uint8('!') || p[pi] == uint8('^'))) {
                    negate = true
                    pi += 1
                }
                var matched = false
                // POSIX: `]` right after `[` (or `[!`/`[^`) is a literal class
                // member, not the terminator.
                if (pi < plen && p[pi] == uint8(']')) {
                    if (sc == uint8(']')) {
                        matched = true
                    }
                    pi += 1
                }
                while (pi < plen && p[pi] != uint8(']')) {
                    if (pi + 2 < plen && p[pi + 1] == uint8('-') && p[pi + 2] != uint8(']')) {
                        let lo = uint(p[pi])
                        let hi = uint(p[pi + 2])
                        let scv = uint(sc)
                        if (scv >= lo && scv <= hi) {
                            matched = true
                        }
                        pi += 3
                    } else {
                        if (sc == p[pi]) {
                            matched = true
                        }
                        pi += 1
                    }
                }
                if (pi < plen) {
                    pi += 1
                }
                return false if (matched == negate)
                si += 1
            }
        } else {
            return false if (si >= slen)
            return false if (s[si] != pc)
            pi += 1
            si += 1
        }
    }
    return si == slen
}

//! Match a pathname against a shell-style glob.
//! Supports `*` (any chars except `/`), `**` (any chars incl. `/`), `?` (one non-`/` char),
//! `[abc]` / `[a-z]` character classes, `[!abc]` / `[^abc]` negated classes.
//! Pure path-string match — does not touch the filesystem. Both pattern and path use `/`.
def match_glob(pattern : string; path : string) : bool {
    let plen = length(pattern)
    let slen = length(path)
    return true if (plen == 0 && slen == 0)
    return false if (plen == 0)
    if (slen == 0) {
        // Empty path matches a pattern only when every char is `*` (any number of stars or **).
        var ok = true
        peek_data(pattern) $(arr) {
            for (i in range(length(arr))) {
                if (arr[i] != uint8('*')) {
                    ok = false
                    break
                }
            }
        }
        return ok
    }
    var result = false
    peek_data(pattern) $(parr) {
        peek_data(path) $(sarr) {
            result = _match_glob_bytes(parr, 0, sarr, 0)
        }
    }
    return result
}

//! Convert a path to use forward slashes regardless of host OS.
//! Useful for paths that flow into `/`-based matchers (`match_glob`,
//! `glob`, RST `:ref:` targets, comma-separated path manifests, etc.).
//! Note: `fio.normalize` is the *opposite* — it uses the platform-preferred
//! separator (`\` on Windows). Use `to_generic_path` when you want stable
//! `/`-separated output independent of host.
def to_generic_path(s : string) : string {
    return modify_data(s) $(var arr) {
        for (i in range(length(arr))) {
            if (arr[i] == uint8('\\')) {
                arr[i] = uint8('/')
            }
        }
    }
}

//! Walk `root` recursively and invoke `blk` for every entry whose path (relative to `root`)
//! matches `pattern`. Both the matched path and the path passed to `blk` are normalized
//! via `to_generic_path` so callbacks receive stable `/`-separated paths regardless of host.
//! Order matches `dir_rec`.
[generic]
def glob(root : string; pattern : string; blk : block<(filename : string; is_dir : bool) : void>) {
    dir_rec(root) $(filename, is_dir) {
        let normalized = to_generic_path(filename)
        if (match_glob(pattern, normalized)) {
            invoke(blk, normalized, is_dir)
        }
    }
}

//! Walk `root` and invoke `blk` for entries that match at least one of `includes`
//! and none of `excludes`. Excludes win on conflict. Paths passed to `blk` are
//! normalized via `to_generic_path` (forward slashes regardless of host OS).
[generic]
def glob_filtered(root : string; includes : array<string>; excludes : array<string>;
                  blk : block<(filename : string; is_dir : bool) : void>) {
    dir_rec(root) $(filename, is_dir) {
        let normalized = to_generic_path(filename)
        var included = false
        for (pat in includes) {
            if (match_glob(pat, normalized)) {
                included = true
                break
            }
        }
        if (included) {
            for (pat in excludes) {
                if (match_glob(pat, normalized)) {
                    included = false
                    break
                }
            }
        }
        if (included) {
            invoke(blk, normalized, is_dir)
        }
    }
}

//! Returns true if `pattern` contains any glob metacharacter (`*`, `?`, `[`).
//! Used by callers that treat literal patterns differently from glob patterns.
def is_glob_pattern(pattern : string) : bool {
    var has_meta = false
    peek_data(pattern) $(arr) {
        for (i in range(length(arr))) {
            let c = arr[i]
            if (c == uint8('*') || c == uint8('?') || c == uint8('[')) {
                has_meta = true
                break
            }
        }
    }
    return has_meta
}

// convenience: disk_space returning struct
[generic]
def disk_space(path : string) : DiskSpaceInfo {
    var info = DiskSpaceInfo()
    var error : string
    builtin_disk_space(path, info, error)
    return info
}

[generic]
def disk_space(path : string; var error : string&) : DiskSpaceInfo {
    var info = DiskSpaceInfo()
    builtin_disk_space(path, info, error)
    return info
}

// ---- result variants for error reporting ----

variant fs_result_bool {
    value : bool;
    error : string;
}

variant fs_result_int64 {
    value : int64;
    error : string;
}

variant fs_result_string {
    value : string;
    error : string;
}

// ---- _result wrappers ----

[generic]
def file_size_result(path : string) : fs_result_int64 {
    var error : string
    let sz = file_size(path, error)
    if (!empty(error)) {
        return fs_result_int64(error = error)
    }
    return fs_result_int64(value = sz)
}

[generic]
def equivalent_result(a : string; b : string) : fs_result_bool {
    var error : string
    let res = equivalent(a, b, error)
    if (!empty(error)) {
        return fs_result_bool(error = error)
    }
    return fs_result_bool(value = res)
}

[generic]
def is_symlink_result(path : string) : fs_result_bool {
    var error : string
    let res = is_symlink(path, error)
    if (!empty(error)) {
        return fs_result_bool(error = error)
    }
    return fs_result_bool(value = res)
}

[generic]
def copy_file_result(src : string; dst : string; overwrite : bool) : fs_result_bool {
    var error : string
    let res = copy_file(src, dst, overwrite, error)
    if (!empty(error)) {
        return fs_result_bool(error = error)
    }
    return fs_result_bool(value = res)
}

[generic]
def set_mtime_result(path : string; time : Time) : fs_result_bool {
    var error : string
    let res = set_mtime(path, time, error)
    if (!empty(error)) {
        return fs_result_bool(error = error)
    }
    return fs_result_bool(value = res)
}

[generic]
def temp_directory_result() : fs_result_string {
    var error : string
    let res = temp_directory(error)
    if (!empty(error)) {
        return fs_result_string(error = error)
    }
    return fs_result_string(value = res)
}

[generic]
def create_temp_file_result(prefix : string; ext : string) : fs_result_string {
    var error : string
    let res = create_temp_file(prefix, ext, error)
    if (!empty(error)) {
        return fs_result_string(error = error)
    }
    return fs_result_string(value = res)
}

[generic]
def create_temp_directory_result(prefix : string) : fs_result_string {
    var error : string
    let res = create_temp_directory(prefix, error)
    if (!empty(error)) {
        return fs_result_string(error = error)
    }
    return fs_result_string(value = res)
}

[generic]
def relative_result(path : string; base : string) : fs_result_string {
    var error : string
    let res = relative(path, base, error)
    if (!empty(error)) {
        return fs_result_string(error = error)
    }
    return fs_result_string(value = res)
}

[generic]
def remove_result(path : string) : fs_result_bool {
    var error : string
    let res = remove(path, error)
    if (!empty(error)) {
        return fs_result_bool(error = error)
    }
    return fs_result_bool(value = res)
}

[generic]
def rename_result(old_name : string; new_name : string) : fs_result_bool {
    var error : string
    let res = rename(old_name, new_name, error)
    if (!empty(error)) {
        return fs_result_bool(error = error)
    }
    return fs_result_bool(value = res)
}

[generic]
def mkdir_result(path : string) : fs_result_bool {
    var error : string
    let res = mkdir(path, error)
    if (!empty(error)) {
        return fs_result_bool(error = error)
    }
    return fs_result_bool(value = res)
}

[generic]
def rmdir_result(path : string) : fs_result_bool {
    var error : string
    let res = rmdir(path, error)
    if (!empty(error)) {
        return fs_result_bool(error = error)
    }
    return fs_result_bool(value = res)
}

[generic]
def rmdir_rec_result(path : string) : fs_result_bool {
    var error : string
    let res = rmdir_rec(path, error)
    if (!empty(error)) {
        return fs_result_bool(error = error)
    }
    return fs_result_bool(value = res)
}
