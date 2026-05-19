---
slug: dasimgui-ffi-const-ref-return-and-decltype-name-collision
title: When binding a C++ helper to daslang via `addExtern<DAS_BIND_FUN(...)>`, what trips the runtime "missing WrapType implementation" error and the compile-time `decltype incorrect argument` error?
created: 2026-05-19
last_verified: 2026-05-19
links: []
---

**Two FFI gotchas that bit on dasImgui PR-D (`GetSortSpec` / `GetColumnSortDirection` helpers):**

## 1. `const T&` return type fails at RUNTIME with "missing WrapType implementation"

A bound C++ helper returning `const SomeStruct&` (reference to a non-workhorse type) compiles fine and links fine, but when invoked from daslang at runtime throws:

```
EXCEPTION: internal integration error, missing WrapType implementation or it's not included
```

The check is in `include/daScript/simulate/interop.h:146-152`:

```cpp
} else if constexpr ( !is_workhorse_type<Result>::value && is_same<WrapResult,Result>::value ) {
    // if the WrapType is the same as Result, we are missing WrapType implementation, or its not included
    context.throw_error("internal integration error, missing WrapType implementation or it's not included");
```

**Fix:** Return `const T*` (pointer) instead of `const T&` (reference). Pointers are always workhorse-handled, so no specialization needed.

```cpp
// BAD: runtime "missing WrapType implementation"
const ImGuiTableColumnSortSpecs & GetSortSpec ( ImGuiTableSortSpecs * specs, int idx ) {
    return specs->Specs[idx];
}

// GOOD: pointer return, runtime-safe
const ImGuiTableColumnSortSpecs * GetSortSpec ( ImGuiTableSortSpecs * specs, int idx ) {
    return &specs->Specs[idx];
}
```

Daslang side then uses `let s = GetSortSpec(p, i)` and gets `T const?`. Field access via `.` auto-derefs (`s.ColumnIndex` works fine).

## 2. `decltype(&das::Foo)` fails when `Foo` shadows a struct member name

`DAS_BIND_FUN` expands to `decltype(&fn), &fn`. If you name a free helper the same as a struct's member field, MSVC's `decltype(&das::SortDirection)` chokes with:

```
error C3556: 'das::SortDirection': incorrect argument to 'decltype'
error C2672: 'addExtern': no matching overloaded function found
```

This bit even though the helper was in `das::` namespace and the conflict was with `ImGuiTableColumnSortSpecs::SortDirection` (a different scope). MSVC's qualified-name lookup for `decltype` is more aggressive than expected.

**Fix:** Rename the helper to something that doesn't collide:

```cpp
// BAD: clashes with ImGuiTableColumnSortSpecs::SortDirection
ImGuiSortDirection_ SortDirection ( const ImGuiTableColumnSortSpecs * specs ) { ... }
addExtern<DAS_BIND_FUN(das::SortDirection)>(*this, lib, "SortDirection", ...);

// GOOD: unambiguous name
ImGuiSortDirection_ GetColumnSortDirection ( const ImGuiTableColumnSortSpecs * specs ) { ... }
addExtern<DAS_BIND_FUN(das::GetColumnSortDirection)>(*this, lib, "GetColumnSortDirection", ...);
```

**Why this collides only sometimes:** The original `SortDirection(const T&)` binding existed in dasImgui master without conflict, but flipping the param to `const T*` triggered the decltype path differently. MSVC's overload-resolution heuristics for `decltype(&qualified_name)` are touchy around any name that exists as both a free symbol AND a struct member, even if scopes are disjoint. Renaming is the safe move; trying to qualify harder (`das::SortDirection`, `&das::SortDirection`, etc.) doesn't help — MSVC has already decided the name is ambiguous.

## Bonus rule consistency

If your bound free helper takes a struct, also prefer pointer-arg over ref-arg for daslang interop consistency — `TableGetSortSpecs() : ImGuiTableSortSpecs*` returns a pointer, so making your helpers take `T*` lets daslang call them directly without `*p` deref or `let s & = unsafe(...)` ref-binding gymnastics. Match the binding-side type to what daslang naturally hands to you.

## Questions
- When binding a C++ helper to daslang via `addExtern<DAS_BIND_FUN(...)>`, what trips the runtime "missing WrapType implementation" error and the compile-time `decltype incorrect argument` error?
