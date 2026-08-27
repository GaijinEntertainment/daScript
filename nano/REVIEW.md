# nano Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**Never edit a source to make it build under `NANO_SHARED_SRC` - split the file in the full
runtime's tree instead, moving its runtime half into a file of its own.** An edited copy is a
fork that drifts silently.

**Never use a name from `daScript/ast/ast.h` beyond `Annotation`, `TypeAnnotation` and
`Module::name` in a source nano compiles - its own under `src/` here and the reused ones under
`src/` (repo root).** Any other name compiles here and means something different than it does
in the full runtime.

**Never add a stub to `src/nano_stubs.cpp` that returns a default and continues - implement
the behaviour or stop the program instead.** On a target with no debugger attached, a default
return turns a missing feature into a wrong answer.

**Weakening the include-clear check in `REVIEW.das` (beside this file) is a defect.**

**Never add a name to `include/daScript/simulate/simulate.h` unless a source nano compiles
unchanged from the full runtime, or generated code, already refers to it.** This header is a
subset, not a copy: a name added for later use is one nobody will know to remove.

**Never rename a member of this folder's `Context` or change its type - it keeps the name and
the type it has in `include/daScript/simulate/simulate.h` at the repo root.** Generated code
and the reused headers are written against those names, so a rename is a compile error at best
and a different field at worst.

**A diff that changes what nano leaves out also updates the tier table in `ARCHITECTURE.md`
and the "What it leaves out" list in `README.md`, in the same change.** Both are read by
embedders deciding whether their script fits.

**A diff that adds a tier (a group of script features nano supports as a unit) or a
fail-closed seam (a place where nano stops the program instead of returning a default) also
adds an example under `examples/standalone/` and a case in
`tests-cpp/big/nano_ctx/test_nano_ctx.cpp`, in the same change.** A tier with no program
linking it stops working with nothing turning red.

**Never put an estimated number in `README.md` - measure it, and name in the table the
toolchain and the targets that produced it.** A reader reproduces the number from those two
facts.
