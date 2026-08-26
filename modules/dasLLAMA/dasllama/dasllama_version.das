options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_version shared public

require dasllama/dasllama_lint public

//! dasLLAMA's release counter — decoupled from the daslang version and LLVM_JIT_CODEGEN_VERSION.
//! ANY kernel work bumps it (REVIEW.md): equal versions mean an equal kernel roster, and the
//! sidecar exchange keys validity on (version, box). Bench records and sidecar provenance carry it.
let DASLLAMA_VERSION = 10   // v10: the M5 NAX shapes arc - tall M-tile + half-X + dev-W stamps, MoE tensor twins + gather panel, f16 attention slab, bk=64 staged chunks
