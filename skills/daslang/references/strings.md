# Strings and Regular Expressions

## Modules

```das
require strings
require daslib/strings_boost
require daslib/strings_convert
require daslib/regex
require daslib/regex_boost
```

`length(s)` / `empty(s)` are base builtins, no `require`; `long_length(s)` is the 64-bit form
(`length` panics rather than wrapping past 2^31). Below is `strings` unless marked *(boost)*.

## Reach for a built-in before a byte loop

| To do this | Use |
|---|---|
| Find a char or a substring | `find(s, '*')` / `find(s, "foo")` — `-1` on miss; `rfind` takes substrings only |
| Contains / count occurrences | `contains(s, "foo")` / `count(s, "foo")` *(boost)* |
| Test or strip a prefix / suffix | `starts_with` / `ends_with`; `trim_prefix` / `trim_suffix` *(boost)* |
| Replace | `replace(s, "a", "b")`; many pairs in ONE pass: `replace_multiple` *(boost)* |
| "Is every char X" | `empty(replace(s, "X", ""))` |
| Split | `let parts <- split(s, ",")` *(boost)* — non-copyable `array<string>`; `split_by_chars(s, " \t\n")` for any-of-these |
| Trim whitespace | `trim` / `strip` and the sided `ltrim` / `rtrim` / `strip_left` / `strip_right`; `skip_white_space(s, i)` returns the offset where content resumes |
| Case | `to_upper` / `to_lower`; compare via `compare_ignore_case(a, b) == 0` |
| Pad, repeat, reverse, join | `pad_left(s, w, ' ')` / `pad_right`, `repeat(unit, n)`, `reverse(s)`, `join(items, ", ")` *(all boost except `repeat` / `reverse`)* |
| Edit distance | `levenshtein_distance(a, b)`, or `levenshtein_distance_fast` for short strings |

`replace_multiple(s, [(text="a", replacement="b"), (text="b", replacement="a")])` swaps a and b —
replacements never see each other's output. `join(items, ", ") $(var w, elem) { … }` is the
custom-formatter form.

## Parsing numbers — validate at the boundary

```das
let silent  = to_int(s)              // 0 for "foo", 12 for "12abc"
let lenient = try_to_int(s) ?? 0

let r = try_to_int(s)                // Result<int; ConversionError>
if (is_err(r)) { report("bad input: {s} ({unwrap_err(r)})") }
use(unwrap(r))
```

- **`to_int` / `to_float` / `int(s)` / `float(s)` parse silently** — only for data already known
  clean.
- **`try_to_int` / `try_to_float`** *(strings_convert)* cover `int8`…`uint64`, `float`, `double`
  and distinguish `invalid_argument`, `out_of_range`, `trailing_garbage`. Accessors (`is_ok` /
  `unwrap` / `unwrap_or` / `map` / `and_then` / `??`) arrive transitively. `Result` is a structural
  **tuple**, not a variant — `r is value` is `error[30190]`; never touch the raw `_is_ok` /
  `_value` / `_error` fields.
- **Raw builtins do not flag trailing garbage.** `int(s, var conv, var consumed, accept_hex)`
  (3-arg for `float` / `double`) reports `conv = ok` on `"12abc"`, `consumed = 2`; only
  `consumed != length(s)` catches it.

## Building strings

```das
let result = build_string() $(var writer) {
    for (x in items) {
        writer |> write(", ")
        writer |> write(x)
    }
}
```

- Writer ops: `write(any)`, `write_char(c)`, `write_chars(c, n)`, `write_escape_string(s)`;
  `build_hash()` runs the same block, returning `uint64` instead of the string.
- **The result lives in the context string heap** — safe to return or store, no `unsafe`. An empty
  build returns `""`.
- **`fmt` vs `format`.** `fmt(":.2f", v)` takes a colon-prefixed libfmt spec, no `%`; `format` is
  the deprecated printf-style API, and a printf spec given to `fmt` compiles then panics at
  runtime. Both are numeric-only — a spec on a `string` is `error[30341]`; pad text with
  `pad_left` / `pad_right`.
- **Interpolation is sugar for `build_string`** — in a loop, write the builder. Inline specs work
  there: `"{x:.2f}"`, `"{n:>8}"`, `"{v:08.2f}"`, `"{v:*>8}"` rewrite to `fmt(":spec", expr)`,
  numeric-only as above.
