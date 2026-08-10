# dasLLAMA Code Review Checklist

Run this list on every dasLLAMA change before it ships — including changes to this file.

**What stays in this document:** criteria that can be checked against a diff. Nothing else.
A reader must be able to apply every entry below **without reading the code and without prior
knowledge of the module.** A rule may cite `ARCHITECTURE.md` for the reason behind it; it may
not require that section to be read before the criterion can be applied. If an entry needs
code-reading or prior knowledge, it is not a review criterion — move it to `ARCHITECTURE.md`
and leave a one-line criterion here.

**This file reviews itself: a rule a reviewer cannot apply as written is a defect of this
file.** Mark it like any other finding — a checklist defect blocks nothing, but its fix (a
rewrite or a move, never silent tolerance) lands in the same batch as the round's other fixes.

**Form, and it is a hard limit:**

- **One rule is one short paragraph.** An entry that needs more than that is describing how to
  write code, not how to review it. Split it or move it.
- **No numbers.** These are criteria, not a spec, and numbering invites citation. Anything that
  needs a stable reference lives in `ARCHITECTURE.md`, which is numbered for that purpose.
- **Cite files by name; cite `ARCHITECTURE.md` by section.** Never cite an entry in this file.
- **Name the API a rule is about; never name an example of it.** A rule governing specific
  functions or files must name them or it cannot be checked — that name is the criterion. An
  illustrative aside ("the way the X family does it") has no such excuse: nothing keeps it in
  sync with the code, and a stale example is worse than none.
- **One sentence of WHY is allowed where it makes the criterion decidable; anything longer
  belongs in `ARCHITECTURE.md`.** No history, no PR numbers, no direction of travel; planned
  work lives in the follow-up ledgers.

---

## Tests

**Run `modules/dasLLAMA/tests` before any PR.** The suites `run.das` lists
(`--suite decode|prefill|matrix|kernels|image|image-vulkan|coverage`) are invoked only through
`modules/dasLLAMA/tests/run.das`; dastest run directly on one of those is a defect. Every
other suite — the vulkan ones included — runs under dastest directly.

**Every test runs under `-jit`.** Never the interpreter, never AOT. A test invocation without
`-jit` is a defect even if it passes.

**dasLLAMA `[test]` files live under `modules/dasLLAMA/tests/`**, with one carve-out: a
benchmark self-check may sit beside the bench it verifies (`benchmarks/matmul/test_matmul_par.das`).
No dasLLAMA test is registered in any `CMakeLists.txt` — no AOT registration, no ctest wiring.

**A test passes or skips explicitly on every platform.** A skip goes through a capability or
model gate. A test that silently vanishes on one platform is a defect.

**A test that loads a model over 6 GiB sits behind `model_available`**, which runs it only
under `DASLLAMA_PARITY_FULL=1` — a final pre-PR gate, never the iteration loop. Check what a
test loads before launching it.

**Every new, moved, or extracted function or data transform ships a test for the bit itself** —
feed the function, check the bytes. "The model still runs" is not a test for a move. A
platform-fixed predicate has no bytes to feed — test its observable (the argv it gates, the
mode it selects) on the platform the test runs on.

**Every test that compares logits also logs decoded text for both sides.** A red, or a
suspicious green, must be readable as text in the log, not only as an id or float difference.

**A new GPU kernel ships with a small model in the kernel coverage suite** that dispatches it.

**A kernel-unit arm compares its kernel against a CPU oracle.** A GPU-vs-GPU arm is allowed
only where the property under test is cross-dispatch bit-identity that no CPU oracle can
witness — and then its output buffers are prefilled with a sentinel value before the runs.
Two dead dispatches compare equal; the sentinel is what turns that into a red. An IN-PLACE
output plane cannot hold both its input and a sentinel: pair its compare with a liveness
check that the plane actually changed.

**A harness that prints output for another tool to compare fails loudly when it has nothing
to print.** If the run ends without producing its comparison lines — wrong flags, failed
load, anything — the process returns a non-zero exit code. Two empty outputs diff clean; the
exit code is what keeps that from reading as a pass.

