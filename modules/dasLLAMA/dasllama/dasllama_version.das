options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_version shared public

require dasllama/dasllama_lint public

//! dasLLAMA's release counter — decoupled from the daslang version and LLVM_JIT_CODEGEN_VERSION.
//! ANY kernel work bumps it (REVIEW.md): equal versions mean an equal kernel roster, and the
//! sidecar exchange keys validity on (version, box). Bench records and sidecar provenance carry it.
let DASLLAMA_VERSION = 11   // v11: the M5 pp deep-dense arc - tall in-kernel-dequant kq stamps (TH128), MoE q5_1 tensor twins, dev-W tiles to 32
