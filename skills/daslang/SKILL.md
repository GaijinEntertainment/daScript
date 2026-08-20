---
name: daslang
description: >-
  daslang (formerly daScript) language reference — gen2 syntax, type system, memory model,
  generics, macros, standard library. Invoke whenever a task involves writing, reviewing,
  refactoring, debugging, or understanding .das files or a daslang project.
---

# daslang

Statically typed scripting language for games and real-time applications, by Gaijin
Entertainment. `.das` files run interpreted, AOT-compiled to C++, or LLVM-JIT-compiled; data
layout mirrors C++.

**gen2 is the DEFAULT parser:** a `.das` file is gen2 (C-like, braces) unless it opts out with
`options gen2 = false` — never infer gen1 (indentation-based) from a missing `options gen2`
marker. Never write gen1; by convention new files still open with `options gen2`. Everything
below is gen2.

## References

Detailed semantics live in `./references/`:

- `types.md` — type catalog, vector lattice, literal suffixes, enums/variants/tuples/
  bitfields, fixed arrays, the two const positions, temporary (`#`) and distinct types
- `functions.md` — parameters, named arguments, overload resolution, operator overloading,
  computed properties, pipes, precedence
- `structs-and-classes.md` — initialization modes, virtuals, sealed/static, runtime type checks
- `closures.md` — blocks/lambdas/function pointers, capture modes, generators, iterators
- `memory.md` — the const model, move/copy/clone, finalizers, `inscope`, delete semantics,
  contexts and threading, unsafe catalog
- `generics.md` — `auto(TT)`, type contracts, typeinfo traits, `static_if`, pattern matching
- `macros.md` — compilation pipeline, macro classes, `qmacro`, `qmatch`, annotations
- `modules-and-stdlib.md` — module declarations, `require`, `with (module ...)`, options,
  containers, daslib catalog
- `strings.md` — the string surface, byte views, the parse family, `build_string`,
  conversions, regular expressions
- `files-and-paths.md` — fio helpers, directory walking, subprocess capture, glob patterns
- `json.md` — `sprint_json`/`sscan_json`, `JV`, `JsonValue?`, safe navigation
- `queries.md` — comprehensions, the linq surface, what fuses into one pass
- `cli-and-config.md` — argv parsing, environment twins, config knobs

## Functions

```das
def add(a, b : int) : int {         // `a, b : int` — one type for both names
    return a + b
}

def twice(a) => a + a               // arrow body; untyped param makes it generic
```

- Typed parameter groups separate with `,` or `;`.
- A parameter is const unless declared `var a : int`.
- Return type follows `:`, inferred when omitted; a `=>` body's expression must start on the
  same line as the `=>`.
- Named arguments follow positionals: `foo(pos, name = value)`.
- Defaults: `def greet(name : string = "world")`.
- Visibility is a prefix keyword — `def private helper()`; there is no `[private]` annotation.

## Variables and the const model

```das
let a = 13                          // immutable handle: type is `int const`
var b = 13                          // mutable: type is `int`
var c : float = 0.5
```

`let x = e` means `x : auto const = e` — const lives entirely in the type, and dereference,
indexing and field access flow it onto the result, so a `let` handle gives const access to
*everything reachable through it*. Never strip const with `reinterpret` to write: the
optimizer trusts the const type and may silently delete the write.

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

- Conditions must be `bool` — no truthiness: `if (ptr != null)`. `if` / `for` / `while` /
  `static_if` all require the parentheses.
- Postfix conditionals: `return x if (cond)`, likewise `break` / `continue`.
- A bare `{ ... }` at statement level is a lexical scope, and takes `{ ... } finally { ... }`.
- `for (k, v in keys(tab), values(tab))` iterates in parallel.

## Strings

- Strings are immutable; interpolate `{expr}` as in `"hello, {name}"` — not `${}` (compiles,
  prints a literal `$`), not `%s` — with format specifiers `"{value:08x}"`, escaping literal
  braces `"literal \{braces\}"`. Number to string: `"{x}"`
  or `string(42)`.
- `length(s)` / `empty(s)` need no `require`; the rest of the string API needs
  `require strings`.
- `int("123")` does NOT exist; `to_int` (`require strings`) silently returns `0` on garbage.
  External input goes through `try_to_int` / `try_to_float` from
  `require daslib/strings_convert`, whose Result distinguishes invalid input from a real zero.

## Pipes and block arguments

```das
foo |> setXY(10, 11)                // setXY(foo, 10, 11)

arr |> sort() $(a, b) => a < b      // trailing block pipes as last argument

build_string() $(var writer) {      // block with parameters
    writer |> write("hello")
}
```

- A block/lambda immediately after a call is piped as its LAST argument ("assumed pipe"); a
  parameterless block needs no `$`: `defer() { cleanup() }`.
- Arrow shorthand also spells lambdas: `@(x) => x + 1` (capturing), `@@(x) => x + 1`
  (no-capture function pointer).
- `a.foo(b)` is sugar for `foo(a, b)` only when `a` is a struct/class value; primitives and
  lambda-typedef values need `|>`.

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

- `table[key]` on a mutable table INSERTS a default entry when missing — read with
  `t?[key] ?? default`, test with `key_exists(t, k)`.
- Never do two `[]` lookups on the same table in one expression — rehashing can invalidate
  the first reference.
