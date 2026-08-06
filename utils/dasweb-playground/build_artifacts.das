options gen2
options indenting = 4

module build_artifacts public

require daslib/fio
require daslib/sha_256
require daslib/logger
require daslib/json_boost
require daslib/strings_boost
require strings

//! The artifact side of the build cache: the blobs/<toolchain>/<hash>/<file>
//! layout, component validation, integrity verification, and stage-then-rename
//! placement so a half-written upload can never be served. Every name that
//! becomes a filesystem path is validated here — this module is the choke
//! point for that. Zero HTTP, zero SQL.

//! Grows as delivery modes do; an unlisted suffix fails the upload loudly
//! rather than caching something the pages cannot use.
let ALLOWED_ARTIFACT_SUFFIXES = fixed_array(".wasm", ".js", ".html", ".data", ".css")

struct ArtifactFile {
    name : string
    sha256 : string
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

def is_valid_toolchain_id(t : string) : bool {
    //! Abbreviated-to-full git sha: 7..64 lowercase hex.
    return is_lower_hex(t, 7, 64)
}

def is_valid_artifact_hash(h : string) : bool {
    //! Full sha256: exactly 64 lowercase hex.
    return is_lower_hex(h, 64, 64)
}

def private has_allowed_suffix(name : string) : bool {
    // a .br sidecar is allowed only stacked on an allowed base name
    var base = name
    if (base |> ends_with(".br")) {
        base = slice(base, 0, length(base) - 3)
    }
    for (suffix in ALLOWED_ARTIFACT_SUFFIXES) {
        if (base |> ends_with(suffix)) {
            return true
        }
    }
    return false
}

def is_valid_artifact_filename(name : string) : bool {
    //! One boring path component: short, [A-Za-z0-9._-] only, no leading dot,
    //! no "..", known suffix. The charset alone excludes separators, so a name
    //! that passes cannot traverse.
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
    return has_allowed_suffix(name)
}

def artifact_rel_dir(toolchain : string; hash : string) : string {
    //! The blobs-relative artifact directory — what BuildJob.ArtifactPath stores.
    return path_join(toolchain, hash)
}

def private stage_root(blobs_dir : string) : string {
    return path_join(blobs_dir, "tmp")
}

def clear_stage_dirs(blobs_dir : string) {
    //! Startup sweep: a crash mid-upload leaks a stage dir; they live under
    //! blobs/tmp and nothing serves from there, so removal is always safe.
    if (!empty(blobs_dir)) {
        rmdir_rec(stage_root(blobs_dir))
    }
}

var private g_stage_counter = 0

def create_stage_dir(blobs_dir : string) : string {
    //! A fresh upload staging dir under blobs/ — same filesystem as the final
    //! location, so the finalize rename is atomic. "" on failure.
    let root = stage_root(blobs_dir)
    mkdir_rec(root)
    if (!stat(root).is_valid) {
        logger_error("playground.artifacts", "cannot create stage root {root}")
        return ""
    }
    g_stage_counter ++
    let stage = path_join(root, "stage_{int64(get_clock())}_{g_stage_counter}")
    if (!mkdir(stage)) {
        logger_error("playground.artifacts", "cannot create stage dir {stage}")
        return ""
    }
    return stage
}

def stage_file_path(stage_dir : string; name : string) : string {
    //! Where one uploaded part lands inside a staging dir; "" when the name is
    //! not one this cache would ever serve. Path assembly for the artifact
    //! cache lives in this file, so there is one place to audit it.
    if (empty(stage_dir) || !is_valid_artifact_filename(name)) {
        return ""
    }
    return path_join(stage_dir, name)
}

def discard_stage(stage_dir : string) {
    //! Drop a staging dir and everything in it — the failure path of an upload.
    if (!empty(stage_dir)) {
        rmdir_rec(stage_dir)
    }
}

def artifact_size(path : string) : int {
    //! Byte size for the request log; 0 when the path is not a readable file.
    let st = stat(path)
    return st.is_valid && st.is_reg ? int(st.size) : 0
}

def finalize_artifact(blobs_dir : string; toolchain : string; hash : string; stage_dir : string; files : array<ArtifactFile> const) : bool {
    //! Verify every staged file against its manifest sha, then rename the stage
    //! into place. The stage dir is consumed either way, so a failed upload
    //! leaves nothing behind — and nothing half-written is ever served.
    if (empty(files)) {
        rmdir_rec(stage_dir)
        return false
    }
    for (f in files) {
        let p = path_join(stage_dir, f.name)
        let st = stat(p)
        if (!st.is_valid || !st.is_reg || st.size == 0ul) {
            logger_error("playground.artifacts", "staged file missing or empty",
                JV((hash = hash, toolchain = toolchain, name = f.name)))
            rmdir_rec(stage_dir)
            return false
        }
        let hex = sha256_file_hex(p)
        if (hex != f.sha256) {
            logger_error("playground.artifacts", "staged file failed integrity check",
                JV((hash = hash, toolchain = toolchain, name = f.name)))
            rmdir_rec(stage_dir)
            return false
        }
    }
    let final_dir = path_join(blobs_dir, artifact_rel_dir(toolchain, hash))
    if (stat(final_dir).is_valid) {
        // content-addressed: an existing artifact for this key IS this artifact
        rmdir_rec(stage_dir)
        logger_info("playground.artifacts", "artifact already present",
            JV((hash = hash, toolchain = toolchain)))
        return true
    }
    mkdir_rec(path_join(blobs_dir, toolchain))
    var err = ""
    if (!rename(stage_dir, final_dir, err)) {
        logger_error("playground.artifacts", "cannot place artifact: {err}",
            JV((hash = hash, toolchain = toolchain)))
        rmdir_rec(stage_dir)
        return false
    }
    logger_info("playground.artifacts", "artifact stored",
        JV((hash = hash, toolchain = toolchain, files = length(files))))
    return true
}

def resolve_artifact_file(blobs_dir : string; toolchain : string; hash : string; name : string) : string {
    //! Absolute path of a served artifact file, or "" — every component is
    //! request-derived, so each one revalidates here regardless of what the
    //! caller already checked.
    if (empty(blobs_dir) || !is_valid_toolchain_id(toolchain) || !is_valid_artifact_hash(hash) ||
            !is_valid_artifact_filename(name)) {
        return ""
    }
    let p = path_join(path_join(blobs_dir, artifact_rel_dir(toolchain, hash)), name)
    let st = stat(p)
    return st.is_valid && st.is_reg ? p : ""
}
