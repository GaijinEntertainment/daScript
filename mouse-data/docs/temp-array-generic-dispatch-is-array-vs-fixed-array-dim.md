---
slug: temp-array-generic-dispatch-is-array-vs-fixed-array-dim
title: How do I write a generic `temp_array` / `array_helper`-style helper that accepts both `array<T>` and fixed-size `T[N]`?
created: 2026-05-21
last_verified: 2026-05-21
links: []
---

A generic daslang helper like `temp_array(arr)` may be called with either a runtime `array<T>` (e.g. `var arr : array<int>`) or a fixed-size dim (e.g. `int[5]` inside `algorithm.reverse(a : int[5])`). The two need different length and builder primitives — `_::long_length` and `_builtin_make_temp_array_i64` only accept `array<T>` / `table<K;V>`, not dims.

The dispatch is `static_if (typeinfo is_array(arr))`:

```das
[unsafe_operation, template(a), unused_argument(a)]
def private array_helper(var arr : auto implicit ==const; a : auto(TT)) : array<TT -const -#> {
    var res : array<TT -const -#>
    static_if (typeinfo is_array(arr)) {
        // array<T> path: long_length + i64 builder for huge-array safety
        let lenA = _::long_length(arr)
        if (lenA >= 1_l) {
            unsafe { _builtin_make_temp_array_i64(res, addr(arr[0]), lenA) }
        }
    } else {
        // fixed-array path: length is compile-time-bounded, use int builder
        let lenA = _::length(arr)
        if (lenA >= 1) {
            unsafe { _builtin_make_temp_array(res, addr(arr[0]), lenA) }
        }
    }
    return <- res
}
```

## Why two paths

- `_::long_length(arr)` is defined for `array const` and `table const` only. Calling it on `int[5]` fails compilation with `error[30341]: no matching functions or generics`. Surfaces as a tutorial dry-run failure on darwin extended_checks when something through `algorithm.reverse(int[5])` reaches the helper.
- `_builtin_make_temp_array_i64` expects an `int64_t` length. Passing `int` from a fixed-array would also need conversion. The simpler split keeps both builders honest with their natural arg types.
- Fixed-array sizes are compile-time bounded (typically <100 elements) — there's no realistic concern about exceeding INT_MAX. The `int` builder is fine for them.

## Other typeinfo predicates that distinguish these

- `typeinfo is_array(x)` — true for `array<T>` runtime arrays only.
- `typeinfo is_dim(x)` — true for fixed-size `T[N]` (caveat: some daslang versions return this for both `T[N]` and `array<T>` in certain phases; `is_array` is the safer discriminator).
- `typeinfo is_table(x)` — true for `table<K;V>`.

When you want to dispatch between "is this something `length()` accepts" vs "is this something `long_length()` accepts", `is_array(x) || is_table(x)` is the predicate for the `long_*` family.

## Bit me in PR-G (#2773)

PR-G consolidated `array_helper` to use `long_length + _i64` builder unconditionally — that broke `algorithm.reverse(int[5])` because fixed-arrays don't satisfy the `long_length` overload set. Caught only by `extended_checks (darwin15, arm64)` running the language tutorials in dry-run. Fix landed in #2773 round 2: the static_if dispatch above.

## Questions
- How do I write a generic `temp_array` / `array_helper`-style helper that accepts both `array<T>` and fixed-size `T[N]`?
