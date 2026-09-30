# Repository Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `CLAUDE.md`.

**Removing or weakening `REVIEW.das`'s check that no two git-tracked `.das` files declare the
same `shared` module name is a defect.**

**Outside deliberate negative compilation tests, a `TOLERATED_SHARED_TWINS` row's `why`
names what keeps every process from compiling two of its name's files; a `why` that names nothing, or that a build or test run contradicts, is a
defect.** The first file a process compiles that declares `module X shared` becomes that
process's module `X`, and every later file declaring or requiring `X` in the same process gets
that module - so any process compiling both gives the second file the first's module.

**A diff changing `getVersion()` in `include/daScript/ast/ast_serializer.h`
replaces its version comment instead of appending to it.** The comment describes the current
version rather than earlier versions. A diff that keeps the old note in a parenthesis is a defect; git carries the
history.
