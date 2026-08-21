# Reference-engine patches

Local patches applied to reference engines before benchmarking. Every patch here is part of a
benchmarked binary's identity: the version capture in the profiling rig records the base commit
**plus** the patch file's sha256, so a receipt can name exactly what ran.

## mtmd-timing.patch

Base: llama.cpp @ `ebd048fc5` (ggml-org/llama.cpp). Applied to the CPU-only build whose
`llama-mtmd-cli` serves as the audio-in reference (`MTMD_BIN`). Three changes:

1. `MTMD_TIMING encode_prefill_ms E decode_ms D total_ms T n_prefill P n_decode N` line on
   stderr — the timing the rig parses. Wall-clock around `eval_message` (encode + prefill) and
   `generate_response` (decode); model load is outside both, so the parsed total excludes it.
   `n_prefill`/`n_decode` are the position counts (`n_past` after prefill, and the decode
   delta), so a tok/s derived from the split prices own-positions over own-time — the image
   reference cells' contract.
2. `--reasoning off` enabled for the mtmd example (upstream gates it to server/completion/cli)
   and plumbed through `common_chat_format_single` as `enable_thinking` — benchmark turns must
   not spend tokens on a thinking channel.
3. `DASLLAMA_IDS: <token ids>` dump on stderr — token-for-token parity verification against
   the das side.

Apply with: `git apply patches/mtmd-timing.patch` on a clean checkout of the base commit
(`setup_asr_rig.das --refs` does this).
