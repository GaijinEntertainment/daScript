# Modules, Options, and the Standard Library

## File layout

```das
options gen2
module physics shared public
require math
require daslib/strings_boost

struct Body { pos, vel : float3 }
let GRAVITY = float3(0.0, -9.8, 0.0)

def step(var b : Body; dt : float) {
    b.pos += b.vel * dt
}

[export]
def main { print("ok\n") }
```

The only enforced ordering is `module` before any type declaration; `options` / `module` /
`require` otherwise interleave. A file with no `module` line is a program, named by its file stem.

`[export]` makes a function callable from the host by name; `[init]` / `[finalize]` run at context
init / shutdown (no arguments, no return). `main` is a convention, not a keyword: it returns `void`
unless declared `def main() : int`, whose return value is the process exit code (do not `panic` to
force one). (probe-verified 2026-08-16)

## Module declaration

```das
module physics shared public !inscope
```

Modifiers are positional in that order; `module physics public shared` is a syntax error.

| Modifier | Meaning |
|---|---|
| `shared` | Compile once per environment and share across contexts, not once per context. |
| `public` / `private` | Default visibility of everything in the file. Omitted means the environment default (normally public). |
| `!inscope` | Visible to every module in the program, with no `require`. |

`!inscope` loads nothing by itself — something must still require the module; after that its public
symbols resolve unqualified everywhere, even in files that reach it only through someone else's
non-`public` require.

## Visibility

Visibility is a **prefix keyword**, not an annotation; there is no `[private]`.

```das
module toolkit shared private        // everything below defaults to private

let public MAX = 100
var private counter = 0
let shared private SEEN : table<string> <- { "a", "b" }

struct public Point { x, y : int }
typedef private distinct Handle = int

def public bump { counter++ }
def private helper { }
```

`shared` on a global shares the value across cloned contexts. Calling a private symbol from another
module errors `function is private to module <name>`.

## require

```das
require math                          // module name (resolved by the host/project)
require daslib/strings_boost          // module names may contain `/` and `.`
require geom                          // a bare name also finds geom.das next to this file
require ./helpers.das                 // file-relative path
require %/daslib/random.das as rng    // `%` is the daslang root; `as` binds a local qualifier
require dastest/testing_boost public  // re-export to whoever requires me
require ?pugixml pugixml/PUGIXML_boost  // load only if module `pugixml` is available
```

- **Path form needs the `.das`:** a require is a literal path only when it starts with `./`, `../`,
  or `%/` **and** ends in `.das` (or `.das_project`); anything else resolves as a module name.
- **A file's `module` name must match its file stem** (`wrong module name 'x'; did you mean 'y'?`),
  so one file required through several paths dedupes to one module.
- **Without `public`** a require is visible only inside the requiring file.
- **`as`** takes effect for the *path* forms under the default (no-project) resolver; otherwise the
  host project's resolver decides.
- **A require path is `NAME` tokens joined by `/ . % ..`** — so `require ../my-tools/x.das` parses
  as `../my` and fails, and a directory named after a keyword (`shared`, `block`, `where`, ...) is
  unreachable; use a same-directory bare require instead.
- **`?guard` skips silently** when the guard module is unavailable — no dependency, no error, even
  if the target does not exist; with the guard present a missing target errors normally. A guard
  containing `/` is satisfied when that path resolves, a plain name when that module is registered.
  Pair with `static_if (typeinfo builtin_module_exists(guard)) { ... }`.

## Qualified calls

```das
let a = sin(0.0)            // implicit: searched in every directly visible module
let b = math::sin(0.0)      // explicit
```

An implicit call matching in two modules is an ambiguity error, not a silent pick.

`_::name` resolves in the **calling** module, `__::name` in the current module only. Full table and
the generic-instancing consequences: generics.md.

### with (module ...)

`with (module physics) { ... }` opens a compile-time **resolution scope**: names inside resolve as
if written in that module — its *private* symbols included, plus everything it requires.

