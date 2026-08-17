# Adding a PERF lint rule (repo-only)

Read before changing `daslib/perf_lint.das`. What a `PERF*` finding means to whoever hits
it — and the `[hot_path]` contracts — is `skills/perf_lint.md`. The STYLE twin is
`skills/internal/style_lint_authoring.md`: same policy layer, same fixture conventions.

## The pieces

| Piece | Where |
|---|---|
| Module + `[lint_macro] class PerfLintMacro : AstPassMacro` | `daslib/perf_lint.das` |
| The walk — `class PerfLintVisitor : AstVisitor` | same file |
| Warning sink — `macro_performance_warning(...)`, error code 31208 | same file |
| Enablement policy, `LintIssue`, `is_user_authored_body` | `daslib/lint_config.das` |
| Standalone runner (paranoid + perf + style) | `utils/lint/main.das` |
| Fixtures, one per rule | `utils/lint/tests/` |
| Rule documentation (LINT + PERF + STYLE in one page) | `doc/source/reference/language/lint.rst` |

Public entry points: `perf_lint(prog; compile_time_errors[; disabled_codes])`,
`perf_lint_collect(prog; var warnings[; disabled, enabled])`, and
`perf_lint_collect_issues(prog; var issues; disabled, enabled)` — the structured form, since
`LintIssue` carries the code and position an LSP diagnostic needs. The unfiltered pairs are thin
wrappers; every real caller (MCP subtool, runner, LSP) passes tables.

## Enablement is never per-rule bespoke

It goes through `daslib/lint_config.das`, shared by all three passes:

- `seed_default_disabled(disabled)` — the canonical default-off set. A rule that must ship silent
  is declared here and nowhere else.
- `load_lint_config(...)` — layers the repo `.lint_config` `[rules]` table on top, either
  direction (`PERF007 = false`, `STYLE005 = true`).
- `build_lint_macro_disabled(prog)` — what each `[lint_macro]` `apply()` calls. Honors the
  module-local `options _enable_default_off_rules = true` (fixture mode: skips both the
  default-off seed and the config's `= false` directives, so a fixture always exercises its rule),
  then `.lint_config` re-enables, then `$DAS_LINT_DISABLE`.

## Adding a rule

1. Take the next free `PERFnnn`.
2. Override the visitor method that sees the shape:

| Looking for | Override |
|---|---|
| a call | `preVisitExprCall` |
| an operator | `preVisitExprOp2` (check `expr.op`) |
| assignment / copy / move | `preVisitExprCopy`, `preVisitExprMove` |
| interpolation | `preVisitExprStringBuilder`, or `...Element` for a per-element rule |
| a `while` condition | the `in_while_cond` flag set in `preVisitExprWhile` |
| anything loop-scoped | `loop_depth > 0`, the `loop_variables` / `var_stack` state |

3. Report with `perf_warning("PERFnnn: what it is; the fix", expr.at)`. Call it **bare** —
   `self->perf_warning(...)` trips this tree's own STYLE028. The method covers both compile-time
   and runtime (print) modes, and dedups by location.
4. Write `utils/lint/tests/perfNNN_<name>.das` with a bad example and a good one:

```das
options gen2
options auto_inline_functions = false   // fixtures assert SOURCE shapes; splices rewrite them
expect 31208:2
require daslib/perf_lint
```

   Without the `auto_inline_functions` line inline splices rewrite the shapes the fixture asserts
   and the counts go flaky. Run it:
   `bin/daslang utils/lint/main.das -- utils/lint/tests/perfNNN_<name>.das --perf-only`.
5. Document the rule in `doc/source/reference/language/lint.rst`.

## Visitor state

`loop_depth` (for/while nesting), `var_stack` (every declared variable with its loop depth and
whether it is an iteration variable), `scope_stack` (`var_stack` length saved per block for
pop-on-exit), `in_while_cond`, `in_closure` (closure depth — a lambda declared in a loop does not
run in it).

A variable is outside the current loop when its `var_stack` entry has `depth < loop_depth`;
arguments and globals are not on the stack and count as outside. Types are resolved by lint time,
so `expr._type.baseType` is reliable.

## Gotchas

- **Do not early-return on `in_closure > 0` in `preVisitExprOp2`.** `loop_depth` already stops
  incrementing inside closure bodies, so loop-scoped rules are correct without it — and the
  blanket skip hides the syntactic rules (PERF007/008/010/013/014/017) inside the ordinary
  `build_string() $(var w) { ... }` idiom.
- **Generated functions must stay silent.** `perf_warning` early-returns on
  `current_function.flags.generated`; otherwise codegen (`[CommandLineArgs]`-style) reports at the
  source struct's line with no way to act on it.
- **Self-implementation.** When the suggested replacement is itself written in terms of the
  pattern (`empty(arr)` IS `length(arr) == 0`), gate the rule on
  `current_function.name == "<callee>" || (fromGeneric != null && fromGeneric.name == "<callee>")` —
  the second arm catches instantiations.
- **Hot-path rules (PERF026-028) are annotation-gated**, so their cost is a closure walk from
  annotated roots only. Keep it that way: the five markers are registered C++-side as
  metadata-only annotations precisely so code under contract never has to `require` this module.
