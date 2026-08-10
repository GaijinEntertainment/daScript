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

- (2026-08-10) Rule-0 probe on M1: zero true binary gaps; all scares were require-spelling
  errors (dasSQLITE → sqlite/sqlite_boost, openai → openai/openai_chat, peg → peg/peg,
  strudel → strudel/strudel). llvm/vulkan/anthropic referenced by no authored RST.
- (2026-08-10) Confirmed doc bugs pre-scan: 53_clargs.rst (whole-page parse_args drift +
  invalid enum one-liner), classes.rst:275 (gen1), embedding/cpp_api.rst:188 (born-wrong
  ManagedVectorAnnotation<int32_t> + plain addAnnotation; correct:
  addVectorAnnotation<vector<int32_t>>(this, lib, "IntVector")).
