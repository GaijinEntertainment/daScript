# Glob Pattern Matching

Read this skill before writing or reviewing any glob/wildcard pattern handling in `.das` code — file selection, include/exclude masks, pattern-match-on-paths, anything where `*` / `?` / `**` / `[abc]` appears in user input or a config.

## Two flavors — pick the right one

daslib ships **two** glob matchers. They look similar and both call themselves "glob", but they treat `/` differently. Confusing them is the most common mistake in this area.

| Function | Module | `*` crosses `/` | `**` | char classes | When to use |
|---|---|---|---|---|---|
| `match_glob(pattern, path)` | `daslib/fio` | **No** (stops at `/`) | **Yes** (crosses `/`) | `[abc]`, `[a-z]`, `[!abc]`, `[^abc]` | Path patterns: `assets/**/*.png`, `src/*.das`, `--exclude tests/**` |
| `glob_match(pattern, text)` | `daslib/strings_boost` | **Yes** | No | None | Single name component, or text where `/` is incidental |

**Default to `fio.match_glob`.** Reach for `glob_match` only when you are matching against a single filename or a string where slashes carry no structural meaning (e.g. tag names, channel names).

## Public surface (`daslib/fio`)

```das
def match_glob(pattern : string; path : string) : bool
def glob(root : string; pattern : string;
         blk : block<(filename : string; is_dir : bool) : void>)
def glob_filtered(root : string; includes, excludes : array<string>;
                  blk : block<(filename : string; is_dir : bool) : void>)
def is_glob_pattern(pattern : string) : bool
def to_generic_path(s : string) : string
def expand_glob(pattern : string; var result : array<string>)
def parse_file_list(file : string; var result : array<string>)
```

