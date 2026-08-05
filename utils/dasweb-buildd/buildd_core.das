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

//! What a build may produce; must stay within what the server serves
//! (build_artifacts.das is authoritative).
let ALLOWED_OUTPUT_SUFFIXES = fixed_array(".wasm", ".js", ".html", ".data", ".css")

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

def is_valid_output_filename(name : string) : bool {
    return is_boring_component(name, ALLOWED_OUTPUT_SUFFIXES)
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

def collect_artifacts(out_dir : string) : array<BuiltFile> {
    //! The build output dir, one level deep, sorted by name — the upload
    //! manifest. Names the server would refuse are skipped loudly rather than
    //! failing the set: a build may legitimately drop scratch files beside its
    //! outputs.
    var names : array<string>
    dir(out_dir) $(name) {
        if (name != "." && name != "..") {
            names |> push(name)
        }
    }
    sort(names)
    var out : array<BuiltFile>
    out |> reserve(length(names))
    for (name in names) {
        let full = path_join(out_dir, name)
        let st = stat(full)
        if (!st.is_valid || !st.is_reg) {
            continue
        }
        if (!is_valid_output_filename(name)) {
            logger_warning("buildd.core", "skipping non-artifact output {name}")
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
