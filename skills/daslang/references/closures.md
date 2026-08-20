# Closures — blocks, lambdas, function pointers, generators, iterators

| | `block` (`$`) | `lambda` (`@`) | `function` (`@@`) |
|---|---|---|---|
| Storage | enclosing stack frame | heap capture frame | none (code pointer) |
| Captures | whole enclosing scope, by reference, implicit | explicit per-variable: copy / move / clone / ref | none — cannot see enclosing locals |
| Copyable (`=`) | no | **yes** — copies the pointer, aliasing the same frame | yes |
| Movable (`<-`) | no | yes | yes |
| Cloneable (`:=`) | no | no | yes |
| Local variable | yes, but only initialized from a block literal | yes | yes |
| Global / struct field | no | yes | yes |
| Returnable | no | yes | yes |
| Custom finalizer | n/a | yes, `finally` clause | n/a |

Block for a callback consumed inside the call; lambda when it must outlive the frame or be stored;
function pointer when there is nothing to capture.

---

## Blocks

The type is `block` plus an optional signature; `:` and `->` both introduce the return type
(`block<(a : int) -> bool>` is the same type as `block<(a : int) : bool>`), and bare `block` takes
no arguments and returns nothing.

```das
def goo(b : block) { invoke(b) }
def radd(var ext : int&; b : block<(var arg : int&) : int>) : int { return invoke(b, ext) }
```

`invoke(b, args...)` and `b(args...)` are equivalent. Call-like syntax works when the callee is a
plain name or a struct field (`h.fn(21)`); anything else — an array element, a call result — needs
`invoke`, since `lams[0](100)` is a syntax error.

### Declaring one

```das
var r = radd(v1) $(var a : int&) : int {          // assumed pipe: trailing block = last argument
    return a++
}
goo() { print("inside goo\n") }                   // no '$' when the block takes no arguments
radd(v1, $(var a : int&) : int { return a++; })   // inline argument
radd(v1, $(a) => a++)                             // arrow body; types inferred from the block type
```

The assumed pipe also works on method calls (`obj.method() { ... }`, `obj->method() { ... }`), and
pads defaulted parameters sitting between the explicit arguments and the trailing block. The
explicit pipe `radd(v1) <| $(a) => a++` is equivalent.

A block or lambda literal needs a target that states the type; assigned to a bare `var` it fails
with `error[30200] initialization type can't be inferred`.

**A lambda or block parameter silently shadows an enclosing function parameter of the same name**;
the body sees the inner one. (probe-verified 2026-08-16)

### Capture and lifetime

Reads and writes inside a block hit the enclosing frame's original variables. A block can never
escape the frame it was made in — all compile errors:

| Attempt | Error |
|---|---|
| `def make : block<() : void>` | `30167` not a valid function return type |
| `var g_blk : block<() : void>` at module scope | `30179` can't have a global variable of that type |
| `var blk2 = blk` / `var blk2 <- blk` | `30198` can only be initialized with make block expression |
| `break` / `continue` inside a block, targeting an outer loop | `30125` captured block can't 'break' outside of the block |

Blocks may carry annotations between the sigil and the argument list: `$ [es] (var pos : float3&)`.

---

## Lambdas

```das
var cnt = 0
var counter <- @ capture(copy(cnt)) (extra : int) : int {
    return cnt++ + extra
}
print("{invoke(counter, 13)}\n")    // 13
print("{counter(14)}\n")            // 15
```

`<-` is the idiomatic initializer; `=` also compiles — same pointer copy.

### Copies alias the frame

```das
var alias_of = counter    // copies the pointer, NOT the frame
print("{alias_of(0)}\n")  // 2 — continues counter's captured cnt
print("{cnt}\n")          // 0 — captured by copy, the outer variable was never touched
```

So they store directly in containers, struct fields, and globals:

```das
struct Handler {
    name : string
    fn : lambda<(x : int) : int>
}

var h = Handler(name = "double", fn <- @(x : int) => x * 2)
var lams : array<lambda<(extra : int) : int>>
lams |> push(counter)                 // push, not push_clone — lambdas are copyable
```

`delete lam` frees a frame other copies may point at, so it needs `unsafe`
(`error[31009] delete of lambda<...> requires unsafe`); the gate cascades to anything containing a
lambda, so `delete` on an `array<lambda<...>>` is equally unsafe — and deleting an array holding two
copies of one lambda is a double free. No reference counting: a frame lives until the lambda is
deleted or the host collects/resets the owning context's heap, so undeleted lambdas in a
long-running loop leak.