```das
with (module physics) {
    let s = internal_state()
    _::report(describe(s))          // describe: physics; _::report: this module
}
```

- The module has to be required — an unloaded name is `error[30298] with module 'x' is not found`.
- The enclosing module's own symbols are **not** visible inside: a bare call to one reports
  `module is not visible directly from physics`. Reach them with `_::`.
- Locals stay lexical and win over the target module's names.
- Nested forms do not combine — the innermost wins outright.
- Erased after inference: no runtime cost, no value bound.

A host can set `options with_module_is_unsafe = true` (or a `.das_project` `with_module_unsafe()`
rule) to make user-written ones require an `unsafe` wrap (`error[31037]`); inliner-generated forms
are exempt. (probe-verified 2026-08-16)

## options

```das
options rtti = true, no_aot = true      // several per line
options profiler                        // bare name == `= true`
```

Values are `bool`, `int`, or `string`. An unrecognized name is `error[50100] invalid option 'x'`,
and a wrong value type is reported too; names starting with `_` are never validated. Modules may
register their own.

**Scope:**

- **Compile-time options apply to the declaring file and nothing else.**
- **Context options (`stack`, `heap_size_hint`, `gc`, `persistent_heap`, `rtti`) are read from the
  entry program only.** A library **cannot** raise them for its users: a module declaring
  `options stack = 262144` still overflows at the 16 KB default under a program that does not
  declare it, and the failure surfaces *inside* the library.
- **`threadlock_context` alone unifies upward:** OR-ed across every module parsed, so a library
  needing the context mutex gets it regardless of the program.

| Option | Type | Default | Notes |
|---|---|---|---|
| `gen2` | bool | **true** | `options gen2 = false` opts back to legacy syntax. |
| `stack` | int | 16384 | Context stack, bytes; `0` = per-context stack. |
| `heap_size_hint` / `string_heap_size_hint` | int | 65536 | Initial heap sizes (`*_size_limit` default `0` = unlimited). |
| `gc` / `persistent_heap` | bool | false | Garbage collection; never-releasing heap (they pair). |
| `rtti` | bool | false | Extended runtime type info; needed by `daslib/rtti` and reflection. |
| `relaxed_assign` | bool | true | Silently promote `=` to `<-` for non-copyable temporaries. |
| `default_init_containers` | bool | true | New `resize` slots and fresh `tab[key]` slots get `default<T>`, not zeros. |
| `unsafe_table_lookup` | bool | false | Make every `tab[key]` require `unsafe`. |
| `force_inscope_pod` | bool | false | Scope exit / move-assign collect POD-like contents; arms container collect on `erase`/`clear`/shrink. |
| `no_unused_function_arguments`, `no_unused_block_arguments`, `no_deprecated`, `no_aliasing`, `no_init`, `no_global_variables`, `strict_properties`, `multiple_contexts`, `no_aot`, `debugger`, `profiler` | bool | false | Opt-in strictness and tooling. |
| `strict_smart_pointers`, `strict_unsafe_delete`, `no_unsafe_uninitialized_structures`, `no_writing_to_nameless`, `no_local_class_members`, `lint`, `optimize`, `fusion`, `scoped_stack_allocator`, `remove_unused_symbols` | bool | true | On by default; `no_optimization = true` inverts `optimize`, and `remove_unused_symbols` must be `false` for a module fed to an AOT tool. |
| `max_infer_passes` (50), `indenting` (0) | int | — | Inference-pass cap; indentation-sensitive parsing (`0`, `2`, `4`, `8`). |

Diagnostic dumps (`log`, `log_infer_passes`, `log_compile_time`, ...) are all bool, default false.

## Built-in modules

`builtin` is always in scope with no `require`: `print`, `assert`/`verify`, `panic`, `invoke`,
`clone`, all container operations, `length`/`empty` on strings, and `to_log(LOG_INFO, msg)` —
level-tagged output (`LOG_WARNING`, `LOG_ERROR`, …) a host can filter, unlike `print`. Everything
else is a require.

