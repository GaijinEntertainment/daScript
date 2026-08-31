options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_version shared public

require dasllama/dasllama_lint public

//! dasLLAMA's RELEASE number — decoupled from the daslang version, LLVM_JIT_CODEGEN_VERSION
//! and IMAGE_VERSION. Bumped ONLY on a declared release: when the maintainer rules that bench
//! comparability is broken and the leaderboard must re-mint. Routine kernel work does NOT bump
//! it — per-change invalidation lives in finer mechanisms (IMAGE_VERSION and the reflected
//! layout fingerprint for images; kernel-identity hashing for tune winners when it lands).
//! Tune sidecars scope on it, the sidecar exchange keys validity on (release, box), and bench
//! records carry it — all under the wire name `dasllama_version`, which is schema-stable.
let DASLLAMA_RELEASE = 15
