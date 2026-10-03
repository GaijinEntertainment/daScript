options gen2
options indenting = 4
module release_workspace shared public
require daslib/fio
require daslib/logger

def private release_workspace_remove(path : string) : bool {
    var error : string
    if (rmdir_rec(path, error)) {
        return true
    }
    to_log(LOG_ERROR, "release: cannot remove temporary workspace {path}: {error}\n")
    return false
}

def with_release_workspace(work : block<(path : string) : int>) : int {
    var error : string
    let path = create_temp_directory("daspkg_release", error)
    if (empty(path)) {
        to_log(LOG_ERROR, "release: cannot create temporary workspace: {error}\n")
        return 1
    }
    let rc = work(path)
    let removed = release_workspace_remove(path)
    return removed ? rc : 1
}
