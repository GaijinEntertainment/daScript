# Modules Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`../CLAUDE.md`.

**Some module folders carry their own `REVIEW.md`; a diff applies the `REVIEW.md` of every folder
between this one and each file it changes.** A `.das` or `.cpp` under this folder but outside
`modules/<M>/` answers to `modules/<M>/REVIEW.md` as well when it generates the C++ binding of a
module `modules/<M>/` registers, or when every module it requires, `daslib/` aside, is registered
under `modules/<M>/`. A `[test]` file under `modules/<M>/` also answers to
`modules/<M>/tests/REVIEW.md` where that checklist exists.

**A diff that changes a `.das` file declaring a function carrying an annotation whose name ends
in `_kernel` or `_shader` and that a module under this folder registers - or a `.das` file such
a file requires, directly or through other requires - applies `REVIEW_SHADER_EMITTERS.md` too.**

**A diff that changes a `.das` under this folder that the full `test_aot` binary compiles - a
file in a `*_AOT_FILES` list of its module's `CMakeLists.txt` or an `AOT_*_MODULE_FILES` list of
`tests/aot/CMakeLists.txt` (repo root), or a file such a file requires, directly or through other
requires - and not marked `options no_aot`, states in the PR body that the full `test_aot` lane
(`utils/internal/preflight/main.das` (repo root) run with `--full`, or a manual dispatch of
`.github/workflows/nightly.yml` (repo root) on the branch) ran green on the diff's head commit.**
Per-PR CI compiles only the `tests/language` AOT subset.

**A diff that links an in-tree module into the CMake target of a C++ module whose
`initDependencies` is hand-written calls `Module::require` with the linked module's registered
name in that `initDependencies`, and calls `initDependencies()` on the module that call returns,
in the same change.** A module no other module requires is left unloaded, so the linking
module's next load fails on the missing sibling.

**A diff that makes a `dasClangBind`-generated binding depend on another in-tree module declares
that dependency in the module's binder - the `bind_*.das` that generates the module's binding -
`require_modules` when the binding uses the other module's types, `require_load_modules` when
only the shared library links against the other's - never by hand in the generated file.** The
binder emits `initDependencies` from those two lists, and `require_modules` puts the other
module's types into this module's type library, so both modules binding the same C++ types
resolve to one copy.

**A diff that hand-binds a function in a module's own C++ - a function the `bind_*.das` that
generates the module's binding would otherwise generate - makes that binder's `skip_function`
override return true for the function, and deletes the function's generated registration from
the module's `src/`, in the same change.** Otherwise the generated registration sits beside the
hand-bound one and the module registers the name twice.
