# Modules Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.**

**A diff to a function carrying `[metal_kernel]`, `[spirv_kernel]`, or an annotation whose name
ends `_shader` and comes from `dasSpirv`, `dasVulkan` or `dasMetal`; to any `def` such a
function reaches directly or through another; or to a class whose own or inherited method carries
`[metal_kernel]` or `[spirv_kernel]`, or to a class such a class inherits from, member
declarations included, or to a struct a `@push_constant` member of either class names - applies
`REVIEW_SHADER_EMITTERS.md` (beside this file) together with its own folder's checklist.**

**A C++ module whose CMake target links another in-tree module's target calls
`Module::require("<name>")` for it, and the returned module's `initDependencies()`, in its own
`initDependencies`, in the same change.** A module no other module requires is left unloaded, and
the loader resolves an importer's imports before any of its code runs, so the importer's next
load fails on the missing sibling.

**A module whose `dasClangBind`-generated binding depends on another in-tree module declares
that dependency in its `bind_*.das` - `require_modules` when the binding uses the other module's
types, `require_load_modules` when the module's shared library links against the other's library
and the binding uses none of its types.** `require_modules` puts the other module's types into this
module's type library, so both modules binding the same C++ types resolve to one copy.

**Never hand-write a module dependency into a `dasClangBind`-generated file - declare it in the
module's `bind_*.das` instead.** The binder emits `initDependencies` from `require_modules` and
`require_load_modules`.
