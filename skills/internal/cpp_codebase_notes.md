# C++ codebase notes (repo-only)

Read this when working on daslang's own C++ rather than embedding it. The binding and
embedding API is `skills/cpp_integration.md`; this file is where things live in the tree
and how to search them.

## Where things live

| What | Where |
|---|---|
| Type inference | `src/ast/ast_infer_type.cpp` + `include/daScript/ast/ast_infer_type.h` |
| Builtin runtime functions | `src/builtin/module_builtin_runtime.cpp` |
| Error codes (10005–50640) | `include/daScript/ast/compilation_errors.h` |
| Lexer / parser | `src/parser/ds2_lexer.lpp` / `src/parser/ds2_parser.ypp` (grammar truth) |
| Operator-name validation | `isValidBuiltinName(name, canPunkt)` in `src/ast/ast_module.cpp` — characters only, no whitelist |
| `TextPrinter` in anger | `src/misc/job_que.cpp` (`JobStatus::DumpJobQueLeaks`), `include/daScript/ast/ast_handle.h` (`dumpHandleLeaks<T>`) |

## AST function flags

- `func.flags.isClassMethod` — struct/class method, set after inference
- `func.moreFlags.isStaticClassMethod` — `def static`; name is ``StructName`methodName``
  and self is an explicit first argument
- Non-static methods keep an unqualified name (`finalize`, `[]`); self is added implicitly
  during inference
- `func.moreFlags.propertyFunction` — property accessor, name starts with `` .` ``
- `func.classParent` — the struct/class owning the method

## Diffing interpreter against AOT

`options log_nodes` output from `daslang` and from `test_aot` is comparable line for line;
divergent constant-folded values are how the `strstr("","")` null-vs-`""` bug was found.
The AOT harness itself (`test_aot`, `tests/aot` registration, `libDaScriptAot`) is
`skills/internal/aot_testing.md`.

## Searching C++ — MCP over grep

The daslang MCP server's C++ tools are parse-aware (ast-grep + tree-sitter-cpp) and
pre-scoped to `src/` `include/` `modules/`, so they skip vendored and submodule noise:

- `cpp_find_symbol` — declarations by name and kind
- `cpp_grep_usage` — usages across `.cpp/.h/.hpp/.cc`
- `cpp_outline` — top-level declarations of a file
- `cpp_goto_definition` — ranked best-effort definition candidates
- `cpp_compile_check` / `cpp_build_info` — syntax-check a TU / print its exact compile
  command, off `compile_commands.json`

Fall back to `Grep` for string literals, comments, and non-C++ files. Full table and
configuration: `skills/mcp_tools.md`.
