# Modules Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`../CLAUDE.md`.

**Some module folders carry their own `REVIEW.md`; a diff applies the `REVIEW.md` of every folder
between this one and each file it changes.** A `.das` or `.cpp` under this folder but outside
`modules/<M>/` that binds or requires only the modules of `<M>` - a `bind_*.das` under
`modules/dasClangBind/bind/` - answers to `modules/<M>/REVIEW.md` as well. A `[test]` file under
`modules/<M>/` also answers to `modules/<M>/tests/REVIEW.md` where that checklist exists.

**A diff that changes a `.das` file which declares a GPU kernel - a function carrying
`[metal_kernel]`, `[spirv_kernel]`, or an annotation from `dasSpirv`, `dasVulkan` or `dasMetal`
whose name ends in `_shader` - or a `.das` file such a file requires, directly or through other
requires, applies
`REVIEW_SHADER_EMITTERS.md` (beside this file) together with its own folder's checklist.**

**A diff that changes a `.das` under this folder that the full `test_aot` binary compiles - a
file in a `*_AOT_FILES` list of its module's `CMakeLists.txt` or an `AOT_*_MODULE_FILES` list of
`tests/aot/CMakeLists.txt` (repo root), or a file such a file requires, directly or through other
requires - and not marked `options no_aot`, states in the PR body that the full `test_aot` lane
(`preflight --full`, or a manual dispatch of `build.yml` on the branch) ran green on the diff's
head commit.** Per-PR CI compiles only the `tests/language` AOT subset.

**In its own hand-written `initDependencies`, a C++ module calls `Module::require("<name>")` for
every in-tree module its CMake target links, and calls `initDependencies()` on each module that
call returns - in the same change as the link.** A module no other module requires is left
unloaded, and the linking module's imports are resolved before any of its code runs, so its next
load fails on the missing sibling.

**A diff that makes a `dasClangBind`-generated binding depend on another in-tree module declares
that dependency in the module's binder - its `bind_*.das`, under `modules/<M>/bind/` or
`modules/dasClangBind/bind/` - `require_modules` when the binding uses the other module's types,
`require_load_modules` when only the shared library links against the other's - never by hand in
the generated file.** The binder emits `initDependencies` from those two lists, and
`require_modules` puts the other module's types into this module's type library, so both modules
binding the same C++ types resolve to one copy.

**A diff that hand-binds a function in a module's own C++ - a function the binder of the module
(its `bind_*.das` under `modules/<M>/bind/` or `modules/dasClangBind/bind/`) would otherwise
generate - makes that binder's `skip_function` override return true for the function, and deletes
the function's generated registration from the module's `src/`, in the same change.** Otherwise
the generated registration sits beside the hand-bound one and the module registers the name twice.
