# Vulkan hybrid ladder - the whole-model resident driver serves Qwen3.5-class hybrids

The arc after PR #3932. Branch `bbatkin/vk-hybrid-ladder`, commit one is the serving-decline
logging (every gate names its reason). Ledger home: `modules/dasLLAMA/followup_vulkan.md` item 2
(family coverage in the resident driver); the k-native deltanet planes and the device KV codecs
stay their own items (`PERF_LEDGER.md` k4 dn planes, `followup_vulkan.md` item 3).

## The target

Qwen3.5-9B-MTP-UD-Q5_K_XL on the 5060 Ti. Today the resident driver declines it on four arms it
lacks (the load log names them: in-file nextn block, gated attention, partial rotary 64 of 256,
24 of 32 recurrent layers), so the per-op tier serves attention and deltanet on the device and
streams the 32 dense FFNs (3330 MB) through the CPU every token:

| row | ours (per-op tier) | llama.cpp Vulkan b10660 | ratio |
|---|---|---|---|
| pp512 | 131.8 | 2527.0 | 0.052x |
| tg128 | 11.07 | 56.74 | 0.195x |

llama.cpp's decode is bandwidth-bound: 6.4 GB per token in 17.6 ms, about 363 GB/s. Fully
resident with the Q8-transcoded deltanet planes the model reads ~6.9 GB per token; at the
driver's usual ~360 GB/s that is ~52 tok/s (0.92x), and the k-native dn planes close the rest.
Parity bar is Boris's: same or better on the row.

Dev vehicle for correctness: Qwen3.5-0.8B-Q8_0 (fits every arm, minutes per run). The 9B UD
file is the headline and the regression gate. Dev runs carry `DASLLAMA_ALLOW_UNTUNED=1` (the
rebuilt binary aged the sidecar; Boris ruled no re-mint until Vulkan is fully functional).

## What exists - the port pattern

- Metal's resident loop (`dasllama_metal_decode.das` `encode_layer`) already serves this shape:
  the recurrent branch (qkv/z/beta/alpha GEMVs, conv, l2norm, delta scan, gated out-norm, out
  GEMV) with per-layer device state slices, the gated-attention deinterleave and sigmoid gate,
  partial rope, and the NextN draft arm at `l == n_layers`.
- Every Vulkan kernel the ladder needs is a class kernel already: the fused decode step
  `dn_step_cls` with its per-layer `DnStep` (state, smalls, parity ring, session owner -
  `ARCHITECTURE_GPU_VULKAN_DECODE.md` sec.2.2u), the prefill chain
  `dn_conv_cls` / `dn_scan_cls` / `dn_scan_p3_cls` (`record_dn_cmd`), and the attention
  prep/attention pair `at_prep_cls` / `at_attn_cls` whose `flags` already carry the gated
  deinterleave and the sigmoid epilogue (`record_at_cmd`).
- The resident token command (`rd_encode_token`) is one recorded chain per layer: requant, q/k/v
  GEMVs over arena planes, qk-norm+rope storing the mirror row, attention over the mirror,
  requant, wo, add+rms, gate/up, act, down, add+rms. Its kernels are `qkn_rope_cls` / `rope_kv_cls`
  / `da_attn_cls` over the `k_mirror`/`v_mirror` slabs, sized `n_layers x seq_cap x kv_dim`.

## The design (ruled by default; a fork worth Boris's eye is marked)

1. **Recurrent layers ride the token command.** `RLayer` grows a recurrent form: arena blocks for
   qkv/z/out (Q8-transcoded, the loader already tags dn planes q8 for the GPU tiers) plus the
   beta/alpha rows (q8 planes, or the f32 fblob rows the 35B carries), the GEMV sets, and a
   `DnStep` slot. The chain per recurrent layer: requant, qkv GEMV (cd rows), z GEMV (di rows,
   y base cd), beta/alpha GEMVs into the smalls, `dn_step_cls`, out GEMV (di -> dim), add+rms.
   State stays where sec.2.2u put it - the `DnStep` owner slots keyed by the session's host
   addresses, flushed home on a switch - so the single-session resident driver and the per-op
   tier share one state discipline and one release path (`vk_dn_step_release`).
