# Memory and Lifetime

## 1. The const model

| Spelling | Declared type |
|---|---|
| `var a = x` | `T` |
| `let a = x` | `T const` |
| `def f(a : T)` | `T const` (parameters are const by default) |
| `def f(var a : T)` | `T` |

**Dereference, index, and field access append the handle's const to the result:**

```das
var mp = unsafe(addr(pt))       // Point?        -> mp.x is writable
let cp = unsafe(addr(pt))       // Point? const  -> cp.x is float const
```

**A pointer type has two independent const positions.** `T const?` = the *pointee* is const;
`T? const` = the *pointer* is const (the trailing one `let` adds). To write through a pointer
parameter both must be absent: `def scale_into(var dst : float?; src : float const?)`.
`addr(arr[0])` binds to both `var T?` and `T const?` — add-const is implicit.

**Never strip const in order to write.** The const type already licensed read-only parameters, dead
non-aliasing write removal, and dropped calls, so `var q = unsafe(reinterpret<P?>(p))` then
`q.x = 5` compiles, appears to work, and can silently vanish in optimized builds. Declare the
parameter `var P?`.

---

## 2. Copy, move, clone

| Operator | Effect |
|---|---|
| `=` copy | bitwise copy; source unchanged |
| `<-` move | transfers ownership; **source is zeroed** |
| `:=` clone | deep copy; both sides valid and independent afterwards |

| Type | `=` | `<-` | `:=` |
|---|---|---|---|
| POD scalars (`int`, `float`, vectors, …) | yes | yes | yes (becomes copy) |
| `string` | yes (pointer copy) | yes | yes (new allocation) |
| `array<T>` / `table<K;V>` | **no** | yes | yes |
| struct, all-POD fields | yes | yes | yes (becomes copy) |
| struct with array/table fields | **no** | yes | yes |
| `tuple` / `variant` | per element | per element | per element |
| raw pointer `T?` | yes | yes | yes |
| `smart_ptr<T>` | **no** | yes | yes |
| `lambda<…>` | **yes** (aliases the capture frame) | yes | **no** |
| `block<…>` | no | no | no |
| `iterator<T>` | no | yes | no |

A struct, tuple, or variant supports an operation only if **all** its fields do. Because a lambda
copy aliases the capture frame, `push` into `array<lambda<…>>` needs no `push_clone`, and the
`unsafe` on `delete lam` cascades to any container or struct holding one.

### Relaxed assign (on by default)

`=` is promoted to `<-` when the right-hand side is a **temporary** (a literal, a `new`, a lambda,
or a call whose result is not a reference) and the type is moveable but not copyable — at variable
init, assignment, struct-literal fields, and `return`. A *named* source is never relaxed: `b = a`
on two arrays is `error[30950]`. `options relaxed_assign = false` requires explicit `<-` everywhere.

`var x := y` (clone-init) expands to `var x <- clone_to_move(y)`. Cloning is allowed across the
regular/temporary (`#`) boundary.

### Per-field and per-element modes

```das
var f = Foo(name = "hello", data <- items)   // items ends up empty
tab["k"] <- v                                // move into a table slot zeroes v
```

Lambda captures use the same four operators — `capture(= a, <- b, := c, & d)`; see the closures
reference.

### What clone does

Generated per type: arrays resize the destination and clone each element; tables clear it and
re-insert every key/value; structs clone non-POD fields and copy POD ones; tuples element-wise;
variants only the active element. A user-defined `def clone(var dest : T; src : T)` overrides all
of that, and wins even for types not natively cloneable.

---

## 3. Scope exit

`var inscope` adds an automatic `delete` to the enclosing scope's `finally`, covering anything
deletable — structs with finalizers, plain `array`/`table` locals, heap pointers, `smart_ptr`. A
plain `var arr : array<T>` does **not** free itself: declare it `var inscope`, `delete` it, or move
it out with `<-`.

