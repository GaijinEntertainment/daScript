---
name: daslang
description: >-
  daslang (formerly daScript) language reference — gen2 syntax, type system, memory model,
  generics, macros, standard library. Invoke whenever a task involves writing, reviewing,
  refactoring, debugging, or understanding .das files or a daslang project.
---

# daslang

High-performance, statically typed scripting language for games and real-time applications,
by Gaijin Entertainment. Programs run interpreted, AOT-compiled to C++, or LLVM-JIT-compiled;
data layout mirrors C++ for zero-cost interop. The design is explicit over implicit: no
implicit conversions, no truthiness, explicit move/clone semantics.

File extension: `.das`.

**Syntax generations:** gen2 (C-like, braces) is the DEFAULT parser — every `.das` file
parses as gen2 unless it explicitly opts out with `options gen2 = false` (legacy gen1 is
indentation-based; never write it, and never infer gen1 from a missing `options gen2`
marker). Convention: new files still open with `options gen2`. Everything below is gen2.

## References

Full catalogs and detailed semantics live in `./references/`:

- **Types and literals** — `./references/types.md`: complete type catalog, 16/8-bit
  vector lattice, literal suffixes, enums/variants/tuples/bitfields, structural fixed
  arrays, pointers and the two const positions, temporary (`#`) types, distinct types
- **Functions and operators** — `./references/functions.md`: parameter/declaration rules,
  named arguments, overload resolution, operator overloading catalog, computed properties,
  pipes, precedence
- **Structs and classes** — `./references/structs-and-classes.md`: initialization modes,
  inheritance, virtuals, sealed/static, runtime type checks
- **Closures** — `./references/closures.md`: block/lambda/function-pointer semantics,
  capture modes, generators, iterators
- **Memory and lifetime** — `./references/memory.md`: the const model, move/copy/clone
  rules, finalizers, `inscope`, delete semantics, contexts and threading, unsafe catalog
- **Generic programming** — `./references/generics.md`: `auto(TT)` patterns, type
  contracts, typeinfo traits, `static_if`, module resolution in generics, pattern matching
- **Macros and annotations** — `./references/macros.md`: compilation pipeline, macro
  classes, reification (`qmacro`), AST matching (`qmatch`), annotation catalog
- **Modules and standard library** — `./references/modules-and-stdlib.md`: module
  declarations, `require` rules, options reference, container operations, daslib catalog

## Functions

```das
def add(a, b : int) : int {         // `a, b : int` — one type for both names
    return a + b
}

def madd(a : int, b : float) : float {   // `,` or `;` both separate typed groups
    return float(a) + b
}

def twice(a) => a + a               // arrow body; untyped param makes it generic
```

- Parameters are grouped: `a, b : int` declares two ints. Groups are separated by `,` or
  `;` — both are valid; house style uses `;` between groups of different types.
- A parameter without `var` is const (see The const model below). `def f(var a : int)`
  makes `a` mutable inside the body.
- Return type after `:`; inferred when omitted. Single-expression bodies use `=>` — the
  expression must start on the same line as `=>`.
- Named arguments: `foo(pos, name = value)` — bare named args after positionals. Works on
  method calls too: `obj.m(name = v)`.
- Default parameter values: `def greet(name : string = "world")`.
- Visibility is a prefix keyword: `def private helper()` — there is no `[private]`
  annotation.

## Variables and the const model

```das
let a = 13                          // immutable handle: type is `int const`
var b = 13                          // mutable: type is `int`
var c : float = 0.5                 // explicit type
```

`const` lives entirely in the type. `let x = e` means `x : auto const = e`; `var` means
only "do not append the trailing const". Dereference, indexing, and field access flow the
handle's const onto the result — a `let` handle gives const access to *everything reachable
through it* (unlike C++, where a const pointer still yields a mutable pointee). To write
through a pointer parameter, declare it `var p : T?`. Never strip const with `reinterpret`
to write — the optimizer trusts the const type and may silently delete the write. Full
story: `./references/memory.md`.

## Control flow

```das
if (x > 0) {
    r = 1
} elif (x < 0) {
    r = -1
} else {
    r = 0
}

for (i in range(10)) { ... }        // 0..9
for (x in arr) { ... }              // arrays, tables, iterators
while (running) { ... }
```

- Conditions must be `bool` — no truthiness: `if (ptr != null)`, `if (n != 0)`.
- Parentheses are required on `if` / `for` / `while` / `static_if`.
- Postfix conditionals for early exit: `return x if (cond)`, `break if (cond)`,
  `continue if (cond)`. Prefer `if (cond) return x` over `if (cond) { return x }`.
- A bare `{ ... }` at statement level is a lexical scope (supports
  `{ ... } finally { ... }` — the `finally` runs on normal exit, NOT on panic).
