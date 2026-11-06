// The scratch directory the model-free and stocked cells write their file fixtures into: dastest's `test_temp_dir`,
// private to this run of the test file and removed when the run ends. Writing fixtures into the worktree would leave
// untracked state, and a shared path races parallel dastest runs. Root-free: a model-free file may require it.

options gen2
options _dasllama_internal = true

module _scratch_dir shared public

require daslib/fio
require dastest/testing_boost
require dasllama/dasllama_gguf   // gguf_put_le, gguf_put_str: the header-only GGUF fixture's bytes
require _gguf_writer   // gguf_header: the fixture's v3 header

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

//! a header-only GGUF whose one key is `general.architecture` = `arch`, written to `name` in the scratch directory: a
//! file every reader recognizes and no backend can load
def arch_gguf(name, arch : string) : string {
    var b : array<uint8>
    gguf_header(b, 0ul, 1ul)
    gguf_put_str(b, "general.architecture")
    gguf_put_le(b, 8ul, 4)            //! a string value
    gguf_put_str(b, arch)
    let p = scratch_path(name)
    fopen(p, "wb") $(f) {
        if (f != null) {
            fwrite(f, b)
        }
    }
    delete b
    return p
}
