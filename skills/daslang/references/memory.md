# Memory and Lifetime

How values move (`=` / `<-` / `:=`), how `const` works, when destruction runs, where memory lives,
and what needs `unsafe`. All examples are gen2 (the default parser).

---

## 1. The const model

`const` is **purely a type qualifier**. There is no separate "const variable" concept — `let` vs
`var` only decides whether `const` is appended to the declared type.

| Spelling | Declared type |
|---|---|
| `var a = x` | `T` |
| `let a = x` | `T const` |
| `def f(a : T)` | `T const` (parameters are const by default) |
| `def f(var a : T)` | `T` |

**Dereference, index, and field access append the handle's const to the result.** Unlike C++ (where
a `T* const` still dereferences to a mutable `T&`), daslang flows the const of the *handle* onto
everything reachable through it:

```das
var mp = unsafe(addr(pt))       // Point?        -> mp.x is writable
let cp = unsafe(addr(pt))       // Point? const  -> cp.x is float const
// cp.x = 5.0                   // error[30952] can't write to a constant value
```

**A pointer type has two independent const positions.** `T const?` = the *pointee* is const;
`T? const` = the *pointer* is const (this is the trailing one `let` adds). To write through a
pointer parameter both must be absent — declare it `var p : T?`:

```das
def scale_into(var dst : float?; src : float const?; n : int; k : float) {
    unsafe {
        for (i in range(n)) {
            dst[i] = src[i] * k
        }
    }
}
```

Callers pass `addr(arr[0])` straight in — it binds to `var T?` (output) and to `T const?` (input,
add-const is implicit) with no laundering cast.

**Never strip const in order to write.** A `const` pointee promises the compiler that the memory is
never written, and the optimizer acts on it: read-only parameters, dead non-aliasing writes, dropped
calls. `var q = unsafe(reinterpret<P?>(p))` then `q.x = 5` compiles and appears to work, but the
*original* const type already licensed those optimizations, so the write can silently vanish in
optimized builds with no diagnostic. Declare the parameter `var P?` instead.

---

## 2. Copy, move, clone

| Operator | Effect |
|---|---|
| `=` copy | bitwise copy; source unchanged (`var b = a` -> both hold 10) |
| `<-` move | transfers ownership; **source is zeroed** (`var m <- src` -> `src` is `[]`) |
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

A struct, tuple, or variant supports an operation only if **all** its fields do. Note the lambda
row: lambdas *are* copyable — a copy is a fat-pointer copy aliasing the same capture frame, so
`push` into `array<lambda<…>>` needs no `push_clone`. Because aliases may be live, `delete lam`
requires `unsafe`, and that requirement cascades to any container or struct holding one.

### Relaxed assign (on by default)

`=` is automatically promoted to `<-` when the right-hand side is a **temporary** — a literal, a
`new`, a lambda, or a call whose result is not a reference — and the type is moveable but not
copyable. Applies at variable init, assignment, struct-literal fields, and `return`.

```das
var a : array<int>
a = get_data()          // becomes: a <- get_data()
var b = get_data()      // same, at initialization
a = [4, 5, 6]           // array literal is a temporary
```

A *named* source is never relaxed, since that would silently empty it — `b = a` on two arrays is
`error[30950] this type can't be copied … use move (<-) or clone (:=) instead`. Set
`options relaxed_assign = false` to require explicit `<-` everywhere.

Non-copyable results leave a function by move — `return <- result`, not `return result`.
`var x := y` (clone-init) expands to `var x <- clone_to_move(y)`; for POD types it degrades to a
plain copy. Cloning is allowed across the regular/temporary (`#`) boundary.

### Per-field and per-element modes

Struct literals pick a mode per field; container slots work the same way:

```das
var f = Foo(name = "hello", data <- items)   // items ends up empty
var g = Foo(name = "hello", data := items2)  // items2 still intact
tab["k"] <- v                                // move into a table zeroes v
t2["k"] := w                                 // clone leaves w intact
```

