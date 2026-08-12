# Strings in daslang

Read this skill before writing or editing `.das` code that searches, splits, replaces, parses, formats, or otherwise manipulates strings — anywhere you'd reach for `find`, `replace`, `split`, `to_int`, `build_string`, or `peek_data` / `modify_data`.

## Modules

```das
require strings                  // core: most ops live here
require daslib/strings_boost     // extras: split, contains, count, join, levenshtein, pad, replace_multiple
require daslib/strings_convert   // safe parsing: try_to_int / try_to_float (Result-returning)
```

`strings` is the always-on built-in — `find`, `replace`, `slice`, `to_int`, `build_string`, `format`, `peek_data`/`modify_data`, the `is_*` char predicates, etc. **`length` and `empty` need no `require` at all** — both live in the base module, so `length(s)` works in a bare file. Their 64-bit forms `long_length(s)` are there too; the `int`-returning `length` panics rather than wrapping past 2^31. `strings_boost` is the daslang-side companion (multi-result `split`, `contains`, `count`, generic `join`, `pad_left`/`pad_right`, `levenshtein_distance`, `replace_multiple`, `glob_match`, `is_null_or_whitespace`). `strings_convert` is the validating-parse layer.

## Don't iterate bytes when a built-in does it

The single most common antipattern in this codebase is opening `peek_data` and writing a `for (i in range(length(arr)))` loop to do something that `find` / `replace` / `starts_with` / `split` already does. Reach for byte-level access **only** when the operation is genuinely not expressible as a built-in (state machines, custom parsers, multi-byte transforms with cross-byte context).

| If you want to | Use | Don't |
|---|---|---|
| Find a char or substring | `find(s, '*')` / `find(s, "foo")` (returns -1 on miss) | byte loop with `c == uint8('*')` |
| Find from the right | `rfind(s, "/")` / `rfind(s, "ab")` | reverse byte loop |
| "Does it contain X" | `contains(s, "foo")` (boost) or `find(s, "foo") >= 0` | byte loop |
| "Does it start/end with X" | `starts_with(s, "foo")` / `ends_with(s, ".das")` | `slice(s, 0, n) == "foo"` |
| Replace all `a` with `b` | `replace(s, "a", "b")` | `modify_data` byte loop |
| Replace many pairs in one pass | `replace_multiple(s, [(text="a", replacement="x"), (text="b", replacement="y")])` (boost) | nested `replace` calls |
| "Is every char X" | `empty(replace(s, "X", ""))` | byte loop accumulating a flag |
| Count occurrences | `count(s, "foo")` (boost) | byte loop |
| Split on a separator | `split(s, ",")` (boost) → `array<string>` | manual `find` + `slice` loop |
| Split on any of several chars | `split_by_chars(s, " \t\n")` (boost) | byte loop with branching |
| Trim whitespace | `trim(s)` / `strip(s)`, or `ltrim`/`rtrim`/`strip_left`/`strip_right` | manual leading/trailing scan |
| Skip a whitespace run from offset `i` | `skip_white_space(s, i)` — the offset where content resumes | `while (is_white_space(int(d[i]))) { i++ }` |
| Strip a known prefix/suffix | `trim_prefix(s, "foo/")` / `trim_suffix(s, ".das")` (boost) | `starts_with` + `slice` |
| Uppercase / lowercase | `to_upper(s)` / `to_lower(s)` | manual case-fold loop |
| Pad to width | `pad_left(s, w, ' ')` / `pad_right(s, w, ' ')` (boost) | manual `repeat` + concat |
| Repeat a unit | `repeat(unit, n)` | concat loop |
| Reverse | `reverse(s)` | byte loop |
| Case-insensitive compare | `compare_ignore_case(a, b) == 0` | `to_lower(a) == to_lower(b)` (allocates twice) |

