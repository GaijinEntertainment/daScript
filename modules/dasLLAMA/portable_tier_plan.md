# The portable tier - dasLLAMA without the JIT crowns: no-LLVM build, AOT, WASM64 - plan

**Status: decisions settled (2026-09-04 discussion), pre-implementation.** The arc has one
product goal, reached in three stages that each prove the next one's premise: first a regular
`-jit` build with the `[tune]` framework short-circuited (the portable tier running on JIT-compiled
reference bodies), then an AOT compilation target under `examples/dasLLAMA/` on a very small
model, and only then WASM64. Main targets are ASR and TTS; a small LLM rides along, and the
model set may go smaller still. Vectorizing the default kernels is a separate, measured leg
between the stages (sec. "The vectorization leg"). The `-jit`-only rule in
`ARCHITECTURE_INVARIANTS.md` sec.3 is adjusted when the AOT stage reaches it, not before.

Companion evidence: the four inventories the 2026-09-04 session produced (the LLVM dependency
census, the tune-framework coupling, the WASM build state, the require-guard grammar, and the
kernel test coverage) are summarized here with their file:line cites; nothing below is a guess.

---

## The frame

dasLLAMA is pure daslang (108 files under `dasllama/`, no C++ of its own). Its whole
dependency on LLVM is one file, `dasllama/dasllama_gemm_gen.das` (3756 lines), which builds
LLVM IR through the `llvm_boost` / `llvm_jit_code` / `llvm_jit_common` bindings; 40 generator
keys, permutations over `dot` (sdot, maddubs, vpdpbusd, vpdpbssd, smmla, amx_int8) and `width`
(128, 256, 512). It is reached only through path guards (`dasllama_common.das:22`,
`modules/dasLLVM/daslib/llvm_user_modules.das:8`). Everything else that spells `llvm/` is
packaging: 33 requires of `llvm/daslib/f16_cvt` and 4 of `llvm/daslib/aarch64_neon` (pure
das, zero requires of their own, recognized by the JIT by mangled name), and 27 of
`llvm/daslib/llvm_tune` (2759 lines, not one LLVM call). The `llvm/` path root exists only when
`DAS_LLVM_DISABLED=OFF` (`modules/dasLLVM/CMakeLists.txt:5`), and a source tree still resolves
it through the `.das_module` folder scan, which is why the module "works" today with LLVM
disabled by accident and fails in an installed SDK.

`[tune]` never rewrites a body: it stamps `[llvm_code]`, and the tuned kernel IS the IR the
generator emits (`llvm_tune.das:1436-1448`, `llvm_jit.das:7961`). Every `[tune]` family keeps a
runnable das reference body; the `portable` backend registers unconditionally at priority 0
(`dasllama_math_default.das:1613`); `arm64-gen`, `x64-gen` and `arm64-sdot` are
`jit_enabled()`-gated (`dasllama_math_gen.das:2775, 2807`, `dasllama_math_aarch64_neon.das:458`).
das `float4`/`int4` lower to 128-bit SIMD in every tier; 256/512-bit exists only as JIT
name-recognized intrinsics and the IR generators. There is no mechanism that freezes a chosen
permutation as das or C++; `policies.tune_frozen` means de-tuned.

What the portable tier therefore is: the reference bodies, on whichever compiler the tier has.
Under the JIT they are auto-vectorized (the NEON file prefers the portable `dot_q8q8` over its
own scalar `sdot4` fallback for that reason, `dasllama_math_aarch64_neon.das:455-457`). Under
AOT they are C++ through clang or gcc. Under WASM they are that C++ through emcc with
`-msimd128`, or LLVM IR from the host emitter through the `-exe` cross path.

The WASM side is further along than assumed: a wasm64 + pthreads runtime builds and ships
(`web/CMakeLists.txt` with `DAS_WASM_MEMORY64` and `DAS_WASM_PTHREADS`, deployed by
`pages.yml:260`); the cross-compile is `daslang -exe --jit-target=wasm64-unknown-emscripten`
with LLVM on the host only; wasm32 cross is unsupported by design (pointer-width layout bake,
`web/CMakeLists.txt:363-372`).

---

## Settled decisions

- **The cut.** Annotations that are 100% inert without the framework move to daslib; the
  framework stays in dasLLVM whole. With no LLVM there is no tune, no sidecar, no exchange,
  no tuner spawn - as if nothing happened. Not the read/mint split first proposed.
- **`f16_cvt` moves to `daslib/f16_cvt.das`.** The JIT recognizes it by mangled name from a
  target-independent table (`llvm_jit_intrin.das:232-234`), so the move changes nothing for
  the JIT; 33 require spellings change.