`daslib/defer` runs statements at scope exit in LIFO order:

```das
require daslib/defer

def work() {
    defer() { print("close A\n") }
    defer() { print("close B\n") }     // runs first
}
```

`defer` is **rejected** (`error[50503]`) directly inside a loop body — a loop's `finally` runs once,
not per iteration — and at the top level of a lambda or generator body, where it would land in the
`finally` that is the finalizer (run once, on `delete`), not a per-call cleanup
(probe-verified 2026-08-20). A wrapping `{ }` block gives a per-iteration / per-call scope; a `$()`
block body fires at exit as expected. `var inscope` has no loop restriction — the loop body is its
own scope. (`defer_delete` is deprecated; use `var inscope`.)

**Panic skips `finally`** and everything built on it — `defer` and `inscope` cleanup does not run.

---

## 4. delete and finalizers

`delete x` runs the finalizer for `x`, recursing into subtypes, and frees the memory if the memory
model allows.

| Type | Generated finalizer |
|---|---|
| pointer `T?` | finalize the pointee, free it, null the pointer (needs `unsafe`) |
| fixed array `T[N]` | finalize each element |
| `array<T>` | finalize each element, then free the buffer |
| `table<K;V>` | finalize each **value** (keys are not finalized), then free |
| struct | finalize each field in declaration order, then `memzero` |
| tuple | finalize each element, then `memzero` |
| variant | finalize only the active element, then `memzero` |
| lambda / generator | finalize the capture frame |

Define `def finalize(var x : T)` for any type to replace the generated one; `@do_not_delete` on a
struct field excludes it entirely (nothing finalized, nothing freed) — how a struct holds a
*borrowed* reference:

```das
struct Holder {
    owned : Leaf
    @do_not_delete borrowed : Leaf      // skipped by delete Holder
}
```

### delete vs erase / clear

**`delete` runs user finalizers; `erase`, `clear`, `pop`, and shrinking `resize` do not** — those
release the slot with generated teardown only.

### The `array<T?>` trap

`delete` on a container of raw pointers **frees the pointees**, not just the buffer — correct for
owned pointers, heap corruption for borrowed ones:

```das
unsafe { delete owned }         // owned array<Foo?>: finalizes + frees each Foo, then the buffer

view |> clear()                 // borrowed view: clear never touches pointees
unsafe { delete view }          // frees only the buffer
```

Interpreted, the bad free reports `deleting <ptr>, which is not a chunk pointer` at the delete;
optimized and JIT-ed, it corrupts the heap silently and crashes later at an unrelated allocation.

---

## 5. Heaps, contexts, threads

A compiled program is simulated into a **context** owning its own stack, heap, string heap, and
globals; `new Foo()` allocates on the *current* one.

- **Contexts cannot retain another context's data**, only copy it — a pointer into context A's heap
  is meaningless in context B.
- **Threads run in separate contexts.** `new_thread` clones the context and the lambda; captured
  values arrive as copies.
- **Channel payloads arrive as temporary references** (`T#`) — clone or copy what you need before
  the callback returns.

```das
require daslib/jobque_boost

with_job_que() {
    with_channel(1) $(ch) {
        new_thread() @() {
            ch |> push_clone(Msg(v = 6 * factor))
            ch |> notify_and_release
        }
        ch |> for_each_clone() $(m : Msg#) { print("got {m.v}\n") }
    }
}
```

### Strings

`string` is a pointer into the context's string heap; `var b = a` copies the pointer. `var c := a`
clones only when the source is a temporary (`#`) string or `options multiple_contexts` is set —
otherwise it is the same pointer copy as `=`; the reliable cross-context copy is `clone_string(a)`,
which allocates in the *current* context's heap. A `var s : string` parameter is a writable local
copy that does not propagate; `var s : string&` writes through to the caller. Interpolation
(`"x = {x}"`) compiles to writes into one temporary string; an all-constant one folds at compile
time.