**A test arm's skip gate keys on a device capability or mode predicate (`coopmat_mode`,
`has_coopmat2`, `dn_shared_ok`), never on the existence of a runtime artifact such as a
pipeline or a cached set.** An artifact gate goes permanently false when its producer moves,
and the arm then skips forever while the suite stays green.

**A test suite loads models with `load_model_`, never the image rail.** Image-rail coverage
belongs to the image suites alone. See `ARCHITECTURE.md` §2.1.

**A new measuring entry point calls `tune_gate()` (`performance/profile_common.das`) before its
first timed rep.** A timed rep without the gate can measure fallback kernels silently. The
three worlds the gate covers are `ARCHITECTURE.md` §2.5. Kernel A/B labs are exempt: both arms
run under one tune state by construction, so the comparison holds untuned.

**No new record-grade timing harness is written.** Model-level time is measured by the rigs
`PROFILE.md` documents — `performance/gen_profile.das` (the routine check) and
`performance/gen_bench_records.das` (`--oracle` = the regression gate, bare = the publishing
board); both spawn `benchmarks/lcpp_bench.das`, the only thing that measures record-grade
time. A new model-timing harness, a one-off measurement script, or a revived rig is a defect;
the one sanctioned sub-model instrument is the kernel A/B lab (next rule). `PROFILE.md`
carries the commands; the rig's shape is `ARCHITECTURE.md` §2.5.

**A kernel A/B lab under `benchmarks/` is a decision instrument, never a record rig.** A lab
answers a kernel-join question by interleaved same-instrument A/B with a bit-exact cross-arm
gate; hand-rolled bind lists are legal there, which is exactly where binding rot hides — a lab
races LIVE production classes, so the change that renumbers a binding fixes or deletes the lab
arms it rots, in the same change. Two arms that compile identical kernel text measure noise:
when adoption closes a lab's question, its arm pair is deleted, not kept. Lab numbers never
enter the record stores or `PERF_LEDGER.md`.

**An out-of-process observer measures only what no in-process rig can observe about itself,
and its numbers are ledger-grade.** The one instance is `benchmarks/asr/mem_census.sh`: peak
memory via `/usr/bin/time -l` around a whole process. It stays macOS-only and its numbers live
in `PERF_LEDGER.md`, never in the record stores.

---

## Placement — one file, one rule

Every file states what it holds. Code that belongs to a file and is written anywhere else is a
defect, and this section is the whole test — a rule here names only its own file's contents,
never where a neighbouring concern lives. `ARCHITECTURE.md` §1 carries the boundaries and the
carve-outs.

**A new file under `dasllama/` ships with its rule here, its charter in `ARCHITECTURE.md` §1,
and its tests, in the same change.** A file without its records is a defect. A new file under
`tests/` registers in `tests/CLAUDE.md` (and in `tests/run.das` when it joins a suite) instead.

**A symbol the facade re-exports is required through `dasllama/dasllama` (or
`dasllama/dasllama_transformer`), never from the engine file that defines it.** An engine-level
symbol the facade does not re-export may be required directly. In-module tests, harnesses and
benchmarks may require internals — that is what they test. A split that adds facade-reachable
requires across the tree instead of fixing the facade re-export is a defect. Engine internals
may require each other.

**`dasllama_env.das` — every environment knob's single home.** All `[EnvConfig]` area structs
and their `g_env_*` globals live here; `ENVIRONMENT.md` generates from them, and
`tests/test_env_registry.das` enforces both directions. A knob declared anywhere else is a
defect.

**A new module file is registered in `.das_module` in the same change.** The install rule is a
directory glob; `CMakeLists.txt`'s `ADD_MODULE_DAS` list is a subset and is touched only when a
file joins it.

**`performance/fetch_models.das` is the model-provenance manifest and nothing else.** Per
catalog file: the exact HF repo + revision pin, canonical bytes + sha256, or the conversion
recipe where no registry serves the file. Verify is the default, `--fetch` downloads what is
absent; it never converts, never benches, never touches tune state
(`tune_policy(missing="fallback")`). Its gate is `fetch_models.das --` ending `0 failed` on a
provisioned box — BRINGUP.md §2 is the runbook.