| Module | Contents |
|---|---|
| `math` | Trig, `sqrt`/`pow`/`exp`/`log`, `abs`, `min`/`max`/`clamp`/`saturate`, `lerp`, rounding, `PI`, vector and matrix math. |
| `strings` | `find`, `slice`, `replace`, `strip`, `to_upper`/`to_lower`, `starts_with`/`ends_with`, `to_int`/`to_float`, `character_at`, `peek_data`/`modify_data`, `build_string`. Byte-view twins and the cost model that makes them matter: strings.md. |
| `jobque` | Job queue, threads, channels, lock boxes, atomics. |
| `fio_core`, `rtti_core`, `ast_core`, `network_core` | Low-level C++ layers; require the wrapper instead — `daslib/fio`, `daslib/rtti`, `daslib/ast`, `daslib/network`. Bare `require rtti` / `require ast` do **not** resolve. |

Which built-in modules exist depends on how the host embedded daslang — guard anything non-core
with `require ?mod ...`.

## Container operations

Arrays and tables always pass by reference and cannot be copied — only moved (`<-`) or cloned
(`:=`).

### Array

```das
var a <- [3, 1, 2]
a |> push(4)                       // push(v, at) inserts at an index
a |> push_from(other)
a |> sort() $(x, y) => x > y
a |> erase_if() $(x) => x == 1
let sub <- a[1..3]                 // subarray
```

| Operation | Effect |
|---|---|
| `push` / `push_clone` / `emplace` | Copy in / deep-clone in / move in (zeroes the source). |
| `push_from` / `push_clone_from` | Bulk forms taking a whole `array<T>` or `T[]`; reserve once. |
| `resize` / `resize_and_init` / `resize_no_init` / `reserve` | Grow or shrink; `reserve` only changes capacity, exactly (no pow2 round-up). A resize that must GROW past `max_unreserved_size` bytes (64 MB default) without a prior reserve panics. |
| `ensure_capacity` | Geometric (at-least-doubling) reserve for open-ended appends — a following `resize` never grows, so it never trips `max_unreserved_size`. |
| `erase` / `erase_if` / `remove_value` / `pop` / `clear` | Remove; `erase(at [, count])` removes by index, `clear` keeps the capacity. |
| `length` / `long_length` / `capacity` / `empty` / `back` | Query; `back` panics (`back empty array`) when empty. |
| `find_index` / `find_index_if` / `has_value` | Search; the `find_*` pair returns `-1` when absent. |
| `sort` / `subarray` / `to_array` / `to_array_move` / `swap` | Order, convert, exchange. |

`length` returns `int` and panics past 2^31 rather than wrapping. Non-copyable element types need
`push_clone` or `emplace`, never `push`.

### Table

```das
var t <- { "one" => 1, "two" => 2 }        // table<string; int>
t |> insert("three", 3)
t |> emplace("five", five)
let safe = t?["nope"] ?? -1
let found = get(t, "three") $(v) { print("{v}\n") }
for (k, v in keys(t), values(t)) { }

let stop : table<string> <- { "the", "a", "an" }   // ONE type parameter == a set
```

| Operation | Effect |
|---|---|
| `insert` / `insert_clone` / `emplace` | Add; clone / move forms as with arrays. |
| `insert_default` / `emplace_default` | Add only when absent (`emplace_default` needs a reference value type). |
| `key_exists` / `get` / `get_value` | Test / borrow under a lock / copy out. |
| `erase` / `clear` / `length` / `empty` | Remove one, remove all, query. |
| `keys` / `values` | Iterators, usable together in one `for`. |

- **`tab[key]` inserts a default entry when the key is missing** — read with `tab?[key] ?? fallback`.
  Indexing a non-`var` table is a compile error outright. Under `default_init_containers` the
  inserted slot gets `default<V>`, but only for a *reading* index; a direct store (`tab[k] = v`,
  `<- v`, `:= v`) and `addr(tab[k])` keep the raw zeroed slot.
- **Never index the same table twice in one expression:** `tab[a] = tab[b]` is rejected
  (`table_lookup_collision`); two *different* tables are fine.

