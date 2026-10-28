# Reference-engine patches - translation

Local patches applied to the translation reference before it runs. Every patch here is part of the
reference binary's identity: the version capture records the base commit plus each patch file's
sha256, so a receipt names exactly what ran.

## The base

`marian-decoder` from Mozilla's `translations` repository (the clone URL and the pin are
`MARIAN_COMMIT` in `../../../performance/setup_asr_rig.das`), the `inference/` tree only: the
bergamot-translator and the marian fork Firefox Translations builds. On ARM it serves the int8 GEMMs
through ruy and the f32 ones through Apple Accelerate; on x86 through intgemm.

## marian-oracle.patch

Applied at the repository root. Three changes:

1. `MARIAN_DUMP_DIR=<dir>` turns every `debug()`-marked node into a `.npy` written as the forward
   computes it, named `<stage>_<NNN>.npy` with one counter per stage: `enc_emb`, `enc_l<N>` once a
   sentence, then per decode step `dec_emb`, `dec_l<N>_rnn`, `dec_l<N>_ctx`, `dec_l<N>_ffn` and
   `logits`. The marks sit in `src/models/transformer.h`; unset, a marked node does nothing, so the
   patched binary times as upstream does (`MARIAN_DEBUG_LOG=1` restores upstream's log of every
   `debug()` node).
2. Clang 21 builds: two variable-length arrays in `ExceptionWithCallStack.cpp` become `const`
   sizes, and zlib's `zutil.h` stops defining `fdopen` away on `TARGET_OS_MAC`.
3. `marian-fork/CMakeLists.txt` no longer runs `git submodule update --init --recursive` at
   configure: from inside the repository it initializes every submodule of the training pipeline
   and emsdk too. The rig initializes the `inference/` submodules it builds, emsdk excepted.

## marian-sentencepiece.patch

Applied inside the `inference/marian-fork/src/3rd_party/sentencepiece` submodule: one `constexpr`
enum cast clang 21 rejects becomes `const`.
