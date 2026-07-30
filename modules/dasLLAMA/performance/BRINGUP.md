# Box bring-up — the profiling rig from zero

## The visibility contract

Every tool in this rig narrates every stage on **stdout**: which model, which leg, which clip,
which conversion stage, live as it happens — the orchestrator streams its children's output
through rather than buffering it. Reading the stdout alone must always answer "how far along is
it and what is it doing right now."

Consequences:
- **Never launch a rig tool through `| tail`, `| grep`, `| head` or any pipe that truncates or
  delays the stream.** A run started that way gets restarted. Backgrounding is fine — redirect
  the WHOLE stdout to a file (`> sweep.log 2>&1`) and read the file; the stream is the record.
- A tool that goes silent for minutes is a visibility BUG — fix the tool, don't wrap it in
  polling.

The checklist a fresh box (rented or owned) follows to produce the full record board — LLM and
ASR — with receipts complete enough to publish. Every step is scripted; if a step needs
laptop-local knowledge, that is a bug in this file (the zen2 pilot is the enforcement run).

The published methodology lives in `modules/dasLLAMA/METHODOLOGY.md`; this file is the *doing*
side. One rule from it matters here: **a das number never ships without its same-session
reference pair**, so the reference engines are not optional.

## 0. Prerequisites

- git, cmake, a C++ toolchain, **CPython 3.10–3.12** (the venv pins were frozen on 3.11;
  3.13+ breaks them — the numpy pin has no cp313 wheel and pip falls into a Meson source
  build; a fresh mac's CLT python is 3.9 — no-admin route:
  `curl -LsSf https://astral.sh/uv/install.sh | sh`, `uv python install 3.11`; setup_asr_rig
  probes PATH names first, then asks uv itself), curl, tar, ffmpeg (corpus
  conversion — or copy the converted corpus from a manifest-clean box), ~30 GB free beyond the
  models.
- Windows boxes: run everything from a shell where `cmake`/`git` resolve; the rig handles
  SMT masking itself.
- Set the box identity FIRST — every store filename derives from it:

```sh
export DASLLAMA_BOX=<box>          # e.g. zen3a — short, stable, lowercase
export DAS_TUNE_MANIFEST=modules/dasLLAMA/performance/$DASLLAMA_BOX.tune.json
```

`DAS_TUNE_MANIFEST` is the **one-tune-per-box** rule: every rig process (cells, converter
pre-bakes) stamps winners from this single manifest, so their `.dlim` image identities agree
and a pre-baked image serves every consumer. Without it each script mints its own sidecar and
near-tie winners flip between mints — observed to fork the identity (`q51 mr4` vs `mr8`),
which makes every cell silently re-bake its own duplicate images. The orchestrator sets the
env itself when unset, but the converter runs in step 4 need it exported in the shell.

**Mint the tune BEFORE the pre-bake, and re-mint after ANY daslang rebuild.** The manifest is
mtime-gated against the binary: a rebuild silently turns it stale and every kernel drops to
its fallback winner — the m4 pilot pre-baked one identity, then swept a whole board on
fallback kernels under a manifest one rebuild older. The bench cells now REFUSE to measure on
non-manifest winners (`tune gate`; `DASLLAMA_ALLOW_UNTUNED=1` is the dev-run escape). The
auto-tune policy does NOT mint into an env-pinned manifest — mint explicitly (~5 min):

```sh
DAS_TUNE_MODE=tune bin/daslang -jit modules/dasLLAMA/harness/dasllama_tuner.das -dasroot <repo>
```

Order: build → mint → pre-bake → sweep.

## 1. daslang

```sh
git clone <daScript remote> && cd daScript
cmake --build build --config Release -j 16        # 15-25 min clean
```

`bin/daslang` (or `build/daslang`) must run `-jit`. `export DASLANG_BIN=<path>` if it is not
`bin/daslang` relative to the repo root.

## 2. Models

- **LLM**: the public catalog ggufs (see `pub_catalog()` in `gen_bench_records.das`) into a
  models dir; `export DASLLAMA_MODELS_DIR=<dir>`. Reuse an existing models dir when the box has
  one — do NOT re-download tens of GB (fetch script: `plans/m4_fetch_models.sh` pattern).
