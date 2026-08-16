# Strings and Regular Expressions

Searching, slicing, parsing, building, and matching text. All examples are gen2.

## Modules

```das
require strings                  // the core surface
require daslib/strings_boost     // split, join, contains, count, pad, levenshtein, replace_multiple
require daslib/strings_convert   // try_to_int / try_to_float — validating parses
require daslib/regex             // the regex engine
require daslib/regex_boost       // the %regex~...%% compile-time pattern macro
```

`length(s)` and `empty(s)` are base-module builtins needing no `require`; `long_length(s)` is the
64-bit form (the `int`-returning `length` panics rather than wrapping past 2^31). Everything below
is `strings` unless marked *(boost)*.

## Reach for a built-in before a byte loop

Opening `peek_data` and hand-writing a byte scan for something the library already does is the most
common mistake in daslang string code. Byte-level access is for what is genuinely not expressible
as a built-in: state machines, custom lexers, transforms with cross-byte context.

| To do this | Use |
|---|---|
| Find a char or a substring | `find(s, '*')` / `find(s, "foo")` — `-1` on miss; `rfind` takes substrings only |
| Contains / count occurrences | `contains(s, "foo")` / `count(s, "foo")` *(boost)* |
| Test or strip a prefix / suffix | `starts_with` / `ends_with`; `trim_prefix` / `trim_suffix` *(boost)* |
| Replace | `replace(s, "a", "b")`; many pairs in ONE pass: `replace_multiple` *(boost)* |
| "Is every char X" | `empty(replace(s, "X", ""))` |
| Split | `split(s, ",")` *(boost)*; `split_by_chars(s, " \t\n")` for any-of-these |
| Trim whitespace | `trim` / `strip` and the sided `ltrim` / `rtrim` / `strip_left` / `strip_right`; `skip_white_space(s, i)` returns the offset where content resumes |
| Case | `to_upper` / `to_lower`; compare via `compare_ignore_case(a, b) == 0` |
| Pad, repeat, reverse, join | `pad_left(s, w, ' ')` / `pad_right`, `repeat(unit, n)`, `reverse(s)`, `join(items, ", ")` *(all boost except `repeat` / `reverse`)* |
| Edit distance | `levenshtein_distance(a, b)`, or `levenshtein_distance_fast` for short strings |

`split` returns a non-copyable `array<string>` — move-receive it: `let parts <- split(s, ",")`.
`replace_multiple` does one pass with named tuples, so replacements never see each other's output:
`replace_multiple(s, [(text="a", replacement="b"), (text="b", replacement="a")])` swaps. `join` also
has a block form — `join(items, ", ") $(var w, elem) { … }` — for a custom per-element formatter.

## Parsing numbers — validate at the boundary

```das
let silent  = to_int(s)              // 0 for "foo", 12 for "12abc" — no error signal
let lenient = try_to_int(s) ?? 0     // explicit default on failure

let r = try_to_int(s)                // Result<int; ConversionError>
if (is_err(r)) { report("bad input: {s} ({unwrap_err(r)})") }
use(unwrap(r))
```

- **`to_int` / `to_float` / `int(s)` / `float(s)` are the silent parsers.** Fine for data already
  known clean; never for user input, environment variables, or file contents — `";rm -rf;"` parses
  as `0`.
- **`try_to_int` / `try_to_float`** *(strings_convert)* distinguish `invalid_argument`,
  `out_of_range`, and `trailing_garbage`, and cover `int8`…`uint64`, `float`, `double`. The
  `Result` accessors (`is_ok` / `unwrap` / `unwrap_or` / `map` / `and_then` / `??`) arrive
  transitively. `Result` is a structural **tuple**, not a variant — `r is value` is `error[30190]`;
  use the accessors, never the raw `_is_ok` / `_value` / `_error` fields.
- **The raw builtins do not flag trailing garbage.** `int(s, var conv, var consumed, accept_hex)`
  (3-arg for `float` / `double`) reports `conv = ok` on `"12abc"` with `consumed = 2`; only
  `consumed != length(s)` catches it, which is exactly what `try_to_*` does for you.

## Building strings

```das
let result = build_string() $(var writer) {
    writer |> write("prefix=")
    for (x in items) {
        writer |> write(", ")
        writer |> write(x)
    }
}
```

- One pass, versus a `+=` concatenation loop that reallocates on every append. Writer ops:
  `write(any)`, `write_char(c)`, `write_chars(c, n)`, `write_escape_string(s)`. `build_hash()` runs
  the same block and returns a `uint64` instead of building the string.
- **The result is copied into the context string heap**, so returning or storing it is safe and
  needs no `unsafe`. A build that wrote nothing comes back as `""`.
- **`fmt` vs `format`.** `fmt(":.2f", v)` takes a colon-prefixed libfmt spec with no `%`; `format`
  is the deprecated printf-style API, and a printf spec handed to `fmt` compiles then panics at
  runtime. **Both are numeric-only** — a spec on a `string` is `error[30341]`, so pad text columns
  with `pad_left` / `pad_right`.
