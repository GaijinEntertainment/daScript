---
slug: daslang-strings-cannot-carry-embedded-nulls-use-array-uint8-and-reinterpret-string-temp-for-c-api-zero-separated-buffers
title: Daslang strings can't carry embedded null bytes — heap-allocated via strlen sizing. How do I pass zero-separated buffers to C APIs (e.g. ImGui's Combo zero-separated overload, glShaderSource sentinel arrays)?
created: 2026-05-10
last_verified: 2026-05-10
links: []
---

**Short answer:** Daslang `string` is null-terminated. Embedded `\x00` bytes are valid in source (lexer accepts `\xNN`), but the heap allocator sizes the string via `strlen`, so anything past the first null is **dropped on heap copy**.

**Empirical verification:**
```das
let s = "Apple\x00Banana\x00"
print("len = {length(s)}")   // 5, not 13
peek_data(s) $(arr) {
    print("buf len = {length(arr)}")  // 5 — the bytes "Banana" are gone
}
```

You cannot use `clone_string` / `:=` / any string-allocator path to preserve embedded nulls. The bytes don't survive `ExprConstString` lowering; the heap copy uses `strlen` of the C++ `std::string`'s `c_str()`.

**The solution: stay in `array<uint8>` for the bytes, and reinterpret the address as `string#` when handing it to the C API.**

```das
struct ComboState {
    @optional items_buf : array<uint8>           // your zero-separated bytes
    // ... rest of state ...
}

// per-frame (or once, depending on how stable items are):
state.items_buf |> clear()
for (s in items) {
    peek_data(s) $(arr) {
        for (b in arr) { state.items_buf |> push(b) }
    }
    state.items_buf |> push(0u8)
}
state.items_buf |> push(0u8)                     // double-null terminator if API needs it

// hand the raw pointer to the C function:
let zsv = unsafe(reinterpret<string#>(unsafe(addr(state.items_buf[0]))))
Combo(text, addr(state.value), zsv, popup_max)
```

**Why `string#` and not `string`:**
- `string#` is the **temporary/borrowed** form — daslang's typer treats it as "char* I don't own". No clone, no heap involvement, no refcount.
- `unsafe(reinterpret<string>(...))` would lie about ownership; daslang might try to clone or finalize it later (depending on where the value flows), corrupting the buffer or double-freeing.
- `string#` is the right primitive whenever you have a C-string-shaped pointer that you don't want daslang to touch.

**Why two `unsafe()` wrappers:**
`reinterpret<...>` requires `unsafe`. `addr(arr[i])` is "address of reference" which **also** requires `unsafe` — and `unsafe(reinterpret(addr(...)))` only marks `reinterpret` unsafe, not `addr` inside. Hence the nested form. Same shape used throughout dasImgui (e.g. `ColorEdit3(text, unsafe(addr(state.value.x)), flags)`).

**Generalization (the long-term pattern):**
Any C API that uses embedded nulls as in-band delimiters (Combo zero-separated, environment-block-style `KEY=VAL\0KEY=VAL\0\0`, scattered C-string sentinel arrays) gets the same treatment in daslang:
1. Build the bytes in `array<uint8>` (push raw bytes + `0u8` separators).
2. `unsafe(reinterpret<string#>(unsafe(addr(buf[0]))))` to hand off a pointer.
3. Buffer's lifetime must outlive the C call — keep it on a state struct, in a local with a longer scope than the call, or in a module-global scratch buffer.

**Don't try to "fix" daslang strings.** Their null-terminated semantics are correct for actual text. Use `array<uint8>` for byte payloads and reinterpret only at the boundary.

**Don't reach for a C++ shim** for this kind of marshaling unless something else mandates it. The daslang-side reinterpret path is shimless, AOT-clean, and the same shape as how dasImgui already passes `addr(state.value.x)` for vector-typed widgets.

**Counter-example (when you DO need a shim):** if the C API takes a `const char* const items[]` (array-of-pointer-to-C-string) and you have `array<string>` in daslang — that one's actually shim-free too: daslang `array<string>` is laid out as `char**` already, so `unsafe(addr(items[0]))` matches the C signature directly. ImGui's `ListBox(label, &cur, items_array, count, height)` takes that path.

**Verified at:** dasImgui Phase 0b.2 (2026-05-10), `combo` + `list_box` widgets in `modules/dasImgui/widgets/imgui_widgets_builtin.das`. Combo wires the zero-separated path; ListBox uses the direct-array pass-through. Both shipped without C++ changes.

## Questions
- Daslang strings can't carry embedded null bytes — heap-allocated via strlen sizing. How do I pass zero-separated buffers to C APIs (e.g. ImGui's Combo zero-separated overload, glShaderSource sentinel arrays)?
