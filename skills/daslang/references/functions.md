# Functions and Operators

Declaration syntax, overload resolution, function pointers, pipes, the operator-overloading
catalog, and operator precedence.

## Declaration

The return type is written with `:` or `->` (equivalent), and may be omitted — daslang infers
it. Returning two different types from one function is a compile error.

```das
def add(a, b : int) : int { return a + b }
def sub(a, b : int) -> int { return a - b }
def mul(a, b : int) { return a * b }        // inferred: int
def hello {                                 // no arguments -> parentheses optional
    print("hello\n")                        // (the call site still needs them: hello())
}
```

Visibility is a **prefix keyword**, not an annotation: `def private helper(...)`,
`def public api(...)`. Without it a function inherits its module's publicity.

### Parameter list

A parameter list is a sequence of **groups**. A group is either `name[, name...] : Type`
(all names share that one type) or a bare untyped `name[, name...]` (each name is
independently generic).

| Separator | Legal after |
|---|---|
| `;` | any group |
| `,` | a **typed** group only |

```das
def a(x : int, y : float) => float(x) + y      // comma between two typed groups
def b(x : int; y : float) => float(x) + y      // semicolon, same meaning
def c(x, y : int) => x + y                     // ONE group: both x and y are int
def d(x, y : int, z : float) => float(x+y) + z // typed group, then another typed group
def e(x; y : int) => x + y                     // untyped x (generic), then typed y
def f(x, y) => "{x}/{y}"                       // two untyped names, independently generic
```

Not: "semicolon is the separator and comma is an error" — both work. But note `c` above:
`x, y : int` does **not** mean "generic `x`, int `y`" — it means both are `int`. To leave `x`
generic you must use `;` (form `e`). Every `def` needs a body; there are no bodiless
declarations.

### var / let parameters

A parameter with no keyword is const (`let` is the explicit, redundant spelling). `var` makes
it mutable; add `&` for a reference the caller sees written back.

```das
def peek(a : int) : int => a          // a is int const
def bump(var a : int&) { a += 1 }     // caller's variable is modified
def fill(var out : float?; n : int) { unsafe(out[0]) = float(n) }
```

To write *through* a pointer parameter, declare it `var T?`. Do not take `T const?` and
reinterpret the const away — the optimizer is licensed to drop the write.

### Single-expression (arrow) body

```das
def succ(x : int) => x + 1
def greeting(name : string) : string => "hello, {name}"
def make_arr() : array<int> => <- [1, 2, 3]      // move-return
```

The body must **start on the `=>` line** — a newline right after `=>` is a syntax error. To wrap
a long body, open a paren on the `=>` line and break inside it:

```das
def blend(a, b : float) : float => (
    a * 0.25
    + b * 0.75
)
```

Struct and class methods use the same form: `def area() : int => w * h`.

### Default parameters

Any parameter may have a default (a compile-time constant expression), including
non-trailing ones. Defaults on non-trailing parameters can only be skipped by a named call.

```das
def rect(x, y : int; w : int = 10; h : int = 20) : int => x + y + w * 100 + h * 1000
def lead(c : int = 1; d : int = 1; a, b : int) : int => a + b + c + d

rect(1, 2)                  // w=10, h=20
lead(a = 2, b = 3)          // c=1, d=1
```

## Calling

Named arguments are written **bare**, after the positional ones: `foo(pos, name = value)`.
They must be a strict suffix (a positional after a named one is a syntax error) and must stay
in declaration order (out-of-order gives "no matching functions").

```das
rect(1, 2, w = 3)               // x=1, y=2, w=3, h=20
rect(x = 1, y = 2)              // all named
rect(1, 2, [w = 3, h = 4])      // legacy bracketed form, still valid
```

The bracketed form groups **all** named arguments in one `[...]`; separate groups
(`foo([a = 1], [b = 2])`) are a syntax error. Prefer the bare form. Named arguments also work
on method calls: `obj.m(bias = 1)`, `obj->m(bias = 1)`.

Defaults sitting between the explicit arguments and a trailing block are padded automatically —
don't spell them out:

