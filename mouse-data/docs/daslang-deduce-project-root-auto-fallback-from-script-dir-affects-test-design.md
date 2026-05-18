---
slug: daslang-deduce-project-root-auto-fallback-from-script-dir-affects-test-design
title: Why does daslang.exe auto-resolve daspkg modules without an explicit `-project_root` when the script lives inside the project root?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**`daslang.exe` auto-deduces `-project_root` from `files.front()`'s directory** when the flag isn't passed explicitly. See `utils/daScript/main.cpp:379` `deduce_project_root()`:

```cpp
static string deduce_project_root(string maybe_project_root, string compile_file) {
    if (!maybe_project_root.empty()) return maybe_project_root;       // explicit wins
    if (!projectFile.empty()) { /* try .das_project's getDynModulesFolder() */ }
    if (!compile_file.empty()) {                                      // fallback: script's dir
        auto filename_start = compile_file.find_last_of("\\/");
        return filename_start != string::npos
            ? compile_file.substr(0, filename_start)
            : "./";
    }
    return "";
}
```

Used at `main.cpp:738`: `project_root = deduce_project_root(project_root, files.front())` → fed into `require_dynamic_modules(access, getDasRoot(), project_root, tout)` → which scans `<project_root>/modules/*/.das_module`.

**Implication for MCP project_root testing**: which file is `files.front()`?

| Spawn path | files.front() | Auto-deduces project_root from |
|---|---|---|
| `daslang.exe consumer.das` (direct) | `consumer.das` | consumer's own dir |
| `daslang.exe subtool.das -- consumer.das` (MCP subprocess) | `subtool.das` | `utils/mcp/subtools/` (no `modules/` there) |
| `do_run_script` (spawns `daslang.exe consumer.das` directly) | `consumer.das` | consumer's own dir |

So for an MCP test fixture at `utils/mcp/tests/_pretend_root/consumer.das`:
- MCP subtool tests (`compile_check`, `lint`, etc.) → spawn `daslang.exe subtool.das -- consumer.das` → auto-deduce uses `utils/mcp/subtools/` → no `modules/` → `pretend_mod` unregistered → compile fails without explicit `-project_root`. ✓ TESTABLE.
- `do_run_script(consumer.das, ...)` → spawns `daslang.exe ... consumer.das` directly → auto-deduce uses `_pretend_root/` → finds `modules/pretend_mod/` → resolves WITHOUT explicit project_root. ✗ FALSE POSITIVE.

**Workaround for testing `do_run_script`**: use inline code mode (`do_run_script("", code_string, ...)`) — the temp stub is written to `get_das_root()` whose `modules/` tree is daslang's own (no pretend_mod). Then the negative case fails as expected.

Same gotcha for `do_eval_expression` — its temp stub also lands in `get_das_root()`, so the auto-deduce doesn't help and explicit project_root is required.

Surfaced 2026-05-18 during Tier 1 MCP project_root test design. Affects ANY test fixture that places the consumer file inside the project_root subtree.

## Questions
- Why does daslang.exe auto-resolve daspkg modules without an explicit `-project_root` when the script lives inside the project root?
- How does `deduce_project_root` interact with MCP test fixtures?
- Why does `do_run_script` succeed against my pretend-daspkg fixture without project_root, but `do_compile_check` fails?

## Questions
- Why does daslang.exe auto-resolve daspkg modules without an explicit `-project_root` when the script lives inside the project root?