### Engine

- `dasllama.das` — the public API surface and its re-exports.
- `dasllama_common.das` — engine types, forward loops, override registries, runtime knobs. No
  platform-specific code, and no load walk.
- `dasllama_load.das` — the GGUF load walk: metadata to `Config`, plane layout, format detection,
  the eager and streamed conversion ladders, and the load entry points.
- `dasllama_transformer.das` — block composition.
- `dasllama_blocks.das` — the std/dense/MoE block kernels and the default block sets. `forward()`
  reaches them only through the `ArchBlocks` pointers; a block kernel called by name from
  `dasllama_common.das` is a defect.
- `dasllama_moe.das` — MoE routing and expert dispatch, reached only through `moe_ffn_core`.
- `dasllama_attn_prefill.das` — prefill attention. `prefill_attention` is the only entry; a caller
  of a head kernel below it is a defect.
- `dasllama_batch.das` — the batched decode step.
- `dasllama_sampling.das` — sampling and the generation drivers; the engine never calls back in.
- `dasllama_ple.das` — gemma-4 per-layer embeddings and its MoE FFN, reached only through the
  hooks it registers.
- `dasllama_config.das` — every input that changes `.dlim` image bytes, and its identity
  formatter.
- `dasllama_chat.das` — conversation turns and chat-template application.
- `dasllama_par.das` — the parallel-for macro.
- `dasllama_prefix.das` — the prefix cache for evaluated token history.
- `dasllama_parity.das` — CPU reference caches for parity instruments.
- `dasllama_env.das` — the environment-knob registry and nothing else; `ENVIRONMENT.md`
  generates from it.

### Formats and data movement

- `dasllama_kqformat.das` — format identity: the format enum, per-format descriptors, format
  predicates.
- `dasllama_convert.das` — every tensor format conversion, any platform, any caller; also owns
  what the Q8_0/Q4_0 format IS (block geometry, scales) and the quant-quality detector.
- `dasllama_repack.das` — every disk-order to compute-order kernel-layout transform.
- `dasllama_kv_codec.das` — the KV-cache runtime codec: one family per cache format, kept whole.
- `dasllama_rope.das` — RoPE angle and table generation.
- `dasllama_gguf.das` — the GGUF container and its byte readers.
- `dasllama_layout.das` — disk-format to compute-layout transforms at load scope.
- `dasllama_image.das` — the prepared-model image rail.
- `dasllama_plane.das` — the borrowed-plane types and their accessors, and nothing else. A
  weight carrier holds planes; only this file knows how one is bound, read, or dropped.
- `dasllama_tokenizer.das` — the SentencePiece tokenizer.
- `dasllama_bpe.das` — the byte-level BPE tokenizer.
- `dasllama_unicode.das` — the transcoded unicode RANGES/WS tables and their lookups.

### CPU kernel tiers

- `dasllama_math.das` — the numeric abstraction: typedefs, active backend pointers, public
  wrappers, dispatch shaping (jobque caps and OS-conditional scheduling defaults). No kernel
  bodies.
- `dasllama_math_default.das` — the portable kernel backend.
- `dasllama_math_aarch64_neon.das` — the arm64 kernel backend.
- `dasllama_math_accelerate.das` — the Accelerate/BNNS float tier.
- `dasllama_math_gen.das` — the generated GEMM tier's runtime registration.
- `dasllama_gemm_gen.das` — the GEMM tile generator.
- `dasllama_gemm_schema.das` — the layout and permutation schema shared by generator and runtime.
- `dasllama_gemm_register.das` — tune-family registration.
- `dasllama_tune.das` — per-box kernel tuning policy. Tuned values live in the box sidecar, never
  in source.

### GPU

A backend is a family of role files, and the role names the contents. `<gpu>` is `metal` or
`vulkan`.

- the kernel home — kernel source, no device state: `dasllama_metal_kernels.das` (Metal),
  `dasllama_vulkan_classes.das` (Vulkan — every kernel a `[spirv_kernel]`/`[vk_dispatch]`
  class). The dispatch census tables and report are `<gpu>_common`'s.
