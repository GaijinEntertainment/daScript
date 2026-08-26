# nano Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`ARCHITECTURE.md`.

**A shared source added to `NANO_SHARED_SRC` compiles with no edit to the file itself.**
A source that needs an edit to build here is split upstream instead - its runtime half moved
into a file of its own - because an edited copy is a fork that drifts silently.

**A source under `src/` that nano compiles must not include `daScript/ast/ast.h` for anything
beyond `Annotation`, `TypeAnnotation` and `Module::name`.** Those three are all nano's
`include/daScript/ast/ast.h` provides; anything else compiles here and means something
different than it does upstream.

**A new stub in `src/nano_stubs.cpp` either implements the behaviour or stops the program.**
A stub that returns a default and continues turns a missing feature into a wrong answer on a
target with no debugger attached.

**A directory holding a target that links `libDaScriptNano` clears its inherited include
directories.** CMake searches a directory's `include_directories()` before any target's own,
so without `set_property(DIRECTORY PROPERTY INCLUDE_DIRECTORIES "")` the target compiles
against the full runtime's headers while linking nano's library.

**A name added to `include/daScript/simulate/simulate.h` here is a name something in the reuse
set or in generated code refers to.** This header is a subset, not a copy: a name added
speculatively is one nobody will know to remove.

**A member kept in this folder's `Context` keeps the name and type it has in
`include/daScript/simulate/simulate.h` at the repo root.** Generated code and the reused
headers are written against those names, so a renamed member is a compile error at best and a
different field at worst.

**A change to what nano leaves out updates the tier table in `ARCHITECTURE.md` and the
"What it leaves out" list in `README.md`.** Both are read by embedders deciding whether their
script fits.

**A new tier or a new fail-closed seam ships an example under `examples/standalone/` and a
case in `tests-cpp/big/nano_ctx/test_nano_ctx.cpp`.** A tier with no linked program is a tier
that stops working without anything turning red.

**A number in `README.md` was measured, not estimated.** The table says which toolchain and
which targets produced it, so a reader can reproduce it.