- **Interpolation is sugar for `build_string`** — fine one-off, but inside a loop write the
  explicit builder so the writer is reused. An inline spec works there too: `"{x:.2f}"`,
  `"{n:>8}"`, `"{v:08.2f}"`, `"{v:*>8}"` rewrite to `fmt(":spec", expr)`, numeric-only as above.
- **For a per-iteration string in a hot loop**, `daslib/temp_strings` has
  `temp_string("…{x}…") $(s) { … }` and `build_temp_string() $(var writer) { … }`, which hand the
  block a `string#` and free it on return. `temp_string` takes only a string-builder argument (an
  interpolated literal); a plain variable or call is `error[40102]`.

## Byte views — `peek_data` and `modify_data`

`peek_data(s) $(d)` opens the bytes as an `array<uint8> const#`: one `strlen` up front, then O(1)
per byte. `modify_data(s) $(var arr)` opens a mutable view and returns a NEW string with the edits
— its loop variable is a mutable element reference, so `for (c in arr) { c = uint8('/') }` works
with no index.

**`slice`, `chop`, `find` and friends re-`strlen` the whole source on every call** — das strings
cache no length — and each returns a fresh heap string, so a slice-per-element loop over a big
string is O(n²). The fix is mechanical: wrap the loop in `peek_data` and swap the receiver, so
every op reads the length the view already carries.

```das
peek_data(line) $(d) {
    var i = 0
    while (i < n) {
        total += length(slice(d, i, i + 4))
        if (starts_with(d, i, "//")) break
        i = find(d, ' ', i) + 1
    }
}
```

| Family | View forms |
|---|---|
| slice / chop | `slice(d, a, b)`, `slice(d, a)`, `chop(d, a, n)` — negative indices count from the end |
| starts_with | `starts_with(d, cmp)`, `(d, cmp, cmpLen)`, `(d, off, cmp)`, `(d, off, cmp, cmpLen)` |
| ends_with / find / rfind | `ends_with(d, cmp)`; `find(d, sub[, start])`, `find(d, ch[, start])`; `rfind(d, sub[, start])` |
| strip / trim | `strip(d)`, `strip_left`, `strip_right`; `trim(d)`, `ltrim`, `rtrim(d[, chars])` |
| whitespace cursor | `skip_white_space(d, from)` — `strip_left` as a cursor, no allocation |
| parse | `int(d, res, cursor, hex)` for every integer width; `float(d, res, cursor)` / `double(…)` |
| writer / materialize | `write_string(w, d[, a, b])`; `string(d)` view → string, `to_bytes(s)` string → owned bytes |

- **Only the haystack becomes a view** — needles stay `string`: `find(d, "foo")`, `rtrim(d, " \t")`.
  `length(d)`, `empty(d)`, `d[i]` are the array builtins and need no twin.
- **The parse cursor is IN and OUT.** It starts the scan, skips leading whitespace, and on success
  lands just past the number — feed it to the next call to walk a list. On failure `res` says why
  and the cursor stays put; a cursor outside `0..length(d)` reports `invalid_argument`. (The string
  forms write their offset out only, and always start at 0.)
- **Materializing costs an allocation, not a scan.** Comparisons and searches answer from the view
  with no allocation; `slice` / `strip` / `trim` still allocate a temp string.
- **Views are binary-safe, strings are not.** Every view op is bounded by the view's length, so an
  interior `\0` is ordinary data — but materialize a window containing one and downstream
  `strlen`-based ops read the result as truncated there. A view past `INT_MAX` bytes panics.
- **To pass a view to your own helper**, declare the parameter `bytes : array<uint8> const implicit`
  — that spelling binds both a plain `array<uint8>` and the `#` view.
- **`peek_data("")` does not invoke its block** (an empty das string is a null pointer), so a
  function running external input through it needs an explicit empty check at the top.
- **`character_at(s, i)` is O(n)**, not O(1) — fine for an isolated test, never inside a loop.

## Predicates, char sets, escaping

- **`is_alpha` / `is_alnum` / `is_hex` / `is_number` / `is_white_space` / `is_tab_or_space` /
  `is_new_line`** all take an `int`, so from a view write `is_white_space(int(c))`.
- **Whitespace is one six-character set everywhere** — space, tab, CR, LF, FF, VT (the C `isspace`
  set) — shared by the classifiers, `skip_white_space`, `strip` / `trim`, and every parse
  function's leading skip. JSON is the deliberate exception, keeping the spec-exact four-char set.
- **Char sets** are a `uint[8]` bitset over the 256-byte alphabet: build one with bit math
  (`cset[ch / 32] |= 1u << uint(ch & 31)`), query with `is_char_in_set(ch, cset)` — faster than
  `find(charset, c) >= 0` in a hot loop. `set_total` counts members and `set_element(i, cset)`
  returns the i-th; both are readers, nothing builtin *adds* to a set.