- `dasllama_vulkan_dispatch.das` — the `[vk_dispatch]` structure macro: generates the
  `ensure_*`/`set_*`/`enc_*` surface, derives per-binding access, seeds the census. Macro code
  only; a kernel body or device call here is a defect.
- `dasllama_vulkan_seams.das` — the per-op class-rail seams (`vk_add_rms`, `vk_rope_kv_store`,
  `vk_decode_attn`) and nothing else.
- `dasllama_<gpu>_common.das` — device state, buffer and command plumbing, the hazard and capture
  rail, the profiler.
- `dasllama_<gpu>_decode.das` — the resident token-step driver and its decode-time arms.
- `dasllama_<gpu>_prefill.das` — the batched prefill driver and its batch arms.
- `dasllama_metal_shapes.das` — model-shape servability gates, portable: no GPU requires, so
  any box can bake. Vulkan has no shapes file (`ARCHITECTURE.md` §1.5).
- `dasllama_metal_lens.das` — the `[metal_dispatch]` structure macro: generates the `enc_*`
  builders, derives per-field access (`kernel=` picks the method; a multi-kernel class takes
  one lens instance per kernel), runs the manual-dispatch census. Macro code only; a kernel
  body or device call here is a defect. On Vulkan the dispatch macro itself derives access —
  there is no separate lens file.
- `dasllama_math_vulkan.das` — the Vulkan family entry: probe, arm, image identity, routers,
  init hooks. Its name is a require contract; renaming it is a defect.
- `dasllama_metal_gemm.das` — the Metal batch-GEMM donor.
- `dasllama_gpu_tier.das` — the device-cooperation SPI: hook types, install slots, status.
- `dasllama_kernel_access.das` — the shared body-walk read/write classifier both lenses run on,
  and the dispatch-lens micro-grammar (grid/tg/params spec parsing and emission). A grammar
  helper duplicated into a lens file is a defect.
- `dasllama_gpu_resident.das` — the whole-model GPU residency rail: the flavor bake, the stack
  upload, and the device-resident decode/prefill overrides. No device call and no GPU require
  belongs here; a device-specific arm is a defect. See `ARCHITECTURE.md` §1.5.

A backend-only capability goes in that backend's matching role file. A new grab-bag file for it
is a defect.

**A GPU family shares ONE device and queue**, created by `<gpu>_common`'s init — with one
exception: `dasllama_metal_gemm.das` owns its second device+queue (ARCHITECTURE §1.5). Any
other module creating a device is a defect.

**A PSO is compiled and released by the file that owns its kernel class**, through that file's
init/release pair (`metal_decode_init` / `metal_kernels_release`, both in
`dasllama_metal_kernels.das`; `metal_prefill_init` / `metal_prefill_shutdown` in
`dasllama_metal_prefill.das`) — ownership follows the class, not the pipeline stage. A PSO compiled
or released anywhere else in the engine is a defect. Tests are exempt: a kernel-unit gate
compiles its own short-lived pipeline from the emitted source and releases it in the same gate.

**Race code for a kernel family lives beside the family** — kernels races its families, prefill
races its own; the shared scaffolding (`race_buf`, `race_envelope_ok`, `race_pair_ms`) is
`<gpu>_common`'s. A race harness anywhere else is a defect.

**A Metal decline reason is a `MetalDecodeDecline` / `MetalPrefillDecline` value in
`dasllama_metal_shapes.das`; decline counting lives in `dasllama_metal_common.das`.** A
string-typed metal decline, or a counter beside the decline site, is a defect. (Vulkan decline
typing is `followup_vulkan.md` item 1.)

**The backend asymmetries are the closed list in `ARCHITECTURE.md` §1.5.** A diff that makes
Metal and Vulkan differ in a new way lands with its entry there, or it is a defect.

### Model families

- `dasllama_arch_*.das` — one file per architecture family, declarative registration only: build
  the descriptor, register it at init. An architecture that changes a forward loop, or a name
  test on a shared path, is a defect.

### Audio and ASR

