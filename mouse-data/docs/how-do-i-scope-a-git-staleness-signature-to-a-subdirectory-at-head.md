---
slug: how-do-i-scope-a-git-staleness-signature-to-a-subdirectory-at-head
title: How do I scope a git-tracked staleness signature to a single subdirectory so commits outside it don't invalidate the cache?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

`git rev-parse HEAD:<repo_rel_dir>` returns the **tree object hash** for that subdirectory at HEAD. Fold it into your staleness signature instead of the global `git rev-parse HEAD`:

- **Stable** when only files outside the subtree change at HEAD (commits/branch switches that don't touch your indexed paths).
- **Changes** when any tracked file under the subtree changes at HEAD.

For the signature to also pick up pending uncommitted changes, combine with filtered `git status --porcelain --untracked-files=normal` — keep only lines whose extracted path is under one of your search_dirs.

Why per-tree HEAD matters: global `git rev-parse HEAD` flips on every commit anywhere in the repo. In a monorepo, every `git pull` or branch switch invalidates every cache that uses it, even when nothing under the cache's scope changed. Per-tree HEAD avoids that whole class of spurious rebuilds.

**The pattern lives in `utils/common/git_signature.das:compute_signature`** (added in PR #2621). Both `utils/mouse/index.das` (markdown docs index) and `utils/mcp/tools/cpp_common.das` (C++ source-search index) use it:

```daslang
require ../../common/git_signature.das

let core = compute_signature(
    root,                                                // any dir inside the repo
    @(p : string) => ends_with(p, ".md"),                // file-class predicate
    [docs_abs])                                          // indexed dirs (absolute)
```

Edge cases handled in the shared module:
- `git rev-parse HEAD:<dir>` errors when `<dir>` isn't tracked at HEAD (e.g., newly added) → push a placeholder; the next `git status` line catches new files.
- Empty `<dir_rel>` (search_dir == toplevel) → fall back to bare `HEAD`.
- Caller-passed `search_dirs_abs` order matters for the hash if not normalized → `dirs_rel` is sorted before folding.
- No git checkout at all → fall back to `compute_filesystem_signature` (recursive walk, sorted by path before hashing for cross-platform determinism).
- `git status --porcelain` paths are pre-narrowed to those under any search_dir, so the consumer's predicate only needs the file-class filter.

If you're building a NEW indexed cache under `utils/`, plug into the shared module rather than copying the git plumbing — the predicate is the only consumer-specific bit.

## Questions
- How do I scope a git-tracked staleness signature to a single subdirectory at HEAD?
- How do I avoid spurious rebuilds on `git pull` for caches scoped to a subtree?
- What does `git rev-parse HEAD:<path>` actually return?
- Where does the shared staleness-signature code live in this repo?
- How do both mouse and the cpp-search MCP tool detect index staleness?

## Questions
- How do I scope a git-tracked staleness signature to a single subdirectory so commits outside it don't invalidate the cache?
