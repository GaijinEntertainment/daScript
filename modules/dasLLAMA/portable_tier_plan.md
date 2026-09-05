# The portable tier - dasLLAMA without the JIT crowns: no-LLVM build, AOT, WASM64 - plan

**Status: stage 0 landed (2026-09-04) - the coverage additions (a)-(h) are in `tests/` and green
on this box, `test_parity` reproduces the frozen ids on the portable backend, and the four owed
rule fixes are in; stage 1 in progress.** Decisions settled in the 2026-09-04 discussion. The arc has one
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
- **`aarch64_neon` and `x64_avx` move to daslib too** (ruled 2026-09-04, landed). The first
  cut kept them in dasLLVM behind a path guard on the NEON tier; the no-LLVM gate then showed
  the generated families' reference bodies in `dasllama_math_gen.das` call the NEON module's
  `dot_q8q8_laneq4` and `idot4`, so the whole generated tier and the six tests that require it
  fell out of a no-LLVM build. Both intrinsic modules are pure das with zero requires; the JIT
  recognizes them by module name, unchanged. With them in daslib the NEON tier and `math_gen`
  compile everywhere, their `[init]` registrations self-gate on `jit_enabled()` and the arch,
  and the kq ladder runs on the no-LLVM tier. The three pure table builders `test_kquant`
  borrowed from the IR generator (`pm1_of`, `vbmi_alphabet`, `vbmi_pack_word`) moved to
  `dasllama_gemm_schema.das` for the same reason.
- **`require ?G target` names a guard module, never the target** (`ds2_parser.ypp:849-861`,
  `parser_impl.cpp:1225-1279`). The guard for everything LLVM is `?llvm`: dasLLVM is a pure-das
  dasbind package, so it now carries one C++ witness module named `llvm`
  (`modules/dasLLVM/src/dasLLVM.cpp`), compiled in exactly when the build is configured with
  dasLLVM. The build configuration decides, never the files on disk - a wasm build never
  compiles the witness, and a console build with the LLVM sources present but configured out
  loads no framework, no sidecar, no IR generator (ruled 2026-09-04; the path-guard spelling
  `?llvm/daslib/llvm_tune` that preceded it satisfied itself off the filesystem). `get_platform_name()`,
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

**(f2) The same shared-bug class in the two mx4 groupn cells.** `test_mx4q8_groupn` takes its
expected values from the per-region `matmul_mx4q8` under the same backend pin, and
`test_groupn_bias_fold` takes its q8 and mx4 expectations from the unbiased groupn kernel; the
fp64 mx4 reference already exists as `ref_dot_f64` in `test_mxfp4.das:35` (private to that
file). Lift it into a shared `_kernel_ref.das` beside the two files and close both cells the
way (f) closed the q8 one. Lint candidate for a folder-local `tests/REVIEW.das` gate: a
`t |> run` arm that returns before asserting registers a skip (the guard whitelist is
`model_available`, `arm_on`, `family_on`, `model_missing`, `gguf_missing`,
`facade_data_missing`); the `test_mxfp4.das` silent return was the one raw-capability return
that did neither.

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

**(h) findings from writing the tower/ASR/TTS unit.** The four family mels and every TTS
kernel named above are already public; only the seven conv-stem helpers and `lstm_direction`
are private, and the whisper-class stem is not in the whisper file at all but lines 1300-1333 of
`audio_encode_blocks` (`dasllama_audio.das`), serving whisper, ultravox, voxtral, qwen2audio and
omni. Ranked exposure: (1) split that stem into one public kernel in `dasllama_audio.das`
(ARCHITECTURE_MEDIA sec.2.14 names the seam); (2)-(3) the four mels need no engine change, only
an fp64 STFT reference; (4) `bilstm` is public, gate `lstm_direction` through it and run one
shape twice for the scratch-global reset; (5) `magnitude_phase` / `istft_envelope_divide` are
pure and now covered; (6) drop `private` on the two parakeet convs; (7) `g4a_conv_stage` splits
into a public im2col and a LayerNorm+ReLU epilogue that has no twin anywhere; (8)-(10) the
qwen3a and canary stems converge on the parakeet shape (the three im2col walks are
near-identical copies); `cn_conv_pw` is a wrapper, not a kernel. `sine_source` cannot take an
fp64 reference (ARCHITECTURE_TTS sec.2.33 phase law): its gate is a golden vector.

