# Modules Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**

**A diff to any kernel body or fixture the `dasMetal` or `dasSpirv` emitter compiles applies
`REVIEW_SHADER_EMITTERS.md` (beside this file) together with its own folder's checklist.**

**A C++ module whose CMake target links another in-tree module's target calls
`Module::require("<name>")` for it, and the returned module's `initDependencies()`, in its own
`initDependencies`, in the same change; a link without that call is a defect.** The loader
records a module nothing required as deferred and resolves an importer's imports before any of
its code runs, so an unrequired sibling fails the importer's next load.

**A module whose binding `dasClangBind` generates declares that requirement in its `bind_*.das` -
`require_modules` when the binding uses the other module's types, `require_load_modules` when
only the library import needs it - and the binder emits `initDependencies` from the lists; a hand
edit of the generated file alone is a defect.** `require_modules` adds the other module to this
module's type library, so a module that binds the same C++ types twice resolves the fields to
the other's copies.