- `for (k, v in keys(tab), values(tab))` iterates in parallel.

## Strings

```das
let name = "world"
let s = "hello, {name}"             // interpolation with {}
let t = "literal \{braces\}"        // escape literal braces
```

- Strings are immutable; interpolation is `{expr}` — not `${}`, not `%s`.
- `length(s)` and `empty(s)` work without any `require`; the rest of the string API needs
  `require strings`.
- `int("123")` does NOT exist. `to_int("123")` (from `require strings`) parses but silently
  returns `0` on garbage. For anything that validates external input use `try_to_int` /
  `try_to_float` from `require daslib/strings_convert` — they return a Result that
  distinguishes invalid input from a real zero.
- Number to string: interpolation (`"{x}"`) or `string(42)`. Format specifiers:
  `"{value:08x}"`.

## Pipes and block arguments

```das
foo |> setXY(10, 11)                // setXY(foo, 10, 11)

arr |> sort() $(a, b) => a < b      // trailing block pipes as last argument

build_string() $(var writer) {      // block with parameters
    writer |> write("hello")
}

defer() {                           // parameterless block needs no $
    cleanup()
}
```

- `x |> f(y)` passes `x` as the first argument. A block/lambda written immediately after a
  call is piped as the LAST argument ("assumed pipe").
- Arrow shorthand for one-expression blocks/lambdas: `$(a, b) => a < b`,
  `@(x) => x + 1` (capturing lambda), `@@(x) => x + 1` (no-capture function pointer).
- Dot calls are sugar for the same thing — `a.foo(b)` is `foo(a, b)` — but only when `a`
  is a struct/class value. Primitives and lambda-typedef values need `|>`.

## Collections

```das
var a <- [1, 2, 3]                          // array<int> literal (heap)
let f = fixed_array(1, 2, 3)                // fixed-size int[3] (stack)
var t <- { "hp" => 100, "mp" => 50 }        // table<string; int> literal
var s : table<string>                       // one type param = a SET
let v = t?["hp"] ?? 0                       // safe lookup with default
var sq <- [for (x in range(10)); x * x]     // array comprehension
var m <- { for (x in range(5)); x => x * x } // table comprehension
```

- `table[key]` on a mutable table INSERTS a default entry when missing — use
  `t?[key] ?? default` to read without inserting, `key_exists(t, k)` to test.
- Never do two `[]` lookups on the same table in one expression — rehashing can invalidate
  the first reference.
- `push` copies, `emplace` moves (zeroes the source), `push_clone` deep-clones. Bulk forms
  `push_from` / `push_clone_from` copy a whole array at once.
- Arrays and tables cannot be copied with `=` — move with `<-` or clone with `:=` (next
  section).

## Move, copy, clone

| Operator | Effect |
|---|---|
| `=`  | copy — value types (`int`, `float`, `bool`, `string`, pointers, POD structs) |
| `<-` | move — transfers ownership, source is zeroed |
| `:=` | clone — deep copy, source unchanged |

```das
var a <- [1, 2, 3]
var b <- a                          // move: a is now empty
var c := b                          // clone: independent copy

def make() : array<int> {
    var r <- [1, 2, 3]
    return <- r                     // moving return for non-copyable types
}
```

Structs are copyable only if all fields are. Lambdas ARE copyable — copying copies the
pointer to the shared capture frame (an alias), which is why `delete` on a lambda requires
`unsafe`. Details and the full compatibility table: `./references/memory.md`.

## Pointers

```das
var p = new Foo(x = 1)              // heap allocation, type Foo?
let x = p?.x ?? 0                   // safe navigation + null coalescing
var inscope q = new Foo()           // auto-deleted at scope exit
unsafe {
    delete p                        // manual delete is unsafe
}
```

- `T?` is a nullable pointer; `new` allocates on the current context's heap.
- A pointer type has TWO const positions: `Foo const?` = const pointee, `Foo? const` =
  const pointer. Writing through a parameter needs both non-const: `var p : Foo?`.
- `addr(x)` takes an address (requires `unsafe`); `safe_addr(x)` yields a temporary
  pointer (`T?#`) without unsafe.

## Enums, variants, tuples, bitfields

```das
enum Color {
    red
    green
    blue
}
let c = Color.red                   // dot access
let i = int(c)                      // explicit cast to int

var t = (1, 2.0, "three")           // tuple
let first = t._0                    // access by index
let (x, y, z) = t                   // destructuring

bitfield Flags {
    read
    write
}
var fl : Flags = Flags.read | Flags.write
fl.write = false                    // single-bit assignment
if (fl.read) { ... }                // single-bit test
```

