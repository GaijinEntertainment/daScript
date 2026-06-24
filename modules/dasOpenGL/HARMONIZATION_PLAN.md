# OpenGL ↔ Vulkan Shader Harmonization — Canonical Plan

> Canonical record for the dasOpenGL ↔ dasVulkan harmonization arc.
> Sibling records: `modules/dasSpirv/MASTERPLAN.md` (SPIR-V emitter), dasVulkan `ROADMAP.md`.
> Keep this file current as phases land — it is the arc's source of truth.

**Branch:** `bbatkin/opengl-harmonization`  ·  **Worktree:** `D:\Work\daScript-opengl-harmonize`
**Started:** 2026-06-22  ·  **Status (2026-06-23):** Phase 1 done — shared module + union grown (`fwidth` / `gl_FragCoord` / `gl_VertexIndex`→GL `gl_VertexID` / `gl_InstanceIndex`→`gl_InstanceID` / `gl_FragDepth` / `textureLod` / `texelFetch`); dasVulkan render-smoke established as the external-consumer gate, caught + fixed the Plan-A host-bind regression (dasVulkan **PR #45 merged**). Arc branch is **local, not yet PR'd**. **Resume → "NEXT" at the bottom** (GL emission gate `tests/glsl` + the `spirv_builtins` audit-tail clean moves **landed**; keep dasVulkan smoke as pre-push gate; Plan-A revisit + bind-codegen coverage; the deferred sampler-marker moves; Phase 2 `@`-annotations).

**Workflow (locked):** ONE branch, many commits, no sub-PRs. The gate is
`daslang utils/preflight/main.das -- --full` run locally, **not** CI (CI is too slow for the
iteration loop). Land the safe intersection first, then grow toward the modern union on the same
branch — every commit preflight-green.

> ⛔ **WORKING CONSTRAINT (2026-06-23, Boris — active until Boris is at his desk): NO executable rebuilds.**
> Windows Defender throws a first-run prompt on freshly-built unsigned `daslang.exe` / `test_aot.exe`, which
> blocks unattended builds while Boris is away. Until the Defender exclusion is applied (admin-only;
> self-elevating fix script at `D:\Work\add-daslang-defender-exclusions.ps1`), proceed with **`.das`-only
> changes** and validate with what the EXISTING worktree binary already runs: **interp + JIT + lint + format
> + ci-das + the dasVulkan render smoke** (24+15) — none of these rebuild an exe. **BATCH for desk-time:** any
> C++ change, and **AOT validation** (a `tests/glsl` add makes `test_aot`'s baked stub stale → the AOT gate
> needs a `test_aot` rebuild). So under this constraint preflight runs with `--skip cpp-syntax,tests-aot` and
> `sequence` is the GL-game proxy (run its script directly — it bundles in a temp dir, no worktree relink).

---

## 0. TL;DR

OpenGL is daslang's **portable** graphics rail; Vulkan is the **high-end desktop** rail. They
already share a deliberately-mirrored shader architecture (two emitter cores, two backend
subclasses). This arc harmonizes the **shader-body lingua franca** (`gl_Position`, `@in`/`@out`/
`@location`, the intrinsic signatures) into one shared module both cores `require ... public`,
teaches the GLSL generator to *accept* the Vulkan `@`-annotations (one-way porting aid), and
builds a GL tutorial ladder + the first real GLSL-emitter tests. The strategic prize — **GL on
the web (WebGL2/WASM)** — is the logical continuation of the wasm killer feature and is the
*next* session, not this one. **No existing OpenGL code may break.**

---

## 1. Why / strategic frame

- **OpenGL's value is reach, not power.** GL 4.6 (2017) is the terminal OpenGL version — Khronos
  redirected all forward effort to Vulkan. GLSL 4.60 already expresses 100% of GL 4.6; there is
  nothing to "catch up" on the desktop. The expansion that matters is **WebGL2/WASM** ("when, not
  if, and soon"), which is the continuation of the wasm killer feature.
- **Vulkan is the high-end desktop rail.** Explicit, uniform target; the existing dasVulkan +
  dasSpirv stack already ships it.
- **Two rails, not one merged super-rail.** We unify the *shader-body lingua franca* so that a
  shader body reads identically on both and **GL → VK porting is mechanical** (porting is always
  GL→VK, never the reverse). We do **not** merge the macros or the host-binding model.
- **Preserve everything.** Existing OpenGL demos stay on their current path. Anything outside the
  new compatibility envelope is left as-is for now. We are expanding and harmonizing, not rewriting.

---

## 2. Locked decisions (from the design discussion)

| # | Decision |
|---|---|
| D1 | **Target matrix = WebGL2 + GL-4.1-ish.** WebGL2 is the priority. macOS (GL 4.1) is best-effort — if something doesn't run on Mac, document the mismatch and move on (better to lose Mac than lose web). **WebGL1 is ignored** (legacy; WebGL2 is everywhere WebGL is). |
| D2 | **Emitter cores stay separate** — `dasGlsl` (→GLSL text) and `dasSpirv` (→SPIR-V binary). **Stage macros stay separate**: `[vertex_program]`/`[fragment_program]`/`[compute_program]` (GL) ≠ `[vertex_shader]`/`[fragment_shader]`/`[compute_shader]` (SPIR-V). |
| D3 | **Unify the I/O lingua franca** into one shared module both cores `require shader_lingua_franca public`. `require public` re-export makes the transition borderline transparent for existing consumers. |
| D4 | **Backport the Vulkan `@`-annotations into the GLSL generator** — GL understands `@binding`/`@set`/`@push_constant`, and `@in`/`@out`/`@location` are unified. Purpose: make GL→VK porting mechanical. "That's about it." |
| D5 | **Uniforms: preserve the loose authoring surface.** The user writes loose-looking `@uniform x : T`; each backend's binder does the right thing — GL binds loose (`glUniform`, preserves every existing demo + WebGL1-shape) or packs a std140 UBO (WebGL2/perf); Vulkan packs UBO/push-constant. Under-the-hood packing, zero breakage. |
| D6 | **es/wasm profile is keyed off the build target** (wasm ⇒ es300, native ⇒ desktop). No `options` knob until proven necessary; add one later only if needed. |
| D7 | **SPIR-V→GL (`GL_ARB_gl_spirv`) is demoted to a last-phase nice-to-have, possibly never.** It serves only the Win/Linux GL-4.6 slice (already covered by GLSL) and cannot reach the web. |
| D8 | **wasm + GL is the NEXT session, not this one.** Recorded here so the foundation work doesn't paint it into a corner. |
| D9 | **The shared module lives in `daslib/` (`daslib/shader_lingua_franca.das`), required `daslib/shader_lingua_franca public`.** `daslib` is always on every consumer's path — no CMake module registration, no mount, no build-ordering. "There will be more shaders" → `daslib` is the home for shared shader libs. The module NAME is `shader_lingua_franca` (independent of the require path), which is what the emitters key their by-module-name recognition off. |
| D10 | **The dividing line is MODERN-vs-LEGACY, not intersection-vs-union.** The shared module carries only the modern WebGL2 / GL-4.1-core surface BOTH rails speak. **GL legacy (GLSL 1.x: `gl_FragColor`, `texture2D()`) never enters the shared module and is never pulled toward Vulkan** — it stays in `glsl_common` (which `require shader_lingua_franca public`), marked legacy there (clear section comment, not `[deprecated]` — existing demos must stay warning-free). The harmonization flow is GL→VK: the modern webgl/4.1 builtins land in the shared surface; where GL still spells something the old way but the modern unified name is Vulkan's (e.g. `gl_VertexIndex`, which GL must emit as `gl_VertexID`), the shared module uses the unified spelling and the GL emitter renames on the way out. **STOP signal:** if about to pull a GL-1.1 legacy thing toward Vulkan, keep it GL-legacy instead. |

---

## 3. Architecture (current, grounded in source)

Two emitter cores and two API bindings, built as deliberate siblings (the dasSpirv source comments
say so: *"mirrors dasGlsl's GlslShader / [compute_program]"*, *"The dasGlsl/dasOpenGL split puts the
body fill in a backend subclass; we mirror that."*).

| Role | GLSL side | SPIR-V side |
|---|---|---|
| Emitter core (in-tree) | `modules/dasGlsl` (`glsl_common`, `glsl_internal`) | `modules/dasSpirv` (`spirv_emit`, `spirv_shader`, `spirv_builtins`) |
| Shader base class | `GlslShader` | `SpirvShader` |
| Backend subclass | `GlslOpenGLShader` (in `modules/dasOpenGL/glsl/glsl_opengl.das`) | `SpirvVulkanShader` (in dasVulkan, `D:\DASPKG\dasVulkan`) |
| API binding | `modules/dasOpenGL` (1,272 fn, `extern(opengl, late, …)`) | dasVulkan (volk-dispatched, generated) |
| Output artifact | global string `{name}\`shader_text` (GLSL source) | global `{name}\`spirv : array<uint>` + `{name}\`spirv_reflect` |

**Shared mechanics already common to both:** field annotations `@in`/`@out`/`@uniform`/`@location`;
the `collect_dependencies` → generate `{name}_bind_uniform` compile-time reflection pattern; the
dummy-in-`apply()` / fill-in-`patch()` dance.

**The seam this arc targets:** `glsl_common` and `spirv_builtins` declare the *same* builtin surface
twice (`gl_Position`, compute IDs, `texture()`, `imageLoad`, `textureSize`, `dFdx`, `discard`, …) as
AST-only stub functions/globals. One shared module collapses that duplication.

**The irreducible divergence (host-binding model leaking into the type):** the opaque resource types.
`sampler2D` is `struct { texture2D : uint }` in GL (host reads the texture-unit handle) vs `struct {}`
in SPIR-V (pure descriptor marker). See §6 for the resolution.

---

## 4. The capability lattice — what makes GL harder than Vulkan

Vulkan is *one* target. OpenGL is a **capability lattice**, and that — not the API surface — is the
real difficulty.

| Tier | Reach | Compute | SSBO / images | UBO (std140) | Shading language |
|---|---|:---:|:---:|:---:|---|
| GLES 3.0 / WebGL2 | web + everywhere | ✗ | ✗ | ✓ | GLSL ES 3.00 (`#version 300 es`) |
| GL 4.1 | macOS | ✗ | ✗ | ✓ | `#version 410 core` |
| GL 4.6 | Win / Linux | ✓ | ✓ | ✓ | `#version 460 core` |

WebGL2 and GL 4.1 are nearly the same constraint set (UBO yes; `layout(binding)` on blocks no →
bind host-side; explicit uniform locations no → by-name; no compute/SSBO/images), so **one "portable
floor" dialect covers both**; desktop Win/Linux is that floor plus opt-in compute/SSBO.

**Tier-honesty (fail-closed, per the codegen-fail-closed rule):** a web-targeted program that reaches
for compute/SSBO/images should get a clean *compile* error, not a silent WebGL link failure. With D6,
the gate is one boolean (`are we wasm?`), no annotation.

The existing `examples/opengl/` ladder already straddles the lattice: `05_hello_compute` (GL 4.3) and
`09_hello_mesh` (NV mesh, desktop) are desktop-only; `01`–`04` are web-capable. Good test material.

---

## 5. GLSL ES 3.00 emitter delta (recorded for the next-session web work)

The shader *body* is essentially identical across desktop GLSL and GLSL ES 3.00. The entire dialect
delta is preamble + two token renames — and most of it is what desktop **core** wants anyway:

1. **Version line:** `#version 300 es` vs `#version 330/410/460`.
2. **Mandatory precision defaults** (fragment shaders are invalid without them): `precision highp
   float; precision highp int;` (+ sampler precisions). Desktop treats these as a legal no-op → **always
   emit them.** ES 3.0 guarantees `highp` in fragment, so no precision-loss risk.
3. **Modern builtins (also required for desktop core):** `out vec4 fragColor;` instead of `gl_FragColor`;
   `texture(s,uv)` instead of `texture2D(s,uv)`; unified `gl_VertexIndex` → emit `gl_VertexID`. The
   current GL emitter still produces GLSL-1.20-isms (`gl_FragColor`, `texture2D()`) that are invalid in
   both ES 3.00 *and* desktop core — fixing them *is* the lingua-franca modernization.
4. **Block binding:** ES 3.00 has UBO + std140 but **not** `layout(binding=N)` on blocks (4.2 / ES 3.1)
   and **not** explicit uniform `layout(location=)`. So UBO block binding is set host-side via
   `glUniformBlockBinding`; loose uniforms are by-name. (Desktop ≥4.2 could use `layout(binding)`; don't —
   one path serves both.)

**Hard ceiling (the real cost of web, not the dialect):** no compute (ES 3.1), no SSBO, no
`image2D`/`imageLoad`/`imageStore` (ES 3.1), no geometry/tessellation. The compute/path-tracer examples
are inherently desktop-only.

---

## 6. The shared module — `shader_lingua_franca`

New in-tree module, independent of both cores; both `require shader_lingua_franca public`.

**Goes IN (backend-invariant):**
- Value builtins: `gl_Position`, `gl_VertexIndex`, `gl_FragCoord`, `gl_PointSize`, `gl_PointCoord`, the
  compute IDs (`gl_GlobalInvocationID`, `gl_LocalInvocationID`, `gl_WorkGroupID`, `gl_NumWorkGroups`,
  `gl_LocalInvocationIndex`).
- Intrinsic *signatures* (AST-only, body never runs): `texture`, `textureLod`, `texelFetch`,
  `textureSize`, `textureOffset`, `imageLoad`, `imageStore`, `imageSize`, `dFdx`, `dFdy`, `discard`,
  `barrier`, `memoryBarrier*`, `fract`.

**The knot — opaque resource types — RESOLVED (Plan A confirmed, Phase 1).** `sampler2D`/`sampler3D`/
`image2D` differ by backend (GL carries a `uint` handle; SPIR-V is an empty marker). The SPIR-V emitter
keys off the *marker name* (`sampler_info` / `classify_global`) and **ignores the struct fields entirely**.
Verified empirically: adding the GL-superset field to `sampler2D` left the golden disassembly BYTE-EXACT
and spirv-val clean. So **Plan A holds for the emitter** — `sampler2D { texture2D : uint }` etc. live in the
shared module; the SPIR-V *emitter* ignores the field. (`image3D` is GL-only — Vulkan has no `image3D` — so
it stays in `glsl_common`.)

> ⚠️ **Correction (2026-06-23): "Vulkan ignores the field" is true only of the EMITTER, not the host.**
> dasVulkan's host `generate_bind_uniform` told UBO blocks from sampler/image markers by an *empty-field-list*
> test, so the non-empty shared `sampler2D` was miscollected as a UBO → the deferred tutorial's `scene` UBO
> uploaded as zeros → `normalize(light_dir=0)` = NaN → black lighting. The byte-exact-golden verification
> checked the emitter but NOT the host bind path. Fixed (dasVulkan marker-by-name); see the 2026-06-23
> changelog + NEXT #3 (Plan-A revisit: does a backend-specific field belong in the shared marker at all?).
> **Lesson: Plan-A "transparency" must be verified on BOTH the emitter AND every host consumer's bind/reflect
> tooling — a render smoke, not just spirv-val + golden.**

**Recognition mechanism (the load-bearing fact §3/§6's first draft got half-right).** "Both `require …
public` makes the move transparent" is only true for the SPIR-V emitter, which recognizes builtins by
**bare symbol name**. The **GLSL emitter recognizes builtins by MODULE name** (`fun._module.name ==
"glsl_common"`) — and the SPIR-V emitter has ONE module-name gate too (`is_user_shader_func`). So the move
is NOT free on the GL side: it required teaching the emitters the new module — **6 sites in
`glsl_internal.das`** (name-passthrough, forward-decl skip, struct/function/global emission skips, the
`is_glsl_structure` type check) **+ 1 in `spirv_emit.das`** (`is_user_shader_func` exclusion list). Each
gate now accepts `shader_lingua_franca` alongside its original module.

**Move discipline.** `glsl_common` and `spirv_builtins` each `require daslib/shader_lingua_franca public`,
so every existing consumer sees the same symbols unchanged. The move is **delete-from-both-then-add-to-
shared** (no duplicate decls → no redefinition errors), reconciling spelling differences to a superset
(the shared module uses the annotated `[unused_argument(...), sideeffects] def public … { return … }`
stub style; bodies never run, only the call/reference AST is lowered).

**Emission caveat (verified benign).** Moving `gl_Position` to a different module changes the *order* in
which the SPIR-V emitter discovers it relative to a builtin kept in `spirv_builtins` — so a vertex shader
using both (`gl_Position` + `gl_VertexIndex`/`gl_DrawID`) emits an entry-point interface in a different
order. This is semantically irrelevant in SPIR-V (interface + variable-decl order are unordered), spirv-val
clean, byte-equal everywhere else. The `vindex`/`draw_id` goldens were regenerated; a full-suite regen
changed ONLY those two files, proving nothing else shifted.

---

## 7. Phased plan

- **Phase 0 — Foundation (this session).** Worktree on `bbatkin/opengl-harmonization`; full Release
  build (LLVM/GLFW/STBImage/STBTrueType/ImGui/Audio/HV/PUGIXML/SQLite ON, shared OpenSSL cache);
  MCP bootstrapped against the worktree binary; this plan committed; the sweepable test scaffolding
  stood up (§8). *Preflight-ready: AOT + JIT + lint-via-MCP.*
- **Phase 1 — Shared module + header reconciliation.** Create `shader_lingua_franca`; move + reconcile
  the builtins out of `glsl_common` and `spirv_builtins`; wire `require ... public` in both cores;
  resolve the sampler knot (Plan A / B). **Gate:** existing dasOpenGL *and* dasVulkan shaders still
  compile green (compat sweep + a VK shader smoke).
- **Phase 2 — GLSL generator accepts Vulkan `@`-annotations.** `@binding`/`@set`/`@push_constant`
  understood GL-side and mapped to GL semantics; `@in`/`@out`/`@location` unified. Uniform binder learns
  the loose-or-pack behavior (D5). **Gate:** a shader authored in the Vulkan annotation style compiles
  on the GL rail; emission tests assert the GLSL.
- **Phase 3 — GL tutorial arc (forcing function).** A structured GL ladder mirroring the dasVulkan
  subjects, seeded from the existing `examples/opengl/0x_hello_*`. Expect things to "kaboom" — fix the
  emitter/boost holes they surface. Each tutorial carries an emission test (CI) and a render test (local).
- **Phase 4 — wasm + GL (NEXT SESSION).** Prove `late`/`library` extern resolution under emscripten
  (the actual gate for "does the binding reach the web at all"); add the es300 emitter mode (§5); curate
  the GLES3/WebGL2 binding profile.
- **Phase 5 — SPIR-V→GL (nice-to-have, possibly never).** `SpirvOpenGLShader` backend + `glShaderBinary`
  via `GL_ARB_gl_spirv`, Win/Linux 4.6 only. Only if it ever proves worth it.

---

## 8. Test strategy (early, sweepable — mandated)

Three sweeps, distinguished by whether they need a GPU:

### 8.1 Compatibility regression sweep (no break allowed)
Compile-check every existing GL/GLSL consumer; run the ones that can run headless. This is the guard that
we didn't break compatibility. Verified-green targets (corrected from the real tree layout — games /
daStrudel / daslive live in `<name>/main.das` subdirs):
- `examples/opengl/0{1..7}_hello_*`, `09_hello_mesh` (`05_compute`, `09_mesh` desktop-only)
- `examples/pathTracer/toy_path_tracer_opengl{,_hdr}.das`, `toy_pathtracer_opengl_basic.das` (GL)
- `examples/games/{arcanoid,asteroids,pacman,river_run}/main.das` (real games on GL)
- `examples/daStrudel/{drum_compare,strudel_visualizer}/main.das`, `examples/daslive/{hello,hello_stdio,triangle,tank_game}/main.das`, `examples/audio/hrtf/main.das`
- **Not compile-checkable in a bare worktree** (need external daspkg modules — NOT regressions):
  `examples/games/sequence/main.das` (`cards/opengl_cards` → dasCards), `examples/graphics/furier_opengl_imgui_example.das` + `examples/node-editor/imgui_node_editor_basic.das` (dasImgui/node-editor). Fail at require-resolution before any shader code is reached.

**Pathtracer = a first-class compat target (CPU + GL), MANDATED into the test suite.** The pathtracer has
both a CPU rail (`examples/pathTracer/{path_tracer,toy_path_tracer,toy_path_tracer_profile}.das`, no `glsl`
require — own CPU implementation) and a GL rail (the `_opengl*` files above). Both must stay green across
this arc:
- **CPU**: compile **+ run headless** (it traces a frame and exits; ~9 s, exit 0). Independent of the
  shader modules, so it must never regress.
- **GL**: compile-checked (needs a GL context to run; local-only render check).
- **TODO (wire it in):** register both rails into a runnable sweep — the CPU one as a real run-test, the
  GL ones as compile gates — so the harmonization can't silently break either. The deeper convergence
  (one shader body that runs on CPU *and* GPU, via real CPU bodies for `texture`/`imageLoad`/… — "add
  daslang builtin functions if needed") is a later forcing-function goal, not Phase 1; Phase 1 confirmed
  the CPU rail needs **no** new builtins today.

A sweep driver (compile-only, plus optional N-frame run) lives with the arc; run it before/after every
Phase 1–3 change. CI already exercises the GLFW-gated `.das` via the `sequence` smoke lane — keep it green.

### 8.2 GLSL emitter emission tests — NEW `tests/glsl/` (CI-safe)
The GLSL emitter is currently **100% untested except by use**. Add **emission-only** tests (CI has no
graphics hardware): author shaders with the `[*_program]` macros, assert the emitted `{name}\`shader_text`
GLSL string (version line, precision preamble, declarations, body, uniform-bind function). Register the
new directory in `tests/aot/CMakeLists.txt` (CI runs all tests with AOT — unregistered dirs cause
`error[50101]`).

### 8.3 Graphics render tests — NEW `examples/opengl/render_tests/` (local, GPU)
The tests that actually render. Run **locally** with a real GPU (or llvmpipe/osmesa), **not** on CI.
Offscreen FBO → read back → pixel assertions, mirroring dasVulkan's lavapipe oracle pattern but GL-side.
These back the tutorial ladder's correctness claims.

**Preflight:** `daslang utils/preflight/main.das [-- --full]` — format + lint (via MCP) + interp/JIT/AOT
sweeps. The worktree is built LLVM-ON specifically so JIT + AOT-exe paths work.

---

## 9. Kaboom candidates / open unknowns

- **Sampler superset (Phase 1):** does the SPIR-V emitter tolerate a non-empty `sampler2D` body? → Plan A vs B.
- **GLSL emitter coverage gaps:** surfaced by the tutorial arc (Phase 3) — the forcing function.
- **Legacy → modern builtins:** `gl_FragColor`/`texture2D()` are GLSL-1.20-isms invalid in core/ES;
  modernizing them is both the lingua-franca unification and the web-readiness fix.
- **wasm `late`-extern resolution (Phase 4):** emscripten exposes WebGL2 as *linked symbols*, not a
  proc-address table — the `late`/`library` extern model may need an emscripten path. This is the real
  "does the binding reach the web" gate, bigger than any shader-emitter work.

---

## 10. Non-goals (for now)

- Breaking any existing OpenGL demo. (Hard constraint.)
- Forcing old GL code into the new annotation/uniform world — older path stays valid.
- SPIR-V→GL ingestion (Phase 5 at most).
- WebGL1 / GLES2 support.
- Merging the two stage-macro sets or the host-binding models.

---

## Changelog

- 2026-06-22 — Phase 0 started: worktree + full LLVM/GLFW build + plan authored.
- 2026-06-22 — Phase 0 complete. Full Release build green — LLVM/JIT verified (`lib/LLVM.dll` + JIT exe-gen during build), GLFW/StbImage/HV/Audio/PUGIXML built; SQLite dropped locally (vendored `sqlite3.c` `/WX` C4701/C4703 — separate fix, not this branch). MCP wired to the worktree binary (`.mcp.json` + `sgconfig.yml` + tree-sitter grammar). Memory store shared into the worktree session via junction. Baseline green: `examples/opengl/01_hello_triangle.das` compiles clean.
- 2026-06-22 — **Phase 1 core landed (commit `e935d6d`).** Created `daslib/shader_lingua_franca.das` (D9); moved the modern backend-invariant intersection out of `glsl_common` + `spirv_builtins`, both now `require … public`; taught the two emitters the new module (6 GL sites + 1 SPIR-V site). Sampler-superset knot resolved Plan A (golden byte-exact). Legacy/modern dividing line locked (D10): `gl_FragColor`/`texture2D()` stay GL-legacy. Verified: `tests/spirv` 164/164 (spirv-val); all in-tree GL examples + all pathtracers compile; CPU pathtracer runs; every changed `.das` lint-clean; `vindex`/`draw_id` goldens regenerated (benign reorder, §6).
- 2026-06-22 — **Pathtracer wired as a compat gate (commit `0c994ab`).** CPU `toy_path_tracer.das` → `ADD_EXAMPLE_RUN` (headless run gate in CI's `run_examples`; the call lives in the TOP CMakeLists next to `dasbind_example` — before `run_examples` is generated ~line 593, since `examples/pathTracer/CMakeLists.txt` is included too late at ~1273). GL `examples/pathTracer/*opengl*.das` → `utils/preflight/ci_only_das.txt` compile sweep (`ci-das` gate now checks 14 files, was 11).
- 2026-06-22 — **`with` emitter fix, both rails (commit `fc17370`).** `with` survives infer (a transparent scope; body fields already resolved to `subject.field`) and is normally removed by `BlockFolding` IN THE OPTIMIZE PASS — so a no-optimization compile (lint, `options no_optimization`) keeps it and both emitters errored "with is not supported", even though `compile_check` + the GPU run (optimized) succeed. Now both EMIT THE BODY: GLSL `#if 0`'s the subject (mirrors `daslib/aot_cpp`), SPIR-V no-ops it (subject is a struct lvalue, harmless for block globals). Test `tests/spirv/test_with.das`; suite 166/166. The OpenGL path tracers (inline `path_tracer.das`'s `with`-using Camera/getRay) surfaced it — the forcing function (§9) working.
- 2026-06-22 — **GL path tracers `--frames N` (commit `62193b2`).** All three OpenGL path tracers gained a `daslib/clargs` `--frames` limit (0 = run until window close; `-?` help) so they exit cleanly after N rendered frames and run headless as smoke/render tests. All three render + exit 0; dropped a dead undefined-`srgb` else-branch. **§9 `with` gap CLOSED.**
- 2026-06-23 — **Modern-union growth: `fwidth` + `gl_FragCoord` + `gl_VertexIndex` (commits `7a156ec`, `7645884`, + this one).** Moved three more builtins from the VK-only `spirv_builtins` into the shared `shader_lingua_franca`, each its own preflight-green commit. `fwidth` (a function) and `gl_FragCoord` (a global) are GLSL name-identical, so the GL emitter's existing passthrough lowers them unchanged — neither reordered any SPIR-V golden (the byte-exact `fragx`/`gswizzle` goldens stayed identical). `gl_VertexIndex` carries the **unified (Vulkan) spelling**: the GL emitter now renames it to `gl_VertexID` (the GLSL desktop-core / ES name) — a new gate in `glsl_internal.das` `preVisitExprVar`, **module-gated on `shader_lingua_franca`** so a user var of the same name is never rewritten. SPIR-V recognition stays by-name (module-agnostic) throughout: a full-suite golden regen changed ONLY `vindex.txt` — a pure SSA-id swap of the two builtin Input vars (`gl_VertexIndex` now discovered first), spirv-val clean (§6 benign reorder, exactly as the `gl_Position` move). Each lowering GL-probe-verified (emitted GLSL inspected: `fwidth(...)`, `gl_FragCoord`, `gl_VertexID`) + `tests/spirv` green under interp + JIT. **Open gap:** the new GL lowerings have **no permanent CI emission gate** yet — that is `tests/glsl` (§8.2), which needs `tests/aot/CMakeLists.txt` registration + a `test_aot` rebuild, so it is the next step rather than part of these moves.

- 2026-06-23 — **External-consumer smoke (dasVulkan) introduced + a foundational regression it caught & fixed.** Stood up the dasVulkan render smoke as the external-consumer gate, run with the WORKTREE daslang (so it exercises the arc's emission): `daslang -load_module D:/DASPKG/dasVulkan dastest/dastest.das -- --test <repo>/tests/integration` (24) and `--test <repo>/tutorials --isolated-mode` (15 pixel-oracle `[test]`s) — offscreen render + readback, real GPU locally / lavapipe in CI. It caught a regression the SPIR-V golden + spirv-val + reflection gates ALL missed: tutorial 10 (deferred) rendered the LIT scene **black**. Root-caused with RenderDoc (F12 capture → `rdc` inspect): the lighting pass's `scene` UBO uploaded as **all-zeros** → `normalize(light_dir=0)` = NaN → black. Cause (causal A/B confirmed — empty the shared markers ⇒ green; keep the field ⇒ black): **Plan A's GL-superset field on the shared `sampler2D`/`sampler3D`/`image2D` (`texture2D : uint`)** made those opaque markers non-empty, and dasVulkan's host `generate_bind_uniform` told real UBO blocks from sampler markers by an **empty-field-list** test — so the lighting shader's `@uniform` samplers got miscollected with `scene`, the bind bailed, and `scene` was never written. Invisible to every compile-output gate because the SPIR-V **emitter** ignores the field (byte-identical SPIR-V + reflection); only the **render** smoke surfaced it. The field is **GL-rail-only** (`opengl_ttf` + `glsl_common` `:=` operators + `glsl_opengl` bind codegen; Vulkan never reads it). **Fixed** in dasVulkan — **PR #45 MERGED to dasVulkan master** (`d55e28b`; was `67f6589`, CI-green incl. lavapipe integration + macOS + Copilot-approved): detect markers by TYPE NAME (`is_opaque_resource_struct`), not emptiness — matches dasSpirv's `sampler_info`/`classify_global`, and is a no-op for empty markers. So dasVulkan master now tolerates a non-empty shared `sampler2D` ahead of the arc shipping it. Smoke now **24/24 + 15/15** on the worktree root. (Corrects §6's "Vulkan ignores the field": the EMITTER does, but host bind tooling did not.)

- 2026-06-23 — **Permanent GL emission gate landed (`tests/glsl/`, NEXT #2 done).** First dedicated test suite for the GLSL emitter (previously 100% untested except by use). Two files: `test_lingua_franca.das` pins the arc's union-builtin GL lowerings — `gl_VertexIndex` → `gl_VertexID` (plus a negative-control assert that the Vulkan spelling is fully renamed away), `fwidth` → `fwidth(...)`, `gl_FragCoord` → `gl_FragCoord`; `test_emission_basics.das` pins the general surface — `#version 330 core`, `@in/@out/@inout` declaration lowering, `void main()` / `gl_Position`. Each `[vertex_program]`/`[fragment_program]` bakes its emitted GLSL into a compile-time `const string` global, asserted by substring (`find(...) >= 0`) — **no GPU, pure compile-time**. Key simplification: uniform-free shaders need only `require glsl/glsl_opengl` (the compile-time emitter), NOT `opengl/opengl_boost` (the GL runtime externs), so the suite has zero GL-runtime / AOT-stub dependency — **no module-AOT list and no `_`-exclude needed** in `tests/aot/CMakeLists.txt`, unlike `tests/spirv` (registered at 5 sites: glob + custom target + source-group + executable + deps). `test_aot` rebuilt; suite **7/7 green across interp + AOT + JIT** (no `error[50101]`). The new GL lowerings now carry the permanent CI regression lock they previously lacked (were probe-verified only). Build gotcha (re-confirmed + diagnosed): the worktree MCP `daslang.exe` holds `libDaScriptDyn_runtime.dll`, so the reconfigure-cascade relink `LNK1104`s until that process is killed — kill it pre-build, but the daslang MCP does **not** auto-reconnect from the agent side once down (`defer_loading: false` is set but broken upstream → tools stay deferred → `ToolSearch` can't re-trigger a dial; even the protocol `shutdown`'s documented auto-restart doesn't fire), so a manual `/mcp` restores it.

- 2026-06-23 — **Union growth: `gl_InstanceIndex` → GL `gl_InstanceID` (NEXT #4 done).** The direct sibling of the `gl_VertexIndex` rename. Moved `gl_InstanceIndex` from the VK-only `spirv_builtins` into the shared `shader_lingua_franca` (both rails now speak it); added the GL emitter rename gate in `glsl_internal.das` `preVisitExprVar` (`gl_InstanceIndex` → `gl_InstanceID`, module-gated on `shader_lingua_franca` so a same-named user var is never rewritten) right beside the `gl_VertexIndex` one; SPIR-V recognition stays by-name (`spirv_emit.das`, unchanged). `gl_DrawID` deliberately stays VK-only — its GLSL form needs `GL_ARB_shader_draw_parameters`, outside the WebGL2 / GL-4.1 portable floor. Locked with a new subtest in `tests/glsl/test_lingua_franca.das` (asserts `gl_InstanceID` emitted + the `gl_InstanceIndex` spelling fully renamed away). A full SPIR-V golden regen changed **only `vindex.txt`** — the benign SSA-id + variable-decl-order reorder (both indices now discovered in one module → `gl_InstanceIndex` takes an earlier id; every `OpDecorate`/`OpLoad`/`OpStore` tracks the new ids to the same BuiltIn, id-bound unchanged, §6). Verified: `tests/glsl` 8/8 + `tests/spirv` 166/166 under **interp + AOT + JIT**; lint + format clean (all 4 changed `.das`, CLI fallback since MCP was down); **dasVulkan external-consumer smoke 24/24 (integration) + 15/15 (tutorials, incl. the deferred pixel-oracle that caught Plan-A) green** — the shared-surface change is transparent to the real GPU consumer.

- 2026-06-23 — **Union growth: `gl_FragDepth` + `textureLod` + `texelFetch` (NEXT #4-tail clean moves, commit `2670b9e`).** The three name-identical builtins from the audit triage, moved out of the VK-only `spirv_builtins` into the shared `shader_lingua_franca` in one preflight-green commit. All three are GLSL name-identical (core + ES 3.0), so the GL emitter's existing module-gated passthrough lowers them unchanged — **no new rename gate** (unlike `gl_VertexIndex`/`gl_InstanceIndex`). `gl_FragDepth` is a write-only Output (SPIR-V BuiltIn FragDepth + DepthReplacing); `textureLod` / `texelFetch` are the explicit-LOD / unfiltered-fetch sampler functions. SPIR-V recognition stays by call/symbol name (module-agnostic) → a full golden regen changed **NOTHING**: `gl_FragDepth` co-uses no second builtin global in `fragx`, so there is no SSA reorder (cleaner than `gl_VertexIndex` — the prior "fragx WILL reorder" prediction was conservative; it behaves like the `gl_FragCoord` move), and the two functions never enter the entry interface (`texlod.txt` byte-identical). `glsl_common` declares none of the three, so each was a clean delete-from-`spirv_builtins` → add-to-shared (no delete-from-both). **tests/glsl locks:** `gl_FragDepth` folded into the clean uniform-free `test_lingua_franca.das`; `textureLod` / `texelFetch` get a NEW `test_lingua_franca_textures.das` — and here the **sampler-test dependency problem surfaced + was solved (Boris's call).** A `@uniform sampler2D` emission test generates a `<shader>_bind_uniform` that calls `bind_sampler_2d` / `glGetUniformLocation` / `glUniformAny`, which live in `opengl_boost` → would drag the GL runtime + a large opengl AOT module surface into the **deliberately dependency-free** suite. But the GL bind codegen emits those calls **UNQUALIFIED**, so the test defines local **dummy stubs** of the three; the generated bind function resolves to the dummies, the suite stays `glsl/glsl_opengl`-only (zero GL-runtime / AOT-stub dependency, **no CMake module-list change** — the new file is picked up by the existing `tests/glsl/*.das` glob), and the emission test (which reads only the emitted GLSL string) is unaffected by the inert dummies. **Reusable pattern for the deferred Plan-A sampler moves** (`samplerCube` / `sampler2DArray` / `sampler2DShadow` are all sampler-typed). Validated: `tests/glsl` **12/12** + `tests/spirv` **166/166** under **interp + AOT + JIT**; goldens zero-diff; lint + format clean (4 files, CLI fallback — MCP down); **dasVulkan external-consumer smoke 24/24 + 15/15 green** on the worktree daslang.

- 2026-06-23 — **Bind-codegen coverage layer — (a) foundational fix (NEXT #3 sub-item; daslang commit `97189b5`).** Root-caused why dasVulkan's fail-closed bind guards never fired — the reason the Plan-A regression shipped a *black render* instead of a clean compile error. The footgun is **daslang-core, not dasVulkan**: a `[function_macro]`/structure-macro `patch()` pass that BOTH mutates the AST (`astChanged=true`) AND records an error (a `return false` or a `macro_error()`) had that error **silently wiped** — `patchAnnotations` re-infers on `astChanged`, and `inferTypes` clears `program->errors` at the top of every infer pass, so the deliberate error vanished before it could surface and the program compiled clean. The four guards in `generate_bind_uniform`/`generate_push_constants` (>1-UBO, >1-push-constant, and the two std140-layout-fail paths) all call `macro_error` on the same pass that fills the SPIR-V blob (`astChanged=true`), so they were all dead. **FIX (Boris's call — foundational over the surgical dasVulkan-only option): `src/ast/ast_parse.cpp`** patch loop now only restarts inference when the patch pass stayed clean; if a patch recorded an error, it falls through and the error surfaces as a compile failure. Makes `macro_error()`-during-patch reliably fatal for **every** annotation macro (codegen fail-closed by construction) — and the dasVulkan guards now fire with **ZERO dasVulkan change** (verified: a 2-UBO `[vulkan_fragment_shader]` now errors `50503` "shader reaches 2 @uniform struct globals"; a 1-UBO+sampler shader still compiles). Locked in daslang CI by `tests/language/failed_macro_error_in_patch.das` (+ `_helper_macro_patch_error.das`): a function_macro whose patch raises `macro_error` on the same pass it mutates the AST must fail to compile (`expect 50503`, passes under dastest). **Full preflight on the fix alone — all gates green** (format / lint 14-clean / dasgen / ci-das / docs ×6 / tests-cpp / tests-interp / tests-jit / tests-aot / sequence). Remaining coverage (a dasVulkan PR, gated on this fix reaching dasVulkan's CI daslang): **negative compile tests** for the >1-UBO / >1-push-constant shapes; optional **(b)** a `write_field`-per-std140-field AST assertion; optional **(c)** a GPU bind round-trip fixture. The render smoke stays the backstop, not the first line.

- 2026-06-23 — **Portable sampler-marker moves: `samplerCube` / `sampler2DArray` / `sampler2DShadow` (NEXT #4 follow-up, commit `ce7e6e6`).** The three remaining GL-portable opaque markers moved from the VK-only `spirv_builtins` into `shader_lingua_franca`, with their `texture` / `textureSize` / `textureCompare` overloads. First time these types reach the GL rail, so the dasOpenGL side did real work, not passthrough: (1) `glsl_opengl.das` gained per-type bind paths (a new `emit_combined_sampler_bind` helper feeding `bind_sampler_cube` / `bind_sampler_2d_array` / `bind_sampler_2d_shadow`; they used to fall into the generic `glUniformAny` else branch, wrong for a sampler), (2) `opengl_boost.das` gained the three runtime binds (`glBindTexture` to `GL_TEXTURE_CUBE_MAP` / `GL_TEXTURE_2D_ARRAY` / `GL_TEXTURE_2D`), (3) `glsl_internal.das` rewrites `textureCompare(s, uv, ref)` to the GL/ES shadow spelling `texture(s, vec3(uv, ref))` in `preVisitExprCall`/`visitExprCallArgument` (GL has no `textureCompare`; the emitted GLSL was eyeball-verified `texture(sh_shadow,vec3(sh_uv,0.5))`). SPIR-V keys all three by name and ignores the GL handle field, so the move is byte-neutral on the VK rail — `tests/spirv` 166/166, **golden regen = zero diff**. Locked by an extended `tests/glsl/test_lingua_franca_textures.das` (declarations + `texture()` sampling + the `textureCompare` rewrite + a negative control that the daslang-only name is gone), kept `opengl_boost`-free via the dummy-host-bind-stub pattern. Validated: full preflight all ran-gates green + AOT `tests/glsl` 17/17 + `tests/spirv` 166/166 + sequence smoke PASS; **dasVulkan external smoke 24/24 integration + 15/15 tutorials** (skybox `samplerCube` + deferred, real GPU). Coverage gap noted: the new GL bind paths are not *render*-tested locally (no GL cubemap/array/shadow render test yet — same coverage as the existing `sampler2D` GL bind; a Phase-3 forcing-function item).

- 2026-06-23 — **Phase 2 first slice: GL accepts the Vulkan `@`-annotations (commit `4cf2c20`).** D4's "make GL→VK porting mechanical" via the EXISTING loose-uniform binder (the D5 std140 UBO pack path is the deferred second slice). Probe-grounded the gaps first: a Vulkan-style shader already *emitted* on GL (struct `@uniform` → `uniform Camera cam;` correct; `@set`/`@binding` tolerated/ignored), but `@push_constant` was emitted as a **dead zero-init global** (never bound) and a struct `@uniform` hit the generic `glUniformAny(struct)` else branch → no overload → compile error, and a vertex `@out` came out unqualified (invalid GLSL). Fixes: (1) `glsl_internal` treats `@push_constant` as a uniform in all three stages (an `isUniform = @uniform || @push_constant` flag) and emits vertex `@out` as `out` (alongside `@inout`); (2) `glsl_opengl` collects `@push_constant` globals too and binds any struct uniform/push-constant by **flattening** each leaf field loose-by-name (`emit_loose_struct_bind` → `glUniformAny(glGetUniformLocation(prog, "name.field"), name.field)`, recursing nested structs). Emitted GLSL eyeball-verified (`uniform Camera cam;` / `uniform ModelPC pc;` / `layout(location=0) out vec4 …;` / `cam.proj * cam.view * pc.model`). Locked by `tests/glsl/test_lingua_franca_annotations.das` (5 subtests incl. the `@push_constant`-not-a-dead-global negative control). Validated: preflight all ran-gates green (format / lint 17-clean / dasgen / ci-das / docs ×6 / tests-cpp / tests-interp / tests-jit) + sequence GL-game smoke PASS + full `tests/glsl` 22/22, no regression. AOT of the new file pending a `test_aot` rebuild (deferred this session — a Defender first-run prompt on freshly-built exes was blocking unattended builds; fix script left at `D:\Work\add-daslang-defender-exclusions.ps1`). Fail-closed limits noted: array-of-struct uniform fields + `mat3` fields lack a `glUniformAny` overload (clean compile error until added / the UBO-pack path lands).

- 2026-06-23 — **Loose struct-uniform binder completed: mat3 + array-of-struct (commit `5f3d0ad`, `.das`-only under the no-rebuild constraint).** Closed the two fail-closed gaps the Phase-2 first slice left: `glUniformAny(float3x3)` added to opengl_boost (mat3 uniform fields, e.g. a normal matrix — float3x3 verified 36B = 9 tight column-major floats → straight `glUniformMatrix3fv`), and `emit_loose_struct_bind` now flattens array-of-struct fields by index (`lights : NLight[2]` → binds `lights[0].pos`/`lights[0].color`/`lights[1].pos`/`lights[1].color`; scalar/vec arrays still take `glUniformAny`'s array overload in one call). Probe-verified the exact flattened leaf names on a nested + mat3 + array-of-struct UBO. `tests/glsl/test_lingua_franca_annotations.das` +2 subtests. Validated interp + JIT + ci-das (no exe rebuild). **Surfaced a pre-existing orthogonal GL-emitter gap** (recorded above): matrix-COLUMN indexing in a shader body fails under a no-opt compile — the test body uses the uniform without `m[0]` (the bind flattens all fields regardless of body use).

**▶ NEXT:**
1. **Land the dasVulkan fix with the arc.** The `bbatkin/bind-uniform-marker-by-name` fix is safe to merge into dasVulkan independently (no-op pre-arc), but the arc MUST NOT ship the non-empty shared `sampler2D` to consumers until that fix is in their daslang. Keep the dasVulkan smoke (above) as a standing pre-push gate for any further arc change touching the shared shader surface.
2. **✅ DONE (2026-06-23) — Permanent GL emission gate (§8.2).** `tests/glsl/test_lingua_franca.das` (union-builtin lock + negative control) + `tests/glsl/test_emission_basics.das` (surface baseline), `require glsl/glsl_opengl` only (no GL runtime, no module-AOT list), registered in `tests/aot/CMakeLists.txt`, validated **7/7 interp + AOT + JIT**. See changelog. The new GL lowerings are no longer probe-verified-only.
3. **✅ DECIDED (2026-06-23, Boris) — Plan-A stands: keep the GL handle field in the shared markers for now.** The shared `sampler2D`/`sampler3D`/`image2D` carry a GL-only `texture2D`/`texture3D` field used by nothing on the Vulkan rail. **Decision: leave it in `shader_lingua_franca`.** dasVulkan's host already tolerates the non-empty marker by TYPE NAME (PR #45, `is_opaque_resource_struct`), so nothing is broken by keeping it; the empty-marker form is only marginally cleaner. **Low-priority follow-up (do only if easy; otherwise fine as-is):** make the shared marker **empty** with the GL texture-unit handle carried GL-side (a GL-only wrapper / parallel storage). Still-worth-doing tie-in regardless of the field: dasVulkan's fail-closed `macro_error` (>1 `@uniform`/`@push_constant` struct) should have surfaced the original regression at compile time but shipped a black render — check why the guard didn't fire fatally in the annotation fixup pass.
   - **Bind-codegen coverage (the missing layer — agreed follow-up; Copilot-blessed framing).** The regression lived in **compile-time codegen** (the generated `<shader>_bind_uniform` came out a no-op) yet ONLY the GPU render test caught it — the two existing oracles sit on the wrong side of it: SPIR-V golden / spirv-val / reflection are *above* (emitter output, byte-identical), render tests are *below* (pixels, slow, GPU-gated). Nothing asserted the **host-side bind/descriptor code in between**. Add, cheapest → strongest: **(a) ✅ DONE (2026-06-23, daslang commit `97189b5`)** — fixed deeper than scoped. The root cause was a daslang-core footgun (a `macro_error` recorded on a patch pass that also sets `astChanged` was wiped by the subsequent re-infer, since `inferTypes` clears `program->errors` each pass), so the fix is one site in `ast_parse.cpp` and **all four** dasVulkan guards (not just >1-UBO) now fire fatally with **zero dasVulkan change** — locked by `tests/language/failed_macro_error_in_patch.das`, full preflight green (see changelog). **(b)** a macro/AST assertion that `<shader>_bind_uniform` emits one `write_field` per UBO std140 field (pure compile-time, runs in CI without a GPU); **(c)** a **bind round-trip** fixture in `tests/integration` — map a host-visible UBO, run the generated bind, assert the bytes (needs a device but NO render; same weight class as the existing compute-to-storage-buffer tests). **(b)/(c) + the >1-UBO/>1-push-constant negative compile tests are now a dasVulkan PR, gated on `97189b5` reaching dasVulkan's CI daslang** (the negative tests `expect 50503`, which only fires once the daslang fix is in their toolchain). The unprotected fixture shape is a shader mixing a `@uniform` UBO with ≥1 `@uniform sampler2D`. Render tests stay as the backstop, not the first line for a codegen bug.
4. **✅ DONE (2026-06-23) — `gl_InstanceIndex` → GL `gl_InstanceID`** (only `vindex.txt` reordered, benign) **and the `spirv_builtins` audit tail (commit `2670b9e`).** See both changelog entries. **Audit done (2026-06-23), candidates triaged:**
   - **✅ DONE — Clean name-identical moves (commit `2670b9e`):** `gl_FragDepth` (Output `float`), `textureLod`, `texelFetch` — all moved into the shared union, no rename, golden-neutral (`gl_FragDepth` reordered NOTHING — the prior `fragx` prediction was conservative; the two functions never enter the entry interface). `glsl_common` declared none of the three (verified) → clean delete-from-`spirv_builtins`. `textureLod`/`texelFetch` locked via the **dummy-host-bind-stub pattern** (the bind codegen emits `bind_sampler_2d`/`glGetUniformLocation`/`glUniformAny` unqualified → local dummies keep `tests/glsl` `opengl_boost`-free; reusable for the deferred sampler moves below).
   - **✅ DONE (2026-06-23, commit `ce7e6e6`) — `samplerCube`, `sampler2DArray`, `sampler2DShadow` moved to the shared module.** All three GL-portable (ES 3.0/WebGL2) opaque markers, with their `texture` / `textureSize` / `textureCompare` overloads, now live in `shader_lingua_franca`, each carrying the same GL handle field as `sampler2D`. The dasOpenGL side got the REAL work it needed: new GL bind paths in `glsl_opengl.das` (`emit_combined_sampler_bind` helper → `bind_sampler_cube` / `bind_sampler_2d_array` / `bind_sampler_2d_shadow`; they previously fell into the wrong generic `glUniformAny` `else` branch); the three runtime helpers in `opengl_boost.das` (`glBindTexture` to `GL_TEXTURE_CUBE_MAP` / `GL_TEXTURE_2D_ARRAY` / `GL_TEXTURE_2D`); and the `textureCompare` GL lowering in `glsl_internal.das` — GL has no `textureCompare`, so `preVisitExprCall`/`visitExprCallArgument` rewrite it to `texture(s, vec3(uv, ref))` (emitted GLSL eyeball-verified). SPIR-V is byte-neutral (emitter keys by name, ignores the field): `tests/spirv` 166/166, **golden regen = zero diff**. Locked in `tests/glsl/test_lingua_franca_textures.das` (declarations + `texture()` sampling + the `textureCompare` rewrite + a negative control that the daslang name is gone), via the **dummy-host-bind-stub pattern** so the suite stays `opengl_boost`-free. Validated: full preflight 13/13 ran-gates green + AOT `tests/glsl` 17/17 + sequence smoke; **dasVulkan external smoke 24/24 + 15/15 (skybox uses `samplerCube`)**. Open: GL *render* correctness of the new bind paths is untested locally (no GL cubemap/array/shadow render test — same coverage level as the existing `sampler2D`/`sampler3D` GL binds; a Phase-3 forcing-function item).
   - **Stay VK-only:** `gl_DrawID` (needs `GL_ARB_shader_draw_parameters`), `gl_PrimitiveID` (ES 3.2, not in the WebGL2 floor), `nonuniformEXT` (VK descriptor indexing), `texture2D` + `sampler` (VK separate-image/sampler model — GL is always combined).
5. **Phase 2 — GLSL generator accepts the Vulkan `@`-annotations.**
   - **✅ DONE (2026-06-23, commit `4cf2c20`) — first slice: annotation acceptance via the loose binder.** A shader authored in the dasVulkan annotation style now compiles + emits on the GL rail. `@push_constant` → a GL `uniform` (was a dead zero-init global) + host-bound like `@uniform`; struct `@uniform`/`@push_constant` → `uniform StructType name;` with the bind codegen flattening each leaf field loose by name (`emit_loose_struct_bind`, recurses nested structs); vertex `@out @location` → a real `out` varying (the emitter only handled `@inout` before); `@set` ignored, `@binding` tolerated. Locked by `tests/glsl/test_lingua_franca_annotations.das`. Validated interp + JIT + ci-das + sequence smoke + tests/glsl 22/22; AOT of the new test pending a `test_aot` rebuild. **Loose-binder gaps CLOSED (`5f3d0ad`):** `mat3` (added `glUniformAny(float3x3)` — float3x3 is 36B = 9 tight floats, uploads via `glUniformMatrix3fv`) + array-of-struct fields (`emit_loose_struct_bind` now flattens `name[i].field`), so the loose path covers the full nested + array-of-struct + mat3 shape. **Orthogonal pre-existing GL-emitter gap discovered:** matrix-COLUMN indexing in a shader body (`m[0]`) trips a no-opt bounds-check (`__lineinfo__`/`__context__`) the GLSL emitter rejects — same class as the `with` issue; folds away under an optimized compile but fails lint/JIT. Future emitter fix, not Phase 2.
   - **TODO — second slice: the D5 std140 UBO loose-OR-pack binder.** Net-new on the GL rail (the emitter only does loose `glUniform` today; no `layout(std140)` block emission / packing). Emit `layout(std140) uniform Block {…}`, compute the std140 layout, pack + `glBindBufferBase`/`glUniformBlockBinding`, loose-vs-pack keyed off the build target (D6: wasm/es300 ⇒ pack). Brings the WebGL2/perf path online.
Run from a session rooted in the worktree so MCP/compiler see the in-tree edits.
