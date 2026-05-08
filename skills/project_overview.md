# daslang Project Overview

Read this skill once at the start of significant work in or with daslang — it's the why and the design lens that informs how every other skill makes its trade-offs. The full long-form rationale lives in `doc/source/reference/design_philosophy.rst`.

## What is daslang

[daslang](https://dascript.org/) (formerly daScript) is a high-performance statically-typed scripting language designed for games and real-time applications, standalone or embedded. The language has been renamed to **daslang**, but the repository (GaijinEntertainment/daScript) and many C++ API names still use the old "daScript" spelling.

## What and Why

daslang was created at Gaijin Entertainment to solve a concrete problem: **interop overhead** between scripting languages (Lua/LuaJIT, Squirrel) and C++ was killing frame budgets in their ECS game engine. The key insight is that daslang's data layout matches C++ — no marshaling, no boxing — making script↔C++ calls near-zero cost.

**Core design principles:**
- **Iteration speed is king** — full production game recompiles in ~5 sec; hot reload built in
- **Explicit, not implicit** — no hidden conversions, no silent allocations; `options log` shows exactly what the compiler produces
- **99% safe, not 100%** — eliminate real-world C++ bugs pragmatically, not at Rust-level cost; `unsafe` for the remaining 1%
- **No data marshaling** — C++-compatible data layout; this is what makes ECS-scale scripting viable
- **If it gets slow, you can fix it** — manual `delete` to reduce GC pressure, AOT compilation for native speed
- **Language reflects the domain** — macros (FORTRAN+LISP inspiration) let libraries reshape syntax to match the problem

**Three execution tiers** (all planned from day one): fast tree-based interpreter → AOT to C++ (required for consoles) → JIT via LLVM. Hybrid mode uses semantic hashing: unchanged functions stay AOT, changed ones fall back to the interpreter.

**Audience:** game scripters (largest group — hot reload, fast compile, never rewrite to C++), engine/tools programmers (zero-cost interop, macros), and a growing standalone/ecosystem community (LLVM executables, package manager in development).

## Designing with macros — the quantum advantage

Other languages have macros (Rust, C++, Lisp). What daslang combines that they don't is full AST + type access at compile time, gen2 syntax that already reads like the target domain, qmacro/quote sugar for AST construction, and zero-runtime-cost expansion. The mix makes a different class of EDSL viable. Every flagship win in this codebase falls out of that — `[sql_table]`, `_sql`, `linq_boost`, `text_match`, `[sql_index]`, the AOT codegen itself — each one collapses 20+ lines of user-side ceremony into a single declarative form *and* keeps the library implementation smaller than the hand-rolled equivalent. Macros are not a "fancy template" — they are how daslang competes.

**Use macros as a design lens.** Before writing an API or its implementation, ask: *would a macro collapse this?* If the user's call site is mechanical — derivable from a smaller declarative form — the macro is probably the answer. The win compounds: a typed surface saves boilerplate at the call site (`add_column(type<T>, .Field)` vs hand-typed `ALTER TABLE … ADD COLUMN …` strings), and the macro generates the codegen at the library layer. Two boilerplates collapsed at once.

**Don't stop at one meta level.** When a macro itself has mechanical patterns — qmatch templates, repeated AST shape walks, parallel arm-per-form blocks — write a sub-macro. `linq_boost`'s pattern-matchers, dasSQLITE's shared walker, qmacro-built ExprMakeStruct helpers — all macro-on-macro. If macro-on-macro buys an order-of-magnitude reduction in either user code or library code, do it. The compile-time cost is a few ms per expansion; the maintenance cost saved is ongoing.

For the macro mechanics themselves (qmacro/quote, gc_node patterns, AST manipulation), read `skills/das_macros.md`.
