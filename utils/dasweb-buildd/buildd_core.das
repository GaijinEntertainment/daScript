options gen2
options indenting = 4

module buildd_core public

require daslib/fio
require daslib/sha_256
require daslib/json_boost
require daslib/logger
require daslib/strings_boost
require strings

//! The build-execution side of dasweb-buildd: materialize a claimed job's
//! source document, run the configured build command, and collect the produced
//! artifacts with their integrity hashes. Zero network — buildd_client owns
//! HTTP, main owns lifecycle. The server revalidates everything this module
//! produces; validation here exists to fail fast and to keep user-authored
//! names from ever becoming interesting paths on this box.

struct BuiltFile {
    name : string
    sha256 : string
    path : string   // absolute, for the upload
}

struct BuildRunResult {
    ok : bool
    exit_code : int
    output : string   // combined stdout+stderr (the build script redirects its own stderr)
}

def private is_boring_component(name : string; suffixes) : bool {
    let n = length(name)
    if (n < 2 || n > 128) {
        return false
    }
    var ok = true
    peek_data(name) $(bytes) {
        for (b in bytes) {
            let c = int(b)
            if (!(is_alpha(c) || is_number(c) || c == '.' || c == '_' || c == '-')) {
                ok = false
            }
        }
    }
    if (!ok || name |> starts_with(".") || find(name, "..") >= 0) {
        return false
    }
    for (suffix in suffixes) {
        if (name |> ends_with(suffix)) {
            return true
        }
    }
    return false
}

def is_valid_source_filename(name : string) : bool {
    //! Bundle file names are user-authored; one boring `.das` path component
    //! or the job is refused before anything touches the filesystem.
    return is_boring_component(name, fixed_array(".das"))
}

def private is_lower_hex(s : string; min_len : int; max_len : int) : bool {
    let n = length(s)
    if (n < min_len || n > max_len) {
        return false
    }
    var ok = true
    peek_data(s) $(bytes) {
        for (b in bytes) {
            let c = int(b)
            if (!(is_number(c) || (c >= 'a' && c <= 'f'))) {
                ok = false
            }
        }
    }
    return ok
}

def is_valid_job_hash(h : string) : bool {
    //! A claimed job's hash becomes a scratch directory name here, so its shape
    //! is checked on arrival rather than trusted: this service talks to a
    //! server over the network, and "the server said so" is not validation.
    return is_lower_hex(h, 64, 64)
}

def is_valid_job_toolchain(t : string) : bool {
    return is_lower_hex(t, 7, 64)
}

def unpack_sources(src_dir : string; source : string) : string {
    //! Materialize a job's source document into src_dir and return the entry
    //! file name; "" = malformed document (refused). Two shapes, the same two
    //! the playground stores: a raw `.das` text (entry `main.das`), or the
    //! multi-file `{files, active}` JSON bundle.
    var jerr = ""
    var doc = read_json(source, jerr)
    if (doc == null || !(doc.value is _object) || !key_exists(doc.value as _object, "files")) {
        // raw single-file source
        if (!fwrite(path_join(src_dir, "main.das"), source)) {
            logger_error("buildd.core", "cannot write main.das into {src_dir}")
            return ""
        }
        return "main.das"
    }
    let files = doc?["files"]
    if (files == null || !(files.value is _object)) {
        return ""
    }
    let entry : string = doc?["active"] ?? ""
    if (!is_valid_source_filename(entry)) {
        logger_error("buildd.core", "bundle has no usable active entry")
        return ""
    }
    var entry_seen = false
    for (name, v in keys(files.value as _object), values(files.value as _object)) {
        if (!is_valid_source_filename(name)) {
            logger_error("buildd.core", "refusing bundle file name {name}")
            return ""
        }
        // Refuse a file whose content is not text, rather than coercing it to
        // "" — `?? ""` would quietly compile an EMPTY file under a hash that
        // describes something else, and the user would read the resulting
        // compile error as a fault in code the builder had actually discarded.
        // Reachable: POST /api/samples stores arbitrary text, so a bundle can
        // carry a number or an object here.
        if (v == null || !(v.value is _string)) {
            logger_error("buildd.core", "bundle file {name} is not text")
            return ""
        }
        let content : string = v ?? ""
        if (!fwrite(path_join(src_dir, name), content)) {
            logger_error("buildd.core", "cannot write {name} into {src_dir}")
            return ""
        }
        if (name == entry) {
            entry_seen = true
        }
    }
    return entry_seen ? entry : ""
}

