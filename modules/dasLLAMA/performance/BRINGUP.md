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

- git, cmake, a C++ toolchain, python3, curl, tar, ffmpeg (corpus conversion), ~30 GB free
  beyond the models.
- Windows boxes: run everything from a shell where `cmake`/`git` resolve; the rig handles
  SMT masking itself.
- Set the box identity FIRST — every store filename derives from it:

```sh
export DASLLAMA_BOX=<box>          # e.g. zen3a — short, stable, lowercase
```

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
- **ASR**: whisper/parakeet ggml carriers into `$WHISPER_CPP/models` (`WHISPER_CPP_MODELS`
  overrides); the audio-chat ggufs + mmproj files live with the LLM models. Absent models skip
  with a warning — partial boards are fine, silent substitutions are not.

## 3. Reference engines

LLM references (two llama-bench builds):

```sh
bin/daslang modules/dasLLAMA/benchmarks/setup_lcpp_ref.das   # clean-cpu + stock builds
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
# STEP ZERO, every time: GC stale images and check space — this box once carried 748 GB of
# stale identities and a 22 GB bake died mid-write on the ceiling
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

Expected wall-clock (M1 Max, NVMe ~6 GB/s image writes, catalog of 8): a FULL re-bake of both
flavors is **~5–6 minutes total**. Per model: bake time is dominated by reading + transcoding
the GGUF, not the write — the 22 GB 35B ≈ 19 s planar / 10 s metal, the 16–19 GB class ≈ 7–15 s
per flavor, the 7–8 GB class ≈ 4–7 s. A re-run over valid images completes in ~2 minutes of
verify-and-skip.

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

## 5. Publish

```sh
bin/daslang dastest/dastest.das -- --test modules/dasLLAMA/tests/test_bench_records_schema.das
bin/daslang modules/dasLLAMA/performance/gen_site_records.das
```

Commit `performance/records/<box>.json` + `site/files/dasllama/bench_records.json` together.
Anomalies get an entry in `performance/records/annotations.json` — explain, never delete a
measured row. Check the rendered site locally before pushing (site/README.md).
