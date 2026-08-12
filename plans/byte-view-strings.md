# Byte-view string surface + PERF031 re-sweep

Goal: the string surface gets 1-to-1 counterparts over `array<uint8>` / `array<uint8>#`
(the `peek_data` view — length is carried, so no operation ever re-strlens its source),
implemented as the SAME C++ cores the string entries call. Then the PERF031 fix recipe is
mechanical — wrap the loop in `peek_data`, swap `slice(s,…)` → `slice(d,…)` — and the tree
sweep lands with 2–4-line diffs instead of hand-rolled byte walks. One PR, one commit per
phase.

Context: sweep v1 (branch `bbatkin/perf031-sweep`, unpushed) rewrote 111 sites by hand and
was vetoed — the hand-rolled `bytes_substr`/`span_str`/`write_char`-loop pattern is the wrong
answer to a missing stdlib surface. Three v1 agents independently reported the gap. What
survives v1: the PERF031 rule-precision commit + fixture, the env-registry master-red fix,
four negative-controlled test gates, and the CODEREVIEW wording fixes (salvaged in phase 5).

## Design rulings (Boris, 2026-08-12)

- 1-to-1 names and semantics with the string counterpart wherever a counterpart exists.
- One C++ implementation per function: a length-bounded core `(const char * s, uint32_t len, …)`;
  the string entry is `stringLengthSafe` + core, the view entry is `arr.data + arr.size` + core.
  `array<uint8> const implicit` binds both array kinds (precedent: `builtin_string_from_array`)
  — no wrapper pairs.
- `is_space` vs `is_white_space` (strip family strips FF/VT, trim family does not) **is a bug**;
  the sets converge. Direction decided by phase-0 evidence.
- `replace`/`rtrim`/`trim` lose the return-the-input passthrough: always allocate on non-null
  input, gain `setTempStringResult` like `strip`. (Also fixes `delete_string(rtrim(s))` freeing
  `s` when the passthrough fired.)
- Writer range-copy is named `write_string` (`write(w, d, …)` would be obscure; `write(w, d)`
  already means "print an array").
- Parse-at-offset family ships now, not phase-2 ("might as well").

## Surface (from the v1 111-site inventory)

New externs, registered beside their string counterparts in `module_builtin_string.cpp`,
temp-string results marked, view param `array<uint8> const implicit`:

| family | view externs | count |
|---|---|---|
| slice / chop | `slice(d,a,b)` `slice(d,a)` `chop(d,a,n)` → string | 3 |
| starts_with | `(d,cmp)` `(d,cmp,cmpLen)` `(d,off,cmp)` `(d,off,cmp,cmpLen)` | 4 |
| ends_with | `(d,cmp)` | 1 |
| find / rfind | `find(d,sub)` `(d,sub,start)` `(d,ch)` `(d,ch,start)`; `rfind(d,sub)` `(d,sub,start)` | 6 |
| strip family | `strip(d)` `strip_left(d)` `strip_right(d)` → string | 3 |
| trim family | `trim(d)` `ltrim(d)` `rtrim(d)` `rtrim(d,chars)` → string | 4 |
| parse-at-offset | `int8/uint8/int16/uint16/int/uint/int64/uint64(d, result : ConversionResult&, offset : int&, hex)`, `float/double(d, result, offset)` | 10 |
| writer | `write_string(w,d)` `write_string(w,d,a,b)` | 2 |

Total ~33. Needles stay `string` (short constants; no v1 site wanted view needles).
NOT twinned, with reason: `length`/`empty` (array builtins already), `string(d)` (exists),
`character_at`/`first_character` (`int(d[i])` is O(1)+checked; `is_character_at` already in
strings_boost), `is_*` classifiers (take `int`), whole-string transforms (`to_upper`, `reverse`,
`repeat`, `escape`, `replace`, `compare_ignore_case`, `format` — compose via `slice(d,…)`),
lifecycle/builder ops. daslib sugar (`split(d)`, `contains`, `count`, `last_index_of`,
`trim_prefix/suffix`, `is_null_or_whitespace`) is phase-2-if-a-site-demands, das-side atop
the A set.

## Phase 0 — pin & audit (no behavior change)

1. Pin tests for the current string-side behavior of every function getting a twin or a
   semantic change: slice/chop negative-wrap + clamp, find/rfind returns incl. empty-needle
   convention, all four starts_with overloads, strip null-on-all-space + FF/VT set, trim
   4-char set + passthrough, replace passthrough, parse-at-offset ConversionResult/offset on
   garbage/overflow/hex/trailing, `string(d)` with interior NUL. These are the refactor's
   regression net; the pins that phase 1 consciously flips get flipped with a comment.