## daslib

Pure-daslang modules, required by path: `require daslib/<name>`. A `_boost` module re-exports its
base and adds macro sugar.

| Module | What it gives you |
|---|---|
| `strings_boost` | `split`, `join`, `escape`/`unescape`, `wide`. |
| `strings_convert` | `try_to_int` / `try_to_float` returning `Result<T; ConversionError>` — the safe counterpart to the silently-zeroing `to_int`. |
| `fio` | Files and paths: read/write helpers, `base_name`, `dir_name`, `path_join`, directory walks. |
| `json` / `json_boost` | JSON. `JsValue` is the variant, user code passes `JsonValue?`; `json_boost` adds struct <-> JSON conversion and `JV(...)`. |
| `toml`, `base64`, `sha_256`, `uriparser_boost`, `md_boost` | Other formats and encodings. |
| `regex` / `regex_boost` | Regex engine; `regex_boost` compiles patterns at compile time via a reader macro. |
| `linq` / `linq_boost` | Chainable queries: `where_` (trailing underscore — `where` is a keyword), `select`, `order_by`, `group_by`, `zip`, `to_array`. `linq_boost` adds the `\|>` rewrite fusing a chain into one pass. |
| `algorithm` / `sort_boost` | `unique`, `sort_unique`, `reverse`, `combine`; `partial_sort`, `nth_element`, heap ops. |
| `functional` | Lazy `filter`/`map`/`reduce`/`scan`. Legacy — prefer comprehensions or `linq`. |
| `match` | `match` / `static_match` / `multi_match` over values, enums, variants, structs. |
| `option` / `result` | Monadic `Option<T>` and `Result<T; E>`. |
| `defer` / `safe_addr` / `static_let` | LIFO scope-exit cleanup; `safe_addr(x)` giving `T?#` with no `unsafe`; locals promoted to globals. |
| `class_boost` / `interfaces` / `dynamic_cast_rtti` / `contracts` | `[class_method]` on structs; `[interface]` classes; `is_instance_of` / `as_instance_of`; `[expect_*]` argument contracts. |
| `enum_trait`, `apply`, `type_traits`, `bitfield_trait` | Compile-time reflection over enums, fields, types. |
| `decs` / `decs_boost` | Entity-component-system runtime plus the `decs_query` macro. |
| `jobque_boost` / `async_boost` / `coroutines` | `new_job`, `new_thread`, `with_job_que`; `[async]` + `await`; `[coroutine]`. |
| `ast_boost` / `templates_boost` / `ast_match` / `macro_boost` | Macro authoring: annotations, the `qmacro` reification family, `qmatch`. |
| `archive`, `clargs`, `logger`, `profiler_boost`, `debug` | Binary serialization, argument parsing, structured logging, profiling, debug/DAP server. |
| `math_boost`, `random`, `math_bits` | `AABB`/`Ray`/`Plane`, RNG, bit twiddling. |
| `lint`, `perf_lint`, `style_lint` | Extra lint passes, opt-in per file. |
| `shader_lingua_franca` | Operator rail for porting GLSL: closed `+` / `-` and ordering compares over the int16 lattice family, `half4(half2, half2)`, `unpackHalf2x16`; the operators exist only where this module is required. |
| `sql_linq` (+ `sql_boost`, `sql_migrate`) | LINQ-to-SQL against a provider module (SQLite, DuckDB, PostgreSQL): `[sql_table]` declarations, `_sql(...)` queries, transactions, migrations. |
| `pugixml/PUGIXML_boost` | XML over the `dasPUGIXML` module (not daslib): RAII parsing, builder, XPath, struct round-trip. |

## dastest

`dastest` ships with daslang distributions.

```das
require dastest/testing_boost public

[test]
def sum_works(t : T?) {
    t |> equal(2 + 2, 4)
}
```

`[test]` functions take a single `T?` asserter; `equal(a, b)`, `success(cond)` and `failure(msg)`
record results; the `dastest` driver discovers and runs them.
