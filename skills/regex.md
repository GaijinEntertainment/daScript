# Regular expressions in daslang

## When to use this skill

Read this skill when writing regular expressions in `.das` code — anywhere you reach for `regex_compile`, `regex_match`, `regex_search`, the `%regex~...%%` reader macro, or any of the `regex_replace` / `regex_split` / `regex_foreach` family.

## API

Two modules: `daslib/regex` (the engine, all runtime functions) and `daslib/regex_boost` (the compile-time `%regex~...%%` reader macro).

```daslang
require daslib/regex
require daslib/regex_boost
```

### Compiling

```daslang
def regex_compile(expr : string; case_insensitive : bool = false; dot_all : bool = false) : Regex
def regex_compile(var re : Regex; expr : string; case_insensitive : bool = false; dot_all : bool = false) : bool
def is_valid(var re : Regex) : bool
```

For literal patterns, prefer the **reader macro** over runtime compilation:

```daslang
var private RE <- %regex~^\s*(?:def|struct)\s+private\b%%
var private RE_I <- %regex~hello%%i        // case-insensitive
var private RE_S <- %regex~start.*end%%s   // dot-all (`.` matches `\n`)
var private RE_IS <- %regex~Foo%%is        // both flags
```

The macro compiles the pattern at parse time and embeds the resulting `Regex` directly into the AST. Three benefits: (1) no escape-doubling — write `\s+` not `"\\s+"`; (2) parse-time validation — bad patterns surface as compile errors; (3) no per-call compilation cost. Module-scope `var` (mutable, since matching mutates internal state) is the standard pattern — see `daslib/cpp_gen.das:810` and `daslib/faker.das:70` for production use.

### Matching

```daslang
def regex_match(var regex : Regex; str : string; offset : int = 0) : int
```
Anchored at `offset` — checks "does the regex match starting here?". Returns the **length** of the match in bytes consumed, or `-1` if no match. Pattern starting with `^` and `regex_match(re, line)` is the common idiom for "does this line begin with...".

```daslang
def regex_search(var regex : Regex; str : string; offset : int = 0) : int2
```
Scans forward — returns `int2(start, end)` of the first match, or `int2(-1, -1)` if not found. Use this for substring search.

```daslang
def regex_match_all(var regex : Regex; str : string) : array<range>
```
Returns every match position as an `array<range>` (each `range.x` = start, `range.y` = end+1).

### Group access

```daslang
def regex_group(regex : Regex; index : int; match : string) : string
def regex_group_by_name(regex : Regex; name : string; str : string) : string
def operator [](regex : Regex; index : int) : range
def operator [](regex : Regex; name : string) : range
```

Group `0` is the whole match; numbered groups follow parenthesization order. Named groups via `(?P<name>...)`.

### Iteration and replacement

```daslang
def regex_foreach(var regex : Regex; str : string; blk : block<(at : range) : bool>)
def regex_replace(var regex : Regex; str : string; replacement : string) : string
def regex_replace(var regex : Regex; str : string; blk : block<(matched : string) : string>) : string
def regex_split(var regex : Regex; str : string) : array<string>
```

Block form lets you compute each replacement; template-string form handles the common case:

```daslang
let rewritten = regex_replace(re, src, "$1=$2")              // template
let rewritten = regex_replace(re, src) <| $(matched) {       // callback
    return upper(matched)
}
```

### Template replacement references

| Token | Meaning |
|---|---|
| `$0` or `$&` | the whole match |
| `$1`–`$9` | numbered capture groups |
| `${name}` | named capture group |
| `$$` | literal `$` |

## Syntax (this engine)

Pure NFA implementation in `daslib/regex.das` — not PCRE, not POSIX. Supports:

| Feature | Syntax |
|---|---|
| Anchors | `^` (start), `$` (end) |
| Character classes | `\w` `\W` `\d` `\D` `\s` `\S` `\t` `\n` `\r` `\f` `\v` |
| Quantifiers (greedy) | `+` `*` `?` `{n}` `{n,}` `{n,m}` |
| Quantifiers (lazy) | `+?` `*?` `??` `{n,m}?` |
| Capturing group | `(...)` |
| Non-capturing group | `(?:...)` |
| Named group | `(?P<name>...)` |
| Lookahead | `(?=...)` (positive), `(?!...)` (negative) |
| Word boundary | `\b` (boundary), `\B` (non-boundary) |
| Char set | `[a-z]`, `[^abc]`; meta-classes inside: `[\d_]` |
| Alternation | `|` (use `\|` for a literal pipe) |
| Hex escape | `\xHH` |
| Metacharacter escape | `\.` `\+` `\*` `\(` `\)` `\[` `\]` `\|` `\\` `\^` `\{` `\}` |
| Any char | `.` (matches anything but `\n` unless `dot_all=true`) |

## Gotchas

- **`regex_match` is anchored, not substring.** `regex_match(re, "abc")` only matches at offset 0; for "find anywhere" use `regex_search`.
- **ASCII-only.** The engine's char sets are 256-bit; no Unicode classes, no Unicode-aware case folding. `case_insensitive=true` only folds ASCII A–Z.
- **Pure NFA.** Catastrophic backtracking is possible on poorly-anchored patterns with nested quantifiers — anchor with `^`, use atomic-style non-capturing groups, and avoid nested `*`/`+` whenever possible.
- **Escape doubling in plain string literals.** `regex_compile("\\d+")` versus `%regex~\d+%%` — reader macro wins for literal patterns. Reach for runtime `regex_compile` only when the pattern is constructed dynamically.
- **`Regex` is mutable.** Match functions take `var regex : Regex` and mutate scratch state. Module-scope `var` (not `let`) for shared compiled patterns.

## Idiomatic example

From `utils/hygiene/rule_private_docs.das`:

```daslang
require daslib/regex public
require daslib/regex_boost

var private HEADER_RE <- %regex~^\s*(?:def|struct|enum|variant|class)\s+private\b%%
var private TRAILING_RE <- %regex~\s*//!<?[^\n]*$%%

def is_private_decl_start(line : string) : bool {
    return regex_match(HEADER_RE, line) >= 0
}

def strip_trailing_doc(line : string) : string {
    let pos = regex_search(TRAILING_RE, line)
    return pos.x < 0 ? line : slice(line, 0, pos.x)
}
```

## Reference

- Engine: `daslib/regex.das`
- Compile-time reader macro: `daslib/regex_boost.das`
- Tutorial: `doc/source/reference/tutorials/31_regex.rst`
- Generated stdlib reference: `doc/source/stdlib/generated/regex.rst`, `regex_boost.rst`
