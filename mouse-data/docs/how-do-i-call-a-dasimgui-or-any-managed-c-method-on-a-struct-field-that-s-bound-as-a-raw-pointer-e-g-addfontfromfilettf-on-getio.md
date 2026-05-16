---
slug: how-do-i-call-a-dasimgui-or-any-managed-c-method-on-a-struct-field-that-s-bound-as-a-raw-pointer-e-g-addfontfromfilettf-on-getio
title: How do I call a dasImgui (or any managed C++) method on a struct field that's bound as a raw pointer — e.g. AddFontFromFileTTF on GetIO().Fonts?
created: 2026-05-16
last_verified: 2026-05-16
links: []
---

## TL;DR

When a managed struct's field is bound as a pointer (`T?`) and the method on that pointed-to struct expects the value by-ref (`T implicit`), you must explicitly **dereference**. Plain `field |> method(...)` errors with mismatched types.

## The error you'll hit

```
error[30341]: no matching functions or generics: AddFontFromFileTTF(imgui::ImFontAtlas?&, string const&, ...)
  candidate function: ImFontAtlas implicit ...
  invalid argument 'self' (0). expecting 'imgui::ImFontAtlas implicit', passing 'imgui::ImFontAtlas?&'
```

The `?` is the giveaway — `GetIO().Fonts` is `ImFontAtlas?` (raw pointer; field bound via `addField<DAS_BIND_MANAGED_FIELD(Fonts)>` against C++ `ImFontAtlas* Fonts`), but the method binding `das_call_member< ImFont * (ImFontAtlas::*)(...) >` takes the receiver by-value/ref.

## The fix

Bind a local ref through `unsafe(*ptr)`, then call as usual:

```daslang
var atlas & = unsafe(*GetIO().Fonts)
let f = atlas |> AddFontFromFileTTF(ttf, 14.0f, null, null)
```

Equivalent inline form:

```daslang
unsafe(*GetIO().Fonts) |> AddFontFromFileTTF(ttf, 14.0f, null, null)
```

## Why each part

- **`*ptr`** is daslang's pointer-deref syntax (see `daslib/if_not_null.rst`: *"a dereferenced call: ``if (ptr != null) { call(*ptr, args) }``"*). The alternative `deref(ptr)` exists too but is rarer in modules; `*` is the idiom.
- **`unsafe(...)`** is required because dereferencing a raw `T?` is unsafe (no null check, no lifetime guarantee).
- **`var atlas &`** binds a local *reference* — without `&` you'd be copying the whole `ImFontAtlas` struct into a stack temporary, which (a) wastes memory and (b) means any mutation the method does (font atlas builds, glyph rasterization) hits the copy and is lost.
- **The pipe `|>` works fine on the local ref** — `atlas |> method(x, y)` desugars to `method(atlas, x, y)` and the `implicit` first-param accepts the ref directly.

## Why NOT the other shapes

- `GetIO().Fonts.AddFontFromFileTTF(...)` — `.method()` sugar is sugar for `method(self, ...)` only when `self` is a struct value. CLAUDE.md explicitly: *"Does NOT work on: primitives, tuples/arrays, and lambda typedefs"* — and (this case) raw pointers. Field *access* on a pointer auto-derefs (`GetIO().Fonts.TexID` works); method dispatch does not.
- `GetIO().Fonts->AddFontFromFileTTF(...)` — `->` is for class instances (smart_ptr / class types), not raw C-struct pointers from `ManagedStructureAnnotation`.
- `deref(GetIO().Fonts) |> AddFontFromFileTTF(...)` — works but the pipe gets a temporary value not a ref; mutations on the receiver disappear. Use `var x & = unsafe(*p)` instead.

## When this comes up

Anywhere a C++ binding exposes a struct field as `T*` (typical for "owns-an-atlas" or "owns-a-context" patterns):
- `ImGuiIO::Fonts` → `ImFontAtlas?`
- `ImDrawData::CmdLists` → indirection on lists
- anything bound via raw `addField<DAS_BIND_MANAGED_FIELD(X)>` where the C++ type is `Foo*`

If the C++ field were a value (`ImFontAtlas Fonts;` instead of `ImFontAtlas* Fonts;`), it'd bind as the struct directly and the pipe would just work.

## Related

- [[dasimgui-new-state-struct-widget-auto-emit-just-works]] — different topic (state-struct registration) but same module family.
- [[how-do-i-pack-an-im-col32-color-from-dasimgui-v2-code-without-depending-on-the-v1-daslib-imgui-boost-path]] — sibling dasImgui idiom.

## Questions
- How do I call a dasImgui (or any managed C++) method on a struct field that's bound as a raw pointer — e.g. AddFontFromFileTTF on GetIO().Fonts?
