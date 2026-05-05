# Strings in daslang

Read this skill before writing or editing `.das` code that searches, splits, replaces, parses, formats, or otherwise manipulates strings — anywhere you'd reach for `find`, `replace`, `split`, `to_int`, `build_string`, or `peek_data` / `modify_data`.

## Modules

```das
require strings                  // core: most ops live here
require daslib/strings_boost     // extras: split, contains, count, join, levenshtein, pad, replace_multiple
require daslib/strings_convert   // safe parsing: try_to_int / try_to_float (Result-returning)
```

`strings` is the always-on built-in — `find`, `replace`, `slice`, `length`, `to_int`, `build_string`, `format`, `peek_data`/`modify_data`, the `is_*` char predicates, etc. `strings_boost` is the daslang-side companion (multi-result `split`, `contains`, `count`, generic `join`, `pad_left`/`pad_right`, `levenshtein_distance`, `replace_multiple`, `glob_match`, `is_null_or_whitespace`). `strings_convert` is the validating-parse layer.

## Don't iterate bytes when a built-in does it

The single most common antipattern in this codebase is opening `peek_data` and writing a `for (i in range(length(arr)))` loop to do something that `find` / `replace` / `starts_with` / `split` already does. Reach for byte-level access **only** when the operation is genuinely not expressible as a built-in (state machines, custom parsers, multi-byte transforms with cross-byte context).

| If you want to | Use | Don't |
|---|---|---|
| Find a char or substring | `find(s, '*')` / `find(s, "foo")` (returns -1 on miss) | byte loop with `c == uint8('*')` |
| Find from the right | `rfind(s, '/')` / `rfind(s, "ab")` | reverse byte loop |
| "Does it contain X" | `contains(s, "foo")` (boost) or `find(s, "foo") >= 0` | byte loop |
| "Does it start/end with X" | `starts_with(s, "foo")` / `ends_with(s, ".das")` | `slice(s, 0, n) == "foo"` |
| Replace all `a` with `b` | `replace(s, "a", "b")` | `modify_data` byte loop |
| Replace many pairs in one pass | `replace_multiple(s, [("a","x"), ("b","y")])` (boost) | nested `replace` calls |
| "Is every char X" | `empty(replace(s, "X", ""))` | byte loop accumulating a flag |
| Count occurrences | `count(s, "foo")` (boost) | byte loop |
| Split on a separator | `split(s, ",")` (boost) → `array<string>` | manual `find` + `slice` loop |
| Split on any of several chars | `split_by_chars(s, " \t\n")` (boost) | byte loop with branching |
| Trim whitespace | `trim(s)` / `strip(s)`, or `ltrim`/`rtrim`/`strip_left`/`strip_right` | manual leading/trailing scan |
| Strip a known prefix/suffix | `trim_prefix(s, "foo/")` / `trim_suffix(s, ".das")` (boost) | `starts_with` + `slice` |
| Uppercase / lowercase | `to_upper(s)` / `to_lower(s)` (allocate); `_in_place` variants mutate in place | manual case-fold loop |
| Pad to width | `pad_left(s, w, ' ')` / `pad_right(s, w, ' ')` (boost) | manual `repeat` + concat |
| Repeat a unit | `repeat(unit, n)` | concat loop |
| Reverse | `reverse(s)` | byte loop |
| Case-insensitive compare | `compare_ignore_case(a, b) == 0` | `to_lower(a) == to_lower(b)` (allocates twice) |

The refactor that motivated this skill (commits in fio.das around `is_glob_pattern`, `to_generic_path`, the empty-path branch of `match_glob`) collapsed three byte-level loops into one-liners using `find`, `replace`, and `empty(replace(s, "*", ""))`. If you're staring at a `peek_data` block that only reads bytes, ask "does `find` or `replace` already do this?" first.

## Parsing numbers — validate at boundaries

Two families of parse functions, picked by what the caller does on garbage input:

```das
require strings
require daslib/strings_convert

let n = to_int(s)              // silent — returns 0 for "foo", 12 for "12abc"
let r = try_to_int(s)          // Result<int; ConversionError>
if (r is value) {
    use(r as value)
} else {
    bail("bad input: {s} ({r as error})")
}
```