- **`aarch64_neon` and `x64_avx` stay in dasLLVM.** Nothing in the engine requires `x64_avx`.
  The NEON tier file registers only under `jit_enabled()`, so its require becomes the path
  guard `?llvm/daslib/aarch64_neon` from the umbrella; the harness, probe and benchmark that
  require it are JIT-only by nature.
- **`require ?G target` names a guard module, never the target** (`ds2_parser.ypp:849-861`,
  `parser_impl.cpp:1225-1279`). There is no module named `llvm` (dasLLVM is a pure-das dasbind
  package), so `?llvm` and `builtin_module_exists(llvm)` are permanently false; the working
  spelling is the path guard `?llvm/daslib/llvm_tune`. `get_platform_name()`,
  `get_cross_platform_name()` and `get_architecture_name()` fold at compile time
  (`module_builtin_runtime.cpp:2887-2907`), so a synthetic `wasm` module buys nothing for code
  gating. `require !?X` is about five parser lines plus the hand-written pre-parse scanner
  mirror (`ast_parse.cpp:229-268`); reach for it only if a module must not LOAD when X exists.
- **The routes.** AOT C++ through emcc is the LLVM-free WASM route. The `-exe` cross route
  keeps tuned kernels and needs host LLVM; it is the later option, not the first.

---

## Stage 0 - the coverage audit, and what it must add before any kernel moves

Running the suite with the JIT off proves nothing by itself: every model-backed cell self-skips
on `if (!jit_enabled())`, and `guard_interp_inference` (`dasllama_math.das:29-33`) turns anything
that tries to serve into a panic. The portable tier's real safety net is the synthetic,
direct-call unit layer, and that layer is complete exactly where the K-quant work happened
(`tests/test_kquant.das`: 13 formats x dots, gemv rows, repack grp, tile, groupn against fp64
dequant and bit-exact disk references; its `test_kq_tile` header at `:1673-1676` claims validity
on reference bodies and stamped kernels alike) and thin to absent elsewhere. The additions, in
priority order:

**(a) A `[tuned]` elementwise unit.** `dot` (`dasllama_math.das:48`), `axpy` (`:66`),
`dot_bf16` (`:81`), `add_inplace` (`:97`), `add_scale_inplace` (`:111`), `mul_inplace` (`:123`),
`scale_inplace` (`:137`), `copy_floats` (`:151`). All `[tuned]`, the highest-traffic kernels in the
module (`copy_floats` 62 engine sites, `add_inplace` 35, `scale_inplace` 33, `axpy` 26), zero
coverage. `dot` is pinned only through three hand-computed `matmul` cases. The unit sweeps
lengths with tails against fp64 accumulation references.

**(b) `test_q8q8_family.das`.** The crown family (`q8q8_tile_gen` `dasllama_math_gen.das:1222`,
`q8q8_gemv_gen` `:107`, the mx4 and s16 companions `:124, :145, :1153, :1170`) has its
tile-vs-gemv-vs-reference parity only in hand-run probes (`harness/gen_parity_probe.das:110,131`,
`harness/gen_slot_parity_probe.das`). Promote them into the suite with the ladder the 13 kq
families already have, and cover what has no gate at all: the whole s16 (`wscale_f16`) arm
(`dot_q8q8_f16s` `dasllama_math_default.das:64`, `q8q8_rows_kernel_s16` `:320`,
`q8q8_groupn_kernel_s16` `:354`, the s16 overloads `dasllama_math.das:2096-2138`),
`matmul_q8q8_group3` (`:2081`), `matmul_q8` / `dot_q8` (`:906, :881`), and
`matmul_mx4q8_batch` / `_batch_groupn` (`:2194-2226`).

**(c) A CPU rope-application unit.** `rope_scaled`, `rope_scaled_neox`, `rope_scaled_tab`,
`rope_scaled_neox_tab`, the `_part` twins and `rope_apply` (`dasllama_math.das:2992-3160`) are
today only the oracle of the Apple-only Metal rope tests; `test_rope.das` covers the table
builders and the legacy `rope()`. An in-test angle reference makes them a subject.

**(d) The activation and norm cells.** `softmax_sink` (`:2569`, six gpt-oss sites, no test),
`swiglu_oai` / `swiglu_oai4` (`:2749, :2820`), `softcap` / `softcap4` (`:2932, :2947`),
`sigmoid_gate` / `sigmoid_gate4` (`:2884, :2893`), `silu_mul4` (`:2911`), `silu4_batch`
(`:2869`), `l2_norm_rows` (`:2677`), `gemm_f32` / `gemm_f32_jo` / `gemm_f32_uk_4x16`
(`:834, :858, :828`; 23 engine sites, the tower and TTS float GEMM). Alongside the existing
`test_silu.das` / `test_softmax.das` property tests.