- **Per-iteration strings in a hot loop**: `daslib/temp_strings` has `temp_string("…{x}…") $(s) {
  … }` and `build_temp_string() $(var writer) { … }` — the block gets a `string#`, freed on return.
  `temp_string` takes only a string-builder argument (an interpolated literal); a variable or call
  is `error[40102]`.

## Byte views — `peek_data` and `modify_data`

`peek_data(s) $(d)` opens the bytes as an `array<uint8> const#`: one `strlen` up front, then O(1)
per byte. `modify_data(s) $(var arr)` gives a mutable view and returns a NEW string; its loop
variable is a mutable element reference — `for (c in arr) { c = uint8('/') }`, no index.

**`slice`, `chop`, `find` and friends re-`strlen` the source on every call** and return a fresh
heap string — a slice-per-element loop over a big string is O(n²).

| Family | View forms |
|---|---|
| slice / chop | `slice(d, a, b)`, `slice(d, a)`, `chop(d, a, n)` — negative indices count from the end |
| starts_with | `starts_with(d, cmp)`, `(d, cmp, cmpLen)`, `(d, off, cmp)`, `(d, off, cmp, cmpLen)` |
| ends_with / find / rfind | `ends_with(d, cmp)`; `find(d, sub[, start])`, `find(d, ch[, start])`; `rfind(d, sub[, start])` |
| strip / trim | `strip(d)`, `strip_left`, `strip_right`; `trim(d)`, `ltrim`, `rtrim(d[, chars])` |
| whitespace cursor | `skip_white_space(d, from)` — no allocation |
| parse | `int(d, res, cursor, hex)` for every integer width; `float(d, res, cursor)` / `double(…)` |
| writer / materialize | `write_string(w, d[, a, b])`; `string(d)` view → string, `to_bytes(s)` string → owned bytes |

- **Only the haystack becomes a view** — needles stay `string`: `find(d, "foo")`, `rtrim(d, " \t")`.
  `length(d)`, `empty(d)`, `d[i]` are array builtins, no twin needed.
- **The parse cursor is IN and OUT** — it starts the scan, skips leading whitespace, and on success
  lands just past the number. On failure `res` says why and the cursor stays put; outside
  `0..length(d)` it reports `invalid_argument`. (String forms write the offset out only, always
  starting at 0.)
- **Comparisons and searches allocate nothing**; `slice` / `strip` / `trim` still allocate a temp
  string.
- **Views are binary-safe, strings are not.** View ops are length-bounded, so an interior `\0` is
  ordinary data — but materialize a window containing one and downstream `strlen` ops read it as
  truncated there. A view past `INT_MAX` bytes panics.
- **A helper taking a view** declares `bytes : array<uint8> const implicit` — binds both a plain
  `array<uint8>` and the `#` view.
- **`peek_data("")` does not invoke its block** — external input needs an explicit empty check.
- **`character_at(s, i)` is O(n)** — never inside a loop.

## Predicates, char sets, escaping

- **`is_alpha` / `is_alnum` / `is_hex` / `is_number` / `is_white_space` / `is_tab_or_space` /
  `is_new_line`** all take an `int`, so from a view write `is_white_space(int(c))`.
- **One whitespace set everywhere** — space, tab, CR, LF, FF, VT (the C `isspace` set) — across the
  classifiers, `skip_white_space`, `strip` / `trim`, and every parse function's leading skip. JSON
  is the exception: the spec-exact four-char set.
- **Char sets** are a `uint[8]` bitset over the 256-byte alphabet: build one with bit math
  (`cset[ch / 32] |= 1u << uint(ch & 31)`), query with `is_char_in_set(ch, cset)` — beats
  `find(charset, c) >= 0` in a hot loop. `set_total` counts members, `set_element(i, cset)` returns
  the i-th; nothing builtin *adds* to a set.