The refactor that motivated this skill (commits in fio.das around `is_glob_pattern`, `to_generic_path`, the empty-path branch of `match_glob`) collapsed three byte-level loops into one-liners using `find`, `replace`, and `empty(replace(s, "*", ""))`. If you're staring at a `peek_data` block that only reads bytes, ask "does `find` or `replace` already do this?" first.

Opening a `peek_data` view is not the same as hand-rolling a byte loop: most of the table above also takes the view, so the built-in still does the work — see **Byte views** below.

## Parsing numbers — validate at boundaries

Two families of parse functions, picked by what the caller does on garbage input:

```das
require strings
require daslib/strings_convert

let silent  = to_int(s)                           // silent — returns 0 for "foo", 12 for "12abc"
let lenient = try_to_int(s) ?? 0                  // default on failure — the one-liner

let r = try_to_int(s)                             // Result<int; ConversionError>
if (is_err(r)) { bail("bad input: {s} ({unwrap_err(r)})") }
use(unwrap(r))
```

`Result<T; E>` (daslib/result) ships an accessor + combinator API and its own docstring says to prefer it over direct field reads: `is_ok` / `is_err` / `unwrap` / `unwrap_or` / `unwrap_or_default` / `unwrap_or_else` / `unwrap_err` / `move_unwrap` / `expect_value` / `expect_err` / `if_ok` / `if_err` / `map` / `map_err` / `and_then` / `or_else` / `to_option`, plus operators `??`, `==`, `!=`. They arrive transitively — `strings_convert` does `require daslib/result public`, so the require block above is enough.

**Why `is`/`as` don't work on a Result:** it is a **structural tuple** `tuple<_is_ok : bool; _value : T; _error : E>`, not a variant, so `r is value` is `error[30190] is value only allowed for variants` and `r as value` is `error[30113]`. Those three `_`-prefixed fields are the raw representation; reach for the accessors instead. (The variant form belongs to fio's `fs_result_*` types, which are real variants.)

- **`to_int(s)` / `to_float(s)`** silently return `0` / `0.0` for unparseable input and partial-parse for `"12abc"`. Fine for trusted internal data; **never** for user input, env vars, file contents, command-line args, or anything that flows into a shell call, file path, or system call. `";rm -rf;"` parses as `0` with `to_int`.
- **`try_to_int` / `try_to_float`** in `daslib/strings_convert` return a `Result<T; ConversionError>` distinguishing `invalid_argument` (no digits), `out_of_range` (overflow), and `trailing_garbage` (`"12abc"`). The whole `try_to_*` family covers `int8`/`uint8`/.../`int64`/`uint64`/`float`/`double`.
- The raw builtins `int(s, var conv_result, var consumed, accept_hex)` (4-arg for the integer types, 3-arg for `float` / `double` — no `accept_hex`) surface a `ConversionResult` plus a consumed-character count. `ConversionResult.ok` alone is **not** a boundary check: `int("12abc", cr, consumed, false)` yields `12` with `cr = ok` and `consumed = 2`. Trailing garbage only shows up as `consumed != length(s)` — which is exactly what `try_to_*` does for you.

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
- `writer |> fmt(spec, val)` / `writer |> format(spec, val)` — formatting for one numeric value, with **different spec syntax per function**. `fmt` (per-type overloads, int8…double) takes a **colon-prefixed libfmt-style spec, no `%`**: `fmt(":.2f", v)` — a printf `"%.2f"` spec compiles but panics at runtime ("invalid format string"). `format` (int/uint/int64/uint64/float/double) is the old printf-style API (`snprintf` underneath, `%` specs work) and is **deprecated — use `fmt`**. The same split applies to the string-returning forms: `format(spec, v) : string` is printf-style and deprecated, `fmt(spec, v) : string` is colon-style. **Both are numeric-only** — overloads exist for the int/uint/float/double workhorse types but **not `string`**, so a spec on a text expression is a *compile error* (`error[30341] no matching … fmt(string, …)`), not a runtime one. To pad/align a **text** column use `pad_left` / `pad_right` from `daslib/strings_boost`.
- **String interpolation** `"{x} {y}"` is sugar for `build_string` — fine for one-off small constructions, but inside a loop or for long pipelines, write the explicit `build_string` so the writer is reused.
- **Inline format spec** — append `:spec` to an interpolated expression: `"{x:.2f}"` → `206.85`, `"{r:.2f}x"` → `1.44x`, `"{n:>8}"` right-aligns to width 8, `"{v:08.2f}"` zero-pads, `"{v:*>8}"` custom-fills. The parser rewrites `{expr:spec}` into `fmt(":spec", expr)`, so `spec` is a full **libfmt / Python-format** replacement field: `[[fill]align][sign][0][width][.precision][type]`. Same numeric-only limit as `fmt` above — a spec on a `string` won't compile.
- **Per-iteration strings in a hot loop:** `daslib/temp_strings` gives `temp_string("…{x}…") $(s) { ... }` and `build_temp_string() $(var writer) { ... }` + a `lpipe() $(s) { ... }` consumer block (`daslib/lpipe`), which hand a `string#` to the callback and free it on return — no string-heap growth across iterations. `temp_string` accepts only a *string-builder* argument (an interpolated literal); a plain variable, plain literal, or function call is `error[40102]`. Both skip the delete when `intern_strings` is on.