**(e) Prefill and KV cells.** `matmul_kq_batch` (`:1944`, no test; portable has no slot, so
the serial-GEMV fallback is also ungated), `matmul_kq_batch_groupn` (`:1918`, whose only gate
`test_kquant.das:2136` self-skips off-JIT at `:1962-1966`), `kv_row_to_f32` and
`cvt_q8kv_to_f32` (`dasllama_kv_codec.das:394-409, :140`), `requant_rows_q8` /
`requant_rows_q8k_bs` (`dasllama_convert.das:1490, :1509`), `repack_kq_weight`
(`dasllama_math.das:1974`).

**(f) Suite membership and honesty.** `test_kv_codec.das` and `test_vad.das` (a frozen
onnxruntime oracle with weights in-tree, `tests/test_vad.das:75`) are absent from
`tests/run.das:75-171`'s `model-free` list; add them. `test_mxfp4.das:168-170` returns silently
instead of `t |> skip` (against `tests/CLAUDE.md:40-42`). `test_groupn.das:12-15` takes its
expected values from the per-region GEMV under the same backend pin, so a bug shared by
`q8q8_kernel` and `q8q8_groupn_kernel` passes; give it an independent reference.

**(g) A way to point the parity rails at the portable backend.** `tests/test_parity.das`
(`:177`) and `harness/parity.das` cannot select a backend from the command line; the
documented `DASLLAMA_PIN_BACKEND` knob (`ENVIRONMENT.md:25`, `dasllama_env.das:50`) is read by
three benchmarks and never by the library. Wire it into the parity harness (or add
`--backend`), so the same frozen ids replay against the tier this arc ships.

**(h) Tower, ASR and TTS.** Encoder-tower GEMM wrappers `mm_blob_b` / `mm_bf16_b` /
`mm_plane_b` (`dasllama_tower.das:253-277`) and `layernorm_batch` (`:182`) have no CPU unit;
the ASR conv stems and mels (`dasllama_qwen3a.das:514, :406`, `dasllama_canary.das:786-841,
:670`, `dasllama_gemma4a.das:778, :744`, `dasllama_parakeet.das:905, :987, :805`, the whisper
stem `dasllama_whisper.das:987`) are `private` and covered only by jit-gated, model-gated
transcript oracles in `test_whisper.das`; `fft_pow2_run` (`dasllama_audio.das:157`) has no
direct test. TTS: `bilstm`, `sine_source`, `magnitude_phase`, `istft_envelope_divide`
(`dasllama_tts_blocks.das:1633, :1920, :1973, :2037`) reach only `_tts_parity.das` (model +
oracle-dir gated); `resize_linear` / `resize_linear_torch` (`:1738, :1776`) have none. Since ASR
and TTS are the main targets, the private stems get exposed to a synthetic unit or a
weights-in-tree oracle the way VAD already has one.

Stage 0 also lands the four rule-document and gate fixes owed from the is_nan arc
(a `tests-cpp/REVIEW.das` gate that every `set_source_files_properties` path exists, the
`tests-cpp/REVIEW.md` rule 3 wording, the `include/daScript/simulate/REVIEW.md` hot-path rule
naming the shipped build as the judge, and the `writing_cpp_tests.md` line about CMake edits).

**Exit:** every kernel family in the inventory has a synthetic gate that does not self-skip
without the JIT, or a written reason it cannot; the suite's `model-free` arm lists every such
file.

---

## Stage 1 - the refactor, and the portable tier on a regular `-jit` build

1. `daslib/f16_cvt.das`; 33 require spellings.
2. The inert annotation module in daslib carrying `tune`, `tune_perm`, `tune_companion`,
   `tune_scope`, `tune_policy` and `llvm_code` as shells: `require ?llvm/daslib/llvm_tune
   llvm/daslib/llvm_tune`, then `static_if (typeinfo builtin_module_exists(llvm_tune))` in each
   `apply` delegating to the framework, else return true and do nothing. The shell's own
   path-guarded require is the witness, so the order caveat on `builtin_module_exists`
   (`ast_infer_type.cpp:2833-2848`) does not bite. Framework change: its annotation classes
   stop self-registering under those names (two registrations of `[tune]` would resolve
   ambiguously) and expose their `apply` as plain functions the shells forward to. The
   `_variants()` registries are emitted only by the framework; the engine references none
   outside the guarded generator tier.
3. `[tuned]` goes framework-optional: `dasllama_tune.das:14` takes the path guard, and
   `resolve_perm` (`:330-346`) with no framework returns the declared `fallback=` or the default
   perm and reports nothing - no sidecar opened, no `tune_status` line, no box profile.
4. `dasllama_exchange.das` becomes a guarded require from the umbrella and the server.
5. The NEON tier's require takes the path guard.
6. `tests/run.das` gets a no-tune arm (the framework's reference policy; `run.das:309`
   hard-codes `-jit` with no knob today), and the suite plus the parity rails run under it on
   the stage-0 gates.
