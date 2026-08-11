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

**This file is validated by execution, not by reading.** Every claim below either was run on a
box or is marked as untested — a step that turns out to be wrong during a bring-up is fixed here
in that session, from the transcript. The M1 run of 2026-08-02 is the most recent such pass.

The published methodology lives in `modules/dasLLAMA/METHODOLOGY.md`; this file is the *doing*
side. One rule from it matters here: **a das number never ships without its same-session
reference pair**, so the reference engines are not optional.

## The bring-up sequence

The tuner is the detector, so the human never has to be. For a box with existing data:

1. **Delete every `.dlim`** under the models dirs — the fresh generation re-bakes identities.
2. **Mint PARANOID as part of building the exe**:
   ```sh
   bin/daslang utils/daspkg/main.das -- release --root modules/dasLLAMA/benchmarks \
     --out modules/dasLLAMA/performance/_rig --paranoid
   ```
   Release ALWAYS mints (`--quick` is the sole inherit path, for later session iteration).
   The mint refuses a noisy box (probe cv over the gate — quiet it and re-run; nothing was
   written), self-validates (a heavy subset re-races and the winners must reproduce), and runs the
   e2e confirm for divergent GEMM crowns (every platform; zen4-verified). The confirm model
   auto-resolves from the models dir (the preferred carrier, else the largest present q8 gguf;
   `DASLLAMA_CONFIRM_MODEL=<full path>` overrides) — the fallback pins only on a box with no
   q8 model at all, which the standard fetch order makes unreachable.
   The previous sidecar snapshots to `.bak`, the DIFF prints, and the mint archives to
   `~/.tune-history/<box>/` (failures too, marked). **Review the DIFF**: uniform time shift =
   box state; scattered past-floor flips = one of the mints was noisy; same-direction twin
   flips = an estimator change.
   Measured on the M1 (2026-08-02): 886 s total — 72 s build, 732 s paranoid tune, 73 s rebuild.
3. **Run the oracle set — LLM and audio — plus one big known-good model on CPU** (section 5's
   oracle mode). The board owns its image lifecycle (wipe at start, bake per cell, delete after
   each model — `PROFILE.md`), so no pre-bake step precedes it; section 4 stays for converter
   workflows. Human + AI review of the board; anything out of the ordinary = stop and discuss.
   A FAIL after a re-mint is either a real regression or a generation change — the crossgen
   gate names which.

   **Any dasLLAMA source edit between here and the sweep invalidates the exe** — it bakes both
   the sources and the tune winners, so the rig refuses a stale one and prints the rebuild line.
   Re-release with `--quick` (inherits the complete sidecar, ~85 s) and carry on; the paranoid
   mint is not repeated for a code edit.
4. **New box only:** the same big model vs llama.cpp (section 3 references) — when the
   absolutes have no history, the RATIO is the known quantity.

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
and a pre-baked image serves every consumer.

**Scope, and it matters: export it for the CONVERTER, never for the board rigs.** The converter
must bake under the same winners the exe carries, so it needs the manifest. The board rigs
(`gen_bench_records`) measure through the released exe, which carries its own baked winners and
actively clears an inherited value — see `PROFILE.md`. Setting it there pins the ORCHESTRATOR to
different winners than its cells use; the two disagree about every image identity. Without it each script mints its own sidecar and
near-tie winners flip between mints — observed to fork the identity (`q51 mr4` vs `mr8`),
which makes every cell silently re-bake its own duplicate images. The orchestrator sets the
env itself when unset, but the converter runs in step 4 need it exported in the shell.

**Mint the tune BEFORE the pre-bake, and re-mint after ANY daslang rebuild.** The manifest is
mtime-gated against the binary: a rebuild silently turns it stale and every kernel drops to
its fallback winner — the m4 pilot pre-baked one identity, then swept a whole board on
fallback kernels under a manifest one rebuild older. The bench cells REFUSE to measure on
non-manifest winners (`tune gate`; `DASLLAMA_ALLOW_UNTUNED=1` is the dev-run escape — it also
suppresses the auto-mint, so an untuned box runs fallback stamps instead of tuning first).

Minting goes through the hardened wrapper — noise gates at start/mid/end (a refusal exits
nonzero and writes NOTHING), median ranking with deterministic tie-breaks inside the noise
floor, a validation re-race (winners must reproduce or the mint fails), race tables +
provenance (noise verdict, mode, engine sha, box, date) in the sidecar, `.bak` + printed
DIFF on re-mint, and an archive in `~/.tune-history/<box>/`. Two budgets only: normal
(minutes) and `--tune-paranoid` (~3x, 1% gate) — there is no fast race. Direct form when
not going through `daspkg release`:

