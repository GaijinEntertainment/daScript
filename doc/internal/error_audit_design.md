# daslang error-code refactor — audit synthesis & design

This document is the design output of Phase B of the error-reporting refactor.
It is **a working document**, not user-facing documentation. Source of truth
during implementation; can be moved to a permanent design-docs location once
the work lands, or removed if not useful.

## Source artifacts

All produced by the audit pipeline at `D:/daslang-error-audit/`:

| Artifact | Role |
|---|---|
| `D:/daslang-error-audit/taxonomy.md` | The shared facet/subject/modifier vocabulary used by audit agents |
| `D:/daslang-error-audit/synth_design.py` | Reads `error_audit.csv`, produces the design artifacts below |
| `D:/daslang-error-audit/design/new_enum_draft.h` | Drafted replacement for `compilation_errors.h` (561 entries) |
| `D:/daslang-error-audit/design/uniques.tsv` | Each new enum name + its site count + the old cerrs that mapped into it |
| `D:/daslang-error-audit/design/legacy_map.tsv` | Per-site: file, line, old cerr, new name, new num — input to retag/sweep |
| `D:/daslang-error-audit/design/verbose_gate.tsv` | 89 sites that should be wrapped in `if (verbose)` |
| `D:/daslang-error-audit/design/recategorize.tsv` | 62 sites whose cerr semantics change (not just rename) |
| `D:/daslang-error-audit/design/wording.tsv` | Wording fixes (typos, missing spaces, vague phrasing) |
| `doc/internal/error_audit.csv` | The consolidated audit (1116 rows, 1 per error site) — checked in |

## What was audited

Every `error(...)` call site in:
- `src/ast/*.cpp` (semantic, infer, lint, validate, simulate, derive_alias, export, const_folding, annotations, typedecl)
- `src/parser/*.ypp`, `*.lpp`, `parser_impl.cpp` (lexer/parser sources only — generated `.cpp` skipped)
- `src/builtin/*.cpp`, `src/simulate/*.cpp`

**Total: 1116 sites across 32 source files.**

Six parallel Explore-style agents performed the inventory; each wrote a CSV
slice; slices consolidated into `doc/internal/error_audit.csv` with columns:
`file, line, current_cerr, message_template, context_function, classification,
tag_composition, proposed_enum_name, wording_change`.

## Decisions locked with Boris

