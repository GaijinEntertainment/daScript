# daslang repository architecture

This document follows `ARCHITECTURE_COMMON.md`. It holds what is true of the tree as a whole;
a folder's own mechanisms live in that folder's architecture doc. `CLAUDE.md` holds the
authoring rules that lean on these facts, and `REVIEW.md` the rules a diff is reviewed against.

## The document system {#document-system}

Two document types split one grammar, each with a repo-root contract. A `REVIEW.md` holds the
bans and duties that bind a diff; the folder walk over a diff's changed files finds the
checklists that apply. An `ARCHITECTURE.md` holds present-tense statements: how the folder's
system is and why. `REVIEW_COMMON.md` and `ARCHITECTURE_COMMON.md` are the two contracts.

An implementation folder - `daslib/`, a module's `daslib/` or engine folder - holds no `.md`
besides `REVIEW*.md` and `ARCHITECTURE*.md`; the `rule_docs_only` key of `.lint_config` is the
lint that enforces it. Ledgers live at the module root.

No `REVIEW*.md`, `ARCHITECTURE*.md` or ledger installs; the SDK bundle gate refuses a bundle
that carries one. `REVIEW_COMMON.md` and `ARCHITECTURE_COMMON.md` at the install root are the
two vendoring exceptions. A shipped tree with `[arch]` citations installs a generated excerpt
of each cited document instead: the cited sections and the sections their own text cites,
nothing else, marked with a banner. `utils/internal/arch-extract` writes the excerpt for each
tree registered on the `DAS_ARCH_EXTRACT_SPECS` global property. The excerpts keep LINT026
armed in an installed SDK.

LINT026 checks citations in every folder; no tag arms it. It fails a citation that names no
such file or anchor, a `.das` citation that reaches a document outside the citing file's
folder tree, a `.das` citation that resolves but sits inside a comment - it annotates nothing -
and an anchor nothing cites. LINT027 fails a `REVIEW*.md` or `ARCHITECTURE*.md` past
300 lines, wherever it sits. An architecture doc splits into `ARCHITECTURE_<CONCERN>.md`
companions, and a checklist into `REVIEW_<KIND>.md` companions its opening routes to.

## Folder charters {#folder-charters}

Most of the layout is what `ls` shows. These are the folders and files whose role it does not
show.

- `skills/daslang/` - the distributable daslang language skill for third-party AI agents:
  `SKILL.md` plus `references/`. It needs no SDK and is not a repo task skill. Every example in
  it is probe-verified, and its `README.md` carries its editing rules.
- `daslib/aot_cpp.das` - the AOT C++ emitter. It is daslang, not C++.
- `nano/` - `libDaScriptNano`, the runtime with no compiler in it. It is a shadow include root
  whose four headers win over `include/`, so thirteen `src/` sources compile against a minimal
  `Context` unmodified. `nano/REVIEW.das` checks that every directory linking it clears its
  inherited include dirs. Mechanisms: `nano/ARCHITECTURE.md`.
- `tests/aot/CMakeLists.txt` - the registry of test directories compiled ahead of time. It
  builds two binaries. `test_aot_subset` covers `tests/language` only and is part of ALL: the
  per-PR build lane builds and runs it, with cached results. The full `test_aot` is
  `EXCLUDE_FROM_ALL`, about 1080 translation units; the nightly CI and `preflight --full` build
  it through `--target test_aot` or `run_tests_aot`.
- `dastest/` - the test framework. `tests/` uses it, and so do external repos.
- `utils/detect-dupe/` - the in-repo duplicate-function finder. `utils/find-dupe/` - the Claude
  judge over its report; it needs `daspkg install --root utils/find-dupe` and
  `ANTHROPIC_API_KEY`. Both are also MCP tools.
- `utils/mcp/`, `utils/daslang-live/`, `utils/daspkg/` - in-tree tools; most have a skill.
- `tutorials/language/` - the language tour. `tutorials/<area>/` - one folder per area.

## Indexing lives in five tiers {#indexing-tiers}

Array, dim and vector indexing is implemented once per execution tier. Bounds checks,
negative-index detection and width-aware bounds exist in each:

- AOT C++ - `include/daScript/simulate/aot.h`.
- The interpreter, non-fused - `include/daScript/simulate/runtime_array.h` and
  `simulate_nodes.h`.
- The interpreter, fused - `src/simulate/simulate_fusion_at_array.cpp` and
  `simulate_fusion_at.cpp`.
- The JIT - `modules/dasLLVM/daslib/llvm_jit.das`.
- AST const-fold - `src/ast/ast_simulate.cpp`.

A Debug build bypasses the fused permutations. A fix on the fused path alone therefore passes
every Release lane and fails the Debug ARM lane.

## The JIT `.dll` cache key {#jit-cache}

The JIT caches the `.dll` it emits. The cache hash folds `LLVM_JIT_CODEGEN_VERSION`
(`modules/dasLLVM/daslib/llvm_jit_plan.das`) and the AOT hash of each function. An AST-level
change moves a function's hash and invalidates its entry by itself; an emitter or ABI change
moves no hash, so only a new version number invalidates the cache.

## The MCP server {#mcp-server}

`utils/mcp/main.das` is the daslang MCP server. It exposes compiler diagnostics, program
introspection and live-reload control. Its tool table, the live-API caveats and the `.mcp.json`
configuration are in `skills/mcp_tools.md`.

`.mcp.json`, `sgconfig.yml`, `bin/` and the tree-sitter grammar library are all gitignored, so
a fresh worktree or clone has no daslang MCP tools. `utils/mcp/setup.das` writes them from any
existing daslang binary.

The installed SDK's server runs with no `-project`, so it is self-contained: every `.das`
reachable from `main.das` is installed. `tools/` and `subtools/` install by glob; the
top-level files are listed one by one in an `install(FILES ...)` block in `CMakeLists.txt`. A
reachable file that list lacks stops the SDK's server at startup with `error[20605] missing
prerequisite '<mod>'`, while the in-tree server keeps working.
