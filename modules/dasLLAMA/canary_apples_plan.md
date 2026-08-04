# Canary apples-to-apples + perf + memory arc

Goals (Boris, 2026-08-02): **1.** apples-to-apples — cli references go q8_0 (replacing the fp
reference config), canary goes q8 on our side (NeMo has no 8-bit; bf16 greedy stays their
best). **2.** canary performance. **3.** runtime memory — right bits first, census later in
session. Win = all three + end-of-arc profile on M1, M4, zen2. One PR, commits at stopping
points. This doc retires to `/history` at close.

Parity acceptance: token-exact ideally; word-exact or word-very-close ("The"/"the" class)
accepted; anything more divergent → stop and discuss with transcript diffs. Eyeball text on
all corpus clips at every phase boundary.

## Going-in facts

- gb1 das/nemo: 3.01× m1 / 3.64× m4 / 4.17× zen2 (site records 2026-07-29); short clips lead;
  m4 jfk3 already trails 1.19×. zen2 das gb1 = 224.9 s — the fp32 dot-loop with no AMX.
- Canary is the only carrier on the fp32 reference rail end-to-end (decoder
  `QuantMode.fp32` at dasllama_asr.das:206; all-f32 CNRY encoder). The fp32 kernels are
  bit-exact reference kernels by contract (`matmul_batch_core`) — never to be raced.
- NVIDIA ships the checkpoint 100% BF16 (safetensors header); nobody quantizes canary
  anywhere (llama.cpp: zero support; onnx-asr: no export). A q8 canary is the first.
- Board precision audit (scouting bench, M1 8T, interleaved best-of-3, cv≤2.5%): upstream
  `whisper-quantize`/`parakeet-quantize` q8_0 models transcribe correctly AND are the
  references' best config (faster in 6/7 cells); das still leads 8-bit-vs-8-bit
  (parakeet 0.73–0.76×, tiny 0.69–0.81×, turbo gb1 0.76×, turbo jfk 0.96× tie).
- Serve peaks (PERF_LEDGER 2026-08-02): canary 13.11 GB warm (≈9.5 GB fp32 weights +
  quadratic encoder attention scratch `sc[tt×tt]` + `rl[tt×ww]`, ww=2tt−1); parakeet
  6.69 GB at hp0x2 — same disease, and pk_/cn_ are deliberate twins.
- The results.md ASR pipeline is orphaned (ec09383fa): live surface is
  `performance/records/<box>.json` → gen_site_records → site; gen_results must learn the
  store (Phase 4).

## Phases

0. Arc doc + canary gb1 stage profile (cn.encode / cn.prefill / cn.decode split) — DONE rows
   in the log below.
1. Decoder q8 flip (dasllama_asr.das:206 → QuantMode.q8) + das-side exec_fmt stamping.
   Gates: test_canary_qwen_oracle (id re-freeze per protocol), canary_transcripts.expected,
   full-corpus eyeball. A/B vs NeMo m1; scoped reprofile.
