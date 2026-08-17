# Style lint (`STYLE*`)

Read this before suppressing a `STYLE*` finding, and for the STYLE037/038 resolution policy.
`daslib/style_lint.das` is a `[lint_macro]` AST pass that reports non-idiomatic shapes as
error code 31209. How to run lint, and the shared `.lint_config` / `DAS_LINT_DISABLE` /
`--enable` / `--disable` rail, are in `skills/perf_lint.md`; the per-rule knobs are below.

There is no per-rule catalog here: every finding's error text states the pattern and the
rewrite on its own. A finding you cannot act on from its message alone is a defect of the
message — fix the text in `daslib/style_lint.das`, don't document around it.

Suppress one finding with `// nolint:STYLEnnn` on the line. Per-module `options` knobs:
`_comment_hygiene` (STYLE014/015), `_ascii_strings` (STYLE039), `_cyclomatic_complexity`
(STYLE037), `_function_length` (STYLE038), `_duplicate_regions` plus `_dupe_min_nodes` /
`_dupe_min_statements` (STYLE040), and `_enable_default_off_rules` to force default-off rules on.

## STYLE037 / STYLE038 — suppress an honest shape, never force a split

Both metric rules spell their escapes the same way: `// nolint:STYLE03x` on the `def` line, or a
per-module `options _cyclomatic_complexity = N` / `options _function_length = N` (`0` disables).

**New code should just follow the limits.** Design functions under both from the start; the
escapes exist for irreducible shapes, not as a license to write oversized code and silence the
warning.

For EXISTING code a hit is a prompt to look, not an order to refactor. Never rewrite a working
function — let alone a file's worth — to get under a number: mechanical extract-a-helper splits
hide control flow behind single-caller helpers, and a wide refactor of working code is riskier
than the count it removes. Options 1 and 2 are peers; pick by whether a natural seam exists.

1. **Split along a natural seam** — a self-contained arm, a repeated pattern that collapses into
   one helper, a distinct phase of a multi-phase function, genuinely duplicated logic. The
   extracted helper must stand on its own; if its only virtue is that the caller's number went
   down, do not extract it.
2. **Suppress, with a short tail-comment reason**, when the shape is irreducible by design:
   exact-type `is` ladders over handled AST types, visitor and dispatch tables, flat CLI-argument
   handling, flat one-call-per-item lists (PSO compile/release runs, registration tables), and GPU
   kernel bodies whose phases are coupled by barriers, simdgroup ops or register residency and
   cannot cross a function boundary without changing the shader. Leaving a known-honest warning to
   accumulate just trains everyone to ignore the rule.
3. **Raise or disable the limit per module** when a whole file is legitimately dense — codegen
   emitters, parsers, ported code.

**A refactor that grows an already-over-cap function is not automatically wrong.** Folding N
scalar parameters into one struct adds N unpack lines; if every field is used several times,
inlining `ka.field` at each use is noisier, not shorter. Take the growth and either suppress or
ledger the real seam — but never suppress a function you have just argued is reducible.