Note the explicit `: Flags` on the bitfield variable — `var fl = Flags.read | Flags.write`
infers an ANONYMOUS bitfield type and the named single-bit access (`fl.read`) stops
compiling. Spell the type when you want the field sugar.

Variants (tagged unions) are declared like structs, constructed with exactly one field
(`Value(asInt = 42)`), tested with `is`, read with `as` / `?as ... ?? default`. Writing a
variant field or switching the active field requires `unsafe`. Full detail:
`./references/types.md`.

## Memory model in one paragraph

Locals and structs live on the stack; `new`, arrays, and tables use the current context's
heap. Cleanup is deterministic: `delete` explicitly, `var inscope` to delete at scope exit,
or move ownership out with `<-`. A plain local `var arr : array<int>` is NOT finalized at
scope exit — leaks in per-frame code usually trace to exactly that. Each execution context
owns its heap; contexts (threads) cannot retain pointers into each other's heaps — clone
what crosses. `delete` on a container of pointers finalizes AND FREES every pointee — for
non-owning pointers `clear()` first. Details: `./references/memory.md`.

## Error handling

- `panic("msg")` is FATAL — daslang has no exception model. `try { } recover { }` exists to
  capture the message (log, report) before exiting, NOT to recover and continue. Never
  design APIs around panic-as-control-flow.
- `assert(cond, "msg")` may be stripped in release; `verify(cond, "msg")` always runs. The
  message must be a string CONSTANT — for a runtime-value diagnostic write
  `if (!cond) panic("bad n = {n}")`. The asserted expression must be side-effect-free
  (invoking a lambda counts as a side effect) — the compiler rejects it; use `verify`.
- Recoverable errors are values: return `bool`, `T?`, a variant with an error case, or a
  `Result`-style type.
- `{ } finally { }` cleanup is SKIPPED on panic, by design — do not put must-run-on-failure
  cleanup there.

## Callable types

| Kind | Prefix | Allocation | Captures | Storable | Copyable |
|---|---|---|---|---|---|
| block    | `$`  | stack | surrounding scope by reference | no  | no |
| lambda   | `@`  | heap  | explicit modes: copy/move/clone/ref | yes | yes — pointer copy (alias) |
| function | `@@` | none  | none | yes | yes |

Blocks are for inline callbacks that never escape; lambdas for stored/returned closures;
`@@name` for a pointer to a named function. Capture modes, generators (`yield`), and
iterator protocols: `./references/closures.md`.

## Gotchas (highest-frequency mistakes)

- **No implicit conversions between values.** `float_var + int_var` is a compile error;
  cast one side. The one exception is bare integer LITERALS, which adapt to a known numeric
  target — `f + 1`, `d > 1`, `var f : float = 1`, `return 200` into `uint8` — but NOT at
  call arguments (`take_f(1)` fails when `take_f` wants float) or parameter defaults, and
  float literals never adapt to double. No `bool(int)` — write `x != 0`. No `string(bool)`
  — interpolate `"{flag}"`.
- **Hex literals are `uint`** — `int(0x3F)` when an int is needed.
- **Reserved words that look like identifiers:** `where`, `shared`, `label`, `expect`,
  `pass`, `explicit`, `capture`, `deref`, `template` are keywords; `range`, `urange`,
  `block`, `function`, `lambda`, `iterator`, and the small-vector type names (`half2`,
  `short4`, `byte16`, ...) are type tokens. Using any as a variable/parameter/field/function
  name is a syntax error — rename (`rng`, `body`, `fn`, `grab`, `unref`, `pattern`).
- **A statement-level expression must fit one line** unless wrapped in `(...)`. DANGER: a
  continuation line starting with `+` or `-` parses as a separate unary statement and is
  silently optimized away — wrap multi-line arithmetic in parentheses.
- **`options stack = N` counts only in the MAIN module** — a required library cannot raise
  the program's stack; the top-level program file must declare it.
- **`require` paths use `/`**, resolve module mounts or same-directory names, and cannot
  contain hyphens; a file elsewhere in the tree needs a relative path with the explicit
  `.das` extension: `require ../lib/util.das`.
- **`with (self) { x = 1 }` brings struct fields into scope**; `assume alias = long.chain`
  makes a textual alias (re-evaluated each use, no copy).

## Applying this skill

When writing daslang: apply the conventions silently — gen2 forms, explicit moves/clones,
pipes and trailing blocks for callbacks, comprehensions over build-loops, `let` unless
mutation is needed. When reviewing: cite the specific rule and show the fix inline; the
highest-value checks are implicit-conversion attempts, `=` on non-copyable types, missing
`return <-`, table double-lookups, panic-as-control-flow, and const-stripping writes.

---
Verified against daslang 0.6.4 (2026-08-08). Maintained in the daslang repository under
`skills/daslang/`; report errors there.