`perf_lint` flags some bad patterns here — PERF001 (`+=` concat in a loop), PERF004 (interpolation rebuild in a loop), PERF007 / PERF012 (redundant `string(das_string)`) — see `skills/perf_lint.md`.

`build_string` copies the assembled bytes into the context string heap (`context->allocateString`), so the result outlives the block. Returning or storing it is safe and needs no `unsafe(...)` wrapper. Do not infer a dangling-string bug from a direct `return build_string() $(...) { ... }` call. A build that wrote nothing comes back as the empty string.

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
- **`slice(s, i, j)` and `chop(s, i, n)` carry the same trap** — each call strlens the SOURCE (das strings cache no length), so per-call cost is O(length(s)) no matter how small the slice, and the result is a fresh heap string nothing frees. A slice-per-element loop over a big string is O(n²). The fix is mechanical: wrap the loop in `peek_data(s) $(d) { ... }` and swap the receiver — `slice(d, i, j)`, `find(d, ...)`, `starts_with(d, ...)`. The view carries its length, so the strlen happens once, at `peek_data`. See **Byte views** below; `perf_lint` flags the loop as PERF031.
- **Empty-string caveat**: `peek_data("")` does NOT invoke its block. Any function that processes an external pattern/path through `peek_data` must handle empty-input explicitly at the top. `fio.match_glob` does this; copy the pattern.
- **Loop-var mutation in `modify_data`**: `for (c in arr) { c = uint8('/') }` works because `arr` is `array<uint8>` and the loop variable is a mutable element reference. No need for `for (i in range(length(arr)))` + `arr[i] = ...` unless you actually need the index.
- **Pointer-based access** (`reinterpret<uint8?>(s)`) is for the core library implementation only — user code and daslib should always go through `peek_data` / `modify_data`.

## Byte views — the same operations without the re-strlen

`peek_data(s) $(d)` hands the block an `array<uint8>` view of the string's bytes. Most of the search / slice / parse surface has a twin that takes that view instead of a `string`, same name and same argument order — so fixing a per-iteration `slice` is a two-line edit: wrap the loop, swap the receiver.

```das
// Before — every slice/find/starts_with re-strlens `line`, so the loop is O(n²)
var i = 0
while (i < n) {
    total += length(slice(line, i, i + 4))
    if (starts_with(line, i, "//")) break
    i = find(line, ' ', i) + 1
}

// After — one strlen at peek_data; every op below reads the length the view carries
peek_data(line) $(d) {
    var i = 0
    while (i < n) {
        total += length(slice(d, i, i + 4))
        if (starts_with(d, i, "//")) break
        i = find(d, ' ', i) + 1
    }
}
```

