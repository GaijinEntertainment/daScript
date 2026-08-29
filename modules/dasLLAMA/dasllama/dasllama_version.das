options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_version shared public

require dasllama/dasllama_lint public

//! dasLLAMA's release counter — decoupled from the daslang version and LLVM_JIT_CODEGEN_VERSION.
//! ANY kernel work bumps it (REVIEW.md): equal versions mean an equal kernel roster, and the
//! sidecar exchange keys validity on (version, box). Bench records and sidecar provenance carry it.
let DASLLAMA_VERSION = 13   // v13: the MoE decode arc - cm2 s tiles + f16 gather, the decode attention block, the FFN tail, the router/top-k/combine span kernels
