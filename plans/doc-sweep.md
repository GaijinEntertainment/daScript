# 0.6.4 documentation sweep — authored RST verification

Goal: every authored RST page (NOT das2rst-generated) carries code that compiles and prose
that matches the implementation, verified mechanically where possible, by agents where not —
and a nightly lane that keeps it that way.

## Scope

- `doc/source/reference/tutorials/**` (313 pages incl. imgui/macros/opengl subdirs)
- `doc/source/reference/language/` (42)
- `doc/source/reference/embedding/` (6) — cpp blocks, separate rail
- `doc/source/reference/utils/` (12)
- `doc/source/stdlib/handmade/**` — code blocks only (152 das blocks); prose corpus out of scope
- OUT: `doc/source/stdlib/generated/**` (das2rst output), `external_modules/` (0 das blocks)

Census at plan time: ~2,260 `code-block:: das` + 152 `code-block:: cpp`.

## Rule 0 — the checker binary carries the corpus

Before any verdict is minted, the tool emits one require-probe per module family referenced
by the corpus and compiles each. Any probe failure = "binary-gap" bucket; the audit REFUSES
to report page verdicts while that bucket is non-empty. This catches both a lean binary and
a wrong require spelling — the dry-run produced false drift verdicts from both causes.

## Mechanical model — page as literate program

Each page's das blocks concatenate in order into one synthetic module:

- `require`/`options`/`module` lines hoisted (deduped); preamble taken from the page's
  companion `.das` (`tutorials/<fam>/NN_x.das`, hyphen variant too) when one exists —
  never from a hand table when a companion is present
- decl chunks (def/struct/class/enum/...) at module scope; stmt chunks appended in order
  into one function so locals flow block-to-block
- rename-on-redeclare: daslang bans shadowing; a narrative re-declaration becomes `name__N`
  from that point on
- class-redecl whose body STARTS with `...` merges into the original class (elision =
  "previous members here"); other redecls rename as independent examples
- `...` in statement position → `pass` (indentation preserved); in decl-body position → merge/drop

Markers — RST comments `.. das-doc: <word>` directly above a block; invisible in rendered
output, no Sphinx extension:

| marker | meaning |
|---|---|
| `skip` | not code (wire formats, output dumps, pseudo-code) |
| `fresh` | start a new synthetic program at this block |
| `given <one-line das>` | inject context the prose assumes (free vars, tiny base classes) |
| `signatures` | API-surface listing — compiler skips; agents verify against the real API |
| `expect error[NNNNN]` | block must FAIL to compile with that code (verified by appending to the page program and compiling) |

## Predictions (filed 2026-08-10, BEFORE the full scan)

1. 30–45% of pages red under auto mode (no markers yet); ~25% of red pages carry a genuine
   doc bug, the rest need markers/context only.
2. Worst areas: `reference/language/` (gen1 remnants) and fast-moving module families
   (clargs/Result-era tutorials, sql, macros). Best: numbered language tutorials with
   compile-gated companions; `stdlib/handmade` ≥95% green.
3. ≥5 more gen1-syntax remnants tree-wide (first language page examined already had one).
4. ≥1 more page documenting an API that no longer exists in that shape (clargs pattern).
5. The 135 error-talk blocks: most convert to `expect error[NNNNN]` markers cleanly; ≤10%
   quote error codes/messages that are themselves stale.

Dry-run priors (6 pages): 3 green after harness fixes, 1 one-marker-away, 2 with genuine
bugs (53_clargs: string→Result drift + invalid one-line enum; classes.rst:275: gen1
braceless `class sealed`). Born-wrong exists (embedding cpp_api.rst ManagedVectorAnnotation).

## Phases

1. **Tool** (`utils/doc-verify/`, das) — extractor, emitter, Rule-0 probes, compile driver
   (fresh `bin/daslang -compile-only` per page, MCP-subtool isolation pattern), JSON+md report.