**(i) findings from writing the other units.** `test_kquant.das`'s synthetic plane builders are
private in a `[test]` root; lift them into a `_kq_fixtures.das` sibling (the
`_metal_kernel_common.das` precedent) so the kq gates stop hand-packing blocks.
`kq_batch_kernel_gen` / `kq_batch_groupn_gen` (`dasllama_math_gen.das:2185, :2233`) are private
unlike their groupn sibling, so their reference bodies are reachable only through a backend
pin; make them public. `KernelBackend` has no `kernel_backend_has_mx4_batch()` predicate, so a
sweep of `kernel_backend_names()` that touches `matmul_mx4q8_batch` panics on `arm64-sdot`
(`mx4q8_unset_batch`, `dasllama_math.das:1051, :1573`). An exact-equality float compare whose
fixture lattice no longer guarantees exactness has no lint; the two elementwise files carry
hand-derived lattice arguments, worth a fixture-intent annotation before any rule.

Stage 0 also lands the four rule-document and gate fixes owed from the is_nan arc
(a `tests-cpp/REVIEW.das` gate that every `set_source_files_properties` path exists, the
`tests-cpp/REVIEW.md` rule 3 wording, the `include/daScript/simulate/REVIEW.md` hot-path rule
naming the shipped build as the judge, and the `writing_cpp_tests.md` line about CMake edits).

**Exit:** every kernel family in the inventory has a synthetic gate that does not self-skip
without the JIT, or a written reason it cannot; the suite's `model-free` arm lists every such
file.

---

## Stage 1 - the refactor, and the portable tier on a regular `-jit` build

1. `daslib/f16_cvt.das`; 37 require spellings (landed).
2. The inert annotation module in daslib carrying `tune`, `tune_perm`, `tune_companion`,
   `tune_scope`, `tune_policy` and `llvm_code` as shells: `require ?llvm/daslib/llvm_tune
   llvm/daslib/llvm_tune`, then `static_if (typeinfo module_exists(llvm_tune))` in each
   `apply` delegating to the framework, else return true and do nothing. The shell's own
   path-guarded require is the witness (`module_exists`, not `builtin_module_exists` - see the
   ruling under "Open questions"). Framework change: its annotation classes
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
   the stage-0 gates. (Landed as `--no-tune`; the whole-suite run under it is 2.5 h and was
   cancelled - the suite rework in flight elsewhere is the gate, this arc runs single files.)
7. Measure the portable floor on the reference small models (decode and prefill, ASR and
   TTS cells), under the measurement discipline: it is the number every later tier is judged
   against.
8. The `-jit -exe` rail under the reference policy (landed). A standalone exe targets the host
   CPU only when the program carries `[llvm_code]` kernels (`llvm_jit_run.das`,
   `has_generated_kernel`); with the framework short-circuited there are none, the exe is the
   generic ARMv8.0 baseline, and the NEON tier's reference bodies still name-resolved
   `aarch64_neon::sdot4` to SDOT - a fatal `Cannot select` at codegen. The SDOT and SMMLA
   tables now gate on `g_target_arm64_dotprod` / `g_target_arm64_i8mm` (host rail: always /
   detected; generic and cross rails: the force env only), and a force-env feature reaches the
   generic machine's string, so the gates and the machine stay one truth
   (`modules/dasLLVM/ARCHITECTURE.md#aarch64-feature-truth`). The generic exe runs the NEON
   tier on the scalar `sdot4` fallback - slower than the portable backend's auto-vectorized
   `dot_q8q8`; whether the `[init]` backend gates should also consult the exe's feature rail is a
   stage-2 question, alongside the same shape under AOT. Gates: `tests/jit_tests/aarch64_neon.das`
   (hardware vs reference per intrinsic), the baseline probe in `tests/jit_tests/exe_host_cpu.das`
   (red on the pre-fix emitter), and `test_exe_smoke` under `DAS_TUNE_POLICY=reference`.

**Exit:** a `-DDAS_LLVM_DISABLED=ON` tree compiles the engine and its suite; a `-jit` run with
the framework short-circuited is green on the stage-0 gates and parity; the floor is in
`PERF_LEDGER.md`.

---

## Stage 2 - an AOT compilation target under `examples/dasLLAMA/`

