// The scratch directory the model-free and stocked cells write their file fixtures into: dastest's `test_temp_dir`,
// private to this run of the test file and removed when the run ends. Writing fixtures into the worktree would leave
// untracked state, and a shared path races parallel dastest runs. Root-free: a model-free file may require it.

options gen2
options _dasllama_internal = true

module _scratch_dir shared public

require daslib/fio
require dastest/testing_boost
require strings   // peek_data: the header-only GGUF fixture's string bytes

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
    gguf_push_le(b, 0x46554747ul, 4)   //! GGUF
    gguf_push_le(b, 3ul, 4)
    gguf_push_le(b, 0ul, 8)            //! no tensor
    gguf_push_le(b, 1ul, 8)            //! one key
    gguf_push_str(b, "general.architecture")
    gguf_push_le(b, 8ul, 4)            //! a string value
    gguf_push_str(b, arch)
    let p = scratch_path(name)
    fopen(p, "wb") $(f) {
        if (f != null) {
            fwrite(f, b)
        }
    }
    delete b
    return p
}

def private gguf_push_le(var b : array<uint8>; v : uint64; nbytes : int) {
    b |> reserve(length(b) + nbytes)
    for (i in range(nbytes)) {
        b |> push(uint8(uint(v >> uint64(i * 8)) & 0xFFu))
    }
}

def private gguf_push_str(var b : array<uint8>; s : string) {
    gguf_push_le(b, uint64(long_length(s)), 8)
    peek_data(s) $(d) {
        for (c in d) {
            b |> push(c)
        }
    }
}