| Topic | Decision |
|---|---|
| Storage | Flat `enum class CompilationError` — unchanged shape |
| Naming | Tag-composition discipline: `<facet>_<subject>[_<modifier>]` |
| Migration scope | Bundle everything in one PR — retag + wording + recategorize + verbose-plumbing + dedup helper + 111-test sweep + fix the 5 pre-existing under-specified `expect` lines |
| Renaming policy | Rename `*_not_found` → `lookup_*` for prefix consistency. Other agent-proposed names kept as-is (after mild collapse of trailing modifier suffixes). |
| Granularity | Mild collapse: 1116 sites → 561 unique codes (was 632 before collapse). Modifier suffixes like `_void`, `_ref`, `_const`, `_value` are dropped from cerr names; the info stays in the message text. |
| Follow-on / internal granularity | Fine-grained per subject. `not_resolved_yet_<subject>` for each subject (38 codes); `internal_<subject>_<facet>` for each (42 codes). |
| Numbering | Loose ranges: 1xxxx lex / 2xxxx parse / 3xxxx semantic / 4xxxx logic / 5xxxx integration. Inside a stage, cluster by leading facet (within-stage offset = `(facet_idx % 90 + 1) * 100`). New codes go next-available within the cluster. |
| Verbose flag | Reuse existing `InferTypes::verbose` ([include/daScript/ast/ast_infer_type.h:30](../../include/daScript/ast/ast_infer_type.h#L30)) — no new user-facing flag. Plumb through 89 emit sites identified by audit. |
| Taxonomy expansion | Accept 11 new tags surfaced by the audit: 8 subjects (`string, comment, indentation, runtime, iterator, pointer, buffer, context`) + 3 facets (`runtime, performance_lint, style_lint`). Reject narrow parser-only tags (`aka, pipe, keyword, dispatcher`). |

## The naming discipline

```
<facet>_<subject>[_<subject2>][_<modifier>]
```

| Slot | Source | Examples |
|---|---|---|
| facet | what kind of fault | `invalid`, `lookup`, `cant`, `missing`, `mismatching`, `exceeds`, `ambiguous`, `already_declared`, `not_resolved_yet`, `internal`, `unsafe`, `recursion`, `runtime` |
| subject | what construct | `type`, `function`, `argument`, `result`, `structure`, `field`, `variable`, `array`, `table`, `tuple`, `variant`, `enumeration`, `enumerator`, `expression`, `module`, `annotation`, ... (full list in `taxonomy.md`) |
| modifier | rare; only when needed | (mostly stripped during normalization — they live in message text, not the cerr) |

Composition order: facet first, then subject(s) general → specific. So
`function | argument | type | invalid` becomes `invalid_function_argument_type`.
Multi-word facets (`not_resolved_yet`, `already_declared`) are matched as a
single facet token.

**Future contributors:** when adding a new error site, derive the cerr from the
tag composition. If the composed name doesn't yet exist in
`compilation_errors.h`, add it.

## The new enum — facet sizes

| Stage | Facet | Count | Notes |
|---|---|---|---|
| 1xxxx (lex) | invalid | 2 | line, string |
| | mismatching | 3 | curly_bracers, parens, module_name |
| | exceeds | 2 | constant ranges, file boundary |
| 2xxxx (parse) | invalid | ~30 | grammar-rule rejections |
| | missing | 1 | module_name |
| | exceeds | 1 | bitfield |
| | already_declared | ~10 | parser-detected name collisions |
| 3xxxx (semantic) | invalid | 213 | the bulk — type/argument/field/etc faults |
| | cant | 62 | `cant_<verb>` family — operations forbidden in context |
| | lookup | 46 | `*_not_found` renamed to `lookup_*` |
| | unsafe | 37 | unsafe-required operations |
| | not_resolved_yet | 37 | follow-on cascade family (verbose-gated) |
| | missing | 22 | required-but-absent (return paths, init, etc.) |
| | already_declared | 12 | name collisions |
| | exceeds | 16 | counts/sizes/ranges |
| | ambiguous | 17 | multi-candidate resolution |
| | mismatching | 15 | two things expected to match don't |
| | recursion | 7 | unbounded/forbidden recursion |
| 4xxxx (logic) | (lint) | small | performance_lint, style_lint families |
| 5xxxx (integration) | internal | 42 | validate-AST stubs, DAS_VERIFY, "should not happen" sites |
| | runtime | 14 | DataWalker/hash/serializer compile-time-eval errors |

**Total: 561 distinct codes** + 1 `unspecified = 0`.

Full enum draft: `D:/daslang-error-audit/design/new_enum_draft.h`. To copy into
`include/daScript/ast/compilation_errors.h` in Phase C.

## Transition by phase

### Phase C — Apply enum + emit-site retags + wording fixes

1. Replace `include/daScript/ast/compilation_errors.h` body with `new_enum_draft.h` content (preserve `#pragma once` + namespace + `unspecified = 0` line).
2. For each site in `legacy_map.tsv`, edit the `error(...)` call to pass the new cerr value. The TSV is per-site `(file, line, new_name)` — straight mechanical retag.
3. For each site in `wording.tsv` (~22 sites), apply the wording fix inline. Examples:
   - `subexpresion → subexpression` (×2: ast_infer_type_helper.cpp:966, 969)
   - `strcut → struct` (ast_infer_type_function.cpp:1162)
   - `cant't → can't` (ast_infer_type.cpp:4126)
   - `intializer → initializer` (parser_impl.cpp:791)
   - `simluateNullCoalescing → simulateNullCoalescing` (ast_simulate.cpp:1352)
   - missing space at ast_infer_type.cpp:312 (`<name>can't` → `<name> can't`)
   - duplicate emit at ast_infer_type.cpp:1617/1620 — drop the second
   - inverted logic at ast_simulate.cpp:3874 (`stt.size()==1` triggers "unknown structure" — should be `>1`)
   - 7 sites in `ds_lexer.lpp` saying "curly braces"/"square braces" but tagged `mismatching_parens` — retag accordingly
4. Update consumer files (only matters if they reference enum names by-symbol; none currently observed but check during edit):
   - `utils/daScript/main.cpp`
   - `utils/daslang-live/main.cpp`
   - `utils/dasFormatter/`
   - `include/daScript/simulate/runtime_string.h` (the `reportError` formatter — unchanged behavior)
5. `include/daScript/ast/ast_infer_type.h:5` declares CompilationError values used in inference; verify still resolve.
6. Regenerate parsers (CMake handles `.ypp`/`.lpp` automatically when sources touched).

Worker plan: split Phase C across 4-6 subagents by file group, mirroring the inventory-agent split. Each agent reads `legacy_map.tsv`, filters by its files, applies edits via Edit tool. **No hand-tuning** — the TSV is the source of truth.

### Phase D — Plumb `verbose` through 89 emit sites

`verbose_gate.tsv` lists every site classified `mid_inference` or `follow_on`.
89 rows. For each:

```cpp
if (verbose) {
    error(...);
}
```

`verbose` is `false` on intermediate infer passes (no work, no emit) and
`true` on the final failure-explanation pass via `reportingInferErrors`. Match
the style of the ~25 existing `if (verbose)` sites in `ast_infer_type.cpp`
(e.g. lines 1011, 3712, 4135).

### Phase E — `Program::deduplicateErrors()`

See plan file `C:/Users/Boris/.claude/plans/look-at-wip-das-and-synchronous-quill.md` "Phase E" — implementation per the four-rule algorithm. The `not_resolved_yet_*` family is identified by the facet prefix; Rule 4 drops all of them when ANY non-not_resolved_yet, non-unspecified code exists.

### Phase F.1 — Test sweep

`D:/daslang-error-audit/regen_expect.py` already validated baseline (106/111 match, 5 pre-existing under-specified `expect` lines logged at `D:/daslang-error-audit/expect_report.tsv`).

After Phase C-E land:
1. Re-run `regen_expect.py` — every test will now mismatch.
2. Run with `--write` to apply the new emitted codes in-place.
3. Manual review of sample (~20 files) confirms correctness.
4. Fix the 5 pre-existing under-specifications (`cant_get_field.das`, `failed_aliasing.das`, `failed_call_depth.das`, `failed_condition_must_be_bool.das`, `failed_const_ref.das`) — full multiset, not subset.

### Phase F.2 — Final verify

1. `examples/wip.das` — exactly one error.
2. Full test suite green: `daslang.exe dastest/dastest.das -- --test tests`.
3. AOT suite green: `test_aot.exe -use-aot dastest/dastest.das -- --use-aot --test tests`.
4. MCP smoke: `compile_check` on `daslib/`, `tutorials/`, `D:\DASPKG\*\*.das`.
5. Negative test: a `.das` emitting only `not_resolved_yet_*` errors prints them all (Rule 4 doesn't fire when no primary).
6. Performance: time `dastest -- --test tests` before/after; expect small (<5%) speedup on tests that fail compile (verbose-plumbing avoids the work on intermediate passes).

## Open notes / followups

1. **Recategorization scope.** The 62 `recategorize` sites change cerr semantics, not just names. Most consequential clusters in the audit:
   - 12 shadowing sites in ast_infer_type.cpp (L3298…L4901) marked `variable_not_found` → `already_declared_variable`
   - 4 "returning … requires unsafe" sites carrying `invalid_return_type` → `unsafe_function_result`
   - 3 call-depth sites carrying `too_many_infer_passes` → `exceeds_call_depth`
   - 8 lookup-style codes reused for ambiguous-multi-match → `ambiguous_<subject>` family
   - Per-site detail in `recategorize.tsv`.

2. **Duplicate emit at ast_infer_type.cpp:1617/1620.** Audit flagged identical message emitted twice in adjacent lines. Drop the second during Phase C wording sweep.

3. **`mismatching_parens` mislabel cluster** (7 sites in ds_lexer.lpp at L989, 1005, 1009, 1026, 1030, 1047, 1064) — message says "curly braces" or "square braces" but cerr is `mismatching_parens`. Retag during Phase C using `mismatching_curly_bracers` (existing) or new `mismatching_square_brackets`.

4. **Inverted logic at ast_simulate.cpp:3874.** Audit flagged: `stt.size()==1` triggers "unknown structure" but should be `>1`. This is a *bug fix*, not an audit change. Apply alongside wording sweep but call it out in the PR description as such.

5. **Sites with empty `current_cerr` (default unspecified).** ~120 sites total. After Phase C every site has a cerr; the `unspecified` code becomes a fallback for runtime-emitted errors only.

6. **Tests with `expect` headers under-specified pre-refactor.** `dastest` accepts subset matches today, hiding the under-spec. Phase F.1 explicitly tightens these 5 to full multiset matches.

7. **Parser-side `expect` directive.** Stays numeric (`expect 30305:3`). No syntax change. The 111-test sweep handles renumbering.

8. **Backwards compatibility for code that reads `CompilationError::xxx` by-symbol.** All four consumer files (`utils/daScript/main.cpp`, `utils/daslang-live/main.cpp`, `utils/dasFormatter/`, `runtime_string.h`) currently flow numeric codes through; no symbol references observed. Re-verify during Phase C.