- `dasllama_asr_types.das` — the shared ASR floor: `AsrCaps`/`AsrTimestamps`/`TranscribeSegment`.
- `dasllama_audio.das` — the shared audio tower, the mel/FFT machinery, the encoder block loop.
- `dasllama_audio_io.das` — audio decode to PCM. The only file that talks to the audio library.
- `dasllama_asr.das` — the ASR facade: the sniffing loaders, the model/session unions, one-call
  dispatch arms. Family names never appear in its public API.
- `dasllama_whisper.das`, `dasllama_parakeet.das`, `dasllama_canary.das`, `dasllama_qwen3a.das`,
  `dasllama_gemma4a.das` — one file per model family: its weights, its encoder, its session
  bundle, its caps sheet, its transcribe driver, its quirks. Shared tower pieces move up into
  `dasllama_audio.das`, never sideways between families.
- `dasllama_vad.das` — voice-activity detection weights and stream state.

**A verb arm in `dasllama_asr.das` is one forwarding call.** A new family touches the facade
only at the union field, the finalize line, the `AsrKind` value, and the one-line arms; a
prompt, a decode loop, a caps value, or a language rule in the facade is a defect.

**A GEMM in a family file goes through a `*_mm` wrapper or `mm_blob_b`.** A hand-written
dot-product loop beside them is a defect.

**Every `*_encode` and `*_log_mel` in `dasllama_audio.das` and the family files carries
`[hot_path]` and lints at zero.** Reused buffers take `@scratch`; debug and profiling legs
take `[cold_path]`. A nolint where either annotation fits is a defect.

**A mel frontend builds on the FFT-plan machinery in `dasllama_audio.das`.** A hand-rolled DFT
in a family file is a defect.

**Every family has a token-for-token oracle cell, and every oracle cell logs its transcript
as an `eyeball:` line.** An id-only comparison is a defect.

**A `create_session` / `transcribe` option that the model's `caps()` does not declare panics
at the call site** (`dasllama_asr.das`). Accepting it and silently ignoring it is a defect.

### Generated

- `ENVIRONMENT.md` — generated from `dasllama_env.das`'s `[EnvConfig]` declarations by
  `harness/gen_env_doc.das` (`tests/test_env_registry.das` fails on drift). Hand-editing the .md
  is a defect; edit the declarations and regenerate. `dasllama_unicode.das`'s RANGES/WS tables are transcoded from
  llama.cpp's unicode-data.cpp — hand-editing the tables is a defect; retranscode.

---

## File and memory model

**Runtime serves weights out of a mapped `.dlim`.** A live weight carrier's planes point into the
mapping `parse_image` produced, and going live does no significant work — repacking, quantizing,
folding, and permuting belong to the mint. A transform on the path that makes a carrier live is a
defect. See `ARCHITECTURE.md` §2.1.

**A missing `.dlim` is minted first, and the model is served from what was minted.** A load path
that falls back to reading weights out of the source file into a live carrier is a defect, and so
is one that serves a carrier the mint did not produce.

**There is one way to mint, and one way to load.** A weight carrier becomes live through
`build_image` and `parse_image` in `dasllama_image.das`, and nothing else. Code anywhere else that
reads weights into a live carrier, or releases an image backing, is a defect — and a second mint
path, whether per family, per format, or per backend, is a defect even where it produces an
identical file. See `ARCHITECTURE.md` §2.1.

**A mint never holds the whole model.** It sizes the image before the first byte goes out and
writes each plane as it is produced. Keeping the source model resident to write from is a defect,
and a mint that is slower in exchange for a lower peak is correct. See `ARCHITECTURE.md` §3.

---

## Image identity and contents

**A `.dlim` is box- and config-specific, not a portable format.** `image_identity` names the box
profile, the knobs, and the flavor a file was baked for, and a mismatch declines loudly. A path
that reinterprets a mismatched image, or widens an identity so that more files match, is a defect.

**A bake reaps only its own lane.** An identity's (quant, tag) pair is its lane; a save may drop
that lane's dead siblings plus BROKEN/version-stale images in any lane, and nothing else. See
`ARCHITECTURE.md` §2.1.