def run_build_command(build_cmd : string; src_dir : string; out_dir : string; mode : string; entry : string; timeout_seconds : float) : BuildRunResult {
    //! Invoke `<build_cmd> <src_dir> <out_dir> <mode> <entry>` via argv — no
    //! shell, so nothing in a path or name is ever interpreted. build_cmd is a
    //! space-separated prefix (`bash run_build.sh`); the timeout is the
    //! wall-clock kill for a wedged build.
    var argv <- [for (part in split(build_cmd, " ")); part; where !empty(part)]
    if (empty(argv)) {
        return BuildRunResult(exit_code = -1, output = "build_cmd is not configured")
    }
    argv |> push(src_dir)
    argv |> push(out_dir)
    argv |> push(mode)
    argv |> push(entry)
    var output = ""
    let rc = unsafe(popen_argv(argv, timeout_seconds) $(f) {
        if (f != null) {
            output = fread(f)
        }
    })
    return BuildRunResult(ok = rc == 0, exit_code = rc, output = output)
}

def expected_artifact_names(mode : string) : array<string> {
    //! The EXACT file set a mode may publish. Not a suffix filter: the build
    //! writes into this directory with the user's own compile-time code, so
    //! anything it drops beside the real output would otherwise be uploaded
    //! and served publicly. Empty = the mode publishes nothing (page mode
    //! defines its set when the page checkpoint wires it).
    if (mode == "module") {
        return <- ["sample.wasm"]
    }
    return <- array<string>()
}

def collect_artifacts(out_dir : string; mode : string) : array<BuiltFile> {
    //! The mode's expected artifacts, in order, hashed for the upload
    //! manifest. Everything else in the directory is ignored and logged —
    //! a build cannot widen its own output set.
    var out : array<BuiltFile>
    let wanted <- expected_artifact_names(mode)
    if (empty(wanted)) {
        logger_error("buildd.core", "no artifact set defined for mode '{mode}'")
        return <- out
    }
    var seen : table<string>
    dir(out_dir) $(name) {
        if (name != "." && name != "..") {
            seen |> insert(name)
        }
    }
    for (name in seen |> keys()) {
        if (find_index(wanted, name) < 0) {
            logger_warning("buildd.core", "ignoring unexpected build output", JV((name = name, mode = mode)))
        }
    }
    out |> reserve(length(wanted))
    for (name in wanted) {
        let full = path_join(out_dir, name)
        // A symlink here is an attack, not an artifact: the build writes this
        // directory with the user's own compile-time code, and everything
        // downstream (stat, hashing, the multipart upload) reads the path on
        // the HOST, outside the sandbox. Pointed at a host file, that
        // publishes it. Refuse the job rather than resolve the link.
        var link_err = ""
        if (is_symlink(full, link_err)) {
            logger_error("buildd.core", "refusing symlinked artifact", JV((name = name, mode = mode)))
            out |> clear()
            return <- out
        }
        let st = stat(full)
        if (!st.is_valid || !st.is_reg) {
            logger_error("buildd.core", "expected artifact missing", JV((name = name, mode = mode)))
            continue
        }
        let hex = sha256_file_hex(full)
        if (empty(hex)) {
            logger_warning("buildd.core", "skipping unreadable or empty output {name}")
            continue
        }
        out |> push(BuiltFile(name = name, sha256 = hex, path = full))
    }
    return <- out
}

def manifest_json(files : array<BuiltFile>) : string {
    //! The [{name, sha256}] manifest field of a result upload.
    var arr <- [for (f in files); JV((name = f.name, sha256 = f.sha256))]
    return write_json(JV(arr))
}

def derive_toolchain(worktree : string) : string {
    //! The worktree's HEAD sha = the toolchain identity; "" on any failure.
    var out = ""
    let rc = unsafe(popen_argv(["git", "-C", worktree, "rev-parse", "HEAD"], 30.0) $(f) {
        if (f != null) {
            out = fread(f)
        }
    })
    let sha = strip(out)
    if (rc != 0 || length(sha) != 40) {
        logger_error("buildd.core", "cannot derive toolchain from {worktree} (rc {rc})")
        return ""
    }
    return sha
}
