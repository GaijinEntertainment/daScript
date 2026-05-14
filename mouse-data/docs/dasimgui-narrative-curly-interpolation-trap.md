---
slug: dasimgui-narrative-curly-interpolation-trap
title: Why does `{value}` in a dasImgui text widget try to look up a variable?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**daslang strings interpolate `{expr}` at compile time.** Any `{...}` inside a string literal is treated as an embedded expression to evaluate and concatenate. The compiler lowers `"foo {x}"` to `string_builder("foo ", x)` (an `ExprOp2`), not `ExprConstString`.

This isn't a dasImgui rule — it's daslang itself. But it hits hardest in narrative widgets where you want to show literal curly braces or describe a placeholder value:

    // ✗ tries to look up a variable named `value` — compile error.
    text("payload: {value}")

    // ✓ rephrase to avoid `{ident}` shape.
    text("payload value")

    // ✓ to print a literal `{`, escape with double-braces.
    text("payload: {{value}}")        // renders as: payload: {value}

## Where this bites

- **Showcase feature demos** writing narrative explanations like `"text() — one line, telemetry-visible."`. Anything `{IDENT}` (matching daslang identifier shape) is hot.
- **DRIVE: ... curl -X POST -d '{"name":...}'` comments inside `//!` blocks** — those embed JSON which has its own braces. They sit inside `//` comments so the parser ignores them; but if you accidentally put JSON-shaped literal inside a `text("...")`, it errors.

## Side effect on positional-string form (commit 19)

`text("foo {x}")` parses to `string_builder("foo ", x)`, not `ExprConstString`. The positional-string sugar in the `[widget]` macro only matches bare `ExprConstString` literals. So interpolated text can't use positional form — must use named-tuple no-ident:

    text((text = "value = {x}"))     // ✓
    text("value = {x}")              // ✗ "first argument must be identifier, named-tuple, ...; got string_builder(...)"

## Side effect on macro / qmacro debugging

`qmacro` accepts `das_string` directly via `$i(name)` / `$v(value)`. If you generate a string with embedded `{...}` and pass it through a string-formatted error message (e.g. `macro_error(prog, expr.at, "got {expr_dump}")`), the inner `{...}` re-interpolates. Use `\{` to escape or build the string out-of-band.

Bit Boris repeatedly during the v2 boost mega-detour PR — narrative showcase demos kept including `{value}` shapes in their descriptive text.

## Questions
- Why does `{value}` in a dasImgui text widget try to look up a variable?
- Why does `text("foo {x}")` fail to compile as positional form?
- How do I show literal curly braces in a daslang string?
- What does daslang's string interpolation lower to?