7. Measure the portable floor on the reference small models (decode and prefill, ASR and
   TTS cells), under the measurement discipline: it is the number every later tier is judged
   against.

**Exit:** a `-DDAS_LLVM_DISABLED=ON` tree compiles the engine and its suite; a `-jit` run with
the framework short-circuited is green on the stage-0 gates and parity; the floor is in
`PERF_LEDGER.md`.

---

## Stage 2 - an AOT compilation target under `examples/dasLLAMA/`

- One example program (a small-LLM chat, then ASR and TTS twins) registered as an AOT target,
  the first exception to `modules/dasLLAMA/CMakeLists.txt:7` ("never AOT'd"), scoped to that
  example.
- `guard_interp_inference` accepts `aot_enabled()` beside `jit_enabled()` and
  `is_standalone_exe()`; `guard_interp_gguf_load` likewise. This is the rule adjustment the
  invariants doc gets in the same change.
- The stamped-function hash question is moot under `-aot` (`tune_frozen`), and with the
  stage-1 shells the AOT emitter sees ordinary functions.
- Expect heavy TUs and a long C++ compile: the engine closure is large. Known AOT traps from
  earlier arcs to watch first: int64 pointer indexing, managed-vector for-in.
- Parity against the stage-1 JIT run on the same model is the pass criterion, ids exact.

**Exit:** the example compiles and runs the small model with exact parity to the JIT run; its
decode/prefill numbers sit beside the stage-1 floor.

---

## Stage 3 - WASM64

- Route: the stage-2 example's AOT C++ through emcc against `web/output64` (memory64 +
  pthreads), under `wasmtime -W memory64=y -W exceptions=y` first, the browser after.
- Repairs on the way, all small: the `wasm_cross` CI lane configures without
  `-DDAS_WASM_MEMORY64=ON` (`wasm_build.yml:209`) so `web/CMakeLists.txt:374` registers no
  examples and the lane is a green no-op; `get_architecture_name()` answers `wasm32` on a
  memory64 build (`module_builtin_runtime.cpp:2064`) and the engine keys backends off that
  string; `DAS_MAX_HW_JOBS` is 4 on wasm (`platform.h:629`); the two emsdk pins contradict
  (`wasm_build.yml:188` vs `pages.yml:73`).
- Limits to design around: SIMD is 128-bit through the `-msse2` shim and that flag is
  load-bearing (`web/CMakeLists.txt:34`; without it vecmath drops to scalar); a process-wide
  malloc spinlock under shared memory (`src/hal/wasm_thread_malloc.cpp`); the main-thread join
  is sliced at 500 ms; no dasHV on wasm, so no server surface.
- Model IO is the wall: `fmap` on MEMFS copies the whole file into the heap, there is no
  OPFS or WASMFS anywhere, and no `-sMAXIMUM_MEMORY`. Small quantized models load; a
  windowed OPFS reader is its own item if anything larger is ever wanted.

**Exit:** the ASR and TTS examples run under wasmtime with parity; a small LLM decodes; the
browser demo is a stretch on the same artifact.

---

## The vectorization leg (between stages 1 and 3, measured)

Under the JIT the reference loops are auto-vectorized, so stage 1 is the SIMD-achievable floor
for the default kernels. AOT and WASM have no vectorizer we control: the emitted C++ goes
through clang, gcc or emcc, whose auto-vectorization of daslang loop shapes is hit and miss,
and the interpreter never vectorizes a loop. Explicit `float4` / `int4` bodies are how those
tiers reach the floor structurally. Do it per hot kernel by the stage-2 profile, not up
front: the q8 dot and GEMV/GEMM, the k-quant dequant-dot, f16 widen, attention dot and axpy,
the KV codec, the TTS conv and GEMM. The crux is the int8 dot: das has no int8 vector type,
so it either widens through `int4` lanes or gets a name-recognized lattice intrinsic with a
fallback body (the `x64_avx.das` pattern), which on wasm would want SIMD128's
`i32x4.dot_i16x8_s`. That decision is taken with stage-2 numbers in hand.

---

## Models

ASR and TTS first: whisper tiny, the silero VAD (weights in-tree), Kitten nano, Kokoro. For the
LLM: gemma-3-270m, SmolLM2-135M, Qwen3-0.6B are all registered arches; the two under 300M are
the honest wasm candidates, and smaller is on the table.

## Open questions

- Which small-LLM carrier gets the parity fixture for stages 1 and 2 (SmolLM2-135M already has
  a cls_q8 parity cell, `test_parity.das:76`).
- Whether `test_groupn.das`'s independent reference is fp64 dequant (the kq ladder's choice) or
  a second, scalar implementation.
- Whether the stage-2 example is its own daspkg app or a plain `examples/` program.
