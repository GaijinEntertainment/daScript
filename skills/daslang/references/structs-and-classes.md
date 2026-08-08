# Structs and Classes

**Structs** are pure data: no built-in method dispatch, value semantics, stack- or
heap-allocatable. **Classes** are structs plus a generated vtable of function-pointer fields:
virtual/abstract methods, initializers, finalizers. Both support single inheritance and both are
first-class values. All examples are gen2, the default parser.

## Struct declaration

```das
struct Foo {
    x, y : int              // one type shared by a comma group
    xf : float
}

struct private Hidden { a : int }   // visibility is a prefix keyword, not an annotation
```

Without a modifier a struct inherits its module's publicity. Field types are inferred from
initializers: `struct Foo { x = 1; y = 2.0 }` gives `int` and `float`.

## Struct initialization

A function whose name matches the struct is its initializer; the compiler generates a default one.

| Form | Meaning |
|---|---|
| `var b : Bar` | uninitialized declaration, zero-filled. Legal **only** if no field has an initializer |
| `Foo()` | run the initializer: every field gets its declared initializer, or zero |
| `Foo(x = 13)` | name only what changes; the rest keep their declared initializers |
| `unsafe(Foo(uninitialized x = 13))` | skip default-init of unnamed fields — hence `unsafe` |
| `new Foo(x = 1)` | same forms, on the current context heap; yields `Foo?` |
| `Foo(a <- expr)` | move-init one field (zeroes the source) |
| `Foo(a := expr)` | clone-init one field (source survives) |

```das
struct Foo { x : int = 1; y : float = 2.0 }

let fInit = Foo()                               // x = 1,  y = 2.0
let fPost = Foo(x = 13)                         // x = 13, y = 2.0
let fUninit = unsafe(Foo(uninitialized x = 13)) // x = 13, y is garbage
```

**The zero-init trap.** As soon as any field carries an initializer, a bare `var f : Foo` is
rejected with `error[31016]: Uninitialized variable f is unsafe`. Write `var f = Foo()` instead.
Note that `x, y : int = 0` *does* count as field initializers. `[safe_when_uninitialized]` on the
struct permits the bare declaration but does **not** apply the initializers — it zero-fills.

**Clone initializer** — an initializer taking a pointer to an existing instance, so that
`new Foo(a)` deep-copies `a`:

```das
def Foo(p : Foo?) {
    var self := *p          // := clones
    return <- self
}
```

For an array of structs, a plain array literal of initializer calls is the normal form:
`var fs <- [Foo(x = 11, y = 22.0), Foo(x = 33)]`. The older
`array struct<Foo>((x = 11, y = 22.0), (x = 33))` shorthand (bare field tuples, declared
initializers applied to the rest) still works.

## Methods on structs

Structs deliberately have no member functions. Four idioms replace them:

**1. Free function + pipe** — zero overhead, statically resolved.

```das
def setXY(var self : Point; X, Y : int) {
    with (self) { x = X; y = Y }    // brings fields into scope; no self. prefix
}

var p = Point()
p |> setXY(10, 11)          // sugar for setXY(p, 10, 11)
let n = p.length2()         // dot works on struct values too: length2(p)
```

`with (expr) { ... }` opens the fields of any struct expression (including a temporary) as bare
names; writes go back through the reference. There is no `with (var x = expr)` binding form.

**2. Function-pointer field** — this is what "virtual" means for a struct: an indirect call through
a data member, overridable per derived type. **3. An inline `def` in the struct body** is exactly
sugar for it — same field, same standalone function, body wrapped in `with (self)`.

```das
struct Vfoo { x, y : int = 0; set = @@setV }     // @@ makes a function pointer

struct Inline {
    x, y : int = 0
    def setXY(X, Y : int) { x = X; y = Y }       // generates the same field + function
}

v->set(1, 2)                // sugar for invoke(v.set, v, 1, 2)
i.setXY(1, 2)               // dot, arrow and raw invoke all work on the inline form
invoke(i.setXY, i, 5, 6)
```

**4. `[class_method]` from `daslib/class_boost`** — a direct (non-virtual, no function-pointer
field) method: the macro prepends a `self` argument and wraps the body in `with (self)`.

```das
require daslib/class_boost

struct Counter {
    value : int = 0
    [class_method]
    def static increment(amount : int = 1) { value += amount }
    [class_method]
    def static const get_value : int { return value }   // const self: cannot write fields
}

var c = Counter()
c.increment(10)
```