2. **The KV mirror gets per-layer bases: only attention layers own rows.** Today
   `mirbase = l * seq_cap * kvd`; a hybrid at 32k would carve 4 GB where 1 GB is used. The plan
   sizes `kv_bytes` per layer already (`layer_kv_dim` is 0 on recurrent layers); prepare takes
   the attention-layer count and each RLayer carries its mirror base. `rdec_sync_kv` /
   `read_kv` / hydrate address by the same base.
3. **Gated attention in the resident kernels, not a detour through the batch chain.** The q GEMV
   writes `2 * qd` rows in the per-head `[q | gate]` layout; the qk-norm+rope class and the
   decode attention class take a `gated` word (the `at_prep` / `at_attn` arms transplanted): q
   is read head-strided from the packed row, the gate row stashed, and the attention epilogue
   multiplies by `sigmoid(gate)` before the wo requant. The q buffer grows to `2 * qd`.
4. **Partial rope is a `rot` word on the rope kernels**: rotate pairs `< rot / 2` per head, the
   rest pass through; the cos/sin row carries `rot / 2` entries. The per-op quad chain already
   does this (`rot` in the attention-quad walk); the resident kernels get the same word.
5. **The nextn block is not a decline.** The trunk is `n_layers`; the MTP head's planes are
   separate offsets the CPU round reads (Vulkan's speculative round is the CPU round today,
   `followup_vulkan.md` item 5). `resident_upload` drops `n_layer_nextn > 0` from its feature
   list; nothing in the resident arm walks past `n_layers`. Fork for Boris: an MTP head resident
   as a 33rd layer of the token command (the Metal shape) is a later item, not this arc.
6. **Prefill: the window chain gets the recurrent layer.** `pf_run` encodes, per recurrent layer,
   the chunked chain over the arena planes (`dn_conv`, `scan_p1`, `scan_p2`, requant, out GEMM)
   with the state and conv tail carried across windows on the device, and the gated attention
   through the `at_prep`/`at_attn` flags the batch chain already has (the cm2 flash tile has no
   gated epilogue - gated layers keep `at_attn`, as `at_fa_serves` says). Decode lands first;
   prefill is the second half.
7. **Batch decode** stays what it is: the single-session chain per row, KV synced in and out.
   The deltanet state for a row rides the `DnStep` owner switch (flush + cold upload per layer
   per switch) - correct, and slow for two streams; per-session slots are `followup_vulkan.md`
   item 38, after this arc.

## Order of work

1. DONE - Decode: recurrent RLayer + `dn_step_cls` in the token command, per-layer mirror bases, the
   nextn gate dropped. Gate: `tests/test_gpu_resident_hybrid.das` (resident logits vs the CPU chain
   within the deltanet bar on Qwen3.5-0.8B-Q8_0), `test_gpu_model_swap` under `DASLLAMA_GPU=1`.
2. DONE - Decode: the `qstride`/`gated` words on `qkn_rope_cls` / `da_attn_cls`, `half = rot / 2` and
   the `rot`-built cos row. The 0.8B cell runs the whole model resident: maxdiff 0.19-0.25 on a
   bar of ~0.5, argmax equal every step, controls red. Found on the way: the arena tally and
   placement used `qd` rows for a gated q plane (2 x qd) - half the plane unplaced, garbage gate.
