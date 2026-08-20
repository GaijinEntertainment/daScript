# Functions and Operators

## Declaration

`:` and `->` are equivalent; the return type may be omitted and inferred. Returning two
different types from one function is a compile error.

```das
def add(a, b : int) : int { return a + b }
def sub(a, b : int) -> int { return a - b }
def mul(a, b : int) { return a * b }     // inferred: int
def hello { print("hello\n") }           // no arguments -> parens optional (call site: hello())
```

Visibility is a **prefix keyword**: `def private helper(...)`, `def public api(...)`. Without it
a function inherits its module's publicity.

### Parameter list

A parameter list is a sequence of **groups**: `name[, name...] : Type` (all names share that one
type) or a bare untyped `name[, name...]` (each name independently generic).

| Separator | Legal after |
|---|---|
| `;` | any group |
| `,` | a **typed** group only |

```das
def a(x : int, y : float) => float(x) + y      // comma between two typed groups
def b(x, y : int) => x + y                     // ONE group: both x and y are int
def c(x; y : int) => x + y                     // untyped x (generic), then typed y
def d(x, y) => "{x}/{y}"                       // two untyped names, independently generic
```

Every `def` needs a body.

The parameter list may wrap, but **the return type must stay on the closing-paren line** — a
continuation line starting `: int {` is
`error[30151] syntax error, unexpected ':', expecting => or '{'`. (probe-verified 2026-08-16)

### var / let parameters

No keyword means const (`let` is the explicit, redundant spelling). `var` makes it mutable; add
`&` for a reference the caller sees written back.

```das
def bump(var a : int&) { a += 1 }     // caller's variable is modified
def fill(var out : float?; n : int) { unsafe(out[0]) = float(n) }
```

To write *through* a pointer parameter, declare it `var T?`; taking `T const?` and reinterpreting
the const away licenses the optimizer to drop the write.

### Single-expression (arrow) body

```das
def succ(x : int) => x + 1
def make_arr() : array<int> => <- [1, 2, 3]      // move-return
```

The body must **start on the `=>` line**; to wrap, open a paren there and break inside it:

```das
def blend(a, b : float) : float => (
    a * 0.25
    + b * 0.75
)
```

### Default parameters

Any parameter may have a default (a compile-time constant expression), including non-trailing
ones; a non-trailing default can only be skipped by a named call.

```das
def rect(x, y : int; w : int = 10; h : int = 20) : int => x + y + w * 100 + h * 1000
def lead(c : int = 1; d : int = 1; a, b : int) : int => a + b + c + d

rect(1, 2)                  // w=10, h=20
lead(a = 2, b = 3)          // c=1, d=1
```

## Calling

Named arguments are written **bare**, after all positional ones (a positional after a named one
is a syntax error) and in declaration order (out-of-order gives "no matching functions").

```das
rect(1, 2, w = 3)               // x=1, y=2, w=3, h=20
rect(1, 2, [w = 3, h = 4])      // legacy bracketed form, still valid
```

One `[...]` holds **all** named arguments; separate groups (`foo([a = 1], [b = 2])`) are a
syntax error. Prefer the bare form; it works on method calls too: `obj.m(bias = 1)`,
`obj->m(bias = 1)`.

**There is no angle-bracket call form.** `take<int>(1, 2)` is
`error[30151] syntax error, unexpected '>', expecting '('`. Pass the type as an ordinary
argument, `take(type<int>, 1, 2)`, against a parameter declared `t : type<auto(TT)>` (tag it
`[unused_argument(t)]` — a `type<>` parameter occupies no stack and cannot be read); take
`default<T>` when the body needs a value. (probe-verified 2026-08-16)

Defaults between the explicit arguments and a trailing block are padded automatically:

```das
def visit(tag : string; depth : int = 3; blk : block<(d : int) : void>) { invoke(blk, depth) }
visit("root") $(d : int) { print("depth={d}\n") }        // depth = 3
```

## Overloading and specialization

Overloads must differ in argument types; identical argument lists are a compile
error. Matching picks the **most specialized** candidate: non-`auto` beats `auto` (alias
base type ranks lowest, two aliases rank equal); among non-`auto`, no-cast beats cast; array
beats non-array; concrete element type beats abstract. Compound types (pointer, array, table,
tuple, variant, function/block/lambda) compare recursively by subtype and return type — all parts
must match or rank equally. Ties break on smallest *substitute distance* (+1 per argument needing
a substitution cast); a remaining tie is a compile error.

