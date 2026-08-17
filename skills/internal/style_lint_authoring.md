# Adding a STYLE lint rule (repo-only)

Read before changing `daslib/style_lint.das`. What a `STYLE*` finding means to whoever hits
it — and the STYLE037/038 resolution policy — is `skills/style_lint.md`. The PERF twin,
`skills/internal/perf_lint_authoring.md`, carries the shared enablement policy, the fixture
header, and the runner command; only the STYLE-specific parts are repeated here.

## The pieces

| Piece | Where |
|---|---|
| Module + `[lint_macro] class StyleLintMacro : AstPassMacro` | `daslib/style_lint.das` |
| The walk — `class StyleLintVisitor : AstVisitor` | same file |
| Warning sink — `macro_style_warning(...)`, error code 31209 | same file |
| Duplicate-region engine behind STYLE040 | `daslib/dupe_detect.das` |
| MCP `lint` tool: popen wrapper + the subtool that does the work | `utils/mcp/tools/lint_tool.das`, `utils/mcp/subtools/lint_tool.das` |
| Fixtures, one per rule | `utils/lint/tests/` |

The macro calls `style_lint(prog, true, build_lint_macro_disabled(prog), [comment_hygiene = <options
_comment_hygiene>])`. Comment hygiene defaults on for sources under `daslib/`
(`comment_hygiene_for`); `options _comment_hygiene` overrides in either direction. Public entry
points mirror the PERF set — `style_lint`, `style_lint_collect`, `style_lint_collect_issues` —
each with an optional `comment_hygiene` flag. STYLE005 is gated by the shared policy
(`seed_default_disabled`), never by a function parameter.

## Adding a rule

1. Take the next free `STYLEnnn`.
2. Override the visitor method that sees the shape — `preVisitExprCall`, `preVisitExprOp2`,
   `preVisitExprField`, `preVisitExprIfThenElse`, or a source-line read (below).
3. Report with `style_warning("STYLEnnn: what it is; the fix", expr.at)`. Call it **bare** —
   `self->style_warning(...)` trips STYLE028, this module's own rule.
4. Write `utils/lint/tests/styleNNN_<name>.das` with a bad example and a good one, using the
   fixture header from the PERF authoring skill (`expect 31209:N` here). A default-off rule needs
   `--enable <CODE>` or `options _enable_default_off_rules = true` in the fixture to fire at all.
   Run it: `bin/daslang utils/lint/main.das -- utils/lint/tests/styleNNN_<name>.das --style-only`.
5. Document the rule in `doc/source/reference/language/lint.rst`.

## Detection patterns worth reusing

- **Source-line inspection** (STYLE001-003, STYLE028, STYLE035). Syntax that desugars before lint
  runs is invisible in the AST: `foo() <| $(a) {}` and `foo() $(a) {}` compile to the same
  `ExprCall`; `self->m()`, `self.m()` and a promoted bare `m()` to the same invoke; `T('a')` folds
  to a typed const indistinguishable from `T(97)`. Read the original text with
  `get_file_source_line()` / `source_line_between()`, column-precise where the shapes differ by one
  byte (`expr.at.column` points at the `-` of `->`, at the `.`, or at the identifier). Two special
  cases: a generator is lowered to `ExprMakeStruct` + `each`, so STYLE001/002 match in
  `preVisitExprMakeStruct` after verifying the source starts with `generator<`; `defer` erases its
  call node before the pass, so `preVisitFunction` scans body lines for `defer` followed by `<|`
  and a block opener.
- **The `ExprLet` run scan** (STYLE012, STYLE013, STYLE021, STYLE027, STYLE031, STYLE032,
  STYLE033). All dispatch from `preVisitExprBlockExpression` when `expr is ExprLet`: locate the let
  inside `blk.list` by pointer equality, anchor on a variable with `init == null` and the expected
  `_type.baseType`, excluding `generated` / `inScope` / generic-host instantiations, then walk
  forward over contiguous statements matching a per-rule predicate. Argument matching always peels
  one `ExprRef2Value` (the typer inserts it whenever a ref-typed var is passed to a non-ref
  parameter), and callee identity is `call.func.fromGeneric.name` — a `das_string`, so compare it
  against a literal rather than binding it to a `let`.
- **Whole-block scans** (STYLE012's chain form, STYLE033's existing-target form) run from
  `preVisitExprBlock` instead of from a declaration, so they need explicit overlap handling: a run
  already owned by the declaration-anchored path is skipped, or the two double-fire.
- **Counter stacks** (STYLE026 nesting, STYLE037 complexity) — one slot per closure level, pushed
  on function entry and on `blockFlags.isClosure` entry, popped and checked on exit. A closure body
  gets a fresh slot: an outer `unsafe` does not propagate into a separate context, and a metric
  belongs to the host.
- **Accumulated state** (STYLE024/025). Unsafe-ness is folded child-to-parent —
  `preVisitExpression` pushes an `UnsafeFrame`, `visitExpression` pops it and adds its count to the
  parent — so a rule asks "how many nodes under this statement needed unsafe" instead of
  re-walking. Inherently-unsafe node kinds mark the stack; `genFlags.generated` subtrees are
  skipped.
- **Post-visit passes** (STYLE014/015 comment blocks, STYLE029/030 requires, STYLE040 duplicates)
  run after the walk and over `prog.getThisModule` only. The comment scan reconstructs blocks by
  reading source lines and maps each to the smallest enclosing function range, which is how the
  private-symbol cap and the module-leading-docstring exemption are decided.

## Facts that are not in this module

- **STYLE022's suggestion is only valid because the AOT side can express it**:
  `include/daScript/simulate/aot.h` provides `__bit_set` overloads for `Bitfield` /
  `Bitfield8/16/64` and for raw `uint8/16/32/64_t`, the latter covering handle-bound bitfield
  fields such as `Function::flags` (a `uint32_t` in C++).
- **Lambda, generator and local-function bodies lower to `flags.generated` carrier functions**
  whose bodies are verbatim user AST. Emit gates admit them via `is_user_authored_body`
  (`daslib/lint_config.das`), so every rule reaches them EXCEPT the metric rules: STYLE037/038
  leave carriers unmetered, because a `` _lambda_… `` name means nothing to the reader.
- **STYLE040's engine invariants** are in the module docstring of `daslib/dupe_detect.das` —
  read it before touching the hashing. Hashes only pick candidates; `subtree_equal` decides, so no
  collision can produce a finding, and `payload_key` is the single reader of per-node payload used
  by both hashing and verification so the two cannot drift.
- **Match arms count toward STYLE037** because `MatchMacro` emits plain `ExprIfThenElse` without
  the `generated` flag, and comprehension `for`/`where` nodes count for the same reason — only the
  wrapper closure block is marked generated. Generated closures bill the host, so the score cannot
  depend on whether the optimizer inlined the wrapper.

## Known limitation

STYLE001-003 read one source line at a time; a multi-line call expression is handled by scanning
from the call's line to the block's line.
