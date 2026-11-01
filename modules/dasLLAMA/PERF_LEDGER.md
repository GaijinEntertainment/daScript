# dasLLAMA Performance Ledger

**Living document.** The perf backlog for the whole dasLLAMA engine - CPU, Metal, and the
audio/ASR towers: what each finding costs today, what the fix would change, and which
hypotheses were measured and refuted.

Standing rule (Boris, 2026-07-01): any performance possibility spotted while doing wave work
gets a note HERE instead of being acted on mid-wave - the model waves optimize for correctness
and coverage; this ledger is the backlog for the perf pass that follows them. Every entry says
what it costs today and what the fix would change.

## Entries
- **MEASURED (2026-10-28, `debug-jit`) - the Vulkan embedding window's bars and lasting allocations
  (`ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-scratch`).** Box: the RunPod RTX PRO 4500 Blackwell 32 GB (Linux, 48
  vCPU), branch `bbatkin/embed-speed` at 4588f21a08 plus the pre-PR fix batch, `bin/daslang -jit` with `DASLLAMA_GPU=1`,
  `DAS_TUNE_MANIFEST=/workspace/nmt_pod.tune.json` (predating the binary: untuned stamps), Qwen3-Embedding-0.6B-Q8_0 loaded
  in memory (`load_model_`), one reading each.

  | bar | value | what it holds | reading | cell |
  |---|---|---|---|---|
  | `EMB_MEAN_BAR` (test_embeddings) | 0.999 cosine | the device's mean read (f16 mirror, GPU sums) of a 2x-passage text against the CPU's f32-cache read | 0.99989 | `test_embeddings_on_the_resident_driver` |
  | the batch bar (test_embeddings) | 0.9999 cosine, least over the texts | the device batch's vectors against the device singles' (one scratch window against a window a text) | 0.99997 over the six batch texts | `test_embeddings_on_the_resident_driver`, `test_embeddings_window_rows_follow_the_region` |
  | `EMB_MEAN_BAR` on a held run (test_embeddings) | 0.999 cosine | the free run's device vectors against the held run's f16 host-cached reads | 0.99931 (device batch against the CPU's f16 sessions, the same texts) | `test_embeddings_every_region_held` |

  Lasting allocations, bytes: the context's embed session pool (`g_embed_pool`) holds one session a text of the largest
  window the context ran; a host-cached session's flat K/V is 2 x n_layers x seq_len x n_kv_heads x head_dim x codec bytes
  (Qwen3-Embedding-0.6B: 28 x 8 x 128 at f16 - 3.76 GB at its 32768 context, 0.94 GB at the server worker's 8192), a
  device-home session holds no K/V, only its rows' scratch per window (npos x dim x 4 bytes a buffer). The Vulkan window's
  hidden-rows landing (`pf_hid_host`) is pf_np x dim x 4 bytes, host-visible, made at the first ask: 16 MiB at 4096 rows x
  1024 (the plan's window on this box), 8 MiB at 2048.
- **MEASURED (2026-10-27 / 2026-10-28, `direction-grade`, `debug-jit`) - the short window's split-K form, the GEMV tail
  peel, the dev-W floors and the hidden-only window (`ARCHITECTURE_GPU_PREFILL_GEMM.md#prefill-split-k`,
  `#devw-panel-knees`, `#gemv-tail-peel`, `ARCHITECTURE_GPU_PREFILL.md#prefill-hidden-only`,
  `ARCHITECTURE_GPU_VULKAN_GEMM.md#vk-short-split`).** Branch `bbatkin/embed-speed` at 4588f21a08 and its working tree,
  `-jit`, `DAS_TUNE_POLICY` unset. Boxes: m5 (Apple M5 Max, `DAS_TUNE_MANIFEST=performance/m5.tune.json`), m1 (Apple M1 Max,
  its own mint), pod (RunPod RTX PRO 4500 Blackwell 32 GB, driver 595.91, `DASLLAMA_GPU=1`, the pod profile). Every
  two-candidate row toggles one lever in one process unless it names separate runs; ratios are candidate / control.
  - **Races (`metal_split_knee_race`, `harness/tune_kernels.das` under `DAS_TUNE_MODE=tune`).** m5 crowns q8 256, K-quant
    2048, one-way rows 64, base 256 / 2048 (the q8 base knee reads 128 or 256 between rounds - a flat total), half tile 64,
    dev-W floor 64; m1 512 / 256 / 128, base 256 / 256, dev-W floor off. Pod (`vk_split_knee_race`, the ring and co-running pairs, two rounds, ms per rung's 24 steps, tile /
    split x2 / x4 / x8): 16 tiles 17.0 / 38.0 / 24.0 / 18.0, 32 tiles 18.0 / 38.6 / 26.0 / 22.4, 64 tiles 29.0 / 42.3 / 35.5 /
    38.1, 128 tiles 61.7 / 65.5 / 69.0 / 69.8, 256 tiles 123.3 / 132.5 / 136.7 / 137.3 - the split loses at every rung and
    the race picks 0, against the served window A/B below, where 128 wins every model; the default stays 128. The earlier
    race shape (one chain on one hazard rail, resident weights) picked 128.
  - **Isolated GEMM, us** (`harness/short_window_gemm_race.das`, m5, best of 9 after a 150 ms burn), the q8 32-column split
    stamp at its count against the shipped tile, K x N at 32 / 64 / 128 rows: 1024 x 1024 18.3 / 6.7 (8), 18.2 / 8.1 (4),
    18.8 / 12.0 (2); 1024 x 3072 22.8 / 12.5 (2), 23.6 / 17.9 (1), 26.3 / 30.4 past the knee; 3072 x 1024 59.7 / 12.8 (8),
    59.4 / 17.8 (4), 60.3 / 29.4 (2); 4096 x 4096 89.3 / 49.2 (2), 98.2 / 80.1 (1), 178.1 / 140.7 past; 4096 x 14336 184.7 /
    171.7, 282.8 / 249.4, 528.7 / 514.6, all past. `--wide`: the 32-column one-way tile at 96 rows on 4096 x 4096 129.7
    against the 64-column 101.4.
  - **Windows, Metal** (`benchmarks/prefill_window_probe.das --cold --ab split`, the knob forced off in the control; the
    toggles are window by window in one process). m1, the base split, 9 to 31 rows: Qwen3-Embedding-0.6B Q8_0 0.64-0.65,
    Llama-3.2-1B Q8_0 / Q4_K_M / IQ4_XS 0.67-0.72, Qwen3-4B Q4_K_M 0.75-0.82, Llama-3.1-8B Q8_0 0.88-0.91, Mistral-7B Q4_K_M
    0.89-0.95, Qwen3.6-27B Q4_K_M 288-292 -> 251-256 ms. m5 under `DASLLAMA_METAL_TENSOR=0`, 9 to 33 rows: 0.59-0.69 on the
    0.6B, 0.76-0.78 on Llama-3.2-1B Q4_K_M, 0.85 on the 27B, 1.00 at 40. m5 crowned, Qwen3.6-27B-MTP Q4_K_M at 9 / 16 rows:
    98.0 / 97.1 ms whole-K, 76.4 / 75.9 the 32-row split, 65.8 / 62.2 the 16-row split; 17 to 31 rows 74-76 on the 32-row
    split. `--ab split16` after a 2000-token prefix, 16-row / 32-row split at 9 / 16 rows: Qwen3-0.6B Q8_0 0.97 / 0.97,
    Llama-3.2-1B Q4_K_M 0.89 / 0.87, Llama-3.1-8B Q8_0 0.97 / 0.85 (walls not kept for the m1 and split16 rows; the ratios
    are as recorded). The 27B at 9 rows: 65.5 GPU ms a window, 53.5 in its split GEMMs (11.9 with them knocked out), 17.1 GB
    at about 320 GB/s against a decode step's 39.6 ms at 432 (`followup_metal.md` row 59).
  - **The K-quant knee.** The 27B's 17408-wide gate and up sites at 9 rows: 37.8 -> 29.0 ms of the window split four ways.
  - **Tail peel** (m5). Embedding corpus, the past-whole-tiles reach cut from eight rows to one: 3.87 -> 3.77 ms a query
    (`harness/embed_query_probe.das`, Qwen3-Embedding-0.6B Q8_0, 60 queries). `--ab tail`, GEMV forms / split tile over a
    whole window at 2 / 4 / 5 / 8 rows: Qwen3-0.6B Q8_0 0.85 / 0.86 / 1.10 / 1.10, Llama-3.2-1B Q4_K_M 0.68 / 0.75 / 1.03 /
    1.03. `--cold --ab tail` past the knee, row forms / tile at 34 / 36 / 40 rows: Qwen3.6-27B Q4_K_M 0.94 / 1.01 / 1.25,
    Llama-3.3-70B Q4_K_M 0.97 / 1.01 / 0.98.
  - **Dev-W floors** (`--cold --ab devw`, `DASLLAMA_METAL_TENSOR=0`, 27B Q4_K_M, dev-W / quantized): m5 1.04 / 1.04 / 0.74 /
    0.52 at 40 / 64 / 128 / 256 rows (1.34 at 40 and 64 under the fixed floor); m1 464 ms at 40 rows against 983 under the
    fixed floor, the route off by its race.
  - **Hidden-only window** (`harness/embed_query_probe.das`, m5): the embedding query 3.71 -> 3.39 ms.
  - **Served, m5** (`--ab split` against the whole-K tiles on the minted profile): the Qwen3-Embedding-0.6B query 5.89 -> 3.40
    ms in-process (`harness/embed_query_probe.das`, 60 queries); the ladder Q4_K 5.04 -> 3.14, IQ4_XS 5.73 -> 3.34, Q2_K
    4.67 -> 3.05. `benchmarks/lcpp_bench.das --for-debug-purposes -p 32 -n 0 -r 10 --ngl 99`, `DASLLAMA_METAL_SPLIT_K`
    forced per run (separate processes), tok/s off -> on: Qwen3-0.6B Q8_0 5522 -> 9165, Llama-3.1-8B Q8_0 1024 -> 1372,
    Mistral-7B Q4_K_M 1339 -> 1697, Qwen3-8B BF16 (served as q8) 1034 -> 1317. The q8 knob end to end: 3.93 / 3.90 / 3.91 /
    3.96 ms a query at 128 / 256 / 512 / 1024.
  - **Windows, Vulkan** (pod, the in-process window driver toggling `vk_split_k_tgs` window by window on one session after a
    40-window warm-up, median of 21, ms a 31-row window, knee 128 -> 0): Qwen3-Embedding-0.6B Q8_0 3.01 / 4.33, Qwen3-0.6B
    2.90 / 4.21, Llama-3.2-1B IQ4_XS 3.48 / 5.14, gemma-3-1b 4.31 / 6.09, Qwen3.5-0.8B 6.72 / 7.54, gemma-4-E2B 8.28 / 13.45 -
    each within a tenth of its own 32-row window, which reads the same either way. The rule on a full column's K-under-2048
    roles read 5-9% slower at 32 to 64 rows on the Qwen3 0.6B family and Llama-3.2-1B, faster only on gemma.
  - **Served, pod, 2026-10-28** (the working tree on 4588f21a08; ours `benchmarks/lcpp_bench.das --for-debug-purposes -p
    <pp> -n 0 -r 7` under `DASLLAMA_GPU=1 DASLLAMA_ALLOW_UNTUNED=1`; llama.cpp 9febe63 Vulkan `llama-bench -p <pp> -n 0 -ngl
    999 -r 7`; separate processes, tok/s, ours / llama.cpp, pp31 then pp32): Qwen3-0.6B Q8_0 10841 +/- 32 / 9096 +/- 76,
    12376 +/- 43 / 9171 +/- 46; Qwen2.5-0.5B Q8_0 11983 +/- 33 / 9365 +/- 110, 13563 +/- 72 / 9681 +/- 42; gemma-3-1b Q8_0
    7291 +/- 27 / 6686 +/- 52, 8176 +/- 23 / 6753 +/- 25; Llama-3.2-1B IQ4_XS 8940 +/- 24 / 6413 +/- 75, 9703 +/- 24 / 6613
    +/- 52; Qwen3.5-0.8B Q8_0 4625 +/- 10 / 6471 +/- 58, 5326 +/- 16 / 5440 +/- 32 (`followup_vulkan.md` row 163);
    Qwen3-Embedding-0.6B Q8_0 pp32 11611 +/- 115 / 8863 +/- 87, its pp31 VOID (ours +/- 3541 - a cold first rep); gemma-4-E2B
    ours 3732 +/- 20 / 3682 +/- 18, llama.cpp VOID (+/- 1360 / 748). The embedding server bench (`utils/dasllama-server`
    against `llama-server --embedding -ngl 999 -c 8192 -b 2048 -ub 2048`, Qwen3-Embedding-0.6B Q8_0, median of 7): one
    query a request 3.98 ms cv 0.2% (knee 128) / 3.96 cv 0.1% (knee 0) / llama.cpp 6.09 cv 1.0%; 255 phrases in requests of
    32 0.213 s cv 0.7% / 0.214 cv 0.7% / 0.566 cv 0.4% - the served query does not reach the short-window rule.
- **MEASURED (2026-10-27, `direction-grade`) - the Bergamot translation family on three homes
  (`ARCHITECTURE_TRANSLATE.md#nmt-device-homes`).** Harness `harness/nmt_probe.das` (the race fixture: interleaved rounds of
  the 30-line ru and en corpora, `benchmarks/nmt/corpus/`, best of 9 rounds after warm-up, ms a sentence, back to back),
  branch `bbatkin/bergamot-translate`, `-jit`, `DAS_TUNE_POLICY` unset. Boxes: m5 (Apple M5 Max, macOS 27.0.1,
  `DAS_TUNE_MANIFEST` = `performance/m5.tune.json`, which predated the binary - the kernels ran their fallback stamps) and
  the RunPod RTX PRO 4500 Blackwell 32 GB (Linux, 48 vCPU, driver 595.91, `DASLLAMA_GPU=1`). Lasting allocations: the q8
  lane adds a Q8_0 copy of the classifier beside the tied embedding (n_vocab x d x 34 / 32 bytes); each GPU home adds one
  device slab of the block GEMMs' and the classifier's Q8_0 regions (the q8 planes' elements x 34 / 32 bytes) and
  scratch sized to `NMT_DEVICE_SRC_CAP` (256) source rows; neither was measured as RSS.

  | student | m5 CPU q8 | m5 Metal | pod CPU q8 | pod Vulkan |
  |---|---|---|---|---|
  | ru-en base-memory | 3.02 | 3.79 | 6.73 | 3.54 |
  | en-ru base-memory | 3.23 | 3.76 | 6.63 | 3.50 |
  | ru-en tiny | 1.91 | 2.68 | VOID | MISS 25/30 |
  | en-ru base | 2.63 | 2.90 | VOID | VOID |

  On the M5 the CPU wins by 10-30 %: a device step is a chain of single-row dispatches, latency-bound, and the Metal
  sentence pays the command buffers' round trips. On the pod Vulkan is 1.9x the pod's CPU. VOID rows read cv above 3 % on
  both pod passes (spread to 15 %). A third pass on a fresh container with the pod profile re-minted for the binary, the
  VOID students first, kept the same picture: ru-en base-memory 6.40 CPU / 3.52 Vulkan (both valid), en-ru base-memory
  6.95 / 3.62 (cv 3.5 % / 5.0 %), en-ru base 6.10 / 2.24 (cv 9.3 % / 12.2 %), ru-en tiny CPU 4.41 (cv 6.1 %) - the pod's
  48 shared vCPUs read noisy whatever runs first, so its CPU rows stay VOID outside the base-memory pair. The tiny MISS is
  the probe's agreement gate: Vulkan's lines match the CPU q8 chain on 25 of 30 against the floor of 26, deterministically
  (Metal reads 26). Each of the five lines leaves the CPU's at one step where the device's token is the CPU's runner-up, a
  top-two gap of 0.04 to 0.23 logits on a top logit near 13 (lines 2, 7, 16, 20, 27 of `corpus/ru.txt`): near-ties the
  device's f16 keys and q8 activations decide the other way, and the step's own stamps hold their CPU references on the same device
  (`test_vkt_nmt`, `test_vkt_nmt_attn`). Interactive
  use pays more than these rows on either GPU home: after an idle gap the device's clocks and wake cost more than the CPU's.
  The bars the suite holds (`tests/test_nmt.das`, m5, `run.das -- --area translate`): the exact lane's forced-feed stages
  within 1e-4 of each stage's largest magnitude (worst 5.1e-6, en-de line 10, `dec_l2_rnn`) and its free run 30 of 30 lines
  on all five students (floor 28); the q8 lane against the f32 oracle 26 (ru-en) and 27 (en-ru) of 30 (floor 25); a device
  home against the CPU q8 chain 28 (ru-en) and 29 (en-ru) of 30 on Metal (floor 26); marian's own int8 path agrees with
  its f32 graph on 20-23 of 30. Translation LLMs through the CLI, m5 Metal:
  MiLMMT-46-1B Q4_K_M ~30-37 ms a sentence, Qwen3.6-35B-A3B (chat template, thinking off) ~120 ms.
- **MEASURED (2026-10-26, `debug-jit`) - GigaAM's exact lane on the carrier's f16 halfwords, the streamed mint, and the
  q8 lane on the Metal chain (`ARCHITECTURE_MEDIA_ASR.md#gigaam-planes`).** Box m5 (Apple M5 Max, macOS 27.0.1), branch
  `bbatkin/gigaam-asr` at d4195376b9 with the change, `bin/daslang -jit`, `DAS_TUNE_MANIFEST` = `performance/m5.tune.json`
  (41352 bytes), carrier `gigaam-v3-e2e-rnnt-f16.gguf` (452 MB); load average 6.5 to 8.3 from other sessions on the box
  through every run below. Lasting allocations: the exact lane's block GEMMs leave the f32 `blob` (n_layer x (4 ff d +
  7 d^2) floats) for the halfword `wblob` (the same count in halfwords); the f16 image is 890 MB before, 456 MB after; the
  q8 image is unchanged at 266 MB. The q8 lane on Metal adds one device slab of the block GEMMs' Q8_0 regions (n_layer x
  (4 ff d + 7 d^2) x 34 / 32 bytes, 231 MB at d 768, ff 3072, 16 layers). Peak RSS of one process that compiles, loads and
  mints the image cold (`/usr/bin/time -l`, the `.dlim` removed first, three runs each, the median; the compile alone reads
  479 MB before and 480 MB after):

  | lane | peak RSS before | peak RSS after | load + mint wall before / after |
  |---|---|---|---|
  | f16 (exact) | 1762.6 MB | 940.6 MB | 376 / 97 ms |
  | q8 | 1156.2 MB | 949.8 MB | 438 / 404 ms |

  The mint holds the carrier's mapping and one tensor's scratch; the rest of the after figure is the compile and the
  mapped carrier's pages. The CPU exact lane's outputs are the f32 plane's to the bit (the oracle cells' 108 stage compares
  read the same largest differences before and after). The q8 lane on the Metal chain: the encoder rows' rel_l2 from the CPU
  exact chain 0.0126 (ru_short) and 0.0138 (ru_mid) against the CPU q8 chain's 0.0178 and 0.0189, the token ids the CPU q8
  chain's. `lcpp_bench --asr -m gigaam-v3-e2e-rnnt-f16.gguf -r 5 -t 6 --ngl 1 [--asr-q8]`, best of 5 per run, three
  processes per lane alternating, the median run and the cv over the three:

  | lane | ru_short ms | ru_mid ms | ru_long ms |
  |---|---|---|---|
  | Metal q8 | 9.80 (cv 0.16%) | 15.22 (cv 0.35%) | 64.72 (cv 0.09%) |
  | Metal f16 | 8.64 (cv 2.3%) | 13.91 (cv 1.6%) | 59.28 (cv 1.1%) |

  On the device the q8 lane is 9 to 13% slower than the halfwords and serves off 58% of the f16 image's bytes; the policy
  keeps the exact lane on Metal and `set_gigaam_q8(true)` picks the q8 lane for memory.
- **MEASURED (2026-10-25, `debug-jit`, `direction-grade`) - GigaAM v3 e2e-RNNT against CrispASR on the M5, and where
  its wall goes on each lane.** Box m5: Apple M5 Max (Mac17,6, 18 cores, 6 performance), macOS 27.0.1 (26A434),
  branch `bbatkin/gigaam-asr` at c2362503cc plus the change that lands this entry (the VAD's calling-lane matvecs and
  float conv, the transducer step's narrowed pool, the GigaAM CPU block's `ga.*` buckets, the Metal chain's timed
  encoder), `bin/daslang -jit`, `DAS_TUNE_POLICY` unset, `DAS_TUNE_MANIFEST` = `performance/m5.tune.json` (41352
  bytes, checked before every run, re-minted against this binary at 20:21; the scope reads untuned on 21 entries -
  `nv4q8_tile_gen` and the twenty `<fmt>q8_rm_gemv_gen` (`q8q8`, `iq4nl32q8`, `q4032q8`, `q51q8`, `q5032q8`, `mx4q8`,
  `k4q8`, `k5q8`, `k6q8`, `k3q8`, `k2q8`, `q40q8`, `iq4nlq8`, `iq4xsq8`, `iq3s4q8`, `iq3sq8`, `iq3xxsq8`, `iq2sq8`,
  `iq2xsq8`, `iq2xxsq8`) - which fall back while `tune_kernel_pick` serves the 72 minted winners from their own
  entries: the rm_gemv entries master drops from the profile on purpose (a35f08cc47, the fallback chain answers the
  same mr4 on arm), and the tuner's generator plan does not race the nv4 tile yet; neither family is on the GigaAM or
  parakeet path), carrier `gigaam-v3-e2e-rnnt-f16.gguf`, the clips `corpus/ru_short.wav` (7.92 s) / `ru_mid.wav`
  (19.44 s) / `ru_long.wav` (84.72 s, past the 25 s window, so VAD spans). The das stage figures are
  `harness/asr_stage_probe.das` (`bin/daslang -jit modules/dasLLAMA/harness/asr_stage_probe.das -- -m <carrier> -w <clip>
  --lane <lane> --lang ru -r 5 --e2e [--warm 10 | --settle 60] [--vad]`: the model loaded off the clock, `--warm` untimed
  transcribes, 5 profiled passes for the `ga.encode_total` encode-only figure and the buckets, then with `--vad` 5 passes
  of `find_speech_timestamps` over the whole clip alone after one off the clock); the probe's turn walls are not stated
  here, the das end-to-end figure being the `lcpp_bench --asr` cell's. Lanes `cpu-f16` (the CPU tower's exact
  f16 planes) and `cpu-q8` under `DAS_JOBQUE_THREADS=6`, `metal` (the whole encode on the device, the transducer on
  the CPU) at the default 18 lanes; one instrument process a row, nothing else loading the box, load under 3 at each
  start. Settled is a row run with `--settle 60` and one warm-up; clocked a row after `--warm 10`. Word agreement is
  the LCS of the normalized words (lowercase, punctuation stripped, yo (U+0451) read as ye (U+0435)) over the longer of the two word counts,
  against `corpus/ru_fleurs.json`; every das row printed the transcripts the agreement column scores. The
  `lcpp_bench --asr` witnesses (`bin/daslang -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- --for-debug-purposes
  --asr -m gigaam-v3-e2e-rnnt-f16.gguf -r 5 -t 6 [--ngl 1]`, best of 5, 6 dispatch lanes, total / encode ms, taken at
  c2362503cc before the change; the cell has no f16-CPU lever, and prints its best alone, so the witnesses carry no
  spread - `followup_general.md` row 241 owes it): CPU q8 60.40 / 57.55, 153.09 / 148.06, 1053.47 / 572.88; Metal
  9.90 / 6.94, 17.27 / 11.04, 543.13 / 50.52.

  The other board boxes hold no figure in this entry: gnr, epyc9r14 and graviton4 cannot mint the CPU cell, the VAD
  walls or the ru_long walls, because their AWS instances and images were deleted on 2026-10-26.

  The CrispASR rows are CrispASR at `CRISPASR_COMMIT` 119c40317 (ggml submodule e2ae679a), the recipe
  `ARCHITECTURE_MEASUREMENT_AUDIO.md#asr-cpu-references` and `#asr-gpu-pairs`: `crispasr -m <carrier> --warmup -t 6
  <clip> x5` (`build-cpu/` Metal OFF, `build/` Metal ON), the q8_0 carrier `crispasr-quantize <f16> <q8_0> q8_0`; a
  rep's wall its `transcribed <a>s audio in <t>s (<r>x realtime)` line. The ru_mid CPU q8_0 and Metal f16 rows are
  re-takes run back to back with the scratch probe's ru_mid rows that preceded the instrument (Metal clocked: the clip
  listed 15 times, the last 5 kept); every other CrispASR figure is the same recipe through the same scratch driver
  earlier the same day, its settled and clocked rows the box states that run named. The CPU f16 rows and the re-taken
  q8_0 row read cv 0.00 because the line resolves 10 ms and 0.1x: every rep printed the same value. The onnx-asr int8
  reference (onnx-asr 0.11.0, onnxruntime 1.27.0, onnx 1.22.0, `benchmarks/asr/onnx_bench.py --variant
  gigaam-v3-e2e-rnnt-int8 --threads 6 --reps 5`) read ru_long 1347.2 ms (cv 1.33%, agreement 0.852) and its short / mid
  rows stayed void (cv 4-14% over three runs).

  The CrispASR end-to-end walls (`external`), mean of 5 (min-max) and cv, beside the word agreement of the das and
  the CrispASR transcripts on the same lane:

  | lane | clip | CrispASR ms | CrispASR cv | agreement das / CrispASR |
  |---|---|---|---|---|
  | CPU f16 | short | 900.0 (900-900) | 0.00% | 1.000 / 1.000 |
  | CPU f16 | mid | 2210.0 (2210-2210) | 0.00% | 0.962 / 0.962 |
  | CPU f16 | long | 11224.0 (11110-11330) | 0.84% | 0.991 / 0.930 |
  | CPU q8 / q8_0 | short | 195.4 (194.6-196.5) | 0.38% | 1.000 / 1.000 |
  | CPU q8 / q8_0 | mid | 446.9 (446.9-446.9), re-take | 0.00% | 0.962 / 0.962 |
  | CPU q8 / q8_0 | long | 2988.0 (2980-3000) | 0.28% | 0.991 / 0.930 |
  | Metal settled | short | 77.3 (77.2-77.5) | 0.15% | 1.000 / 1.000 |
  | Metal settled | mid | 108.2 (105.9-110.5); re-take VOID | 1.61% | 0.962 / 0.962 |
  | Metal settled | long | 484.5 (480.0-489.4) | 0.70% | 0.991 / 0.930 |
  | Metal clocked | short | 39.7 (39.4-40.6) | 1.21% | 1.000 / 1.000 |
  | Metal clocked | mid | 54.88 (54.6-55.7), re-take | 0.89% | 0.962 / 0.962 |
  | Metal clocked | long | 233.1 (231.2-238.6) | 1.32% | 0.991 / 0.930 |

  The settled CrispASR Metal re-take on ru_mid stayed void over three runs (cv 12.7 / 22.9 / 17.9%): the first rep 71.5
  / 87.3 / 79.1 ms, reps 2-5 at 54.7-57.2 - the box no longer reproduces the earlier settled 108 ms, and the earlier
  figure stands in the table. The first clocked re-take was void too (cv 37.8%, a 116.8 ms first rep).

  On ru_long the gap is the silero VAD. `find_speech_timestamps` over the whole clip read, before the change, in the
  author's scratch stage probe (`external`: the same loop at c2362503cc on the same box and lanes, without the runtime
  profile apply this instrument runs), 377.9 ms (cv 0.06%) at 6 lanes and 477.9 (cv 0.64%) settled / 485.4 (cv 0.54%)
  clocked at 18; through the instrument it reads 94.60 ms (cv 0.09%) on the CPU f16 row's 6 lanes, 95.04 (cv 1.12%)
  on the q8 row's, 92.50 (cv 0.37%) settled and 94.75 (cv 0.66%) clocked at 18 - 1.09 to 1.12 ms an audio second. The VAD's
  probabilities on ru_long, jfk and gb1 dump bit-identical before and after the change at both lane counts
  (a scratch dump of `vad_probs`' bits and the segments). On ru_mid the Metal gap is the transducer decode: 6.41 ms
  before at the default 18 lanes against 4.98 at `DAS_JOBQUE_THREADS=6` (the probe's `ga.decode_total`, 5 passes), and
  5.25 ms at 18 lanes after, the step narrowing the pool to 6 lanes for the decode (5.04 ms where the probe narrowed
  it to 5); parakeet v3's 640-wide predictor keeps the whole pool, its gb1 Metal decode 69.3 ms before and 69.0 after
  in the probe, and 88 ms in the probe when the slow tier was parked for it - why the narrowing follows the step's work
  and not the box's decode-phase parking.

  The das encode-only figure (`ga.encode_total`, mean of the 5 profiled passes, cv), short / mid / long: CPU q8 60.91
  (1.34%) / 158.97 (0.12%) / 614.03 (0.28%); CPU f16 155.51 (0.36%) / 398.53 (0.19%) / 1554.15 (0.51%); Metal settled
  6.95 (0.62%) / 11.07 (0.14%) / 50.33 (0.30%), clocked 6.98 (1.11%) / 11.01 (0.35%) / 50.37 (0.19%). CrispASR runs no
  VAD (three fixed 30 s slices, its `--chunk-seconds` default).

  The profile, ru_mid, ms per audio second (the instrument's profiled passes; the CPU block's `ga.*` buckets, the
  attention its `ga.attn_proj` (rope 0.025) and `ga.attn_heads` buckets):

  | bucket | CPU q8 | CPU f16 | Metal clocked |
  |---|---|---|---|
  | e2e | 8.408 | 20.611 | 0.868 |
  | mel (CPU) | 0.073 | 0.071 | 0.035 |
  | conv front (CPU im2col + GEMM) | 0.319 | 0.319 | on device |
  | blocks: ffn (both halves + out-LN) | 4.281 | 12.701 | on device |
  | blocks: attention (proj / heads) | 2.363 (0.842 / 1.521) | 4.083 (2.497 / 1.586) | on device |
  | blocks: conv module | 0.943 | 2.583 | on device |
  | blocks: attn_out | 0.267 | 0.809 | on device |
  | encode total | 8.178 | 20.501 | 0.566 (855 dispatches; GPU 0.521) |
  | decode (CPU RNNT) | 0.259 | 0.265 | 0.269 |

  The decode on ru_mid is 485 joint calls and 95 predictor steps a transcription; a joint call costs 4.19 us and a
  step 13.9 us on the CPU q8 row's 6 lanes, 4.95 and 14.4 us on the Metal row's narrowed 18. Shares of the CPU q8 e2e:
  ffn 51%, attention 28%, conv 11%, front 4%, attn_out 3%, decode 3%, mel 1%; CPU f16: 62 / 20 / 13 / 2 / 4 / 1 /
  0.3%; Metal: encode 65%, decode 31%, mel 4%. The Metal chain's command buffer reads 10.12 ms of GPU time and 0.18 ms
  of recording an encode against the 11.01 ms encode wall (`ga.encode.gpu`, `ga.encode.record`); the replay
  (`--replay`, one more run of the clocked ru_mid command, encode wall 13.47 ms with the replay inside it) reads the
  head restrides at 0.40 ms an encode (16 blocks x 4 dispatches, 4% of the GPU time) and the slab trio over the
  restrided heads at 1.75 ms (17%).

  The parakeet v3 yardstick (`ggml-parakeet-tdt-0.6b-v3-f32.bin`, the same instrument and lanes, Metal clocked; 24
  layers, d 1024, 8x subsampling against GigaAM's 16, 768, 4x), encode ms per audio second on jfk (11.0 s) / gb1
  (198.73 s): CPU q8 11.563 / 22.482, f32 30.142 / 40.952, Metal 1.084 / 1.123 (1025 / 2083 dispatches; GPU 0.981 / 1.113). GigaAM over parakeet, encode a second
  against jfk: CPU q8 0.71, CPU f16-against-f32 0.68, Metal 0.52; parakeet's Metal front stays on the CPU where
  GigaAM's runs on the device.

  Russian WER, scored 2026-10-26 by the author's scratch WER driver and scorer, outside the tree (the
  driver one `bin/daslang -jit` process per engine over every file, the facade `load_asr_model` / `create_session` /
  `transcribe` with the `lcpp_bench --asr --ngl` Metal arming, Metal tower encodes on every file and 0 declines; same
  box, branch at ebb375ffe0, `DAS_TUNE_MANIFEST` = `performance/m5.tune.json`, `DASLLAMA_CPU_PREFILL=1`,
  `DAS_TUNE_POLICY` unset). Sets: FLEURS ru_ru test whole (`google/fleurs` on Hugging Face, 775 utterances, 2.50 h,
  reference = its `transcription` field) and Golos test (SberDevices, OpenSLR SLR114; audio from the Hugging Face
  re-host `bond005/sberdevices_golos_10h_crowd` / `_100h_farfield` test splits, 9994 / 1916 utterances as SLR114
  lists, the OpenSLR `test.tar` link being dead): 1000 utterances per part drawn with seed 20261026 (1.11 h crowd,
  0.74 h farfield; widened from 300 because a GigaAM-minus-parakeet CI straddled zero at 300). Hypotheses and
  references normalized alike (lowercase, yo (U+0451) read as ye (U+0435), non-letter/digit to space, digits kept); WER = corpus word edits
  over reference words, 95% CI from 1000 utterance bootstraps. CrispASR is 0.8.45 at 119c40317, Metal build,
  `crispasr -m gigaam-v3-e2e-rnnt-f16.gguf --warmup -t 6 -nt <every wav>`. Carriers `gigaam-v3-rnnt-f16.gguf`
  (lowercase head), `gigaam-v3-e2e-rnnt-f16.gguf`, `ggml-parakeet-tdt-0.6b-v3-f32.bin` (lang `ru`). FLEURS references
  keep numerals and Golos references spell them, so each GigaAM head loses on the set whose convention it does not
  write; the digit-free rows drop every utterance with a digit on any side. The WER points (`external`):

  | set | GigaAM rnnt | GigaAM e2e | CrispASR e2e | parakeet v3 | rnnt - parakeet [CI] | e2e - parakeet [CI] |
  |---|---|---|---|---|---|---|
  | FLEURS, all 775 | 7.09 | 4.49 | 4.52 | 5.54 | +1.56 [+0.93, +2.19] | -1.05 [-1.46, -0.62] |
  | FLEURS, digit-free 611 | 3.37 | 3.21 | 3.20 | 4.60 | -1.23 [-1.63, -0.78] | -1.39 [-1.79, -0.98] |
  | Golos farfield, 1000 | 3.82 | 6.95 | 6.97 | 7.09 | -3.26 [-4.07, -2.53] | -0.13 [-1.16, +0.93] |
  | Golos farfield, digit-free 968 | 3.80 | 5.43 | 5.45 | 7.25 | -3.44 [-4.24, -2.70] | -1.82 [-2.72, -0.86] |
  | Golos crowd, 1000 | 2.67 | 17.08 | 17.08 | 3.81 | -1.14 [-2.11, -0.32] | +13.27 [+11.09, +15.41] |
  | Golos crowd, digit-free 874 | 2.96 | 8.05 | 8.05 | 4.25 | -1.29 [-2.64, -0.23] | +3.80 [+2.78, +4.87] |

  das and CrispASR on the same e2e weights differ by at most 0.03 WER points (`external`); 9 / 3 / 3 utterances' hypotheses
  differ at all (FLEURS / farfield / crowd).

- **MEASURED (2026-10-26, `debug-jit`) - the GigaAM leg's device-against-CPU bars and the readings behind them.** Box
  m5 (Apple M5 Max), the branch's working tree at d4195376b9 with this round's fixes, `bin/daslang -jit dastest/dastest.das
  -- --test <file> [--test-names <cell>]` under `DAS_LOG_LEVEL=info`, `DAS_TUNE_MANIFEST` = `performance/m5.tune.json`;
  each reading is the cell's own `largest difference` / `plog off by` line over its clips.

  | bar | value | what it holds | reading | cell |
  |---|---|---|---|---|
  | `GA_TWIN_BAR` (test_audio) | 1e-2 of the CPU rows' largest magnitude | the Metal GigaAM encoder rows against the CPU exact chain | 1.3e-3 to 2.7e-3 against a bar of 2.5e-2 to 2.6e-2 (about 1.05e-3 relative) on ru_short / ru_mid | `test_gigaam_metal_twin` |
  | `GA_MEL_BAR` (test_audio) | 1e-5 of each element's magnitude, floored at 1 | the device log-mel against `gigaam_log_mel` | 1.9e-6 against a bar of 1.2e-4 to 1.7e-4 | `test_gigaam_metal_twin` |
  | `PK_DEVICE_PLOG_BAR` (test_audio) | 1e-4 | parakeet's device transducer plog against the CPU decoder's | 2.3e-5 (v2, gb1, 786 tokens); 1.5e-5 on v3 gb1 | `test_parakeet_device_decode_twin` |
  | `DEVICE_PLOG_BAR` (test_gigaam) | 1e-5 | GigaAM's device transducer plog against the CPU decoder's | 2.9e-6 (rnnt q8, ru_mid, 221 tokens) | `test_gigaam_device_decode_twin` |
  | `VAD_BAR` (test_vad) | 1e-5 absolute | the VAD seat's probabilities against the CPU chunk step | 2.8e-6 on ru_long (2648 chunks), 7.7e-7 on jfk | `test_vad_gpu_twin` |
  | `TWIN_SLACK_VK` (_tower_twin) on the gigaam f16 carrier | 1.5x the tower twin bar | the Vulkan q8 tower against the CPU q8 tower | passed on the RTX PRO 4500 pod (2026-10-26); the largest difference was not kept, and the pod is stopped - the next Vulkan run of `test_gigaam_vulkan_twin` records it | `test_gigaam_vulkan_twin` |

- **STATED (2026-10-26) - the lasting buffers the GigaAM leg adds, at `GIGAAM_MAX_CLIP_S` (25 s).** Derived from the
  allocation sites and the e2e-RNNT carrier's header (d 768, 16 layers, 16 heads of 48, ff 3072, 64 mels, n_fft 320,
  hop 160, predictor and joint 320, 1025 logits); no run. A 25 s clip is 400000 samples: 2499 mel frames, 1250 after the
  first conv, T = 625 encoder frames. MiB = 2^20 B.

  | allocation | formula | bytes at 25 s |
  |---|---|---|
  | `GigaamModel` planes, exact lane (`blob` + `wblob`, mapped from the `.dlim`) | f32 front, norms and heads; L(4 ff d + 7 d^2) halfwords | 455926788 (434.8 MiB) |
  | `GigaamModel` planes, q8 lane (`blob` + `qblob` + `qscales`) | f32 front, norms and heads; L(4 ff d + 7 d^2) int8 + a scale a 32 | 266003460 (253.7 MiB) |
  | `GigaamState` CPU scratch, q8 / exact lane | the mel, front and block rows at T, the rope tables, the transducer halves | 55060248 / 52900248 (52.5 / 50.4 MiB) |
  | `GigaamState` with the encode and decode on a GPU | x, the rope tables and the transducer state | 4723072 (4.5 MiB) |
  | `GigaamState.att` | (dispatch slots) x 128 x T x 4 | 6080000 at 19 slots |
  | Metal `g_tw_rows` (the held encoder rows) | round_up(T, 32) x d x 4, a pool bucket | 2097152 (1966080 raw) |
  | Metal `g_tw_ga_mel_panel` | (384 x n_fft + n_fft + n_mel x 161) x 4 | 534016 |
  | Vulkan `g_vt_ga` (q8 lane: planes 259281924, front scratch 25544080, block scratch 62046208) | the scratch at 2500 frames and 768 rows | 346872212 (330.8 MiB); +5505024 on a cm2 device |
  | Vulkan `g_vt_fc_out_*` | an alias of the block scratch's `x_dev`, T x d x 4 | 0 of its own |
  | Metal `g_rd_w` (the transducer weights) + its pooled per-clip buffers | wh, joint halves, f32 rows, the encoder half; buckets | 4969220 + 3483904 |
  | Vulkan `g_rv` (the same weights + its scratch and host words) | exact sizes | 8211780 (7.8 MiB) |

  The VAD's resident Vulkan scratch takes `cap = max(chunks, 64)` 512-sample chunks: `vt_vad_scratch` is
  (64 + 512 cap) x 4 + 2048 cap + 1024 + 4 cap bytes on the device plus its host words, 2497536 at ru_mid (608 chunks)
  and 10869696 at ru_long (2648); `g_vt_vad_x` is (64 + 512 chunks) x 4, 1245440 and 5423360. GigaAM runs the VAD on
  ru_long alone - a clip under 25 s encodes whole - so the ru_mid figure is the formula's, not a GigaAM allocation. The
  VAD's weight slab (1236484 B) is resident whatever the clip.
- **MEASURED (2026-10-25, `debug-jit`, `direction-grade`) - the float GELU, the bf16 plane on the tile walk, the AMX
  bf16 weight tile, the mel's lanes and the decoder's K/V bytes, on the four board boxes.** The author's board boxes: m5
  (M5 Max, 18 lanes), gnr (AWS m8i, Xeon 6975P-C, 8 cores), epyc9r14 (AWS m7a, Zen 4, 16 cores), graviton4 (AWS m8g,
  Neoverse V2, 16 cores). Every figure but the board rows is a `-jit` script run on branch `bbatkin/bf16-tower-gemm` at
  the commit named, `DAS_TUNE_POLICY` unset, `DAS_TUNE_MANIFEST` the box's rig release sidecar
  (`performance/_rig/dasllama-bench*/` after `daspkg release --quick` at that commit), `-no-module-cache`; the board
  rows are the rig exe's, `performance/gen_bench_records.das` with `--store <scratch>` (the three AWS boxes hold no
  `llama-bench`, so their das rows pair with no reference), rows 240 and 242 own their re-mint; none enters
  `performance/records/`.

  The Qwen3-ASR CPU rows (`-- --workload asr -o Qwen3-ASR`, the references
  `ARCHITECTURE_MEASUREMENT_AUDIO.md#asr-cpu-references`, every cell the best of `--asr-reps` 2 - the exe prints a
  clip's best alone, row 241 - das against `llama-mtmd-cli` on gb1 (198.7 s), total / encode ms; the AWS rows at
  3f7134f8f with every `was` the 2026-10-24 tile entry's ea6d942c2 row, the M5 row at cbfe4d670f with its `was` the
  2026-10-22 release-17 entry's d71b1aacc board row): gnr 13807 / 2007 against 20857 / 7481 (1.51x; was 15797 / 2917
  against 21610 / 7304), epyc9r14 11694 / 2499 against 15916 / 8009 (1.36x; was 12784 / 3071 against 15981 / 8051),
  graviton4 8406 / 2462 against 67900 / 64309 (8.1x; was 9209 / 3168 against 67958 / 64362), m5 cpu tuned 8675 / 2475
  and cpu accel 7913 / 1608 against 46465 / 42396 (5.4x and 5.9x; was 13092 / 6460 and 10546 / 3856 against 46394 /
  42193).

  The gemma-4-E4B Q8_0 CPU rows at cbfe4d670f (`-- --workload llm -o gemma-4-E4B-it-Q8_0`, `-- --workload image -o
  gemma-4-E4B-it-Q8_0`; the PLE projection and the BF16 mmproj ride the bf16 plane; `was` the 2026-10-22 release-17
  entry's board rows): gnr (3f7134f8f) pp512 375.3 +/- 24.7 tok/s (cv 6.6 %, void - the box's pp512 reads 6 % spreads on
  every run), tg128 17.88 +/- 0.27, tg128@4 61.46 +/- 0.08 (was 325.7 / 18.1 / 61.7); m5 cpu tuned pp512 207.1 +/- 2.6,
  tg128 53.11 +/- 0.08, tg128@4 126.6 +/- 2.3 (was 202.4 / 52.5 / 125.4), cpu accel 217.7 +/- 4.5 / 53.1 / 126.6 (was
  206.6 / 53.5 / 125.8); the image-chat cell on the m5 (coco 000000039769, 130 soft tokens, the tower on the q8 lane,
  best of 3) cpu tuned encode 420 ms, pp 223, tg 51.5 tok/s (was 375 / 220.6 / 52.6), cpu accel 452 / 224.5 / 52.1 (was
  442.6 / 217.6 / 52.2): the encode's 420 against 375 is not this diff's - master 2d7d99fbff reads 427 ms and the tip
  422 under `benchmarks/lcpp_bench.das -- -m gemma-4-E4B-it-Q8_0.gguf --image-mmproj mmproj-gemma-4-E4B-it-BF16.gguf
  --image coco-val2017-000000039769.jpg -r 3 -t 6 --for-debug-purposes` (the master run from a detached worktree at
  origin/master, the same binary), an open gap against the release-17 board for the release re-mint (row 240). No other
  board box of the author's can mint the image-chat cell (gnr lacks the E4B mmproj; epyc9r14 and graviton4 hold no E4B).

  The Xeon's stage split (`harness/asr_stage_probe.das -- -m Qwen3-ASR-0.6B-Q8_0.gguf --mmproj
  mmproj-Qwen3-ASR-0.6B-bf16.gguf -w gb1.wav -r 1 --lang auto`, `JOBQUE_PROFILING=1 DAS_LOG_LEVEL=info` for the engine
  buckets, gb1): at 7d7e39fc7 (the GELU kernel and the bf16 walk) against ea6d942c2 (the 2026-10-24 tile entry's split)
  the encode 2451 against 2924 ms, `enc.ffn.gelu` 41 against 194, `q3a.conv.mm` 1123 against 1446, `q3a.blocks` 588
  against 726, `q3a.mel` 396 against 403 (the mel's frame split is 258ca3799e; the race below carries it). `q3a.conv.mm`
  is GEMM-bound: 200 chunks of a 3.3 GFLOP c2 and a 0.86 GFLOP c3 are 840 GFLOP at 750 GFLOP/s of f32 tile on 8 cores,
  against the race's 1000 hot; the mmproj's conv weights are F16 and its blocks run the q8 lane, so the bf16 family
  reaches the conv front only through an f16 ruling (row 240).

  `harness/cpu_asr_race.das -- --arm all --reps 5` at cbfe4d670f (one process a box, the arms in turn, one warmup round
  dropped, the best round; every compared arm within its bound on 512 sampled elements - the baselines to a double
  reference, the family to the bf16 envelope, the K-pass orders bit-exact with each other; the PR body carries the
  rows). The `bf16` arm, GFLOP/s at n d ntok 2560 10752 512 / 4320 480 800 / 1152 1152 256, the f32 plane on the tile
  walk / the bf16 plane on the stamped family / the widen walk / the row nest it replaced: gnr 794 / 3740 / 959 / 91,
  1013 / 2083 / 980 / 90, 1042 / 2891 / 870 / 323; epyc9r14 1425 / - / 1551 / 442, 1413 / - / 1435 / 511, 1449 / - /
  1354 / 526; graviton4 598 / - / 662 / 192, 563 / - / 599 / 153, 660 / - / 664 / 295; m5 1474 / - / 1394 / 666, 1453 /
  - / 1270 / 625, 1558 / - / 1340 / 658 (the family declines off AMX bf16). The `gelu` arm, ns an element over 4194304,
  the double body it replaced against the float kernel: gnr 9.06 against 0.78, epyc9r14 5.41 against 0.79, graviton4
  11.28 against 2.12, m5 3.34 against 0.83. The `mel` arm (`log_mel_qwen3a` over a 60 s synthetic clip, 128 mels,
  timing-only; one lane / the lanes, ms): gnr 99.4 / 17.1 (8 lanes), epyc9r14 105.4 / 11.9 (16), graviton4 106.1 / 13.0
  (16), m5 56.0 / 6.9 (18); the lanes' image is the one-lane image element for element (`tests/test_whisper.das`, the
  mel cell). The `kpass` arm (the decode attention's K pass over 28 layers x 3000 keys x 8 kv heads x 128, f16, two q
  heads a kv head, `dot_f16` a score; GB/s one lane / all lanes): (kv head, slice) units gnr 3.4 / 26.6, epyc9r14 2.6 /
  30.8, graviton4 8.1 / 85.8, m5 9.8 / 123.1; one slice over every kv head gnr 9.2 / 54.4, epyc9r14 12.3 / 70.7,
  graviton4 9.5 / 101.3, m5 20.3 / 186.4.

  gemma-4-E4B Q8_0 pp512 on gnr (`benchmarks/lcpp_bench.das -- -m gemma-4-E4B-it-Q8_0.gguf -p 512 -n 0 -r 3 -t 8
  --for-debug-purposes`, the file `unsloth/gemma-4-E4B-it-GGUF` at `resolve/main`): master 2d7d99fbf 284.7 +/- 18.3
  tok/s (cv 6.4 %, void), 24e19e9916's kernels on the widen walk 359.8 +/- 0.3, the same with the AMX entry in the
  sidecar 368.8 +/- 0.2, the frozen fixture's 8 opening tokens matching on all three. The decode on gnr at 24e19e9916's
  kernels (the stage probe, the engine buckets on): f16 K/V (the ASR session's default) 11115 ms over 451 steps,
  `attn_chain` 8843 of it, `blk_ffn` 1843, `mm_gemm` 525; `--kv q8` (the q8_0 K/V codec) 6285 ms over 443 steps,
  `attn_chain` 4097 - the transcript moves (row 242). The chain's units re-ordered to one slice over every kv head
  (fcfb1dacd8) read `attn_chain` 8942 ms against 24e19e9916's 8724 on gnr - within the run's noise; the slice order's
  two-fold over the (kv head, slice) units in the K pass does not reach the chain, whose wall sits elsewhere.

  The lasting buffers the bf16 plane's batch calls now fill, bytes as formulas in ntok and n with their values at the
  race's 2560 x 512 and 4320 x 800 shapes: `g_bf16w_xr` (new) 4 ntok n - 5.24 MB, 13.8 MB; `g_kq_xbf16` 2 (ntok n + 32)
  - 2.62 MB, 6.91 MB (the quant families' plane, now the bf16 plane's too); `g_walk_panel` where the family serves,
  (round_up(32 form n, 64) + 64) slots + 64 at form 2 - 163968 and 276608 bytes a dispatch slot, 1.31 MB and 2.21 MB
  over gnr's 8 slots.

- **MEASURED (2026-10-24, `direction-grade`) - the fp32 batch GEMM tile and the lane-split conv front on the three AWS
boards.** `matmul_batch_core` runs a four-by-four tile (`f32_batch_tile`,
`ARCHITECTURE_CPU_KERNELS.md#f32-batch-tile-lattice`); `harness/f32_batch_race.das -- --reps 5` races its two constants
and the tile against the dot-a-cell nest it replaces in one process, interleaved, best of 5 (`bin/daslang -jit`,
`DAS_TUNE_POLICY` unset and no other override; GFLOP/s; the M5 Max under `DAS_TUNE_MANIFEST` naming a scratch sidecar
the tuner minted for this kernel - `f32_batch_tile` vec4_u4 - with `DASLLAMA_ALLOW_UNTUNED=1` for the rest, the Granite
Rapids box - AWS m8i, Xeon 6975P-C, 8 cores - under its rig's minted sidecar, vec16_u2). The X block at n 4320 d 480
ntok 800: M5 1463 / 1461 / 1431 / 1448 / 1441 / 1419 and Xeon 1067 / 1075 / 1053 / 1049 / 947 / 349 at 64K / 128K / 256K
/ 512K / 1M / 4M; at n 1152 d 480 ntok 1536: M5 1431 / 1466 / 1497 / 1505 / 1511 / 1494, Xeon 943 / 980 / 999 / 984 /
955 / 310 - the walk holds 256 KB. The tile against the nest at those shapes: M5 1405 against 509 and 1493 against 525,
Xeon 1064 against 86 and 999 against 85. The tile against the nest at d 480 ntok 3200 by dot length 8 / 16 / 24 / 32 /
48 / 64 / 128: M5 346 / 578 / 730 / 862 / 1010 / 1105 / 1333 against 123 / 161 / 155 / 351 / 254 / 452 / 514, Xeon 95 /
54 / 53 / 101 / 150 / 191 / 326 against 38 / 48 / 50 / 44 / 37 / 158 / 215 - the tile wins at every length, so it serves
every length and the nest stays as the race's arm. The conv front's im2col and erf GELU ran in one lane (`bin/daslang
-jit modules/dasLLAMA/harness/asr_stage_probe.das -- -m Qwen3-ASR-0.6B-Q8_0.gguf --mmproj
mmproj-Qwen3-ASR-0.6B-bf16.gguf -w gb1.wav -r 1 --lang auto`, debug-jit, one rep, `DAS_TUNE_POLICY` unset,
`DAS_TUNE_MANIFEST` the rig release's minted sidecar under `performance/_rig/dasllama-bench/` on the Xeon and
`performance/m5.tune.json` on the M5: the Xeon's `q3a.conv.mm` 13293 ms before the tile, 5719 with it, 1446 with the two
across lanes; its encode 15227 / 7728 / 2924 ms; the M5 encode 287 ms throughout, its blocks on the Metal seat). The
boards' CPU rows: `performance/gen_bench_records.das -- --workload asr -o <model>` and `--workload tts -o kitten` on
each box, the released `dasllama-bench` exe minted on the box (`daspkg release`, the box class's backend, no policy
override) at the branch's tip (ea6d942c2; kitten and whisper rows at 5907be553 / 13c357f98, the same kernels), the
references `ARCHITECTURE_MEASUREMENT_AUDIO.md#asr-cpu-references` and `#the-tts-reference-instrument`, every cell the
best of the rig's reps (`--asr-reps` 2 a clip, `--tts-reps` 3 a sentence; the exe prints a clip's best alone, so no
per-rep spread is on record for these cells - `followup_general.md` row 241); das against the reference on gb1 (198.7
s), total / encode ms, every `was` figure the 2026-10-22 release-17 boards entry's: Qwen3-ASR-0.6B Q8_0 - gnr 15797 /
2917 against `llama-mtmd-cli` 21610 / 7304 (1.37x; was 26609 / 13996 against 15555 / 7076, 0.58x), epyc9r14 12784 / 3071
against 15981 / 8051 (1.25x; was 16875 / 7257 against 15882 / 8040), graviton4 9209 / 3168 against 67958 / 64362 (7.4x;
was 16446 / 10522 against 67613); whisper large-v3-turbo - epyc9r14 10805 against 35132 (was 10892 against 35126),
graviton4 14633 against 82439 (was 14643 against 89922); whisper tiny - epyc9r14 946 against 1517 (was 966 against
1531), graviton4 973 against 4042 (was 916 against 4064) - the whisper encoder is not on the fp32 batch path. The kitten
TTS rows re-measured in the pack's own voice (`tts:g2p20@Bella`, ms, das against `kittentts`, audio seconds agreeing
within 2%): gnr nano 8370 against 8211, mini 13022 against 88939; epyc9r14 9849 against 11445, 13845 against 112833;
graviton4 6991 against 8698, 10373 against 92066 - the generator's conv stages are not on the fp32 batch path, so the
tile is not in these walls. None of these rows is published: the record stores and the site merge stay at release 17,
the release re-mint owes the rows under `followup_general.md` row 238
(`ARCHITECTURE_MEASUREMENT_RELEASE.md#board-rows-release-boundary`). What the Xeon's encode still holds (the same probe
run): `q3a.conv.mm` 1446 ms of a 2924 ms encode - the first conv's nine-tap GEMM, which the race reads at 95 GFLOP/s
against the wide layers' thousand, and the erf GELU, whose exp runs in double a lane; its decode, 11.5 s over 451 steps,
is the attention over the ~3000 audio tokens (the GELU is the float kernel `gelu_erf` of the 2026-10-25 entry, the
attention row 242, the conv front row 240's operand item). The reference's encoder runs bf16 x bf16 arithmetic where
ours widens the file's bf16 to f32 and multiplies in f32 (the mmproj conv weights, 2 bytes a weight more and no
half-width dot); the whole bf16 tower set shares that gap.

- **MEASURED (2026-10-24, `direction-grade`) - the Granite Rapids board's losses are the DeltaNet scan and the f32
  conv front, not the AMX crowns.** AWS m8i.4xlarge (Xeon 6975P-C, 8 cores with SMT, 61 GB, the `x86-amx` class),
  master 1f52af3e4, the released `dasllama-bench` exe minted on the box (68 kernels, noise ok; `DASLLAMA_PIN_BACKEND`
  unset, the registry's own pick of the `x86-amx` backend), 8 threads unless named, 3 reps. Qwen3.6-35B-A3B UD-Q4_K_M
  pp512: `DAS_TUNE_POLICY` unset 119.7 +/- 1.6 tok/s, `DAS_TUNE_POLICY=reference` (the unstamped bodies) 122.1 +/-
  1.3, tuned at 16 threads 136.9 +/- 1.7; gemma-4-E4B Q8_0 pp512: `DAS_TUNE_POLICY` unset 321.6 +/- 18.8 (cv 5.8%,
  void), reference 328.8 +/- 0.6 - the tune crowns, the `amx_bf16` tile among them, move neither model, so the board's
  0.91x on the 35B (119.7 against `llama-bench` `clean-cpu` 131.8 tok/s) is not the AMX prefill path. The `lcpp_bench
  --prof` buckets (`JOBQUE_PROFILING=1 bin/daslang -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m
  Qwen3.6-35B-A3B-MTP-UD-Q4_K_M.gguf -p 512 -n 0 -t 8 --prof`, debug-jit, one rep, `DAS_TUNE_POLICY` unset,
  `DAS_TUNE_MANIFEST` the rig release's minted sidecar under `performance/_rig/dasllama-bench/`; shares of the 4.22 s
  pp512 window, the wrappers nest): `dn_scan` 2670 ms (63%) - the DeltaNet chunked scan, `deltanet_prefill_chunked`
  threads one v-head per lane and the box has 8 lanes where the Zen4 board has 16 - `blk_ffn` 1046 (25%, the MoE
  block), `mm_gemm` 921, `moe_router` 266, `dn_mm_in` 185; the census reads 40 grouped MoE prefill arms, E=256 k=8, so
  an expert sees about 16 tokens at pp512, under the 32-token amx tile step. Qwen3-ASR-0.6B Q8_0 on gb1 (198.7 s), the
  release-17 board rows' encode and total walls: gnr das 13996 / 26609 ms against llama-mtmd-cli 7076 / 15555;
  epyc9r14 das 7257 / 16875 against 8040 / 15882 - our encode is the same core-seconds on both boxes (core-bound), the
  reference's encode is 2.3x faster per core on the Xeon. `harness/asr_stage_probe.das` on gnr (`bin/daslang -jit
  modules/dasLLAMA/harness/asr_stage_probe.das -- -m Qwen3-ASR-0.6B-Q8_0.gguf --mmproj mmproj-Qwen3-ASR-0.6B-bf16.gguf
  -w gb1.wav -r 1 --lang auto`, debug-jit, one rep, the same sidecar, `DAS_TUNE_POLICY` unset): `q3a.conv.mm` 13293 ms
  of the 15227 ms encode - the conv2d front's im2col GEMM through `matmul_batch_core` (`dasllama_math.das`), a
  dot-per-row-per-token f32 nest with no register tile; the transformer blocks 683 ms; decode 10918 ms over 451 steps
  with `attn_chain` and `blk_attn` the dominant buckets (the audio context is about 3000 tokens). The M5 `accel`
  flavor, which hands that GEMM to Accelerate, encodes gb1 in 3856 ms against the tuned flavor's 6460 (the release-17
  boards entry).

- **MEASURED (2026-10-24, `direction-grade`) - the M5 kitten board pairs speak the pack's own voice.** A kitten voice
  name resolves through the pack's `voice_aliases` table on both sides of a pair
  (`ARCHITECTURE_MEASUREMENT_AUDIO.md#the-tts-reference-instrument`). The kitten reference rows the 2026-10-22 entry
  cites spoke the pack's Kiki voice (`expr-voice-5-f`) for `Bella` (`expr-voice-2-f`), so this entry replaces that
  entry's kitten reference figures - `kittentts` 159.95 s / 144.98 s and `mlx-audio` 159.85 s / 144.83 s of audio on
  the corpus head against das's 242.60 / 206.50 - and the 2026-09-24 entry's conclusion that Kitten's package speaks
  shorter audio than das for the same phonemes (its speed prior and 5000-sample trim): in the same voice the two sides
  agree within 2%, and a kitten pair compares on audio seconds as every other TTS pair does. `m5.json`, the corpus
  head (`tts:g2p20@Bella`, `audio_s`; walls in ms, das against the reference, the reference's 2026-10-22 wall in
  parentheses): kitten-nano - das `cpu/tuned` (259d0a3fb) 242.60 s, 3560 ms against `kittentts` `cpu/stock` (torch
  2.14.0 / kittentts 0.8.1 / onnxruntime 1.29.0) 237.85 s, 6165 ms (4256); das `metal/tuned` (259d0a3fb) 242.43 s, 550
  ms against `mlx-audio` `metal/stock` (mlx 0.32.4 / mlx-audio 0.6.1) 237.63 s, 1578 ms (1285); kitten-mini - das cpu
  206.50 s, 7329 ms against `kittentts` 207.65 s, 41545 ms (28742); das metal 206.58 s, 1729 ms against `mlx-audio`
  207.25 s, 5062 ms (4181). The das rows' walls stand within 5% of the 2026-10-22 rows' (cpu 3570 / 7412, metal 580 /
  1773; the nano metal wall moves the most, 580 to 550), on macOS 27.0.1 with the `rm_gemv` family on the box profile
  (`records/m5.tune.918b6c0e2d0f.json`) where the earlier rows ran its fallback. The AWS boxes' kitten reference rows
  (`epyc9r14`, `gnr`, `graviton4`) still speak Kiki - 159.95 s of kitten-nano audio on each, 144.7 / 144.7 / 144.6 s
  of kitten-mini - and are held off the board - `followup_general.md` row 238.

- **MEASURED (2026-10-22, `direction-grade`) - the release-17 boards' figures the ledger rows cite.** Every figure is a
  committed board row of `performance/records/<box>.json`, named below by box, model, `(engine, backend, flavor)`,
  date and `sha`; a das row is a `gen_bench_records.das` spawned cell of the released `lcpp_bench` exe, a reference
  row a spawned cell of the reference build at 9febe63d5 (its row's `cmd` the command line, its `versions` the
  packages) - the CPU rows on `ARCHITECTURE_MEASUREMENT.md#one-benchmark-rig`'s `--ref` recipe, the Vulkan rows on
  `ARCHITECTURE_MEASUREMENT_REFERENCE_ROWS.md#vulkan-reference-rows`, the TTS rows on
  `ARCHITECTURE_MEASUREMENT_AUDIO.md#the-tts-reference-instrument`, the MLX ASR rows on
  `ARCHITECTURE_MEASUREMENT_AUDIO.md#mlx-asr-reference`. A ratio is the das row over the reference row of the same
  box, two processes. The CPU reference flavor is `clean-cpu` throughout.

  The TTS corpus head (`tts:g2p20@<voice>`, `audio_s`), `m5.json`: kitten-nano Bella - das `cpu/tuned` (2026-10-22,
  d71b1aacc) 242.60 s, das `metal/tuned` (2026-10-21, 57af9f1a9) 242.43, `kittentts` `cpu/stock` (2026-10-22, torch
  2.14.0 / kittentts 0.8.1 / onnxruntime 1.29.0) 159.95, `mlx-audio` `metal/stock` (2026-10-21, mlx 0.32.4 /
  mlx-audio 0.6.1) 159.85; kitten-mini Bella - 206.50, 206.58, 144.98, 144.83; kokoro-82m af_heart - das
  `metal/tuned` 145.68, `kokoro` `metal/stock` (torch MPS, 2026-10-21) 146.63, `mlx-audio` 146.65.

  The batched decode (`tg128@4`, tok/s, das `cpu/tuned` against `llama.cpp` `cpu/clean-cpu`). `epyc9r14.json` (AWS
  m7a, das 2026-10-21 792defade): gpt-oss-20b mxfp4 46.9 against 64.7 (0.72), Qwen3.6-35B-A3B UD-Q4_K_M 44.0 / 40.4
  (1.09), Qwen3-VL-4B Q8_0 58.6 / 60.0 (0.98), Qwen2.5-Omni-3B Q8_0 76.6 / 77.7 (0.99), gemma-4-E4B Q8_0 50.1 / 50.7
  (0.99). `m5.json` (das 2026-10-22 d71b1aacc): gpt-oss-20b 82.3 / 87.8 (0.94), Qwen3.6-35B-A3B 85.2 / 90.4 (0.94).

  The image turns (`image-chat`, the reference `llama-mtmd-cli` at 9febe63d5 + mtmd-timing.patch@557dd85a).
  `m5.json`: Qwen3-VL-4B `cpu/clean-cpu` (2026-10-22) `img:pp` 30.2 tok/s against that flavor's llama-bench pp512 of
  221.8; `metal/stock` (2026-10-21) `img:pp` 441.3 against gemma-4-E4B's `metal/stock` `img:pp` 2108.8. Qwen2.5-Omni-3B
  `cpu/clean-cpu` `img:pp` against pp512: `m5.json` 46.8 / 303.3, `epyc9r14.json` 42.4 / 293.1, `gnr.json` 60.5 /
  454.0, `graviton4.json` 33.5 / 249.7. gemma-4-E4B `cpu/clean-cpu` `img:enc`: `m5.json` 13545 ms (`cpu/stock`
  13556), `graviton4.json` 10824, `epyc9r14.json` 430, `gnr.json` 730.

  The prefill (pp512, tok/s, das `cpu/tuned` against `cpu/clean-cpu`). `gnr.json` (AWS m8i, das 2026-10-22
  fbf750ffd): Qwen3.6-35B-A3B 119.7 / 131.8 (0.91), gpt-oss-20b 200.5 / 102.8 (1.95), gemma-4-E4B 325.7 / 268.7
  (1.21), Qwen3-VL-4B 489.2 / 305.4 (1.60). `epyc9r14.json` (792defade): 424.8 / 181.2 (2.34), 406.0 / 159.3
  (2.55), 368.5 / 202.5 (1.82), 531.7 / 217.4 (2.45). `graviton4.json` (AWS m8g, fbf750ffd): 309.3 / 136.9 (2.26),
  240.6 / 111.2 (2.16), 230.9 / 130.9 (1.76), 290.5 / 158.0 (1.84). The `gnr.json` das rows the release-17 rows
  replaced (2026-09-30, 60beeec07): 117.1, 198.5, 293.1, 438.1 - the release-17 rows 1.0 % to 11.7 % above them,
  Qwen2.5-Omni-3B (657.0 -> 719.5), gemma-4-26B-A4B Q4_K_M (279.5 -> 288.2) and Qwen3.8-27B Q4_K_M (46.2 -> 51.0)
  inside that span.

  The Whisper tiny CPU row, `m5.json` `ggml-tiny.bin` das `cpu/tuned`, gb1 + hp0: 728.6 + 959.4 = 1688.0 ms (2026-10-22,
  d71b1aacc) against the release-16 row it replaced, 719.2 + 912.2 = 1631.4 ms (2026-09-23, ea5747c91).
- **MEASURED (2026-10-21, `served`, `out-of-process`, `debug-jit`, `direction-grade`) - a recurrent model's checkpoint stops
  inside one Vulkan prefill window.** RunPod RTX PRO 4500 Blackwell 32 GB, the IQ4_XS-pure 35B whole on the card, every kernel
  on the x86-avx2 class profile (`DASLLAMA_ALLOW_UNTUNED=1`: the pod has no box profile). The server at `--gpu vulkan --ctx
  16384 --streams 1 --mtp=false` under `harness/served_bench.das --sizes 3000,9000 --reply 200 --reps 8 --settle 2`; the
  window under `lcpp_bench -p <N> -n 8 -r 3 -t 16 --for-debug-purposes` with `DASLLAMA_GPU=1`, the same commit:

    | row | served cold TTFT | bench window | gap |
    | :--- | ---: | ---: | ---: |
    | 9k (8812 tokens) | 1385 ms (cv 1.3%) | 1241 ms (pp9000 7104 +/- 10 tok/s) | 144 ms |
    | 3k (2990 tokens) | 540 ms (cv 6.2%, void) | 410 ms (pp3000 7291 +/- 20 tok/s) | 130 ms |

  The gap does not grow with the prompt, so the served prompt is the bench's one window plus the turn's own cost (the warm
  turn's server-side floor reads 82 / 84 ms). The warm turns attach 2966 of 2990 and 8788 of 8812 prompt tokens off the
  checkpoint; decode reads 182.3 / 170.6 tok/s at 3k / 9k. No reading before the capture exists on this card: the split's
  readings are Flash-Next's on zen2 (2026-10-19), which `followup_vulkan.md` row 157 owes.
- **MEASURED (2026-10-20, `out-of-process`, `direction-grade`) - a recurrent model's checkpoint stops inside one prefill
  window on Metal.** M5 Max, the IQ4_XS-pure 35B, the server at `--ctx 32768 --streams 4` on the plain arm (`--mtp=false`) under the
  box profile (`DAS_TUNE_MANIFEST=performance/m5.tune.json`), a cold and a warm pass, the same run as the entry below
  with the stops cut (`replay_fix_off`) against the stops captured (`replay_cap_off`; that repository's ledger): the cold
  pass runs 516 Metal prefill windows over the 248 requests where the cut form ran 645 (a window a chunk: the four planned
  stops no longer end it), its wall median 0.22 s against 0.27, first-token median 0.17 against 0.21, p90 0.59 against 0.62,
  the same 85 % cached and 7 first rounds with nothing cached; the warm pass reads the same on both (0.12 / 0.06 against
  0.11 / 0.06, 100 % cached). The served turn at one stream (`harness/served_bench.das --sizes 3000,9000 --reply 200 --reps
  5`, `--ctx 16384 --streams 1`; the row medians of the five reps, whose spread the run did not keep): the 9k cold first
  token 1800 ms against 1910 (prefill 4896 tok/s against 4611), whose
  stops split the cut form's prompt; the 3k's three stops coincide at one row, so its cold first token is the idle chunk's
  split either way (606 ms, cv 3.2 %, void, against 586) and its warm and decode rows read alike (59.8 / 68.5 ms warm,
  182.5 / 171.7 tok/s). One of 22 overheard lines reads differently on the cold pass (20 silent against 21; the warm pass
  21 on both): the CPU-side chunked scan ends a chunk at a stop's row now, a reassociation a near-tie flips on. The bytes a capture
  costs: one checkpoint - the recurrent state and the conv history, four bytes a float over `dn_state_floats`, 65.5 MB on
  the 35B - held on the host from the window's end until the cache files it under its byte budget (a device-home stream
  holds none: its region's row is the checkpoint); the Vulkan checkpoint rows (`rd_snap_alloc`) are `RDEC_DN_SNAPS` (4)
  a region from the first capture or save, 262 MiB a region on the 35B.
- **MEASURED (2026-10-20, `out-of-process`, `direction-grade`) - the house replay's cache rate with and without the speculative
  round, after the finished-turn fix.** M5 Max, the IQ4_XS-pure 35B, the server at `--ctx 32768 --streams 4`, the arms
  `--mtp=false` and `--mtp` under the box profile, a cold and a warm pass an arm, one
  process a pass (the numbers in that repository's ledger): before, the plain arm cached 91 % warm and the `--mtp` arm 100 %
  on the same host-cached slot - the speculating arm refused the finished-turn checkpoint on 56 of 124 streams (a stop
  accepted as a draft, its row evaled and the close evaled again: the token list one row short), and the plain arm, keeping
  every finished turn, hit the flat 256-checkpoint cap with half the 32 GB byte budget free and evicted the message-end
  stops the warm pass attaches. After (the walk ends on a stop draft, the close evals past the evaled stop row, the count
  cap the budget over the model's recurrent state - 521 on this box): both arms cold 85 %, warm 100 %, 248 of 248 finished
  turns kept on each.
- **MEASURED (2026-10-20, `debug-jit`, `direction-grade`) - the speculative round on Metal through the common seats: the
  three carriers' five rows on the M5.** Apple M5 Max, `lcpp_bench --mtp-ab -p 32 -n 64 -r 2 --ngl 99` under
  `DAS_TUNE_MANIFEST=performance/m5.tune.json`, the sampled rows at temp 0.7 top-p 0.9, every file served whole; the 27B is
  the trunk with the split head attached (`DASLLAMA_MTP_HEAD=mtp-Qwen3.8-27B-Q8_0.gguf`), the 26B the assistant drafter
  (`mtp-gemma-4-26B-A4B-it-Q8_0.gguf`, no q: its walk under the rejection knob is the typical walk, the same drafts
  accepted); `DAS_TUNE_POLICY` and `DASLLAMA_PIN_BACKEND` unset, the box profile crowned, no rig leg runs the round
  (`lcpp_bench --mtp-ab` is the instrument). The off column of a model drifts down across its five rows (the 27B 28.8 ->
  25.1) - the box warms under a long GPU stretch - so a row's on / off is the comparison, not a column. This entry
  replaces the 2026-09-03 depth ladder's M5 figures (its 27B depth 3 at 1.31 and gemma's depth 2 as the best row: here
  the 27B's depth 3 reads 1.20 and gemma's depth 3 is the best row at 1.43, the round now the common one on the seats):

    Qwen3.6-35B-A3B-MTP UD-Q4_K_M:

    | row | off tok/s | on tok/s | on / off | drafts accepted | per position |
    | :--- | ---: | ---: | ---: | ---: | :--- |
    | greedy depth 1 | 138.0 +/- 0.5 | 157.9 +/- 0.3 | 1.14 | 75.4% | 75.4 |
    | greedy depth 2 | 138.4 +/- 0.1 | 141.6 +/- 2.6 | 1.02 | 58.8% | 72.5 / 45.1 |
    | greedy depth 3 | 138.2 +/- 0.3 | 127.5 +/- 8.3 (cv 6.5%, void) | 0.92 | 47.0% | 74.2 / 42.1 / 24.9 |
    | sampled depth 2, rejection (the default) | 128.6 +/- 0.1 | 134.0 +/- 0.4 | 1.04 | 58.4% | 72.0 / 44.8 |
    | sampled depth 2, typical | 129.2 +/- 0.0 | 174.6 +/- 0.2 | 1.35 | 86.0% | 92.1 / 79.9 |

    gemma-4-26B-A4B-it Q4_K_M with the Q8_0 drafter:

    | row | off tok/s | on tok/s | on / off | drafts accepted | per position |
    | :--- | ---: | ---: | ---: | ---: | :--- |
    | greedy depth 1 | 121.8 +/- 0.0 | 153.4 +/- 0.2 | 1.26 | 84.7% | 84.7 |
    | greedy depth 2 | 121.4 +/- 0.1 | 158.2 +/- 4.5 | 1.30 | 76.5% | 80.3 / 72.7 |
    | greedy depth 3 | 121.0 +/- 0.3 | 173.0 +/- 1.6 | 1.43 | 72.4% | 80.3 / 71.3 / 65.6 |
    | sampled depth 2, rejection (the default) | 111.6 +/- 1.0 | 150.0 +/- 2.0 | 1.34 | 89.2% | 92.2 / 86.1 |
    | sampled depth 2, typical | 108.2 +/- 2.8 | 138.1 +/- 0.1 | 1.28 | 89.2% | 92.2 / 86.1 |

    Qwen3.8-27B Q4_K_M with the split Q8_0 head:

    | row | off tok/s | on tok/s | on / off | drafts accepted | per position |
    | :--- | ---: | ---: | ---: | ---: | :--- |
    | greedy depth 1 | 28.8 +/- 0.1 | 38.3 +/- 0.2 | 1.33 | 76.0% | 76.0 |
    | greedy depth 2 | 28.0 +/- 0.4 | 29.2 +/- 0.4 | 1.04 | 57.0% | 70.9 / 43.0 |
    | greedy depth 3 | 25.7 +/- 0.4 | 30.8 +/- 0.1 | 1.20 | 47.1% | 70.5 / 43.8 / 27.1 |
    | sampled depth 2, rejection (the default) | 25.4 +/- 0.4 | 28.7 +/- 0.1 | 1.13 | 58.0% | 70.0 / 46.0 |
    | sampled depth 2, typical | 25.1 +/- 0.5 | 36.2 +/- 0.1 | 1.45 | 87.0% | 91.9 / 82.2 |

  The Qwen heads' second position accepts under half, so depth 2 carries three verify rows for little on both Qwen
  carriers (the 35B's depth 3 is under plain); the gemma drafter's positions hold, and its depth 3 is the best row on the
  box. Rejection on the 35B is 1.04 against typical's 1.35 at the same prompts: the head's q cuts the accepted count from
  86% to 58%, the price of the exact distribution. The catalog's `serve_mtp` lists metal on all three
  (`performance/model_specs.das`). The house replay's three servers on this tip (`out-of-process`,
  `direction-grade`; its walls and rates in that repository's ledger) - no `--mtp`, `--mtp` under the
  rejection default, `--mtp --mtp-accept typical` - read a wash on its short replies (the same alone
  wall, TTFT and steady rate, the pair phase's spread past the arms' order) and no scored quality drop
  (silent 21 of 22 on all three, the same tool-call split); typical's replies run about a sixth longer. The served
  turn at one stream on the same file (`harness/served_bench.das --sizes 3000,9000 --reply 200 --reps 5`, `--ctx 16384
  --streams 1`, the arms run cooled and reversed, `out-of-process`, `direction-grade`): prefill 5113-5136 tok/s at 3k and
  4611-4768 at 9k on every arm, decode off 182.9 / 171.7, rejection 176.6 / 161.0 (cv 4 %, void), typical 174.7 / 163.2
  (cv 3.4-3.8 %, void) - the round under off on the served turn at the box knob's depth 1, where the bench reads a win;
  followup_general 218 carries the gap.
- **MEASURED (2026-10-20, `debug-jit`, `direction-grade`) - the selection landing on Metal: the shared `GkClsSelect`
  stamp against the host sampler's pass over a 248k row, on unified memory.** The same 35B sampled rows with the landing
  off (`--mtp-select=false`, the same command and box, a run its own process): typical 160.3 +/- 0.5 over plain 125.0 +/-
  0.1 (1.28) against the landed 174.6 (1.35); rejection 109.1 +/- 6.2 (cv 5.7%, void) over 125.2 +/- 0.2 (0.87) against
  the landed 134.0 (1.04). The first stamp, a commit earlier, lost on both rows - typical 131.3 +/- 0.7 over 121.9 +/- 12.0
  (void), rejection 110.2 +/- 0.0 over 129.5 +/- 0.7 - with a histogram that adds one atomic a lane: a row's logits cluster, the key's top bits put
  most of a row into a few bins, and a threadgroup of 1024 serializes on them; the subgroup-aggregated add (the lanes of a
  subgroup sharing lane 0's bin add once through lane 0, called uniformly so lane 0 is active whenever its subgroup is)
  recovers the win. Unified memory saves no transfer here, only the host's sampler passes over the row.
- **MEASURED (2026-10-20, `debug-jit`, `direction-grade`) - the depth-1 greedy round on Metal through the common seats
  keeps its one command buffer.** Apple M5 Max, Qwen3.5-0.8B-MTP Q8_0, `lcpp_bench --mtp-ab --mtp-depth 1 -p 32 -n 64
  -r 2 --ngl 99` under the box profile, three commits each its own process, 684/837 drafts accepted on every form: the
  retired Metal round 443.0 +/- 0.7 on over 443.1 +/- 0.1 off (1.00); the seats with the draft and the verify as two
  command buffers 422.9 +/- 1.8 (0.95 of the same run's off); the seats with the draft parked and encoded ahead of the
  verify's rows in the verify's command buffer 452.7 +/- 0.6 over 443.0 off (1.02, three reps) - the single-row draft
  beats the retired round's two-row rows-form draft. The losing forms ship under no flag: the readings are cross-commit.
  The parked verify's n-gram ring save (`g_v_ring_save`, pooled, held from the verify seat to the rollback or the next
  verify) is `ple_hist_rows x hc_dim x 4` bytes, an n-gram model's alone (Flash-Next; no box here holds it whole).
- **MEASURED (2026-10-19, `served`, `out-of-process`, `direction-grade`) - Flash-Next served without the round, ours against
  Strata on the same shards, every expert pinned on both.** zen2, `harness/served_bench.das --sizes 512,3000,9000 --reply 200
  --reps 2`; ours `dasllama-server --gpu vulkan --ctx 32768 --streams 1 --kv-dtype q8_0` at the engine's defaults (no expert
  cache budget, the idle chunk the 4096 window), Strata v0.1.45 at `--spec 2 --suffix-draft 0` (the native pack's floor,
  the MTP head and the suffix drafter off; the recipe `ARCHITECTURE_MEASUREMENT_SERVED.md#served-third-party`). Every
  decode row on both sides reads a cv over 3% (two reps a row; the tier the resident driver, `DAS_TUNE_POLICY` unset,
  the CPU backend the box's AVX2 class, coopmat cm2).

    | row | ours IQ4_XS | ours Q2_K_XL | Strata IQ4_XS |
    | :--- | ---: | ---: | ---: |
    | cold prefill 512 / 3000 / 9000, tok/s | 218 / 494 / 554 | 184 / 469 / 534 | 379 / 637 / 1268 |
    | decode at 512 / 3000 / 9000, tok/s | 22.2 / 22.6 / 21.0 | 24.4 / 23.6 / 20.8 | 39.7 / 41.1 / 40.4 |
    | warm TTFT 3000 / 9000, server-side, s | 0.64 / 0.63 | 0.80 / 0.79 | 0.67 / 0.65 |
    | cold TTFT 3000 / 9000, s | 6.1 / 16.0 | 7.4 / 16.7 | 4.7 / 7.1 |

  Decode at half of Strata's on the same shards: their device cache holds 3754 experts (8.4 GiB, per-layer slot sizes - `external`, their own log line,
  allocated across layers by a shipped routing profile, adapted at run time) against the pool's 1568 at 23 to 32 a layer,
  so their CPU share is about a third of the picks where ours is half or more, and their step is one captured graph with
  the host's sums landing through mapped words where ours submits and waits a segment a layer (`followup_vulkan.md` row
  153). Prefill: the served prompt splits at its recurrent checkpoints into three window chains where the bench runs
  one (row 154; the bench's 793 / 753 at the same window), and Strata's 8192-token chunk streams every expert once a
  chunk. The 2-bit trunk's 35% bench advantage is 10% served at best: the checkpoint windows and the turn machinery
  around the step do not shrink with the experts. Their warm turn and ours are at parity.
- **MEASURED (2026-10-19, `debug-jit`, `direction-grade`) - the host's expert sums on 64 threads against 16, Flash-Next
  UD-IQ4_XS off its head lane, every expert pinned (no cache budget), the pool 23 slots on both.** zen2 (3990X, 64 cores),
  `lcpp_bench --mtp-ab -n 64 -r 2 -t <N> --mtp-depth 1` under `DAS_JOBQUE_THREADS=<N>`, the off arm the reading: 16 threads
  21.56 +/- 0.70 (the control, taken after the 64 row; the first 16 row, at a 22-slot pool, 19.80 +/- 0.89), 64 threads
  18.50 +/- 0.06 - 14% under the matched control. The sums are DRAM-bound and 16 lanes already draw the bandwidth; the
  extra lanes pay in the per-layer joins between the token command's segments. The bench's 16-thread rule stands.
- **MEASURED (2026-10-19, `debug-jit`, `direction-grade`) - the speculative round on a fully resident Qwen3.8-27B at the
  served shape on the 16 GB card: an IQ2_XXS mixture with the NextN layer in the file, the context asked whole at 32768.**
  zen2, `lcpp_bench --mtp-ab -n 64 -r 2 -t 16` under `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_GPU_CTX_STRICT=1
  DASLLAMA_GPU_CTX_MAX=32768`, the sampled rows at temp 0.7 top-p 0.9. The file is `Qwen3.8-27B-IQ2_XXS-mix.gguf` (8.9 GB):
  `llama-quantize --allow-requantize --imatrix <unsloth's> --tensor-type 'blk\.64\.=Q8_0' <UD-Q2_K_XL> <out> IQ2_XXS` - unsloth's
  own 2-bit files (UD-IQ2_XXS 7.3 GB, UD-Q2_K_XL 9.8 GB) carry IQ1_S and IQ1_M tensors (122 and 21) the reader has no arm
  for, and unsloth's imatrix has no entries for the NextN layer, so the recipe pins the head's tensors to their Q8_0. The
  dense mirror costs about 67 KB a position (the UD-Q3_K_XL at the unasked context asked 30.4 GB of 13.0: declined to the
  per-op rails, where `--mtp-ab` ran the host verify at 4.7 tok/s against 15.6 plain). The head in the arena (441 MB, a
  128 MB K/V slot a region), the draft and the verify on the device:

    | row | off tok/s | on tok/s | on / off | drafts accepted | per position |
    | :--- | ---: | ---: | ---: | ---: | :--- |
    | greedy depth 1 | 38.8 | 52.6 +/- 0.0 | 1.36 | 74.3% | 74.3 |
    | greedy depth 2 | 38.8 | 53.9 +/- 0.1 | 1.39 | 62.6% | 75.3 / 49.8 |
    | greedy depth 3 | 38.8 | 48.2 +/- 0.0 | 1.24 | 48.4% | 73.7 / 45.5 / 25.8 |
    | sampled depth 2, rejection (the default) | 37.8 | 49.3 +/- 0.1 | 1.30 | 58.4% | 71.1 / 45.6 |
    | sampled depth 2, typical | 38.0 | 66.0 +/- 0.2 | 1.74 | 92.8% | 97.2 / 88.4 |

  A 2-bit trunk's rate and acceptance stand for the mechanics alone: the whole-card round at a 32k context on this card,
  every seat on the device, with no host sums in the step.

  The served quantization on a card that holds it (RunPod RTX PRO 4500 Blackwell 32 GB, the same flags and commit):
  `Qwen3.8-27B-Q4_K_M.gguf` (16.5 GB) with the split head `mtp-Qwen3.8-27B-Q8_0.gguf` (`DASLLAMA_MTP_HEAD`), whole in the
  arena, the head's 441 MB and a 128 MB K/V slot a region beside it:

    | row | off tok/s | on tok/s | on / off | drafts accepted | per position |
    | :--- | ---: | ---: | ---: | ---: | :--- |
    | greedy depth 1 | 39.0 | 60.3 +/- 0.0 | 1.55 | 76.1% | 76.1 |
    | greedy depth 2 | 39.0 | 67.1 +/- 0.1 | 1.72 | 60.8% | 75.0 / 46.6 |
    | greedy depth 3 | 38.9 | 67.1 +/- 0.0 | 1.73 | 49.4% | 74.2 / 45.0 / 29.2 |
    | sampled depth 2, rejection (the default) | 38.1 | 66.2 +/- 0.0 | 1.74 | 66.2% | 79.3 / 53.2 |
    | sampled depth 2, typical | 38.1 | 80.8 +/- 0.0 | 2.12 | 89.6% | 95.2 / 84.0 |

  The dense trunk's round pays as the mixture's does not: every verify row rides the whole 16.5 GB weight stream once
  (the off arm sits at the card's bandwidth, 39 tok/s), so a second and third row cost a fraction of a step and the
  acceptance is the whole of the win - 1.7x at depth 2 against the 35B MoE's 1.2x on the same card, where a row's routed
  experts stream per row. Depth 2 is the point; depth 3 ties it.
- **Long-lived memory the MTP arc adds, bytes.** `Session.mtp_q` / `mtp_qn`: `MTP_Q_CAP (1024) x 16 B` a draft row, grown
  to the round's depth - 16 KB a row, 112 KB a session at the seven-row cap. The selection landing's planes on the Vulkan
  driver: a row's header 16 B, 2048 survivor ids and logits 16 KB, 128 recent logits 512 B - 16.5 KB a row, 66 KB at the
  four-row driver and 115 KB at seven, the device planes and the packed host landing each. The expert heat profile and
  the stream-layer table take the NextN head's layer beside the trunk's: `(n_layers + n_layer_nextn) x n_expert x 8 B` -
  50 KB on the 35B (49 x 128), 200 KB on Flash-Next (49 x 512). The pool fit's floor change moves no bytes of its own:
  the slot count the fit yields changes, and the pool's peak footprint stays its slots' planes (the gemma-4 host-form
  entry below: 31 slots at 174 MB a slot).
- **MEASURED (2026-10-19, `debug-jit`, `direction-grade`) - gemma-4-26B-A4B's assistant drafter on the Vulkan resident
  driver on the 16 GB card: the UD-IQ3_XXS trunk whole at a 2048 context with a 512-row prompt window.** zen2, `lcpp_bench
  --mtp-ab -n 64 -r 2 -t 16` under `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_GPU_CTX_STRICT=1 DASLLAMA_GPU_CTX_MAX=2048
  DASLLAMA_GPU_PF_ROWS=512`, the drafter `mtp-gemma-4-26B-A4B-it-Q8_0.gguf` (the published head file) beside the trunk, the sampled rows at
  temp 0.7 top-p 0.9. The drafter attaches only to a trunk the driver serves whole with the N-row command admitted: at the
  bench's whole-context ask every routed layer goes to the host, at 4096 and at 2048 with the wide window four of 30 still
  do (the 11.4 GB file against about 13 GB usable), and the 512-row window's scratch (275 MB against 1325) is what brings it
  whole - a 16 GB card's serving shape for this file is a short context. The pod's 32 GB row (the 2026-10-16 entry: the
  Q4_K_M trunk, 131 -> 193, 1.47x at 88%) is the whole-card reference.

    | row | off tok/s | on tok/s | on / off | drafts accepted | per position | draft / verify+commit a round |
    | :--- | ---: | ---: | ---: | ---: | :--- | :--- |
    | greedy depth 1 | 98.5 | 133.6 +/- 0.1 | 1.36 | 85.3% | 85.3 | 1.6 / 12.3 ms |
    | greedy depth 2 | 98.6 | 147.1 +/- 0.2 | 1.49 | 75.4% | 79.7 / 71.0 | 2.9 / 14.2 |
    | greedy depth 3 | 98.6 | 151.0 +/- 0.2 | 1.53 | 69.0% | 79.3 / 69.8 / 58.0 | 4.2 / 16.2 |
    | sampled depth 2, typical | 93.5 | 125.1 +/- 0.4 | 1.34 | 86.6% | 89.5 / 83.8 | 2.9 / 18.9 |
    | sampled depth 2, rejection (the default) | 92.5 | 124.3 +/- 0.0 | 1.34 | 86.6% | 89.5 / 83.8 | 2.9 / 19.0 |

  Depth 3 still pays on this drafter (58% at the third position against the 35B head's 34%): the drafter's four Q-only
  layers draft a row in about a millisecond, so the round is the verify. Where the drafter attaches on this card is the
  file's size: the 9.9 GB UD-IQ2_XXS trunk sits whole with the wide window and the drafter at 8192 and 16384 (depth 2:
  109.9 -> 166.3 and 109.7 -> 166.4, 1.51x at 80.8%), and at 32768 the plan moves 3 of 30 routed layers to the host and
  the drafter refuses - a 2-bit trunk's rate and acceptance say nothing about a served model, so the boundary is the
  reading: no gemma MoE file serves the drafter at 32k on a 16 GB card. The rejection row's first run died with an index
  past `mtp_qn`: the drafter's device picks record no q, and the rejection walk read a record the session never grew -
  `mtp_walk_by_accept` walks typical for a round with no q records now (`ARCHITECTURE_GPU_MTP_ACCEPT.md#mtp-accept-modes`).

  The served quantization on a card that holds it with the drafter at the served context (RunPod RTX PRO 4500 Blackwell 32 GB,
  `gemma-4-26B-A4B-it-Q4_K_M.gguf` 17 GB whole, the drafter beside it, the context asked whole at 32768, the same flags and
  commit; the drafter's rounds draft on the device by argmax, so the rejection row walks typical and reads as it):

    | row | off tok/s | on tok/s | on / off | drafts accepted | per position |
    | :--- | ---: | ---: | ---: | ---: | :--- |
    | greedy depth 1 | 147.3 +/- 0.1 | 212.4 +/- 0.1 | 1.44 | 82.9% | 82.9 |
    | greedy depth 2 | 147.2 | 247.5 +/- 0.2 | 1.68 | 76.5% | 81.6 / 71.4 |
    | greedy depth 3 | 147.2 | 272.5 +/- 0.3 | 1.85 | 72.2% | 79.9 / 72.6 / 64.0 |
    | sampled depth 2, rejection (the default) | 135.4 +/- 0.2 | 183.5 +/- 0.1 | 1.36 | 84.1% | 86.7 / 81.5 |
    | sampled depth 2, typical | 134.8 | 183.1 +/- 0.2 | 1.36 | 84.1% | 86.7 / 81.5 |

  The UD-IQ3_XXS trunk whole on the same card at the same context (the 16 GB card's quant above, there at 2048): off
  162.6 / 162.6 / 162.3 and on 223.4 / 249.4 / 260.4 at depths 1 to 3 (1.37 / 1.53 / 1.60x, 84.6 / 74.8 / 69.8% accepted),
  the sampled rows 148.4 -> 185.6 under rejection and 148.3 -> 184.3 under typical (1.25x, 82.0%, the same walk).

  The drafter's acceptance holds with depth (64% at the third position), so depth 3 is this carrier's point where the NextN
  heads' is 2: an assistant drafter trained for the task against a head that shares the trunk's hidden. The 26B's 4B active
  parameters decode at 147 whole against the 35B's 193 on the same card - the sliding and global attention layers' K/V
  rows and the 262k vocabulary's classifier are the difference, not the experts.
- **MEASURED (2026-10-19, `debug-jit`, `direction-grade`) - the speculative round on Qwen3.6-35B-A3B-MTP IQ4_XS-pure at the
  served shape: the context asked whole at 32768.** zen2, `lcpp_bench --mtp-ab -n 64 -r 2 -t 16` under `DASLLAMA_GPU=1
  DASLLAMA_IMAGE=0 DASLLAMA_GPU_CTX_STRICT=1 DASLLAMA_GPU_CTX_MAX=32768`, the planar load (the file's lane was baked under the
  unasked plan and its baked gathers diverge at this one - the lane's identity folds no context pin, said in the decline),
  the sampled rows at temp 0.7 top-p 0.9. The plan moves the first 18 of 41 routed layers' experts to the host (the unasked
  plan's 34, with its shrunken mirror); the NextN head stays in the arena (438 MB of planes, a 64 MB K/V slot a region), so
  the draft runs on the device and the verify's rows run the split command's segments:

    | row | off tok/s | on tok/s | on / off | drafts accepted | per position |
    | :--- | ---: | ---: | ---: | ---: | :--- |
    | greedy depth 1 | 74.0 | 90.0 +/- 0.1 | 1.22 | 77.6% | 77.6 |
    | greedy depth 2 | 74.1 | 88.3 +/- 1.0 | 1.19 | 64.1% | 75.3 / 53.0 |
    | greedy depth 3 | 75.0 | 85.0 +/- 1.9 | 1.13 | 52.6% | 71.9 / 51.3 / 34.5 |
    | sampled depth 2, rejection (the default) | 69.6 | 79.4 +/- 2.4 | 1.14 | 61.9% | 76.8 / 47.0 |
    | sampled depth 2, typical | 70.6 | 97.8 +/- 0.9 | 1.38 | 86.1% | 94.2 / 78.0 |

  The off arm sits 12 tok/s over the unasked plan's (74 against 62): fewer layers on the host. Depth 1 is this plan's
  point where the unasked plan's was depth 2 - the second row's host sums cost more here than the weight stream it rides
  saves, with 23 layers' experts already on the device.

  The same table on a card that holds the file whole (RunPod RTX PRO 4500 Blackwell 32 GB, 62 GB RAM, the same flags and
  the same commit; every routed layer resident, no host sums, the pool unneeded):

    | row | off tok/s | on tok/s | on / off | drafts accepted | per position |
    | :--- | ---: | ---: | ---: | ---: | :--- |
    | greedy depth 1 | 192.3 +/- 3.3 | 236.7 +/- 0.6 | 1.23 | 75.5% | 75.5 |
    | greedy depth 2 | 194.6 | 237.4 +/- 0.2 | 1.22 | 63.3% | 73.4 / 53.3 |
    | greedy depth 3 | 194.5 | 221.2 +/- 0.1 | 1.14 | 52.8% | 75.5 / 50.5 / 32.4 |
    | sampled depth 2, rejection (the default) | 174.0 +/- 2.0 | 190.9 +/- 0.4 | 1.10 | 61.7% | 75.7 / 47.8 |
    | sampled depth 2, typical | 174.8 +/- 1.0 | 243.8 +/- 0.3 | 1.39 | 86.1% | 94.2 / 77.9 |

  Whole on the card the round's shape is the 16 GB card's: depths 1 and 2 tie, depth 3 loses, and the sampled rows read
  the same acceptances to the point - the win is the verify rows' weight stream shared across rows, which the host form
  pays for in host sums and the whole form in nothing. Rejection at 1.10 against typical's 1.39 is the price of the
  lossless walk: a quarter of the drafts the sampler would admit fail the p/q test.
- **MEASURED (2026-10-19, `debug-jit`, `direction-grade`) - gemma-4-26B-A4B on the whole-model driver's host form: the pool
  the plan refused, and the resident q8 expert layers' wait.** zen2, `lcpp_bench -p 64 -n 32..64 -r 2 -t 16` under
  `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_GPU_CTX_STRICT=1 DASLLAMA_GPU_CTX_MAX=32768` (the server's shape: the context
  asked whole), the frozen fixture 8/8 on every run:

    | file | host layers | pool | tg | token step (host wall, ms) |
    | :--- | ---: | ---: | ---: | :--- |
    | Q8_0, the plan's pick | 22 of 30 | none (439 MB left, 132 MB a slot) | 8.1 | 122 = wait 95 + host experts 19 + segments 8 |
    | Q8_0, `DASLLAMA_GPU_HOST_LAYERS=29`, before the fix | 29 | none (5839 MB left, 174 a slot) | 28.0 | 35 = host experts 24 + segments 8 + wait 2.5 |
    | Q8_0, 29 host layers, after the fix | 29 | 31 slots, 81% hits | 51.7 | 19 = segments 10 + host experts 6.6 + wait 2.5 |
    | Q8_0, every routed layer on the host | 30 | pool, 64-84% hits | 52.0 | 19 |
    | UD-Q4_K_M, the plan's pick | 16 of 30 | none | 49.1 | 20 = host experts 9 + wait 6.4 + segments 4.5 |
    | UD-IQ4_XS, the plan's pick | 13 of 30 | none | 27.2 | 36 = wait 26 + host experts 6.2 + segments 3.7 |

  Two things. **The pool fit refused a pool with 5.8 GB of room** (the 29-host-layer row): the K/V floor it measured the
  yield against was the widest layer's row over every position and region - 8 GB on gemma-4, whose sliding layers keep
  rings, where the mirror at that context is 1.6 GB - so the room beside the floor read negative, and the auto arm, with
  31 slots fitting beside the whole mirror but under the 32 the yield aims for, took the yield's zero over the 31. The
  floor is the mirror's own bytes at the floor context now and the yield only adds slots (`resident_hot_slots`,
  `resident_plan`'s floor); the gemma-4 pool gate (`!moe_dense_shexp`) is gone too, since the fold precedes the pool's
  split and the resumed segment accumulates the hits onto the host's sum before the gemma-4 combine. **The 95 ms `wait` was
  the ReBAR-typed planes parked in system memory** (`ARCHITECTURE_GPU_VULKAN_ROOM.md#vk-plan-room`): the demotion tell
  read 4262 MB of this process's planes in system memory on every Q8_0 plan-pick row and 4058 on the IQ4_XS row, none on
  the UD-Q4_K_M row or at a 12 GB room, and the wait followed the demoted bytes, not the resident layer count. Not the
  transfer queue (`DASLLAMA_VK_XFERQ=0` the same 95), not the priority tag (`DASLLAMA_VK_MEMPRIO` was off), not the
  profiler (off, 8.10). `DASLLAMA_VK_REBAR=0` at the plan's own room: tg 32.7, the wait 5 ms, pp64 243 against 141; the
  same on Qwen-AgentWorld-35B-A3B UD-IQ3_XXS at the strict 32768 (12 host layers, 4077 MB demoted): 38.4 -> 71.1, pp64
  150 -> 473. The staged, unmapped device-local plane is the default now; the load's "direct" upload figure (9.0 GB in
  0.9 s against 1.3 s staged) was the shadow's fill rate. Where nothing demoted the arms read the same: Qwen3.6-35B-A3B-MTP
  UD-Q4_K_M under its lane at the default context (39 host layers, pool 34) tg 53.4 mapped against 54.1 staged, pp64 264
  against 283, the token step 18.6 against 18.1 ms. The one thing the staged rail pays is the pool's host placements at a
  window's end (`vk_rdec_hot_upload`, a staging round trip an expert): 2340 experts 231 ms
  mapped, 995 staged - 0.4 ms an expert; on Flash-Next's window, hundreds of experts a layer, that was the prompt: pp512
  415 mapped against 117 staged on the same 25-slot plan (tg128 29.6 against 29.8). The placement loops bracket their
  uploads in one staging batch now (`rdec_hot_batch`: one submit, one fence a layer): Flash-Next pp512 419 staged and
  batched (tg128 30.1), the Qwen3.6-35B placements' upload 117 ms for 2350 experts against the mapped rail's 231 and
  the unbatched staged 995, pp64 302; the DMA fill that lands a step's misses never touched the mapped type.
- **MEASURED (2026-10-19, `debug-jit`, `direction-grade`) - the prompt window at 4096 rows on Qwen3.8-Flash-Next, every
  expert resident.** zen2, `lcpp_bench -p <N> -n 8 -r 3 -t 16` under `DASLLAMA_GPU=1 DASLLAMA_GPU_PF_ROWS=<rows>
  DASLLAMA_ALLOW_UNTUNED=1`, `DN_WINDOW` at 4096 beside the knob (a recurrent window is one deltanet chunk), the pool and
  the split at their defaults, the frozen fixture's gate 8/8 on every run:

    | window | pp3000 | pp9000 | scratch | pool | K/V positions | tg8 |
    | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
    | 2048 | 368 +/- 152 | 663 +/- 5 | 2417 MB | 32 slots | 67669 | 22.3 |
    | 4096 | 793 +/- 2 | 753 +/- 5 | 3962 MB | 27 slots | 33670 | 21.8 |

  A window streams the routed layers' whole expert stacks over the link once (64 GB, about 4.6 s at 14 GB/s), so the
  sweep count is the prompt's rate: a 3k prompt in one window instead of two doubles and loses its noise, a 9k prompt in
  three instead of five gains 14%; the decode step does not move. The default becomes the wide window where the plan fits
  with it (`resident_window_pick`), the narrow one otherwise; 8192 is held by the scan kernel's 48 KB staging and 7 GB of
  window scratch on this card (`followup_vulkan.md` row 151). The 2048 row's noise is the pool's window prefix under the DMA
  knob on a 3k prompt's short second window: pp3000 at 2048 with the pool off and whole layers 264 +/- 0.4, the pool off
  476 +/- 1.6, the DMA fill off 657 +/- 2.2 (the 10/17 record's arm), the split off 261 +/- 3; at 4096 the fill costs the
  prompt nothing (787 / 760 off against 793 / 753 on) and keeps its decode win (tg8 19.7-19.9 off against 21.3-21.8 on).
  A 512-token prompt is one window at either size: at the pick, every expert resident, pp512 411 +/- 42 with the fill and
  425 +/- 3 without over five reps - the fill's prefix is the short window's noise, not its mean (`followup_vulkan.md` row 150
  keeps the short prompt's question); the same row read 158-189 +/- 100 earlier the same day with the shards' pages cold. Strata v0.1.45 on the same shards and card reads 629 and 1181
  at two 8192 chunks (the served rows below). The served path on Vulkan ran its prompts in 512-token chunks until this day
  (`chunk_defaults`: the idle chunk 2048 on Metal alone) - a 3k prompt six sweeps - and the idle chunk on a GPU is now the wider of 2048 and the resident driver's prompt window (4096 where the plan fits it).
- **MEASURED (2026-10-19, `debug-jit`, `direction-grade`) - the speculative round on Qwen3.8-Flash-Next through the lane
  under the expert cache, the draft and the verify on the device, with the Q8_0 head and with a 2-bit head.** zen2 (Ryzen
  3990X 16 threads, RTX 5060 Ti 16 GB), the same `lcpp_bench --mtp-ab -n 64 -r 2 -t 16` rows as the 35B's below, under
  `DASLLAMA_GPU=1 DASLLAMA_MTP_HEAD=<head> DASLLAMA_EXPERT_CACHE_MB=32768 DAS_JOBQUE_THREADS=16 DASLLAMA_ALLOW_UNTUNED=1`:
  the trunk UD-IQ4_XS off its `-mtp-<head>` lane (71 GB of planes, minted in 329 s for the Q8_0 head and 560 s for the
  2-bit one), the 49 routed layers' experts on the host behind a 32-slot pool, 240 of 512 experts a layer resident in the
  32 GB cache by the lane's heat profile (a fresh lane's cold profile - the first ids - serves both arms at 8.6 tok/s;
  a seeded one 14.4-15.5), the NextN head's experts a streamed stack like the trunk's and the draft wholly on the device.
  The sampled rows at temp 0.7, top-p 0.9; the on arm's rows through the hyper-connection verify rail.
  - the Q8_0 head (`mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf`):

    | row | off tok/s | on tok/s | on / off | drafts accepted | per position |
    | :--- | ---: | ---: | ---: | ---: | :--- |
    | depth 1 greedy | 14.93 +/- 0.38 | 16.55 +/- 0.94 | 1.11 | 69.2% | 69 |
    | depth 2 greedy | 15.39 +/- 0.20 | 15.22 +/- 0.09 | 0.99 | 50.4% | 63.7 / 37.1 |
    | depth 3 greedy | 15.34 +/- 0.43 | 13.27 +/- 0.58 | 0.87 | 37.7% | 61.2 / 32.8 / 19.0 |
    | depth 2 exact | 14.49 +/- 0.03 | 14.31 +/- 0.37 | 0.99 | 49.0% | 62.0 / 35.9 |
    | depth 2 typical | 15.46 +/- 0.44 | 16.88 +/- 0.24 | 1.09 | 60.3% | 72.1 / 48.4 |
    | depth 2 rejection | 15.50 +/- 0.78 | 14.58 +/- 0.46 | 0.94 | 49.2% | 61.7 / 36.8 |

  - the 2-bit head (`mtp-Qwen3.8-Flash-Next-shared-Q8_0-exps-2bit.gguf`: the head's expert gate and up planes Q2_K, the
    down IQ4_NL since its 640 columns are not a 256-multiple, no imatrix - the IQ2 formats refuse without one), 1.1 GB
    against the Q8_0 head's 2.6 GB:

    | row | off tok/s | on tok/s | on / off | drafts accepted | per position |
    | :--- | ---: | ---: | ---: | ---: | :--- |
    | depth 1 greedy | 14.53 +/- 1.66 | 13.52 +/- 0.68 | 0.93 | 41.2% | 41 |
    | depth 2 greedy | 14.35 +/- 0.07 | 10.45 +/- 0.16 | 0.73 | 24.4% | 38.4 / 10.4 |
    | depth 3 greedy | 14.57 +/- 0.51 | 9.47 +/- 0.47 | 0.65 | 17.3% | 40.3 / 9.7 / 1.9 |
    | depth 2 exact | 15.02 +/- 1.04 | 11.06 +/- 0.20 | 0.74 | 22.7% | 36.3 / 9.2 |
    | depth 2 typical | 15.16 +/- 0.80 | 11.64 +/- 0.63 | 0.77 | 30.8% | 48.0 / 13.6 |
    | depth 2 rejection | 15.29 +/- 0.95 | 11.17 +/- 0.27 | 0.73 | 25.9% | 41.7 / 10.2 |

  The shape, under `DASLLAMA_GPU_PROF=1` at depth 1 with the Q8_0 head (host wall a step, microseconds): the token step
  65,700 = segments 20,700 + host experts 43,500 + wait 1,400; the draft 4,200, all of it the submit and the wait of a
  device command; the two-row verify 93,000 = segments 22,900 + host experts 68,500 + wait 1,600. The host's expert sums
  are the step on this box - the trunk's 10 picks a layer over 48 layers read about 1.25 GB a token off DRAM - and two
  positions share few experts, so the verify's second row costs 0.6 of a token where the 35B's rode the stream the first
  row paid for; with 69% of first drafts accepted the round yields 1.69 tokens for 1.48 token-steps, the 1.11 measured.
  Depth 2 and 3 pay a third and fourth row's host sums for 37% and 19% acceptance and lose; typical acceptance keeps
  depth 2 at 1.09 by accepting more. The 2-bit head drafts too poorly to pay even its 4 ms: 41% first-position acceptance
  against 69%, 10% at the second, and every row loses - the draft was never the cost here, so a lighter head cannot win
  on this shape; it would pay where the head's experts were summed on the host, which the streamed stack ended. Not
  publishable rows: the box's sidecar is stale (`DASLLAMA_ALLOW_UNTUNED=1`).

  The off arm here is the tg-real row - real prompts behind a 512-token context, the sample inside the timed loop - and
  it is not the tg128 bench row the Flash-Next rows above were taken on. The same commit's tg128 (`--tg 128`,
  five reps, synthetic ids from position 0, no logit read): 30.92 +/- 0.43 with no head and no cache, 30.70 +/- 0.50 with
  the head attached, 27.20 +/- 0.83 under the 32 GB cache - the record's 30.2 stands and the head costs the token step
  nothing. The gap is the host's expert sums: 9.6 ms a token on the synthetic row against 43.5 on real text, at pool hit
  rates alike (24-60% a layer on both), since the synthetic ids route each step to the same few experts and their rows
  stay in the 3990X's 256 MB L3 where real text's spread reads DRAM. The 35B's two rows sat 8% apart; on Flash-Next the
  synthetic row says twice what the served shape delivers, and the tg-real row is the one to read.

  The pool's two fills against the host's sums, on the real row (the off arm under the profiler, host wall a token step):
  the DMA fill (`DASLLAMA_GPU_HOT_DMA`, on by default) 67.8 ms a token with the pool serving 50.4% of the picks and the
  host experts 45.0, against 69.7 / 46.0% / 46.6 under `=0` - a win of 2 ms a token, since a 32-slot pool over 512
  experts follows real routing no better whatever fills it. The decode meet (`DASLLAMA_GPU_DEC_MEET=1`, a token's picks
  as a one-row window over the pool, the device taking the hits and half the misses by DMA, the host the other half)
  loses on real text as it did on the synthetic row: 8.14 +/- 0.57 tok/s against 13.42 +/- 0.30, the token step 116 ms
  against 69.6, since a layer's meet costs 2.5-4.3 ms of window machinery (`rdec_meet l48: device 6 experts, host 4, 3500
  us to the meeting`) where the host sums the whole layer's picks in about 1 ms. The split itself is sound on this box:
  the host's sum is 0.18 ms an expert (46.6 ms over about 260 misses a token), the transfer queue's fill 0.19 ms an
  expert (2.6 MB at 14 GB/s), so a token step whose misses were filled by DMA inside the recorded command's segments
  and summed by the pool's kernel in the SAME step, half of them, would cut the host's piece near 24 ms and the token
  step near 47 - a 1.5x at IQ4_XS - with no window chain in the loop. **LEDGERED lever: the same-step DMA share of a
  decode token's misses at a host/device ratio set by the two rates** (the DMA fill lands the copies today and the next
  token reads them; the step that filled them still sums them on the host).

  **MEASURED (2026-10-19) - the lever above, built and refuted on the same row.** The same-step sum was already the
  pool's shape: a miss the DMA fill admits reads as a hit before the step's split, and the chain waited the batch. What
  held it to six experts a token was the heat gate on admission, so the lever became `DASLLAMA_GPU_HOT_SHARE` - a share of
  the gate's leftovers admitted ungated, the warmest first into the coldest unpicked slots - and lost at every setting,
  under either landing (the profiled off arm, one profiled run a row and no spread, host wall a token step):

    | landing | share | device picks | token step | segments | host experts |
    | :--- | ---: | ---: | ---: | ---: | ---: |
    | same step (chain waits the batch) | gate alone | 50% | 67.8 | 21.4 | 45.0 |
    | same step | 0.3 | 80-84% | 77.1 | 32.8 | 42.4 |
    | same step | 0.5 | 85-89% | 78.9 | 37.4 | 39.4 |
    | same step | 0.7 | 88-93% | 86.0 | 44.2 | 39.5 |
    | next step (the chain waits nothing) | gate alone | 42% | 68.2 | 21.1 | 45.6 |
    | next step | 0.5 | 50% | 91.3 | 28.4 | 61.2 |
    | next step | 1.0 | 58% | 91.5 | 32.8 | 56.7 |

  The share is the pool's working-set policy: the picks of a layer the slots serve without a fill rose from 49% under the
  gate alone to 61-63% at shares 0.5-1.0 on the next-step landing (the same runs).

  Three things the rate arithmetic missed. The host's step carries a per-layer floor - the feed requant, the team dispatch,
  the reduce - that fewer misses do not lower: at 11% of the picks left to it the host piece was still 39 ms. The copies
  sat on the chain's critical path under the same-step landing, and under the deferred one the next segment's wait and the
  host's own sums met on the same DRAM: a fill reads the expert's 2.6 MB off the pinned mirror exactly as a sum reads it,
  so a fill pays only where the expert is hit again more than once before it leaves, and at 130-140 fills a token the
  marginal fill bought 0.57 hits (hits 42% to 58% of picks for 6 to 143 fills a token). The gate's churn rose with the
  share's too - the coldest slot's heat falls as cold experts enter, more misses pass the gate, and the ones the tier
  cannot copy take the host's gather and upload at 1.5 ms each, 20 ms a token at share 0.5. The admission policy is the
  working-set lever (the frequency-filtered gate, a TinyLFU shape, against the share's insert-on-miss with LFU-aging
  eviction, and an LRU neither of them is), and `harness/hot_pool_sim.das` on a `DASLLAMA_MOE_TRACE` of real text ranks
  them by hit rate without the box; the live row says a DRAM-bound host wants the selective one. The knob ships at 0; the
  deferred landing stays (the gate alone at 68.2 against 67.8, and no command reads a slot a DMA still writes).

  - the UD-Q2_K_XL trunk (unsloth's, 78.9 GB in three shards at `C:/models/Qwen3.8-Flash-Next-GGUF/UD-Q2_K_XL`, 16%
    under the IQ4_XS file: its experts 49.5 GB against 64.4, so the 32 GB cache keeps 329 of 512 a layer resident
    against 240; the lane 52.6 GB, minted on the C: NVMe in 99 s), the same Q8_0 head, the same rows:

    | row | off tok/s | on tok/s | on / off | drafts accepted | per position |
    | :--- | ---: | ---: | ---: | ---: | :--- |
    | depth 1 greedy | 19.52 +/- 0.49 | 20.87 +/- 0.32 | 1.07 | 70.2% | 70 |
    | depth 2 greedy | 20.43 +/- 0.41 | 19.96 +/- 0.51 | 0.98 | 54.4% | 66.0 / 42.9 |
    | depth 3 greedy | 18.19 +/- 1.91 | 16.96 +/- 0.63 | 0.93 | 40.3% | 63.3 / 37.1 / 20.5 |
    | depth 2 typical | 20.04 +/- 1.04 | 22.44 +/- 1.11 | 1.12 | 66.4% | 76.4 / 56.5 |
    | depth 2 rejection | 18.84 +/- 0.75 | 17.92 +/- 0.81 | 0.95 | 55.2% | 69.2 / 41.2 |
    | depth 1 rejection | 19.32 +/- 0.05 | 19.74 +/- 1.04 | 1.02 | 65.2% | 65.2 |

    The IQ4_XS trunk's depth 1 rejection row, taken with a lint compile beside it (the off arm 11.46 +/- 1.56 against its
    usual 15): 12.75 +/- 0.34 on, 1.11, 66.7% accepted. The sampled round at the NextN depth is neutral to a small win on
    the host form, as the greedy one is, so the server's auto-arm (`mtp_auto_arms`) arms a one-stream slot whose routed
    experts the resident driver sums on the host like any other whole-model slot; the box's NextN depth stays 1, where every
    host-form row measured best (the 35B at 18 host layers 1.22x, these two trunks 1.07-1.11x).

    The off arm 35% over IQ4_XS: the token step 47.6 ms = segments 19.3 + host experts 27.2 + wait 1.1 (against 65.7 =
    20.7 + 43.5 + 1.4), the misses fewer and each a smaller read; the draft 3.0 ms, the two-row verify 70.4 = 20.2 + 48.9
    + 1.2. The round's shape holds: the second row costs 0.5 of a token on the host, and the depths' standings are the
    IQ4_XS ones. The repo carries unsloth's imatrix (580 MB), the one an IQ2 head needed.
- **MEASURED (2026-10-19, `debug-jit`, `out-of-process`, `direction-grade`, parity record) - the hyper-connection chain's token step on Qwen3.8-Flash-Next
  against the CPU chain, through files.** zen2, `harness/parity_dump.das` (a dump compare between two processes: a parity
  reading in the sense of `ARCHITECTURE_GPU_PARITY.md` comes from `harness/parity.das`, `lcpp_bench --parity` or a
  `tests/run.das` cell; this record is the carrier's only form, since no box holds both arms in one process): the device arm (the resident driver, every
  routed layer's experts on the host, the hot pool at 32 slots, ctx 61810) dumps a 94-token prompt's row and six fed
  steps' rows with the 48 routed layers' picks a position (48,000 picks) under `DASLLAMA_GPU=1`; the CPU arm loads the
  three shards planar in 59 s (meta 10, weights 24, repack 8) in its own process, replays the tape (4,800 selects, 152 of
  48,000 picks its own top-k would have taken differently) and dumps; the compare at the regions files' 0.20 bar:

    | row | maxdiff | bar | one-step-off control | argmax |
    | :--- | ---: | ---: | ---: | :--- |
    | prefill (94 tokens) | 0.42 | 4.54 | 13.0 | 17 / 17 |
    | step 0 (fed 17) | 1.28 | 4.84 | 13.4 | 20 / 20 |
    | step 1 (fed 20) | 0.33 | 4.80 | 17.4 | 11 / 11 |
    | step 2 (fed 11) | 0.41 | 4.62 | 19.3 | 220 / 220 |
    | step 3 (fed 220) | 0.23 | 4.85 | 12.0 | 17 / 17 |
    | step 4 (fed 17) | 0.53 | 4.91 | 15.2 | 21 / 21 |
    | step 5 (fed 21) | 0.42 | 4.85 | 19.8 | 11 / 11 |

  Every row inside the tight bar, under the in-process hc cells' wide bar of 0.35 by a factor of 3 to 20. The same run
  on Qwen3.6-35B-A3B (IQ4_XS-pure, `DASLLAMA_GPU_HOST_LAYERS=40` so the tape covers the 40 routed layers): prefill 4.0
  against 5.3, steps 0.44-0.92 against bars near 5, controls 13-21, 136 of 32,000 picks pinned. Each arm alone fits the
  box - the device arm about 137 GB with the lane mapped, the CPU arm under 190 GB with the shards' page cache evictable -
  where both arms in one process did not; the dumps are kept beside the model
  (`Qwen3.8-Flash-Next-GGUF/UD-IQ4_XS/parity/token_step_p94_f6.{device,cpu}.pd`, 7.3 MB each), the CPU one the reference
  every device run compares against from here.
- **MEASURED (2026-10-18, `debug-jit`, `direction-grade`) - the k-deep speculative round on the Vulkan seats, and the
  plain chain's verify rows through the split command's segments on a MoE the card does not hold whole.** zen2 (Ryzen
  3990X 16 threads, RTX 5060 Ti 16 GB on PCIe 4.0 x8, 1.3-2.4 GB held by the desktop), `bin/Release/daslang.exe -jit
  modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m <file> --mtp-ab -n 64 -r 2 -t 16 --mtp-depth <k>
  --for-debug-purposes` under `DASLLAMA_GPU=1 DASLLAMA_ALLOW_UNTUNED=1` (the box's sidecar is stale: the kernels run
  the class profile's stamps), eight real-text prompts, two reps, one process a row, the off and on arms in one process.
  Where every round on the Vulkan driver was depth 1 (the CPU round seated the draft and the verify and drafted one
  token), `mtp_spec_round_k` drafts k through the head's own hidden and verifies k + 1 rows on the seat
  (`ARCHITECTURE_GPU_MTP.md#mtp-round-one-join`).
  - Qwen3.5-0.8B-MTP Q8_0 (whole on the card), tg-real64 off 310-329 tok/s across the rows:

    | depth | on tok/s | on / off | drafts accepted | per position |
    | ---: | ---: | ---: | ---: | :--- |
    | 1 | 387.1 +/- 0.7 | 1.25 | 82.3% | 82 |
    | 2 | 414.8 +/- 3.7 | 1.26 | 70.6% | 76.4 / 64.8 |
    | 3 | 411.0 +/- 0.4 | 1.28 | 64.8% | 77.0 / 64.6 / 52.8 |
    | 4 | 371.1 +/- 0.3 | 1.15 | 55.1% | 74.1 / 59.6 / 47.6 / 39.2 |

  - Qwen3.6-35B-A3B-MTP-IQ4_XS-pure (19 GB; the context unasked, so the plan puts the first 34 of 41 routed layers'
    experts on the host with a 32-34 slot pool, the rest and the routed NextN head on the device; the verify's rows run
    the split command's segments with the host summing each row's picks between them,
    `ARCHITECTURE_GPU_VULKAN_HC.md#hc-verify-rows`), tg-real64 off 61.2-62.8 tok/s:

    | depth | on tok/s | on / off | drafts accepted | per position |
    | ---: | ---: | ---: | ---: | :--- |
    | 1 | 73.8 +/- 1.5 | 1.21 | 72.8% | 73 |
    | 2 | 82.7 +/- 1.0 | 1.32 | 67.2% | 76.9 / 57.5 |
    | 3 | 74.7 +/- 0.5 | 1.21 | 51.8% | 73.6 / 49.1 / 32.8 |

  Depth 2 is the 35B's point on this box: the second row rides the weight stream the first row paid for, the third
  row's 33% acceptance does not pay its host sums. The chain cut (`--mtp-min-p`, `set_mtp_chain_min_p`: a further draft
  only while the last draft's probability under the head reaches p) lifts depth 3 to depth 2's level and moves depth 2
  nowhere - the rows it drops are the ones that would have been rejected:

    | depth | cut | on tok/s | on / off | drafts accepted | per position |
    | ---: | ---: | ---: | ---: | ---: | :--- |
    | 3 | 0.5 | 80.1 +/- 2.8 | 1.30 | 65.0% | 72.3 / 61.2 / 54.5 |
    | 3 | 0.3 | 80.8 +/- 0.9 | 1.28 | 62.1% | 75.3 / 58.7 / 46.9 |
    | 2 | 0.5 | 81.6 +/- 1.2 | 1.29 | 72.4% | 74.3 / 69.5 |

  A sampled stream at depth 2 (`--mtp-temp 0.7 --mtp-top-p 0.9`, both arms sampled, the off arm 55.2-55.9 tok/s - the
  nucleus sampler's softmax and passes over the 248k-token row cost the token about 1.8 ms against the greedy pick): the
  exact walk (sample-and-match) 63.0 +/- 0.4 tok/s, 1.13x, 53.5% of the drafts accepted (66.2 / 40.8 per position); typical
  acceptance (`--mtp-accept typical`, `set_mtp_accept`: a draft the sampler's cuts admit on its row is accepted whatever
  the row's draw) 80.1 +/- 2.7 tok/s, 1.45x, 84.9% accepted (91.9 / 77.9). With no cut at all a typical walk admits every
  draft (100% accepted, three tokens a round, 100.1 tok/s) - the mode means something under a nucleus, a top-k or a min-p
  alone. Speculative rejection sampling (`--mtp-accept rejection`: the head draws its draft through the sampler and records
  q, the row accepts with min(1, p/q), else the residual) at the same depth and sampler, and the three acceptances again
  under the chat-preset shape (`--mtp-top-k 20`, the candidate-list sampler path):

    | sampler | acceptance | off tok/s | on tok/s | on / off | drafts accepted | per position |
    | :--- | :--- | ---: | ---: | ---: | ---: | :--- |
    | top-p 0.9 | exact | 55.9 | 63.0 +/- 0.4 | 1.13 | 53.5% | 66.2 / 40.8 |
    | top-p 0.9 | typical | 55.2 | 80.1 +/- 2.7 | 1.45 | 84.9% | 91.9 / 77.9 |
    | top-p 0.9 | rejection | 55.5 | 56.4 +/- 1.3 | 1.02 | 61.9% | 75.9 / 47.8 |
    | top-k 20 + top-p 0.9 | exact | 60.0 | 64.7 +/- 3.4 | 1.08 | 51.0% | 62.7 / 39.3 |
    | top-k 20 + top-p 0.9 | typical | 59.1 | 83.2 +/- 0.1 | 1.41 | 83.6% | 92.2 / 74.9 |
    | top-k 20 + top-p 0.9 | rejection | 57.2 | 66.8 +/- 0.7 | 1.17 | 62.0% | 76.4 / 47.6 |

  The same three rows with the device's selection landing in place of the rows (`cls_select`, on by default,
  `--mtp-select=false` for the rows; `ARCHITECTURE_GPU_VULKAN_LANDING.md#logits-selection-landing`): every sampled step -
  the off arm's plain steps, the round's drafts and its verify rows - lands the rows' 1024 largest logits with the max and
  the denominator at the temperature, about 17 KB a row for the host's candidate chain instead of the 1 MB row and the
  passes over it. The off arm itself moves from 55-56 to 61-62 tok/s:

    | sampler | acceptance | reps | off tok/s | on tok/s | on / off | drafts accepted | per position |
    | :--- | :--- | ---: | ---: | ---: | ---: | ---: | :--- |
    | top-p 0.9 | exact | 4 | 61.8 | 66.7 +/- 2.9 | 1.08 | 50.2% | 62.8 / 37.7 |
    | top-p 0.9 | typical | 2 | 60.8 | 90.9 +/- 1.7 | 1.49 | 83.2% | 90.2 / 76.2 |
    | top-p 0.9 | rejection | 4 | 60.0 | 71.2 +/- 2.5 | 1.19 | 62.6% | 77.6 / 47.7 |

  Rejection sampling is the sampled walk's default from here: the same round cost as the exact walk - the
  `DASLLAMA_GPU_PROF=1` ledgers read the draft at 1.36 against 1.37 ms and the three-row verify at 26.0 against 26.6 ms,
  12.6 against 13.2 of it the host expert sums - with ten to twelve points more accepted, and lossless where typical is
  not. Two-rep rows cannot rank the two: the host expert sums wander run to run (the off arm itself reads 58 to 62), and
  single pairs read exact ahead by 6 and rejection ahead by 5 on the same afternoon; four reps each settle it at about
  seven percent for rejection. No step's sampler took the rows late on these prompts (a nucleus of 0.9 at temperature 0.7
  is reached well inside 1024 survivors). Found by these rows: the selection's transfer twin is recorded once per row count, and the carry flip's
  re-record (`rd_carry_sync`) had to clear its records too - a twin recorded under the off arm's spec-off stream landed no
  carry, so every draft of the on arm read a stale hidden and the second draft was garbage (position 2 accepted 19.6%
  under exact, 34.6% under typical); the 0.8B cell pins it by recording the twin under a spec-off stream first and asserting
  the accepted counts, not only the tokens, since a garbage draft is rejected and the tokens stay alike.
  Rejection sampling accepts between the exact and typical walks, as it must (it keeps the sampler's distribution where
  typical leans toward the head), and what it cost with the rows landed was the host sampler, not the mode: under top-p alone every draft
  lands the head's 248k-float row on the host and pays a softmax, the nucleus select and a 1024-wide selection to record
  q, and every verify row's judge adds a residual walk over the whole row - five landed rows and about five full-row
  passes a round against the exact walk's three - so the 62% acceptance buys nothing; under a top-k of 20 the same work
  runs over 20 candidates and the same acceptance pays 1.17x. Every acceptance's sampler is host arithmetic over landed
  rows (the greedy walk alone lands picks): a device-side selection landing the survivors instead of the row is the
  lever for all three. Found by these rows and fixed with them: the vocab-wide sampler's nucleus cut sorted the whole row a token (the
  path every request with a top-p and no top-k takes - the OpenAI API carries no top-k), 26.6 tok/s on the 35B against
  55.9 with the cut read off a selected set (`nucleus_cut_`), bit-identical to the sorted prefix.
  The GPU profile of the depth-2 round (`DASLLAMA_GPU_PROF=1`, one rep): the three-row verify 26.9 ms of GPU time with
  the host experts' 13.9 ms inside its wall, the recurrent rollback copies 0.62 ms of it (2.3%), so the copies stay; the
  draft 1.0 ms, of which the classifier over the 248k vocabulary is 0.75 - the draft head over a vocabulary subset is
  the lever on the draft, not its layer. The per-position curve is the one every carrier has read
  (`ARCHITECTURE_GPU_MTP.md#mtp-depth-knob`). The segments form's compare is the census file's verify-rows cell
  (`tests/test_gpu_resident_format_census.das`), a driver-against-itself compare - the verify rows against the same
  driver's one-row steps, a rounding control and not parity evidence: one round's two rows within 0.58 and 0.64 of their 2.0 and 2.4 bars
  against the split command's one-row steps (controls 8.96), the carry rows within 1.0 of 8.0, and four rounds'
  committed stream within 0.86 of 2.28 against the same tokens stepped (control 8.74). The served baseline on the same
  file with the round off (`served`, `out-of-process`: `harness/served_bench.das --sizes 3000,9000 --reply 200 --reps 5` against
  `utils/dasllama-server/main.das --gpu vulkan --ctx 16384 --streams 1`, so 16 host layers and no pool under the strict
  context): cold TTFT 2069 ms (cv 4.6%, VOID) / 5245 ms, prefill 1446 / 1680 tok/s, warm TTFT 304 / 476 ms (both VOID
  on cv), decode 72.4 / 68.5 tok/s at 3k / 9k; the on arm through the server waits on the same segments form reaching
  the slot's auto-arm, which still leaves the round off where the experts sum on the host.
- **MEASURED (2026-10-19) - the long-prompt pass: the block codecs' store and the serving rules, Apple M5 Max (128 GB,
  thermally capped: `powermetrics` read the GPU at 840-1030 of 1620 MHz under Heavy pressure through every row, so no
  figure here goes beside a cool box's) and the RunPod RTX PRO 4500 Blackwell (32 GB, driver 580.173.02).** Every figure is
  debug-jit (`bin/daslang -jit`, the box profile `performance/m5.tune.json` re-minted at 4cf21c280a on the M5, one box
  mint `/workspace/pod.tune.json` on the pod, one model process a box), and every before/after pair is direction-grade:
  the same binary on both arms, the before arm master 4cf21c280a's `modules/dasLLAMA`, the after arm 737794e076 (the Metal
  store and the serving rules) or 8a575d5869 (the Vulkan store and attention), each arm its own tree.
  - The detector, `modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m <file> -p <512 | 2048 | 8192> -n 32 --npl 2 --npl-plen
    <p - 64> -r 3 --kv <f16 | q8_0 | tq4> --ngl 1 --for-debug-purposes` (`DASLLAMA_BENCH_VERBOSE=1`, the rows read at the
    best rep; on the pod `DASLLAMA_GPU=1`, no `--ngl`) over every stocked text model on both homes, read the
    512 -> 2048 -> 8192 ratios inside one row and the codecs beside f16; the readings stand in the session's table, not
    here - the pass fixed what the ratios pointed at and recorded no board.
  - The Metal q8_0 store, a 32-block on one thread before and a lane an element after (`benchmarks/decode_step_trace.das`,
    one trace a reading, GPU us a decode step at the trace's steady window, no spread): Qwen3-30B-A3B Q4_K_M q8_0
    7661 -> 6613 at 512 keys (f16 6223), 12292 -> 11323 at 8192; Llama-3.2-1B Q8_0 3334 -> 3092; Qwen3.6-35B-A3B UD-Q4_K_M
    7556 -> 7306 (f16 7220). `DASLLAMA_METAL_DECODE_SKIP=attn` put a block codec's cost outside attention at every
    context, which is what pointed at the store.
  - The Vulkan q8_0 store, the same change (`lcpp_bench -m Qwen3-0.6B-Q8_0.gguf -p 512 -n 128 -r 3 --kv q8_0`, tg128 at the
    best rep, the per-kernel us from `DASLLAMA_GPU_PROF=1 DAS_LOG_LEVEL=info` `vk_rdpf` lines, one profiled run a reading):
    decode 465 -> 489 tok/s (f16 519), the 512-row window's store 1931 -> 1313 us (pp512 39.9k -> 42.1k tok/s), tq4's
    window store 2926 -> 2355 us (the rotation's in-group passes by shuffles).
  - The Vulkan attention stamps with the packed integer dot (`harness/vk_attn_probe.das -- 2 8192 128 32 8 4 <f16 | q8_0 |
    tq4>`, `DASLLAMA_GPU=1`, the best split of the ladder, us a layer): f16 65.7, q8_0 61.5 (76.8 before), tq4 57.3
    (65.6 before). In a model at 8k the pair reads, Qwen3-0.6B Q8_0 on the pod, `lcpp_bench -p 8192 -n 32 --npl 2 --npl-plen
    8128 -r 3 --kv <codec>`, master 4cf21c280a's `modules/dasLLAMA` against 87ce65175c's on the same binary (direction-grade,
    two passes an arm, cv under 0.2 %): q8_0 tg32@2 462.5 -> 484.0 / 485.0 tok/s, pp8192 35.9k -> 37.0k; tq4 tg32@2 503.9 /
    505.4 -> 517.6 / 517.8, pp8192 30.9k / 31.0k -> 32.6k / 32.7k. The cost is noise: both chains round the query, each its
    own, so the q8_0 device-vs-CPU readings widen from 0.42-0.63 to 0.42-0.89 of the max logit on Qwen2.5-0.5B (the cells'
    bars moved from 8 to 10 %, the one-step-off controls at 7.9-9.6).
  - The Metal fused decode attention over a block codec stays about 1.5x the f16 stamp a key (`harness/attn_d_codec_race.das`,
    the 30B shape, two rows, a ring of eight 16384-key slabs the rounds walk so a depth streams its rows past the cache; ms a layer-call, best of 5 rounds after a 150 ms burn, at 8192 keys f16 0.111, q8_0 0.177, tq4 0.184 - 303, 101 and 51 GB/s of K/V bytes - and at 16384 0.213, 0.352, 0.373; at 512 0.014, 0.019, 0.019; followup_metal 54). Neither a
    16-bit word view of the quants nor a lane-eight mapping with shuffle reductions beat the per-key loop.
  - tq4 on Vulkan pays two latency-bound dispatches a layer a token - the store (`KvTq4StoreT`, four barriers) and the
    un-rotation after attention (`KvTq4Unrot`), 8 us together by the `vk_rdec` lines of one profiled Qwen3-0.6B tg128 run
    (followup_vulkan 152), where q8_0's store is one.
  - The arena bound's footprint pair (the Metal mirror arena bounded by the device's largest buffer in place of the 4 GiB
    cap): gemma-4-12B-it-Q4_K_M, `lcpp_bench -p 8192 -n 32 --npl 2 --npl-plen 8128 -r 3 --kv f16 --ngl 1`, `/usr/bin/time
    -l`, the M5 Max, the same binary, master 4cf21c280a's `modules/dasLLAMA` against 29df907702's (direction-grade): before,
    peak 18.8 GB resident and 67 s wall to the batched row, which the arena refused (`batch decode declined: mirror`,
    the run's exception under the required mode); after, peak 21.5 GB and 90 s wall with the row served - pp8192
    2052 +- 67 against 1970 +- 97 tok/s (inside the box's thermal spread), tg32 55.9 +- 7.0 against 55.9 +- 7.3, tg32@2
    91.0 +- 1.4 tok/s summed over the two streams where before there was no row. The extra 2.7 GB is the two streams'
    8k mirror the arena now holds.
  - tq4 on Qwen2.5-0.5B served wrong tokens on every home: its Q/K/V bias outliers (|b| to 316 against a median of 0.3)
    set every 4-bit block's scale. The serving rule steps such a model down to q8_0 by name (`kv_codec_served`). The Metal
    mirror arena's 4 GiB idle cap no longer refuses a live batched step: under f16 the 12B+ dense models' two-stream decode
    after 8k took the CPU rails (`EXC ... batch decode declined: mirror` in the detector's rows), and now serves.
  - The pod's Vulkan MoE pairs, the same binary, new = master's `modules/dasLLAMA` (4cf21c280a, the streamed-expert change
    in) against ctl = a28ffe7e83's (the commit before it), two reps an arm with the arm order alternated, `lcpp_bench -m
    <file> -p 512 -n 128 -r 2` and `-p 8192 -n 32 --npl 2 --npl-plen 8128 -r 2`, tok/s mean (min-max of the two reps);
    the pp8192 gain is the 2048-row prompt window the change widened, the decode rows a tie:

    | file | pp512 new / ctl | tg128 new / ctl | pp8192 new / ctl | tg32@2 new / ctl |
    |---|---|---|---|---|
    | Qwen3-30B-A3B-Instruct-2507-Q4_K_M | 6402 (6389-6416) / 6468 (6467-6468) | 172 (172-173) / 172 (172-172) | 7402 (7397-7406) / 5199 (5197-5200) | 191 (189-192) / 192 (192-192) |
    | Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS | 5441 (5388-5493) / 5480 (5447-5514) | 189 (188-189) / 188 (188-188) | 6638 (6630-6646) / 4501 (4488-4513) | 196 (196-196) / 196 (196-196) |
    | Qwen3-Coder-30B-A3B-Instruct-Q4_K_M | 6084 (6069-6099) / 6097 (6089-6104) | 172 (172-172) / 172 (172-172) | 7357 (7351-7364) / 5002 (4996-5008) | 193 (193-193) / 193 (193-193) |
    | Qwen3.5-35B-A3B-UD-IQ3_S | 4545 (4538-4553) / 4602 (4601-4603) | 154 (154-154) / 154 (153-154) | 6872 (6867-6878) / 4410 (4406-4414) | 238 (237-238) / 238 (238-238) |
    | Qwen3.6-35B-A3B-MTP-IQ4_XS-pure | 5384 (5367-5401) / 5428 (5422-5435) | 167 (167-168) / 168 (168-168) | 7650 (7631-7669) / 5151 (5143-5159) | 220 (220-220) / 220 (220-220) |
    | Qwen3.6-35B-A3B-MTP-UD-Q4_K_M | 5140 (5033-5247) / 5326 (5320-5333) | 142 (142-142) / 142 (142-142) | 7586 (7578-7594) / 5122 (5120-5124) | 220 (218-222) / 225 (225-226) |
    | Qwen3.6-35B-A3B-UD-IQ2_XXS | 4477 (4476-4477) / 4486 (4420-4553) | 174 (174-174) / 175 (175-175) | 6747 (6742-6753) / 4310 (4305-4315) | 259 (259-259) / 259 (259-259) |
    | gemma-4-26B-A4B-it-Q4_K_M | 6284 (6259-6308) / 6317 (6311-6323) | 135 (135-135) / 135 (135-135) | 7715 (7701-7729) / 5645 (5634-5656) | 206 (206-206) / 207 (207-207) |
    | gemma-4-26B-A4B-it-UD-IQ3_XXS | 5327 (5304-5351) / 5346 (5320-5372) | 147 (147-147) / 146 (146-147) | 6847 (6839-6854) / 4843 (4833-4853) | 211 (211-211) / 211 (211-211) |
    | gemma-4-26B-A4B-it-MXFP4_MOE (the local mint) | 5543 (5533-5552) / 5545 (5538-5553) | 138 (138-138) / 138 (138-138) | 6908 (6896-6919) / 4963 (4956-4969) | 210 (210-210) / 210 (210-210) |
    | gpt-oss-20b-mxfp4 | 6210 (6203-6217) / 6264 (6260-6267) | 209 (209-209) / 209 (209-209) | 8552 (8551-8554) / 5988 (5986-5990) | 288 (288-288) / 289 (289-289) |
    | Qwen3.6-35B-A3B-Q8_0 (streamed by design) | 1671 (1661-1681) / 514 (511-517) | 99 (99-100) / 34 (34-34) | 3712 (3678-3745) / 264 (264-264) | 83 (83-83) / 20 (20-20) |

    The streamed 35B Q8_0's control ran the per-op tier (the change is what streams it), so its pair measures the tier
    switch, not a kernel. The resident files: no deterioration - the A/B the streamed-expert change (PR 4321) owed.
- **MEASURED (2026-10-17) - the residual step's race past the slab, widened, M5 Max (128 GB).** `bench_metal_gk_rows`
  (`dastest.das -- --bench --test`, debug-jit, the box profile `performance/m5.tune.json` at 39,101 B,
  `DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_CPU_PREFILL=1 DASLLAMA_METAL_HAZARD_STRICT=1`) at b4b207a287 adds to the entry below the
  widths 4608 and 5120, the window's plain step past the slab and the decode rows' f16-twin stamps (`enc_preaddrms_bhxw_c`,
  `enc_add_rms_bhxw_c`); the figures stand in `ARCHITECTURE_GPU_SHARED_KERNELS.md`'s seam bullet. The one dispatch beats the
  split chain at every width, row count and form timed: the seam at 4608 7.13 / 7.21 us at one / four decode rows against
  9.26 / 9.52 and 49.10 against 101.4 on the 512-row window; the window's plain step 43.90 against 73.88 at 4608 and 114.9
  against 136.8 at 8192; the seam with the twin at 5376 7.48 / 7.58 against 9.66 / 9.98. The 1024-lane decode builders beat
  the 256-lane ones past the slab at every width and form, with the twin and without.
- **MEASURED (2026-10-17) - the Metal residual step past its slab, M5 Max (128 GB).** The step stages a row's first 4096
  columns in the threadgroup slab and re-reads the columns past it, so the gemma-4 31B's 5376-wide seams take one dispatch
  where they took the split chain (the rms, the scaled add, the next norm), and the decode rows past the slab run 1024 lanes.
  The kernel figures are `bench_metal_gk_rows` (`dastest.das -- --bench --test`, debug-jit, the box profile
  `performance/m5.tune.json` at 39,101 B, `DASLLAMA_ALLOW_UNTUNED=1`) at 5677d5b4da, in
  `ARCHITECTURE_GPU_SHARED_KERNELS.md`'s seam bullet: at 5376 the seam reads 7.48 / 7.58 us at one / four decode rows against
  the split chain's 9.59 / 10.00, and 58.94 against 125.3 on the 512-row window. The same run's real 1024-lane stamp reads
  5.66 / 5.72 us at dim 1152, 6.19 / 6.25 at 2560 and 6.66 / 6.73 at 3840 on the decode rows and 46.17, 54.62 and 63.44 on
  the window, where the 256-lane builders read 3.24 / 3.35, 4.96 / 5.10, 6.75 / 6.91 and 9.26, 27.65, 40.82: the 2026-10-16
  entry's "1024-lane bench stamp" readings (3.23 / 3.27 and the rest) were the 256-lane builders, which that race's 1024 arm
  dispatched once the builders moved to 256 lanes, so 256 lanes wins the decode rows at 1152 and 2560 and ties at 3840, not
  at every width, and loses past the slab (8.76 / 8.99 at 5376 against 1024's 7.48 / 7.59).
  The 31B row, `benchmarks/lcpp_bench.das -m gemma-4-31B-it-Q4_K_M.gguf --ngl 99 -p 512 -n 128 -r 5 --for-debug-purposes`
  under the manifest with `DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_METAL_HAZARD_STRICT=1`, the base 794cc1ac1a against
  5677d5b4da, each tree's own copy of the binary, one model process on the box: pp512 829.4 (cv 0.1 %; the base's other
  valid readings 825.7-831.7) against 832.6 (cv 0.15 %), +0.4 %, inside the base's own spread. tg128 holds no verdict: every
  reading on both trees is VOID (cv 3.5-4.3 %), its first rep about 2 tok/s under the four after it on both (base 24.5 then
  26.6-27.1, the change 24.7 then 25.7-27.1); readings taken back to back on the box fell by up to a third as it heated and
  are dropped. The 31B's parity row - cell `gemma4_hetero_row` of `tests/test_metal_support_matrix.das`, tag `gemma4-31b/k4` -
  is green at c824d8e8a3 (which carries 5677d5b4da) through `tests/run.das -- --suite matrix --arm fam-gemma4 --family
  gemma4` under the same manifest and `DASLLAMA_PARITY_FULL=1`: the freeform rows within 8 of the CPU's (widest 4.25 at
  row 20, the CPU's largest logit there 26.9), the counting and B=2 streams token for token.
- **MEASURED (2026-10-17) - a driver-against-itself bar and an external-reference bar the M5's box profile moves, M5 Max
  (128 GB).** Every reading below is a `tests/run.das` run at c824d8e8a3, one model process on the box, the box profile
  `performance/m5.tune.json` (39,101 B), `DAS_TUNE_MANIFEST` at it with `DASLLAMA_ALLOW_UNTUNED=1
  DASLLAMA_METAL_HAZARD_STRICT=1 DASLLAMA_PARITY_FULL=1`.
  - The driver-against-itself bar: `tests/test_metal_mtp_parity.das`'s `vff_cell` holds the Metal batch rail's same-slab
    verify against the Metal plain steps, every plain step counted a Metal decode. `MM_TOL` 1.0 now holds a K-quant
    carrier's two-to-eight-row verify wherever a rows site's half-x forms serve on the box (`kq_rows_half_<fmt>` crowned, the
    rail on - the M5 profile crowns thirteen formats'), over every verify row of twelve rounds; the f32-x forms keep the
    carrier's own bar. Run `tests/run.das -- --suite mtp --arm mtp-vff`, maxd over the rows, the control (the same verify
    with its GEMV stage knocked out, `set_metal_decode_skip("gemv")`) the widest row of its four rounds:
    - gemma-4-12B-it Q4_K_M (`test_metal_verify_gemma12`, bar 0.5 before): arm `mtp-vff-g12` 0.61 over 48 rows, control
      56.4; arm `mtp-vff5-g12` 0.40 over 60 rows, control 54.1; no argmax flip. The nine-row arm `mtp-vff9-g12`, at `MM_TOL`
      before and after, reads 0.84 over 108.
    - the Qwen MTP fixtures' arm `mtp-vff-<tag>` (`FF_TOL` 0.5 before), four rows over 48: Qwen3.5-9B-MTP UD-Q5_K_XL
      (`9b`) 0.0061, control 27.2; Qwen3.6-27B-MTP Q4_K_M (`27b`) 0.0068, control 25.4; Qwen3.6-35B-A3B-MTP UD-Q4_K_M
      (`35b`) 0.00083, control 20.6; Qwen3.6-35B-A3B-MTP IQ4_XS-pure (`35b-iq4xs`) 0.081, control 17.8; Qwen3.8-27B
      Q4_K_M with its split Q8_0 head (`3.8-27b`) 0.0038, control 23.4. The Q8_0 fixtures (`0.8b`, `4b`) have no half-x
      site and keep 0.5.
    A probe, not evidence: the gemma-12B arms under `DASLLAMA_METAL_KQ_HALF=0` (dastest on the cell, the same tree) read
    0.08 at four rows and 0.24 at five, the half-x forms' share of the readings above.
  - The external-reference bar: `tests/test_whisper.das`'s `test_canary_qwen_oracle` holds the decoder's forced walk to the
    frozen NeMo SALM ids (`harness/canary_qwen_expected.tsv`). `CN_TIE_BAR` 0.25 is the largest gap the walk may read
    between our argmax logit and the oracle id's, on the f32 lane (f32 K/V), every step of the three clips. Run
    `tests/run.das -- --area audio`: jfk (23 ids) and `09_1089-134686-0008` (19) read 0 on the CPU chain and on the
    Metal tower - every oracle id our argmax; `05_1089-134686-0004` (14) reads 0.084 on the CPU chain and 0.102 on the
    tower at step 9, a greedy near-tie between "." (13) and " Good" (7684) where NeMo picked ".". The control feeds the
    oracle ids one step early: every step whose input moved lands outside the bar, the nearest 10.58 to 10.62 on jfk's 21
    steps, 11.55 to 11.61 on clip 09's 17 and 11.40 to 11.41 on clip 05's 12. The same run's `test_vulkan_tower_kernels.das`
    dies at start (exit 139): the Vulkan files do not run on the M5.
- **MEASURED (2026-10-17) - the window's f32 per-layer-embedding GEMMs on the staged KHR tile, and the K-quant staged race,
  the pod.** Box: the RunPod RTX PRO 4500 Blackwell 32 GB, driver 580.159.04 (no `VK_NV_cooperative_matrix_decode_vector`:
  the cm2 tiles run the scalar decode arm), one GPU process at a time. Commands, verbatim, from the tree root under
  `DASLLAMA_GPU=1 DASLLAMA_MODELS_DIR=/workspace/models DASLLAMA_ALLOW_UNTUNED=1`: the rows
  `DASLLAMA_IMAGE_SAVE=0 ./bin/daslang -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m /workspace/models/<file> -p 512 -n 0 -r 3 --for-debug-purposes`,
  the stages the same line under `DASLLAMA_GPU_PROF=1 DAS_LOG_LEVEL=info` (the `vk_rdpf` line, us a 512-row window); the races
  `./bin/daslang -jit modules/dasLLAMA/harness/vk_gemm_probe.das -- plef32`, `-- kqst:<fmt>` (each figure an arm's best of four
  interleaved rounds after one untimed round, `sweep_best_us`) and `-- narrow:<fmt>` (its second round). The gemma-4 E2B
  Q4_K_M ships its `inp_gate` and `proj` planes F32, which the window ran on the scalar f32 tile (`DnBaGemm`) and runs on the
  staged tile (`ARCHITECTURE_GPU_VULKAN_KHR.md#f32-rows-staged-tile`): at b06df183e8 pp512 12754 (cv 0.8 %), window 41056 us,
  `ple_gate` 5932, `ple_proj` 4770, `ple_rq` 621; at ad39b47963 pp512 16359 (cv 0.2 %), window 31941, `ple_gate` 718 (split
  six ways), `ple_proj` 555, `ple_rq` 837 (now the f16 convert of the stream rows); every other stage within 1 %. The E-series
  files without f32 planes, b06df183e8 -> 5e46f3acb3: E2B Q4_0 15378 -> 15386, E2B Q8_0 16097 -> 16390, E4B Q8_0 9728 -> 9745
  (cv 1.6 %); their `ple_proj` takes the f16 feed on the cm2 tiles (Q8_0 575 -> 569, Q4_0 646 -> 672). `plef32` at
  ad39b47963: the scalar f32 tile 3.1 TFLOP/s at the E2B and E4B gate and projection shapes, the staged tile 75-85 at 512 rows
  (E2B gate 127.6 -> 5.06 us, proj 127.9 -> 5.33, E4B gate 212.7 -> 7.87, proj 212.7 -> 8.25); one dispatch at a time the E2B
  gate 164.6 us scalar, 53.8 staged whole, 33.1 / 25.2 / 17.0 split 2 / 3 / 6 with the reduce, the E4B gate 273.4, 86.7, 25.2
  split 5; at 300 rows the E2B gate 105.8 -> 19.0 split six ways, the proj 82.5 -> 14.9. `kqst:<fmt>` at ad39b47963, us at
  512 rows, cm2 l stamp / staged: k4 gate 75.9 / 81.9, down 72.7 / 83.0, q 27.0 / 29.0, 4B down 185.2 / 210.8; k5 gate 79.9 /
  94.8, 4B down 200.4 / 246.6; k6 gate 82.1 / 95.6, down2x 158.2 / 194.7; iq4xs gate 78.1 / 85.4, down2x 150.9 / 181.8; at
  500 rows the m stamp / staged: k4 89.4 / 82.7, k5 97.8 / 95.2, k6 97.6 / 95.7, iq4xs 97.7 / 86.8 (`followup_vulkan.md` row
  149). Decision: the f32 rows take the staged tile on every coopmat device at subgroup 32 whose dim and ple sit on its k
  step; the K-quant planes stay on the cm2 tiles. The tile pick's narrow rule (`narrow:<fmt>` at ad39b47963, every K-quant
  format, K 1536) one dispatch at a time, us s / m / l at 512 rows: k4 at 256 and 512 wide 23.1 / 35.8-36.8 / 60.0, k6 25.2 /
  39.5 / 64.1, iq4xs 27.2 / 39.5 / 62.0, k2 37.4 / 51.9 / 76.4, iq2xxs 45.7 / 55.9 / 81.6, iq2s at 256 wide 44.8 / 54.0; at
  768 wide m wins (k4 37.3 / 43.6 s); overlapped sixteen deep the order turns (k4 at 256 wide 10.0 / 5.1 / 6.0). The E2B
  Q4_K_M window (the stage command above) on 5e46f3acb3 against the same tree with the narrow rule off for the K-quant planes
  (their k and v on the wave model's m): k 496 -> 632 us, v 477 -> 661, the window 31859 -> 32227. Decision: the narrow rule
  stands for the K-quants; the overlapped reading that suggested otherwise is not the cost a window pays.
- **MEASURED (2026-10-16) - the gemma leg's end re-measure, the M5 half.** Instrument `benchmarks/lcpp_bench.das`
  (`--ngl 99 -p 512 -n 128 -r 5 --for-debug-purposes`, debug-jit), the M5 Max (128 GB) alone on the box, one box profile
  minted at the leg tip (505 s, 39,101 B) and read by both trees: master's own build (33e7910669) against the leg tip
  (ab97a01251). pp512 / tg128 tok/s, master -> leg: gemma-2 2B Q8_0 10471 -> 10810 / 165 -> 165; gemma-3 1B Q8_0 21663
  -> 22857 / 317 -> 321; gemma-3 4B Q8_0 7058 -> 7225 / 112 -> 114; gemma-4 E2B Q8_0 9479 -> 9936 / 157 -> 158; E4B Q8_0
  5470 -> 5640 / 89.6 -> 89.5 (the first leg reading, 81.6 at cv 11.7 %, VOID); 12B Q8_0 2101 -> 2154 / 40.7 -> 41.0
  (a first leg reading of 32.1 at cv 3.6 % did not reproduce: 41.0 with the ring, 40.9 without, at the same form); 26B-A4B
  UD-IQ4_XS 3952 -> 4059 / 121 -> 122; 26B-A4B Q4_K_M 4143 -> 4271 / 118 -> 120 (the leg's first tg at cv 13.7 % VOID; a
  master repeat at cv 8.6 % VOID). Prefill gains 2 to 5 % on every row; decode holds parity. The ring costs nothing at the
  wrap: 12B tg after a 512-token prefill 41.0 with the ring, 40.9 without; E4B 89.5 / 89.8; at position zero 41.0 / 41.0
  and 88.9 / 89.4. The house replay's quick probe (22 short replies, wall median s), master -> leg: 12B Q8_0 1.882 -> 1.259,
  26B-A4B UD-IQ4_XS 0.710 -> 0.633, E4B Q8_0 0.614 -> 0.533. The row races (`bench_metal_gk_rows`, 109 rows, none VOID) stand
  in `ARCHITECTURE_GPU_SHARED_KERNELS.md`'s tables. The seam's decode builders moved from 1024 to 256 lanes at this
  re-measure: the builders at 1024 read 5.76 / 5.83 us at one / four rows at dim 1152 and 6.14 / 6.30 at 2560 where the
  256-lane builders read 3.19 / 3.28 and 4.87 / 5.04 (3840: 6.67 / 6.74 against 6.71 / 6.83), a 1024-lane bench stamp
  reads with the 256-lane one at every width (3.23 / 3.27, 4.88 / 5.07, 6.71 / 6.85), and 128 lanes loses everywhere; the
  window's stamps keep 256 (27.10 against 1024's 27.10 and 128's 34.50 at dim 2560, 40.33 / 40.30 / 54.31 at 3840). The
  12B Q8_0 row on one tree, 1024 -> 256 lanes: pp512 2126 -> 2154; its tg128 read 40.6 and 39.5 at cv 4.8 / 4.2 %, both
  VOID (three more 256-lane readings 39.0-39.7 at cv 4.1-4.3 %, after an hour of GPU suites), so the decode row holds no
  verdict and the switch rests on the race. Every Metal kernel gate, the batched row and the gemma4 and E-series arms are
  green at 256. The fused seam holds no arm at dim 5376 (`followup_metal.md` row 54).
  **The pod half** (RTX PRO 4500 32 GB, the 580 driver without decode-vector, `-p 512 -n 128 -r 3`, no box profile on either
  side, one binary for both trees since the leg changes no Vulkan C++): master 33e7910669 -> the leg, pp512 / tg128 tok/s:
  gemma-2 2B Q8_0 19597 -> 19462 / 217.0 -> 218.1; gemma-3 1B Q8_0 pp VOID on both sides (cv 46 and 58 %: a 20 ms row) /
  396.5 -> 397.3; gemma-3 4B Q8_0 13049 -> 13134 / 150.7 -> 151.4; gemma-4 E2B Q8_0 16382 -> 16294 (16088 at five reps) /
  tg VOID on both sides (149 at cv 25 %, 185 at cv 9 %, 188.5 at cv 6.7 % over five reps); E4B Q8_0 9718 -> 9541 / tg VOID
  (93.6 at cv 16 %, 107 at cv 7 %); E2B Q4_K_M 15917 -> 12304 (12353 at five reps) / 202 at cv 21 % VOID -> 221.6; 12B
  Q8_0 4158 -> 4094 / 55.75 -> 55.85; 26B-A4B UD-IQ3_XXS 5422 -> 5353 / 143.3 -> 146.2; 26B-A4B Q4_K_M 6405 -> 6240 /
  135.0 -> 135.3; 31B Q4_K_M (the CPU rows, the card declines it) 1435 -> 1403 / 36.54 -> 36.47. Every prefill row but
  one holds within 3 % and every valid decode row holds parity or gains; the E2B Q4_K_M prefill loses 23 %, bisected to
  the PLE merge and split to the window's gate and projection planes, which that file stores in f32 and the window ran
  on the scalar f32 tile; the staged KHR tile (the MEASURED (2026-10-17) entry above) returns the row to 16555. The assistant drafter on the 26B Q4_K_M (`--mtp-ab`, 64 tokens, three prompts): 131 -> 193 tok/s (x1.47) at 88 %
  acceptance on two prompts, x1.35 on the third. The ring at pp8192 on the 26B IQ3_XXS: 4824 tok/s and tg32 147.0 with the
  ring against 4830 and 147.0 without, host peak RSS 24.4 against 23.8 GB. The house quick probe (one server a model,
  alone: wall s / first token s / generation tok/s as the client sees), master -> leg: 12B Q8_0 7.21 / 6.87 / 12 ->
  0.57 / 0.46 / 41; 26B IQ3_XXS 4.24 / 3.60 / 19 -> 0.43 / 0.33 / 106; E4B Q8_0 0.29 / 0.18 / 56 -> 0.36 / 0.23 / 77. The
  first pass of these rows read `exit=1` on every big model: the volume had hit its quota under the leg's re-minted images,
  and a chain logging onto the full volume keeps only the lines before the fill - the rows above are the re-runs.


- **MEASURED (2026-10-16) - the gemma leg's parity bars, one row a file and bar.** Each row names the cell, the bar
  (its constant where it has one), the reading and the box. The Vulkan box is the RTX PRO 4500 (32 GB) pod, its cells run
  one file through `dastest` under `DASLLAMA_GPU=1 DASLLAMA_PARITY_FULL=1` with the cell's arm in `DASLLAMA_TEST_ARMS`;
  the Metal box is the M5 Max, through `tests/run.das -- --arm <family>` under `DASLLAMA_PARITY_FULL=1`.
  - gemma-4-31B Q4_K_M, the forced-feed freeform decode on Metal (`test_metal_support_matrix.das`'s
    `gemma4_hetero_row`, tag `gemma4-31b/k4`, arm `fam-gemma4`): the 12B's bar, 8.0 on the whole-vocab logit gap, held over
    the stream's first 29 rows of 128 (`free_rows`) where the 12B rows hold all 128. Reads under 8 on those rows; at the
    30th row the greedy stream (llama.cpp's too) falls into a one-token loop, both chains pick the loop token, and the gap
    inside it grows to 18, so the rows past the 29th carry no information the bar could read. M5.
  - gemma-4-26B-A4B Q4_K_M, the assistant drafter's device step against the CPU oracle on Vulkan
    (`test_gemma_drafter_gpu_step`, arm `mtp-gdraft-gpu`): `VK_FEED_FLIP_BAR` 0.5 on the draft logits and on `h_next`,
    the oracle reading Q8_0 activation rows as the device's GEMVs do. Reads 0.30 / 0.29 at the short anchor and 8.6e-6 /
    8.1e-6 one past the sliding window, both drafts' argmax the oracle's; the oracle drafting off the next token id lands
    past the bar (the control). Pod.
  - gemma-4-E2B Q4_K_M, the forced feed held to the file's f32 chain on Vulkan (`test_gpu_resident_gemma4_e2b_ple_planes`,
    arm `g4e-kq`): `GEMMA_LOGIT_BAR_REL` 0.16 of each row's max |logit|, the prompt and eight steps. Reads 0.26 at the prompt
    (bar 4.05) and 0.73-1.82 over the steps (bars 2.88-3.95), every row's argmax the f32 chain's, the one-step-off control
    17-26; step 4's top logit 19.28 on the device where the f32 chain reads 19.14. Pod.
  - the same file and cell, `F32_DIST_REL` 1.15 (`f32_chain_witness`): the device's summed distance from the f32 chain over
    the CPU q8 chain's own. Reads 9.54 against 13.50 (0.71), the f32 rows one step off 188.7. Pod.
  - gemma-4-E2B Q8_0, the decode step's side row built on the device against the CPU pre-step's row on Vulkan
    (`test_gpu_resident_gemma4_e2b_ple_pre_step`, arm `g4e-pre`): 1e-4 of the CPU row's largest value. Reads 2.9e-6 of a
    1.6e-3 bar (largest 15.98); another token's CPU row 7.69 off (the control). Pod.
  - gemma-4-E2B Q4_0, the forced feed against the CPU chain on Vulkan (`test_gpu_resident_gemma4_e2b_ple_planes`, arm
    `g4e-kq-q40`): `GEMMA_LOGIT_BAR_REL` 0.16. Reads 1.25 at the prompt (bar 4.43) and 0.75-1.77 over the steps (bars
    3.29-4.25), every argmax agreeing, the one-step-off control 14.4 and past. Pod.
  - the four format carriers off the E2B Q8_0 on Vulkan, the forced feed against the CPU chain: `GEMMA_LOGIT_BAR_REL` 0.16.
    emb-Q6_K and emb-IQ4_XS (`test_gpu_resident_gemma4_e2b_ple_tables`, arm `g4e-ple`) read 0.76 and 0.62 at the prompt
    (bars 4.17, 4.14), 0.51-1.74 and 0.49-1.17 over the steps (bars 2.91-4.02); ple-Q6_K and ple-Q5_0
    (`test_gpu_resident_gemma4_e2b_ple_planes`, arms `g4e-kq-ple-q6k`, `g4e-kq-ple-q50`) read 0.86 and 0.71 at the prompt
    (bar 4.17), 0.59-1.54 and 0.43-0.87 over the steps (bars 2.92-3.97). Every argmax agrees but ple-Q6_K's step 4, a near
    tie the cell's tie bar admits (the CPU's ' river' 19.61 against the device's ' miller' 19.59). Pod.
  - Qwen2.5-0.5B IQ4_NL-pure (`-local`), the per-op FFN rail's per-32 carrier on Vulkan, its table and classifier per-32
    too (`test_gpu_resident_format_census`, arm `vk-perop-ffn`): 0.08 of each row's max |logit|, moved from the llama
    carriers' 0.06. Reads 0.44-0.73 over the prompt and six steps (bars 0.86-1.57, max logits 10.7-19.6), every argmax
    agreeing but step 4's near tie (10.70 against 10.79); the one-step-off control 7.4 and past. Pod.
  - gemma-4-26B-A4B MXFP4_MOE (`-MXFP4_MOE-q4km-local`) on Vulkan, 150 prose ids prefilled and 150 teacher-forced
    (`test_gpu_resident_gemma4_26b_mx4_ppl`, arm `mx26`): `GEMMA_MOE_PPL_RATIO` 2.0 on the perplexity, and the argmax hits
    held within 12 of the CPU chain's (`GEMMA4_26B_MX4_HITS_SLACK`). Reads 4.25 against the CPU chain's 3.14 (1.35x), hits
    128 against 136; the one-position-off control's hits 2. Pod.
  - gemma-4-26B-A4B UD-IQ3_XXS under an 8000 MB plan room, the first routed layers' experts on the host
    (`test_gpu_resident_gemma4_26b_host_experts`, arm `host26`): `GEMMA_MOE_PPL_RATIO` 2.0, hits within 12
    (`GEMMA4_26B_HITS_SLACK`). Reads 3.00 against 4.02 (0.75x), hits 129 against 130; the control's hits 2. Pod.
  - gemma-4-E4B Q8_0 under a tq4 mirror (`test_gpu_resident_gemma4_e4b_block_codecs`, arm `g4f-codec`): `GEMMA_PPL_RATIO`
    1.05, hits within 3 (`ppl_compare`'s default). Reads 1.076 against 1.064 (1.011x), hits 148 against 148; the control's
    hits 0. Its q8_0 forced feed reads 0.53-1.57 against bars 4.11-4.65 at `GEMMA_LOGIT_BAR_REL`. Pod.
  - gemma-3-12b Q4_K_M on Metal (`test_metal_support_matrix`, row `gemma3-12b/k4/f16`, arm `fam-gemma3`): the overrides'
    greedy generate held to the CPU chain token for token, eight tokens after the 30-number count prompt. Reads every
    token equal. M5.
  - gemma-4-26B-A4B UD-IQ3_XXS, UD-IQ4_XS and MXFP4_MOE (`-MXFP4_MOE-local`) on Metal (`gemma4moe_row`, arm
    `fam-gemma4moe`): 8.0 on the post-prefill logits and 24.0 on the forced step, on the fused tail, the unfused tail and
    a q8_0 mirror. Read post-prefill / forced step: UD-IQ3_XXS 3.45 / 5.28, 3.38 / 6.69, 3.13 / 15.95; UD-IQ4_XS 2.07 /
    5.34, 2.25 / 5.02, 1.73 / 4.95; MXFP4_MOE 2.11 / 10.60, 2.02 / 11.71, 2.40 / 14.02; the eyeball picks agree on every
    row. M5.
  - gemma-4-E2B Q4_0 and the four format carriers off the E2B Q8_0 (emb-Q6_K, emb-IQ4_XS, ple-Q6_K, ple-Q5_0 `-local`
    mints) on Metal (`gemma4e_row`, arms `fam-gemma4e-q40` and `fam-gemma4e-e2b-*`): 4.0 on the single-stream forced step,
    the batched step's two rows and the depth cell. Read single / batched / depth: Q4_0 0.63 / 1.28 / 1.08; emb-Q6_K 0.30 /
    0.51 / 0.67; emb-IQ4_XS 0.43 / 0.75 / 0.72; ple-Q6_K 0.72 / 0.73 / 0.64; ple-Q5_0 0.33 / 0.53 / 0.55; every argmax
    agreeing. M5.
- **STATED (2026-10-16) - the lasting buffers the gemma leg adds: the resident assistant drafter on both homes, the
  per-layer-embedding planes on the device, the carry's host rows, the concat norm regions and the round scratch.** Not
  measured; the formulas are the record. The drafter is `mtp-gemma-4-26B-A4B-it-Q8_0.gguf` (`dim` 1024, `tdim` 2816,
  4 layers of head sizes 256, 256, 256, 512 - `hs_mx` 512, 16 heads, `hidden` 8192, `vocab` 262144; 419,692,544 q8
  elements) on the 26B-A4B target (`dim` 2816, 30 layers, rope widths 512 + 256, so `cos_elems` 768); the PLE models are
  gemma-4-E2B (35 layers, `dim` 1536, `ple` 256, `per_layer_model_proj` BF16) and E4B (42, 2560, 256, BF16). `nb` is
  `RD_NB_MAX` (16), the N-row command's rows; `regions` the mirror regions, one a stream served at once.
  - Vulkan drafter weights (`dasllama_vulkan_mtp_gemma.das` `vk_buffers_make`), kept while the drafter is attached: the
    q8 planes as 1 byte an element plus an f16 scale a 32-block (34 / 32 bytes an element) and the f32 rows (norms, q
    norms, rope table): 445,923,328 B (425.3 MiB) plus 75,776 B on the 26B's drafter - the device twin of the 440 MiB
    host sidecar row in the MTP depth-N section, which the attach keeps. The repack (`q8_repack`) holds the same 425.3
    MiB on the host until the upload returns, so the attach's host peak holds the sidecar at least twice.
  - Vulkan drafter activations: device `4 x (3 tdim + 2 dim + 2 qdm + 2 hidden + vocab) + 2 tdim + dim + qdm + hidden`
    plus the q8 scale rows (`/ 8` of the four requantized widths), the attention partials (`4 x heads x (hs_mx + 3)`) and
    528 B of ids and pick partials, `qdm` = heads x `hs_mx`: 1,281,040 B on the 26B's drafter, the `4 x vocab` logits
    1 MiB of it; host `4 x (vocab + tdim)` landed logits and hidden, `4 x tdim + 16` input, `4 x cos_elems` rope row,
    4096 B of GEMV meta a site (3 + 5 a layer) and 68 B of step meta: 1,168,468 B. The recorded draft commands are
    `regions x 9` command buffers (one a region and depth, `MTP_MAX_ROWS`) from the driver's pool, plus `regions x 9`
    bytes of recorded flags.
  - Metal drafter slots (`dasllama_metal_mtp_gemma.das` `gemma_slot`): `4 x tdim + 8 x hs_mx` bytes a speculating
    stream (its carried hidden and the two rope classes' cos/sin rows), grown to the widest tick: 15,360 B a stream on the
    26B, 61,440 B at four streams; the tick's token rows `bvtok` hold `4 x` the rows at most doubled, 288 B at four
    streams of nine rows.
  - Vulkan `ple_wp_dev` (`dasllama_vulkan_prefill.das` `vk_rdec_set_ple_proj`): the PLE projection on its own rows,
    `n_layers x ple x dim` elements at 2 bytes (BF16) or 4 (F32), beside the window's f16 panel (`ple_w16_dev`, 2 bytes an
    element) where the shape admits it: 27,525,120 B (26.25 MiB) on E2B, 55,050,240 B (52.5 MiB) on E4B.
  - The F32 PLE planes, on a file whose gate or projection kind ships F32: `4 x n_layers x dim x ple` bytes a kind, on
    the Vulkan device (`ple_gf32_dev` / `ple_pf32_dev`) and in the Metal region cache (`enc_ple_f32`,
    `pf_enc_ple_site`): 55,050,240 B a kind on E2B, 110,100,480 B on E4B. The Metal window's F32 projection rows
    (`ple_model_off`, a file whose `per_layer_model_proj` ships F32) are the same `4 x n_layers x ple x dim` in the
    region cache.
  - Vulkan `pleact_dev` (`dasllama_vulkan_decode.das` `vk_rdec_set_ple`): `4 x ple x nb` = 16,384 B on E2B and E4B
    (both `ple` 256).
  - Vulkan `hid_host` (`vk_rdec_carry_arm`), reached by an assistant-armed model with no NextN head: `4 x dim x nb`
    bytes, cached host memory: 180,224 B on the 26B, 98,304 B on E2B.
  - Metal concat norm regions (`dasllama_metal_common.das` `upload_region_cat`, `g_regions_cat3`), kept until the region
    cache drains: a residual seam's post and next norm rows, `8 x dim` bytes, at most two a layer (`seam_norms`) -
    860,160 B on E2B; the gemma-4 MoE step's routed, dense, post-FFN and next attention norms, `16 x dim` a routed layer
    and `12 x dim` the last (`moe_g4_norms`) - 1,340,416 B on the 26B, beside its attention seams' 675,840.
  - Session verify rows under the assistant drafter's verify (`gpu_resident_assist_verify`): `mtp_logits_b` `4 x vocab`
    and `mtp_hrows` `4 x dim` a row, nine rows at depth 8 - 9,437,184 + 101,376 B a speculating session on the 26B,
    grown once (the depth-N section's footprint row states the same buffer at Qwen's vocabulary).
  - Module scratch: `g_rdec_av_hid` `4 x dim` (11,264 B on the 26B); `g_vk_cosrow` `4 x cos_elems` (3,072 B);
    `g_rdec_row_ids` 8 bytes a row of one command, 128 B at `nb`; `g_b_grp_nr` 8 bytes a row of a batched Metal step;
    `g_gw_*` and `g_vk_*` 8 bytes a stream each, their token lists `8 x 9` a stream. None passes 12 KiB at four streams.
  - Measured - the Metal release pair (the binding reads `contents` inside an autorelease pool; `REVIEW_MEMORY.md`'s
    before/after rule). The shape: the support matrix's `fam-gemma4` arm under `DASLLAMA_PARITY_FULL=1` on the M5 Max
    (128 GB) - gemma-4 12B Q4_K_M and Q8_0, 31B Q4_K_M, the 26B-A4B in six formats, E4B and the E2B rows, a planar load
    and its blob twin a row, the next row's load the only release between rows. Before: every blob and plane buffer a row
    uploaded stayed resident after its release - the IOAccelerator category 9 -> 22 -> 45 -> 113 -> 174 GB row over row -
    and the per-process limit killed the run 9 minutes in at a 193 GB footprint. After: a row's device memory leaves at the
    next load; the footprint peaks at 75 GB (`phys_footprint_peak`, the 26B Q8_0 row's load; the 31B row holds 71 GB, its
    planar load beside its blob twin - 47 GB host, 22 GB IOAccelerator, the most IOAccelerator any 5 s sample read) and the
    arm passes whole in 433 s. One E2B load-serve-delete cycle closed at the drivers' shutdown (`metal_release_row`) adds
    5,435 MB before and 30 MB after.

- **STATED (2026-10-16) - two conclusions of the 2026-09-21 MoE-carriers entry are replaced.** Its 15.4 GB served-weight
  figure for gemma-4-26B-A4B UD-IQ3_XXS named "demoted down-expert rows": the 704-wide down-expert rows serve in their own
  per-32 formats, nothing demotes them (`ARCHITECTURE_ENGINE_FORMATS.md`), so the 15.4 GB is the served weights at their
  own formats inside a plan figure that tallied each routed layer's dense shared expert twice. Its "fits no plan on this
  card" conclusion for the same file on a 16 GB card is replaced: the host-experts plan serves it under an 8000 MB room
  with the first routed layers' experts on the host and every call served (`tests/test_gpu_resident_gemma4_26b_host.das`,
  arm `host26`, the pod's RTX PRO 4500 under `DASLLAMA_PARITY_FULL=1`); the rate rows of that form land with the leg's
  re-measure entry.

- **STATED (2026-10-15) - the host pool's pages under a speculating device-home scheduler carry the NextN head's rows
  beside the trunk's.** A pooled opening of `n` tokens holds `n - 1` rows of the head's K/V slab (`layer_kv_dim(c,
  n_layers)` halves of K and of V a row on the f16 mirror, `4 * kv_dim` bytes): Qwen3.5-0.8B-MTP (head kv_dim 512) 2 KB a
  row, 2 MB an opening of 1,024 tokens; Qwen3.5-9B-MTP (1,024) 4 KB a row, 4 MB. The pages sit under the pool's own budget
  (`prefix_groups`, `--prefix-state-mb`) beside the trunk's, so the total the pool holds does not grow; each opening
  costs its head rows more and the budget keeps one opening fewer. A plain scheduler on the same carrier keeps the pool
  it had: the head rides no move while speculation is off.

- **MEASURED (2026-10-14) - the K-quant base tile against the IQ4_XS one, and the routed twin pick that made them read a
  third apart.** `benchmarks/matmul/bench_metal_kq_race.das --fmts k4,iq4xs --tiers mm --rounds 5` on the M5 Max (synthetic
  planes, 512 rows, `direction-grade`, cv under 3 %): the simdgroup base tiles read within 2 % at every shape - k4 / iq4xs
  6958 / 7112 GMAC/s on the 2048 x 8192 up plane, 6954 / 7106 on the 5120 x 8192 qkv, 7080 / 7203 on the 5120 x 17408 up,
  6585 / 6649 on the 8192 x 2048 down - so the K-quant decode costs the tile nothing the codebook lookup does not. The third
  came from the pick: `benchmarks/prefill_window_probe.das --windows 1024 --kprof 1024` on the 35B (`DASLLAMA_METAL_TENSOR=0`,
  32 rows an expert, the M5 Max) put the IQ4_XS-pure file's experts on `MetalMoeMulMmIq4xsTH` (53 + 30 ms of a 271 ms window)
  and the UD-Q4_K_M file's on `metal_moe_mulmm_k4` / `k5` (145 + 70 of 403): the K-quant twins took their twin under a crown
  or the knee, every other format's whenever it compiled. With one rule for every format the same arm reads base against base -
  `metal_moe_mulmm_iq4xs` 143 + 71 ms, window 394 ms, against 145 + 70 and 403 - and at 375 tokens (11.7 rows an expert,
  under the knee) both files ride their twins. The M1 Max (no tensor lane), its profile re-minted with every format's own
  `moe_mulmm_<fmt>` race: k4, k5, k3, k2, iq2s, iq2xs and iq2xxs crown their routed twin, iq4xs, iq3s4, iq4nl, q40, iq3s,
  iq3xxs and k6 decline it; the UD-IQ4_XS 35B's IQ3_S experts fall off `MetalMoeMulMmIq3sTHR` onto `metal_moe_mulmm_iq3s` and
  the 1024-token window reads 1115 to 1065 ms, the Q4_K-pure file's 1029 on both trees (k4 crowned there). The M5 Max crowns
  every routed family; its 1024-token window on the IQ4_XS-pure file reads 215 ms crowned on both trees.

- **MEASURED (2026-10-13) - the qwen leg's end re-measure: every plane on its own bytes against the stored q8 and f16
  copies, MTP, the greedy chain, the per-op dense-FFN rail, on the house file `Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf`
  (and Llama-3.2-3B Q4_K_M), the M5 Max and the RTX PRO 4500 (Vulkan), the leg against its base `fa7ce42649`.** Every
  reading in this entry is `out-of-process` and `direction-grade`: a client's clock times a server process, and each
  reading sets the leg against its base in another process.
  - `harness/served_bench.das` as `ARCHITECTURE_MEASUREMENT_SERVED.md#served-turn-instrument` runs it (`--sizes
    3000,9000 --reply 200 --reps 5`, cv under 3 % unless marked) against `utils/dasllama-server/main.das` at one stream
    and `--ctx 16384`, Metal on the M5 Max, Vulkan on the RTX PRO 4500 pod; the server's tier, its `DAS_TUNE_POLICY`, the
    CPU kernel backend and, on the pod, `DASLLAMA_COOPMAT` are not recorded.
  - M5, uncrowned on both trees (the committed M5 profile carries no tensor crowns): native non-expert planes lift
    served decode 150.6 to 153.9 (3k) and 140.2 to 145.9 (9k) and cost the window: served cold TTFT 846 to 1383 ms at 3k
    and 2942 to 4302 at 9k, the window's own GPU time 623 to 1032 ms at 2967 rows; the base's image carried 318 baked
    dev-W f16 panels (2761 MB) that the K-quant and Q8_0 window GEMMs read, and the deltanet planes and the shared expert
    as stored q8 copies; the per-forward scratch panel arm (`DASLLAMA_METAL_DEVW=1`) reads 1301 ms. MTP at one stream on
    Metal on this MoE costs decode (served 157/148 with the server's `--mtp` round on, 178/168 off); at four streams it
    is a wash on both trees.
  - Vulkan, 35B: served decode 147.9 to 159.8 (3k) and 141.2 to 151.6 (9k) with MTP off, prefill 4294 against 4158
    tok/s. With the server's `--mtp` the base drafted on the CPU (TTFT 18.6 s, 18.6 tok/s) and the leg drafts on the
    device (746 ms, 176.6 tok/s at cv 8 % - past the 3 % line, a row the instrument prints VOID); the shipped
    `serve_mtp = "vulkan"` (`performance/model_specs.das`) rests on that VOID reading. The greedy chain
    (`DASLLAMA_VK_CHAIN=1` against unset) reads 157.4 against 159.8 and adds 6 ms of warm TTFT, +1.5 % on the 3B - off by
    default. Llama-3B whole-model: 202 to 210 tok/s (3k). The per-op tier's dense-FFN rail (`DASLLAMA_GPU_DENSE_FFN=1`
    against `=0`): the 3B decodes 29.2 against 17.4 tok/s and prefills 274 against 153 with it on - on by default.
  - The house replay is an instrument outside the tree: a client replaying a home assistant's recorded requests against
    `utils/dasllama-server/main.das`, a warm and a cold pass. Every figure it read is `external`. Its command line, the
    server's tier, `DAS_TUNE_POLICY`, CPU kernel backend and, on the pod, `DASLLAMA_COOPMAT` are not recorded in the
    tree, and no `ARCHITECTURE_MEASUREMENT*.md` section carries the recipe of the llama-server rows beside it (version,
    command line, draft head).
  - M5, uncrowned: the short turns' steady gen 118 to 146-150 tok/s; short-turn TTFT with the scratch panels matches the
    base (0.232 against 0.235 s), without them 0.355 against 0.287 on the M4 arm. The pod, warm pass (cached / cold /
    TTFT med, p90 / steady): Llama-3B 86 % / 0 / 0.13, 0.28 / 204 (the base 0.20, 0.98 / 196; llama-server 93 % / 0.11,
    0.15 / 207); the 35B device-home 67 % / 33 / 0.22, 1.48 / 140 (the base 0 % / 98 / 1.40, 1.70 / 129; llama-server
    with its draft head 88 % / 0.51, 0.91 / 162), with `--mtp` 64 % / 36 / 0.30, 1.59 / 159; its pair phase reads 39
    tok/s steady a stream (29 with `--mtp`) against 180 on the 3B. Open from these readings: the Metal window's
    own-bytes sites (a per-window scratch panel from the row floor), the device-home cache rate against the M5's 91 %
    (the card holds one parked claim and four checkpoints a region, the host prefix cache a quarter of RAM), the 35B
    pair phase on Vulkan.
  - With the Metal window on per-window scratch panels and the box crowned (the minted M5 profile on both trees), two
    alternated rounds, each round's figure listed, leg against base: cached 91 % / 0 cold on both; TTFT med 0.28, 0.25
    against 0.24, 0.23 s; steady gen 120, 136 against 111, 114; short alone wall 0.335, 0.312 against 0.336, 0.334,
    TTFT 0.263, 0.254 against 0.245, 0.242, steady 135, 140 against 114, 115; pair wall 0.643, 0.655 against 0.690,
    0.685, steady 109, 102 against 97, 98 - decode and the pair ahead, the short TTFT 10-18 ms behind (the base form's
    dequant passes and the exact f32 router tile against the base's stored half copy).
  - With the host pool behind the device-home streams (`--prefix-state-mb`, a quarter of RAM or the container's limit -
    14,848 MB on the pod), the 35B device-home replay on the pod reads 93 % cached / 0 cold on the warm pass and 86 % / 7
    on the cold pass (the 7 are the openings nothing precedes, the M5's floor too) against the M5's 91 % / 0; TTFT med
    0.25, p90 0.59 s against the card-only 0.24, 1.47 - the pool costs about 10 ms at the median; the pool held to the
    card's own bytes (1,700 MB) reads 86 % / 7. An opening of 4.2-4.8k tokens comes up from the pool in 10-16 ms, a
    checkpoint goes down in about 15 ms, a new conversation forcing three copies and an upload pays about 64 ms at
    admission. 22 of 124 warm replies differ from the card-only run: the attach position moves, and the split prefill's
    routed numerics with it.
  - The M4 arm (`DASLLAMA_METAL_TENSOR=0`, served at one stream): the 35B IQ4_XS, two alternated rounds,
    leg against base, each round's figure listed - decode 188.9, 176.0 against 158.4, 150.0 (3k) and 175.2, 166.3 against
    147.8, 141.4 (9k); cold TTFT 733, 934 against 726, 850 ms (3k) and 2710, 3123 against 2679, 2952 (9k) - decode 18 %
    ahead on the base forms, the cold window inside the rounds' own drift (the second round slower on both trees). The
    same arm on the leg across the file's forms, one round each: UD-Q4_K_M (Q4_K / Q5_K experts, Q8_0 rest) cold 1042 /
    3662 ms, decode 139.1 / 131.6; Q8_0 cold 2001 (cv 19 %) / 5229 (cv 3.6 %) ms, decode 96.8 (cv 13 %) / 104.2 (cv
    6.5 %) - every Q8_0 row and a second UD-Q4_K_M round sit past the 3 % line (the box after an hour of back-to-back
    loads), so the Q8_0 column is a direction only; the IQ4_XS file is the M4 arm's pick on both axes.
  - The same arm, the box idle, two alternated rounds of three files minted from unsloth's BF16 shards (`llama-quantize
    --tensor-type 'ffn_(gate|up|down)_exps=<fmt>' <bf16> <out> Q8_0`: the routed experts in the named format, every
    other plane - the shared expert, attention, deltanet, token table, classifier and the NextN head - Q8_0; no
    imatrix): pure IQ4_XS (18.95 GB) cold 730, 865 / 2631, 2950 ms, decode 189.2, 182.2 / 175.6, 169.9; Q4_K experts +
    Q8_0 rest (21.29 GB) cold 1037, 1239 / 3942 (cv 6.6 %), 3947 ms, decode 143.6, 139.8 / 133.7, 133.6; IQ4_XS experts
    + Q8_0 rest (20.26 GB) cold 948 (cv 3.4 %), 879 / 3079, 2946 ms, decode 139.1, 142.0 / 133.4, 134.9. The Q8_0 rest
    costs a quarter of the decode and a fifth to a third of the cold window on the bandwidth-bound arm: an A3B's dense
    planes are most of a token's bytes (about 1.9 GB a token at Q8_0 against 1.0 at 4 bits, the eight routed experts
    0.55), and the pure file's window reads the 4-bit experts and dense planes on its own bytes at the q8 tile's rate.
  - Pure Q4_K (`llama-quantize --pure`, 20.06 GB, 4.5 bpw) against pure IQ4_XS on the same arm, two alternated rounds:
    decode 187.3, 177.3 / 174.4, 166.9 against 188.6, 182.4 / 168.8, 169.8 - the two within the rounds' drift, so the
    IQ4_XS lookup costs the bandwidth-bound decode nothing; prefill 2903, 2367 / 2456, 2172 against 4092, 3498 / 3220,
    3023 tok/s (the Q8_0-rest twins above show the same gap on their expert planes: 2236 against 2862 at 9k). That arm
    read the Q4_K experts on their base tile against the IQ4_XS experts on their tensor twin, not tile against tile: the
    routed pick took a K-quant twin under a crown or the knee and every other format's twin whenever it compiled, and
    the base tiles themselves race even (the 2026-10-14 entry on the routed twin pick).
  - The same six files on the M5's own arm (the box profile re-minted, its tensor crowns applied), two alternated
    rounds, prefill 3k / 9k tok/s then decode 3k / 9k: pure IQ4_XS 5117, 4502 / 4411, 4072 and 189.0, 184.5 / 175.8,
    171.0; pure Q4_K 4627, 4651 / 4153, 4205 and 182.6, 182.7 / 168.5, 169.3; Q4_K + Q8_0 rest 4394, 4531 / 4047, 4132
    and 139.8, 141.2 / 132.6, 134.0; IQ4_XS + Q8_0 rest 4355, 4406 / 4002, 4036 and 142.4, 142.9 / 135.1, 135.9;
    UD-Q4_K_M 4401, 4471 / 4061, 4099 and 134.7, 135.0 / 127.9, 128.5; Q8_0 (no NextN head) 4113, 4140 / 3809, 3811
    and 128.0, 128.3 / 122.2, 122.3. The tensor twins close the K-quant window gap (pure Q4_K within 10 % of pure
    IQ4_XS, every file within 20 %), so the base-form tile's third is the M4 class's alone; decode is the M4 arm's
    figure for every file - the crowns move the window, not the bandwidth-bound step.
- **MEASURED (2026-10-13) - the Metal routed base form's bars (`tests/test_metal_prefill_parity.das`,
  `test_metal_prefill_moe_base_form`, the `moe-base` arm).** A `tests/run.das` cell; the GPU side is the Metal prefill
  window with the gather panel held off (`set_metal_moe_gather_min(1000000)`, so the gate and up sites ride the base form
  `MetalMoeMulMmKqT`, every superblock and per-32 format, the form a box without the tensor toolchain serves) against the
  same window with the panel built from one token (`set_metal_moe_gather_min(1)`) on the tensor twins, each leg against
  the CPU chain's rows; the witness is the base kernel's dispatch count. Bars: `Qwen3-30B-A3B-Instruct-2507-UD-Q3_K_XL.gguf` 0.10 (read 0.042 on both legs),
  `Qwen3-30B-A3B-Instruct-2507-UD-IQ2_XXS.gguf` 0.15 (read 0.095 on the twins, 0.046 on the base form) - the M5 Max,
  `-jit`, one model process. The IQ2 carrier's bar sits above the twins' own reading on this file; the base form reads
  under half of it.
- **MEASURED (2026-10-13, `direction-grade`) - the shared row templates' stamps against the per-home bodies they
  replaced.** One dispatch alone at a qwen35moe / qwen4exp decode shape, in us, the per-home body read on the tree before
  the templates landed against the stamp on the tree after, by the same instrument; each row the median of its rounds at
  under 3 % cv (the instruments print a row past it VOID). M5 Max, Metal:
  `bin/daslang -jit dastest/dastest.das -- --bench --test modules/dasLLAMA/benchmarks/ew/bench_metal_gk_rows.das`
  (at least 9 rounds of 200 back-to-back dispatches). RTX PRO 4500 Blackwell pod, Vulkan:
  `bin/daslang -jit modules/dasLLAMA/harness/vk_gk_rows_probe.das` (64 dispatches a command, 8 submits a round, 9 rounds,
  the device clock, which steps by 2.05 us at these sizes). `DAS_TUNE_POLICY` and, on the pod, `DASLLAMA_COOPMAT` are not
  recorded. M5 before / stamp, then Vulkan before / stamp:
  - hc norm (dim 2048, hc 4, 1 row) 3.87 / 3.87, 4.19 / 4.19; hc mix 1.75 / 1.63, 4.06 / 4.06; hc combine 1.45 / 1.50,
    2.50 (the MoE arm) / 2.16; ple gate (the ring row) 11.05 / 11.13, 6.17 / 6.50; ple conv 2.38 / 2.25, 4.10 / 4.09.
  - q/k norm (hs 256, q 16, k 2) 1.57 / 1.71, 2.40 / 2.40; select (256 experts, k 8) 3.05 / 2.81, 6.14 / 6.14; select
    (512 experts, k 10) 5.51 / 3.67, Vulkan stamp 6.14; routed combine (dim 2048, k 8) 1.70 / 1.92, 4.12 / 4.11.
  - deltanet out-norm (32 heads of 128) 1 row 1.28 / 1.61, 10.24 / 6.14, 512 rows 34.6 / 29.0, 22.5 / 12.1.
  - q/k norm + rope + store (hs 128, q 32, k 8): 1 row f32 against the split pair 3.27 / 1.89, 4.09 / 4.09; 4 rows f16 on
    the M5 3.66 / 1.95; 1 row f16 against the half-twin rope store 1.79 / 1.93; the window's pass with its q twin (512
    rows) 26.9 / 26.5.
  - router (256 experts, dim 2048): 1 column f32 rows 3.82 / 3.37, 4.2 / 4.43 (f16 rows 4.10 / 4.11; the pre-template
    body through the same probe 4.40); 8 columns on Vulkan 14.34 / 14.34 (f16 rows 10.24 / 10.24); 512 columns on the
    M5 141.6 / 144.4.
  - shared gate on the M5: its own GEMV then the routed combine and the gated add 3.08 + 2.93, against 0 + 2.01 as the
    router's row `ne` under the combine's `SH` arm; gated attention's deinterleave + sigmoid epilogue (8 heads of 256)
    2.19 (2.23 at 4 rows) against 0.
  - Pocket row GEMV (1024 x 1024) 9.67 / 9.79, 5.4-5.8 / 5.2-5.7; with LayerNorm + SiLU 12.79 / 12.99, 6.14 / 6.14.
  - With the subgroup reductions behind calls rather than `[spirv_inline]`, the Vulkan stamp read 1.9 us slower. The
    widest move is the 1-row out-norm's 0.33 us on the M5 against the 512-row window's 5.6 us back.
  - The pair rulings the shared-kernels doc carries beside the table, same instruments: the router's two lane splits on
    the M5 - a workgroup a row 3.37 against a subgroup a row 3.82 at one column, a subgroup a row 144 against a workgroup
    a row 745 at 512 columns; the deltanet conv + L2 fold on the M5 at a 512-row window 83 against the fused 75 (a PB of
    32 reads 108), at 8 rows 3.37 against 3.81 and at 16 rows 4.77 against 5.00, so `DN_CONV_FUSED_ROWS` sits at 16;
    the one MoE tile walk under both Metal scaffolds within 0.5 % of the per-scaffold walks (the tile lab, two
    alternated rounds, best of three; the lab's command line is not recorded); the Vulkan cm2 tile's unsplit k-loop
    beside the split one on the pod, `harness/vk_gemm_probe.das` `cm2:k4`, two alternated rounds: the k4 l and m
    stamps 1.2 to 1.5 % slower with the split arm alone (gate 0.190 to 0.193 ms, down 0.184 to 0.187 ms).
- **STATED (2026-10-13) - the lasting buffers the device-home checkpoints, the host-cached claim stash and the Metal
  recurrent pool hold.** Not measured; the formulas are the record, f32 recurrent state, `L` the recurrent layers, `nvh`
  the value heads, `ds` the state size, `cd` the conv width (`2 x groups x ds + inner`), `taps` the conv kernel less one.
  The 35B is `Qwen3.6-35B-A3B` (`L` 30, `nvh` 32, `ds` 128, `cd` 8192, `taps` 3, 10 attention layers of a 512-wide K/V),
  the 0.8B `Qwen3.5-0.8B` (`L` 18, `nvh` 16, `ds` 128, `cd` 6144, `taps` 3, 6 attention layers of a 512-wide K/V).
  - Vulkan `snap_dev` (`dasllama_vulkan_decode.das` `rd_snap_alloc`; budgeted by `rdec_dn_snap_bytes`): regions x
    `RDEC_DN_SNAPS` (4) x `L` x (`nvh x ds x ds` + 2 x `cd x taps`) x 4 bytes, allocated at the first checkpoint save and
    kept for the driver's life: 262.5 MiB a region on the 35B, 82.1 MiB on the 0.8B.
  - Vulkan `g_rdec_stash` (`dasllama_gpu_resident.das` `rdec_stash_region`): one entry a host-cached session whose region
    changed hands while it held rows its cache lacked, kept until the session takes a region again or is released; an
    entry is 2 x the attention layers x the stashed rows x the mirror codec's row bytes (K and V), plus the NextN head's
    rows past its host count - at an f16 mirror 20 KiB a row on the 35B (320 MiB at 16384 rows) and 12 KiB a row on the
    0.8B (192 MiB at 16384), so it scales with context and with the sessions that lost a region.
  - Metal `g_dn_pool` (`dasllama_metal_common.das` `dn_pool_fit`): slots x a slot of 3 regions x `L` x (`nvh x ds x ds`
    + `cd x taps`) x 4 bytes (plus a gemma PLE ring), the slots doubling up to the widest batch served (`DN_MIRRORS_MAX`
    4 at least): a slot is 188.4 MiB on the 35B (753.8 MiB at four slots), 57.8 MiB on the 0.8B (231.2 MiB at four);
    `g_dn_stash` keeps one region a mirror evicted past the cap, 62.8 MiB on the 35B and 19.3 MiB on the 0.8B.
- **MEASURED (2026-10-12) - the router race against the f32 tile that replaced the half panel.** M5 Max,
  `bin/daslang -jit modules/dasLLAMA/harness/router_race.das`, one process, no flags, 2048 positions x 256 experts over
  2048, best of its rounds; it now races the router's f32 tile (`MetalF32ExactMm`) where the 2026-10-01 entry's half
  tensor GEMM arm (`MetalHalfMulMmTH128`) ran, that arm gone with the half panel. The slab GEMV (`MetalMoeRouterB`)
  0.646 ms, off the double dot by 8.1e-7; the f32 tile 0.174 ms, x3.72, off by 8.3e-6.
- **STATED (2026-10-12) - the hybrid same-slab verify serves.** The 2026-10-05 entry's conclusion that the same-slab
  probe declines at two rows on its hybrid no longer holds: a same-slab batch on a hybrid is one row group over the
  session's recurrent mirror, so the batch rail serves the hybrid verify. That entry's two-row figure stays the
  two-session batch step it measured.
- **STATED (2026-10-13) - the 2026-10-02 qwen ladder's deltanet K-quant rows are not like for like.** The deltanet
  hybrids' K-quant labels in that entry (Qwen3.5-9B-MTP UD-Q5_K_XL, Qwen3.6-27B-MTP Q4_K_M, Qwen3.6-35B-A3B-MTP
  UD-Q4_K_M, and the entry before it's Qwen3.8-27B UD-Q3_K_XL) name the file, not the served form: the Metal drivers of
  that date read every deltanet plane, and the 35B's shared expert, through their q8 transcodes, so those rows measure a
  mixed q8 serve against the reference's native one.

- **MEASURED (2026-10-10) - the house replay on the M5 Max: a reply's tail, the pair's re-prefill, the uncrowned
  sidecar.** The instrument is outside the tree (`external`, `out-of-process`): the house replay's runners,
  `BOX=m5 ./run_replay.sh <tag>` and `BOX=m5 ./run_short.sh <tag>` in the private `dasllama-bench` repository, a
  client replaying a home assistant's recorded requests against `utils/dasllama-server/main.das` under `-jit`,
  `--ctx 32768 --streams 4`, `DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/m5.tune.json`, Metal, the model
  `Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf`; its walls and rates stay in that repository's ledger. What it decided,
  master `9f285a9339` against this change (`direction-grade`): every reply's finished event waited for the
  finishing tick's remaining work - the scheduler reaped a finished stream inside the tick that finished it
  (the close tokens' 2-token prefill, the recurrent snapshot, the donation) and still ran another stream's
  prefill chunk before the server flushed; the reap now runs at the next tick's start and a tick that finished a
  stream ends before its chunk (`dasllama_scheduler.das`, `ARCHITECTURE_ENGINE_SERVING.md#scheduler-step`), and
  the client's tail after a reply's last token fell to the wire's floor. Two short requests admitted on one tick
  both attached the cache before either had prefilled, so the second prefilled the shared opening again: the
  server's `usage` read a median 582 uncached prompt tokens a request against 375 for the same request alone;
  admission now waits while a stream is still prefilling (it would wait for those chunks anyway) and the second
  attaches the first's checkpoints - median 401 uncached. Both are structural: no timing chose a value, the
  instrument confirmed the mechanism. Also found: the tune verdict named a sidecar binary-stale before reading its
  box, so on a box whose app sidecar is another box's (this M5's `~/.dasllama/tune/main.tune.json`, the M1's)
  the foreign runtime knobs applied and no tensor crown did; the verdict reads the box first (`llvm_tune.das`).
  `DASLLAMA_METAL_TENSOR=0` is the arm that serves every family on its base form, as a GPU without the tensor
  lane does.

- **MEASURED (2026-10-09) - the gathered route against the tiles, and the K-quant tail peel against the tile, each an
  interleaved A/B in one process on the M1 Max.** `benchmarks/prefill_window_probe.das --ab`: medians of 4 reps an arm, the
  arms alternating after one warmup rep of A, every window appended in sequence past the warm prefix, the figures the Metal
  driver's GPU timestamps; `DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/m1.tune.json bin/daslang -jit
  modules/dasLLAMA/benchmarks/prefill_window_probe.das -- -m <gguf> --prefix <P> -r 4 --ab <knee|tail> --windows <list>`,
  `DAS_TUNE_POLICY` unset, Metal on the M1 Max (MacBookPro18,2, 64 GB). The knee A/B on Qwen3.6-35B-A3B-MTP Q4_K-pure, prefix
  4500: A = the gathered route (`set_metal_moe_gemv_max(512)`), B = the tiles (`set_metal_moe_gemv_max(0)`), GPU ms a window:

  | window | route | tiles | route / tiles |
  |---|---|---|---|
  | 2 | 22.7 | 34.3 | 0.66 |
  | 4 | 28.0 | 43.3 | 0.65 |
  | 8 | 43.1 | 60.4 | 0.71 |
  | 12 | 57.3 | 77.4 | 0.74 |
  | 16 | 66.4 | 84.3 | 0.79 |
  | 24 | 84.5 | 97.6 | 0.87 |
  | 32 | 102.4 | 109.3 | 0.94 |

  On the M1 the route wins through 32 rows (the decode step at that position 11.9 ms). The same A/B on the M5 Max (the
  box's own sidecar `~/Work/daScript/modules/dasLLAMA/performance/m5.tune.json` as `DAS_TUNE_MANIFEST`, `-no-module-cache`,
  a whisper `dasllama-server` idle on the GPU) on the IQ4_XS-pure file of the same model, the decode step 6.5 ms:

  | window | route | tiles | route / tiles |
  |---|---|---|---|
  | 2 | 9.6 | 13.0 | 0.73 |
  | 4 | 11.3 | 16.2 | 0.70 |
  | 8 | 16.3 | 23.3 | 0.70 |
  | 12 | 22.7 | 31.3 | 0.73 |
  | 16 | 25.4 | 33.9 | 0.75 |
  | 24 | 31.1 | 39.1 | 0.79 |
  | 32 | 36.7 | 42.7 | 0.86 |

  The shipped knee of 16 (`pf_moe_gemv_knee`) is the value every carrier wins at on both boxes - the M5 sweep of the
  2026-10-09 knee entry below loses at 32 on nine of eleven carriers, the 35B hybrids among the two that win - and this race
  brackets it at 12 and 24. The tail A/B on the dense Qwen3.6-27B-MTP Q4_K_M, prefix 2000: A = the K-quant tail peel on
  (`set_metal_prefill_mm_tail(true)`), B = off, GPU ms a window, the M1 Max then the M5 Max under the same flags:

  | window | M1 peel | M1 tile | peel / tile | M5 peel | M5 tile | peel / tile |
  |---|---|---|---|---|---|---|
  | 2 | 82.0 | 275.3 | 0.30 | 38.2 | 95.9 | 0.40 |
  | 3 | 102.1 | 275.2 | 0.37 | 42.2 | 96.3 | 0.44 |
  | 4 | 106.0 | 275.7 | 0.38 | 41.8 | 95.9 | 0.44 |
  | 5 | 175.9 | 276.1 | 0.64 | 73.3 | 96.3 | 0.76 |
  | 8 | 181.7 | 277.0 | 0.66 | 73.5 | 95.9 | 0.77 |

  The tile costs the same whatever the window holds - one 32-row tile a site, 275-277 ms on the M1, 96 on the M5 - and the
  peel steps with the GEMV form it takes (the two-row form at 2, the four-row at 3-4, the eight-row at 5-8); the decode step
  at that position is 62.9 ms on the M1 and 35.6 on the M5, so a two-token window on the peel is 1.3 (M1) or 1.1 (M5) decode
  steps against the tile's 4.4 or 2.7.

- **MEASURED (2026-10-09, `direction-grade`, `debug-jit`) - where a Qwen3.6-35B-A3B decode step's GPU time goes on the M1
  Max: the routed expert GEMVs are 1.9 ms of a 10.6 ms step, at the memory wall.** This entry replaces the 2026-10-06
  equal-bytes entry's reading that the q8 kernel's shape is the difference between the 4-bit and the q8 files, and its
  citation of `followup_metal.md` 45 (the row is deleted; its successor is row 52). M1 Max (MacBookPro18,2, 64 GB), Metal,
  one stream, the pure IQ4_XS file of the 2026-10-06 entry; `benchmarks/lcpp_bench.das` as the `-jit` script, `-m <gguf>
  --ngl 99 -n 64 -r 2 --mtp-ab --prof --for-debug-purposes` under `DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/m1.tune.json`
  and `DAS_LOG_LEVEL=info` (the decode driver's stage report is `LOG_INFO`), `DAS_TUNE_POLICY` unset, decode = the tg-real64
  row, MTP off. The step's stage report: setup 5 us, encode 578 us, wait 11047 us of which the GPU is busy 10638 us, readback
  9 us - GPU-bound, 10.6 ms of GPU time a token. The knockout (the decode driver's `moe_rt` stage drop, which skips the
  routed expert GEMVs and leaves the router and the select live; its output is garbage, so the arm is timing-only) reads
  100.7 tok/s against the full step's 84.4 (3 reps each, interleaved in one process): the expert GEMVs are 1.9 ms of the
  step for about 0.8 GB of expert bytes a token, 420 GB/s - the M1 Max's wall - and an infinitely fast 4-bit expert kernel
  would gain 19 % at most. The other 8.7 ms move about 0.7 GB (the attention-side and deltanet projections, the 248k x 2048
  classifier, the router, the norms and the recurrence kernels) at about 80 GB/s through some 600 dispatches a token with
  their gaps. The 2026-10-06 entry's 160 GB/s is the end-to-end average over the whole step, not the expert kernel's rate.

- **MEASURED (2026-10-09, `direction-grade`) - the K-quant MoE twins against the base form they were gated behind, and the
  pick that came of it, on the M1 Max and the M5 Max.** The kq-scaffold twins (k4, k5, k6; q5_1 and iq4nl32 under k4's
  verdict) compiled only where the DENSE `kq_mulmm_k*` crown stood, so on the M1 (its sidecar carries none) every routed
  K-quant expert site ran the simdgroup `metal_moe_mulmm_k4` form and the adaptive op never reached it: the house replay
  on the Q4_K-pure 35B read the same before and after ADAPT shipped (`external`, `out-of-process`: the house assistant's
  recorded 124 requests replayed by its out-of-tree harness against `dasllama-server --ctx 32768 --streams 4` on this box,
  two passes, the warm one scored by its scorer; the 112 short answers of 64 completion tokens or fewer: wall 0.90 -> 0.92 s,
  first token 0.75 -> 0.75, the windows' GPU sum 679 -> 682 ms off the server log). The tile lab's `base` arm
  (`bin/daslang -jit modules/dasLLAMA/benchmarks/matmul/bench_metal_moe_tile_lab.das`, no model, `DAS_TUNE_POLICY` unset,
  the M5 under `DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/m5.tune.json`; the production builder `pf_enc_moe_mm_k4_c`,
  f32 x and the identity bucket map, the same planes and patterns as the twin arms):

  | box | rows an expert | base form | twin + adaptive op | twin / base |
  |---|---|---|---|---|
  | M1 Max | 4 / 8-16 / 32 | 2.38 / 2.38 / 2.38 ms (63 GB/s) | 1.42 / 1.80 / 2.76 | 0.59 / 0.76 / 1.16 |
  | M5 Max | 4 / 8-16 / 32 | 1.31 / 1.31 / 1.31 ms (115 GB/s) | 0.40 / 0.42 / 0.49 | 0.31 / 0.32 / 0.38 |

  Bit-equal on every live element. The base form is flat in live rows like the fixed tile was; on the M5 the twin wins
  everywhere, on the M1 it wins the house shape and loses whole tiles by 16%, which one crown cannot carry - so the
  twins now compile behind the toolchain probe, each family races its own MoE-shaped `moe_mulmm_k4/k5/k6` (128 experts of a
  1024 x 2048 plane, past the cache; one whole-tile region and one of twelve rows an expert, the outputs compared on the
  whole-tile region) for the crown, and an uncrowned box takes the twin per dispatch at or under a mean of 16 rows an
  expert (`set_metal_moe_kq_twin_avg`, `ARCHITECTURE_GPU_PREFILL_MOE.md#prefill-moe-kq-twin-pick`). The races on the two boxes
  (the read-only race, `DAS_LOG_LEVEL=info`): M1 the base form wins all three (k4 0.115 vs 0.131 ms, k5 0.130 vs 0.143,
  k6 0.125 vs 0.141, the four-expert shape before this entry's race resize), M5 the twin wins all three (k4 0.067 vs 0.042,
  k5 0.070 vs 0.045, k6 0.068 vs 0.046); both sidecars carry the verdicts. End to end on the M1 under its sidecar
  (`DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/m1.tune.json bin/daslang -jit modules/dasLLAMA/benchmarks/prefill_window_probe.das
  -- -m <Q4_K-pure> --prefix 4500 --windows 375 --kprof 375`, `DAS_TUNE_POLICY` unset, medians of 3 after the warmup rep),
  the Q4_K-pure 375-token window: 483 -> 461 ms, the k4 expert twins now `MetalMoeMulMmK4TH` at 218 ms of kernel time
  against the base form's 229 - less than the lab's 0.76 because the real routing is skewed, and an expert past sixteen
  rows pays the whole op. The house replay on the IQ4_XS-pure 35B (the format the house serves; its twins compile behind
  the probe on every box) on the M1, the warm pass (`external`, `out-of-process`, the same harness): wall 0.85 s, first
  token 0.71, the windows' GPU sum 648 ms, 130 ms of it (31%) the windows past each request's largest - the checkpoint
  split (closed by the 2026-10-20 one-window entry); no pre-ADAPT replay of this file exists, the window probe's 554 -> 487 ms is its A/B.

- **MEASURED (2026-10-09, `direction-grade`, `external`, `out-of-process`) - where a short house reply's first token goes on
  the server, M1 Max, the 35B Q4_K-pure.** The house replay's server log (`DAS_LOG_LEVEL=info`, the server's own per-window
  and per-stream lines) beside the client's results (the house assistant's recorded 124 requests, its out-of-tree harness
  and scorer, two passes, the warm one), read by an out-of-tree timeline script into a per-request timeline: the prefill
  windows the server ran with their GPU ms, the server's prefill-done and ttft, the client's first token; the 114 short
  replies (64 completion tokens or fewer), medians: 374 uncached prompt tokens, prefilled in TWO windows (108 of 114 in
  two or more), the windows' GPU time summed 679 ms, prefill done 696 ms, the server's ttft 696 ms, the client's first
  token 752 ms. So the request's CPU side is 15 ms (prefill done less the windows' GPU time; p90 36) and the wire 12 ms
  (client first token less server ttft; p90 582 where a request queued) - the gap above a single window's compute is
  the WINDOW SPLIT, not the CPU: the prefill stops at every planned prefix checkpoint (the stable opening, the system
  text) and runs the rest as its own window, and the windows past each request's largest cost a median 166 ms, 32% of
  the short requests' window time (a 51-token window 178-191 ms, a 7-token one 40, a 16-token one 66, against the
  300-token one at 1.4-1.5 ms a token). Sixteen tokens and fewer ride the gathered route (40-66 ms); seventeen to a few
  hundred ride the tiles, where every populated expert pays a tile whatever its rows - the adaptive op above cuts that
  tile; the split itself is closed by the 2026-10-20 one-window entry.

- **MEASURED (2026-10-09, `direction-grade`; the mlx-lm columns `external`) - mlx-lm 0.32.0 on the house window, beside
  ours, on the M1 Max and the M5 Max.** `mlx-community/Qwen3.6-35B-A3B-4bit` (affine 4-bit, group 64) under mlx 0.32.3, the
  recipe `ARCHITECTURE_MEASUREMENT.md#mlx-window-recipe` (`python modules/dasLLAMA/harness/mlx_window_profile.py --prefix 4500
  --window 375`): a 4500-token prompt cache in 512-token chunks, then the 375-token window timed as served (`mx.eval` of the
  logits and the cache state, the cache restored between reps, median of four after a warmup), then the same window with
  every module class's call forced to evaluate for a per-class exclusive breakdown (the forced pass reads 925.8 ms on the M1
  and 269.1 on the M5 against 659 and 113 served - the sync a call and the lost overlap - so the shares are the attribution,
  the served figure the cost; the class times in ms: M1 `QuantizedLinear` 431.6, `QuantizedSwitchLinear` 314.6, the two norm
  classes 93.4, `Conv1d` 11.5; M5 116.3, 73.8, 32.7, 6.1). Ours is the same window under the adaptive expert tile, the
  IQ4_XS-pure file, the box sidecar (the M5 its real one), the window probe's `--kprof` report.

  | | M1 Max ours | M1 Max mlx-lm | M5 Max ours | M5 Max mlx-lm |
  |---|---|---|---|---|
  | 375-token window as served | 487 ms | 659 ms | 102 ms | 113 ms |
  | dense projections (deltanet qkv / z / out, attention qkv / o, router, shared expert) | 149 ms, 31% | 47% (`QuantizedLinear`, 391 calls) | 23 ms, 22% | 43% |
  | expert GEMMs | 232 ms, 48% | 34% (`QuantizedSwitchLinear`, 120 calls) | 49 ms, 48% | 27% |
  | norms | ~3 ms | 10% (`RMSNorm` + the gated norm) | ~2 ms | 12% |
  | MoE glue (sort, gather, act, scatter) | 13 ms | 6% | 8 ms | 10% |
  | attention | 66 ms, 14% | inside the dense share (10 layers, 115 ms inclusive on the M1) | 8.5 ms | inside the dense share |
  | deltanet (conv, scan) | 28 ms, 6% | 2% (`Conv1d` 11 ms; the scan rides the kernel the norm calls) | 9 ms | 4% |
  | 4500-token prefix | - | 7.1 s (630 tok/s) | - | 1.18 s (3800 tok/s) |

  The reading: on this shape ours is the faster window on both boxes (M1 by 26%, M5 by 10%), and the shapes differ -
  mlx-lm's dense 4-bit `QuantizedLinear` GEMMs are its largest cost (scaled, ~310 ms on the M1 against our 149 for the
  same sites on f16 dev-W panels), its expert GEMMs about ours (~225 scaled against our 232), and its norms are ten times
  ours. End to end on the same M1 (`external`, `out-of-process`: the house replay against each server, two passes, the warm
  one): mlx-lm's short replies at a first token of 0.92-0.94 s and a wall of 1.15-1.19, ours 0.74-0.75 and 0.90. Both servers
  sit ~260 ms above their window compute on a short reply (ours 0.75 against a 0.49 window) - the entry above reads where. The
  house's M4 Pro reading (`external`: the house assistant's own run on its M4 Pro, ours 46 against mlx 78 tok/s on short
  replies, reported, not reproduced here) is not explained by the window compute on either box here; that box runs
  uncrowned unless its first start minted a profile (the M5 under a copied profile read 121 against 107 under its own).

- **MEASURED (2026-10-09, `direction-grade`) - what a routed 32-row tile costs and where, and the adaptive tensor op that
  came of it, on the M1 Max and the M5 Max.** The house's short reply is a ~375-token window over 256 experts at top-8,
  twelve live rows an expert in a 32-row tile, and the window probe (`prefill_window_probe --prefix 4500 --windows 375
  --kprof 375`, the 35B IQ4_XS-pure) read the expert GEMMs at 298 of 554 ms on the M1 and 55 of 107 on the M5 (its real
  sidecar; 121 under the lcpp copy, whose attention ran the plain form) - flat in live rows: the same kernels read 244 ms
  at 4 rows an expert (128 tokens) and 298 at 12. `benchmarks/matmul/bench_metal_moe_tile_lab.das` (no model; the M1 with
  no sidecar read, the M5 under `DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/m5.tune.json`, `DAS_TUNE_POLICY` unset;
  128 experts of a 1024 x 2048 plane, 146 MB past the SLC, the production builder's binds and grid, every dispatch its own
  output, four arms alternating, best of three; ms a dispatch):

  | box | format | rows an expert | fixed 32-row tile | staging alone | tensor op alone | adaptive op | adaptive / fixed |
  |---|---|---|---|---|---|---|---|
  | M1 Max | iq4xs | 4 / 12 / 32 | 2.97 / 2.96 / 2.97 | 1.00 | 2.18 | 1.68 / 2.03 / 2.98 | 0.57 / 0.69 / 1.00 |
  | M1 Max | k4 | 4 / 12 / 32 | 2.76 / 2.76 / 2.76 | 0.80 | 2.11 | 1.42 / 1.80 / 2.77 | 0.51 / 0.65 / 1.00 |
  | M5 Max | iq4xs | 4 / 12 / 32 | 0.52 / 0.52 / 0.52 | 0.33 | 0.17 / 0.18 / 0.29 | 0.43 / 0.45 / 0.52 | 0.82 / 0.86 / 1.00 |
  | M5 Max | k4 | 4 / 12 / 32 | 0.49 / 0.49 / 0.49 | 0.31 | 0.16 / 0.17 / 0.29 | 0.40 / 0.42 / 0.49 | 0.82 / 0.86 / 1.00 |

  The tile's cost does not move with its live rows. On the M1 it is the 32-row tensor op (three quarters of the tile;
  the staging a third, the two overlapping), so an op sized to the live rows - eight for one to eight, sixteen for nine
  to sixteen, the `ADAPT` template constant of both MoE scaffolds, now the shipped default - takes a third off the house
  tile; on the M5 it is the staging (two thirds, ~450 GB/s over the planes, near the box's memory), so the adaptive op
  takes 14-18% and the staging is the next lever there. The adaptive op is bit-equal to the fixed tile on every live
  element in every cell (the op's per-element sums are the same at any height), and a 32-row expert pays nothing. End to
  end the M1 house window read 554 -> 487 ms, its expert kernels 298 -> 232 (real routing leaves some experts past
  sixteen rows, so the window takes less than the lab's 0.69). The reading on the same window in the Q4_K-pure 35B:
  483 ms, its expert kernels 229 (the base `metal_moe_mulmm_k4` form, the M1 carrying no twin crown) - 13% under the
  IQ4_XS window's pre-adaptive 554 ms, 1% under its adaptive 487.

- **REFERENCE (2026-10-09) - the prefill expert GEMM's shape in llama.cpp and MLX, read from their sources (external:
  llama.cpp 98c4764b6 `ggml/src/ggml-metal`, MLX 77bf1fa `mlx/backend/metal`, mlx-lm 0.32.0).** Ours: a 32-row tile of four
  simdgroups (128 threads), W dequantized into threadgroup memory 64 x 64 a K chunk under two barriers, x streamed from
  device, `npos / 32` tiles launched an expert with the empty ones exiting. llama.cpp (`kernel_mul_mm_id`, taken from 32
  tokens total, below that one GEMV a (token, slot)): a 64 x 32 x 32 tile of four simdgroups, tiles over every token an
  expert with an exit past the expert's count (our launch shape), x gathered through `ids` and converted into threadgroup
  memory, a partial tile computing the full MMA with the write-out clipped, two barriers a K=32 step, no double buffer.
  MLX (mlx-lm `SwitchGLU`, from 64 routed entries: argsort by expert, x gathered contiguous in expert order,
  `gather_qmm_rhs` once the mean rows an expert pass 4): a TIGHT grid - one padded tile run an expert, `ceil(M / bm) + E - 1`
  tiles in all, each expert padded to the next `bm` alone, the tile found by a prefix sum over the experts' offsets; the
  non-NAX tile (M1 to M4 class) is 16 rows x 32 cols x 32 K on TWO simdgroups (64 threads), x and W both staged into
  threadgroup memory, two barriers a step, no double buffer, a 12-row expert padded to 16; the NAX tile (M5 class) is 32
  rows (64 past a mean of 64 an expert) x 64 x 64 on four simdgroups of 16 x 32, W alone staged, x read device to
  registers, and a simdgroup with no live rows skips its MMA, so a 12-row expert runs two of its four simdgroups. Neither
  engine double-buffers the staging. The reading: against ours MLX's levers are the 16-row two-simdgroup tile on the
  non-NAX boxes, the tight grid, and the live-row skip on NAX - not the staging pattern.

- **MEASURED (2026-10-09, `direction-grade`) - every K-quant format's half-x rows route under its own
  `kq_rows_half_<fmt>` crown, on the M1 Max and the M5 Max; the house replay and the short-answer bench on the 35B.**
  The per-format race (`race_kq_half`, the last of the decode races: a 5120 x 16384 plane of the format, two, four and
  eight rows in one timed encoder, the box's f32 route the base, the half route - k4's wide forms, every other format's
  small-batch half stamps off one x mixin - the twin; both arms bit-equal, x on the f16 lattice; the kernels suite's
  `test_kq_rows_race_verdicts` log: `bin/daslang -jit modules/dasLLAMA/tests/run.das -- --suite kernels --arm kernels`,
  no model, the box sidecar through the runner's `DAS_TUNE_MANIFEST`). The twins a step acquires for the route are
  `bxh = max(mp, 8) * max(dim, qd_max) * 2` and `bfgh = max(mp, 8) * hid_max * 2` bytes (Llama-3.2-1B at eight rows 32 KB and
  128 KB, Qwen3.6-27B 80 KB and 272 KB), pooled and released with the step. The half route wins every format on both
  boxes; the gain follows the format's decode cost - the k4 nibble halves its time on the M1, the LUT grids (iq4xs,
  iq4nl) move a few percent:

  | format | M1 f32 ms | M1 half ms | ratio | M5 f32 ms | M5 half ms | ratio |
  |---|---|---|---|---|---|---|
  | k4 (wide forms) | 3.378 | 1.829 | 0.54 | 1.013 | 0.724 | 0.71 |
  | k5 | 3.309 | 2.989 | 0.90 | 1.273 | 1.190 | 0.93 |
  | k6 | 3.027 | 2.457 | 0.81 | 1.078 | 0.988 | 0.92 |
  | iq4xs | 3.768 | 3.577 | 0.95 | 1.161 | 1.125 | 0.97 |
  | iq4nl | 3.734 | 3.544 | 0.95 | 1.142 | 1.119 | 0.98 |
  | q40 | 3.052 | 2.503 | 0.82 | 0.865 | 0.805 | 0.93 |
  | k3 | 2.946 | 2.438 | 0.83 | 0.949 | 0.883 | 0.93 |
  | iq3s | 3.647 | 2.839 | 0.78 | 0.976 | 0.918 | 0.94 |
  | iq3xxs | 3.515 | 2.880 | 0.82 | 0.934 | 0.878 | 0.94 |
  | k2 | 2.822 | 2.425 | 0.86 | 0.907 | 0.842 | 0.93 |
  | iq2s | 4.186 | 3.262 | 0.78 | 0.972 | 0.890 | 0.92 |
  | iq2xs | 3.481 | 2.986 | 0.86 | 0.968 | 0.912 | 0.94 |
  | iq2xxs | 3.520 | 2.880 | 0.82 | 0.917 | 0.870 | 0.95 |

  The house assistant's recorded 124 requests replayed against `dasllama-server --ctx 32768 --streams 4` (`external`,
  `out-of-process`: its out-of-tree harness and scorer; two passes, the warm one read; `DASLLAMA_METAL_KQ_HALF` on / off /
  on, a fresh server each): the k4 crown moves nothing
  and nothing regresses - M1 Max, Qwen3.6-35B-A3B Q4_K-pure, the 112 short replies (64 completion tokens or fewer) at a
  wall median of 0.91 / 0.93 / 0.93 s and a first token of 0.74 / 0.78 / 0.80 s, 91% of the prompt tokens cached, 21 of
  22 overheard cases silent on every leg; M5 Max, the UD-Q4_K_M file, 0.40 / 0.41 / 0.43 s and 0.30 / 0.29 / 0.30 s. The
  short-answer bench (the same requests, each phase on a fresh server so a request meets the cache the house flow gives
  it - a median of 375 uncached prompt tokens): alone, M1 0.94 / 0.93 / 0.95 s wall (first token 0.75 s), M5 0.36 /
  0.38 / 0.39 s (0.26 s); two consecutive requests sent at once take 0.94-0.97 of the two alone summed on both boxes -
  no gain, and the second request of a pair misses the first one's prefix (510 uncached tokens a request). A short
  house reply is a 375-token MoE prefill at 20-60 rows per expert - the tiles' latency regime - plus a few single-row
  decode steps; the rows forms serve neither. Fully cached repeats of the same set gain x1.3 from pairing on the M5, so
  the two-row decode step itself batches well.

- **MEASURED (2026-10-09, `direction-grade`) - the k4 half-x rows forms served under the `kq_rows_half_k4` crown, on
  the M1 Max and the M5 Max.** The crown race is the k4 row of the table above (`race_kq_half`, 5120 x 16384, two, four
  and eight rows in one encoder, the box's f32 route the base): M1 Max 3.378 -> 1.829 ms (the half forms at 0.54 of the
  f32 route), M5 Max 1.013 -> 0.724 (0.71); both arms bit-equal (x on the f16 lattice). End to end on
  Qwen3.6-27B-MTP Q4_K_M (a deltanet hybrid, 65 layers, dim 5120, FFN 17408), `DASLLAMA_METAL_KQ_HALF=1` against `=0`
  under the crown, medians of three after a warmup rep (`benchmarks/prefill_window_probe.das -m <27B> --windows 2,3,4,8
  -r 4`, GPU ms of the short window after a 2000-token prefix); the single-row decode step is untouched (M1 62.9 / 62.7
  ms, M5 35.7 / 35.6 ms):

  | box | window rows | half x | f32 x | ratio |
  |---|---|---|---|---|
  | M1 Max | 2 | 83.0 ms | 92.6 ms | 0.90 |
  | M1 Max | 3 | 110.1 | 132.4 | 0.83 |
  | M1 Max | 4 | 112.6 | 150.9 | 0.75 |
  | M1 Max | 8 | 183.3 | 280.1 | 0.65 |
  | M5 Max | 2 | 40.3 | 40.3 | 1.00 |
  | M5 Max | 3 | 46.3 | 52.4 | 0.88 |
  | M5 Max | 4 | 45.1 | 50.5 | 0.89 |
  | M5 Max | 8 | 75.6 | 93.4 | 0.81 |

  The MTP verify rows (`lcpp_bench --mtp-ab --mtp-depth 2 -r 3`, eight real prompts, three verify rows a round, 68-69%
  of the drafts accepted on both boxes and both arms): the on arm M1 14.73 -> 17.13 tok/s against an off arm of 15.70
  on both (speculation at depth 2 turned from a loss, x0.94, into a gain, x1.09); M5 26.66 -> 31.39 against 25.35 /
  25.73 off (x1.05 -> x1.22); the 50%-acceptance prompt lost under the f32 forms on both boxes (x0.79 M1, x0.89 M5)
  and holds its own under the half forms (x0.92, x1.02). The two-stream row (`--npl 2 -p 32 -n 128 -r 3`): on the dense
  Llama-3.2-3B Q4_K_M, M1, rail off / on / off interleaved, tg128@2 summed 150.0 / 166.4 / 149.9 tok/s (+11%); on the
  27B hybrid the M1 row did not move (22.52 -> 22.68 summed) - its recurrent layers' projections ride the q8 rows GEMV,
  not the K-quant sites, so the two-row step's bytes sit mostly off the forms this crown reaches; the M5 27B row read
  47.47 / 48.83 / 43.57 summed off / on / off (the last at cv 7%, void; a first cold pair had read 38.58 -> 47.61 with
  the solo row moving the same way - a clock artifact, the window probe's decode step unmoved), so neither box moves
  the hybrid's two-stream row. The kernel cells (`test_metal_kq_k4_half_forms`, every row count the dispatcher
  serves; `tests/run.das -- --suite kernels --arm kernels`) and the rows parity cell (`test_metal_decode_parity.das`,
  `batchB8-kq`'s half-x leg, reached through `tests/run.das -- --suite decode --arm batch`: B=2/3/8 token for token
  the CPU control under the pinned crown, the three forms' dispatch counts and the batched driver's step count the
  witnesses, the rail off as the control) pass on the M1.

- **MEASURED (2026-10-09, `direction-grade`) - the K-quant small-batch GEMV forms against B single-row passes, on
  the M1 Max and the M5 Max.** `benchmarks/matmul/bench_metal_kq_race.das --fmts k4 --shapes 27b --tiers
  gemv,mvb,mvb4,mvb8` (k4 planes at Qwen3.6-27B's three dense shapes; `passes` = B dispatches of the production
  single-row GEMV, x and y stepping a column each; `prod` = the production B-column form, `r2` the k4 register tile,
  `wide` = the single-row GEMV's lane map over B columns, `wideh` the same reading x as halves; best of three rounds of
  50 dispatches after a 300 ms burn; the M5 rows under its box sidecar's crowns). GB/s is the weight stream; the
  cost column is the form's time in single-row passes.

  | box | shape | single pass | B | passes | prod | r2 | wide | wideh |
  |---|---|---|---|---|---|---|---|---|
  | M1 Max | w13 (5120 x 17408) | 162 us, 309 GB/s | 2 | 2.00 | 1.66 | 1.55 | 1.77 | 1.26 |
  | M1 Max | w13 | | 4 | 4.00 | 2.88 | - | 3.37 | 1.80 |
  | M1 Max | w13 | | 8 | 7.98 | 5.14 | - | 5.35 | 2.82 |
  | M1 Max | w2 (17408 x 5120) | 180 us, 279 GB/s | 2 | 2.00 | 1.57 | 1.62 | 1.72 | 1.14 |
  | M1 Max | w2 | | 4 | 4.01 | 3.03 | - | 3.30 | 1.72 |
  | M1 Max | w2 | | 8 | 8.01 | 5.07 | - | 4.72 | 2.87 |
  | M5 Max | w13 | 56 us, 891 GB/s | 2 | 2.05 | 1.54 | 1.76 | 1.41 | 1.43 |
  | M5 Max | w13 | | 4 | 3.94 | 2.57 | - | 1.95 | 1.89 |
  | M5 Max | w13 | | 8 | 7.83 | 5.10 | - | 3.91 | 3.12 |
  | M5 Max | w2 | 62 us, 813 GB/s | 2 | 1.85 | 1.59 | 1.76 | 1.47 | 1.45 |
  | M5 Max | w2 | | 4 | 3.78 | 3.07 | - | 2.32 | 2.08 |
  | M5 Max | w2 | | 8 | 8.01 | 5.53 | - | 4.34 | 3.49 |

  Reading: on the M1 the production forms and the wide form cost the same 0.65-0.8 of a pass per column whatever the
  lane map, and a probe that reads column 0's x for both columns runs the two-column wide form at 1.07 passes - the
  bound is the x vector's traffic through L1 (2 bytes of f32 x a weight against 0.56 of weight), which reading x as
  halves cuts to 1.26 / 1.80 / 2.82 passes. On the M5 the x traffic is not the bound (the half form gains nothing at
  two columns) and the wide form beats the production forms on the decode's ALU alone - the full nibble decode is the
  cost there, and the half form wins at eight columns on the ALU the x converts save. Register-resident x walked over
  four or eight weight rows loses on both boxes (0.17 to 0.87 of the passes' rate - the sixty-four floats of x spill;
  the four-row two-column form holds registers and still only reaches the production form). In absolute terms the cost
  column times the single pass: M1 w13 at eight columns prod 833 us against wideh 457, M5 w13 prod 286 against wideh 175.
  The two-stream decode on the 35B pure IQ4_XS measured the same day (`lcpp_bench`'s tg128 row and its `--npl 2`
  two-stream row, MTP off, three reps, the box sidecar): M1 Max tg128 84.8 tok/s solo, 115.2 summed over two streams
  (57.6 a stream, 0.68 of solo); M5 Max 145.4 solo, 207.6 summed (103.8 a stream, 0.71). The work this opens is
  `followup_metal.md` row 50.

- **MEASURED (2026-10-09, `direction-grade`, `debug-jit`) - the short prefill window's routed block: the gathered
  route against the bucketed tiles, on the M1 Max and the M5 Max.** `benchmarks/prefill_window_probe.das` on
  Qwen3.6-35B-A3B-MTP-IQ4_XS-pure (the M5 under its box sidecar's crowns through `DAS_TUNE_MANIFEST`, the M1 on the base
  forms under its own sidecar - the box has no tensor lanes; `DAS_TUNE_POLICY` unset), Metal required, a 2000-token warm
  prefix, windows appended in sequence, GPU ms a window as the driver's own timestamp report, medians of three after a
  warmup rep (`--gemv-max 512` routes every window, `--gemv-max 0` none, each a process of its own):

  | window | M1 tiles | M1 route | M5 tiles | M5 route |
  |---|---|---|---|---|
  | 2 | 57 | 23 | 21 | 10 |
  | 8 | 100 | 39 | 31 | 16 |
  | 16 | 141 | 58 | 41 | 25 |
  | 32 | 182 | 87 | 48 | 36 |
  | 48 | 222 | 130 | 53 | 49 |
  | 64 | 236 | 159 | 57 | 60 |
  | 128 | 308 | 308 | 67 | 107 |
  | 256 | 430 | 615 | 86 | 203 |
  | decode step | 10.9 | | 7.6 | |

  The route's cost is linear in the tokens (2.4 ms a token on the M1, 0.8 on the M5); the tiles' steps with the
  M-tile count. On this carrier the two cross near 128 tokens on the M1 and 56 on the M5. The crossing is per
  model as well as per box - the route's slope is the layers times the slots times the expert bytes a token
  reads, the tiles' floor the experts a window populates - so the same probe over the other MoE carriers
  (`-r 3`, `--gemv-max 512` against `--gemv-max 0`, GPU ms a window):

  | carrier | box | 8 route / tiles | 32 route / tiles | 64 route / tiles | 96 route / tiles |
  |---|---|---|---|---|---|
  | Qwen1.5-MoE-A2.7B Q8_0 | M1 | 37 / 58 | 95 / 105 | 179 / 130 | 265 / 146 |
  | Qwen3-30B-A3B Q4_K_M | M1 | 65 / 100 | 151 / 143 | 278 / 175 | 415 / 214 |
  | gpt-oss-20b mxfp4 | M1 | 60 / 80 | 163 / 106 | 305 / 144 | 450 / 175 |
  | Qwen3.6-35B-A3B UD-Q4_K_M | M1 | 47 / 84 | 122 / 151 | 229 / 199 | 337 / 232 |
  | Qwen3.6-35B-A3B Q4_0-pure | M1 | 38 / 97 | 85 / 176 | 155 / 232 | 228 / 272 |

  | carrier | box | 2 route / tiles | 8 route / tiles | 32 route / tiles |
  |---|---|---|---|---|
  | Qwen3-30B-A3B Q8_0 | M5 | 14 / 20 | 24 / 34 | 61 / 51 |
  | Qwen3-30B-A3B Q4_K_M | M5 | 16 / 22 | 23 / 32 | 55 / 43 |
  | Qwen3-30B-A3B UD-IQ2_XXS | M5 | 17 / 22 | 25 / 33 | 58 / 43 |
  | Qwen3-30B-A3B UD-Q3_K_XL | M5 | 17 / 22 | 27 / 32 | 69 / 43 |
  | Qwen3.6-35B-A3B UD-IQ3_S | M5 | 10 / 16 | 16 / 27 | 36 / 47 |
  | gemma-4-26B-A4B Q4_0 | M5 | 18 / 23 | 24 / 31 | 50 / 41 |
  | gemma-4-26B-A4B Q4_K_M | M5 | 13 / 17 | 21 / 28 | 52 / 41 |
  | gemma-4-26B-A4B UD-IQ3_XXS | M5 | 17 / 23 | 23 / 30 | 50 / 41 |
  | gemma-4-26B-A4B UD-IQ4_XS | M5 | 12 / 18 | 21 / 30 | 50 / 44 |
  | gpt-oss-20b mxfp4 | M5 | 12 / 18 | 20 / 25 | 58 / 33 |
  | Qwen1.5-MoE-A2.7B Q6_K | M5 | 10 / 13 | 17 / 24 | 33 / 38 |

  Every carrier wins at 8 tokens on both boxes; at 32 the 48-layer Qwen3-30B, the gemma 26B and gpt-oss lose on
  both, the 35B hybrids and the 60-expert Qwen1.5-MoE still win. The default knee is 16, the window every carrier
  won at; a one-model deployment sets the sidecar's `metal_moe_gemv_max` at its own crossing. On the house replay
  (the 35B pure, `--ctx 32768 --streams 4`, 122 requests twice) the route leaves the first-token median where it
  was, 0.87 s: a first-round request attaches 84 % of its prompt and prefills a tail of about 300 tokens, above
  any knee, so the windows the route serves are the checkpoint tails (56 -> 26 ms at 2 tokens, 136 -> 84 at
  17-64) and the band that sets the first token is the tiles' 128-512 (`followup_metal.md` 47). Replies, calls,
  quiet and silent counts are the tile run's. On the tiles at 2 tokens the per-kernel split (each node alone) read the
  expert mul_mm at 38 of 76 ms summed on the M1 and 7.5 of 24 on the M5, the padded-panel gather and activation
  at 6.5 and 4.5. The dense sibling Qwen3.6-27B Q4_K_M reads a 2-token window at 235 ms on the M1 (decode step
  64.9) and 80 on the M5 (36.3): its K-quant mul_mm sites have no small-M form (`followup_metal.md` 48).

  The bars the route's parity cells hold (`tests/test_metal_prefill_parity.das`, `test_metal_prefill_moe_gemv_route`,
  arm `moe-gemv`), each the largest |GPU - CPU| over the last row's logits as a share of the CPU row's largest,
  after a 300-row window on the tiles and a 2-, 7- or 24-row window on the route; the readings from the cells'
  own logs on the M1 Max (`bin/daslang -jit modules/dasLLAMA/tests/run.das -- --suite prefill --arm moe-gemv
  --family <f>` under `DASLLAMA_PARITY_FULL=1`), the tile path's own reading on the same window beside each:
  - Qwen1.5-MoE-A2.7B Q8_0 (`qwen2moe`): 0.08 (reads 0.020 / 0.035 / 0.015; the tiles 0.020 / 0.035 / 0.015 -
    the two forms agree to 1e-3; the control window 0.43-1.14).
  - Qwen3-30B-A3B-Instruct-2507 Q4_K_M (`qwen3moe`): 0.10 (reads 0.046 / 0.029 / 0.037; the tiles 0.038 /
    0.028 / 0.036; the control 0.47-0.71).
  - gpt-oss-20b mxfp4 (`gptoss`): 0.12 (reads 0.055 / 0.034 / 0.025; the tiles 0.055 / 0.034 / 0.033; the
    control 0.45-0.73).
  - The Qwen3.6 hybrid cannot stand as a cell carrier (its pure IQ4_XS file refuses the in-memory blob transform and
    its UD file's twin declines the Metal prefill), so its route reads driver-against-itself on the served image, a
    probe's reading and no parity evidence: route against tiles on the same 2000-token prefix, 0.0006 / 0.0004 /
    0.0014 of the peak logit at 2, 5 and 8 tokens and 0.040 at 16 (the tile arm's f16 X twins), the argmax equal at
    every window, a window whose last token differs 0.56-0.78 off.
- **MEASURED (2026-10-07, `direction-grade`, `debug-jit`) - the forced tool call under the grammar, on the house assistant's tool bench.**
  M1 Max, Metal, one stream, the server run from the tree under `-jit` with the m1 tune manifest, the house assistant's `bench_llm.py`
  tool part (the house prompt, a nonce a run, no-think, 256-token budget), 12 runs an arm; `auto` = `tools` alone,
  `forced` = the same with `tool_choice: "required"`, which samples the call through the dasLR1 grammar.

  | file | auto ok | forced ok | auto warm total | forced warm total | tokens |
  |---|---|---|---|---|---|
  | pure Q4_0 (4.52 bpw, `Qwen3.6-35B-A3B-MTP-Q4_0-pure.gguf`) | 6/12 | 12/12 | 1.29 s | 1.34 s | 40 |
  | pure IQ4_XS (4.27 bpw, `Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf`) | 12/12 | 12/12 | 1.31 s | 1.35 s | 40 |

  Q4_0's six misses were plain-text answers (`finish: stop`, no call), not malformed arguments; the grammar turns every
  reply into a well-formed `control` call. The constraint adds about 40-50 ms to a 40-token call (3-4 %): the piece table
  is built on the first constrained request (a second, folded into that request's time) and each token probes the
  candidate list against the grammar.
- **STATED (2026-10-07) - the constrained sampled draw's candidate copy.** `Session.cand_full` (16 B an entry) holds the
  candidate list a sampled constrained draw starts from - the top-k, at most `SAMPLE_TOPK_FAST_CAP` (1024) entries plus
  ties at the k-th logit - so a refused first pick refilters from the logits the draw consumed: 16 KB a session at the
  cap, `@scratch @exact_size`, reserved once at `make_run_state`. Not measured; the formula is the record.
- **STATED (2026-10-07) - the token constraint's two vocabulary-sized allocations.** `Session.cand` (16 B an entry)
  grows to the whole row only when a constraint refuses every candidate of the probe window: 16 x vocab bytes a
  session, 2.4 MB at a 151936-entry vocabulary (Qwen), 4.2 MB at 262144 (gemma); it is `@scratch @exact_size` and
  reused, so the growth is paid once a session. `utils/dasllama-server`'s token piece table holds one string a token
  a served slot, built on the slot's first constrained request: about (24 + the piece's bytes) x vocab, 6 MB at
  151936 and 10 MB at 262144, freed with the slot. Neither is measured; the formulas are the record.

- **MEASURED (2026-10-06, `direction-grade`, `debug-jit`) - the 4-bit formats of Qwen3.6-35B-A3B at equal bytes, and
  four races on their Metal expert GEMVs.** M1 Max (MacBookPro18,2, 64 GB), Metal, one stream, the sidecar
  re-minted on the binary the same day (0 winner changes against the one the mtime rule had called stale);
  `benchmarks/lcpp_bench.das` as the `-jit` script, `-m <gguf> --ngl 99 -n 64 -r 2 --mtp-ab --for-debug-purposes`
  under `DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/m1.tune.json`, decode = the tg-real64 row, MTP off; perplexity
  = 256 wikitext-2 positions teacher-forced after a 256-id prefill (a small sample: differences past the second decimal
  are noise); tool call = the house assistant's prompt (`bench_llm.py`, a random nonce a run, `control` well-formed), 12 runs.
  The pure files are `llama-quantize --pure` from unsloth's BF16 shards; the two UD files are unsloth's. Decode rates
  are tok/s; bpw is the file's bits a weight.

  | file | bpw | tg | PPL | hits/256 | tool x12 |
  |---|---|---|---|---|---|
  | Q8_0 (reference) | 8.5 | 72.1 | 4.62 | 173 | 12 |
  | UD-Q4_K_M (Q4_K/Q5_K experts, Q8_0 rest) | 5.4 | 71.6 | 4.83 | 173 | 12 |
  | pure Q4_K | 4.52 | 81.1 | 4.93 | 174 | 11 |
  | pure IQ4_XS | 4.27 | 84.4 | 4.95 | 169 | 12 |
  | pure Q4_0 | 4.52 | 85.1 | 4.95 | 169 | 4 |
  | UD-IQ4_XS (IQ3_S gate/up experts, IQ4_XS down) | 4.3 | 73.6 | 4.97 | 167 | 12 |

  - Q8_0 is the one row near bandwidth: 3.5 GB a token at 72 tok/s is 250 GB/s. The 4-bit files move 1.9 GB a token
    at 85 tok/s, 160 GB/s: the 4-bit expert GEMVs reach 60 % of the q8 GEMV's bytes a second, so a 4.5-bpw file at
    the q8 kernel's rate would decode near 130 tok/s. The q8 kernel's shape - one simdgroup a row, four 32-weight
    chunks loaded ahead of the FMA chains - is the difference (`followup_metal.md` 45).
  - Pure Q4_0 slips the tool call on 8 of 12 runs, the same dropped `"arguments":` key every time, at the perplexity
    of the asymmetric formats: the symmetric codebook lands that near-tie wrong. Pure IQ4_XS is the fewest bytes at
    the K-quant files' quality, and the format to put against mlx's 4-bit group-64 affine weights (4.5 bpw, 19 GB).
  - Four kernel races on the gathered (expert) GEMVs, each the e2e tg minutes apart on the same binary and sidecar:
    the IQ3_S f4-slab twin on the gathered site, 73.6 -> 75.2 on the UD-IQ4_XS file (shipped, the dense twin's crown
    gates it); the same twin at sixteen rows a threadgroup 74.2 (fewer threadgroups a 512-row expert cost more than
    the halved staging saved); the grid read from constant memory instead of the staged float4 slab 69.8 (scattered
    constant reads, four scalar loads a word - vector literals do not hoist); the q40 identity codebook folded to
    arithmetic 85.3 +- 0.4 against 85.1 +- 0.5 (noise); the iq4 codebook held in registers as four packed words
    61.4 against 84.4 (seven ALU ops a nibble lose to one threadgroup read). The codebook read and the staging are not
    where a 4-bit expert GEMV's time goes.
- **MEASURED (2026-10-06, `audio-shared-kernels`) - the audio chains' kernels moved to templates both homes stamp read as
  before on both boxes.** whisper large-v3-turbo q8 served (`dasllama-server` with the ASR model alone, `main.das -- --asr
  <bin>`; `harness/served_bench.das --no-chat --asr-url <server> --clip jfk_ask_not.wav --reps 8`), master and the branch
  alternated twice in one sitting on each box, one life a server (the master lives first); the branch moves the Metal
  layernorm, bias, clamp, GEGLU, im2col, row gather, K/V store and conv0 stamps and the Vulkan tower's seam, im2col, gather
  and K/V classes onto the shared templates, so the served encode and decode ride every one of them.
  - M5 Max, Metal, ms a request, master / branch: 229.6 / 228.8 (cv 0.5% / 0.7%); the second pair 242.5 / 243.0, both void
    at cv 3.1% / 2.9% (the box warmed under the four lives).
  - RTX PRO 4500, Vulkan, ms a request, master / branch: 51.3 / 51.7 and 50.8 / 49.5 (cv 1.9% / 2.4% / 1.8% / 0.8%); a
    first sitting read every row void on that box's round-trip spread (cv 6-28%), with one master life refused at its
    second rep because the transcript differed from the first - the Vulkan chain's text moved between reps on master.
  - Long-lived device memory the branch adds, bytes: the Vulkan chat tail's rows `cap x (hid + gate + out) x 4` beside its
    host copy `cap x out x 4` (the whisper-class projector widths, a few MB at the 1500-row cap), its projector q8 planes
    beside the block records (`d x stack x mm1_out + mm2_in x proj_dim` bytes); parakeet's Vulkan resident (its q8 planes and
    norms plane, as canary's); the Metal fronts' conv0 panel `nch x 32 x 4` once an encoder (32 KB at 256 channels).

- **MEASURED (2026-10-06, `direction-grade`) - Pocket's text prompt on the device takes a third of its request on
  both boxes; the thirteen TTS kernels moved to templates both homes stamp read as before.** The prompt seat runs the
  chunk's text rows through the frames slab's layers at the voice's positions (`ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames`,
  `ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-pocket-chain`), the keys and values back to the host; the last layer ends at
  its K/V on every rail, including the CPU's. `dasllama-server` with the TTS model alone, `harness/served_bench.das
  --no-chat --tts-url <server> --reps 8`, one sentence, master and the branch alternated twice in one sitting on each
  box (the master lives first); the stages are the engine's own stage clock. The M5's tune sidecar predates the binary.
  - M5 Max, Metal, ms a request, master / branch: Pocket q8 67.5 / 49.5 and 67.5 / 49.4 (4.56 s of speech on master,
    4.64 on the branch - the device prompt moves the EOS frame by one), its stages prompt 15.5 / 2.1, backbone
    38.4 / 34.2 (the attention's unrolled loads and the frames call's upload of rows the device already holds),
    codec 8.8 / 8.6; Kokoro-82M 73.5 / 73.5 (a third branch life 73.5, two branch lives void at cv 3.3% and 3.4%),
    its stages the same to the tenth (decoder 41.22 / 41.29); Kitten mini 64.5 / 64.5 (one branch life void at cv
    4.1%), decoder 29.37 / 29.36.
  - RTX PRO 4500, Vulkan (driver 580.159), ms a request, master / branch: Pocket q8 114.7 / 68.1 and 114.2 / 67.9,
    its stages prompt 50.0 / 3.8, backbone 50.2 / 49.2, codec 8.7 / 8.8; Kokoro-82M 48.3 / 48.1 and 48.3 / 47.9 (the
    first master row void at cv 9.3%), its stages the same to the tenth (bert 4.29 / 4.29, decoder 13.9 / 13.9);
    Kitten mini 48.6 / 46.4 and 52.5 / 47.8, every row void on spread (that box's round trip jitters), its stages the
    same to the tenth (decoder 10.5 / 10.4). The shared reduce base the row kernels now derive puts one call under
    every `WgReduceBase` reduce (eight audio-tower stamps and five TTS stamps moved by it, `harness/vk_spv_diff.das`):
    the backbone and bert stages above ride it, and whisper large-v3-turbo q8 served (`main.das -- --asr <bin>`,
    `served_bench.das --no-chat --asr-url <server> --clip jfk_ask_not.wav --reps 8`) reads 48.8 / 48.5 on the one
    round both sides held under 3% (54.2 / 54.7 on the other, both void).
  - Quality, `harness/tts_rig.py` on the 200-sentence corpus, flag-free on the M5 (the Metal seats serve), WER % /
    UTMOS before -> after: Pocket f32 4.18 / 4.373 -> 3.95 / 4.368, q8 4.41 / 4.363 -> 4.23 / 4.354, kq 3.91 / 4.325 ->
    3.77 / 4.314, stuart-kq 3.32 / 4.137 -> 3.27 / 4.139 (the device prompt moves an EOS frame here and there);
    kitten-nano 3.09 / 3.980 -> 3.09 / 3.976, kitten-mini 2.77 / 4.331 -> 2.77 / 4.331, kokoro 2.73 / 4.501 ->
    2.73 / 4.501.
- **MEASURED (2026-10-05, `direction-grade`, `debug-jit`) - the depth-1 NextN round on the M1 Max: the draft at two
  rows, then chained into the verify's command buffer.** M1 Max (MacBookPro18,2, 64 GB), Metal,
  Qwen3.6-35B-A3B-MTP UD-Q4_K_M, `benchmarks/lcpp_bench.das` as the `-jit` script (`-no-module-cache`):
  `-m <gguf> --ngl 99 -n 64 -r 2 --mtp-ab --prof --for-debug-purposes` under `JOBQUE_PROFILING=1
  DAS_LOG_LEVEL=info DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/m1.tune.json` (the sidecar re-minted on
  the binary by `harness/dasllama_tuner.das`), `DAS_TUNE_POLICY` and `DASLLAMA_PIN_BACKEND` unset; the
  tg-real64 row is the mean and stdev over 2 reps of 8 prompts, the per-round split one untimed
  `forward_profile` window of 36-37 rounds, each arm one commit - `add367ec1` (the four-row tile, the host
  chain), `bf99297f5` (two rows), `3826f59ff` (two rows, chained). Decode rates are MTP off / on, tok/s, the
  parenthesis the on arm against the off arm; the round's parts are ms a round. The chained round parks its
  draft step through the verify, so the pool holds one more step than the host chain did: its logits rows
  `max(ng, 2) x vocab x 4` bytes (1.99 MB at one or two streams, 0.99 MB a stream past that on this vocabulary)
  beside a few activation rows.

  | | `add367ec1` | `bf99297f5` | `3826f59ff` |
  |---|---|---|---|
  | tg-real64 off / on | 71.8 +/- 0.0 / 69.1 +/- 0.1 (0.96x) | 71.7 +/- 0.0 / 73.9 +/- 0.9 (1.03x) | 71.7 +/- 0.0 / 74.3 +/- 0.0 (1.04x) |
  | acceptance | 76.4% (440/576) | 76.4% (440/576) | 76.4% (440/576) |
  | mtp.draft, host wall | 3.92 | 2.55 | 0.01 |
  | mtp.verify (its GPU wait) | 20.9 (20.5) | 20.5 (20.1) | 23.1 (22.6), the draft's GPU inside it |
  | mtp.walk + mtp.replay | 0.57 | 0.60 | 0.57 |

  - The four-row tile cost a solo stream a second pair of rows over the draft layer and the 417 MB Q6_K
    classifier (248320 x 2048); the two-row tile drafts the same tokens (0 of 288 rounds differ,
    `DASLLAMA_MTP_DEBUG=trace`, `-r 1`, the same flags) and the one-row form does not: 36 of 257 differ, that
    run an uncommitted probe edit of `bf99297f5` (`nrows = ng`, the rows forms' buffers and uniforms forced on
    for a draft step), no commit carries it (`followup_metal.md` 44).
  - The chain removed the draft's join, its 1 MB logits readback and the host argmax - 0.4 ms of the 2.55;
    the draft's GPU work now sits inside the verify's wait. The chained round drafts and commits the two-row
    host chain's tokens on every traced round (0 of 288 differ, the same trace flags).
  - The row-cost ladder at `a8655daf0`, `harness/batch_rows_probe.das -m <gguf> --bs 1,2,4 --steps 32` (two
    sessions a row) and `--sameslab --bs 1` (one session), same flags, ms a step: the single-row driver
    13.6; the rows driver at one row 15.5 (x1.14), at two rows 19.8 (x1.45), at four 31.2 (x2.29), every
    ratio against the single-row driver's step. The verify's 22.2 ms of GPU is that two-row step plus the
    draft layer, two classifier passes and the draft-slab re-warm; two rows route to two expert sets, so the
    step streams their union. At 1.76 tokens a round over a 1.45x step the depth-1 ceiling here is 1.21x.
  - The same-slab probe declines at two rows on this hybrid, so the two-row figure is
    the two-session batch step, not the verify itself.
- **MEASURED (2026-10-05, `direction-grade`) - Axpy and Concat as kernels both GPU homes stamp read as before on
  both.** The two kernels moved to one class template each (`dasllama_gpu_kernels_common.das`); on Metal their
  arguments became one struct and Axpy lost two offsets no site passed, on Vulkan the body became a function the
  entry calls (Concat's SPIR-V 52 bytes longer, Axpy's 196 shorter for the dropped offsets; 25 of the 27 TTS
  stamps byte-identical). `dasllama-server` with the TTS model alone (`main.das -- --tts <gguf>`),
  `harness/served_bench.das --no-chat --tts-url <server> --reps 8`, one sentence, master and the branch alternated
  twice in one sitting on each box; the decoder stage is the engine's own stage clock and is the stage that runs
  both kernels. The M5's tune sidecar predates the binary.
  - M5 Max, Metal, ms a request, master / branch / master / branch: Kokoro-82M 73.7 / 73.8 / 73.6 / 73.8 for
    5.68 s of speech (the last two void at cv 2.9% and 3.5%), its decoder 41.29 / 41.30 / 41.30 / 41.27; Kitten
    mini 64.6 / 64.7 / 64.5 / 64.8 for 5.84 s (the third void at cv 3.9%), its decoder 29.44 / 29.37 / 29.38 / 29.27.
  - RTX PRO 4500, Vulkan (driver 580.173), decoder stage ms, master / branch / master / branch: Kokoro-82M 14.14 /
    14.04 / 13.91 / 13.97; Kitten mini 10.54 / 10.53 / 10.57 / 10.63. The request walls there read 46.3-52.4
    (Kokoro) and 46.1-48.1 (Kitten mini) with seven of the eight rows void on spread - the round trip jitters on
    that box, the stage clock does not.

- **MEASURED (2026-10-04, `direction-grade`) - a served media turn's first token, and the speech rows beside
  mlx-audio.** M5 Max, Metal, `dasllama-server` with one model (`main.das -- -m <gguf> --image-mmproj <mmproj>` or
  `--audio-mmproj <mmproj>`, `-s 1 --ctx 16384`), the pinned llama-server as `-m <gguf> --mmproj <mmproj> -ngl 99
  -np 1 -c 16384`, `harness/served_bench.das --url <server> --no-chat --reps 5 --image <jpg> --chat-clip <wav>`
  (tagged `served`, `out-of-process`), a 60 s rest before each life; the tune sidecar predates the binary. The
  first token of a question about a 640x480 image or an 8 s clip under a system line no server has seen, ms,
  ours / the reference; a row over 3% cv is marked void.
  - The paged KV pool grew by doubling, and a doubling copied and zero-filled every cached page inside one
    request: Qwen3-VL 4B's image row read cv 7.9% with prefills of 116 ms beside ones of 158, 189 and 271. With
    the pool's address room reserved at scheduler creation, twenty prefills in a row read 116-118 ms and the
    row cv 0.9-1.2% over three runs. A model whose K/V passes the 4 GiB reserve (Qwen2-Audio, 524 KB a
    position) still pays one copy a server life: 246 ms, once.
  - A media prompt prefilled as three calls - text, rows, text - and each short call paid the call's floor
    (about 20 ms): Voxtral Mini's clip read 214.7 with 102 of encode and 84 of prefill (21 + 41 + 20). As one
    call it reads 181.0; Qwen3-VL 4B's image 185 to 148; gemma-4 E2B's image 115 to 82 and E4B's 164 to 98,
    once the body carries each row's token id.
  - Images: gemma-4 E2B 81.7 / 141.4, E4B 98.3 / 206.9, 12B 164.3 / 371.2; gemma-3 4B 426.6 / 466.8, 12B
    561.0 / 666.1; Qwen3-VL 4B 147.2 / 196.5 (the reference void, 3.2%), 8B 220.8 / 295.5; Qwen2.5-Omni 3B
    188.8 (void, 3.8%) / 224.6, 7B 257.0 (void, 5.7%) / 349.3.
  - Clips: gemma-4 E2B 77.9 / 125.8 (the reference void, 6.3%), E4B 107.4 / 187.2; Qwen2.5-Omni 3B
    219.3 / 294.5, 7B 348.2 / 465.6; Ultravox 1B 132.0 / 147.9 (the reference void, 6.1%), 8B 219.1 / 292.3;
    Voxtral Mini 181.6 / 189.1 (the reference void, 5.3%), Small 439.6 / 503.6 (the reference void, 4.2%);
    Qwen2-Audio 465.0 / 508.2. The E-series rows and the 12B image are single re-runs after their change; the
    rest are one sweep taken before the instrument's lead-in request, which is what voids the two Omni images.
  - Asked again under the same line, both servers answer off their cache within a few ms of each other on
    every row; the one row the reference leads is Voxtral Small, 167.8 / 161.8.
  - Speech, `main.das -- --tts <gguf>` against `mlx_audio.server` (mlx-audio 0.5.7), `served_bench.das
    --no-chat --tts-url <server> --reps 8`, one sentence: ms a request / s of speech / real-time factor.
    Kokoro-82M (the same voice, ours q8, theirs bf16): 73.1 / 5.50 / 0.013 against 108.9 / 5.47 / 0.020; three
    sentences 218.9 / 17.5 / 0.013 against 323.5 / 17.1 / 0.019. Kitten nano 22.5 / 5.24 / 0.004 against
    75.1 / 7.93 / 0.009 and mini 64.6 / 5.84 / 0.011 against 242.7 / 7.55 / 0.032 - their files are the 0.8
    release and speak the sentence longer, so the factor is the comparison. Pocket (ours the English 2026-04
    file at q8 and kq, theirs Kyutai's without-voice-cloning weights at full and 8 bit): 67.7 / 4.64 / 0.015
    and 61.7 / 4.80 / 0.013 against 200.3 / 4.96 / 0.040 (void, 8.6%) and 152.6 / 5.12 / 0.030 (void, 6.7%).
  - The M5 and M4 boards' Metal rows this arc re-routes are withdrawn until a mint on a fresh sidecar: the
    ASR rows of whisper tiny and large-v3-turbo, Canary-Qwen, Qwen3-ASR 0.6B, Qwen3-Omni 30B and gemma-4 E2B,
    and the image rows of gemma-4 E2B and E4B.

- **MEASURED (2026-10-04, `direction-grade`) - canary's mel and subsample front run on the Metal tower.**
  M5 Max, Metal, canary-qwen-2.5b (the q8_0 decoder, the f32 encoder), in process as the server's
  ASR worker runs it (the engine's stage clock, the fourth transcription of a clip), the tune sidecar
  predating the binary. The front seat was filled by the Vulkan driver alone, so on Metal the mel
  (a twiddle GEMM on the CPU) and the subsample stack ran on the CPU beside 80 ms of blocks on the
  device.
  - A 64 s clip: 1274 ms before - mel 239, front 258, blocks 80, projection 7, prefill 56, decode
    629 - and 772 after, the mel and the front together 8.0. An 8 s clip: 163 ms before, 106 after
    (3.5 for the two).
  - The device rows sit 2.1e-3 rel-l2 from the CPU chain's on the jfk clip (3.1e-4 with the CPU
    front ahead of the device blocks); the transcripts are equal. The reading is `test_model_image.das`'s
    canary `mtower` arm (`canary mtower encode rel-l2`).

- **MEASURED (2026-10-03, `direction-grade`) - the Metal tower reads a whisper encoder's q8 planes.**
  M5 Max, Metal, whisper large-v3-turbo, `dasllama-server` with the ASR model alone
  (`main.das -- --asr ggml-large-v3-turbo.bin`), the reference whisper.cpp `6fc7c33` as
  `whisper-server -m <bin> --inference-path /v1/audio/transcriptions`, `harness/served_bench.das
  --no-chat --asr-url <server> --clip <wav> --reps 5` (tagged `served`, `out-of-process`), a 60 s
  rest before each life, every request a clip no server has heard; the tune sidecar predates the
  binary, so the CPU kernels ran their fallback stamps. Clips of 8 s, 11 s and 64 s (one 30 s
  window, one, three), ms a transcription.
  - The tower declined a q8 encoder, and whisper serves q8: the encoder ran on the CPU and read
    911.7 / 951.4 / 3092.8. The reference reads 131.3 / 147.1 / 601.5 on the f16 file and
    127.1 / 139.8 / 554.8 on its q8_0 file (0.87 GB).
  - The block GEMMs off the q8 blob on the plain q8 tile: 240.5 / 253.1 / 876.7. On the prefill
    ladder: 153.5 / 166.1 / 608.9. With the stem's second conv off the same blob: 137.4 / 149.0 /
    541.0, on the 1.2 GB q8 image. The f32 encoder lane on the same tower reads 132.3 / 140.0 /
    517.9 on a 4.3 GB image.
  - A window's stages on the q8 lane (the encoder's own stage clock, in process): blocks 102.5 ms
    (94.0 on the f32 lane), the stem 2.2 (15.6 on the CPU), the decode 21.2, the cross K/V 6.4,
    the mel 1.4.

- **MEASURED (2026-10-03, `direction-grade`) - a served Qwen3-ASR transcription decodes on the device, and a
  repeated clip measures the other server's prompt cache.** M5 Max, Metal, `dasllama-server` with the
  ASR model alone (`main.das -- --asr <gguf> --mmproj <mmproj>`), the pinned llama-server as
  `-m <gguf> --mmproj <mmproj> -ngl 99 -np 1`, `harness/served_bench.das --no-chat --asr-url <server>
  --clip <wav> --reps 5` (tagged `served`, `out-of-process`), a 60 s rest before each life; the tune
  sidecar predates the binary, so the CPU kernels ran their fallback stamps.
  - The ASR worker's context carried no Metal mode, so the decoder - a `Model` session - prefilled
    and decoded on the CPU: Qwen3-ASR-0.6B Q8_0 read a 4 s clip in 127.5 ms. The worker takes the
    engine's mode for a two-file model: 50.5 ms. Inside it (the worker's own stage clock): encode
    10.7 ms, the 75-row prefill 6.7, twelve decode steps 29.0 - 2.4 ms a token, where the reference's
    own timing line reads 2.65.
  - The reference keeps the prompts it has evaluated, audio rows included: the same clip sent again
    costs it one prompt token (`cached_tokens` 79 of 80) and reads 37.4 ms, and a clip one sample
    off costs the whole prompt. The instrument's rows before this entry repeated one clip and so
    read that cache; every request now uploads a clip no server has heard.
  - Unseen clips, ours / the reference, ms a transcription: Qwen3-ASR-0.6B 4 s 50.5 / 60.7, 8 s
    76.6 / 98.1 (the reference's cv 6.1%, void); Qwen3-ASR-1.7B 4 s 85.5 / 95.1, 8 s 130.8 / 164.4.

- **MEASURED (2026-10-02) - the K/V mirror's block codecs on the whole-model driver
  (`ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs`, `set_gpu_kv_dtype` / the server's `--kv-dtype`): a
  q8_0 mirror is 17/32 of the f16 mirror's bytes and tq4 9/32, and on a model whose context was asked whole
  the bytes they give back are resident layers - the 35B at 131072 positions goes from seven routed layers'
  experts on the host to none and reads 3.1-3.4x the prompt rate; on a model the card holds whole they cost
  the long prompt 6% (q8_0) and 16% (tq4) and a token nothing (direction-grade).**

  zen2 (Ryzen 16 threads, RTX 5060 Ti 16 GB on a PCIe 4.0 x8 link), `bin/Release/daslang.exe -jit
  modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m <file> --ngl 0 -p <P> -n <N> -r <reps> -t 16 --kv <codec>
  --for-debug-purposes` under `DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_GPU=1 DAS_JOBQUE_THREADS=16`;
  `DAS_TUNE_POLICY`, `DASLLAMA_PIN_BACKEND` and `DASLLAMA_COOPMAT` unset (the registry's own pick `x64-gen`
  served the CPU kernels, the device ran cm2), one process a row - a second load of a model in one process
  is barred, so the codecs cannot race interleaved. `--kv` asks the mirror's codec and makes the bench's
  sessions in it, so each row is a served shape. A cell is the mean of its reps and their standard deviation.
  Every row is its own process, so every comparison between two rows of the tables below is direction-grade.

  Llama-3.2-1B-Instruct Q4_K_M, whole on the card at the model's 131072 positions under every codec, two
  reps, 1.9-2.1 GB of the card held by the desktop:

  | mirror | pp512 | tg128 | pp8192 | tg64 after 8192 |
  | :--- | ---: | ---: | ---: | ---: |
  | f16 | 19337 +/- 69 | 331.0 +/- 25.8 | 16854 +/- 83 | 344.7 +/- 1.2 |
  | q8_0 | 21873 +/- 44 | 330.7 +/- 5.5 | 15860 +/- 161 | 338.0 +/- 0.0 |
  | tq4 | 18454 +/- 267 | 344.2 +/- 0.5 | 14150 +/- 5 | 327.9 +/- 2.0 |

  Qwen3-30B-A3B-Instruct-2507 Q4_K_M (17.3 GB, 48 routed layers), `DASLLAMA_GPU_CTX_MAX=32768
  DASLLAMA_GPU_CTX_STRICT=1` added, three reps, the four processes back to back with 1924 MB of the card
  held by the desktop:

  | mirror | routed layers' experts on the host | pp512 | tg128 |
  | :--- | ---: | ---: | ---: |
  | f16, the round's first process | 25 of 48 | 409.9 +/- 89.6 | 43.7 +/- 2.6 |
  | q8_0 | 21 of 48 | 572.5 +/- 26.2 | 49.8 +/- 1.7 |
  | tq4 | 19 of 48 | 586.9 +/- 13.2 | 52.5 +/- 4.0 |
  | f16, its last | 25 of 48 | 516.0 +/- 3.8 | 46.7 +/- 0.9 |

  Qwen3.6-35B-A3B UD-IQ2_XXS (10.3 GB, 40 layers, a deltanet hybrid), `DASLLAMA_GPU_CTX_MAX=131072
  DASLLAMA_GPU_CTX_STRICT=1` added, three reps, the four processes back to back with 1916-1930 MB held:

  | mirror | routed layers' experts on the host | pp512 | tg128 |
  | :--- | ---: | ---: | ---: |
  | f16, the round's first process | 7 of 40 | 863.2 +/- 35.1 | 89.8 +/- 0.5 |
  | q8_0 | 3 of 40 | 1544.6 +/- 7.0 | 100.0 +/- 0.8 |
  | tq4 | none | 2962.7 +/- 34.7 | 111.6 +/- 0.6 |
  | f16, its last | 7 of 40 | 966.8 +/- 11.3 | 84.7 +/- 1.1 |

  The count of hosted layers follows the room the desktop leaves, so a codec's rows compare only at one
  share: the entry below reads the 30B under f16 at 28 of 48 layers (517.9 / 41.8) with 4.6 GB held, and
  its parity run the 35B at 10 of 40; both stand for their share, and the rows above for theirs.

  The NextN round over the codecs: `--mtp-ab -n 64 -r 2` in place of `-p` / `-n` / `-r` (real text, eight
  prompts, depth 1, the off and on arms in one process a row), the head on the device in every row and no
  call passed to the CPU rails:

  | model | mirror | off | on | on / off | drafts accepted |
  | :--- | :--- | ---: | ---: | ---: | ---: |
  | Qwen3.5-0.8B-MTP Q8_0 | f16 | 316.6 +/- 10.7 | 397.6 +/- 6.8 | 1.26 | 81.9% |
  | | q8_0 | 317.6 +/- 3.9 | 392.0 +/- 1.3 | 1.23 | 81.3% |
  | | tq4 | 314.1 +/- 1.1 | 394.8 +/- 0.3 | 1.26 | 82.7% |
  | Qwen3.5-4B-MTP Q8_0 | f16 | 76.5 +/- 3.1 | 115.4 +/- 0.1 | 1.51 | 81.3% |
  | | q8_0 | 78.3 +/- 0.3 | 115.8 +/- 0.3 | 1.48 | 82.0% |
  | | tq4 | 74.5 +/- 5.3 | 114.8 +/- 0.6 | 1.54 | 82.3% |
  | Qwen3.5-9B-MTP UD-Q5_K_XL | f16 | 59.1 +/- 1.2 | 88.5 +/- 0.2 | 1.50 | 80.7% |
  | | q8_0 | 59.3 +/- 0.3 | 87.2 +/- 0.5 | 1.47 | 78.8% |
  | | tq4 | 59.4 +/- 0.6 | 88.9 +/- 1.5 | 1.50 | 82.6% |

  The round's rate does not follow the codec (the on arm moves 1.5% at most across a model's three rows,
  direction-grade) and neither does the acceptance (78.8-82.7%): the draft and the verify read the blocks
  through the token command's attention, which the codec does not slow. These nine rows were read at
  372db0f70, before the codec arms' block loads were factored into shared helpers: the codec stamps' words
  differ at the tip (`harness/vk_spv_diff.das`: 13 of 13 moved), the arithmetic they encode does not. A
  re-read of the 0.8B at the tip, the tree merged over master at 42c4013cd and the desktop holding 2.4 GB
  of the card, accepts the same drafts (81.9%, 81.3%, 82.7%) at lower rates on every engine: the f16 row
  with master's `dasllama/` and `benchmarks/` checked out in place reads 281-285 off and 327-350 on, this
  tree 291-292 and 339-340 back to back, its q8_0 row 255 and 332, its tq4 row 272 and 337.

  The per-op rails under the session's codec (`ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block`):
  Qwen3-30B-A3B-Instruct-2507 Q4_K_M with no context pin - the whole-model driver declines the 17.3 GB
  file and the per-op rails serve - `-p 512 -n 128 -r 1`, one process a row. The tq4 row before this entry's
  change is the same command on the tree where the decode block and the prefill chain both refused a tq4
  session and its attention ran on the CPU, a compare across two commits (direction-grade):

  | session codec | pp512 | tg128 |
  | :--- | ---: | ---: |
  | f16 | 662.6 | 59.9 |
  | q8_0 | 733.5 | 63.6 |
  | tq4, its attention on the CPU (before) | 274.8 | 31.3 |
  | tq4, the block's f16 mirror hydrated out of the rotated basis | 688.2 | 65.0 |

  The three served rows land one warm-up argmax and logit (`ency` 15.93165 after the prompt, `and`
  11.371113 after the first step): the block attends its own float rows under every codec, and the codec
  is the cache's.

  The 1B's long prompt is the shadow's price (`ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-block-codecs`): the
  flash tiles read f16 rows, so each layer of each window dequantizes the span it attends first, and tq4
  adds the rotation of the window's rows and the un-rotation of its attention rows. The decode attention
  reads the blocks natively and the token rate does not follow the codec beyond the rows' own spread (the
  1B's tg128 reads 331-344 with f16's own two reps 26 apart, and at 8192 positions 328-345; direction-grade).
  The MoE rows are the reason the codecs exist on this driver: a hosted layer costs a fence round-trip a
  token and a host callback a window, so every layer the smaller mirror brings back shows in both rates; the
  35B under q8_0 still hosts three layers and its prompt rate reads half of tq4's, which hosts none
  (direction-grade).

  What the codecs allocate, beside a mirror 17/32 or 9/32 of the f16 one's bytes. On the device: the f16
  shadow `ksh_dev` / `vsh_dev`, `(seq_cap x kv_dim + 131072) x 2` B a plane with `kv_dim` the widest
  attention layer's K/V row - 128.2 MiB a plane on Llama-3.2-1B (512 wide) at 131072 positions, 2.25 MiB on
  Qwen2.5-0.5B (128 wide) at 8192; the window's staged rows `pf_kst` / `pf_vst`, `512 x kv_dim x 4` B a
  plane (1 MiB and 256 KiB on the same two); the token command's staged rows `kst_dev` / `vst_dev`,
  `kv_dim x 4 x nb` B a plane with `nb` the command's rows, eight at most (16 KiB on the 1B at eight rows,
  512 B on the 0.5B at one), beside `stage_tok`, 32 B a row; tq4's sign row, 2048 B. On the host, the
  per-op rails' hydrate arrays `g_dat_hyd_k` / `g_dat_hyd_v`, which a tq4 session now reaches: `pos x
  kv_dim x 4` B each, 8 MiB at 4096 positions of a 512-wide row and 512 KiB at 1024 of a 128-wide one.

  The bars the codec cells hold, each constant with the reading behind it. The readings come from the
  cells' own logs on zen2 (the box of this entry, `DASLLAMA_GPU=1`, `DASLLAMA_COOPMAT`, `DASLLAMA_PIN_BACKEND`
  and `DAS_TUNE_POLICY` unset, eight job threads), run through `bin/Release/daslang.exe -jit
  modules/dasLLAMA/tests/run.das -- --changed`:
  - `test_gpu_resident_qwen2.das` (Qwen2.5-0.5B Q8_0): `Q8KV_BAR_REL` 0.08 of the step's max logit (reads
    0.39-0.72 of 9.4-18); `CODEC_DIST_REL` 1.15, the device's summed distance from the f16 chain over the CPU
    codec chain's own (reads 0.95-1.01, the rows one step off 1.6-3.7); `CODEC_PAIR_REL` 0.8, the two codec
    chains' distance over that same distance (reads 0.20-0.61, one step off 1.5-3.7); `ROWS_BARS`, the share
    of layer 0's stored bytes that differ from the CPU cache's - `k` 0.02 (reads 0.002), `v_step` 0.05
    (reads 0.007-0.010), `v_prefill_of_ctrl` 0.6 of the control's share (reads 0.39 and 0.12 against 0.97);
    `CODEC_ROWS_CTRL` 0.2 in `_kv_codec_cells.das`, a row against the next one (reads 0.36-0.98), and
    `CODEC_APPLIED_REL` 0.5 there, the least the device's distance from the f16 chain may be of the CPU
    codec chain's (every resident carrier reads 0.95-1.08; a driver serving f16 rows in the codec's place
    would read near 0).
  - `test_gpu_resident_codec_llama_k.das` / `_llama_iq.das` (Llama-3.2-1B Q4_K_M and IQ4_XS):
    `Q8KV_BAR_REL` 0.10 (reads 0.25-0.42 of 8.4-9.4); `CODEC_DIST_REL` 1.15 (reads 0.99 and 1.00, one step
    off 4.7 and 5.0); `CODEC_PAIR_REL` 1.4 (reads 1.02 and 1.13, one step off 4.9 and 5.1) - tq4 moves these
    carriers a point a row, three times the kernels' rounding, so the two codec chains part as two
    independent quantizations do.
  - `test_gpu_perop_kv_codec.das` / `_k.das` / `_iq.das` (Qwen3-0.6B Q8_0, Qwen3-4B Q4_K_M, Qwen2.5-1.5B
    IQ3_XS): `PeropBars.rows` 0.05, the share of layer 0's cache bytes off the CPU chain's (reads 0 to
    4e-5); `PeropBars.dist_rel` 1.0, the device's summed distance from the f16 chain over the CPU codec
    chain's (reads 0.05 and 0.31 on the 0.6B where the device prefilled, 0.90 over rows the CPU quantized,
    0.28 and 0.80 on the 4B, 0.79 under q8_0 on the 1.5B), and 0.5 for the 1.5B's tq4 cell, whose CPU chain
    tq4 moves nine points a row (reads 0.11, one step off 0.84).
  - `test_gpu_resident_hybrid.das`: the block-codec cell holds the NextN draft, verify, warmed head rows
    and rounds under q8_0 to the file's f16 bars; the gated cell (Qwen3.5-0.8B Q8_0) holds
    `GATED_Q8KV_BAR_REL` 0.06 (reads 0.21-0.31 of a 10.7-12.3 max logit), `GATED_CODEC_DIST_REL` 1.15
    (reads 1.08, one step off 9.2) and `GATED_CODEC_PAIR_REL` 0.8 (reads 0.63, one step off 9.3) - the
    head gate applied twice under tq4 reads 9.2; `DN_LOGIT_BAR_REL_KQ_Q8OUT` 0.08 is the mixed twin's
    bar (one step reads 0.91 of a 12.3 max logit, the rest 0.42-0.60), and `F32_DIST_REL` 1.15 its
    witness against the file's f32 chain on the CPU - the device's summed distance from it over the CPU q8
    chain's own (reads 0.91 at 40 tokens, 3.42 against 3.77, and 0.93 at 600, one step off 21 and 25): at
    the step the old 6% bar missed, the device sits 0.63 and the CPU chain 0.50 from the f32 row, on
    either side of it, and the device's pick is the f32 chain's.
  - `test_gpu_resident_gemma3_1b.das` and `test_gpu_resident_gemma4_e2b.das` hold their block-codec cells
    to the gemma rig's own bars (`GEMMA_LOGIT_BAR_REL` 0.16 under q8_0: gemma-3-1b at 1100 tokens reads
    0.46-1.37 against bars of 3.3-5.1; `GEMMA_PPL_RATIO` 1.05 under tq4: 1.022 against the CPU chain's
    1.033 on gemma-3-1b, 1.032 against 1.032 on E2B). `test_gpu_resident_moe.das` holds its twin under
    q8_0 to `MOE_LOGIT_BAR_REL` 0.20 (reads 0.83-1.17 against bars of 2.1-2.3, the f16 mirror's reading)
    and under tq4 to `MOE_CODEC_DIST_REL` 1.15 (reads 1.06, one step off 5.0) and `MOE_CODEC_PAIR_REL`
    1.4 (reads 1.11, one step off 5.0).
  - `_resident_regions.das` (the driver against itself): under a tq4 mirror `TQ4_CTRL_SHARE` 0.6 holds a
    tolerance row to that share of its distance from the one-token-off row (reads 0.09-0.44) in place of
    a share of the peak, which no tq4 carrier holds - two window splits of one prompt read 0.8-2.0 apart
    at a 12 max logit, the split q/k norm against the fused one 1.1-3.5 at 13-15 (Qwen3-0.6B; that split
    form sits 0.94 of the CPU tq4 chain's distance from the f16 chain, the fused one 1.02), and the
    Llama K-quant carrier, bit for bit under tq4, has a row one off 1.9 away.

  Evidence, through the same run. Kernel cells, `test_vulkan_kv_codec_kernels.das`: `test_vkc_q8_store` and
  `test_vkc_tq4_store` hold the stores byte for byte the CPU's, `test_vkc_q8_da_attn` and
  `test_vkc_tq4_da_attn` the attention stamps within 6e-8 to 2.5e-6 of the CPU attention over the
  dequantized rows at a 1e-4 bar, `test_vkc_shadow` and `test_vkc_tq4_unrot` the shadows and the un-rotation
  bit for bit. GPU against CPU: the cells of the four bullets above, each log carrying `resident driver
  armed` with the mirror's codec (the per-op cells the attention block's census witness) and no `resident
  override passed a call` line for a served call. The driver against itself:
  `test_gpu_resident_regions_q8kv.das` and `test_gpu_resident_regions_tq4kv.das` (26 cells each), and
  `test_vulkan_mint_kv_codecs`. On Qwen2.5-0.5B a tq4 mirror sits 51.6 summed over nine rows from the f16
  chain and the CPU's tq4 chain 54.3 - the codec's own distance, which the distance cells measure on both
  chains (`ARCHITECTURE_GPU_SHARED_ATTN.md#vk-kv-tq4-basis`).
- **MEASURED (2026-10-02) - the remaining families through the server, smallest up.** M5 Max, Metal,
  `dasllama-server` against the pinned llama-server, `harness/served_bench.das` as the qwen ladder
  ran it; the 24B and up at 2 reps (the rows' turns run 17 s a reply on a 40 GB model). Every
  served entry of this date read its system prompt off the corpus a line at a time; the
  instrument reads it a `%%` segment at a time since (`harness/_corpus.das`), the prompt's
  characters pinned by `--sizes` either way, so a re-run's token counts differ by a few. Decode
  tok/s at 3k / 9k, then warm TTFT ms, then cold TTFT:
  - Mistral-7B Q4_K_M 100.5 / 83.1 vs 98.7 / 85.0; 38.4 / 50.8 vs 68.7 / 94.0; 1015 / 4103 vs
    1377 / 5998. Decode a tie at the weight-read floor.
  - Llama-3.1-8B Q8_0 62.8 / 54.3 vs 61.8 / 55.7; 44.2 / 62.1 vs 64.9 / 84.0; 1028 / 4266 vs
    1265 / 6212. The same.
  - Qwen1.5-MoE-A2.7B Q8_0 146.3 / 113.8 vs 134.8 / 105.8; 44.0 / 50.0 vs 48.2 / 61.8; 576 / 1513
    vs 693 / 2702.
  - Mistral-Small-3.1-24B Q4_K_M 35.8 / 23.6 vs 34.9 / 22.8 (both 9k rows void at cv 14-15%: the
    14 GB dense model runs hot); 94 / 167 vs 180 / 369; 3253 / 12966 vs 4151 / 16834.
  - Qwen3.8-27B UD-Q3_K_XL, self-speculative (the file carries its NextN layer, `blk.64`, so
    `--mtp` arms with no head sidecar): 35.7 / 32.0 vs 29.7 / 29.7; 133 / 128 vs 282 / 297;
    3586 / 12520 vs 4553 / 15455. Plain (a TOML `mtp = false` - the `--mtp` flag is a bool and
    `--mtp 0` reads as on) 29.2 at 3k: the round is +22% served, as it is in a flat session
    (`q27probe`: 29.5 -> 36.3 tok/s, 45 of 55 drafts accepted; a paged session 36.2). The split
    `mtp-Qwen3.8-27B-Q8_0.gguf` pinned over it reads 34.8 / 31.4 - the same head, a separate
    `metal-mtp` image lane.
  - Llama-3.3-70B Q4_K_M 12.0 / 7.9 vs 11.5 / 7.8; 243 / 390 vs 601 / 912; 10911 / 40055 vs
    14121 / 55697. Decode at the floor on both; the 3k-to-9k fall is 80 layers of 1024-wide f16
    K/V (2.9 GB a step) on top of the 40 GB of weights.
  - Qwen3-4B Q4_K_M again after the partial-page attach: warm TTFT 36.4 / 67.6 -> 32.4 / 42.4,
    decode 144.1 / 110.9 unchanged.

- **MEASURED (2026-10-02, `direction-grade`) - phi-3 through the server: a head of 96 and an untied K-quant embedding.**
  M5 Max, Metal, Phi-3.5-mini-instruct Q4_K_M (32 heads of 96 over 32 K/V heads - 384 KB of f16 K/V
  a row, so the mirror clamps the context to 10922 and a 9k step reads 3.4 GB of K/V) against the
  pinned llama-server, `harness/served_bench.das` as the qwen ladder ran it.
  - Before: decode 107.5 / 64.4 tok/s at 3k / 9k against 125.7 / 81.5, warm TTFT 96.9 / 213.7 ms
    against 43.6 / 65.1, cold 9k 4894 ms against 4298 - the one family the reference led on every
    row. Three shapes: the fused decode attention's gate refused a head of 96 (the chunked pair at
    depth), the device prefill pair's gate wanted a head on the 64 lattice (the staged trio and the
    panels form, no mirror-fed window), and the greedy chain gathered a winner's row from a Q8 or
    tied-K6 table alone (the untied Q4_K embedding keeps its fp32 table: no chain, a host turnaround
    a step).
  - After the three: decode 125.1 / 80.4 (a tie at the K/V read floor: 7.8 / 12.0 ms a step on the
    trace, 14.1 ms chunked at 9k), warm TTFT 34.4 / 53.2, cold 603 / 2856, the windows `kv=mirror`.
  - The fp32 gather chains every untied K-quant carrier that never chained: the Q4_K_M mistrals and
    llamas, Qwen3-4B Q4_K_M (its 9k row read a tie), gemma-3-12b Q4_K_M.

- **MEASURED (2026-10-02) - the qwen ladder through the server, smallest up.** M5 Max, Metal,
  `dasllama-server` against the pinned llama-server, `harness/served_bench.das` (tagged `served`, `out-of-process`, `direction-grade`,
  f16 KV, 200-token replies, 5 reps, a rest before each life; `--mtp` on the NextN carriers,
  which llama-server serves plain). Decode tok/s at 3k / 9k of context, ours against the reference,
  then warm TTFT ms:
  - Qwen2.5-0.5B Q8_0 498.5 / 409.9 vs 312.5 / 310.8; 11.8 / 18.8 vs 25.5 / 38.4.
  - Qwen3-0.6B Q8_0 366.8 / 252.2 vs 275.8 / 217.5; 13.2 / 21.1 vs 25.6 / 39.1. Both fall with
    depth on the f16 K/V read - 28 layers of a 1024-wide K/V, about a gigabyte a step at 9k.
  - Qwen3.5-0.8B-MTP Q8_0 381.7 / 355.6 vs 244.8 / 267.5; 13.6 / 16.1 vs 27.2 / 33.3.
  - Qwen3-4B Q4_K_M 144.0 / 110.6 vs 137.7 / 111.2; 36.4 / 67.6 vs 51.3 / 69.5 - a tie at 9k:
    the 3k-to-9k cost (2.1 ms a step over 36 layers of 1024-wide f16 K/V) is the read both pay.
  - Qwen3.5-4B-MTP Q8_0 130.3 / 125.3 vs 90.3 / 86.6; 37.7 / 44.1 vs 62.4 / 72.8.
  - Qwen3.5-9B-MTP UD-Q5_K_XL 86.2 / 84.6 vs 71.5 / 68.2; 46.5 / 49.6 vs 93.2 / 106.8.
  - Qwen3.6-27B-MTP Q4_K_M 37.1 / 34.8 vs 26.2 / 18.4; 133.6 / 133.5 vs 276.0 / 497.9. The 16 GB
    dense model sits at the bandwidth floor plain (26 tok/s) and runs hot: its rows read cv 10-22%
    on both servers, the 9k row re-measured alone after a rest (23.7 right after the 3k rows).
  - Qwen3-30B-A3B Q4_K_M 131.2 / 102.2 vs 117.8 / 95.1; warm TTFT 69.0 / 98.3 vs 66.6 / 89.8 -
    the one row the reference led: the prefix cache attached whole 64-row pages (2944 of 3000,
    8768 of 8844 cached) where llama-server reuses all but the turn's new tokens (2981, 8825), so
    the warm turn prefilled 57-76 tokens through the routed experts against its 19. With the page
    past the whole hits copied for its shared rows (`ARCHITECTURE_ENGINE_SERVING.md#prefix-tail-page`)
    the same run reads 2981 / 8825 cached and warm TTFT 48.9 / 61.1 ms; decode 132.8 / 102.4.
  - Qwen3.6-35B-A3B-MTP UD-Q4_K_M 141.3 / 132.1 vs 92.8 / 89.4; 56.8 / 58.2 vs 86.7 / 98.1.

- **MEASURED (2026-10-02, `direction-grade`) - the fused decode attention's block-codec stamps carry every head class.**
  M5 Max, Metal, `benchmarks/decode_step_trace.das -p 9000 -n 128 --bucket 128 --kv q8_0`, GPU ms a
  step, `DASLLAMA_METAL_ATTN_D=0` (the chunked pair) against the fused form.
  - Llama-3.2-1B Q8_0 (a head of 64, 8 K/V heads): 5.98 / 5.99 chunked, 4.87 / 4.89 fused - 167 ->
    205 tok/s at 9k on the q8_0 mirror, where the head of 64 had no quant stamp and paid the pair.
  - gemma-3-1b Q8_0 (a head of 256, one global layer in six at 9k, the rest a 512-key window):
    3.97 chunked, 4.02 fused on the q8_0 mirror - level; the f16 mirror's own A/B on the same run
    reads 3.35 -> 3.17. The wide quant stamp buys nothing a model this light on attention can
    show; it exists so the block codecs leave the chunked pair with the float mirrors.

- **MEASURED (2026-10-02, `direction-grade`) - the server attaches gemma-4's assistant drafter, and the same-slab
  verify's arena demand is the session's slice once.** M5 Max, Metal, gemma-4-26B-A4B-it Q4_K_M
  with `mtp-gemma-4-26B-A4B-it-Q8_0.gguf` beside it, through `dasllama-server --mtp` and
  `harness/served_bench.das` (tagged `served`, `out-of-process`, `direction-grade`, f16 KV, 200-token replies, 5 reps, a rest before
  each life).
  - Plain, the 26B decodes 112.9 / 107.0 tok/s at 3k / 9k of context (the pinned llama-server
    92.6 / 87.1). The server never attached the drafter: its speculation gate asked for a NextN head.
    Attached, 139.8 (void) / 49.8 - the 9k row a regression, every round a draft, a declined verify
    and a plain step. The batch driver summed each verify row's `mirror_arena_demand`, and the
    same-slab verify names ONE session for every row: at 9k the 26B's slice is a 16384-row cap, and
    five of it pass the 4096 MB arena ceiling, where five of the 4096-row slice at 3k did not.
    Counted once: 136.7 (cv 3.7%) / 131.2 (cv 4.0%) tok/s, warm TTFT 51.8 / 66.8 ms; the decode
    cv is the rounds' acceptance moving turn to turn, and sits past the 3% bar on both rows.
  - The scheduler probe (a 60-token free run on a number ramp at 9k, `MetalMode.required`): the
    exception `batch decode declined: mirror` before, 10.6 ms a token after, 42 drafted / 16
    accepted - the ramp accepts poorly, prose through the server far better.

- **MEASURED (2026-10-02, `direction-grade`) - the fused decode attention serves a head of 512 and a model of two head
  classes, its wide stamps a slice group a threadgroup.** M5 Max, Metal, gemma-4-E2B-it Q8_0 (a
  head of 512 on the full-context layers, of 256 under a 512-key window on the sliding ones, 8
  heads over one K/V head) through `dasllama-server` and `harness/served_bench.das` (tagged
  `served`, f16 KV, 200-token replies, 5 reps, a rest before each life).
  - Decode at 3k / 9k of context: 141.2 / 126.6 tok/s on the chunked pair, 145.0 / 139.2 on the
    fused form; the pinned llama-server 130.3 / 122.5. The same run: warm TTFT 27.9 / 35.8 ms
    against 51.2 / 63.5, cold 304 ms (void) / 956 against 511 / 1705.
  - The wide stamps' first form - a lane walking two or four quads of the head in turn - lost to
    the chunked pair at every depth (`decode_step_trace`, GPU ms a step at 400 / 3000 / 9000
    keys: 6.07 / 6.44 / 7.33 chunked, 6.63 / 7.17 / 7.78 fused): a decode step is some 540
    serial dispatches, so a dispatch costs what its longest threadgroup takes, and a slice took a
    wide head four times a narrow one's time. With a threadgroup per slice group and a simdgroup
    per 128 dims, a slice's scores summed through threadgroup memory, the fused form reads 6.07 /
    6.26 / 6.56. More slice groups on a wide head (64 against 16) read worse at depth (6.70 at
    9000 with the split form), so the cap stays at 16.
  - The head-of-256 stamp takes the same form: gemma-2-2b-it Q8_0 decodes 154.0 / 143.0 tok/s at
    3k / 7k (145.3 / 135.0 on the lane-walking form, llama-server 140.8 / 132.2), and
    Qwen3.6-35B-A3B under self-speculation 144.9 (void) / 134.3 at 3k / 9k (144.4 / 132.1).
  - The narrow row (Llama-3.2-1B Q8_0, two streams at 512 / 9000) reads 532.9 / 391.0 tok/s
    against 533.3 / 394.2 before: the narrow stamps' source is unchanged.

- **MEASURED (2026-10-02, `direction-grade`) - the fused decode attention carries the attention logit soft cap.**
  M5 Max, Metal, gemma-2-2b-it Q8_0 (a head of 256, the cap, a sliding window on alternate
  layers; context 8192) through `dasllama-server` and `harness/served_bench.das` (tagged
  `served`, f16 KV, 200-token replies, 5 reps, a rest before each life). Decode at 3k / 7k of
  context: 142.3 / 124.4 tok/s on the chunked pair, 145.3 / 135.0 on the fused form; the pinned
  llama-server 141.4 / 132.5. The same run: warm TTFT 25.3 / 30.4 ms against 118.1 / 63.3, cold
  3k 288 ms (void) against 467. The two-stream batch row on a model with no cap (Llama-3.2-1B
  Q8_0, `lcpp_bench --npl 2 --npl-plen 512 | 9000`) reads 535.3 / 393.3 tok/s after the kernel
  took the uniform, 533.3 / 394.2 before. The kernel cells' cap was 30 on a fixture whose scores
  stay under 1, where an uncapped kernel passed; they cap at 0.05 now.

- **MEASURED (2026-10-02, `direction-grade`) - a prefill window's K/V rows live in the KV mirror, and a released
  session's mirror goes to the next session on the same pages**
  (`ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-kv-mirror`). M5 Max, Metal, `dasllama-server`
  through `harness/served_bench.das` (tagged `served`, `out-of-process`, `direction-grade`, f16 KV, 200-token replies, 5 reps, a rest
  before each model, the same text on both servers). Before: every continuation gathered the
  cached rows on the host into f32 panels (11 ms at 3k of context, 33 ms at 9k on Llama-3.2-3B),
  and the first decode step uploaded them again into the f16 mirror. After: a warm turn's window
  finds the previous turn's mirror under its attached pages and copies nothing.
  - Warm TTFT at 3k / 9k of context, ms, before -> after (reference):
    Llama-3.2-3B Q8_0 42.8 / 77.6 -> 29.4 / 34.9 (pinned llama-server 38.2 / 51.5, the 9k row
    void at cv 3.9%); Llama-3.2-1B Q8_0 19.9 / 33 -> 16.2 / 19.8 (llama-server 24.2 / 24.3, both
    void); gpt-oss-20b mxfp4 58.5 / 82.3 -> 51.6 / 59.1 (llama-server 85.7 / 103.9, the earlier
    rested run - today's reference life answered 503 at its calibration turn);
    Qwen3.6-35B-A3B UD-Q4_K_M with self-speculation 65 / 79 -> 51.8 / 56.2 (mlx_lm.server on the
    4-bit MLX quant 72.4 / 87.5).
  - The 3B's warm window at 3k: host setup 11.4 -> 0.07 ms, GPU 25.9 -> 23.1 ms for the 52
    uncached rows; a cold window's K/V readback 20 -> 6 ms (halves copied, no host convert); the
    two-row window that closes a turn 30-45 -> 17 ms.
  - Cold TTFT and decode read inside their run-to-run spread on every model (the cold rows void
    on both servers at these reps).
  - The form serves under the `attn_dev` crown on f16 KV only (`followup_metal.md` row 36).
    `MetalCvtHalf` and `MetalKvTwin` are `MetalHalfRows` now: one half converter, scaled, at a
    byte offset.

- **MEASURED (2026-10-02, `direction-grade`) - a weight plane cached before the residency set existed was never
  pinned, and a prefill after a decode turn repaid its wiring.** M5 Max, Metal, Llama-3.2-3B
  Q8_0, `dasllama-server` through `harness/served_bench.das` (tagged `served`, `out-of-process`, `direction-grade`, f16 KV, 200-token
  replies, 5 reps). The image's baked dev-W plane (5.4 GB, a page wrap of the mapped file) is
  seeded ahead of the first `residency_flush`, and `residency_note` was a no-op until that flush
  made the set. A decode turn longer than about a second un-wires the plane; the first command
  buffer of the next prefill then spends 20 to 24 ms in the kernel driver span (kernelStart ->
  kernelEnd), its GPU time unchanged. A 100-token reply (0.76 s) does not show it, a 150-token one
  (1.15 s) does; `DASLLAMA_METAL_RESIDENCY=0` reads the same, the plane being outside the set
  either way.
  - The first note makes the set. Warm TTFT at 3k / 9k of context: 61.7 / 100.0 ms before,
    42.8 / 77.6 after (the pinned llama-server, rested: 37.2 / 52.6). A cold prefill that follows
    a decode turn loses the same span.
  - What is left of a warm turn at 3k: 11 ms gathering the cached history into the f32 K/V panels
    on the CPU (33 ms at 9k - it scales with the cached rows), 26 ms of GPU for the 52 uncached
    rows, 4 ms of HTTP. The gather is the next cost: the decode mirror holds the same rows in
    f16, and a continuation that read its history there would copy nothing.

- **MEASURED (2026-10-02, `direction-grade`) - the single row's decode takes the fused single-pass attention past the
  single-dispatch ceiling.** M5 Max, Metal, Llama-3.2-1B Q8_0 (16 layers, 32 heads of 64 over 8 KV
  heads), f16 KV, `bin/daslang -jit modules/dasLLAMA/benchmarks/decode_step_trace.das -- -m <gguf>
  -p <depth> -n 256 --bucket 256`, one process a depth, greedy chain adaptive; debug-jit readings,
  not board rows. The step's wall is its GPU time at every depth.
  - ms a step at depth 0 / 3000 / 9000: 2.90 / 3.61 / 5.41 on the chunked pair (the single row's
    form before), 2.77 / 3.02 / 3.50 on the fused form - 344 / 275 / 182 tok/s against
    361 / 329 / 280. The chunked pair costs about 0.28 us a cached key a token, the fused form
    about 0.08.
  - The chunked pair's depth cost does not follow the bytes it reads: at depth 9000 it adds 2.51 ms
    on the f16 mirror, 3.06 on f32, 2.80 on q8_0 and 2.54 on tq4.
  - At this entry's reading the chunked pair still served a head of 64 or 256 on a block-codec
    mirror and every model with a soft cap or a second head class; the entries above it moved each
    of those to the fused form, and the pair stays as its `DASLLAMA_METAL_ATTN_D=0` control.
  - The fused form carries a sliding layer's window and a layer's attention sinks. gpt-oss-20b
    mxfp4 (a head of 64, sinks, 128-key sliding layers on alternate layers), `decode_step_trace`
    as above with `DASLLAMA_METAL_ATTN_D` 0 then 1: 134.8 tok/s at depth 3000 and 101.5 at 9000
    on the chunked pair, 154.6 and 140.2 on the fused form. Through `dasllama-server`
    (`harness/served_bench.das`, tagged `served`, `out-of-process`, `direction-grade`, a rested box, the same text on both servers):
    decode 132.1 / 99.9 tok/s at 3k / 9k of context before, 150.5 / 135.4 after; the pinned
    reference server reads 143.0 / 133.1. A probe reading, not a parity cell: against the CPU
    chain over 400 forced steps (`harness/forced_feed_probe.das <gguf> --steps 400`) the fused
    form's largest logit difference is 3.48 and its mean 1.02 with no argmax flip, the chunked
    pair's 3.45 and 1.01; the gates are arm11 of the decode suite and the matrix's two-class cell.
  - The speculative verify's rows and a head of 256 (the WIDE stamps) take the fused form too.
    Qwen3.6-35B-A3B UD-Q4_K_M under self-speculation, one stream through `dasllama-server`
    (`harness/served_bench.das --sizes 3000,9000 --reply 200 --reps 5`, tagged `served`, `out-of-process`, `direction-grade`, both
    decode rows past the 3% cv bar - the accept rate moves with the reply): 138 tok/s at 3k of
    context and 115 at 9k on the chunked pair, 147 and 133 on the fused form; `mlx_lm.server`
    on its 4-bit build reads 116 and 112 in the same series.
  - The four narrow stamps' source moved by the shared rescale helper and two regrouped
    expressions, the arithmetic unchanged; the batch row that rides them
    (`lcpp_bench --ngl 99 --npl 2 --npl-plen 512|9000 -r 3 --for-debug-purposes`, Llama-3.2-1B
    Q8_0) reads 540.9 / 396.6 tok/s summed before, 535.0 / 394.9 after the rescale helper, and
    533.3 / 394.2 with the window start and the masked key in the slice loop.

- **MEASURED (2026-10-01) - the attention and router races on the stamps that ship, and the bytes
  the folded snapshot holds.** M5 Max, `bin/daslang -jit modules/dasLLAMA/harness/router_race.das`
  and `.../attn_depth_race.das`, one process each, no flags; both harnesses dispatch the engine's
  own stamps through their generated builders, where they timed hand-written copies before.
  - `router_race` (2048 positions x 256 experts over 2048, best of its rounds): the slab GEMV
    (`MetalMoeRouterB`) 0.647 ms, off the double dot by 8.1e-7 of 4.24; the half tensor GEMM
    (`MetalHalfMulMmTH128`) 0.0485 ms, x13.3, off by 1.39e-3.
  - `attn_depth_race` (2048 queries at 8192 rows of context, 16 heads of 256 over 2 KV heads,
    cumulative stages): QK 6.25 ms staged against 3.02 device; + softmax 7.85 against 5.37; + AV
    19.84 against 8.06; + the K/V twin pass 19.85 against 8.12 - x2.44. Against the attention
    computed in double from the f32 operands the staged trio is off by 7.4e-7 and the device pair
    by 7.0e-7 of 8.2e-4. The lab copies read 23.1 against 8.2 ms on the same shape (the device
    attention pair's entry below, direction-grade: another commit).
  - A speculative session's rollback snapshot (`Session.mtp_snap`) holds the conv history, the
    recurrent state and the n-gram ring: `(long_length(dn_conv_state) + long_length(dn_state) +
    ple_hist_rows x hc_dim) x 4` bytes a session, sized on the first CPU-rail snapshot - 151 MB of
    state at a 27B hybrid's geometry (48 recurrent layers x 32 heads x 128 x 192), the ring
    `((ple_conv_kernel - 1) x ple_ngram_size + 1) x hc_count x dim` floats on a hyper-connection
    carrier and nothing elsewhere. The conv and state buffers were the session's before; the ring
    is the addition.
- **MEASURED (2026-10-01) - a plain MoE the card does not hold whole, with its context asked whole
  (`ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool`, `set_gpu_ctx_strict` / the server's `--ctx`): the first routed
  layers' experts on the host through the split token command and the cut window chain, the mirror whole
  - the form wins the long prompt by 5.7x and loses decode by a third to the per-op rails
  (direction-grade), so it arms only where the context was asked.** zen2 (Ryzen 16 threads, RTX 5060 Ti 16 GB on a PCIe 4.0 x8 link, the
  desktop holding 4.6 GB of the card), Qwen3-30B-A3B-Instruct-2507 Q4_K_M (17.3 GB, 48 routed layers of
  128 experts, 8 picked; the plan puts the first 28 layers' experts on the host beside a 32768-position
  f16 mirror, no pool fits), `bin/Release/daslang.exe -jit modules/dasLLAMA/benchmarks/lcpp_bench.das --
  -m <file> --ngl 0 -p <P> -n <N> -r 1 -t 16 --for-debug-purposes` under `DASLLAMA_ALLOW_UNTUNED=1
  DASLLAMA_GPU=1 DAS_JOBQUE_THREADS=16` (`DAS_TUNE_POLICY`, `DASLLAMA_COOPMAT` unset; the second row adds
  `DASLLAMA_GPU_CTX_MAX=32768 DASLLAMA_GPU_CTX_STRICT=1`, the pin the form arms under, the first
  `DASLLAMA_GPU_RESIDENT=0`), one rep, one process a row - a second load of a model in one process is barred,
  so the two forms cannot race interleaved - direction-grade across the rows and in every sentence below
  that sets one row against the other:

  | form | pp512 | tg128 | pp8192 | tg64 after 8192 |
  | :--- | ---: | ---: | ---: | ---: |
  | the per-op rails (`DASLLAMA_GPU_RESIDENT=0`: master's shape for this file) | 398.14 | 64.03 | 93.39 | 64.52 |
  | the whole-model driver, 28 of 48 layers' experts on the host, the mirror on device | 517.88 (290.36 with the first window's one-time cost inside the pass) | 41.81 | 533.97 | 47.79 |

  The second row's decode cells were taken while the split token command embedded the last landed pick in place
  of the fed token on a model whose table gathers on the device (every step the same token, the routing frozen
  with it); re-measured 2026-10-16 with the fix, the same command, environment, box and one rep: pp512 348.31
  (the first window's one-time cost inside the pass), tg128 48.40, pp8192 431.02, tg64 after 8192 46.79 -
  direction-grade, and every sentence below that sets one row against the other stands.

  The per-op rails' decode does not slow with the position (64.0 at 640, 64.5 at 8192, direction-grade); the
  host-layer form's token is 24 ms against 15.6 (direction-grade) - the 28 fence round-trips a token, the cost
  the hyper-connection carrier pays too - so it keeps the per-op rails' place for an unasked context and
  takes the asked one, where an 8192-token prompt costs 15 s against 88 s (direction-grade). Its prefill is
  flat with the length: a 512-row window records in 0.85-0.90 s and submits in 0.07-0.09 s whatever the
  position (the same command and environment as the second row with `DASLLAMA_GPU_PROF=1` added, its
  `vk_rdpf` lines; pp2048 reads 551.5 there), and the first window of a process pays 0.45 s once (the host
  callback's scratch, the host combine's pipelines) - a pp512 pass that includes it reads 290. The form's
  landings are small beside the planes: the decode's `hx_host`, `hacc_host` and `hacc_dev` hold `dim*4*nb`
  bytes each and `hlog_host` `ner*4*nb` (2048 x 4 x 1 = 8 KB and 128 x 4 = 512 B here), the prefill's
  `pf_hx_host`, `pf_hacc_host` and `pf_hacc_dev` `np*dim*4` each and `pf_hlog_host` `np*ner*4` (4 MB and
  256 KB at the 512-row window), `pf_ident_dev` and `pf_ones_dev` `np*4` (2 KB), the decode's `ones_dev`
  `MAX_ROUTED_SLOTS*4`; the pool over the host layers takes one slot's gate, up and down planes per hosted
  layer per slot (`resident_hot_slot_bytes_host_layers`), none here. The same file on this card at the bench's shape
  before this change: the whole-model driver declined (17.3 GB of weights past the room) and the per-op
  rails served, which is the first row. Parity: `test_gpu_resident_moe.das`'s `test_gpu_resident_moe_host_layers`
  (this file at 32768 asked whole, 27-28 layers hosted, one and two windows) and
  `test_gpu_resident_moe_ctx_strict` (Qwen3.6-35B-A3B UD-IQ2_XXS at 131072 asked whole: 10 of 40 layers
  hosted, the region 131072 positions, two windows) pass on this box, run as `bin/Release/daslang.exe -jit
  dastest/dastest.das -- --test modules/dasLLAMA/tests/test_gpu_resident_moe.das` under `DASLLAMA_GPU=1
  DASLLAMA_PARITY_FULL=1 DAS_JOBQUE_THREADS=8`; the file's one-window hybrid and no-shared-expert cells
  red on the cm2e census floor on this card (117 of 120, 134 of 144) exactly as on master run through
  this binary - a miss of the witness's own format list, which lacked `iq3s4`, the form this box serves
  IQ3_S planes in - unchanged by this entry.
- **MEASURED (2026-10-01) - the server's ASR worker on the team beside a GPU-decoding LLM: a
  64-second parakeet clip 281 -> 140 ms a transcription.** M5 Max, `dasllama-server` with the
  Qwen3.6-35B-A3B on Metal (streams 1) and parakeet-tdt-0.6b-v3 f32, `POST
  /v1/audio/transcriptions` seven times, medians, wall time at the client (out-of-process;
  direction-grade across the rows: a server life a dispatch mode).

  | ASR worker | LLM idle | while the LLM decodes 400 tokens | that decode, tok/s |
  |---|---|---|---|
  | inline (`hybrid` before) | 281 ms | 390 ms | 136 |
  | team (`--team-dispatch team`, and `hybrid` now) | 140 ms | 178 ms | 138 |

  A Metal-served decode publishes nothing to the team, so the worker's joint, predictor and mel
  split across it at no cost to the stream. mlx-audio 0.5.7 reads 162 ms on the clip in its own
  process (external: `model.generate(clip)` on `mlx-community/parakeet-tdt-0.6b-v3` loaded through
  `mlx_audio.stt.utils.load_model`, the median of five calls after one warmup).
- **MEASURED (2026-10-01) - the FastConformer block loop on the half tensor GEMM: 115 -> 63 ms for
  a 64-second clip.** M5 Max, parakeet-tdt-0.6b-v3 f32, `harness/asr_stage_probe.das -r 5` on a
  64 s clip; every before -> after pair of this entry is the probe on the tree before and after
  the change (direction-grade), save where a sentence says one process. Per encode: 187 -> 133 ms, of which the CPU front stays 66 and the rel table 6. The
  chain's weighted GEMMs ran the f16-staged simdgroup tile stamp (`enc_f32_mm`, ~4.8 T MAC/s over
  the ~570 G MAC of 24 blocks); they read a device halfword twin of the blob through the half x
  half tensor GEMM now. The encoder rows read 0.0042 rel-l2 from the CPU rail on the twin and
  0.0039 on the tiles (jfk), the transcript the CPU's on both - every rel-l2 on jfk in this entry
  is `test_parakeet_tower_metal`'s log (`tests/test_model_image.das`, `tests/run.das --suite image
  --arm mtower`), the 64 s clip's the probe's.
  - What the 63 ms holds, by leaving parts out (best of three, three rounds): the per-head
    attention loop 25 (41 dispatches a block - a pack, three tile GEMMs and a softmax a head), the
    weighted GEMMs 27 (~18 T MAC/s with their half converts), the norms, the depthwise conv and
    the rest ~10.
  - mlx-audio 0.5.7 on the same clip: 162 ms whole (external, the call of the entry above; its
    encoder 64 and its decode loop 97 timed apart in the same process). Ours whole, team
    dispatch: ~220 (front 66, blocks 63, rel table 6, mel 8, decode 80).
  - **The subsample front on the device, the rel table kept a session: the encode 133 -> 66 ms,
    the clip whole ~153.** The front's three convs, two 1x1 GEMMs and the pre-projection run ahead
    of the blocks in the same command buffer (66 ms of CPU, team-dispatched, to under 5), and the
    rel table is built once a frame count (6 ms an encode before). The same probe, per pass:
    encode 66, decode 79 (the predictor 38, the joint 25, the pick 13, the encoder projection 2),
    mel 8. Rows 0.0057 rel-l2 from the CPU rail (jfk), the transcript the CPU's.
  - **The TDT joint as a column pass over its live inputs, the pick in one exp pass: decode 79 ->
    61 ms, the clip whole ~133.** The joint row is a ReLU's output, so the joint net's weight laid
    as input columns adds only the live ones (a transposed copy a session, 21 MB): the joint 25 ->
    16 ms with the logits split across the team's lanes. One thread alone runs it 56 - the weight
    streams from memory either way, and the row-dot it replaces had the team. The token mass is
    the log-sum pass's partial sum: the pick 12.7 -> 6.9. Left: the predictor 31.5 (four 640 x
    2560 GEMVs a token over 26 MB of f32).
  - **Run inline - one thread, as the server's ASR worker does by default - the same clip reads
    ~263 ms:** encode 69, decode 165 (the predictor 86, the joint 45, the encoder projection 23,
    the pick 7), mel 29. It read 790 before the front and the twin.
  - **The attention over every head at once on the device pair: the encode 57.0 -> 44.4 ms.** One
    process, the two forms in turn, seven timed encodes a form after two dropped (per head 56.9 -
    57.1, all heads 44.3 - 44.5), under the `attn_dev` crown. Five dispatches a block replace
    forty-one: the half operands, the pair's QK twice (content, rel), the rel-shift softmax, the
    pair's AV. Rows 0.0056 rel-l2 from the CPU rail on the clip against the per-head form's 0.0059
    (jfk: 0.0042 against 0.0057), the transcript the CPU's. Not taken: the rel GEMM computes the
    whole `mp x wwpad` slab though a row reads `npos` of its `2 npos - 1` columns - a band skip
    halves its 31 G MAC and its slab's bytes.
  - **The decode step as one team publish over half-precision weights: decode 61 -> 33 ms, team
    dispatch (inline 165 -> 146).** `asr_stage_probe -r 5`, the 64 s clip, per pass. By change:
    the predictor's weights read as halves, 31.5 -> 15.7 ms - twice the rate at half the bytes,
    so a token's predictor is bound by the 26 MB it read; the joint's columns as halves, no change
    (13.9) - its lanes read a column table in short runs a column apart; the predictor's stages
    and the joint in one publish, the two together 29.6 -> 24.6; the joint table in 256-logit
    blocks, a token step 88 -> 81 us and a blank step 50 -> 34 (a 512 block reads 107 and 48, a
    128 block 82 and 30, with the inline joint 147 -> 159 us); the log-sum's cut and the max and
    argmax in one pass, the pick 6.8 -> 3.1 ms. Now, per pass: a token step ~83 us x 260, a blank
    step ~40 us x 42, the pick 3.1 ms, the encoder projection 2.6. A bare six-stage publish over
    18 lanes measures 14 us and a one-stage one 5, so what is left is the step's bytes - 7.4 MB
    of predictor halves and ~5 MB of live joint columns a token, about 150 GB/s. The transcripts
    are the reference's token for token (jfk, v2 and v3).
  - **What the routes hold, in bytes.** The device twin of the blob is half the blob: 1.25 GB for
    the 2.51 GB 0.6B file, resident from the first encode to the weights drop. The all-heads score
    slabs are `heads x mp x (nkp + wwpad) x 2` bytes - 33 MB at 64 s of audio and eight heads -
    and the route stands down past 256 MB. A session's decode halves are `(8 layers + 1) x pd^2 x
    2` bytes for the predictor (13.9 MB at two layers of 640) and `blocks x pd x 256 x 2` for the
    joint table (10.8 MB at 8198 logits).
- **MEASURED (2026-10-01) - the prefill chunk on the CPU: a prompt alone runs flat from 512 tokens
  up, and a decoding stream waits one chunk's time a token while another prompt prefills.** M5 Max
  CPU, `utils/dasllama-server/server_bench.das -b 1 -n 32 --long-prompt 2048 --chunks
  64,128,256,512,1024,2048`, one process a model, paged sessions.

  | chunk | 64 | 128 | 256 | 512 | 1024 | 2048 |
  |---|---|---|---|---|---|---|
  | Llama-3.2-3B Q4_K_M, lone prompt, tok/s | 357 | 388 | 401 | 410 | 413 | 402 |
  | its decoding stream's gap, p95 ms | 204 | 378 | 736 | 1425 | 2733 | 5020 |
  | Qwen3.5-0.8B Q8_0, lone prompt, tok/s | 1343 | 1658 | 1832 | 1892 | 1788 | 1991 |
  | its decoding stream's gap, p95 ms | 50 | 92 | 153 | 283 | 547 | 1032 |

  The idle quantum at 512 is right for the CPU - nothing past it. The chunk a decoding stream
  waits out is a token count, so its stall is the model's: 512 tokens is 0.28 s on the 0.8B and
  1.4 s on the 3B (its token gap is 14 ms with nothing prefilling).
  - **The CPU's chunk beside a decoding stream defaults to 64.** The stall follows the chunk
    down (50 ms on the 0.8B, 204 on the 3B, ~460 on Mistral-7B Q4_K_M) and the prefilling prompt
    pays 13 - 29% of its rate, only while a stream decodes; under 32 tokens a batch leaves the
    batched kernels (the 7B's 2048-token prompt read 19 s against 11). A GPU's chunk stays one
    window. Measured and not taken: sizing the chunk to a 250 ms stall from the last chunk's
    rate - on the 3B and the 7B it lands on 64 anyway (311 - 324 and 459 - 462 ms with a
    64-token floor), and on the 0.8B it keeps 512 (268 - 283 ms); `--chunk` is the same knob.
- **MEASURED (2026-10-01) - both engines held to what an M4 can run, on the M5 Max: prefill 1.3 -
  1.4x mlx-lm's, decode level, the warm hit 20 - 30 ms behind.** The 35B through each server
  (ours streams=1 with speculation; the series of the checkpoint entry below, medians of three,
  two spaced lives an arm). Ours under the box's manifest with its `metal_tensor` list replaced by
  the M4 Pro's (`records/m4.tune.2b2e79baf8ab.json`: `mulmm_bf16`, `kq_mvb2_k4_r2` and the
  `kq_rows_*` forms - no routed, K-quant GEMM or attention tensor crown); mlx-lm 0.32.0 over MLX
  0.32.3 built with a deployment target of 15.0, which compiles its Metal-4 kernels out
  (`MLX_METAL_NO_NAX`; no `_nax` kernel in its metallib, 6322 in the stock one). The mlx-lm columns
  are external - `mlx_lm.server --model mlx-community/Qwen3.6-35B-A3B-4bit --chat-template-args
  '{"enable_thinking":false}' --max-tokens 4096 --prompt-cache-size 32 --prompt-cache-bytes
  8589934592`, the same client series - and every column is direction-grade: a server life an arm.

  | | ours, M4 crowns | mlx-lm, no Metal-4 kernels | ours, M5 crowns | mlx-lm, stock |
  |---|---|---|---|---|
  | cold 3k, tok/s (TTFT) | 2773 / 2794 (1.07 s) | 1980 (1.50 s) | 4959 (0.60 s) | 4480 (0.66 s) |
  | cold 9k, tok/s (TTFT) | 2517 / 2534 (3.6 s) | 1892 (4.82 s) | 4603 (1.98 s) | 4217 (2.16 s) |
  | warm TTFT, 3k / 9k | 0.09 / 0.10 s | 0.06 / 0.08 s | 0.05 / 0.08 s | 0.06 - 0.08 s |
  | decode, 3k / 9k, tok/s | 121 / 103 | 113 - 120 / 106 - 111 | 139 / 114 | 110 / 105 |

  - The warm hit's short window takes the in-kernel gather K4 form without the routed tensor
    crown: 64 ms of GPU for 20 tokens against 43 on the M5's crowns.
  - The numbers are the M5 Max's GPU on the M4's forms, not an M4's: the ratio between the engines
    is what reads across, and the M4's own race predates the device attention pair and the half
    scan.
  - **A life run inside 14 minutes of back-to-back GPU load reads 1.5 - 1.8x slow with no thermal
    warning from the OS.** Ours: cold 3k 1568 tok/s, decode 91 / 85, a 2945-token window 950 ->
    1740 ms of GPU; five minutes idle and four between lives put it back (2773, 2794). mlx-lm's
    lives in the same heat read 1705 and 1524 against 1980 cool. A series is read from spaced lives.
- **MEASURED (2026-10-01) - pointer loads in every fully unrolled Metal kernel loop: 3% of a dense
  K-quant window, nothing on the 35B's served path.** 88 loads over the K-quant, IQ and Q4_0 GEMV
  and staging kernels, the legacy routed stamps, the deltanet norm and gate and the staged
  attention moved from `buf[base + k]` to a pointer taken outside the loop
  (`ARCHITECTURE_GPU_PREFILL_WINDOW.md#kernel-load-addressing`). M5 Max, Mistral-7B Q4_K_M,
  `p0_ko_probe -n 512 --kprof --text skills/design_philosophy.md`, the indexed tree then the
  pointer tree (direction-grade): the dense K4 stage 69.7 -> 66.6 ms and 19.8 -> 19.3, K6 20.3 -> 20.4, the window
  as it serves 149.7 -> 145.4 ms. The 35B through the server reads as before the sweep (cold 3k
  0.62 s, 9k 2.02 s, decode 139 / 114 tok/s) - its expert stamps and its scan took the form
  already. Decode on the dense model is not read: `lcpp_bench -n 128 -r 12` as a `-jit` script
  spreads 20% a row (78 - 86 +- 13 - 18 tok/s), past any difference the loads could make.
- **MEASURED (2026-10-01) - a reused prompt on a deltanet hybrid attaches a checkpoint; warm time
  to first token 0.6 s -> 0.05 s at 3k, 2.1 s -> 0.08 s at 9k.** M5 Max, the 35B, the server at
  streams=1 with speculation, a client script outside the tree (`bench_llm.py <server url> <model>
  <tag>`: a fixed system prompt cut from `skills/design_philosophy.md`, a primed request, three
  more with other user turns over `/v1/chat/completions`; client side, medians of three;
  out-of-process, and direction-grade against the tree before the change): warm 3k 0.05 s (server
  side 48 / 48 / 60 ms), warm 9k 0.08 s (57 / 58 / 71 ms), every request after the prime a hit;
  mlx-lm 0.32.0's server on the same series reads 0.06 - 0.08 s (external, the `mlx_lm.server`
  command of the entry above). Cold is where it was: 3k 0.60 s (4959 tok/s), 9k 1.98 s
  (4603), decode 139 / 114 tok/s. A hit's time is one short window: 17 - 21 tokens read 43 ms at no
  depth and 45 ms of GPU at 9k, plus 6 - 10 ms of setup.
  - **The short window was 59 ms: the routed gate and up took the in-kernel gather form below 256
    tokens.** `p0_ko_probe -n 17 --kprof --depth 9108`: `metal_moe_mulmm_k4` 38.8 of 74 ms. The
    gathered half panel at every window (`--gather-min 256,1`, three interleaved rounds each):
    17 tokens 58.9 -> 42.7 ms, 64 80.3 -> 56.0, 128 95.4 -> 65.8, 224 119.2 -> 83.5; the logits
    move 0.16 - 0.67 of a largest 20 - 25, the half twin against the integer form.
  - **What is left in a short window is the weight stream.** 17 tokens pick 136 expert rows over
    77 experts, each a 32-row tile: the K4 pair 17.2 ms and the K5 down 7.0 at 0.2 ms a dispatch,
    which is 590 KB an expert plane at ~137 GB/s - a decode step's 7.3 ms plus 4.3 us an expert
    plane past its eight. A shorter tile does not buy it back (an 8-row tile costs 0.62 of a
    32-row one).
  - **Stopping a prefill at the checkpoint costs one window, so the chunk ends there.** With the
    chunk left at its quantum the cold 3k prompt ran 2048 + 897 + 20 tokens, 0.65 s (4556 tok/s);
    a checkpoint within a chunk and a half ending the chunk runs 2945 + 20, 0.60 s (4959).
  - Open: the seam row of the draft head declines at the first window of every attached prompt
    (a quality-only miss, 26 - 34 a series, as before the cache); the close tokens of a finished
    turn eval as a 2 - 3 token window, 20 - 35 ms after the reply's last token.
- **MEASURED (2026-10-01) - the job queue's workers spinning the CPU window through Metal work
  are the box's "slow mode".** M5 Max, the 35B, `p0_ko_probe -n 2048 --depth 10240 --text
  skills/design_philosophy.md --hold none`, four walks of five windows back to back. Under the
  30 ms CPU spin window (`--cpu-spin`) the process holds 1086-1296% CPU while the GPU does the
  work (a sample: the workers in the spin loop's clock read) and the walks read 366 / 388 / 417 /
  421 / 475, then 361 / 379 / 400 / 417 / 439, then 363 / 389 / 426 / 466 / 512, then 430 / 458 /
  481 / 504 / 521 ms - 19% slower from the third walk on. Under the GPU-served window (500 us;
  13-16% CPU) the four walks read 365 / 388 / 415 / 421 / 468, 361 / 380 / 400 / 417 / 440, 359 /
  379 / 399 / 418 / 438, 362 / 382 / 409 / 429 / 452. The Metal overrides now report a served
  forward (`metal_served`). The server, cold prompts, one stream, MTP on, before -> after
  (direction-grade, the client series of the checkpoint entry above): 3k 4820 -> 4959 tok/s, 9k
  4224 -> 4574; decode 138 / 111 -> 137 / 113. mlx-lm 0.32.0's server on the same series: 4480 /
  4217, decode 110 / 105 (external). A later request of a long series still creeps
  (9k windows 431 / 460 / 482 / 506 against 362 / 380 / 400 / 418 in the probe) with the CPU idle:
  that part is the GPU's own.
  - **The largest gap this entry left on the reused-prompt case:** a deltanet hybrid attached
    no prefix, so a repeated 9k system prompt cost its whole prefill again (TTFT 2.1 s) where
    mlx-lm restores a checkpoint (0.06-0.08 s) - closed by the checkpoint entry above.

- **MEASURED (2026-10-01) - a Metal prefill window by kernel, on both engines, and the three
  forms the reading bought.** M5 Max, Qwen3.6-35B-A3B-MTP UD-Q4_K_M, the device attention pair
  crowned. `p0_ko_probe --kprof` replays every captured dispatch in a command buffer of its own
  (`metal_kernel_profile`), so a kernel's GPU time reads on the rows the window really computes:
  the kernels sum to 436 ms where the window serves in 428, so nothing hides in overlap. mlx-lm
  0.32.0 the same way (external: its model's `__call__` of every module class wrapped to force an
  evaluation, a script outside the tree; 536 ms against 363 as it serves, about 0.15 ms of sync a
  call, so its big modules read true and its small ones high). A 2048-token
  window at depth 0, ours ms (mlx-lm instrumented, external): the recurrent scan 70 (41); the expert GEMMs
  146 - K4 96, K5 47, K6 3 (gate 44 + up 53 + down 43); the router 27 (9); the dense half GEMMs
  95 (in_proj_qkv 41, z 25, out 25, shared expert 31, attention projections ~38); reduce 12,
  activation 11, gather 8, count 7, bucket 5; the attention trio 11.
  - **The router as a half GEMM** (`pf_enc_router_mm`; `harness/router_race.das`, 2048 x 256 over
    2048): the slab GEMV 0.647 ms, the tensor GEMM over halves 0.048 - x13.4, off the double dot
    by 1.4e-3 of 4.2 against the GEMV's 8e-7. The window 428 -> 402 ms.
  - **The scan** (`harness/dn_scan_race.das`, a layer at 2048 tokens, thirty dispatches a command
    buffer). mlx-lm's scan kernel raced beside ours on its own layout (external; an arm the harness
    does not keep): 1.05 ms on half rows, 1.81 on f32. Ours as it stood:
    2.25 on f32 rows with the gates in the walk, 1.77 on half rows with the gates prepared - the
    same algorithm, 1.7x slower. The whole difference was addressing: `cv[cb + koff + i]` under a
    uint index against a pointer and a constant offset
    (`ARCHITECTURE_GPU_PREFILL_WINDOW.md#kernel-load-addressing`). With the pointer form: 1.48 on
    f32 rows, 0.97 on half rows - under mlx-lm's. Two simdgroups a threadgroup read 10% under
    four. Preparing the gates alone bought 9% (70 -> 64 ms a window); half rows without the
    pointer form bought nothing net. The window 402 -> 368 ms; the scan 70 -> 29.
  - **The K-quant expert stage on pointer loads**: K4 95.5 -> 88.4 ms, K5 46.7 -> 40.8; the window
    368 -> 356.
  - **The standing** (`--depth 10240 --text skills/design_philosophy.md`, the windows in turn):
    362 / 380 / 400 / 418 / 445 ms; mlx-lm 0.32.0 on its synthetic ids 365 / 377 / 396 / 412 / 431
    (external, direction-grade).
- **MEASURED (2026-10-01) - what a routed tile costs, for the tile ladder.** One process each,
  hand MSL over a 2048 x 2048 x 2048 half GEMM, device operands (scratch races, not kept):
  - **A tile's live rows do not make it cheaper.** The A tensor's row extent set under the
    descriptor's m: a 128-row tile reads flat from 128 live rows to 1 (0.280 -> 0.274 ms); a
    32-row tile reads 0.330 at 32, 0.636 at 24, 0.414 at 16, 0.287 at 8 - a partial extent takes
    an edge path that is slower, not faster. Padding is not avoided by telling the op the count.
  - **A tile's cost is mostly fixed.** The whole GEMM by tile height: 8 rows 0.825 ms, 16 0.461,
    32 0.332, 64 0.276, 128 0.281 - a tile of 16 costs 0.69 of a tile of 32, 8 costs 0.62, 64
    costs 1.66, 128 costs 3.38. Fewer, taller tiles win; a remainder of 33..64 rows is one 64-row
    tile at 1.66 against two 32-row tiles at 2.0, each also staging the plane again.
  - **An idle threadgroup costs 0.68 ns** (the 128-row expert stamp over a grid no expert fills:
    32768 threadgroups 0.022 ms, 131072 0.086). A window's idle launches total about 6 ms, most
    of it the K5 stamps' 32-strip grid.
  - **The synthetic probe ids route a MoE onto half its experts** (last routed layer: 127 of 256
    idle, the busiest 1729 rows, 78% of the rows in 128-row tiles); a real prompt leaves 51 idle,
    59% in 128-row tiles, and 6656 rows in 335 tiles of 32 padded to 10720. The 64-row rung read
    level on the synthetic ids for that reason; it is owed a reading on text (`--text`).
  - From the two expert stamps' times and those counts: the multiply runs 22.7 T multiply-adds a
    second staged (30.7 device-direct at 128 rows), staging is 7% of a 128-row stamp and 23% of
    the remainder stamp, and the remainder stamp's pad rows are 29% of it.

- **MEASURED (2026-10-01) - a Metal prefill window's cost against the context behind it: the
  attention is the whole growth, and the device pair
  (`ARCHITECTURE_GPU_PREFILL_WINDOW.md#prefill-attn-device`) takes 2.8x off it.** M5 Max,
  Qwen3.6-35B-A3B-MTP UD-Q4_K_M (10 attention layers of 40, 16 heads of 256 over 2 KV heads), the
  box's own mint, `bin/daslang -jit modules/dasLLAMA/harness/p0_ko_probe.das -- -m <gguf> -n 2048
  --depth 10240`: one session's 2048-token windows in turn, the staged trio and the device pair
  alternating in one process. Staged, by the rows of context behind the window (0 / 2048 / 4096 /
  6144 / 8192): 452 / 493 / 563 / 599 / 694 ms. The device pair: 427 / 440 / 466 / 483 / 521 -
  the five windows 2800 -> 2318 ms, the deepest x1.33. mlx-lm 0.32.0 on the same box
  (external, direction-grade: `mlx-community/Qwen3.6-35B-A3B-4bit`, its model called a window at a
  time over one cache by a script outside the tree): 365 / 377 / 396 / 412 / 431. The kernels alone, one layer at 2048 queries on 8192 rows of context
  (`harness/attn_depth_race.das`, GPU time, best of 3): staged 23.1 ms - QK 9.5, row stats 1.6, AV
  12.0 - against 8.2 - the K/V twin pass 0.1, QK 3.0, the softmax write 2.3, AV 2.7; both within
  7e-7 to 9e-4 of the attention computed in double from the f32 operands, the pair the closer of
  the two at every score spread tried. The time does not move with the scores. The tune race
  (`race_attn_dev`, 512 queries on 1536 rows, 8 heads of 128): 0.344 -> 0.191 ms.
  - **The staged tensor AV twin gains nothing at depth** (`attn_avmm` force-crowned through a
    manifest copy: 446 / 490 / 558 / 590 / 682): threadgroup staging is the cost, not the multiply.
  - **Idea, not built: the softmax write reads the row twice.** A thread holds 80 scores of a
    10240-key row; kept in registers between the max and the exp they save one of three slab
    traversals.
  - **Idea, not built: a continuation window gathers every prior K/V row from the session's host
    cache into f32 panels** (`kv_load_row`, 335 MB at 8192 rows of context). The device pair reads
    halves, so an f16 session's rows could land as halves directly.
- **MEASURED (2026-10-01) - the knockout probe's shares are not additive on a routed model.**
  Knocking a family out upstream of a router feeds the router stale rows, the picks collapse onto
  few experts, and the expert GEMMs' padded tiles shrink: `act_cvt` read 28-45 ms of a 150 ms
  window on the 35B, and removing two of its three convert passes a layer outright moved the window
  151 -> 149 ms (512 tokens) and 446 -> 443 (2048), the logits bit for bit. `--census` counts the
  dispatches instead (`metal_cvt_half` 120 -> 40 a window). On a routed model a family's cost is
  read by replacing it with a form that keeps the rows valid, as `--depth` does for the attention.
- **MEASURED (2026-10-01) - the gated delta scan packed eight state columns a simdgroup**
  (`MetalDnScan`): the scan x1.5 from 64 tokens up at the 35B's geometry, level at one row; the
  window 160 -> 150 ms at 512 tokens, 526 -> 475 at 2048. **The routed tall pair from a mean of 32
  rows an expert** (`set_metal_moe_tall_avg`; `p0_ko_probe --tall-avg 32,64,128`, three interleaved
  rounds, logits bit for bit): the 2048 window 474 -> 449 ms; a 64-row rung between the pair's
  stamps read level (+2% at 512) and is not carried.

- **MEASURED (2026-10-01) - the NextN head on the hyper-connection chain
  (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-draft-head`, `#hc-verify-rows`): the round loses on this form while
  the pool's hits sit near 40% on real text, and the real-text token is 45 ms against the synthetic
  row's 34 - the hits, not the head, are the lever, and the routed pick trace with its offline pool
  sim (`DASLLAMA_MOE_TRACE`, `harness/hot_pool_sim.das`) says where it goes.** zen2 (Ryzen 16 threads,
  RTX 5060 Ti 16 GB on a PCIe 4.0 x8 link, 3.9-4.3 GB held by the desktop), Qwen3.8-Flash-Next
  UD-IQ4_XS with its split head (`mtp-Qwen3.8-Flash-Next-shared-Q8_0.gguf`, `DASLLAMA_MTP_HEAD`),
  `bin/Release/daslang.exe -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m <shard 1> --mtp-ab
  --mtp-depth 1 -p 0 -r 1 -t 16 --for-debug-purposes` under `DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1`,
  eight real-text prompts of 128 tokens, one rep a prompt, one process a row. Acceptance on the CPU
  rail (no `DASLLAMA_GPU`; the rail does not move it): 456 of 571 drafts accepted, 79.9% (per prompt
  64.1-95.5%) - the same 456 of 571 under `--mtp-depth 2` and `3`, since the CPU round is one token
  deep and the flag reaches the seated rounds alone - tg-real128 off 7.07 / on 7.08 tok/s (per prompt
  off 7.0-7.1, on 5.8-8.6; 7.15 / 7.15 at depth 3), the CPU round flat. On the device (`DASLLAMA_GPU=1`,
  the head resident, the pool at the plan's 32 slots serving the verify rows through the rows chain):
  tg-real128 off 22.49 -> on 18.77 tok/s, x0.83 at 407 of 619 accepted, 65.8% (per prompt off 20.1-24.8,
  on 16.0-21.7, x0.74-x0.94 at 47.1-77.8%); a second process 22.85 -> 18.99 at the same 65.8%
  (direction-grade across the two); tg-real64 22.57 -> 18.57 and 22.61 -> 18.73 at 62.7%. The device
  accepts 14 points under the CPU (65.8 against 79.9, the two rails' processes, direction-grade): the
  device draft and the verify rows route their near-ties apart, as every arm pair on this model does
  (`ARCHITECTURE_ENGINE_MECHANISMS.md#moe-pick-tape`). The round under
  `DASLLAMA_GPU_PROF=1` (debug-jit): the draft 4 ms, the verify 81 ms of which the device 25 and the
  host's sums of the two rows' misses 56, the pool serving 48-53% of the rows' picks; a plain real-text
  token 45 ms = device 22 + the host's sums 22.7 at hits 31-45% a layer (the per-layer line of the
  profile: hits, picks and swaps a routed layer) - the entry below's synthetic ids read 70-83% hits and
  29.7 tok/s, a third over the 22.5 of real text (two runs, direction-grade). So a round costs 85 ms for
  1.66 tokens (51 ms a token) against a plain token's 45. The trace (`DASLLAMA_MOE_TRACE=<file>` over the
  same command: every host expert step's layer, rows, k ids and weights, a mark a prompt; 24.7 MB for
  the eight prompts' two arms, 1084090 decode picks and 1927680 window rows) replayed through
  `bin/Release/daslang.exe -jit modules/dasLLAMA/harness/hot_pool_sim.das -- <trace> --policy heat
  --per-layer` reproduces the live run: 37.5% of the decode picks on the device against the profile's
  31-45%, 2.38 placements a token, a token modeled at 45.7 ms (the device 22.0 + the misses 21.7 + the
  placements 2.0, from the profile's costs: a miss 72 us, a placement 570 us of host gather + 270 us of
  upload) against the 45 measured, and `--per-layer` matches the profile's line layer for layer - the
  instrument is calibrated; the policies it ranks are `followup_vulkan.md` row 127's table, which owns
  the lever. The round rides the same lift - its verify rows at 85% hits cost a token's misses, not
  two - so `mtp_auto_arms` leaves it off where the resident driver sums the experts on the host
  (`gpu_resident_experts_host`), and a slot's `mtp=1` arms it by hand. The new device planes the rows
  form adds, at this model's shape (dim 2560, hc 4, the verify's nb 2): the wide rows, the carry's
  source, the rows' carry source, the normed streams and the gated rows 80 KB each, the head's cat
  planes 160 KB each (two), the embed rows 20 KB, the side panel 80 KB, the rest under 40 KB - 1.4 MB
  a driver; the prompt's head warm at the 512-row window: the head's cat plane 42 MB (its f16 twin 21,
  its Q8 image 10.5 + 1.3), the wide-residual source and the stream-major scratch 21 MB each - 117 MB
  a driver. Parity (the CPU chain is the reference; the split command's own one-row steps are a
  rounding control, not parity): `test_gpu_resident_hc.das`'s draft cell (the device draft against the
  CPU `forward_mtp` on one token, wide carry and row, `LOGIT_BAR_REL` 0.20 and `CARRY_BAR_REL` 0.05 of
  the row's max: argmax 271 on both at 9.57 / 9.50, the logits row 0.22 within 1.91, the wide carry
  0.06 within 1.10, the controls 3.00 and 3.50, the head's one select replayed) and its verify cell
  (one round: the draft 271, verify row 0's argmax 12 rejecting it, one rollback; the two rows against
  the CPU's one-row steps at `WIDE_BAR_REL` 0.35 and `WIDE_CARRY_BAR_REL` 0.25, the other row as each
  row's control - rows 1.52 and 3.10 within 3.47 and 4.24, the carry rows 0.49 and 0.49 within 1.74 and
  1.02, the controls 12.53 and 12.06, 4.86 and 4.81 - and the rounding control against the split
  command's own one-row steps on the same picks at `HC_SPLIT_BAR_REL` 0.12 and `SPLIT_CARRY_BAR_REL` 0.10 -
  rows 0.42 and 0.82 within 1.14 and 1.52, the carry rows 0.07 and 0.30 within 0.73 and 0.39, the
  controls 11.62 and 11.74, 5.10 and 5.12; 97 selects replayed, 2 of the CPU draft's own picks off the
  device's); kernel
  cell `test_vkd_ple_side`'s commit arm (the position off the token record, three rows committed to the ring).

- **LANDED (2026-09-29) - the hot expert pool on the hyper-connection carrier
  (`ARCHITECTURE_GPU_VULKAN_POOL.md#hc-hot-pool`), the `iq3s4` device format under it, and the resident
  plan sizing the pool with the mirror.** zen2 (Ryzen 16 threads, RTX 5060 Ti 16 GB, 3.9-4.3 GB held
  by the desktop, NVIDIA driver 616.56), Qwen3.8-Flash-Next UD-IQ4_XS, `bin/Release/daslang.exe -jit
  benchmarks/lcpp_bench.das -- --ngl 0 -p 512 -n 128 -r 6 -t 16 --for-debug-purposes` under
  `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1` (`DAS_TUNE_POLICY` unset - the box's
  sidecar; `DASLLAMA_COOPMAT` at its default, cm2 on this card; `DASLLAMA_IQ3S_SERVE=auto`, the `iq3s4`
  form), one process a row, the rows of one day on one branch - direction-grade across the rows:

  | pool | the window's hits | mirror | pp512 | tg128 |
  | :--- | :--- | :--- | ---: | ---: |
  | the plan's own: 32 slots (4025 MB) | on the device | 114518 positions (2684 MB) | 286.01 +/- 8.85 | 29.74 +/- 0.50 |
  | 32 slots (`DASLLAMA_GPU_HEAT=32`) | on the device | 32768 (`DASLLAMA_GPU_CTX_MAX`, 768 MB) | 288.81 +/- 8.72 | 29.93 +/- 0.49 |
  | the plan's own: 32 slots | the host's | 64413 (1509 MB) | 121.49 +/- 4.08 | 29.70 +/- 0.44 |
  | 32 slots | the host's | 32768 (768 MB) | 128.39 +/- 0.72 | 30.04 +/- 0.41 |
  | none (`DASLLAMA_GPU_HEAT=0`) | - | 32768 (768 MB) | 128.76 +/- 0.36 | 19.38 +/- 0.06 |
  | none, the plan before this entry | - | 203419 (4767 MB) | 110.33 +/- 2.08 | 2.93 +/- 0.03 |

  The reference rows (external): llama.cpp b10659 (`6c84c7d5d`, the qwen4exp commit), the Vulkan and
  the CUDA 13.4 builds' `llama-bench -m <shard 1> -ngl 99 -ncmoe 48 -t 16 -p 512 -n 128 -r 3` with the
  48 layers' experts on the CPU - Vulkan pp512 91.66 / tg128 13.07, CUDA 147.3 +/- 21 / 17.32
  (`ARCHITECTURE_MEASUREMENT.md#host-experts-reference`). Decode is 1.55x the host-experts form (30.04
  against 19.38) and 1.71x the CUDA row (29.74 against 17.32) - on the bench's synthetic ids, whose picks
  the pool holds at 70-83%; real text reads 22.5 tok/s at 31-45% hits (the NextN entry above); the prompt's row is 2.24x the host-experts
  form (288.81 against 128.76) and 1.96x the CUDA row (288.81 against 147.3) once the window's hits run
  through the batch arm's chain beside the host (the middle rows: the pool serving decode alone leaves
  the prompt's row where it was). Over four windows and 32 steps the pool placed 1656 experts - the 1536 of its fill and
  120 swaps - for 0.9 s of gather and 0.45 s of upload, once a load. The bottom row is the box paging: the plan filled the card to 681 MB under its
  room beside a 4.3 GB desktop, the driver's own warning fired (a decode token under 40 GB/s over the
  resident weights), and the row this ledger's entry below reads at 110.13 / 19.66 was the same plan
  on a quieter desktop - its prompt row already paid some of it. The token under
  `DASLLAMA_GPU_PROF=1`, warm, 32 slots: 32.3 ms, of it the host's sums of the misses 8.6 ms (29.5 with
  every pick on the host), the segments and the hit chains 22 ms, the last segment's wait 1.5 ms; the
  device serves 70-83% of the picks on the bench's synthetic ids, 74.6% in the parity cell's steps.
  Research before the kernels (`bin/Release/daslang.exe -jit harness/vk_gemv_probe.das -- 2560 672`,
  the expert gate shape, this card): `iq3s4` decodes at 418 GB/s streaming and 7.2 us a cold plane of
  0.9 MB (iq4xs 294-352 GB/s, 9.2 us; iq3s 400 GB/s, 8.3 us; k4 402 GB/s, 8.1 us; q8 418 GB/s, 11.1
  us), the host's chain 61 us an expert (the `DASLLAMA_GPU_PROF=1` token report: the host's routed sums
  over the picks it summed) - the device is six to ten times the host on a hit. Placing an expert in a
  slot (the same report's pool line, the gather and upload clocks summed over the placements): the
  gather of its three planes 0.57 ms and the upload of 2.6 MiB 0.27 ms (the gather parallel over rows;
  it read 1.85 ms while one expert's rows ran on one lane), so a pool of 32 x 48 slots fills in 1.3 s,
  once a model load, at the first window's host steps; a full pool swapped 6-20 experts over 32 tokens.
  The pool's bytes are `slots x routed layers x (the three expert planes' slot bytes)`: 32 x 48 x 2.6
  MiB = 4025 MB, and 64 slots would be 8050 MB, which the plan yields only beside the whole mirror. A
  decode that starts with no prompt fills two slots a layer a step and reads 14-21 tok/s over its first
  128 tokens (a tg128 row run with no prompt row before it in the process). Parity: `test_gpu_resident_hc.das` run directly through dastest (`bin/Release/daslang.exe -jit
  dastest/dastest.das -- --test modules/dasLLAMA/tests/test_gpu_resident_hc.das` under `DASLLAMA_GPU=1
  DASLLAMA_PARITY_FULL=1`) with the pool armed, a 48-token prompt through the window chain (its hits on
  the device) and six fed steps - the prompt's row 0.46 against 2.00, the steps 0.37-0.68 against bars
  1.70-2.56, the one-step-off control 5.15-11.98, argmax equal on all seven rows, 1536 experts
  uploaded, 18793 of 23040 window slot rows and 2359 of 2880 decode picks on the device, 36 of the CPU's
  own picks off the tape; the bench's frozen pinned-greedy fixture (`lcpp_bench`'s fixture gate)
  reproduces its opening tokens with the pool serving. Kernel cells:
  `test_vkd_hot_combine`, the `iq3s4` stamps' fourteen cells, `test_resident_hot_slots` and
  `test_vk_coopmat_default_and_tile_pick`'s per-32 column arm (model-free).

- **LANDED (2026-09-29) - the Vulkan whole-model driver on the hyper-connection carrier
  (Qwen3.8-Flash-Next UD-IQ4_XS, 94 GB, 512 experts top-10): every plane but the routed expert stacks
  resident, the token command and the prompt's windows recorded as 49 segments cut after each layer's
  router, the host summing the routed experts between them
  (`ARCHITECTURE_GPU_VULKAN_HC.md#hc-token-command`, `#hc-window-chain`).** zen2 (Ryzen 16 threads, RTX
  5060 Ti 16 GB on a PCIe 4.0 x8 link, 4 GB held by the desktop), `bin/Release/daslang.exe -jit
  benchmarks/lcpp_bench.das -- --ngl 0 -p 512 -n 128 -t 16 -r 3 --for-debug-purposes` under
  `DASLLAMA_GPU=1 DASLLAMA_IQ3S_SERVE=lut DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1`: tg128 19.66
  +/- 0.19 / 18.07 +/- 0.25 / 18.93 +/- 0.11 tok/s over three processes (direction-grade; the CPU chain
  6.89; the reference rows, external - llama.cpp b10659 `llama-bench -ngl 99 -ncmoe 48 -t 16 -p 512 -n
  128 -r 3`, the experts on the CPU: Vulkan 13.07, CUDA 17.32), pp512 110.13 +/- 0.37 through the
  window chain (63.18 +/- 0.80 with the prompt read on the CPU, the form before it; llama.cpp Vulkan
  91.66, CUDA 147.3 +/- 21, external - both stream the 55.4 GiB of experts across the link per
  512-token batch, `nvidia-smi dmon -s t` reading 10.5-12.0 GB/s under CUDA and 4.8-7.2 under Vulkan,
  external). The token under `DASLLAMA_GPU_PROF=1`: 52.6 ms, of it the host's
  routed sums 29.5 ms, the 49 segments' submits and waits the rest, the last segment's wait 1.6 ms.
  Resident bytes: 4.98 GiB (deltanet 2.09, hc mixers 0.64 as 97 q8 down/up pairs of [10240 x 320], the
  n-gram key [3840 x 10240] and value [3840 x 2560] planes 49 MB, token_embd 0.63, attention 0.59, head
  0.49, router 0.23, shexp 0.23); the conv ring 10 rows x 10240 floats a region (400 KB), the token's
  segment landings 10 KB + 2 KB + 10 KB host-visible, 49 command buffers a region; the window's panels
  at 512 rows: five [512 x 10240] float planes of 21 MB each (the wide rows, the normed streams, the up
  projection, the gated rows, the side panel), the key rows 21 MB, the streams' Q8_0 image 5.9 MB, the
  side rows 7.9 MB + 2.2 MB, the landings 5.2 MB + 1.1 MB + 5.2 MB host-visible, 49 command buffers;
  the 55.4 GiB of routed experts and the 26.8 GiB n-gram table stay on the host. Parity (the cell's
  form before the hot pool; the entry above carries its 48-token form): `tests/run.das` cell
  `test_gpu_resident_hc.das`, a 24-token prompt through the window chain and six fed steps, the
  CPU arm routing as the device arm did through the decode pick tape
  (`ARCHITECTURE_ENGINE_MECHANISMS.md#moe-pick-tape`) - every row within 0.20 x max|logit| (the prompt's row
  0.88 against 2.55, the steps 0.62-0.86 against bars 2.00-2.48, argmax equal on all seven), the
  one-step-off control 5.81-10.64, 1440 selects replayed, the CPU's own decode top-k off the tape 62
  of 2880 picks; without the tape the arms read 1.8-4.1 with two argmax flips, and the CPU chain on
  `DAS_TUNE_POLICY=reference` reads 3.4-7.0 against the tuned chain with three flips of seven rows - a
  probe's reading, the model's own near-tie sensitivity. The bench's frozen pinned-greedy fixture
  reproduces 8/8 opening tokens with decode on the device. Kernel cells: `test_vulkan_kernels.das`
  `test_vkd_hc_seams` (four cells), `test_vkd_hc_rows` (the rows forms, four cells) and
  `test_vkd_ple_side` (two positions and the window's panel form), the deltanet sigmoid stamps under
  their silu controls.

- **LANDED (2026-09-28) - the Metal batch rail's hyper-connection arm (Qwen3.8-Flash-Next tg128@4):
  the batched step opens the rows' wide residual, runs the mixers through the shared layer body,
  gathers each row's n-gram heads on its own session and writes each row's conv ring in place.**
  M5 Max, `lcpp_bench --ngl 99 --npl 4 --npl-plen 512 -n 128 -r 5 --for-debug-purposes` under the
  box's scratch manifest: tg128 56.86 +/- 0.05 tok/s, tg128@4 119.58 +/- 2.51 summed (2.10x; reps
  124.0 / 118.5 / 117.9 / 118.9 / 118.6 - the first rep of every run reads 3-5% high, a 3-rep read
  lands at cv 3-9%, five reps hold cv 2%). The step is device-bound: `--prof` reads the batched step
  at setup 0.08 / encode 1.5 / wait 31.7 ms (gpu 30.7) against the single step's 1.0 / 1.4 / 19.8
  (gpu 18.9), so four rows cost 1.63x one row's GPU time and the host is 5% of the wall. The MoE
  siblings on this box read the same shape (Qwen3-30B-A3B 165 -> 314, 1.9x; Qwen3.6-35B-A3B 128 ->
  269, 2.1x): four rows' top-10 of 512 experts barely overlap, so the expert traffic scales with the
  rows and only the dense and attention passes amortize. Prediction on record before the run:
  140-180 summed (2.5-3.2x) - missed on the expert-traffic term. Parity: the batch probe holds
  B=2 and B=4 to 5e-4 logits against the single step over 16 steps (0 argmax flips), and
  `mtp-dff-3.8fn` pins it in the suite.

- **OPEN (2026-09-27) - the Vulkan resident driver's NextN prompt warm: the head's slab warmed on
  the window chain, and what it costs a prompt.** Where the driver homes a NextN head and the
  round is on (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-head-prompt-warm`) every window of a
  resident prefill encodes one more layer's worth of rows after the trunk: the last layer's FFN
  over the whole window instead of its last thirty-two rows, eh_proj [2dim -> dim] and the head's
  k and v projections [dim -> kvd] over the window's rows, two norms, a feed requant, the head's
  q/k norm and the rope store - on Qwen3.5-0.8B-MTP the eh_proj plane's 2,228,224 bytes and the k
  and v planes' 557,056 each read once a window, the last layer's three FFN planes (3,899,392 each)
  over 512 rows in place of 32. The chain's planes grow by the head's staging - the embed rows, the
  shifted post-norm rows and the [enorm ; hnorm] image with its feed: `PF_WINDOW x (4 dim + 8 dim
  + 2 dim + dim / 4) bytes` under a Q8_0 feed, `PF_WINDOW x (16 dim) bytes` under the f16 feed, plus
  `PF_WINDOW x cos_elems x 4` for the rope rows - 512 x 14,592 = 7,471,104 bytes at dim 1024 on the
  Q8_0 feed, 512 x 16,384 = 8,388,608 on the f16 feed, and 131,072 of rope rows at a 64-wide plane;
  the head rows' readback moves `(npos - 1) x kvd x 2 sides x 2` bytes a prompt (1 MB a 512-row
  prompt on the 0.8B's f16 rows). Owed from the pod: pp512 of `Qwen3.5-0.8B-MTP-Q8_0.gguf` under
  `DASLLAMA_GPU=1` with `set_mtp_spec` on against off (`lcpp_bench --mtp-ab -p 512` with the
  `DASLLAMA_GPU_PROF=1` window profile: the `vk_rdpf` submit wall per window with the warm against
  without), and the round's acceptance on the SpecBench chat corpus against the CPU round's on the
  same box - the reading the warm exists for.

- **OPEN (2026-09-27) - the Vulkan resident driver's same-slab speculative verify: its row
  buffers and its round against the CPU verify.** Where the driver homes a NextN head
  (`ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command`) the per-row planes hold
  max(regions, depth + 1) rows, eight at most, so a one-region load gains `depth` rows.
  A row is 2 x vocab x 4 bytes of logits (the device plane and its host landing) beside the
  activation rows: 4 dim + 2 hid + 2 qd + 2 kvd (+ 2 qd under a gated q) floats, the quant
  rows' wide + wide / 8 + qd + qd / 8 bytes, the attention partials' n_heads x (splits x (hs + 2)
  + 1) floats, the host x, carry, rope and TokMeta rows, the picks' 520 bytes, on a hybrid the deltanet projection (cd + di floats)
  and o rows (the step's fixed 8,192 + 1,024 + 32,768 bytes), and under the head its three cat
  rows (6 dim floats), its TokMeta row and two rope rows. On Qwen3.5-0.8B-MTP (vocab 248,320,
  dim 1024, hid 3584, qd 2048, kvd 512, gated q, cd 6144, di 2048, 8 heads of 256, a 64-wide rope
  row) that is 1,986,560 bytes of logits and 197,384 of the rest, 2,183,944 a row, plus the
  partials' 8 x (splits x 1,032) + 32 (132,128 at 16 splits): one region gains 2,316,072 bytes at
  depth 1 (nb 1 -> 2) and 16,212,504 at depth 7 (nb 8), both at 16 splits. The plan's scratch
  term counts the same rows. Owed from the pod: `lcpp_bench --mtp-ab` on the 0.8B-MTP under
  `DASLLAMA_GPU=1` with `--prof` and `JOBQUE_PROFILING=1` - the `mtp.verify` section against a
  one-row step's wall and against the CPU verify's on the same box; the reject's `mtp.rollback`
  section against the `mtp.snapshot` + `mtp.replay` the CPU restore paid (the recurrent state's
  round trip, about 20 MB down and up on the 0.8B, retired: the verify command copies each
  recurrent layer's slot state and ring pair after every row but its last into a rollback scratch
  of `depth x n_rec x (nvh x ds x ds + 2 x cd x (dconv - 1)) x 4` bytes, and the reject copies one
  row back - on Qwen3.5-0.8B-MTP (18 recurrent layers, 16 heads of 128, cd 6144, dconv 4)
  21,528,576 bytes at depth 1 and 150,700,032 at depth 7, counted in the plan's scratch term,
  `ARCHITECTURE_GPU_VULKAN_MTP.md#resident-verify-rollback`); the verify command's own
  `DASLLAMA_GPU_PROF=1` ledger (`vk_rdec gpu avg/verify` with its `dn avg/verify` line and
  `vk_rdec host wall/verify`, `ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-verify-command`) against the
  one-row `vk_rdec gpu avg/token` line of the same run's off arm, role by role - under the greedy
  `--mtp-ab` arms' pick ask the verify lands the picks and two hidden rows alone (the
  2 x vocab x 4-byte logits plane, its host copy, the four row copies and both host argmaxes gone
  from the round), the re-warm runs no q GEMV and the store pass over the k heads alone
  (`warm_q` gone from the ledger, `warm_qknrope` smaller), and no head row is read back (the `kv
  readback` piece gone from `host wall/verify`). The on/off rate with the drafts' acceptance, as
  measured: `benchmarks/lcpp_bench.das --mtp-ab -n 64 -r 2` (`-r 4` on the 0.8B) as the `-jit`
  script (debug-jit) under `DASLLAMA_GPU=1 DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=16`,
  `DAS_TUNE_POLICY` unset (the box profile), the resident driver at cm2 coopmat mode, on the
  RTX PRO 4500 pod, tok/s off / on, the off arm the plain tg64: Qwen3.5-0.8B-MTP-Q8_0 444.7
  +/- 9.5 / 574.9 +/- 14.3 (x1.29, acceptance 81.9%); Qwen3.5-4B-MTP-Q8_0 138.0 / 202.5 (x1.47,
  82.6%); Qwen3.5-9B-MTP UD-Q5_K_XL 103.8 / 149.3 (x1.44, 82.7%); Qwen3.8-27B-Q4_K_M with its
  Q8_0 head, the image rail off, 37.1 / 54.3 (x1.46, 69.9%) - direction-grade, the on and off
  arms in one process per row. The stamped ledgers at 66b143122 on the 0.8B under the same
  command with `DASLLAMA_GPU_PROF=1`, microseconds: the verify's host wall 2917 (GPU 2870), the
  draft's 491 (GPU 449), against a one-row step's 2571 (GPU 2357). Measured on the RTX PRO 4500
  pod - `benchmarks/lcpp_bench.das --model Qwen3.5-0.8B-MTP-Q8_0.gguf --mtp-ab -n 96 -r 1
  --for-debug-purposes` as the `-jit` script (debug-jit) under `DASLLAMA_GPU=1
  DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=16 DAS_LOG_LEVEL=info DASLLAMA_GPU_PROF=1`,
  `DAS_TUNE_POLICY` unset (the box profile), the resident driver at cm2 coopmat mode,
  microseconds a round from the `DASLLAMA_GPU_PROF=1` ledger: the two-row verify's `dn_seq`
  reads 421 against the one-row `step` 243 and the rollback copies (`roll`) 74 - a recurrent
  layer's row costs its dispatch latency (16 workgroups, one a head), not the state's reload. A
  form that looped the rows inside one dispatch a layer with the scratch stored in-kernel and the
  final ring pair written by a last-arriving workgroup was bit for bit the chained steps and read
  `dn_seq` 721 under the same command and environment at its own commit (direction-grade, two
  commits): the loop serializes the rows a workgroup already ran back to back, and the epilogue's
  barrier and staged copy add to every layer - the per-row dispatch stays. What moves `dn_seq` is
  more workgroups a head (the ds x ds state split across workgroups), a step-kernel change the
  one-row decode shares.

- **LANDED (2026-09-28) - the head K-row bar of `tests/test_gpu_resident_hybrid.das`
  (`HEAD_K_BAR_REL = 0.08`, twice the deltanet bar): the prompt warm's K rows against the CPU
  warm's.** The cell `test_gpu_resident_hybrid_mtp_head_warm`, through `tests/run.das` under
  `-jit`, `DASLLAMA_GPU=1 DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=16`, `DAS_TUNE_POLICY` unset
  (the box profile), the resident driver at cm2 coopmat mode, on the RTX PRO 4500 pod, on
  Qwen3.5-0.8B-MTP-Q8_0 over the 40-token prompt: the widest miss reads 0.05 of the CPU row's max
  under both codecs - f16 mirrors 0.357 at row 38 on a 0.577 bar, f32 mirrors
  (`DASLLAMA_VK_KV32=1`) 0.278 at row 2 - and the one-down control lands 7.3 to 7.6 bars away.
  The bar is twice the deltanet bar because the row is the f16-stored projection of a hidden row
  already held within the deltanet bar; the control is tightened to two bars from it.
- **MEASURED (2026-10-29) - the head K-row bar moved to 0.10 (`HEAD_K_BAR_REL`, now in
  `tests/_mtp_resident_cells.das`, the one body both discrete homes' MTP files run).** On the CUDA
  home's q8_0-mirror two-window warm (`test_gpu_resident_cuda_mtp.das`'s block-codec cell, through
  `tests/run.das` under `-jit`, `DASLLAMA_GPU=1 DASLLAMA_CUDA=1 DASLLAMA_ALLOW_UNTUNED=1
  DAS_JOBQUE_THREADS=16`, `DAS_TUNE_POLICY` unset) the RTX 5060 Ti read 0.0803 of the CPU row's max at
  head K row 523 of the 600-token warm, where the RTX PRO 4500 pod held 0.08; the f16 mirrors on both
  cards sit under 0.06. The row is the q8_0-stored projection, a quantizer's step wider than the f16
  one's; the one-down control stays two bars away at the new bar.

- **OPEN (2026-09-27) - the Vulkan resident driver's NextN draft head: its plane and slot bytes,
  and the device draft against the CPU draft.** Where the driver takes a model's head
  (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head`) the plan grows by the head's eight
  planes and one K/V slot a region; a headless model plans what it did. The planes are
  (2 dim x dim + dim x qrows + 2 dim x kvd + qd x dim + 3 dim x hid) weights at their formats' block
  bytes - Qwen3.5-0.8B-MTP-Q8_0 (dim 1024, the gated q's 4096 rows, kvd 512, qd 2048, hid 3584,
  every head plane Q8_0 at 34 bytes a 32-weight block): eh_proj 2,228,224 bytes, q 4,456,448, k and
  v 557,056 each, o 2,228,224, gate, up and down 3,899,392 each, 21,725,184 in all. The slot is
  regions x seq_cap x kvd x 2 sides x 2 bytes (f16), regions x seq_cap x 2,048 bytes on the 0.8B:
  8,388,608 at one region of 4096 positions, 67,108,864 at four regions of 8192. The norms plane
  gains (5 dim + 2 head_size) x 4 bytes (22,528 on the 0.8B) and the draft's staging 3 x 2 dim x 4
  bytes a row of the per-row planes (24,576 a row at dim 1024, 122,880 at dim 5120; the next entry
  prices the rows). Owed from the pod: the 0.8B-MTP's `resident image`
  and `NextN draft head in the arena` load lines (the bytes read, not computed); the draft's wall
  against the CPU `forward_mtp` - `lcpp_bench --mtp-ab` under `DASLLAMA_GPU=1` with `--prof` and
  `JOBQUE_PROFILING=1`, the `mtp.draft` section against the CPU draft's on the same box - including
  the hydrate's upload where one runs, a submit of its own beside the draft's (the drafted row
  stays on the device, so no readback follows the command, and the greedy round takes the pick the
  seat answers, the vocab x 4-byte logits row neither landing nor re-read by a host argmax); the
  draft command's own `DASLLAMA_GPU_PROF=1` ledger (`vk_rdec gpu avg/draft` and `vk_rdec host
  wall/draft`, `ARCHITECTURE_GPU_VULKAN_MTP.md#resident-draft-head`), the classifier's share of it
  against the one-row `cls` role; and the acceptance the
  device draft reads off the head's slab the window chain warmed (the entry above) - the pod
  measures.

- **OPEN (2026-09-27) - the Vulkan resident driver's speculative carry: its landing plane and its
  step cost on a NextN-headed model.** The landing plane (`RDec.hid_host`, host-visible, allocated
  only where the driver is prepared with `carry`) is dim x 4 x nb bytes, nb the per-row planes'
  rows (a row a region, or the speculative verify's rows where more, eight at most):
  Qwen3.5-0.8B-MTP (dim 1024) 4,096 bytes a row, 32,768 at eight. Each step moves
  dim x 4 bytes a row over the logits' path (4 KB a token on the 0.8B), and the last layer's
  residual step leaves the q8 down GEMV's epilogue for the row-storing stamp, one dispatch more a
  token - while the speculative knob is on; with it off the step pays none of this and keeps the
  fused forms (`ARCHITECTURE_GPU_VULKAN_MTP.md#resident-spec-knob-gate`). Owed from the pod:
  tg128 of `Qwen3.5-0.8B-MTP-Q8_0.gguf` with `set_mtp_spec` on against `Qwen3.5-0.8B-Q8_0.gguf`
  (the same trunk, no carry) under `DASLLAMA_GPU=1`, with the `DASLLAMA_GPU_PROF=1` token profile
  of each, so the step's delta is read rather than estimated.

- **LANDED (2026-09-27) - the Metal speculative round on the hyper-connection model
  (Qwen3.8-Flash-Next + its split shared head): the verify's rows carry the wide residual into the
  draft head, the n-gram side input takes the panel form with the accepted rows committed after the
  walk, and every row form picks the deltanet z activation through `enc_dn_zgate`.** M5 Max,
  `lcpp_bench --ngl 99 --mtp-ab -r 3 --for-debug-purposes` under the box's scratch manifest, tg-real128
  over eight prompts, greedy, depth 1: off 55.32 +/- 0.49 tok/s, on 59.97 +/- 0.96 (1.08x), 1323 of 1716
  drafts accepted (77.1%; per prompt 55.6% to 98.4%, x0.94 to x1.23 - the round loses at 56%
  accept and wins from ~68%). The counting probe is token-exact at depth 1, 2 and 4 (100% accept,
  spec 2x plain wall) and the prose probe at depth 1 (77%, 0 flips). The same shape as the 9B's
  round (`followup_metal.md` row 26): an accept rate this high buys 1.77 tokens a round and the
  round returns a tenth of it, so the verify's two rows cost most of a plain step here too - the
  stage split (`harness/mtp_ruler.das`) is the next lever. The CPU round on this model is a loss
  (`followup_general.md` row 178).

- **LANDED (2026-09-27) - IQ3_S served as a 16-entry codebook plane on the CPU rail (`iq3s4`,
  `ARCHITECTURE_ENGINE_FORMATS.md#served-form`, `DASLLAMA_IQ3S_SERVE=auto|grid|lut`): every IQ3_S weight
  is a signed odd 1..15 times its block scale, so grid entry, qh bit and sign fold at transcode into
  one nibble over `-15, -13, .., 15`, decoded by the iq4xs LUT kernels - one `tbl` per 16 nibbles
  in place of a 2 KB grid gather, one load per 4 weights.** Qwen3.8-Flash-Next UD-IQ4_XS (gate/up
  experts IQ3_S) on the M5 Max, 18 threads, `lcpp_bench --for-debug-purposes -p 512 -n 128 -r 3`
  under the box's scratch manifest, one binary, warm passes (the pass after a mint is void: cv 7%):
  grid pp512 134.1 +/- 2.0 / tg128 18.98 +/- 0.13, lut 140.6 +/- 0.7 / 26.9 +/- 0.5, llama.cpp at 2b129ccfa
  107.8 +/- 3.5 / 19.9 +/- 0.4. `decode_prof -n 128 -t 18` under `JOBQUE_PROFILING=1`: 59.5 -> 40.3 ms a
  token; `mm_moe` 25.1 -> 6.7 ms, the attention block 19.5 / 20.0, the hc mixes 5.8 / 6.0,
  `mm_moe_dn` 5.0 / 3.5, the head 2.9 / 2.9. Predictions on record before the run: `mm_moe` ~7 ms,
  the token ~42 ms, tg128 ~25 - all three held. Cost: the planar image 98.1 -> 105.5 GB (+23% on
  the IQ3_S planes, the scale rows unchanged); the dequant is bit-identical to the grid form's
  (`test_kquant`). The GPU tiers keep the grid form - their gather is a device read. Provenance:
  the four bench logs and two profile logs of the 2026-09-27 session, `followup_general.md` 170.

- **LANDED (2026-09-27) - the Vulkan TTS dedup pass: the Pocket row GEMV (`TtsPkGemvT`) folds
  its own subgroup reduction onto the shared `WgReduceBase`, and the pass's bench rows against
  master's.** Box: the pod - the RunPod RTX PRO 4500 Blackwell, Linux, the Vulkan backend - one
  process at a time.

  The reduction fold's ground is the bits, not a race: the folded stamp writes the same output
  as the shipped one (max abs diff 0 over both served frame-loop shapes, ln_qkv 1024 -> 3072 and
  ln_gelu 1024 -> 4096), its workgroup memory is 16912 bytes under the device's 49152-byte cap,
  and the served Pocket row below reads the same on the folded tree as on master. The one-off
  probe that timed the two forms did not land, so its figures are not this ledger's.

  The bench rows, `direction-grade`, the same instrument on the same box at two named commits,
  one process a run: `./bin/daslang -jit modules/dasLLAMA/harness/tts_synth.das -- --model
  <file> --voice <voice> --out <dir> --limit 20` with `kitten-nano.gguf` and `kitten-mini.gguf`
  on `Bella`, `kokoro-82m.gguf` on `af_heart`, `pocket-tts-en-q8.gguf` on `alba`, under
  `DASLLAMA_GPU=1 DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=16 DAS_LOG_LEVEL=info`,
  `DAS_TUNE_POLICY` and `DASLLAMA_COOPMAT` unset, no other overrides; the mean generation wall
  (`gen_ms`) a sentence over the 20 sentences, with its min and max. At the dedup tree
  7e06befab against master 45277047d: kitten-nano 45 ms (16-431) against 44 (15-369), kitten-mini
  94 (36-653) against 91 (36-688), kokoro 87 (37-641) against 86 (37-654), Pocket q8 128 (60-634)
  against 133 (63-654) - within noise. The Metal row of the same instrument on the M5 is owed.

  The reflect pad's two `TtsPkRows` copies (the last generator stage's one `TtsReflect1` dispatch
  became two), on the device clock (`pfq_*`, 200 submits after a warm-up, `debug-jit`, the probe
  a one-run harness file that did not land) over t2 rows of c2 channels, both copies a submit:
  c2 8: 4.8 us at t2 2 through 4096, 6.5 at 65536, 12.0 at 262144; c2 24: 4.8 through 4096, 10.6
  at 65536, 24.9 at 262144; c2 64: 4.8 through 256, 6.4 at 4096, 18.6 at 65536, 173.7 at 262144.
  The pair costs a dispatch's floor at every row count a sentence produces, so the path ships no
  gate on the row count.
- **LANDED (2026-09-26) - the StyleTTS2 and Pocket seats ride the Vulkan TTS driver
  (`ARCHITECTURE_GPU_TOWER_VULKAN_TTS.md#vk-tts-chain`): the kitten and kokoro families' seven
  seats and the Pocket codec and frames seats on the Vulkan tower's knob, the front end on the
  f32-exact tile, the decoder and generator convs on the f16 tile, every slab through the shared
  host writer.** Box: the pod - the RunPod RTX PRO 4500 Blackwell, Linux - every das figure
  `-jit` on this tree under `DASLLAMA_GPU=1 DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=16
  DAS_LOG_LEVEL=info`, no other overrides unless named, one process at a time. The served rows:
  the first 20 sentences of the g2p corpus through `./bin/daslang -jit
  modules/dasLLAMA/harness/tts_synth.das -- --model <gguf> --voice <voice> --out <dir> --limit
  20`, the mean generation wall a sentence over the 20 with the first sentence's wall apart (it
  pays the device bring-up and the slab builds; of the spread only the min named below is in the
  record, the rest is the run's log). Pocket q8 (`pocket-tts-en-q8.gguf`, alba): 132 ms a
  sentence, the first 641, a steady sentence about 99 - the prompt 27 ms of it on the CPU chain
  (the prompt seat since serves it on the device, the entry above), the backbone 62, the codec 10. kokoro (`kokoro-82m.gguf`): 98 ms,
  the first 785, min 36, the steady sentences about 62, against the torch CUDA reference row of
  52 ms a sentence (`external`: `harness/tts_ref_bench.py --device cuda --models
  kokoro-82m:af_heart --limit 20` on the pod under
  `ARCHITECTURE_MEASUREMENT_AUDIO.md#tts-ref-g2p-venv`'s recipe; the package versions that
  run printed are not in this branch's record). kitten-nano (`kitten-nano.gguf`): 54 ms against
  its onnxruntime CUDA EP reference row of 132 (`external`: the same script with `--device cuda
  --models kitten-nano`, onnxruntime-gpu with the cu12 pip libraries, providers CUDA then CPU);
  kitten-mini 302 against 4166 (the mini graph runs mostly off the provider). The LSTM recurrence's share,
  from the same run's per-role GPU ledger under `DASLLAMA_GPU_PROF=1`: a `TtsLstmDir` step 14 us
  at kokoro's H = 256, 22 ms of a kokoro sentence (`followup_vulkan.md` 106). The device
  bring-up - the process's first seat call, reported under the info log - 254..257 ms at the 64
  MB upload staging chunk against 322..349 at 256 MB, `direction-grade` (a process a chunk
  size): the pinned staging buffer costs 0.4 ms a MB (101 ms at 256 MB, 26 at 64) and a chunk
  past 64 MB buys no rate - the Pocket frames slab's 322 MB uploads in 27 ms on either, a 1.3 GB
  Llama file's resident upload reads 129 and 156 ms at 64 MB against 196 and 127 at 256 (two runs
  a size, the run-to-run noise: `benchmarks/lcpp_bench.das -p 32 -n 0 -r 1`,
  `Llama-3.2-1B-Instruct-Q8_0`), the Pocket first sentence 708 -> 641 ms with the chunk. The
  attach costs, same box and flags, `direction-grade` (two commits a pair): a seat's scratch is
  sized a quarter past the call's need, so a longer sentence after a shorter one reuses it -
  kokoro's scratch builds over the 20 sentences 9 -> 6, the second sentence's decoder 154 -> 23
  ms; each seat's host readback buffer is sized to the rows it copies out, never the widest slot
  - the Pocket codec's 765 MB -> 0.6 MB for a wave, the kokoro decoder's up to 717 MB before; the
  Pocket frames slab (322 MB) builds in 70 ms against 201 with the q8 rows dequantized over the
  job pool. What the seats hold: the Pocket frames slab 322 MB, the kokoro decoder's scratch past
  3 GB at its widest sentence, the Pocket codec's one-shot column slot about 4 GB at the 512-frame
  cap (`followup_vulkan.md` 108, 109).
- **LANDED (2026-09-26) - three TTS parity bars widened to cover the pod's x64 CPU chain beside
  the M5 Max's.** Every reading `-jit` through `tests/run.das -- --area tts` on the named box: the
  pod (the RunPod RTX PRO 4500, Linux; its x64 CPU chain is the arm under compare) and the M5 Max
  (Metal). `SOURCE_BAR` (`tests/_tts_parity.das`) 5e-3, absolute - the harmonic source against
  the oracle's, where the phase past 1e5 radians turns a ulp of an increment into radians: the M5
  Max reads under 1e-4, the pod's x64 chain 2.5e-3 on kitten's ONNX law. `GPU_CODEC_SERVED_BAR`
  (`tests/test_tts_pocket.das`) 2e-1 - the codec seat on the served planes against each file's
  own CPU chain: the M5 Max 1.3e-2 / 7.7e-3 (q8 / kq), the pod 9.2e-2 / 1.5e-1, all of it the
  CPU's Q8_0 activation blocks of the first layer's rows while the tower sits at 3e-6 of the CPU
  f32 chain on either box. `GPU_FRAME_FREE_BAR` (the same file) 5e-3 - the frames seat
  free-running on the f32 lane against the CPU chain's free run on the same seed: the M5 Max
  1.5e-4 and 1.1e-4 on the two stage cases, the pod 7e-5 and 1.8e-3 (lj028's 150 frames compound
  the f16 feed's rounding); the 1.8e-3 reading predates the fixes this arc landed after it, and
  the bar stands over both readings.
- **LANDED (2026-09-26) - the sampler is a candidate list when top-k is set
  (`ARCHITECTURE_ENGINE.md`, `dasllama_sampling.das`): one pass over the row selects the k
  largest logits into a heap, and the temperature, softmax, nucleus and min-p cuts and the draw run
  over those survivors; top-k off or past `SAMPLE_TOPK_FAST_CAP` (a design bound, not a timed
  pick) keeps the vocab-wide reference.** Before, a sampled decode paid k full-vocab max passes, a
  vocab-wide softmax, a full sort for top-p and a vocab-wide CDF walk per token. `direction-grade`,
  `debug-jit` (two commits, one process a run): `bin/daslang -jit utils/dasllama-server/cli.das --
  complete --model Qwen3.5-4B-Q8_0.gguf -p "<a story prompt>" -n 256 --seed 7`, once greedy and once
  with `--temp 0.7 --top-k 20 --top-p 0.95`, five reps each, the CLI's own stats line, this box
  (M5 Max, Metal, the box's tune profile, no environment overrides): greedy 105 t/s before and
  104-105 after; the chat preset 74-75 t/s before, 101-103 after - the sampler's per-token cost
  from 4.0 ms (the difference of the two rates) to under the run's noise. Prediction on record
  before the change: sampled within 5% of greedy. `lcpp_bench` carries no sampler flag (its
  `--mtp-temp` sets a temperature alone), so no board row holds the sampled rate.
- **LANDED (2026-09-25) - the Pocket TTS frame loop rides the Metal tower as the family's second
  seat (`ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-frames`): the backbone step and the flow head for every
  frame, eight frames a command buffer over a per-voice device K/V slot, the EOS rule on the host
  between batches, the q8 backbone on the decode GEMV, the head's GEMVs carrying their norm and
  activations, the attention row a threadgroup a head with its scores staged.** The bare q8
  dot's route was A/B'd on this box (`harness/pocket_stage_probe.das -m pocket-tts-en-q8.gguf -t
  "The quick brown fox jumps over the lazy dog." --voice alba --reps 5`, best of 5,
  `DASLLAMA_ALLOW_UNTUNED=1` on both arms - the tower's stamps carry no crown - two processes, one a
  route, `debug-jit`): the decode split-K GEMV plus a bias add 1.228 ms a frame (backbone 40.8 ms of
  51.6), the row stamp with the bias fused 1.477 (backbone 53.3); the probe's CPU arm in the same
  process read 1.331 and 1.303, the box's control across the two runs - the row stamp's
  one-lane-a-block dot loses to the split-K walk, so the bare q8 dot's stamp is gone and every bare
  q8 linear (the backbone's) takes the decode GEMV. Box, flags and
  the 20-sentence walls as the codec seat's entry below; `direction-grade` - the CPU arm a second
  process, the codec-seat column and the rig's before-rows the previous commit's readings; both
  seats serve in these rows.

  | file | voice | audio | das tower, mean (min-max) | das tower, codec seat alone | das CPU, mean (min-max) |
  |---|---|---|---|---|---|
  | pocket-tts-en-q8 | alba | 120.8 s | 81 ms (43-186), rtf 0.0133 | 142 ms (81-227), rtf 0.0238 | 200 ms (107-264), 0.0334 |
  | pocket-tts-en-kq | alba | 121.2 s | 72 ms (37-198), 0.0118 | 117 ms (61-189), 0.0195 | 176 ms (92-232), 0.0293 |
  | pocket-tts-en-stuart-kq | stuart_bell | 143.6 s | 78 ms (42-209), 0.0109 | 134 ms (80-210), 0.0187 | 204 ms (111-280), 0.0286 |

  The loop alone - the frames hook's wall a frame, the voice slot and the batches' round trips
  inside it - through `harness/pocket_stage_probe.das -m <gguf> -t "<one sentence>" --voice
  <voice> -r 5` (a 4.6 s sentence free-running, the best of five passes each way in one process):
  on the q8 file 0.91 ms a frame on the tower against 1.29 on the CPU chain, on the kq file 0.69
  against 1.04, on the one-voice stuart-kq file 0.78 against 0.98, on the f16 file's f32 lane
  (`--f32`, every linear on the f32 row GEMV) 1.91 against 5.89. Of the GPU's own time
  the backbone's 57 dispatches take about two thirds and the head's 22 the rest; the first form
  of the loop, a dispatch per row operation (105 a frame) and an attention kernel accumulating
  in a dynamically indexed register array, cost half again as much a frame, most of it the
  attention - the rope-and-store, the residual joins and the head's operations folded into
  their GEMVs, and the attention row rewritten over staged scores, are what brought it down.
  The device footprint on the q8 file (`harness/pocket_stage_probe.das`'s attach log lines): the
  frames slab 36.3 MB of f32 rows and the q8 blob 80.3 MB, the codec slab 41.3 MB, the voice slot
  20.2 MB of K/V over 411 rows (16 layers x 2 x 411 x 768 x 4 bytes) with 0.1 MB of rope tables;
  a chunk's working buffers come from the pool. Parity, teacher-forced on
  the oracle's noise and frames against the CPU chain (`test_pocket_frames_metal`): the f32
  lane's latents, conditioning rows and EOS logits at 2e-6 rel-rms, the cell asserting the
  tower's encodes rose one a batch, and its free run - the loop's own noise, every frame fed its
  own output - at 1.5e-4 and 1.1e-4 on the two stage cases against the CPU chain's free run on
  the same seed (`GPU_FRAME_FREE_BAR`, the M5 Max; the bar's value since the pod's reading, and that reading, are the widened-bars entry above); on the served lanes (`test_pocket_frames_metal_served`, which
  logs both distances) the tower reads nearer the f32 oracle than the CPU chain does - the q8
  file's latents 0.012 against the CPU chain's 0.025, the kq file's 0.10 against 0.11 - since the
  CPU quantizes the activations it feeds a q8 or K-quant plane and the tower feeds them f32. Quality,
  `harness/tts_rig.py` on the 200-sentence corpus against the codec-seat rows: q8 4.23 / 4.364 -> 4.18 / 4.364, kq 4.04 / 4.333 -> 3.91 / 4.325, stuart-kq 3.23 / 4.117 -> 3.36 / 4.136 (WER / UTMOS, a word or two of the 2201 either way, the UTMOS within a hundredth). The
  levers left are `followup_metal.md` sec.27.

- **LANDED (2026-09-25) - the Pocket TTS codec rides the Metal tower (`ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-codec`): a chunk's latents up, its samples back, one command buffer, the CPU's windowed codec
  run as one shot over the chunk on the f32-exact GEMM stamps, the K-quant transformer linears
  dequantized into the slab so the small form serves too.** Box: the M5 Max, every das figure
  `-jit` on this tree with `DAS_TUNE_MANIFEST=performance/m5.tune.json` (its runtime section
  applied, the kernel winners on their fallback bodies - the sidecar predates the binary) and
  `DAS_LOG_LEVEL=info`, no other overrides unless named, one process at a time. The first 20
  sentences of the g2p corpus through `harness/tts_synth.das --model <gguf> --voice <voice> --out
  <dir> --limit 20` (every sentence timed from the first, so the first one pays the slab build
  and the pipeline warm-up), the mean generation wall a sentence with its min and max over the 20
  and the real-time factor of the run; the CPU arm a second process under
  `DASLLAMA_METAL_TOWER=0` (`direction-grade`). The frame loop - the backbone and the flow head -
  stays on the CPU in both arms, so the tower arm moves the codec's share only.

  | file | voice | audio | das tower, mean (min-max) | das CPU, mean (min-max) |
  |---|---|---|---|---|
  | pocket-tts-en-q8 | alba | 119.6 s | 142 ms (81-227), rtf 0.0238 | 200 ms (107-264), 0.0334 |
  | pocket-tts-en-kq | alba | 119.8 s | 117 ms (61-189), 0.0195 | 176 ms (92-232), 0.0293 |
  | pocket-tts-en-stuart-kq | stuart_bell | 142.8 s | 134 ms (80-210), 0.0187 | 204 ms (111-280), 0.0286 |

  The codec alone, one sentence's 60-odd latent frames (4.6 s of audio) through
  `harness/pocket_stage_probe.das -r 5` (the best of five passes each way in one process; a
  tower pass spreads over a fifth of its mean, and the first pays the pipeline library, about
  5 s once a process): the f16 file on the f32 lane (`--f32`) 87 ms CPU -> 23 ms tower at
  rel-rms 9.0e-7 against the CPU chain (`test_pocket_codec_metal` reads 8.7e-7 on the oracle's
  latents, the cell asserting one tower encode a call); the q8 file 70 ms -> 20 ms at 7.1e-3,
  the kq file 64 ms -> 22 ms at 1.7e-2, the stuart-kq file 77 ms -> 24 ms at 2.3e-2
  (`test_pocket_codec_metal_served` reads 1.3e-2 and 7.7e-3 on the oracle's latents
  of the q8 and kq files: the CPU's served lanes quantize their activations against the q8 and
  K-quant planes, the tower reads the planes dequantized at f32, the StyleTTS2 decode seat's
  gap); the f16-staged
  GEMM twins read the same 33 ms at 9e-4, so the seat keeps the exact stamps. The served-lane
  cell's stage clocks (`test_pocket_synthesis_metal` in `tests/test_tts_pocket.das`, a 4.08 s
  sentence, a first synthesis): the codec 61 -> 40 ms with the slab build inside it. Quality, `harness/tts_rig.py` on the 200-sentence corpus (WER
  through parakeet, UTMOS), the tower arm against `DASLLAMA_METAL_TOWER=0`: q8 4.23 / 4.364 both
  arms; kq 4.09 / 4.333 -> 4.04 / 4.333; stuart-kq 3.32 / 4.117 -> 3.23 / 4.117 (a word or two of
  the 2201 on the served lanes' 1e-2 gap, the UTMOS equal to the third digit, the audio lengths
  equal). The frames seat is the entry above.

- **LANDED (2026-09-24) - the whole StyleTTS2 synthesis rides the Metal tower
  (`ARCHITECTURE_GPU_TOWER.md#tower-tts-chain`): seven seats from PL-BERT to the inverse STFT, the front
  end on the f32-exact GEMM stamp and an f32 attention row kernel so the durations round as the
  CPU's, the harmonic source the CPU's operation for operation, the decoder through the
  generator and the inverse STFT as one command buffer.** Box: the M5 Max, every das figure `-jit`
  on this tree with `DAS_TUNE_MANIFEST=performance/m5.tune.json` (its runtime section applied,
  the kernel winners on their fallback bodies - the sidecar predates the binary) and
  `DAS_LOG_LEVEL=info`, no other overrides unless named, one process at a time. The first 20
  sentences of the g2p corpus through `harness/tts_synth.das --model <gguf> --voice <voice> --out
  <dir> --limit 20` (the served q8 lane, every sentence timed from the first, so the first one
  pays the slab build and the pipeline warm-up), the mean generation wall a sentence with its min
  and max over the 20, and the real-time factor of the run; the CPU arm is a second process under
  `DASLLAMA_METAL_TOWER=0` (`direction-grade`). The reference column is `harness/tts_ref_bench.py`
  (`ARCHITECTURE_MEASUREMENT_AUDIO.md#tts-ref-g2p-venv`;
  `~/Work/tts-ab/g2p/.venv-g2p/bin/python harness/tts_ref_bench.py --device mps --threads 8
  --limit 20`, and `--device cpu --models kokoro-82m:af_heart` for the torch CPU row; torch
  2.13.0, kokoro 0.9.4, kittentts 0.8.1, onnxruntime 1.29.0, `HF_HOME=~/Work/tts-ab/g2p/.hf`):
  the same 20 sentences' `ps_espeak` phonemes fed to the packages' own models, generation only,
  each sentence timed three times and the best kept, the mean of the 20; its audio runs shorter
  on the kitten models (the package's per-voice speed prior and its 5000-sample trim), so the
  compare is the wall a sentence, not the real-time factor.

  | model | voice | audio | das tower, mean (min-max) | das CPU q8, mean (min-max) | reference |
  |---|---|---|---|---|---|
  | kokoro-82m | af_heart | 145.7 s | 112 ms (65-310), rtf 0.0154 | 331 ms (192-625), 0.0455 | torch MPS 147 ms (rtf 0.0200); torch CPU 8 threads 564 ms (0.0769) |
  | kitten-nano | Bella | 242.4 s | 37 ms (21-131), 0.0031 | 192 ms (114-283), 0.0158 | onnxruntime CPU 8 threads 207 ms (0.0259, 160 s of audio) |
  | kitten-mini | Bella | 206.6 s | 111 ms (60-412), 0.0107 | 352 ms (193-562), 0.0341 | onnxruntime CPU 8 threads 1463 ms (0.2018, 145 s of audio) |

  The first sentence carries the slab build and the pipeline warm-up: on the tower arm it reads
  310 ms on kokoro against the run's 65 ms floor, 131 against 21 on kitten-nano, 412 against 60
  on kitten-mini. Where a sentence's time goes, from the served-lane cell's stage clocks
  (`test_kitten_synthesis_metal` in `tests/test_tts_kitten.das`, the same tree, flags and box; a
  first synthesis, so the slab build sits in the decoder clock): kitten-nano's 5.8 s sentence in
  64.6 ms on the tower - bert 18.1, text 1.7, durations 3.9, prosody 3.4, decoder 37.2 (the decode
  seat, two thirds of the sentence) - against 101.3 ms on the CPU q8 chain. Parity on the f32
  lane, every seat fed the CPU chain's inputs (the parity rail's stage cells, `gpu_stage_checks` in
  `tests/_tts_parity.das`, the same tree, flags and box): PL-BERT 3e-6, the text encoder 4e-7,
  the duration encoder, raw durations and prosody under 2e-6, the durations token for token on
  every stocked oracle case, the mixed source signal 4e-7; the decode seat 3e-3 to 7e-3 on
  kitten-nano and under 2e-2 on kokoro against the CPU chain on the oracle's noise (the decoder
  and generator keep the f16-staged GEMM; kitten-nano's small decoder amplifies that staging
  twelvefold against kokoro's); the decode seat on the served q8 planes 2e-2 to 6e-2 (the CPU
  chain quantizes its activations, the tower reads the weights dequantized at f32); the seam form
  of the generator alone 1.2e-3 at most; the served lane's sample counts within a hundredth of the
  CPU chain's (139600 against 140800 on the kitten-nano sentence, equal on kokoro's). The cells
  ran through `tests/run.das -- --area tts` (`-jit`, the manifest above, the M5 Max). The remaining
  cost and the f32-exact decoder question are `followup_metal.md` sec.27.

- **LANDED (2026-09-24) - the FastConformer Metal chain's rel-pos attention rides the f32 GEMM
  builder per head (`ARCHITECTURE_GPU_TOWER.md#tower-encode-chains`), where the per-row kernel it replaced
  was most of the encode and lost to the CPU q8 lane past a minute of audio.** Box: the M5 Max,
  every figure below `-jit` on this tree with `DAS_TUNE_MANIFEST=performance/m5.tune.json`
  (its runtime section applied, the kernel winners on their fallback bodies - the sidecar
  predates the binary), no other overrides unless named. Encode alone, parakeet-TDT v3 f32,
  gb1 (199 s of audio, 2485 frames), `harness/asr_stage_probe.das -m
  ggml-parakeet-tdt-0.6b-v3-f32.bin -w gb1.wav --reps 2`, its `pk.encode_total` bucket per
  pass, `direction-grade` (three processes): the GEMM-built chain 652 ms; the CPU q8 lane
  (`DASLLAMA_METAL_TOWER=0`) 2306 ms; the CPU f32 lane (`DASLLAMA_METAL_TOWER=0 --fp32`)
  9687 ms. The per-row kernel's chain read 3.39 / 3.40 s on the same clip, and that chain with
  its attention dispatch skipped by a local edit 0.46 s - the share the rework closed - both
  from a scratch timer around `parakeet_encode`, `direction-grade`. Served cells, `debug-jit`,
  `benchmarks/lcpp_bench.das --asr -m <model> -r 2 -t 6 --ngl 99 --for-debug-purposes` with
  `DASLLAMA_ALLOW_UNTUNED=1`, one value per clip (the bench prints the best of its reps),
  `direction-grade` against the board's 2026-09-23 rows (das sha ea5747c91): parakeet v3 jfk 61 ms, jfk3 161 ms,
  gb1 0.96 s, hp0 1.37 s, hp0x2 3.33 s, librispeech25 1.36 s - the board's das q8 CPU rows
  158 ms / 4.35 s / 25.7 s on jfk / gb1 / hp0x2 (`performance/records/m5.json`, `-t 6`);
  canary jfk 275 ms, jfk3 763 ms, gb1 3.22 s - the board's Metal row 322 ms / 6.50 s on the
  per-row kernel. `external`, whisper.cpp at 6fc7c33 built on this box,
  `~/Work/whisper.cpp/build/bin/parakeet-cli -m models/ggml-parakeet-tdt-0.6b-v3-{f32,q8_0}.bin
  -f <clip> -t 6` (its default Metal arm; the records rig passes `-ng`), the sum of its
  `mel time` + `encode time` + `decode time` + `predict time` print lines, f32 / q8_0: jfk
  82 / 86 ms, gb1 1.06 / 1.02 s, hp0 1.63 / 1.59 s, hp0x2 4.68 / 4.59 s; its `encode time`
  print (`t_encode_us`) alone 0.63-0.64 s at gb1 and 3.52-3.53 s at hp0x2. `external`, the
  board's NeMo `--device mps` row: canary jfk 525 ms, gb1 4.36 s. The board's reference roster
  wants parakeet-cli's Metal arm beside the `-ng` one - the run above, v3 f32, `-t 6`, no env.
  What remains of the encode is the block GEMM chain on the f32 builder (the chain less its
  attention, about 0.47 s at gb1 by the same local edit) and the shift-add the softmax row
  folds. The chain's per-encode scratch grows with the frame count T and has no cap (mp = T
  to 32, nkp = T to 64, wwpad = 2T-1 to 64, all f32): the two score slabs mp x nkp and mp x
  wwpad plus the probability slab mp x nkp, and the AV rows heads x mp x hs - at gb1 (T =
  2485) 25 + 50 + 25 + 10 MB, at hp0x2 (T = 6800) 187 + 373 + 187 + 28 MB. The lanes' own
  distance: on gb1 the q8 lane flips 16 of 655 TDT durations against the f32 planes, token
  ids and text exact.
- **The resident plan and upload read one plane list (`resident_planes`,
  `ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan`).** The list is built twice per load - once
  for the plan's byte sum, once for the upload's reserve - and holds one 24-byte record per
  device plane: about 25 KB on a 94-layer model, a few microseconds each, against a load that
  moves gigabytes. Cost today: nothing measurable; the fix, if the list ever grows per-token
  readers, is to build it once at load and keep it on the resident state.
- **LANDED (2026-09-18) - a K/V mirror region per stream trades context for concurrency, not
  for throughput (`ARCHITECTURE_GPU_VULKAN_RESIDENCY.md#resident-plan`).** The resident driver's
  mirror is one allocation a side; `set_gpu_resident_regions` splits it, and the residency plan
  pays for the regions out of each stream's context. gemma-4-E2B-it-Q4_K_M.gguf served by
  `daslang -jit utils/dasllama-server/main.das -- --config <toml>` (`backend = "gpu"`, the vulkan
  tier in its default cm2 mode, kv f16, the box's tuned sidecar, `DAS_JOBQUE_THREADS=16`, no other
  override) on a Threadripper 3990X with an RTX 5060 Ti 16 GB under Windows; the instrument is
  128-token chat requests at temperature 0 through `/v1/chat/completions`, a rate being completion
  tokens over the request's wall, prefill included, and the footprint `/v1/stats`'s
  `gpu_vram_bytes`. At `streams = 1`: one region of 131072 positions, a 2304 MB mirror, 3628 MiB
  on the device, one stream 147.9 / 150.8 / 149.6 tok/s, four requests queued on the one slot
  148.9 tok/s summed. At `streams = 4`: four regions of 49926 positions, a 3510 MB mirror, 4836
  MiB on the device, one stream 148.8 / 151.8 / 152.3 tok/s, four requests at once 140.9 tok/s
  summed, `gpu_cpu_passes` empty in both. The decision: the server asks for a region per stream
  (1208 MiB and 62% of the context, on this model), because four conversations then move
  together instead of waiting in a queue; the summed rate does not rise, since the batched step
  still dispatches its rows one at a time - `followup_vulkan.md` item 69's closing paragraph is
  the batched dispatch that would raise it. Before the regions the same four-stream config served
  on the per-op tier at tg128 34.0 tok/s (the server's in-process bench), its paged sessions
  handed to the CPU on every step.
- **LANDED (2026-09-18) - the StyleTTS2 harmonic source streams in frame windows
  (`ARCHITECTURE_TTS_MEMORY.md#tts-source-stream`).** The source held four tables of samples times
  harmonics for the whole chunk - the cycles, the upsampled phase, the noise draw and the sine
  rows, 6.2 MB each for a 7 s sentence and 21.5 MB each at 25 s - to hand `linear_rows` nine
  values per sample. Only the cumulative phase sum reaches across the chunk, and it runs at frame
  rate, so the source now runs 64 phase frames (19200 samples) at a time on a carried sum, the
  resamplers taking a window of columns with the same tap arithmetic and the noise stream drawn
  in row order a window at a time. Bit-identical PCM on every carrier and length (FNV over the
  samples, one probe program over both trees); on one thread, five reps, cv under 2%, the say
  takes what it took - kitten-nano 345 ms -> 343, kokoro 1.56 s -> 1.55, kitten-mini 0.91 s -> 0.91
  at 7 s, 1.23 s -> 1.24, 4.95 -> 4.95, 3.13 -> 3.14 at 25 s; heap peak after one say on top of
  the six-buffer entry below: kitten-nano 170 -> 147
  MB (7 s) and 448 -> 365 (25 s), kokoro-82m 245 -> 222 and 630 -> 554, kitten-mini 222 -> 203
  and 595 -> 524. What remains is the generator's six stream-sized buffers, which the chunk cap
  bounds; the generator itself cannot window, because every Snake block's AdaIN takes its
  statistics over the whole stream.
- **LANDED (2026-09-18) - the StyleTTS2 generator runs on six buffers, the idle release covers
  its carrier, and the chunk cap is a knob (`ARCHITECTURE_TTS_MEMORY.md#tts-generator-buffers`).** A
  Kitten or Kokoro say holds its memory in the iSTFTNet generator's `[t][c]` rows, every buffer
  the stage's whole stream and the count of them live at once the footprint: nine same-size
  fields, three of which held values dead by the time the next was written. Six fields now
  carry the same roles (the block input doubles as the pair's second conv output, the noise
  branch's rows as the stage stream, its residual as each Snake block's), which moves values
  between addresses and nothing else - the PCM is bit-identical on every carrier and length
  (FNV over the samples, before and after). Heap peak after one say, das heap counters, the q8
  lane, M5 Max, one process on the box: kitten-nano 196 -> 170 MB for a 7 s sentence and 539
  -> 448 for a 25 s one, kokoro-82m 299 -> 245 and 800 -> 630, kitten-mini 269 -> 222 and 752
  -> 595 (the model itself 42 MB; the peak is per chunk and linear in its length). On one thread,
  five reps, cv under 2%, the say takes what it took: kitten-nano 345 ms -> 345, kokoro 1.56 s
  -> 1.56, kitten-mini 0.92 s -> 0.91 at 7 s; 1.25 s -> 1.23, 5.00 -> 4.95, 3.14 -> 3.13 at 25 s.
  `tts_release_scratch` deleted the block home's globals but left the `St2Scratch` carrier
  (109 MB of the 7 s kitten-nano say's 167) to keep a noise state a synthesis redraws from its
  seed anyway; it now frees the carrier too unless the parity rail captured the noise, and the
  heap after the release reads the model alone on every carrier. `tts_set_chunk_chars` lowers
  the 400-codepoint chunk cap per model, so a memory-tight build bounds the peak directly.
  One trap on the way: the after-tree read 15% slower in the generator on the 7 s says (400 ms
  against 345, reproducible across processes) with one probe program and identical to the
  before-tree with any other - the JIT cache had pinned one codegen of that program, and the
  op buckets under the profiling rail were equal to the millisecond (`followup_general.md`
  row 154).

- **LANDED (2026-09-18) - the Pocket codec streams in windows of 16 latent frames
  (`ARCHITECTURE_POCKET.md#pocket-codec-stream`).** The codec's activations, about 20 MB per second of
  audio across the chain's ping-pong rows, were the say's and the clone's working set: 116 MB
  for a 5.7 s chunk, 233 MB for an 11 s clip (das heap counters, the English q8 file, M5 Max);
  at 16 frames they are 41 MB whatever the run's length, 21 MB at 8. On one thread (26 runs, cv
  under 1%) a window is cache-sized rows and beats the whole run: the 5.7 s decode 374 ms -> 365
  at 16 and 360 at 8, the 11 s encode 710 -> 672 and 664. On fourteen threads the rows a step
  hands the workers thin out: codec minima 81-89 ms for the whole run against 93 at 16, 94 at 8,
  106 at 4, inside a run-to-run band of a tenth (cv 5-7%, void by the discipline; the say's total
  196-217 ms whole, 204 at 16). The remaining cost is the lane split at a few hundred rows -
  `followup_general.md` row 153. What a say still leaves behind, by the heap report's blocks:
  the roster voice's state (six layers, keys and values), the model's `PocketScratch` at 57 MB,
  and the block home's own scratch globals - `conv1d_rows_transposed`'s tap lift and the
  attention head scratch at 6 MB. Nothing leaks; the scratch is sized to the largest run it saw.
  Two levers landed with it: the voice's caches reserve the rows one chunk of the budget can
  take instead of a flat 1024 (54 MB -> 19 MB a voice, cap 411 rows for a 126-row clip), and the
  tap lift runs a cache-sized block of input rows at a time from the last block down, so an
  output row sums its taps in the same order (66 MB on the whole run, 15 at 16 frames -> 4 MB;
  the whole-run decode on one thread 374 -> 360 ms for the cache-sized rows). A say now adds 87
  MB to the heap where it added 133, and `tts_release_scratch` gives the carrier and the globals
  back when the caller says idle memory matters: the browser examples' speech threads call it
  after half a second with nothing queued.
- **LANDED (2026-09-16) - the browser build runs on every engine; `+relaxed-simd` leaves the
  wasm feature string.** WebKit implements none of the relaxed opcodes, and the previous entry's
  `+relaxed-simd` put `f32x4.relaxed_max` (the backend's lowering of an `nnan` vector max, which
  the previous entry believed it never emitted) into function 298 of every release, so Safari
  refused the whole module at validation. The feature is gone and the module links
  `-sMEMORY64=2` (wasm64 pointers, the memory lowered to 32-bit). Cost measured on the storywish
  page in Chrome (this M5, `DASLLAMA_ALLOW_UNTUNED=1`, one run each, steady-state clips): Pocket
  TTS rtf 0.167-0.171 with the fused multiply-add, 0.167-0.171 without it - the split
  `mul` + `add` costs nothing measurable, so the previous entry's first lever bought reach on
  Chrome only in theory. Prefill and decode of the story model were not measured separately.

  Provenance, **direction-grade** and **out-of-process**: the two readings compare two wasm
  builds (the feature string with and without `+relaxed-simd`, same tree otherwise), and every
  figure is read from a console line outside the timed program. Box: the M5 Max Mac (this
  machine). Harness: the storywish page's own first story, reading the engine's
  `dasLLAMA tts: ... rtf` line from the Chrome console; the artifact is the wasm64 release of
  `examples/dasLLAMA/storywish` built by `daspkg release wasm` from the wasm-host worktree,
  served locally by `site-dasllama/serve.py` to Chrome. Flags: the wasm cross target (not `-jit`,
  not AOT), `DASLLAMA_ALLOW_UNTUNED=1` so `DAS_TUNE_POLICY` served the reference bodies, the
  release's 16-worker pool. One story per build, the steady-state clips after the first two.
  NOT board-grade: no record cell backs it, and an rtf read from a page console is not an
  `lcpp_bench` figure.
- **LANDED (2026-09-16) - the browser gets its FMA and an int8 dot, and the spin loop stops
  crossing into JS.** Four levers on the parrot page (Pocket TTS, zen2 box, Chrome with the window
  verified in front before AND after each run, 7 workers + team dispatch, `DASLLAMA_ALLOW_UNTUNED=1`
  so the figures are the fallback bodies'). One, `+relaxed-simd` joins the wasm feature string:
  `f32x4.relaxed_madd` is the only multiply-add wasm has, so without it every contracted
  multiply-add split back into `mul` + `add` - 8728 contractions across the module once on
  (`SIMDTernary` 1457 -> 10185). Two, `dot_k4q8` stopped walking its 32 weights one at a time: both
  nibbles land in a SIGNED `byte16` (`q & 15`, `(q >> 4) & 15` - 0..15 is non-negative, so both are
  valid int8) and feed `idot`, which every target already lowers for itself, so one body needs no
  target branch. The measure button's three runs went 1.1x -> 1.7x real time (per chunk, rtf
  0.892-0.947 -> 0.565-0.613; per stage ms, prompt 656-716 -> 313-349, backbone 1545-1731 ->
  812-909, codec 3739-4216 -> 2545-2822; head, which is not K-quant, 145-166 -> 153-173, the
  control). Three, spinning workers: 1.7x -> 2.3x, the two arms taken in ONE page session with the
  lab's spin checkbox flipped in place (`set_jobque_worker_spin`), so one process and one
  instrument. Four, the spin loop's two JS crossings per iteration - `steady_clock::now()` for the
  deadline and `this_thread::yield()` for the pause - are now a strided read and a counted volatile
  loop; clock time across all threads fell 25034 ms -> 5089 ms in a 15 s window. That last one did
  NOT move wall time here and is not expected to on this box: 64 cores against 8 busy threads, so a
  spinning worker's waste is free. It is the core-scarce box where it should pay.

  Provenance, **direction-grade** and **out-of-process**: the kernel lever compares two wasm builds
  (two commits), and every browser figure is read from a console line outside the timed program.
  Box: the zen2 Windows box, 64 logical cores, `performance/defaults/x86-avx2` in force. Harness:
  the parrot lab's own measure button (three silent says a press), reading the engine's
  `dasLLAMA tts: ... rtf` line from the page console; the artifact is the wasm64 release of
  `examples/dasLLAMA/parrot` built by `daspkg release wasm`, served locally to Chrome. Flags: the
  wasm cross target (not `-jit`, not AOT), `DASLLAMA_ALLOW_UNTUNED=1` so `DAS_TUNE_POLICY` served
  the reference bodies, 7 workers with team dispatch on. The window was verified in the foreground
  before AND after each run. The contraction count is `wasm-opt --metrics` over the two builds'
  `parrot.wasm`. The clock-time split is a Chrome CPU profile of one measure run, aggregated per
  thread by node self time. NOT board-grade: no `performance/records/zen2.json` cell backs any of
  it, and an rtf read from a page console is not an `lcpp_bench` figure.

- **REVERSED (2026-09-16) - "a browser's job queue workers park instead of spinning" (2026-09-10,
  below) rested on two premises this arc falsified, and the browser now spins.** That entry
  measured spinning as HARMFUL on the M-series box (1.1x spinning against 1.4x parked) and set
  `dasllama_jobque_spin_default` to 0 under emscripten. It was measured (a) against the scalar
  `dot_k4q8`, which made every parallel section about twice as long as it now is, and (b) with a
  spin loop that crossed into JS twice an iteration - `steady_clock::now()` for the deadline and
  `sched_yield` inside the pause - which is most of what a spinning web worker was costing the
  caller. Both are gone. On the zen2 box spinning wins, 1.7x against 2.3x. The emscripten `[init]`
  that zeroed the window is deleted: a browser now takes the one 30 ms default every other target
  takes, and `setup_dasllama_jobque` pushes it with no platform test. Ruling (Boris): a slower
  M-series result is acceptable - the box is faster to begin with and most visitors are on a PC. A
  box that prefers parking still has the knobs (`jobque_spin_us` in a profile, the setter).

- **OWED - the playback underrun counter is browser-only.** `sound_playback_underrun_frames`
  answers a real count under emscripten with pthreads and a hard 0 everywhere else, because the
  ring and its producer compile only there - native, null-device and non-pthread wasm run the
  mixer straight off the device callback and have no ring to underrun. A script therefore cannot
  tell "no underruns" from "this target does not count them". The native implementation is owed:
  a device-callback path can still miss its own deadline, and counting that would make the same
  question answerable on a desktop. It is also the lever that would make the whole ring path
  testable off a browser, which is why the browser mixer has no test today.

- **LANDED (2026-09-14) - the gemma family at parity on the KHR cooperative-matrix arm.** A card
  without `VK_NV_cooperative_matrix2` (or the mode forced, `DASLLAMA_COOPMAT=mm`) ran every gemma
  prefill at 0.33 to 0.82 of llama.cpp b10660's own KHR path (`GGML_VK_DISABLE_COOPMAT2=1`, the
  same card): the attention on the 8-row and 4-row scalar tiles, the E-series' per-layer-embedding
  projection on the CPU, every Q8_0 plane on the q8-fed mul_mm L-tile in one k chunk. Four levers,
  each pod-confirmed: the KHR flash tile (now `FaT`'s KHR arm, 16x16x16 fragments; gemma-3-1b's attention
  12578 -> 1857 us a 512-row window, E2B's 37392 -> 3547, the 12B's 83941 -> 5746), the KHR f16 GEMM
  for the projection (`F16GemmKhr`; E2B's window 102 -> 44 ms), q8 on the KHR kq tile through
  `khr_stage16` (E2B's down 14021 -> 5165 us with the split), and the wave model's k chunks on the
  KHR arm (E2B's down 48 workgroups on 82 SMs before it). Every dense carrier now sits at or above
  parity under `mm` (pp512 / tg128, ours against llama.cpp's KHR path in tok/s, the llama.cpp figures
  `external`, five reps, one model a process): gemma-3-1b Q8_0 31751 / 27962 and 373.5 / 330.4 (1.136 /
  1.131), gemma-2-2b Q8_0 14909 / 13618 and 207.8 / 202.7 (1.095 / 1.025), gemma-3-4b Q8_0 10354 /
  8637 and 145.3 / 140.8 (1.199 / 1.032), gemma-4-E2B Q8_0 13748 / 12586 and 190.8 / 180.5 (1.092 /
  1.057), gemma-4-E4B Q8_0 7556 / 6511 and 108.9 / 109.3 (1.160 / 0.996), gemma-4-12B Q4_K_M 3514 /
  3048 and 79.98 / 80.08 (1.153 / 0.999), gemma-4-12B Q8_0 3395 / 2907 and 54.88 / 53.16 (1.168 /
  1.032), gemma-4-31B Q4_K_M 1253 / 1025 and 35.89 / 35.80 (1.222 / 1.003). The 26B-A4B is the gap
  as measured then: its routed block was cm2-only, so under `mm` the resident driver declined and
  the UD-IQ3_XXS read 328 / 39.1 against 4180 / 129.3 `external` (0.078 / 0.303) on the per-op
  rails; the resident block serves a KHR-mode card now, and the row's re-measurement is owed
  (`followup_vulkan.md` item 66).
  Decode is mode-independent. Provenance, direction-grade: the RunPod RTX PRO 4500 Blackwell 32 GB
  (82 SMs) with the device created in KHR mode, `lcpp_bench --for-debug-purposes -r 5 -p 512 -n 128
  -t 16` under `DASLLAMA_COOPMAT=mm DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_GPU_MIN_CTX=2048
  DASLLAMA_GPU_VRAM_MB=31000` (the image rail off: a forced mode re-mints the image per box identity),
  against the prebuilt llama.cpp b10660 Vulkan `llama-bench -ngl 99 -fa 1 -t 16 -r 3` under
  `GGML_VK_DISABLE_COOPMAT2=1` on the same pod; the per-role figures from the resident prefill's
  `DASLLAMA_GPU_PROF=1` window profile. The same tip in the default cm2 mode reads gemma-3-1b at
  0.979 of llama.cpp's cm2 path on the pod (34546 against 35284 `external`).

- **LANDED (2026-09-14) - the gemma family at parity on the RTX 5060 Ti, no kernel work.** The
  pod's levers carry to the 16 GB Blackwell consumer card as they stand: every gemma carrier that
  fits sits at or above 0.95 of llama.cpp b10660 on both rows (pp512 / tg128, ours against
  llama.cpp in tok/s, the llama.cpp figures `external`, five reps, one model a process, the box
  idle): gemma-3-1b Q8_0 22213 to 22900 / 21233 and 248.6 / 214.5 (1.062 to 1.079 / 1.159),
  gemma-2-2b Q8_0 10722 to 11095 / 10737 and 121.1 / 116.8 (0.999 to 1.033 / 1.037), gemma-3-4b
  Q8_0 6321 / 5905 and 82.9 / 79.7 (1.070 / 1.040), gemma-4-E2B Q8_0 9689 / 8372 and 121.9 / 110.2
  (1.157 / 1.106), gemma-4-E4B Q8_0 4333 / 4328 and 65.06 / 63.34 (1.001 / 1.027), gemma-4-12B
  Q4_K_M 2120 / 1998 and 46.9 / 46.0 (1.061 / 1.019), gemma-4-12B Q8_0 2251 / 2156 and 30.04 /
  29.84 (1.044 / 1.007; the Q8_0 file at `DASLLAMA_GPU_MIN_CTX=1024`, its 11.8 GB against the
  card's 16; the 26B and 31B files do not fit and decline on memory). The small carriers' prefill
  on this box is bimodal by the process: unconfined, seven of fourteen gemma-3-1b runs read 19.3 to
  19.7 thousand tok/s (0.91 to 0.93) and the rest 22.7 to 23.3 thousand, the rep spread inside a
  run small either way, while llama.cpp holds 21.2 thousand (`external`) across every run; the same
  process confined to sixteen logical processors (`start /affinity`, either end of the 3990X's 128)
  reads the high level six of six times (22.2 to 22.9 thousand) and gemma-2-2b 10.7 to 11.1
  thousand against its unconfined 10.3 +- 1.0. The figures above are the confined runs for those
  two; the rest are the unconfined record sweep. The cause is the host pacing of a 25 to 50 ms window over a many-CCD
  part - `followup_general.md` item 148. Provenance, direction-grade: Boris's box (Threadripper
  3990X, RTX 5060 Ti 16 GB, 36 SMs, driver 616.56, `VK_NV_cooperative_matrix2` with the decode
  vector arm live), `lcpp_bench --for-debug-purposes -r 5 -p 512 -n 128 -t 16` under
  `DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_GPU_MIN_CTX=2048` against the local llama.cpp b10660 Vulkan
  `llama-bench -ngl 99 -fa 1 -t 16 -r 3`; the 12B Q8_0 row from the pass's first sweep - in the
  record sweep the desktop held 4.3 GB of the card and both sides spilled to host memory (ours
  64 / 4.4, llama.cpp 793 / 4.9 `external`), a reading of nothing.

- **LANDED (2026-09-13) - the gemma family at parity on the Vulkan resident driver.** Every gemma
  carrier the model table stocks sits at or above 0.95 of llama.cpp b10660 on both rows, seven of
  nine prefills and two decodes ahead of it (pp512 / tg128 ratios, five reps, one model a process):
  gemma-3-1b Q8_0 0.974 / 1.038, gemma-2-2b Q8_0 1.023 / 0.970, gemma-3-4b Q8_0 1.017 / 0.988,
  gemma-4-E2B Q8_0 1.038 / 0.981, gemma-4-E4B Q8_0 0.994 / 0.950, gemma-4-12B Q4_K_M 1.026 / 0.950,
  gemma-4-12B Q8_0 1.010 / 0.987, gemma-4-26B-A4B UD-IQ3_XXS 1.071 / 1.025, gemma-4-26B-A4B UD-Q4_K_M
  1.128 / 0.950. The last three prefill levers of the arc, each pod-confirmed: the split-k arm's k
  offset masked to its chunk alignment (unmasked, one chunk of gemma-3-1b's down ran 304 us against
  the whole GEMM's 152, the probe's `splitk` arm on the pod, so every split lost) with the tile and split picks folded into one wave model (`cm2_gemm_pick`; gemma-3-1b's
  down 152 -> 78 us in four l chunks, E2B's 12288-deep down 268 -> 162 in three); the flash tile's
  f16 O accumulator under a 3 ln 2 row-max bias, its mask pass gated to the edge steps and a
  `[dont_unroll]` KV loop (E4B's attention 6868 -> 4014 us a window, gemma-3-1b's 2671 -> 1844);
  and a partial last weight tile served on the fast path over the plane's last whole 128 rows with
  Q6_K's scales staged (the 26B's 2112-wide shared expert 300 -> 71 us a dispatch, its 704-wide
  experts 26.6 -> 20.5 ms a window). The three decodes at 0.950 (E4B, the two Q4_K_M files) are
  `followup_vulkan.md` item 51's dependent-dispatch gap. Provenance, direction-grade: the RunPod
  RTX PRO 4500 Blackwell 32 GB (82 SMs, driver without `VK_NV_cooperative_matrix_decode_vector`,
  the scalar decode arm), `lcpp_bench --for-debug-purposes -r 5 -p 512 -n 128 -t 16` under
  `DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_GPU_MIN_CTX=2048 DASLLAMA_GPU_VRAM_MB=31000`, against the
  prebuilt llama.cpp b10660 Vulkan `llama-bench -ngl 99 -fa 1 -t 16 -r 3` on the same pod; the
  per-dispatch figures from `harness/vk_gemm_probe.das`'s `gemma`, `splitk` and `cm2g:k6` arms and
  the resident prefill's `DASLLAMA_GPU_PROF=1` window profile.

- **LANDED (2026-09-10) - a browser's job queue workers park instead of spinning.** The
  engine's 30 ms spin-before-park window (`g_jobque_spin_us`) was unconditional, and in Chrome
  the 8 spinning web workers cost the caller its core: Pocket TTS (the parrot page's poem, four
  chunks, wasm64, 8 workers in the pool, the M-series box) generated at 1.1x real time with the
  renderer at 680-920% CPU, against 1.4x at ~112% with the workers parked (per chunk, ms:
  prompt 820 / backbone 1850 / codec 3600 spinning, 550 / 1250 / 2900 parked; three runs each
  within 3%). Team dispatch stays on there (0.69 against 0.72 rtf off, the head 98 against 140
  ms). One worker instead of eight, parked or spinning, reads 0.7x (prompt 1370 / backbone 3080
  / codec 5800), so the pool does pay - 2.2x from one to eight - once nothing spins. The
  emscripten default was set to 0 here (`dasllama_jobque_spin_default`) - REVERSED 2026-09-16, see
  the entry above; the desktop keeps its window.
  Provenance, direction-grade: the m1 box (10 cores, the `m1.tune.json` profile); the browser
  figures are the engine's own per-synthesis timing line (`dasLLAMA tts: ... rtf`) read from the
  page's console, the wasm64 release of `examples/dasLLAMA/parrot` served locally to Chrome under
  `DASLLAMA_ALLOW_UNTUNED=1` (the reference kernel bodies, no tune sidecar, so the absolute
  figures are the fallback bodies'), the lab's own toggles flipping the arms between measures,
  the CPU readings `top` over the renderer process; the desktop figure (18x on 7 workers) is the
  same timing line under `-jit` with the box's profile, the same text through the lab's measure.

- **OWED ROWS - the Vulkan cm2 prefill's board cells.** `performance/records/zen2.json` carries
  one das/vulkan cell (Qwen3-4B Q8_0 under `DASLLAMA_COOPMAT=mm`, 2026-07-25) and no cm2 cell,
  while the Vulkan pp arc (the group-aware split-k, the hand-laid four-wide twins, the
  last-layer slice, the parallel embed) made every Vulkan prefill faster -
  the Q4_K_M 1B window 27.8 -> 22.1 ms on the `lcpp_bench --for-debug-purposes` rig,
  stage readings only. `performance/gen_bench_records.das`'s hardware stamp refuses this box
  while the remote-access daemon runs, so the mint waits for the owner's rig window. Owed: the
  zen2 vulkan cell re-minted, plus a cm2 cell (the served default on the 5060 Ti) on a 1B kq
  vehicle, both spawned by `gen_bench_records`, both walls out-of-process, direction-grade
  against the 07-25 row; done when both rows sit in `performance/records/zen2.json` and the
  site records are regenerated.

- **OWED ROWS - the parity pass's ten 1B pp512 vehicles.** The mirror pass (2026-09-03) read
  Q3_K_L at 1.08, IQ3_M at 1.06 and IQ2_XXS
  at 0.98 of the reference on the `lcpp_bench --for-debug-purposes --plen 512 --ngen 0 --reps
  12` rig bracketed by `llama-bench -p 512 -n 0 -r 6`, stage readings only; the board's rows
  for those vehicles are two-rep readings and stand until re-minted. Same refusal as above -
  the hardware stamp declines this box while the remote-access daemon runs. Owed: the ten
  vehicles' vulkan pp512 rows re-minted by `gen_bench_records`, direction-grade against the
  rows they replace; done when they sit in `performance/records/zen2.json`.

- **LANDED (2026-09-09) - the shared expert's K-quant planes are minted beside the q8
  transcode, and the whole-model resident driver reads those.** A MoE's shared expert reached
  the device only as the loader's q8 transcode of the file's Q4_K / Q6_K planes - twice the
  bytes its decode GEMVs read. The loader now keeps the gate/up/down planes in the file's own
  K-quant format beside that transcode (`wsh*_fmt`, `wshk*_offs`; `IMAGE_VERSION` 36, so every
  image re-mints once) and the whole-model resident driver places the K-quant copies. On the
  Qwen1.5-MoE-A2.7B-Chat Q4_K_M twin the device image reads 10107 -> 9756 MB and the token
  7.06 -> 6.12 ms (the `vk_rdec gpu avg/token` line of `benchmarks/lcpp_bench.das --prof
  --jobque-profiling` under `DASLLAMA_GPU_PROF=1`, RTX 5060 Ti) [direction-grade - two commits].
  The pair to read is footprint against wall clock: the device image and the token both fall,
  while the loaded model on a GPU box carries BOTH copies - the loader mints the K-quant planes
  only on a load with a GPU tier armed, so a CPU-only box's planar image is what it was, and the
  tier-armed load of the twin serves 11781 MB of weights against the CPU-only load's 11291 (the
  490 MB: three planes x layers x dim x n_ff_shexp weights in the file's format, k4 +381 MB and
  k6 +109). That growth stands until the per-op and CPU rails read the
  K-quant planes too and the q8 transcode goes,
  which `followup_vulkan 43` owns in its still-open list (the CPU chain's shared expert on the
  same K-quant planes; today the resident-vs-CPU bar carries the two forms' rounding).

- **LANDED (2026-09-10) - the expert stamps of the grid-codebook formats run a 32-deep k step;
  every other stamp keeps 64.** The cm2 tile template's k step (`BK`) is a stamp constant: the s
  and e stamps (the expert schedule's 32-row and 128-row columns, `CM2_TC_E` in the class ladders)
  of iq2xxs, iq2xs, iq2s, iq3xxs and iq3s run 32-deep, every other stamp 64
  (`ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-tile-body` carries the tile readings behind the split). On the
  RTX 5060 Ti (`benchmarks/lcpp_bench.das -jit --for-debug-purposes -r 3 -p 512 -n 128 -t 16`
  under `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_GPU_MIN_CTX=2048`, the
  x64-gen backend, untuned-stamped; the arms' rates sit in `followup_vulkan.md` item 45):
  Qwen3.6-35B-A3B UD-IQ2_XXS pp512 1.08x with the four-wide decode twin and 1.17x without it
  (`DASLLAMA_VK_DECVEC=0`, the arm a driver without `VK_NV_cooperative_matrix_decode_vector`
  runs), tg128 1.00x on both arms; the Qwen1.5-MoE-A2.7B-Chat Q4_K_M twin (k4 expert planes)
  1.07x on the four-wide arm, where the same stamps at 32 had read behind the 64-deep ones - the
  reason the K-quants keep 64; against llama.cpp b10660 on the same box (`external`, its rates and both builds
  in item 45) the 35B's twin arm moves from 1.06x to 1.14x of its four-wide arm and the scalar arm
  from 0.86x to 1.00x of its scalar arm [direction-grade - one commit].

- **LANDED (2026-09-10) - a split role takes as many k chunks as fill the device by itself.**
  The split pick (since folded into `cm2_gemm_pick`) still lets the dispatch group (a role plus the neighbours it co-runs beside)
  decide whether k splits, but the chunk count is the SM count over the role's own workgroups,
  since a split role serializes its group through the one scratch plane anyway
  (`ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default`). On a Linux RTX 5080 (84 SMs, driver 580.173,
  the scalar decode arm, `-t 8`, the bench form of the row above at `-n 32`) the 35B's shared
  expert gate and up move from two chunks of 32 workgroups to four of 64: pp512 1.019x and
  1.014x of the two-chunk arm's two reads, tg32 0.99x and 1.00x; the profiled window's
  sh_gate + sh_up 8.25 -> 5.42 ms (`DASLLAMA_GPU_PROF=1`, the same runs; the reference exe's
  rows the ratios below name are `llama-bench -p 512 -n 32 -r 5` on b10660 on the same pod,
  `external`, and `followup_vulkan.md` item 45 carries every absolute rate of this sequence). The
  36-SM card's rows are unchanged by construction: no
  stocked model's window there has a group at half the SMs whose member alone would take more
  chunks than the group (the 35B's pair fills 32 of 36 and runs whole) [direction-grade -
  one commit].

- **LANDED (2026-09-10) - the cm2 tile loop's hand unroll is one superblock per block (`UNR`: 4
  steps of 64, 8 of 32) instead of eight steps.** The decode inlines once per unrolled copy, so an
  eight-copy 64-deep stamp's code, refetched after each window's weight stream had passed the L2,
  cost a 64-workgroup GEMM 16 us of its 27 on the Linux RTX 5080 (`harness/vk_gemm_probe.das --
  cold:k6`'s flush row: 43.0 us against 26.7 warm; 3 us on the RTX 5060 Ti), and four copies run
  the m tiles 10-14% faster hot there (`harness/vk_gemm_probe.das -- cm2:k6` gate m 57.3 -> 64.5
  TFLOP/s, q/wo m 51.1 -> 55.5; the l and s tiles' sweep sits in `followup_vulkan.md` item 45,
  `ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-tile-body`). Pod (the bench form of the two rows above at `-n
  32`, `-t 8`, the scalar decode arm): pp512 1.021x of the eight-copy form, tg32 1.007x; the
  32-deep expert stamps at 2 copies read below their eight-copy form with e_gate / e_up longer, at
  8 as before. RTX 5060 Ti (`-r 5`, `-t 16`, same session): the 35B twin arm within noise of the
  eight-copy form (0.997x and 0.989x on two reads), its scalar arm 0.999x, the Qwen1.5-MoE Q4_K_M
  twin 1.023x [direction-grade - one commit].

- **LANDED (2026-09-10) - the iq2xxs and iq2s scalar decode callbacks in pair form.** The callback
  derives every shared read from the pair's first element and selects the element last
  (`ARCHITECTURE_GPU_SHARED_GEMM.md#cm2-decode-16bit-lanes`), so the driver's two-wide callback commons the
  pair's work. Linux RTX 5080 (the scalar decode arm, the bench form of the rows above): the skewed
  expert schedule's e+s ladder `moesk:iq2xxs` 0.357 -> 0.316 ms (gate/up) and 0.360 -> 0.320 (down),
  `moesk:iq2s` 0.428 -> 0.380 and 0.484 -> 0.432 (`harness/vk_gemm_probe.das`); the 35B pp512
  1.054x of the single-element form, tg32 0.997x (0.805x of the reference exe's 5217 there,
  `external`). The four-wide twin arm (the RTX
  5060 Ti's) never runs the scalar callback, so its rows stand [direction-grade - one commit].

- **LANDED (2026-09-10) - the iq2xs, iq3s, iq3xxs, k5 and k6 scalar decodes take the pair form
  too, the K-quants dropping their byte2 lane selects for the shift form on the way.** Linux RTX
  5080, the scalar arm, `harness/vk_gemm_probe.das -- cm2:<fmt>` hot rates: k6 gate l 85.6 -> 94.3 TFLOP/s, m 65.4 -> 70.5, s
  27.6 -> 34.6, q/wo l 71.0 -> 78.2, m 56.1 -> 60.7, the shared expert's down (64 workgroups of
  K 512) 22.6 -> 21.5 us; k5 gate l 83.9 / m 62.7, q/wo l 69.9 / m 54.0, the shared expert's
  gate/up m stamp 105 -> 81 us, its down 31 -> 25 (the k5 rows before were read at the eight-copy
  unroll, so they fold that lever in); `moesk:iq3s` e+s reads 0.326 / 0.347 ms (no earlier pod
  row). The 35B's profiled q 3106 -> 2918, k 1205 -> 1103, v 1025 -> 940, wo 1974 -> 1815, the
  shared expert's three stamps 6653 -> 6430 us; pp512 1.008x of the single-element form (`-r 3`
  against `-r 5`), tg32 1.00x (0.812x of the reference exe's 5217, `external`) [direction-grade -
  one commit].

- **LANDED (2026-09-10) - the deltanet scan runs one column per lane cluster, k and q read from
  the read-only conv plane per lane, no shared staging and no per-token barrier.** Linux RTX
  5080, Qwen3.5-0.8B-Q8_0 (18 recurrent layers): the profiled scan 8854 -> 8084 us per window,
  pp512 1.033x of the staged form (0.746x of the reference exe's 29957, `external`), tg32 within
  noise (0.985x); the 35B 0.996x (noise). The same kernel without `@readonly` on `conv` and
  `smalls` read the scan at 19004 us and pp512 at 0.68x of the staged form: the NonWritable
  decoration is what lets a token's k and q loads pass the previous token's o store
  [direction-grade - one commit].

- **LANDED (2026-09-10) - every Vulkan binding no kernel of its class writes is NonWritable: the
  `[vk_dispatch]` lens stamps `readonly` from its access classification.** Only four bindings in
  the kernel file carried the qualifier by hand; every other buffer was declared writable, so the
  driver ordered a kernel's loads behind its stores to any other buffer. Linux RTX 5080: the 35B
  pp512 1.108x of the hand-qualified form (0.896x of the reference exe's 5217, `external`), tg32
  1.028x; the 0.8B pp512 1.043x (0.778x of 29957), tg32 1.056x (0.705x of 480); the 0.8B's
  profiled conv 1685 -> 1344 us per window, gate 1091 -> 1000, up 1007 -> 916, the decode step's
  total 3685 -> 3478 us per token; the sanity logits bit-identical. A same-run float4 form of the
  scan's lane shards (a lane's 16 rows contiguous, four loads per operand) read the scan and the
  window slower, and is not kept: the reference exe's shader interleaves its rows exactly as the
  scalar form does [direction-grade - one commit].

- **LANDED (2026-09-10) - the deltanet scan computes its per-token gates once per workgroup
  into shared memory.** The recurrence paid the decay (`exp(a * softplus(g + dt))`: exp, log,
  exp) and `sigmoid(beta)` (exp and a divide) per token per lane, with two global loads; the
  ablation that replaced them by constants cut the scan 7816 -> 5156 us per 0.8B window (the
  cluster reductions' ablation 7816 -> 6585, the ds-as-literal experiment 8060 -> 7816). The
  workgroup now fills two `DN_WINDOW`-long shared arrays before the token loop and the loop reads
  them. Linux RTX 5080, Qwen3.5-0.8B-Q8_0: scan 8060 -> 5240 us, pp512 1.146x of the per-lane
  form (0.892x of the reference exe's 29957, `external`), tg32 within noise; the 35B 1.022x at
  five reps (0.915x of 5217; a three-rep read was noise), tg32 1.00x; the sanity logits
  bit-identical on both [direction-grade - one commit].

- **LANDED (2026-09-10) - the window chain's MoE router and deltanet beta/alpha GEMMs ride the
  cm2 tile as f16 GEMMs (`F16GemmCm2`, eight k chunks into the split-k scratch, the reduce
  into the target).** The scalar tiles were bound by shared-memory traffic: the router at 53 us
  per layer for 0.27 GFLOP (82 before the NonWritable lever), the 16 x 16 beta/alpha tile at 94
  us per layer for 0.13 GFLOP, against the reference exe's f16 mul_mm at 10-20 us. Linux RTX
  5080, the 35B's profiled window: ba 2828 -> 1170 us over 30 layers, router 2125 -> 1621 over
  40 (three dispatches each under the drain: the f16 convert, the GEMM, the reduce); pp512
  1.039x of the scalar-tile form at five reps (0.951x of the reference exe's 5217, `external`),
  tg32 1.007x; the sanity argmax the same token at a logit 0.02 apart (f16 rows and activations
  where f32 ones were); the 0.8B unchanged (q8 beta/alpha planes, no router). The route costs a
  MoE model an f16 mirror of every router plane - `n_expert x dim` halfwords a MoE layer, minted
  only where the shape admits the route and counted by the resident plan (`resident_router_bytes`)
  [direction-grade - one commit].

- **LANDED (2026-09-10) - the deltanet conv is channel-major: a workgroup owns 256 channels
  over 32 positions, a thread slides one channel's window with the taps in registers, the
  per-head norm crosses lanes by shuffles and warps by one shared row.** The position-major form
  (one workgroup per position, the row staged in 32 KB of shared memory) re-read every input row
  once per tap and the whole tap table once per position from L2. Linux RTX 5080: the 0.8B's
  profiled conv 1351 -> 824 us per window (75 -> 46 us per layer; the reference exe's SSM_CONV
  27-47, `external`, its `GGML_VK_PERF_LOGGER=1 llama-bench -p 512 -n 0` run), pp512 1.029x of the
  position-major form (0.919x of the reference exe's 29957, `external`), tg32 within noise; the
  35B's conv 2740 -> 1660 us over 30 layers, pp512 1.010x at five reps (0.960x of 5217), tg32
  flat; the sanity argmax the same token, its logit 0.02 apart (the norm's sum in butterfly order)
  [direction-grade - one commit].

- **LANDED (2026-09-11) - the deltanet scan loads the next token's k, q and v into registers while
  the current token computes.** The per-token chain was latency-bound on its own row's loads (the
  9B's scan 342 us a layer against the reference exe's GATED_DELTA_NET 250: 668 ns a token); now a
  token's loads issue a token ahead and only the recurrence stays on the chain. pp512 on the pod
  (Linux RTX 5080, -r 5, -r 10 on the 0.8B): the 0.8B 1.053x of the unprefetched form (0.919x ->
  0.969x of the reference exe's 29957, `external`; its scan is a quarter of the window), the 9B
  1.002x (its scan role 8193 -> 7736 us a window), the 35B 1.013x; tg32 flat. The deltanet cells
  match the CPU oracle (6 of 6). Dead beside it, all measured on the pod by whole-model rows: a
  forced barrier between the prefill's gate and up GEMMs costs little (they already co-run), a
  hazard barrier naming this op's stage alone leaves the chained-dispatch floor and tg32 where they
  are, and the step computing its own beta/alpha dots in place of the GEMV dispatch reads slower on
  the 0.8B and the 9B (the GEMV co-runs under qkv and z for free; its profile figure was the Linux
  driver's drain at the stamp) [direction-grade - one commit].

- **LANDED (2026-09-11) - the decode GEMV's block loop takes four blocks a lane straight-line, so
  four blocks' loads are in flight before a sum waits on one.** The rolled loop issued a block's
  loads after the last block's sum, one DRAM latency a block; the guarded single step stays as the
  tail. The GEMV probe's `single` arm (`harness/vk_gemv_probe.das`, one plane a dispatch under the
  token's hazard chain) reads the chained-dispatch floor at 4.5 us on every format (0.6 MB planes,
  Linux RTX 5080) and the served planes L2-warm; its ring form reads the streaming rate unchanged
  by the unroll (k4 903 -> 897 GB/s, k5 896 -> 895, k6 902 -> 903 at 4096 x 12288), so the gain is
  the in-token ramp of a plane read once: tg32 on the pod the 9B 1.021x of the rolled loop (0.912x
  of the reference exe's 112.8, `external`; its rows are four blocks a lane at K 4096 and twelve
  at K 12288), the 0.8B 1.004x (one or two blocks a lane: the tail alone), the 35B 1.001x (the
  expert rows under one block a lane), the 27B UD-IQ4_XS 0.995x; pp512 flat. The GEMV family cell runs
  its 13 formats at 5120 wide too (four unrolled steps and a tail at every lane split) beside the
  512-wide rows the tail alone serves [direction-grade - one commit].

- **LANDED (2026-09-11) - the token command's fused add+rms+requant serves every site on every model:
  a Q8_K twin for the K-quant feeds and a row-storing twin for the consumers that read the normed row
  as floats.** The fused twin (`cls_ar_rq`) quantized Q8_0 only and never stored the row, so
  `rd_fuse_gates` kept it off any model with a K-quant consumer, off hybrids (the beta/alpha GEMV
  reads the row) and off MoE (the router reads it) - two dispatches and a barrier per site where one
  would do. Now `ArRqT` stamps `cls_ar_rqx` / `cls_ar_rqk` / `cls_ar_rqkx` beside it, each site's
  stamp (`RqStamp`) picked once every layer is registered (`rd_ensure_fused_sets`: the consumer's
  block form, the row stored where a recurrent head or a router reads it), the layer after a MoE
  layer's combine requanting on its own; the profiler keys on the stamps' recorded names (`rd_ts`)
  instead of a count per layer kind. tg32 on the pod (Linux RTX 5080, -r 5, -r 10 on the 0.8B):
  the 0.8B 1.029x of the split rail (0.79x of the reference exe's 480, `external`), the 9B 1.033x
  (0.894x of 112.8), the 35B 1.019x (1.009x of 145.2), the 27B UD-IQ4_XS 1.092x (0.88x of 47.4);
  pp512 flat. The resident hybrid parity file holds (15 of 15); the
  requant-family cell holds the three stamps to the plain twin (bit for bit) and the Q8_K oracle
  [direction-grade - one commit].

- **LANDED (2026-09-11) - the resident decode's qkv and z GEMVs co-run: the z half of the projection
  row rides its own hazard class (`VHZ_DNZ`, the prefill's z class) and the step waits on both.** Both
  halves wrote under `VHZ_DNP`, so the rail put a write-after-write barrier between two GEMVs that
  read the same row and write disjoint halves; the per-op tier already split them (`VHG_Y1 / Y2`).
  tg32 on the pod (Linux RTX 5080, -r 5, -r 10 on the 0.8B): the 0.8B 1.046x of the shared-class
  form, the 9B 1.016x, the 35B 1.017x (0.991x of the reference exe's 145.2, `external`); pp512
  flat. The resident hybrid parity file holds on the split (15 of 15)
  [direction-grade - one commit].

- **LANDED (2026-09-11) - the fused deltanet decode step runs the delta rule with every thread owning
  one state column's row part in registers, its norms and q.k by subgroup adds, and the beta and
  alpha rows as one GEMV dispatch.** The step read the state column-per-thread in a rolled loop
  (each load's latency exposed in turn) with three single-thread loops over the head's 128 values:
  25 us a layer on every hybrid (Linux RTX 5080), the beta/alpha pair two 32-workgroup dispatches
  with a hazard barrier between. Now: the 64 loads of a thread's row part issue straight-line
  before a sum waits on one, the state held in registers from the k.S / q.S pass through the
  update; the q and k squared sums, the raw q.k and the out-norm's sum by `subgroupAdd`;
  `RouterGemvF16` reads its rows as half pairs and lands the alpha half at a second base
  (`RouterArgs.ne_split / obase_split`). Isolated (`harness/vk_gemm_probe.das dec`, Linux RTX
  5080): the step 8.8 us at 32 or 16 heads of 128, the GEMV 4.6 / 3.0 / 4.7 us at the 9B's, 0.8B's
  and 27B's shapes; the token profile's step role on the 0.8B 456 -> 222 us a token (25.3 -> 12.3
  a layer), ba 380 -> 310. tg32 (pod, -r 5, -r 10 on the 0.8B): the 0.8B 1.054x of the rolled form
  (0.70x -> 0.74x of the reference exe's 480, `external`), the 9B 1.048x (0.81x -> 0.85x of 112.8),
  the 35B 1.068x (0.91x -> 0.975x of 145.2); pp512 flat on all three. A first form with the load
  guarded inside the unrolled loop read SLOWER than the rolled form: a guard per copy puts each
  load in its own block, so the sums wait on the loads one by one. RTX 5060 Ti four-wide arm, the
  0.8B: 1.007x. The step and router GEMV cells match their CPU oracles, the GEMV cell gains
  the two-base arm [direction-grade - one commit].

- **LANDED (2026-09-10) - the k4, k5 and iq4xs cm2 tiles stage a scale cache: the tile's 128
  rows' eight sub-block scales (the K-quants' (d x sc, dmin x mn) pairs, IQ4_XS's d x (ls - 32)),
  filled once per superblock into shared memory, the decode reading one word pair where it read
  the scale plane per element.** The reference exe's `shAscales` form (its Q4_K and Q5_K tiles
  alone carry it; it has none for IQ4_XS). Linux RTX 5080, the scalar arm, `harness/vk_gemm_probe.das
  -- cm2:k5`: gate l 83.9 -> 116.2 TFLOP/s, m 62.7 -> 99.9, q/wo l 69.9 -> 95.9, m 54.0 -> 86.7,
  the shared expert's down 25 -> 17 us; `cm2:k4`: gate l 119.5, m 102.0, q/wo l 100.5, m 87.1 -
  past the reference exe's own 93-96 on the gate shape (`external`, its `GGML_VK_PERF_LOGGER=1
  llama-bench -p 512 -n 0` run); `cm2:iq4xs`: gate l 109.4, m 102.9, q/wo l 91.4. Whole-model rows
  (`benchmarks/lcpp_bench.das -jit --for-debug-purposes -r 5 -p 512 -n 32 -t 8` under
  `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1`, the pod): the 35B pp512 1.016x of the
  per-element scale read (0.970x of the day's reference row, `external`; its q5_K roles are the
  qkv, z, q and shared-expert planes), tg32 flat; the Qwen3.5-9B UD-Q5_K_XL pp512 1.175x (0.760x ->
  0.892x of the reference row), its profiled window 116 -> 99 ms (the q5_K FFN roles 18.2 / 17.9 /
  16.6 -> 13.9 / 13.7 / 13.9 ms, qkv 11.2 -> 9.6, the attention layers' FFN 16.0 -> 13.4); the
  Qwen3.8-27B UD-IQ4_XS pp512 1.142x (0.892x -> 1.014x of the reference row), tg32 flat; the
  Qwen3.8-27B UD-Q3_K_XL pp512 1.087x (0.883x -> 0.959x; its q3_K roles stage no cache), tg32 flat.
  RTX 5060 Ti, the four-wide arm: the 9B 1.028x (the twin already shared one scale extraction
  across four elements), the 35B flat. The k4, k5 and iq4xs tile cells match the CPU oracle on all
  five arms [direction-grade - one commit].

- **OPEN (narrowed) - the gemma3v encode residual after the tower flash: ~0.92x vs the
  pair.** The slab road closed in three landings: the 96 head pad (guarded AV columns,
  668 -> 486 -> 452), then the LIFTED dk72 flash (MetalTowerFlash + the per-head-contiguous
  f16 K/V restride - a strided compact K/V scatters every simdgroup tile; the adoption race
  lives in `benchmarks/attn/bench_metal_pf_fused_attn.das`'s TOWER/PORTC arms and the PR).
  Cell shape now (lcpp_bench image cells, m5): gemma-3-4b 404 ms and gemma-3-12b 407 vs the
  pair's 370/380 [direction-grade - two processes]. The
  remaining ~30 ms splits across the per-layer hc-cvt dispatches (~8 ms), the ledgered
  post-CPU-window commit slack, and race-vs-cb occupancy; next levers if reopened: fold
  the hc restride into the K/V GEMM epilogues (dual-store), a Q=16/C=128 flash variant via
  the disasm discipline, and the flash's masked ragged-npos tail (would admit the qwen
  towers, whose ~1200-row attention is small anyway).

- **LANDED - the mul_mm prefill billed ceil-32 M tiles, so an npos just past a tile boundary
  paid a whole extra tile-row of wall for rows nothing reads; the GEMV-tail dispatch peels
  npos % 32 in [1,8] onto the fixed-B mv family (measured 2026-08-24, M1 Max, the qwen
  img:pp chase; plan: `qwen_pp_plan.md`).** The dig that got here killed two prior theories:
  the "8B small-M text GEMM gap" (-6% at p341) was the cold-first-process page tax - the warm
  re-run prices das +2.1% AHEAD (never conclude from run 1, struck AGAIN); and the "4B
  wide-staging tax" was dead too - the one-process probe (`harness/image_turn_probe.das`,
  three workloads x five skip arms) put the ds-wide quantum at +0.1 ms over narrow and the
  whole in-process image tax at ~+6 ms, all in the attention family. The real structure is
  the M staircase: 8B text p320 = 551 ms (10 tile-rows), p321 = 599, p352 = 600 (released
  exe walls, the bench's own driver lines) - one
  tile-row = ~48 ms, and the image turn (321 = 10x32+1) bills it for ONE real row. The tail
  dispatches floor(npos/32) tiles + the remainder rows on the mv GEMVs; a lone row rides the
  reduction-split decode GEMV - the mv forms idle half their lanes on it (probe before/after
  across the two commits, direction-grade, tokens wall 599.2 -> 575.4 -> 566.8 ms: the
  mv-only tail -24, the r==1 reroute -8.6).
  Per-form reduction alignment gates the peel (the b4 stripe reads whole 128-quant rounds;
  kdim % 128 != 0 keeps the padded tile - dims 576/896/1152 misread otherwise,
  device-probed), and r >= 2 always takes the b4 form (b2's stripe needs % 256). The mv_b2
  store now gates on nrows - defensive: at nrows == 1 it phantom-wrote its second row, an
  unread pad row or an off-the-buffer write depending on which batch path sized y.
  Results (released exe, r=3, quiet M1 Max, fresh upstream pairs): 8B img:pp 522 -> 552
  tok/s (-7.5% -> -2.2%), 4B img:pp 942 -> 986 (-2.7%), 8B text p321 534.5 -> 566.7 (+7.8%
  ahead); p341/p512 unchanged (remainders outside the window); 30B Omni img:pp 783 (rides -
  its FFN is MoE, out of tail scope). The span-attention tax split on the finer knockouts
  (attn_qk/attn_sm/attn_av x span-size scaling): ~3 ms was a CONSTANT AV overhead - with the
  span armed the AV kernels walked every query tile to np32, dropping the causal limit
  wholesale; the bound is now tile-conditional (max(uend, causal reach) only where the tile
  holds span rows - a pure shrink over softmax-zeroed columns, the f16 fused-vs-splice
  witness reads 0 with it). The 1-row-span tax fell 3.3 -> 0.58 ms (small-media turns win);
  at the 300-row turn the remaining ~6 ms is INTRINSIC non-causal pairs - the image cell
  does not move. The stage/split figures here are `harness/image_turn_probe.das` readings
  (one process, best-of-3, the `DASLLAMA_METAL_PREFILL_SKIP` arms) - direction-grade; the
  Results line above is the record-grade board. **OPEN followups: (1) the post-encode
  context residue (~10 ms on 8B - the bench-context image eval vs the probe's in-process
  wall: gpu 577 vs 570.3 ms = ~6.7 GPU cache aftermath of the per-turn tower encode, and
  total-minus-gpu 5.6 vs 1.8 ms = ~3.8 host commit slack;
  lever = encode-ahead overlap, shared with the decode per-step-overhead entry); (2) the kq
  mul_mm sites and the MoE prefill GEMMs take no tail yet (the kq mv twins exist); (3) r in
  [9,15] via a third mv pass (~12 ms more on the shapes that hit it); (4) PRE-EXISTING: the
  batch-DECODE mv rail dispatches the same unguarded-stripe kernels with no reduction
  alignment clause - B in [2,8] on a model whose dim/qd/hidden is % 128 != 0 (576/896/1152
  class) misreads; its fix wants a failing test first, own arc; (5) the b2/b4 y binding
  carries no @span, so each peeled site pays a conservative whole-buffer barrier -
  correctness-neutral, but it confounds per-r timing comparisons.**

- **LANDED - the Metal q8 decode GEMV family ran one-row-per-simdgroup with a full-n walk,
  spilling the x vector past L1 from n=4096; the reduction-split form takes every q8 model
  (measured 2026-08-24, M1 Max, the decode-tg chase).** The dig: qwen3vl text cells were
  never on the ladder - fresh pairs showed tg -9.6% (8B) / -11% (4B) vs upstream with pp at
  parity; the per-step trace (`benchmarks/decode_step_trace.das`, -p 0) put 99% of the step
  in GPU execution (gap 0.5 ms, sched 0.1 ms); the knockout ladder (the same trace under
  `DASLLAMA_METAL_DECODE_SKIP` arms) attributed 86% of the step to the GEMV class at 309-334
  GB/s achieved vs upstream's ~352 effective; the GEMV lab
  (`benchmarks/matmul/bench_metal_gemv_kernels.das`) at new 8B shapes
  showed the production form dominated at every shape, small-n included - the old "at the
  wall" verdict had measured the form's own ceiling (the per-shape rates settle in the lab's
  own report, per benchmarks/REVIEW.md). Mechanism: one row per simdgroup walks the FULL
  reduction, so the shared x stream (n x 4B) spills L1 at n>=4096 and every weight load
  pairs with a stalled x load; the fix splits the reduction across the tg's 4 simdgroups
  (n/4 window per sg, L1-resident at every decode dim) and shares each x load across 2 rows,
  reducing cross-sg via tgmem + two barriers. Swapped: `MetalQ8Gemv` (wo, dense w2, logits,
  ASR-decoder sites), the fused `qkv_rs16/rs32` (one tg per rope pair) and `w13sw16` (one tg
  per hidden row); `MetalDnBa` (d = dt_rank 16..64) keeps the old shape for occupancy.
  Results (tg, das vs upstream; the released `lcpp_bench` exe, r=3, quiet M1 Max): 8B
  36.4 -> 39.3 (-2.3%, was -9.6%), 4B 62.3 -> 65.4, E2B
  96.2 -> 98.8 (+15% lead), E4B 52.8 -> 55.3 (tie -> +4.3% ahead), gemma-3-4b 66.2, Omni-30B
  71.2; pp cells unchanged everywhere. NOTE the fused-kernel ports measured near-zero on the
  8B (qwen3 never dispatches qkv_rs - qk_norm disqualifies fused_qkv - and w13sw's two weight
  streams already amortized x); they pay on the families that do dispatch them. **OPEN
  followups: (1) the fixed per-step submission overhead (~0.5 ms GPU-idle gap + ~0.3 ms
  encode) is now the WHOLE remaining deficit - it is why qwen-4B/gemma-3-4b sit at -6..-7%
  and the 8B at -2.3% (encode-ahead is the lever); (2) residual kernel headroom - the qkv
  class runs below the form's isolated lab rate in situ (knockout ladder vs the lab's own
  report).** E2B attribution for the ladder
  lead: the non-GEMV half (fused chains) costs das ~2.7 ms/token vs upstream's ~5 - the win is
  op-fusion overhead on the machinery-heavy E-series, not GEMM speed.

- **MOSTLY LANDED (was OPEN, narrowed by slice J) - the first Metal submission after a
  CPU-only window repaid the kernel-side driver window; the residency-set HEARTBEAT removes
  ~85% of it (measured 2026-08-23, M1 Max; probe = `harness/residency_ramp_probe.das`, the
  qwen3v Metal tower encode after controlled CPU burns, submission split via
  `das_metal_boost`'s `metal_submit_trace`; plan: `qwen3vl_plan.md` slices M/J).**
  The four-timestamp split localized the whole ramp to drv = kernelStart->kernelEnd (queue
  and GPU execution FLAT): warm 0.11 ms, 200-1000 ms burn ~1.2 ms, 3000 ms burn 17.7 ms.
  The long-window mechanism is the OS collecting a committed+requested `MTLResidencySet`
  during inactivity - the cure is a background thread re-calling `requestResidency` every
  5 ms: `metal_residency_set_heartbeat` in dasMetal (5 ms cadence, keep-alive countdown),
  kicked from `residency_flush` per served step, knob `DASLLAMA_METAL_HEARTBEAT_S` (default 180 s,
  0 = A/B rail). Probe ladder at the 3000 ms burn (drv ms, x2 each): baseline 17.7/17.8,
  heartbeat 2.95/3.01, whole-map anchor buffer 18.5/18.1, heartbeat+anchor 2.94/3.00 - the
  heartbeat takes the win alone. REFUTED by that ladder: the whole-map no-copy anchor (one
  pinned unbound buffer over the full mapping - page pre-wiring changes NOTHING in either
  arm, so neither the collect nor the residual is host-page work) and the previously
  proposed **per-buffer tracking audit** (resource count is not the lever; DEMOTED).
  Earlier refutations stand: driver wake (empty cb 0.107 ms), per-page faults, light
  pulse-trains, denormals, affinity, cb-split counts, queue identity; prefill pools stay
  tracked (untracked reds `test_metal_prefill_parity`). **OPEN residual:** ~1-3 ms drv on
  the first submission after a multi-hundred-ms window, scaling with window length,
  insensitive to residency cadence AND page pinning - the driver/GPU idle-state wake class;
  no user-space lever found (pulse trains already refuted), live with it. **Post-merge
  bench confirmation (2026-08-24, fresh rig at the merged master):** the heartbeat also
  removed the ~40 ms commit->execution slack slice J could only name inside the image turn
  (timed prefill 652 -> 612 ms) - qwen img:pp gaps halved, 4B/8B -16% -> -7%, 30B -4%
  unchanged, encode flat; the encode->prefill hand-off IS a CPU-only window.

- **OPEN - arming Metal makes the CPU q8 tower encode ~1.7x slower (measured 2026-08-23,
  M1 Max, 8B qwen3v tower: 3.26-3.45 s with MetalMode.required vs 1.90 s on the CPU leg,
  same t=8; probe = a quiet `-jit` run with the rig's tune manifest, leg = the released
  `lcpp_bench` exe).** Reproduces in the quiet probe, so it is not the bench harness. Mechanism
  unnamed - worker placement / QoS when the Metal queue is live is the suspect class.
  Costs every Metal-leg image cell ~1.4-1.6 s of encode today; the Metal tower deletes the
  arm entirely, but the mechanism matters wherever a CPU encoder coexists with Metal serving.

- **RULED AND LANDED - the spliced image prefill trailed upstream on Metal; the fused span
  eval closed it (measured 2026-08-21, M1 Max, the fused-image-span arc).** The record-grade
  gap was das/upstream **0.52-0.62x** `img:pp` across all three vision models (E2B 687 vs 1101,
  E4B 362 vs 658, 12B 140 vs 268 tok/s) while das led every CPU prefill, encode and decode
  cell. Two hypotheses refuted by per-slice probes (`eval_embd_slice_` timing +
  `metal_prefill_stage_stats`): NOT a CPU fallback (all quanta served on Metal, blob models
  exempt the `min_npos` floor, zero declines) and NOT submit/readback (gpu ~ wall, CPU-side
  ~ 4 ms/turn). Also refuted: "upstream runs one graph over the whole span" - it issues the
  SAME three decodes (one per chunk, causal-flag toggled around the image
  chunk). The measured mechanism is the **pad-64 GEMM floor**: the mul_mm M tile pads every
  quantum to 64 rows (`mp = ((npos+63)/64)*64`), so the ~5-token head and ~21-token tail each
  billed a full 64-row GEMM pass through the stack - E2B slices 58.7 / 123.2 / 54.6 ms and
  head+tail 48 % of the wall for 17 % of the positions, measured by a temporary -jit per-slice
  probe (a `ref_time_ticks` wrap in `eval_embd_slice_` + `metal_prefill_stage_stats`, both
  reverted; `harness/prefill_msweep_probe.das` is the kept instrument), while the rows slice
  alone already ran at the upstream whole-span rate. **The fix:** `eval_embd_span_` issues ONE eval with a per-query
  mask - span rows [ulo, uend) uniform, head/tail causal - through the CPU flash/blocked/
  classic arms and `AttnArgs.ulo` on Metal (DASLLAMA_VERSION 7); the three-eval splice remains
  the vulkan-leg fallback and the `set_span_fuse(false)` parity rail. Record-grade after the
  arc's second lever (the M-pad fix below): Metal `img:pp` E2B **1400.2** vs 1102.6 (1.27x),
  E4B **713.7** vs 663.1 (1.08x), 12B **268.0** vs 270.7 (parity within noise) - das leads or
  ties every Metal image cell, with the decode/encode crowns re-confirmed.
- **RULED AND LANDED - the "kquant small-M kernel deficit" was the 64-row M pad (measured and
  fixed 2026-08-21, M1 Max, the fused-image-span arc round 2).** The suspected ~27 % kq
  mul_mm throughput deficit at image-turn batch sizes was REFUTED by the M-sweep
  (`harness/prefill_msweep_probe.das`, 12B Q4_K_M metal leg): the ms/npos curve is a pure
  64-boundary STAIRCASE - npos=160 cost exactly npos=192's wall (604.6 vs 606.0 ms), 96 cost
  128's - while per-PADDED-token cost is flat 3.1-3.2 ms from 128 to 512; the knockouts put
  the GEMM family at 96 % of the wall at both M. Every prefill kernel's M grid is `mp/32`
  (weight mul_mm, kq twins, attention trio), so the `mp = ceil64(npos)` pad billed a dead
  32-row GEMM block on every short prefill; the fix is `ceil32`. Record-grade image cells
  after (released rig, refs adjacent): 12B **268.0** tok/s (upstream 270.7 - parity within
  noise), E4B **713.7** (663.1), E2B **1400.2** (1102.6) - das leads or ties every Metal
  image-prefill cell. The pad cut also reaches every Metal-served model's short prefill
  (`npos % 64` in [1, 32] - the interactive-prompt shape; pp512 cells sit at % 64 == 0, which
  is why the pad went unnoticed). Full prefill-parity arm set + the gemma2 sliding-window +
  gemma4e/qwen35 family rows green on the new pad.

- **gemma4v ViT tower (E-series): the encode is the image turn's biggest single stage on every
  CPU tier; the q8 lane and the Metal leg both landed (measured 2026-08-19, M1 Max, the gemma4v
  arc).** The tower is 16 blocks x 7 GEMMs (112 bf16 GEMMs, ~151 MMAC per patch) over 1170
  patches for a 640x480 photo (-> 130 soft tokens). Record-grade, from the released `lcpp_bench`
  exe's image cell (gemma-4-E2B Q8_0 off the image rail, `-r 5`): the **CPU row** (the q8 block-
  GEMM lane, `flavor tuned`) `img:enc` **419 ms**, `img:pp` 366 tok/s over the 155-token spliced
  prompt (0.42 s), `img:tg` 41.6 tok/s - the encode is ~ 40 % of a 32-token turn; the
  **`--accel` row** (the +AMX float-batch tier on the file's planes, `flavor accel`) `img:enc`
  **459 ms**, `img:pp` 369, `img:tg` 42.6. Stage-level, from the `g4v.*` `asr_prof_report`
  buckets of a `-jit` probe (`gemma4v_encode` on a 624x480 canvas, `DASLLAMA_CPU_PREFILL=1`,
  three reps, direction only): the exact-plane CPU lane through `matmul_bf16_batch`'s per-row
  `dot_bf16` loop costs ~ 1.9 s per encode (GEMMs ~ 90 %, ~ 92 GMAC/s aggregate - ~10x under
  the q8xq8 prefill kernels), the q8 lane ~ 0.43 s (GEMMs 238 ms, the attention core 163 ms -
  now the #2 bucket), and the Metal block loop ~ 89 ms with the stem (10 ms) and tail (2 ms) on
  the CPU. Prediction P2 (0.9-1.8 s, >= 12 % of the turn, for the exact CPU lane): the share held
  by a wide margin, the absolute landed just above the band. **Ruled and landed in the arc:** the
  q8 tower lane (the gemma4a / parakeet recipe - Q8_0 GEMM planes + per-row requant) is the CPU
  serving default, the Metal leg (`register_gemma4v_gpu`) serves the file's planes on Apple
  builds, and the lane follows the serving path (Metal tower or the accelerate tier -> exact
  planes; else q8). Left on the table: the CPU attention core (163 ms of the q8 lane's 430), the
  per-row `pf_enc_rms` at head width (256-thread groups over 64 elements, ~14k tiny dispatches
  per block pair), and the E2B decoder's own Metal prefill decline ("layers", `required` mode,
  independent of the image span) - until that is served, the gemma4v Metal leg is the only GPU
  piece of an E2B image turn and the `--ngl` image row cannot run.
- **gemma4uv vision embedder: no quantized lane; the dlim rail is owed anyway (measured
  2026-08-14, M1 Max, the vision arc's slice G).** The embedder is two GEMMs (6912->3840 patch
  projection, 3840x3840 output projection). In a product-shaped turn - gemma-4-12B Q4_K_M
  served q8 off the image rail, a 640x480 photo -> 130 soft tokens - the whole image side
  (geometry + letterbox + encode) is **under one percent of the turn** against the
  time-to-first-token and decode the cell reproduces (`img:enc` vs `img:pp`/`img:tg`). Encode alone is ~54 ms at 130 tokens
  (`img:enc`, the cell) and scales with the row count, so the 280-row geometry ceiling bounds
  what any image can cost - re-measure via the cell with a max-geometry input. **A quantized plane pair is DECLINED:** a second
  plane format cannot be worth it against 0.7%.
  [!] **Measurement provenance, because the first pass got it wrong twice:** the original
  numbers came from a one-off script that (1) took `load_model_`'s `QuantMode.fp32` default,
  dequantizing a K-quant 12B and reading 2 t/s instead of 14 - a 7x error - and (2) skipped
  `tune_gate()`, worth a further ~9%. Both defaults are stated automatically by the profiling
  rig, which is why model-level timing belongs there and not in a bench somebody writes. **The `.dlim`
  rail is NOT declined, and the timing above is the wrong instrument for it** - the rail is an
  ownership question, not a speed one. Every other model artifact loads mapped, with no
  allocation and no processing at load; the embedder's `blob : array<float>` is the second,
  array-owning shape `dasllama_audio.das` explicitly refuses to maintain. What it buys is RSS
  (mapped pages are shared and evictable across processes; a 190 MB owned bf16->f32 widening is
  neither) and zero load-time work - not the load wall. **Both are DONE (2026-08-14, the same arc's slices I and J).** The rail
  landed: the embedder stages, mints and maps like the audio towers, and the prepared image maps
  with no load-time work (mmap). Plane format settled as **per-GEMM, following each source tensor's on-disk type** -
  and the premise that the mmproj is uniformly bf16 was HALF WRONG: gemma-4's shipped "BF16"
  mmproj carries an **F32** patch embedder (26.5M params) and a **BF16** projection (14.7M), so a
  both-or-neither rule bought nothing. Per-GEMM gives 200 MB -> 170 MB with tier-1 unmoved to the
  ulp (2.098e-05 before and after) - the bit-for-bit claim about `dot_bf16`'s widen, confirmed.
  Rounding the F32 half down would cost ~0.2% relative, two orders over the tier-1 gate, so it
  stays fp32. The turn is also a rig cell now (`lcpp_bench --image`: `img:enc` 53.8 ms,
  `img:pp` 63.5 tok/s, `img:tg` 14.3 tok/s on the M1 Max), so these numbers are re-measurable
  rather than quoted. **What reopens the quantized half:** a family whose vision side is a real
  ViT (the E-series `gemma4v` tower - 16 attention blocks, not two GEMMs), or batch image serving,
  where per-image cost stops hiding behind one prefill.

- **CPU attention: promote it from the loop-crown tier to the EMITTED tier (named 2026-08-05,
  the E2B deep-clip fade probes; corrected same day after a double-check).** Attention is NOT
  kernel-less: flash prefill (the default mode) tiles with online softmax and rides the
  per-box-tuned `gemm_f32_uk_4x16` for QK^T and A*V, with full f16/q8/tq4 KV codec arms.
  What it lacks is the EMITTED tier the weight GEMMs got: with f16 KV the flash path
  packs-and-converts K/V to f32 first and then runs the fp32 GEMM - no f16-native dots
  (F16C / fp16 hw-convert loads fused into the tile), no int8 QK option, softmax outside
  the tile. The measured tier gap IS the finding: on a gb1-class ~5k-token ASR prefill the
  emitted-tier blk_ffn crosses zen2/M1 at 1.13x while the loop-crown attn crosses at 2.9x
  (16.4s vs 5.6s); the f16-KV default bought only -3% (read bytes halve, the fp32 compute
  stays). It reads fine at pp512 (small share) and hurts at depth - deep prompts and batch
  serving pay the same tax audio does, text board included. **What a fix looks like:** an
  emitted, per-box-tuned attention family - convert-in-tile f16 dots, optional int8 QK
  (requant Q once, vnni/sdot straight against q8 KV - q8 flips from slowest cache format to
  likely fastest), softmax fused into the flash walk. **Why it's a WIN, not catch-up (Boris
  2026-08-05): upstream runs the same dequant-to-f32-scratch seam - an emitted tile is
  'potentially faster' than the ref, and the deep-clip audio cards on the site are the
  ready-made scoreboard.** NOT the quant-lane arc; a kernel arc of its own, eventually.

- **Batched-tg bench row: tg at B=16 sequences is the kernel story decode can't tell at B=1
  (spotted 2026-08-04, zen2 thread ladder).** Single-sequence decode is GEMV - bandwidth-bound
  at any thread count, so das:ref tg reads as parity (t=4 zen2: 13.10 vs 12.59) while pp shows
  +45% on the same box. At B=16 every decode step is a 16-row GEMM: arithmetic intensity x16,
  weight stream amortized across the batch - the same regime pp wins in, and the serving shape
  dasllama.io actually runs. The engine side EXISTS (`eval_batch_` in `dasllama_batch.das`,
  synchronous B-row step; Metal has `metal_batch_decode_forward`); what's missing is only the
  bench row: a `--tg-batch N` mode in `lcpp_bench.das` driving B sessions per step, ref via
  `llama-batched-bench` (separate binary from `llama-bench` - check the ref checkouts build it).
  **What a fix changes:** a board column where the kernel advantage shows in decode, and the
  first honest throughput-serving number for the server story.

- **Decode work-splitting vs thread count: tg on the 3990X ladder read flat 13.1-13.3 at t=4-8
  then jumped +59% to 21.2 at t=12 (2026-08-04).** Read: the per-op split count is mismatched to
  the lane count at some t (undersplit or oversplit, depending), with CCD-spread-vs-pack pin
  placement a secondary suspect (unpinned llama-bench led das tg at t=8, 14.1 vs 13.3). Cost
  today: optics only - single-sequence tg absolutes don't drive engine choices. Why it stays
  ledgered: batched decode (see the batched-tg entry) turns each step into a B-row GEMM where the
  same split/placement question prices real serving throughput; revisit there, not as a B=1 hunt.

- **`BatchLanding.cbs` per-step buffer churn: the `@scratch` recycling never happens on the batch-decode
  pipeline path (spotted 2026-08-02, lint uplift).** `batch_step_build` builds each
  step's command-buffer list in a LOCAL `cbs` and hands it over with `L.cbs <- cbs`
  (`dasllama_metal_decode.das`, the landing stash at the end of the build) - the move drops the slot's retained buffer every step,
  so `finish_pending_step`'s capacity-retaining `clear()` recycles nothing, and on a persistent
  heap the dropped buffer is ~ncb pointers per step. **What a fix changes:** prefill's
  `g_pf_cbs` ping-pong pattern (build into the global, or `push_from` a reference) recycles both
  buffers, deletes the per-step allocation, and makes the PERF026/PERF030 nolints at that site
  unnecessary. Cost today is small (one small alloc + drop per step); the reason to do it is the
  allocation sits on the measured hot orchestrator, so it belongs to a perf pass with A/B cells,
  not a lint sweep.

- **ASR peak-memory census (2026-08-03, m1, /usr/bin/time -l around asr_bench - the
  reproducible ladder is `benchmarks/asr/mem_census.sh`): canary warm footprint
  jfk 2.71 / gb1 4.29 / hp0x2 8.35 GB; canary MINT gb1 4.62 (+0.33 over warm);
  parakeet-v3 warm hp0x2 5.61 GB.** The canary-apples arc closed the 2026-08-02 baseline's
  two findings: the 13.1 GB canary serve peak (fp32 weights + quadratic scratch) is now
  ~4.3 GB on gb1 (q8 end-to-end + per-job attention slabs), the mint spike is +0.3 GB
  (read-time tensor-at-a-time transcode), and hp0x2-length canary - previously one clip from
  swap on a 16 GB box - runs at 8.35 GB. Parakeet's hp0x2 quadratic scratch died with the
  same slab change (-0.6 GB vs its pre-slab control). REMAINING: canary's hp0x2 tail is
  ~4 GB of decoder-side prefill/KV at ~7k soft tokens - map it (KV codec residency, prefill
  panel sizing) before chasing; and whisper's mint spike (1.48/5.29) predates this arc's
  machinery, already ledgered via the streamed mint.

- **Try the borrowed plane WITHOUT its bounds check, at profiling time (spotted 2026-08-01, audio
  `.dlim` arc).** `PlaneF`/`PlaneI8` `operator []` in `dasllama_plane.das` guards every read with
  one unsigned compare. **Costs nothing today, measured:** M1, `-jit`, cache-resident
  4-accumulator sum, 67.1 M elements, 7 interleaved reps - raw pointer 15.608 ms, checked plane
  15.611 ms, and the `array<float>` indexing it replaces 20.856 ms (cv <= 0.6% on every arm). So
  the check ships. **What the removal would change:** in that microbenchmark, nothing - LLVM
  folds the compare out of a loop whose bound it can prove. Real audio kernels index with computed
  offsets (`blob[woff + i]`, layer strides, conv taps) where the bound is not provable, so the
  compare may survive there and the microbenchmark does not answer it. The A/B to run once the
  audio carriers are converted: an `unsafe`/unchecked accessor variant behind a build knob, A/B'd
  on real encode/decode cells per family, interleaved, not on a synthetic sum. **Do not remove it
  on the strength of the number above** - that number measures a loop the real kernels are not.

- **gemma-4-26B-A4B tg 0.84x - routed k4 GEMV was load-issue-bound; float4-x fix SHIPPED, +8.7% tg
  (2026-07-23).** Chapter 1, knockout (new moe_rt/moe_sh arms, decode_metal_chase nomoert/nomoesh,
  Q4_K_M @512, best-of-3, clean window): full 18.80ms; routed k4/q51 expert GEMVs 5.48ms (29%) at
  ~163 GB/s; other GEMVs (QKV/WO/router) 5.30ms; dense-shared 1.89ms; attention 1.93ms; ew 1.96ms;
  non-gemv floor 6.13ms. Classifier ~ ZERO time despite 21% byte share - the greedy spec chain
  covers it (1342 hits / 3 misses). Chapter 2, kernel-lab (benchmarks/matmul/bench_metal_moe_lab
  - exact 26B expert shapes E=128 nfe=704 dim=2816 k=8, exact-arith bit-exact contract, sel
  rotation for DRAM honesty): the barrier/bubble theory REFUTED (pipelined-no-barriers ==
  in-graph rate, +/-7%; scatter-vs-contiguous sel: nil) - the production MoeGemvK4 kernel itself
  ran **142 wGB/s** at expert shapes, load-issue-bound on 32 scalar x loads per 256-block
  (the AGX checklist's vector-width-views item, missing in the MoE twins). float4-x view =
  **321 wGB/s (2.25x)**; W13-pair fusion ties it (327) so NO driver surgery; 4-rows/sg partial
  (202). FIX SHIPPED in MetalMoeGemvK4 (kernel-only; dispatch/driver/blob unchanged): in-graph
  full 53.06 -> **57.65 t/s (+8.7%)**, gpu 18.80 -> 17.31ms/step; kernel units + fam tolerance
  cells green. Chapter 3, the "other gemv" anomaly RESOLVED as a phantom (finer noqkv/nowo
  knockout arms, post-fix @512, full 17.27ms): QKV 2.56ms is AT ceiling - the 26B is HETERO
  (global heads 512-wide: wq 8192x2816 on global layers), so real QKV bytes are ~857MB/step,
  ideal 2.60ms; the old "3x" was a byte-estimate error. WO 1.52ms vs 1.30 ideal (~85%, ~0.2ms
  slack). The residual gemv-class remainder 1.23ms = router (~0.15) + the greedy spec chain's
  classifier dispatches (~1.1ms, gemv-gated, useful work). Routed experts post-fix: 3.84ms.
  Remaining quantified levers, by size: (1) **the dispatch model - MEASURED from both sides
  (2026-07-23 evening, same-window)**: das encodes **780 dispatches/step @8 / 811 @512** (new
  metal_dispatch_call_count instrument), every one implicitly barriered on the serial encoder.
  upstream's own knobs price the structure on this exact model (tg128, +/-0.4% reps): stock 58.50
  t/s; GGML_METAL_CONCURRENCY_DISABLE **52.17 (+2.08ms/tok)**; FUSION_DISABLE 53.90 (+1.46);
  GRAPH_OPTIMIZE_DISABLE 53.96 (+1.44) - upstream-with-serial-encoder lands in OUR class, i.e. no
  kernel section loses; the whole gap is dispatch structure. Same-window das: chase full @8
  **62.3 t/s (16.05ms) - AHEAD of upstream stock 58.50** under real greedy decode; the llama-bench-
  protocol rail reads 55.75 +/-3.20 (synthetic-id feed perturbs the spec chain + the rail's tg is
  intrinsically noisy - the recorded 50.31 +/-2.2 board cell is protocol-dragged and/or tinted).
  The blueprint: a concurrent encoder +
  per-op src/dst range tracking - barrier + reset only when a new op's ranges conflict with the
  live set - plus ew-chain fusion and a graph-reorder pass that grows concurrent sets. Our shape:
  an enc_dispatch wrapper in dasllama_metal_common taking declared read/write (buffer, off, len)
  ranges per dispatch (every enc_* helper already knows its buffers), auto-barriering on conflict;
  g_skip runs must stay serial (the knockout-unbarriered caveat at dasllama_metal_decode.das:2511).
  **SESSION 1 SHIPPED (2026-07-23): the hazard-tracked concurrent encoder is in** - range tracker
  in dasllama_metal_common (exact (buffer, off, len) ranges, mutable frame buffers only; weights/
  uniforms untracked), hz_gate in every decode-path enc_* helper, undeclared dispatches take a
  conservative barrier (or PANIC under DASLLAMA_METAL_HAZARD_STRICT - the parity suite's arm13
  runs strict, so declaration coverage is test-enforced), a dispatch-counter check makes a gate
  bypass loud, DASLLAMA_METAL_HAZARD_PARANOID isolates the encoder flip. Measured on the 26B
  Q4_K_M chase (full arm, best of 3): @8 63.08 -> 65.78 t/s (+4.3%), gpu 15.81 -> 15.11ms; @512
  58.42 -> 60.84 t/s (+4.1%), gpu 17.07 -> 16.40ms; barriers 780/811 implicit -> 634/665 real.
  DEFAULT flipped ON after the family gates (llama arm13 strict, arm7 q8/tq4, gemma4moe both
  rows, qwen35/qwen35moe, gptoss, gemma4e - all green under CONCURRENT+STRICT); 12B Q4_K_M spot
  pair confirms: @8 35.05 -> 36.31 t/s (+3.6%, gpu -0.83ms), @512 32.37 -> 33.34 (+3.0%, -0.88ms).
  The barrier floor is the PROGRAM-ORDER chain depth (~19 genuine links/layer on the g4 graph:
  qkv triple, we1||we3, pre_ffn2||router_norm, post_ffn2||shared-w13sw group - everything else
  chains). **SESSION 2 PHASES A-C SHIPPED (2026-07-23 night): step-graph capture (kn_* twins
  record KNodes; graph_flush replays with the hz oracle deriving barriers, so a schedule can
  only change speed) + a shape-class schedule cache (FNV over the pso sequence, compiled once)
  with ASAP leveling over the conflict DAG + the g4 dense-shared/qwen35moe-shexp scratch split
  (own bh12s panel).** Measured 26B: @8 69.23 t/s gpu 14.40ms, @512 62.90 gpu 15.87 - cumulative
  vs serial @8 +9.7%/-1.41ms (das now well ahead of upstream stock 58.50 same-window); barriers
  634->574 (the split's 2 levels/layer; leveling found nothing else - depth 574 IS the true
  chain). 12B (dense, no branch): unchanged, as expected. 26B/qwen35moe tolerance cells
  byte-identical maxd under STRICT; llama arm1/7/13 token-exact.
  **R1 ew-FUSION MEASURED (2026-07-23): one kernel ships, one refuted - the barrier lever has a
  sub-1% ceiling on the MoE-bound 26B.** MetalPreAddRms collapses the pre_post_norm epilogue
  post_attn_rms + add_rms into one dispatch (enc_pre_add_rms_site: two register reductions, x
  staged for the trailing norm; bit-identical to the pair - reduction structure copied from
  metal_rmsnorm/add_rms, the one reordered product is IEEE-multiply-commutative). DEFAULT ON via
  DASLLAMA_METAL_FUSE; chase `nofuse` A/B arm. Correctness: gemma3 token-exact (pre_post_norm
  fires it), gemma4moe-26b tolerance maxd byte-identical to baseline. 26B Q4_K_M 3-arm interleaved
  chase (full/fuse1/nofuse - Parsec-active, read the DELTA): **fuse1 vs nofuse +0.35% @8 / +0.37%
  @512, gpu -0.05ms, barriers 574->544 (-30 = one collapsed level/layer x 30 layers)** - small but
  consistent + reusable capability (larger on dense gemma: shorter chain, 30 barriers a bigger
  fraction). The SECOND fusion, MetalRmsAdd on the g4 dense-shared post_ffn1_rms + add, was
  authored, correctness-proven, and **REFUTED** by the same run: **-1.0% at both depths and
  DETERMINISTICALLY 0 barrier reduction** (720 vs 750 disp but 544 = 544 bar) - that tail is on the
  SHARED branch, which the leveler already overlaps with the longer routed branch, so fusing rms
  (inputs ready early) with the add (needs bmoe_rt, ready late) SERIALIZES the reduction behind the
  routed branch it used to overlap. Reverted; kept as this ledger's negative control. **LESSON:
  barrier COUNT isn't the whole story - a fusion OFF the critical path can't help and can hurt by
  moving a bigger node into a later, tighter level; and the 26B's barrier lever is bounded (~sub-1%)
  because the GPU is routed-GEMV-bandwidth-bound (round-1 dig), not barrier-bound.** Implication
  for the remaining rungs R2-R5: the [metal_dispatch] macro lens stays worth it as CAPABILITY/eDSL, but the
  fusion+reorder PERF upside on this MoE model is small - the candidate deeper fusions
  (router_norm+router, swiglu+we2) hit the same norm-into-GEMV grid-wide-dep wall or need the GEMV
  kernel itself to fuse the activation (ledger-class). Still open from the plan: [tune]
  schedule axes (thin until fusion adds real choices), batch-rail unification (R4), dasMetal
  promotion of the graph/lens machinery;
  (2) the q51 w2 MoE kernel at 224 wGB/s in-lab -
  the integer-compose form (q | hbit<<4 pre-convert, replacing the select chain) TESTED + REFUTED
  2026-07-23: 226 vs 224 wGB/s (+1%), bit-exact but the dot stays issue-bound in the shift/mask
  decode regardless of compose shape; kept as the lab's w2_ic negative control, do not re-chase -
  a real q51 win needs a different decode strategy (per-thread multi-block amortization or an
  upload-time qh transpose, both ledger-class); (3) WO ~0.2ms slack; (4) MetalMoeGemvK5 carries the same
  scalar-x block - mechanical sibling of the shipped fix, prove via a lab arm first. Lab kept as
  the standing rig: variants stay as arms/negative controls.

- **[metal_dispatch] lens ROLLED OUT across the decode path (2026-07-23): 34 kernels generate
  their dispatch builders from the class; @role ratchet live.** The structure macro
  (dasllama_metal_lens.das) reads a [metal_kernel] class's @ssbo/@uniform fields and generates
  the enc_* builder - binds in @binding order, hz from @role (read/write/readwrite; weight =
  untracked static upload; alias = second view of a dual-bound buffer), @off = caller-passed
  bind offset (param interleaved after its buffer; shared names emit once - dual views ride
  two fields sharing one @off), @span = exact hz byte length (product micro-grammar), grid =
  per-WorkGroupID-dim `1|param|param/coverage` (literal coverage folds the ceil bias to the
  hand-written spelling; named coverage like g_gemv_rows keeps the +cov-1 form), tg = int
  const | "param" | "name*int". Every conversion was proven ast_dump byte-identical against
  its hand-written twin before deletion (only sanctioned diffs: dn_scan's exact-div grid
  emitted as value-identical ceil; order-free hz/bind permutations). Coverage: the simple
  single-PSO set + moe routing chain + deltanet chain + embed/gemv/w13sw/qkv_rs cores + the
  whole attn and rope-store families (14 cores) - variation points stay thin hand-written
  wrappers (fmt dispatchers kq_*/moe_gemv*, pso-parameterized ew2, dummy binds, f16 picks);
  batch rail untouched (R4). Net ~ -690 lines of hand-maintained bind/hz code. RATCHET: a
  role-less @ssbo on a lensed class is now a COMPILE ERROR (the flip immediately caught
  RopeStoreQ8's undeclared bias plane); un-lensed classes stay covered by runtime STRICT.
  Gates: decode-parity suite green under CONCURRENT+STRICT (arm13 coverage proof); six
  fam matrix gates green - 26B q8/k4 tolerance cells BYTE-IDENTICAL to the pre-refactor
  baseline (2.498179/5.1424093, 2.5177252/13.825883), gptoss/qwen35/gemma2/gemma4/gemma4e
  clean; the only reds are the tracked pre-existing metal object leak (30/36/15 by filter,
  unchanged counts). `range` is a lexer type token - the hz-length tag is @span, not @range.

- **.dlim mint abort past ~11GB: root-caused as DISK-FULL, writer fixed, red cleared (2026-07-23).**
  Not an int-width bug - the write rail is 64-bit clean end-to-end (ftello, long_fwrite -> size_t
  fwrite, long_length, uint64 offsets; the Jul-21 29GB mint of this same model and the image
  suite's 5.4GB voxtral arm are standing proof). The disk had filled: 804 GiB of .dlim images
  across ~27 identity generations back to Jul 16 - nothing ever deletes a superseded image, and
  the Jul-22 sweep's ~92GB of MTP mints ate the last headroom. ENOSPC then surfaced as the
  per-plane "offset-accounting" cascade (the quoted `0xf0` was the nbytes field - 240 bytes, the
  30-layer offs plane), and the abort path removed the tmp, hiding the space pressure. Fixed in
  the writer: `write_plane` now reports short writes honestly ("disk full?"), bails the walk on
  the first failure instead of pushing the remaining planes at a full disk, and `save_image`
  verifies the on-disk byte count after close (a buffered small-tail ENOSPC at fclose was
  silent). Swept the 115 dead pre-v5 images (659 GiB; every pre-Jul-22-13:18 file predates the
  IMAGE_VERSION=5 bump and is unloadable by construction); fam-gemma4moe re-mint verified green
  (both 26B rows' engage + tolerance cells pass; the arm's remaining red is the tracked
  pre-existing metal-object leak, 30 objects on this filter). OPEN (design, propose-first):
  .dlim GC - every IMAGE_VERSION/knob change silently orphans the previous generation (~20GB per
  big model). Candidate shapes: fold the tag into the filename and keep-one-per-(gguf, tag) on
  successful save; or utime-touch images on load and age out cold siblings at save time.

- **Q6-greedy spec-chain inversion on big pure-k6 files (2026-07-22 re-pair).** `spec_cls_capable`
  (dasllama_metal_decode.das:164) is a pure CAPABILITY test - it engages the greedy spec chain for
  any tied-k6 classifier with no BENEFICIAL condition, so a big pure-k6 file eats the spec-chain
  work where it is a net loss. Cost today: gemma4-12B Q6_K B=1 greedy runs 26.81 t/s with spec on
  vs 27.11 spec-off (~+1%; `DASLLAMA_METAL_SPEC=0` recovers). This was the board's "0.81x" cell -
  RE-PAIRED to 0.977 on / 0.988 off in a clean quiet window (the 0.81 was window-skew; spec-on had
  drifted 22.4->26.8). Fix: a size/format beneficial-gate (decline spec on pure-k6 above a
  param/layer threshold, picked at the 4B-wins / 12B-loses crossover) in spec_cls_capable or the
  :1888 engagement gate. Small inch (~+1%); deferred (Boris, 2026-07-22).

- **Small-model q8 single-stream GEMV vector-load: TESTED + REFUTED (2026-07-22), do not
  re-chase.** The hypothesis (the blob's +2 quant phase forces 4 scalar int8 loads instead of
  one byte4 vector load, costing ~7-8% at B=1) was IMPLEMENTED as Fix A (a uint16 blob view +
  2 aligned ushort loads + sign-extend unpack in MetalQ8Gemv), proven bit-exact on GPU, and
  measured in a clean A/B (distinct dll hashes): PERF-NEUTRAL-to-slightly-NEGATIVE. gemma3-1b
  B=1 master 192.1 vs Fix A 188.7 (the unpack ADDS ALU where the GEMV is not load-issue-bound);
  qwen3-4B B=1 63.40 vs 63.45 (noise). Root cause of the original "regression": the ledgered
  64.5->59.5 was WINDOW-SKEW, not a real issue-bound cost - the q8 B=1 GEMV is
  bandwidth-bound at the M1 ceiling (qwen3-4B 349 GB/s, gemma4-12B 327 GB/s, both near peak),
  so cutting weight-load issues does nothing. Contrast the gpt-oss expert-GEMV win, which cut
  X-RELOAD issues (a different axis). Reverted. The real q8 B=1 lever is the non-GEMV overhead
  (elementwise dispatch fusion + attention), not the weight-load path - see the ew-fusion
  ledger items below. Kept a byproduct: the gemv kernel-unit now covers the main vectorized
  loop (n=1120), which it never did before (committed).

- **Per-config .dlim: map-only load, BLOB-ONLY metal flavor - SHIPPED (2026-07-18).** The
  contract "no processing on load FOR THE CONFIG IT WAS BUILT FOR" holds: image v3 +
  METAL_IMAGE_TAG identity flavor; the 34B block_q8_0 blob REPLACES the planar q8 planes
  (one zero-copy MTLBuffer per plane via `metal_new_buffer_no_copy_untracked`, region byte
  offsets at bind - kernel indices stay uint32-safe); k4s/k5s ride the 16B strips and k6s
  the GPU split form; every q8 kernel is blob-addressed (the S16 scale twins collapsed),
  the fused QKV/W13 kernels bind per-segment views (kind-major layout), the fixed-B
  kernels grew the kq twins' `ys` y-row-stride uniform for fused-buffer writes, and the
  cat-blob caches + the `metal_blob_region` repack are DELETED. CPU inference on a blob
  model panics; the gathers (embed_row, dequant_q8_row, split-k6) read the blob directly;
  `load_model` picks the flavor via the registered `metal_model_servable` hook. Measured:
  1B transform 133ms / map 24ms; kq-pure transform 21ms / map 13ms; all three GPU paths
  serve with 0 declines. Rewiring residue for the ledger (re-measure the q8 cells first):
  the batch qkv site's split-K stands down (its reduce writes contiguous y - plain GemmB
  serves per-segment), the B<=4 unfused single-decode qkv/w13 cats became per-tensor
  dispatches (~2 extra dispatches/layer on those rails), and the legacy quantized-X
  prefill rail is DELETED (the `!mm` serving arms, the fused add+rms+quant/swiglu+quant/
  rope_qk kernels + PSOs, enc_gemm, and the X-quant pools - ~350 lines; the mulmm_legacy
  knob survives as the required-mode forced-decline test switch, and dasllama_metal_gemm's
  planar GEMM donor is untouched - it serves CPU-flavor models' batch offload).

- **QK-norm rope-store fusion - the f16 single-stream H-form SHIPPED (wave A chase round 2);
  the rest of the family is the residual (2026-07-17).** MetalRopeStoreHF16 folds bias +
  per-head RMS + rope + store into one threadgroup-per-head dispatch on QK-norm x f16-mirror
  single-stream decode (+~1% on the 4B board; Q6_K B=1 tied upstream exactly). Residual scope,
  build when a board shows the gap: the f32/q8_0/tq4 codec twins and the BATCH H twins
  (those paths keep the MetalQkNorm prepass + flat rope-store - batch amortizes the extra
  dispatch over B rows, so the gap is smaller there), and a norm-capable fused qkv_rs
  two-pass form for the s16 path (fused qkv_rs still stands down under qk_norm).

- **Gemma stage-1 Metal deferrals (wave B, 2026-07-17).** The gemma2 enablement chose
  correctness-first shapes; each entry names today's cost on gemma-class models only (llama/qwen
  paths untouched):
  (1) *GeGLU fused-w13 stand-down* - SINGLE-STREAM HALF SHIPPED (2026-07-22): the decode
  MetalQ8GemvW13Sw grew an `act` uniform (0 = silu, 1 = gelu using the CPU geglu4 tanh-via-exp
  identity), and the dense-FFN gate fires the fused path for `w13_q8` regardless of activation
  (gemma's geglu no longer stands it down), skipping the separate geglu ew pass - -2
  dispatches/layer. Measured: gemma4-12B Q8 B=1 20.98->22.31 (+6.3%, 0.85->0.90x), gemma3-4b
  0.92->0.95, gemma2-2b 0.93->0.96, gemma3-1b 1.22->1.24; parity green (fam-gemma3/gemma4 +
  arm1-basic swiglu regression). STILL OPEN: the batch fuse13 rail (b2/b4 PSOs) keeps the
  hardcoded silu epilogue, and prefill's legacy fused rail (`enc_swiglu_quant` has no gelu twin,
  and the deferred-W2-add trick has no post-ffn-norm slot, so `fused` stands down entirely -
  mm-path prefill, the default, is unaffected).
  (2) *pre_post_norm sites are composed, not fused* - each post-norm is a separate in-place rms
  dispatch before the residual add (+2 dispatches/layer on every path). Fix: a `post_add_rms`
  kernel (rms(branch)*w_post + x, then the next pre-norm in the same tg - the row is already
  staged for the add_rms reduction).
  (3) *Sliding chunked dispatch is not compacted* - the single-stream part dispatch still grids
  ALL context chunks; below-window chunks exit whole-tg on entry (the comb skips them via chlo).
  At gemma2's 4096 window this only bites past 4K depth; fix = dispatch chunks [chlo, nchunks)
  with a ch0 uniform. Batch shares the early-exit shape (per-row windows preclude one compact
  range).
  (4) *Spec-chain stands down on embed-scale models* - the greedy GPU-argmax chain requires
  `embed_scale == 1.0`, so every gemma (sqrt-dim scale) eats the CPU next-token poke per step.
  Fix: a scale uniform on the embed-gather kernels (enc_embed / enc_embed_k6). Revisit at the
  gemma4-12B B=1 board - this was the 4B chase's biggest single lever.
  (5) *Batch window-crossing parity has no dedicated test* - the fam-gemma2 masking row proves
  single-decode + prefill past the window; the batch part/comb twins share the masked-kernel
  code but no batch test drives depth > window. Add one when a batch harness with deep
  per-row contexts exists.
  (6) *Prefill V-from-K is two dispatches* (stage 3c, 2026-07-18) - the no-wv layers run a flat
  panel copy (MetalPfCopy) then the ones-plane MetalQkNorm in place (+2 dispatches on gemma4's
  8 global layers). Fix: a read-K-write-V weightless-RMS variant (the CPU fuses the copy into
  rms_batch). Suppress adds one tiny classifier-tail dispatch - not worth fusing.
  (7) *Batch mv + split-K rails stand down under hetero* (stage 3d, 2026-07-18) - the mul_mv
  x-staging strides (u_xs4_*) and the split-K totals (u_skt_*, and the sk sites' u_qd k-dims)
  bake ONE attention class, so hetero (gemma4) batch rides the cat-GEMV forms at B <= 4 and
  the planar GEMM without sk at B >= 5. Fix when the gemma4 batch ladder says it matters:
  per-class xs4/skt twins bound per layer, same shape as the attention uniform picks. The
  batch V-from-K copy is also nrows tiny enc_copy_row dispatches - a strided-seg copy kernel
  collapses them to one.

- **Embeddings path (spotted building `/v1/embeddings`, 2026-07-06).** Two small items, neither
  chased: (1) `embed_forward` takes approach A - reuse `forward_prefill` then re-norm every
  position - which pays **one wasted last-position classifier GEMM** (vocabxdim) per embed call,
  because `forward_prefill` always runs the final norm+classifier on the last token. Fix would be
  to split the transformer stack out of `forward_prefill_body` (a `forward_prefill_stack` helper)
  and call it directly, skipping the classifier; cost is one GEMM against a whole forward, so
  negligible until embeddings are hot. (2) The server's `/v1/embeddings` calls facade `embed`
  per input, which **creates + deletes a full KV-cache session per input** - for a batch of N
  strings that's N session allocations. Fix: a dedicated reused embed session in the server (the
  facade would need a session-taking `embed` overload, or the server reaches the public
  `embed_forward` primitive - but that breaks the facade-only invariant, so the overload is the
  clean path). Both are backlog; the server is serial and embeddings are low-frequency.
- **q8 GEMV loses to fp32 on cache-resident weights without VNNI (zen2 whisper decoder,
  2026-07-08).** Decoder-q8 stage A/B on zen2 tiny/jfk: logits GEMV 3.8x faster (76 MB -
  bandwidth-bound, the q8 win) but per-layer decoder GEMVs +7-21% SLOWER q8 (~2.3 MB mats,
  L2/L3-hot across the serial decode steps; AVX2 int8-dot + per-step requant loses to plain
  FMA when there's no memory traffic to save). Cost today: whisper-tiny zen2 decode leaves
  ~10-15 ms/clip on the table; net end-to-end neutral so nothing urgent. Possible fix: a
  per-region "stay-fp32 when the mat fits cache and the box lacks int8-dot silicon" load
  heuristic (or a box_profile knob like batch_grid_2d) - DON'T build until the M1 decoder-q8
  re-sweep says whether sdot boxes want q8 everywhere (likely yes -> knob would be x64-only).
- **Gemma-4 E-series DENSE prefill trails upstream Accelerate-BLAS on M1 (Wave G3 A/B, 2026-07-07).**
  E4B pp512 das **178.7** vs upstream **192.9 = 0.93x**; E2B pp512 das **376** vs upstream **382 = 0.98x**
  (E2B ties, E4B ~7% back). Decode tied both (bandwidth-bound: E4B 18.7/19.2, E2B 36.0/36.9).
  Root: dense prefill has no sparsity/grouping lever (unlike the MoE waves that LED upstream), so it's
  das NEON-SDOT vs upstream Accelerate-BLAS (AMX-backed) on the projection/FFN GEMMs - the larger E4B
  dim (2560 x ff 10240) favors AMX more, hence 0.93x vs E2B's 0.98x. Cost today: ~7% E4B prefill
  on M1 only; boxes without AMX exposure (zen2/SPR VNNI) already close it. Possible lever: a
  gemm-gen tuned Q8 kernel for the E-series shapes (the tune framework already exists) - don't
  build until an AMX-less box A/B says the shape actually leaves das-kernel headroom on the table.

- **Canary-Qwen ASR runs fp32 for parity; q8 decoder+encoder is the follow-up (Wave A1,
  2026-07-08).** The token-for-token gate loads the LoRA-merged Qwen3-1.7B decoder + FastConformer
  encoder at fp32. Perf A/B (M1 8T, das vs NeMo SALM greedy; `benchmarks/asr/results.md`, now git history): das
  LEADS every short/dictation clip 1.4-3x (jfk das/nemo 0.61x, LibriSpeech 0.34-0.49x) - the
  Canary-Qwen use case - but TRAILS 3.7x on the 3-min gb1, where the fp32 1.7B decoder is
  bandwidth-bound over gb1's ~2500 audio soft tokens. Fix: a q8 decoder (the existing q8 GEMV path,
  ~2x decode on the bandwidth rail) + q8 encoder, both straight ports of the parakeet/whisper q8
  machinery gated behind the fp32 parity default. Don't chase until the ASR-perf pass - but
  gb1-class long-audio is where it pays.

- **Gemma-4 E-series audio (gemma4a) encoder is fp32 SCALAR - big A/B gap (Wave A2, 2026-07-08).**
  The parity gate is fp32 encoder correctness, so the gemma4a Conformer runs a plain fp32 scalar
  forward. A/B (M1 Max 8T, das vs llama-mtmd-cli; `benchmarks/asr/results.md`, now git history): das transcribe
  6028 ms / xRT 2.89 vs mtmd-cli 1547 ms / xRT 11.3 -> **das TRAILS 3.9x**, dominated by the encoder:
  das encode 1888 ms vs mtmd 117 ms = **16x** (fp32 scalar Conformer vs upstream's bf16-weight SIMD
  GEMMs); long-context decode 21.7 vs 78 tok/s also lags. Unlike A1/parakeet/whisper (which lead or
  tie), this tower has had NO perf pass. Fix: route the gemma4a tower through the gemm-gen Q8 audio
  kernel (the same SIMD/threaded machinery parakeet/whisper towers already use) - likely the single
  biggest audio-side win on the shelf - plus the long-context decode path. Not chased mid-wave.

- **Qwen3-Omni AuT tower is fp32 scalar too; perf numbers are SOFT (Wave A3, 2026-07-08).**
  Same shape as A2: parity gate is fp32, so the shared qwen3a AuT encoder runs scalar. A/B (M1 Max
  8T, das vs llama-mtmd-cli): jfk das 3625 ms / xRT 3.03 vs mtmd 1173 / 9.4 = **das trails 3.09x**;
  jfk3 das 8263 / 3.99 vs mtmd 2079 / 15.9 = **3.97x**. Dominant gap = the fp32 scalar qwen3a tower
  (~4.8x encode), same lever as the gemma4a entry above - SIMD/Q8 the shared AuT/qwen3a encoder
  covers BOTH A2 and A3. The q8 MoE thinker (grouped prefill ~207 t/s + q8 decode) also trails upstream.
  [!] These A/B numbers are SOFT: measured with a dormant Parsec host daemon (1.6% CPU) + Spotlight
  indexing the freshly-downloaded 34 GB - a clean announced Parsec-off re-sweep would firm them (parity
  is unaffected). Not chased mid-wave.

- **ASR short-clip fixed costs (parakeet, M1 - NEXT ROUND, Boris 2026-07-06; whisper tower
  q8 postponed one session behind it).** Cost today at matched 8T: jfk das 703 ms vs cli 352
  (2.0x), LibriSpeech dictation p50 651 vs 324; long clips already 1.07-1.10x, so the short
  end is where the M1 gap lives. jfk stage profile (q8): ffn 215 / conv_module 126 /
  attn_heads 120 / attn_proj 119 / conv_sub 55 / mel 33 / decode 27 ms. Levers: mel is
  single-threaded (thread FFT/mel-dot over frames); conv_sub GEMMs stay fp32 - check their
  threading; audit every `lanes_for_work` gate at jfk-size tt (small clips may run stages
  inline that would profitably thread at 8 lanes); small-tt q8 tile shapes /
  `effective_token_block` at tt~140; per-layer requant overhead at small tt; v3 decode GEMV
  is 8198x640 per step (8x v2's rows) - q8 the joint if it shows. Gate per lever: token
  parity + jfk best-of-3 + LS p50 before/after, both boxes.

- **DONE SHIPPED + SILICON-ADJUDICATED (zen2, 2026-07-05): 2-D batch chunk space (row-units x
  token-blocks).** Landed as `batch_grid_2d` (0 = 1-D / 1 = fine grid, 16-token cells /
  2 = wave-aligned, rc*ntc = whole L-waves); the knob arms a per-dispatch auto-gate (engages only
  when the 1-D grain cap starves the admitted lanes), gen ts=4 walk factored into a shared
  body_cell so the off-path is the old walk verbatim; bit-exact all three ways (op-level test +
  512-tok GEN_IDS on both boxes). zen2 A/B (T=48, 3 interleaved reps): 135M pp512 knob2 +7.9%
  (every rep; +15% in the post-pin interleaved confirm), knob1 only +2.8% - **wave alignment
  beats amortize-the-tail chunking at one-claim-per-lane granularity, the same physics as the 1-D
  wave invariant**; 1B +3-4% (kv d=512 starves); Qwen3-0.6B null as geometry predicts (nothing
  starves at 48 lanes); T=24 control identical (gate can't engage). zen2 profile pinned
  `batch_grid_2d: 2`; M1 pin stays off (<=11 lanes can never starve - verified no-regress).
  OPEN: the SPR respin should A/B the pin at T>=48 (more shapes starve at higher lane counts;
  fine-vs-aligned may flip where claim overhead differs). Original scoping kept below.
  Our batch dispatch chunks over out-row units only; tokens loop inside each chunk. Shapes
  with few row-units starve high lane counts (135M d=576 -> ~5-36 chunks for 48 lanes; the
  Qwen3-0.6B attn_chain "deep-thin" 50% lead is the same geometry). The upstream GENERIC path
  chunks 2-D - (out-rows x tokens) grid, chunk 16, one atomic counter, all architectures -
  which is exactly why its mid/high-lane scaling holds on tiny models where our gated pool
  tops out. Our batch walk already loops tokens inside units, so
  the chunk space generalizes mechanically (matmul_chunks gains a token axis; worker chunk =
  (unit range, token-block range)); y-slices stay disjoint by construction. Ordering: linearize
  ROWS-FASTEST (concurrent workers then share the token block's activation slice in LLC and
  stream disjoint weight rows; starting the counter at the nth chunk on the first wave is
  the rank gate's natural analog on our side). **Delivery shape (Boris, 2026-07-05): a PIN per
  architecture, not a default rewrite** - the 1-D scheme already wins 2 of 3 ladder rungs
  head-to-head (avx2, vnni) and the M1/zen2 boxes; 2-D lands as a box_profile knob (default
  off, current behavior byte-identical), enabled where the per-box tune/fleet A/B proves it
  (SPR-class high-T tiny models, deep-thin attn shapes). Candidate refinement to discuss at
  implementation: an in-code auto-gate that only engages 2-D when the 1-D unit count starves
  the admitted lanes (units < k*lanes) - the knob then pins the gate rather than the mode.
  Sized: the 135M-class T>=24 residual vs upstream (its 0.90-1.0 cells) + the attn_chain lead.
- **DONE SHIPPED (audio arc, 2026-07-06): the Qwen2-Audio arc (speech->text, Boris 2026-07-05).**
  Landed token-for-token vs mtmd, then grew into the whole audio wave - whisper-proper ASR
  (6-model family sweep), Ultravox/Qwen2.5-Omni/Voxtral, Qwen3-ASR (new qwen3a chunked
  encoder), Parakeet-TDT, live-mic dictation, uniform `load_asr_model`/`transcribe` surface +
  chat-audio verbs on the facade. Arc-local perf ledgers folded in below. Original scoping kept:
  The cheapest audio-input path: Whisper-large-v3 encoder (~640M - mel frontend via the
  ALREADY-BOUND dasMinfft real FFT, the same per-frame-FFT pattern dasAudio's partitioned
  convolution reverb production-tests; 2x conv1d+GELU subsample = kernel-3 neural convs ->
  im2col over EXISTING matmuls, no new compute kernel - FFT convolution only pays at long
  kernels; N PLAIN encoder blocks = existing matmul/norm/softmax with non-causal no-cache
  attention) -> avg-pool -> linear projector -> soft tokens spliced at the `<|audio|>`
  placeholder; the decoder is our EXISTING qwen2 arch untouched (no cross-attention anywhere).
  New pieces: im2col gather, encoder forward, embedding-span prefill (driver), mmproj GGUF
  loader; oracle = the upstream mtmd tool (GGUF pairs ship). ~1 modest arc; the encoder is SHARED
  infrastructure - the same implementation unlocks Ultravox (llama-3 decoder yes have it) and
  ~80% of a Whisper-proper port later.
- **DONE SHIPPED (kq chain regrain, 2026-07-12 PM^2): kq layers re-admitted to both fused decode
  chains.** The chains run one activation image per consumed form (the per-op mm_pre_f
  contract): q8 projections read the Q8_0 image, kq projections the Q8_K image (kxq/kxs/kxbs);
  the w2-input requant moves to 256-row stage-0 groups and the wo-input requant to HEAD GROUPS
  of 256/head_size heads (Qwen3's 128 = head pairs) so every Q8_K quantize covers whole
  superblocks. Gates: dim % 256 for any kq weight, hidden % 256 for a kq down-proj, qd % 256 +
  head-size divisibility for a kq wo (all real kq models pass). Bit-exact vs the per-op path -
  test_fused_decode grew a kq arm (Qwen3-4B Q5_K_M = k5+k6+head-pairs, gemma-2-2b Q4_K_M =
  k4+k6+softcap/post-norm; ids + full logits EXACT). Measured (M1 Max steady-state): Q6 tg
  30.5 -> 31.1 (+2.1%), Q5 ~flat (33.8, within run wobble), Q4 control flat; decode_prof
  confirms attn_chain 26% + ffn_chain 63% carry the whole kq decode. Dead requant_rows_q8_bs
  deleted with the last Q8_0-bs chain arms.
- **DONE SHIPPED (kq v3 panel-scratch, 2026-07-12 PM): the k5/k6 kernel unpack lever.** The
  tile now reads a BYTE-EXPANDED panel (one byte per weight - zero unpack ALU) that the
  batch cell unpacks from the packed grp planes once per (group, token-block)
  (`unpack_kq_panel_grp`, SWAR uint64 deposit), amortizing the 5/6-bit deposit over TB=128
  tokens instead of the tile's 4; the DRAM planes KEEP the packed 160/192 B/superblock form
  the decode path streams. M1 Max e2e (Qwen3-4B, warm/quiet, 3 alternated reps): Q5 pp512
  110 -> 150-168 (upstream 131 -> das 1.14-1.28x), Q6 111 -> 140-143 (upstream 138.5 -> 1.01-1.03x),
  tg64 at v2 parity (Q5 ~33.7, Q6 ~30.5, Q4 control untouched). Iso tile: k5 65->89, k6
  58->76 GMAC/s at the probe's x16 amortization (~94/~79 effective at production x32).
  **THE LESSON (measured, do not re-learn):** pure byte-expanded DRAM planes (no scratch)
  win the same pp but cost tg -30%/-20% on Q5/Q6 - M1 Max decode is DRAM-bound at the
  model level (~90 GB/s effective), so plane bytes ARE decode time; k4 stays nibble-packed
  everywhere for the same reason. Residual headroom, ledgered: (a) k6 tile 76 vs k4 95 -
  the per-16 SIGNED sub-scale fold pays 2 scale-row sexts + 4 muls per block vs k4/k5's 1;
  (b) k5 unpack still ~6% of production tile (the broadcast+carry deposit - a generated
  NEON tbl/cmtst unpack kernel would close it).
- **DONE RESOLVED (2026-07-12 PM^2, trace-diagnosed): the das prefill "bimodality" is M1
  package DVFS, not code.** Lane-timeline traces of a fast (168 t/s) vs slow (150) Q5
  pp512 run: all 8 lanes 98-99% utilized in BOTH, identical chunk counts, and a UNIFORM
  x1.113 per-chunk slowdown flat across run-deciles - one P-cluster clock step
  (3228->2904-class), run-scoped. First-run-after-idle rides a ~3 s boost window (168);
  back-to-back runs sit at the sustained clock (150-152, +/-0.6% - exactly llama-bench's
  stability, because llama-bench's reps are always steady-state). 45 s cool-downs recover
  only partially (155-162); pmset shows no thermal warnings (ordinary sustained-load DVFS).
  E-core lane placement is EXONERATED - nothing to pin. **METHOD RULE: report the
  steady-state MEDIAN of >=3 back-to-back reps and discard the first-after-idle rep;
  best-of-N systematically picks the boost outlier.** Steady-state scoreboard (M1 Max,
  Qwen3-4B vs upstream steady): Q5 ~150 vs 131 = 1.15x, Q6 ~141 vs 138.5 = 1.02x, Q4_K_M
  ~155-158 vs 172 = 0.90x.
- **DONE RACED (zen2, 2026-07-12 PM^2, 16 affinity-pinned cores both sides, ABBA before=52a22a39b
  after=3d78ca8ef, Qwen3-4B): das WINS Q5 pp 1.84x / Q6 1.32x vs upstream; the kq v2+v3 arc itself
  is ~NEUTRAL on the maddubs lattice.** pp512 das-after/upstream: Q4 161-180 vs 168.3 (~parity -
  its one AVX2 K-quant repack), Q5 158-161 vs 87.0 (**1.84x**), Q6 128-130 vs 98.0
  (**1.32x**); tg64 ~ upstream parity all three (19.3/16.6/14.5 vs 19.25/16.87/14.69). ABBA
  before->after: Q4/Q5 pp par-to-+3%, **Q6 pp -4.6%** (135.2->129.0 median), tg within noise
  (Q4 -4%, Q5 +2%, Q6 -3%). Reading: the M1 unpack win was an sdot-lattice property - on
  Zen2's maddubs lattice the v1 unpack was never the bottleneck, and the Q5/Q6 upstream wins
  pre-date the arc (its dot-product rail is that slow). The Q6 -4.6% is the arc's one x64 cost;
  suspects (not yet attributed - needs a mid-arc leg): the v2 signed per-16 fold's separate
  lo/hi i16 chains (flush every 2 madds) vs v1's, or v3's second verbatim load per weight
  vector on a load-port-bound lattice. Candidate fix if chased: gate kqBytes per-ISA (byte
  panels NEON-only - needs a per-format panel-flag companion so the batch cell knows).
  Sized: ~5% Q6 pp on zen2 only; zen2 stays 1.32x ahead of upstream regardless. Also not yet run:
  the zen2 kq tune sweep (families ran the maddubs-mr8 fallback rows; crowns could shift a
  few %). SPR when a box respins - per Boris (2026-07-12): future profiling moves to an
  AWS box, local boxes are a bottleneck.
- **kq tune bench lacks a MoE-shaped cell row (spotted validating the mr4 crowns, 2026-07-12).**
  The mr4 tile crown gave dense pp +10-26% but gave back ~3% MoE prefill on qwen3moe-30B
  (fused expert cells average ~32 tokens per expert with d=768-class group spans - a regime
  the d=512/ntok=64 batch fixture doesn't represent; mr8's halved group count wins there).
  Adding a MoE-cell-shaped fixture to kq_tune_family and weighting the decision (or a
  per-model-class entry) recovers it. Sized: ~3% MoE pp on M1; re-check on zen2/SPR grids.
- **AMX fold pipelining (double-buffered C spill) - only if amx silicon verdict ever flips.**
  The shape to build: interleave block i-1's AVX-512 scale-fold between block i's
  TMUL ops (double-buffered thread-local C scratch) - the fold hides under
  tile latency; our emit_amx_tile serializes them, likely most of the T-independent ~1.6x
  end-to-end amx loss on SPR. Pure emission-order change (second spill alloca + reordered
  fold). NOT worth doing while the amx leg loses on frequency/bandwidth grounds anyway
  (SPR session-3 verdict: grid-resident, biased busd512 keeps the manifest); revisit on
  Granite-Rapids-class silicon or a cache-resident serving regime. A thread-local
  once-per-thread `ldtilecfg` (vs our per-call config+release) rides the same follow-up.
- **x64 intrinsic backends lack the `mm_rows` row-range GEMV core (fused-chain fallback).** The
  fused decode chains (team_parallel_stages, 2026-07-03) gate on `kernel_backend_has_rows()`;
  portable + both arm64 backends carry the core, so the EPYC (profile-pinned portable) and M1
  get the fused path - but an x64 box on auto-select (avx2-repack / acc8 / vnni tiers) falls
  back to per-op dispatches. Fix = extract each x64 family's GEMV inner loop into a
  `q8q8_rows_kernel_*` (mechanical, mirrors the arm64 extraction); worth ~the same join-tail
  savings wherever an intrinsic tier ever beats tuned-portable at decode. Cost today: none on
  the campaign boxes (both run backends that carry the core). (Spotted during the fused-dispatch
  work, 2026-07-03.)
- **Metal batched decode's skinny-M kernel valley - the P4 chase (emission arc, 2026-07-14).**
  The 3B d512 engineering curve (Parsec-on, fixed-token protocol) is 101/103/148/270 t/s at
  B=2/4/8/16 vs llama-batched-bench Metal's 134/219/250/363 - steps are GPU-bound (CPU side
  < 1.5ms), so the whole gap is skinny-M GEMM efficiency: the fixed-B batched GEMV forms
  ALU-saturate (~340-540 GMAC/s - the B=4 valley's 35.9ms step), and gemm32's M-pad-32 staging
  floor is ~51-54ms/step (~2900 GMAC/s padded) regardless of B, while the upstream mul_mv sustains
  near-weight-stream rates to ne11~8. Chase candidates, LAB ROWS FIRST (bench_metal_gemv_kernels
  grows variants): (a) x-staged batched GEMV - stage the B-row X panel slice in threadgroup
  memory per k-chunk so per-lane device x loads (the ALU-bound form's limiter) become tgmem
  reads; (b) a gemm16 twin (16-row M tile - halves the pad waste and A-staging at B <= 16);
  (c) the production 34B-blob mul_mm at M-pad-64 (f32-X, no quant dispatches - costs blob
  residency next to the decode driver's planar regions). Smaller shavings on the same row: a
  cls-specific GEMM crossover (the lab says the 128256-row classifier flips at B~4, the uniform
  policy switches at 5); per-row KV writeback + logits scatter memcpys (~2.3ms at B=16) could
  thread; llama-8B (dim 4096) declines batch at B >= 5 (the add+rms+quant row slab caps at
  3072 - an unfused rms+quant pair would re-admit it); GPU-side per-row greedy argmax for the
  batch (the single-stream spec chain already carries the kernel) if greedy batch serving ever
  matters. Sized honestly: closing to the upstream curve is worth up to ~2.1x at B=4, ~1.7x at B=8,
  ~1.34x at B=16 on M1. UPDATE (2026-07-14 late): (a) LANDED as the x-staged fixed-B forms -
  lab b2x 326-363 wGB/s (+40-88%), b4x 188-217 (~2x, above upstream's ~186 at ne11=4); driver
  same-window A/B B=4 +19.2%, B=2 +1.8% (B=2's step is residual/dispatch-bound: knockout says
  GEMV+cls is 11.7ms of the 16.7ms gpu step at B=2, 21.4 of 27.6 at B=4 - the rest is the
  per-step kernel residual + ~141 serialized dispatch tails, the encode-ahead/fusion rounds'
  territory). (b)/(c) were settled by the GemmB v2 + per-site split-K rounds (32x32 occupancy
  beats 64-wide at M-pad-32; mul_mm rail kept opt-in). NEW smaller shaving measured en route:
  the in-loop s16 scale select costs ~3% at B=2-4 (wscale-f16 0 vs 1 driver A/B) - the per-PSO
  s16 bake stays on the ranked list.
- **Fused-chain follow-ups: MoE FFN chain + norm/quantize as a stage.** The 2026-07-03 fused
  decode covers the attention block (all q8 arches, head_size % 32 == 0) and the DENSE FFN;
  gpt-oss's routed-expert FFN keeps its per-op groupn dispatches (3/layer) - a 2-stage MoE chain
  (router serial, [experts' gate+up + act + requant] -> [downs + weighted reduce]) is the same
  shape one level up. And each chain still has a serial ~3-6us norm+quantize prologue per block;
  folding it in as a tiny stage-0 (chunked scale+quantize after a serial sum-of-squares) or
  fusing norm INTO the quantize pass is the last serial glue. Sized: MoE chain ~= the dense win
  for gpt-oss decode; norm-stage ~1% at 1B. (Spotted during the fused-dispatch work, 2026-07-03.)
- **DONE (perf pass, 2026-07-02): tied classifier matmuls the Q8 disk quants (`Model.cls_q8`).**
  Tied Q8 loads of a Q8_0 embedding (every tied model we run - probed all 11) transcode
  `token_embd` twice into qblob - a classifier copy at wcls_off (repacked with the other 2D
  weights) and a LINEAR copy at emb_q8_off that embedding rows dequant from on demand (the laneq
  repack interleaves wcls in place, so row reads need their own un-repacked copy; on a no-repack
  box the two could alias - noted x64 follow-up) - and drop the fp32 table: on gemma-4-12B,
  classifier traffic 4.03GB -> 1.13GB/token and resident 4.03GB -> 2.26GB. Rows are bit-identical
  (same Q8_0 data the fblob decode used; gated by test_parity_tied_cls_q8_rows); the classifier
  quants are exactly what upstream matmuls. 8 of 9 tied-model fixtures held token-for-token
  unchanged; gemma2's "Once upon a time" flipped a near-tie under the PINNED classic+libm test
  kernels only (default kernels still matched the oracle 24/24) -> moved to the counting prompt
  like Qwen2.5/Phi, oracle-refrozen. fp32/q4 loads and non-Q8_0 embeddings keep the exact old
  path. (Spotted wave 3.)
- **DONE (2026-09-05): the UNTIED Q8 embedding keeps its Q8_0 quants too (`Model.emb_q8`).**
  `cls_q8` had coupled "tied classifier on the disk quants" with "no fp32 table", so a Q8 load
  of a model with its own `output.weight` still decoded `token_embd` to fp32 into fblob: vocab x
  dim x 4 bytes of RAM and image on every untied model (37 MB on stories15M, 262 MB on
  TinyLlama-1.1B, 394 MB on Phi-3.5-mini, 1.24 GB on the 152k-vocab Qwen3-30B-A3B - sized, not
  measured; Qwen3-4B, Llama-3.2, Qwen2.5, gemma-3 and SmolLM2 are tied and never carried it). Now
  any Q8 load of a Q8_0 `token_embd` transcodes the LINEAR copy at emb_q8_off and drops the table;
  tied models are unchanged (`cls_q8` = tied && emb_q8). Rows bit-identical to the fp32 decode
  (`test_parity_untied_emb_q8_rows`); stories15M's image 51 MB -> 26.6 MB, mint + map + token-exact
  (image suite arm `untied`); the Metal blob flavor carries the copy on TinyLlama (arm
  `metal-untied`). IMAGE_VERSION 33: every image re-mints once. Vulkan's device embed gather has
  no untied-q8 arm yet (the f32-upload arm declines, the CPU embed loop serves). (Spotted by the
  storyteller's wasm download budget.)
- **DONE (perf pass, 2026-07-02): V-from-K layers fuse the K->V copy with the weightless V-norm.**
  Decode (mm_qkv) and prefill both rmsnorm k->v out-of-place when v_norm is on (bit-identical to
  copy + in-place norm; the block's v_norm step skips those layers). (Spotted wave 3.)
- **DONE (perf pass, 2026-07-02): upstream A/Bs run (quiet box, CPU `-ngl 0`, llama-bench
  pp512/tg64 vs our matched driver, ggml-parity fast-math).** gemma-4-12B: prefill us 75-80 t/s
  vs upstream 74.4+/-0.5 (parity to +5%); decode us ~7.3 vs 8.74 (~84% - the remaining decode gap
  is the next lever). gpt-oss-20b: prefill us ~219 vs 117 (~1.9x FASTER - the grouped MoE GEMM);
  decode us ~19 vs ~39-42 (~0.47x - exactly the MXFP4->Q8 doubled expert-weight-traffic asymmetry
  quantified: the native-MXFP4/Q4_0 entry below is now the headline gpt-oss decode lever).
  POST-JOBQUE (#3361 wake propagation + batch dispatch + worker spin, same-window anchors):
  12B decode 7.3 -> 7.9 t/s vs 8.63 (gap 84% -> ~92%); gpt-oss decode 19.2 -> 22.0 vs 39.9
  (0.47x -> 0.55x) - the dispatch-latency share of the decode gap is banked; what remains on
  gpt-oss is the weight-format asymmetry. (Spotted waves 3/5.)
- **DONE (perf pass, 2026-07-02): MoE prefill runs expert-bucketed grouped GEMMs - bit-exact.**
  `ffn_moe_prefill_grouped` routes every position (one batched router GEMM + the shared
  `moe_select`), CSR-buckets the (position, slot) pairs by expert, runs one batched GEMM chain
  per touched expert off a single whole-batch requant, and reduces the parked outputs in exactly
  the decode accumulation order (k slots then shared expert) - so it is bit-identical to the
  per-position path: the batch GEMM/requant/gate kernels are bit-for-bit their single-token
  forms per row. Proven on both MoE models (all logits identical after a 300-token prefill,
  grouped vs reference) and pinned by the qwen2moe fixture running through BOTH paths
  (`set_moe_grouped_prefill` A/B). Decode unchanged (one token = no bucketing win).
  Measured (M1 Max, interleaved in-process A/B): Qwen1.5-MoE 512-tok prefill 31 -> ~270 t/s
  (~8.7x); gpt-oss-20b 256-tok prefill 27 -> ~186 t/s (~6.8x). (Spotted wave 4.)
- **DONE (perf pass, 2026-07-02): MoE decode re-quantized the same activation per expert.**
  `moe_ffn_core` now quantizes xb once per layer into dedicated `moe_xq/moe_xs` (the
  down-projections quantize s.hb into the shared xq/xs, which would clobber a hoisted image
  there) and routes every gate/up matmul through `mm_at_q8_pre`. Bit-identical (same quants).
  (Spotted wave 4.)
- **DONE (MXFP4 arc, 2026-07-02): native-MXFP4 expert stacks + repacked TBL/SDOT kernels + the
  MoE dispatch fuse.** Was: gpt-oss-20b's 4.25-bit expert stacks ran as Q8 (2x resident, 2x
  decode traffic). Now: the stacks stay as raw nibble + E8M0 planes (mxq/mxs, exact disk bits -
  the old dequant->requant amax error is gone), decoded in-register by new aarch64_neon
  intrinsics (tbl16_lo/tbl16_hi = vqtbl1q_s8 of the doubled-e2m1 LUT; sdot4_w / sdot4_laneq_w
  take the tbl result as a VALUE) through dot_mx4q8 ([tuned], row-major) and dot_mx4q8_laneq4
  (interleaved 4-row repack, the block_mxfp4x4 twin). Grouped prefill expands each touched
  expert to EXACT Q8 (lossless: q = LUT int, scale = e8m0_half), writing the interleaved form
  directly on a repack backend; short prompts route per-position (npos*k >= 8*n_expert guard).
  On top, the MoE decode dispatch fuse: region-list groupn/groupn_mx4 kernels run all k
  experts' gates (ups, downs) in ONE fork/join - 288 -> ~72 mm dispatches/token, bit-exact.
  Measured (QUIET box, 2026-07-02, same-window anchors): decode 22.0 -> **31.8 t/s** @ ctx 8
  (30.7 @ ctx 512) vs llama-bench tg64 41.1 - **0.55x -> 0.77x**; per-op profile: mm_moe 50%
  measured vs 47.9% theoretical share (the 66/66 format asymmetry is GONE), MoE mms sustain
  ~77GB/s vs the dense mms' ~99 (the remaining MoE-efficiency gap = the next lever); 12B decode
  7.98 vs anchor 8.67 (92%, unchanged - dense path untouched); resident 26 -> 13.2GB; every
  fixture token-for-token unchanged (no refreeze - the counting fixtures absorbed all
  kernel-order changes). Cost paid: gpt-oss pp512 ~186 -> ~121-149 t/s (the per-expert
  expansion) vs upstream's 119.9 - still >= parity; the native mx4 batch GEMM in the expansion
  entry below reclaims it. (Spotted wave 5; the Q4_0 halfway house was skipped - native landed
  directly.)
- **q4 has no batched prefill kernel - prefill collapses to decode rate.** The q4 path serves
  everything through the scalar fp32-activation `dot_q4`/`matmul_q4` (no q8-style token-blocked
  batch GEMM, no NEON arm, no repack backend), so a q4 prefill runs at generation speed:
  measured on SmolLM2-135M, q8 prefill 1391 t/s vs q4 prefill 70 t/s ~ its own 69 t/s decode.
  A q4 batch kernel (or the load-time q4->q8 transcode as the cheap fix) closes it.
  (Spotted tutorials wave.) **LOW PRIORITY (2026-07-02):** the path is cold - every model we
  test/ship parity for is Q8_0 on disk (plus gpt-oss MXFP4->Q8); no Q4_0 GGUF anywhere in the
  fixture set. q4 only fires when a user opts into `QuantMode q4` for footprint. Priority rises
  only if the MXFP4->Q4_0 halfway house above lands (q4 becomes the resident format of a real
  20B model). **RESOLVED DIFFERENTLY (2026-07-16):** Q4_0 *files* now serve natively on the kq
  rails under q8 mode (KqFmt.q40, #3481) - batched prefill included; the legacy tier was renamed
  `QuantMode.q4_0` and remains the requant-from-f32 footprint option, per-row kernels unchanged.
- **LOW PRIORITY: f32 projection GEMM is untiled - dot-per-token, no token block.**
  `matmul_batch` (dasllama_math.das) is the exact pre-#3315 shape the Q8 path had: weight-
  stationary nest with one horizontal-reduce `dot()` per (row, token), zero register reuse
  across rows/tokens, and no L2 token-blocking (long-prefill X re-streams from DRAM per weight
  row). The SDOT-era fix transfers verbatim since it's dtype-agnostic: a 4-row x 4-token
  register tile with float4 `mad` chains and per-tile reduces (the fp32 twin of
  `dot_q8q8_sdot4x4` - keeps W row-major, no repack, decode GEMV untouched; do NOT reuse the
  broadcast-A `gemm_f32` form, it needs a transposed W copy) plus an `effective_token_block`
  at ~1/4 the Q8 block (fp32 activations are 4x fatter). Expected kernel win ~2-3.5x (what the
  attention tile measured), ceiling below Q8 (fmla = 4 MACs/instr vs SDOT 16; 4B/weight vs
  ~1.06B). LOW because the f32 arm only fires for f32 GGUF tensors - in practice the tiny
  teaching models; attention's fp32 GEMMs already have the register tile (`gemm_f32_uk_4x16`).
  (Spotted post-#3354, 2026-07-02.)
- **DONE (perf pass, 2026-07-02): `kv_cache_off` prefix-summed per call.** `Model.kv_row_prefix`
  (filled by layout_offsets, seq_len-independent) x the LIVE seq_len at call time - the O(1)
  Model overload serves both hot call sites; the Config walking form stays as the definitional
  reference. (Spotted post-wave-3 review, per Copilot on #3346.)
- **DONE (perf pass, 2026-07-02): decode attention threads over heads - crossover measured,
  default re-set.** `attention_std_decode` maybe_parallel_fors the head loop (disjoint per-head
  rows => bit-exact vs inline, gated by test_forward), behind `g_decode_attn_par_threshold`
  (profile `runtime.decode_attn_par_threshold`). Quiet-box sweep (M1 Max, inline-vs-threaded
  interleaved at 32..2048 ctx on Llama-3.2-1B and gemma-4-12B): crossover at ~200-260K work on
  BOTH; below it threading costs <=2%, above it wins reach +74% (1B) / +89% (12B) at 2048 ctx -
  the derived 4M default was ~15x too conservative (the 12B ran inline below ctx 512). Default
  is now the measured 262144. (Spotted tune audit, 2026-07-02.)
- **LOW PRIORITY: `sample_` top-k is O(top_k x vocab) scalar selection.** Each of the top_k
  rounds rescans the whole vocab (dasllama_common.das sample_) - top_k=40 on gemma-4's 262144
  vocab is ~10M compares per sampled token. Cold today (SamplingParams defaults are greedy /
  top_k=0, and all parity fixtures are greedy), but it's the sampling path the tutorials teach.
  Fix = single-pass partial selection (bounded min-heap of size top_k, or threshold-and-count).
  (Spotted tune audit, 2026-07-02.)
- **DONE (MXFP4 arc follow-up, 2026-07-02): the mm_moe bandwidth-gap profile + the bias fold.**
  Iso-benched the exact decode dispatch shape (4x [2880 x 2880] regions, DRAM-rotating):
  the fused mx4 groupn GEMV sustains **~101 GB/s - bandwidth parity with the q8 dense kernels**
  (the "77 GB/s" in-decode reading was largely single-window wobble: same build re-measured
  90 GB/s an hour later; METHOD: only round-robin interleaved cells within one process are
  trustworthy on this box, single-window absolutes swing +/-10-15%). The pre-fuse 4x1 dispatch
  shape measures 63-72 GB/s - the dispatch fuse was worth ~30% and is confirmed load-bearing.
  Follow-up landed: **expert bias vectors fold into the groupn workers' stores** (bp/boffs on the
  groupn contract; bit-identical to the post-pass add_bias, minus its serial ~36us/layer) -
  decode 34.7 -> **35.2 t/s @ ctx 8 / 33.8 @ ctx 512** (upstream same-window anchor 41.1 ->
  0.86x). Also swept: decode-attn threshold 0-vs-262144 under the spinner at ctx 8/512 -
  a WASH at both depths (the low-ctx attention is memory-latency-bound; threading's dispatch
  cost ~ its serial cost), so the measured default stands; moe_reduce/rope threading rejected
  (~8us/layer each, below dispatch cost). What remains vs upstream is its continuous-polling
  threadpool (the bus never idles between ops) - picked up by the x64 arc's jobque work, not
  patchable here. (Profiling session, 2026-07-02.)
- **MXFP4 grouped prefill pays a per-touched-expert Q8 expansion (~120MB of traffic each, half of
  it the repack scratch copy).** `expand_mx4_region_q8` writes exact row-major Q8 then runs the
  load-time `repack_q8q8_weight` (temp copy + interleave) so the laneq batch GEMM applies. Levers,
  in effort order: (a) expand DIRECTLY into the interleaved layout (folds the repack's copy away -
  needs a backend-provided expand-repack, not a layout hardcode in common); (b) a native MXFP4
  batch GEMM (mx4 twin of the laneq 4x4 tile - halves the GEMM's weight streaming too, likely wins
  outright); (c) the `npos * k >= 8 * n_expert` tiny-batch guard is an ESTIMATED breakeven
  (4-tok-prompt ttft 1895ms -> 114ms) - sweep it when the mx4 A/B rig exists. (Spotted MXFP4 arc,
  2026-07-02.)
- **Flash-attention tile shape is a frozen compile-time constant - deferred x64 tuning axis.**
  `ATTN_FLASH_QT/KV = 64x64` (dasllama_common.das) was chosen on M1 and never swept; tile shape
  is the classic per-box cache parameter, and x64's small private L2 differs in kind from M1's
  big shared L2. QT is compile-time-coupled to the `float[64]` running max/sum fixed arrays and
  the fa_* scratch sizing, so this is a compile-time axis a la `[tuned]` (profile-keyed), not a
  runtime setter. DEFERRED until an x64 box exists to measure on - do not solve the coupling
  speculatively. (Spotted tune audit, 2026-07-02.)
- **DONE (GEMV hunt, 2026-07-03): the decode "kernel top-end gap" was chunk-count misalignment,
  not the kernel.** A 12-variant GEMV race (`bench_gemv_decode.das` + the matmul_variants decode
  cells: unroll/ILP/fma-flush/dual-group/inline-scale) proved every kernel micro-opt a wash -
  the upstream live M1 kernel (its 4x8 tier needs i8mm) is the same
  shape as `dot_q8q8_laneq4`. The real delta: njobs = `get_total_hw_jobs()*k` sized chunks to the
   7 WORKERS, but 8 lanes serve them (workers + caller - team by design, fifo via main-steal), so
  the last wave ran half-empty (28 chunks / 8 lanes = 87.5% utilization). Fix:
  `get_dispatch_lanes()` (workers + caller; knob `set_dispatch_caller_lane`, decode_prof
  `--legacy-lanes`) at all 54 dispatch sites incl. x64 (same effect there, smaller: ~2-3% at 32+
  workers). Measured: GEMV cls 109 -> 118-120 GB/s (the upstream rate exactly); decode e2e 74.3 -> 77.6 t/s
  @ ctx 8 (+4.4%, ~0.95x upstream) / 66.3 -> 67.8 @ ctx 512; mm_ffn 1.10x -> 1.04x, cls 1.19x -> 1.12x
  of upstream per-op. Suite 171/171 token-for-token (chunk splits don't move per-row math). Oversplit
  re-sweep under aligned lanes: decode e2e is a WASH across x1/x2/x4 (x8 past the knee) - the
  straggler-mitigation rationale dissolved once the split became 8-on-8, x4 default stands on the
  prefill iso numbers. (GEMV hunt session, 2026-07-03.)
- **Q8 scales stream as a separate fp32 plane - an inline-fp16 layout moves ~6% less
  weight traffic.** Packing 4 fp16 scales WITH the 128 quant bytes is 136 B per
  4-row block-group in one stream; our laneq layout reads a second fp32-plane stream (144 B total
  per group). At the now-aligned ~118-120 GB/s both sides saturate equally, so the remaining
  cls/ffn per-op gap (1.04-1.12x of upstream) is almost exactly this byte ratio (144/136 = 5.9%;
  bench: our best cls call 2448us vs upstream's 2307 = 6.1%). The bench's `inline`/`inline_u2m` cells
  already prove the single-stream kernel shape works (fp32 inline = parity, occasionally +1%).
  The win requires fp16: (a) an f16->f32 convert intrinsic in dasLLVM aarch64_neon (vcvt_f32_f16
  via LLVM fpext, x64 twin F16C `vcvtph2ps` - gate on `cpu_supports`); (b) a repack variant
  writing [4xfp16 d | 128 qs] blocks + the GEMV/batch/group3/groupn kernels reading it; (c) the
  element-offset plumbing moves from plane offsets to block strides (loader `w*_off` math, groupn
  offs contracts). Sized: dasLLVM intrinsic + codegen bump, ~4 kernel twins, loader/offset sweep,
  fixture risk LOW (same math, scales exact fp16 round-trip of what the loader already computes -
  but the quantizer would store fp16-rounded scales, so oracle fixtures need a refreeze check).
  Expected: cls 1.12x -> ~1.06x, ffn 1.04x -> ~parity; decode e2e ~+2-3%. NOT this PR - needs its
  own arc. (Spotted GEMV hunt, 2026-07-03.)
- **Audio arc: the fp32 encoder is the whole ASR cost - q8 the encoder GEMMs.** The whisper-family
  encoder runs fp32 ~ 18-19 s per 30 s chunk on M1 (~37 s for qwen2a's standard 2 chunks;
  large-v3-turbo transcribe = 0.49x realtime, almost all of it the encoder window). Fast path:
  q8 the 6 encoder GEMM families + projector at load (`quantize_weights` pattern) onto the
  generated q8q8 kernels - expect the usual ~4x (turbo -> ~2x realtime) - plus threading the
  im2col/pack loops. Tolerance-gate like flash-decode (stage witness vs fp32), token-parity
  revalidate. (Audio arc, 2026-07-05/06.)
- **Audio arc: the all-silence second chunk's soft tokens are input-independent.** Every <=30 s
  clip pads to 2 chunks and the second is all mel-floor - its 750 soft tokens are the same for
  every clip, cacheable per tower; halves qwen2a encoder cost for short clips. (Audio arc,
  2026-07-05.)
- **Audio arc: whisper decoder logits GEMV, only if it shows.** The ASR decoders are small
  (tiny ~35 MMAC/token, turbo ~140 incl. the 66 MMAC tied logits) - fp32 is fine; q8 the
  token_embd logits GEMV if profiling ever surfaces it. (Audio arc, 2026-07-06.)
- **Audio arc (correctness follow-up, not perf): honor `encode(parse_special=true)`.** The flag
  is documented-unhonored; every call site assembles specials by id + per-segment text encode
  (the chat-layer pattern). Fix belongs in the tokenizer proper; the workaround callers migrate
  after. (Audio arc, 2026-07-05.)

## Transferred entries (folded in 2026-07-29, dasLLAMA doc reorg)

Arc-local ledgers and standing verdicts that were living inside per-arc plan docs. Two of them
(`whisper_plan.md`, `qwen2_audio_plan.md`) carried their own header saying "fold into
API_REWORK.md at PR time" - this is that fold, into the ledger's new home. Source doc named per
group; wording kept.

### From `gemm_generator_plan.md`

- **AMX tile family - arc CLOSED (slice I built it, slice K's SPR session 4 adjudicated it,
  2026-07-05).** amx is 0.36-0.55x the biased-busd512 champion across all four ladder models and
  the best cell ever measured sits below the plain AVX2 maddubs tier; the bill is the kstep=1
  Q8-scale-boundary fold (~256KB of C-spill per 32-row x 32-token unit vs 64KB of weight traffic -
  codegen, frequency license, and dispatch all separately exonerated). Family stays grid-resident,
  `spr_manifest` unchanged; revisit only on Granite-Rapids-class silicon or a cache-resident
  serving regime. Conditional-only tail: an **amx kstep=2 fold** (K=64 tiles, two scale segments
  per spill) halves the C traffic but its honest ceiling is ~1.6x behind the champion.
- **smmla leg (slice J) - no fallback-chain change (M3 Air silicon, 2026-07-05).** The smmla tile
  sits at **100% of the M3's ONE-NEON-pipe MMA ceiling** (130.6 GMAC/s = 4.08 G MMA/s ~ 1/cyc,
  marshalling fully hidden) and still loses to sdot's ~2.45 issues/cyc across 4 pipes - 1 pipe x 32
  MACs < 4 pipes x 16 MACs, structurally. The leg stays grid-resident + `requires="i8mm"`-gated for
  server arm64 (Neoverse V1/V2 / Graviton3+ issue smmla 2-4/cyc, where the same stamp should flip
  the verdict - the tune framework decides per box, which is the whole point).
- **PARTLY SHIPPED - loop-hint manifest kind.** The *storage* unified: the per-app tune sidecar's
  `"kernels"` section is documented to carry "this module's stamps and the loop-hint perms"
  (`modules/dasLLVM/daslib/llvm_tune.das:40`), and `dasllama_tune.das` now reads it through
  llvm_tune (`box_profile_path` -> `tune_manifest_path`, `box_profile_stale` ->
  `tune_sidecar_stale`, the perm read -> `tune_manifest_get`). Still OPEN: llvm_tune has **no
  loop-hint value kind of its own** (no loop-stamping code anywhere in it) - `dasllama_tune.das`
  still owns the `[tuned]` grid and does the hint stamping, with 18 live `[tuned]` sites, so the
  plan's "subsume, then delete dasllama_tune's `[tuned]`/box_profile rail" is not done.
- **OPEN - per-slot perms.** Unimplemented: no per-slot perm plumbing exists in llvm_tune or
  `dasllama_math_gen.das`; the phrase appears only in plan prose and in the unrelated per-slot
  *backend* pin (`x64_arch.md`, `dasllama_math.das`). What landed instead is per-FORMAT families -
  separate `[tune]` brackets for k4 / k5 / k6 / q40 / q8q8 / q51 in `dasllama_math_gen.das`, each
  its own manifest entry. Per-slot perms remains the named escalation if a box ever flips the amx
  end-to-end verdict, or if biased-vs-plain gemv turns out to matter under an amx stamp.

### From `kv_cache_compression.md` sec.9

- **BLAS / Accelerate on macOS - RULED OUT (measured).** dasLLAMA's 8-bit `sdot` kernel beats the
  BLAS (Accelerate) path, and it is structural, not incidental: BLAS is f32-only (no `hgemm`, no
  int8 - the upstream BLAS backend routes through one `cblas_sgemm` that dequantizes any non-f32 operand into
  scratch first), while the weight matmul is memory-bound on the weight stream, so a 4x weight
  expansion to feed AMX/SME f32 throughput can never win. Decision: don't route dasLLAMA matmuls
  through BLAS. **Its "for Apple lower-precision matmul the framework is BNNS ... not pursued" tail
  is SUPERSEDED** - the AMX arc (#3562) shipped a BNNS-f16 lane in
  `dasllama/dasllama_math_accelerate.das` (bf16 planes via BNNS f16 matmul under
  `DASLLAMA_ACCEL_F16=1`, jobque strip-dispatched).

- **OPEN - k4-native deltanet planes (the "dn planes are ALWAYS q8" loader invariant aged).**
  Minted on 0.8B-4B where the dn projections were a rounding error; on Qwen3.8-27B (dense
  hybrid, 48 recurrent layers) they are ~5.6 B of the 6.7 B q8-region weights, so the
  q4k->q8 load transcode costs **+~2.8 GB of image (22.3 vs ~19.5 GB metal blob) and ~20% of
  decode's per-token weight reads** (measured via the 2026-08-17 27B bring-up: dlim plane
  sizes + the q8-region scale count). The dn shapes are why the invariant exists - beta/alpha are
  dim x dt_rank (16-64, under every kq GEMM tile's N) - so the fix is a narrow k4 GEMV
  family (`MetalDnBa`-class + the CPU dot), not a kq-tile retrofit. Pays on every lane, and
  it is step one of **the 16 GB-card game** (Boris, 2026-08-17): 27B q4k on a discrete
  16 GB GPU needs the image at ~19.5 GB *plus* partial residency (the vulkan tier's
  offload/stream lane) or a ~3.5 bpw format family (k3/IQ - unsupported today).

### From `audio_models_plan.md` (Findings; correctness, not perf)

- **RESOLVED (87ffe39fe) - the v0.3 Mistral `[INST]` template's derived close was ` [/INST]` -
  wrong for multi-turn.** Fixed: `mistral_instruct_template`'s `assistant_close` is `</s>`, like
  v7-tekken's. Spotted in audio session B while adding `ChatTemplate.assistant_close`.

### From `whisper_plan.md` ("Performance ledger - fold into API_REWORK.md at PR time")

- Measured (M1, `examples/dasLLAMA/transcribe.das`, jfk 11 s, fp32): tiny 22x, base 10x, small
  2.6x, medium 0.85x, large-v3 0.42x, **large-v3-turbo 0.49x realtime** - almost all of it the fp32
  encoder window.
- Turbo encoder = the qwen2a tower cost (fp32 ~ 18-19 s per 30 s chunk on M1) - the q8
  encoder-GEMM path from the qwen2-audio ledger applies verbatim and is the headline item
  (expected ~4x -> ~2x realtime turbo). Same lever as the "Audio arc: the fp32 encoder is the whole
  ASR cost" entry above.
- Decoder is small (tiny ~35 MMAC/token, turbo ~140 MMAC/token incl. 66 MMAC tied logits) - fp32
  fine; q8 the token_embd logits GEMV if it shows up.
- Model load: byte-wise f16->f32 tensor reads (the bin stores tensors unaligned) - turbo ~1.6 GB f16
  loads in a few seconds; an aligned-fast-path only if load time starts to bite.
- gelu LUT round-trip is per-element float work - noise next to GEMMs, don't optimize.
- Long audio: encoder windows are sequential per seek; pipelining only if long-form becomes a real
  use case.
- One WhisperSession per stream shares one loaded model; sessions are cheap (scratch + caches),
  models are not - same economics as the LLM side.

### From `qwen2_audio_plan.md` (its two duplicate "Performance ledger" sections, deduped)

- Encoder GEMMs ride the existing generated-kernel family as-is (n=1280/5120 batch shapes). A
  dedicated tune pass over encoder shapes (1500-token batch, always-prefill regime) only if the
  encoder becomes a measured bottleneck vs mtmd. *(This is the later of the doc's two takes; the
  earlier "q8 the 6 GEMM families + projector at load" phrasing is the same lever, already carried
  by the "Audio arc: the fp32 encoder is the whole ASR cost" entry above.)*
- prefill 1518 pos (1500 audio) ~ 16.4 s on M1 for the 7B q8 decoder (~93 t/s); decode ~11 t/s -
  normal 7B-on-M1 numbers, no audio-specific cost.
- mel/DFT-GEMM is noise, leave it: single-threaded `gemm_f32`, ~0.13 s per clip, and the
  Mel-as-GEMM twiddle matrices are f32 fblob consts (~410 KB) - quantizing them is noise, skip.
- 30 s chunking processes chunks serially; multi-chunk audio could pipeline encoder chunks - only
  relevant for long-audio use, not scoped.
- *(Deduped away: the all-silence-second-chunk soft-token cache appears in both sections and is
  already its own entry above - "Audio arc: the all-silence second chunk's soft tokens are
  input-independent", 2026-07-05.)*

### From `PUBLIC_BENCH_PLAN.md` (v2 backlog)

- **OPEN - v2: hosted submission server**, which also hosts / auto-fetches the pinned upstream ref
  binaries. v1 accepts community records via reviewed PR plus the result panel's "copy record"
  paste funnel; the server replaces that funnel.
- **OPEN - v2: select-two compare widget (maybe)** on the leaderboard, and publishing the
  depth/batch test variants (`pp512@d4096`, `tg128@b4`, ...) that stay internal-only under v1.

### From the prefill-driver harvest (2026-08-27)

- **OPEN - MoE k-quant twin tune families.** The moe k4/k5/k6/q51 tensor twins are gated by
  the DENSE k-quant crowns rather than raced on their own; give each its own
  `metal_tensor_race` family and crown so the gate stops proxying. Instrument:
  `metal_tensor_race` on an M5 box, one q8 and one k-quant MoE model.

### From the qwen25v Metal-tower bring-up (2026-08-29)

- **OPEN - qwen25v window-attention kernel is the naive per-thread form.** `enc_tower_win_attn`
  runs one thread per q row, scalar dots straight from device f32 (the f16-staged simdgroup
  form was reverted: its staging noise re-rolls the 32-layer chaos, and the naive form still
  lands the pair ahead - enc 106 vs 117 ms, lcpp_bench image cell vs mtmd, m5
  [direction-grade - two processes]). If the encoder profile ever shows the window
  layers, the upgrade is a simdgroup-tiled f32 form (windows are 64x80 tiles; K%16 wants pad
  to 96). Instrument: asr_prof q25v.gpu split via a skip-family knockout, npos ~1564.
- **OPEN - qwen25v full layers ride the padded slab (hs 80 -> 96).** Four of 32 layers restride
  to hs_pad 96 through the attention trio; a dk80 flash monomorph (the gemma3v dk72 recipe,
  per-head-contiguous K/V) applies if the slab ever shows at big npos.

### From the gemma4a Metal-tower bring-up (2026-08-29)

- **CLOSED (same day) - the "long-context decode gap" was the embd path's CPU PLE pre-step.**
  tg128-at-depth refuted the rail attribution (das ahead at every depth); the asr_prof turn
  buckets (`benchmarks/lcpp_bench.das --asr`) pinned 774 ms of gb1's prefill on
  ple_pre_prefill_pad - the E-series PLE model_proj GEMM
  run on CPU because only the TOKEN prefill offered the pre-step to the device gate. The embd
  path now offers it too (ple_pre_prefill_pad_gated), the CPU fallback broadcasts one gather,
  and the scalar sampler is vectorized (hargmax/hlse). gb1 cell 0.83x -> 1.00x.

### From the canary M5 baseline (2026-08-29)

- **CLOSED - canary gb1 44.6 s -> 6.5 s (2.7x AHEAD of nemo's 17.8).** Notch (a) closed by the
  Metal FastConformer driver: the 32-block loop rides one command buffer on the f32 blob
  (enc_cn_attn = full bidirectional rel-pos XL attention with online softmax, one thread per
  (q,head); enc_cn_dw = centered k9 + folded BN + SiLU; LN/GLU/SiLU/axpy/GEMM/bias reuse the
  tower kernels). gb1 encode 26.8 -> 5.9 s; cells jfk 310 ms (8.5x ahead), jfk3 895 (7.2x),
  gb1 6522; transcripts verbatim on all three; encode rel-rms vs CPU 3.1e-4; kernels-suite
  oracle+poison gates + the canary mtower cell green. Residue: the encode's CPU half (mel +
  subsample front + proj, ~5.9 s of gb1) - the gemma4a whole-chunk recipe applies if the cell
  must go further.
  Original finding (decoder half): Notch (b) is
  closed: the serving decoder is the Q8_0 re-quant (CANARY_DEC_RECIPE), cls_q8 serves and the
  blob drivers accept - jfk 1.25 s (2.12x ahead), jfk3 3.91 s (1.64x), gb1 27.7 s (0.64x),
  transcript gate verbatim both serves. What remains of gb1 is notch (a) alone.
  Original finding: Two independent notches: (a) the FastConformer-32 encoder
  is CPU f32/q8 (26.8 s of the gb1 turn) - a Metal driver is the gemma4a-recipe class of
  work; (b) the decoder cannot ride the Metal rail: the f16-sourced GGUF's TIED classifier
  serves the exact fp32 path (cls_q8 wants a Q8_0 disk embedding) and the blob drivers
  decline a tied-fp32 classifier - either an fp32-table classifier GEMV on the rail, or a
  Q8_0 re-quantized decoder artifact (provenance churn: the CANARY_DEC_RECIPE would change).
  Instrument: lcpp_bench --asr -m Canary vs canary_qwen_bench.py.

### From the CPU board sweep under the hybrid pool (2026-08-29)

- **CLOSED (same day) - MoE CPU batch pp trailed on a groupn straggler, fixed by the 32-row
  region split.** The grouped-prefill offs builder now caps a CPU sub-region at 32 rows: the
  batch-groupn dispatch chunks its region x row-group units by COUNT while a unit's cost is the
  region's row count, so one zipf-heavy expert straggled the whole barrier
  (`harness/moe_kq_probe.das`: 620 GFLOP/s zipf vs 3537 uniform vs 3929 split; the kernel
  itself was never slow - narrow D=704 costs 2%, M=32 costs 14%). Post-fix cells (das/stock-BLAS ref best): 26B-A4B 448/301 =
  1.49x (was 0.82x), gpt-oss 382/238 = 1.60x (0.99x), 30B-A3B 396/280 = 1.41x (0.91x), 35B-A3B
  410/283 = 1.45x (0.94x); sanity argmax+logit fingerprints bit-identical on all four, MoE
  family matrix cells green. GPU tiles keep whole regions (unsplit). Residue: MoE pp cv 6-9%
  under the 18-lane pool remains on both engines.
  Original finding (das/stock-BLAS
  ref, both at their best lane count: 26B-A4B 242/301 = 0.82x, 30B-A3B 254/280 = 0.91x,
  35B-A3B 266/283 = 0.94x) while the same sweep has das AHEAD on every dense Q4_K_M pp cell
  (1.10-1.28x) and at 0.99-1.12x on q8/mxfp4. Not a pool-policy artifact - the hybrid pool
  roughly doubled our own MoE pp vs 6 lanes; the gap class is the k-quant MoE batch GEMM
  (expert-grouped rows), and both engines' MoE pp turns noisy at 18 lanes (cv 2-8% vs <1%
  dense - routing imbalance across lanes). Profile (26B-A4B pp512/pp2048, --prof): the grouped
  sb_kq expert batch GEMMs are 87% of ffn wall at ~0.85 TFLOP/s vs the dense batch kernel's
  ~4.7 on the same box - NOT per-expert-M starvation (rate flat from M~32 to M~128), NOT MoE
  plumbing (route+gather+reduce ~6%), NOT q51 alone (k4 gate/up 0.87 vs q51 down 0.79); mxfp4
  MoE sits at parity, so the sb_kq expert path specifically. Remaining suspects: the narrow
  per-expert output dim (D=704 -> 88 mr=8 row-tiles; q51 mr=4) and the kq_batch_groupn
  per-region walk vs the dense flat sweep. Instrument: `harness/moe_kq_probe.das` (synthetic-k4
  bare-kernel cells at expert shapes, dense-shape control included); ref bar re-taken against
  the b10659 clean-cpu build at the arc-end re-mint.

- **The mlxfast arena trunk is a standing M5 kernel-shape intel source - watch it, adopt
  shamelessly (Boris, 2026-08-30: "if there are better shapes on M5 - we are shamelessly
  adopting").** The arena (yukon.org/mlxfast, repo Layr-Labs/mlxfast-gemma4-26b-a4b-engine,
  local clone ~/Work/mlxfast) runs a collective ratchet: every accepted submission lands on
  `main`, so the trunk is a continuously-updated record of what 26 solvers' search found on
  Apple silicon. Every figure in this entry is `external` (their arena composite scorer and
  their `mlxfast-reports` engine.md - not our instruments; the composite is a two-commit
  delta of their trunk on their box, `direction-grade`): the 2026-08 delta (+30% composite
  in 33 commits, 100% kernel work) carries three candidates for our Metal kernels:
  (1) tight-grid dispatch - launch only causally/structurally valid threadgroups instead of
  early-returning ~75% of them (their decode step waived 158k of 211k tgs; our QK/AV grids
  have the same triangular waste, their engine.md item 4); (2) identity `scale*q`
  elision (~4.4 GB/prefill of traffic on their shapes); (3) a nine-dispatch norm/residual
  fusion. External figures ground the lead only - each candidate adopts through our own race
  or knockout plus a re-minted cell. Re-check the trunk each arc; diff since the last-read
  sha before every adoption pass. License verified 2026-08-30: MIT top to bottom - harness + submissions (c) 2026
  Layr Labs, vendored mlx-swift incl. Metal kernels (c) 2023 ml-explore/Apple - so adoption
  is clean with those two attribution lines (the NAX arc's steel_attention_nax credit is the
  house pattern); model weights are not in the repo and carry their own terms.

### From the MTP depth-N arc (2026-09-02)

Instruments: `harness/mtp_ruler.das` (both engines in one run, our released `dasllama-bench` exe
first from a parent that has loaded nothing, then a stock llama.cpp `llama-server` at the ref pin
6fdd0ac89, settle 180 s, `-n 128 -r 3`, the SpecBench-4 chat corpus, greedy on the identical
rendered thinking-off prompt; one record per box and model under `performance/records/mtp/`,
`--render` prints it); the round clocks (`DASLLAMA_MTP_DEBUG=time` on the exe at measurement time,
since folded into the `mtp.draft` / `mtp.verify` / `mtp.walk` / `mtp.replay` profiler sections
under `--jobque-profiling`); `harness/batch_rows_probe.das --sameslab` (ms per same-slab step at
B rows, `-jit`). Every ours-vs-llama.cpp pair is two processes and every before/after is two
commits: direction-grade.

- **gemma-4-26B-A4B-it-Q4_K_M + the Q8_0 assistant drafter, ruler on the M5 Max (7907b133c):**
  ours off 122.7 -> depth 1 145.9 (1.19x, accept 75.2%) -> depth 2 148.1 (1.22x; p1 73.9 p2
  51.3); per prompt depth 1 144.0 / 133.4 / 160.1 / 148.6 (writing / summarization / math / qa).
  llama.cpp off 98.2 / 95.8 / 98.4 / 98.2 -> n_max 1 139.6 / 119.8 / 145.8 / 129.5 (accept 81.4 /
  70.3 / 89.6 / 76.4%) -> n_max 2 130.8 / 126.0 / 163.4 / 147.9. Same drafter, same acceptance
  shape. Direction-grade.
- **Same pair on the M4 Pro (fa0b0eac4):** ours off 59.2 -> depth 1 67.5 (1.14x, 74.6%) ->
  depth 2 55.6 (0.93x); llama.cpp off 53.1 / 51.7 / 53.2 / 53.0 -> n_max 1 67.5 / 63.4 / 72.5 /
  65.4 -> n_max 2 58.0 / 56.0 / 72.6 / 63.4. Depth is a box knob the tuner mints
  (`mtp_depth_assistant`): 1 on the M4 Pro, a tie on the M5. Direction-grade.
- **Round clocks, gemma (`--ngl 99`, SpecBench-4 chat):** M5 plain step 8.15 ms, draft chain
  0.05 ms, verify+commit 11.8 ms - the two-row verify is 1.45x a step on the MoE expert union
  (1.51x on the M4 Pro); that, not the drafter, caps the gain (#88). The Q8_0 target accepts
  76.5% vs 74.6% on Q4_K_M (`lcpp_bench --mtp-ab`, M5): the target quant is not what caps
  acceptance. The pre-norm target hidden lost to post-norm on the same rail (#95).
- **Qwen3.8-27B-Q4_K_M + its split Q8_0 NextN head, ruler on the M5 Max (497dcf10a):** ours off
  27.6 -> depth 1 33.0 (1.20x, 77.4%) -> depth 2 31.4 (1.14x; p1 83.9 p2 66.3); llama.cpp off
  25.8 / 25.4 / 24.9 / 24.6 -> n_max 1 35.1 / 33.5 / 35.0 / 32.9 -> n_max 2 34.4 / 32.7 / 36.7 /
  30.0. Their verify row is cheaper on this dense hybrid (#85). Direction-grade.
- **Same pair on the M4 Pro (f591c4c13, the k4 pair walk in):** ours off 12.7 -> depth 1 14.0
  (1.10x, 77.4%) -> depth 2 12.4 (0.97x); llama.cpp off 11.5 -> n_max 1 13.6 / 13.8 / 14.5 / 13.5
  -> n_max 2 12.3 / 12.4 / 13.9 / 11.5. On the same box before the pair walk (47422ea68): depth 1
  13.9 (1.09x), depth 2 7.9 (0.62x). Direction-grade.
- **Round clocks, Qwen3.8-27B on the M4 Pro (`--ngl 99`, `-n 64 -r 2`, plain step 78.7 ms):**
  depth 1 per round draft 7.4 ms, verify 119.0 (59.5 per row, 1.51x a step), walk+commit 1.0,
  replay 0.7, tokens/round 1.80. Depth 2 before the pair walk: draft 14.1, verify 302.5 (the third
  row on the four-column form; 3.84x a step); after it: verify 194.4, the round 4.04x -> 2.65x a
  step. Direction-grade. CPU rails on the same box (ruler `--ngl 0`): off 11.56 -> depth 1 10.66
  (0.92x, 78.6% accept), identical at `--mtp-depth 2` (#94).
- **Row ladder, gemma-4-12B-it-Q4_K_M same-slab step on the M4 Pro (`batch_rows_probe --sameslab`,
  16 steps; ms per step / x a single step, before -> after the pair walk):** 1 row 44.1 / 1.00x
  both; 2 rows 52.9 / 1.20x both (the tile); 3 rows 148.1 / 3.36x -> 83.1 / 1.88x; 4 rows 156.7 /
  3.55x -> 103.1 / 2.34x; 5 rows 166.6 / 3.78x -> 133.5 / 3.03x; 8 rows (after only, the
  rebuilt tree on the archived m4 mint) 204.7 / 4.65x. A tile pair costs 1.2 single passes and
  the step is nearly all weight streaming, so pairs add linearly out to the eighth row. The M5 Max has no
  three-row cliff: 1 / 2 / 3 / 4 / 5 / 8 rows = 20.8 / 17.3 / 24.7 / 24.6 / 42.1 / 42.7 ms = 1.00 /
  0.83 / 1.19 / 1.19 / 2.03 / 2.06x - the walk stays off there by the crown. Direction-grade.
- **Depth ladder 1..4 on the M5 Max, both carriers (2026-09-03, ruler ours-only, settle 45 s -
  the off arms drift 27.3 -> 26.0 across the four Qwen arms, so the speeds are direction-grade;
  the acceptance counts are exact; records `mtp_m5_Qwen3.8-27B_depths.json` and
  `mtp_m5_gemma-4-26B-A4B_depths.json`):** Qwen3.8-27B + Q8_0 head: depth 1 27.3 -> 33.6
  (1.23x), depth 2 26.7 -> 31.6 (1.18x), depth 3 26.3 -> 34.4 (1.31x), depth 4 26.0 -> 19.7
  (0.76x); per-position acceptance at depth 4: p1 81.3 p2 64.3 p3 34.5 p4 22.4% of rounds (depth 3:
  86.4 / 69.3 / 35.2). gemma-26B + the assistant drafter: depth 1 112.2 -> 130.4 (1.16x), depth 2
  122.5 -> 146.5 (1.20x), depth 3 122.4 -> 139.5 (1.14x), depth 4 121.5 -> 112.1 (0.92x); at depth
  4: p1 71.7 p2 47.6 p3 33.2 p4 22.5%. Tokens per round 1.77 / 2.50 / 2.90 / 3.02 (Qwen) and 1.75 /
  2.25 / 2.52 / 2.75 (gemma): the fourth draft adds a tenth to a quarter of a token. The implied
  round cost in steps (tokens per round over the speedup) is 1.44 / 2.12 / 2.21 / 3.97 on Qwen
  and 1.51 / 1.88 / 2.21 / 2.99 on gemma. Depth 3 is the best Qwen point on this box today.
  Direction-grade.
- **The sampled walk (sample-and-match, #101; `lcpp_bench --mtp-ab --mtp-temp`, `-jit` debug rail,
  M5, depth 1, `-n 128 -r 1`, SpecBench-4 chat, both arms seeded alike; a `-jit` A/B enters as
  its ratio, the arms' absolute rates stay in the run's report):** gemma-26B + the assistant
  drafter at temp 0.8: on / off 1.26x, acceptance 74.7% - the greedy rate; the sampled off arm
  sits 6% under the greedy off arm (the 262k-vocab CPU sample per token, ~0.5 ms). Per prompt 1.26
  / 1.25 / 1.31 / 1.22x at 71.6 / 77.8 / 81.4 / 68.4%. At temp 2.0 the same pair accepts 29.9%
  (42.7 / 15.5 / 71.6 / 7.6% per prompt) and the round LOSES: 0.93x, the two cold prompts 0.81x and
  0.78x - the round's cost is paid whether the draws match or not (#103). An instruct model's chat
  distributions are peaked, so at serving temperatures the draw matches the draft about as often
  as the argmax did. Qwen3.8-27B + the Q8_0 NextN head at temp 0.8: 1.28x, acceptance 70.2% (greedy
  77%); per prompt 1.26 / 1.30 / 1.40 / 1.17x at 58.0 / 71.6 / 89.6 / 64.9%. Direction-grade. Board
  rows owed for both paths (`REVIEW_MEASUREMENT.md`'s re-mint duty): no `records/<box>.json` cell
  runs a sampled decode or a CPU MTP round yet, so the sampled-decode and CPU-round cells are
  minted with the arc's records-grade re-measure.
- **Where the CPU round's time goes (Qwen3.8-27B-Q4_K_M + the Q8_0 head, M5 Max CPU rails
  `--ngl 0`, `lcpp_bench --mtp-ab --prof` `-jit` debug rail, `-n 64 -r 1`, SpecBench-4 chat; the
  `mtp.draft` / `mtp.snapshot` / `mtp.verify` / `mtp.walk` / `mtp.replay` sections; plain step
  89 ms; a `-jit` A/B enters as its ratio):** on / off 0.78x at 82.3% acceptance, 1.82 tokens per
  round. Per round in steps: the two-row verify 1.79, the recurrent-state snapshot 0.25 (151 MB
  through a scalar copy loop, 22.7 ms), the reject re-forward 0.24 (a restore plus one plain step
  on 18% of the rounds), the draft 0.09 - 2.37 steps for 1.82 tokens. The verify's `mm_gemm` is
  1.75x a step's GEMVs because the kq batch tile is four-token granular and its token tail runs
  one GEMV pass per row: two rows stream and dequantize the weights twice; the padded-tile arm
  that was timed against it and refuted is followup #104's, with its numbers. LANDED: the
  snapshot as lane-parallel memcpy, 22.7 -> 1.0 ms per round, the round 2.37 -> 2.13 steps
  (0.78x -> 0.89x); verify and replay unchanged. The reject re-forward is the other lever (#105);
  with a two-token tile the round is predicted at ~1.5 steps, 1.2x. Direction-grade.
- **Where the NextN round's time goes (`lcpp_bench --prof`, `-jit` debug rail, Qwen3.8-27B on the
  M5, prompt 0 of the corpus, 128 tokens, plain step 37 ms):** depth 1, 75 rounds: draft 3.1 ms
  per draft (0.08 step), the two-row verify 45.7 ms (1.24x a step), walk 0.3, replay 0.5; depth 3,
  54 rounds: draft 3.0 ms per draft (9 ms per round), the four-row verify 66 ms (1.79x a step),
  walk 0.6, replay 1.3. The verify is 85-92% of the round; the draft chain is 6-11%. The round's
  four-row verify costs 13% more than the batch driver's four-row same-slab step (1.59x) - the
  head layer, the serial encoder and the host prep. **The gemma round the same way (gemma-26B +
  the assistant drafter, prompt 0, plain step 8.1 ms):** depth 1, 75 rounds: the fused draft chain
  0.03 ms per draft, the two-row verify 11.4 ms (1.41x a step), walk 0.01; depth 2, 63 rounds:
  the three-row verify 13.9 ms (1.72x). The verify is 99% of the gemma round - the routed experts
  re-streamed per row plus the batch step's own overhead (the batch driver's same-slab two-row step
  reads 10.4 ms against this 8.1 ms greedy step). Direction-grade.
- **A code prompt, gemma on the M5 (c3239b46c, `benchmarks/data/code1_prompts.txt`):** ours off
  123.4 -> depth 1 159.0 (1.29x, 85.5%) -> depth 2 174.0 (1.41x; p1 86.0 p2 68.0); llama.cpp off
  99.7 -> n_max 1 140.0 (84.1%) -> n_max 2 153.7. Acceptance is a property of the text: code
  drafts a fifth better than chat. Direction-grade.
- **CPU ruler leg, Qwen3.8-27B on the M5 (4da0045f0, `--ngl 0`; llama.cpp arms only - our
  in-process CPU greedy dies on this model, #91):** llama.cpp off 9.6 / 9.4 / 9.6 / 9.6 -> n_max 1
  8.4 / 9.0 / 9.5 / 8.4. Speculation loses on their CPU rail too.
- **Context sweep on the tuned m5 mint:** the dev rail under a binary-stale sidecar served no
  `runtime.metal_tensor` crowns and read prefill at well under half the board (fixed: the runtime
  section of a binary-stale sidecar now applies, #86 step 1); the 08-30 rig exe read gemma-26B
  pp512 3861 the same day - the board row (3868) stands. The sweep's own rows are #84's,
  direction-grade on the dev rail.
- **Footprint:** the assistant drafter is a 440 MiB Q8_0 sidecar resident for the session (the
  `mtp-gdraft-load` arm asserts its size) plus its GPU workspace; a session with a head or drafter
  grows `mtp_cat` to 2 x 9 x dim floats (Qwen3.8-27B: 370 KB) and `mtp_logits_b` to 9 x vocab
  floats once (about 5 MB). Decision: taken - the round's gain rides on the drafter's presence, and
  the sidecar is a fraction of a percent of the target it drafts for.

### From the Pocket small form (2026-09-10)

Instruments: `harness/tts_synth.das` (`-jit -module-cache`, alba, the first 60 sentences of the
rig corpus, M1 Max, the box's tune profile, JIT cache warm) with the process's resident set read
through `ps -o rss` every half second, two reps per file; `harness/tts_rig.py` (alba, the 200
sentences, parakeet WER + UTMOS) per weight lane. Every pair here is two processes on one box:
direction-grade.

- **The English file, the q8 form against the small form (`pocket-tts-en-q8.gguf` 152 MB,
  `pocket-tts-en-kq.gguf` 75 MB; the one-voice `pocket-tts-en-stuart-kq.gguf` 65 MB):** the
  compiled program before the load reads 1.87 GB on both; the load adds 0.66 GB on the q8 file
  and 0.50 on the kq file; over the synthesis the q8 process holds 2.54 GB and the kq process
  2.70 - the kq lane steps up 0.33 GB at its first synthesis and stays there, the q8 lane does not
  (reps within 2 MB). RTF over the 60 sentences: q8 0.0510 / 0.0507, kq 0.0435 / 0.0434 - the
  small file decodes 15% faster and its process is 6% larger. The step is followup 129's.
- **The kq file's three lanes on the rig (the q8 file 3.91 / 4.328, its f32 lane through the f16
  file 4.32 / 4.366):** native 3.86 / 4.295, the q8 pin 3.73 / 4.295, the f32 pin 3.86 / 4.330.
  The lanes agree within the rig's own spread; the small form loses nothing the rig can hear.
  Decision: taken - the browser pages read the small forms (storywish the one-voice file, parrot
  the 19-voice one), the q8 file stays the desktop default of the served set.

### From the Vulkan batched-decode arc (2026-09-19)

Instruments: `daslang -jit benchmarks/lcpp_bench.das --npl 4` (the flat rows and the four-stream row
through the scheduler's device mode; the vulkan tier in its cm2 mode and in its KHR (mm) mode as two
arms, kv f16, the box's tuned sidecar - `DASLLAMA_ALLOW_UNTUNED=1` on the pod, whose sidecar is
untuned - `DAS_JOBQUE_THREADS=16` locally and the pod's 48 vCPUs; `DASLLAMA_GPU_PROF=1` for the token
command's stamps, which itself costs ~5% of the rate, so every pair below is profiler-on against
profiler-on or off against off), the attention ruler `harness/vk_attn_probe.das` (device timestamps
a layer), the copy ruler `harness/vk_dma_probe.das`; the reference exe llama.cpp b10660
`llama-batched-bench -m <model> -c 4096 -b 2048 -ub 512 -npp 512 -ntg 128 -npl 1,4 -ngl 99 -fa on`
on both boxes the same hour, with `GGML_VK_PERF_LOGGER=1` for its per-kernel reads. The `tg128@4`
row is the summed served rate over the scheduler step whole - sampling, detokenization and the event
list inside the clock - where the batched reference exe times the decode call alone, so every ratio
against it is conservative. The pod's RTX PRO 4500 (48 vCPUs, cm2 without decode-vector) unless
named; the local RTX 5060 Ti carries decode-vector.

- **The worker spin window on a GPU-served step (pod, Llama-3.2-1B Q8_0, 16 lanes, profiler off):**
  the scheduler's greedy step runs a team-parallel argmax a row, which wakes every worker, and each
  then spins the 30 ms window across the whole GPU step (all sixteen at 100% in `top`); on the
  48-vCPU VM that starves the driver's submission thread - the four-row command's head read 2.7 ms.
  tg128@4 summed by window: 30 ms 707, 8 ms 698, 4 ms 734, 2 ms 1065, 1 ms 1058, 500 us 1053,
  100 us 1046, 0 (park at once) 1005 - the knee is the step's own length. Shipped: 500 us while the
  resident driver serves (`jobque_spin_gpu_us`); 1B 707 -> 1067, 3B 362 -> 538, 8B 269 -> 364
  (llama.cpp 1228 / 607 / 275, `external`). The flat rows unchanged (the flat bench feeds synthetic
  ids and never samples); a `--npl 4` run loads the model at four regions, each region's context a
  quarter of the plan's, so this section's flat rows are the four-region model's - the plain
  one-region flat rows are the ones the earlier entries carry [direction-grade - two commits].
- **The decode attention at four rows (pod, profiler on, us a step over 16 layers):** the
  committed pass (a workgroup a head, two keys a step behind a subgroup reduction) 380 + 70
  combine at its rule's two splits, 327 + 102 at six, 346 + 103 at twelve - the split does not
  move it; the shipped grouped pass (a workgroup a kv head's four q heads, the accumulators in
  registers and the V words prefetched) 361 with the combine fused into the last piece. The
  ruler's per-layer floor at 32 keys unsplit is 8.3 us and a split adds 4 (the release, the
  atomic, the last piece's combine); the ruler reads 14.5 at four rows and 12.6 at one where the
  committed pass's profile read 28 and 8.5 - so the four-row form gains and the one-row form pays
  ~4 us a layer, which the transfer-queue logits repay several times over. The score FMAs, the
  softmax reductions, the K loads and the V loads each price ~2 us of an 18.5 us unsplit layer on
  the 5060 Ti, the finish 4: no single cost dominates, the chain of them does
  [direction-grade - two commits].
- **The logits' trip to host memory (the copy ruler, cached host memory):** the pod's compute-queue
  copy 4.30 GB/s at 512 KB, 2 MB, 8 MB and 32 MB alike (0.95 us a 4 KB page); the transfer queue
  11.2 / 19.4 / 24.7 / 26.8 GB/s including its submit and wait (108 us for the four-row plane
  against 487). The 5060 Ti: 6.3 / 5.7 / 5.6 / 5.5 against 5.1 / 9.8 / 12.7 / 13.8. An uncached
  host target leaves the copy at 471 us and slows the host's read to 7.8 ms; the classifier
  writing straight into host memory costs 26.7 ms. Shipped: the transfer-queue copy behind a
  compute -> transfer timeline. Pod, 1B, profiler on: the four-row step 3761 -> 3371 us, tg128@4
  1019 -> 1136, tg128 426 -> 451 [direction-grade - two commits].
- **The scoreboard after the three (pod, profiler off, tg128@4 summed, ours cm2 / ours KHR /
  llama.cpp, the llama.cpp figure `external`; then tg128 ours / theirs, theirs `external`):**
  Llama-3.2-1B Q8_0 1207 / 1206 / 1217 (0.99), flat 464 / 404; Llama-3.2-3B Q8_0 580 / 579 / 605
  (0.96), flat 194 / 184; Llama-3.1-8B Q4_K_M 379 / 379 / 361 (1.05), flat 134 / 132. The RTX
  5060 Ti the same session (cm2 with decode-vector, `-c 4096` on both sides): 1B 724 / 744 / 738
  (0.98 / 1.01), flat 258 / 242; 3B 351 / 352 / 351 (1.00), flat 107 / 104; 8B 250 / 250 / 209
  (1.20), flat 78 / 77. The 1B's four-row step under the profiler: GPU 2994 us (qkv 231, rope 65,
  attn 361, wo 210, gate 486, up 457, down 588, cls 332, the norm and requant sites 250), host 375
  (the logits' landing 285, the submit 9); llama.cpp's step 3.26 ms wall with 2.82 of kernels
  (`external`, its logger's rows): its `MUL_MAT_VEC` GEMVs 655 GB/s (`external`) to our
  `Q8GemvNT`'s 540-600 on the long-K down plane, its `FLASH_ATTN_EXT` 251 (`external`) to our
  `DaAttnT`'s 361 [direction-grade - two processes].
- **The nb-row planes' footprint, the decision `RD_NB_MAX = 8` rests on:** at nb rows every
  per-token device plane (x, xb, xq, xs, kv, q, attn, aq, as, gate, up, ffnout, xb2, cos, logits)
  is nb times its one-row size, and the partials plane is nb times. Llama-3.2-1B (dim 2048, hidden
  8192, qd 2048, kvd 512, vocab 128256, 16 layers of 32 heads at head 64), one row: x, xb, xb2 and
  ffnout 4 x 2048 x 4 B = 32 KB; xq 2048 B and xs 64 x 4 B; kv (2048 + 2 x 512) x 4 = 12 KB; q 8 KB;
  attn 8 KB; aq 2048 B and as 256 B; gate and up 32 KB each; cos one rope table row, 2 x 32 x 4 =
  256 B; logits 128256 x 4 = 513 KB; the partials 32 heads x 1 split (the pod) x 66 x 4 B = 8.4 KB
  plus the counters - 0.65 MB a row, the logits four fifths of it: 0.65 MB at nb 1, 2.6 MB at nb 4,
  5.2 MB at nb 8, against a weight pass the rows share, which streams the whole model once a step
  whatever nb. The mirror's share: `moe_gpu_binding_cap` takes `RDEC_MIR_SLACK` x 4 x regions bytes
  off the mirror's binding range (a fragment's slack rows past each region's last plane), and the
  context is negotiated inside what is left; the per-row planes cost no context. Decision: taken -
  the planes are megabytes against a weight pass shared across the rows, so the row cap is the
  command's, not memory's.

### From the Metal batched-decode arc (2026-09-20)

Instruments: `daslang -jit benchmarks/lcpp_bench.das --npl 4 --ngl 99 -r 5 --ref <llama-bench>`
on the M5 Max (the four-stream row host-cached through the scheduler and the Metal batch driver,
kv f16, `DAS_TUNE_POLICY` unset - the tuned tier - with the box's fresh rig sidecar pinned through
`DAS_TUNE_MANIFEST=performance/m5.tune.json`, the box idle - the iOS Simulator and Parsec off, a
90 s settle between cells); the flat reference rows are the pinned `llama-bench` under
`lcpp_bench`'s `--ref` spawn (`-m <gguf> -ngl 99 -t 18 -p 512 -n 128 -r 5 -o json`) and the batched
reference row is `llama-batched-bench` beside it at `-ngl 99 -t 18 -c 4096 -b 2048 -ub 512
-npp 512 -ntg 128 -npl 4 -fa on`, five runs folded. The `tg128@4` row is the summed served rate
over the scheduler step whole, the reference times its decode call alone, so every ratio reads
conservative for ours; every row here is a `-jit` script reading against a reference in another
process, so every bullet is [direction-grade - two processes] unless it says one process.

- **The scoreboard (ours / llama.cpp, tg128@4 summed; then flat tg128 ours / theirs), all Q8_0
  [direction-grade - two processes]:**
  Llama-3.2-1B 944 +/- 2 / 1031 +/- 55 (0.92), flat 323 / 317; Llama-3.2-3B 430 +/- 18 / 430 +/- 30 (1.00),
  flat 139 / 134; Llama-3.1-8B 208 +/- 2 / 199 +/- 10 (1.04), flat 67 / 64; gemma-4-E2B 441 +/- 15 /
  465 +/- 19 (0.95), flat 160 / 136 - the E2B row measured with `-p 0`: behind five pp512 reps
  in the same process it read 389 +/- 23 (0.84), a heat shadow the 1B does not cast (944 either
  way), so the batched row of a small hot model is read without a prefill row in front of it or
  after a settle. The 3B row reads a cv past 3% and stands as direction-grade until a re-run; the
  reference's E2B batched row moved between 234 and 465 across the day's runs, so that ratio's
  denominator is unsettled [direction-grade - two processes].
- **The E-series batch arm (gemma-4-E2B Q8):** per-row steps read ~90 tok/s summed at four streams
  (each stream a weight pass a token); the batched step with the PLE rows form, the Q-only shared-KV
  rows and the two-width panels reads 389 - the same step's flat row unchanged at 152 [direction-grade].
- **The deltanet batch arm (Qwen3.5-0.8B Q8, the recurrent rows form - projections as rows GEMVs,
  the conv, history, norm, scan and gate one dispatch a row against that session's mirror):**
  tg128@4 982.6 +/- 21.3 against llama.cpp's 795.7 +/- 7.2 (1.23); the flat tg128 402.1 +/- 1.1 on the
  arm's tree against 401.9 +/- 1.0 on the tree before it - the row index the five kernels gained
  costs the single row nothing. Per-row dispatch stands at ~360 small dispatches a four-row step
  on this 24-layer hybrid and still reads 2.4x the flat row; the state arena that folds them into
  one dispatch a stage is `followup_metal.md` item 22 [direction-grade - two processes].
- **The 1B's four-row step, attributed (the knockout rail `set_metal_decode_skip` under the board's
  own rep, one process, heat drifting 937 -> 865 across the ladder):** full 4267 us a step; the
  weight sites out 1952 (~2.3 ms of weights, 1.24 GB at ~540 GB/s - the memory roof); attention out
  3438 (~830 us, 52 us a layer, five times its byte roof); elementwise out 4134; the qkv sites out
  4076. The stage report (`--prof` after the batched row): GPU 3807, encode 156, handoff 70, readback
  34, setup 22 us a step. The worker spin window (30 ms against 500 us) moves nothing on this box:
  942.8 against 942.0 [direction-grade - one process].
- **The split single-pass attention serves a head of 64 (the 1B's) on the f16/f32 mirrors, where the
  batch took the chunked per-(row, head) pair [direction-grade - two commits, two processes for the
  board numbers; the forms raced interleaved in ONE process below]:** a lane owns one quad of the head, the lanes past a
  narrow head re-read a valid quad with a zero query and never store, the combine merges four subgroups
  at 128 threads whatever the head. Llama-3.2-1B Q8 tg128@4 942 -> 1024 +/- 10 against llama.cpp's
  1026 (0.92 -> 1.00); the flat tg128 unchanged at 324; the head-128 kernel cells bit-exact before
  and after; the batch parity arm's f16 and f32 rows within their bars on the new form. The q8_0 and
  tq4 mirrors keep the chunked pair at head 64 (the quant twin's lane mapping is its own). The
  two forms raced interleaved in one process at the served shape (`set_metal_attn_d` flipped
  between reps of the four-stream row on the 1B, five pairs, the first pair the warm-up): the
  split form 815-951 against the chunked pair 699-858 tok/s summed, the split form ahead in every
  pair after the first (rep 1: 950.7 against 857.7; rep 3: 920.8 against 699.1); the reps drift
  past a 3% cv in the un-settled process, so the ranking is settled and the margin is not
  [direction-grade - one process, cv past 3%].
- **The NextN carriers' batched rows, plain and self-speculative (three reps, `-p 0`):** Qwen3.5-0.8B-MTP
  Q8_0 plain tg128@4 977 +/- 4 against llama.cpp's 803 +/- 4 (1.22), flat 402 / 303; Qwen3.6-27B-MTP
  Q4_K_M plain 56.3 +/- 0.3 against 45.9 +/- 2.1 (1.23), flat 27.6 / 25.3 - the batched step lands a
  NextN model's logits on the GPU like any other's now (the gate that sent them to the CPU classifier
  is gone) and carries each row's post-norm hidden as its stream's speculative carry. The
  `--npl-mtp` arm - every stream drafting on its own chain, the four streams' eight verify rows in ONE
  pass - reads 720 +/- 11 on the 0.8B and 50.3 +/- 3.6 (cv 7%) on the 27B: below the plain row on both,
  on the bench's synthetic ids, where the drafts are accepted as often as noise and the round pays
  four draft passes (each a NextN layer plus the 248k-row classifier) and a double-width verify for
  them. The arm proves the joint verify at four streams; its served rate is the ruler's question
  (`ARCHITECTURE_MEASUREMENT.md#ruler-records` of the measurement doc) and the batched draft pass - the four drafts as rows of one
  dispatch - is the lever the ledger row names [direction-grade - synthetic ids, two processes].
- **The sidecar reaches the batched row:** the 1B's tg128@4 read 840 under the shipped class profile
  (`DASLLAMA_ALLOW_UNTUNED=1`) and 944 under the box's fresh mint in the same tree - the runtime knobs
  and the Metal crowns are part of the step, not only its provenance [direction-grade - two processes].
- **A loaded box voids the batched row first [direction-grade - two processes, a loaded box]:** with an iOS Simulator rendering (two WebContent
  processes at 98% and 48%, SimMetalHost on the GPU, load 12) the 1B batched row read 466 +/- 277 and
  the reference's own flat tg128 fell from 317 to 200 - the GPU queue is shared with the simulator's
  Metal clients, and the step's CPU half (sampler, encode, spin) loses its performance cores. The
  amortized weight stream leaves dispatch latency and CPU work as what is left per step, so four rows
  feel the load where one row hides it.
### From the close-all-gaps arc, the Metal catalog rows and the CPU hybrid step (2026-09-21)

Instruments: the released bench exe (`daspkg release --root modules/dasLLAMA/benchmarks --out
modules/dasLLAMA/performance/_rig`, its own fresh mint - noise ok, validation ok, 62 kernels - and
DAS_TUNE_MANIFEST unset) at `-p 0 -n 128 -r 5 --npl 4 --ngl 99 --ref <llama-bench>` on the idle M5
Max, the reference rows as the Metal batched-decode section's; the CPU row is the `-jit` script under
`--for-debug-purposes` (the arm's tree, not the released exe) at `--npl 4` with no `--ngl`, its
reference `llama-batched-bench` at `-ngl 0`. Every ratio is tg128@4 summed over four host-cached
streams, the step served by the batch driver (the row refuses otherwise) [direction-grade - two
processes throughout].

- **The Metal rows the E-series, shared-expert and deltanet arms unlocked, all Q8_0 (ours /
  llama.cpp, tg128@4; then flat tg128 ours / theirs):** Qwen3.5-4B 232 +/- 17 / 220 +/- 2 (1.05)
  read first, in the mint's heat shadow, then 295 +/- 12 / 248 +/- 10 (1.19) on the settled box -
  both sides moved, so the first read is the shadow, not the arm; flat 108 / 93. Qwen3-30B-A3B
  256 +/- 15 / 215 +/- 3 (1.19), flat 123 / 105. Qwen3.6-35B-A3B 232 +/- 7 / 198 +/- 0 (1.17), flat
  122 / 91. The 4B and 30B rows read a cv past 3% and stand as direction-grade until a re-run.
- **The batched-step decline census (`harness/batch_decline_census.das`: one four-stream
  host-cached row per carrier, then `batch_step_census` and the driver's declines-by-reason
  table):** every official catalog row that loads - gemma-4-E2B and E4B Q8, Qwen3VL-4B Q8,
  Qwen2.5-Omni-3B Q8, gemma-4-12B Q4_K_M, gpt-oss-20b mxfp4, Mistral-Small-24B Q4_K_M,
  Qwen3.6-27B-MTP Q4_K_M, Qwen3-30B-A3B Q4_K_M, Qwen3.8-27B Q4_K_M, Qwen3.6-35B-A3B-MTP UD-Q4_K_M -
  and the Q8 board carriers (Llama-3.2-1B/3B, Llama-3.1-8B, Qwen3.5-0.8B and its MTP twin,
  Qwen3.5-4B, Qwen3-30B-A3B, Qwen3.6-35B-A3B) served eleven device steps, no CPU-stack step, no
  decline; the one per-row step each is the row's one-stream tail, per-row by definition. The
  gemma-4-26B-A4B Q4_K_M row never stepped: its image mint declined on the map-back (two sections
  named `q51q` - the legacy top-level Q5_1 plane pair beside the per-format table's q51 slot, the
  streaming writer keyed by name), the defect the arc fixes.
- **The CPU batched stack's hybrid form (Qwen3.5-0.8B Q8, the deltanet rows form per session +
  the gated attention rows, no device driver):** tg128@4 517 +/- 8 against llama.cpp's 471 +/- 10
  at `-ngl 0` (1.10); the flat tg128 208 +/- 18 / 178 - the batched step reads 2.5x the flat row
  [direction-grade - two processes, the `-jit` script].
- **The block codecs on the partial-rope carrier (the q8_0 / tq4 rope-store kernels take `rot`):**
  the census (`harness/batch_decline_census.das -- --kv q8_0 | tq4 <gguf>`, the `-jit` script,
  the untuned tier) serves Qwen3.5-0.8B Q8 and Llama-3.2-1B Q8 batched at four streams on both
  codecs, no decline. The tq4 signs table read past its 128 floats on every 256-wide head: the
  support matrix's `fam-qwen35` row (`tests/run.das -- --suite matrix --arm fam-qwen35 --family
  qwen35`, the Metal-override generate against the CPU truth token-for-token, per KV codec) reds
  on tq4 at master and passes with the table at the kernels' 512 ceiling; the forced-feed probe
  (`harness/forced_feed_probe.das -- <gguf> --kv tq4 --steps 6` - a CPU prefill, then six steps
  fed the CPU chain's tokens on a CPU and a GPU-decode session, per-step logits maxd printed)
  localized it to the tq4 rows of every 256-wide head, gemma-2-2b Q4_K_M included, with argmax
  flips on the unfixed table and none on q8_0 [one process, the M5 Max].
- **Nine streams (the census at `--npl 9`):** every non-hybrid K-quant catalog carrier serves
  batched - gemma-4-12B Q4_K_M, Llama-3.2-1B Q4_K_M, Qwen3-30B-A3B Q4_K_M, Mistral-Small-24B,
  gemma-4-26B-A4B Q4_K_M, gemma-4-E2B Q8 - no off-lattice decline in the catalog; every hybrid
  declined `dn_state` (the four-mirror LRU evicting the rows of the step it prepared) and the Q8
  hybrid then faulted on the device (nine rows written into nine-row planes by the rows form's
  four-row tiles); with the cache grown to the batch and the planes sized to the tile
  Qwen3.5-4B Q4_K_M, Qwen3.6-35B-A3B UD-Q4_K_M and Qwen3.5-0.8B Q8 serve at nine, the 0.8B at
  twelve too.
- **The drafts as rows steps (the self-speculative batched row, `--npl-mtp`, three or five reps,
  `-p 0`):** Qwen3.5-0.8B-MTP Q8 spec @4 720 -> 881 +/- 10 against the plain row's 987 +/- 47 in the
  same session; Qwen3.6-27B-MTP Q4_K_M read spec 41.4 +/- 7.0 (cv 17%, void) beside plain 54.4 in
  one pair and spec 45.3 +/- 2.3 beside plain 41.8 +/- 0.2 in the next, the plain row itself moving
  54 -> 42 between the pairs - the box's heat under the untuned debug script [direction-grade - the
  `-jit` script, untuned, synthetic ids]. The released exe on the idle box (five reps, `-p 0`,
  its own fresh sidecar): Qwen3.5-0.8B-MTP plain 977 +/- 6 against llama.cpp's 811 (1.21), spec
  890 +/- 9 - 0.91 of the plain row where the per-stream chain read 0.74, and above the
  reference's plain row (1.10); Qwen3.6-27B-MTP plain 52.2 +/- 2.4 against 38.6 (1.35), spec
  47.5 +/- 5.8 (cv 12%) - 0.91 of plain against 0.89 before, the bar too wide to rank the two
  draft forms on this carrier; on synthetic ids every accepted draft is noise, so the spec row's
  ceiling is the plain row and the served rate is the ruler's question (`ARCHITECTURE_MEASUREMENT.md#ruler-records` of the
  measurement doc) [direction-grade - two processes].

### From the server-MTP arc, the batched pre-encoded step (2026-09-21)

- **What the pre-encoded step holds (`ARCHITECTURE_GPU_MTP_DECODE.md#batch-pre-encode`).** A pre-encoded
  batched step holds its own set of `batch_step_build`'s pooled buffers beside the step in flight, so
  the pool's peak grows by one step's set. The bytes are `batch_step_build`'s own sizing at four rows
  (the server's default stream count, the fixed-row forms' four-row pad) over each carrier's loaded
  `Config`, each buffer rounded up to the pool's power-of-two bucket. The fixed part - the rows'
  activations, the FFN pair and the logits plane - is 3.5 to 5.6 MB on the catalog's large carriers
  (gemma-4-26B-A4B 4.6 MB, Qwen3.8-27B 5.6 MB, gpt-oss-20b 4.5 MB, Mistral-Small-3.1-24B 3.5 MB,
  gemma-4-E4B 4.9 MB). A carrier the fused partD attention does not serve (a head size other than
  64 or 128, mixed head sizes, a sliding window, sinks) also holds the chunked attention's partials:
  rows x heads x (largest head size + 2) x 4 bytes per 64-key chunk of the deepest row. At each
  carrier's trained context that is gemma-4-26B-A4B 539 MB at 262144 keys (a 1 GiB bucket),
  Qwen3.8-27B 406 MB at 262144 (512 MiB), gpt-oss-20b 138 MB at 131072 (256 MiB), gemma-4-E4B
  135 MB at 131072 (256 MiB); Mistral-Small-3.1-24B rides partD and holds none. Decision: the rail
  ships off (`DASLLAMA_METAL_BATCH_PRE=1` arms it) - the E4B four-row step times the same with it
  on and off (`followup_metal.md` row 23), so the second set buys nothing until a step chained on
  the GPU's own picks lands on top of it.

### From the Vulkan batched-decode arc, the qwen and phi carriers (2026-09-20)

Instruments as the section above: `daslang -jit benchmarks/lcpp_bench.das --npl 4` on the pod (RTX
PRO 4500, driver 580.173, cm2 without decode-vector, and KHR) and the local RTX 5060 Ti (driver
616.56, cm2 with decode-vector), `DASLLAMA_ALLOW_UNTUNED=1` and no `DAS_TUNE_POLICY` override on
either box (an untuned sidecar serves the reference bodies), `DAS_JOBQUE_THREADS=16`, llama.cpp
b10660's `llama-batched-bench` under the section above's command line the same hour on the same
box (every llama.cpp figure below `external`, from that exe's own table), `DASLLAMA_GPU_PROF=1`
for the stamps, the attention ruler `harness/vk_attn_probe.das` for the key split. Every ratio is
`tg128@4` summed over four device-home streams through the scheduler's device mode against the
reference's `S_TG` at `-npl 4`, conservative as before (the served step samples and detokenizes
inside the clock); every lever's before -> after pair is two commits in two processes
[direction-grade - two commits], every ratio against llama.cpp two processes [direction-grade -
two processes]. The wide twin's command buffers (one a region, one a row count) are pool handles
the recorder fills - no plane grows with them, so the ladder's footprint half is nil.

- **The baseline (master before the arc, the pod):** qwen2 0.5B 1205 against 1385 (0.87 - batched,
  a gap); qwen3 0.6B 425 against 1193 and 4B 129 against 473 (0.36 and 0.27 - not batched: the N-row
  command answered zero rows for a q/k-norm model, so the scheduler stepped the four streams a row
  at a time, four weight passes a step, below one stream's rate).
- **The q/k-norm form of the N-row command:** the per-head rms kernel was already row-indexed, and
  the fused norm+rope+store kernel gained the row in its workgroup id; 0.6B 425 -> 1044, 4B 129 ->
  427 (0.87 and 0.90). The fused kernel and the split pair round apart (the compiler contracts
  each kernel on its own - the earlier "bit-identical" claim was false), so the rows take the
  form the one-row command takes and read bit for bit. On the 5060 Ti's Windows driver the
  one-row form then read twelve logits off the CPU chain: the new row arithmetic divided by a
  push field that is zero in the one-row form behind a select, and that driver evaluates both
  arms - an integer division by zero is undefined in SPIR-V. The divisor is clamped;
  `DASLLAMA_VK_FUSE_BISECT` names such a kernel in seven runs against the CPU chain.
- **The key split follows the attended span, not the SM count (the ruler, us a layer, qwen2 0.5B
  geometry, the pod):** at 640 positions one piece 14.5 and sixteen 33; at 2048 four 20.6 and
  sixteen 33; at 4096 four to eight 27 and sixteen 34; one row or four the same at four pieces or
  fewer. The old rule recorded sixteen for one row and eleven for four; the ladder (one under 512,
  four to 3072, eight past it) cut the 0.5B's four-row attention 793 -> 446 us a step, and the
  qwen3 0.6B's 767 stays bandwidth-bound (eight kv heads of 128, the rows' K/V bytes eight times
  the 0.5B's, linear in rows). It also puts the one-row and N-row forms in one summation order,
  so the qwen3 regions cells read bit for bit on the 36-SM card where the rules had them nine and
  sixteen pieces apart. Pod: 0.5B 1356, 0.6B 1111, 4B 449 (0.98, 0.92, 0.95).
- **A residual epilogue fused into the GEMV's last workgroup does not batch (Decision: rejected,
  the tree keeps the separate residual dispatch):**
  the N-column form of `Q8GemvAr`, bit-exact in its kernel cell and the regions cells, read the
  0.5B 1076 -> 988 and the 0.6B 822 -> 784 on the 5060 Ti: the last workgroup runs the rows'
  residual steps one after another where the separate `cls_ar_rq_b` dispatch runs them in
  parallel workgroups. The fused gate-up's N form (`Q8GemvGuN`, a subgroup a column quantizes)
  batches and is bit-exact, and bought nothing measurable on either card (pod 0.5B 1331, 0.6B
  1103, 4B 454); kept for the dispatch it saves.
- **The rows' logits a row a job-queue lane (`rd_land_logits_n`):** the one-lane form passed
  2.4 MB of a 152k-vocab plane twice, 322 us of a 3037 us step on the pod; the lanes read the
  plane once. 5060 Ti: 0.5B 1068 -> 1172, 0.6B 833 -> 895. A parallel sample of the rows threw in
  a forked context and stays `followup_vulkan.md` item 76.
- **The Q8_0 N-column GEMV's column loads issue together (`Q8GemvNT`):** the qwen2 1.5B's
  four-row step on the pod read 4313 us against the one-row step's 2963, and the stamps put the
  gap in the N-column GEMVs - the down GEMV 1144 against the one-row form's 694 (n 8960, d
  1536), the qkv 340 against 229, the wo 289 against the fused 289 plus a 168 requant - where
  the fused gate-up at n 1536 sat 14 percent over its one-row twin. Each column's activation
  load sat behind its own `c < ncols` branch, so the four loads issued one after another, an L2
  latency each, per weight word. Unguarded (a column past the live ones dots the last live
  column's row again, its sum never stored), the step reads 3775: down 744, qkv 286, wo 230.
  Pod, cm2 / KHR: 1.5B 871 -> 980 / 982, 0.5B 1443 -> 1615 / 1604, 0.6B 1154 -> 1261 / 1251, 4B
  459 -> 482 / 481; the 5060 Ti 0.5B 1172 -> 1246, 1.5B 615 -> 621 (its decode-vector rail hid
  most of the serialisation). Bit for bit in the kernel cell and the regions cells.
- **The ruler on both sides of the ladder's edges (us a layer, the pod, one row / four rows):** the 0.5B
  geometry at 384 one piece 10.4 / 10.4 against four 16.5 / 16.5, at 640 14.5 / 14.5 against 16.5 /
  16.5, at 1024 18.6 / 18.6 against 16.5 / 16.5, at 1536 26.7 / 26.7 against 18.6 / 20.6 - the
  one-to-four edge sits between 640 and 1024 for this geometry; the qwen3 0.6B geometry (head 128,
  two heads a kv head) at 384 one piece 14.5 / 14.5 against four 12.4 / 18.6, at 640 20.6 / 20.6
  against 14.4 / 20.8, at 1024 28.8 / 28.8 against 14.5 / 24.6 - its edge sits at 512 for one row
  and near 640 for four. `RD_UNSPLIT_POS` stays 512: the wrong side of either edge costs two
  microseconds a layer. The wide edge: at 8192 eight pieces read 32.9 against four's 41.1 for one
  row of the 0.5B geometry (37.3 against 59.2 for the 0.6B's) and 51.3 against 44.3 for four rows
  (189 against 181); at 16384 47.3 against 71.6 for one row, 81.1 against 73.8 for four. Decision:
  taken - the one-row command keeps its wide twin from 3072, the N-row command takes the split
  form at every span past 512 and its wide twin, its recorded flag and its stamp slots go.
- **The fused gate-up N form keeps its column guard (Decision: rejected - the unguarded form):**
  the same clamp that freed the plain N-column GEMV's loads made `Q8GemvGuN` slower - the 1.5B's
  four-row gate-up 1212 -> 1252 us a step on the pod, the step 3775 -> 3847, tg128@4 980 -> 965,
  the 4B 482 -> 477 and the llama 1B 1343 -> 1325 - because its unroll is eight columns wide
  whatever the count where the plain GEMV picks a stamp by count: a dead column's two dots against
  both planes cost more than its branch. The stamp by column count is `followup_vulkan.md` item
  77's fold. The ladder caps a sliding-window layer's count at its window's span: a windowed
  carrier (gemma-3's 512, gemma-4's 1024) attends at most its window, so its local layers take
  one piece at every position where the position alone read four or eight; no windowed carrier
  sits on this arc's board, so the count is the ruler's reading, not a measured step.
- **The boards at the arc's tip, tg128@4 against llama.cpp the same hour** - the pod, cm2 / KHR:
  qwen2 0.5B 1611 / 1602 against 1373 (1.17 / 1.17), qwen2 1.5B 980 / 981 against 920 (1.07 /
  1.07 - the flat row 335 against 314), qwen3 0.6B 1267 / 1251 against 1199 (1.06 / 1.04), qwen3
  4B 483 / 481 against 476 (1.01 / 1.01), Phi-3.5-mini Q4_K_M 524 / 522 against 418 (1.25 / 1.25,
  the K-quant leaves the lever does not reach); the llama family at the same tip 1B 1347 / 1345
  against 1220 (1.10 / 1.10), 3B 613 / 612 against 606 (1.01 / 1.01), 8B 380 / 378 against 361
  (1.05 / 1.05). The
  5060 Ti, cm2 with decode-vector: 0.5B 1211 against 922 (1.31), 1.5B 624 against 578 (1.08),
  0.6B 915 against 740 (1.24), 4B 273 against 263 (1.04, under `DASLLAMA_GPU_VRAM_MB=9000`: the
  16 GB card holds 3.5 GB of desktop and the four-region mirror otherwise pages), Phi-3.5-mini
  Q4_K_M 348 against 317 (1.10 - an earlier reference row of 183 on this box was the outlier: the
  reference re-run the same hour as the CUDA rows below read 924 / 577 / 743 / 266 / 317 for the
  five, the first four within a percent of the rows above); the desktop box reads three percent
  apart run to run on the 0.5B (1246 the run before). The flat rows did not move.
- **llama.cpp's CUDA build beside the Vulkan reference on the 5060 Ti (every figure `external`;
  the same b10660 checkout built with CUDA 13.4 for sm_120, `llama-bench -ngl 99 -fa 1 -r 3` for
  pp512 / tg128 and `llama-batched-bench` under the section's command line for tg128@4; ours the
  rows above, not re-run) [direction-grade - two processes]:** pp512 / tg128 / tg128@4 - Llama-3.2-1B
  Q8_0 21185 / 262 / 857, Llama-3.2-3B Q8_0 8628 / 108 / 371, Llama-3.1-8B Q4_K_M 3630 / 82 / 245,
  Qwen2.5-0.5B Q8_0 34230 / 488 / 1412, Qwen2.5-1.5B Q8_0 14831 / 202 / 652, Qwen3-0.6B Q8_0
  26458 / 403 / 1034, Qwen3-4B Q8_0 6361 / 86 / 294, Phi-3.5-mini Q4_K_M 6503 / 144 / 312. Against
  ours at four streams: 0.5B 0.86, 1.5B 0.96, 0.6B 0.89, 4B 0.93, phi 1.12; the llama family's
  local rows are the section above's (1B 724, 3B 351, 8B 250: 0.84, 0.95, 1.02); flat, ours reads
  0.93 to 0.99 of CUDA on every carrier. CUDA's batched rows beat its own Vulkan rows by 1.06 to
  1.53 on the Q8 carriers (0.98 on phi's K-quant) on this card, so the Vulkan-reference ratios
  above overstate ours by that much against the card's best engine.
- **Phi's prefill at half the reference is the attention, not the K-quant tiles (the prefill
  profiler's role stamps, 512 tokens, us over 32 layers): q 7420, k 6858, v 4807, rope 3208,
  attn 81557, wo 6825, gate 15453, up 14034, down 17669, 162 ms whole; the reference's per-kernel
  log the same window: qkv 23.0 ms, gate-up 30.3, down 27.9, flash attention 2.65 (83 us a layer at
  head 96).** The GEMM roles read on par or ahead (the k5 and k4 decode-in-load tiles 55 and 60
  TFLOP/s on the ruler's l stamp); the flash tile admits heads of 64, 128, 256 and 512 alone, so
  phi's head of 96 runs the chunked pair at thirty times the flash cost - `followup_vulkan.md`
  item 78 [direction-grade - two processes].

### From the Vulkan batched-decode arc, the gemma dense carriers (2026-09-20)

Instruments as the two sections above - `daslang -jit benchmarks/lcpp_bench.das --npl 4` on the pod
(RTX PRO 4500 Blackwell, driver 580.173, cm2 without decode-vector, and the KHR arm under
`DASLLAMA_COOPMAT=mm`), `DASLLAMA_ALLOW_UNTUNED=1`, `DAS_JOBQUE_THREADS=16`, llama.cpp b10660's
`llama-batched-bench` under the same command line the same hour (every llama.cpp figure `external`),
`DASLLAMA_GPU_PROF=1` for the stamps. Every ratio is `tg128@4` summed over four device-home streams
against the reference's `S_TG` at `-npl 4` [direction-grade - two processes]; every lever's pair is
two commits in two processes [direction-grade - two commits].

- **The baseline (master after the qwen section, the pod, one rep each):** gemma-3-1b Q8_0 1056
  against 869 and gemma-3-4b Q8_0 499 against 445 - served by the N-row command already (no epilogue,
  no epilogue); gemma-2-2b Q8_0 195 against 616 (0.32) and gemma-4-12B Q8_0 53 against 186 (0.28) -
  not batched: the command declined the classifier epilogue (the final softcap, gemma-4's suppressed
  ids), so the four streams stepped a row at a time, below one stream's rate. The 12B's global
  layers take V from K, which the command served already; the file shares no K/V across layers
  (`gemma4.attention.shared_kv_layers` 0 on the 12B and the 31B, 20 and 18 on the E2B and E4B,
  which carry per-layer embeddings too).
- **The epilogue's rows form:** gemma-2 195 -> 687 (1.11), the 12B 53 -> 173 (0.93). The 12B's
  four-row step under the profiler, us: qkv 2440, qknrope 295,
  attn 1853, wo 1428, arrq_f 592, gu 10153, down 3976, arrq_n 594, cls 1252, epi 10, 22677 whole
  against the flat step's 20556 (gu 8404, attn 628, down_ar 5143, wo_ar 2133): the fused gate-up's
  N form and the attention carried the batch's whole overhead, the host between steps under half a
  millisecond (the wall step 22935 against a 22697 fence). Under `DASLLAMA_VK_FUSE_BISECT=4` the
  split gate, up and act requant read 3981 + 3921 + 197 = 8099 at four rows against the fused
  form's 10107, and the row 187.9 (1.01) - the fused N form was the gap.
- **The fused gate-up's N form as the plain q8 leaf's shape (`Q8GemvGuNT`, stamped at two, four
  and eight columns, a column's half-block one 16-byte load beside the weight word, a column past
  the live ones re-dotting the last live column):** the 12B's gu 10153 -> 8553 a step, the row
  171.5 -> 185.1 +/- 0.07 (0.99, three reps); gemma-2 688 -> 702; Qwen2.5-1.5B 980 -> 1019 against
  the qwen section's 920 (1.11, that section's 1.07). The eight-column guarded unroll it replaced loaded a column's block as four
  words under a guard on every column - the clamp alone the qwen section priced kept that shape,
  which is why it read slower there; the stamp is a different kernel. Against the split pair the
  stamp still reads 0.45 ms behind at this shape: the remaining room, with the host's half
  millisecond.
- **The two-head attention slab (`DaAttnT` at `G = 2` on a group of one or two heads, the same
  workgroup count):** the 12B's four-row attn 1835 -> 1729 (6%, the lever-2 tip's profile against
  the lever-3 tip's), the row 185.1 -> 186.15 +/- 0.16
  against 186.19 (1.00); gemma-2 704 -> 710. The pass is bound by its K/V reads and chunk barriers
  (the four-row attention at 57% of the mirror's byte roof), not by the dead heads' FMAs -
  `followup_vulkan.md` item 73 carries the reading. The two slabs raced in one process on the RTX
  5060 Ti (`harness/vk_attn_probe.das` with its slab arm, four rows at 640 positions, device
  timestamps a layer, the arms alternated four-head / two-head / four-head / two-head): the 12B's
  sliding shape (head 256, 16 heads, two a kv head) 40.9 / 40.9 -> 27.5 / 27.5 us a layer at one
  split and 35.8 / 36.0 -> 26.6 / 26.6 at two; the gemma-2 shape (head 256, 8 heads, two a kv
  head) 39.0 -> 26.6 at one split and 22.5 -> 18.4 at four, its best split - the two-head slab
  a quarter to a third faster a layer where the pod's step read 6%, the smaller card's pass
  nearer its FMA bound. The same race on the pod (82 SMs, the arms alternated the same way, the
  four-head rows repeating within 0.5 us across the two orders): the 12B's sliding shape 32.9 ->
  26.8 at one split, 24.7 -> 20.6 at two and 28.8 -> 18.6 at four, its best; gemma-2's shape 32.9
  -> 26.8 at one split and 18.6 -> 16.5 at three and four - a third at the best split where the
  served step moved 6%, so the layer's attention is a smaller share of that step than of the
  smaller card's.
- **The board at the arc's tip, tg128@4 (ours cm2 / ours KHR / llama.cpp, three reps ours; then flat
  tg128 ours / theirs - the reference's flat figure is the same `llama-batched-bench` run's
  `S_TG` at `-npl 1`, ours the `-npl 4` run's flat row):** gemma-2-2b Q8_0 704 / 709 / 618 (1.14 / 1.15), flat 216 / 201; gemma-3-1b
  Q8_0 1084 / 1089 / 878 (1.23 / 1.24), flat 396 / 320; gemma-3-4b Q8_0 507 / 507 / 446 (1.14), flat
  149 / 136; gemma-4-12B Q8_0 185 / 185 / 186 (0.99 at the lever-2 tip; 186.15 / - / 186.19 at the
  arc's tip, 1.00), flat 55.1 / 54.3; gemma-4-12B Q4_K_M 233 / 233 / 184 (1.26), flat 81.9 / 78.1;
  gemma-4-31B Q4_K_M 103 / 103 / 90 (1.14), flat 36.6 / 32.9. The KHR arm reads the cm2 arm's rows:
  the decode kernels are one set on both. The reference's pp512 column on gemma-2-2b and gemma-3-1b
  reads 87 to 4395 tok/s across its runs (`external`) and is no prefill reference on those two.
- **The RTX 5060 Ti at the section's tip (driver 616.56, the desktop holding 0.9 to 2.1 GB of the
  16 GB, `DAS_JOBQUE_THREADS=16`), tg128@4 ours cm2 (with decode-vector) / ours KHR
  (`DASLLAMA_COOPMAT=mm`) / llama.cpp Vulkan / llama.cpp CUDA (the same b10660 checkout built with
  CUDA 13.4, every reference figure `external` from `llama-batched-bench` under the section's
  command line the same hour; then flat tg128 ours cm2 / theirs Vulkan / CUDA):** gemma-2-2b
  Q8_0 411 / 414 / 363 / 412 (1.13 against Vulkan, 1.00 against CUDA), flat 124 / 115 / 123;
  gemma-3-1b Q8_0 734 / 738 / 564 / 810 (1.30 / 0.91), flat 246 / 208 / 236; gemma-3-4b Q8_0 294
  / 292 / 258 / 291 (1.14 / 1.01), flat 84 / 78 / 84. The 4B's first pass ran in the paging class
  (3.2 flat on both arms) with the desktop holding 2.1 GB beside its 11 GB plan; the rows above are
  its repeat on the cleared card (0.9 GB held), the plan the same 8187 MB mirror at ctx 15412 that
  served at 1.4 GB held. The gemma-4-12B files fit no four-region plan on this card - the resident
  driver declines the Q4_K_M at 15.6 GB of 12.2 to 13.3 asked (item 80 carries the message's unit)
  and the Q8_0 outright (12.1 GB of weights against the 9 GB cap), the per-op rails serve them a
  row at a time at 15 to 16 (Q4_K_M) and 14 to 15 (Q8_0) summed - and the reference pages: its
  Vulkan rows read 106 (Q4_K_M) and 46 (Q8_0) at four streams, its CUDA rows 141 and 88 - so the
  12B's board is the pod's above. Ours reads at or above CUDA's four-stream rate on the two
  carriers CUDA does not win outright, and 0.91 of it on the 1B.
- **The 12B files homed on the 5060 Ti (2026-09-21, the bench pinning its batched row's context
  before the load - `followup_vulkan.md` item 81's first rung - so the plan sizes the mirror to
  the 660 positions a region the row serves instead of the binding cap's 6238):** gemma-4-12B
  Q4_K_M 151 +/- 0.2 / 106 / 141 (1.42 against Vulkan, 1.07 against CUDA), flat 48 / 44 / 48,
  the image 7179 MB with an 866 MB mirror; gemma-4-12B Q8_0 104 +/- 0.04 / 46 / 88 (2.25 / 1.18),
  flat 30 / 21 / 26, the image 12064 MB with the same mirror - 13.2 GB on the 16 GB card, the
  desktop holding 0.9 GB, no paging flag. The reference pages both files there.
- **Our gemma-2-2b pp512 on the pod reads 6468 to 19388 across runs (three-rep cv up to 38%)
  while its tg rows hold within 1%** - `followup_vulkan.md` item 79; the gemma-3 and gemma-4 rows
  hold within 2% on pp512.

### From the Vulkan batched-decode arc, the MoE carriers (2026-09-21)

Instruments as the sections above - `daslang -jit benchmarks/lcpp_bench.das --npl 4` on the pod
(RTX PRO 4500 Blackwell, driver 580.173, cm2 without decode-vector, and the KHR arm under
`DASLLAMA_COOPMAT=mm`), `DASLLAMA_ALLOW_UNTUNED=1`, `DAS_JOBQUE_THREADS=16`,
`DASLLAMA_PARITY_FULL=1` (every carrier is large-tier), llama.cpp b10660's `llama-batched-bench`
under the same command line the same hour (every llama.cpp figure `external`). Every ratio is
`tg128@4` summed over four device-home streams against the reference's `S_TG` at `-npl 4`
[direction-grade - two processes]; the lever's pair is two commits in two processes
[direction-grade - two commits].

- **The baseline (master 1c6ce2d2f, the pod, three reps ours), tg128@4 ours / llama.cpp, then flat
  ours / theirs:** gpt-oss-20b mxfp4 196 / 234 (0.84), flat 213 / 97; Qwen3-30B-A3B Q4_K_M 159 /
  312 (0.51), flat 174 / 184; Qwen3-30B-A3B UD-IQ2_XXS 165 / 196 (0.84), flat 184 / 86;
  gemma-4-26B-A4B Q4_K_M 129 / 264 (0.49), flat 140 / 94; gemma-4-26B-A4B UD-IQ3_XXS 129 / 232
  (0.55), flat 141 / 94. The N-row command declined every MoE layer, so the four streams stepped a
  row at a time and read below one stream's rate on every carrier. Qwen3.6-35B-A3B UD-IQ2_XXS
  (the hybrid) 30 / 236 (0.13), flat 167 / 112: the recurrent layers' one device state slot goes
  home and back on every stream switch - the hybrid arc's residency change, the board's worst
  served shape.
- **The reference at decode size groups nothing by expert.** The local llama.cpp clone
  (`D:/Work/llama.cpp`, the research report in the arc's notes): Vulkan takes its mat-vec path
  for up to 8 tokens (`mul_mat_vec_max_cols` in `ggml-vulkan.cpp`) and loops one dispatch a
  token on the host, CUDA one launch with a warp per token-slot pair (`MMVQ_MAX_BATCH_SIZE` in
  `ggml-cuda.cu`) - both read N x k expert planes a step; the grouped form (a counting sort into
  per-expert buckets, one GEMM with a grid axis per expert) sits past those thresholds. So the batched
  step's gain on a MoE carrier is the attention, the router, the shared expert and the submit,
  never the experts' bytes - and the one-row block's regions form (the k experts as regions of
  one slot-mapped dispatch a plane) is already the shape the reference's Vulkan loops per token.
- **The routed block's rows form (the router's columns, the per-row top-k over the shared record
  base, the expert GEMVs as the one-row leaves over `nrows x k` regions, the act over every slot,
  the unfolded down, the combine per row; `ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command`), cm2 /
  KHR / llama.cpp at tg128@4, three reps ours:** gpt-oss-20b 436 +/- 7 / 434 +/- 8 / 234 (1.86);
  Qwen3-30B-A3B Q4_K_M 460 +/- 14 / 432 +/- 30 / 312 (1.47); Qwen3-30B-A3B UD-IQ2_XXS 348 +/- 5 /
  347 +/- 10 / 196 (1.77); gemma-4-26B-A4B Q4_K_M 363 +/- 4 / 366 +/- 5 / 264 (1.38);
  gemma-4-26B-A4B UD-IQ3_XXS 353 +/- 3 / 355 +/- 3 / 232 (1.52). The flat rows did not move
  (214, 174, 184, 140, 141). Every carrier reads past the reference on both arms from the one
  lever; the KHR arm reads the cm2 arm within its spread on every row (the 30B Q4_K_M's KHR
  spread of 30 is the widest on the board). At the arc's tip - the rows' own combine form on a
  batched-first run, the router's columns loop - the three cm2 rows read 428 +/- 7, 453 +/- 15 and
  358 +/- 4, each within its row's spread [direction-grade - two commits].
- **The 5060 Ti (driver 616.56, the desktop holding about 0.9 GB of the 16 GB), tg128@4 ours cm2
  / llama.cpp Vulkan / llama.cpp CUDA (the same b10660 checkout built with CUDA 13.4, `external`,
  the same hour), then flat ours / theirs Vulkan / CUDA:** gpt-oss-20b 245 +/- 7 / 135 / 293
  (1.82 against Vulkan, 0.84 against CUDA), flat 130 / 61 / 127; Qwen3-30B-A3B UD-IQ2_XXS 237 +/- 1
  / 91 / 359 (2.61 / 0.66), flat 143 / 59 / 135. gemma-4-26B-A4B UD-IQ3_XXS fits no plan on this
  card (the resident driver declines at 23.9 GB asked of 13.3, its demoted down-expert rows taking
  the served weights alone to 15.4 GB - a plan figure that tallied each routed layer's dense shared
  expert twice, the low-format N-row arc's section below; the per-op rails serve 16 summed) where the reference
  pages: 109 on Vulkan, 275 on CUDA - row 81's shape. Against CUDA the two homed carriers read
  0.66 and 0.84. The expert-bucket form (the N-column
  expert GEMV a bucket, which decodes a weight block once for every row that picked its expert)
  is the lever that room names; it would serve a fifth of gpt-oss's slots at four rows and a
  tenth of the 30B's.
- **The rows against the sessions alone:** gpt-oss bit for bit (the regions file's six cells);
  Qwen3-30B Q4_K_M within 0.062 of the peak, past the K-quant dense carrier's 0.06 bar and under
  the wide bar's 0.10 (item 75's rounding through twenty-four K-quant expert planes a token over
  forty-eight layers; the regions cells' own maxdiff lines on the pod, cm2 arm); gemma-4-26B
  Q4_K_M within the wide bar against the split one-row command and off by up to 0.13 of the peak
  on five of thirty-two compares against the fused one (the same cells run before the file pinned
  the fused forms off, the pod, cm2 arm) - the folded down sum's rounding through the 26B's
  router near-ties (the N-row routed down now sums its k rows in the GEMV,
  `ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-block32-leaf`). The routed planes at `nb` rows cost the resident image nothing a
  reader sees: gpt-oss-20b's image reads 10914 MB with a 123 MB mirror at 660 x 4 and 220 MB of
  scratch on the pod, the planes' growth inside the scratch's rounding.

### From the Vulkan batched-decode arc, the E-series carriers (2026-09-21)

Instruments as the MoE section above (the pod's cm2 arm and the 5060 Ti, `lcpp_bench.das --npl 4`
three reps, llama.cpp b10660's `llama-batched-bench` the same hour, `external`; every ratio
`tg128@4` against the reference's `S_TG` at `-npl 4` [direction-grade - two processes]; every
lever's pair is two commits in two processes [direction-grade - two commits]); both carriers
Q8_0, `DASLLAMA_PARITY_FULL=1` for the E4B. The one-row rates did not move through the
section (the pod: E2B 198.4 -> 198.7, E4B 112.7 -> 112.5; the 5060 Ti: 122.2 -> 121.5, 66.1 -> 66.2).

- **The rows form alone (commit e43ace31d: the side input a row, the pre-step projection a row a
  column, the branch's split forms, q alone on a shared-KV layer), tg128@4 ours / llama.cpp:** the
  pod E2B 540.7 +/- 1.7 / 185.5 (2.92), E4B 335.8 +/- 0.3 / 360.2 (0.93); the 5060 Ti E2B 371.0 +/- 1.0
  / 113.1 (3.28), E4B 209.9 +/- 0.5 / 213.0 (0.99). The E4B's four-row step under the profiler read
  2.4 ms of device idle a step on the pod and 4.4 on the 5060 Ti against a dense carrier's half a
  millisecond: the batch driver ran the CPU pre-step (`ple_pre_prefill`: the gather and the
  [dim x layers*ple] host GEMM over the rows) on every step, for rows the device then gathered and
  projected again.
- **The pre-step gate (commit 0840fc4a0: the batch step skips the CPU pre-step where the decode
  gate says the armed driver projects, and runs it late on a declined step):** the pod E2B 567.7
  +/- 1.6 (3.06), E4B 357.5 +/- 0.2 (0.99); the 5060 Ti E2B 386.3 +/- 2.0 (3.42), E4B 226.5 +/- 0.7
  (1.06). The E4B's idle a step 2.4 -> 1.7 ms on the pod, 4.4 -> 2.9 on the 5060 Ti.
- **The router's columns and the fused branch's rows form (commit 00ac50e0b: `RouterGemvT` reads
  each row once over every column - the pre-step projection had streamed its 55 MB plane once a
  row - and the branch takes the fused act + requant + proj over the columns where the one-row
  branch fuses, `ARCHITECTURE_GPU_VULKAN_NROW.md#nrow-token-command`):** the pod E2B 585.0 +/- 0.2 / 185.5
  (3.15), E4B 363.4 +/- 0.8 / 360.2 (1.01); the 5060 Ti E2B 394.9 +/- 0.3 / 113.1 (3.49), E4B 234.7
  +/- 0.6 / 213.0 (1.10). Against llama.cpp CUDA on the 5060 Ti (the same checkout built with CUDA
  13.4): E2B 449.7 (0.88), E4B 247.6 (0.95). The E4B's four-row step, us, pod / 5060 Ti: pleproj
  159 -> 120 / 555 -> 233; the branch (the FFN step's rows requant, the gate, the fused act + proj)
  1465 -> 1122 / 1375 -> 1195; the step whole 10763 -> 10512 / 16647 -> 16138. The reference's
  E2B batched row reads 185 on the pod against its own flat 111.5 (1.66x, where its E4B row scales
  3.3x over 108.9) and 113 on the 5060 Ti against 64.4: the reference's four-stream E2B shape is its
  own question, and the E2B ratios stand as measured.
- **The rows at the admission's ends (the pod, the cm2 arm, three reps):** E4B at two streams 195.8
  +/- 0.2 fused against 185.8 +/- 0.1 split (`DASLLAMA_VK_FUSE=0`), the reference 120.3 (1.63); at eight
  streams 614.2 +/- 0.5 fused against 597.6 +/- 0.3 split, no reference row (its `-c 4096` holds no
  eight streams of 640). gpt-oss at two streams 304.9 +/- 2.8 against 186.3 (1.64), at eight 520.4
  +/- 4.9. The fused branch wins at both ends, so the pick does not branch on the column count.
  The E4B's footprint on the 5060 Ti: the image 7533 MB, the mirror 144 MB at 660 x 4, 167 MB of
  scratch; the E2B's 4675 + 46 + 166.
- **The rows against the sessions alone:** the E2B regions file's nine cells bit for bit on both
  boxes at every commit, the batched cells served (`test_gpu_resident_regions_e2b.das`); the fused
  branch's columns held to the one-row dispatches bit for bit (`test_vkd_q8_gemv_pleact`'s columns
  arms), the router's columns likewise (`test_vkd_router_gemv_cols`), and the gpt-oss regions file
  bit for bit over the router's new form.

### The CUDA levers on the pod (2026-09-21)

Provenance of every "ours" figure in this section: `benchmarks/lcpp_bench.das` under `-jit` on the
pod (RTX PRO 4500 Blackwell, `DASLLAMA_GPU=1`, cm2, the f16 K/V mirror, the box's tuned
`DAS_TUNE_POLICY`, `--for-debug-purposes -r 3 -p 512 -n 128 -t 16 --npl 4`: the flat row and the
four-stream row from one process, so the flat row runs on the four-region plan), the shapes from the
same bench under `DASLLAMA_GPU_PROF=1`; every before -> after pair is two commits [direction-grade -
two commits]. The references: llama.cpp b10660 built with CUDA on the pod
(`/workspace/llama/src-b10660/build-cuda`, nvcc 12.8, `-DCMAKE_CUDA_ARCHITECTURES=120`) and the
prebuilt Vulkan b10660 (`/workspace/llama/b10660/llama-b10660/`, the E-series section's build),
both under `llama-bench -ngl 99 -fa 1 -r 3` and `llama-batched-bench -c 4096 -b 2048 -ub 512 -npp 512
-ntg 128 -npl 1,4 -ngl 99 -fa on`, `external` [direction-grade - two processes]; the CUDA kernels under nsys
2024.6.2 (`-t cuda`, the trace grouped by kernel name and grid over the last 16 steps, a step closed
by the classifier launch) beside our `DASLLAMA_GPU_PROF=1` stamps. nsys's per-kernel records inflate
a step of about 1200 launches by about 1.5 ms (the traced step ran a fifth slower), so its sums
rank the shapes and bound them from above; the reference's tg steps launch as one CUDA graph each.

- **The rows, the pod (RTX PRO 4500), tg128@4 summed / tg128:** CUDA E4B 440.2 / 125.5 against ours
  363.0 / 112.6 (0.82 / 0.90; llama.cpp Vulkan 360.2), CUDA gpt-oss 523.3 / 234.4 against ours 425.6 /
  209.1 (0.81 / 0.89; Vulkan 234). The E4B four-row step: ours 10.5 ms of GPU time and 0.65 ms of
  host (the logits copy 252 us, four argmaxes, the scheduler tick, the fence wake) against CUDA's
  9.09 ms wall - on this card the gap is GPU time, where the 5060 Ti's was the host (the E-series
  section above).
- **The shapes, E4B at four rows, ours stamps / CUDA traced, us a step:** gate + up 3025 / 3218,
  down + wo + proj 2619 / 2263, the classifier 837 / 876, the projections 697 / about 1050, the
  attention 1354 / about 755, the add + rms + requant glue 1394 / about 1850, the per-layer-embedding
  gate 430 / 226. gpt-oss at four rows: the expert GEMVs 5348 (e_gate 1768, e_up 1791, e_down 1789 -
  the floor for reading every slot's expert whole: 16 slots x 4.15 MB x 3 planes x 24 layers at
  896 GB/s) / 3903, the attention 618 / 279, the rest within a tenth. The reference's MoE GEMV reads
  every slot's expert too; its launch puts every token's dot of one row index in one block, so an
  expert two slots share leaves DRAM once.
- **The interleaved region walk (commit 36df5105f: the one-column leaves map a subgroup to
  `rg % nreg`, `rg / nreg`, so a weight row two slots share reaches the second from cache),
  tg128@4 ours before -> after / CUDA:** gpt-oss 425.6 +/- 6.8 -> 450.3 +/- 3.9 / 523.3 (0.81 ->
  0.86), Qwen3-30B-A3B Q4_K_M 453 -> 459.0 +/- 18.9 (within the spread: 128 experts over 32 slots
  share few), Qwen1.5-MoE Q8_0 520.1 +/- 35.3 -> 526.3 +/- 39.8 (the same session, within the
  spread). The one-row rates did not move (gpt-oss 209.13 -> 209.13). Under the profiler the expert GEMVs read 5348 -> 4733 us a step (e_gate 1768 -> 1517, e_up 1791 -> 1618, e_down 1789 -> 1598; the step 9235 -> 8672), 830 us above the reference's traced 3903: the walk shares a row between slots that sit within a cache's reach, the reference's block shares it within one warp set.

- **E2B, the pod, tg128@4 summed / tg128:** CUDA 745.5 / 224.3 against ours 578.3 +/- 1.3 / 198.5 (0.78 /
  0.88; llama.cpp Vulkan 185.5) - the widest of the three. The shapes at four rows, ours stamps / CUDA
  traced, us a step: gate + up 1527 / 1591, down + wo + proj 1616 / 1187, the classifier 504 / 530, the
  projections 395 / 593, the attention 1016 / 478 (a head of 256, eight heads a kv head: two workgroups a
  row, four pieces - the chain itself, 29 us a layer against their 9.5), the glue 1209 / about 1550; the
  host 0.56 ms a step (the logits copy 266 us) against their device-to-host copy of 148.
- **The attention pieces capped to one wave (commit ee15d5217: `rd_layer_pieces` bounds a layer's count
  by the SM count over its (kv head, slab) workgroups times the plan's rows, both commands alike), the
  ruler's readings at 640 positions and four rows (`harness/vk_attn_probe.das`, us a layer):** gpt-oss's
  shape (sixteen workgroups a row) 34.9 at four pieces -> 16.4 at one, Llama-1B's (eight a row) 22.6 ->
  16.5 at two, the 30B's (eight a row, head 128) 27.3 -> 20.6 at two, E4B's (two a row) stays at four
  (22.7; 34.4 unsplit): the pass costs one workgroup's chain a wave and climbs past about thirty-two
  workgroups a layer. The rows, tg128@4 before -> after: gpt-oss 450.3 -> 461.1 +/- 4.0 (CUDA 523.3: 0.86 ->
  0.88), Llama-3.2-1B Q8_0 1369.1 +/- 1.5 -> 1432.8 +/- 2.4 in one session (flat 459.2 -> 465.0: the one-row
  command takes the plan's count too), Qwen3-30B-A3B 459.0 -> 450.2 +/- 20.5 (within the spread), E4B 362.3
  +/- 0.6 (its count unchanged). The regions files hold bit for bit through the change (gpt-oss, the 30B on
  its bar, qwen2, E2B, Qwen1.5-MoE), since both commands read one count.

- **The q8 N-column row-pair stamps armed from a reduction of 4096 (`g_q8_n2_min_n`, the E-series' down GEMVs)
  - DECLINED, measured:** tg128@4 E2B 578.3 -> 525.1 +/- 1.3, E4B 362.3 -> 343.2 +/- 0.1, Llama-3.2-1B 1419.1 ->
  1312.1 +/- 1.2 (flat rates unmoved); E2B's down stamp 941 -> 1608 us a step. The pair twin halves the activation
  re-reads and still loses on this card, so the lever stays at its off default; the E-series' down group (1616 us
  a step against the reference's 1187) waits on another form.
- **The device argmax pick (`ClsArgmaxPart` + `ClsArgmaxFin` after the epilogue, the picks-only transfer twin;
  `ARCHITECTURE_GPU_VULKAN_LANDING.md#logits-transfer-queue`), tg128@4 before -> after / CUDA:** E2B 578.3 -> 630.0 +/- 0.7 (745.5:
  0.78 -> 0.85), E4B 362.3 -> 382.0 +/- 0.1 (440.2: 0.87), gpt-oss 461.1 -> 476.8 +/- 4.6 (523.3: 0.91), Llama-3.2-1B
  1432.8 -> 1577.4 +/- 3.9, Qwen3-30B-A3B 450.2 -> 467.7 +/- 21.9 (within the spread); flat E2B 198.5 -> 198.8, E4B 112.5 ->
  112.4, Llama-1B 465.0 -> 464.1, gpt-oss 209.2 -> 207.8 (the two passes over a 201k vocab, on a step the flat row still
  lands logits for). The E2B four-row step under the profiler: the host's logits copy 266 -> 20 us, the idle between
  commands 1407 -> 862, the passes' `t:pick` stamp 10 us a step - and 12 on Qwen2.5-0.5B (152k vocab), 9 on Llama-1B
  (128k): flat across the vocab widths the tier serves, so the split has no width gate. The first form, one
  workgroup a row over the whole row, cost 140 us on a 262k vocab and the flat rates 2-6% (E2B 198.5 -> 193.1, gpt-oss
  209.2 -> 199.7), which is why the pass is two. The pick planes take 4 x (2 x 64 + 1) bytes a row of the plan (1 KB at
  four rows), a footprint the plan's report does not itemize. The scheduler's device-mode cell counts the picks
  landed against the rows stepped (`test_gpu_resident_regions*.das`), the kernel cell holds the passes to
  `parallel_argmax` (`test_vkd_cls_argmax`), and the regions files hold bit for bit through the change (qwen2 26/26,
  E2B 13/13, gpt-oss 8/8, the hybrid 6/6, qwen2 under `DASLLAMA_VK_KV32=1`).

### From the Vulkan hybrid rows arc (2026-09-22)

Instruments as the E-series section above: the pod (RTX PRO 4500, Linux), `daslang -jit
benchmarks/lcpp_bench.das --for-debug-purposes -r 3 -p 512 -n 128 -t 16 --npl 4` on the cm2 arm
and on the KHR arm under `DASLLAMA_COOPMAT=mm`, `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0
DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_PARITY_FULL=1 DAS_JOBQUE_THREADS=16` and no `DAS_TUNE_POLICY`
override (the untuned tier) for every carrier; llama.cpp b10660's prebuilt Vulkan
`llama-batched-bench -c 4096 -b 2048 -ub 512 -npp 512 -ntg 128 -npl 1,4 -ngl 99 -fa on` the same
hour, `external`; every ratio `tg128@4` against the reference's `S_TG` at `-npl 4`
[direction-grade - two processes]; a lever's pair is two commits in two processes
[direction-grade - two commits]. The test counts below are the same box and tier under
`dastest -jit`, cm2, `DAS_JOBQUE_THREADS=8`. No 5060 Ti row is in this section.

- **The residency (commit d14bc2e32: a deltanet state slot per mirror region, the row's slot and
  parity in its `TokMeta` row, the prefill in the selected region's slot):** the one-row rates
  unmoved - Qwen3.5-0.8B Q8_0 flat 474.4 +/- 0.7 against 470.3 at the arc's tip - and the batched
  rows still a row at a time (tg128@4 475.5, one stream's rate: the N-row command still declined
  the recurrent layers); the resident, regions and scheduler hybrid files green at the commit.
- **The N-row recurrent form and the gated q (commits dcc6852a6, ca0ac2a0e, da7d315f2), tg128@4
  ours cm2 / ours KHR / llama.cpp:** Qwen3.5-0.8B Q8_0 1553.1 +/- 1.3 / 1561.0 +/- 3.8 / 971.1
  (1.60 / 1.61), flat 470.3 / 468.6 / 381.5; Qwen3.5-9B-MTP UD-Q5_K_XL 316.1 +/- 0.4 / 315.8 +/- 0.4 /
  277.4 (1.14 / 1.14), flat 104.8 / 104.8 / 102.3 (the rows form that fed its K-quant recurrent
  heads Q8_0 blocks read 314.8 on both arms: the Q8_K feed the shipped form requants at every
  recurrent head costs nothing the ruler sees); Qwen3.6-35B-A3B UD-IQ2_XXS 440.6 +/- 2.6 / 443.4
  +/- 2.0 / 235.9 (1.87 / 1.88), flat 163.7 / 163.7 / 131.5 - against the MoE section's 30 (0.13)
  a row at a time, where the one slot went home and back on every stream switch. pp512 cm2 / KHR:
  the 0.8B 29102 / 23862, the 9B 5326 / 4223, the 35B 4583 / 4138 (the reference's four-stream
  prompt rate 29067 / 5008 / 4588). The 9B's four-row ratio is the section's tightest and its flat
  ratio 1.02: the dense K-quant 9B trunk, not the recurrent form, sets it. Its regions file
  (`test_gpu_resident_regions_hybrid_k.das`, the split bar, 4 of 4) is what found the rows form's
  Q8_0 feed and, after it, an N rail that never ensured a recurrent layer's FFN leaves.
- **The rows against the sessions alone:** the hybrid regions file's batched cells bit for bit
  through the N-row command (11 of 11), the scheduler's deltanet cells, the resident hybrid file,
  and the fused step's two-row arm against the one-row dispatch a row at a time bit for bit on both
  o forms (`test_vkd_dn_step_rows`). The regions file found two defects on the way: the rows-form
  prologue stamp stores no float row (a recurrent layer 0 reading one now takes the split
  prologue), and four rope and attention sets bound the grown `TokMeta` block at its old row width,
  every row past the first reading its position outside the binding.
- **The per-region planes' footprint (computed from the geometry, no device measurement):** a
  recurrent layer's state slots are `regions x nvh x ds x ds x 4` bytes and its ring pairs
  `regions x 2 x cd x (dconv - 1) x 4` - at four regions on the Qwen3.5-0.8B (16 heads of 128,
  cd 6144, dconv 4) 4 MB + 0.6 MB a layer over 18 recurrent layers, 83 MB; on the Qwen3.6-35B-A3B
  (32 heads of 128, cd 12288) 8 MB + 1.2 MB a layer over 30 layers, 276 MB; at `RD_NB_MAX` (eight)
  regions twice that. The rows' deltanet planes grow by the row count: the projection row
  `(cd + di) x 4`, the o row `di` bytes plus `di / 32 x 4` of scales plus `di x 4` of f32, so
  eight rows on the 35B hold 0.9 MB. Beside the K/V mirror the same regions hold (the residency
  section's sizing), the slots are within a percent of the plan.
- **The carriers that served batched without evidence (the phi3, gemma3 and mistral3 families), the
  same instruments, tg128@4 ours cm2 / ours KHR / llama.cpp:** Phi-3.5-mini Q4_K_M 578.7 +/- 0.4 /
  574.8 +/- 0.4 / 552.1 (1.05 / 1.04), flat 220.4 / 221.0 / 217.4; gemma-3-1b Q8_0 1240.1 +/- 0.3 /
  1240.3 +/- 1.2 / 845.5 (1.47 / 1.47), flat 398.3 / 395.2 / 310.5; gemma-3-4b Q8_0 542.8 +/- 0.6 /
  543.4 +/- 0.6 / 442.6 (1.23 / 1.23), flat 152.2 / 152.6 / 137.2. The regions files pin the rows:
  `test_gpu_resident_regions_gemma3.das` bit for bit (6 of 6), `test_gpu_resident_regions_phi3.das`
  on the split bar (4 of 4). Mistral-Small-3.1-24B Q4_K_M has no row and no regions file: the pod's
  volume holds no room for its 14 GB beside the stocked models (`followup_vulkan.md` item 87).

### From the M4 Metal pass (2026-09-13)

Instruments: `benchmarks/matmul/bench_metal_gemv_kernels.das` at the Qwen2.5-0.5B decode shapes
(the `05b` set, plus an n-ladder at d=4096 and n=2048 at classifier width), best of 3 rounds,
production `v0_gemv` against the lab's row-per-simdgroup forms, every arm interleaved in one
process; `benchmarks/decode_step_trace.das` on the 0.5B Q8 file, whose step wall is read outside
the benchmark process: out-of-process; the records rig's Metal pair, `benchmarks/lcpp_bench.das`
under `-jit --for-debug-purposes --ngl 99 -p 512 -n 128`: debug-jit. The M4 Pro and the M5 Max,
each on its own mint. The rows are in `followup_metal.md` sec.7.1. Every rig pair below is two
builds or two knob settings in two processes - direction-grade - and enters as its ratio, the
arms' rates left in the run's report. Qwen3-30B-A3B-Instruct-2507-Q4_K_M and
Qwen3.8-27B-Q4_K_M carry committed board rows on both boxes (`performance/records/m4.json`,
`performance/records/m5.json`); the debug-jit readings here are read beside those rows.

- **The q8 GEMV at n=896, M4 Pro (ms, weight GB/s): split form vs row-per-simdgroup:** qkv
  (d=1152) 0.0081 / 135 vs 0.0051 / 228; wo (896) 0.0068 / 126 vs 0.0044 / 208; w13 (4864)
  0.032 / 145 vs 0.019 / 262; w2 (n=4864, d=896) 0.0129 / 360 vs 0.0186 / 263; cls (151936)
  0.919 / 157 vs 0.574 / 267. The first lab pass read the split form 2-3x slower still (0.029
  ms on qkv, 0.98 on cls): the M4's clocks idle between sparse dispatches, the interleaved
  all-variant run is the honest one.
- **The same on the M5 Max:** qkv 0.0055 vs 0.0047; wo 0.0050 vs 0.0044; w13 0.0110 vs
  0.0075; w2 0.0078 vs 0.0072; cls 0.240 vs 0.255 - the row form wins or ties every short shape.
- **The crossover, both boxes:** n=1536 (d=4096) row form 1.5x on both; n=2048 at d=4096 M4
  +28% row form, M5 -9%; n=2048 at d=128256 split ahead on both (M5 0.464 vs 0.491, M4 1.169
  vs 1.217); n=3072 split ahead on both. `GEMV_SG_MAX_N = 1536`.
- **The 0.5B Q8 decode step, M4 Pro:** 4.2 ms per token (gpu 4.07, gap 0.02, sched 0.03;
  197 serial nodes) before, 3.2 after; the fusion, spec-chain, concurrent-encoder and fused-
  attention rails each moved the step under 2%. Rig tg128, the row form against the split: the
  0.5B 1.27x (0.73 -> 0.94 of llama.cpp), the 0.6B 1.23x (0.80 -> 0.97), the 0.8B 1.25x (0.97
  -> 1.21). M5: 0.99x, 1.00x, 0.99x and 1.00x on the 0.5B, 0.6B, 1.5B and Llama-1B Q8 - flat.
- **dev-W on the raced M4 sidecar (no tensor crown):** Llama-1B IQ3_XXS pp 0.93 -> 1.00,
  IQ2_XXS 0.93 -> 1.00, Q4_K_M 0.97 -> 1.02 - the same three rows the hand-crowned sidecar
  gave, so the panel is the whole gain and the tensor stamp none of it. Qwen3-30B IQ2_XXS
  serves at 0.95 / 1.09 where it declined to the CPU before.
- **The bake's footprint pair (M5 Max, `utils/dasllama-convert -f metal --stream`, the
  541 MB Llama-1B IQ3_XXS):** the image at IMAGE_VERSION 36 is 659 MB with a 32 MB dev-W plane
  over 16 sites (the q8/k4/k5/k6 mirrors), at 37 it is 2571 MB with an 1856 MB plane over 112
  sites; the mint's convert time 222 -> 1016 ms. The plane is 3.4x the model on a small
  split-scale dense file - the per-panel cap keeps a 30B's expert planes out, so the cost
  lands on the small files - and it buys the resident panel's prefill (1.05 scratch -> 1.23
  resident on the M4 rows above). Decision: the bake stays on; the cap and a per-file
  footprint knob are the ledger's if the disk cost bites.
- **The step wait, Llama-1B Q8, M4 Pro (`decode_step_trace -o`, 96 steps):** GPU 5.73 ms per
  token, wall 6.38-6.71, gap 0.61-0.98 ms per token averaged over a three-step cycle (two
  steps at gap 0, then 3.3-3.6 ms; commits at +0.1, +6.0, +13.9 ms; waits 13.7, 0, 5.9 ms).
  The same file on the M5 Max: wall 2.81 = GPU 2.80, one commit per step, gap 0.00. The 0.5B
  Q8 on the M4: gap 0.02 on three processes, 1.46 (6 ms holes) on a fourth. The attention rail,
  `taskpolicy -l 0 -t 0` and `DAS_JOBQUE_AFFINITY=2` each left the gap at 0.75-0.87. A spin on
  `GPUEndTime` before the blocking wait: 5.66 ms per token, gap 0.007, 176.5 t/s, three runs
  within 0.02 ms (llama-bench 167). Rig re-times, the spin against the blocking wait: Llama-1B
  Q8 tg 1.06x (1.00 of llama.cpp), Q4_K_M 1.08x (1.00), Qwen2.5-0.5B Q8 1.10x (1.03), 1.5B Q8
  1.07x (1.03); prefill unchanged. The adaptive form arms on the first hole (3.9 ms, both runs)
  and the later buckets read gap 0.001. The rig's own process pays the wake on every unchained
  step (IQ2_XXS 1.09x spinning over blocking) while the trace tool's does not, so the spin
  ships on by default.
- **The tall stamp on the M4 Pro (rig cells, the sidecar's `metal_tall_floor` at 100000 =
  never tall, against 64):** Llama-1B Q4_0 pp 1.06x (0.97 -> 1.03 of llama.cpp), Llama-1B Q8
  1.08x (0.98 -> 1.05), Qwen3-30B-A3B Q4_K_M 1.01x (1.02 -> 1.04; its board row is
  `performance/records/m4.json`'s), Qwen3.5-9B Q8 1.00x (even); decode unchanged. The 1-D grid
  twin, the double buffer off, the floor at 16, a re-mint and a re-baked image each left Q4_0
  within 0.2% of the never-tall arm. The M5's tall win stands (its Llama-1B Q8 prefill reads
  1.00x either way under the hand crown).

### From the CPU dedup arc (2026-09-20)

Every row here is one process, both arms interleaved, best of six after a warm-up; the M5 Max
and the zen4 (Ryzen 7 PRO 8700GE) both, `-jit`, `DAS_TUNE_MANIFEST` unset on the zen4 and the
box's rig sidecar on the M5, no `DAS_TUNE_POLICY` override. The two kernel races ran an
out-of-tree race script over the engine's own calls (`gemm_f32` / `gemm_f32_jo` fed the same
operands; `eval_` over the loaded model for the prefill row), so their readings are ratios of
the two arms, not board figures; the tokenizer rows are `lcpp_bench --tok`.

- **`gemm_f32` (row blocks outer) against `gemm_f32_jo` (column blocks outer), C += A*B at
  the deltanet chunk and ASR attention shapes, jo / ij:** M5 64x128x64 0.99, 64x64x128 1.00,
  128x128x128 0.96, 128x64x1000 0.96, 128x128x3000 0.94, 256x64x4000 0.99, 1024x64x1024
  **1.17**; zen4 the same seven 1.01, 1.01, 1.00, 1.01, 0.97, **0.92**, 1.00. Both forms stay:
  jo wins where B is wide and streamed once (the ASR scores, its callers today), ij wins the
  M5's tall square; neither is a default for the other's shapes.
- **Classic against blocked prefill attention (the two bit-identical CPU forms; flash is the
  default), the whole CPU prefill of gemma-4-E2B-it-Q8_0 at 2048 tokens, `DASLLAMA_GPU=0`,
  blocked / classic:** M5 **1.052**, zen4 0.996. The blocked
  form's K/V reuse across an 8-query block buys nothing the score-row reuse of the classic form
  does not, so the blocked arm goes and the tests' bit-exact mode is classic.
- **The BPE encode walk (`lcpp_bench --tok`, every corpus, 1 KB to 1 MB), MB/s master -> arc,
  best of two interleaved rounds:** the walk appends each codepoint's bytes into one reused
  buffer instead of building a string per codepoint. M5, Qwen3-0.6B: prose 12.1 -> 16.6, code
  11.5 -> 15.6, cjk 13.5 -> 16.9 (1.15 to 1.38 at every size); zen4, Qwen3-30B-A3B (the same
  qwen2 pretokenizer): prose 7.9 -> 12.6, code 7.8 -> 12.0, cjk 10.9 -> 13.9, digits 5.5 -> 7.3,
  longword 7.0 -> 9.7 (1.13 to 1.58). cv under 2% on every row but the 4 KB ones (8%).
- **One merge heap for both tokenizer backends - DECLINED, measured:** SPM's own heap
  (`spm_merge_heap`) and BPE's are the same forty lines, and the fold read gemma-3-1b SPM
  encode on the M5 24.0 -> 22.8 at 1 KB (prose, cv under 1%), 16.1 -> 13.7 at 4 KB code, parity
  from 64 KB up; on the zen4 gemma-4-E2B read 0.81 to 0.98 of master on every corpus and size
  (prose 7.6 -> 6.6 at 4 KB, cjk 28.8 -> 23.3 at 1 MB). A float priority in place of a double
  took back one to two points; stamping the merge as a generic per backend with the push/pop
  helpers left in `dasllama_bpe.das` took the M5's 1 KB row to 0.98 and left 4 to 16 KB at 0.93
  to 0.96. The SPM heap stays its own body, in the partition of the encode that runs it
  (`ARCHITECTURE_ENGINE_FORMATS.md#bpe-merge-heap`); the BPE side keeps the unshared form.

### From the Vulkan low-format N-row arc (2026-09-23)

Instruments as the hybrid rows section above: the pod (RTX PRO 4500, Linux), `daslang -jit
benchmarks/lcpp_bench.das --for-debug-purposes -r 3 -p 512 -n 128 -t 16 --npl 4` on the cm2 arm
under `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_PARITY_FULL=1
DAS_JOBQUE_THREADS=16`, the untuned tier, every figure below the mean of the three repetitions with
its standard deviation; llama.cpp b10660's prebuilt Vulkan `llama-batched-bench -c 4096 -b 2048
-ub 512 -npp 512 -ntg 128 -npl 1,4 -ngl 99 -fa on` the same hour, `external`, its second run of a
file; every ratio `tg128@4` against the reference's `S_TG` at `-npl 4` [direction-grade - two
processes]; a lever's pair is two commits in two processes [direction-grade - two commits]. The
5060 Ti rows (driver 616.56, Windows, the desktop holding 1.8 GB of the 16 GB) take the same command
lines, the bench pinning its batched row's context so the plan homes four regions at 660 positions;
the local llama.cpp is the same b10660 Vulkan build. The N-column ruler is
`daslang -jit harness/vk_gemv_probe.das -- cols 5120 17408` (the 27B's FFN gate shape) on the pod
under the same environment, the untuned tier in cm2 mode, its last column an N-column dispatch's
wall over N one-column dispatches, each wall the mean of its dispatch ring [direction-grade - two
commits].

The reference's first run of a file compiles its pipelines inside the measurement: i1-IQ3_S read
57.9 cold and 67.3 warm at four streams. The carriers are the three Qwen3.8-27B files whose dense
planes sit in sub-4-bit formats: i1-IQ3_S's FFN and attention q/k/o planes are IQ3_S, its recurrent
qkv and attention v planes Q4_K and its classifier Q6_K (440 IQ3_S tensors, 65 Q4_K, 1 Q6_K);
UD-Q3_K_XL mixes IQ4_XS (156 tensors, its most common), Q8_0 (98), Q4_K through Q6_K (69) and the
eight low-bit formats Q3_K, Q2_K, IQ3_S, IQ3_XXS, IQ2_S, IQ2_XS, IQ2_XXS and IQ4_NL across the
planes; UD-IQ4_XS is IQ4_XS in bulk with the same eight mixed in. All three have 64 layers with
attention every fourth, so a position costs 64 KB of f16 mirror and a region 151 MB of DeltaNet
state.

- **The eight N-column leaves alone (commit 181398621: `KqGemv<Fmt>N` for k3, k2, iq3s, iq3xxs,
  iq4nl, iq2s, iq2xs, iq2xxs on the shell's per-column `blk_contrib`), tg128@4 ours cm2 /
  llama.cpp, then flat:** i1-IQ3_S 95.4 +/- 0.2 / 67.3 (1.42), flat 48.4 / 42.4; UD-Q3_K_XL 62.4 +/-
  0.1 / 78.0 (**0.80**), flat 43.8 / 42.0; UD-IQ4_XS 55.0 +/- 0.1 / 84.9 (**0.65**), flat 41.1 / 40.5.
  The rows form scaled 1.97x over flat on the IQ3_S file and 1.34x to 1.42x on the mixed files.
- **The ruler at that tip, an N-column dispatch at four columns over four one-column ones:** q8
  0.25, k4 0.28, k6 0.27, q40 0.26, k5 0.35, k3 0.48, k2 0.50, iq3s 0.61, iq3xxs 0.61, iq2s 0.71,
  iq2xs 0.76, iq2xxs 0.78, iq4xs 1.06, iq4nl 1.06 - the shell re-ran each block's decode for every
  column, so a codebook or grid leaf shared nothing and the one-column class at 1 column read 1.23x
  the one form.
- **Lever 1 - decode once, fold per column (commit f18cb9ffa: every leaf's `blk_decode` hands back
  the block's packed int8 halves and four fold terms, one shared `blk_fold` dots them, `mad`
  throughout):** the ruler at four columns q8 0.25, k4 0.26, k6 0.26, q40 0.26, k5 0.26, k3 0.31, k2
  0.37, iq3s 0.35, iq3xxs 0.32, iq2s 0.44, iq2xs 0.41, iq2xxs 0.42, iq4xs 0.36, iq4nl 0.36; at eight
  columns k4 0.18, iq4xs 0.22, iq3s 0.27, iq2s 0.33; the one-column rates within 1% of before
  (iq4xs 196 us against 202). The rows: i1-IQ3_S 137.1 +/- 0.3 / 67.3 (**2.04**), flat 48.8;
  UD-Q3_K_XL 123.8 +/- 0.2 / 78.0 (**1.59**), flat 44.6; UD-IQ4_XS 118.2 +/- 0.2 / 84.9 (**1.39**),
  flat 41.7 (flat tg128 48.80 +/- 0.02, 44.58 +/- 0.07, 41.66 +/- 0.07); pp512 1475 +/- 5 / 1559
  +/- 4 / 1634 +/- 7. The KHR arm the same hour: tg128@4 137.3 +/- 0.1 / 124.0 +/- 0.2 / 118.0 +/-
  0.2, pp512 1394 +/- 5 / 1377 +/- 5 / 1394 +/- 4. The kernel family, N-column and gate-up cells
  hold at the tip on both boxes: the N form's every column is the one-column class's word for word.
- **llama.cpp's CUDA build of the same b10660 checkout on the pod (`external`, a reference row and
  not a target; `-DGGML_CUDA=ON`, nvcc 12.8, the Vulkan reference's command line, second run):**
  tg128@4 125.9 / 132.6 / 125.9 (i1-IQ3_S / UD-Q3_K_XL / UD-IQ4_XS), tg128 51.0 / 50.9 / 48.8,
  pp512 1811 / 1863 / 1906 - ours at the lever-1 tip reads 1.09x / 0.93x / 0.94x of it batched and
  0.96x / 0.88x / 0.85x flat [direction-grade - two processes]. CUDA's batched rate hardly moves
  with the format (126 to 133) where ours follows the decode cost of the leaves.
- **The 5060 Ti at the lever-1 tip, ours cm2 tg128@4 / tg128 with the reference's rows beside:**
  i1-IQ3_S homes (11526 MB image, 165 MB mirror, 230 MB scratch): 89.5 +/- 0.9 / 28.3 (56.5 before
  the lever) against llama.cpp's 2.5 / 2.6 - the reference pages the 12.6 GB file on this card at
  those arguments (its first and second runs alike); UD-Q3_K_XL homes (11900 MB image): 80.9 +/- 2.4
  / 25.7 (38.9 before) against 19.6 / 7.4, paging; UD-IQ4_XS declines the resident driver - 13295 MB
  asked of 12422 MB usable, the weights alone 12899 MB with other processes holding 1835 MB - and the
  per-op rails read 8.5 / 3.4 against the reference's 2.5 / 2.0, both paging. The IQ4_XS file on 16 GB
  is `followup_vulkan.md` item 81's streamed-weights arm, not a kernel.

### From the Vulkan vision tower arc (2026-09-24)

The instrument: the pod (RTX PRO 4500 Blackwell, driver 580.173, an AMD EPYC 7443 container at 48
threads under the 62 GB cgroup cap), `daslang -jit benchmarks/lcpp_bench.das -- -m <decoder> --image
<coco 640x480> --image-mmproj <mmproj> --for-debug-purposes -r 3 -t 16` under `DASLLAMA_IMAGE=0
DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=16`, `DASLLAMA_COOPMAT` unset (cm2, the pod's default
mode) and `DAS_TUNE_POLICY` unset (the box default; the untuned tier is what
`DASLLAMA_ALLOW_UNTUNED=1` arms, so the kernels are the fallback bodies); the `vk` arm
`DASLLAMA_GPU=1` (the tower chain on the Vulkan tower driver, the bench's engage line reading
encodes +3 of 3), the `cpu` arm `DASLLAMA_GPU=0` (the driver declining `device` on every encode,
the CPU tower chain - the q8 lane where the family has one, the exact f32 lane on qwen25v - and the
decoder on the CPU too, so only the encode column is a tower-vs-tower reading) [direction-grade -
two processes]. The encode is one image's tower time in ms, the best of the three timed reps with
the three listed beside it (the cell prints every rep), and a pp or tg figure is the decoder's
best rep in tok/s.

- **The four families, the driver against the CPU chain, encode ms (best of three, the three
  beside):** gemma-4-E2B (gemma4v q8, 130 soft tokens) **35.2** (40.5 / 35.2 / 35.8) against 701.7
  (715.1 / 701.7 / 708.4); gemma-3-4b (gemma3v q8, 256 tokens, the fixed 896 canvas - 4096 patch
  rows through 27 blocks) **144.9** (168.1 / 144.9 / 146.1) against 14471 (14506 / 14471 / 14527);
  Qwen3-VL-4B (qwen3v q8 with the three deepstack taps, 300 tokens) **247.9** (255.3 / 247.9 /
  251.9) against 1680 (1719 / 1691 / 1680); Qwen2.5-Omni-3B (qwen25v over the halfword twin, 391
  tokens, the f32 per-window route) **174.3** (219.6 / 180.3 / 174.3) against 7161 (7161 / 7198 /
  7163) - the slotted f16 route the arc replaced read 162 (the window layers read K and V off the
  compact rows, `followup_vulkan.md` row 95's staging lever). The first rep of a `vk` row carries
  the tower's upload and pipeline builds. The captions read on both arms of every pair are the
  same text and name the two cats and the remotes - an observation of the served output, not a
  parity claim; the parity instruments are the twin cells below and the chat pairs of
  `tests/test_vision_chat.das`.
- **The image turn's decode with the resident driver's span pass hydrating the mirror's rows
  before the CPU takes the image eval, tg tok/s on the `vk` arm against the `cpu` arm:** E2B
  **189.0** against 24.4 (24.7 on the `vk` arm at the tip before the pass hydrated: the decode fell
  to the CPU after the span eval), gemma-3-4b **106.2** against 15.2 (15.2 before), Qwen3-VL-4B
  **70.3** against 14.0 (56.0 before), Omni **99.0** against 20.0 [direction-grade - two commits].
  The caption the chat cell `test_vision_chat_e2b` reads under `DASLLAMA_GPU=1` is that cell's
  own assert.
- **The pp / tg columns are the decoder's**, not the tower's: on the `cpu` arm the decoder runs on
  the CPU too, and pp reads within 10% on both arms (the image turn's prefill is the CPU's on
  either: the Vulkan resident prefill declines the span eval).
- **The driver's allocations at the largest shape the path serves - the 4096-row encode cap
  (`VT_MAX_ENCODE_ROWS`, the `shape` decline past it), which is gemma3v's fixed canvas exactly:**
  the qwen25v halfword twin uploaded whole, 32 layers x (4 x 1280^2 + 3 x 1280 x 3456) weights x 2
  bytes = 1,268,776,960 bytes, about 1.27 GB (`q25v_gemm_sizes`: the four d x d GEMMs, the gate and
  up at d x ff_pad, the down at ff_pad x d); the gemma3v Q8_0 plane uploaded whole, 27 layers x (4 x
  1152^2 + 2 x 1152 x 4352) = 414 M weights, about 414 MB of quants plus 26 MB of f16 scales (2 bytes
  a 32-weight block); the per-encode device scratch at 4096 rows, from `vt_scratch` /
  `vt_scratch_f16`'s buffer list - eleven f32 row buffers at rows x d x 4 (eight on qwen25v), two
  f32 hidden buffers at rows x ff x 4, the two f16 K/V shadows at rows x d x 2, the Q8_0 feed at
  rows x max(d, ff) x 33/32 (the f16 feed at rows x max(d, ff) x 2 on qwen25v), the padded panels at
  rows x n_head x 128 x 12 where the head is not 64 wide, the rope tables at rows x hd x 4 - reads
  about 491 MB for gemma3v (d 1152, ff 4352, 16 padded heads), about 432 MB for qwen25v (d 1280, ff
  3456, 16 padded heads) and about 507 MB for the Qwen3-VL-4B qwen3v tower (d 1024, ff 4096, the
  fused [q | k | v] rows at 3 x rows x d x 4 and the three tap stashes at 3 x rows x d x 4 beside the
  padded panels); and the qwen3v host-side tap stash `ds_xtaps` at n_ds x rows x d x 4 bytes, 3 x
  4096 x 1024 x 4 = 50,331,648 bytes, about 50 MB on the 4B. The gemma4v scratch follows the same
  list at its d and ff with no padded panels (its head is 64 wide). One resident a family, released
  by the model drop's sweep.
- **The device chain's distance from the exact CPU chain, the twin cells at the tip, x token rms
  (the CPU q8 chain's own distance beside where the family has one):** gemma4v E2B 16 blocks
  0.082 / 0.160 / 0.63 / 0.26 against 0.064 / 0.193 / 0.46 / 0.25 (cb96, cb336, green 672x336,
  cb768x384; the 1- and 2-block towers within 0.015 on both); gemma3v 27 blocks 3.26 against 2.68 (the fixed canvas; the 1- and 2-block towers 0.12
  against 0.12 / 0.13); qwen3v 4B 24 blocks 0.42 / 0.62 / 0.61 against 0.36 / 0.58 / 0.44 (cb96,
  cb448, cb640x320); qwen25v 32 blocks 0.015 / 0.076 / 0.092 against the exact chain (cb112, cb448,
  cb616x336; no q8 lane), the 5060 Ti's KHR arm 0.020 / 0.034 / 0.020. The slotted f16 window
  route read 0.16 to 0.79 on the 5060 Ti's cm2 arm and 1.53 on the pod's at 32 blocks - the
  coopmat tile's f16 staging compounds over the 28 window layers, which is why the window layers
  attend in f32 on the compact rows (`ARCHITECTURE_GPU_TOWER_VULKAN.md` 2.2aq).

  Provenance of the twin-cell figures: `test_gemma4v_vulkan_twin`, `test_gemma3v_vulkan_twin`,
  `test_qwen3v_vulkan_twin` and `test_qwen25v_vulkan_twin` (`tests/test_<family>.das`), each run
  through dastest under `-jit` with `--test-names` naming the cell, `DASLLAMA_GPU=1`,
  `DASLLAMA_IMAGE=0`, `DAS_TUNE_POLICY` unset; the box is the pod (RTX PRO 4500 Blackwell, driver
  580.173) on its cm2 arm (`DASLLAMA_COOPMAT` unset) unless the sentence names the 5060 Ti (Boris's
  box, RTX 5060 Ti 16 GB, driver 616.56), whose cm2 arm is likewise the default and whose KHR arm
  is `DASLLAMA_COOPMAT=mm`. A distance is the cell's own logged x-rms reading over one run of the
  cell; the cells log the reading on green and assert the bar.

### From the Vulkan audio tower arc (2026-09-24)

The instrument: the pod (RTX PRO 4500 Blackwell, driver 580.173, an AMD EPYC 7443 container at 48
threads under the 62 GB cgroup cap), `daslang -jit benchmarks/lcpp_bench.das -- --asr -m <catalog
substring> -r 2 --for-debug-purposes` (the rows stamp 16 threads) under `DASLLAMA_IMAGE=0
DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=8 DASLLAMA_MODELS_DIR=/workspace/models
WHISPER_CPP_MODELS=/workspace/models`, `DASLLAMA_COOPMAT` unset (cm2) and `DAS_TUNE_POLICY` unset
(the untuned tier's fallback bodies); the `vk` arm `DASLLAMA_GPU=1` (the block loops on the Vulkan
tower driver, the cell's engage line reading encodes +N over 3 transcriptions, declines +0, and
the row stamped `vulkan` - the bench stamps `vulkan` only when the drivers served every clip,
`ARCHITECTURE_MEASUREMENT_AUDIO.md#asr-gpu-pairs`), the `cpu` arm `DASLLAMA_GPU=0` (the driver declining `device` on every
encode, the CPU q8 chain, the decoder on the CPU too - so only the encode column is a tower-vs-tower
reading; on the `vk` arm the decoder of the LLM-backed families rides the resident driver, which is
most of the wall's drop on E2B and canary) [direction-grade - two processes]. A wall is the clip's
transcription in ms, the best of two timed reps (the bench prints only the best, so the rows
carry no spread); the encode is the clip's encoder time in ms from
the cell's encode-split transcription (one per clip, `encode_ms` of the row). The five clips are
the ASR corpus's jfk (11 s), jfk3 (33 s), gb1 (199 s), hp0 (273 s) and hp0x2 (547 s); canary's
catalog row stops at gb1.

The reference figures below are all `external`, each run on the pod: whisper.cpp at the pod's
checkout built with Vulkan, `whisper-cli -m ggml-<model>-q8_0.bin -f <clip> -t 16 -bs 1 -bo 1
-nf -nt` (`-ng` for its CPU arm); llama-mtmd-cli b10660 (Vulkan), `llama-mtmd-cli -m <model
Q8_0> --mmproj <mmproj bf16> --audio <clip> -p "Transcribe the audio." --temp 0 --jinja -ngl 99
-t 16` (E2B with `-n 256`), two reps a clip, its encode the sum of the clip's `encoding done in
N ms` lines; NeMo's `generate()` through `benchmarks/asr/canary_qwen_bench.py --device cuda` on
the CUDA box; ggml's per-op profile is the whisper-cli command above over turbo jfk under
`GGML_VK_PERF_LOGGER=1`, its figures named by ggml op (`MUL_MAT`, `FLASH_ATTN_EXT`). The das
per-dispatch and per-role readings (the chain's ms a chunk, fc1 / fc2 / q / k / v / o, a
dispatch's us) are the same bench command under `DASLLAMA_GPU_PROF=1`; a forced split names its
`DASLLAMA_CM2_SPLITK` value. The RTX 5060 Ti readings run on Boris's box (driver 616.56) as
`bin/Release/daslang.exe -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- --asr -m <catalog
substring> -r 2 --for-debug-purposes` from the repo root under `DASLLAMA_GPU_PROF=1`, the pod's
other overrides alike.

- **Whisper large-v3-turbo (q8 encoder, 32 blocks at d 1280 / ff 5120 / 64-wide heads, 1500 rows
  a 30 s chunk), the `vk` arm against the `cpu` arm, wall ms / encode ms:** jfk **419 / 109**
  against 3395 / 2998; jfk3 **1306 / 219** against 7278 / 6120; gb1 **8000 / 1070** against 31608 /
  25159; hp0 **9538 / 1527** against 45858 / 37482; hp0x2 **18616 / 2910** against 86941 / 70385 -
  the encode 23x to 27x faster on the device, and the wall's remainder is the decoder on the CPU
  (`followup_vulkan.md` row 101), 15.7 s of hp0x2's 18.6 s.
- **Whisper tiny (q8, 4 blocks at d 384), wall / encode:** jfk **70 / 11** against 181 / 122; jfk3
  **228 / 22** against 438 / 261; gb1 **1367 / 251** against 2015 / 834; hp0 **1707 / 176** against
  2484 / 1056; hp0x2 **3341 / 453** against 4907 / 2151.
- **The whisper reference exe on the pod's own Vulkan build (`external`, the whisper-cli command
  above on the q8_0 siblings, `-ng` for the CPU arm; its `encode time` is the sum over the clip's
  30 s chunks, its `total time` the transcription), against the das rows above:** turbo per-chunk
  encode 35 to 40 ms on the device (hp0x2 660 ms over 19 chunks) against das's 153 ms a chunk
  (2910 / 19) - a 4x encode gap; the transcription hp0x2 3.70 s against das's 18.6 s, the decoder
  on the device there and on the CPU here; on the CPU arm the reference reads turbo hp0x2 encode
  93.7 s / total 107.9 s against das's 70.4 s / 86.9 s. Tiny per-chunk encode 3.1 to 9.3 ms on
  the device against das's 24 ms (453 / 19), total hp0x2 1.98 s against 3.34 s; the CPU arm 2.54 s
  / 5.74 s against das's 2.15 s / 4.91 s. The reference's first clip on the device carries its
  pipeline builds (tiny jfk 7.7 s, turbo 2.3 s of `encode time`), the das rows' warmup
  transcription is outside the timed reps.
- **The whisper rows again with the decoder on the Vulkan ASR-decoder driver (the per-window
  cross-KV and every decode batch on the device; the encoder's GEMMs on the cm2 f16 feed, the conv
  stem and mel still on the CPU), the same `vk` arm, wall ms / encode ms:** turbo jfk **136 / 77**,
  jfk3 **289 / 163**, gb1 **1340 / 622**, hp0 **1857 / 995**, hp0x2 **3623 / 1830** - the wall 3x
  to 6x under the CPU-decoder rows above, and hp0x2's transcription at the reference exe's 3.70 s;
  the encode 96 ms a chunk (1830 / 19) against its 47 ms on the device - the CPU mel and stem and
  the wake between chunks. Tiny jfk **40 / 30**, jfk3 **121 / 21**, gb1 **575 / 89**, hp0 **724 /
  121**, hp0x2 **1451 / 271** against the reference's 1.98 s total on hp0x2.
- **The same rows with the conv stem on the device too (the whisper-class chain's conv seat on
  the cm2 feed; the mel the one CPU phase left), wall ms / encode ms:** turbo jfk **101 / 49**, jfk3
  **236 / 98**, gb1 **1087 / 388**, hp0 **1450 / 601**, hp0x2 **2903 / 1122** - the transcription
  under the reference exe's 3.70 s on hp0x2, the encode 59 ms a chunk (1122 / 19) against its 47 ms
  on the device and the reference's 35 to 40; tiny jfk **33 / 19**, jfk3 **97 / 20**, gb1 **487 /
  62**, hp0 **599 / 70**, hp0x2 **1170 / 120** against the reference's 1.98 s. Canary on the same
  tip with the chain's row cap replaced by the device's range, the `vk` arm over all five clips
  (`--asr-clips`), wall / encode: jfk **157 / 59**, jfk3 **433 / 140**, gb1 **1258 / 784**, hp0
  **2288 / 1035** against the NeMo CUDA reference's `generate()` at 743 / 2213 / 3443 / 9022 ms
  (`external`) - 4x
  ahead on hp0; hp0x2 at this tip still declined on the front's own row cap (20.1 s, the CPU front)
  and takes the next tip's row.
- **The next tip - gemma4a's projector tail on the blocks chain and the canary front off its row
  cap - E2B (`vk`, wall / encode):** jfk **184 / 18**, jfk3 **493 / 40**, gb1 **3194 / 190**, hp0
  **3910 / 268** against llama-mtmd-cli's Vulkan encode 21 / 54 / 223 / 327 (`external`) - at or
  under the reference on every clip; canary hp0x2 **4130 / 2577** on the device (6840 rows, the
  plane R at 375 MB) against NeMo's 13508 ms (`external`).
- **The whisper rows with the pre-LN chain's row passes folded into its f16 feed (six row passes
  a block where twelve ran), wall / encode:** turbo jfk **95 / 44**, jfk3 **226 / 88**, gb1
  **1095 / 354**, hp0 **1396 / 546**, hp0x2 **2710 / 1046** against the reference exe's 3.70 s
  on hp0x2 - the chain 42.2 ms a chunk on the device (`DASLLAMA_GPU_PROF=1`, 450 dispatches,
  was 47 ms over 642) against the reference's 37.4 ms of ops for the same chunk (`external`,
  `GGML_VK_PERF_LOGGER=1`: its `MUL_MAT` ops on the q8_0 planes 23.1 ms - 128 x 84.5 us at k
  1280, 32 x 219 us at k 5120, 32 x 165 us at m 5120 - and 14.3 ms of its other ops,
  `FLASH_ATTN_EXT` among them); ours by role under `DASLLAMA_GPU_PROF=1`: fc2 11.8, fc1 8.0, q /
  k / v / o 12.5, the attention 4.1, the row passes 3.7, the readback 1.7. Tiny jfk **32 / 3**, jfk3 **100 / 6**, gb1
  **488 / 22**, hp0 **591 / 33**, hp0x2 **1171 / 66** against the reference's 1.98 s. Qwen3-ASR
  on the same chain, wall / encode: jfk **83 / 15**, jfk3 **226 / 38**, gb1 **1427 / 213**, hp0
  **1863 / 299**, hp0x2 **4299 / 626** against llama-mtmd-cli's Vulkan encode 21 / 177 / 307 / 403
  / 831 (`external`) - under the reference on every clip (the 128-row windows' blocks 4.6 ms a
  window on the device under `DASLLAMA_GPU_PROF=1`, 254 dispatches). E2B on this tip reads jfk **186 / 18**, jfk3 **493 / 40**, gb1 **3201
  / 190**, hp0 **3921 / 268**; its hp0x2 row is the family's - 13,665 soft tokens pass the E2B
  decoder's 8192-position context, on the CPU chain alike.
- **The whisper chain's fc2 GEMM under the split-k pick (four 1280-deep chunks of its 5120 K into
  the partial planes, the reduce after), every dispatch figure under `DASLLAMA_GPU_PROF=1`:** on
  the RTX PRO 4500 the fc2 dispatch reads **260 + 16** us where it read 367 whole (8.3 + 0.5 ms a
  chunk over 32 blocks, was 11.8), the chain **39.6 - 40.5 ms** a chunk on the device where it
  read 42.2 (482 dispatches) against the reference's 37.4 (`external`); the turbo rows move inside
  noise - jfk **114** vs 116 ms and hp0x2 **3458** vs 3456 with the split off
  (`DASLLAMA_CM2_SPLITK=1`), the decoder's share of the wall. On the 36-SM RTX 5060 Ti the pick
  refuses (60 tiles fill two waves there): forced, the same site reads 467 + 20 us in two chunks
  (`DASLLAMA_CM2_SPLITK=2`) and 438 + 55 in four (`DASLLAMA_CM2_SPLITK=4`) against 477 whole.
  Qwen3-ASR's 128-row windows (seven tiles at d 896) under a forced five-way split
  (`DASLLAMA_CM2_SPLITK=5`) read the reduce as a loss - hp0 encode **314** where it read 299,
  hp0x2 **658** where it read 626, 5% - so the chain's gate splits only at `VT_SK_MIN_ROWS` = 512
  rows or more; past the gate the wave model picks the chunk count (`DASLLAMA_CM2_SPLITK` unset or
  `0`), and the tower logs the pick once per model.
- **Qwen3-ASR-0.6B (the whisper-class tower through its conv front, 18 blocks at d 896 / ff 3584,
  128-row batch sets at tile 32 - the family's chunk is short), wall / encode:** jfk **628 / 541**
  against 1319 / 684; jfk3 **1569 / 1429** against 3872 / 2017; gb1 **9244 / 7835** against 29437 /
  11608; hp0 **13881 / 11465** against 42471 / 16256; hp0x2 **28114 / 24361** against 113686 /
  32035 - the encode only 1.3x to 1.5x faster: at 128 rows an encode is one 128 x 896 batch on the
  32-wide tile, and the per-encode fixed cost (the residual upload, the meta rebuild, the readback)
  is the column; a lever for the fronts row (`followup_vulkan.md` row 99) or a batching of the
  family's chunks.
- **Canary-Qwen 2.5B (the FastConformer encoder, 32 blocks at d 1024 / ff 4096 / 128-wide heads,
  the rel table at 2 npos - 1 rows a clip), wall / encode:** jfk **505 / 407** against 2233 / 1120;
  jfk3 **1585 / 1296** against 6802 / 3084; gb1 **9500 / 9057** against 33780 / 20697 - the encode
  2.3x to 2.7x faster; the full bidirectional rel-pos attention at O(npos^2 hs) a head is the
  column at gb1's 2496 rows (the wall is the encode there: 9.1 of 9.5 s).
- **Gemma-4 E2B audio (the Conformer, 12 blocks at d 1024 / ff 4096 / 128-wide heads, the chunk-12
  windowed attention, 320-row
  and 768-row batch sets on one residency), wall / encode:** jfk **461 / 295** against 3126 / 854;
  jfk3 **1306 / 849** against 9392 / 2533; gb1 **8067 / 5017** against 62576 / 15183; hp0 **10570 /
  6904** against 86876 / 20871 - the encode 2.9x to 3.0x faster; the wall's larger drop is the
  E2B decoder on the resident driver.
- **The arc's tip on the pod (`vk`, every row stamped `vulkan`, declines 0; wall / encode over
  jfk / jfk3 / gb1 / hp0 / hp0x2):** whisper turbo **93 / 41**, **213 / 83**, **1042 / 334**,
  **1383 / 513**, **2685 / 983**; Qwen3-ASR **80 / 14**, **224 / 37**, **1430 / 217**, **1866 /
  303**, **4305 / 648** against llama-mtmd-cli's 21 / 177 / 307 / 403 / 831 (`external`) - under
  the reference on every clip; canary **156 / 59**, **431 / 137**, **1263 / 787**, **2275 /
  1022**, **4224 / 2579** against NeMo's 743 / 2213 / 3443 / 9022 / 13508 (`external`); E2B **185 /
  -**, **493 / -**, **3210 / -**, **3917 / -** (the E2B ASR row takes no hp0x2: 13665 audio
  tokens exceed its decoder's 8192-token context, the facade's own refusal). The split-k A/B at
  the tip under `DASLLAMA_GPU_PROF=1`: turbo hp0x2 **3578** with the pick (fc2 8.38 ms + the
  reduce 0.52 ms a chunk over 32 blocks, the wave model's four chunks) against **3674** with the
  split off (`DASLLAMA_CM2_SPLITK=1`, fc2 11.80 ms).
- **The whisper parity pass (the same rig, `vk`, wall / encode over jfk / jfk3 / gb1 / hp0 /
  hp0x2; the walls the best of two timed reps, the provenance above) [direction-grade - two
  processes, and the pass's tip against the arc's]:** turbo **61 / 31**, **151 / 62**,
  **771 / 248**, **981 / 371**, **1941 / 719**, the loop 1 / 2 / 8 / 12 / 23 windows a clip. The
  reference is whisper-cli with its timestamps on (`external`: whisper.cpp d09f61a, version
  1.9.4-dev, the pod's Vulkan build; `whisper-cli -m ggml-large-v3-turbo-q8_0.bin -f <clip> -t 16
  -bs 1 -bo 1 -nf`, the recipe `ARCHITECTURE_MEASUREMENT_AUDIO.md#asr-gpu-pairs` carries: each clip behind jfk in
  one process, the jfk-only process subtracted, two reps and the min - the reps' pairs sit in the
  run logs, not here): encode 36 / 70 / 342 / 444 / 886, total 101 / 252 / 1302 / 1646 / 3409, over
  1 / 2 / 10 / 13 / 26 encoder runs - so the encoder reads 1.12x to 1.38x ahead a clip (31.2 ms a
  window against 34.5) and the transcription 1.67x to 1.76x. Under `-nt` the same reference
  (`external`, the same build and flags plus `-nt`) decodes no timestamp tokens and advances a
  whole 30 s a window (19 runs on hp0x2, encode 649, total 3170), a shape the das loop does not
  run. The four levers, read on the jfk ledger (`DASLLAMA_GPU_PROF=1`): the block chain 39.3 ->
  29.0 ms - the l stamp's clamped edge path on the chunk's partial last token column (1500 rows =
  five columns and 220), lifted by the GEMM records rounded to the column (the probe's `wh` arm,
  `daslang -jit harness/vk_gemm_probe.das -- wh` on the pod's RTX PRO 4500 under `DASLLAMA_GPU=1`
  in cm2 mode, the l column at 1500 rows against 1536 - the arm's alternate is the same shape at
  the whole column: q / k / v / o 95 -> 52 us, fc1 241 -> 173, fc2 357 -> 185 whole, split4
  240 -> 158 + 16, the m column 62 / 194 / 229 at either count; ggml's `MUL_MAT` on the same
  shapes reads 84 / 166 / 221 - `external`, `GGML_VK_PERF_LOGGER=1 whisper-cli -m
  ggml-large-v3-turbo-q8_0.bin -f jfk.wav -t 16 -bs 1 -bo 1 -nf`, the same build) - the cross-KV
  chain 15.8 -> 0.7 ms (the 61 MB CPU-layout readback made lazy), the stem chain 2.1 -> 0.3 ms
  with the encoder rows handed to the decoder device to device and the post-norm folded into the
  chain, the decode step 91 -> 66 dispatches a token (80 -> 55 ledger stamps: the row passes
  carrying their Q8_0 feed; a three-row prompt batch 0.84 -> 0.77 ms on the device). The
  served-row check the pass opened with: the resident prefill's tile pick beats both forced
  columns on every model measured (pp512 tok/s, the rig's `lcpp_bench -p 512 -n 0 -r 2`, the best
  of two, the tile through `DASLLAMA_CM2_TILE` unset / 128 / 256: Llama-3.2-3B Q8 14792 / 11007 /
  12442, gemma-3-1b 34671 / 34280 / 32909, E2B 16313 / 13196 / 14920, E4B 9804 / 8918 / 9496,
  gemma-2-2b 19540 / 18044 / 17533), and the wave model picks the faster column on every 512-row
  shape the probe's `g3`, `gemma`, `tl` and default arms time.
- **The tower dedup pass's tip (the same rig, `vk`, wall / encode over the five clips, the best
  of two) [direction-grade - the dedup tip against the parity pass's]:** turbo **61 / 31**,
  **149 / 61**, **772 / 249**, **988 / 371**, **1934 / 721** against the parity pass's 61 / 31,
  151 / 62, 771 / 248, 981 / 371, 1941 / 719 - within noise; the jfk ledger the same (the stem
  0.28 ms, the blocks 29.9, the cross-KV 0.73, the decode 66 dispatches - 55 stamps - at 0.77 ms
  for the three-row batch). The pass's gate for a stamp it left alone is `harness/vk_spv_diff.das`
  over the dump before and after (the kernel files and every twin): 256 stamps byte-identical, 18
  moved, none appeared, and the moved set is the fold set - the six residual seams (`cls_ar_rq`,
  `cls_ar_rq_b`, `cls_ar_rqx`, `cls_ar_comb_rq`, `cls_ar_comb_g4_rq`, `q8_gemv_ar`) on the shared
  block pass, the two clamp arms, the seven post-add and Q8 norm stamps on the seam template, the
  decoder's attention partial on the shared reduces and its two combines on one template; the f32
  and f16 layer-norm stamps and every requant stamp stayed identical. The residual seams' own gate
  is a decode row: E2B Q8_0 flat tg128 **198.45 +/- 0.09** against the ledger's 198.5 to 198.8
  (pp512 16352 +/- 43 against 16313), the rig's `lcpp_bench -m gemma-4-E2B-it-Q8_0.gguf -p 512 -n
  128 -r 3 -t 16` under `DASLLAMA_GPU=1`, `DASLLAMA_COOPMAT` unset (cm2), `DAS_TUNE_POLICY` unset,
  three reps and their spread. The four classes the pass deleted (`TowerHeadGather`,
  `TowerZeroRows`, `TowerQ3aFinish`, `TowerGluAct`) are the four that vanish, the family twins
  holding their chains' bars on the pod: `test_gemma4a_vulkan_twin`, `test_canary_vulkan_twin`,
  `test_qwen3a_vulkan_front`, `test_encoder_blocks_vulkan` (`tests/test_audio.das`),
  `test_whisper_vulkan_twin`, `test_whisper_vulkan_wdec` (`tests/test_whisper.das`),
  `test_gemma4v_vulkan_twin`, `test_gemma3v_vulkan_twin`, `test_qwen3v_vulkan_twin`,
  `test_qwen25v_vulkan_twin`, and `tests/test_vulkan_tower_kernels.das` whole, each run through
  dastest's `--test-names` filter on the pod at the tip. The review round's fix tip (the
  layernorm and post-add stores on one `mad`, the block pass behind a buffer barrier, the fused
  bias pass on the literal tanh GELU) reads **61 / 31**, **151 / 62**, **772 / 247**, **993 / 370**,
  **1934 / 714** on the same rig - within noise again - with E2B tg128 198.67 +/- 0.32 and pp512
  16432 +/- 79, and the probe's `wh` arm, its feed now carrying the read slack and its two columns
  raced round by round, reads q / k / v / o 95 -> 52 us, fc1 240 -> 170, fc2 359 -> 184 whole,
  split4 240 -> 153 + 16, the m column 62 / 186 / 228 at either count; the differ over the fix
  tip's dump against the dedup tip's: 256 identical, 18 moved (the requant family on the one
  block-width constant, the layernorm and post-add stamps on the `mad`, the fused bias pass on
  its literal GELU), none appeared or vanished. The whisper decoder's flushed-memory bar
  (`test_whisper_vulkan_wdec_flush`, `WDEC_FLUSH_LOGIT_BAR` = 5% of the knob-off chain's peak
  logit) reads 0.11 of a 23.5 peak on tiny on the pod (the bar 1.18; the batch with its last token
  changed moves the knob-off logits by 3.29, outside it).
- **The driver's allocations at the largest shape the path serves, the 4096-row encode cap
  (`VT_MAX_ENCODE_ROWS`; whisper-class chunks stop at 1500 rows, gemma4a's at 768; canary has no
  row cap and declines `shape` only past the device's storage-buffer range):** the rel quartet
  (`vt_rel_bufs`) at a 4096-row canary encode's 2 x 4096 = 8192 rows and d 1024
  - the f32 table 8192 x 1024 x 4 = 33,554,432 bytes, its Q8_0 quants 8,388,608, their scales 8192 x
  1024 / 32 x 4 = 1,048,576, the projected rows another 33,554,432 - about 76 MB; gemma4a's quartet
  is 13 rows, under 60 KB. The per-encode device scratch follows the vision arc's list above at each
  family's d and ff with no padded panels (every audio head is 64 or 128 wide), plus the qwen25v
  hidden's zero row map (`rex_dev`, cap x 4 bytes: 16 KiB at 4096 rows): about 441 MB for
  turbo at 4096 rows (d 1280, ff 5120: eleven f32 row buffers 230 MB, two hidden buffers 168 MB, the
  f16 K/V shadows 21 MB, the Q8_0 feed 22 MB) and about 353 MB for canary and E2B (d 1024, ff 4096).
  The weights uploaded whole: turbo's Q8_0 plane 32 x (4 x 1280^2 + 2 x 1280 x 5120) = 629 M
  weights, about 629 MB of quants plus 39 MB of f16 scales; canary's and E2B's eleven records a
  block, 8 d^2 + 4 d ff = 25.2 M weights a block - 806 M weights, about 806 MB plus 50 MB of
  scales, over canary's 32 blocks and 302 M, about 302 MB plus 19 MB, over E2B's 12. One resident
  a family (`g_vt_aud`, `g_vt_g4a`, `g_vt_cn`), released by the model drop's sweep.
- **The allocations that scale with the input or the model, by formula (bytes), at two shapes:**
  the fc2 split-k partial planes, nsplit x cap x d x 4 (cap = the rows rounded up to 64) - turbo's
  four-way split over 1500 rows 4 x 1536 x 1280 x 4 = 31,457,280, the pick's ceiling of eight planes
  at 4096 rows and d 1280 167,772,160; canary's rel plane, cap x (2 cap - 1) x 4 - gb1's 2496 rows
  49,830,144, hp0x2's 6840 rows (cap 6848) 375,133,440. The ASR decoder driver, with nl layers, d,
  ta encoder positions (1500) and tmax text positions (448): the cross K/V planes kx / vx at nl x d
  x ta x 4 each and their f16 halves kxr / vxr at nl x d x ta x 2 each - turbo (nl 4, d 1280)
  30,720,000 and 15,360,000, large-v3 (nl 32) 245,760,000 and 122,880,000; the self K/V caches kc
  / vc at nl x d x tmax x 2 each - turbo 4,587,520, large-v3 36,700,160; the host readback of the
  cross K/V at 2 x nl x d x ta x 4 - turbo 61,440,000, large-v3 491,520,000; the attention
  partials pms at nchunks x heads x 8 x 2 x 4 and py at nchunks x heads x 8 x 64 x 4 (nchunks = the
  longer of ta and tmax over 256 keys: 6 chunks, 20 heads) - 7,680 and 245,760 on both; the weight
  gather uploads the decoder's Q8_0 plane whole, its quant bytes plus 2 bytes a block scale, staged
  through host arrays of the same sizes.
- **The device chain's distance from the exact f32 CPU chain at the tip, rel_l2 (the CPU q8 chain's
  own distance beside):** gemma4a E2B over jfk 0.0554 against 0.0550 (the device 0.0134 from the
  CPU q8 chain); canary over jfk's log-mel 0.0233 against 0.0220 (0.0213 from the CPU q8 chain);
  the whisper-class blocks over the synthetic mel of the Metal blocks cell, on the 5060 Ti (the pod
  stocks no f32 mmproj): qwen2audio 0.0528 against 0.0684, voxtral 0.1706 against 0.1741, omni-3b
  0.0753 against 0.0790; the 5060 Ti's gemma4a and canary readings 0.0540 / 0.0229 against 0.0560 /
  0.0220. Whisper tiny and turbo transcribe jfk to the same text on the CPU q8 chain and the device
  chain, turbo with one near-tie token of 28 flipped.

  Provenance of the twin-cell figures: `test_gemma4a_vulkan_twin`, `test_canary_vulkan_twin` and
  `test_encoder_blocks_vulkan` (`tests/test_audio.das`) and `test_whisper_vulkan_twin`
  (`tests/test_whisper.das`), each run through dastest under `-jit` with `--test-names` naming the
  cell, `DASLLAMA_GPU=1`, `DASLLAMA_IMAGE=0`, `DASLLAMA_ALLOW_UNTUNED=1`, `DAS_JOBQUE_THREADS=8`,
  `DAS_TUNE_POLICY` unset; the box is the pod on its cm2 arm unless the sentence names the 5060 Ti
  (Boris's box, RTX 5060 Ti 16 GB, driver 616.56, cm2 arm). A distance is the cell's own logged
  rel_l2 over one run of the cell; the cells log the reading on green and assert the 1.5x bar.

### From the vulkan-lane mint arc (2026-09-29)

The instrument: Boris's box (RTX 5060 Ti 16 GB, driver 616.56, cm2 arm, the Zen2 CPU),
`bin/Release/daslang.exe -jit -module-cache .jitted_scripts/module_cache/dastest.dascache
dastest/dastest.das -- --test modules/dasLLAMA/tests/test_vulkan_mint.das --test-names
test_vulkan_mint_lanes` from the repo root under `DASLLAMA_GPU=1 DAS_LOG_LEVEL=info
DAS_JOBQUE_THREADS=8 DASLLAMA_MODELS_DIR=D:/Work/llama.cpp/models`, `DASLLAMA_COOPMAT` and
`DAS_TUNE_POLICY` unset [debug-jit - the cell's own log lines, one run]. A save wall is the
`prepared image streamed in N ms` line (the whole streaming save: the walk's uploads, the twin
streamed to the writer, every planar family transcoded off the gguf mapping), a twin's size and
mint time the `plane vkblob N MB minted off the gguf in N ms` line, the warm wall the `prepared
vulkan image mapped in N ms` line. The peak footprint is the dastest process's peak working set
(`PeakWorkingSet64`, sampled every 500 ms from a wrapper) over the whole file - three loads of the
MTP carrier, two of its K-quant twin, and the cells' generations - so it bounds the cold mint from
above and is not the mint's own peak.

- **Qwen3.5-0.8B-MTP-Q8_0, the vulkan lane minted off the gguf inside the streaming save:** the
  untrimmed lane's twin **782 MB, 195 entries, minted in 4319 ms**, the whole save **6364 ms**; the
  trimmed lane's twin the same 782 MB in 4284 ms, its save **5446 ms** (no planar family written);
  the trimmed lane mapped back warm in **2508 ms**. Peak working set of the file's process
  **6558 MB**.
- **The reserve the sizing pass hands the writer for the twin** (`vk_mint_extra_bytes`): the quant
  planes' bytes, both halves, plus an eighth for the entry pads, plus the packed emb region where
  the lane is trimmed, plus 64 MB; the twin never outgrows its planar source (a device scale row is
  the disk row's f16 half or narrower), so the surplus is the pads and the window. The file sink
  truncates it at close; the in-memory sink holds it for the model's lifetime, which is the
  `DASLLAMA_IMAGE_SAVE=0` cost of the lane.
- **The lane files** (read in the arc's first run on this box, the same design before the review
  batch; the batch moved the twin's section, not a plane's bytes - `test_vulkan_inline_bake` holds
  the minted twin to the eager bake's byte for byte): one file of **1.92 GB** (1.10 GB trimmed)
  where the planar image and its twin summed 3.03 GB.

### From the AMX bf16 tile arc (2026-09-29, Granite Rapids `c8i.4xlarge` `i-02a87cabc8fcd2e4c`, 16 lanes)

The race and the carriers are from one box and one mint: `daspkg release --root
modules/dasLLAMA/benchmarks` with `DASLLAMA_CONFIRM_MODEL` on the 1B Q8_0 and no other
environment override (noise probes cv 0.08 / 0.03 / 0.06 / 0.12 %, validation drift 1.89 %, 66
kernels), the profile it exported in force. The token-block readings are from an earlier mint
of the same day on the same box, the same winners on every family. The reference build is
`HOW_TO_GET_SIDECAR.md` section 3's (`6c84c7d5d`, `clean-cpu`).

- **The bf16 tile runs every family at the matrix unit's rate: 7.4-8.7 ms a tile, where the
  best vector seat takes 20-48.** The mint's race (`harness/gen_tune_probe.das`,
  `DAS_TUNE_MODE=tune`, `-jit`), one lane, one process, the arms interleaved, best of 8 rounds
  (an arm screened out after 2), the 2048x8192x256 streaming fixture; the sidecar's `race`
  rows, tile ms, the `amx_bf16` two-group arm against the best other arm (busd512 mr16; the
  grid formats' `grid_vbmi`; q8's bias128): k4 7.44 vs 34.50, k5 7.98 vs 23.20, k6 7.42 vs
  32.44, q40 7.43 vs 24.40, iq4xs 7.49 vs 36.72, iq4nl 7.46 vs 36.95, iq3s4 7.46 vs 37.53, k3
  7.38 vs 47.91, iq3s 8.72 vs 36.80, iq3xxs 8.50 vs 36.32, k2 7.44 vs 45.29, iq2s 8.46 vs
  39.58, iq2xs 8.18 vs 39.06, iq2xxs 8.16 vs 35.63, q8 7.43 vs 20.18. The mint crowns the bf16
  arm on all fifteen families, and the q8 crown passed the end-to-end confirm. The int8 tile
  (`amx_int8`) stores and folds its sum at every 32-weight block - the traffic the bf16 leg has
  none of.
- **End to end the minted exe holds 1.27-1.41x the reference build on the mainstream carriers,
  5.9x and 21.5x on the two low-bit ones, tg ahead on all seven.** The released `lcpp_bench`
  that mint built, `dasllama-bench.exe -m <gguf> -p 512 -n 16 -r 3 -t 16 --ref <llama-bench>
  --ref-flavor clean-cpu --ref-no-affinity`; the rig runs the reference in its own process, so
  each pair below is `direction-grade`. pp512 / tg16 tok/s, ours (the spread over 3 reps) vs the
  reference: Llama-3.2-1B Q8_0 1971.7 +- 9.7 / 82.4 +- 0.3 vs 1394.7 / 78.3; Q4_K_M 1943.8 +-
  9.4 / 108.0 +- 0.5 vs 1453.1 / 87.1; IQ4_XS 1954.0 +- 3.9 / 116.5 +- 0.8 vs 1501.1 / 85.1;
  Q3_K_L 1932.1 +- 3.9 / 103.4 +- 0.4 vs 329.5 / 80.6; IQ3_M 1952.0 +- 10.4 / 115.1 +- 2.4 vs
  90.6 / 53.4; Qwen3-4B Q4_K_M 491.1 +- 0.6 / 34.0 +- 0.1 vs 385.5 / 27.8; Qwen3-4B Q8_0 483.4
  +- 1.4 / 25.0 +- 0.06 vs 374.5 / 22.3. The grid carrier (IQ3_M) sits level with the k4 one:
  the gather-fed panel is not the bill.
- **The fat exe takes the same leg.** `daspkg release --fat x86-avx2 --root
  utils/dasllama-server` - the x86-avx2 baseline with the class clones beside it - picks the
  `x86-amx` clone on this box: `dasllama-bench.exe -m <gguf> -p 512 -n 16 -r 3 -t 16` on the
  Llama-3.2-1B Q4_K_M reads 1915.1 +- 6.9 pp512 and 107.7 +- 0.6 tg16 tok/s.
- **The walk's token block: 512 holds end to end; per kernel the best block splits by K
  length.** The kernel race, one process, one script, the candidates interleaved, 9 rounds
  after 2 dropped, the job queue at its default width, `matmul_kq_batch` on k4 through the
  `x64-gen` backend with the walk's floor set to 1 for the race, median ms (cv %): at n=2048 d=2048 ntok=2048 a block of
  128 3.635 (1.5), 256 3.187 (2.7), 512 3.532 (1.5), 1024 3.965 (0.7), 2048 3.877 (0.3); at
  n=8192 d=2048 ntok=2048 128 18.515 (0.5), 256 17.005 (0.5), 512 15.658 (0.4) - 512 is that
  shape's L2 clamp. End to end (`lcpp_bench.das` as the `-jit` script, `--for-debug-purposes`,
  `DAS_TUNE_MANIFEST` on the mint's sidecar with `runtime.q8_token_block` set, the floor at 1,
  `-p 512 -n 16 -r 3 -t 16`; one process a reading, two alternated passes, `direction-grade`),
  pp512 tok/s at 128 / 256 / 512: Llama-3.2-1B Q4_K_M 1724.8 +- 2.3 and 1730.2 +- 6.6 / 1921.3
  +- 2.7 and 1944.4 +- 2.4 / 1963.3 +- 4.2 and 1968.4 +- 4.0; Q8_0 1732.2 +- 1.7 and 1724.7 +-
  6.8 / 1949.3 +- 4.0 and 1930.4 +- 3.9 / 1970.2 +- 9.6 and 1979.4 +- 4.7. The per-shape block
  is `followup_general.md` row 190.
- **What the bf16 leg allocates.** The bf16 activation plane, once per batch call:
  2 x (ntok x n + 32) bytes - 8,388,672 at ntok 512, n 8192 (the 1B's FFN down), 9,961,536 at
  n 9728 (the 4B's). The panel, one per dispatch chunk: 2 x (form x 16 x n + 32) bytes - 524,352
  and 622,656 a lane at form 2, 8,389,632 and 9,962,496 over 16 lanes. The row-dequant panel's
  row buffer, off the emitted panels only: 4 x n bytes, 32,768 and 38,912.

### From the AMX bf16 walk follow-up (2026-09-29, Granite Rapids `c8i.4xlarge` `i-02a87cabc8fcd2e4c`, 8 cores, 16 lanes)

Every rate below is `benchmarks/lcpp_bench.das` as the `-jit` script with `--for-debug-purposes`
under `-no-module-cache`, `DAS_TUNE_POLICY` unset, served on the `x64-gen` backend, one process
a reading, the arms alternated; an arm is a copy of `dasllama/dasllama_math_gen.das` put in place
before its process starts. Each pair compares across processes and is `direction-grade`.

- **The bf16 walk's block in whole tiles under no L2 clamp: 1.065x the clamped arm's pp512 on
  the 4B carriers, the 1B level.** The clamped arm is the walk of the AMX bf16 tile arc (the 512
  floor under `effective_token_block`, 431 tokens at K 9728); the other is `bf16_token_block` -
  512, a multiple of the 32-token tile. No environment override, the sidecar beside the script
  in force (the box's mint), `-p 512 -n 16 -r 3 -t 16`, three alternated passes, pp512 tok/s
  (the spread over 3 reps), clamped then whole-tile: Qwen3-4B Q4_K_M 486.94 +- 1.19, 486.23 +-
  0.61, 488.05 +- 1.45 against 518.70 +- 1.37, 523.30 +- 0.51, 517.21 +- 0.42; Qwen3-4B Q8_0
  491.48 +- 0.46, 494.14 +- 1.13, 492.13 +- 1.60 against 524.42 +- 0.79, 522.26 +- 0.85 (the
  third pass, 511.75 +- 18.91, is past the 3% spread and void); Llama-3.2-1B Q4_K_M 1912.78 +-
  1.69, 1905.51 +- 14.95, 1900.51 +- 3.15 against 1919.13 +- 6.08, 1916.96 +- 4.10, 1911.24 +-
  9.05.
- **The race that picked the block: 512 leads every block the L2 clamp would hand the walk.** Not
  a rate of `lcpp_bench`: `DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=<lanes> bin/daslang -jit
  -no-module-cache modules/dasLLAMA/harness/token_block_race.das` (the `x86-amx` class profile
  stamps the kernels, no sidecar; `DAS_TUNE_POLICY` unset), k4 on the `x64-gen` backend at tile
  form 2 and ntok 512, the candidates interleaved in one process, 11 rounds after 2 dropped, the
  second of two passes a lane count; best ms (cv %), every arm's sampled output equal to the 512
  arm's and the 512 arm's within the bf16 envelope of the disk-order dots. The clamp's 4 MB cuts
  the block to 431 tokens at K 9728 - 416 in whole tiles - and to 256 at K 16384. K 9728 x 2560
  rows, a block of 256 / 384 / 416 / 512: 4.839 (0.2) / 4.804 (1.5) / 4.828 (0.4) / 4.478 (0.5)
  at 16 lanes, 5.323 (1.4) / 5.359 (1.1) / 5.299 (0.8) / 5.139 (0.5) at 8. K 16384 x 4096 rows,
  256 / 384 / 512: 25.561 (0.1) / 25.630 (0.2) / 24.667 (0.1) at 16 lanes, 17.977 (0.8) /
  18.950 (0.3) / 17.190 (0.2) at 8.
- **What the walk scratch holds.** The panels, one allocation a calling context: stride x slots
  + 64 bytes, the stride a panel rounded up to a cache line plus a line, the slots
  `get_dispatch_slot_bound()` - 8 under this box's default job queue. A bf16 panel is form x 16
  x n x 2 bytes, 524,288 at form 2 and n 8192 (the 1B's FFN down) and 622,592 at n 9728 (the
  4B's), so the scratch at 8 slots is 4,194,880 and 4,981,312 bytes; a byte panel is mr x n
  bytes, 4,096 at mr 8 and n 512. The q8
  walk's bias sums, where the stamped plane carries a bias: 4 x ntok x n / 32 bytes, 524,288 and
  622,592 at ntok 512. Each figure is its formula's value at the named shape
  (`walk_panels`, `q8q8_bias_sums`, `kq_walk_panel_bytes` in `dasllama/dasllama_math_gen.das`).
- **The walk scratch costs no rate and saves no bytes.** The arms are the walk that allocates a
  panel per dispatch chunk (the file of master `640826d79`) and the walk on one scratch panel
  per dispatch slot. `DASLLAMA_ALLOW_UNTUNED=1` (the `x86-amx` class profile stamps the kernels,
  no sidecar), `-p 512 -n 16 -r 5 -t 16`, four passes a model in alternating order; pp512 tok/s (the spread
  over 5 reps) and the process's peak resident set read by `/usr/bin/time -v` (`out-of-process`),
  per-chunk then scratch. Llama-3.2-1B Q4_K_M: 1806.67 +- 11.73, 1856.04 +- 4.35, 1832.40 +-
  9.71, 1804.75 +- 22.14 against 1865.93 +- 6.94, 1878.12 +- 16.26, 1834.13 +- 37.13 (one pass
  void at 1798.04 +- 193.89); peak 1,976,720 / 1,976,804 / 1,976,868 kB (the box's first process
  read 2,885,920) against 1,990,836 / 1,980,220 / 1,980,472 / 1,980,224. Llama-3.2-1B Q8_0:
  1836.54 +- 2.94, 1837.14 +- 14.53, 1853.00 +- 10.72, 1860.91 +- 12.28 against 1909.45 +- 7.58,
  1863.06 +- 13.23, 1838.20 +- 27.82, 1849.83 +- 24.48; peak 2,473,812-2,474,228 kB against
  2,477,580-2,484,108. The scratch stays mapped where the per-chunk panels were freed - 3.4 to
  14 MB more at the peak - and the rates sit inside the spread between two processes of one arm.
- **One walk behind the cells reads the rate of the two it replaced.** The arms are the q8 and
  superblock cells each with its own walk and the cells on `bf16_walk`; the flags and override
  of the entry above, Llama-3.2-1B Q8_0, four passes in alternating order, pp512 tok/s, two walks
  then one: 1901.34 +- 28.39, 1924.55 +- 14.96, 1913.96 +- 9.64, 1845.53 +- 48.54 against
  1952.21 +- 6.37, 1859.75 +- 6.32, 1887.56 +- 22.77, 1900.56 +- 7.18.

### The CPU self-speculation entry (2026-09-29, Apple M5 Max, 18 lanes, decode on the 6 fast ones)

Every pair below is one process of `benchmarks/lcpp_bench.das` as the `-jit` script, master
`640826d79`: `lcpp_bench.das -- -m <gguf> --ngl 0 --mtp-ab --chat-prompts --prompts
modules/dasLLAMA/benchmarks/data/specbench4_prompts.txt --for-debug-purposes` (`--ngl 99` for
the Metal pair), `DAS_TUNE_POLICY` unset, `DASLLAMA_ALLOW_UNTUNED=1` (the `arm-i8mm` class
profile stamps the kernels, no sidecar), depth 1, served on the `arm64-gen` backend (`portable`
under Metal). The off and on arms run in that one process; tg-real128 tok/s (the spread over 5
reps, 4 prompts), off then on, with the drafts accepted.

- **Self-speculation on the CPU loses on every carrier; on Metal it gains.** CPU:
  Qwen3.5-0.8B-MTP Q8_0 189.56 +- 3.13 / 139.41 +- 3.71 (on is 0.74x off), 1005 of 1560;
  Qwen3.5-4B-MTP Q8_0 52.17 +- 1.15 / 45.65 +- 0.53 (0.88x), 1110 of 1455; Qwen3.5-9B-MTP Q8_0
  29.66 +- 0.50 / 26.50 +- 0.49 (0.89x), 1100 of 1470; Ornith-1.5-35B-A3B Q4_K_M 58.75 +- 1.05 /
  40.44 +- 0.74 (0.69x), 1095 of 1475. Metal: Ornith-1.5-35B-A3B Q4_K_M 132.53 +- 1.69 / 143.37
  +- 0.85 (on is 1.08x off), 1080 of 1470. The server's default arms the round on a one-stream
  GPU slot and leaves it off on the CPU.

### From the AMX short-batch arc (2026-09-30, Granite Rapids `c8i.4xlarge` `i-02a87cabc8fcd2e4c`, 8 cores, 8 lanes)

The race: `DASLLAMA_ALLOW_UNTUNED=1 DAS_JOBQUE_THREADS=8 bin/daslang -jit -no-module-cache
modules/dasLLAMA/harness/tile4_race.das` at commit `60beeec07` (the `x86-amx` class profile stamps
the kernels, no sidecar; `DAS_TUNE_POLICY` unset), one process a run, two runs. A four-token step
over 1536 MB of q8 planes a shape, walked plane after plane so every weight group streams from
DRAM; every arm timed once a round in turn, 31 rounds after 3 dropped. An arm is the stamped
GEMV a token, the stamped tile4, or the vector tile at a kstep (`k`), nrsplit (`n`) and prefetch
distance in bytes (`pf`); each figure is the median ms of a pass over all the planes (cv %), run a
/ run b. Every tile arm's output is bit for bit the GEMV arm's, the stamped tile4's the `k4 n4
pf2048` arm's.

- **The tile4's shape - kstep 4, nrsplit 4, a 2048-byte prefetch - leads every candidate on
  every shape, 1.45x to 1.58x the GEMV a token.**
  K 1536 x 6144 rows, 170 planes: gemv a token 28.57 (1.4) / 28.46 (1.1); tile4 (stamped) 19.70
  (1.8) / 19.58 (1.2); k1 n4 pf0 24.01 (2.5) / 23.93 (2.0); k2 n2 pf0 25.07 (1.9) / 25.08 (1.9);
  k2 n4 pf0 24.54 (2.0) / 24.48 (2.3); k4 n4 pf0 21.68 (1.9) / 21.68 (2.2); k2 n4 pf1024 21.39
  (1.8) / 21.43 (2.0); k2 n4 pf2048 20.19 (1.7) / 20.32 (1.4); k2 n4 pf4096 20.12 (1.6) / 20.13
  (1.2); k1 n4 pf2048 20.20 (1.8) / 20.11 (1.8); k2 n2 pf2048 22.11 (1.2) / 21.94 (1.0); k4 n4
  pf2048 19.59 (1.4) / 19.54 (1.1); k4 n4 pf4096 19.76 (1.4) / 19.74 (1.2).
  K 6144 x 1536 rows, 170 planes: gemv a token 28.42 (1.4) / 28.42 (1.1); tile4 (stamped) 19.66
  (1.2) / 19.66 (1.1); k1 n4 pf0 24.21 (2.3) / 24.38 (1.9); k2 n2 pf0 25.86 (2.0) / 25.76 (2.0);
  k2 n4 pf0 24.91 (3.2) / 24.70 (2.7); k4 n4 pf0 22.42 (2.7) / 22.23 (1.9); k2 n4 pf1024 22.73
  (5.3) / 21.84 (4.8), past the 3% spread and void; k2 n4 pf2048 20.52 (2.4) / 20.52 (1.7); k2 n4
  pf4096 20.45 (2.0) / 20.55 (1.6); k1 n4 pf2048 20.56 (1.7) / 20.51 (1.6); k2 n2 pf2048 22.53
  (1.2) / 22.54 (1.2); k4 n4 pf2048 19.64 (1.3) / 19.60 (1.2); k4 n4 pf4096 20.07 (1.4) / 20.05
  (1.5).
  K 2560 x 9728 rows, 64 planes: gemv a token 29.37 (1.5) / 29.71 (1.3); tile4 (stamped) 18.70
  (1.1) / 18.95 (1.0); k1 n4 pf0 23.51 (2.4) / 23.91 (2.0); k2 n2 pf0 24.61 (2.4) / 25.10 (2.1);
  k2 n4 pf0 24.00 (2.8) / 24.16 (3.2); k4 n4 pf0 21.47 (2.2) / 21.77 (2.7); k2 n4 pf1024 20.76
  (3.9) / 21.60 (4.9), void; k2 n4 pf2048 19.61 (1.6) / 19.59 (1.5); k2 n4 pf4096 19.71 (1.5) /
  19.73 (1.5); k1 n4 pf2048 19.48 (1.5) / 19.59 (1.3); k2 n2 pf2048 21.36 (1.1) / 21.52 (1.0); k4
  n4 pf2048 18.68 (1.1) / 18.89 (0.9); k4 n4 pf4096 19.09 (1.4) / 19.42 (1.6).
  K 9728 x 2560 rows, 64 planes: gemv a token 29.57 (1.9) / 29.16 (1.6); tile4 (stamped) 18.71
  (1.2) / 18.55 (1.2); k1 n4 pf0 23.76 (2.6) / 23.60 (2.1); k2 n2 pf0 25.21 (2.5) / 25.09 (2.1);
  k2 n4 pf0 24.30 (2.6) / 24.09 (2.2); k4 n4 pf0 21.77 (2.4) / 21.47 (2.4); k2 n4 pf1024 21.32
  (5.5) / 20.76 (5.4), void; k2 n4 pf2048 19.52 (1.8) / 19.39 (2.0); k2 n4 pf4096 19.67 (1.6) /
  19.52 (1.7); k1 n4 pf2048 19.39 (1.3) / 19.31 (1.6); k2 n2 pf2048 21.63 (1.2) / 21.62 (1.1); k4
  n4 pf2048 18.66 (0.8) / 18.54 (1.0); k4 n4 pf4096 18.98 (1.5) / 18.95 (1.4).
  The prefetch is worth 1.11x to 1.17x at kstep 4 (the `k4 n4 pf0` arm against `k4 n4 pf2048`),
  and that tile with no prefetch is already 1.27x to 1.37x the GEMV a token.

The served turn: `benchmarks/lcpp_bench.das` as the `-jit` script under `-no-module-cache`,
`lcpp_bench.das -- -m <gguf> -p 64 -n 128 -r 3 --npl 4 -t 8 --for-debug-purposes`,
`DAS_TUNE_POLICY` unset, the box's sidecar beside the script (its mint at master `202427bd2`),
served on the `x64-gen` backend, one process a reading, `.jitted_scripts` removed before each
process. An arm is the tree's `dasllama/` sources put in place before its process starts, and
the tile4 of these readings is the `k2 n4 pf2048` arm of the race - they were taken before it.
Each pair compares across processes and is `direction-grade`; rates are tok/s (the spread over 3
reps), tg128@4 summed over the 4 streams. The released reading of the landed shape is the
board's (`performance/records/gnr.json`).

- **A batch shorter than the amx tile's 32 tokens on the 4-token vector tile: 1.35x master's
  tg128@4 on gemma-4-E2B Q8_0, tg128 unmoved.** Master walks such a batch one GEMV pass a token
  (`bf16_walk` runs no tile below the token step); the arm hands it to the stamped perm's
  `tile4` companion, with no activation plane and no panel built. Master then the arm, tg128@4 /
  tg128: gemma-4-E2B Q8_0 76.46 +- 0.41 / 32.50 +- 0.38 against 103.05 +- 0.56 / 32.24 +- 0.40
  (a second master process read 81.36 +- 0.37 / 32.80 +- 0.04); Qwen3VL-4B Q8_0 49.91 +- 0.09 /
  18.97 +- 0.21 against 65.32 +- 0.42 / 19.30 +- 0.03; Qwen2.5-Omni-3B Q8_0 65.45 +- 0.34 /
  24.93 +- 0.34 against 86.08 +- 0.29 / 24.89 +- 0.19; gemma-4-E4B Q8_0 41.98 +- 0.43 / 16.22 +-
  0.07 against 55.27 +- 0.05 / 16.20 +- 0.04.
- **In the served turn the prefetch is most of the gain.** The arms are `tile4_perm`'s three
  constants edited in `dasllama/dasllama_gemm_gen.das`, gemma-4-E2B Q8_0, tg128@4 at kstep /
  nrsplit / prefetch distance: 2 / 4 / 0 80.35 +- 0.40; 1 / 4 / 0 74.32 +- 0.12; 2 / 2 / 0 78.73
  +- 0.22; 4 / 4 / 0 83.73 +- 1.26; 2 / 4 / 2048 101.87 +- 0.88; 2 / 4 / 4096 103.63 +- 0.32.
  With no prefetch the vector tile reads master's rate here, where the race's planes give it
  1.27x and more: a whole step streams every layer's groups, and four dots a line leave too few
  lines in flight.
- **The batch below the tile step skipping the bf16 walk reads 1.14x the walk's tile4 tail.**
  The arms are the kernels with the `below_amx_step` test forced false at its two q8 sites (the
  batch takes `bf16_walk`, whose tail below a tile runs `tile4`, after the activation plane and
  the panel scratch are built for nothing) and the kernels as they are, gemma-4-E2B Q8_0,
  tg128@4: 90.94 +- 0.19 against 103.99 +- 0.87. The predicate is structural - a batch below the
  step runs no tile, so the plane and the panel are dead work - and this pair is its reading,
  taken in two processes.
- **The superblock families gain less: their step is less bound by the weight stream.** Master,
  then the `tile4` with no prefetch in the superblock block emitters, then with the packed
  planes' prefetch, tg128@4: gemma-4-12B Q4_K_M 22.30 +- 0.13 / 22.51 +- 0.13 / 23.33 +- 0.03;
  Qwen3.8-27B IQ4_XS 8.64 +- 0.02 / 9.71 +- 0.01 / 10.29 +- 0.02. Qwen3-30B-A3B Q4_K_M read
  40.15 +- 0.71 on the no-prefetch arm; its master reading, 37.90 +- 2.09, is past the 3% spread
  and void.
- **Refuted: an `amx_bf16` perm over the bias-128 plane.** The vnni512 class's bias-128 `vpdpbusd`
  lattice reads ahead of the amx class's at tg128@4, so the arm stamped the amx perm with
  `bias=128` (the panel un-biasing the plane, the GEMV and `tile4` riding the biased lattice).
  gemma-4-E2B Q8_0, two alternated passes, unbiased then biased, tg128@4 / tg128: 103.08 +- 0.59
  / 31.53 +- 0.13 and 103.26 +- 0.69 / 32.16 +- 0.33 against 98.00 +- 0.40 / 29.97 +- 0.20 and
  99.02 +- 0.50 / 30.29 +- 0.48 - the biased arm is 0.95x on both legs. `.jitted_scripts` was
  kept across these four processes; the perm's `bias` argument keys the biased arm's kernels
  apart.

### From the Vulkan dedup pass (2026-10-05, RunPod RTX PRO 4500 Blackwell, 16 lanes, the models network volume)

The pass folds 247 tier-A and 76 tier-B duplicate sets across the Vulkan engine, its kernel classes
and the test rigs (one commit per fold, net about -5300 lines against master 1a0729a6ef) and means
to move no served number. The readings are `lcpp_bench.das` rows on the same card in one process a
row, master then the tip right after it, `--ngl 0 -p 512 -n 128 -r 5 -t 16`, the Vulkan lane minted
once per model and shared by both trees. pp512 on this card spreads past 10% between reps on the
small models (the 1B reads +-12500 of 30600 on master), so tg128 is the column that holds a verdict
and pp512 is read against its own spread.

- **tg128 holds on every row.** Master then the tip: Llama-3.2-1B Q4_K_M 554.87 +- 0.51 / 551.73 +-
  0.55; Qwen3-4B Q8_0 146.54 +- 12.62 / 152.21 +- 0.22; gemma-4-E4B Q8_0 98.33 +- 15.27 / 112.00 +-
  0.05; gemma-4-12B Q4_K_M 82.12 +- 0.02 / 82.18 +- 0.02; Qwen3.5-9B-MTP Q5_K_XL 105.20 +- 0.03 /
  105.07 +- 0.03; Qwen3.8-27B Q4_K_M under the tq4 K/V codec 36.97 +- 0.01 / 36.96 +- 0.01 and under
  q8_0 36.98 +- 0.09 / 37.01 +- 0.01. The +-12 to +-15 spreads land on either tree at random (the
  10-rep recheck put them on master once and on the tip once) - a stall, not a tree.
- **Two folds cost the Q8_0 prefill 3.7% and came back.** pp512 read Qwen3-4B Q8_0 11948 +- 344 ->
  11423 +- 256 and gemma-4-E4B Q8_0 9661 +- 113 -> 9371 +- 107, held by a 10-rep master-tip-master-tip
  recheck (11734 / 11294 / 11688 / 11277 and 9559 / 9269 / 9496 / 9268). The per-stage GPU profile
  (`DASLLAMA_GPU_PROF=1`, Qwen3-4B Q8_0, 512 tokens) put every Q8 weight GEMM stage 3-5% longer (q
  4229 -> 4426, k 2482 -> 2560, wo 3685 -> 3816, gate 8353 -> 8799, up 8288 -> 8652, down 7872 ->
  8244 us) and the rest flat. An A/B on the tip named them: `Q8Cm2T.decode` back to its 16-bit-lane
  `unpack8` select reads 11483 +- 243 (the shift form of the F8 rule change, 1.5%); that plus
  `MmBatchT`'s two tiles back as two bodies reads 11894 +- 331 (the one coopmat accumulator array
  walked under `[unroll_full]`, 2.5% - its own 4096x4096 probe had read flat per dispatch, 147.63 ->
  147.68 us; the served shape did not agree, and the old and new tiles load the same fragments, so
  the array itself is what the driver stops keeping in registers). With both back, at 10 reps
  master-tip-master-tip: Qwen3-4B Q8_0 11765 / 11753 / 11508 / 11754, gemma-4-E4B Q8_0 9610 / 9584 /
  9575 / 9587. A probe shape is not the served shape: the served row is the gate a fold passes.
- **The tq4 mirror's control share reads as the constant's doc says on this card.** The hybrid file's
  `tq4_within` holds a tolerance row to `TQ4_CTRL_SHARE` 0.85 of its distance from the one-token-off row
  (`tests/_resident_feed.das`): the share reads 0.09-0.44 on the RTX 5060 Ti and 0.73 on the RTX PRO
  4500 (the 900-token prompt cut at 300), so the bar sits halfway from the widest reading to the row one
  off's 1.0; the model-free controls hold a device row at 0.8 of the distance and red one at 0.9, and a
  NaN reference row with no control row reds. The pod's hybrid file: 69 cells, 65 passed, 4 skipped.
- **The resident llama file's Q4_0 carrier holds the file's logit bar.** `Llama-3.2-1B-Instruct-Q4_0-local.gguf`
  (the 1B requantized, the q40 planes on the window chain's kq batch arm) runs the file's cells under
  `LOGIT_BAR_REL` 0.06 of each row's largest live |logit| with the one-step-off control past it: 14 cells,
  12 passed, 2 skipped on the pod.
- **`rows_within_control` is the one bar statistic the resident rigs read.** The per-file copies
  (`carries_within`, `head_rows_within`, `rows_bar_fails`) fold onto it: every row within `rel` of its CPU
  row's largest live |value| (`live_maxabs`, a NaN and the suppressed-id floor left out) and the control row
  past `ctrl_bars` bars, the widest miss and the tightest control logged with their rows. The cells it
  serves read the same verdicts before and after the fold (hybrid, moe, hc, the regions files 26/26 twice).

### From the vision shared-kernels arc (2026-10-08, Apple M5 Max and the RunPod RTX PRO 4500 Blackwell)

The arc folds the vision towers' kernel pairs onto templates both homes stamp, runs every Vulkan
vision chain whole off the image planes on both weight lanes, and gives the Metal vision chains the
q8 lane. The lane readings below come from `harness/vision_lane_probe.das`:
`DASLLAMA_IMAGE=0 bin/daslang -jit modules/dasLLAMA/harness/vision_lane_probe.das -- <family> <q8 0|1> <mmproj> <w> <h>`
with `DAS_TUNE_POLICY` unset - the tower staged and minted in memory under the lane pin, no `.dlim`,
the family's encode (`gemma3v_encode`, `gemma4v_encode`, `qwen3v_encode`, `qwen25v_encode`) called
thirteen times on a checkerboard canvas, the median of the last ten in ms. The Metal box ran the
probe alone, no box profile applied; the Vulkan box ran it under `DASLLAMA_GPU=1` with
`DASLLAMA_COOPMAT` unset (the cm2 arm). Each lane is its own process, so a lane pair is
`direction-grade`. Encode ms, the exact lane then the q8 lane of the same file.

- **The lane the policy keeps is the faster one on each home** (`direction-grade`). Metal (M5 Max):
  gemma3v at the fixed 896 canvas 340.9 / 507.3, gemma4v E2B at 672x336 29.1 / 30.1, qwen3v 4B at
  448 26.2 / 55.2 - the f32 tiles beat the q8 GEMM ladder with its half feed on every family, so the
  Metal chains keep the file's planes as the default. Vulkan (RTX PRO 4500): gemma3v 281.5 / 88.2,
  gemma4v 26.5 / 23.3, qwen3v 48.0 / (no figure: the q8 encode alternates 34 and 66 ms on
  consecutive calls, the pre-arc driver the same, `followup_vulkan.md` row 146), qwen25v 30.8 -
  the exact lane rides the f32 tile over the whole blob (no feed, no schedule) and the q8 lane the
  cm2 tiles, so the Vulkan chains keep the q8 image as the default and serve a pinned exact tower on
  the f32 tile; the f16 slab for the exact lane is `followup_vulkan.md` row 95's lever.
- **The qwen25v Metal chain's folds read 67.5 -> 52.8 -> 50.9 ms** on the 448 checkerboard
  (`direction-grade`, the probe at three commits of the arc): the shared bias-gate stamp storing the
  halves the down GEMM reads in one dispatch (52.8), then the seam stamps folding each block's
  pre-norm into the residual add (50.9).
- **A weight drop discharges the vision towers' device memory.** The Metal tower frees its q8
  vision slab (`g_tw_q8_vis` - the block regions' q8 bytes and scales, one slab for the file, keyed
  on the blob and the scales so a re-mint of the same file reuses it) on `tw_weights_drop` and at
  shutdown; the Vulkan tower forgets its vision marks (`vt_vis_forget` - the planes, im2col, stem
  weight and norm buffers' capacities and the stem-slab freshness) with its scratch, so the next
  file's canvas grows them from zero instead of reading a stale key as fresh.

### From the llama leg's K/V mirror slabs (2026-10-11, RunPod RTX PRO 4500 Blackwell 32 GB, the models network volume)

- The whole-model driver's K/V mirror allocates `2 x regions x n_slabs x widest-slab units x unit bytes`
  (`resident_mirror_bytes`): the slabs are contiguous layer runs under one binding's range, the cut the one with the
  least `n_slabs x widest`, so the padding is the widest slab less each other slab. Before the slabs each side was one
  binding over every layer's rows, and a context asked whole past it declined the driver: Llama-3.2-3B f16 held 18715
  positions a region at four regions, and the house's `--ctx 32768 --streams 4` server ran on the per-op tier (the earlier
  row). With them the 3B at 32768 x 4 regions seats 28 layers of 2 KiB rows a position in two slabs of 14, 15.0 GiB
  charged and allocated, no padding, and the server arms the resident driver under the house's flags.
- Llama-3.2-3B-Instruct Q4_K_M, the resident rows against the CPU chain over a 12288-token prompt and eight fed steps on
  the f16 mirror at four regions, the cm2 tiles (`test_gpu_resident_regions_llama3b_ctx32k.das`, the 10% bar the codec
  llama files hold): the prefill row reads 0.47 of a 24.5 max logit (bar 2.45), the steps 0.22 to 0.63 against bars 0.57 to
  0.78 - the tightest 0.63 of 0.78 at step 0 - and the one-step-off control 1.1 to 19; the 30000-token prompt continued
  from 20480 and four device-home streams at 9000 to 27000 positions read bit for bit the one-call prefill and the
  sessions alone.

### From the llama leg's parity gaps (2026-10-12, RunPod RTX PRO 4500 Blackwell 32 GB - `DAS_TUNE_POLICY` unset, the x86-avx2 class profile stamped, `DASLLAMA_ALLOW_UNTUNED=1`, `DASLLAMA_COOPMAT` unset - and Apple M5 Max)

Every rate here is `debug-jit`: the `-jit` script form `DASLLAMA_GPU=1 DASLLAMA_MODELS_DIR=<models> bin/daslang -jit
modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m <gguf> --for-debug-purposes`, no fat exe; the Vulkan rig has no leg (no
record store carries a Vulkan row), so the pod's figures are direction-grade throughout.

- **The decode rope-and-store on one template leaves the Vulkan step where it was** (`direction-grade`, two commits).
  Llama-3.2-3B Q4_K_M, the bench above with `--plen 0 --reps 3` and `DASLLAMA_GPU_PROF=1 DAS_LOG_LEVEL=info`, two alternated
  rounds an arm on a warm card: tg128 227.8 +-0.05 / 227.6 +-0.18 tok/s at 8624bce999 (`RopeKvStore` with its own body), 227.4
  +-0.01 / 227.7 +-0.07 at 5c5d49e363 (`GkRopeKvStore`'s `rope_kv_pair`, the partial-rotation cover and the per-row table
  stride in the shared body); the profile's rope stamp 113 -> 115 us a token over 28 layers (the `vk_rdec gpu avg/token`
  line of the same runs). A first pair of runs separated by a rebuild had read 252.0 against 250.3 at `--reps 5` with the
  profile off - the card's clock state between builds, not the kernel. On the M5 (`DASLLAMA_CPU_PREFILL=1`, the m5 tune
  manifest, `--ngl 99`, `--reps 5`, three alternated pairs on worktrees of the two commits) the fold reads pp512 5541 / 5534 /
  5507 against 5538 / 5523 / 5490 tok/s and tg128 205.3 / 204.8 / 204.6 against 205.1 / 204.7 / 204.7, every tg128 at
  +-8.5 - the box's own spread.
- **A short window on the Vulkan chain costs about two decode steps whatever its row count, and the peel brings it to
  one.** The race bed `DASLLAMA_GPU=1 bin/daslang -jit modules/dasLLAMA/harness/vk_short_window_race.das -- --model <gguf>`
  (`--rows 1,2,4,8 --reps 5 --prompt 40`: the lever `set_vulkan_prefill_peel` flipped between the forms in one process, five
  interleaved reps a form, the fastest kept; `ARCHITECTURE_GPU_VULKAN_WINDOW.md#vk-prefill-peel`) at 0f1e793e18 reads on the 3B
  peel 6.5 / 6.9 / 4.5 / 7.4 ms against the chain's 13.5 / 11.7 / 13.4 / 13.5 at 1 / 2 / 4 / 8 rows, on the 1B Q8_0 2.2 / 4.1
  / 4.2 / 3.0 against 5.4 / 6.6 / 6.6 / 5.5, the forms' last-row logits at most 0.27 apart on the 3B and 0.23 on the 1B (the
  chain's tiles and the command's GEMV leaves round apart; the split bar holds them). The bed prints the slowest rep beside the
  fastest from 0f1e793e18 on; the rows above predate that print. At e38a26bb46 the same bed reads on the 3B peel 4.0 / 4.2 /
  7.0 / 7.4 against the chain's 13.4 / 13.5 / 11.8 / 11.8, on the 1B 3.8 / 2.5 / 4.2 / 4.6 against 6.6 / 5.5 / 6.7 / 6.6, the
  slowest rep of either form 35 to 78 ms (the first rep's compile), the logits at most 0.38 and 0.23 apart, no call passed to
  the CPU rails: the two readings part most on the peel at one row (6.5 against 4.0), the chain within 0.3 ms.
- **The one-row decode's attention split follows the command's rows.** `rd_layer_pieces` divided the SM wave by the per-row
  planes' height - the region count before this leg, eight after - so the one-row command split a head's keys into a quarter,
  then an eighth, of the pieces that fill the card. The instrument (`external`, outside the tree: a client replaying 22
  conversation turns of 4000 to 6000 tokens, 55 to 5900 of them cached, against `DASLLAMA_GPU=1 DASLLAMA_GPU_PROF=1
  DAS_LOG_LEVEL=info bin/daslang -jit utils/dasllama-server/main.das -- -m <3B gguf> --ctx 32768 --streams 4`; its
  server-side `vk_rdec gpu avg/token` lines are the figures; `direction-grade`, three commits) read the decode attention at
  2220 us a token at 1e2302d610, 3780 at 5c5d49e363 (the planes at eight) and 1274 at a343012085 (the wave over the command's
  own rows), the step 6232 / 7792 / 5295 us. The short-context bench never sees it: at 128 keys the attention is 3% of the
  step.
- **The Metal side of the leg reads the same as its base on the house's shape.** The same client (`external`, outside the
  tree, `direction-grade`) on the M5's M4 arm, Llama-3.2-3B Q4_K_M, two alternated rounds of the base tree and the branch:
  the arms sit inside the rounds' own drift, so the probe decided nothing moved; its walls are the client's and stay
  out of this ledger.
- **The leg's lasting allocations, in bytes.** The segmented window's staging (`g_rdec_seg_x`, `g_rdec_seg_cos`,
  `g_rdec_seg_logits`, `g_rdec_segs`, `g_rdec_seg_regions`): `rows x dim x 4` + `rows x cos_elems x 4` + `regions x vocab x 4`
  + `segments x 32` + `segments x 8` - on Llama-3.2-3B (dim 3072, head 128, vocab 128256) a 512-row window of two segments
  holds 6.3 MB + 0.26 MB + 1.0 MB + 80 B at two regions, 12.6 MB + 0.52 MB + 2.1 MB at four regions and a 1024-row window.
  The grouped step's `g_rdec_bregs` / `g_rdec_grp_toks`: `rows x 8` each, 96 B at twelve rows. The window's segment tables
  `g_pf_segs` / `g_pf_wsegs`: `segments x 32` each. The driver's logits slots `pf_logits_host`: `regions x vocab x 4` (host,
  was `vocab x 4`) - 2.1 MB at four regions on the 3B against 0.5 MB before. The per-row planes at `RDEC_NROW_MAX` rows
  whatever the region count, where they held one row a region: the plan charges `(vocab x 4 + heads x RDEC_ATTN_SPLIT_CAP x
  (hs + 2) x 4) x 8` = (513 KB + 200 KB) x 8 = 5.7 MB on the 3B, against 0.7 MB a region before; the x, cos, residual and
  logits rows the command stages scale the same way (`dasllama_vulkan_decode.das`, `vk_rdec_prepare`).
- **The leg's footprint on the one-region bench is the base's.** `/usr/bin/time -v` around the bench above (`--plen 0 --reps
  2`, Llama-3.2-3B Q4_K_M, the JIT cache warm, two runs an arm): max RSS 3687160 / 3656932 KB at faf9ef95e8 against 3672036 /
  3988724 KB at 1e2302d610 (the base's higher run the first after its rebuild - a JIT recompile; a cold tip run read 5740944 KB
  at 294 s of user time against the warm runs' 16 s), tg128 250.3 +-0.1 against 252.2 +-0.0 tok/s (`direction-grade`, two
  reps; the fold's 0.7% above).
- **The segmented window's rows against the one-session window, on the 1B.** `test_gpu_resident_segments.das` at
  a9fa027c9c under `DAS_LOG_LEVEL=info`, the split bar `SPLIT_BAR_REL` 0.06 of the row's max |logit| (bars 0.47 to 0.85): the
  two tails as one segmented call read a widest miss of 0.18 (Q8_0) and 0.47 (Q4_K_M, session B's 31-row tail at a 0.50 bar -
  the K-quant window's GEMM ladder at the two row counts) on the f16 mirror, 0.20 and 0.39 on the f32 mirror
  (`DASLLAMA_VK_KV32=1`); the two openings as one window 0.07 / 0.12; the 300-row tails 0.13 / 0.26; the decline cell's two
  sessions, prefilled on the CPU rails after the pin and held to the device-prefilled references, 0.22 / 0.20 (Q8_0) and
  0.35 / 0.35 (Q4_K_M) against bars of 0.53 to 0.55; every control 2.5 to 8.7 bars out.

### From the prefill rotation arc (2026-10-18, the reference box: Threadripper 3990X at 2666 MT/s, RTX 4080 16 GB, Windows, NVMe; `DAS_TUNE_POLICY` unset, the x86-avx2 class profile stamped)

Every rate here is `debug-jit`, the `-jit` script form `DASLLAMA_GPU=1 DASLLAMA_GPU_PF_STREAM=1 DASLLAMA_MODELS_DIR=<models>
bin/daslang -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m <gguf> --pp 3000 --tg 128 --for-debug-purposes` at the
capped prompt window (`DASLLAMA_GPU_PF_ROWS=2048`, which a recurrent carrier cut at 1024 when these were taken - the
window's cap, `DN_WINDOW`, is 2048 since, and the prompt window's default with it; the rates at the uncut 2048 window are
`followup_vulkan.md` rows 150 and 151's), the frozen fixture's gate 8/8 on every run; no record store carries a Vulkan row,
so every figure is direction-grade.

- **The arc's headline rows.** Qwen3.6-35B-A3B UD-Q4_K_M, 28 of 40 routed layers streamed, `DASLLAMA_EXPERT_CACHE_MB`
  unset: pp3000 1488, tg128 67.5 tok/s, the hot pool 76 slots a layer (the 64-slot cap gone: a slot for every expert
  that fits beside the mirror); Qwen3.8-Flash-Next UD-IQ4_XS, every routed layer streamed: pp3000 440, tg128 30.2, the
  pool 41 slots. The ring form of the streamed slot: 250 MB from a whole layer's 476 on the 35B, 445 MB from 1.7 GB on
  Flash-Next (the `streamed prefill engaged` line of each load); on Flash-Next the plan counted the slot for the first
  time, 41 slots a layer against 44 that sat 1.7 GB past the plan's room.
- **The DMA fill of the pool's misses** (`DASLLAMA_GPU_HOT_DMA=1` against unset, the bench's tg128 at the same commit, one
  run an arm): the 35B 19.8 ms a token against 23.6, the pool serving 97.5% of the picks against 78.7%; Flash-Next 56.5
  against 62.3. A prompt window's fill stays the host's place: the DMA fill beside the window's own copies cost pp3000 a
  fifth. The knob ships off.
- **The prompt window's pool prefix** (`DASLLAMA_GPU_HOT_DMA=1`, the bench's tg128 behind its pp3000 against the knob
  off, the 35B): the decode's pool hits 90.7% against 79.0%, its host experts 2.5 ms a token against 6.0; a 64-row window
  on layer 12 serves 414 of 512 rows off 29 hits with nothing copied (`DASLLAMA_GPU_PROF=1`'s window line).
- **The expert cache at half the experts** (`DASLLAMA_EXPERT_CACHE_MB` at half the streamed layers' expert bytes against
  unset, the same bench): Flash-Next pp3000 577 and tg128 19.0 against 598 and 21.1; the 35B's on-demand reads halve
  and its decode rate doubles once the heat profile seats the hottest experts (the second run of the same prompt).
- **The lane read off the file** (`DASLLAMA_VK_STREAM_FILE_READ` on against `=0`, the 30B's warm load): 7.7 s against
  8.5, the peak working set down by the streamed layers' twin, 8 GB. The readers' arm: `harness/kq_disk_rail_probe.das`
  and the reader threads over a mapped lane read 0.77 GB/s unbuffered (the kernel serializes direct reads of a mapped
  file), 1.97 GB/s unbuffered unmapped, 1.68 GB/s buffered mapped - the readers are buffered.
- **The submit grain** (`harness/vk_submit_grain_probe.das`, one 64 MB copy against 512 pieces of 128 KB on the transfer
  queue): about 4 us a submit, 512 pieces costing what one does within the noise.
- **The lasting allocations the arc adds, a model**: the streamed slot as a ring - 250 MB (35B) / 445 MB (Flash-Next) in
  place of 476 MB / 1.7 GB; the expert cache's transient ring - the non-resident count of experts a plane kind, 0 bytes
  with the cache off; the readers' bounce buffers - eight pages-aligned slots, one a reader; the heat table - eight bytes
  an (layer, expert); the pinned stream mirrors - the streamed layers' expert bytes under `DASLLAMA_GPU_PF_STREAM=1`,
  the same bytes the host twin read before; the pool - every slot that fits beside the mirror (76 x 28 layers on the
  35B, 41 x 48 on Flash-Next), where the cap of 64 left room idle. The engaged line prints the host-visible total and
  the das heap beside the slot.
- **The bars the arc's cells hold** (`tests/run.das -- --suite stocked` on this box; the readings the cells print under
  `DAS_LOG_LEVEL=info`): `test_gpu_resident_continuation.das` holds Qwen3.5-0.8B-Q4_K_M's continued and adopted rows
  within `CONT_BAR_REL` 0.05 of the whole prompt's row (readings 0.3-1.2% of the row's max, the one-token-short control
  past 1 bar); Qwen3.8-Flash-Next UD-IQ4_XS at `ROUTED_WIDE_BAR_REL` 0.5 (the same device run twice reads up to 20%: a
  near-tie expert flips on the pool's fill and the meet's split), its n-gram conv ring after a window row for row the CPU
  chain's within 2%, a zeroed ring 15% off; `test_gpu_resident_long_window.das` holds the 0.8B's last row at a 2048-row
  pin within `BAR_REL` 0.10 of the CPU chain's (reading 3%; an uncut 2048-row window read 30%, 1536 read 72%);
  `test_gpu_resident_moe_formats.das` holds every moe-formats carrier at `MOE_LOGIT_BAR_REL` (the routed block's bar
  `test_gpu_resident_moe.das` reads) through the one-copy rail, one window and two, and the expert-cache cell's
  second feed reads fewer planes than its first.

### From the dasCUDA arc (2026-10-22, the reference box: Threadripper 3990X, RTX 5060 Ti 16 GB, driver 610.74, Windows, clocks locked at 2400; and 2026-10-23, RunPod RTX PRO 4500 Blackwell 32 GB, driver 595.91, Linux, on boost - the container refuses `nvidia-smi -lgc`)

Every rate of ours is `debug-jit`, untuned: `DASLLAMA_GPU=1 DASLLAMA_ALLOW_UNTUNED=1 [DASLLAMA_CUDA=1] bin/daslang -jit
modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m <gguf> -p 512 -n 128 -r 3 -t 16 --ngl 0 --for-debug-purposes` (the pod's runs
add `-o md`), the CUDA home under `DASLLAMA_CUDA=1` and the Vulkan home without it. llama.cpp's run is its b10660 CUDA build,
`llama-bench -ngl 99 -fa 1 -p 512 -n 128 -r 3` (`ARCHITECTURE_MEASUREMENT_REFERENCE_ROWS.md#cuda-reference-rows`). Each figure is the mean of
3 reps +/- their standard deviation, as each bench prints it. No record store carries a CUDA row (the bench stamped the
device tier `vulkan` then; the `cuda` leg's rows are `followup_general.md` row 272's), so every figure is direction-grade; pp512 / tg128 in tok/s.

- **The 5060 Ti, ours CUDA against llama.cpp CUDA.** Llama-3.2-1B Q8_0 26840 +/- 377 / 273 +/- 0.9 (llama.cpp `external`:
  19083 +/- 1030 / 254 +/- 0.4); Q4_K_M 19675 +/- 170 / 395 +/- 1.0 (llama.cpp `external`: 17692 +/- 1043 / 372 +/- 4.2);
  Q5_1 19927 +/- 229 / 363 +/- 0.5 (llama.cpp `external`: 17867 +/- 1125 / 331 +/- 0.4); IQ4_XS 19666 +/- 82 / 427 +/- 0.6
  (llama.cpp `external`: 19677 +/- 1489 / 388 +/- 5.2). Llama-3.2-3B Q8_0 9982 +/- 126 / 109 +/- 0.3 (llama.cpp
  `external`: 7751 +/- 485 / 106.6 +/- 0.6). Llama-3.1-8B Q4_K_M 3371 +/- 10 / 80.5 +/- 0.09 (llama.cpp `external`: 3229
  +/- 63 / 80.5 +/- 0.75); Q8_0 4627 +/- 8 / 49.4 +/- 0.06 (llama.cpp `external`: 3436 +/- 82 / 46.8 +/- 1.9). The token
  step is a captured graph replayed under programmatic launches, the attention unsplit under `RD_UNSPLIT_POS` (the entries
  above).
- **The pod, ours CUDA / ours Vulkan / llama.cpp CUDA.** Llama-3.2-1B Q8_0 50581 +/- 255 / 505.1 +/- 0.3, 41081 +/- 88 /
  502.4 +/- 0.2 (llama.cpp `external`: 38686 +/- 6871 / 489.9 +/- 1.6); Q4_K_M 40385 +/- 34 / 682.8 +/- 0.3, 40477 +/- 110
  / 644.4 +/- 0.3 (llama.cpp `external`: 35605 +/- 5446 / 687.2 +/- 0.9); Q5_1 42043 +/- 31 / 665.9 +/- 3.1, 25629 +/-
  17597 / 625.3 +/- 0.3 (the Vulkan pp carried one slow rep in two runs) (llama.cpp `external`: 35249 +/- 6060 / 634.4 +/-
  2.5). Llama-3.2-3B Q8_0 17200 +/- 139 / 202.4 +/- 0.3, 15232 +/- 118 / 204.2 +/- 0.04 (llama.cpp `external`: 16072 +/-
  1908 / 202.9 +/- 0.3); Q4_K_M 15182 +/- 500 / 293.9 +/- 0.9, 15050 +/- 352 / 286.9 +/- 0.1 (llama.cpp `external`: 14887
  +/- 1721 / 295.5 +/- 0.3). Decode at parity with llama.cpp on every row (0.99 to 1.05), prefill ahead on every row
  (1.02 to 1.31); the Q5_1 prefill gap the 5060 Ti showed before the per-32 l and m leaves is gone on both cards.
- **The lasting allocations the CUDA route adds, a model** (`dasllama_cuda_decode.das`; computed from the geometry, no
  device measurement; the 1B values at dim 2048, 16 layers, kv_dim 512, hidden 8192, vocab 128256, 32 heads of 64, `rope` (the
  rope row's floats) 64, one region, on the 5060 Ti's 36 SMs). The weight arenas (`cud_arena_reserve`): a format's
  `blocks x (quant bytes + scale bytes)` a block over every plane the resident walk places in it, at least 64 scale
  bytes - the model's resident plane bytes. The K and V mirror slabs: `2 x regions x (mir_units + slack units) x unit
  bytes`, `mir_units` the plan's `n_layers x n_ctx x kv_dim` on a dense model and the slack `RDEC_MIR_SLACK` (131072)
  elements - under f16, `4 x regions x (n_layers x n_ctx x kv_dim + 131072)` bytes: 134742016 at n_ctx 4096, 4295491584 at
  131072. A block codec's f16 shadows (`ksh`, `vsh`): `4 x (n_ctx x kv_dim + 131072)` bytes - 8912896 at n_ctx 4096,
  268959744 at 131072 - the slack added to the element count as the Vulkan home's shadow adds it. The step's per-row planes at the N-row command's 16 rows: `16 x (16 dim + 1.125
  wide + 9.125 qd + 8 kv_dim + 9.125 hidden + 4 rope + 4 vocab) + 64 n_heads x (nsplit x (head + 2) + 1) + 8192` bytes,
  `wide` the widest of qd, hidden and dim and `nsplit` `da_nsplit`'s (3 on the 1B), plus `128 x kv_dim` under a block codec
  - 10860544 on the 1B, the logits 8208384 of it. The norms plane `4 x ((10 n_layers + 1) x dim + 2)`. The mapped host
  blocks: the in block `4 x (144 + 16 x (rope + dim))` (135744 on the 1B), the out block `64 x vocab + 64` (8208448). The
  window planes (`crd_pf_setup`, `np` the prompt window's rows, 2048 by default): `np x (15.125 dim + 11.125 qd + 12
  kv_dim + 11.125 hidden + 4 rope) + 256 x (dim + qd + hidden)` at the model's widest layer, plus `8 x np x kv_dim` under a
  block codec, the split-k scratch `262144 x SMs` and a schedule buffer of `4 x (4 + max(cm2_tiles_at(dmax, np, 32), 8 x
  cm2_tiles_at(dmax, np, 128)))` bytes a role a layer (`dmax` the widest GEMM output), seven roles - 90965760 at np 512, 326108928 at 2048 on the 1B.
- **The cm2 tiles at two blocks an SM on the 5060 Ti** (`min_blocks = 2` at 128 registers, `dasllama_cuda_classes.das`).
  The l tile: the gate/up shape 458 -> 344 us, the down shape 402 -> 424 us, the square shape even. The m tile under the
  pipelined decode: 93 / 351 / 345 us at the window's three shapes, against 102 / 413 / 377 us unbound.
- **The window GEMMs' k split on the 5060 Ti** (`cud_cm2_pick`). Unsplit, the down and q/o shapes run 32 l tiles on 36
  SMs, one block an SM; the k/v shapes run 16 m tiles. A grid short of two blocks an SM splits k until it fills them.
- **The pod's cells on the Linux build.** `tests/cuda` 79 of 79, `test_cuda_kernels.das` 44 of 44, the CUDA llama parity
  file 19 of 22 (three unstocked mints skip), the codec file 6 of 6, the batch file 6 of 6, the segments file 2 of 2 with
  the recurrent and E-series cells skipping under the CUDA ask by design.
- **The MMQ leaves against the cm2 tiles, the 5060 Ti at locked clocks** (`harness/gemm_cuda_vs_vulkan.das -- --only
  <fmt>`, the 1B window's three shapes at 512 rows, the Q4_K cm2 tiles as the control; `followup_general.md` item 245
  carries the pick). Per kernel: the Q8 leaf 67 to 74 TFLOP/s, Q4_0 65 to 72, Q4_K 63 to 66, IQ4_XS 63 to 69, IQ4_NL 60
  to 65, Q5_0 51 to 55, Q5_1 47 to 50, the Q4_K cm2 tiles 41 to 55; a Q6_K leaf on the k16 int8 product 36.8 to 38.9
  against the Q6_K cm2 tile's 38 to 51 (the leaf is not kept - MMQ is a per-32-scale property). The windows
  (`lcpp_bench`, the command above, `DASLLAMA_CUDA_MMQ=1` against `=0`): pp512 MMQ against cm2 27150 against 20310 on
  Llama 1B Q8_0, 10050 against 7065 on Llama 3B Q8_0, 7045 against 4883 on Qwen3-4B Q8_0, 8019 against 5653 on Phi-3.5
  Q8_0; the Llama 1B windows 1.27x (Q4_0), 1.23x (IQ4_XS), 1.16x (Q4_K_M), 1.15x (IQ4_NL), 1.05x (Q5_0) and 0.96x (Q5_1)
  over the tiles; on the pod 50751 against 39945 on the 1B Q8_0 and 17174 against 14559 on the 3B. tg128 the same on both
  forms everywhere.
- **The FP4 leaf (`CuNv4Mmq`) and its feeds, the 5060 Ti at locked clocks** (`harness/gemm_cuda_vs_vulkan.das -- --only
  nv4,q40`, 512 rows, the 1B window's three shapes; commit 9a69dcc10a the leaf, 158c85196c the feeds, a7657463c9 the strip
  code). The leaf 39.2 / 141.8 / 144.8 us = 110 / 121 / 119 TFLOP/s against the Q4_0 int8 leaf's 66.0 / 252.0 / 240.2 us
  (65 / 68 / 72) and the cm2 f16 tiles' 101 to 111 / 307 to 325 / 381 to 419 us, bounded against the e2m1 x UE4M3
  reference at 5e-6; one block of 512 threads an SM at 86 registers and 45056 B of shared. The 2-block form (`min_blocks =
  2`, 64 registers) read 42.8 / 150.7 / 162.0 us - 9 to 12% slower, spilling; not kept. The feeds on the 8B NVFP4 window
  (nsys 2026.4.1, `DASLLAMA_CUDA_PDL=0`): a lane a k64 of strided scalar loads read `CuFp4ActRows` 534 us a call,
  `CuClsArAddRmsFp4B` 68, `CuFp4Rows` 32 (the Q8 twins 142 and 42; the feeds 22 ms of the window's 64 ms of GEMM); a lane
  a float4 chunk reads 175 / 51 / 13 us, and the window went 5472 to 6265 pp512. A row kept in `float4[16]` registers
  between the two passes read 185 / 22 us for the act and plain feeds - no gain and a loss; not kept. The rows: Llama
  3.2-1B NVFP4-local pp512 19821 / tg128 400.7 on the cm2 tiles, 26113 / 404 on the leaf, our Q4_0-local 25546 / 413.0
  (llama.cpp `external`, native FP4: 26594 / 380.4; Q4_0 19580 / 402.7); Meta-Llama-3.1-8B NVFP4-local 5472 -> 6265 / 83.0
  -> 82.4, Q4_0-local 4520 / 84.8 (llama.cpp `external`: 5948 / 86.0; Q4_0 3691 / 86.3). The pod (boost): 1B NVFP4 55133 /
  733, Q4_0 49114 / 757 (llama.cpp `external`: 41921 +/- 13663 / 700; Q4_0 37702 +/- 5962 / 732); 8B NVFP4 11787 -> 14072 /
  150.3, Q4_0 8017 / 157.7 (llama.cpp `external`: 12047 / 160.2; Q4_0 7903 / 158.8).
- **The nv4 decode GEMV's nibble decode, the 5060 Ti at locked clocks** (`harness/gemv_cuda_vs_vulkan.das -- --only
  nv4,q40`, the 8B shapes at the rule's lanes and 8 / 16 / 32, CUDA event us; commit 516e4e1fb8). The SWAR formula at the
  rule's lanes: nv4 27.75 / 84.33 / 88.84 against Q4_0 26.48 / 84.14 / 84.77 (4.8 / 0.2 / 4.8% behind - the whole of the 3%
  tg gap, weighted). The byte permute at 32 lanes a row: nv4 26.60 / 84.13 / 85.02 against Q4_0 26.47 / 84.07 / 84.41
  (0.5 / 0.1 / 0.7%). The 8B token step back to back (`lcpp_bench -p 16 -n 128 -r 5`, clocks 2392): NVFP4 80.30 +/- 2.24
  against Q4_0 80.80 +/- 2.35 tok/s. The pod after it: 8B NVFP4 tg128 150.3 -> 154.7, Q4_0 157.8.
- **The gate and up GEMVs as one dispatch at one row** (`CudLayer.gu_merged`, commit 634d570472; the Vulkan twin
  `RLayer.gu_merged`, 3d905bc190). Per layer under nsys with `DASLLAMA_CUDA_PDL=0` on the 8B token step: ours 332 us
  against the reference build's 347 traced; its `rms_norm_f32` 3.2 + `quantize_q8_1` 2.9 x 5 against our `ArAddRmsRqK`
  4.3 and requants 0.9 to 1.3; its `flash_attn_ext_f16` 5.5 + `flash_attn_combine_results` 5.8 against our 7.0; its
  `rope_neox` 1.0 x 2 + `k_set_rows` 1.0 against our 1.5; the one line ours lost, gate + up as two GEMV launches of 83.3
  against its one fused `mul_mat_vec_q` launch of 162.4. (Under `DASLLAMA_CUDA_PDL=1` the trace counts a kernel's
  `griddepcontrol.wait` as its span - every small kernel read 6 to 14 us and the lever was a phantom.) The A/B, `lcpp_bench
  -p 16 -n 128 -r 5` at clocks 2392 on the 8B: NVFP4 84.02 -> 84.35, Q4_0 84.09 -> 84.53 tok/s; the pod 8B NVFP4 tg128
  154.7 -> 155.9, Q4_0 157.8 -> 158.9 (llama.cpp `external` Q4_0 158.8). The Vulkan twin on the 5060 Ti: Q4_0 78.10 ->
  78.49, NVFP4 77.55 -> 79.00. The merge sizes the gate buffer `hid x 4 x max(nb, 2)` bytes in place of `hid x 4 x nb` -
  at nb = 1 one more f32 row (57344 bytes on the 8B, 32768 on the 1B), no change above it; its wall-clock before and
  after is the A/B above.
- **W4A4's perplexity cost on the FP4 leaf** (`harness/w4a4_ppl.das -- -m <nv4 gguf> --text wiki.test.raw --windows
  <n> --ctx 512 --stride 400`, `DASLLAMA_GPU=1 DASLLAMA_CUDA=1 DASLLAMA_ALLOW_UNTUNED=1`; the FP4 leaf through
  `set_cuda_mmq_pin(1)` against the f16 tiles through `set_cuda_mmq_pin(0)` on the same windows and the same nv4 weights,
  the next token scored off the last row, the delta in nats +/- its standard error; commit a7657463c9). The nearest strip
  code: 1B (600 windows) 16.74 against 15.37, +0.0858 +/- 0.0163 (+9.0%); 8B (300 windows) 8.855 against 8.491, +0.0420
  +/- 0.0140 (+4.3%). The least-squared-error pick over five codes: 1B 17.17 against 15.37, +0.1111 +/- 0.0156 (+11.75%);
  8B 8.00 against 7.59, +0.0527 +/- 0.0169 (+5.4%); the 8B pp512 6265 -> 5872 - worse on both counts, not kept. The nearest
  code stepped up once where it saturates the strip's largest value (`fp4_strip_code`, kept): 1B 16.23 against 15.37,
  +0.0550 +/- 0.0166 (+5.7%); 8B 7.80 against 7.59, +0.0277 +/- 0.0188 (+2.8%); the argmax 267 of 271 and 153 of 153. The
  reference build's own rows (`llama-perplexity -c 512` on wikitext-2 test, the CUDA build b10660): 8B Q8_0 7.3282,
  NVFP4-local 8.3443 (its W4A4 at batch 512) against 8.0135 at `-ub 8` (weight-only), Q4_0-local 7.7523, Q4_K_M 7.4978; 1B
  Q8_0 14.0135, NVFP4-local 17.0224 against 16.0061 at `-ub 8`, Q4_0-local 16.0415, Q4_K_M 14.5497; a 1B NVFP4 mint with an
  imatrix 17.0224 - identical, its quantizer reads no imatrix for this type. The format's weights sit 6 to 8% behind Q4_0
  on these models by that quantizer; W4A4 adds 2.8 to 5.7% on ours and 4.1 to 6.3% on the reference build.
- **The lasting allocations the FP4 leaf and the nv4 planes add** (`crd_pf_setup`, `np` the window's rows; computed from
  the geometry, no device measurement). The FP4 feeds, when a plane takes the leaf: `np x (dim + qd + hid) x (1/2 + 1/16)
  + 12 np` bytes - the nibbles, a UE4M3 code a strip of sixteen, an f32 row scale a feed - 3545088 at np 512 on the 1B
  (dim 2048, qd 2048, hid 8192), 14180352 at 2048; 6494208 and 25976832 on the 8B (4096, 4096, 14336). The Q8 feeds' block
  sums (`pf_xsum`, `pf_asum`, `pf_hsum`), when a plane takes a min-term leaf: `np x (dim + qd + hid) / 8` bytes - 786432
  at np 512 on the 1B. `Model.site_scale`: one f32 a `.scale` tensor of the file, in the model and the image.
- **The CUDA resident bars** (`tests/_cuda_feed.das`, of the step's max |logit| against the CPU chain). The dense
  carriers' `LOGIT_BAR_REL` 0.06 holds the Llama 1B Q8_0 / Q4_K_M / Q5_1 / Q5_0 / IQ4_XS / IQ4_NL files, the Phi-3.5-mini
  Q4_K_M (its two-window arm; the one-window arm's steps 41 and 44 read 2.64 and 2.75 of 34 and 42 - 7.7 and 6.6% - the
  argmax the CPU's, `followup_general.md` item 268) and Q8_0 files, the Qwen3-0.6B and 4B files, and the 1B NVFP4 mint
  on the cm2 tiles. The 1B NVFP4 mint on the FP4 leaf holds `LOGIT_BAR_REL_FP4` 0.14: the arm's worst step 0.135, the
  tightest one-step-off control 0.158 - W4A4's activation step, which the f32 chain does not take.

### From the Pocket TTS small-model leg (2026-10-23, Apple M5 Max, the box profile re-minted after the macOS update; `-jit`, the JIT tier, `DAS_TUNE_POLICY` unset; `direction-grade`)

Every rate here compares across processes or commits (one process on the box; the small files are compared with one
another and the 100M across commits). The instruments: the corpus real-time factor is `harness/tts_rig.py`'s RTF column
(`python3 modules/dasLLAMA/harness/tts_rig.py --model <file>`: the 200 sentences at `alba` through the facade, the voice
state built once; a mean over the 200 inputs - the rig prints no spread); the stage split is the facade's
`DAS_LOG_LEVEL=info` timings line over one synthesis of `harness/pocket_pace.das`'s four-line stanza (7 s of speech; three
runs within 3 %; the CPU kernels the `arm64-gen` backend the load logs, the Metal seats on unless `DASLLAMA_METAL_TOWER=0`
names the CPU arm).

- **The 2026-10 small models on the CPU chain against the 100M model on the CPU tower and on its Metal seats.**
  Real-time factor over the corpus: `pocket-tts-en-18m-q8.gguf` 0.021, `pocket-tts-en-18m-kq.gguf` 0.021,
  `pocket-tts-en-18m-refl-q8.gguf` 0.023, `pocket-tts-en-7m-q8.gguf` 0.019, `pocket-tts-en-2m.gguf` 0.009, every
  GPU seat then declining their shapes (the seats took them later in this leg - the fifth item); the 100M `pocket-tts-en-kq.gguf` 0.027 on the CPU
  tower alone (`DASLLAMA_METAL_TOWER=0`) and 0.009 with its three Metal seats serving. The stanza's stages, ms: the
  18m backbone 37, head 8, codec 91; the 7m 33, 8, 100; the 2m 17, 6, 38; the 100M on the CPU tower 61, 15, 98 and
  on Metal 50, 0, 34. So CPU for CPU the 18m is a third faster than the 100M, not the tenth its parameter count
  says: its backbone step costs 60% of the 100M's for a fifth of the weights, and its codec decoder - a 128-wide
  12-layer transformer at 200 Hz over 24- to 256-channel convs - costs as much as the 100M's 512-wide 2-layer one
  with its 64- to 512-channel convs, so the rows kernels at these widths are overhead-bound (the per-row GEMM tile
  and the per-call setup, not the FLOPs). The codec is two thirds of a small model's wall; the 100M's codec seat
  does the same speech in 34 ms on the device. The next factor for the browser build sits in the CPU kernels at
  narrow widths.
- **The Metal codec seat over the small decoder** (the attention row's head size an argument, `GkPkAttnArgs.hs`;
  `ARCHITECTURE_GPU_TOWER_POCKET.md#tower-pocket-codec`): the stanza's codec 91 -> 21.7 ms on the 18m, 100 -> 20.8
  on the 7m (`harness/pocket_pace.das` at one build, the seat off under `DASLLAMA_METAL_TOWER=0` against on); over the corpus `pocket-tts-en-18m-q8.gguf` 0.021 -> 0.011, `pocket-tts-en-18m-kq.gguf` 0.021 -> 0.011,
  `pocket-tts-en-18m-refl-q8.gguf` 0.023 -> 0.010, `pocket-tts-en-7m-q8.gguf` 0.019 -> 0.008 - level with the 100M
  model's 0.009 on its three seats, the backbone still on the CPU chain (the frames and prompt seats decline the
  grouped-query shapes - then; the fifth item). The seat reads 1.3e-6 of the CPU chain on the f32
  references (GPU-vs-CPU: `tests/run.das -- --area tts`, the `test_pocket_small_models` cell's codec-seat leg); the rig's UTMOS moved 4.341 -> 4.345 (18m-q8) and 4.232 -> 4.238
  (18m-kq) with the codec's activations exact instead of the CPU's Q8_0 blocks. The 2m's 12-channel residual
  stage sits off the gathering stamps' 8-run lattice, so its codec stays on the CPU at 0.009.
- **The browser** (the parrot page's measure button - `examples/dasLLAMA/parrot`'s own rail - on the wasm64 release under
  Chrome on the same box, `web/output64/examples/parrot`, 7 jobque workers, the portable kernels, the smoke rail's stanza
  cloned from the reference clip; two runs each within 0.2 %):
  `pocket-tts-en-18m-kq.gguf` 8.4 s of speech in 667 ms, RTF 0.079 (prompt 28, backbone 104, head 13, codec 522);
  `pocket-tts-en-kq.gguf` 10.3 s in 1657 ms, RTF 0.161 (prompt 88, backbone 366, head 48, codec 1154); the voice
  clone of the 6.5 s clip 470 ms against 866. Per second of speech the 18m costs 79 ms against the 100M's 161 - half
  - with the codec three quarters of its wall (62 against 112 ms a second) and the backbone a third of the 100M's.
  Both run many times faster than real time in the browser (12.6x and 6.2x); the 2x is the small model's whole
  browser gain today, and the codec at 128 wide is where the next one sits.
- **The AOT host under node** (`web/output64/dasllama_aot.wasm`, the same portable chain without a browser,
  `examples/dasLLAMA/wasm/run_node.js` driving `examples/dasLLAMA/speak.das`, one 4.8 s / 5.4 s sentence at `alba`,
  two runs each within 1 %): 18m-kq 4.8 s in 407 ms, RTF 0.085; 100M kq 5.4 s in 1428 ms, RTF 0.26 - the browser's
  ratio, with the 100M paying more here than under Chrome's 7 workers. The same 18m sentence native (`-jit`, the
  Metal codec seat) 105-128 ms, RTF 0.022-0.027.
- **The small models on the GPU seats, both homes (the seats took the grouped-query backbones, the bottleneck row and the
  time-free tail in this leg).** The served bench's protocol (`dasllama-server --tts <file> --gpu metal|vulkan`,
  `harness/served_bench.das --no-chat --tts-url --tts-voice alba --reps 8`, the box idle, the box profile re-minted that
  hour; `out-of-process` - the bench client's clock; a row voided on a spread past 3% over its 8 reps is marked, its
  median kept for the eye, every unmarked row's spread under 3%). M5 Max, Metal, ms a request / s of
  speech / RTF, two lives:
  | file | one sentence | two sentences |
  |---|---|---|
  | pocket-tts-en-q8 | 50.3 / 4.56 / 0.011 and 49.6 | 91.4 / 8.80 / 0.010 and 92.6 |
  | pocket-tts-en-kq | 46.3 (void 3.4%) and 45.1 / 4.64 / 0.010 | - |
  | pocket-tts-en-18m-q8 | 29.5 / 4.56 / 0.006 and 30.0 (void 6.9%) | 51.9 / 8.72 / 0.006 and 51.3 |
  | pocket-tts-en-18m-kq | 27.9 and 27.0 / 4.40 / 0.006 (both void, 4%) | 49.8 / 8.72 / 0.006 and 49.6 |
  | pocket-tts-en-7m-q8 | 27.0 (void 4.5%) and 28.1 / 5.04 / 0.006 | 47.6 / 9.44 / 0.005 and 48.6 (void 3.3%) |
  | pocket-tts-en-2m | 25.0 (void 3.3%) and 25.0 (void 5.2%) / 4.72 / 0.005 | 43.3 / 9.20 / 0.005 and 43.4 |
  A one-sentence request on a small file is 27-30 ms, where the request path's own jitter is the spread; the two-sentence
  rows clear the rule. The 18m costs 0.57 of the 100M a request on Metal, the 2m 0.47 - and 18m, 7m and 2m sit within 20% of
  one another: the frame loop is dispatch-bound below the 18m (row 237). The reference build on the same box and sentence
  (`external`: `python3 modules/dasLLAMA/harness/pocket_ref_time.py english_2026-04,english_18m_2026-10,english_7m_2026-10,english_2m_2026-10 1 cpu`
  under the pocket-tts 4.0.1 venv, torch 2.14.1, one CPU thread - the package's own pin; it does not run on MPS, a Metal
  command-encoder assertion in torch; 8 reps, the median): 100M 325 / 4.72 / 0.069, 18m 156 / 4.36 / 0.036,
  7m 162 / 4.92 / 0.033, 2m 121 / 4.76 / 0.025 (cv 2-7%). Ours on Metal is 6.5x the reference build on the 100M and 5.3x
  on the 18m.
  RTX PRO 4500 (the pod, driver 580.173.02, no CPU profile - every CPU-side kernel on its reference body, so the Vulkan rows
  are the seats' and nothing else is read there), master c247033f47 (the dasCUDA merge) against the branch at 5ff376734e
  (its measured tip; the folds after it rewrote the history), `direction-grade`, the bench's one-sentence rows
  (`out-of-process`, the same server flags and bench command as the Metal rows with `--gpu vulkan`): 100M q8 master 59.9
  and 61.3 ms / 4.64 s / 0.013 against the branch's 60.8 and 61.4 - no regression; the branch
  alone 18m-q8 26.8 / 4.32 / 0.006 (its twin 26.2 void 9.3%). The bench's two-sentence rows on the small files read
  20-30% spreads on this box while eight `curl` requests to the same server, timed by `curl` itself, sit within 2% (100M
  115 ms, 18m 49-58, 7m 48-50 with one 91, 2m 45-48; the server's own stage clock a chunk: 100M 50.5 and 57.0 ms, 18m
  21.5 and 21.7, 7m 20.4 and 22.9, 2m 19.2 and 19.3 - prompt 1-4, backbone 15-17 on the small files, codec 3-5) - the
  spread is the bench client's on that box, not the engine's, and the clean rows there are 18m-q8 50.1 / 8.32 / 0.006 and
  49.5, 18m-kq 50.1 / 8.72 / 0.006 and 50.4. The engine's clock reads the 18m, 7m and 2m within 15% of one another and at
  0.4 of the 100M a chunk on Vulkan, the Metal picture again (row 237). The reference build on the same card (`external`:
  `harness/pocket_ref_time.py <the four configs> 0 cuda` under pocket-tts 4.0.1, torch 2.11.0+cu128, one sentence, 8 reps,
  the median): 100M 739 ms / 4.96 s / 0.149, 18m 1044 / 4.56 / 0.229, 7m 1149 / 4.80 / 0.239, 2m 885 / 4.76 / 0.186 -
  launch-bound at batch one, the small models slower than the 100M; on the pod's CPU (`... 1 cpu`: the package's one thread,
  an EPYC 7543P core): 100M 1463 / 0.307, 18m 913 / 0.204, 7m 551 / 0.113, 2m 592 / 0.124. Our Vulkan 18m at
  21.6 ms a chunk is 48x the reference build's CUDA figure and 42x its CPU one; the 100M 12x and 25x.
- **The small files' GPU seats held to the CPU chain** (GPU-vs-CPU on the f32 references: `tests/run.das -- --area tts`
  on this box, the `test_pocket_small_models` cell of `tests/test_tts_pocket.das`; the first compares to hold these files):
  the codec seat within `GPU_CODEC_BAR` (1e-5) over every oracle case's latents, reading 2.3e-6; the frames seat
  teacher-forced within `GPU_FRAME_BAR` (2e-5) over the latents, conditioning and EOS rows, 2.3e-6; the prompt seat's K/V
  rows within `GPU_PROMPT_BAR` (1e-5), 1.1e-6; the grouped-keys attention kernel cell (`test_vkt_pk_attn_grouped`, RTX
  PRO 4500 under `DASLLAMA_GPU=1`) 3.6e-7. The same cell's external-reference compares against the oracle's dumps
  (`harness/pocket_oracle.py` under pocket-tts 4.0.1): the reference build's precomputed voice states match the clip path
  within 1e-4 on the 2026-10 files and sit 1.5e-2 off on the 2026-04 file, another checkpoint revision.
- **Memory.** The lasting allocations the small backbones add, a model loaded: the `z_proj` bottleneck's f32 rows,
  d x 64 x 4 bytes - 131072 on the 18m (d 512), 28672 on the 2m (d 112) - and its slot in each GPU home's frames slab
  (the 32-padded d by 64 rows); the text table a file stores as low-rank factors, multiplied out at load to f32
  [4001][d] - 8.2 MB on the 18m, 1.8 MB on the 2m (the 100M stores the full table and allocates as before). The q8
  blob's rows padded to 64 reach the published 100M q8 file through its 32-row head `final_linear` - 32 more rows of
  1024 Q8_0 bytes plus their scales, 33 KiB - and the first compares of this leg (the item above) are the only shapes
  the other size formulas change on; the peak footprint of one load of `pocket-tts-en-q8.gguf` (`-jit`, warm caches,
  the Metal tower off, `/usr/bin/time -l`, M5 Max) reads 1.76 GB at master c247033f47 and 1.75 GB on the branch, the
  load 3.2 s and 3.3 s. One flag-free synthesis of the served bench's sentence at `alba` hashes to the same PCM bits on
  both commits for `pocket-tts-en-q8.gguf` (the Metal seats on, and the CPU chain) and `pocket-tts-en-kq.gguf` (the
  seats on).
- **The browser examples keep the 100M model.** The parrot's measure button on the served page (Chrome, this box, no
  other load): 18m-kq 12x real time, 100M kq 6x. Listening to the two, the 18m reads rushed - the pace difference the
  rig's durations show - and the 100M's speech is preferred; the parrot stays on `pocket-tts-en-kq.gguf` (the page's
  `?tts=<file>` picks another file from its manifest), storywish on `pocket-tts-en-stuart-kq.gguf`. The 18m twins stay
  published for the asking: `pocket-tts-en-18m-kq.gguf` and the one-voice `pocket-tts-en-18m-stuart-kq.gguf` (13 MB
  against 67; its `stuart_bell` samples are the 18m-kq's, bit for bit).
- **Quality, the rig at `alba` over 200 sentences** (`harness/tts_rig.py`, WER / UTMOS): 18m-q8 3.09 / 4.341,
  18m-refl-q8 3.04 / 4.369, 18m-kq 3.18 / 4.232, 7m-q8 3.41 / 4.357, 7m-refl-q8 3.86 / 4.377, 2m 5.32 / 3.777,
  2m-refl 4.68 / 3.823; the reference build (`external`: `harness/pocket_synth.py` under pocket-tts 4.0.1, one torch
  thread, the rig's sentences, scored by `harness/tts_rig.py`) 18m 5.41 / 4.364, 7m 5.59 / 4.378, 2m 9.18 / 3.817 - the WER gap is the English normalizer in front of
  our files (numeric 1.49 against 16.67), the UTMOS gap the served q8 lane's (0.02-0.04). The 100M
  `pocket-tts-en-kq.gguf` under the 4.0.1 text rules reads 3.86 / 4.319, where its 3.1.0-rules row read 4.09 / 4.333.

### From the deltanet leg of the dasCUDA arc (2026-10-25, the reference box: Threadripper 3990X, RTX 5060 Ti 16 GB, driver 610.74, Windows, the GPU clock locked at 2392 MHz; `direction-grade` - every row compares separate processes or builds; flags `DASLLAMA_GPU=1 DASLLAMA_CUDA=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1`, `DASLLAMA_CUDA_MMQ`, `DASLLAMA_CUDA_PDL` and `DAS_TUNE_POLICY` unset unless a row names them, `-jit`)

The bench invocation and the llama.cpp reference are the dasCUDA arc entry's above (`debug-jit`, untuned, `-p 512 -n 128
-r 3 -t 16 --ngl 0`; llama.cpp b10660 CUDA `-ngl 99 -fa 1`); the profile is `nsys --cuda-graph-trace=node` under
`DASLLAMA_CUDA_PDL=0` (the dasCUDA arc's instrument note: a kernel's span under programmatic launches counts its wait) over
`-p 512 -n 64 -r 1`, each figure the mean of a kernel's launches at one grid, in us; pp512 / tg128 in tok/s.

- **Qwen3.5-0.8B Q8_0, the deltanet hybrid, ours CUDA / ours Vulkan / llama.cpp CUDA.** 18569 +/- 409 / 339.8 +/- 9.4
  (the window attention on the gated h256 flash tile; 14856 +/- 223 / 351.2 +/- 0.5 on the chunked tiles before it - the
  token step is the same; one run between read 339.8 +/- 9.4, the run-to-run spread), 15028 +/- 14 / 301.5 +/- 3.0 (llama.cpp `external`:
  14312 +/- 1342 / 283.6 +/- 3.8). Decode 1.13x Vulkan and 1.20x llama.cpp, prefill 1.24x Vulkan and 1.30x llama.cpp. The
  flash-tile figure is 84613dac1f's build against 10751c4c10's (the chunked tiles), one process each under the bench protocol.
- **Qwen3.5-4B Q8_0, ours CUDA / ours Vulkan / llama.cpp CUDA.** 5019 +/- 32 / 78.8 +/- 1.9, 4096 +/- 15 / 76.7 +/- 0.2 (the
  Vulkan plan trimmed the mirror to 197571 of the 262144 positions asked), llama.cpp `external`: 4316 +/- 210 / 77.6 +/-
  0.2. Decode at parity (1.02x llama.cpp, 1.03x Vulkan), prefill 1.16x llama.cpp and 1.23x Vulkan.
- **Qwen3.8-27B, the 64-layer deltanet hybrid, on the 16 GB card** (the mirror pinned at 4096 positions through
  `DASLLAMA_GPU_CTX_MAX`; the NextN head stays off the device on every row, the trunk is the row on ours and on llama-bench
  alike). UD-Q3_K_XL (13.15 GB): ours CUDA 880.3 +/- 1.2 / 27.45 +/- 0.00, llama.cpp `external` 852.1 +/- 15.6 / 28.01 +/-
  0.04 - prefill 1.03x, decode 0.98x; the Vulkan plan declines the same file (weights 12634 MB and the window scratch past
  its 13257 MB usable), its per-op tier 108.5 / 15.75 (one rep) with 384 stacks resident. UD-IQ4_XS (14.25 GB: weights 13235 MB and
  the 2048-row window's scratch 731 past the 13636 MB usable) and i1-IQ3_S (12.6 GB on disk, 13457 MB served - the IQ3_S
  planes' served form outgrows the file) decline both homes at every context, the per-op rails serving (llama.cpp
  `external` 871.7 +/- 15.4 / 26.71 +/- 0.04 and 837.0 +/- 14.7 / 27.90 +/- 0.02 on them).
- **Qwen3.8-27B on the pod** (RTX PRO 4500 Blackwell 32 GB, Linux, llama.cpp b10660; the clock is not lockable in the
  container; the mirror pinned at 8192 positions). Q4_K_M (19.0 GB): ours CUDA 1980.1 +/- 3.7 / 38.66 +/- 0.01, ours Vulkan
  1781.5 +/- 3.6 / 38.79 +/- 0.01 (the pod's driver without `VK_NV_cooperative_matrix_decode_vector`, the cm2 tiles on
  the scalar decode arm), llama.cpp `external` 1846.0 +/- 55.9 / 38.97 +/- 0.05 - prefill 1.07x llama.cpp and 1.11x
  Vulkan, decode at parity (0.99x both). NVFP4-MTP iMatrix (17.1 GB, the trunk): ours CUDA 2299.6 +/- 1.9 / 43.07 +/-
  0.01, ours Vulkan 1647.8 +/- 2.5 / 42.90 +/- 0.01, llama.cpp `external` 2569.7 +/- 114.5 / 44.21 +/- 0.06 - prefill
  0.89x llama.cpp (1.40x Vulkan), decode 0.97x. The two files share the trunk's deltanet kernels, and the Q4_K_M window
  leads llama.cpp where the NVFP4 window trails it (the 8B NVFP4 window above led it 1.17x), so the shortfall sits in the
  NVFP4 window's GEMM tiles at the 27B's shape against llama.cpp's W4A4 path (the next entry's profile).
- **The 27B NVFP4 window on the pod, kernel by kernel** (nsys 2026.3.2 installed on the pod, `--cuda-graph-trace=node`,
  `DASLLAMA_CUDA_PDL=0`, p512 n64 r1, both runs the same pass count; every llama.cpp figure in this entry and the two
  after it is `external`, its binary under the same tracer: llama.cpp under the tracer reads 2238 pp512 against its
  untraced 2570 - the per-launch cost lands on its many small kernels - so the per-kernel times are the comparison, not
  the traced rates). The file mixes formats a layer: NVFP4 on 57 of 64 FFNs, 34 of 48 deltanet qkv/z/out triples and
  every beta/alpha plane (d 48), Q5_K on the rest and on every attention projection, Q6_K on 8 downs. Window time per
  group, ours against llama.cpp: the NVFP4 GEMMs on the FP4 leaf 147.5 ms against 136.9 (the 544-tile gate/up 380 us a
  call against 325, the down 332 against 288, the 320-tile qkv 212 against 185, the 192-tile z 158 against their 82-block
  stream-k 116: the tile at k 128 a step on a two-deep ring against their k 512 and stream-k, 1.15x to 1.36x a call);
  NVFP4 planes the leaf did not take 53.2 ms against 0 - the beta/alpha rows at d 48 on the cm2 s tile, 16 workgroups a
  512-row window each walking all of k, 180 us a plane x 192 calls = 34.7 ms where llama.cpp's stream-k reads 11 us, and
  the deltanet out site excluded from the leaf by name, 273 us a call on the cm2 l tile against their 111; the Q5_K planes
  on the f16 cm2 tiles 101.2 ms against their int8 MMQ 80.4 (the Q6_K downs 13.2 against their 15.1 - ours ahead); the
  deltanet scan and conv 64.5 against their 74.5. Two levers landed (2bfa6ffb34, the rows after it against 5cdf6dcdc7's build): the s tile's grid splits k like the l and
  m grids (the beta/alpha planes 38 us a call on 112 workgroups, 7.3 ms), and the recurrent out site feeds the FP4 leaf
  through `fp4_rows` over the scan's o rows (118 us a call). The rows after, same protocol: NVFP4 trunk ours CUDA 2437.6
  +/- 3.7 / 43.02 +/- 0.02 (from 2299.6 / 43.07) - prefill 0.95x llama.cpp (from 0.89x), decode 0.97x; Q4_K_M 2013.3
  +/- 3.7 / 38.66 +/- 0.01 (from 1980.1) - prefill 1.09x. The 16 GB card's UD-Q3_K_XL row 885.8 +/- 2.0 / 27.06 +/- 0.02
  (from 880.3 / 27.45). Owed from the same trace: a Q5_K int8 MMQ leaf (24 ms of the 375 ms window at the 27B shape) and
  the FP4 tile's k step and ring depth (13 ms).
- **The token step kernel by kernel, ours CUDA against llama.cpp CUDA.** The classifier GEMV 660 against 673 (23% of the
  ~2.85 ms step, the 254 MB table read on both); gate+up fused 21.6 against 22.5; the deltanet qkv GEMV (6144 rows) 17.4
  against 20.0; the z GEMV 6.6 against 7.4; the fused deltanet step 9.7 against their chain of gated_delta_net 2.7, ssm_conv
  2.8, two l2_norm 1.5, softplus 1.6, the sigmoid gate, the copies and get_rows (13 to 15); the wo and down epilogue GEMVs
  11.4 against 12.7; the gated q GEMV (4096 rows) 12.0 against 13.7; k and v 2.7 against 3.0; beta and alpha 1.4 against
  1.9; the residual step storing the normed row 3.0 against rms_norm 2.0 + quantize_q8_1 1.8. The one kernel behind: the
  decode attention at head 256, `CuDaAttnF16` 11.6 against `flash_attn_ext_vec<256>` 7.4 (1.57x; six layers, under 1% of
  the step).
- **The window at 512 rows, a recurrent layer.** The scan 457 (+ the conv 87, the gated out-norm 13, the ring tail) against
  llama.cpp's gated_delta_net window launches at ~617; the gated h256 flash tile 164 a layer-window; the gate/up MMQ 68
  against their mul_mat_q 65.
- **The token step, ours CUDA against ours Vulkan** (the Vulkan home's own role profile, `DASLLAMA_GPU_PROF=1`, its
  per-token sums over 64 tokens divided by the layer count; 2945 us a token on Vulkan): an attention layer's q GEMV 12.0
  against 16.5, the fused qk-norm rope 1.9 against 3.7, the decode attention 11.6 against 17.7, the wo epilogue 11.4
  against 11.7, gate+up 21.6 against 22.8; a recurrent layer's qkv GEMV 17.4 against 22.4, the beta and alpha GEMVs 2.8
  against 4.1, the fused step 9.7 against 11.4, the out GEMV 9.1 against 9.7, the residual step 3.0 against 3.7, gate+up
  21.6 against 22.7, down 9.1 against 9.1; the classifier 660 against 647. The Vulkan z GEMV reads 3.3 because it co-runs
  with the qkv GEMV under its own hazard class, where the CUDA graph serializes the two (6.6): the merged qkv+z GEMV
  (`CudLayer.dnqkvz_merged`, one dispatch of `cd / di + 1` regions of `di` rows) is that gap's lever - 18893 +/- 34 /
  352.9 +/- 0.8 with it, the token step one launch shorter a recurrent layer (5cdf6dcdc7's build against 84613dac1f's, one
  process each under the bench protocol).

### From the routed block on the CUDA home (2026-10-26, the reference box: Threadripper 3990X, RTX 5060 Ti 16 GB, driver 610.74, Windows, the GPU clock locked at 2392 MHz; `direction-grade` - every row compares separate processes or builds; flags `DASLLAMA_GPU=1 DASLLAMA_CUDA=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1`, `DASLLAMA_CUDA_MMQ`, `DASLLAMA_CUDA_PDL` and `DAS_TUNE_POLICY` unset unless a row names them, `-jit`)

The bench invocation and the llama.cpp reference are the dasCUDA arc entry's (`debug-jit`, untuned, `-p 512 -n 128 -r 3
-t 16 --ngl 0` under `DASLLAMA_GPU=1 [DASLLAMA_CUDA=1]`; llama.cpp b10660 CUDA `-ngl 99 -fa 1 -r 3`); pp512 / tg128 in
tok/s, ours CUDA / ours Vulkan / llama.cpp CUDA. The routed block is the Vulkan home's on the discrete templates
(`ARCHITECTURE_GPU_SHARED_KERNELS.md#moe-routing-kernels`): the window's router GEMM, select, bucket schedule, f16 gather,
the expert pieces on the cm2 s and e columns and the combine-folded residual step; the token step's router GEMV, top-k
records as the expert GEMVs' regions, the act and the combine.

- **Qwen1.5-MoE-A2.7B-Chat Q4_K_M (60 experts, 4 routed, a gated shared expert, K-quant stacks).** 5476.0 +/- 8.6 /
  196.2 +/- 0.9, 5271.3 +/- 15.0 / 173.2 +/- 0.3, llama.cpp `external` 4830.3 +/- 211.5 / 184.4 +/- 1.0 - prefill 1.13x
  llama.cpp and 1.04x Vulkan, decode 1.06x llama.cpp and 1.13x Vulkan.
- **Qwen3-30B-A3B-Instruct UD-IQ2_XXS (128 experts, 8 routed, no shared expert; the iq2xxs stacks on their own 32-step e
  stamps).** 3230.2 +/- 10.4 / 171.9 +/- 0.2, 3302.3 +/- 48.1 / 130.5 +/- 0.3, llama.cpp `external` 2848.8 +/- 139.7 /
  130.0 +/- 0.5 - prefill 1.13x llama.cpp and 0.98x Vulkan, decode 1.32x both.
- **gpt-oss-20b MXFP4 (32 experts, 4 routed, the native-MXFP4 stacks on `CuMx4Gemv` and the `CuMx4Cm2S` / `CuMx4Cm2E`
  tiles, every bias, the clamped swiglu, the attention sinks on the sink stamps, the sliding layers on flat slabs).**
  Measured after a 150 s idle, the three engines back to back and the CUDA arm repeated last (the card drifts a tenth and
  more under sustained runs with the clock locked - a reading taken right after a regression run sat at 2645 / 102.5, one rep):
  3473.4 +/- 18.6 / 133.8 +/- 0.4 (the repeat 3351.2 +/- 124.6 / 133.2 +/- 0.2), 3571.2 +/- 7.5 / 124.7 +/- 0.0, llama.cpp
  `external` 4945.1 +/- 200.0 / 125.1 +/- 1.8 - decode 1.07x both, prefill 0.97x Vulkan and 0.70x llama.cpp. The token step
  is at parity kernel by kernel (`nsys --cuda-graph-trace=node`, PDL off, the llama.cpp figures `external` under the same
  tracer: the expert GEMVs 132 us a layer against their
  141, the fused qkv 40.7 against 49.4, the sink attention 6.3 against 8.5, the classifier 1488 against 1503); the window
  is 99% busy and its loss is the expert tiles - the f16 cm2 e and s stamps over the mx4 planes 99 ms of the 146 ms window
  (the e stamp 932 us a call at 1104 workgroups, the s stamp 444 us at 736 on the 21 small buckets of 32 experts) against
  llama.cpp's `mul_mat_q<MXFP4>` on the native FP4 rung 56 ms of its 111 (`external`, the same tracer; the lever is the next entry: an mx4
  leaf fed from the expert schedule).
- **gpt-oss-20b MXFP4, the expert pieces fed from the expert schedule (`SchedArgs.row_pad` / `m_only`: every bucket from a
  128-row multiple, all on the m column), two leaves measured against the cm2 tiles under the cool protocol (150 s idle, the
  arms back to back, the first repeated last; same box, same clock).** The int8 leaf first (`MmqT` under a record arm, the
  gathered rows requantized to Q8_0): pp512 3270.1 +/- 20.6 against the cm2 tiles' 3486.7 +/- 4.2 - behind, though the tile
  ran at 55 TFLOP/s raw (1.58 ms a call at 1104 workgroups): a 512-row window's 32 buckets hold about 64 rows each, so
  every 128-row tile computes half pad, and the int8 rate does not pay for it; its first wiring sized the feed and act
  kernels by the residency's widest window (20448 rows) and read 2686 (one rep) before the per-window sizing. llama.cpp pays the same
  pad: its `mul_mat_q<MXFP4>` picks the 128-row tile too (`J_best` over 512 columns, `external`) and runs it on the native FP4 rung
  (`kind::mxf4`, 781 us a call). So the leaf moved to the FP4 rung (`CuMx4Fp4MmqMoe` over `MmqFp4T`'s `MX` / `KTAIL` /
  `MOE` arms, the mx4 feed kernels, `mma_block_scaled_mx`): pp512 4948.1 +/- 16.5 / tg128 135.8 +/- 0.4, the cm2 tiles
  3496.0 +/- 14.1 / 135.9 +/- 0.1, the FP4 arm repeated 4948.3 +/- 14.5 / 136.3 +/- 0.1 - against the cold references
  above, prefill 1.00x llama.cpp (4945.1) and 1.39x Vulkan (3571.2), decode 1.09x both. The 4-bit activation widens the
  gpt-oss parity cells' logit distance (the 40-token window 0.89 of the 1.58 bar, the steps 0.6, against 0.11 and 0.12 on
  the Q8_0 feed); the Q8_0-fed leaf, correct and slower, is not kept. The int8 figure is an uncommitted build between
  aeb9f401b0 and 0285da5862 (the leaf is not kept); the FP4 and cm2 figures are 0285da5862's build under
  `DASLLAMA_CUDA_MMQ=1` and `DASLLAMA_CUDA_MMQ=0`, one process each under the bench protocol. The parity readings are
  `test_gpu_resident_cuda_gptoss`'s cell of `test_gpu_resident_cuda_moe.das` through `tests/run.das`.

### From the host experts on the CUDA home (2026-10-27, the reference box: Threadripper 3990X, RTX 5060 Ti 16 GB, driver 610.74, Windows, the GPU clock locked at 2392 MHz; `direction-grade` - every row compares separate processes. Every rate of ours is `benchmarks/lcpp_bench.das`'s unless its entry names another instrument: `DASLLAMA_GPU=1 DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1 DASLLAMA_GPU_CTX_MAX=32768 DASLLAMA_GPU_CTX_STRICT=1 [DASLLAMA_CUDA=1] bin/daslang -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m <gguf> -p 512 -n 128 -r 3 -t 16 --ngl 0 --for-debug-purposes`, `DASLLAMA_CUDA=1` on the CUDA rows alone; `DAS_TUNE_POLICY`, `DASLLAMA_COOPMAT` (auto: cm2 on both cards), `DASLLAMA_CUDA_MMQ` (1), `DASLLAMA_CUDA_PDL` (1) and `DASLLAMA_CUDA_CHAIN` (0) unset unless an entry names a value, the CPU kernels the shipped x86-avx2 class profile stamps under `DASLLAMA_ALLOW_UNTUNED=1`; a figure's `+/-` is the standard deviation over its reps as the bench prints it; an entry names the commit whose tree built the exe. llama.cpp's rows are `external`: its b10660 `-DGGML_CUDA=ON` build (`ARCHITECTURE_MEASUREMENT_REFERENCE_ROWS.md#cuda-reference-rows`), `llama-bench -m <gguf> -ngl 99 -fa 1 -t 16 -p 512 -n 128 -r 3`, `-ncmoe <n>` where the entry says so, no environment setting)

- **MEASURED (2026-10-27, `debug-jit`, `direction-grade`; `lcpp_bench` at `-p 0 -n 128 -r 3` under the heading's flags, one
  process a `DASLLAMA_GPU_PF_STREAM` / `DASLLAMA_RARE_ARENA` value, the tree of 7c9a1f4a01) - the rare rail on an untrimmed
  load costs the decode its host step's margin: Qwen3.6-35B-A3B-MTP UD-Q4_K_M at the strict 32k (22 of 41 routed layers'
  experts on the host on CUDA, 24 on Vulkan), decode only.** CUDA tg128 60.06 +/- 0.37 with the prompt stream on against 64.52 +/- 0.31
  with it off (`DASLLAMA_GPU_PF_STREAM=0`) and 64.73 +/- 0.19 on the pre-stream build; Vulkan 51.92 +/- 0.76 against
  56.84 +/- 0.16. Not the pinned memory: the mirror in ordinary memory 59.54 +/- 0.49, the mirror allocated and never
  filled 60.10 +/- 0.53, one layer streamed 64.14 +/- 0.36; threads and page faults the same in both arms. The host step
  (`rdec_host_experts_step`, 8624 calls) summed 3058 ms with the stream on against 2633 off - 354 against 305 us a
  layer-token, the whole 1.2 ms a token the bench lost - because `rare_dev_layer` saw every streamed stack's pinned view and
  the token's picks took the rail's row-major k4/k5 bodies in place of the tuned planar kernels, with both copies in RAM;
  `DASLLAMA_RARE_ARENA=0` read 64.68 +/- 0.03. The gate is `rare_arena_serves`: the rail where the load kept no planar
  planes (the trimmed lane, where it is the only path), the planar kernels elsewhere. The rail's own kernel gap stands on
  the trimmed Vulkan lane, the served shape there.
- **MEASURED (2026-10-27, `debug-jit`, `direction-grade`; `lcpp_bench` as the heading, the tree of 27b82886fb) - the
  host-experts rows under the served shape (the strict 32k pin, pp512 / tg128, 3 reps, 16 threads), the rare rail gated;
  llama.cpp's rows `external` - the b10660 `-DGGML_CUDA=ON` build, `llama-bench -m <gguf> -ngl 99 -fa 1 -ncmoe <our CUDA
  plan's host layers> -t 16 -p 512 -n 128 -r 3`, no environment setting (`ARCHITECTURE_MEASUREMENT.md#host-experts-reference`).**
  Qwen3.6-35B-A3B-MTP UD-Q4_K_M: ours CUDA (22 of 41 routed layers' experts on the host) pp512 1210.7 +/- 13.5, tg128 64.59
  +/- 0.23; ours Vulkan (24) 1120.5 +/- 31.2 / 55.76 +/- 0.24; llama.cpp CUDA (`external`) at `-ncmoe 22` 511.7 +/- 49.2 / 56.84 +/- 0.60 -
  the CUDA home 2.37x the reference's prefill and 1.14x its decode (the pre-stream build read 722.8 / 63.88). gemma-4-26B-A4B
  UD-IQ3_XXS: ours CUDA (6 of 30) 1663.0 +/- 13.3 / 83.02 +/- 0.59; ours Vulkan (8) 1637.8 +/- 55.1 / 69.13 +/- 0.52; llama.cpp
  (`external`) at `-ncmoe 6` 1501.9 +/- 46.5 / 81.44 +/- 0.48 (1.11x / 1.02x). gemma-4-26B-A4B Q4_K_M: ours CUDA (15 of 30) 1256.5 +/- 25.7 /
  55.27 +/- 0.19; ours Vulkan (16) 1298.5 +/- 35.9 / 49.60 +/- 0.20; llama.cpp (`external`) at `-ncmoe 15` 753.6 +/- 30.9 / 50.33 +/- 0.41
  (1.67x / 1.10x). On both gemma quants the streamed slot's carve (one layer's planes: 325 MB on the IQ3_XXS, 484 on the Q4_K_M,
  plus the chain's scratch) moved two more routed layers to the host than the pre-stream plan held there (4 -> 6, 13 -> 15): the
  IQ3_XXS decode 85.9 -> 83.0 for a prefill 976.5 -> 1663.0, the Q4_K_M 58.45 -> 55.27 for 768.9 -> 1256.5. `DASLLAMA_EXPERT_RING`
  does not shrink the CUDA chain's slot (64 read the same plan and 1261.1 / 55.27; 32 the same plan and 1153.2 / 50.86 +/- 1.62).
- **MEASURED (2026-10-27, `debug-jit`, `direction-grade`; the tree of 876819d7e6) - the row-major K-quant kernels' pair form (`rm_pair_codes`,
  `rm_sb_min_vec`, `rm_pair_k5_qh`: two blocks a weight load, the strips' i16 pair sums under one scaled pmaddwd, the min term one
  vector a superblock), `kq_kernel_bench.das --rm`, one thread, the stamped x86-avx2 perms (`dot_maddubs_width256_mr8`), best of
  9.** At n=2048 d=768 (the 35B's gate / up): k4 row-major 80 -> 48 us (11.4 -> 18.9 GB/s), k5 100 -> 64 (11.1 -> 17.3); at
  n=768 d=2048 (its down): k4 69 -> 54, k5 105 -> 72; the planar kernels read 25-38 / 43-48 (k4 / k5) across the same runs. The
  research behind it: llama.cpp's AVX2 Q4_K x Q8_K dot applies the sub-scales as an integer pmaddwd multiplier and the mins as
  one vector multiply a superblock (`ggml-cpu/arch/x86/quants.c`), and our device plane's k / k+16 pairing lets one 32-byte
  load serve two blocks where ggml's k / k+32 cannot; the audit of our emitter counted ~165 ops a row-superblock against the
  planar's ~52, 51 of them loads (the scalar min term re-read the 16 strip sums and 8 min bytes a row). The token's host step
  does not follow the per-core gain: the 35B decode through the rail (`lcpp_bench`, the heading's command at `-p 0`,
  `DASLLAMA_RARE_ARENA=2`, 16 lanes, 22 / 24 host layers)
  reads Vulkan 53.92 +/- 0.09 against the old bodies' 52.68 +/- 0.71 and CUDA 59.27 +/- 1.04 against 61.73 +/- 0.26 - at
  sixteen lanes the step streams 23 MB a layer-token at 65-75 GB/s, the box's DRAM ceiling, and the team bench (one expert
  plane over 16 lanes) reads the rail and the planar kernels alike at 45-51 us. The rail's 15% against the planar path on
  the token (354 against 305 us a layer-token) is its per-layer structure - the per-pick fetch and heat calls, the three
  forks' split - not the kernel bodies; the per-core gain stands where fewer lanes serve the step. The generator probe's TEST
  mode does not reach the row-major families; `tests/test_vulkan_host_twin.das` (58 cells, both homes) and the formats witness
  are their gate. The probe's TEST mode fails `k49 dot_maddubs_width256_mr8` (NVFP4's CPU tile, maxdiff 32435.918) on the
  committed tree before this change - a standing red of its own.
- **MEASURED (2026-10-28, `debug-jit`, `direction-grade`; `lcpp_bench` as the heading, the tree of 68b46b36e0) - the 35B on
  the pod's plan: RunPod RTX PRO 4500 Blackwell 32 GB
  (pod eh6no2arx003v4, EPYC 7663, driver on boost - the container refuses the clock lock), the house file
  Qwen3.6-35B-A3B-MTP IQ4_XS-pure (the pod carries no UD-Q4_K_M), the served shape (strict 32k), 16 threads, pp512 / tg128,
  3 reps.** The card holds the model whole (17976 MB of planes on device, no routed layer on the host): ours CUDA pp512
  5320.4 +/- 33.1, tg128 233.56 +/- 0.91; ours Vulkan 5232.0 +/- 30.4 / 192.05 +/- 0.16; llama.cpp CUDA (`external`, the
  pod's b10660 `build-cuda`, `-ngl 99 -fa 1`) 4997.0 +/- 553.3 / 223.87 +/- 1.93 - the CUDA home 1.06x the reference's prefill and 1.04x its decode with the
  streamed prompt idle (nothing to stream). zen2's plan for the same file at the strict 32k keeps 16 of 41 routed layers'
  experts on the host: CUDA pp512 1064.9 +/- 118.2 / tg128 83.08 +/- 3.99, and 1054.7 +/- 52.6 / 75.03 +/- 6.36 over five
  reps a few minutes later - a decode jitter the UD-Q4_K_M's rows never show (64.59 +/- 0.23); the iq4xs host sums are the
  suspect and the item stands open.
- **MEASURED (2026-10-28, `debug-jit`, `direction-grade`; `lcpp_bench` as the heading, the tree of 6f9aca720d - the afternoon
  A/B and the tile race 19c47e0501's) - gemma-4-26B-A4B on the pod's plan (the same pod, boost, the served
  shape, 16 threads, pp512 / tg128, 3 reps), both files whole on the 32 GB card.** UD-IQ3_XXS: ours CUDA 4991.1 +/- 37.1 /
  178.11 +/- 0.01; ours Vulkan 5660.2 +/- 79.4 / 169.20 +/- 0.14; llama.cpp CUDA (`external`, b10660, `-ngl 99 -fa 1`) 5447.0 +/- 368.9 / 185.91
  +/- 0.94. Q4_K_M: ours CUDA 5166.8 +/- 26.8 / 154.09 +/- 0.03; ours Vulkan 6632.3 +/- 60.2 / 154.69 +/- 0.09; llama.cpp (`external`) 6159.0
  +/- 516.7 / 154.99 +/- 0.74. With nothing on the host the CUDA home's gemma-4 prefill trails: 0.92x and 0.84x the reference,
  0.88x and 0.78x our own Vulkan home's tiles; its decode reads 0.96x and 0.99x the reference. zen2's rows above had the CUDA
  home ahead because the host split dominated both engines there. The same afternoon's interleaved A/B (one process a
  pass, Vulkan then CUDA, two passes a file, pp512 alone, 3 reps) did not reproduce the morning's prefill gap on the
  IQ3_XXS file and halved it on the Q4_K_M one: UD-IQ3_XXS Vulkan 5626.9 +/- 92.9 and 5631.8 +/- 84.9 against CUDA 5981.4
  +/- 46.8 and 5959.2 +/- 74.3 (CUDA 1.06x); Q4_K_M Vulkan 6620.0 +/- 53.9 and 6629.3 +/- 56.6 against CUDA 6278.2 +/- 72.4
  and 6262.4 +/- 76.4 (CUDA 0.95x). The IQ3_XXS experts ride the iq3xxs e tiles on both homes and the CUDA home is ahead
  there; the Q4_K_M file's experts are k4 gate/up stacks and q51 down stacks. Nsight Systems over the Q4_K_M window
  (`--cuda-graph-trace=node`, one 512-row window): 82 ms wall, ~111 ms of kernel time (the chains overlap); the k4 expert
  tiles are half of it - `CuK4Cm2S` 24.0 ms (60 x 399 us), `CuK4Cm2M` 17.4 ms (60 x 290 us) - then the q8 int8 leaf 11.7,
  the split-K reduce 8.0 (90 x 88 us), the q51 s and m tiles 7.3 and 6.2, the residual step 4.0 (90 x 44 us). The expert
  tiles raced on both homes at the file's expert shape (`harness/gemm_cuda_vs_vulkan.das -- --only moe`: a schedule of
  128 regions of 32 rows on the s column and 32 regions of 128 rows on the m or e column, each region its own expert plane,
  k 2816 d 704 for the gate and up stacks, k 704 d 2816 for the q51 down), Vulkan against CUDA, us a dispatch: k4 s 504 /
  516 (CUDA 0.976x), k4 m 214 / 231 (0.92x), q51 s 474 / 469 (1.00x), q51 m 259 / 227 (1.14x), iq3xxs s 698 / 485
  (1.44x), iq3xxs e 256 / 230 (1.11x) - the k4 gate/up tiles carry the Q4_K_M gap and the iq3xxs tiles the IQ3_XXS lead.
  By region count the k4 m tile reads CUDA 1.18x at 16 regions (96 workgroups, under one wave of the 164 block slots) and
  0.90-0.96x at 32, 64 and 128; forcing one block an SM (a 60000-byte dynamic shared ask, a scratch probe) reads 0.65-0.70x
  of the two-block tile at every count, so the pair is the tile's latency hiding and the multi-wave deficit is the
  lowering under two co-resident blocks on distinct expert planes - a kernel item of its own. On the 5060 Ti at locked
  clocks the same race reads k4 s 1134 / 1141, k4 m 478 / 466, q51 m 493 / 419 (1.18x), iq3xxs s 925 / 1032 (0.90x),
  iq3xxs e 445 / 502 (0.89x): the two cards disagree on the iq3xxs tiles.
- **MEASURED (2026-10-28, `debug-jit`, `direction-grade`; `lcpp_bench` as the heading, the tree of 0e8f736d95) - the E-series
  on the CUDA home: gemma-4-E2B Q8_0 at the served shape (pp512 / tg128, 3 reps, 16 threads).** With the pre-step on the host and the window's global layers (head 512) on the wide
  batched attention kernel (`DaAttnBWT`): ours CUDA pp512 4923.0 +/- 20.5, tg128 126.20 +/- 0.15, against ours Vulkan 8791.6
  +/- 86.8 / 113.28 +/- 1.72 - 0.56x the Vulkan prefill. Nsight Systems over five windows: `CuDaAttnBWF16` 13.1 ms a
  layer-window, 49 % of the kernel time (seven global layers a window); `CuQ8Mmq` 0.22 ms a GEMM, 22 %; the pre-step's
  projection on the router GEMV (`CuRouterGemvBf16`, 76 dispatches a window under `GEMV_COL_TILE`) 23 ms a window, 15 %. The
  MMQ leaf off (`DASLLAMA_CUDA_MMQ=0`) reads 4514 - not the GEMMs. `FaH512T`'s workgroup-tile arm asks 104,960 B of dynamic
  shared memory (the 64 x 512 q stage, the 512 x 32 K stage and the P tile, each row padded eight halves) and the card's
  101,376 refuses it (`cuFuncSetAttribute: invalid argument`), so the CUDA home stamps the template's KHR arm (`CuFaKhrH512`,
  16-row q tiles over the warps' 16 x 16 fragments): 7095.3 +/- 11.7 / 127.92 +/- 0.20, the h512 layers 0.81 ms a
  layer-window. The pre-step's window form on the cm2 f16 tile (`F16GemmCm2T`, the Vulkan class lifted to the discrete layer
  and stamped on both homes; the bf16 rows as an f16 panel written once at the seat, the embedded rows as f16 under
  1/sqrt(dim)): 9707.2 +/- 30.1 / 128.80 +/- 0.08 - 1.10x our Vulkan home's prefill and 1.14x its decode; llama.cpp CUDA
  (`external`, b10660, `-ngl 99 -fa 1`) 7225.3 +/- 812.3 / 121.04 +/- 0.62 (1.34x / 1.06x). Parity through the steps, the E2B perplexity
  cell against the CPU chain's 1.0406 (the Vulkan home 1.0407): 1.0408 with the host pre-step, 1.0386-1.0412 with the device
  pre-step on the router GEMV, 1.0412 with the KHR flash tile, 1.0419 with the window tile; the window's last side row sits
  8.2e-4 off the CPU pre-step's (a 17.3 largest), the token command's 2.9e-6 (16.0).
- **MEASURED (2026-10-28, `debug-jit`, `direction-grade`; `lcpp_bench` as the heading, the tree of d0fa7a57f5) - gemma-4-E4B
  Q8_0 at the served shape (pp512 / tg128, 3 reps, 16 threads), the CUDA home after the window's two fixes above.** Ours CUDA
  pp512 5613.7 +/- 47.7, tg128 66.35 +/- 1.27; ours
  Vulkan 4588.5 +/- 4.4 / 62.89 +/- 0.54; llama.cpp CUDA (`external`, b10660, `-ngl 99 -fa 1`) 4340.8 +/- 448.1 / 67.00 +/- 1.10 - the CUDA
  home 1.22x our Vulkan home's prefill and 1.06x its decode, 1.29x the reference's prefill and 0.99x its decode. Its tq4 mirror
  scored 14407 before the shared-K/V store fix (the codec store wrote a shared layer's stale staged rows over its donor's mirror
  rows); 1.07x of the CPU chain's perplexity after it, as the f16 mirror's.
- **MEASURED (2026-10-28, `debug-jit`, `direction-grade`; `lcpp_bench` as the heading, the tree of 3281855e21 - the graph
  prologue 47464697ec's) - the greedy chain on the CUDA home (`DASLLAMA_CUDA_CHAIN`: the
  Vulkan seat's form on the one stream, the prologue one graph).** What the chain removes: on Llama-3.2-1B Q8_0 at the served
  shape the GPU idled 5.1 percent of the decode wall - two gaps of 81 us median a 3.8 ms step, the landing and the next launch
  (`nsys --cuda-graph-trace=node` over `lcpp_bench -p 0 -n 128`). A fed-argmax probe (the logits landed every step, 256 steps
  after a 40-token prompt) read the chain on and off alike on Llama-3.2-1B Q8_0 and Qwen3.5-0.8B Q8_0, every step after the
  first a hit on both; its rates are not kept - no instrument of the tree produced them. `lcpp_bench --mtp-ab`'s plain arm on
  Qwen3.5-0.8B-MTP Q8_0 (real prompts, the device pick): 356.0 +/- 1.4 under the chain against 374.2 +/- 0.6 (0.95x), the
  speculative arm of the same pair of runs - a live carry never chains - 484.1 against 498.5 (the runs' drift, 0.97x). Before the
  prologue rode a graph (a copy a recurrent layer's state slot, eighteen launches a step on this carrier) the same arm read
  344.8 and 334.7 against 372.1 and 368.5 (0.92x). `lcpp_bench`'s synthetic `tg128` row, whose fed ids never match the pick, reads 134.4
  against 272.5 tok/s under mode 1 on the 1B (every chained step a miss: a step's work twice) and 263.1 under the adaptive
  mode (-1). The seat ships off by default, as the Vulkan home's does.
- **MEASURED (2026-10-28, the forced-feed arms through `tests/run.das -jit`, the 5060 Ti, the tree of 3281855e21) - the
  Phi-3.5-mini Q4_K_M one-window readings on both homes, replacing the 2026-10-22 "CUDA resident bars" entry's 2.64 and
  2.75 of 34 and 42 at positions 41 and 44 (7.7 and 6.6%).** Of the step's max |logit| against the CPU chain after the
  40-token `resident_prompt`: position 41 reads 1.55 of 34.3 on the CUDA home and 1.40 on the Vulkan home (4.5 and 4.1%),
  position 44 2.48 and 2.42 of 41.6 (5.96 and 5.8% against the 6 percent bar), the argmax the CPU's at every step, the
  other six steps 0.7 to 1.4 apart and the two homes within 0.2 of each other at every step; the fixture's one-step-off
  rows sit 4.8 of 48 apart at three of the steps (the control a wider bar would let through); the CUDA kernels compiled
  without `--use_fast_math` (the option list of `das_cuda_boost.das` edited for the probe, not a shipped flag) read 2.84
  at position 44. The readings are a one-window `cuda_forced_feed` arm of `tests/test_gpu_resident_cuda_phi3.das` under
  `DASLLAMA_GPU=1 DASLLAMA_CUDA=1` and the Vulkan regions file's twin under `DASLLAMA_GPU=1`; neither file keeps the arm,
  so the command line that re-runs it is owed (`followup_general.md` row 270).
- **STATED - the lasting allocations the CUDA home's host-experts, stream, chain, head and hidden-rows paths add**
  (computed from the geometry, no device measurement; `dim`, `ner`, `vocab` the model's, `nb` = `RDEC_NROW_MAX` rows, `np`
  the window's rows, `regions` the mirror regions; `TOK_META_BYTES` 32, `CRD_NB` 16, `RDEC_DN_SNAPS` 4). The pinned expert
  mirrors (`stream_stack_make` over `cud_pinned_alloc`): a streamed stack's `keep` resident experts x an expert's q + s
  plane bytes, a plane kind, so a host layer whose experts are all resident costs its three stacks whole - one layer's
  planes, 325 MB on the 26B UD-IQ3_XXS and 484 MB on its Q4_K_M (the slot figures above): 1.95 GB over the IQ3_XXS plan's
  6 host layers and 7.26 GB over the Q4_K_M's 15 on the 5060 Ti. The pinned transient ring (`stream_ring_make`):
  `DASLLAMA_EXPERT_RING` slots x (the widest q stride + the widest s stride) a plane kind over three kinds, a stride the
  widest expert's bytes rounded up a page plus a page; 0 slots by default, so nothing unless the knob names a count. The
  reader bounce buffers (`stream_readers_make`): `DASLLAMA_EXPERT_READERS` (8) x the widest ring stride. The chain's
  scratch (`cxf_planes`): `(cap + TILE_READ_SLACK) x (n + nfe) x 2 + cap x (2 nfe + n) x 4` bytes at `cap =
  cxf_chunk_rows(n, nfe)` over the expert shape (n, nfe), plus `np x n x 4` of x rows; the schedule planes
  (`cxf_entries`): `XF_RING` (16) entries x (3 x words x 4 + 2 x cap x 4), `words = 4 nreg + ((cap + 31) / 32 + nreg) x
  wtiles` at the window's region count - both need the file's expert shape and the carve's cap, their numbers owed. The
  host-cut mapped blocks (the normed rows, the logits, the sums): the token command's `nb x (2 dim + ner) x 4` and the
  window's `np x (2 dim + ner) x 4` - 8,511,488 bytes at np 512 on Qwen1.5-MoE (dim 2048, ner 60), 11,796,480 on the 26B
  at the race's shape (dim 2816, ner 128). The checkpoint rows (`crd_snap_alloc`): `RDEC_DN_SNAPS x regions` x the sum
  over the recurrent layers of (`slot_state_bytes` + 2 x `ring_image_bytes`) - the Vulkan `rd_snap_alloc` entry's formula,
  the same per-layer terms; the rollback rows (`crd_roll_alloc`): `(verify_rows - 1)` x the same sum; both need the
  hybrid's per-layer bytes, their numbers owed. The head planes (`crd_head_planes`): `nb x (TOK_META_BYTES + 24 dim)` -
  393,728 bytes at nb 16 and dim 1024 (Qwen3.5-0.8B), 1,573,376 at dim 4096. The out block: `(CRD_NB + vocab x nb + dim
  x nb) x 4` mapped bytes - 15,958,080 at vocab 248320, dim 1024, nb 16. The greedy chain's two slots (`crd_chain_alloc`):
  `2 x ((8 + cos_elems) x 4 + (CRD_NB + vocab) x 4)` mapped bytes - 1,026,240 plus 8 x `cos_elems` on the 1B (vocab
  128256), 1,986,752 plus the same on Qwen3.5 (vocab 248320). The hidden rows (`g_crd_hidden`, host): `np x dim x 4` -
  2,097,152 at np 512 and dim 1024 (Qwen3-Embedding-0.6B), 8,388,608 at np 2048. The window's host-side maps (`g_cxf_bkt`,
  `g_cxf_pos`, `g_rdec_stream_experts`): `4 x nrows + 16 x regions` bytes. The drafter's device planes (`cud_buffers_make`):
  the sidecar's q8 quants, scales and f32 blob as they lie, plus about `9.125 x (dim + hidden + qdm) + 18.25 x tdim + 8 x
  vocab + 4 x cos_elems + 4 x n_heads x (hs_mx + 3)` bytes of scratch and mapped blocks (qdm = n_heads x hs_mx) - the
  number needs the drafter sidecar's shape, owed.

### From the short windows on the CUDA home (2026-10-28, the reference box: Threadripper 3990X, RTX 5060 Ti 16 GB, driver 610.74, Windows, the GPU clock locked at 2392 MHz; the pod: RTX PRO 4500 Blackwell 32 GB, 82 SMs, Linux, boost; `debug-jit`, `direction-grade` - every tok/s row `benchmarks/lcpp_bench.das`: `DASLLAMA_GPU=1 [DASLLAMA_CUDA=1] bin/daslang -jit modules/dasLLAMA/benchmarks/lcpp_bench.das -- -m <gguf> --for-debug-purposes -p <rows> -n 0 -r 5 -t 16 --ngl 0`, `DASLLAMA_CUDA=1` on the CUDA rows alone, `DAS_TUNE_POLICY`, `DASLLAMA_COOPMAT` (auto: cm2), `DASLLAMA_CUDA_MMQ` (1: the int8 leaf where the row's gate admits it), `DASLLAMA_CUDA_PDL` (1) and `DASLLAMA_CUDA_CHAIN` (0) unset; a row is the mean of its 5 reps, its spread kept only where shown - the other rows' spreads are owed; the knee rungs are `cud_split_knee_race`'s own lines, the entry says whose)

- **The split-K knee races on the CUDA s column** (`cud_split_knee_race`, `dasllama/dasllama_cuda_decode.das`: a q8 site of K
  2048 at 31 rows, the whole tile against its split at two, four and eight ways, us a GEMM over five widths; the rungs are the
  race's per-arm `timing-only` lines through `dasllama_gpu_knee.das`'s `knee_pick`, printed by `dasllama_cuda_knee_race` at a
  fat exe's first start (`dasllama_fat_start.das`'s runtime-section race) and by `harness/tune_kernels.das`'s knee hook under
  `DASLLAMA_GPU=1 DASLLAMA_CUDA=1`; these rungs are the tree of 57d2ff4531's, and the command line that printed each log is
  owed). The RTX PRO 4500: 8 tiles whole 34.9, split x2 / x4 / x8
  22.0 / 13.3 / 10.0; 16 tiles 34.8, 21.9 / 14.6 / 16.9; 32 tiles 34.9, 22.8 / 25.8 / 29.5; 64 tiles 35.2, 42.6 / 46.5 /
  50.5; 128 tiles 69.0, 80.2 / 79.2 / 90.5 - the knee prices at 64 (`cud_split_k_tgs`'s default). The 5060 Ti: 8 tiles 42.0,
  33.3 / 28.4 / 26.5; 16 tiles 42.2, 34.2 / 29.0 / 30.6; 32 tiles 43.0, 42.0 / 45.4 / 52.2; 64 tiles 95.5, 122.2 / 82.4 /
  131.6; 128 tiles 239.3, 183.2 / 189.8 / 202.8 - 256. On Llama-3.2-1B Q8_0 the knob alone moves the 5060 Ti's window one
  percent (`lcpp_bench -p 20` and `-p 31` at `-n 0 -r 5`: pp20 2839 at 0, 2868 at 64, 2872 at 256; pp31 4389 / 4360 / 4404;
  how each knob value was pinned for its run is owed): under the int8 MMQ leaf the short window's
  GEMMs were not where its time went. Those rungs streamed one weight copy the L2 held.
- **MEASURED (2026-10-29, the same race on the same two cards, the tree of c48ccbceaa) - the knee rungs over the Vulkan race's
  loop and its 192 MiB ring (`KNEE_DISCRETE_RING_BYTES`), replacing the cached rungs above and the 5060 Ti's 256.** Pod: 8
  tiles whole 34.8, split x2 / x4 / x8 22.0 / 13.3 / 10.0; 16 tiles 38.0, 23.3 /
  15.9 / 19.1; 32 tiles 38.4, 24.8 / 27.6 / 31.9; 64 tiles 40.3, 46.0 / 49.7 / 53.3; 128 tiles 78.8, 87.6 / 85.8 / 98.0 - 64
  again. The 5060 Ti: 8 tiles 43.4, 31.6 / 23.7 / 20.0; 16 tiles 44.0, 32.9 / 27.7 / 31.5; 32 tiles 45.8, 45.1 / 49.0 / 54.8;
  64 tiles 76.0, 84.4 / 91.5 / 96.9; 128 tiles 148.4, 171.8 / 182.3 / 203.3 - 64 now, where the cached rungs priced 256: the
  split pays only while the whole-K grid leaves most SMs idle. Beside them the Vulkan race on the same cards (a pair of roles a
  step): pod 16 tiles whole 17.2 against 38.1 / 24.3 / 18.3 split, the 5060 Ti 16 tiles 21.1 against 47.5 / 36.3 / 34.2 - the
  split a loss at every rung on both, the knob 0. At the smallest grid CUDA's whole s tile takes 35 to 43 us a role where
  Vulkan's pair takes 17 to 21 us together: a lowering gap of the s stamp alone (`followup_general.md` row 273), not the split
  rule.
- **MEASURED (2026-10-29, the same two cards, the tree of the arc's landing commit - "the NextN head, host experts and the
  K/V ring on the CUDA home" over 383da4bcc6: the race times its arms in turn -
  `knee_arms_us`, two untimed rounds of each then seven rounds alternating whole and split - and the ring cursor carries across
  the rungs; `DAS_LOG_LEVEL=info DASLLAMA_GPU=1 DASLLAMA_CUDA=1 bin/daslang -jit dastest/dastest.das -- --test
  modules/dasLLAMA/tests/test_fat_start.das --test-names test_the_cuda_knee_race_sets_the_knee`, the Vulkan race the same
  command without `DASLLAMA_CUDA=1` on `test_the_vulkan_knee_race_sets_the_knee`; replacing the entry above's rungs, the
  verdicts unmoved.** CUDA, pod: 8 tiles whole 37.8, split x2 / x4 / x8 23.0 / 14.0 / 11.1; 16 tiles 38.1, 23.5 / 16.2 / 19.3;
  32 tiles 38.8, 25.1 / 28.1 / 32.3; 64 tiles 40.5, 46.4 / 50.0 / 53.7; 128 tiles 79.3, 88.2 / 86.3 / 98.4 - 64. The 5060 Ti:
  8 tiles 43.5, 30.8 / 22.5 / 21.2; 16 tiles 44.1, 33.2 / 28.3 / 31.8; 32 tiles 46.4, 45.4 / 49.3 / 54.7; 64 tiles 76.1,
  85.0 / 91.5 / 99.1; 128 tiles 148.3, 172.0 / 181.3 / 202.8 - 64 (`cud_split_k_tgs`'s default stands). Vulkan, pod: 16 tiles
  whole 17.2, split 38.1 / 24.2 / 18.0; 32 tiles 18.2, 39.1 / 26.2 / 22.6; 64 tiles 28.8, 42.5 / 35.7 / 38.1; 128 tiles 61.3,
  65.3 / 68.8 / 69.2; 256 tiles 127.0, 132.0 / 135.4 / 134.2 - 0. The 5060 Ti: 16 tiles 21.4, 47.3 / 36.3 / 34.6; 32 tiles
  38.0, 55.5 / 47.0 / 49.9; 64 tiles 72.5, 82.9 / 85.0 / 90.1; 128 tiles 145.5, 165.3 / 167.4 / 170.6; 256 tiles 297.0,
  320.1 / 323.3 / 364.0 - 0. Alternating the arms moved no rung past its spread: the cached-plane figures were not an
  ordering artifact.
- **The int8 MMQ leaf padded a short window to its 128-row tile.** Nsight Systems over four 20-row Llama-3.2-1B Q8_0 windows on
  the pod: `CuQ8Mmq` 384 launches at 57.2 us (21.9 ms of 20.7 ms wall - the chains overlap), its residual requant step
  `CuClsArAddRmsRqB` 132 at 72.0 us (9.5 ms), the cm2 flash tile 64 at 5.4 us; the admission read the plan's rows, not the
  window's. The window's rows now gate the leaf (`CudRdec.pf_wlen`, `CUD_MMQ_S_ROWS`): a window of one s tile or fewer rows
  takes the cm2 s column, and the wave model's split-K reaches every window the grid underfills (it stopped at 128 rows
  before). Llama-3.2-1B Q8_0, the padded leaf against the s column, pod: pp20 3964 -> 5773 (Vulkan 6806), pp32 6304 against
  8503 (Vulkan 9906), pp48 9387 against 8643, pp64 12409 against 11354, pp96 18296 against 10359; the 5060 Ti: pp20 2868 ->
  3066 (Vulkan 3256), pp24 3444 -> 3600, pp28 3993 -> 4294, pp32 4497 against 4844 (Vulkan 5071), pp48 6752 against 4653,
  pp64 8844 against 6392 (Vulkan 8113), pp96 12875 against 7514 (Vulkan 9625), pp128 16960 (Vulkan 14526). The crossover
  sits between 32 and 48 rows on both cards, so the gate is the s tile's 32. Qwen3-Embedding-0.6B Q8_0 on the pod, the s
  column against the padded leaf: pp20 6659 against 4376 (Vulkan 6690), pp32 5707 against 6948 (Vulkan 11297) before the
  wave split reached 32 rows.
- **The five files at 20 and 32 rows on the pod under the gate** (CUDA against Vulkan, the padded leaf's reading in
  brackets): Llama-3.2-1B Q8_0 pp20 5734 (3964) against 6806, pp32 8500 (6315) against 9906; Qwen3-Embedding-0.6B Q8_0 pp20
  6669 (4376) against 6690, pp32 10570 (6948) against 11297; gemma-4-E2B Q8_0 pp20 2009 (1501) against 2466, pp32 3298
  (2350) against 3641; gemma-4-26B-A4B Q4_K_M pp20 767 (750) against 839, pp32 1155 (1051) against 1277; UD-IQ3_XXS pp20 733
  (731) against 641, pp32 1250 (905) against 1072. What remains on the dense Q8_0 files (0.84-0.94x) is the s stamp's
  lowering against the driver's (`followup_general.md` row 273) and the residual steps, not the pick; an
  embedding's window stays on the tiles whatever its rows, so a batch's vectors are its singles' (the `test_embeddings.das`
  batch cells hold the 1e-4 cosine bar only on one GEMM form).
- **Every format on a 20-row window, the pod** (Llama-3.2-1B, CUDA against Vulkan, tok/s; every CUDA row the split s column,
  since no leaf serves at or under 32 rows): IQ2_XXS 4713 against 1274, IQ3_XXS 4974 against 4531, IQ3_S 4748 against 5785,
  Q2_K 5247 against 4868 (+/- 454), Q3_K_L 4279 against 2731 (+/- 2229), IQ4_NL 4732 against 6013, IQ4_XS 4308 against 5759,
  NVFP4 4335 against 5442, Q4_0 5337 against 5402, Q4_K_M 4906 against 6051, Q5_1 4383 against 5713. Every format serves;
  the 4-bit lattice and K-quant s stamps trail the driver's by 19-25 percent (`followup_general.md` row 273; the 26B's k4 tile item is row 269), the iq2 and
  iq3xxs stamps lead, and `test_cud_cm2s_short_split_family` holds each row's split against its oracle.

- **MEASURED (2026-10-30, `debug-jit`, the nv4 CPU kernels against q40's - the format whose nibble tiling nv4 shares -
  `kq_kernel_bench.das --fmt nv4,q40 --rm --ntok 512` under `DAS_TUNE_MODE=test`, `DAS_TUNE_POLICY` unset and the CPU
  backend on auto, n 14336 d 4096, best of 5, us; zen4 = x86-vnni512, the M5 Max = arm-i8mm).** zen4: gemv nv4 1103 (`dot_vpdpbusd_width512_mr16`) vs q40 811, 1.36x; tile over 512 tokens 250354 vs 122670,
  2.04x; rm_gemv 4377 (`dot_vpdpbusd_width256_mr8`) vs 1670, 2.62x; rm_tile 897958 vs 568159, 1.58x. The M5: gemv 1614
  (`mr8`) vs 554, 2.9x; tile 358094 vs 204686, 1.75x; rm_gemv 3083 (`mr4`) vs 1519, 2.0x; rm_tile 572842 vs 343202, 1.67x. The reference bodies sit 7x to
  30x behind every stamped perm, so the stamps are the serving floor; the gap to q40 is the per-strip float fold - two
  UE4M3 decodes (a dozen vector ops each), two converts and two fmas a block where q40 folds one integer sub-sum under one
  f16 d - and the lever is decoding a superblock's sixteen scales once as two vectors and folding the strip pair as one fma
  over a pre-combined scale lane (`followup_general.md` row 246).
- **MEASURED (2026-10-30, the same race after the scale path and the accumulator split - the row-major rail decodes a
  superblock's sixteen scales once as vectors and picks a block's strip lanes by shuffle; the UE4M3 decode is eight ops; the
  grp tier lands d8 once a superblock; `rm_block_loop` runs four accumulator sets abreast in the GEMV, two in the x64 tile,
  one on NEON's - `kq_kernel_bench.das --fmt nv4,q40 --rm --ntok 512` with `-p vpdpbusd,maddubs` on zen4 and `-p mr4,mr8`
  on the M5, `DAS_TUNE_POLICY` unset and the CPU backend on auto, us, best of 5; `debug-jit`, the before -> after readings
  `direction-grade` - the before figures are the entry above's run).** zen4: rm_gemv nv4 4377 -> 1759 against q40's 1729
  (1.02x); rm_tile 897958 -> 526914 against 570040 (nv4 ahead); gemv 1103 -> 1057 (the width-256 mr16 row; the tile
  family's race crowns the pair at width512_mr16, whose gemv row reads 1113 - the gemv row alone is faster at width 256)
  against 811 (1.30x); tile 250354 -> 242580 against 125361 (1.93x). The M5: rm_gemv 3083 -> 1911 against q40's 1519 -> 1005
  (the split lifts every row-major family, q40 by 1.5x); rm_tile at four tokens 4092 against 2589; gemv 1614 -> 1392 against
  554; tile 358094 -> 306936 against 204433. The row-major GEMV on NEON was latency-bound - one fma chain, sixteen folds a
  superblock at three cycles each: 48 cycles a superblock against the 50 the before figure reads as 3083 us over the
  4096 x 56 superblocks at the M5's ~3.7 GHz - with only a quarter more instructions than q40 (214 against 171 a superblock,
  the `sbbody` loop of `nv4q8_rm_gemv_gen__mr4` / `q40q8_rm_gemv_gen__mr4` under `kq_kernel_bench ... --jit-dump`); the
  split, not the decode, moved it. What stays is the grp tier's per-strip decode and split accumulators, inherent to the k4
  nibble tiling the device layout shares: the planar kernels keep 1.3x (gemv) to 1.9x (tile) behind q40 on x64, 2.5x and
  1.5x on NEON, and the layout stays as the GPU reads it (hardware compatibility over the last 10%). The accumulator split read
  neutral on zen4's row-major GEMV across the two builds (the in-process race in the entry below puts one set 5 to 10%
  ahead there) and a loss on NEON's tile at two sets (4101 -> 6865 us at four tokens, spills - `RM_TILE_ACC_SETS` set to 2
  on NEON before the per-ISA pick, a one-constant build that is not in the tree; the `accs` perm axis of the row-major
  families now races the candidates in one process).
- **MEASURED (2026-10-31, served, the pod - RTX PRO 4500 Blackwell 32 GB, EPYC 7443P 48 vCPU, a 62 GB container cgroup:
  Qwen3.6-35B-A3B NVFP4 (knoopx) against a pure Q4_0 of the same model (bartowski's Q4_0 requantized `--pure`, its Q4_1
  `ffn_down_exps` have no reader arm), every row the two engines on the same file; ours `lcpp_bench.das
  --for-debug-purposes -p 512 -n 128 -r 5` under `DASLLAMA_IMAGE=0 DASLLAMA_ALLOW_UNTUNED=1`, the split
  `DASLLAMA_GPU_HOST_LAYERS=4`, `DAS_TUNE_POLICY`, `DASLLAMA_COOPMAT` (the Vulkan rows), `DASLLAMA_CUDA_MMQ` and
  `DASLLAMA_CUDA_PDL` (the CUDA rows) unset; the llama.cpp figures `external` - 9febe63 `llama-bench -p 512 -n 128 -r 5
  -ngl 999 -t 48`, its split `-ot 'blk\.[0-3]\.ffn_.*_exps=CPU'`, the recipe in
  `ARCHITECTURE_MEASUREMENT_REFERENCE_ROWS.md#cuda-reference-rows`; tok/s, +/- the run's sd; `debug-jit`, the cross-engine
  readings `direction-grade`).** Vulkan, nv4: ours full 4821 +/- 76 / 174.3 +/- 0.1,
  split 3762 +/- 142 / 168.4 +/- 0.8; llama.cpp full 4795 +/- 49 / 110.0 +/- 0.3, split 1910 +/- 117 / 12.0 +/- 5.8.
  Vulkan, q40: ours full 5259 +/- 69 / 205.2 +/- 0.1, split 4118 +/- 167 / 196.6 +/- 1.0; llama.cpp full 5567 +/- 78 /
  200.0 +/- 1.0, split 2038 +/- 52 / 19.6 +/- 5.3. CUDA, nv4: ours full 5224 +/- 51 / 192.7 +/- 0.3, split 4288 +/- 135 /
  179.6 +/- 0.6; llama.cpp (its CUDA build, `-DCMAKE_CUDA_ARCHITECTURES=120`, the native Blackwell NVFP4 kernels) full
  7949 +/- 839 (cv 10%: VOID, to re-measure - the five reps ranged past the sd; its median would read half again ahead of
  ours, and nothing here leans on it) / 138.6 +/- 0.4, split 3393 +/- 209 / 27.6 +/- 3.5. CUDA, q40: ours full 5361 +/- 59
  / 234.1 +/- 0.8, split 4489 +/- 107 / 215.4 +/- 1.2; llama.cpp full 5867 +/- 356 (cv 6%) / 247.6 +/- 1.5, split 2959 +/-
  142 / 36.4 +/- 6.6. Read: nv4 costs us 8% of pp512 and 15% of tg128 against q40 on Vulkan, 3% and 18% on CUDA - the
  decode GEMV's UE4M3 fold against q40's one d; llama.cpp's CUDA NVFP4 prefill (the FP4 tensor op) is the
  `followup_general.md` row 246 (4) leaf, its full-model figure void here; its NVFP4 decode trails ours on both homes. The
  four-layer host split costs us 16-22% of pp512 and 3-8% of tg128 (the row-major rail's share on 48 avx2 cores);
  llama.cpp's tensor override halves its prefill and leaves a decode of 12-36 tok/s with a third of it as spread. Numbers
  are debug-grade (`-jit`, the untuned pod sidecar's crowns), one process a row, no affinity pin. The lasting allocation
  the nv4 stacks add: `Model.exps_scale`, one float an expert a scaled stack (the 35B: 40 layers x 3 stacks x 256 x 4 B =
  123 KB) plus the `site_scale` table's entry an expert, and the Vulkan mint view's table holds the same entries a gather
  (freed with the view); a dense nv4 carrier adds none.
- **MEASURED (2026-10-31, `debug-jit`, the row-major accumulator sets raced in one process - the `accs` perm axis of the
  rm families, `kq_kernel_bench.das --fmt nv4,q40 --rm --ntok 512` under `DAS_TUNE_MODE=test`, `-p mr4,mr8` on the M5 Max
  (arm-i8mm, the box profile `performance/m5.tune.json`) and `-p vpdpbusd,maddubs` on zen4 (x86-vnni512, the class profile),
  `DAS_TUNE_POLICY` unset, the CPU backend on auto, best of 5, us; no suffix = the shipped pick: four sets in the GEMV, two
  in the x64 tile, one in NEON's).** The M5, nv4: rm_gemv `mr4` 1927, `mr4_accs1` 3082, `mr4_accs2` 1985; rm_tile `mr4`
  536621, `_accs1` 536813, `_accs2` 844747. The M5, q40: rm_gemv 1017 / 1514 / 1005; rm_tile 343100 / 343176 / 343097. zen4,
  nv4: rm_gemv `dot_vpdpbusd_width256_mr8` 1941 / `_accs1` 1746 / `_accs2` 1890, `dot_vpdpbusd_width512_mr16` 1878 / 1835 /
  1907, `dot_maddubs_width256_mr8` 1924 / 1936 / 1927; rm_tile `dot_vpdpbusd_width512_mr16` 526647 / 529620 / 526794,
  `dot_vpdpbusd_width256_mr8` 555283 / 558998 / 555371, `dot_maddubs_width256_mr8` 638887 / 648039 / 639057. zen4, q40:
  rm_gemv `dot_vpdpbusd_width256_mr8` 1742 / 1657 / 1724, `dot_vpdpbusd_width512_mr16` 1871 / 1832 / 1876,
  `dot_maddubs_width256_mr8` 1888 / 1822 / 1851; rm_tile `dot_vpdpbusd_width512_mr16` 574428 / 567441 / 574437,
  `dot_vpdpbusd_width256_mr8` 569771 / 573691 / 569780. Read: on NEON the GEMV's four sets are the pick (one set is 1.6x
  slower, two sets 3% slower) and the tile's one set (two sets spill, 1.57x slower); on zen4 ONE set leads the GEMV by 5
  to 10% on every row (the entry above's "neutral" read the four-set build against the one-set build across two
  processes), and the tile is flat across the sets. The shipped constants stay as they are; the class profiles' races
  now hold the `accs` rows, so a box whose GEMV wants one set crowns `_accs1` at its next mint (x86-vnni512 and x86-amx
  re-mint on this branch).
- **MEASURED (2026-10-31, `debug-jit`, the AMX bf16 tile against the int8 tiles - AWS c7i.4xlarge, Xeon Platinum 8488C
  (Sapphire Rapids, x86-amx), 16 vCPU, `kq_kernel_bench.das --fmt nv4,q40,k4 --ntok 512 -p amx,vpdpbusd,maddubs,mr4` under
  `DAS_TUNE_MODE=test`, `DAS_TUNE_POLICY` unset, the CPU backend on auto, n 14336 d 4096, best of 5, us; the bf16 rows walk
  their panel - the bf16 panel built a token block off the grp planes, inside the time - as the production `bf16_walk`
  does).** tile over 512 tokens: nv4 `dot_amx_bf16_width512_mr16_kstep1_nrsplit2` 87130 / `_nrsplit1` 120963 against
  `dot_vpdpbusd_width512_mr16` 311602, `dot_vpdpbusd_width256_mr8` 396653, `dot_vpdpbusd_width256_mr16` 396241,
  `dot_maddubs_width256_mr8` 532587; q40 amx 85594 / 119247 against 141256 / 226466 / 194476 / 278343; k4 amx 85959 /
  119325 against 264608 / 301884 / 246440 / 355203. gemv (the companion rows, the same int8 GEMV under every tile perm):
  nv4 1390-1735, q40 1086-1106, k4 1101-1129. Read: the AMX bf16 tile is format-blind - nv4, q40 and k4 all land at 86-87 ms
  once their panel is bf16 - so on an AMX box nv4's prefill tile runs at q40's time (3.6x its own best int8 tile; q40 gains
  1.65x, k4 2.9x), and the grp tier's nv4 gap (the entry above, 1.3x-1.9x) is an int8-tile gap only. nrsplit 2 leads
  nrsplit 1 by 1.4x on every format. The x86-amx profile crowns `dot_amx_bf16_width512_mr16_kstep1_nrsplit2` for the three
  tile families.
- **MEASURED (2026-11-01, the zen4 vulkan rig leg - the Ryzen 7 PRO 8700GE's Radeon 760M, RADV Mesa 25.0.7, subgroup 32
  pinned - `performance/gen_bench_records.das -- --legs vulkan -w llm -o Qwen3.6-35B-A3B-MTP-UD-Q4_K_M --ref-vulkan
  <the 9febe63 build-vulkan llama-bench> --store <a scratch store>` under `DASLLAMA_GPU=1`: the released exe (`exe one unit`,
  10f52ca83, dasllama_version 17, the box's mint of that build) at `-p 512 -n 128 -r 5 -t 8`, the cold run kept; tok/s
  +/- sd).** das pp512 324.03 +/- 2.40, tg128 19.24 +/- 0.01; tg128@4 42.02 +/- 3.14 summed over four streams (cv 7.5%: VOID,
  to re-read). The rig's own reference cell, llama.cpp 9febe63 Vulkan: pp512 254.16 +/- 1.68, tg128 17.64 +/- 0.01 (no
  tg128@4 row - no `llama-batched-bench` sits beside that build). Before the arc the same box, file and bench line read
  237.60 +/- 3.49 / 17.50 +/- 0.09 as a `-jit` debug row (`debug-jit`, `direction-grade` against this entry; the merged nv4
  branch tip af4f1f7700, the branch base 730a27e5bd's Vulkan tier): the resident plan read one heap of the APU's two,
  declined the 21.6 GB of weights and summed 31 routed layers' experts on the CPU one layer a submit, and every attention
  layer took the chunked pair for want of a gated KHR flash stamp. With the room over every heap the model is resident
  whole, and the gated head-256 KHR stamps take the ten attention layers (`ARCHITECTURE_GPU_VULKAN_ROOM.md#vk-plan-room`
  carries the plan lines). zen4 carries no vulkan board row in `records/`; the release re-mint owes it
  (`followup_general.md` row 276).
