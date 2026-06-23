# OpenGL ↔ Vulkan Shader Harmonization — Canonical Plan

> Canonical record for the dasOpenGL ↔ dasVulkan harmonization arc.
> Sibling records: `modules/dasSpirv/MASTERPLAN.md` (SPIR-V emitter), dasVulkan `ROADMAP.md`.
> Keep this file current as phases land — it is the arc's source of truth.

**Branch:** `bbatkin/opengl-harmonization`  ·  **Worktree:** `D:\Work\daScript-opengl-harmonize`
**Started:** 2026-06-22  ·  **Status:** Phase 0 complete (foundation built) — **Phase 1 next**

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

**Stays / the knot — opaque resource types.** `sampler2D`/`sampler3D`/`image2D`/`image3D` differ by
backend (GL carries a `uint` handle; SPIR-V is an empty marker). The type is AST-only and each emitter
keys off the *marker name*, not the fields.
- **Plan A (preferred): GL-superset in the shared module** — declare `sampler2D { texture2D : uint }`
  etc.; Vulkan ignores the unused field. Collapses the knot.
- **Plan B (fallback if the SPIR-V emitter rejects a non-empty body):** keep resource types backend-local,
  share only value-builtins + non-resource intrinsics.
- This is the **prime "kaboom" candidate** — resolved on the first wiring pass (Phase 1).

**Transparency + move discipline.** `glsl_common` and `spirv_builtins` each `require
shader_lingua_franca public`, so every existing `require glsl/glsl_common` / `require spirv_builtins`
consumer sees the same symbols unchanged. The move is **delete-from-both-then-add-to-shared** (no
duplicate decls → no redefinition errors), reconciling any spelling differences to a superset.

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
Compile-check every existing GL/GLSL consumer; run-a-few-frames the ones that can run headless. This is
the guard that we didn't break compatibility. Targets (from inventory):
- `examples/opengl/01_hello_triangle … 09_hello_mesh` (8 files; `05_compute`, `09_mesh` desktop-only)
- `examples/pathTracer/toy_path_tracer_opengl{,_basic,_hdr}.das`
- `examples/graphics/furier_opengl_imgui_example.das`
- `examples/games/{arcanoid,asteroids,pacman,sequence,river_run}` (real games on GL)
- `examples/daStrudel/{drum_compare,strudel_visualizer}`, `examples/daslive/{hello,hello_stdio,triangle,tank_game}`, `examples/audio/hrtf`, `examples/node-editor/imgui_node_editor_basic.das`

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

**▶ NEXT (Phase 1 pickup):** create `shader_lingua_franca` (in-tree, independent module); move the builtin surface OUT of `glsl_common` and `spirv_builtins`; wire `require shader_lingua_franca public` in both cores. **First experiment = the sampler-superset knot (§6, Plan A):** put `sampler2D { texture2D : uint }` in the shared module and check whether the SPIR-V emitter tolerates the non-empty body — if not, fall back to Plan B (resource types stay backend-local). Gate: existing dasOpenGL *and* dasVulkan shaders still compile green. Run from a session rooted in the worktree so MCP/compiler see the in-tree edits.