- **`to_int(s)` / `to_float(s)`** silently return `0` / `0.0` for unparseable input and partial-parse for `"12abc"`. Fine for trusted internal data; **never** for user input, env vars, file contents, command-line args, or anything that flows into a shell call, file path, or system call. `";rm -rf;"` parses as `0` with `to_int`.
- **`try_to_int` / `try_to_float`** in `daslib/strings_convert` return a `Result<T; ConversionError>` distinguishing `invalid_argument` (no digits), `out_of_range` (overflow), and `trailing_garbage` (`"12abc"`). The whole `try_to_*` family covers `int8`/`uint8`/.../`int64`/`uint64`/`float`/`double`.
- The 3-arg `int(s, var conv_result, var consumed, accept_hex)` builtins also surface a `ConversionResult` enum if you want the boundary check without pulling in `strings_convert`.

When in doubt: **use `try_to_*` at the input boundary, plain `to_*` once the value is known to be clean.**

## Building strings

For multi-piece output, prefer `build_string` over chain concatenation. The writer does one allocation pass; concat in a loop reallocates on every append.

```das
let result = build_string() $(var writer) {
    writer |> write("prefix=")
    writer |> write(name)
    for (x in items) {
        writer |> write(", ")
        writer |> write(x)
    }
}
```

- `build_string() $(var writer) { ... }` — returns the assembled string. The block receives a `StringBuilderWriter`.
- `build_hash() $(var writer) { ... }` — same pattern, returns `uint64` hash of what would have been written. Useful for cache keys without allocating the string.
- `writer |> write(any)` — generic write; calls the appropriate per-type formatter.
- `writer |> write_char(c)` — single int-as-char.
- `writer |> write_chars(c, n)` — repeat one char n times.
- `writer |> write_escape_string(s)` — write with escape sequences applied (use for emitting JSON-like literals).
- `writer |> fmt(spec, val)` / `writer |> format(spec, val)` — printf-style formatting for one numeric value. `fmt` takes the per-type overloads (int8…double); `format` is the int/uint/int64/uint64/float/double set.
- **String interpolation** `"{x} {y}"` is sugar for `build_string` — fine for one-off small constructions, but inside a loop or for long pipelines, write the explicit `build_string` so the writer is reused.

`perf_lint` flags some bad patterns here (PERF002 string concat in loops, PERF005 unnecessary `string(das_string)` casts) — see `skills/perf_lint.md`.

If the result must outlive the calling block (returned, stored), `build_string` requires `unsafe(...)` or `options persistent_heap`. Most call sites use `unsafe(build_string()) $(var writer) { ... }`.

## Char-level access — `peek_data` and `modify_data`

When you genuinely need byte-level scanning that isn't a built-in (custom lexer, byte-by-byte hex dump, in-place transforms with cross-byte state):

```das
peek_data(s) $(arr) {              // arr : array<uint8> const#
    for (c in arr) {                // loop var is the byte value (or const ref for arrays of structs)
        if (is_white_space(int(c))) { ... }
    }
}

let out = modify_data(s) $(var arr) {   // arr : array<uint8>#  — mutating block returns a new string
    for (c in arr) {                     // here c is a mutable element reference
        if (c == uint8('\\')) { c = uint8('/') }
    }
}
```

- **`peek_data(s) $(arr)`** — single `strlen` + bounds check up front, then O(1) per-byte read. Use for hot loops over string contents.
- **`modify_data(s) $(var arr)`** — opens a mutable byte view, returns a new string with the modifications. Allocates one new string.
- **`character_at(s, i)`** — O(n) per call (it re-walks `strlen` and bounds-checks). Fine for one-off character tests; never use inside a loop. CLAUDE.md's "Don't iterate via `character_at`" rule lives here.
- **Empty-string caveat**: `peek_data("")` does NOT invoke its block. Any function that processes an external pattern/path through `peek_data` must handle empty-input explicitly at the top. `fio.match_glob` does this; copy the pattern.
- **Loop-var mutation in `modify_data`**: `for (c in arr) { c = uint8('/') }` works because `arr` is `array<uint8>` and the loop variable is a mutable element reference. No need for `for (i in range(length(arr)))` + `arr[i] = ...` unless you actually need the index.
- **Pointer-based access** (`reinterpret<uint8?>(s)`) is for the core library implementation only — user code and daslib should always go through `peek_data` / `modify_data`.

## Joining

`join` (boost) handles three shapes:

```das
let csv = join(["a", "b", "c"], ", ")              // simple string-array
let line = join(it, " ")                            // any iterable
let formatted = join(items, ", ") $(var w, elem) {  // custom per-element formatter
    w |> write(elem.name); w |> write("=")
    w |> write(elem.value)
}
```

The block form composes with `build_string` internally, so it's the right tool when you'd otherwise write a manual `for` + `if (skip_first) ... else writer |> write(sep)` loop.

## Comparison and predicates