| family | view forms | notes |
|---|---|---|
| slice / chop | `slice(d, a, b)`, `slice(d, a)`, `chop(d, a, n)` | negative indices count from the end, as on strings |
| starts_with | `starts_with(d, cmp)`, `(d, cmp, cmpLen)`, `(d, off, cmp)`, `(d, off, cmp, cmpLen)` | answers from the view — no allocation |
| ends_with | `ends_with(d, cmp)` | |
| find | `find(d, sub)`, `(d, sub, start)`, `(d, ch)`, `(d, ch, start)` | `ch` matches as an unsigned byte |
| rfind | `rfind(d, sub)`, `(d, sub, start)` | |
| strip | `strip(d)`, `strip_left(d)`, `strip_right(d)` | |
| trim | `trim(d)`, `ltrim(d)`, `rtrim(d)`, `rtrim(d, chars)` | |
| whitespace cursor | `skip_white_space(d, from)` | offset of the first non-whitespace byte at or after `from` — `strip_left` as a cursor, no allocation; `skip_white_space(s, from)` is the string form |
| parse | `int(d, res, cursor, hex)` — same shape for `int8`/`uint8`/`int16`/`uint16`/`uint`/`int64`/`uint64`; `float(d, res, cursor)` / `double(d, res, cursor)` | `cursor` is IN and OUT |
| writer | `write_string(w, d)`, `write_string(w, d, a, b)` | writes view bytes straight into a `build_string` writer |
| materialize | `to_bytes(s)` → owned `array<uint8>` | the inverse direction: `string(d)` makes a string out of bytes, `to_bytes(s)` makes bytes out of a string. A fresh owned copy (no terminator appended), so it outlives the `peek_data` block a view is confined to |

- **Only the haystack becomes a view** — needles stay `string` (`find(d, "foo")`, `rtrim(d, " \t")`).
- **`length(d)`, `empty(d)`, `d[i]` are the array builtins** and need no twin; `string(d)` materializes the whole view.
- **The parse cursor is in AND out.** `var cursor = 0` then `let v = int(d, res, cursor)` starts at `cursor`, skips leading whitespace, and on success leaves `cursor` just past the number — feed it straight to the next call to walk a list of numbers. On failure `res` says why and `cursor` stays where it was; a `cursor` outside `0..length(d)` reports `invalid_argument`. (The string forms write their `offset` out only, and always start at 0.)
- **Materializing costs an allocation, not a scan.** `slice` / `chop` / `strip` / `trim` on a view still allocate a fresh temp string; what disappears is the per-call `strlen`. Comparisons and searches answer from the view with no allocation at all — reach for the materializing forms only when a `string` is what you need next.
- **Views are binary-safe, strings are not.** Every view op is bounded by the view's length, so a `\0` inside the bytes is data like any other byte. Materialize a window that contains one and the result is an ordinary `string`, which every downstream `strlen`-based op reads as truncated at that NUL — same as `string(d)` has always been.
- **A view longer than `INT_MAX` bytes panics.** Offsets are `int`, so such a view has no representable answer; the op stops the way `length` does instead of returning a wrapped offset.
- **Passing a view to your own helper**: declare the parameter `bytes : array<uint8> const implicit` — that spelling binds both a plain `array<uint8>` and the `#` view `peek_data` yields. Needed the moment a scan is factored out of the `peek_data` block.
- **`rfind(d, sub, start)` returns -1 for a negative `start`** (same as the string form) — searching backward from before the start has no answer. A backwards walk that re-feeds `hit - 1` as the next `start` therefore terminates on its own at a match on offset 0.
- **`peek_data("")` skips its block** — an empty das string is a null pointer, so there is nothing to open a view over (a known framework gap, slated to change to invoking the block with an empty view); until then, guard with `if (empty(s))` when the empty case carries behavior.
- **Code that calls a verifying macro (e.g. `macro_verify`) cannot move inside the wrap** — such macros expand to `return null`, and a `return` with a value inside a block argument is `error[31402]`. Leave those loops on the string forms (nolint with the reason) or restructure so the verify stays outside the block.

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
- **`is_alpha`/`is_alnum`/`is_hex`/`is_number`/`is_white_space`/`is_tab_or_space`/`is_new_line`** — all take an `int` (the int-as-char form). When using with `peek_data`, write `is_white_space(int(c))`. To advance over a whole whitespace *run* don't loop on the classifier — `skip_white_space(s, i)` / `skip_white_space(d, i)` returns the offset where content resumes.
- **Whitespace is one six-character set everywhere** — space, tab, CR, LF, FF and VT (the C `isspace` set). `is_white_space`, `skip_white_space`, `strip`/`strip_left`/`strip_right`, `trim`/`ltrim`/`rtrim`, the leading-whitespace skip every parse function does, and `is_null_or_whitespace` all agree on it. JSON is the one deliberate exception: RFC 8259 whitespace is exactly space/tab/CR/LF, so `daslib/json`'s lexer keeps a spec-exact test of its own and rejects FF and VT.
- **Char sets** are a `uint[8]` bitset over the 256-byte alphabet. **Build** one with bit math — `cset[ch / 32] |= 1u << uint(ch & 31)` — then query with `is_char_in_set(ch, cset)` (faster than `find(charset, c) >= 0` in hot loops). `set_total(cset)` is the population count; `set_element(i, cset)` is the i-th member character (or `-1`). Both are readers — there is no builtin that *adds* a char to a set.