```das
def twice(a : int) : int => a + a           // 4
def twice(a : float) : float => a + a       // 4.0
def twice(a : auto[]) => length(a) * 2      // any fixed array
def twice(a) => "generic"                   // anything else
```

`explicit` on a parameter disables substitution — `def only_base(b : Base explicit)` accepts
`Base` but rejects a struct that inherits from it.

Contract annotations (`daslib/contracts`) restrict which types a generic matches, and combine
with `!`, `&&`, `||`, `^^`:

```das
require daslib/contracts
[expect_any_tuple(t) || expect_any_variant(t)]
def kind(t) : string => "tuple-or-variant"
```

## Function pointers

The type is `function<(args) : Ret>` (or `-> Ret`); bare `function` means an unspecified
signature. Take a pointer with `@@`. They are ordinary values — locals, arrays, table slots,
struct fields, parameters.

```das
let f = @@thrice                          // by name
let g = @@<(a : float) : float> twice     // pick one overload by signature
let h <- @@ (a : int) { return a + a }    // nameless
let k = @@(a : int) => a - 1              // nameless, arrow form
```

Call via `invoke(f, args)`, or with call notation on a **variable** holding the pointer (`f(3)`),
never on an arbitrary expression — `arr[0](7)` is a syntax error, use `invoke(arr[0], 7)`.

Nameless functions capture nothing — referring to an enclosing local inside one is a compile
error. `@@gen` on a generic (untyped-parameter) function reports "function not found".

## Pipes and method-style calls

| Form | Means |
|---|---|
| `x \|> f(y)` | `f(x, y)` — value becomes the **first** argument |
| `f(y) <\| x` | `f(y, x)` — value becomes the **last** argument |
| `f() { ... }` | assumed pipe: a trailing block/lambda is the last argument |
| `a.f(b)` | `f(a, b)` — only when `a` is a struct or class value |

```das
let t = 12 |> addX(2) |> addX(3)     // addX(addX(12,2),3) == 17
var f = Foo()
f.setXY(10, 11)                      // == f |> setXY(10,11) == setXY(f,10,11); f.area() is area(f)
```

**Assumed pipe** — parameterless blocks need no `$`; blocks with parameters keep `$`, lambdas
keep `@` / `@@`. Works after named calls, `obj.m()` and `obj->m()`. A non-block expression still
needs explicit `<|`, e.g. `<| new Foo()`.

```das
doSomething() {                                   // parameterless block
    print("hello\n")
}
arr |> sort() $(a, b) => a < b                    // arrow shorthand works here too
let res = r.call_method() $ : int { return 42 }   // also on method calls
```

**Dot limits.** On a primitive, tuple, array, or a `lambda` typedef, `a.f(b)` is a field lookup
and fails (`can't get field 'double' of int const&`); those receivers need `|>` or a direct
call. Conversely, **class
METHODS are invisible to `|>`**: `obj |> method()` fails with "no matching functions", and so do
`[class_method]` struct methods.

## Operator overloading

Define a free function or a struct/class method named `operator <op>`.

```das
struct Vec2 { x, y : float }
def operator + (a, b : Vec2) : Vec2 => Vec2(x = a.x + b.x, y = a.y + b.y)
def operator - (a : Vec2) : Vec2 => Vec2(x = -a.x, y = -a.y)     // unary: one argument
```

| Category | Spellings |
|---|---|
| Arithmetic | `+` `-` `*` `/` `%` |
| Comparison | `==` `!=` `<` `>` `<=` `>=` |
| Bitwise / shift | `&` `\|` `^` `~` `<<` `>>` `<<<` `>>>` |
| Logical | `&&` `\|\|` `^^` `!` |
| Increment / decrement | `++operator` (prefix), `operator++` (postfix); same for `--` |
| Compound assign | `+=` `-=` `*=` `/=` `%=` `&=` `\|=` `^=` `<<=` `>>=` `<<<=` `>>>=` `&&=` `\|\|=` `^^=` |
| Index | `[]` `[]=` `[]<-` `[]:=` `?[]`, plus `[]` + each compound-assign spelling (`[]+=` ...) |
| Field | `.` `?.` `. name` `?. name` `. name :=`, plus `. name` + each compound-assign spelling |
| Clone / finalize | `:=` (alias for `clone`), `delete` (alias for `finalize`) |
| Type test / cast | `is` `as` `?as`, and the named forms `is Name` / `as Name` / `?as Name` |
| Null coalesce | `??` |
| Interval | `..` (alias for `interval`) |

