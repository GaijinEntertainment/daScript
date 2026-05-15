---
slug: aot-bit-set-handle-bound-bitfield-uint32
title: AOT compile fails on `__bit_set` for handle-bound bitfield fields — how do I add a `uint32_t&` overload?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**Symptom.** AOT C++ compile errors like:

```
error: no matching function for call to '__bit_set'
  __bit_set(fn->flags /*flags*/, 0x100u, true);
note: candidate function not viable: no known conversion from 'uint32_t' to 'Bitfield &'
```

**Root cause.** The das infer pass lowers `bitfield_value.field = bool` to `ExprCall("__bit_set", value, mask, on)` ([src/ast/ast_infer_type_op.cpp:646-663](src/ast/ast_infer_type_op.cpp#L646-L663)). The AOT codegen emits the C++ call literally. But the original AOT helper in [include/daScript/simulate/aot.h:68](include/daScript/simulate/aot.h#L68) only accepts `Bitfield &` (`BitfieldAny<uint32_t>`). When the das-side type is bitfield but the C++ side is a raw `uint32_t` (handle-bound via `addFieldEx(..., makeFunctionFlags())` — see [src/builtin/module_builtin_ast_annotations_1.cpp:218-221](src/builtin/module_builtin_ast_annotations_1.cpp#L218-L221) for the `Function::flags` example), the AOT call site passes `uint32_t &`, which doesn't bind.

**Fix.** Add raw-integer overloads to `aot.h` alongside the four `Bitfield*` variants:

```cpp
__forceinline void __bit_set ( uint32_t & value, uint32_t mask, bool on ) {
    value = on ? (value | mask) : (value & ~mask);
}
__forceinline void __bit_set ( uint8_t & value, uint8_t mask, bool on ) {
    value = on ? uint8_t(value | mask) : uint8_t(value & ~mask);
}
// ...and uint16_t, uint64_t
```

The uint8/16 forms need `static_cast` on the result since `|`/`&` promote to int — without the cast you'd get a narrowing-conversion warning in `-Wconversion` builds.

**Trap.** Adding these breaks the runtime binding at `src/builtin/module_builtin_runtime.cpp:2427`:

```cpp
addExtern<DAS_BIND_FUN(__bit_set)>(...)  // ambiguous — 5 overloads now
```

`DAS_BIND_FUN(__bit_set)` expands to `decltype(&__bit_set), __bit_set` — and `&__bit_set` no longer resolves uniquely. Disambiguate with a typed function pointer:

```cpp
using BitSetFn = void(*)(Bitfield&, Bitfield, bool);
addExtern<BitSetFn, static_cast<BitSetFn>(__bit_set)>(*this, lib, "__bit_set",
    SideEffects::modifyArgument, "__bit_set")
        ->args({"value","mask","on"});
```

The runtime only needs the `Bitfield&` form (das-typed values flow through here); the raw-integer overloads are AOT-call-site only and exist purely so C++ overload resolution at the AOT emit site picks them.

**Symptom-to-fix shape**: if you see `no matching function for call to '__bit_set'` in an AOT build, the das-side type is a bitfield but the underlying C++ field is bound as a raw integer. Don't reach for the lint rule (STYLE022's rewrite is correct) — fix the AOT helper.

## Questions
- AOT compile fails on `__bit_set` for handle-bound bitfield fields — how do I add a `uint32_t&` overload?
- Why does my STYLE022 rewrite work in interpreter but break AOT?
- How do I disambiguate `DAS_BIND_FUN` for an overloaded extern function?