- `push` copies, `emplace` moves (zeroes the source), `push_clone` deep-clones. Bulk forms
  `push_from` / `push_clone_from` take a whole array at once.

## Move, copy, clone

| Operator | Effect |
|---|---|
| `=`  | copy — value types (`int`, `float`, `bool`, `string`, pointers, POD structs) |
| `<-` | move — transfers ownership, source is zeroed |
| `:=` | clone — deep copy, source unchanged |

```das
def make() : array<int> {
    var r <- [1, 2, 3]
    return <- r                     // moving return for non-copyable types
}
```

Structs are copyable only if all fields are. `delete` on a lambda requires `unsafe` — copies
alias one shared capture frame.

## Pointers

```das
var p = new Foo(x = 1)              // heap allocation, type Foo?
let x = p?.x ?? 0                   // safe navigation + null coalescing
var inscope q = new Foo()           // auto-deleted at scope exit
unsafe {
    delete p                        // manual delete is unsafe
}
```

- `T?` is a nullable pointer with TWO const positions: `Foo const?` = const pointee,
  `Foo? const` = const pointer. Writing through a parameter needs both non-const:
  `var p : Foo?`.
- `addr(x)` requires `unsafe`; `safe_addr(x)` yields a temporary pointer (`T?#`) without it.

## Enums, variants, tuples, bitfields

```das
enum Color {
    red
    green
}
let i = int(Color.red)              // dot access; explicit cast to int

var t = (1, 2.0, "three")
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

Without the explicit `: Flags`, `var fl = Flags.read | Flags.write` infers an ANONYMOUS
bitfield type and the named single-bit access (`fl.read`) stops compiling.

Variants (tagged unions) are declared like structs, constructed with exactly one field
(`Value(asInt = 42)`), tested with `is`, read with `as` / `?as ... ?? default`. Writing a
variant field or switching the active field requires `unsafe`.

## Memory model

Locals and structs live on the stack; `new`, arrays and tables use the current context's
heap. Cleanup is deterministic and explicit — `delete`, `var inscope`, or moving ownership
out with `<-`; a plain local `var arr : array<int>` is NOT finalized at scope exit. Each
context (thread) owns its heap and cannot retain pointers into another's — clone what
crosses. `delete` on a container of pointers finalizes AND FREES every pointee; `clear()`
first when the pointers are non-owning.

## Error handling

- `panic("msg")` is FATAL — daslang has no exception model. `try { } recover { }` captures
  the message before exiting; it does NOT recover and continue. Never design APIs around
  panic-as-control-flow.
- `assert(cond, "msg")` may be stripped in release; `verify(cond, "msg")` always runs. The
  message must be a string CONSTANT — for a runtime-value diagnostic write
  `if (!cond) panic("bad n = {n}")`. An asserted expression with side effects (invoking a
  lambda counts) is rejected; use `verify`.
- Recoverable errors are values: return `bool`, `T?`, or a variant with an error case.
- `{ } finally { }` is SKIPPED on panic, by design — no must-run-on-failure cleanup there.

## Callable types

| Kind | Prefix | Allocation | Captures | Storable | Copyable |
|---|---|---|---|---|---|
| block    | `$`  | stack | surrounding scope by reference | no  | no |
| lambda   | `@`  | heap  | explicit modes: copy/move/clone/ref | yes | yes — pointer copy (alias) |
| function | `@@` | none  | none | yes | yes |

`@@name` is a pointer to a named function.

## Gotchas

- **No implicit conversions between values.** `float_var + int_var` is a compile error;
  cast one side. Bare integer LITERALS are the one exception, adapting to a known numeric
  target — `f + 1`, `d > 1`, `var f : float = 1`, `return 200` into `uint8` — but NOT at
  call arguments (`take_f(1)` fails when `take_f` wants float) or parameter defaults, and
  float literals never adapt to double. No `bool(int)` — write `x != 0`. No `string(bool)`
  — interpolate `"{flag}"`.
- **Hex literals are `uint`** — `int(0x3F)` when an int is needed.
- **Reserved words that look like identifiers:** `where`, `shared`, `label`, `expect`,
  `pass`, `explicit`, `capture`, `deref`, `template` are keywords; `range`, `urange`,
  `range64`, `urange64`, `block`, `function`, `lambda`, `iterator`, and the small-vector type
  names (`half2`, `short4`, `byte16`, ...) are type tokens. Using any as a
  variable/parameter/field/function or annotation-argument name is a syntax error —
  including `@range = 5` on a field.
- **A statement-level expression must fit one line** unless wrapped in `(...)`: a
  continuation line starting with `+` or `-` parses as a separate unary statement and is
  silently optimized away.
- **`options stack = N` counts only in the MAIN module** — a required library cannot raise
  the program's stack.
- **`require` paths use `/`**, resolve module mounts or same-directory names, and cannot
  contain hyphens; a file elsewhere in the tree needs a relative path with the explicit
  `.das` extension: `require ../lib/util.das`.
- **`with (self) { x = 1 }` brings struct fields into scope**; `assume alias = long.chain`
  makes a textual alias (re-evaluated each use, no copy).

---
Verified against daslang 0.6.4 (2026-08-08). Maintained in the daslang repository under
`skills/daslang/`; report errors there.
