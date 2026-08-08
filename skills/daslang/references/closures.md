# Closures — blocks, lambdas, function pointers, generators, iterators

daslang has three anonymous-callable forms, distinguished by their sigil, and one lazy-sequence
type built on top of them.

| | `block` (`$`) | `lambda` (`@`) | `function` (`@@`) |
|---|---|---|---|
| Storage | enclosing stack frame | heap capture frame | none (code pointer) |
| Captures | whole enclosing scope, by reference, implicit | explicit per-variable: copy / move / clone / ref | none — cannot see enclosing locals |
| Value is | a stack cookie | one machine word (pointer to the frame) | one machine word |
| Copyable (`=`) | no | **yes** — copies the pointer, aliasing the same frame | yes |
| Movable (`<-`) | no | yes | yes |
| Cloneable (`:=`) | no | no | yes |
| Local variable | yes, but only initialized from a block literal | yes | yes |
| Global / struct field | no | yes | yes |
| Returnable | no | yes | yes |
| Custom finalizer | n/a | yes, `finally` clause | n/a |
| Cost | cheapest — no allocation | one heap allocation at creation | free |

Rule of thumb: **block** for a callback consumed inside the call (the common case — iteration,
comparators, visitors); **lambda** when the callable must outlive the current frame or be stored;
**function pointer** when there is nothing to capture.

Not true (a common mis-statement): lambdas are *not* move-only. `=` and pass-by-value copy the
fat pointer, so two names can share one capture frame — which is exactly why `delete` on a lambda
requires `unsafe`.

---

## Blocks

The type is `block` plus an optional signature; `:` and `->` both introduce the return type
(`block<(a : int) -> bool>` is the same type as `block<(a : int) : bool>`), and bare `block` means
"no arguments, returns nothing".

```das
def goo(b : block) { invoke(b) }
def radd(var ext : int&; b : block<(var arg : int&) : int>) : int { return invoke(b, ext) }
def radd2(var ext : int&; b : block<(var arg : int&) : int>) : int { return b(ext) }   // call-like
```

`invoke(b, args...)` and the call-like `b(args...)` are equivalent. Call-like syntax works when the
callee is a plain name or a struct field (`h.fn(21)`); for anything else — an array element, a call
result — use `invoke`, since `lams[0](100)` is a syntax error.

### Declaring one

```das
// assumed pipe: a block written right after a call becomes its last argument
var r = radd(v1) $(var a : int&) : int {
    return a++
}
goo() {                              // no '$' needed when the block takes no arguments
    print("inside goo\n")
}
radd(v1, $(var a : int&) : int { return a++; })   // inline argument
radd(v1, $(var a : int&) : int => a++)            // single-expression arrow body
radd(v1, $(a) => a++)                             // types inferred from the parameter's block type
```

The assumed pipe also works on method calls (`obj.method() { ... }`, `obj->method() { ... }`), and
defaulted parameters sitting between the explicit arguments and the trailing block are padded
automatically. The explicit `<|` pipe (`radd(v1) <| $(a) => a++`) is equivalent.

Types can only be inferred when the target says what the block is. A block or lambda literal
assigned to a bare `var` with no signature fails: `@(x) => x + 1` alone gives
`error[30200] initialization type can't be inferred`.

### Capture and lifetime

A block sees the enclosing frame directly — reads and writes hit the original variables:

```das
var total = 0
run() {
    total += 10        // mutates the caller's variable
}
// total == 10
```

That is also why a block can never escape the frame it was made in. All of these are compile
errors:

| Attempt | Error |
|---|---|
| `def make : block<() : void>` | `30167` not a valid function return type |
| `var g_blk : block<() : void>` at module scope | `30179` can't have a global variable of that type |
| `var blk2 = blk` / `var blk2 <- blk` | `30198` can only be initialized with make block expression |
| `break` / `continue` inside a block, targeting an outer loop | `30125` captured block can't 'break' outside of the block |

A local block variable is fine as long as it is initialized by a literal:

```das
var blk = $(a, b : int) {
    return a + b
}
verify(3 == invoke(blk, 1, 2))
verify(7 == blk(3, 4))
```

Blocks may carry annotations between the sigil and the argument list — `$ [es] (var pos : float3&)`
— which is how block-annotation macros hook them.

---

## Lambdas

Same shape as a block, with `@` instead of `$`, and a heap capture frame instead of the stack.

