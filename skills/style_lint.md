# Style Lint Rules (`daslib/style_lint.das`)

## Overview

The `style_lint` module detects non-idiomatic patterns in daslang code at compile time. It uses a `[lint_macro]` AST pass that walks the compiled program looking for known style issues and reports them as `CompilationError::style_lint` (error code 40218).

## Architecture

- **Module:** `daslib/style_lint.das` — `module style_lint shared private`
- **Entry point:** `[lint_macro] class StyleLintMacro : AstPassMacro` calls `style_lint(prog, true)`
- **Visitor:** `class StyleLintVisitor : AstVisitor` — walks the AST with source-line inspection
- **Error reporting:** `macro_style_warning(compiling_program(), at, message)` — reports as error code 40218
- **Utility:** `utils/lint/main.das` — unified lint checker (all 3 passes: paranoid, perf, style)
- **Tests:** `utils/lint/tests/` — one file per rule

## Rules

| ID | Pattern | Suggestion |
|---|---|---|
| STYLE001 | `foo() <| $(a) { ... }` | Remove `<|` pipe; use `foo() $(a) { ... }` |
| STYLE002 | `foo() <| $() { ... }` | Remove pipe and `$()`; use `foo() { ... }` |
| STYLE003 | `foo() $() { ... }` | Remove redundant `$()`; use `foo() { ... }` |
| STYLE005 | `if (cond) { return val }` | Use `return val if (cond)` (configurable, off by default) |
| STYLE006 | `string(x.__rtti) == "ExprFoo"` | Use `x is ExprFoo` |
| STYLE010 | `if (true) { ... }` | Use a bare block `{ ... }` |
| STYLE011 | `var x : int; x = 5` | Combine into `var x = 5` (or `:=` / `<-`) |
| STYLE012 | `var a : array<T>; a \|> push(x); a \|> emplace(<-y)` (≥ 2 contiguous `push`/`emplace`) | Use array literal `var a <- [x, <-y]`, or typed constructor `var a <- array<T>(x, y)` when an explicit element type is needed (polymorphic upcasts, interface pointers). `push_clone` excluded; single `push` stays silent — often more readable |
| STYLE013 | `var a = Foo(); a.x = 1; a.y = 2` (or `var a : Foo` for `[safe_when_uninitialized]` structs), ≥ 2 contiguous field assignments | Use a named-argument constructor: `var a = Foo(x = 1, y = 2)`. Skipped when init is non-empty (`Foo(x=1)` then `a.y = 2` stays silent), when assignments are not contiguous, or for `inscope`/generated/generic-instantiation vars |
| STYLE014 | `//` or `//!` comment block of more than 3 contiguous lines at module/public scope | Trim to a 1-line WHY (move design notes to a `.md` doc, not source). Module-leading docstring (block before any AST decl in the file) is always allowed. Suppress per-block with `//!@nolint` on first line of a `//!` block (also stripped from generated RST), or `// nolint:STYLE014` on first line of a `//` block |
| STYLE015 | `//` or `//!` comment block of more than 1 contiguous line inside a `def private` | Private symbols don't surface in any doc generator, so multi-line prose there is dead weight. Trim to one line, or suppress on first line with `// nolint:STYLE015` |

Note: `get_ptr()` related patterns (null comparison, field access) are in `perf_lint` as PERF010/PERF011 since they have performance implications.

## Detection Strategies

### Source-line inspection (STYLE001-003)

The `<|` pipe and `$()` are desugared during parsing — in the compiled AST, `foo() <| $(a) { body }` and `foo() $(a) { body }` produce identical `ExprCall` nodes. The visitor uses `get_file_source_line()` to read the original source text and check for `<|` or `$` between the call and the block argument.

**Generators:** `generator<T>() <| $ { ... }` is fully lowered before the lint pass — `ExprMakeGenerator` becomes `ExprMakeStruct` + builtin `each` call. Detection uses `preVisitExprMakeStruct` with column-precise source line check: verifies the `ExprMakeStruct` at position starts with `generator<` in the source, then checks for `<|` pipe after it.

**`defer` special case:** The `defer` macro erases its call node before the lint pass runs, so `preVisitExprCall` never sees `defer <| $() { ... }`. Instead, `preVisitFunction` invokes `check_defer_pipe`, which scans each source line of the function body for `defer` followed by `<|` and a block opener (`$(`, `$ `, `${`, `@`, `{`). This is why STYLE001/002 on `defer` fire from a different code path than on ordinary calls.

### Pure AST (STYLE005-006, STYLE010-011)

