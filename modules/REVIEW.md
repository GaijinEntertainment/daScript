# Modules Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: none at this root - each module's own `ARCHITECTURE*.md`.

**A diff touching any of the following applies `REVIEW_SHADER_EMITTERS.md` (beside this file)
together with its own folder's checklist:**
- a function carrying `[metal_kernel]`, `[spirv_kernel]`, or an annotation whose name ends
  `_shader` and comes from `dasSpirv`, `dasVulkan` or `dasMetal`;
- any `def` such a function reaches, directly or through another;
- a class whose own or inherited method carries `[metal_kernel]` or `[spirv_kernel]`, member
  declarations included;
- a class such a class inherits from, member declarations included;
- a struct a `@push_constant` member of either class names.

**In its own `initDependencies`, a C++ module calls `Module::require("<name>")` for every in-tree
module its CMake target links, and calls `initDependencies()` on each module that call returns -
in the same change as the link.** A module no other module requires is left unloaded, and the
linking module's imports are resolved before any of its code runs, so its next load fails on the
missing sibling.

**A module whose `dasClangBind`-generated binding depends on another in-tree module declares
that dependency in its `bind_*.das` - `require_modules` when the binding uses the other module's
types, `require_load_modules` when the module's shared library links against the other's library
and the binding uses none of its types.** `require_modules` puts the other module's types into this
module's type library, so both modules binding the same C++ types resolve to one copy.

**Never hand-write a module dependency into a `dasClangBind`-generated file - declare it in the
module's `bind_*.das` instead.** The binder emits `initDependencies` from `require_modules` and
`require_load_modules`.

**A diff that hand-binds a function in a module's own C++ - a function the module's
`bind_*.das` binder would otherwise generate - makes that binder's `skip_function` override
return true for the function, and deletes the function's generated registration from the
module's `src/`, in the same change.** Otherwise the generated registration sits beside the
hand-bound one and the module registers the name twice.