- **`escape(s)`** escapes `"`, `\`, and control characters, but adds no quotes and leaves `{` /
  `}`, so its output is unsafe in an interpolated literal. **`unescape(s)`** panics on a bad
  sequence; **`safe_unescape(s)`** never panics and has no error signal.

## String gotchas

- **`find` returns `int`** — `>= 0` means found, never compare to `false`. `find(s, ch)` matches
  `ch` as an unsigned byte: a code outside `0..255` matches nothing.
- **Escape a literal brace as `\{` / `\}`** — an unescaped `{` starts an interpolation even in
  JSON-looking text, and the errors point somewhere else entirely.
- **A nested string literal inside an interpolation is written plain** — `"{s == "abc"}"` works;
  *escaping* the inner quotes (`"{s == \"abc\"}"`) is the syntax error. (probe-verified 2026-08-20)
- **`das_string` compares with `string` directly** — `das_str == "foo"`, `empty(das_str)`; never
  `string(das_str) == "foo"`. `to_int(s, true)` is the accept-hex parse.
- `:=` on a string clones only under `options multiple_contexts` (see memory.md).

## Regular expressions

`daslib/regex` is a pure NFA engine, not PCRE or POSIX. `daslib/regex_boost` adds the compile-time
reader macro, the default for a literal pattern.

```das
var private RE   <- %regex~^\s*(?:def|struct)\s+private\b%%
var private RE_I <- %regex~hello~i%%          // case-insensitive
var private RE_S <- %regex~start.*end~s%%     // dot-all: `.` matches \n; `~is%%` combines both
```

Flags go after a trailing `~` INSIDE the macro; anything after the closing `%%` is a syntax error.
Only `i` and `s` are recognized — any other letter is silently taken as part of the pattern. The
macro compiles at parse time: no escape doubling (`\s+`, not `"\\s+"`), bad patterns become compile
errors, no per-call compile cost. Matching mutates the `Regex`, so module-scope **`var`** is the
standard shape. Runtime patterns: `regex_compile(expr, case_insensitive, dot_all)`; `is_valid(re)`
takes a const `Regex`, so it works on a `let`.

| Call | Result |
|---|---|
| `regex_match(var re; str; offset = 0) : int` | **Anchored** at `offset`, not a substring search. Returns the END position (`offset + length`), or `-1`. |
| `regex_search(var re; str; offset = 0) : int2` | Scans forward. `int2(start, end)`, `end` exclusive; `int2(-1,-1)` if absent. |
| `regex_match_all(var re; str) : array<range>` | Every match, same start/end convention. |
| `regex_group(re, index, match)` / `regex_group_by_name(re, name, str)` | Group text; `re[index]` / `re[name]` give the `range`. Group `0` is the whole match. |
| `regex_foreach(var re; str; blk)` | Per-match callback taking `at : range`. |
| `regex_replace(var re; str; replacement)` / `(…; blk)` | Template form, or a callback computing each replacement. |
| `regex_split(var re; str) : array<string>` | Split on every match. |

```das
let rewritten = regex_replace(re, src, "$1=$2")
let upper = regex_replace(re, src) <| $(matched) { return to_upper(matched) }
```

Template references: `$0` / `$&` the whole match, `$1`–`$9` numbered groups, `${name}` named,
`${N}` any index, `$$` a literal `$`. **`${...}` braces must be escaped** — `"$\{name\}"` — or the
`{` starts an interpolation and the group name resolves as a variable.

| Feature | Syntax |
|---|---|
| Anchors | `^` `$` |
| Classes | `\w` `\W` `\d` `\D` `\s` `\S` `\t` `\n` `\r` `\f` `\v`; sets `[a-z]`, `[^abc]`, `[\d_]` |
| Quantifiers | greedy `+` `*` `?` `{n}` `{n,}` `{n,m}`; lazy `+?` `*?` `??` `{n,m}?` |
| Groups | `(...)`, non-capturing `(?:...)`, named `(?P<name>...)` |
| Lookahead, word boundary | `(?=...)`, `(?!...)`; `\b`, `\B` |
| Alternation, hex, escapes | `\|`; `\xHH`; `\.` `\+` `\*` `\(` `\[` `\^` `\{` `\\` |
| Any char | `.` — not `\n` unless `dot_all` |

- **Every entry point short-circuits on `""`** — even a pattern that matches it (`^$`, `a*`)
  reports no match; handle empty input in the wrapper.
- **ASCII only** — 256-bit char sets, no Unicode classes; `case_insensitive` folds A–Z only.
- **Catastrophic backtracking is possible** — anchor with `^`, prefer non-capturing groups, avoid
  nested `*` / `+`.