2. Encoder q8: read-time tensor-at-a-time quantize in a schema-walk restructure of
   stage_canary_encoder (name-keyed offsets; no dual walk; peak staging ~1 GB); qblob/qscales
   planes; canary-q8 image family; cn_mm dispatch twins; A47 fix (asr_requant_rows_q8 →
   common's thresholded requant_rows_q8); facade rides set_asr_fp32; convert tool mints q8.
   Separate commit from Phase 1 (failure attribution).
3. q-block attention scratch, pk_ then cn_ (two commits): per-lane B-row slabs replacing the
   full sc/rl panels; keep 4-aligned block starts + FMA order (bit-exact); cn_ converges onto
   pk_'s threaded structure. hp0x2 serve-peak before/after.
4. Board plumbing: gen_results learns the record store (AsrProfile retires same commit);
   cli refs switch handles to q8_0 bins (engine tags unchanged; exec_fmt stamped);
   setup_asr_rig provisions q8_0 siblings; encode_ms to the store; uniform clip ladder
   (jfk/jfk3/gb1/hp0/hp0x2) LAST, with per-ref curation for >10-min cells.
5. Memory census: /usr/bin/time -l ladder around the rig exe, canary+parakeet, warm/cold,
   before/after table → PERF_LEDGER; canary mint re-measured.
6. Three-box reprofile (BRINGUP flow; LLM sweep per Boris) + close (ledger, README row,
   this doc → /history).

Out of scope, ledgered at close: gemma4a/qwen3a tower q8 (they LEAD CPU-only; windowed
attention — no quadratic scratch), streamed canary-f32 mint; canary 40 s chunked long-form
(the NVIDIA-sanctioned long-audio usage — would legitimize gb1 cross-box and unlock
hp0-class cells; needs a chunked NeMo arm + fixture re-freeze); gemma4a decoder-ctx bump
8192→16k (+335 MB KV under SWA — gemma's audio encoder is streaming USM with NO trained
envelope, so its cap is OUR facade clamp; 16k fits hp0x2's 13.7k tokens; task #8);
CN_MAX_NEW-class decode caps for gemma4a/qwen3a (same unbounded loop shape, never yet hit).

## Measurement log

(protocol: interleaved arms, ≥3 launches for A/B ratios, discard first, cv>3% void, one
process per box, Parsec off; stage splits are decompositions, not ratios)

### Phase 0 — going-in gb1 stage split (M1, 8T, fp32 rail, 2026-08-02)

`asr_stage_probe -m canary-dec-f16.gguf --mmproj canary-enc-f32.bin -w gb1.wav`
(DASLLAMA_CPU_PREFILL=1; warmup + 1 profiled rep)

| bucket | ms | % |
|---|---:|---:|
| cn.encode | 59548 | 47.5 |
| cn.prefill | 59481 | 47.4 |
| cn.decode | 6361 | 5.1 |
| total | 125390 | |

**Finding: encode and prefill split gb1 50/50** — the ledger's "prefill-dominant" hypothesis
was half right. The decoder flip (Phase 1) attacks ~66 s (prefill+decode); the encoder q8 +
attention restructure (Phases 2-3) attack the other ~60 s. Both halves are load-bearing to
reach NeMo's 35-41 s.

### Phase 1 — decoder q8 flip (M1, 8T, 2026-08-02)

Gates, all green: **transcripts byte-identical** to `canary_transcripts.expected` on
jfk/jfk3/gb1, and **`test_canary_qwen_oracle` PASSES token-for-token unchanged** (all three
clips incl. trailing EOS) — the q8 decoder holds the frozen NeMo greedy ids, so no id
re-freeze and the fp32 parity arm is not needed for canary correctness.

A/B vs NeMo (interleaved, 4 launches/arm, first discarded, best-of-3; das cv ≤0.5%, nemo cv
≤1.9% except jfk 5.1% — one outlier launch, does not move the conclusion; Phase 6's official
sweep re-measures):

| wav | das q8-dec ms | nemo ms | das/nemo now | was (site 07-29) |
|---|---:|---:|---:|---:|
| jfk | 2466 | 7743 | **0.32×** | 0.60× |
| jfk3 | 7780 | 21174 | **0.37×** | 0.70× |
| gb1 | 68868 | 35421 | 1.94× | 3.01× |

gb1 gap halved; the residual is the fp32 encoder (~86% of gb1 now — Phases 2-3's target).
Per-phase gates use these interleaved A/Bs; the OFFICIAL store sweep happens once at Phase 6
per the "reprofile end of session" call (no per-phase rig rebuilds).

Post-flip gb1 stage split (same probe): cn.encode 60233 (86.5%) / cn.prefill 5848 (8.4%) /
cn.decode 3516 (5.1%) — prefill 10.2× faster than the fp32 rail, decode 1.8×.

### Phase 2 — encoder q8 (M1, 8T, 2026-08-02)

Read-time tensor-at-a-time transcode landed (schema-walk stage, name-keyed layout — fixes the
A43 file-order coupling in passing; peak staging ≈ 1 GB, the fp32 GEMM bytes never
materialize). A47 fixed: `asr_requant_rows_q8` rides common's thresholded+profiled requant.

Gates, all green: **transcripts byte-identical** on jfk/jfk3/gb1; **oracle token-for-token
UNCHANGED on the full q8 rail** (encoder + decoder both quantized — no id re-freeze at any
point in this arc so far); canary-f32 image cell still element-exact; new canary-q8 image
cell element-exact (qblob/qscales/compact blob).

gb1 stage split (full q8): cn.encode 35193 (79.7%) / cn.prefill 5562 / cn.decode 3427 —
encoder 1.71× faster; the residual encoder time is the fp32 per-head attention
(gemm_f32 sc/rl/oh) + depthwise — Phase 3's target.

A/B vs NeMo (same protocol; cv ≤ 1.1% everywhere):

| wav | das q8/q8 ms | nemo ms | das/nemo now | Phase 1 | going in |
|---|---:|---:|---:|---:|---:|
| jfk | 1384 | 8473 | **0.16×** | 0.32× | 0.60× |
| jfk3 | 4639 | 21083 | **0.22×** | 0.37× | 0.70× |
| gb1 | 44160 | 35124 | 1.26× | 1.94× | 3.01× |

### Phase 3 — q-block attention slabs, pk_ then cn_ (M1, 8T, 2026-08-03)

Per-job B-row slabs (B=64, 4-aligned — gemm_f32 never mixes accumulation across 4-row
groups, so the decomposition is bit-exact) replace the full `sc[tt×tt]`/`rl[tt×ww]` panels
in BOTH twins; canary's serial per-head loop converged onto parakeet's threaded structure
(threaded packs + `gemm_f32_jo` + slab blocks) in the same change.

- **pk_**: oracles token-for-token (both, durations included). B=16 first cut measured
  −12% on hp0x2 encode (Kᵀ-panel re-streams); B=64 lands at/ahead of the full-matrix
  control (24.7–25.2 s vs 25.6 s). hp0x2 footprint −430 MB single-run (ladder in census).
- **cn_**: oracle token-for-token, transcripts byte-identical — STILL no id movement
  anywhere in this arc. gb1 encode 35.2 s → **8.8 s** (the serial head loop was
  single-threading everything); gb1 stage split now 8834 / 5660 / 3435 = 17.9 s total.

A/B vs NeMo (same protocol; das cv ≤ 0.4%, nemo jfk cv 3.4% best-of noted):

| wav | das ms | nemo ms | das/nemo | going in |
|---|---:|---:|---:|---:|
| jfk | 1304 | 7948 | **0.16×** | 0.60× |
| jfk3 | 3851 | 21342 | **0.18×** | 0.70× |
| gb1 | 17762 | 35257 | **0.50×** | 3.01× |

**das now leads NeMo on every canary cell** — gb1 went 125.4 s → 17.8 s (7.1×) across
Phases 1-3, xRT 11.2.

### Phase 6 interlude — the M4 mint saga (2026-08-03, all archived in ~/.tune-history)

M4's first-ever hardened mint (triggered by the q40→k40 family rename breaking --quick
inherit) refused SEVEN times — and every refusal was correct. The trail: box proven quiet
(probes cv 0.28-0.72%, movers ~3% of a core); data proven deterministic (sin/cos fills, int8
dot); geometry pinning (64B, then 1MB/4MB) changed nothing — because the flip line ran
exactly synthetic-vs-real-shape: every tile-gen family racing REAL shapes validated at 0.00%,
every synthetic N=4096 micro flipped between near-tied variants per context. Fix: the three
flippers (dot, dot_q8q8, quantize_q8_0_into_ptr) race IN SITU — streaming GEMV row walks
past L2 + a batch requant image. The mint then shipped (paranoid, noise=ok, 509 s tune) and
CHANGED a winner: quantize plain → vec8 — the synthetic hot-L1 loop had crowned the wrong
variant for the streaming regime production runs in. Secondary finds, both fixed: the noise
retry window (2s → 8s — composites outlast it) and the daspkg standalone's null fn-pointer
globals (function values don't survive bake serialization; dlim source defaults now
boot-restore under [init] null guards — M4's entire first standalone sweep died on the
unhealed vulkan seam that M1's untracked dasVulkan checkout masked). Plus the short-clip
slab-dispatch clamp (jobs ≤ blocks; jfk +23 ms found by the same-box before/after).
Ledgered for daspkg: ship-source vs DAS_TUNE_MANIFEST mismatch; PATH-dependent host compiler
(brew clang 22 on m1 vs Apple clang-21 on m4). Boris's verdict, kept: "failure to mint
beats failure to profile" — the shipped numbers inherit credibility from the refusals.

### Phase 5 — memory census (M1, `mem_census.sh`, 2026-08-03)

| cell | RSS GB | footprint GB | ms |
|---|---:|---:|---:|
| canary warm jfk | 6.20 | 2.71 | 1302 |
| canary warm gb1 | 7.78 | **4.29** | 17675 |
| canary warm hp0x2 (first ever) | 11.84 | 8.35 | 154171 |
| canary MINT gb1 | 8.12 | 4.62 | 20401 |
| parakeet v3 warm hp0x2 | 6.37 | 5.61 | 25494 |

gb1 serve 13.11 → ~4.3 GB; mint spike +1.33 → +0.33 GB; hp0x2 canary fits a 16 GB box.
Ledgered remainder: the hp0x2 ~4 GB decoder-side prefill/KV tail (map before chasing).
Method note: this ladder wraps `asr_bench` (not the rig exe the 08-02 baseline used) — the
before/after deltas quoted are structural (weights + scratch), not rig-exact.

### Canary honest cap (M1, 2026-08-03)

The runaway class adjudicated (Boris): the model's limitation — trained envelope 40 s audio /
1024 total tokens (model card) — so we stop forcing it. Decode budget `CN_MAX_NEW = 256l`
mirrors the reference arm's `--max-new-tokens 256` (uncapped, das timed runaway length: the
committed hp0 row read 292.7 s das vs 99.3 s nemo BECAUSE das decoded 4.7k tokens of loop
against nemo's 256 — hp0 > hp0x2 precisely because budget = seq_len − npos). Spec ladder caps
at gb1; the void m1 hp0/hp0x2 rows (both engines) dropped from the store; site + results.md
regenerated. Gates: oracle token-for-token UNCHANGED with the cap (13.7 s, all three fixture
clips); jfk/jfk3/gb1 transcripts byte-identical (asr_bench --text diff vs expected).
gb1 remains past-envelope (199 s ≈ 2.5k audio tokens): M1 decodes it clean, M4 tips into a
"01.01…" loop at a greedy near-tie — the M4 cell records for the eyeball, stays unpublished
('incompetent' class) unless the re-released exe's new winners flip it back.

### Phase 7 — zen4 bringup (2026-08-03, fresh Debian 12, `ssh zen4`)

The first fresh-box execution of BRINGUP end-to-end: Ryzen 7 PRO 8700GE (Zen4 8C/16T 35 W,
full AVX-512 incl. vnni+bf16), clang-19 + Ninja + Release + release-lane modules, ~10 min
clean build, JIT green first try. fetch_models: 14/14 registry files (149 GB, ~35 min) +
all 4 conversions reproduced ON x86 — parakeet v2/v3 f32 and canary encoder **sha-identical
to the M1 bytes** (the repack-canonical claim now proven cross-OS/arch), decoder differs
per-arch as declared (transcript parity gate still owed — needs a das inference pass).
Corpus 5/5 after the gb1 fix + M1 wav copies (zen4 ffmpeg decodes ogg to different bytes —
the manifest gate caught it, exactly its job). ASR rig COMPLETE (NeMo-on-Linux-x86 venv came
up clean); lcpp reference pair built @ ebd048f. **Paranoid mint WORKED first try**: 561.6 s
tune, noise=ok, box identity correct, validation passed, sidecar + ~/.tune-history archive +
provenance all present; release total 720.3 s. First AVX-512 race: 30 kernels, **11 crowns
differ from the per-ISA fallbacks** (dot→vec16_u2, dot_f16→vec16_u4, dot_q8q8→u2,
quantize_q8_0→vec32_u2, rmsnorm→vec16 …) — the wider vectors win on real AVX-512, exactly
why per-ISA fallbacks alone were never enough.

Tree fixes landed en route (each caught live by the bringup): setup_asr_rig auto-mint →
tune_policy fallback (05956dca7); gb1 corpus fetch (9a5d6a5d0); gen_site_records policy +
the sweep correction (c26bc9a00 + 17ec8bc39 — annotation needs a DIRECT llvm_tune require;
setup_lcpp_ref has no dasllama graph and needed nothing).

Ledgered from this phase:
- fetch_models prints conversion RECIPES but nothing executes them — the on-box driver
  (`~/zen4_convert_asr.sh` on zen4) should become a `--convert` arm.
- convert-parakeet-to-ggml.py defaults to `ggml-model-f32.bin` — v3 silently overwrote v2
  until `--out-name`; its usage header also says `--output-dir` for what argparse calls
  `--out-dir` (upstream whisper.cpp nit).
- Fresh-Debian-12 build seams (documented, env-side): distro clang-14 lacks
  `-Wno-unqualified-std-cast-call`; gcc-12 libstdc++ + clang-19 -Werror needs
  `-DCMAKE_CXX_FLAGS=-Wno-deprecated-declarations`.
- Box still owed: canary decoder transcript parity, image pre-bake + board sweep (needs the
  DAS_TUNE_MANIFEST/one-tune-per-box decision for the converter vs the release sidecar).

Phase 7 addendum — sanity floor (same evening): E4B q8 cell at 8 lanes, cv 0.2%: das
pp512 249.9 vs clean-cpu llama.cpp 97.2 = **2.57×** (ref confirmed -march=native ⇒ AVX-512;
clean-cpu ≡ stock on Linux), tg128 11.88 vs 11.14 = 1.07× (both engines at the DDR5 wall).
Add the stock leg + a second model before quoting publicly. Canary decoder parity gate
**PASSED byte-identical** (jfk/jfk3/gb1, encoder via --mmproj) — all 18 artifacts now fully
accepted on zen4. Fixed en route (f9deddf13): box-profile "threads" recorded WORKER count
into a total-lanes field and the loader advised exporting the off-by-one value — now a
receipt (workers+1), no override advice. zen4's minted sidecar still carries the inert 7;
heals at next re-mint.

Phase 7 canary pair (zen4, best-of-3, cv <1%): das 1.64/5.15/21.0 s vs NeMo(16 thr)
5.50/17.29/47.98 s on jfk/jfk3/gb1 = das/nemo 0.30/0.30/0.44 — ratio family matches the
Apple boards (M4 0.21/0.28/0.78), long-clip position stronger than M4. das 6.4-9.4× realtime.

Phase 7 audio-chat finding (Boris's eyeball): zen4 E2B/Omni leads are only 1.14-1.38× vs
Apple's 3.5-6× — das audio-chat is ISO-SPEED M1↔zen4 (E2B gb1 45.4 vs 47.7 s) while its LLM
side is FASTER on zen4, so the cells are tower-dominated and the das tower gets nothing from
AVX-512; meanwhile ggml's x86 tower is ~3× its own ARM tower (ref E2B gb1 57.8 vs 198.2 s),
removing the subsidy the Apple leads enjoyed. LEDGER: per-op profile one audio-chat cell on
zen4, name the tower kernels without x86 arms — same work-class as the gemma-26B x86 gap.
