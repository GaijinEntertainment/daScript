# Utils Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture
doc: `CLAUDE.md` (repo root).

A tool directory is a direct child of `utils/` (or `utils/internal/`) with a runnable `.das`
entry point (`main.das`; das-fmt's `dasfmt.das`) — the C++ binary source dirs (`daslang`,
`gen1-to-gen2`; their binaries are documented under `bin/`) and the shared library `common` are
not tool directories. A file belonging to a tool whose directory carries its own `REVIEW.md`,
wherever the diff puts it, answers to that tool's checklist too.

**The folder is the audience decision: a tool directory outside `utils/internal/` has an
`install(...)` rule; one under `utils/internal/` has none.** There is no third state — a tool
that should not ship moves under `utils/internal/`, never "stays but uninstalled".

**No file under `utils/` outside `internal/` references a path under `utils/internal/` — by
`require`, include, or command line.** Internal tools may reference external ones
(`utils/common` is the shared hub); the reverse direction ships a dangling reference. The
installed-bundle side is machine-checked (`ci/smoke_test_bundle.sh`, "no utils/internal
references"); weakening that check is a defect.

**A tool directory name a diff adds or changes is kebab-case: `like-this`, never `like_that`
or `likeThis`.**

**A tool outside `utils/internal/` lands its page under `doc/source/reference/utils/`, its row
in the `doc/source/reference/utils.rst` toctree, and its row in `install/CLAUDE.md`'s layout
list in the same change that creates or renames it.**

**A tool added to or removed from `DAS_UTILS_SHIPPED_EXES` (`utils/CMakeLists.txt`) records the
decision in that tool's own `REVIEW.md` (for `dastest`, `dastest/REVIEW.md` at the repo root)
and updates `install/CLAUDE.md`'s `bin/` line in the same change.**
