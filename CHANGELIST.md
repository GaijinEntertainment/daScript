# daslang Changelog

## 0.6.3 (June 2026)

### New Features

#### LINQ Fusion Engine: `daslib/linq_fold` (#2687, #2689, #2691, #2696, #2697, #2712, #2714, #2721, #2723, #2724, #2725, #2727, #2728, #2729, #2730, #2732, #2733, #2738, #2741, #2750, #2760, #2765, #2766, #2769, #2770, #2772, #2775, #2784, #2785, #2806, #2809, #2822, #2824, #2833, #2834, #2837, #2839, #2841, #2848, #2851, #2852, #2857, #2861, #2862, #2865, #2866, #2875, #2878, #2881, #2883, #2885, #2886, #2887, #2888, #2891, #2893, #2895, #2896, #2899, #2901, #2903, #2913, #2914, #2915, #2917, #2919, #2924, #2926, #2935, #2936, #2939, #2941, #2943, #2945, #2946, #2947, #2948, #2950, #2952, #2955, #3001, #3002, #3003, #3007, #3009, #3011, #3024, #3099, #3100, #3103, #3106, #3110)

`_fold` is a new compile-time macro that fuses an entire LINQ chain into a single splice-emitted loop — no intermediate arrays, no per-stage iterator allocation. It became the universal fusion lane behind the whole `daslib/linq` surface, beating in-memory SQLite on headline benchmark shapes.

