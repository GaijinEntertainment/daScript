# Modules, Options, and the Standard Library

## File layout

```das
options gen2                            // compilation options
module physics shared public            // module declaration - libraries only
require math                            // imports
require daslib/strings_boost

struct Body { pos, vel : float3 }       // types
let GRAVITY = float3(0.0, -9.8, 0.0)    // globals
typedef Bodies = array<Body>            // aliases

def step(var b : Body; dt : float) {    // functions
    b.pos += b.vel * dt
}

[export]
def main { print("ok\n") }              // entry point
```

The only enforced ordering is that `module` precedes any type declaration; `options` / `module` /
`require` may otherwise interleave. A file with no `module` line is a program (entry point) and
takes its module name from the file stem — the same format serves both roles.

`[export]` marks a function the host can call by name; `[init]` runs at context init and
`[finalize]` at shutdown (both take no arguments, return nothing). `main` is a convention, not a
keyword.

## Module declaration

```das
module physics shared public !inscope
```

Modifiers are positional and must appear in this order — `module physics public shared` is a
syntax error.

| Modifier | Meaning |
|---|---|
| `shared` | Compile once per environment and share across contexts, instead of recompiling per context. |
| `public` / `private` | Default visibility of everything in the file. Omitted means the environment default (normally public). |
| `!inscope` | Visible to every module in the program, with no `require`. |

`!inscope` loads nothing by itself — something still has to pull the module in. Once loaded, its
public symbols resolve unqualified everywhere, even from files that reach it only through someone
else's non-`public` require.

Module-level `private` hides everything from direct requirers — right for a module whose value is
its side effects (macro registration), wrong for one whose functions others call.

## Visibility

Visibility is a **prefix keyword**, not an annotation. There is no `[private]`.

```das
module toolkit shared private        // everything below defaults to private

let public MAX = 100
var private counter = 0
let shared private SEEN : table<string> <- { "a", "b" }

struct public Point { x, y : int }
enum public Color { red, green, blue }
typedef public Ints = array<int>
typedef private distinct Handle = int

def public bump : int { counter++; return counter }
def private helper : int { return 0 }
```

`shared` on a global shares the value across cloned contexts. Calling a private symbol from
another module is an error naming the candidate plus `function is private to module <name>`.

## require

```das
require math                          // module name (resolved by the host/project)
require daslib/strings_boost          // module names may contain `/` and `.`
require geom                          // a bare name also finds geom.das next to this file
require ./helpers.das                 // file-relative path
require ../common/util.das            // ...with `..`
require %/daslib/random.das as rng    // `%` is the daslang root; `as` binds a local qualifier
require dastest/testing_boost public  // re-export to whoever requires me
require ?pugixml pugixml/PUGIXML_boost  // load only if module `pugixml` is available
```

- **Path form needs the `.das`.** A require is a literal path only when it starts with `./`,
  `../`, or `%/` **and** ends in `.das` (or `.das_project`); everything else resolves as a module
  name.
- **A file's `module` name must match its file stem** (`wrong module name 'x'; did you mean 'y'?`);
  a file with no `module` line takes the stem. So one file required through several paths dedupes
  to one module.
- **`public` re-exports**; without it a require is visible only inside the requiring file.
- **`as` binds a local qualifier**, to disambiguate same-named modules. Under the default
  (no-project) resolver it takes effect for the *path* forms; otherwise the host project's
  resolver decides.
- **A require path is made of plain identifiers** — `NAME` tokens joined by `/ . % ..`. So
  `require ../my-tools/x.das` parses as `../my` and fails, and a directory named after a keyword
  (`shared`, `block`, `where`, ...) is unreachable. Use a same-directory bare require instead.
- **`?guard` skips silently** when the guard module is unavailable — no dependency, no error, even
  if the target does not exist; with the guard present a missing target errors normally. A guard
  containing `/` is satisfied when that path resolves, a plain-name guard when that module is
  registered. Pair with `static_if (typeinfo builtin_module_exists(guard)) { ... }` so optional
  symbols are referenced only when present.

## Qualified calls

```das
let a = sin(0.0)            // implicit: searched in every directly visible module
let b = math::sin(0.0)      // explicit
```

An implicit call matching in two modules is an ambiguity error, not a silent pick.