2. **Full mechanical scan** → red-list with buckets: genuine-bug / needs-marker / binary-gap.
3. **Mini Opus fan-out** (gate, Boris-mandated): one bounded family + calibration pages with
   known findings; validates agent fix quality and marker discipline before scale.
4. **Full Opus fan-out** — fix pages, add markers, converge to green. Prose claims verified
   per page against implementation (probe access), adversarial verify on prose findings.
5. **cpp-block rail** — one generated TU per embedding page compiled against headers.
6. **Nightly lane** — `verify_docs_and_examples`: doc-verify + /examples + /tutorials
   compile/run/lint. NIGHTLY ONLY (regular per-PR cycle stays untouched). preflight mirror entry.
   WIRING SPEC (needs one Boris call): home = extended_checks.yml nightly cron, step after
   "Run tutorial dry-runs":
   `if: github.event_name == 'schedule' || github.event_name == 'workflow_dispatch'`
   `run: $BIN/daslang ./utils/doc-verify/main.das -- --daslang $BIN/daslang`
   BLOCKER: the extended main build has DAS_HV_DISABLED=ON, so rule 0 correctly aborts on
   the dasHV companions. Decide: (a) flip dasHV ON in the extended main build for the cron,
   or (b) give the lane its own build using ci/release_modules.txt (like the sequence smoke).
   Then add the preflight.md mirror row (`bin/daslang utils/doc-verify/main.das`).
7. **Procedure doc** — `skills/doc_sweep.md` (repo-only), written LAST, once the procedure
   survives the audit; each-release cadence, prose re-sweeps scoped to pages whose subject
   changed since the last sweep tag.

## Ledger

- (2026-08-10) 🐞→✅ FIXED same day: temp-string reclaim + passthrough aliasing
  use-after-free. `slice` is [temp_string_result]-flagged → reclaim queues its result;
  `trim` with no trailing whitespace returns an INTERIOR POINTER into that argument typed
  as a REGULAR string (module_builtin_string.cpp:713 → rtrim `return s`); queue advances →
  cell freed → persistent SHOE reissues it SAME-SIZE-CLASS (why small repros missed: churn
  must match the victim's size class). Fix (Boris-approved option: fail-safe gate, not
  always-allocate): both reclaim phases now skip a consuming call that returns a string
  unless its callee is tempStringResult-flagged — flagged = always-fresh = provably cannot
  alias an argument into its result (MarkTempStrings preVisit(ExprCall) + VarUseClassifier
  in ast_allocate_stack.cpp). Failing-test-first: 2 new rows in
  tests/strings/temp_string_reclaim.das (direct-arg + let-form, deterministic 3/6-key
  corruption pre-fix); post-fix 22/22 + full tests/strings 394/394. Independent second
  reproduction: the sql fan-out agent caught doc-verify's OWN marker strings being eaten
  (parse_marker trim/slice temps) pre-fix, stable post-fix. doc-verify's clone_string
  workaround removed.

## Harness hardening backlog (mini fan-out output, 2026-08-10)

From the sql (13/13 green) and classes (green) agents — fix BEFORE the full fan-out:

1. Companion lookup is slug-only — `sql_01_hello.rst` never finds `01-version.das`; add a
   number-prefix fallback (`NN-*.das` / `NN_*.das` glob on the number).
2. `given` is page-wide, injected at top of `_doc_main` — needs (a) module-scope routing
   for `var`/`let` givens (finalizer-counter globals; also on-page module-scope vars are
   chunked as stmts), (b) renamer seeding (a given `class Base` + an on-page `class Base`
   should rename to Base__2, not error 20512), (c) documented ordering (module-decl givens
   must precede others; hoists always precede givens).
3. `fresh` discards accumulated decls — cannot express "alternative spelling of the same
   member" (def finalize vs def operator delete). Need an `alt` marker: compile the block
   against the page program in ISOLATION (also the right semantics for `expect` — see 5).