## Escape and unescape

- **`escape(s)`** — escapes `"`, `\`, and control characters (`\t`, `\n`, …). It does **not** add surrounding quotes, and it does **not** escape `{` / `}`, so its output is not safe to paste into a daslang *interpolated* string literal.
- **`unescape(s)`** — inverse; panics on bad escape sequence.
- **`safe_unescape(s)`** — the same decoder, but it never panics and has **no error signal**: it returns the best-effort decode (an invalid escape yields the escaped character itself — `safe_unescape("a\\qb")` is `aqb` — and a truncated trailing `\` stops decoding). Use at boundaries where garbage-in-garbage-out is acceptable.

## Levenshtein

```das
let d = levenshtein_distance(a, b)         // exact
let d = levenshtein_distance_fast(a, b)    // bit-parallel; same result for short strings
```

Use for "did you mean" suggestions. The `_fast` variant is meaningfully faster for short strings; both have identical semantics.

## Glob matching

`daslib/strings_boost` exports `glob_match(pattern, text)` — single-pattern shell-style match, **no `**` and no `/` boundary**. For path-aware globbing (`**`, `/`-segment boundaries, character classes, recursive walk + match), use `daslib/fio`'s `match_glob` / `glob` / `glob_filtered` instead. Full guidance: **`skills/glob.md`**.

## Common gotchas

- **Escape a literal opening brace in an interpolated string as `\{`.** An unescaped `{` starts an
  interpolation expression even in JSON-looking text such as `"{\"action\":...}"`; this can produce
  misleading downstream errors (for example, a following local declaration appears not to exist).
  Write `"\{\"action\":...}"`, or preferably serialize a struct with `sprint_json` when the value is
  not a fixed test fixture.
- **No nested string literals inside an interpolation.** `"{find(\"a\", \"b\")}"` is
  `error[30151] syntax error, unexpected invalid token` — escaping the quotes does not help, the
  lexer ends the interpolation expression at the quote. Hoist the call into a local and interpolate
  the local: `let i = find(a, b)` then `"{i}"`.
- **`replace(s, "*", "")` is a fast "is every char X" probe** — collapses to a one-line `empty(replace(s, ch, ""))`. Beats hand-rolled byte loops.
- **`find(s, '*')` accepts a char (int) directly; `rfind` does not.** Use `rfind(s, "*")` with a string substring.
- **`find(s, ch)` matches `ch` as an unsigned byte** — `find(s, 195)` finds the byte `0xC3`, and a code outside `0..255` matches nothing. The view form `find(d, ch)` matches the same way.
- **`find` returns `int`, not `int?`** — `< 0` means not found, **never** compare `find(...) == false`.
- **`split` (boost) returns `array<string>`** — non-copyable, so move-receive: `let parts <- split(s, ",")`. The `split_by_chars` block-form generic is the no-allocation choice when you only need to iterate.
- **`replace_multiple` (boost) does ONE pass** — replacements don't see each other's output. Its replacement array uses named tuples: `replace_multiple(s, [(text="a", replacement="b"), (text="b", replacement="a")])` swaps `a`↔`b`; nested `replace` calls would not.
- **`to_lower` / `to_upper` allocate**; `to_lower_in_place` / `to_upper_in_place` rewrite the input string buffer in place (no extra alloc) and return it. They are **unsafe calls** — write `unsafe(to_lower_in_place(s))`. Because they rewrite the caller's buffer, use them only on a string you just built at runtime; a string *literal* is left untouched.
- **`character_at(s, i)` is O(n)**, not O(1). The compiler does not memoize. CLAUDE.md flags this; perf_lint catches it (PERF002 inside a loop, PERF003 anywhere).
- **`:=` on a string is not a clone unless `options multiple_contexts` is set.** LINT016 flags `dst := src`, `var dst := src`, and `push_clone(arr, s)` on strings: use `=` / `push` for same-context storage, or `clone_string(value)` when the string genuinely crosses contexts.
- **String comparison with `das_string`** works directly — `if (das_str == "foo")`, `if (empty(das_str))`. Don't write `string(das_str)`.
- **Hex literals are `uint`** — `int(0x3F)` for int. Same for `to_int("0x3F", true)` (the `accept_hex` bool).
- **`int(s)` / `float(s)` are the silent parsers** — same caveat as `to_int`/`to_float`. Always use `try_to_*` from `strings_convert` for external input.
- **`peek_data("")` does not call the block.** Empty-input checks go at the top of any wrapping function.
- **`build_string` results can be returned or stored directly** — the assembled bytes are copied into the context string heap. A direct `return build_string() $(...) { ... }` is not a dangling-string bug and does not require `unsafe(...)`.
- **Never compare `find(s, sub) == false`** — `find` returns an `int` (offset or `-1`), not a bool. `find >= 0` for "found".

## Cross-references

- `skills/glob.md` — pathname matching with `*`, `?`, `**`, `[abc]` (`fio.match_glob`, `glob`, `glob_filtered`).
- `skills/regex.md` — when you need full regex (lookarounds, named groups, repetition counts) instead of `find`/`split`.
- `skills/perf_lint.md` — PERF001 (`+=` concat in a loop), PERF004 (interpolation rebuild in a loop), PERF002 (`character_at` inside a loop), PERF003 (`character_at` anywhere / `character_at(s,0)` → `first_character`), PERF005 (`length(string)` in a `while` condition), PERF007 / PERF012 (redundant `string(das_string)`), PERF031 (`slice`/`chop` on a loop-invariant string inside a loop → slice the `peek_data` view). Also LINT016 (`:=` on a string) and LINT017 (`int64(length(s))` → `long_length(s)`).
- `skills/style_lint.md` — STYLE-rules around string-builder pipe form, redundant casts.
- `skills/json.md` — `sprint_json` / `sscan_json` / `JV` / `from_JV` — never hand-build JSON via string concat.
- `daslib/strings_boost.das` — source for the boost layer.
- `daslib/strings_convert.das` — source for `try_to_*`.