```das
def visit(tag : string; depth : int = 3; blk : block<(d : int) : void>) { invoke(blk, depth) }
visit("root") $(d : int) { print("depth={d}\n") }        // depth = 3
visit("root", 7) $(d : int) { print("depth={d}\n") }     // depth = 7
```

## Overloading and specialization

Two functions may share a name if their argument types differ; identical argument lists are a
compile error. Matching picks the **most specialized** candidate: non-`auto` beats `auto` (an
alias base type ranks lowest, two aliases rank equal); among non-`auto`, the one needing no cast
wins; array forms beat non-array forms, and a concrete element type beats an abstract one.
Compound types (pointer, array, table, tuple, variant, function/block/lambda) are compared by
subtype and return type recursively, and all parts must match or rank equally. Among equally
specialized candidates the one with the smallest *substitute distance* wins (+1 per argument
needing a substitution cast); a remaining tie is a compile error.

```das
def twice(a : int) : int => a + a           // 4
def twice(a : float) : float => a + a       // 4.0
def twice(a : auto[]) => length(a) * 2      // any fixed array
def twice(a) => "generic"                   // anything else
```

`explicit` on a parameter disables substitution — `def only_base(b : Base explicit)` accepts
`Base` but rejects a struct that inherits from it.

Contract annotations (from `daslib/contracts`) restrict which types a generic will match, and
combine with `!`, `&&`, `||`, `^^`:

```das
require daslib/contracts
[expect_any_array(blah)]
def count_any(blah) : int => length(blah)
[expect_any_tuple(t) || expect_any_variant(t)]
def kind(t) : string => "tuple-or-variant"
```

Generic parameters (`auto`, `auto(TT)`), OR-types and `static_if` dispatch: see *generics*.

## Function pointers

The type is `function<(args) : Ret>` (or `-> Ret`); bare `function` means an unspecified
signature. Take a pointer with `@@`.

```das
let f = @@thrice                          // by name
let g = @@<(a : float) : float> twice     // pick one overload by signature
let h <- @@ (a : int) { return a + a }    // nameless
let k = @@(a : int) => a - 1              // nameless, arrow form
```

Call via `invoke(f, args)`, or with call notation on a **variable** holding the pointer
(`f(3)`). Call notation does not work on an arbitrary expression — `arr[0](7)` is a syntax
error, use `invoke(arr[0], 7)`.

Nameless functions capture nothing — referring to an enclosing local inside one is a compile
error (that is what distinguishes them from lambdas). You cannot take the address of a generic
function: `@@gen` where `gen` has untyped parameters reports "function not found".

Function pointers are ordinary values — store them in locals, arrays, table slots and struct
fields, and take them as parameters:

```das
def apply(fn : function<(a : int) : int>; v : int) : int => invoke(fn, v)
```

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
f |> setXY(10, 11)                   // setXY(f, 10, 11)
f.setXY(10, 11)                      // same; chains too: f.area() is area(f)
```

**Assumed pipe** — a block or lambda written straight after a call is piped in as the last
argument, so `<|` is not needed. Parameterless blocks need no `$` either; blocks with parameters
keep `$`, lambdas keep `@` / `@@`. It works after named calls, dot-method calls (`obj.m()`) and
arrow-method calls (`obj->m()`). Explicit `<|` is still needed to pipe a non-block expression,
e.g. `<| new Foo()`.

```das
doSomething() {                                   // parameterless block
    print("hello\n")
}
let s = build_string() $(var w : StringBuilderWriter) {
    w |> write("built")
}
arr |> sort() $(a, b) => a < b                    // arrow shorthand works here too
let res = r.call_method() $ : int { return 42 }   // also on method calls
```

**Dot-as-pseudo-pipe limits.** `a.f(b)` only rewrites to `f(a, b)` when `a` is a struct or class
value. On a primitive, tuple, array, or a `lambda` typedef it is a field lookup and fails
(`can't get field 'double' of int const&`). Those receivers must use `|>` or a direct call.

