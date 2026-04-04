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

Note: `get_ptr()` related patterns (null comparison, field access) are in `perf_lint` as PERF010/PERF011 since they have performance implications.

## Detection Strategies

### Source-line inspection (STYLE001-003)

The `<|` pipe and `$()` are desugared during parsing — in the compiled AST, `foo() <| $(a) { body }` and `foo() $(a) { body }` produce identical `ExprCall` nodes. The visitor uses `get_file_source_line()` to read the original source text and check for `<|` or `$` between the call and the block argument.

**Generators:** `generator<T>() <| $ { ... }` is fully lowered before the lint pass — `ExprMakeGenerator` becomes `ExprMakeStruct` + builtin `each` call. Detection uses `preVisitExprMakeStruct` with column-precise source line check: verifies the `ExprMakeStruct` at position starts with `generator<` in the source, then checks for `<|` pipe after it.

### Pure AST (STYLE005-006)

- **STYLE005:** `ExprIfThenElse` with no `if_false`, single-statement body that is `ExprReturn`/`ExprBreak`/`ExprContinue`. Skips already-postfix forms by checking `ifte.at.line != stmt.at.line`.
- **STYLE006:** `ExprOp2("==")` where one side is `string(x.__rtti)` pattern.

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

- **STYLE010 (if true)**: Cannot be detected via AST — `if (true)` is constant-folded before the visitor runs. `no_infer_time_folding` would prevent this but breaks `daslib/ast.das` compilation. Needs tree-sitter.
- **STYLE001-003 source detection**: Uses `get_file_source_line()` which reads one line at a time. Multi-line call expressions are handled by scanning from the call's line to the block's line.
- **`[lint_macro]` errors vs `expect`**: Style warnings emitted via `[lint_macro]` don't work with `expect` directives. Tests use the standalone runner instead.