### Garbage collection

Collecting from inside a context needs both options:

```das
options persistent_heap     // garbage-collectable heap
options gc                  // track stack variables for the collector

unsafe { heap_collect(true, false) }    // collect_string_heap, validate_after_collect
```

`options gc` must be declared by the **program being run** — it does not cascade up from a required
module.

### Locks

Arrays and tables are locked while iterated (tables also during `keys`/`values` and lookups), so
mutating one panics. Lock checks cover `<-`, `return <-`, `resize`, `reserve`, `push`,
`push_clone`, `emplace`, `pop`, `erase`, `clear`, and `insert`.

---

## 6. unsafe

Prefer `unsafe(expr)` over `unsafe { … }` — it authorizes exactly the operation that needs it.

**It does not propagate into nested call arguments.** `unsafe(f(addr(x)))` still reports
`error[31000]` at the inner `addr` — wrap the operation itself, `f(unsafe(addr(x)))`, or fall back
to `unsafe { f(addr(x)) }` for several nested ones.

`unsafe` is inherited by local blocks, **not** by lambdas, generators, or local functions — those
need their own.

| Requires unsafe | Why |
|---|---|
| `addr(x)` | creates a raw pointer to a reference |
| `p[i]`, `p += n` on a pointer | no bounds check |
| `reinterpret<T>(e)`, `upcast<T>(e)` | arbitrary retyping |
| `delete p` (any pointer, class, or lambda) | other aliases may still be live |
| `each(fixedArray)` outside a `for` | the iterator does not capture the array |
| capture by reference / implicit capture by move | lifetime is not provable |
| moving from a `smart_ptr` that is not a fresh `new` | lifetime is not obvious |
| copying or moving a class value | may slice a derived class |
| a local class variable (`var g = Goo()`) | lifetime issues |
| variant `.field` write, `set_variant_index` | effectively a reinterpret |
| `?as` / `?.field` / `?[]` **not** followed by `??` | yields a pointer to a temporary |
| `let r & = expr` binding a reference to a **non-local** expression | the referent's lifetime is not provable from this frame |
| returning a reference or a temporary (`#`) value | escaping the frame |
| `implicit` parameters, `heap_collect` | policy-level escapes |

Reading a variant through `is` + `as` is safe; so is a table lookup `tab[k]` (the
`unsafe_table_lookup` policy exists, off by default).

The local-reference rule turns on *where the referent lives*, not on the syntax: `let r & = s.a` is
fine when `s` is a stack local; with `s` a heap pointer it is
`error[31019] local reference to non-local expression is unsafe`. (probe-verified 2026-08-16)

`addr<T?>(x)` is sugar for `reinterpret<T?>(addr(x))` under a **single** `unsafe`; the target must
be a pointer type.

---

## 7. assume

`assume name = expr` substitutes the expression at each use — no variable, no copy, so writes go
straight through. `typedef name = Type` is the type-level equivalent.

---

## Quick diagnosis

| Message | Meaning |
|---|---|
| `error[30950] this type can't be copied` | non-copyable source; use `<-` or `:=` |
| `error[30197] local variable X can only be move-initialized` | use `var x <- …` (or `:=`) |
| `error[30952] can't write to a constant value` | the handle is `let`, or a parameter missing `var` |
| `error[31000] address of reference requires unsafe` | wrap the `addr` itself, not the enclosing call |
| `error[31023] index of the pointer must be inside the 'unsafe' block` | pointer indexing needs its own `unsafe` |
| `EXCEPTION: can't resize locked array` | mutating a container while iterating it |
| `EXCEPTION: array resize … grows past max_unreserved_size` | a growing `resize` past the byte limit (64 MB default); `reserve` the exact size first, `ensure_capacity` for appends, or raise `options max_unreserved_size` |
| leak reported at exit | a local container or `new` with no `inscope`, `delete`, or `<-` out |