- **`escape(s)`** escapes `"`, `\`, and control characters, but adds no quotes and does not escape
  `{` / `}`, so its output is not safe to paste into an interpolated literal. **`unescape(s)`**
  panics on a bad sequence; **`safe_unescape(s)`** never panics and has no error signal.

## String gotchas

- **`find` returns `int`, not a bool or an option** — `>= 0` means found; never compare to `false`.
  `find(s, ch)` matches `ch` as an unsigned byte, so a code outside `0..255` matches nothing.
- **Escape a literal brace in an interpolated string as `\{` / `\}`.** An unescaped `{` starts an
  interpolation even in JSON-looking text, and the errors then point somewhere else entirely.
- **No nested string literals inside an interpolation** — `"{find(\"a\", \"b\")}"` is a syntax
  error and escaping the quotes does not help. Hoist the call into a local, interpolate the local.
- **`das_string` compares with `string` directly** — `das_str == "foo"`, `empty(das_str)`; never
  `string(das_str) == "foo"`. `to_int(s, true)` is the accept-hex parse.
- `:=` on a string clones only under `options multiple_contexts` (see memory.md).

## Regular expressions

`daslib/regex` is a pure NFA engine — not PCRE, not POSIX. `daslib/regex_boost` adds the
compile-time reader macro, which is the right default for a literal pattern.

```das
var private RE    <- %regex~^\s*(?:def|struct)\s+private\b%%
var private RE_I  <- %regex~hello~i%%          // case-insensitive
var private RE_S  <- %regex~start.*end~s%%     // dot-all: `.` matches \n
var private RE_IS <- %regex~Foo~is%%           // both
```

Flags go after a trailing `~` INSIDE the macro; anything after the closing `%%` stays in the token
stream and is a syntax error. Only `i` and `s` are recognized — an unrecognized letter is silently
taken as part of the pattern. The macro compiles at parse time and embeds the `Regex` in the AST:
no escape doubling (`\s+`, not `"\\s+"`), bad patterns become compile errors, and there is no
per-call compile cost. Module-scope **`var`** is the standard shape, because matching mutates the
`Regex`. Build a pattern at runtime with `regex_compile(expr, case_insensitive, dot_all)`;
`is_valid(re)` takes a const `Regex`, so it works on a `let`.

| Call | Result |
|---|---|
| `regex_match(var re; str; offset = 0) : int` | **Anchored** at `offset`. Returns the END position (`offset + length`), or `-1`. |
| `regex_search(var re; str; offset = 0) : int2` | Scans forward. `int2(start, end)`, `end` exclusive; `int2(-1,-1)` if absent. |
| `regex_match_all(var re; str) : array<range>` | Every match, same start/end convention. |
| `regex_group(re, index, match)` / `regex_group_by_name(re, name, str)` | Group text; `re[index]` / `re[name]` give the `range`. |
| `regex_foreach(var re; str; blk)` | Per-match callback taking `at : range`. |
| `regex_replace(var re; str; replacement)` / `(…; blk)` | Template form, or a callback computing each replacement. |
| `regex_split(var re; str) : array<string>` | Split on every match. |

Group `0` is the whole match; named groups are `(?P<name>...)`.

```das
let rewritten = regex_replace(re, src, "$1=$2")
let upper = regex_replace(re, src) <| $(matched) { return to_upper(matched) }
```

Template references: `$0` / `$&` the whole match, `$1`–`$9` numbered groups, `${name}` named,
`${N}` any index, `$$` a literal `$`. **The braces of the `${...}` forms must be escaped in das
source** — `"$\{name\}"` — or the `{` starts an interpolation and the group name resolves as a
variable.

| Feature | Syntax |
|---|---|
| Anchors | `^` `$` |
| Classes | `\w` `\W` `\d` `\D` `\s` `\S` `\t` `\n` `\r` `\f` `\v`; sets `[a-z]`, `[^abc]`, `[\d_]` |
| Quantifiers | greedy `+` `*` `?` `{n}` `{n,}` `{n,m}`; lazy `+?` `*?` `??` `{n,m}?` |
| Groups | `(...)`, non-capturing `(?:...)`, named `(?P<name>...)` |
| Lookahead, word boundary | `(?=...)`, `(?!...)`; `\b`, `\B` |
| Alternation, hex, escapes | `\|`; `\xHH`; `\.` `\+` `\*` `\(` `\[` `\^` `\{` `\\` |
| Any char | `.` — not `\n` unless `dot_all` |

- **`regex_match` is anchored, not a substring search** — use `regex_search` to find anywhere.
- **Every entry point short-circuits on the empty string.** A pattern that legitimately matches
  `""` (`^$`, `a*`) still reports no match against `""`; handle empty input in the wrapper.
- **ASCII only** — 256-bit char sets, no Unicode classes; `case_insensitive` folds A–Z only.
- **Catastrophic backtracking is possible.** Anchor with `^`, prefer non-capturing groups, avoid
  nested `*` / `+`.