**Only a process that can recompute an image's identity may judge it dead — with one owner
carve-out.** Reaping an image whose identity the code cannot recompute — another flavor's,
another family's — is a defect, except `dlim_wipe` called from `gen_bench_records.das`: the
board rigs own the model dirs for the whole batch (wipe behind the exe gate at start, delete
after a model's last cell), so they delete without judging. Any other `dlim_wipe` caller is a
defect. See `ARCHITECTURE.md` §2.1.

**An image carries only what its flavor uses.** A plane the target platform or config never reads
is not written — the mint decides that, not the load. A flavor takes its own file through
`image_path_for` and its own tag through `register_image_family_tag`; carrying another flavor's
planes alongside its own is a defect.

---

## Documentation

**A change to user-facing API checks every tutorial and document that touches it.** User-facing
means anything a consumer outside this repo calls or types — exported facade functions, CLI
flags, environment knobs, file formats, and their defaults — plus the in-repo rig and tool CLI
surface, whose runbooks are `BRINGUP.md`, `PROFILE.md`, `METHODOLOGY.md`, and `ARCHITECTURE.md`.
The check covers the tutorial `.das` sources, their `.rst` pages, and the module documentation;
a tutorial or doc that still shows the old call, flag, or default is a defect of the change,
not of the docs.

**A changed default is restated everywhere the old default was stated.** Every docstring, help
string, and doc line that named the old value names the new one in the same diff.

## Implementation

**A kernel's shape is compile-time; only its data is runtime.** The test is one question: for a
given compiled kernel, can this value change between dispatches? If yes it is data and belongs in
a uniform or a kargs struct. If no it is shape — a block stride, a lane width, an unroll factor,
a format selector — and it must not reach the kernel as a uniform, a kargs field, or a helper
parameter. See `ARCHITECTURE.md` §2.2.

**A kernel body contains no indirection.** No function pointers, no vtables.

**A claim about a shape constant is checked against the emitted shader, not the das source.**
Read the generated `*_msl` global or the SPIR-V dump and confirm the constant is literal there;
a helper that looks specialized in das can still lower to a runtime multiply.

**Twins of a kernel family bind the same kargs type at the same binding**, even where one twin
ignores a field. A twin that shifts the other's fields to different slots is a defect.
Recorded exception: the moe mul_mm TENSOR twins (`MetalMoeMulMmQ8T` / `MetalMoeMulMmMx4T`)
keep the pre-family compact slots while their base classes bind the family numbers — the
kernel-unit gate and the race arms fork on it deliberately, and no shared bind path may span
that pair.

**Kernel twins share a template.** Same-body single/batch or format twins are stamps of one
`class template` — a `@template_constant` axis carries the body divergence, a `@template_gate`
field carries a stamp-varying binding. A copy-pasted twin class, or a new dummy-bound
always-present field where a gated one serves, is a defect.

**Every weight, bias, or lookup-table field on a `[metal_dispatch]` / `[vk_dispatch]` kernel
class declares `@role = "weight"` explicitly.** Derivation classifies an un-roled weight view
as a read and stages the whole blob per dispatch — the tracked-weight scheduler tax the
untracked-weights doctrine exists to avoid. A weight-shaped field with no `@role` is a defect
even when the kernel compiles and passes parity. Un-lensed lab classes in `benchmarks/` are
out of scope — nothing derives their access.

**A kernel declares its dispatch on the class; the builder is generated.** A new kernel class
carries `[metal_dispatch]` / `[vk_dispatch]` with per-field `@binding` / `@role` / `@off` /
`@span` / `@default`. A NEW hand-written `enc_*` body is a defect unless it is a wrapper
shape: a format or twin pick, a default-filling wrapper, or a composite over generated
builders.

**No value reaches an encoder twice.** A scalar uniform buffer passed alongside the identical
value as a parameter is a defect; so is a kargs field the fields beside it already determine.
Carve-out: a builder parameter the `grid=`/`tg=` spec consumes HOST-side (the dispatch
geometry) may repeat a kargs field the kernel reads device-side — those are two consumers,
not one value bound twice.

**Nothing in the engine or a race harness dispatches a kernel except its `enc_*` builder.** A
hand-rolled bind list in `dasllama/` or `performance/` is a defect; kernel A/B labs under
`benchmarks/` are the recorded exception (the lab rule above), and the kernel-unit gates under
`tests/` hand-bind on purpose: the gate is an independent witness of the binding contract, so
a builder defect cannot mask a kernel defect. Carve-out: the IN-ENGINE moe mul_mm A/B race
harnesses (`dasllama_metal_prefill.das`) encode through `kn_moe_mm_family_tail`, the family's
shared private bind tail, so a binding renumber desyncs them loudly instead of unseen.

**A cache keyed by a host address carries the span and the form in its key.** A hit must cover
the request, and different upload forms live in separate tables.

**A predicate answering "can this run" must not also answer "is this ready".**

**Peak memory wins ties against the cost of loading a model.** A load-time change that trades
footprint for speed ships the measured pair — peak footprint and wall-clock — and an explicit
call. See `ARCHITECTURE.md` §3.

**A complexity or length warning is a prompt to look, not an order to split.** An irreducible
shape takes a suppression with a one-line reason. Splitting where no seam exists — helpers that
exist only to lower a number — is a defect, and so is suppressing a function already ledgered as
wanting a dedup.

**New code meets the complexity and length limits from the first line.** The suppression is for
shapes that cannot reduce, not for code written oversized.

**Platform backends implement narrow registered contracts.** Platform-specific code in a
platform-neutral file is a defect.

**Every program root declares `options stack = 524288`.** A test, harness, benchmark, or tool
that picks its own number — larger or smaller — is a defect, and so is a new root that omits the
declaration. See `ARCHITECTURE.md` §2.8.

**No ad-hoc profiling.** A NEW clock read paired with a print or log of the elapsed interval is
a defect in engine code — instrumentation goes through the sanctioned rails, and a clock whose
value feeds logic is marked `// clock: control`. The rails, the carve-outs (`benchmarks/`,
`performance/`, `harness/`, cold one-shot load/mint progress logs), and the marker's why are
`ARCHITECTURE.md` §2.10.

**Every new kernel or mid-runtime loop is COVERED by `[hot_path]`.** A region entry is an
`*_encode` / `*_decode` / step driver; a new function is a defect only when no annotated
region entry reaches it — a new entry point (including a backend entry: kernel-backend
override, batch donor) carries the annotation itself. The transitive-arming model and the
`@scratch` / `[cold_path]` companions are `ARCHITECTURE.md` §2.11.

**No raw environment access outside `dasllama_env.das`.** A knob is an `[EnvConfig]` field
there, read as `g_env_*.<field>`; `get_env_variable` / `has_env_variable` / `set_env_variable` /
literal-name `env_config_*` anywhere else in the module is a defect. The sanctioned forms —
tri-state knobs, dynamic names, pre-init foreign-library writes — are `ARCHITECTURE.md` §2.9,
and `tests/test_env_registry.das` enforces the lot.


### Vulkan

**A pipeline is created only by a `[vk_dispatch]`-generated `ensure_*` and torn down by
`vk_drop_model_state`.** A hand-written pipeline build anywhere else in the engine is a defect.

**A buffer bound as one SSBO range stays under `vk_max_storage_range()`, checked where its
size is NEGOTIATED, not where it binds.** The bind site cannot shrink a buffer that was sized
wrong; past the range every read is undefined, not slow.

**A resident-driver change ships with `harness/parity.das` GPU-vs-CPU runs on one q8 and one
kq model** — the vulkan arm is `DASLLAMA_GPU=1` (never `--ngl`, which is the Metal arm and
panics without Metal). Kernel-unit green does not cover driver wiring, and the two
activation-quant classes ride different enc paths — one model class cannot witness the other's.

**A descriptor set cached across dispatches lives in state `vk_drop_model_state` clears** — a
`*_ready` latch or a field inside `g_gpu` / the arena. A set cached in anything the drop does
not touch dangles into the next model.