Lambda captures use the same four operators — `capture(= a, <- b, := c, & d)`; see the closures
reference.

### What clone does

Generated per type: arrays resize the destination and clone each element; tables clear it and
re-insert every key/value; structs clone non-POD fields and copy POD fields; tuples clone
element-wise; variants clone only the active element. A user-defined `def clone(var dest : T; src : T)`
overrides all of that, and wins even for types that are not natively cloneable.

---

## 3. Scope exit

`var inscope` adds an automatic `delete` to the enclosing scope's `finally`. It covers anything
deletable — structs with finalizers, plain `array`/`table` locals, heap pointers, `smart_ptr`:

```das
def use_scope() {
    var inscope r = Res(id = 1)     // finalizer runs at scope exit
    var inscope buf : array<int>    // buffer freed at scope exit
    buf |> push(1)
}
```

A plain `var arr : array<T>` does **not** free itself: declare it `var inscope`, `delete` it, or
move it out with `<-`. Per-frame leaks in hot loops almost always trace to a local container that is
never released.

`daslib/defer` runs statements at scope exit in LIFO order — the last `defer()` block declared runs
first:

```das
require daslib/defer

def work() {
    defer() { print("close A\n") }
    defer() { print("close B\n") }     // runs first
    print("body\n")
}
```

`defer` is **rejected** directly inside a loop body (`error[50503]`) because a loop's `finally` runs
once, not once per iteration — wrap the body in a bare block if you need it there. `var inscope` has
no such restriction: the loop body is its own scope, so it releases every iteration.
(`defer_delete` is deprecated; use `var inscope`.)

**Scope-exit cleanup does not run on panic.** A block's `finally` — and everything built on it — is
skipped when the block panics, by design: a panicked program is broken and its cleanup cannot be
trusted. Never rely on `finally`/`defer`/`inscope` for cleanup that must survive a panic.

---

## 4. delete and finalizers

`delete x` is ownership teardown: it runs the finalizer for `x`, recursing into subtypes, frees the
memory if the memory model allows, and nulls a deleted pointer.

```das
var p = new Foo(v = 1)      // allocated on the current context's heap
unsafe { delete p }         // finalize(*p), free, then p = null -> p == null
```

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

Define `finalize` for any type to replace the generated one; `@do_not_delete` on a struct field
excludes that field entirely (nothing finalized, nothing freed) — that is how a struct holds a
*borrowed* reference:

```das
def finalize(var l : Leaf) {
    print("finalize Leaf {l.id}\n")
}

struct Holder {
    owned : Leaf
    @do_not_delete borrowed : Leaf      // skipped by delete Holder
}
```

### delete vs erase / clear

**`delete` runs user finalizers; `erase`, `clear`, `pop`, and shrinking `resize` do not.** Those
release the slot with generated teardown only and never execute user code. So `a |> clear()` drops
every element with zero `finalize()` calls, while `delete a` finalizes each remaining one.

### The `array<T?>` trap

`delete` on a container of raw pointers **frees the pointees**, not just the buffer — correct for
owned pointers, heap corruption for borrowed ones:

```das
var owned : array<Foo?>
owned |> push(new Foo(id = 0))
unsafe { delete owned }         // finalizes + frees each Foo, then the buffer

view |> clear()                 // borrowed view: clear never touches pointees
unsafe { delete view }          // frees only the buffer
```

For a struct field holding borrowed pointers, annotate it `@do_not_delete` instead.

---

## 5. Heaps, contexts, threads

A compiled program is simulated into a **context** owning its own stack, heap, string heap, and
globals. `new Foo()` allocates on the *current* context's heap.

- **Contexts cannot retain another context's data** — only copy it. A pointer into context A's heap
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