Use `[explicit_const_class_method]` when you need both a const and a non-const overload of one name
(typical for `foreach` or `operator []`).

Call forms for idioms 3 and 4: `obj.m(...)`, `ptr->m(...)`, and `` Type`m(obj, ...) ``. **`|>` does
not work** for either — the generated function is named `` Type`m ``, so pipe cannot find it.

## Struct inheritance

Single inheritance with `:`. The base's members are copied into the derived struct first, so a
derived struct is layout-compatible with its base.

```das
struct Base {
    x : int = 0
    def tag : int { return 1 }
}

struct Kid : Base {
    def override tag : int { return 2 }      // override is mandatory
}

def whichTag(var b : Base) : int => b->tag()

whichTag(Kid())     // 2 — dispatch goes through the function-pointer field
```

Three things can be overridden, each with its own spelling:

| What | Spelling |
|---|---|
| inline method | `def override tag : int { ... }` |
| function-pointer field | `override set = cast<auto>(@@Kid_setXY)` (cast needed: `self` type changed) |
| plain data field's initializer | `override typeTag : uint64 = hash("Kid")` |

Casting toward the base is safe; casting toward the derived type is not. `upcast` is named for the
direction in daslang's type lattice — in C++/Java terms it is a *downcast*, so guard it with a
discriminator field when the dynamic type is not statically known.

```das
cast<Base>(k).x = 5                 // safe, always valid
unsafe { upcast<Kid>(b).y = 5 }     // unchecked; corrupts memory if b is not really a Kid
```

`[cpp_layout(pod = false)]` on a struct and its descendants makes inherited alignment follow C++
class rules, for interop with native structures.

**Struct finalizers** are free functions named `operator delete` (or `finalize`) taking `self`:

```das
def operator delete(var self : Bar) {
    print("~Bar\n")
    delete super.self       // runs the parent struct's finalizer on self
}
```

Structs do **not** chain automatically: a derived finalizer that omits `delete super.self` silently
skips its ancestors' finalizers.

## Classes

Declared with `class`, allocated with `new`, reached through a pointer.

```das
class Shape {
    name : string
    static made : int = 0           // shared across all instances
    def Shape { name = "shape"; made ++ }       // initializer: name matches the class
    def abstract area : float       // must be fully qualified, including return type
    def describe : string { return "{name}: area = {area()}" }
    def static howMany : int { return made }    // no self; may touch static fields
}

class Circle : Shape {
    radius : float = 1.0
    def Circle(r : float) {
        super()                     // required: parent has a user constructor
        name = "circle"
        radius = r
    }
    def override area : float { return 3.14159 * radius * radius }
    def override describe : string { return "<{super.describe()}>" }
}
```

`self` is an implicit pointer inside every method and the body is wrapped in `with (self)`, so
fields and sibling methods are reachable without a prefix.

**Method modifiers** appear in a fixed order after `def`:

`def [public|private] [static] [override|sealed] [const] name(args) : Ret { ... }`

`override` and `sealed` are alternatives, never both — `sealed` already implies overriding, so
`def sealed override` is a syntax error. Abstract methods have their own form:
`def [public|private] abstract [const] name : Ret`. `class sealed Unit : Circle` bans further
inheritance (`error[20701]`); `def sealed area : float` overrides and bans further overriding
(`error[20107]`).