- **The same filename exists in multiple HF repos with different bytes** — lmstudio's and
  unsloth's `Mistral-Small-...-Q4_K_M.gguf` differ (416 bytes of metadata, different sha).
  The `.sha` sidecar names the exact build; a fetch that fails the sidecar gate benches a
  DIFFERENT file, not a re-download glitch — find the right repo, never waive the gate.
- **ASR**: whisper/parakeet ggml carriers into `$WHISPER_CPP/models` (`WHISPER_CPP_MODELS`
  overrides — pointing it at the LLM models dir keeps ONE dir per box); the audio-chat ggufs +
  mmproj files live with the LLM models. Absent models skip with a warning — partial boards
  are fine, silent substitutions are not. Provenance (verify sha against an existing box's
  `.sha` sidecars after every fetch):
  - `ggerganov/whisper.cpp` → `ggml-tiny.bin`, `ggml-large-v3-turbo.bin`
  - `ggml-org/gemma-4-E2B-it-GGUF` → `gemma-4-E2B-it-Q8_0.gguf`,
    `mmproj-gemma-4-E2B-it-BF16.gguf` (rename to the catalog's lowercase `-bf16`)
  - `ggml-org/Qwen3-Omni-30B-A3B-Instruct-GGUF` → `Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf`,
    `mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf`
  - the parakeet v2/v3 f32 bins and the canary encoder/decoder are CONVERSIONS from public
    NVIDIA checkpoints, and every box CONVERTS THEM ITSELF (never transfer model bins between
    boxes — box-to-box links are slow relays and the recipe is the provenance): parakeet via
    the pinned whisper.cpp checkout's `models/convert-parakeet-to-ggml.py --use-f32` over the
    `nvidia/parakeet-tdt-0.6b-v2`/`-v3` `.nemo` files; canary via
    `modules/dasLLAMA/harness/convert_canary_to_ggml.py` over `nvidia/canary-qwen-2.5b`
    (both run in the nemo venv setup_asr_rig creates). Acceptance gates differ by what the
    conversion does:
    - **parakeet + canary ENCODER** are pure repacks (zero arithmetic) — they byte-match
      across every OS/arch/python; the bins MUST sha-match the recorded sidecars, and a
      differing hash means environment drift: stop and compare, never bench it.
    - **canary DECODER** does real math (the torch LoRA merge, fp16) — ARM and x86 torch
      round the last ulp differently, and the gguf writer stamps tool versions, so its
      byte hash is canonical only per (architecture, convert-kit version). The gate that
      matters is OUTPUT parity: transcribe the three canary corpus clips
      (`asr_bench.das --text` over jfk/jfk3/gb1) and diff token-for-token against
      `benchmarks/asr/canary_transcripts.expected` — proven identical across an
      ARM-converted and an x86-converted decoder on 2026-07-29. Bit-identity across
      arches was never the goal for LLM/ASR outputs (you cannot attribute a bit of drift
      to model bit reduction vs cache reduction vs speculative decoding without massive
      effort); token-for-token output parity is.

## 3. Reference engines

LLM references (two llama-bench builds):

```sh
# --src = any llama.cpp checkout; the pinned worktrees + builds are created NEXT to it
bin/daslang -jit modules/dasLLAMA/benchmarks/setup_lcpp_ref.das -- --src <llama.cpp> -j 16
export LLAMA_BENCH_CLEAN=<...>/build-clean-cpu/bin/llama-bench
export LLAMA_BENCH_STOCK=<...>/build-stock/bin/llama-bench
```

ASR references + corpus (one tool, all legs):

```sh
bin/daslang modules/dasLLAMA/performance/setup_asr_rig.das -- --refs --venvs --corpus --check
# exports it prints: WHISPER_CPP, MTMD_BIN, ONNX_PY, NEMO_PY
```

`--check` must end `COMPLETE` with **no DIRTY identity** before any sweep. The corpus manifest
(`benchmarks/asr/corpus.sha256`) is a hard gate: a sha mismatch means this box would bench
different bytes — fix it (usually the box's ffmpeg decoded hp0 differently: copy `hp0.wav` +
`hp0x2.wav` from a manifest-clean box), never waive it. `--libri` adds the 25-clip LibriSpeech
set (Parakeet-v3 dictation stats; ~350 MB one-time fetch).

## 4. Pre-bake the images

Sweeps run on pre-baked `.dlim` images — the converter streams the transcode (far lower peak
memory than an in-load conversion), and every das cell then maps instead of converting, which
removes the cold-map variance the tripwire otherwise fights. Bake AFTER the first tuned run
exists (image identity is box- and knob-specific; the converter applies the box profile itself):

```sh
# GC is AUTOMATIC: every image save sweeps its source's version-stale siblings, and
# gen_bench_records GCs the models dirs at session start (one box carried 748 GB of stale
# identities; another 274 GB). The manual form remains for ad-hoc checks — and ALWAYS check
# space before a big bake: a 22 GB write once died mid-file on the ceiling
bin/daslang -jit utils/dasllama-convert/main.das -- -m <models-dir> --clean --apply
df -h <models-dir>

# smoke ONE small and ONE big model end-to-end before committing to the batch — the two
# failure size-classes are different (a tiny model never exercises the >2 GiB plane paths)
for m in <models-dir>/*.gguf; do
    bin/daslang -jit utils/dasllama-convert/main.das -- -m "$m"           # planar (CPU cells)
done
# Apple boxes additionally: -f metal for the gpu cells (some configs legitimately have no
# metal-servable blob flavor — the converter says so and exits nonzero; that is a decline,
# not a failure). Already-valid images are a no-op (~10 s each, load-and-verify).
```

The AUDIO models ride the same rail — the converter sniffs the family (whisper/parakeet ggml
bins, the CNRY canary encoder, audio mmproj GGUFs) and bakes the family-tagged image; ASR
decoder GGUFs are plain decoder bakes (canary's is fp32 — the parity-pinned exception):

```sh
for m in $WHISPER_CPP/models/ggml-tiny.bin $WHISPER_CPP/models/ggml-large-v3-turbo.bin \
         $WHISPER_CPP/models/ggml-parakeet-tdt-0.6b-v*-f32.bin \
         <models-dir>/canary-qwen-2.5b-encoder-f32.bin \
         <models-dir>/mmproj-gemma-4-E2B-it-bf16.gguf <models-dir>/gemma-4-E2B-it-Q8_0.gguf \
         <models-dir>/mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf \
         <models-dir>/Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf; do
    bin/daslang -jit utils/dasllama-convert/main.das -- -m "$m"
done
bin/daslang -jit utils/dasllama-convert/main.das -- -m <models-dir>/canary-qwen-2.5b-decoder-f16.gguf -q fp32
# the GC step covers the whisper models dir too:
bin/daslang -jit utils/dasllama-convert/main.das -- -m $WHISPER_CPP/models --clean --apply
```

Expected wall-clock (M1 Max, NVMe ~6 GB/s image writes, catalog of 8): a FULL re-bake of both
flavors is **~5–6 minutes total**. Per model: bake time is dominated by reading + transcoding
the GGUF, not the write — the 22 GB 35B ≈ 19 s planar / 10 s metal, the 16–19 GB class ≈ 7–15 s
per flavor, the 7–8 GB class ≈ 4–7 s. A re-run over valid images completes in ~2 minutes of
verify-and-skip. The ASR set adds **~1 minute** (measured M1: the 31 GB Omni decoder 40 s —
34 s eager load + 5 s write at 6 GB/s; every other file 0.2–5 s) and removes the eager loads
from EVERY subsequent `--workload asr` sweep, which then maps all seven models in milliseconds.

## 5. Sweep

```sh
# tune happens automatically on first run ([tune_policy missing=auto]); Parsec/remote-desktop OFF
bin/daslang modules/dasLLAMA/performance/gen_bench_records.das -- --workload all
```

- Thread policy is automatic: `min(16, physical performance cores)`, both engines, pinned on
  x86. Intel hybrid boxes fail loudly until P/E pinning exists — do not work around the panic.
- One model process at a time, das cell and its reference adjacent, 12 s settle between passes —
  the driver owns all of it. Wall-clock: hours; run under a keep-awake and off-hours.
- The store (`performance/records/<box>.json`) persists after every cell — a crash costs at
  most the in-flight cell. Re-runs upsert in place.

### Oracle mode — the tables as a regression tripwire

```sh
# per refactor step on this box (das-only, minutes): re-verify the stored metal rows
bin/daslang modules/dasLLAMA/performance/gen_bench_records.das -- --oracle --legs metal
```

`--oracle` inverts the sweep into a pure check: the work list is the STORE's das LLM rows for
this box (a stored row whose model or `.dlim` is gone is a loud FAIL, not a skip), each row
re-measures ONCE and gates one-sided against its stored mean — drop past `--oracle-fail`
(default 5%) fails, past `--oracle-warn` (default 3%) warns, gains report (flagged past the
fail bar as "suspicious — verify"). Exit is nonzero on any FAIL.

- GATE 1 — llama.cpp never re-measures: no ref runs, ref binaries not even required.
- GATE 2 — one das pass per row. The >3% cv warm-retry stays: it REPLACES a bad cold measure.
- GATE 3 — frozen artifacts: the child runs `lcpp_bench --frozen` (a missing `.dlim` panics
  instead of minting), step-zero GC is skipped, and the store is never written.
- Default is stop-at-first-FAIL (fail fast mid-refactor); `--oracle-keep-going` runs the full
  board. `-o substr` narrows to one model; ASR legs are excluded (their das cells are CPU-path).
- The tune gate applies unchanged: a manifest older than the binary fails every cell — re-mint
  (`DAS_TUNE_MODE=tune DAS_TUNE_MANIFEST=<box manifest> bin/daslang -jit
  modules/dasLLAMA/harness/dasllama_tuner.das`) and check the fresh winners against the stored
  rows' `tune` stamps before trusting deltas.
- ⚠ A PARITY_FULL family-suite run PURGES the bench-flavor images: the suite saves its own
  flavor (test-config identity hash) and the image GC removes every other sibling `.dlim` as
  dead — the next oracle leg then FAILs those models with "cell did not measure" (the frozen
  child refuses to re-mint, by design). Observed 2026-07-30: `--arm fam-gemma4` freed 34 GB of
  "dead siblings" and blinded all three gemma cells. Re-mint with a minimal unfrozen run per
  model (`lcpp_bench -- -m <gguf> --ngl 99 -p 16 -n 4 -r 1`), then re-run the cells with
  `-o <substr>`. The GC-vs-foreign-flavor policy itself is an open design question.
- **A verdict on a long board's tail cells is not evidence — re-run the cell solo.** Three
  times observed on the m1 (2026-07-30): the last cell of an 8-cell keep-going sweep under-read
  −6.5% pp (solo re-run: dead-on the store); the last model of the hours-long Jul-28 rig
  sweep stored tg absolutes ~20-25% low on BOTH engines (the adjacent pairing kept the ratio
  honest; the oracle later read the das side as a +20% "suspicious gain"); and the M1-refactor
  gate's 8th cell read −34% RELATIVE TO TRUE (a hard oracle FAIL at −15/−21% vs store — the
  solo re-run landed dead-on the same morning's pre-refactor fresh reads). The phantom can
  exceed the fail bar by 4×; a tail FAIL is a re-run instruction, never a verdict.
  Discriminating an
  anomaly: probe the stored row's `sha` (code), the `.dlim` mtimes (artifacts), the `tune`
  stamps, and re-run the stored `cmd` of the ADJACENT ref on a quiet chip — if the ref moved
  too, the state was environmental and only the absolutes are stale.

## 5. Publish

```sh
bin/daslang dastest/dastest.das -- --test modules/dasLLAMA/tests/test_bench_records_schema.das
bin/daslang modules/dasLLAMA/performance/gen_site_records.das
```

Commit `performance/records/<box>.json` + `site/files/dasllama/bench_records.json` together.
Anomalies get an entry in `performance/records/annotations.json` — explain, never delete a
measured row. Check the rendered site locally before pushing (site/README.md).