3. IN PROGRESS - The 9B UD file. Resident: tg128 49.74 +- 0.79 (was 11.07; llama.cpp 56.74, 0.877x),
   GPU span 19.5 ms/token, wall 20.0; pp512 95.7 (CPU prefill until step 4). Trap fixed: the auto
   VRAM plan (cap - 2 GiB) armed at 12.5 GB of the 16 GB card and WDDM demoted (6.8 tok/s; 12 GB
   pinned 3.4, 11 GB pinned 49.8) - the auto headroom is now max(2 GiB, 27% of the cap)
   (`ARCHITECTURE_GPU_VULKAN.md` 2.2n). The two-stream server scenario (the 9/4 run: an 8k and a
   short stream in flight together) passed - coherent replies, GC clean, exit 0; it surfaced the
   scheduler's head-of-line wait (~150 s for the short stream behind the 8k prefill) and the
   logger's tee-mode line corruption (fixed in the runtime, commit afbcd4d20). Still owed here:
   `harness/parity.das` pinned greedy on the 0.8B under `DASLLAMA_GPU=1`, the pp512 row on the 9B
   with the resident prefill (step 4).
4. DONE - Prefill: the window chain's recurrent and gated arms (`pf_setup` / `pf_run`,
   dasllama_vulkan_prefill.das). Gate green on the 0.8B: prefill logits within 0.19-0.24 of the
   CPU chain on a bar of ~0.5 at 40, 128, 200, 256, 384, 511 and 600 (two windows) tokens, every
   fed step within 0.23-0.31, argmax equal, controls red. Found on the way: the prefill addressed
   the K/V mirror by layer index (the hybrid mirror has one slot per attention layer), and the
   rope/attention sets declared the q plane at `qd` rows while a gated q row is `2 x qd` - rows
   past 256 fell outside the bound range. The 9B UD file, resident prefill + decode: pp512
   1365.0 +- 4.6 (was 95.7 on the CPU prefill; upstream 2527, 0.54x), tg128 53.3 +- 0.1 (was
   49.7; upstream 56.7, 0.94x). The per-role profile (`followup_vulkan.md` item 2 carries the
   tables) put 141 of the window's 349 ms in the deltanet scan; splitting phase 2 per state column
   slice (+ a phase-3 out-norm) and register-tiling the staged GEMM took the scan to 63 ms and
   pp512 to 1710.6 +- 15.6 (0.68x). As built:
   `rd_pf_recurrent` per
   recurrent layer (the FFN tail shared with the attention head), per-layer conv/scan/tail sets on
   `RLayer`, the o requant shared, beta/alpha by the row-strided `router_gemv_cls` (f32) or a
   batch GEMM + copy (q8); gated q and partial rope by a per-head q stride (`qhs`) and the
   `half = rot / 2` pass-through on `rope_kv_b` / `qk_rms` / `da_attn_b` (no `at_prep` detour -
   the mirror attention gates in its epilogue); the state comes home through
   `vk_rdec_prefill_dn_flush` and the decode's owner bind re-uploads it. The original design notes
   follow; where they differ, the code above is what landed:
   - Mirror slots: `mirb = l * seq_cap * kvd` in `pf_run` becomes `L.mir_idx * ...`, and
     `pf_setup`'s `mirbytes` uses `n_attn` (the decode side already does).
   - Recurrent layer, per window: the qkv and z batch GEMMs (`pf_gemm_enc` into new
     `pf_dnqkv [np x cd]` and `pf_dnz [np x di]` planes), the beta/alpha rows for every position
     into the layer's smalls at `DN_SM_BETA`/`DN_SM_G` (position-major `[pos][nvh]`, the layout
     `dn_scan_cls` reads; q8 arm = a batch GEMM with `d = nvh`, f32 arm = `dn_ba_cls`, a
     16-position tile GEMM over the `[beta ; alpha]` rows), then `dn_conv_cls` (history from the
     layer's ring image at the shared parity on the first window, from the tail rows after),
     `dn_scan_cls` (the sequential scan over the layer's own `L.dn.state_dev`, raw o rows into the
     tier's `dn_ws_dev`) and `dn_scan_p3_cls` (the gated out-norm in place), the o
     requant, the out batch GEMM into `pf_xb2`, and the shared FFN tail. The last window's tail
     rows land in the decode ring's per-channel layout (a small transpose kernel, taps x cd ->
     cd x taps, into the image the next decode step reads); the state stays on device, dirty,
     owned by the prefilling session (`rdn`-style owner bind before the window).
   - Gated attention: the q batch GEMM at `2 x qd`; `at_prep_cls` (deinterleave + qk-norm +
     partial rope, the per-op chain's kernel) writes the compact q panel and a gate stash from
     one smalls buffer holding every attention layer's rms_q row plus the window's cos/sin rows
     (its `rms_off` is a push offset); k/v keep `rope_kv_b` with `half = rot / 2`, q pairs
     excluded; `DaAttnBT` gains the `gated` epilogue reading the stash (the cm2 fa tile has none,
     so a gated model takes the DaAttnB pair). Partial rope alone is the `half` word.
   - The prefill then serves hybrids and `resident_prefill_shape_ok` widens; the embed gate
     follows; `rdec_take_mirror` stays for CPU-prefilled and superseded sessions.
   Gate: prefill parity cells on the 0.8B (the resident window chain vs the CPU chunked prefill
   within the deltanet bar), the decode cell unchanged, pp512 row on the 9B toward the 2527
   reference.
5. Docs: `ARCHITECTURE_GPU_VULKAN_DECODE.md` gains the hybrid token-command section (anchored,
   `[arch]` on the recorder), `followup_vulkan.md` item 2 closes, the decline list in
   `resident_upload` shrinks to what still declines.
6. The k-native deltanet planes (the last lever that moves both pp and tg). Today the loader
   tags the deltanet qkv/z/out planes with their file formats only on the CPU-only path
   (`dasllama_load.das`, the "deltanet dn tags" pass); with a GPU tier installed they stay
   untagged, `fmt_at` answers q8, and `load_big` transcodes the Q5_K/Q6_K planes to Q8_0 - on
   the 9B UD that is ~400 MB more per decoded token (the whole tg gap) and the prefill's
   deltanet GEMMs on the q8 tiles instead of the k5/k6 ones. The resident driver is already
   format-generic below the tag: `rdec_set_dn_layer` carries `fqkv/fz/fout`, the decode's
   `gemv_cls_*` dispatch per format with the Q8_K x feed keyed on `kq_sb(L.fqkv)`, and the
   prefill's `pf_gemm_enc` takes the layer's format with the f16 feed (`pf_dn6`). The work:
   - Loader: tag natively also when the whole-model driver will be attempted (the Vulkan tier
     installed, `gpu_want_auto()`, a dense hybrid, no Metal, not a grouped file). A later
     resident decline leaves those planes on the CPU rail (the per-op deltanet rails read q8),
     the same cliff a kq file has on the per-op tier today; the log names it.
   - `resident_plan` tallies the triple by its formats; `resident_place` and the
     `rdec_set_dn_layer` call pass them; `resident_layer_decline` admits q8 and the superblock
     formats the resident classes serve, and keeps a decline for a kq-tagged OUT plane (the
     decode's o feed is the step's Q8_0 row; a Q8_K o requant is a later seat).
   - Gate: `test_gpu_resident_hybrid` gains the K-quant fixture (Qwen3.5-0.8B-Q4_K_M, minted
     from the Q8_0 with llama-quantize), the declines test names the out-plane case, the 9B
     UD pair against a same-day llama-bench control, the profiled window's dn GEMM roles.
   - Docs: 2.2j / 2.2v say the planes ride their file formats; the loader's tag comment follows.

## Measurement

One rig: `benchmarks/lcpp_bench.das` under `DASLLAMA_GPU=1 DASLLAMA_ALLOW_UNTUNED=1
--for-debug-purposes`, reference `D:/Work/llama.cpp/build-vulkan/bin/Release/llama-bench.exe
-ngl 99` run by hand (the bench's `--ref` passes `-ngl 0`). One row at a time, 16 lanes.
`DASLLAMA_GPU_PROF=1` for the per-role token split when a row disappoints.