```das
var cnt = 0
var counter <- @ capture(copy(cnt)) (extra : int) : int {
    return cnt++ + extra
}
print("{invoke(counter, 13)}\n")    // 13
print("{counter(14)}\n")            // 15
```

`<-` is the idiomatic initializer (it reads as "this binding now owns the fresh frame"); `=` also
compiles, since it is the same pointer copy.

### Copies alias the frame

```das
var alias_of = counter    // copies the pointer, NOT the frame
print("{alias_of(0)}\n")  // 2 — continues counter's captured cnt
print("{cnt}\n")          // 0 — the outer variable was captured by copy, never touched
```

Because copies are cheap and legal, lambdas go into containers, struct fields, and globals with
plain `push` / `=`:

```das
struct Handler {
    name : string
    fn : lambda<(x : int) : int>
}

var h = Handler(name = "double", fn <- @(x : int) => x * 2)
var lams : array<lambda<(extra : int) : int>>
lams |> push(counter)                 // push, not push_clone — lambdas are copyable
let v = invoke(lams[0], 1000)
```

The flip side: `delete lam` frees a frame that other copies may still point at, so it needs
`unsafe` (`error[31009] delete of lambda<...> requires unsafe`). The gate cascades to anything
containing a lambda, so `delete` on an `array<lambda<...>>` is equally unsafe — and deleting an
array holding two copies of one lambda is a double free. There is no reference counting: a frame
lives until the lambda is deleted or the owning context's heap is collected/reset by the host
program, so lambdas created in a long-running loop and never deleted are a common leak.

### Capture modes

Without a `capture(...)` list, every referenced outer variable is captured **by copy**. If the type
is not copyable, the compiler falls back to move and demands `unsafe`
(`error[31003] implicit capture by move requires unsafe, while capturing src`).

| Symbol form | Named form | Effect | Gate |
|---|---|---|---|
| `=x` | `copy(x)` | copy the value into the frame | type must be copyable |
| `<-x` | `move(x)` | move into the frame; **source is left empty** | type must be movable |
| `:=x` | `clone(x)` | deep copy into the frame | type must be cloneable |
| `&x` | `ref(x)` | frame holds a reference to the original | requires `unsafe` (dangling risk) |

Both spellings are identical; entries are comma-separated.

```das
var a1 <- [1, 2]
var a2 <- [1, 2]
var a3 <- [1, 2]
unsafe {                                        // required by the ref capture
    var lam <- @ capture(&a1, <-a2, :=a3) {
        a1 |> push(1)
        a2 |> push(1)
        a3 |> push(1)
    }
    invoke(lam)
}
// a1 == [1,2,1]  (ref: the outer array grew)
// a2 == []       (move: emptied at capture time)
// a3 == [1,2]    (clone: the frame grew its own copy)
```

A lambda cannot capture a block — blocks are neither copyable nor movable
(`error[30129] can't capture variable blk`). Pass a lambda or a function pointer instead.

**An enclosing `unsafe` does not reach into a lambda body.** The body is a separate function, so
unsafe operations inside it need their own wrap; a *block* body, which runs in the same frame, is
covered by the outer wrap.

```das
unsafe {
    var lam <- @ capture(ref(arr)) () {
        var p = unsafe(addr(arr[0]))   // inner unsafe required
    }
    invoke(lam)
}
```

### Finalizer

A `finally` clause after the body runs before the default finalizer, when the lambda is deleted:

```das
var counter <- @ capture(copy(cnt)) (extra : int) : int {
    return cnt++ + extra
} finally {
    print("lambda finalizer ran, cnt = {cnt}\n")
}
var x = invoke(counter, 13)
unsafe { delete counter; }   // prints "lambda finalizer ran, cnt = 1"
```

---

## Function pointers

`@@` produces a callable with **no captures at all** — referencing an enclosing local from a `@@`
body is `error[30838] can't locate variable`. That is the whole difference from a lambda; the
declaration forms, the typed `@@<(...) : T> name` spelling, and `invoke` are covered under
functions.

```das
def apply(x : int; fn : function<(a : int) : int>) : int { return invoke(fn, x) }

apply(21, @@double_it)                     // named function
apply(10, @@(x : int) => x + 1)            // anonymous, arrow body, captures nothing
```

---

## Generators

A generator is a lambda compiled into a state machine; its value is an `iterator<T>`. The body
`yield`s elements and must end by returning `false` — omitting it is
`error[30172] generator must return boolean`.