- `match_glob` — pure path-string match, no filesystem access. Both pattern and path use `/`.
- `glob` — recursive walk + match in one pass. Paths are normalized to `/` for matching AND for the callback, so `blk` receives stable `/`-separated paths regardless of host OS.
- `glob_filtered` — multi-pattern walk. An entry is yielded if it matches ≥1 include and 0 excludes. **Excludes win** on conflict. Callback receives `/`-normalized paths.
- `is_glob_pattern` — true if the string contains `*`, `?`, or `[`. Detects character classes that the inline `find(s, "*") >= 0 || find(s, "?") >= 0` check misses.
- `to_generic_path` — convert a path to use forward slashes regardless of host. Use when feeding `dir_rec` output (native separator) into `match_glob` directly, or anywhere else you need stable `/`-separated paths. **Not** the same as `fio.normalize`, which uses the platform-preferred separator (`\` on Windows).
- `expand_glob` — turn a single glob pattern (e.g. `src/**/*.das`) into a sorted list of matching file paths. Splits the literal directory prefix from the glob remainder, picks recursive (`glob`) vs shallow (`dir`) walk based on whether the remainder contains `**` or `/`, normalizes input via `to_generic_path` (so backslash patterns work), filters out directories, sorts the matches locally, and **appends** to `result`. The local sort + append is deliberate: it preserves the order of any plain entries pushed to `result` before the glob.
- `parse_file_list` — parse a comma- or newline-separated list of files / dirs / globs into a flat list of paths. Strips whitespace, skips empty entries, dispatches each entry through `is_glob_pattern`: literal entries pass through, glob entries go through `expand_glob`. Also **appends** to `result`. The canonical "user-supplied paths argument → expanded file list" expander used by every MCP tool with a `paths` / `file` argument; reach for it from any new CLI tool that takes file/glob input.

## Idioms

### Literal vs glob dispatch

When a tool accepts a user-supplied path:

```das
if (is_glob_pattern(input)) {
    var matches : array<string>
    glob(".", input) $(filename, is_dir) {
        if (!is_dir) { matches |> push_clone(filename) }
    }
    if (empty(matches)) {
        // glob with zero matches — WARN, continue
        to_log(LOG_WARNING, "pattern '{input}' matched no files\n")
    }
    // ... process matches ...
} else {
    if (!fexist(input)) {
        // literal that does not exist — ERROR, abort
        to_log(LOG_ERROR, "file '{input}' not found\n")
        return 1
    }
    // ... process literal ...
}
```

The "literal-missing = error, glob-no-match = warning" rule is the recommended convention for any tool taking user-supplied patterns. `daspkg release` enforces it; consider doing the same in new tools.

### Include + exclude pipelines

```das
var includes : array<string>
var excludes : array<string>
includes |> push_clone("**/*.das")
excludes |> push_clone("_build/**")
excludes |> push_clone("tests/**")
glob_filtered(root, includes, excludes) $(filename, is_dir) {
    if (is_dir) { return }
    // ... process filename ...
}
```

Excludes win — a file matching both an include and an exclude is dropped.

### Order preservation across plain + glob mixed input

`parse_file_list` and `expand_glob` already implement the right pattern: each glob expansion sorts **locally** and appends, so plain entries the user listed in a specific order stay where they were. Don't reach for a global `sort(result)` at the end of a mixed-input parse — it would scramble the user's intended order.

```das
require daslib/fio

var files : array<string>
parse_file_list("zzz.das,assets/**/*.png,aaa.das", files)
// files[0] == "zzz.das" — first plain stays first
// files[length(files) - 1] == "aaa.das" — last plain stays last
// the glob's matches sit between them, sorted within their slice
```

If you're rolling your own expansion (don't, unless `parse_file_list` doesn't fit), the same pattern: per-glob `sort()` + `append`, never a final global sort. See the regression test in [tests/fio/expand_glob_test.das](tests/fio/expand_glob_test.das) (`test_parse_file_list_order_preserved`).

## Performance and correctness gotchas

- **`peek_data("")` does NOT invoke its block.** This caught me when I first wrote `match_glob`. Empty-string inputs need explicit handling at the top of any function that processes pattern/path through `peek_data`. `fio.match_glob` does this; if you wrap `peek_data` over potentially-empty input elsewhere, do the same.
- **`*` does not cross `/` in `match_glob`.** Single-segment patterns only ever match single-segment paths. If you want recursion, use `**`.
- **`[` triggers a character class in `match_glob` / `glob_match`.** A literal filename containing `[` is rare but possible — if your tool may receive such inputs from users, decide explicitly whether to dispatch via `is_glob_pattern` (which treats `[` as "this is a glob") or to escape.
- **Pattern relativity:** `match_glob` matches the entire path against the entire pattern. Passing an absolute path against a relative pattern (or vice versa) silently fails. If you walk via `dir_rec` and then `match_glob` directly, the path you receive is already root-relative — fine. If you mix patterns and paths from different scopes, normalize first via `fio.relative_result`.
- **No brace alternation.** `{a,b}` is not supported in either flavor. If you need it, expand into multiple `glob_filtered` includes manually.
- **`glob` / `glob_filtered` walk recursively** (built on `dir_rec`). Don't call them per-file inside another walk; build the file list once.
- **`glob_filtered` does not prune excluded subtrees.** Excludes are evaluated per-entry, so `excludes=["node_modules/**", "_build/**"]` still descends into those directories — every file is yielded by `dir_rec` and then dropped by the filter. For huge excluded trees this is wasted I/O. If pruning matters, walk via `dir_rec` directly and short-circuit.
- **`**` in `match_glob` is greedy across `/` regardless of position.** `a**b` will match `a/x/b`, not just `a*b`. This differs from bash where `**` is only special as a complete path component. Our docs say "`**` matches across `/`" so this is consistent — but a subtle gotcha for users coming from bash. To get bash-like behavior write `a*b` (single `*`, doesn't cross `/`).
- **No backslash escape.** There is no `\\*` form to match a literal `*`. Use a character class instead: `[*]` matches `*`, `[?]` matches `?`, `[[]` matches `[`. POSIX-standard.

## Common mistakes

- **Reaching for `glob_match` from `strings_boost` when you want `**` semantics.** It does not have `**`. Switch to `fio.match_glob`.
- **Hand-rolling `path |> find(excludePat) >= 0`** as a substring filter when a glob would be safer. Substring filters are fragile: `_build` matches `my_build_system/`, `tests` matches `unit_tests/`. Use `glob_filtered` with proper patterns.
- **Inline `find(s, "*") >= 0 || find(s, "?") >= 0`** to detect "is glob" — misses `[abc]` character classes. Use `is_glob_pattern`.
- **Forgetting to normalize on Windows.** `dir_rec` returns native paths (`\` on Windows). `glob` and `glob_filtered` normalize internally and pass `/`-paths to your callback. If you call `match_glob` directly on a `dir_rec` result, normalize first via `to_generic_path` from `daslib/fio`. **Don't reach for `fio.normalize`** — it does the *opposite* (uses platform-preferred separator, `\` on Windows).

## Cross-references

- `skills/filesystem.md` — `fio` path/filename helpers (`base_name`, `dir_name`, `path_join`, `relative_result`).
- `skills/regex.md` — when you need full regex (lookarounds, named groups, repetition counts) instead of glob.
- `tutorials/language/54_glob.das` — runnable tutorial demonstrating all four functions and the two-flavor distinction.
- `doc/source/stdlib/generated/fio.rst` — generated stdlib reference for the fio module (search for "Glob and pattern matching").