### Capture modes

Without a `capture(...)` list, every referenced outer variable is captured **by copy**. If the type
is not copyable, the compiler falls back to move and rejects it until you write `unsafe` or an
explicit `clone(...)` (`error[31003] implicit capture by move requires unsafe, while capturing src`).

| Symbol form | Named form | Effect | Gate |
|---|---|---|---|
| `=x` | `copy(x)` | copy the value into the frame | type must be copyable |
| `<-x` | `move(x)` | move into the frame; **source is left empty** | type must be movable |
| `:=x` | `clone(x)` | deep copy into the frame | type must be cloneable |
| `&x` | `ref(x)` | frame holds a reference to the original | requires `unsafe` (dangling risk) |

Spellings are interchangeable; entries comma-separated.

```das
var a1 <- [1, 2]
var a2 <- [1, 2]
var a3 <- [1, 2]
unsafe {
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
(`error[30129] can't capture variable blk`).

**Capture freezes the binding, not what it points at.** A copy-captured `var p : T?` is still a
mutable pointer inside the frame: `p[i] = v` writes through to the original memory, and `p + off`
is still a mutable `T?`. (probe-verified 2026-08-16)

**An enclosing `unsafe` does not reach into a lambda body** — operations inside it need their own
wrap; a *block* body, running in the same frame, is covered by the outer wrap.

### Finalizer

A `finally` clause after the body runs on delete, before the default finalizer:

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

`@@` captures nothing — referencing an enclosing local from a `@@` body is
`error[30838] can't locate variable`. Declaration forms, the typed `@@<(...) : T> name` spelling,
and `invoke` are under functions.

```das
def apply(x : int; fn : function<(a : int) : int>) : int { return invoke(fn, x) }

apply(21, @@double_it)                     // named function
apply(10, @@(x : int) => x + 1)            // anonymous, arrow body, captures nothing
```

---

## Generators

A generator is a lambda compiled into a state machine; its value is an `iterator<T>`, so returning
one gives a lazy sequence. The body `yield`s elements and must end with `return false` — omitting it
is `error[30172] generator must return boolean`.

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

- `yield <- expr` move-yields, for element types that cannot be copied.
- `break`, `continue`, `while`, and nested `for` all work inside the body.
- `generator<T&>` yields references, so `for (t in gen) { t++ }` writes through to the source.
- The capture list has the same syntax and gates as a lambda's: default is copy, an implicit move
  needs `unsafe`, a `ref` capture needs `unsafe`.

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

A `finally` on a loop body inside a generator belongs to the **body block** — it runs once per
iteration, interleaved, not once when the loop ends:

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

Older block-literal spellings (`generator<int>() <| $ { ... }`, `generator<int>() <| $() { ... }`)
still parse; write the brace form.

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
verify(empty(it))
delete it
```

`for` closes and finalizes the iterator automatically; an explicit `delete` is only needed for one
you stopped driving early, and is always safe on a sequenced-out one.

**The element type says whether the loop variable writes through.** `each(array<T>)` yields
`iterator<T&>`, so `for (x in it) { x++ }` mutates the array; an iterator comprehension
(`[iterator for (...); expr]`) and a plain `generator<T>` yield `iterator<T>` — a value per step,
not a reference into any source. A generic parameter written `iterator<auto(TT) const>` takes both
flavors through one instantiation, at the cost of a body that may not move from or mutate the
elements. It must still be `var`: a non-`var` iterator parameter is const as a *handle* and cannot
be iterated at all (`error[30939] can't iterate over const iterator`). (probe-verified 2026-08-16)

### Making a type iterable

Define an `each` overload returning an `iterator`; the compiler inserts the call, so
`for (x in value)` works. Mark it `[unsafe_outside_of_for]` when the iterator does not keep its
source alive; direct calls then require `unsafe`
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
for (x in f) { print("x = {x}\n") }
```

Only `each` is auto-inserted; any other iterator-returning function is called explicitly:
`for (x in filtered(f))`.

### Lambda as an iterator

`each(lam)` turns a lambda taking exactly one reference out-parameter and returning `bool` into an
iterator (`true` = a value was produced, `false` = stop):

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
