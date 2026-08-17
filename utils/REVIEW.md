# Utils Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first — its contract binds this checklist.** Architecture doc:
`CLAUDE.md` (repo root).

**The folder is the audience decision: a tool directory outside `utils/internal/` has an
`install(...)` rule; one under `utils/internal/` has none.** There is no third state — a tool
that should not ship moves under `utils/internal/`, never "stays but uninstalled".

**No file outside `utils/internal/` requires a file under it.** Internal tools may require
external ones (`utils/common` is the shared hub); the reverse direction ships a dangling
reference.

**Weakening the `utils/internal/` path ban in `ci/check_shipped_skills.py` is a defect.** It is
what keeps shipped skills from pointing SDK users at tools the bundle does not carry.

**A new tool directory name is kebab-case: `like-this`, never `like_that` or `likeThis`.**

**A tool outside `utils/internal/` is registered where SDK readers look: a page under
`doc/source/reference/utils/`, its row in the `doc/source/reference/utils.rst` toctree, and its
row in `install/CLAUDE.md`'s layout list. A tool under `utils/internal/` appears in none of
them** — internal tools document themselves in their README and `skills/internal/`.

**A tool added to or removed from `DAS_UTILS_SHIPPED_EXES` (`utils/CMakeLists.txt`) records the
decision in that tool's own `REVIEW.md` in the same change.**