- **`==` and `!=`** between `string` and `string`, between `string` and `das_string`, work directly. Don't write `string(das_str) == "foo"` — drop the cast.
- **`compare_ignore_case(a, b)`** — `< 0` / `0` / `> 0`, like `strcmp`.
- **`is_null_or_whitespace(s)`** (boost) — `s == ""` or every char is whitespace.
- **`is_alpha`/`is_alnum`/`is_hex`/`is_number`/`is_white_space`/`is_tab_or_space`/`is_new_line`** — all take an `int` (the int-as-char form). When using with `peek_data`, write `is_white_space(int(c))`.
- **`set_total` / `set_element` / `is_char_in_set`** — bitset over the 256-byte alphabet. Compile-time-buildable char sets; faster than `find(charset, c) >= 0` for hot loops.

## Escape and unescape

- **`escape(s)`** — produces a string literal ready for embedding (quotes added, control chars escaped).
- **`unescape(s)`** — inverse; panics on bad escape sequence.
- **`safe_unescape(s)`** — same, returns `""` on bad input instead of panicking. Use at boundaries.

## Levenshtein

```das
let d = levenshtein_distance(a, b)         // exact
let d = levenshtein_distance_fast(a, b)    // bit-parallel; same result for short strings
```

Use for "did you mean" suggestions. The `_fast` variant is meaningfully faster for short strings; both have identical semantics.

## Glob matching

`daslib/strings_boost` exports `glob_match(pattern, text)` — single-pattern shell-style match, **no `**` and no `/` boundary**. For path-aware globbing (`**`, `/`-segment boundaries, character classes, recursive walk + match), use `daslib/fio`'s `match_glob` / `glob` / `glob_filtered` instead. Full guidance: **`skills/glob.md`**.

## Common gotchas

- **`replace(s, "*", "")` is a fast "is every char X" probe** — collapses to a one-line `empty(replace(s, ch, ""))`. Beats hand-rolled byte loops.
- **`find(s, '*')` accepts a char (int) directly** — no need to wrap as `"*"`. Same for `rfind`. Mixing the int and string overloads is fine.
- **`find` returns `int`, not `int?`** — `< 0` means not found, **never** compare `find(...) == false`.
- **`split` (boost) returns `array<string>`** — non-copyable, so move-receive: `let parts <- split(s, ",")`. The `split_by_chars` block-form generic is the no-allocation choice when you only need to iterate.
- **`replace_multiple` (boost) does ONE pass** — replacements don't see each other's output. `replace_multiple(s, [("a","b"),("b","a")])` swaps `a`↔`b`; nested `replace` calls would not.
- **`to_lower` / `to_upper` allocate**; `to_lower_in_place` / `to_upper_in_place` mutate the input string buffer (still O(n), but no extra alloc). Pick by whether you still need the original.
- **`character_at(s, i)` is O(n)**, not O(1). The compiler does not memoize. CLAUDE.md flags this; perf_lint may catch it (PERF003).
- **String comparison with `das_string`** works directly — `if (das_str == "foo")`, `if (empty(das_str))`. Don't write `string(das_str)`.
- **Hex literals are `uint`** — `int(0x3F)` for int. Same for `to_int("0x3F", true)` (the `accept_hex` bool).
- **`int(s)` / `float(s)` are the silent parsers** — same caveat as `to_int`/`to_float`. Always use `try_to_*` from `strings_convert` for external input.
- **`peek_data("")` does not call the block.** Empty-input checks go at the top of any wrapping function.
- **String-builder result must escape unsafe gates if it outlives the block** — `unsafe(build_string()) $(...) { ... }` or `options persistent_heap` is the standard pattern when returning a built string.
- **Never compare `find(s, sub) == false`** — `find` returns an `int` (offset or `-1`), not a bool. `find >= 0` for "found".

## Cross-references

- `skills/glob.md` — pathname matching with `*`, `?`, `**`, `[abc]` (`fio.match_glob`, `glob`, `glob_filtered`).
- `skills/regex.md` — when you need full regex (lookarounds, named groups, repetition counts) instead of `find`/`split`.
- `skills/perf_lint.md` — PERF002 (concat in loops), PERF003 (`character_at` in loops), PERF005 (`string(das_string)`), PERF010 (`get_ptr` on string).
- `skills/style_lint.md` — STYLE-rules around string-builder pipe form, redundant casts.
- `skills/json.md` — `sprint_json` / `sscan_json` / `JV` / `from_JV` — never hand-build JSON via string concat.
- `daslib/strings_boost.das` — source for the boost layer.
- `daslib/strings_convert.das` — source for `try_to_*`.