2. Whitespace-set evidence: enumerate `is_space` / `is_white_space` definitions, every C++
   caller, and every das-side caller of the exposed `is_white_space(int)` classifier
   (daslib lexers/parsers included — toml, clargs, json, lint internals). Deliverable: the
   convergence direction (default proposal: C `isspace` 6-set everywhere, classifier included)
   plus the explicit list of user-visible changes. **Top risk of the arc lives here** — the
   classifier is called from daslib parsers whose tests must stay green or change consciously.
3. Identity-reliance audit: in-tree uses of `replace`/`rtrim`/`trim` results that depend on
   pointer identity or delete the result (expect none).
4. ABI note: result temp-ness changes on `replace`/`rtrim`/`trim` — check external-canary
   exposure per `skills/abi_break_sweep.md` (expected: recompile-clean, no spelling change).

## Phase 1 — C++ core unification + semantic fixes

- Length-bounded cores for slice, chop, starts_with, ends_with, find, rfind, strip×3, trim×4,
  replace (core shared even though replace gets no twin), parse family.
- The three NUL-dependence rewrites (the genuinely new C++): `find`/`rfind` cores become
  bounded memchr/memmem-style searches (no `strstr`); `strip_l`/`strip_r`/trim walkers take
  `len`; integer parse becomes a bounded parse (float/double already run begin/end via
  fast_float).
- Whitespace convergence per phase-0 decision; passthrough removal + `setTempStringResult`
  on `replace`/`rtrim`/`trim`.
- Gate: phase-0 pins (with conscious flips), tests/ suite, JIT smoke; bump nothing — builtins
  self-invalidate JIT caches via aot hashes.

## Phase 2 — view externs

The ~33 externs above, one commit. Each is entry-glue over a phase-1 core — no new logic.

## Phase 3 — test surface (before any sweep)

- Parity property harness: for every twin, `twin(d, args) == counterpart(string(d), args)`
  over generated NUL-free buffers × an edge lattice (empty view, whole view, negative and
  overshooting indices, needle at boundary, offset == len, all-space, hex/garbage for parses).
- Interior-NUL direct tests: views are binary-safe by design — bounded cores treat `\0` as
  data; document that materializing a NUL-containing window yields a string downstream
  strlen-based ops see as truncated (same as `string(d)` today).
- Core edge tests for the three NUL-dependence rewrites specifically.
- Placement per `skills/tests_in_repo.md`; new tests dir registered for AOT
  (`tests/aot/CMakeLists.txt`) so the externs get AOT coverage.

## Phase 4 — documentation surface

- das2rst: every new extern in a named group (no Uncategorized), stubs filled, both passes.
- `skills/strings.md`: the slice/chop strlen trap section now leads with the recipe
  (`peek_data` + view ops); view-ops table; NUL note; whitespace-set change note.
- `doc/source/reference/language/lint.rst`: PERF031 entry rewritten around the recipe;
  PERF031 message text in `daslib/perf_lint.das` updated to name it (fixture follows).
- CLAUDE.md "Code style" rows that recommend hand `peek_data` walks get the recipe instead.
- Sphinx clean per `skills/make_pr.md` 4f.

## Phase 5 — mechanical re-sweep + salvage

- Fresh branch; cherry-pick from v1: env-registry fix (`4076ee11d`), rule precision + fixture
  (`6d81e072c`), the audit round (gates + CODEREVIEW fixes). The two gates that pinned v1
  semantics (`log_summary` unterminated-line widening, `tok_read_seed`) are honored or
  consciously adjusted by the re-sweep — never silently.
- Opus agents redo the sites FROM MASTER BASELINE (not from v1) with the recipe; same fix bar
  as v1 (read, don't pattern-replace); most v1 nolints should convert to real fixes (they
  existed only because materialization cost a strlen); external-repo hits (4 dasImgui under
  examples/daStrudel, 3 das-claude under utils/find-dupe) stay out — v1's verified das-claude
  upstream patch gets filed against `borisbat/dasAnthropic`.
- End state: tree lint sweep-clean under PERF031 with the rule ON repo-wide (`.lint_config`
  carries no override), only external-checkout hits remaining locally.

## Phase 6 — PR mechanics

`skills/make_pr.md` in full: CODEREVIEW auditors over the changed set, detect-dupe, AST
verify (macro + daslib changes), workaround audit, docs gates, one `preflight --full`,
push, Copilot babysit loop. `.md` diffs (skills, CODEREVIEW, this plan) listed for review
before push.

## Risks

1. `is_white_space` classifier convergence ripples into daslib parsers — phase 0 quantifies;
   every affected caller changes consciously or the direction gets revisited.
2. Temp-result flag on `replace`/`rtrim`/`trim` — external code storing results is fine
   (temp string assignment is legal); pointer-identity reliance would break (expected none;
   phase 0 checks in-tree, externals sweep per `skills/abi_break_sweep.md` if phase 0 finds
   any spelling change).
3. Bounded-search cores (`find`/`rfind`) are the largest genuinely-new C++ — covered by
   parity + direct edge tests before any consumer lands.
4. Interior-NUL semantics are new user-visible surface — documented, not papered over.