`string` is a pointer into the context's string heap. `var b = a` copies the pointer (an alias).
`var c := a` clones only when the source is a temporary (`#`) string or `options
multiple_contexts` is set — otherwise it is the same pointer copy as `=`. The reliable
cross-context copy is explicit `clone_string(a)`, which allocates in the *current* context's
heap — exactly what a string crossing a context boundary needs. A `var s : string` parameter is a writable local
copy that does not propagate; `var s : string&` writes through to the caller. String interpolation
(`"x = {x}"`) compiles to a sequence of writes into one temporary string; an all-constant
interpolation folds at compile time.

### Garbage collection

Collecting from inside a context needs both options, and `heap_collect` itself is `unsafe`:

```das
options persistent_heap     // garbage-collectable heap
options gc                  // track stack variables for the collector

unsafe {
    heap_collect(true, false)   // collect_string_heap, validate_after_collect
}
```

`options gc` must be declared by the **program being run** — it does not cascade up from a required
module.

### Locks

Arrays and tables are locked while iterated (tables also during `keys`/`values` and lookups), so
mutating one panics instead of invalidating references — `a |> push(x)` inside `for (x in a)` gives
`EXCEPTION: can't resize locked array`. Lock checks cover `<-`, `return <-`, `resize`, `reserve`,
`push`, `push_clone`, `emplace`, `pop`, `erase`, `clear`, and `insert`.

---

## 6. unsafe

Prefer the **expression form** `unsafe(expr)` over `unsafe { … }` — it authorizes exactly the
operation that needs it, e.g. `let px = unsafe(addr(x))`.

**It does not propagate into nested call arguments.** `unsafe(f(addr(x)))` still reports
`error[31000] address of reference requires unsafe` at the inner `addr`. Wrap the operation itself
— `f(unsafe(addr(x)))` — or fall back to `unsafe { f(addr(x)) }` when several nested operations
need coverage.

`unsafe` **is** inherited by local blocks; it is **not** inherited by lambdas, generators, or local
functions — those need their own.

| Requires unsafe | Why |
|---|---|
| `addr(x)` | creates a raw pointer to a reference |
| `p[i]`, `p += n` on a pointer | no bounds check; can build invalid pointers |
| `reinterpret<T>(e)`, `upcast<T>(e)` | arbitrary retyping |
| `delete p` (any pointer, class, or lambda) | other aliases may still be live |
| `each(fixedArray)` outside a `for` | the iterator does not capture the array |
| capture by reference / implicit capture by move | lifetime is not provable |
| moving from a `smart_ptr` that is not a fresh `new` | lifetime is not obvious |
| copying or moving a class value | may slice a derived class |
| a local class variable (`var g = Goo()`) | lifetime issues |
| variant `.field` write, `set_variant_index` | effectively a reinterpret |
| `?as` / `?.field` / `?[]` **not** followed by `??` | yields a pointer to a temporary |
| returning a reference or a temporary (`#`) value | escaping the frame |
| `implicit` parameters, `heap_collect` | policy-level escapes |

Reading a variant through `is` + `as` is safe. A table lookup `tab[k]` is safe by default (an
`unsafe_table_lookup` policy exists, off by default).

`addr<T?>(x)` is sugar for `reinterpret<T?>(addr(x))` and takes a **single** `unsafe` covering both
halves — prefer it to the double-wrapped long form. The target must be a pointer type:

```das
let raw = unsafe(addr<uint8?>(x))
print("{unsafe(raw[0])}\n")             // indexing still needs its own unsafe
```

---

## 7. assume

`assume name = expr` binds a compile-time alias to an expression. No variable is created and nothing
is copied — the expression is substituted at each use, so writes go straight through:

```das
assume inner = w.inner
inner |> push(42)               // same as w.inner |> push(42)
```

Use it to shorten deeply nested access. `typedef name = Type` is the type-level equivalent.

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
| leak reported at exit | a local container or `new` with no `inscope`, `delete`, or `<-` out |