| Call form | Notes |
|---|---|
| `p.method(a)` | dot, on a value or a pointer |
| `p->method(a)` | arrow, identical to dot on a pointer |
| `` Type`method(*p, a) `` | explicit, non-virtual — calls that exact class's version |
| `p \|> method(a)` | **does not work** — there is no free function named `method` |

**`const` methods** take `self` as const: they may not write fields, and may not call non-const
methods (that would pass `Shape const` where `Shape` is expected — `error[30187]`).

**Local class instances are unsafe** — `var f = Foo()` gives `error[31017]: local class requires
unsafe`. Allocate with `new Foo()`; use `unsafe { var f = Foo() }` only when a stack instance is
genuinely wanted. `delete` on a class pointer also requires `unsafe` (`error[31009]`).

**Visibility** — `private` fields and `def private` methods are reachable only from inside the
class; touching one from outside is `error[30900]`.

### Constructors and `super`

`super()` calls the parent constructor; `super.method(args)` calls a parent method bypassing
virtual dispatch. Both rewrite to the backtick form (`` Base`Base(self) ``,
`` Base`process(self, x) ``). If the immediate parent has no match, `super` walks up to the nearest
ancestor that does, matching by argument types.

A derived constructor whose parent has a user constructor must call `super(...)` **exactly once on
every control-flow path** — zero calls, two calls, or a call inside a loop are all compile errors
(`error[30308]`). Two synthesis rules follow, and the second is a trap:

- A derived class with **no** constructor of its own gets a synthesized one that chains `super()`,
  so `new NoCtor()` runs the parent's constructor body.
- A class **with** a user constructor still keeps the generated named-field initializer. So
  `new WithCtor(z = 9)` performs plain field-init and **does not run** the user constructor or the
  parent's — leaving parent fields at zero. Use `new WithCtor(3)` when you want the constructor.

### Finalizers

`def finalize { ... }` or, equivalently, `def operator delete { ... }`. Classes chain level by
level; each level finalizes its own field slice exactly once.

```das
class Mid : Base {
    def operator delete {
        print("~Mid\n")         // derived cleanup first
        delete super.self       // then the parent's finalizer on self
    }
}
```

- A class with no finalizer of its own inherits chaining automatically — no forwarding boilerplate.
- When an ancestor has a user finalizer, a derived finalizer must call `delete super.self` exactly
  once per path (same zero/two/loop errors as `super(...)`). A hand-written
  `delete cast<Ancestor>(self)` that skips the immediate parent does not count.
- `delete` on a base-class pointer dispatches to the real dynamic type through the generated
  `__finalize` pointer: `delete b` where `b : Base? = new Leaf()` runs Leaf, then Mid, then Base.
- Finalizers are not guaranteed to run implicitly — `delete` is explicit.

### Operators as methods

Any overloadable operator can be a class method, declared with the usual modifiers — including
computed properties, whose read and write halves are separate methods:

```das
def const operator . len { return length(dir) }                 // read:  v.len
def operator . len := (k : float) { dir = normalize(dir) * k }  // write: v.len := 10.0
def const operator + (other : Vec) : Vec? { return new Vec(dir + other.dir) }
```

## Runtime type checks: `is`, `as`, `?as`

Class RTTI is **opt-in**. Require `daslib/dynamic_cast_rtti`, or `is`/`as`/`?as` on a class pointer
fails to compile with `error[30190]: is Dog only allowed for variants` — not, as older docs claim,
a silent compile-time-only check.

```das
require daslib/dynamic_cast_rtti

var a : Animal? = new Dog()
a is Dog                    // true  — walks the __rtti chain at runtime
a is Animal                 // true  — Dog is an Animal; a is Cat is false
var d = a as Dog            // forced cast: panics if the type is wrong
var c = a ?as Cat           // safe cast: null if the type is wrong
```

Use `?as` when the dynamic type is uncertain, `as` when a mismatch is a bug. This differs from
`is`/`as` on *handled* (C++-bound) types, where the check is exact-type and ignores C++ inheritance.

## Interfaces

`daslib/interfaces` gives polymorphism without a shared base class. Mark the interface (methods
only, no data fields) `[interface]` and each implementor `[implements(IName)]`; implementations are
named `` IName`method ``. Missing methods are a compile error. Interface inheritance
(`[interface] class IChild : IParent`) and non-abstract default methods are both supported.

```das
require daslib/interfaces

[interface]
class IDrawable {
    def abstract draw : string
}

[implements(IDrawable)]
class Sprite {
    tag : string
    def IDrawable`draw : string { return "sprite:{tag}" }
}

s is IDrawable              // true (resolved at compile time)
(s as IDrawable)->draw()    // through a lazily-created proxy
s->get`IDrawable()->draw()  // the explicit getter the macro generates
```

## Class templates

`class template` / `struct template` carrying `[template_structure(Params...)]` from
`daslib/typemacro_boost` is a parameterized blueprint, never instantiated directly; `$Name<Args>`
is a type macro that clones it per argument list (`typedef IntBox = $TBox<int>`). The template must
live in a **required module** — the type macro is registered by that module's macro pass, so
`$TBox<int>` in the file that declares `TBox` fails with `error[30821]: can't find typeMacro TBox`.

C++-bound handle types (not daslang classes) expose `using()`, which constructs the handle on the
stack and finalizes it at block exit: `using() $(var s : das_string) { s := "hello" }`.
