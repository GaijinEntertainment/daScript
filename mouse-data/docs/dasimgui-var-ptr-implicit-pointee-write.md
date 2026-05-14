---
slug: dasimgui-var-ptr-implicit-pointee-write
title: How do I write through a `T?` pointer parameter from a daslang function body?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**Two traps to dodge.** A function that takes a `T?` pointer and writes back through it via `*p = value` needs both the parameter shape and the local binding right.

## Trap 1 — `let p = ptr` loses mutability

    def foo(ptr : bool?) {
        let p = ptr             // ✗ const-deref trap
        unsafe {
            *p = !*p             // error: can't write to a constant value, bool aka TT const&
        }
    }

`let` binds `p` as const. `*p = value` then fails because the LHS resolves to `bool const&` (write to const). Fix:

    def foo(ptr : bool?) {
        var p = ptr             // ✓ mutable local
        unsafe {
            *p = !*p             // OK
        }
    }

## Trap 2 — `T?` parameter rejects `safe_addr`'s temp

    def foo(ptr : bool?) {
        ...
    }
    foo(safe_addr(g_flag))      // ✗ argument expects `bool?`, got `bool?#`

`safe_addr` returns `bool?#` (temp), and `bool?` parameter declaration won't accept it. Fix: declare param `implicit`:

    def foo(var ptr : bool? implicit) {
        var p = ptr
        unsafe {
            *p = !*p
        }
    }

The `implicit` flavor accepts both:
- `safe_addr(local_or_global)` (returns `T?#`)
- `unsafe(addr(struct.field))` (returns `T?`)

Combine `var ptr : T? implicit` + `var p = ptr` and the body writes through the pointee safely.

## Combined idiom (used heavily in commits 10a-c of the v2 boost mega-detour)

    def edit_finalize(widget_ident : string; kind : string;
                      var ptr : auto(TT)? implicit) {
        var entry : WidgetEntry
        unsafe {
            var p = ptr
            let ser <- @ capture(= p) () : JsonValue? {
                return JV((value = *p))
            }
            let disp <- @ capture(= p) (action : string; payload : JsonValue?) {
                if (action == "set") {
                    *p = from_JV(payload?["value"], type<TT>, default<TT>)
                }
            }
            widget_finalize(widget_ident, kind, entry, ser, disp)
        }
    }

The `var p = ptr` captures by value into the lambda, and `*p` reads/writes the pointee.

## Questions
- How do I write through a `T?` pointer parameter from a daslang function body?
- Why does `let p = ptr; *p = value` fail with `can't write to a constant value`?
- What's the difference between `T?` and `T?#` (temp pointer)?
- What does `var ptr : T? implicit` mean?
- Why doesn't my function accept `safe_addr(g_flag)` as a `bool?` parameter?
