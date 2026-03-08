options gen2
options rtti
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false
options no_global_variables = false
options persistent_heap

module lockfile shared public

require fio
require strings
require daslib/json
require daslib/json_boost

struct PackageEntry {
    source : string
    version : string           // requested version (e.g. "1.0" from install foo@1.0)
    tag : string               // resolved git tag (e.g. "v1.0" after resolve())
    branch : string            // resolved git branch (if branch-based)
    root : bool = true
    local : bool = false
}

struct LockFile {
    sdk_version : string
    packages : table<string; PackageEntry>
}

let LOCK_FILE_NAME = "daspkg.lock"

def lock_file_path(project_root : string) : string {
    return "{project_root}/{LOCK_FILE_NAME}"
}

def read_lock_file(project_root : string; var lf : LockFile) : bool {
    let path = lock_file_path(project_root)
    var ok = false
    fopen(path, "rb") <| $(f) {
        if (f == null) {
            return
        }
        let text = fread(f)
        var error : string
        var parsed = read_json(text, error)
        if (parsed == null) {
            print("Error reading {path}: {error}\n")
            return
        }
        lf = from_JV(parsed, type<LockFile>)
        ok = true
    }
    return ok
}

def write_lock_file(project_root : string; lf : LockFile) : bool {
    let path = lock_file_path(project_root)
    let text = sprint_json(lf, true)
    var ok = false
    fopen(path, "wb") <| $(f) {
        if (f == null) {
            print("Error: cannot write {path}\n")
            return
        }
        fwrite(f, text)
        fwrite(f, "\n")
        ok = true
    }
    return ok
}
