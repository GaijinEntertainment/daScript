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
- Parse-at-offset family ships now, not phase-2 ("might as well"). The view forms take
  `offset` as IN/OUT (entry = start position, leading whitespace skipped from there; exit =
  consumed-through position on success, unchanged on failure) — the cursor form is the point;
  the string counterparts keep their out-only offset. Parity oracle compares at offset 0.
- `find(s, ch)` / `find(d, ch)` char form: **unsigned-byte matching both sides** (Boris,
  2026-08-12). `ch` matches as a byte value; outside 0..255 returns -1. `find(s, 195)` now
  hits byte 0xC3, `find(s, -61)` no longer does — conscious string-side flip, pins updated.
  (Phase 1 had preserved the historical signed-char matching; phase 2 flips it.)

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
| whitespace cursor | `skip_white_space(d, from)` → int, plus the `string` twin | 2 |
| trim family | `trim(d)` `ltrim(d)` `rtrim(d)` `rtrim(d,chars)` → string | 4 |
| parse-at-offset | `int8/uint8/int16/uint16/int/uint/int64/uint64(d, result : ConversionResult&, offset : int&, hex)`, `float/double(d, result, offset)` | 10 |
| writer | `write_string(w,d)` `write_string(w,d,a,b)` | 2 |

Total ~35. `skip_white_space` is the one op the re-sweep added to this list rather than
finding here: the offset form of `strip_left` has no string counterpart to twin, and without
it five sites independently hand-rolled the same private cursor helper. It is the only
member of the surface whose *string* form is also new. Needles stay `string` (short
constants; no v1 site wanted view needles).
NOT twinned, with reason: `length`/`empty` (array builtins already), `string(d)` (exists),
`character_at`/`first_character` (`int(d[i])` is O(1)+checked; `is_character_at` already in
strings_boost), `is_*` classifiers (take `int`), whole-string transforms (`to_upper`, `reverse`,
`repeat`, `escape`, `replace`, `compare_ignore_case`, `format` — compose via `slice(d,…)`),
lifecycle/builder ops. daslib sugar (`split(d)`, `contains`, `count`, `last_index_of`,
`trim_prefix/suffix`, `is_null_or_whitespace`) is phase-2-if-a-site-demands, das-side atop
the A set.

## Phase 0 — pin & audit (no behavior change) — DONE 2026-08-12

1. Pin tests: `tests/strings/strings_core_pins.das` — 20 `[test]` functions, 172 pins, green
   interp + `-jit`, AOT TU compiles; no registration needed (`tests/strings/` already globbed
   in `tests/aot/CMakeLists.txt`). Flip candidates carry `// PHASE1-FLIP:` tail comments
   (6 markers: trim FF/VT ×2, strip FF/VT ×2 for the reversed direction, one over the five
   aliasing pins). All haystacks go through a non-foldable `heap_str()` helper — literal
   inputs const-fold the builtin away and pin the folder against itself.
2. Whitespace direction: **C `isspace` 6-set everywhere, classifier included**, with ONE
   carve-out — `daslib/json.das`'s lexer (line 198) gets a private spec-exact
   `is_json_white_space` (RFC 8259 ws is exactly the 4-set; the lexer is spec-correct today
   and nothing pins it — add the FF/VT-rejection pin). Evidence: 4-set direction touches 428
   `strip*` sites; 6-set touches 3 trim builtins (~70 no-arg call sites), 30 parse builtins
   (provably monotonic — FF/VT can't start a number, so no currently-succeeding parse
   changes), and the classifier (14 files, no correctness dependence outside json). It also
   fixes the dasLLAMA `strip_think` divergence (its gate rides the 4-set, its tail and the
   splitter the 6-set). Conscious updates owed in later phases:
   `daslib/strings_boost.das:378` docstring (names the 4-set; flows into RST),
   `tests/strings/test_cpp_functions.das:241` stale comment, FF/VT pins added to
   `strings_traits.das` / `test_new_string_functions.das` / `test_think_split.das` parity
   table, dasPEG `WS` widening documented. `linq_das.das` is safe (das source can't carry
   FF/VT — lexer rejects them outside literals).
3. Identity-reliance: **zero** (class-b and class-c empty). `delete_string` is the only
   string-free path; all 17 sites free builder/fread results, never a trim/replace result.
   `daslib/temp_strings.das` rejects non-builder args at compile time. `replace`'s flip is
   near-unobservable in-tree — every real site passes a non-empty needle.
4. ABI note: **GO — recompile-clean, no spelling change, no serializer bump**
   (`tempStringResult` already serializes). daspkg-universe GitHub sweep: zero observers.
   Two AOT caveats: every caller of replace/rtrim/trim re-hashes (wipe `_aot_generated`
   before the phase-6 preflight — preflight does not wipe it), and `is_white_space` is
   `__forceinline` in the public AOT header, so prebuilt AOT artifacts keep the inlined
   4-set until recompiled — name the mandatory-recompile in the PR body.

### Phase 0 corrections to this plan

- **`ltrim` needs no phase-1 change** — it already always-allocates and is temp-marked.
  The passthrough set is **5 cases across 3 functions**: `rtrim(s)` exact alias,
  `rtrim(s,chars)` on nothing-trimmed AND on empty/null `chars` (line 763 — reachable from
  das), `trim(s)` exact alias, `trim(s)` **interior alias** (`s + k` after leading-ws skip —
  `delete_string(trim(s))` frees mid-buffer), `replace(s,"",x)` empty-pattern alias.
  `replace` has NO no-match passthrough (line 606 allocates on miss).
- **Atomicity constraint**: `MarkTempStrings`/`WrapLetTempStrings`
  (`ast_allocate_stack.cpp:1160,1239`) use `!tempStringResult` as the guard refusing to
  queue temps through a possible-passthrough callee — setting the flag before removing the
  passthrough is a use-after-free. Passthrough removal + the `setTempStringResult` calls
  land in ONE commit. Source-compat proven by a 14-shape A/B probe against already-marked
  `strip`/`ltrim` — marking cannot newly reject anything.
- **Parse-family warts found by the pins — phase-1 decisions (defaults below, Boris to
  veto)**: (a) `ConversionResult` is never written on SUCCESS (stale value survives; every
  caller must pre-init) — default: set `ok` on success in both string and view forms, flip
  the pin; (b) failure-`offset` is asymmetric (integer parse leaves 0, real parse keeps the
  whitespace count) — default: unify on the integer behavior (offset written on success
  only), flip the real-side pin. Parity oracle requires string and view to agree either way.
- Two tests flip consciously in phase 1: `tests/strings/temp_string_reclaim.das:167-209`
  (premise "trim returns an interior pointer" dies — repoint at the das-side `wrap_pass`);
  `tests-cpp/small/test_temp_wrap_fixture.das` re-hashes (stays green; it's the canary).
- Phase-3 trap: the parity harness must build inputs at runtime (`heap_str` pattern) or it
  compares the const-folder against itself.

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
- **A gap the re-sweep surfaces is closed as stdlib surface, not as a private helper in the
  consuming file** — a private cursor helper is the same defect v1 was vetoed for, one file
  down. `skip_white_space` is the worked example: five sites (dasllama_chat, `cpp_skip_space`
  in the MCP cpp tool, `skip_ws` in doc-verify, two inline loops in the MCP outline tool,
  one in `linq_das`) each grew their own copy before the op existed; all five now call it.
  Still missing, and still hand-rolled once (`trimmed_end` in `utils/doc-verify/main.das`):
  the offset form of `strip_right`. Add it if a second site wants it.

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