4. Elision-merge drops a re-declared header whose MODIFIER SET differs (`class sealed
   Foo3D` merged into `Foo3D`, contributing zero lines — the page's gen1 bug was invisible
   to the harness). Differing modifiers = new example, not merge.
5. `expect error[NNNNN]` must compile the block in ISOLATION with a stated require set — a
   negative example can pass/fail for the wrong reason via page-hoisted requires.
6. `def` re-declarations are not renamed (only type decls) — narrative def evolution
   collides.
7. Marker-vocabulary: a `fragment` marker distinct from `skip` ("valid syntax, no chain
   root / no context") so `skip` stays auditable as "never code".
8. Contract note: requires stated in `::` literal blocks are invisible to the extractor —
   pages must use `code-block:: das` for anything the checker should see (docs-authoring
   rule for skills/doc_sweep.md).

## Engine/daslib finds from the fan-out (Boris decision)

RULED by Boris 2026-08-11 — all follow-up work AFTER the sweep PR:

- 🐞 FIX (bug, no discussion): tuple destructuring bypasses the shadowing check —
  `let x = 1; let x = 2` is error[30704], but `let (ok, a) = p1(); let (ok, b) = p2()`
  compiles and SILENTLY rebinds `ok`. Failing test first; sweep in-tree .das for repeated
  destructure names to size breakage; several doc pages with repeated `let (ok, err)`
  narratives will need a coordinated pass.
- 🐞 FIX (bug, "historical reasons" — double-check then fix): a class whose NAME matches a
  class in a required module fails its own generated-method resolution —
  `class MacroMacro : ...` in a module requiring daslib/ast_boost →
  `error[30810] function not found _::MacroMacro'__finalize` (ambiguous with
  ast_boost::MacroMacro's). Generated class-method calls should pin to the defining
  module, not `_::` open resolution (the `_::` convention is for clone/finalize OVERLOAD
  dispatch, not a class's own generated members). Also unblocks doc excerpts of required
  modules.
- ✅ AS-DESIGNED (not a hole): init-move from a smart-pointer value (`var b <- f(p)`)
  needing no unsafe while the statement form fires 31021. Rationale (Boris): the
  statement-form unsafe exists because move-ASSIGN overwrites whatever live smart pointer
  `b` held; an init has nothing to overwrite. Only 3-4 residual smart_ptr classes remain.
- 🔍 INVESTIGATE: `..` field-bypass surface syntax must STAY writable (needed in generic
  code, not only macro-built AST). Docs now teach the reachable `sp. .x` spelling; explore
  whether the lexer/grammar can be made to parse `sp..x` directly (DOTDOT currently wins).
- Re-confirmed open #3678 tail: `let s = match (...)` still yields the misleading
  `error[30231]` instead of the return-match hint.
- Historical note: tables.rst TAUGHT `unsafe { tab[k] = v }` — the likely origin of the
  `unsafe(tab[k])` residue CLAUDE.md warns about; now fixed to state the default.
- modules/dasStbImage/src/dasRaster.cpp:346 — the comment above rast_blend_pixel states a
  `+128` blend formula the code three lines below contradicts (exact /255 via
  `(x + 1 + (x>>8)) >> 8`); page 05's wrong formula was copied from it — fix together.
- FIXED in-tree: imgui_boost_v2.das:672 diagnostic suggested invalid `variable private`
  spelling (now `var private`); state_telemetry.das:92 comment likewise. Still open
  (report-only): modules/dasImgui/tests/record_layout_primitives.das:14 "empty_marker"
  kind comment; examples/features/internal_log_capture.das:13 pre-1.92 ImGuiLogType
  header comment; daslib/strings_boost + daslib/enum_trait docstrings FIXED in-tree.
- imgui family-wide phrase to sweep post-fan-out: "re-exported by imgui_boost_v2" — v2
  re-exports ONLY imgui, imgui_lint, imgui_boost_runtime; four pages carried the false
  claim, more may exist on GREEN pages the fan-out never touched.

- strudel `!N` is implemented as `fast(n)` (`strudel_mini.das:267-272`) with the comment
  "approximation since patterns aren't copyable" — lambdas ARE copyable now, so the
  justification is stale; docs (page 03) now describe actual behaviour. If the engine gets
  the real replicate-into-parent-slots semantics, rewrite page 03 Part D back.
- strudel `end_pos` naming rationale ("`end` is a reserved word") is false — `end` is not
  reserved; stale in `strudel_event.das:78-79` and tutorial 13 comments.
- `tutorials/daStrudel/daStrudel_03_*.das:85-88` + `daStrudel_13_*.das:135,183-192` carry
  the same false claims the RST pages had (companions are compile-gated but their comments
  are not).
- `tutorials/sql/06-error_handling.das:66` count-returns-int64 comment — FIXED in-tree.
- `daslib/sql_boost.das:597` — `[sql_index]` bad-field error omits the `Available: {field_names}`
  suffix its sibling path at :475 has; making them match would let sql_24's original "lists
  the valid columns" prose come back (currently rewritten to match the terse truth).
- More stale companion SQL comments (predate the projection aliaser): tutorials/sql/
  14-group_by.das:122,137, 15-join.das:96-100, 19-update.das:54, 21-upsert.das:85,128.
- tutorials/daStrudel/daStrudel_16_live_reloading.das:132,138 — persistent-store key says
  "tutorial15_reload_count" inside tutorial 16 (renumbering leftover); RST kept matching
  the companion — fix both together.
- examples/daStrudel/sfx_lab/main.das:3-4 — header still says "Layers/Editor/Mix" +
  "(Reference target + save/load land in later slices)"; all shipped since.
- Marker-vocabulary note for post-sweep polish: a page that legitimately SHOWS a
  module-scope `var` in a block can't both display it and compile it (block top-level
  var chunks as a statement; a given seeds the renamer so the visible duplicate renames).
  Needs a `global`-ish per-block marker if it recurs.
- Post-sweep tool polish: (a) hoist companion module-scope `let` constants as implicit
  givens (hand-written `given TWO_PI = ...` duplicates compile-gated ground truth and can
  drift); (b) replace_ident rewrites inside `//` comments and def parameter lists — restrict
  renames to code text and don't apply page aliases inside a decl chunk's own param scope.
- report_page truncates compiler output at 24 lines and surfaces cascade noise before the
  actionable `can't locate variable` — rank 30838 first (sql agent suggestion); also hint
  when ALL errors sit in a required module (peg agent: block likely lacks context).
- `wrap <prefix>` marker idea (peg agent): synthesize a `def f(input; blk) { parse(input) {` shell
  around macro-DSL excerpts so grammar rules compile instead of going fragment.
- `alt` on a hoist-only block is a vacuous pass (checked++ fires, flush early-returns empty) —
  make alt refuse to count an empty segment. `expect` blocks append unrenamed — run them
  through the page renamer (or the isolation redesign). `given member <field>` idea (macros
  agent): inject a FIELD into the member-marker subclass for base-method-parameter context
  (das_string/AnnotationArgumentList can't be globals). Backlog item 6 (def re-decl renaming)
  is DONE for exact headers; modifier-differing headers still collide — extend def_header_key.
- dasLLAMA report-only finds: tutorials/dasLLAMA/04_sessions_and_memory.das:56 hardcodes
  4 bytes/KV-entry (2× under the f16 default — its two prints disagree);
  modules/dasLLAMA/dasllama/dasllama.das:3-8 facade arch roll-call omits GLM-4-MoE and
  Mistral-3 (page 01 copies it verbatim); tutorials/dasLLAMA/07_speech_to_text.das:21-26
  ASR family list missing gemma4a/canary/qwen3omni.
- Marker-idiom note for skills/doc_sweep.md: when a name is declared inside a `with_...()`
  block and a follow-up snippet uses it at top level, `alt` (fresh renamer seeded only by
  givens) is THE idiom — a page-wide given cannot carry the name past the re-declaration.
- Stale companion comments: tutorials/dasPEG/06_debugging.das:76-77 (commit does NOT gate
  error emission — probe-disproved, RST fixed); tutorials/sql/41-triggers.das:84 (wrong
  audit-row count), :24/:104 (wrong module path + tutorial number);
  tutorials/sql/39-schema_from.das:93 ("coming soon" long shipped);
  tutorials/sql/38-concurrency.das:48 (nonexistent get_thread_id in comment).

- ⚠ SPATIAL-AUDIO CONVENTION CONTRADICTION (needs a listening test): companion
  tutorials/dasAudio/04_spatial_audio.das:60-73 + `g_head_direction` default (+Y,
  audio_boost.das:662) vs the engine's own pan math (`pan = nrxy.y`, MIT HRTF azimuth
  sign, volume_mixer.h pan law) and the HRTF demo's "-Y = forward, +X = right" comment —
  a +X source pans LEFT for a +Y-facing listener by the math, RIGHT per the companion.
  One of the two is wrong. The RST (dasAudio_04) now follows the engine math + demo.
- audio_boost.das:1528 `set_position(sid; pos; dir : float3)` — third param named `dir`
  but assigned to `velocity`; misleading for named-arg callers.
- dasHV family-level doc gap: the STREAM/HttpResponseWriter streaming rail and SERVE_FILE
  are documented on no RST page (page 07 covers buffered SSE only).

- daspkg: `resolve(sdk_version, ...)` hook's first parameter is DEAD — both production
  call sites (utils/daspkg/commands.das:404,:853) pass "". Doc now says "reserved; daspkg
  passes an empty string today"; if it's a bug, fix daspkg and revert the doc line.
- dasLiveHost stale-comment cluster (report-only, fix as a batch): main.cpp:440 "lockbox
  dispatch" (none exists), :839 contradicted by find_live_port_in_argv; live_commands.das:12,
  :44; live_api.das:16-28 endpoint list omissions; live_api_builtins.das:20;
  live_watch_boost.das:11-12; dasLiveHost.cpp:326 `live_collect_string_gc` is byte-identical
  to live_collect_gc (name promises a string-only collect it does not do).
- Checker-rule idea (numbered agent): flag an `// output:` block whose chunk contains no
  `print` — caught three false output claims on one page.
- More stale companion comments: tutorials/language/50_soa.das:98 (push CLONES, not moves)
  + :8/:20; 52_option_and_result.das:29,:70; daslib/option.das:29 (no mutators),
  daslib/result.das:30-33,:184,:236; daslib/delegate.das:16-21.

## Regen traps (Boris decision)

- `doc/reflections/gen_module_examples.py` generated the stdlib/handmade module-*.rst
  fragments and still contains the defects the fan-out fixed (double-indent, stale
  base64 output, wrong-module contracts example); it emits `::` literal blocks (invisible
  to the checker) and is unrunnable as-is (hardcoded `d:\Work\daslang` path). Re-running
  a fixed version would silently revert the batch. Delete it, or regenerate it FROM the
  corrected RST before it bites.

- (2026-08-10) Rule-0 probe on M1: zero true binary gaps; all scares were require-spelling
  errors (dasSQLITE → sqlite/sqlite_boost, openai → openai/openai_chat, peg → peg/peg,
  strudel → strudel/strudel). llvm/vulkan/anthropic referenced by no authored RST.
- (2026-08-10) Confirmed doc bugs pre-scan: 53_clargs.rst (whole-page parse_args drift +
  invalid enum one-liner), classes.rst:275 (gen1), embedding/cpp_api.rst:188 (born-wrong
  ManagedVectorAnnotation<int32_t> + plain addAnnotation; correct:
  addVectorAnnotation<vector<int32_t>>(this, lib, "IntVector")).
