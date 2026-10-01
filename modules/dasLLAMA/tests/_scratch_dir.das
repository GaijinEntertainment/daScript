// The per-process scratch directory the model-free and stocked cells write their file fixtures into: created on first
// use, removed at exit. Writing fixtures into the worktree would leave untracked state, and a shared path races
// parallel dastest runs. Root-free: a model-free file may require it.

options gen2
options _dasllama_internal = true

module _scratch_dir shared public

require daslib/fio

var private g_scratch_dir : string
var private g_scratch_inited = false

//! `name` in this process's scratch directory, the directory created on the first call; a directory that cannot be
//! created panics - "" would resolve to the working directory and defeat the hermetic goal
def scratch_path(name : string) : string {
    if (!g_scratch_inited) {
        let r = create_temp_directory_result("dasllama_test")
        if (!(r is value)) {
            panic("could not create the scratch directory for the test's file fixtures: {unsafe(r.error)}")
        }
        g_scratch_dir = unsafe(r.value)
        g_scratch_inited = true
    }
    return path_join(g_scratch_dir, name)
}

//! `content` written to `name` in the scratch directory; its path back
def scratch_file(name, content : string) : string {
    let p = scratch_path(name)
    fopen(p, "wb") $(f) {
        if (f != null) {
            fwrite(f, content)
        }
    }
    return p
}

[finalize]
def private remove_scratch_dir {
    if (g_scratch_inited && !empty(g_scratch_dir)) {
        rmdir_rec(g_scratch_dir)
    }
}
