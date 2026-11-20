options gen2
options indenting = 4

module llvm_jit_sweep shared private

//! The record of the cached JIT artifacts a run used, and the sweep
//! that deletes the rest. A run under DAS_JIT_MANIFEST records each
//! cached DLL it loads or writes as an empty file `<folder>.<key>` in
//! that directory, and a written DLL names the objects it was linked
//! from in `<key>.files` beside it. `jit_sweep` keeps what the
//! manifest names and deletes every other file of the cache's
//! `_anon_*` folders.

require daslib/fio
require daslib/strings_boost
require llvm/daslib/llvm_env

let private CACHE_FOLDER_PREFIX = "_anon_"

def public jit_key_prefix(key : string) : string => "{key}."

def private link_list_path(artifact : string) : string => "{artifact}.files"

//! true for a file one cached DLL owns, given the DLL key's
//! `jit_key_prefix` and the objects it was linked from
def public jit_key_keeps(fname, prefix : string; link_objects : table<string>) : bool =>
    fname |> starts_with(prefix) || link_objects |> key_exists(fname)

[arch(at = "../ARCHITECTURE_CODEGEN_IDENTITY.md#jit-cache-sweep")]
def public jit_manifest_record(artifact, key : string) {
    //! a DLL at a pinned output path has no `key` and is not recorded
    if (empty(key) || empty(g_env_jit.jit_manifest)) return
    mkdir_rec(g_env_jit.jit_manifest)
    let name = "{base_name(dir_name(artifact))}.{base_name(artifact)}"
    to_log(LOG_INFO, "LLVM JIT: DAS_JIT_MANIFEST records {name}\n")
    if (!fwrite(path_join(g_env_jit.jit_manifest, name), "")) {
        to_log(LOG_WARNING, "LLVM JIT: cannot record {name} in {g_env_jit.jit_manifest}\n")
    }
}

[arch(at = "../ARCHITECTURE_CODEGEN_IDENTITY.md#jit-cache-sweep")]
def public jit_write_link_list(artifact : string; objs : array<string>) {
    fwrite(link_list_path(artifact), join([for (o in objs); base_name(o)], "\n"))
}

struct public JitSweepResult {
    error : string //! why nothing was swept; empty when the sweep ran
    folders_kept : int
    folders_removed : int
    files_kept : int
    files_removed : int
    bytes_kept : uint64
    bytes_removed : uint64
    failed_deletes : int //! files and folders it could not delete, a held DLL among them
}

struct private CacheFile {
    name : string
    size : uint64
}

//! one cached DLL the manifest names in a folder: its key's prefix
//! and the objects its link list names
struct private UsedKey {
    prefix : string
    link_objects : table<string>
}

def private read_manifest(manifest_dir : string) : table<string; array<string>> {
    var used : table<string; array<string>>
    dir(manifest_dir) $(name) {
        let dot = find(name, ".")
        if (dot > 0) {
            used[slice(name, 0, dot)] |> push(slice(name, dot + 1))
        }
    }
    return <- used
}

def private list_files(folder : string) : array<CacheFile> {
    var files : array<CacheFile>
    dir(folder) $(name) {
        let st = stat(path_join(folder, name))
        if (st.is_valid && st.is_reg) {
            files |> push(CacheFile(name = name, size = st.size))
        }
    }
    return <- files
}

def private read_used_keys(folder : string; keys : array<string>) : array<UsedKey> {
    var used : array<UsedKey>
    used |> reserve(length(keys))
    for (key in keys) {
        var link_objects : table<string>
        for (obj in split(fread(link_list_path(path_join(folder, key))), "\n")) {
            link_objects |> insert(obj)
        }
        used |> emplace(UsedKey(prefix = jit_key_prefix(key), link_objects <- link_objects))
    }
    return <- used
}

def private delete_or_report(path : string; whole_folder : bool; var res : JitSweepResult&) {
    var err : string
    let deleted = whole_folder ? rmdir_rec(path, err) : remove(path, err)
    if (!deleted || !empty(err)) {
        res.failed_deletes++
        to_log(LOG_WARNING, "LLVM JIT: sweep could not delete {path}: {err}\n")
    }
}

[arch(at = "../ARCHITECTURE_CODEGEN_IDENTITY.md#jit-cache-sweep")]
def public jit_sweep(cache_dir, manifest_dir : string; dry_run : bool) : JitSweepResult {
    //! a `dry_run` counts what it would delete and deletes nothing
    var res : JitSweepResult
    if (!stat(manifest_dir).is_dir) {
        res.error = "no manifest directory {manifest_dir}"
        return res
    }
    var used <- read_manifest(manifest_dir)
    if (empty(used)) {
        res.error = "the manifest directory {manifest_dir} names no DLL"
        return res
    }
    var folders : array<string>
    dir(cache_dir) $(name) {
        if (name |> starts_with(CACHE_FOLDER_PREFIX) && stat(path_join(cache_dir, name)).is_dir) {
            folders |> push(name)
        }
    }
    for (folder_name in folders) {
        let folder = path_join(cache_dir, folder_name)
        var inscope files <- list_files(folder)
        if (!used |> key_exists(folder_name)) {
            res.folders_removed++
            for (f in files) {
                res.files_removed++
                res.bytes_removed += f.size
            }
            if (!dry_run) {
                delete_or_report(folder, true, res)
            }
            continue
        }
        res.folders_kept++
        var inscope keys <- read_used_keys(folder, used[folder_name])
        for (f in files) {
            var kept = false
            for (key in keys) {
                kept ||= jit_key_keeps(f.name, key.prefix, key.link_objects)
            }
            if (kept) {
                res.files_kept++
                res.bytes_kept += f.size
                continue
            }
            res.files_removed++
            res.bytes_removed += f.size
            if (!dry_run) {
                delete_or_report(path_join(folder, f.name), false, res)
            }
        }
    }
    return res
}

def public jit_sweep_summary(res : JitSweepResult; dry_run : bool) : string {
    let mib = 1024ul * 1024ul
    let verb = dry_run ? "would delete" : "deleted"
    let removed = "{res.files_removed} files ({int64(res.bytes_removed / mib)} MiB)"
    let kept = "{res.files_kept} files ({int64(res.bytes_kept / mib)} MiB)"
    let deleted = "{verb} {removed} and {res.folders_removed} whole folders"
    let stayed = "kept {kept} in {res.folders_kept} folders"
    if (res.failed_deletes == 0) return "{deleted}; {stayed}"
    return "{deleted}; {stayed}; {res.failed_deletes} could not be deleted, each named above"
}

def public jit_sweep_cli(cache_dir, manifest_dir : string; dry_run : bool) : int {
    //! 0 when the sweep ran, 1 when it had no manifest to sweep by
    if (empty(manifest_dir)) {
        to_log(LOG_ERROR, "jit: nothing swept - DAS_JIT_MANIFEST is not set\n")
        return 1
    }
    let res = jit_sweep(cache_dir, manifest_dir, dry_run)
    if (!empty(res.error)) {
        to_log(LOG_ERROR, "jit: nothing swept - {res.error}\n")
        return 1
    }
    to_log(LOG_INFO, "jit: {jit_sweep_summary(res, dry_run)}\n")
    return 0
}