`_::name` resolves in the **calling** module, so the caller's overloads are visible — that is what
makes library generics extensible (a generic is instanced into the caller's module), and why `:=`
and `delete` lower to `_::clone` and `_::finalize`. `__::name` means the current module **only**;
inside an instanced generic that is still the *caller's* module, so it cannot pin a call to the
defining module.

## options

```das
options gen2
options no_unused_block_arguments = false
options rtti = true, no_aot = true      // several per line
options profiler                        // bare name == `= true`
```

Values are `bool`, `int`, or `string`. An unrecognized name is `error[50100] invalid option 'x'`,
and a wrong value type is reported too — except that names starting with `_` are never validated,
the escape hatch for a project's own flags. Modules may register their own option names.

**Scope — the part people get wrong:**

- **Compile-time options apply to the declaring file and nothing else.**
  `options no_unused_function_arguments = true` in a library flags that library only.
- **Context options are read from the entry program only.** `stack`, `heap_size_hint`, `gc`,
  `persistent_heap`, `rtti` configure the context the host simulates, which comes from the file
  the host compiles. A library **cannot** raise them for its users: a module declaring
  `options stack = 262144` still overflows at the 16 KB default when driven by a program that
  does not declare it — and the failure surfaces *inside* the library, looking like a library bug.

| Option | Type | Default | Notes |
|---|---|---|---|
| `gen2` | bool | **true** | gen2 is the default parser; `options gen2 = false` opts back to legacy syntax. |
| `stack` | int | 16384 | Context stack, bytes; `0` = per-context stack. Entry program only. |
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
`clone`, all container operations, and `length`/`empty` on strings. Everything else is a require.

| Module | Contents |
|---|---|
| `math` | Trig, `sqrt`/`pow`/`exp`/`log`, `abs`, `min`/`max`/`clamp`/`saturate`, `lerp`, rounding, `PI`, vector and matrix math. |
| `strings` | `find`, `slice`, `replace`, `strip`, `to_upper`/`to_lower`, `starts_with`/`ends_with`, `to_int`/`to_float`, `character_at`, `peek_data`/`modify_data`, `build_string`. |
| `jobque` | Job queue, threads, channels, lock boxes, atomics. |
| `fio_core`, `rtti_core`, `ast_core`, `network_core` | Low-level C++ layers; require the wrapper instead — `daslib/fio`, `daslib/rtti`, `daslib/ast`, `daslib/network`. Bare `require rtti` / `require ast` do **not** resolve. |

Which built-in modules exist depends on how the host embedded daslang; guard anything non-core
with `require ?mod ...` plus `typeinfo builtin_module_exists`.

## Container operations

Arrays and tables always pass by reference and cannot be copied — only moved (`<-`) or cloned
(`:=`).

### Array

```das
var a <- [3, 1, 2]
a |> push(4)                       // copy in; push(v, at) inserts at an index
a |> push_from(other)              // bulk copy; reserves once
a |> sort() $(x, y) => x > y       // custom comparator
a |> erase_if() $(x) => x == 1     // erase(at [, count]) for index removal
let sub <- a[1..3]                 // subarray via a range index
```

| Operation | Effect |
|---|---|
| `push` / `push_clone` / `emplace` | Copy in / deep-clone in / move in (zeroes the source). |
| `push_from` / `push_clone_from` | Bulk forms taking a whole `array<T>` or `T[]`. |
| `resize` / `resize_and_init` / `resize_no_init` / `reserve` | Grow or shrink; `reserve` only changes capacity. |
| `erase` / `erase_if` / `remove_value` / `pop` / `clear` | Remove; `clear` keeps the capacity. |
| `length` / `long_length` / `capacity` / `empty` / `back` | Query; `back` panics (`back empty array`) when empty. |
| `find_index` / `find_index_if` / `has_value` | Search; the `find_*` pair returns `-1` when absent. |
| `sort` / `subarray` / `to_array` / `to_array_move` / `swap` | Order, convert, exchange. |

`length` returns `int` and panics past 2^31 rather than wrapping — use `long_length` for 64-bit
sizes. Non-copyable element types need `push_clone` or `emplace`, never `push`.

### Table

```das
var t <- { "one" => 1, "two" => 2 }        // table<string; int>
t |> insert("three", 3)
t |> emplace("five", five)                 // moves; `five` ends up zeroed
let safe = t?["nope"] ?? -1                // safe lookup: never inserts
let found = get(t, "three") $(v) { print("{v}\n") }
for (k, v in keys(t), values(t)) { }

var seen : table<string>                   // ONE type parameter == a set
let stop : table<string> <- { "the", "a", "an" }
var squares <- { for (x in range(5)); x => x * x }
```

| Operation | Effect |
|---|---|
| `insert` / `insert_clone` / `emplace` | Add; clone / move forms as with arrays. |
| `insert_default` / `emplace_default` | Add only when absent (`emplace_default` needs a reference value type). |
| `key_exists` / `get` / `get_value` | Test / borrow under a lock / copy out. |
| `erase` / `clear` / `length` / `empty` | Remove one, remove all, query. |
| `keys` / `values` | Iterators, usable together in one `for`. |

Two traps:

- **`tab[key]` inserts a default entry when the key is missing.** For a pure read use
  `tab?[key] ?? fallback`. Indexing a non-`var` table is a compile error outright, since the
  insert needs mutability.
- **Never index the same table twice in one expression.** `tab[a] = tab[b]` is rejected
  (`table_lookup_collision`): a rehash from one lookup invalidates the other's reference. Two
  *different* tables in one expression are fine.

## daslib

Pure-daslang modules, required by path: `require daslib/<name>`. Several follow a base + `_boost`
pattern, where `_boost` re-exports its base and adds macro sugar.

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

**Preference order for transforms:** comprehension first
(`[for (x in src); f(x); where p(x)]` and its table form), `daslib/linq` when the chain is long
enough that a comprehension stops reading well, `daslib/functional` only in code already using it.

## dastest

`dastest` ships with daslang distributions and is the test framework daslang itself uses.

```das
options gen2

require dastest/testing_boost public

[test]
def sum_works(t : T?) {
    t |> equal(2 + 2, 4)
}
```

`[test]` functions take a single `T?` asserter; `equal(a, b)`, `success(cond)` and `failure(msg)`
record results. The `dastest` driver discovers and runs them.
