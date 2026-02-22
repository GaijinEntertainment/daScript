# Standard Library Module Conventions

## Base + boost pattern

Many modules come in pairs: `daslib/foo.das` (core) + `daslib/foo_boost.das` (macro layer):

- **Base module** (`linq.das`, `json.das`, `regex.das`, etc.): pure functional API, runtime functions, iterator implementations
- **Boost module** (`linq_boost.das`, `json_boost.das`, etc.): macro-based sugar, compile-time optimizations, pipe-syntax rewrites
- All boost modules re-export their base module publicly (`require daslib/foo public`), so only `require daslib/foo_boost` is needed — do NOT add a separate `require daslib/foo`
- `regex.das` also has `require strings public`, so `require daslib/regex` (or `require daslib/regex_boost`) makes `slice`, `starts_with`, etc. available
- Example: `linq.das` provides `where`, `select`, `order_by` functions; `linq_boost.das` adds `_fold` macro that rewrites iterator chains into imperative loops

## Iterator implementation pattern

Many daslib functions follow this convention for iterator-based operations:

- `foo_impl` — internal generator function (yields values)
- `foo` — public function returning `iterator<T>` (calls `_impl`)
- `foo_to_array` — convenience wrapper returning `array<T>` (pipes through `to_array`)
- Inplace `foo` on arrays — overload taking `var arr : array<T>` and modifying in place

## Key daslib modules