- **Operator coverage** — `where` / `select` / counter lanes; aggregates and early-exit terminators (`any` / `all` / `contains` / `first` / `count` / `long_count` / `last` / `single` / `element_at` / `aggregate`); `take` / `skip` / `take_while` / `skip_while`; the order/sort family, `reverse`, `distinct` / `distinct_by`, and `group_by` with `sum` / `min` / `max` / `first` / `average` / multi-reducer plus `_having`
- **Five sources, one engine** — array, `decs` (spliced into unrolled `for_each_archetype` loops, 10×+ over the eager bridge), XML (`from_xml_node`, with DOM-walk inlining + field pruning), SQL (pure pass-through to `_sql`, gated on `_sql` being linked), and JSON (#3001, #3002, #3003) — `from_json` is a by-name flat query source over `JsonValue` trees, with join / join+group_by fusion and a benchmark lane across the sql families. Deferred/handle-buffering materialization keeps XML buffered reducers single-pass
- **Source follow-ups** — XML backward-DOM reverse fusion + reverse child iterators (#3009); `decs` random-index boundary archetype tail — bare `last` and `reverse |> take` (#3011); simplified `read_json_field` via a `?as` slot read (#3007) and const-key `read_json_field` folding (#3024)
- **N-ary `zip`** (#2737, #2738, #2741) — `zip` / `zip_to_array` extended from pairs to 4-through-8-way, array and iterator forms, each with an optional result selector; rides the shared int64 counter lane
- **Architecture** — migrated onto a unified `splice_patterns` pattern table + capability-method `SourceAdapter`s (G1→G3d), collapsing seven bespoke planner tables and the per-source terminator scaffolds into shared `linq_fold_common` / `_array` / `_decs` lanes
- **Tables as the sixth `_fold` source** (#3099, #3103, #3106, #3110) — `each_kv` / `keys` / `values` query sources and a `to_table` sink via a `TableAdapter`, with `group_by` fusion and point-lookup conjunct extraction (`where(key == X && residual)` probes the table directly), plus a JIT inline table-slot walk for keys/values for-loop sources (#3100)

#### LINQ Query Syntax: C# `%linq!` (#2957, #2958, #2960, #2962, #2966, #2969, #2972, #2975, #2978, #2981, #2987, #2990, #2994, #2996)

`daslib/linq_das` adds a C#-style query expression that rewrites to a `_fold(...)` chain at compile time, built on the new **expression-level reader macros** (`%name! … %%`, #2957). Full clause-level parity shipped incrementally:

- `from … in`, repeatable `where` (#2996), `let` bindings (#2981), `select`
- `join … on … equals …` (#2969) and `join … into g` group-join / GroupJoin (#2994)
- multiple-`from` SelectMany, both uncorrelated (cross product, #2975) and correlated (flatten, #2978)
- `orderby` single-key (#2962) and multi-key composite (#2987); `group … by` (#2962); `into` group/select continuation (#2990)
- **Typed sources via `from_in`** — arrays, `decs`, XML, SQL, and JSON (#3001), with iterator output (#2960); SQL sources push `WHERE` / `GROUP BY` / `ORDER BY` down to SQLite

```das
let names <- %linq! from c in cars where c.price > 100 orderby c.name select c.name %%
```

#### Shader-Style Function Flattening: `daslib/flatten` (#3019, #3022, #3029, #3032, #3035, #3036, #3042, #3046, #3048, #3049, #3053, #3055, #3057, #3059, #3061, #3064, #3069, #3073, #3081, #3086, #3088, #3089, #3093, #3182, #3187, #3190, #3193, #3247, #3250, #3254, #3258, #3259, #3264, #3266, #3267)

`[flatten]` is a new compile-time macro that lowers a function to branchless straight-line code — calls inlined, control flow rewritten into masked selects — for shader-style backends that cannot branch.

- **Core lowering** — call inlining (with multi-site callee-local uniquification), branches via masks, fixed-count loop unrolling (capped) over array / `urange` / parallel multi-source loops, `break` / `continue` via loop-scoped + per-iteration masks, compound-assign/increment lowering, and a whitelist of functions kept as opaque calls
- **Const-opt pipeline** (`daslib/flatten_opt`) — algebraic-identity folds (int / float / bool, vector constructors, uint + vector-const operands), SSA-rename const-propagation of single-def locals, pure-builtin folding on const args, constant reassociation with canonical var sort, copy-prop with folded live masks, dead-store / redundant-zero-init elimination — all iterated to a fixpoint
- **Preshader extraction + CSE** (#3069) — uniform-only subexpressions hoist into a preshader prologue and common subexpressions deduplicate, both to a true fixpoint; reassigned vars excluded for correctness
- **Verified by a metamorphic differential fuzzer** — `utils/flatten-fuzz` compares flattened vs original semantics, with an exhaustive `--bool` truth-table domain, read-only module globals, and opt-residual / fold-completeness oracles
- **Shader-backend example** (#3049, #3059) — `examples/flatten` drives the EdenSpark shader graph through flatten (real shaders, capability + feature tiers, `--das` source dump), with AOT support (`das_iterator<MakeStruct>`) and a reference RST page
- **Post-RC1 fusion refinements** — `mad` / `lerp` disassemble-early + re-fuse-late with zero-init materialization (#3073), `lerp` kept whole by default with opt-in `_flatten_expand_lerp` (#3088), division → reciprocal and ctor-lane algebra under `_flatten_no_fast_math` (#3081), horizontal-add → dot fusion + const-minus regroup-to-share (#3089), swizzle-lane provenance folding (#3093), and a `qmatch` / `match`-based matcher refactor of the opt passes (#3086)
- **More const-opt folds + select fusion** — boolean-inversion folds (comparison negation + De Morgan, #3182), `true && X` loop/iter-mask residue const-prop (#3187), and same-condition `select`-accumulation chains fused N→1 (#3190)
- **Horizontal-sum reduction fusion** — adjacent scalar sums pack into `hadd` (#3264), extended with `[3..4]` remainder widths, an opt-in width-2 pack, and producer-vectorization that pulls a shared componentwise unary over the packed lanes (`sin(a)+sin(b)+sin(c)` → `hadd(sin(float3(a,b,c)))`, #3266); const-weighted scalar sums fold straight to `dot` (the fBm reduction, #3267)
- **Scalar-packing loop re-roll** (#3258) — a map-reduce body over independent scalar lanes re-rolls into a single packed vector loop (−51% nodes on voronoi)
- **Reciprocal & const-factor algebra** — a per-pixel `x / U` by a uniform divisor rewrites to `x * (1/U)` with the reciprocal CSE'd / hoisted into the preshader (#3254), and `radians` / `degrees` lower to a bit-exact multiply so an adjacent const factor reassoc-merges; both land alongside an `examples/flatten` backend re-sync onto native core math (dropping ~24 `[stub]` primitives) plus the latest client shader drop (#3193, #3254)
- **Inlined-helper correctness** — `ref2value` of a non-ref value folds to a no-op load, so copy-prop into a swizzle / field / index base no longer trips `error[30921]` (#3247), and inlined-helper accumulators are SSA-renamed (+ a JIT `ref2value` guard, #3250)
- **Opt-pass dedup + module split** (#3259) — ~10 copy-pasted child-enumeration ladders collapse onto three shared structural-walk primitives with a hard guard on unhandled nodes, and `flatten_opt` splits into a common layer plus 6 themed pass modules behind an unchanged `daslib/flatten_opt` aggregator

#### dasSpirv: Pure-daslang SPIR-V Shader Backend (#3133, #3137, #3139, #3141, #3146, #3149, #3152, #3153, #3157, #3158, #3159, #3160, #3161, #3164, #3166, #3169, #3170, #3171, #3184, #3189, #3195, #3196, #3203, #3207, #3208, #3219, #3222, #3223, #3228, #3232, #3238, #3243, #3244, #3249, #3252)

A new in-tree module that compiles daslang straight to SPIR-V — an AST→SPIR-V shader backend written entirely in daslang, shipped across a long phased arc.

- **Pipeline tiers** — vertex / fragment shaders (#3139) and a compute tier with shared memory, barriers, and atomics (#3166); control-flow + arithmetic breadth (#3137), the ternary / vector-arith / GLSL.std.450 language pass (#3157), and user-defined shader functions via `OpFunctionCall` with ref params (#3170)
- **Resources** — UBOs, matrices, push constants, samplers (#3146); SSBO std430 vector/struct elements (#3159); textures + resource breadth (#3153), `imageSize()` and module-scope shader constants (#3169); const arrays with dynamic indexing (#3141) and composite/global swizzle (#3160)
- **Fragment realism** — `discard`, derivatives, `textureSize`, `gl_FragDepth` (#3161), and depth-compare (shadow) sampling via `Dref` (#3164)
- **Producer side** — shader reflection emitted alongside the SPIR-V blob (#3149); the emitter ported to `AstVisitor`, behavior-preserving (#3152)
- **Docs + goldens** — a SPIR-V reference docs set with a Phase 9–15 plan (#3158) and backfilled golden tests closing the Phase 10.1–10.6 gap (#3171)
- **Mesh + task shader stages** — `[mesh_shader]` / `[task_shader]` with the `MeshShadingEXT` capability + `SPV_EXT_mesh_shader` and a per-module SPIR-V 1.4 header (#3219); output budget / topology via `[mesh_shader(max_vertices=…, max_primitives=…, output_topology=…)]` (#3222); per-vertex / per-primitive output arrays as the first array-of-builtin path (`gl_MeshVerticesEXT[].gl_Position`, `gl_PrimitiveTriangleIndicesEXT[]`, `gl_MeshPrimitivesEXT` / `gl_PrimitiveID`, `@per_primitive` user varyings) (#3223, #3228); and a task→mesh `@task_payload` (`TaskPayloadWorkgroupEXT`) with the workgroup-grid builtins now valid in mesh/task stages (#3232)
- **Deferred-rendering rail** — MRT (multiple `@location` fragment outputs, one `Location`-decorated `Output` each, #3195), subpass input attachments (`subpassInput` + `subpassLoad`, `OpTypeImage Dim=SubpassData` + `InputAttachment`, fragment-only, #3196), and a `Flat` decoration auto-emitted on integer varyings with explicit `@flat` honored on floats (#3203)
- **GPU-driven + bindless builtins** — `gl_PointSize` (BuiltIn `PointSize` for POINT_LIST splat width, #3189), `gl_DrawID` (BuiltIn `DrawIndex` + `DrawParameters`, the multi-draw ordinal, #3207), and `nonuniformEXT` (`OpCopyObject` + `NonUniform` under `ShaderNonUniform` / `SPV_EXT_descriptor_indexing` for divergent descriptor indices, #3208)
- **Arithmetic + interface breadth** — `vec *= scalar` routes through `OpVectorTimesScalar`, and `OpEntryPoint` lists every referenced global under SPIR-V ≥ 1.4 (#3238); scalar-operand broadcast (`OpCompositeConstruct` splat) for vec-result GLSL.std.450 ops like `lerp` / `clamp` / `min` / `max` (#3243); integer `ivecN * int` via splat + component-wise `OpIMul`, plus bool literals lowering to `OpConstantTrue` / `OpConstantFalse` (#3244)
- **Storage-image formats + local-struct reads** — a storage-image `@format` qualifier (`r32f` / `rgba16f` / `rgba32f` / … mapped to `SpvImageFormat`, e.g. R32f Hi-Z depth pyramids, #3249), and member access on a local struct value (`let e = ssbo[i]; e.field` via `OpCompositeExtract`) (#3252)
- **Bind-gen + host layout helpers** (#3184) — abstract bind-generation virtuals for the emitter, plus a public std140 `compute_block_layout` helper so host backends can lay out uniform blocks

#### Fixed Arrays as Structural Types (#3075, #3095, #3097)

`int[10]` is now a structural type (`Type::tFixedArray`) — one node per dimension, with the element in `firstType` and the size in `fixedDim` — replacing the old `TypeDecl` `dim` / `dimExpr` qualifier vectors. Generic binding (`auto(TT)` binds the whole array, `auto(TT)[]` peels one level), C-style `safe_addr` decay, typedef composition, and the one-peel rule all fall out of the structural form; the runtime `TypeInfo` stays flattened for ABI compatibility (#3095). Landed behind a stage-0 characterization suite + target spec (#3075) and documented across CLAUDE.md / skills (#3097).

#### 64-bit Arrays and Tables — the `longarr` arc (#2743, #2746, #2755, #2762, #2764, #2766, #2768, #2771, #2773, #2779, #2899, #3175, #3176, #3177, #3178, #3179)

`array<T>` / `table<K;V>` capacity, length, and index were widened to `int64` internally so collections can exceed `INT_MAX` / 4 GB.

- New `long_length` / `long_reserve` / `long_capacity` / `long_count` / `long_find_index` / `long_iter_range` shadow builtins, `_i64` AOT helpers, and `das_array_*_i64` / `das_table_*_i64` C-API entries
- `int | int64` disjunction-parameter dispatch (plus `typeinfo is_int` / `is_int64` / `is_float` / `is_double`) keeps the daslang surface single-overload while forking by base type; `builtin` / `array_boost` / `json_boost` / `strings_boost` / `decs` widened
- The 8.5k-test existing surface is preserved via shadow names rather than a hard widen; `MemoryModel` / heap byte counts and `alignMask` widened to 64-bit with 4 GB-cap guards
- A post-RC1 pass widened the remaining container-internal 32-bit choke points: the `any_array_foreach` / `any_table_foreach` AST-conversion iterators (#3175), `any_array_long_size` / `any_table_long_size` accessors with panic-on-overflow 32-bit shadows (#3176), the runtime sort / heap any-path for struct-element arrays (#3177), `popen_argv` argv handling (#3178), and the jobque `Stream` byte-buffer push / pushBatch / pop API (#3179)

#### Sort Family: pdqsort, `<algorithm>`, and `stable_sort` (#2706, #2707, #2986)

- **`<algorithm>` bindings + `top_n`** (#2706, #2707) — new `sort_boost` API (`partial_sort`, `nth_element`, `make_heap` / `push_heap` / `pop_heap`, each with optional comparator) over a typed `das_sort<T>` block-partition pdqsort C++ binding, plus `top_n` / `top_n_by` selection in `daslib/linq`; array + iterator sources
- **`stable_sort`** (#2986) — new adaptive natural-run merge (timsort-lite, galloping merge): `das_stable_sort_r` (byte) and typed `das_stable_sort<T>`, with AOT support, tests, bench, and docs

#### dasOPENAI: OpenAI-compatible API Client (#2938, #2942, #2944)

New pure-daslang module over dasHV / json_boost — chat (streaming + tools + structured outputs), embeddings, models, completions, audio TTS/STT, moderations, images, and vision. Works against OpenAI, Ollama, OpenRouter, vLLM, and LM Studio. Streamed tool-call fragments are reassembled by index (#2944). Ships 12 examples, 7 mock-server-backed tutorials, and an RST reference.

#### New Libraries and Transports

- **`daslib/jsonrpc` + stdio live transport** (#2674, #2679) — unified JSON-RPC 2.0 library (request/response, line-dispatch, §6 batch) and a JSON-RPC stdio transport for daslang-live commands that needs no dasHV
- **`daslib/logger`** (#2908) — JSON-Lines structured logger; `logger_install_hook()` diverts `print` / `to_log` to a file (critical for stdio-transport processes like MCP). Adds C++ `iso8601_now()` and `popen_argv_pipe()` (bidirectional, shell-bypass subprocess pipes)
- **`daslib/toml` + `.lint_config`** (#2884) — TOML-1.0 parser (closed-table semantics); repo-level `.lint_config` (TOML) now configures lint, with STYLE005 off by default
- **`daslib/with_boost`** (#2880, #2889) — `with_` call macro binds one or more `arr[i]` / `tab[k]` element references inside a block under an automatic `lock`, so push/erase/resize panic instead of dangling; any arity, mixed array + table
- **LINQ over XML** (#2920) — `from_xml_node(root, type<Row>)` typed LINQ source over XML child elements in `dasPUGIXML` / `PUGIXML_boost`

#### C, Graphics, and Audio

- **C-API arrays & tables** (#2619, #2779) — `das_array_*` / `das_table_*` (with `_i64` completeness) build, lock, and borrow daslang collections from C; integration tutorial 14
- **APNG recording pipeline** (#2630, #2645, #2951) — streaming animated-PNG writer in dasStbImage (`stbi_apng_*`), `record_start` / `stop` / `status` live commands, an async PBO ring in dasOpenGL, and a synchronous frame-exact writer with fixed-delay capture
- **dasGLFW synthetic input** (#2630, #2633) — chain dispatcher plus synthetic mouse / keyboard / char input; `glfw_live` mouse and keyboard drivers (held-input release on stop)
- **dasAudio HRTF + simulated-3D mixing** (#2879) — `set_hrtf_budget(n)` routes the closest channels to HRTF slots and the rest to constant-power-pan / distance-attenuation 3D, with anti-flap margins, calibrated gain, and cross-context stats
- **dasStrudel widening + combinators** (#2995) — every scalar pattern modifier accepts `int | float | double`; exposes `set_X`, adds `freq` / `euclidRot` / `chop` / `slice` and `bd(3,8)` Euclidean mini-notation
- **dasStrudel one-shot patterns** (#3241) — `playFor(pat, n)` / `once(pat)` gate a pattern to its first `n` cycles then silence (a pure transform), and `strudel_one_shot(pat, gain, quant_cycles, len_cycles)` fires a finite phrase as a self-retiring track — the start snaps to the global beat grid and the track auto-removes once every voice has finished ringing, so the release tail is never cut (fire-and-forget stings); plus `strudel_active_tracks()`
- **dasStrudel per-orbit gain** (#3225) — each orbit gains a continuous, ramped output level (`fade_orbit` / `set_orbit_gain` / `orbit_gain` + `strudel_*` wrappers) multiplied per-chunk into every routed voice's gain, so a fade has no per-note latency however sparse the layer is; the level-1.0 no-fade case stays byte-identical — for adaptive music that crossfades layer groups on one scheduler
- **dasStrudel adaptive-audio docs + Stolen sf2 bank** (#3194) — documents the generic `signal(fn)` live-gain primitive and a host-driven / adaptive control pattern (driving pattern parameters from live host state), and adds the Stolen Soundfont GM bank to the `midi_sf2_demo` README
- **dasAudio output safety** (#3215) — audio voices guard against NaN/Inf and avoid amplifying near-silence

#### OpenGL/Vulkan Tutorial Ladder + WebGL2 Playground (#3268, #3270)

A renderable OpenGL tutorial ladder that mirrors the dasVulkan series shader-for-shader, all written in daslang and lowered to a third backend (GLSL / GLSL ES 3.00) — the same shaders run live, click-to-run, in the browser playground.

- **`daslib/shader_lingua_franca`** — a new shared builtin module holding the backend-invariant shader surface both emitter cores (`dasGlsl` → GLSL text, `dasSpirv` → SPIR-V binary) `require ... public`, so existing `glsl_common` / `spirv_builtins` consumers see the symbols unchanged. Only the modern WebGL2 / GL-4.1-core surface both rails speak lives here — the shared union grew across the arc (`fwidth`, `gl_FragCoord`, `gl_FragDepth`, `gl_VertexIndex` → GL `gl_VertexID`, `gl_InstanceIndex` → GL `gl_InstanceID`, `textureLod`, `texelFetch`, and the portable cube / 2D-array / 2D-shadow samplers)
- **GL rail accepts the Vulkan `@`-annotations** and emits a `with`-body instead of rejecting it; the loose struct-uniform binder is completed (`mat3` + array-of-struct). `dasGlsl` emitter holes filled rung-by-rung — integer-valued float literals emit a decimal point (ES strictness), es300 default precision for `sampler2DShadow` / `sampler3D` / `sampler2DArray`, a `_discard` wrapper in every GLSL version, `layout(location=N)` on fragment outputs (MRT), and `samplerCube` / `2DArray` / `2DShadow` `:=` bind operators
- **WebGL2/wasm late-extern GL call path** — daslang's late-extern path type-erases every arg and ABI-puns the indirect call, which traps under strongly-typed wasm `call_indirect`; on emscripten each `gl*` call now routes through an exact-typed wrapper whose static C signature comes from the real GLES3 header via `decltype(&::glX)` (also fixing the `GLsizeiptr` / `GLintptr` 64→32-bit width mismatch), generated into `src/builtin/gl_wasm_wrappers.inc` by `gen_gl_wasm_wrappers.das`
- **Browser run lifecycle** — `dasGLFW` builds and links under emscripten (`-sUSE_GLFW=3`), `dasGlsl` emits GLSL ES 3.00 on the wasm target, and the emscripten main-loop drives a 3-call init/update/shutdown lifecycle with the daslang `Context` persisting across animation frames — so the very same `.das` runs live in the playground

#### dasAudio SFX Lab: Procedural One-Shot Sound Design (#3215, #3236, #3242)

A new interactive GUI workbench (`examples/daStrudel/sfx_lab/`, dasImgui + dasImguiImplot) for designing one-shot game SFX, backed by new procedural-synth voices in dasAudio. daspkg-packaged (installs dasImgui / ImPlot, no `-load_module`).

- **New synth voices** — `strudel_sfxr`, a faithful sfxr/DrPetter one-shot generator (square / saw / sine / triangle / noise, attack/sustain/punch/decay envelope, pitch slide + arpeggio + vibrato, square duty, retrigger, phaser, low/high-pass filter; deterministic mono `array<float>` at 44100, plus the classic 7 presets, randomize/mutate); and `strudel_modal`, a modal-impact voice (a bank of inharmonic decaying-sine resonant modes kicked by a contact transient, for struck/resonant sounds sfxr can't cover)
- **Multi-layer sound model** (`strudel_sfx`) — a sound is a stack of layers, each an sfxr or modal voice (a `VoiceParams` variant) with its own gain + onset delay; `render_sound` mixes them peak-normalized, with `link_source` authoring metadata and `propagate_links`
- **The workbench** — section-grouped param sliders, presets, randomize/mutate/autoplay, live waveform + spectrum (dB) + log-freq spectrogram ImPlot charts, a 10-effect postprocess chain, `.sfx` save/load, an undo/redo stack (menu + Cmd-Z), and export of the current sound as a drop-in `.das` render function (the strudel drum voices were re-authored from these)
- **Reference-driven fitting** — load a reference clip and the lab computes a composite spectral / temporal-shape / attack / texture / pitch fit metric, with an annealed (1+λ) ES auto-tuner that anneals layer params toward the reference (never touching the user's gain mix) and analysis-driven seeding
- **Loader + WAV breadth** — reference decode goes through miniaudio (WAV / OGG / MP3 / FLAC) like strudel playback (#3242); a button-triggered silence trimmer with a dB-below-peak gate (#3236); `audio_wav` `read_wav` now decodes 24/32-bit signed-integer PCM (was 16-bit only), rejecting extensible/float WAVs and bounding malformed chunks (#3236)

#### dasMinfft: DCT-II/III + JPEG Block-Compression Demo (#3255)

The `dasMinfft` binding previously exposed only the real FFT; minfft ships DCT-II/III in its C library but they were unbound. #3255 binds them via a reusable plan handle.

- **DCT API** — a `dct_plan` managed handle (wraps `minfft_aux`) with `make_dct_plan_1d` / `make_dct_plan_2d`; `dct` (forward, DCT-II) and `idct` (inverse, DCT-III) share one plan. Transforms are unnormalized — `idct(dct(x)) == 2N` per axis (so an 8×8 round-trip is `/256`)
- **JPEG-pipeline demo** (`examples/minfft/dct_jpeg.das`) — the JPEG/MPEG 8×8 block-DCT pipeline (DCT → JPEG-luma quantize → dequantize → IDCT) with a quality knob, PSNR, and zeroed-coefficient stats, saving before/after PNGs via dasStbImage

### Improvements

#### Language and Compiler

- **Implicit integer-literal promotion** (#2876) — bare int literals now silently promote to any fitting numeric target (`int8/16/64`, `uint8/16/32/64`, `bitfield*`, `float`, `double`) at infer time across assignments, returns, var/struct/make-* inits and binary ops; range overflow is `error[30515]`, and `LINT011` warns on precision loss
- **`enum_trait` `operator !` / `bool()` for any enum** (#2825) — flag-op enums read like bitfields (`if (!(flags & MASK))`), width-safe across all backings; also folds const-foldable enum exprs at infer time
- **Lambdas are copyable** (#2731) — `=` / pass-by-value / `push` copy the pointer (alias, no `push_clone`); `delete lam` now requires `unsafe`, and the rule cascades to containers/structs holding a lambda
- **C++ keywords as identifiers** (#2735) — any C++ keyword that is not also a daslang reserved word can name vars, structs, fields, enums, and typedefs (`register`, `union`, `short`, …); C++-keyword function names compile via AOT mangling (#2655)
- **Unnamed → named tuple promotion** (#2642) — a positional tuple literal of bare variables is promoted to a named tuple when the names match the target fields
- **Optional `require`** (#2921) — `require ?guard target` loads `target` only when module `guard` is linked, else silently skips; pairs with `typeinfo builtin_module_exists`
- **One-liner block / EOF parsing** (#2925, #2651) — a single-line brace block no longer needs a trailing `;` before `}`, and a gen2 file whose final statement has no trailing newline now parses
- **Arrow bodies on methods** (#3051) — the `=> expr` single-expression body now parses on struct/class methods
- **Piped calls pad defaults** (#3071) — default arguments are filled in front of a trailing piped block, so `foo() $ { … }` resolves when defaulted params precede the block
- **User operators over mismatched enums** (#3010) — a user-defined operator on two different enum types now wins instead of tripping `error[30145]`
- **Native `empty()`** (#3044) — `empty(array)` / `empty(table)` are native builtins with a JIT intrinsic
- **`describe_function` / `describe_expression` emit gen2 syntax** (#3047)
- **`class` super-call discipline** (#2892, #2900) — CFG-aware lint requires exactly one `super(...)` on every control-flow path and rejects skipping an intermediate class; a chaining default ctor is synthesized for empty-derived classes only when safe (`options always_call_super` removed)
- **`uint8` / `uint16` / `uint64`-backed enum casts zero-extend** (#2739) — no longer sign-extend to negative values; fixed across interpreter, AOT, and JIT
- **Faster compile** (#2849, #2853) — hoisted per-module overload visibility + a lazy `findAlias` subtree cache (~19% faster), with the scratch bits excluded from semantic/AOT hashes
- **Clearer diagnostics** (#2989, #2650, #2626) — type-mismatch errors print short alias names with a single `where '<alias> = <def>'` clause (#2989, fixes #2571); assume-alias errors report at the use site (#2650); better for-in iterator-exhaustion message
- **`ref_time_ticks` → nanoseconds** (#2685) — monotonic ns on every platform, so raw subtraction is valid everywhere
- **`quote` / `qmacro` lowering under AOT and JIT** (#3109, #3114) — revived the `aot_macros` lowering so quote-built ASTs compile and run AOT, then extended it to the JIT lane (`jit_enabled` triggers `QuotePass`)
- **Template-structure / default-arg robustness** (the `#3065` arc) — reject a template structure used without instantiation (#3134) and skip them in const-fold (#3136); a clean error instead of an assert on a mismatched default argument (#3138); reject a local reference bound to a temporary value (#3142)
- **`class` finalizers chain through inheritance** (#3085) — like constructors, a derived finalizer now runs base finalizers
- **`describe()` is deterministic** (#3092) — output no longer depends on print history
- **`$i(...)` in every name-list slot** (#3120) — the splice tag is accepted everywhere a name list is parsed
- **`match` / `ast_match` AST-pointer pattern fixes** (#3082) — correct matching on AST-pointer shapes and qualified call names; a module-name grammar-rule fix (#3083)
- **Heap-built array / table literals** (#3174) — a gen2 array/table literal feeding `to_array_move` / `to_table_move` builds the `array<T>` straight on the heap (1 alloc/op) instead of materializing a stack fixed-array and copying it; in-place CMRES fills across interpreter / AOT / JIT, with an `options no_heap_array_literals` opt-out
- **Native math breadth** (#3181) — `step` / `smoothstep` plus a Tier-B dispatch sweep (hyperbolic, `fmod`, `mad`, `radians`, `degrees`) bound natively across the `math` module, dasSpirv, and `llvm_jit` (consumed by the flatten shader example)
- **Horizontal vector reductions** (#3261) — `hmin` / `hmax` / `hadd` (componentwise min / max / sum of a `float2`/`float3`/`float4`) across all three tiers: interp + AOT wrap the vecmath horizontals, the JIT lowers to `llvm.vector.reduce.*` (bumps `LLVM_JIT_CODEGEN_VERSION`); also fixes a NEON `v_hadd4_x` integer-add-across bug and lets `flatten` emit them for packed min/max spines

#### JIT, AOT, and Build

- **Per-loop LLVM hints** (#2634, #2635, #2637, #2638, #2639) — `for` / `while` loops accept `unroll` / `vectorize` / `unroll_count=N` annotations driving `!llvm.loop.*` metadata; plus host-CPU TargetMachine, `mustprogress` backedges, cold panic paths, `math::length` intrinsic, and declared allocator memory effects
- **LLVM update + cross-compilation** (#2631, #2643, #2797, #2821) — bumped LLVM bindings, host-target-only initializers, and WASM JIT/exe backend support
- **LTO in Release** (#2858) and **clang-mingw64 toolchain** (#2838, #2840, #2846) — core + 7 modules + JIT build under clang-mingw64, with a Windows CI worker and dasClangBind regen (libclang 22.1.5)
- **JIT robustness** (#2793, #2808, #2811) — non-zero process exit on JIT failure/leaks, `-jit-no-cache` opt-out, and a `jit_table_at` DLLExport cache-hit fix
- **AOT robustness** (the `#3065` arc) — disambiguate C++ names for a same-named struct and enum (#3143), bump the emitter stack and fail closed on a codegen exception (#3145), error when an AOT'd file requires a module-less `.das` (#3150), const-owner handled-string field reads cast to `char *` (#3105), and an inline `testFoo` so AOT callers resolve it (#3140)
- **`jit` joins the completion status before leaving `run_parallel`** (#3124) — fixes a "synch primitive deleted (ref=1)" teardown race
- **Library split pt3** (#3156) — `FileAccess` + the analysis passes moved into the compiler library
- **`das-fmt` verify-compile loads `daslib/builtin`** (#3148) and the formatter recognizes the `$name<types>(args)` type-macro form (#3087)

#### Hash Tables: Open-Addressing Overhaul (#3013, #3017, #3020, #3025)

`builtin_table` got a ground-up performance pass across interpreter, JIT, and layout.

- **Small tables** — packed mode with a 64-bit per-slot hash and hash-only string compare, eliminating `strcmp` from the lookup path
- **Non-string keys** — open-addressed only; packed tables store no per-slot hash at all, and large tables use a 1-byte control state instead of a 32-bit hash
- **Interpreter** — const-string-key fast path (`SimNode_*_WithHash` precomputed hashes) with OP1-SET fusion across the full container-shape set
- **JIT** — packed find inlined as a `<8 x i32>` SIMD scan (per-key-type: early-out for strings, SIMD for ints), a fast 16-bit-load string-hash intrinsic, inline find/at for large string tables, and find-glob baking in the standalone-exe path
- Plus a large-table benchmark suite and two `-flto` miscompile fixes (unaligned packed-hash access on clang, loop-instead-of-recursion `reserve` on gcc)

#### Memory: AST GC, Escape Analysis, and Stack Allocation (#3040, #3015, #3045, #3067, #3032, #3035, #3036)

- **AST GC during compile** (#3040) — each module owns a swappable GC root and the working root is collected+swapped during infer (default on), with per-stage histograms grouped by source `at` and live-vs-garbage end-of-module diagnostics; flatten's twin lowering runs under a per-pass GC scope, and the opt passes reclaim orphaned AST mid-pass (#3035, #3036)
- **Escape analysis** (#3045, #3067) — non-escaping pointer locals get GC-equivalent scope-free (with precision for pointers passed to pure builtins), and a non-escaping `new` is stack-allocated (`allocate_on_stack`, now default on)
- **Runtime GC** (#3015) — popcount-based live-slot counting in sweep, bounded mark-walk recursion (deep-data overflow safety), per-cycle heap GC timing, and proper GC benchmarks
- **Shoe allocator** fully migrated to 64-bit (#3032)

#### Runtime Libraries, Tooling, and Hosting

- **sqlite_linq LINQ-to-SQL coverage** (#2845, #2847, #2906, #2909, #2910, #2928, #2979, #2984) — `long_count` → `COUNT(*)`, `_distinct_by` as a chain operator + `COUNT(DISTINCT …)`, expression keys in `_group_by`, `_.<alias>` resolution after `_join`, computed-scalar `_select`, take/skip-before-aggregate subqueries, and workhorse-cast → SQLite `CAST` pushdown
- **`daslib/linq` mixed-source overloads** (#2931, #2932, #2934) — binary ops (join/union/intersect/except/concat/zip) and their `*_to_array` forms accepting one array + one iterator in either direction
- **daslang-live** (#2681, #2693, #2726, #2859, #2976) — post-`--` argv forwarding, `-project_root`, `--live-port=N` with a port-keyed lock, `POST /reset` (re-simulate without recompile, + a `reload_generation` counter), and a ModuleGroup-lifetime crash fix
- **Dynamic modules** (#2774, #2956, #2937, #2991) — `-load_module <path>` loads a module straight from its folder (no junction); order-independent loading via deferred-`dlopen` fixed-point retry; `compileDaScript` verifies modules are initialized; a `module_file_access` `option_blocked` hook
- **MCP** (#2620, #2701, #2718, #2908) — per-tool subprocess isolation, `project` / `project_root` threading through every tool, and stdio-audit hardening (logger + `popen_argv_pipe` + stdin isolation)
- **dasHV** (#2668, #2684, #2788, #2980) — enabled by default, env-gated log routing, libhv pin bump (trie-router fix), and async (non-blocking) HTTP handlers fixing a `stop()` / teardown deadlock
- **dastest** (#2894, #2897, #2968, #2974, #2985, #2988) — semi-isolated `--batch` mode, `--worker-index` / `--headless` forwarding (1-based, keeping live-API 9090 free), full pre-`--` flag preservation, crashing-subprocess stderr capture, and `--stack-on-exception`
- **das_hash_map** (#2716, #2722) — insert-only `map` / `set` variants (no tombstones/rehash), EASTL/no-exception build support, used for `Module::annotationData` / `requireModule`
- **JSON** (#2626, #2871, #2965, #3034) — lazier `JV` serialization, `vec2/3/4` emitted as `{x,y,z}` objects to match `JV<vec>`, OR-combined flag enums serialized as their numeric value (was invalid empty), and a GC-pressure pass (#3034) — `delete_json` / in-place `update`, a `?[]` miss sentinel, and dispatch frees its trees
- **`apply` hybrid inlining** (#3008) — inline/invoke field iteration fixing an AOT return-mismatch, plus `apply_imm`, a struct-only inlining applicator (scope + assume, no block/invoke)
- **Recording** (#3028, #3030) — lockstep capture via a single fixed-dt clock, and daslang-live keeps rendering on macOS during capture
- **dasStrudel nearest-tick scheduling** (#3014) and **dasAudio looping decoders** (#3066)
- **cbind** (#3006) — constants generated from preprocessor-active macro cursors, plus a `keep_c_arg_type` hook to pin an arg's raw C type
- **`from_decs_template`** (#2745) and **`DelegateReturn` made public** (#2624)
- **rtti self-contained annotation info** (#3101) — `AnnotationInfo` / `AnnotationArgumentInfo` carried in debug info, plus access-detail flags on `Variable` access info (#3118)
- **`format_time(clock, fmt)`** (#3132) — local-time `strftime` rendering
- **dasAudio convolution-reverb performance** (#3168) — quality tiers, an idle gate, SIMD, and a Freeverb low tier
- **Module fixes** — `opengl_boost` `glBufferData` over fixed arrays (#3119), `fio` reports broken dynamic-module artifacts when the context is null (#3121), and `glsl` maps `atan2` to GLSL `atan` (#3123)
- **dasGLFW `glfwInitVulkanLoader`** (#3183) — hand GLFW the app's Vulkan loader `vkGetInstanceProcAddr` before `glfwInit` so `glfwVulkanSupported()` / `glfwGetRequiredInstanceExtensions()` work when the loader sits off GLFW's dlopen search path (Homebrew Vulkan on macOS); bound through a `void*` shim, no Vulkan dependency added
- **dasGLFW native macOS fullscreen** (#3251) — `glfwToggleNativeFullscreen` / `glfwIsNativeFullscreen` drive `[NSWindow toggleFullScreen:]` via the objc runtime (plain C++, `#if __APPLE__`), giving the real green-button / Ctrl-Cmd-F fullscreen Space; non-Apple platforms get stubs
- **Unified `Module_FIO`** (#3186) — `DAS_NO_FILEIO` builds register the same fio API as full builds and raise a recoverable script error (or empty result) instead of hard-crashing via `DAS_FATAL_ERROR`; `FStat` gained a no-fileio fallback
- **`daslib/linq` n-ary `concat`** (#3224) — extended from binary to 3-through-8 sources (array and lazy-iterator forms, plus `concat_to_array` / `concat_inplace`), mirroring `zip`; every appended source sub-folds in the `_fold` chain (`concat` = `UNION ALL` to `union`'s `UNION`)
- **Standalone game-loop GC + stable `get_dt`** (#3245) — a standalone `daslang game.das` loop (no live host) reclaims memory via `glfw_live`'s `maybe_collect_gc()` (collects only a mostly-free heap; no-op under the live host) and `get_dt()` computes the frame delta once per frame; new `heap_total_allocated` / `string_heap_total_allocated` builtins
- **MCP server expansion** (#3199, #3204, #3210, #3214) — worktree bootstrap (`utils/mcp/setup.das` regenerates the gitignored `.mcp.json` / `sgconfig.yml` / grammar / binary a fresh clone lacks); a clangd-free C++ syntax-check tier driven by the CMake compile DB (`cpp_compile_check` / `cpp_build_info` / `cpp_format_file` / `cpp_status`); a data-driven `ToolDef` registry replacing three hardcoded if/elif ladders and splitting the monolith into shared core + `das` / `cpp` registration modules; and a standalone AOT `cpp-mcp` binary downloadable without the daslang toolchain
- **daslang-live startup instrumentation** (#3226) — four `[startup]` phase timings (compile / simulate / init / first-frame) logged to host stdout so a slow boot on a starved CI runner bisects unambiguously

#### Lint

A large rule-and-infrastructure expansion, swept clean across daslib / modules / tutorials / utils.

- **New rules** — PERF018 (direct iteration over `range(length)`), PERF019 (collapse double int-casts on bitfield ORs), PERF020 (redundant same-type workhorse cast), PERF021 (hoist common cast out of a ternary), PERF022 (`push_from` / `push_clone_from` / `emplace_from` bulk overloads), PERF023 (drop pre-clone before a qmacro splice), PERF024 (drop pre-clone before a `[clone(...)]`-annotated fn), PERF025 (redundant `string()` in interpolation), PERF014 extension (negated out-of-range char-class); STYLE005 (braceless early-exit), STYLE013 extension (`new`-allocated structs), STYLE020 / STYLE021 (`??` over `from_JV`, named-tuple `JV`), STYLE022 / STYLE023 (bitfield-as-field read/write), STYLE024 / STYLE025 / STYLE026 (redundant / over-broad / nested `unsafe`), STYLE027 (for-push-loop → comprehension), STYLE028 (`self->` inside class methods), STYLE029 / STYLE030 (transitive-only / fully-unused `require`, #3005), STYLE024 extended to recognize `?as` on non-pointer variants (#3007); LINT010 (dead-store), LINT011 (int-literal precision), LINT012 / LINT013 (unused function / block arguments)
- **Infrastructure** — `--enable` / `--disable` rule selection (#2758), the `.lint_config` TOML (#2884), self-spawn parallelization (43s → 14s, #2672), a daslang auto-fixer (~1900 mechanical rewrites, #2627, #2628), a self-lint check (#2813), an O(1) FileInfo line index + unified suppression API (#2670), the `[clone(...)]` function annotation (#2819), and a `.githooks/pre-push` hook mirroring CI (#2761, #2763)
- **Post-RC1 rules** — STYLE031 (table insert-run → table literal) plus a revived dead STYLE021 (#3115), STYLE012 chain form (fold push runs into the constructor or an array literal, #3091), and LINT014 with a LINT003 pass-mutable false-positive fix (#3118, fixes #3090)
- **Post-RC2 rules** — STYLE032 (`var w : array<T>` + a single `push_from` / `push_clone_from` → `var w := src`, with PERF009 also flagging the clone-init `var x := src; return <- x` → `return clone_to_move(src)`) (#3221), and STYLE033 firing on pure-chain receivers (`obj.arr`, `bags[0].nums`) with both STYLE032/033 fresh-empty suggestions gated on `TypeDecl.canClone` so a copyable-but-not-cloneable element is no longer mis-suggested (#3239)

### Bug Fixes

- **ThinLTO SROA miscompile** (#2922) — `atomic_signal_fence` before reading `value.m[idx]` in `das_index<Matrix>::at` stops ThinLTO/SROA reusing a stale register after an aliased-type write (manifested as wrong shoot direction in rel+ThinLTO AOT builds)
- **`eval_single_expression` crash on folded-away constexpr globals** (#2832) — `ExprVar(PI)`-style refs surviving in rtti-driven ASTs no longer deref a missing runtime slot
- **Empty-named AST nodes** (#2890) — new `invalid_empty_name` (`error[30296]`) catches empty/whitespace names across ~10 node kinds (e.g. the `:=`-in-named-arg bug)
- **Fuzzer-found crash hardening** (#3054) — compiler and runtime hardened against the issue-#2570 fuzzer corpus
- **`to_table_move` leak** (#3038) — now moves (frees) its source instead of copying, and nulls aliased pointer keys before delete (the table-comprehension leak)
- **Recursive ref-arg writes recorded in side-effect analysis** (#3037); **empty/dead-body and labeled for-loops no longer mis-select the fused node** (#3043); **`TableKeysIterator` past-end read** (ASan, #3020); **JIT null-operand store when baking workhorse find globals** (#3025)
- **Spurious `error[30151]` on `concept_assert` / `static_assert` under lint flags** (#2835); **clang-mingw64 `-fwrapv` signed-overflow UB** (#2840); **`ManagedVectorAnnotation::walk` TypeDecl leak** (#2863); **`ExprAt` null subexpression-type guard** (#2767)
- **AOT negative int32 index before unsigned wrap** (#2776); **`alignMask` uint32 truncation on ≥4 GB allocations** (#2762); **`long_count` truncation past 2³¹** (#2899)
- **Build / tooling** — dlopen `RTLD_GLOBAL` on POSIX (#2666), GC on serialization failure (#2669), AOT handled-type references (#2657), dasFormatter CRLF re-normalization unsticking the WASM build (#2829), cbind_boost backslash path strip (#2846, #2850), live_api POSIX shutdown hang (#2688), `migrate_93` disk-artifact leak (#2780), uriparser Windows path encoding (#2970), and assorted JIT/wasm-cross regressions (#2821)
- **Static-analysis sweeps** — six PVS-Studio findings in ast / parser / fio (#3111), four CodeQL integer-overflow findings plus vendored-SARIF filtering (#3112), and gcc `-Werror` breaks that leaked to downstream package builds (#3162)
- **`SerializeNodeId` pointer top bits** (#3172) — masked instead of asserted-zero (the high-bit assert removal); **`dasPEG` debug-codegen bug** fixed alongside the PEG negative-lookahead work (#3131)
- **JIT / stack crash move-returning a register-sized handled value** (#3201) — `return <- MakeLocal()` of a non-cmres handled value (e.g. `BigEntityId`) was routed through CMRES; the JIT read a nonexistent result parameter and segfaulted, the interpreter crashed in `SimNode_ReturnLocalCMRes`. Now gated on the function actually returning by cmres, falling back to a stack slot to match AOT
- **win32 applocal link race** (#3202) — disable vcpkg's per-target `VCPKG_APPLOCAL_DEPS`: dozens of targets emitting into the shared `bin/` concurrently re-copied the same runtime DLLs and collided under Ninja parallel link; each module already deploys its own third-party DLLs idempotently
- **`collectDependencies` crash on empty dep list** (#3216) — an uninitialised `Array` crossed the das boundary and panicked "array magic mismatch on lock", crashing any `[macro_function]` / `[function_macro]` (dasGlsl, dasSpirv) on a body-empty function with no deps; now always `array_mark_locked`-initialised
- **bison/flex parallel build race** (#3218) — `libDaScript` and `libDaScriptDyn` both list the generated parser sources, so MSBuild ran the bison+flex command twice on the same outputs; funnelled through a single `parser_generated` target both libraries depend on (Ninja was unaffected)
- **clang-cl AOT signed-overflow miscompile** (#3220) — apply `-fwrapv` to clang-cl too: `daslib/random.das`'s LCG overflowed `int`, escaped its `& 0x7FFF` mask, and made `random_float` go negative on the win32 clang-cl AOT lane; the `-fwrapv` gate had wrongly excluded clang-cl as MSVC-`cl`-like
- **finally-block locals clobbered by stack reuse** (#3235) — the interpreter's stack-reuse could hand a finally-referenced local's slot to an earlier sub-scope, so the finally saw garbage (e.g. a defer'd close after an early error return); a block with a `finalList` now keeps its locals in distinct slots, mirroring AOT
- **AOT struct emission order** (#3260, fixes #3212) — the C++ emitter wrote a required module's structs in declaration order, so a by-value field whose struct was declared later referenced an incomplete type and broke the `sizeof`/`offsetof` static_asserts; now topologically sorted (also stamps AOT files with a `// Code generated … DO NOT EDIT.` header)
- **AOT tuple/variant over 256 elements + infer crash on a broken call** (#3262, closes #3151/#3263) — `tuple_size` / `tuple_offset` / `variant_sizeof` built a 256-deep comma fold that blew the C++ expression-nesting limit; rewritten as a flat brace-init pack + constexpr loop. Separately, `deriveAliases` reports a compile error instead of dereferencing a null `func` on a fuzz-malformed un-inferred call

### Site / Blog / Docs / Playground

- **Forge site redesign** (#2648, #2653, #2667, #2665, #2856, #2868) — new landing / blog / playground / mobile site, install-snippet fix, URL history, GoatCounter analytics, logo / slogan / favicon refresh, and a real daslang glyph
- **Playground** (#2749, #2797, #2804, #2810, #2827, #2927) — embedded `dastest` Test button gated on `[test]`, ~18 benchmark samples with a multi-platform cycler and deep-link, a JIT/interp toggle, `?example=` URL override, mobile gate dropped, and a 3-service URL-shortener fallback
- **Algolia DocSearch** (#3060, #3062, #3063, #3068, #3070, #3072) — site + RTD docs indexed by Algolia: `sitemap.xml` via sphinx-sitemap, the crawler config checked in as a versioned reference, per-symbol `nosearch` keeping oversized symbol tables out of the index, signatures-only `ast.html` indexing to fit the 750-record cap, and a single top-bar search box replacing the native RTD search
- **Docs** (#2843, #2870, #2874, #2902, #2911, #2649, #2710) — `linq_decs` tutorial + living `linq_fold_patterns` reference, a stdlib-categorization CI gate (no "Uncategorized" sections), External Modules section, and Sphinx polish; news nav with a "N NEW" chip (#2907); Atom/RSS surfacing (#2682, #2683); `/daspkg` page (#2703, #2705); a factual audit of the language reference fixing wrong claims and broken examples (#3023); SQL tutorials-vs-implementation and LINQ-docs audits (#3012, #3000); external ImGui modules (node-editor + implot) linked from docs + site (#3027)
- **Blog** — "The three horsemen" (#2660), "Internal tools" (#2678), "Do you even sort?" (#2708), "Its like dejavu all over again" (#2747), "Verbose much?" (#2807), "from macro hell" (#2867), "spaghetti of choice" (#2898), "10k tests" (#2930), "Kibble" (#2971), "LINQ? yes, really" (#3021), "It's tables all the way down" (#3052), GFM strikethrough rendering (#2973), and a news tag-chip overlap fix (#3031)
- **Post-RC1 docs + blog** — "Bye-bye Elvis", the `tFixedArray` farewell post (#3113); the 0.6.3-RC1 release news (#3079); a coverage-gap docs refresh (fixed-array model + gen2 currency in CLAUDE.md / skills, #3097); a new `imgui_ui_debugging` skill (#3135); dasVulkan linked from the external-modules index (#3128); and a `blocks.rst` self-contradiction fix on local block variables (#3147)
- **Live WebGL2 playground rail** (#3268, #3270) — the 11 OpenGL rungs ship as playground samples (`gl_01_triangle` … `gl_11_hdr`) that run live in the browser via the wasm 3-call lifecycle, with live preview + re-run teardown; the deferred-shading rung fetches its mesh / PBR textures / HDRI at runtime through a `.das.assets.json` manifest, and the Pages deploy now stages `tutorials/_assets` onto the published playground (fixing the 404 asset fetches that panicked `init()` on daslang.io, #3270)
- **Post-RC2 docs + blog** — a new `external_modules/dasvideo.rst` page (MPEG-1 / AV1 codecs, the borrow API, licensing) in the external-modules toctree plus a dasVideo news entry (#3246); the "Drums. Drums. They are coming." blog post (daStrudel drums / SFX lab) with a blog-index multi-tag TOC-pill fix (#3227, #3229); and the `babysit` skill — `pr_review_iteration` renamed and rewritten around the Copilot-first post-open PR loop (#3240, #3248)

### Build / CI

- **Windows on Ninja + Defender exclusion** (#2961, #2963, #2967) across extended_checks / build / release, with sccache on Windows (#2964, #2982) and a no-filesystem build added to CI (#2959)
- **Caching and time** (#2786, #2815, #2982) — PR build-result caching, a CI-time reduction pass, and vcpkg/openssl caching everywhere
- **JIT CI scoping** (#2977) — skip JIT on win64 Debug / RelWithDebInfo, keep it on Release; **dasLLVM mingw fallback** to a prebuilt download (#2992); **eastl CI build fix** (#2949)
- **clang-cl toolset** (#3018) — OpenSSL builds under clang-cl (VS ClangCL toolset) with a CI job; **libhv static-CRT fix** (#3004) — `BUILD_FOR_MT` gated on `DAS_USE_STATIC_STD_LIBS`, unbreaking /MD CI
- **pre-push hook** (#3056) — format-verify only tracked files (respects `.gitignore`, skips build output dirs); **WSL CI-repro skill** (#3016) — replicate CI failures verbatim in a local WSL distro
- **tree-sitter grammar** updated for 0.6.3 syntax — expression reader macros + optional `require` (#2999); **per-source benchmark files** isolating JIT I-cache contamination (#3026)
- **Packaging** (#2618, #2823, #2826, #2656) — stop publishing non-bundled release zips, a bundle install-rule smoke test (regressing the RC3 `cpp_search_config.das` breakage), `release.yml` narrowed to `[prereleased]`, and `install_name` alias / rpath scrubbing for release dylibs; untracked in-tree `modules/dasImgui` (now daspkg-managed, #2636)
- **Post-RC1 CI** — RC1 full-matrix fixes (z3 dep, Intel drop, Ninja util install, vcpkg SSL DLLs, #3078) and an LLVM release workflow off the same-repo link release (#3080, #3084); revived CodeQL (v3 action, build-mode none, wired to PRs + weekly cron, #3104) and MSVC-analysis advisory suppressions (#3117); a `utils/preflight` runner mirroring CI's gates locally, with preflight + ABI-break-sweep skills (#3098, #3102), a header-change → full cpp-syntax-sweep escalation (#3107), and a nightly daspkg-index sweep of every index package against master (#3108); compiler warnings-as-errors on the core + module targets (#3155) plus a gcc `-Werror` Linux lane (#3165); and `-dasroot` forwarded to spawned child interpreters (#3154)
- **Post-RC2 CI** — moved `ast_infer_type*` / `dyn_modules` / the `ast` module / `Program::optimize` out of the runtime lib into the compiler lib, fixed a das-fmt static-destruction crash, and linked das-fmt against shared `libDaScriptDyn` (#2380, #3197); bumped the clang-cl and `msvc.yml` jobs to the `Visual Studio 18 2026` generator after the windows-latest image moved off VS2022 (#3205, #3206); swapped every workflow off billed larger runners onto free GitHub-hosted runners, moved the mingw + clang-cl Windows builds to a nightly cron, and added sccache to mingw + a shared-OpenSSL cache (#3198, #3200, #3233, #3237); and gave `release.yml`'s `-jit` suite a 900s / 3600s timeout (a cold `.jitted_scripts` cache legitimately runs ~600s) plus stronger Windows Defender exclusions (#3188, #3192)

### Examples and Tutorials

- **dasOPENAI** (#2938, #2942) — 12 examples + 7 mock-server tutorials; **dasStrudel tutorial rework** (#2997)
- **LINQ query tutorial** (#2990, #2994) — `tutorials/language/56_linq_query.das` + RST for the `%linq!` syntax; SQL operator and LINQ-over-XML tutorials (#2979, #2906, #2909, #2920)
- **Fourier viz** (#2695, #2699) — modernized to dasImgui boost-v2 with a dark theme; hash benchmark suite (#2722)
- **Post-RC1 tutorial sweep** — closed tutorial gaps (toml / logger / `add_module_option`) under a full factual-correctness audit (#3125), with stale-comment + panic-model followups (#3126) and five verbatim blocks converted to `literalinclude` for drift-proofing (#3127); Tier-A binding-module tutorials for dasStbImage (TTF + HDR), dasHV (TLS), and dasAudio (status + global) (#3129); dasOPENAI vision, image generation, moderations, and completions (#3130); and a PEG negative-lookahead (`!rule`) tutorial (#3131)
- **OpenGL / WebGL2 tutorial ladder** (#3268) — 11 runnable rungs under `tutorials/opengl/`, each a homogeneous `init` / `update` / `shutdown` + `main` program mirroring the dasVulkan series: `01_triangle`, `02_mandelbrot`, `03_sdf`, `04_cube`, `05_instancing` (1000-cube swarm), `06_skybox` (procedural cubemap + reflective sphere), `07_particles` (transform feedback), `08_shadow` (depth-texture shadow map), `09_msaa` (multisample resolve), `10_deferred` (4-pass deferred + MRT G-buffer), and `11_hdr` (HDR float FBO + Karis bloom); RST pages + doc index, wired into the CPU-run + GL-compile gates
- **dasMinfft DCT tutorials** (#3255) — `tutorials/dasMinfft/` (real FFT, DCT basics, image compression) with paired RST pages, plus `tests/minfft/test_dct.das` passing under interp, JIT, and AOT

## 0.6.2 (May 2026)

### New Features

#### daSQLite: Typed SQLite Query Layer Expansion (#2481, #2485, #2487, #2489, #2492, #2496, #2507, #2509, #2511, #2518, #2524, #2528, #2534, #2551, #2553, #2561, #2563, #2564, #2566, #2568, #2575, #2581, #2592, #2595, #2603)

`modules/dasSQLITE` now extends `daslib/linq` with SQL-backed queries, letting familiar linq-style transforms compile down to typed SQLite operations. This release then broadens that foundation with a large tutorial-driven expansion across the rest of the SQLite layer.

- **Insert + query macros** — typed insert flows, raw `query` / `try_query` helpers, much deeper `_sql(...)` analysis with multi-quantifier (multi-Q) lowering, and a parity audit comparing in-memory linq vs. SQL emission
- **Read-side operators** — `distinct`, `take`, `skip`, ordering, aggregates, grouping, the full join family (`_left_join`, `_inner_join`, `_right_join`, `_full_outer_join`, `_cross_join`), set operations, multi-source queries, and `_in` / `_not_in` against captured collections via SQLite `json_each`
- **Write-side operations** — typed update/delete flows, transaction helpers, UPSERT with schema annotations for foreign keys, indexes, defaults, and computed columns, plus user-defined SQL functions via `[sql_function]` (auto-registered and visible inside `_sql` chains)
- **Custom storage types** — custom adapters plus `@sql_json` and `@sql_blob` field support, JSON-path descent inside `_sql`, and column metadata introspection
- **Schema introspection and migrations** — `[sql_table(schema_from=...)]` + `check_schema` validate live database schema against the daslang struct at compile time; `daslib/sqlite_migrate` ships versioned `[sql_migration]` blocks with typed ALTER macros (`add_column` / `drop_column` / `rename_column`) and full-table rebuilds via `[struct_convert]` + `convert_and_rename` + `[sql_table(legacy=true)]` for non-trivial schema changes
- **Operational SQLite features** — SQL fragment building, `ATTACH DATABASE`, FTS5 (`[sql_fts5]` + `text_match`), and pre-migration utilities

#### Style Lint and Unified Linting (#2386, #2390, #2391, #2417, #2441, #2516, #2517, #2533, #2538, #2612)

New compile-time linting now covers daslang style in addition to the existing correctness and performance passes.

- **`daslib/style_lint`** — detects non-idiomatic gen2 patterns such as unnecessary block pipes, declaration-then-assignment, array literal construction via repeated `push`, and long comment blocks
- **Unified lint flow** — shared warning collection, `nolint` suppression, comment-hygiene rules, and codebase-wide cleanup to drive warnings back down
- **Noise reduction** — one-line style enforcement was relaxed where it produced more friction than signal
- **Nine new perf/style rules** (#2612) — `PERF013`–`PERF017` and `STYLE016`–`STYLE019` extend the rule set with additional anti-patterns surfaced during the codebase sweep

#### Duplicate Detection Pipeline: `detect-dupe` and `find-dupe` (#2491, #2493, #2497, #2502, #2508, #2510, #2522)

New utility tooling helps find and triage duplicate daslang functions across large codebases.

- **`utils/detect-dupe`** — scans files, canonicalizes functions, and clusters exact and fuzzy duplicates with corpus export/import support
- **Pattern filtering** — suppresses boilerplate-heavy false positives and handles generics more cleanly
- **AI-assisted triage** — `utils/find-dupe` consumes detect-dupe reports and asks Claude to partition clusters into real duplicates vs false positives, with JSON and Markdown output
- **MCP integration** — export, duplicate detection, and judgment workflows are exposed to AI tooling

#### `daslib/clargs`: Declarative CLI Parsing (#2454, #2482, #2525)

A new command-line parsing module standardizes how daslang tools read flags.

- **Long + short flags** — generated `--flag` and `-f` aliases, help text, required-flag handling, and array/bool parsing
- **Execution-mode awareness** — `get_user_args()` handles both interpreter `--` invocation and standalone executables correctly
- **Tool migration** — utilities can share one parser instead of hand-rolling `get_command_line_arguments()` logic

#### Memory Leak Tracking and Diagnostics (#2436, #2448, #2449, #2453, #2462)

daslang now has a much clearer leak-debugging story for both language-side and C++-side allocations.

- **Leak-tracker mode** — `--das-profiler --das-profiler-leaks` records live allocations with captured daslang call stacks
- **Cheat-sheet documentation** — unified guidance explains GC leaks, heap reports, smart-pointer tracking, jobque leaks, and handle-registry dumps
- **Profiler and heap tooling** — better reporting and supporting leak-audit work across the runtime

#### daspkg: Project Bundling for Distribution (#2579, #2584, #2588, #2590)

`daspkg release` produces a redistributable bundle of a daslang project — a directory containing the standalone exe, every transitively-required `.shared_module` dylib, every asset matching the project's release globs, and every transitive dep package's release assets. The bundle runs on a machine with no daslang installed.

- **`daspkg release` command** — `release()` hook in `.das_package` declares `release_main`, `release_name`, `release_include` / `release_exclude` globs, and force-included shared modules
- **Exe-relative shared-module resolution** — `daslang -exe` resolves shared modules in three tiers (exe directory → `das_root` → absolute path), making relocated bundles portable
- **`-list-shared-modules`** — auxiliary flag walks `program_for_each_module` and writes a JSON manifest of every dylib the program touches; `daspkg release` consumes it for auto-detection
- **Project-local + relocated-bundle support** — `get_this_module_dir`, the JIT runtime, and the daspkg resolver all honor project-local layouts, so a moved or copied bundle continues to find its modules
- **Native shared deps** — packages can ship platform-specific dylibs alongside their `.das` sources via `release_include_dll`; dasPUGIXML is enabled by default for Info.plist generation on macOS

### Improvements

#### Strudel Audio Engine: Second Wave (#2403, #2423, #2425, #2426, #2427, #2428, #2429, #2430, #2435, #2440, #2457, #2464, #2519)

The new Strudel engine from `0.6.1` received a broad follow-up pass across synthesis, playback, docs, and memory behavior.

- Better synth and drum voices, more SF2 support, unique per-event HRTF binaural 3D positioning, offline/WAV rendering workflows, and further live/demo polish
- Reduced memory usage and multiple leak fixes in the threaded player, visualizer, and shutdown path
- Expanded documentation and examples for the newer audio stack

#### Compiler, AOT, and JIT Work (#2396, #2402, #2405, #2406, #2416, #2422, #2433, #2442, #2445, #2458, #2461, #2480, #2499, #2500, #2460, #2554, #2565, #2569, #2585, #2594, #2596, #2601, #2604, #2605)

A substantial runtime/compiler pass improved code generation, packaging flexibility, and test coverage.

- Continued push toward more daslang-authored AOT logic and cleaner AOT/JIT boundaries
- Better standalone-exe behavior, transitive JIT type resolution, prologue sizing, and function registration
- More build configurations (`RelWithDebInfo`, static PIC), more daslib AOT coverage, and platform-specific AOT fixes
- **Tuple-strict typer** (#2565) — tuple field names are now part of the type, so `tuple<a:int;b:int>` and `tuple<x:int;y:int>` no longer collide and unnamed tuples remain distinct from named ones
- **`Option<T>` / `Result<T,E>` via `[template_tuple]`** (#2601) — both types are now structural named-tuples generated through a generic mechanism, replacing bespoke handling
- **Call-site block arrow body** (#2554) — `def f(...) : T => expr` shorthand parses alongside the existing block form
- **Error reporting audit** (#2596) — diagnostics retagged for consistency, with `Program::deduplicateErrors` suppressing repeated identical errors from the same site
- **`super` walks past empty intermediates** (#2594) — `super` now skips empty intermediate base classes when looking for a method
- **Block-form global variable annotations** (#2604) — `@field` annotations now propagate to globals declared inside `variable { ... }` blocks
- **`cbind` prefixes** (#2585) — generated bindings can be namespaced with a per-include prefix
- **Compiler refactor** (#2460, #2569, #2605) — `Program` and `Context` extracted from `simulate` / `ast` into their own translation units; serialization unified with eden / daNetGame; large-file split for build-time and editor-friendliness

#### GC / AST / Ownership Cleanup (#2400, #2404, #2407, #2409, #2410, #2411, #2415, #2420, #2421, #2470, #2506)

Core compiler internals continued the transition away from older `smart_ptr`-style AST ownership, with the biggest user-facing result being that macros became much easier to write.

- `TypeDecl`, `Expression`, annotations, and related AST paths were moved deeper into `gc_node`-style ownership
- Macro, visitor, binding, and validator code no longer has to fight as many `smart_ptr`-era ownership patterns and pointer conversions
- Tracker stability improved when GC is active, reducing false crashes during debugging

#### Runtime Libraries and Infrastructure (#2393, #2398, #2399, #2412, #2413, #2414, #2431, #2443, #2451, #2455, #2466, #2467, #2474, #2488, #2494, #2495, #2512, #2514, #2536, #2539, #2555, #2572, #2576, #2577)

A broad utility pass landed across libraries, docs, and developer workflow.

- **dasPEG** — more tests, docs, CI coverage, and standalone/LLVM fixes
- **Core libraries** — validated numeric conversions in `daslib/strings_convert`, continued `Option` / `Result` work including non-copyable support, and C API completeness fixes; `daslib/clargs` now returns `Result` from parsing for cleaner error handling
- **Runtime data structures** — new in-tree `das_hash_map` backend to avoid empty-construction allocations and behave better in thread-local usage
- **Path-aware glob in `daslib/fio`** (#2576, #2577) — `match_glob`, `glob`, `glob_filtered`, plus `expand_glob` / `parse_file_list` promoted from the MCP utility into the stdlib; byte-loops collapsed to `find` / `replace` with a new `skills/strings.md` rollup
- **Math: common numeric functions** (#2555) — `sinh`, `cosh`, `tanh`, `asinh`, `acosh`, `atanh`, `log10`, `log1p`, `expm1`, `cbrt`, `trunc`, `hypot`, `fmod`, `remainder` added to the math module
- **das-fmt vendored in-tree** — `utils/das-fmt/` now holds a local copy of the formatter; CI runs against it instead of an external clone
- **Tooling/docs** — filesystem guidance, handle-registry tutorial work, class-boost coverage, integer-returning `main()` for tools, compilation progress reporting, and assorted AST/class-method polish

#### Build, CI, and Web (#2578, #2600, #2602, #2611, #2614, #2616)

- **Tutorials build in CI** (#2578) — every tutorial now compiles in CI as part of the regular check run
- **Web target reuses the main `CMakeLists`** (#2600) — `daslang-web` picks up tests and tracks the same build configuration as the native targets, removing the duplicated CMake graph
- **MCP: parse-aware C++ source-search tools** (#2602) — `cpp_find_symbol`, `cpp_grep_usage`, `cpp_outline`, `cpp_goto_definition` join the existing daslang-side tools, with a configurable search root and git-signature staleness detection
- **MCP: auto-retry unqualified module names under `daslib/`** (#2616) — when a tool call references e.g. `fio` and the symbol can't be found, the server retries against `daslib/fio` before reporting a miss
- **`@live` extends to struct fields** (#2614) — `live_host` now annotates individual struct fields with `@live`, with per-field `init_hash` combining `f.init` and `g.init` so partial-struct reloads stay coherent

### Bug Fixes

- **Build, tooling, and CI fixes** (#2385, #2394, #2395, #2424, #2447, #2521, #2537, #2591, #2617) — package `.gitignore` handling, PEG standalone/LLVM issues, `require` fixes, `dasbind` fixes, glob dependency tracking, documentation/error-position corrections, daslang plugin: completion no longer silently bails when an external binding (e.g. OpenGL, GLSL preprocessor) is in scope, and `find_call_macro` null-deref fix with `macro_call` regression coverage
- **Runtime/compiler correctness** (#2387, #2392, #2486, #2520, #2526, #2531, #2587, #2593) — JIT global-function arguments, handled-type property write propagation, function-lookup cache pointer reuse, `runWithCatch` state cleanup, ASAN/diagnostic follow-up, fix #2583 (standalone-exe shutdown crash), and fix #2582 (top-level `let`-init now correctly registers builtins)
- **Fusion engine** — TSan-safe `v_ldu` via `DAS_LDU_WORKHORSE`, `call1` / `call2` `loadSize` stamped from `fnPtr->debugInfo`, and a TSan suppression for the over-read in fusion call/return shells
- **Language/runtime edge cases** (#2444, #2446, #2463, #2468) — `finally` loop rework, clearer inference failure on bad calls, GCC reference shadow fixes, and strict-weak-ordering cleanup
- **Graphics and platform fixes** (#2540) — OpenGL/package integration cleanup after the daspkg migration work
- **Architecture-specific** — skipped `#pragma float_control` on `__e2k__` to silence warnings on the Elbrus toolchain

### Examples

- **Asteroids** (#2552, #2556) — full asteroid-shooter game with waves, audio, and powerups; later migrated to DECS for the entity model with a polish pass on visuals and powerup VFX
- **Pacman** (#2589) — classic arcade game added to the `examples/` lineup
- **River Run** (#2560, #2562, #2567) — top-down river shooter built across three rounds: initial gameplay (river splits, combat, VFX, audio, HUD, collisions), then scenery / bonus / engine sound / tuning, and finally shadows, pickup burst, rich music, 3D HUD, and a juice pass

## 0.6.1 (April 2026)

### New Features

#### VSCode Extension: dastest Integration (#2279)

The daslang VSCode extension now integrates `dastest` directly into the editor — run tests and benchmarks from inline buttons, see results in the Test Explorer, and get per-test pass/fail feedback without leaving the IDE.

- **Inline run buttons** — click to run individual tests or benchmarks right from the editor
- **Test Explorer integration** — test results appear in VS Code's standard Test Explorer panel
- **JIT & isolated mode settings** — `dascript.jit` and `dascript.isolatedMode` toggles in VS Code Settings, read fresh on each test run (no reload needed)
- **Live logging** — real-time stdout/stderr streaming to a "daslang Tests" output channel (tests were previously silent until completion)
- **Cross-platform** compiler auto-detection (`daslang` on macOS/Linux, `daslang.exe` on Windows)
- **Better error messages** — distinguishes "compiler not found" vs "dastest not found"
- **dastest isolated mode fixes** — forward `--bench` flag to subprocesses, emit full per-test JSON results

#### daspkg: Package Manager for daslang (#2234, #2236, #2238, #2240, #2247, #2248, #2254, #2260, #2323)

A full-featured package manager for daslang, built entirely in daslang itself. Packages are installed from git repositories or a central index, with dependencies resolved transitively and tracked in a lockfile. Native C/C++ modules are built automatically via CMake.

**Core commands:**
- `install` — from local path, git URL, or package index name. Supports version pinning (`@v1.0`), semver constraints (`>=1.0,<2.0`), and branch pinning
- `update` / `upgrade` — re-resolve to latest compatible version with rollback on failure
- `remove` — clean removal with dependency cleanup
- `build` — CMake-based native module compilation with `doctor` for toolchain validation
- `check` / `list` — verify integrity, show installed packages
- `introduce` / `withdraw` — publish/unpublish packages to the community index (`borisbat/daspkg-index`)

**`.das_package` manifests** — executable daslang scripts (not static JSON) that define package metadata, SDK-aware build logic, version resolution, and dependencies via `require_package()`. This means packages can have conditional build steps, platform-specific logic, and programmatic version checks — all in daslang.

**`daslib/daspkg.das` API module** — provides `package_name`, `download_tag`, `require_package`, `cmake_build`, and other functions available inside `.das_package` scripts.

**Global module install** (#2323) — `daspkg install --global` installs packages to `das_root/modules/`, shared across all projects. Local installs auto-use compatible global versions (zero clone, zero build). Version mismatches error with suggestions. C++ runtime shadow detection warns when a module exists both locally and globally — local always wins.

**Ecosystem migration** — modules previously shipped as git submodules (`dasImgui`, `dasImguiNodeEditor`, `dasMinfft`, `dasTelegram`) have been extracted into standalone daspkg packages with their own repos and `.das_package` manifests. The corresponding `DAS_*_DISABLED` CMake flags have been removed. Install via `daspkg install dasImgui` etc.

**Example packages** in `examples/daspkg/`:
- `graphics/` — ImGui example (depends on `dasImgui`)
- `node-editor/` — imgui-node-editor (transitive dep on `dasImgui`)
- `minfft/` — FFT via `das-minfft` index package
- `daspkg-claude/` — Telegram bot using Claude API with compile-check sandboxing

**Infrastructure:**
- Ordered lockfile preserving installation order for sequential upgrades
- Dependency cycle detection with clear error messages
- Logging to `modules/.daspkg.log` with `--verbose` for debug output
- `force_rmdir` — fixes failure to remove git repos with read-only objects on Windows
- `.daspkg_standalone` marker — prevents auto-discovery of standalone-build modules
- 151 unit tests + 70 integration tests

#### daslang-live: Live-Reloading Application Host (#2286, #2290, #2291, #2293, #2295, #2299, #2302)

New `daslang-live.exe` — a thin C++ host with compile → init → update → shutdown lifecycle, automatic GC, pause-on-exception, and hot reload on file changes. The same script runs with both `daslang.exe` (shipped) and `daslang-live.exe` (development).

- **`modules/dasLiveHost/`** — C++ dynamic module providing lifecycle API (`request_exit`, `is_reload`, `get_dt`, `get_fps`, persistent byte store, etc.)
- **`live/` module namespace** — `live/live_commands`, `live/live_api`, `live/live_watch`, `live/decs_live`, `live/glfw_live`, `live/opengl_live`
- **`@live` variable macro** — annotate globals with `@live` for automatic serialization across live reloads, replacing ~60 lines of manual `[before_reload]`/`[after_reload]` boilerplate
- **REST API** — `/status`, `/error`, `/reload`, `/pause`, `/unpause`, `/command`, `/shutdown` endpoints. Unknown endpoints return JSON help listing all endpoints with curl examples
- **Error guard** — `/command`, `/pause`, `/unpause` return HTTP 503 with compilation/exception error text
- **Exception handling** — runtime exceptions during `init()`, `update()`, `[after_reload]`, or live commands enter error state, report via `/error`, and recover on next reload
- **Single-instance enforcement** via named mutex (Windows) / flock (Unix)
- **Dependency file watching** — all compiled `.das` files tracked (not just the main script)
- **`-cwd` flag** — changes working directory to script's folder
- **4x MSAA** enabled by default in `glfw_live` window creation
- **38 unit tests** in `tests/live_host/`

#### AST Pattern Matching: `daslib/ast_match` (#2330, #2332, #2333)

New module for pattern matching on compiled AST using the same tag system as `qmacro` (`$e`, `$v`, `$i`, `$t`, `$c`, `$f`, `$b`, `$a`) but in reverse — tags extract values instead of substituting them.

- Three macros: `qmatch` (single expression), `qmatch_block` (block with wildcards), `qmatch_function` (compiled function)
- Wildcards: `_wildcard()` (0+), `_wildcard1()` (1+), `_optional()` (0–1), `_any()` (exactly 1)
- Strict argument matching with `$a(rest)` for flexible tail
- Constant constructor matching (folded and unfolded forms)
- Comprehension matching: array `[for...]`, table `{for...}`, generator `[iterator for...]`
- Block/lambda/local-function discrimination via `mmFlags`
- Post-inference local function matching via `qm_convert_local_function`
- 305 tests across 17 test files, full language reference documentation

#### Eliminate Builtin `.das` Compilation — Compiler-Free Standalone Executables (#2304)

All built-in `.das` wrappers (fio, rtti, network, math helpers, etc.) have been moved out of `compileBuiltinModule` and into regular `daslib/` modules. This is a foundational architectural change with three major consequences:

- **Faster compilation** — ~10ms faster on static builds (35ms → 25ms for hello world), ~7ms on dynamic. The compiler no longer parses and type-checks built-in `.das` code on every startup.
- **Standalone executables without the compiler** — `libDaScript` can now be split into a runtime-only library and a compiler library. Standalone executables (AOT-compiled, JIT-compiled, or via LLVM) no longer need to carry the full compiler just because built-in modules had `.das` wrappers. This enables significantly smaller binaries for deployed applications.
- **Minimal runtime environments** — possible to run a daScript context with only C++-bound modules, no `.das` parsing at all. Opens the door for embedded/constrained targets.

Requires updating bare `require fio` / `require rtti` to `require daslib/fio` / `require daslib/rtti_core` in user code. The `rtti` module was renamed to `rtti_core` to avoid conflicts.

#### Performance Lint Module: `daslib/perf_lint` (#2315, #2317, #2318)

Compile-time AST visitor that detects common performance anti-patterns:

- **PERF001**: string `+=` in loop (O(n^2) allocations)
- **PERF002**: `character_at` with loop variable index (O(n) per call; use `peek_data` instead)
- **PERF003**: `character_at` anywhere (informational; suggests `first_character` for index 0)
- **PERF004**: string interpolation reassignment in loop (O(n^2) allocations)
- **PERF005**: `length(string)` in while condition (strlen each iteration)
- **PERF006**: `push`/`push_clone`/`emplace` in for loop without `reserve()`
- **PERF007**: unnecessary `string(das_string)` in comparison (das_string supports `==`/`!=` directly)
- **PERF008**: unnecessary `get_ptr()` for `is`/`as` type checks (smart_ptr supports them directly)
- **PERF009**: redundant move-init variable immediately returned (use `return <- expr` directly)

Expression chain walking, field path tracking, closure-aware detection, `inferStack` reporting. Two usage modes: `require daslib/perf_lint` for auto-lint, or standalone `utils/perf_lint/main.das` for batch checking. Full codebase sweep applied all fixes.

#### Pointer Safe-At Indexing (#2280)

- `a?[index]` for pointer types — returns null if `a` is null, otherwise `a + index` (pointer arithmetic). Requires `unsafe`. Supports int, uint, int64, uint64 indices.
- **Bug fix**: bounded safe-at types (array, fixed array, vector) accepted int64/uint64 indices but `SimNode_SafeAt` truncated to 32-bit. Fixed by restricting bounded types to int/uint only.
- Implemented across interpreter, AOT, and JIT.

#### C API Type Introspection (#2334)

- Expose `TypeInfo`, `StructInfo`, `EnumInfo`, `FuncInfo` as opaque handles in `daScriptC.h`
- ~40 new C functions for querying struct layouts (field names, types, offsets), enum values, function signatures
- `das_base_type` enum with `static_assert` guards against C/C++ drift
- Tutorial 11: struct layout inspection, nested structs, engine-side allocation using discovered offsets

#### New Builtins

- **`get_module_file_name(name)`** (#2294) — returns source file path of a module. Enables module-relative asset resolution.
- **`get_key(tab, value_ref)`** (#2322) — O(1) table value-to-key lookup via pointer arithmetic on parallel key/value arrays
- **`first_character(string)`** (#2315) — O(1) first character access (both `string` and `das_string` overloads)
- **`with_das_string(block)`** (#2315) — creates a temporary `das_string` for block scope
- **`system()`** (#2293) — fire-and-forget process launch in `fio` module
- **`daslib/command_line`** (#2328) — `get_das_exe()` returns path to the daslang interpreter

#### daStrudel: Pattern Music Engine (#2362)

A TidalCycles-inspired pattern music engine with SF2 SoundFont support, MIDI playback, and a redesigned two-mode player architecture. Patterns are pure functions from time spans to events — composable, lazy, and side-effect-free.

**SF2 SoundFont support:**
- Load General MIDI soundfonts (`strudel_load_sf2`) — 128 GM presets, multi-layered instrument zones
- Per-voice reverb and chorus sends from SF2 generator parameters
- Exclusive class handling (hi-hat choke) and strudel cut groups for voice management
- Note-off scheduling with sample-accurate timing
- SF2 state persistence across live reloads via Archive serialization (~140MB GM font survives hot reload)

**Two-mode player architecture:**
- **Main-thread mode** — `strudel_create_channel()` + `strudel_tick()` from your render loop. Direct access to per-track PCM and pianoroll hap data. No channels, no lockboxes, no `void*` casts.
- **Threaded mode** — `strudel_init(fn)` spawns a dedicated audio thread. `strudel_play()` runs the tick loop with buffer-level sleep. Channel-based command dispatch for live pattern changes.

**Visualizer combinators:**
- `pianoroll("name")`, `spectrum()`, `scope()`, `vectorscope()`, `drums()` — pass-through pattern combinators using a rolling `g_vis_index`. Each registers what visualization the track wants; `strudel_add_track()` bumps the index.
- `pianoroll()` wraps the pattern to capture haps during evaluation into a direct array (same thread, no channels). Other combinators are pure pass-throughs that mark track preferences.
- `vis_name()`, `vis_color()` — set display metadata per track.
- Panel names displayed only when explicitly specified.

**MIDI player** (`strudel_midi_player.das`):
- Load and play `.mid` files through SF2 soundfont rendering
- Per-channel gain, pan, looping, audio visualization channels
- Integrates with the visualizer via the same audio analysis pipeline

**Examples** (6 demos in `examples/daStrudel/`):
- `player_demo` — minimal threaded playback (load samples, play pattern, shutdown)
- `synth_demo` — 100% synthesized, no sample files needed (drums + bass + lead)
- `sf2_demo` — SF2 lo-fi chill beat (Am-F-C-G with GM percussion, piano, bass, strings)
- `piano_demo` — "Ode to Joy" with pitched piano samples via `seq()` bar notation
- `strudel_live` / `strudel_sf2_live` — live-reload with REST commands (play/stop/fade tracks by name)
- `strudel_visualizer` — OpenGL visualizer with 5-track pianoroll, spectrum, drums, scope, vectorscope panels. Main-thread mode with direct data access.

### Improvements

#### Compilation & Runtime

- **Faster CondFolding** (#2309) — optimization phase handles multiple transformations per pass instead of one, reducing total optimization passes.
- **Optimized `character_at`** (#2315) — C++ implementation scans only to index instead of full `strlen`.
- **Fix access flags for array/table lock/unlock** (#2310) — split into const/non-const overloads, fixing incorrect variable access flag inference.
- **One-liner and table lints are warnings** (#2284) — demoted from errors until next major release, controllable via `CodeOfPolicies`.

#### File I/O

- **Explicit error reporting for fio** (#2282) — removed all try-catch from C++17 filesystem functions, added Go-style `string& error` out-parameters to 16 functions. 35 new tests.
- **Fix `stat`/`fstat` side effects** (#2319) — changed from `modifyExternal` to `modifyArgumentAndExternal` for correct const inference.

#### MCP Server (AI Tool Integration)

- **7 live tools** (#2293) — `live_launch`, `live_status`, `live_error`, `live_reload`, `live_pause`, `live_command`, `live_shutdown`
- **`lint` tool** (#2319) — AI-assisted code quality checking (includes both paranoid lint and perf_lint in one pass) with duplicate message deduplication
- **`shutdown` tool** (#2293) — shuts down MCP server for code change pickup
- **`failures_only` param for `run_test`** (#2328)
- **Fix ast-grep detection** (#2326) — fallback to `.venv/Scripts/sg` / `.venv/bin/sg`
- **Fix standalone exe subprocess hang** (#2328) — `get_das_exe()` prevents launching another MCP server instead of `daslang.exe`
- **Unique temp files** (#2328) — `ref_time_ticks()` in names to avoid collisions

#### stb Libraries

- **stbtt glyph shape API** (#2291) — expose `stbtt_GetCodepointShape`, vertex struct, vertex type constants
- **Safe stb_truetype API** (#2312) — two-layer safe API: low-level `stbtt_*` wrappers confining `unsafe`, high-level `Font` methods with zero `unsafe`. 70 tests.

#### DECS (Entity Component System)

- **Bulk entity creation** (#2290) — `create_entities`T` API bypasses ComponentMap, resolves component indices once, writes directly into archetype storage. ~10x faster (174ns vs 1772ns for 1000 entities under JIT).
- **Multi-dimensional fixed array serialization** (#2295) — `daslib/archive.das` supports 2D–6D fixed arrays

#### Documentation & Install

- **daslang-live documentation** (#2314) — full RST page covering lifecycle, API, modules, REST API, CLI, examples
- **CMake install overhaul** (#2313) — complete examples, module install fixes, tutorial install rules, utility install
- **Updated CLI help** (#2324) — all command-line switches documented for both `daslang` and `daslang-live`
- **Lint sweep** (#2317, #2321) — all perf_lint and lint warnings fixed across daslib/, modules/, examples/, tutorials/

#### CI & Infrastructure

- **Custom org runners** (#2307, #2308) — `ubuntu-latest-fat`, `ubuntu-24.04-arm-fat`, `windows-latest-fat`, macOS xlarge
- **Correct CI timeouts** (#2283) — increased to 900s for dastest, 1h for isolated mode
- **`daslang_static` in CI** (#2292) — Release-only testing
- **das-fmt Release-only** (#2292) — run once on Linux only
- **Extended CI checks** (#2331) — additional test configurations

### Bug Fixes

- **AOT `das_null_coalescing`** (#2286) — template used wrong type source (`defaultValue->type` instead of expression result type). Fixed in both C++ and daScript AOT generators.
- **ASAN support on MSVC** (#2301) — fix `NO_ASAN_INLINE` macro (was missing `__declspec(no_sanitize_address)` for MSVC), forward ASAN flags to libhv ExternalProject.
- **ModuleGroup use-after-free** (#2301) — `ModuleGroup` was stack-local in `compile_script()` but outlived by `Program`/`Context`. Moved into `CompileResult`.
- **Fuzzer bugs** (#2311) — clone semantics won't pipe to ascend/new; `getBaseSizeOf64`/`getAlignOfField` correctly crash on type-macros.
- **Fusion ASAN false positives** (#2306) — `NO_ASAN_INLINE` on fusion call/return methods using `v_ldu` on sub-16-byte data.
- **Cross-platform path hardcoding** (#2289) — replace `bin/Release/daslang.exe` with `get_command_line_arguments()[0]` in tests.
- **`array_mark_locked`** (#2289) — use proper API instead of manual `Array` field setup in dasAudio/dasSQLITE.
- **JIT standalone executables** (#2327) — various fixes for JIT-compiled standalone exe generation.
- **JIT support for all nodes** (#2281) — additional node types supported in JIT backend.
- **Handled const string type emission** (#2287) — manually emit type for handled const string.
- **MCP shared module caching** — `ignore_shared_modules` prevents stale structs across `compile_check` calls.

### Examples

- **Arcanoid** (#2286, #2290, #2303) — full Breakout game with 3D rendering, DECS entities, procedural audio, power-ups, planar shadows, particles, ball trail, menu/pause states, 11 live commands. Rounded bricks, shield powerup, pickup VFX, sticky paddle.
- **Sequence board game** (#2295, #2296, #2297, #2300) — complete Sequence implementation with 2-4 players, card placement, jack logic, win detection. Three bots (random, greedy, lookahead), ELO tournament runner, parallel evolution strategy for weight tuning, 176 tests.

### Utilities

- **`utils/migrate-tables/`** (#2298) — two-pass tool transforming `tab.insert(key, tab.get_value(key) + val)` patterns into `tab[key] += val` compound assignments. 22 ast-grep rules + Python script for compound expressions.