Compound-assign operators take a mutable reference first.

```das
def ++operator (var c : Counter&) : Counter { c.value += 1; return c }
def operator++ (var c : Counter&) : Counter { let old = c; c.value += 1; return old }

def operator [] (m : Mat; i : int) : float => m.data[i]
def operator []= (var m : Mat&; i : int; v : float) { m.data[i] = v }
def operator ?[] (var s : Sparse; i : int) : int? {
    return i >= 0 && i < length(s.data) ? unsafe(addr(s.data[i])) : null
}
```

For `is` / `as` / `?as` the target type is part of the **operator name**, not a `type<T>`
parameter.

```das
def operator is Cat (a : Animal) : bool => a.isCat
def operator as Cat (a : Animal) : Cat => a.cat
def operator ?as Cat (var a : Animal) : Cat? { return a.isCat ? unsafe(addr(a.cat)) : null }

def operator := (var dst : Res; src : Res) { dst.name = src.name; dst.n = src.n + 1 }
def operator delete (var r : Res) { print("releasing {r.name}\n") }
def operator ?? (a : Opt; d : int) : int => a.has ? a.val : d
def operator .. (a, b : Deg) : range => range(a.d, b.d)
```

### Field access and computed properties

`operator .` overloads generically (the field name arrives as a `string`) or per name. Inside
it, `t!.field` reaches the real field, bypassing the overload. A read/write pair makes a
computed property.

```das
struct Goo { a : string }
def operator . (t : Goo; name : string) : string => "{name}={t!.a}"   // any field name
def operator . size (t : Goo) : int => length(t!.a)                   // just '.size'

def operator . magnitude (b : Ball) : float => length(b.dir)
def operator . magnitude := (var b : Ball; value : float) { b.dir = normalize(b.dir) * value }
// ball.magnitude reads, ball.magnitude := 10.0 writes
```

### Original-operator access (`!`)

`!` in front of any overloadable access or test operator yields the **original** operator, never
the overload: `a!.x`, `a!?.x`, `a![i]`, `a!?[i]`, `a !?? b`, `a !is x`, `a !as x`, `a !?as x`.
The variant and coalescing forms also bypass variant macros. Legacy spelling: `t . .field` ==
`t!.field`, the space required because `t..field` lexes as the interval operator.

Mark read-only method operators `const`:

```das
struct Bag {
    items : array<int>
    def const operator [] (index : int) : int => items[index]
    def operator []= (index : int; value : int) { items[index] = value }
}
```

## Operator precedence

Highest binding first; left-associative unless noted.

```
::                                       namespace
$  @  @@                                 block / lambda / fn-pointer introducers
(  )  [  ]  ?.  ?[                       call, index, safe field, safe index
.
x++  x--  ->  *(deref)
|>  <|
is  as
-  +  ~  !  ++x  --x                     prefix unary   (right)
??                                                      (right)
*  /  %
+  -
<<  >>  <<<  >>>
<  >  <=  >=
==  !=
&
^
|
&&
^^
||
?  :                                     ternary        (right)
=  +=  -=  *=  /=  %=  &=  |=  ^=  <<=  >>=  <<<=  >>>=  &&=  ||=  ^^=  <-  :=   (right)
..  =>                                   interval, tuple-maker (non-assoc)
,
```

So `p ?? 2 * 10` is `(p ?? 2) * 10`; `2 * 3 |> inc()` is `2 * inc(3)`, not `inc(2 * 3)`;
`!v is i` is `!(v is i)`.

## Also

Recursion is supported, with no tail-call optimization — deep recursion consumes stack.

See *closures* (blocks, lambdas, generators, capture), *generics* (`auto`, `auto(TT)`, OR-types,
`static_if`, `_::name` resolution, type contracts), *structs and classes* (methods, `->`,
`super`, abstract/override).
