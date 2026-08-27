# dasLLAMA architecture - retired notes

Past-tense passages archived out of `modules/dasLLAMA/ARCHITECTURE.md` when it was groomed to
the `ARCHITECTURE_COMMON.md` contract and split into companions. Each passage is verbatim, with
the section it sat in. The present-tense facts they carried stayed in the live documents.

## From sec.1.5 GPU backends

- **Retired: the single-pass whisper-decoder attention (`MetalWdecAttn`/`enc_wdec_attn`)** -
  deleted in the metal-asr review round (2026-08-17, the `bbatkin/metal-asr` PR; the kernel is
  in git history). The chunked part/comb pair replaced it during bring-up (1470->709 ms on the
  turbo decode) and its `float[1504]` tgmem bound was the only reason for the driver's old
  ta/n_text_ctx<=1504 shape cap, which left with it.

## From sec.3 Inherited invariants, the `x64_arch.md` block

(The x64-era "fallback rail" framing - AOT/interp as correct-but-untuned tiers - is retired;
intrinsic fallback bodies exist for off-ARCH correctness *inside* the JIT, not as runnable
tiers.)

## From sec.3 Inherited invariants, the `kv_cache_compression.md` block

**Correction:** that doc's "BNNS not pursued" tail is superseded by the AMX arc -

## From sec.3 Inherited invariants, the `API_REWORK.md` block

**CORRECTED:** the hybrid Gated-DeltaNet lane that doc lists as deferred Tier-3 **SHIPPED** -