**Status: the target exists and the exit's parity half holds (2026-09-04).** `examples/dasLLAMA/CMakeLists.txt`
builds `dasllama_aot` (opt-in, `EXCLUDE_FROM_ALL`): the daslang host with every CPU engine
module's C++ stubs linked in (`DAS_AOT_LIB` over `dasllama/*.das` minus the Vulkan and Metal
tiers, the LLVM IR generators and the exchange server, which interpret as scaffolding) plus the
daslib modules the CPU path calls that `libDaScriptAot` does not carry. `bin/dasllama_aot -use-aot
examples/dasLLAMA/run.das -- SmolLM2-135M-Instruct-Q8_0.gguf` reproduces the `-jit` reference-policy
run's 64 greedy tokens exactly. The run links 4637 functions; the 1178 it does not are the excluded
Metal tier plus 38 generic instantiations attributed to `json` / `strings_boost` / `strings_convert`
(libDaScriptAot's stubs, minted from another root - harmless, they interpret).

What it took, each a general fix rather than a dasLLAMA one:

- `-use-aot` now does what its help text says: the host compiles the script with `policies.aot`
  (and `tune_frozen`, the generator's policy) so the linked stubs bind; a function with no stub
  interprets (`fail_on_no_aot` stays off in the host). Before, only dastest honored the flag.
- The guards read the AOT tier through `aot_kernels_linked()` - `is_aot_function` on one kernel
  (`dasllama_math::silu`), a runtime probe - never `aot_enabled()`, which folds differently under
  generation and the consuming run and would desync every caller's hash. `ARCHITECTURE_INVARIANTS.md`
  sec.3 carries the adjusted rule.
- `das_accelerate` had no `aotRequire`, and a C++ module without one AOT-disables every das
  module whose require closure reaches it - the whole engine above `dasllama_math_accelerate`
  emitted empty TUs (`// AOT disabled due to module requirements`) with no error anywhere. The
  module now ships `dasAccelerate.h`. Lint candidate: every in-tree C++ module class overrides
  `aotRequire` (a tests-cpp or CMake-time gate).
- The emitter's topological structure sort read a function type's parameter types as by-value
  dependencies, saw a false cycle (`BatchWorkspace` embeds `Session`; a `function<(...BatchWorkspace...)>`
  typedef names it) and fell back to source order - C++ then embedded an incomplete type.
  `collectStructDeps` skips function / lambda / block types, and a residual (container-mediated)
  cycle keeps the sorted prefix. Gate: `tests/aot/test_struct_order.das`.
- `das_is_aot_function` / `das_is_jit_function` and the jobque affinity pair had no AOT header
  declaration (`aot_builtin.h`, `aot_builtin_jobque.h`); the generated C++ named them undeclared.
- The AOT TUs include every closure module's C++ header, so the target compiles with dasAudio's,
  dasMinfft's and dasVulkan's (volk) include dirs; the generator reads `aotRequire` out of the
  loaded shared modules, so those are regeneration inputs too (`DAS_AOT_EXTRA_DEPENDS`).

The exit's numbers half is open: on the M5 box, contended by a build, the AOT decode ran 54 t/s
against the JIT reference policy's 360 t/s on the same model (prefill 74 vs 617) - not a
measurement, a gap of the size the vectorization leg predicts (clang over the emitted loop
shapes versus the JIT's auto-vectorizer). Next: the same rail for the ASR and TTS examples, a
model-free gate for the AOT target (CI has no models; the build itself is the compile gate, and a
synthetic-weights run is the cheapest runtime one), Linux and Windows builds of the target, and the
stage-2 profile that decides which kernels the vectorization leg rewrites first.

**Exit:** the example compiles and runs the small model with exact parity to the JIT run; its
decode/prefill numbers sit beside the stage-1 floor.

---

## Stage 3 - WASM64

**Status: the AOT-through-emcc rail runs (2026-09-04).** The same `dasllama_aot` target builds in
the wasm64 web tree (`web/build64`, `DAS_WASM_MEMORY64` + pthreads): the repo's host daslang
generates the engine C++ (the host-generated set compiles for wasm64 unchanged - 8-byte pointers
on both sides, every layout `static_assert` holds), em++ compiles the ~100 TUs, and the engine's
das files embed into MEMFS beside daslib. `examples/dasLLAMA/wasm/run_node.js` mounts the repo
and the model's directory through NODEFS and calls main with `-use-aot`; on SmolLM2-135M-Q8 the
64 greedy tokens match the native JIT run exactly, decode 46 t/s under node 25 against 54 on the
native AOT host (same box, not a measurement). No dasLLVM is embedded, so `require ?llvm` is
false inside the artifact and no tune framework exists in it. Two link-time findings: the
Accelerate driver's TU stays out wherever `das_accelerate` is not built (its C++ calls the
binding by symbol), and the AOT type tables need a 512 MB initial heap on top of the web build's
embeds. The artifact is 60 MB - 35 MB of code, 24 MB of data (about 10 MB of embedded `.das`,
the rest the AOT type tables). It was 306 MB before the web build's embed step learned to
exclude build residue: `_aot_generated/` C++ and `.jitted_scripts/` caches sit inside the
embedded source trees in a working checkout and outweighed the sources twenty to one; the
playground's `daslang_static` carried the same 95 MB. Next: wasmtime (no JS host), the browser
page, ASR and TTS examples.

- Route: the stage-2 example's AOT C++ through emcc against `web/output64` (memory64 +
  pthreads), under node first (NODEFS mounts), the browser after.
- The GPU tiers already self-gate the way a wasm build needs (verified 2026-09-04 with
  `--disable-module dasvulkan`: the umbrella loads, `builtin_module_exists(vulkan)` is false,
  kernel units green interpreted and under the JIT). The Vulkan tier hangs off one guarded
  require in `dasllama_transformer.das` (`require ?vulkan dasllama/dasllama_math_vulkan`), the
  same shape as the five `?das_metal` lines beside it; every unguarded `require vulkan` sits
  inside the tier's own files, which only load through that guard. Owed once the suite rework
  lands: a model-free gate that spawns the umbrella under `--disable-module dasvulkan
  --disable-module dasmetal` (the wasm and console shape) so the seam cannot regress silently.
- Repairs on the way, all small: the `wasm_cross` CI lane configures without
  `-DDAS_WASM_MEMORY64=ON` (`wasm_build.yml:209`) so `web/CMakeLists.txt:374` registers no
  examples and the lane is a green no-op; `DAS_MAX_HW_JOBS` is 4 on wasm (`platform.h:629`);
  the two emsdk pins contradict (`wasm_build.yml:188` vs `pages.yml:73`). Done 2026-09-04:
  `get_architecture_name()` answers `wasm64` on a memory64 build.
- **No tuning under a cross target, by construction (landed 2026-09-04).** Two folds answer for
  the artifact a compile EMITS: `get_target_triple()` (the `--jit-target` triple, "" native) and
  `get_target_architecture_name()` (the triple's arch, the host's with no triple). The tune
  framework's inert gate (`tune_aot_gate`) fires on any cross target, so `[tune]`, `[tuned]` and
  `[llvm_code]` emit nothing but reference bodies into a wasm or console exe; the engine's
  kernel-tier `[init]` gates ask the target arch, so a wasm cross-compile from an arm64 box
  registers no NEON tier. The `--jit-target` argument keys the module cache (it sits after the
  `--` separator, which the key ignored, so a cached native compile served a cross one with the
  host's folds). Gate: `tests/jit_tests/cross_target_folds.das`.
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

- **RULED: "dasLLVM absent" is the build configuration's word.** The path guard
  `?llvm/daslib/llvm_tune` satisfied itself off the filesystem, so a source tree configured
  with `-DDAS_LLVM_DISABLED=ON` still loaded the framework. The two scenarios that matter - a
  wasm build, where no tuning may exist, and a console build (PS5 in dagor) with the LLVM
  sources on disk for the PC build but configured out - both need the configure to decide. The
  `llvm` C++ witness module (see "Settled decisions") is that word; every guard is `?llvm`.
- **RULED: a second trait.** `builtin_module_exists` on a shared das module flipped under
  tool-driven compiles (`Module::requireEx` scans the process's promoted-module list, which a
  nested compile never populates), so lint, ast-verify and the MCP checks audited the
  no-framework arm of every guarded file. The ruling keeps `builtin_module_exists` as it was
  (the process registry) and adds `typeinfo module_exists(X)`: the compiling program's own
  library, the same answer on both rails. Every path-guarded das target in the tree
  (`llvm_tune`, `llvm_code`, `dasllama_exchange`, `dasllama_gemm_gen`) now asks `module_exists`;
  C++-module guards keep `builtin_module_exists`. Gate: `tests/language/optional_require.das`.

- Which small-LLM carrier gets the parity fixture for stages 1 and 2 (SmolLM2-135M already has
  a cls_q8 parity cell, `test_parity.das:76`).
- Whether `test_groupn.das`'s independent reference is fp64 dequant (the kq ladder's choice) or
  a second, scalar implementation.
- Whether the stage-2 example is its own daspkg app or a plain `examples/` program.