```das
var gen <- generator<int> {
    for (t in range(0, 5)) {
        yield t
    }
    return false            // false ends the sequence
}
for (v in gen) {
    print("v = {v}\n")
}
```

Because the result is an iterator, a generator is the normal way to write a lazy sequence function:

```das
def squares(n : int) : iterator<int> {
    return <- generator<int> {
        for (i in range(n)) {
            yield i * i
        }
        return false
    }
}

for (s in squares(4)) { print("s = {s}\n") }    // 0 1 4 9
```

Details:

- `yield <- expr` move-yields, for element types that cannot be copied.
- `break`, `continue`, `while`, and nested `for` all work inside the body.
- `generator<T&>` yields references, so `for (t in gen) { t++ }` writes through to the source.
- The capture list has the same syntax and the same gates as a lambda's: default is copy, an
  implicit move needs `unsafe`, a `ref` capture needs `unsafe`.

```das
unsafe {
    var src = [1, 2, 3, 4]
    var g <- generator<int&> capture(ref(src)) {
        for (w in src) { yield w }
        return false
    }
    for (t in g) { t++ }        // src is now [2, 3, 4, 5]
}
```

A `finally` on a loop body inside a generator belongs to the **body block**, so it runs once per
iteration, interleaved — not once when the loop ends:

```das
var g <- generator<int> {
    for (t in range(0, 3)) {
        yield t
    } finally {
        yield 9
    }
    return false
}
// yields 0, 9, 1, 9, 2, 9
```

The older `generator<int>() <| $ { ... }` spelling still parses; write the brace form.

---

## Iterators

`iterator<T>` is the sequence type consumed by `for`. Iterators can be moved but never copied
(`error[30197] can only be move-initialized`).

```das
var it <- each(range(0, 3))
var v : int
while (next(it, v)) {          // manual stepping; next requires a copyable element type
    print("v = {v}\n")
}
verify(empty(it))              // true once sequenced out
delete it                      // always safe on a sequenced-out iterator
```

`for` loops close and finalize the iterator automatically, so an explicit `delete` is only needed
for one you stopped driving early.

### Making a type iterable

Define an `each` overload returning an `iterator`, and `for (x in value)` starts working — the
compiler inserts the `each` call. Mark it `[unsafe_outside_of_for]` when the iterator does not keep
its source alive; direct calls then require `unsafe`
(`error[31013] 'each' is unsafe, when not source of the for-loop`), while for-loop use stays clean.

```das
struct Foo {
    data : array<int>
}

[unsafe_outside_of_for]
def each(f : Foo) : iterator<int&> {
    return unsafe(each(f.data))
}

var f = Foo(data <- [1, 2, 3])
for (x in f) { print("x = {x}\n") }     // same as: for (x in each(f))
```

Only the name `each` is auto-inserted. Any other iterator-returning function must be called
explicitly: `for (x in filtered(f))`.

### Lambda as an iterator

`each(lam)` turns a lambda into an iterator when the lambda takes exactly one reference
out-parameter and returns `bool` (`true` = a value was produced, `false` = stop):

```das
var lam <- @ capture(copy(count)) (var a : int&) : bool {
    if (count < 3) {
        a = count++
        return true
    }
    return false
}
for (x in each(lam)) { print("{x}\n") }     // 0 1 2
```

### Built-in sources

```das
for (k, v in keys(tab), values(tab)) { ... }         // table: iterator<K> and iterator<V&>
for (ch in unsafe(each("hello"))) { ... }            // string: iterator<int> of characters
for (x, tx in each(lam), range(0, 3)) { ... }        // several iterators advance in lockstep
var sq <- [iterator for (i in range(0, 4)); i * i]   // comprehension: lazy, generator-backed

require daslib/enum_trait
enum Numbers { one, two, ten = 10 }
for (n in type<Numbers>) { ... }                     // every value of the enumeration
```

---

## Quick gotchas

- A lambda copy aliases the capture frame — deleting a container that holds two copies of one
  lambda is a double free.
- A lambda body does not inherit an enclosing `unsafe`; a block body does.
- Capturing a non-copyable value with no capture list becomes a move, and is rejected until you
  write `unsafe` or an explicit `clone(...)`.
- A generator body must end with `return false`, and `finally` on a loop body runs per iteration.
- Call-like invocation works on names and fields; use `invoke` for array elements and other
  expressions.