- `daslib/linq.das` — LINQ-style queries (where, select, order_by, group_by, zip, etc.)
- `daslib/linq_boost.das` — `_fold` optimization macro, pipe-syntax macros
- `daslib/match.das` — pattern matching on variants and types
- `daslib/templates_boost.das` — template/reification infrastructure for AST macros; `apply_template` rewrites AST nodes
- `daslib/functional.das` — lazy iterator adapters and higher-order function utilities (filter, map, reduce, fold, scan, enumerate, chain, pairwise, iterate, find, find_index, partition, tap, for_each, flat_map, sorted, repeat, cycle, islice, echo, sum, any, all). Uses lambdas/functions for generator-returning functions (blocks cannot be captured into generators). Non-generator functions (reduce, fold, for_each, find, find_index, partition) also accept blocks.
- `daslib/strings_boost.das` — string manipulation helpers
- `daslib/json.das` / `daslib/json_boost.das` — JSON parsing/generation. Core: `JsValue` variant (7 types: `_object`, `_array`, `_string`, `_number`, `_longint`, `_bool`, `_null`), `JsonValue` struct wrapper, `read_json`, `write_json`, `JV()` constructors, `JVNull()`. Boost: safe access (`?.`, `?[]`, `??`), `from_JV`/`JV` generic struct↔JSON conversion, `%json~...%%` reader macro, `BetterJsonMacro` (`is`/`as` on `JsonValue?`). Settings: `set_no_trailing_zeros`, `set_no_empty_arrays`, `set_allow_duplicate_keys`. `try_fixing_broken_json` repairs LLM output. Key gotcha: `js?.value` accesses `JsonValue.value` field (returns `JsValue`), not a JSON key named "value" — use `js?["value"]` for that.
- `daslib/regex.das` / `daslib/regex_boost.das` — regular expressions. Re-exports `strings` publicly (`require strings public`), so `require daslib/regex` makes `slice`, `starts_with`, etc. available. Core: recursive-descent parser building `ReNode` AST, function-pointer-driven backtracking matcher. `Regex` struct, `regex_compile(pattern, case_insensitive=false, dot_all=false)`, `regex_match(re, str, offset=0)` → end position or -1, `regex_search(re, str, offset=0)` → `int2(start, end)` or `int2(-1,-1)` (finds first match anywhere), `regex_group(re, group_num, str)` → captured substring, `regex_group_by_name(re, name, str)` → named group substring, `re[index]` → `range` for group by int index (1-based), `re["name"]` → `range` for named group (returns `range(0,0)` if not found), `regex_foreach(re, str, block)` iterates all matches passing `range` values, `regex_replace(re, str, block)` replaces matches via block, `regex_replace(re, str, replacement)` replaces matches with template string (`$0`/`$&` for whole match, `$1`-`$9` for numbered groups, `${name}` for named groups, `$$` for literal `$`), `regex_split(re, str)` → `array<string>` of substrings between matches, `regex_match_all(re, str)` → `array<range>` of all match ranges, `is_valid(re)` checks compilation. Supports: `.` (any char except newline — use `dot_all=true` to also match `\n`), `^` (BOL), `$` (EOL), `+` `*` `?` quantifiers (greedy), `+?` `*?` `??` quantifiers (lazy), `{n}` `{n,}` `{n,m}` counted quantifiers (greedy), `{n}?` `{n,}?` `{n,m}?` counted quantifiers (lazy), `(...)` capturing groups, `(?:...)` non-capturing groups, `(?P<name>...)` named capturing groups, `(?=...)` positive lookahead, `(?!...)` negative lookahead, `|` alternation, `[abc]` `[a-z]` `[^...]` character sets, `\w` `\W` `\d` `\D` `\s` `\S` classes, `\b` `\B` word boundaries, `\t` `\n` `\r` `\f` `\v` escapes, `\xHH` hex escapes. ASCII only (256-bit CharSet). Flags: `case_insensitive=true` for case-insensitive matching (ASCII only), `dot_all=true` for dot matching newline. Boost: `%regex~pattern%%` reader macro (compile-time, no double-escaping); flags via `%regex~pattern~flags%%` where `i`=case-insensitive, `s`=dotAll. Key gotchas: `{` must be escaped as `\{` in daslang strings for counted quantifiers (`"\\d\{3}"`), but reader macro takes literal text (`%regex~\d{3}%%`). `regex_match` always matches from position 0 (or offset) — it does NOT search for the pattern; use `regex_search` for first occurrence or `regex_foreach`/`regex_match_all` to find all occurrences. Nested groups have limited support — prefer sequential groups. `-` is only special inside `[...]` character sets. Quantifiers on lookaheads are not allowed.
- `daslib/interfaces.das` — interface-based polymorphism. `[interface]` marks a class as an interface (function-only fields), `[implements(IFoo)]` generates a proxy class and `get`IFoo`` getter on the implementing struct. `InterfaceAsIs` variant macro enables `is`/`as`/`?as` operators for interface types. Supports **interface inheritance** (`class IChild : IParent`), **default method implementations** (non-abstract methods in interface classes are inherited by the proxy), and **completeness checking** (compile error if abstract methods are missing). Method naming convention: `def IFoo`bar()` on the implementing struct. Tests: `tests/interfaces/` (4 files, 67 tests).
- `daslib/flat_hash_table.das` — template-based open-addressing hash table (`TFlatHashTable`) with methods: `empty`, `length`, `clear`, `grow`, `rehash`, `reserve`, `key_index`, `key_exists`, `get`, `erase`, `foreach`, `keys`, `values`, `operator[]`, `operator?[]`
- `daslib/soa.das` — Structure-of-Arrays macro. Annotate a struct with `[soa]` to generate a parallel SOA layout (`Foo`SOA``) where every field becomes `array<FieldType>`. Provides `[]` indexing via `SOA_INDEX` proxy (rewrites `soa[i].field` → `soa.field[i]` at compile time). Generated functions: `length`, `capacity`, `push` (clone semantics, preserves source), `push_clone`, `emplace` (move semantics, destroys source), `erase`, `pop`, `clear`, `resize`, `reserve`, `swap`, `from_array`, `to_array`. Handles non-copyable fields (e.g. `array<int>`) correctly: `push`/`push_clone`/`from_array` use clone per-field, `emplace` uses move per-field. For-loop macro expands `for (it in soa)` into per-field array iteration. Tests: `tests/soa/` (4 files, 126 tests).
- `daslib/builtin.das` — core builtins like `to_array`, `to_table`

## Structure macro development patterns

When building `[structure_macro]` annotations that generate companion types and operations (like `[soa]`, `[decs]`):

### Generating functions with `qmacro_function`

The most maintainable pattern for generating typed functions is `qmacro_function` with reification:

```das
def make_length(st : StructurePtr; var stype : StructurePtr) : FunctionPtr {
    var inscope stypeT <- make_soa_type(st, stype, true)   // build TypeDeclPtr
    let firstFieldName = string(st.fields[0].name)
    var inscope fn <- qmacro_function("length") $(st : $t(stypeT)) : int {
        return length(st.$f(firstFieldName))
    }
    fn.flags |= FunctionFlags.generated
    return <- fn
}
```

Key techniques:
- **`$t(typeDeclPtr)`** in function signatures — sets parameter/return types dynamically
- **`$f(stringVar)`** for field access — `st.$f(name)` becomes `st.x` when `name="x"`
- **`$c(stringVar)`** for call names — `$c(callName)(arr, val)` calls the function named by `callName`
- **`$b(arrayOfExprPtr)`** for body splicing — build an array of expressions per-field, then inline them all

### Handling non-copyable fields

When generating per-field operations on user structs, fields may be non-copyable (`array<T>`, `table<K;V>`, lambdas):

- **Clone semantics** (preserves source): use `push_clone` per field — safe for `push`, `push_clone`, `from_array`
- **Move semantics** (consumes source): use `emplace` per field — for `emplace` where source destruction is expected
- **Clone-assign**: use `:=` instead of `=` for per-field assignment — `to_array` needs this
- **Test non-copyable fields explicitly**: always include a test struct with at least one `array<T>` field to catch copy errors

### Registration: `add_function` vs `add_generic`

- **`add_function`** — registers a concrete, fully-typed function. Used when all types are known at macro expansion time.
- **`add_generic`** — registers a generic function template that gets instanced at each call site. Use when the function needs to participate in overload resolution or when type inference is needed (e.g., `push` may need to resolve to the correct clone/copy overload depending on field types).
