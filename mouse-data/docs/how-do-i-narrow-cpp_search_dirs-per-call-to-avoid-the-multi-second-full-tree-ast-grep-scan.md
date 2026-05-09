---
slug: how-do-i-narrow-cpp_search_dirs-per-call-to-avoid-the-multi-second-full-tree-ast-grep-scan
title: How do I narrow CPP_SEARCH_DIRS per call to avoid the multi-second full-tree ast-grep scan when looking up a single C++ symbol?
created: 2026-05-09
last_verified: 2026-05-09
links: []
---

The MCP `with_cpp_source` redirect (used by `find_symbol` and `goto_definition` to surface a builtin's C++ source location) walks a global C++ index built from `CPP_SEARCH_DIRS = [src, include, modules]` — about 947 files in daslang. First call costs ~2 s; subsequent calls reuse the cached index with a cheap signature recheck. But for an MCP tool that runs in a fresh subprocess per call, **every call rebuilds**, so each `with_cpp_source=true` lookup pays the full 2 s.

Fix: pass an optional `cpp_dirs` arg that scopes a fresh narrow scan, bypassing the global cache.

## API

`do_find_symbol` and `do_goto_definition` have an optional last param:

```das
def do_find_symbol(query : string; kind : string = ""; file : string = "";
                   project : string = ""; with_cpp_source : bool = false;
                   cpp_dirs : string = "") : string

def public do_goto_definition(file : string; line_str, col_str : string;
                              no_opt_str : string = ""; project : string = "";
                              with_cpp_source : bool = false;
                              cpp_dirs : string = "") : string
```

`cpp_dirs` is a comma-separated list of repo-relative paths. Empty -> use the cached global index (today's behavior, slow for cold subprocess). Non-empty -> fresh scoped scan via `cpp_lookup_by_name_scoped()` (in `utils/mcp/tools/cpp_common.das`), no global cache touched.

## Example

`print` is a builtin defined in `src/builtin/`. Narrowing the scan from full default (947 files) to just `src/builtin` (33 files):

```das
do_find_symbol("=print", "function", "", "", true, "src/builtin")
```

Test timing improved from 19 s to 1.7 s (10×) for `test_goto_definition_with_cpp_source`, and from 19 s to 3.4 s (5.7×) for `test_find_symbol_with_cpp_source`. Total `test_tools.das` suite: 132 s -> 89 s (33% faster).

## How the helper works

`cpp_lookup_by_name_scoped(cppName, dirs, var match)` does a fresh `cpp_run_scan(dirs, "", err)` over only the requested subdirs, then linear-scans the entries. No caching, but the scan is small enough it doesn't matter at this scale.

## When to use

- Tests that look up a known builtin where the source-file location is predictable (e.g. `print` in `src/builtin`, `addFunction` in `src/ast`).
- Production MCP calls where the user is searching for a specific known scope.

When NOT to use:
- General-purpose user queries — they want the full search scope.
- The first `with_cpp_source=true` call in a fresh subprocess gets the same per-call cost regardless; narrowing only pays off for repeated lookups in the same scope.

Verified 2026-05-09 in PR #2620.

## Questions
- How do I narrow CPP_SEARCH_DIRS per call to avoid the multi-second full-tree ast-grep scan when looking up a single C++ symbol?