```sh
DAS_TUNE_MODE=tune bin/daslang modules/dasLLAMA/harness/dasllama_tuner.das -dasroot <repo> [-- --tune-paranoid]
```

Order: build → mint → sweep (the board bakes its own images; pre-bake is for §4 converter
workflows).

## 1. daslang

```sh
git clone <daScript remote> && cd daScript
cmake --build build --config Release -j 16        # 15-25 min clean
```

`bin/daslang` (or `build/daslang`) must run `-jit`. `export DASLANG_BIN=<path>` if it is not
`bin/daslang` relative to the repo root.

## 2. Models

- **One tool provisions and gates the whole set** — `models_provenance()` in
  `performance/fetch_models.das` is the committed manifest: per file, the exact HF repo +
  revision pin, canonical bytes + sha256, and the conversion recipe where no registry serves
  the file. Verify is the default; `--fetch` downloads what is absent (resumable curl):

```sh
export DASLLAMA_MODELS_DIR=<dir>       # WHISPER_CPP_MODELS too, or point both at ONE dir
bin/daslang modules/dasLLAMA/performance/fetch_models.das -- --fetch
# after section 3's setup_asr_rig (the nemo venv + refs): the conversions, tool-driven
bin/daslang modules/dasLLAMA/performance/fetch_models.das -- --convert
bin/daslang modules/dasLLAMA/performance/fetch_models.das --   # later: verify-only, must end 0 failed
```

- **The same filename exists in multiple HF repos with different bytes** — lmstudio's and
  unsloth's `Mistral-Small-...-Q4_K_M.gguf` differ (416 bytes of metadata, different sha), and
  upstream re-uploads change bytes under an unchanged name (gpt-oss is revision-pinned for
  exactly this). The manifest names the exact build; a mismatch means this box would bench a
  DIFFERENT file, not a re-download glitch — find the recorded repo/revision, never waive.
  Reuse an existing models dir when the box has one — verify is free, re-downloading is not.
