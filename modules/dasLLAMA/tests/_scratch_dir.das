// The scratch directory the model-free and stocked cells write their file fixtures into: dastest's `test_temp_dir`,
// private to this run of the test file and removed when the run ends. Writing fixtures into the worktree would leave
// untracked state, and a shared path races parallel dastest runs. Root-free: a model-free file may require it.

options gen2
options _dasllama_internal = true

module _scratch_dir shared public

require daslib/fio
require dastest/testing_boost

//! `name` in this run's scratch directory
def scratch_path(name : string) : string => path_join(test_temp_dir(), name)

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