- **STYLE005:** `ExprIfThenElse` with no `if_false`, single-statement body that is `ExprReturn`/`ExprBreak`/`ExprContinue`. Skips already-postfix forms by checking `ifte.at.line != stmt.at.line`.
- **STYLE006:** `ExprOp2("==")` where one side is `string(x.__rtti)` pattern.
- **STYLE010:** `ExprIfThenElse` with `cond is ExprConstBool && cond.value && if_false == null`. Runs in the same `preVisitExprIfThenElse` override as STYLE005; `if_flags.isStatic` branches are skipped so `static_if (true)` doesn't trigger. Works because the lint visitor runs before constant-folding would collapse the `if`.
- **STYLE011:** Tracks uninitialized variables from `preVisitExprLetVariable` (excluding `generated` and `inScope`). In `preVisitExprBlockExpression`, checks if the next statement is `ExprCopy`/`ExprClone`/`ExprMove` whose left side references a tracked variable.
- **STYLE012:** In `preVisitExprBlockExpression`, when `expr is ExprLet`, locates the let's index inside `blk.list` (pointer equality via `smart_ptr ==`). For each variable `v` in the let that is `var a : array<T>` with no init (also excluding `generated`, `inScope`, and generic-host instantiations), walks forward through `blk.list` counting contiguous `ExprCall` statements where `call.func.fromGeneric.name` is `"push"` or `"emplace"` and `arguments[0]` (unwrapping `ExprRef2Value`) resolves to `v`. Stops at the first non-matching statement. Emits when count ≥ 2 — a single `push` is often the more readable form, so the rule targets "I forgot how to init an array" bugs rather than every possible rewrite. `push_clone` is deliberately excluded because there is no clean array-literal equivalent.
- **STYLE013:** Same dispatch as STYLE012 (from `preVisitExprBlockExpression` when `expr is ExprLet`). Filters to vars with `_type.baseType == Type.tStructure` and `length(_type.dim) == 0`, excluding `generated`/`inScope`/generic instantiations. `is_default_struct_init` accepts the let if `init == null` or `init is ExprMakeStruct` with no `_block` and no `MakeFieldDecl`s in any row (the zero-arg `Foo()`). Walks forward through `blk.list` counting contiguous `ExprCopy`/`ExprClone`/`ExprMove` statements whose LHS is checked directly as `ExprField(value=ExprVar(v))` — assignment targets are lvalues, so no `ExprRef2Value` unwrap is needed (cf. `find_assign_var` / `is_push_or_emplace_of` which handle RHS / argument positions where the wrapper *can* appear). Emits one warning per variable when count ≥ 2.
- **STYLE014/STYLE015:** Post-visit pass — `scan_long_comment_blocks(prog)` runs after the visitor finishes. Walks `prog.getThisModule` only (transitively-required modules are skipped). Per native function/structure (filters: `_module == mod`, not `generated`, not `fromGeneric`, not `moreFlags.isTemplate` for fns / not `flags.isTemplate` for structs, not a name with `<` — the angle-bracket heuristic catches template-instantiated structs whose `at` points to the template's source file). Records `(file, start, end, is_private)` per function and the per-file `first_decl_line` / `max_line`. Then for each unique file, walks source line-by-line via `get_file_source_line`, accumulates contiguous `//` / `//!` lines into a block, and on block end checks: (a) skip if block starts before `first_decl_line` (module-leading docstring), (b) skip if first line contains `@nolint`, (c) find smallest enclosing function range — if private and block size > 1, emit STYLE015; otherwise if size > 3, emit STYLE014. The existing `// nolint:STYLE014` / `// nolint:STYLE015` per-line mechanism (matched by `is_suppressed`) catches the `//` form on the first line of the block; the `//!@nolint` form is matched directly via `find(first_line_text, "@nolint") >= 0` because the `is_suppressed` machinery looks for `:STYLEnnn` after `nolint`.

## How to Add a New Rule

### 1. Choose a rule ID

Rules are numbered: `STYLE001`, `STYLE002`, etc. Pick the next available number.

### 2. Add detection logic to `StyleLintVisitor`

Override the appropriate visitor method(s):

| What you're looking for | Override method |
|---|---|
| Function call patterns | `preVisitExprCall` |
| Binary operator patterns | `preVisitExprOp2` |
| Field access patterns | `preVisitExprField` |
| If/else patterns | `preVisitExprIfThenElse` |
| Source-level syntax | Use `get_file_source_line()` or `source_line_between()` |

### 3. Report the warning

```das
self->style_warning("STYLExxx: description; suggested fix", expr.at)
```

### 4. Write the test file

Create `utils/lint/tests/styleXXX_rule_name.das` with a bad example and a good example. Test through the standalone runner:

```bash
bin/Release/daslang.exe utils/lint/main.das -- utils/lint/tests/styleXXX_rule_name.das --style-only
```

### 5. Suppression

Individual warnings can be suppressed with `// nolint:STYLExxx` on the same line.

## Public API

```das
// Compile-time mode: reports warnings during compilation
def public style_lint(prog : ProgramPtr; compile_time_errors : bool; postfix_conditionals : bool = false) : int

// Collection mode: appends warnings to array
def public style_lint_collect(prog : ProgramPtr; var warnings : array<string>; postfix_conditionals : bool = false) : int
```

## MCP Integration

The MCP `lint` tool (`utils/mcp/tools/lint_tool.das`) calls `style_lint_collect()` alongside `paranoid_collect()` and `perf_lint_collect()`. Style warnings appear in lint results automatically.

## Standalone Usage

```bash
bin/Release/daslang.exe utils/lint/main.das -- file1.das [file2.das ...] [--quiet] [--style-only] [--postfix-conditionals]
```

## Known Limitations

- **STYLE001-003 source detection**: Uses `get_file_source_line()` which reads one line at a time. Multi-line call expressions are handled by scanning from the call's line to the block's line.
- **`[lint_macro]` errors vs `expect`**: Style warnings emitted via `[lint_macro]` don't work with `expect` directives. Tests use the standalone runner instead.