- **ASR**: whisper/parakeet ggml carriers land in `$WHISPER_CPP/models` (`WHISPER_CPP_MODELS`
  overrides); the audio-chat ggufs + mmproj files live with the LLM models (the manifest
  carries the per-file root and the E2B mmproj's lowercase `-bf16` rename). Absent models skip
  with a warning at sweep time — partial boards are fine, silent substitutions are not.
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

## 4. Pre-bake the images (converter workflows — the board sweeps bake their own)

The board rigs wipe, bake, and delete images themselves (`PROFILE.md`, image lifecycle) — this
section is for CONVERTER workflows: parity probes, by-hand image work, serving a model outside
a sweep. The converter streams the transcode (far lower peak memory than an in-load
conversion), and a das consumer then maps instead of converting. Bake AFTER the first tuned run
exists (image identity is box- and knob-specific; the converter applies the box profile itself):

**Bake the CATALOG, not the directory.** `for m in <models-dir>/*.gguf` bakes every file
present — on a working box that is the whole model zoo (81 files / 860 GB of images here on
2026-08-02, against a catalog of 8), and tokenizer fixtures (`ggml-vocab-*.gguf`) report as
bake failures because they are not models. List the catalog models explicitly, or filter.

```sh
# GC is AUTOMATIC: a save sweeps its own lane's dead siblings (and BROKEN/version-stale in any
# lane); images from ANOTHER bake context — a GPU flavor, a family tag this process does not
# register — are FOREIGN and kept, because their identities cannot be recomputed here. The
# manual form remains for ad-hoc checks — and ALWAYS check space before a big bake: a 22 GB
# write once died mid-file on the ceiling
bin/daslang -jit utils/dasllama-convert/main.das -- -m <models-dir> --clean --apply
df -h <models-dir>

# smoke ONE small and ONE big model end-to-end before committing to the batch — the two
# failure size-classes are different (a tiny model never exercises the >2 GiB plane paths)
for m in <the catalog models>; do
    bin/daslang -jit utils/dasllama-convert/main.das -- -m "$m"           # planar (CPU cells)
done
# Apple boxes additionally: -f metal for the gpu cells (some configs legitimately have no
# metal-servable blob flavor — the converter says so and exits nonzero; that is a decline,
# not a failure). Already-valid images are a no-op (~10 s each, load-and-verify).
```

The AUDIO models ride the same rail — the converter sniffs the family (whisper/parakeet ggml
bins, the CNRY canary encoder, audio mmproj GGUFs) and bakes the family-tagged image; ASR
decoder GGUFs are plain q8 decoder bakes (canary's included — the fp32 parity arm bakes
separately with `-q fp32` when the oracle lane needs it):

```sh
for m in $WHISPER_CPP/models/ggml-tiny.bin $WHISPER_CPP/models/ggml-large-v3-turbo.bin \
         $WHISPER_CPP/models/ggml-parakeet-tdt-0.6b-v*-f32.bin \
         <models-dir>/canary-qwen-2.5b-encoder-f32.bin \
         <models-dir>/mmproj-gemma-4-E2B-it-bf16.gguf <models-dir>/gemma-4-E2B-it-Q8_0.gguf \
         <models-dir>/mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf \
         <models-dir>/Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf; do
    bin/daslang -jit utils/dasllama-convert/main.das -- -m "$m"
done
bin/daslang -jit utils/dasllama-convert/main.das -- -m <models-dir>/canary-qwen-2.5b-decoder-f16.gguf
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
# the tune generation already exists (the release minted it); Parsec/remote-desktop OFF stays
# good hygiene, but the noise gates now DETECT a loud box — a mint that succeeded is a mint
# that measured, and the sidecar's provenance says under what conditions
bin/daslang modules/dasLLAMA/performance/gen_bench_records.das -- --workload all
```

- Thread policy is automatic: `min(16, physical performance cores)`, both engines, pinned on
  x86. Intel hybrid boxes fail loudly until P/E pinning exists — do not work around the panic.
- One model process at a time, das cell and its reference adjacent, 12 s settle between passes,
  180 s cool slot (`--das-settle`) before every das cell — the driver owns all of it. das tuned
  kernels run near the package power ceiling and under-read with a clean cv on a heat-soaked
  box; the references are insensitive. Wall-clock: hours; run under a keep-awake and off-hours.
- On a laptop, sleep the display first (`pmset displaysleepnow` on macOS): a lit screen with an
  animated wallpaper is a standing WindowServer tax on CPU and GPU alike.
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
- GATE 3 — the timed cell is frozen: the batch starts with the lifecycle wipe, a prepare pass
  bakes each cell's image, and the timed child runs `lcpp_bench --frozen` (it never converts);
  the store is never written.
- Default is stop-at-first-FAIL (fail fast mid-refactor); `--oracle-keep-going` runs the full
  board. `-o substr` narrows to one model; ASR legs are excluded (their das cells are CPU-path).
- A FAIL auto-triggers ONE solo re-run of that cell after `--oracle-retry-settle` seconds
  (default 180 — thermal recovery takes ~3 minutes; 0 disables), and the RETRY verdict stands —
  the board shows both attempts. This
  is the tail-cell discipline (below) as tool behavior; a FAIL that survives its solo retry is
  a real regression.
- The tune gate applies unchanged: a manifest older than the binary fails every cell — re-mint
  (`DAS_TUNE_MODE=tune DAS_TUNE_MANIFEST=<box manifest> bin/daslang
  modules/dasLLAMA/harness/dasllama_tuner.das`) and check the fresh winners against the stored
  rows' `tune` stamps before trusting deltas.
- **Sidecar generations**: every das row records `tune_sha` (the sidecar's shasum) and the
  records dir archives each generation's full doc once (`records/<box>.tune.<sha12>.json`).
  A stored row from a DIFFERENT generation is `INCOMPARABLE` — the stored mean belongs to
  different winners — and fails the run by name; `--oracle-allow-crossgen` forces the compare,
  legacy rows (no `tune_sha`) warn. After a deliberate re-mint, re-record the affected rows
  instead of arguing with the decline.
- ⚠ A missing `.dlim` under a "cell did not measure" FAIL is now a real failure, not the
  routine cross-identity reaping this note used to describe: a bake proves staleness only inside
  its own (quant, tag) lane, and images it cannot recompute are kept as FOREIGN. If one does go
  missing, re-bake it (`bin/daslang -jit utils/dasllama-convert/main.das -- -m <gguf>`) and
  re-run the cells with `-o <substr>`.
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