The reverse carve-out: **class METHODS are not reachable through `|>`**. `obj.method()` and
`obj->method()` work, but `obj |> method()` fails with "no matching functions" — the generated
method function is invisible to pipe — and the same holds for `[class_method]` struct methods.
Pipe is for free functions; dot and arrow are for methods.

## Operator overloading

Define a free function or a struct/class method named `operator <op>`. The compiler dispatches
`a + b` to `operator +` and so on.

```das
struct Vec2 { x, y : float }
def operator + (a, b : Vec2) : Vec2 => Vec2(x = a.x + b.x, y = a.y + b.y)
def operator - (a : Vec2) : Vec2 => Vec2(x = -a.x, y = -a.y)     // unary: one argument
def operator == (a, b : Vec2) : bool => a.x == b.x && a.y == b.y
def operator += (var a : Vec2&; b : Vec2) { a.x += b.x; a.y += b.y }
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

Compound-assign operators take a mutable reference first. Increment is two separate operators:
`++operator` is prefix `++c`, `operator++` is postfix `c++`. Index reads and writes are separate
too; `?[]` is the null-safe read.

```das
def ++operator (var c : Counter&) : Counter { c.value += 1; return c }
def operator++ (var c : Counter&) : Counter { let old = c; c.value += 1; return old }

def operator [] (m : Mat; i : int) : float => m.data[i]
def operator []= (var m : Mat&; i : int; v : float) { m.data[i] = v }
def operator ?[] (var s : Sparse; i : int) : int? {
    return i >= 0 && i < length(s.data) ? unsafe(addr(s.data[i])) : null
}
```

For `is` / `as` / `?as` the target type is part of the **operator name** — not a `type<T>`
parameter. Call sites then read `a is Cat`, `(a as Cat).name`, `a ?as Cat`.

```das
def operator is Cat (a : Animal) : bool => a.isCat
def operator as Cat (a : Animal) : Cat => a.cat
def operator ?as Cat (var a : Animal) : Cat? { return a.isCat ? unsafe(addr(a.cat)) : null }

def operator := (var dst : Res; src : Res) { dst.name = src.name; dst.n = src.n + 1 }
def operator delete (var r : Res) { print("releasing {r.name}\n") }   // same as 'def finalize'
def operator ?? (a : Opt; d : int) : int => a.has ? a.val : d
def operator .. (a, b : Deg) : range => range(a.d, b.d)              // same as 'def interval'
```

### Field access and computed properties

`operator .` can be overloaded generically (the field name arrives as a `string`) or per name.
Inside such an operator, `t . . field` reaches the real field, bypassing the overload — the
spaces are required, since `t..field` lexes as the interval operator. A read/write pair makes a
computed property; the setter is `operator . name :=`, invoked with `:=` at the call site.

```das
struct Goo { a : string }
def operator . (t : Goo; name : string) : string => "{name}={t . . a}"   // any field name
def operator . size (t : Goo) : int => length(t . . a)                   // just '.size'
// with a = "hello": g.whatever is "whatever=hello", g.size is 5

def operator . magnitude (b : Ball) : float => length(b.dir)
def operator . magnitude := (var b : Ball; value : float) { b.dir = normalize(b.dir) * value }
// ball.magnitude reads, ball.magnitude := 10.0 writes
```

Operators also work as struct/class methods — mark read-only ones `const`:

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

Three rows bite people coming from C-family languages:

- **`??` binds tighter than `*` `/` `%`** (and everything below). `p ?? 2 * 10` is `(p ?? 2) * 10`.
- **Pipes bind tighter than all arithmetic.** `2 * 3 |> inc()` is `2 * inc(3)`, not `inc(2 * 3)`.
- **`is` / `as` bind tighter than prefix `!`.** `!v is i` is `!(v is i)`.

## Also

- Recursion is supported; there is no tail-call optimization — a deep recursion consumes stack.
- Blocks, lambdas, generators and their capture rules: see *closures*.
- `_::name` call resolution, generic instantiation and type contracts: see *generics*.
- Class methods, `->` calls, `super`, abstract/override: see *structs and classes*.
