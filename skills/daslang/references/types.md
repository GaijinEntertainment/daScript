# Types

Statically typed, everything zero-initialized.

## Type catalog

| Group | Types |
|---|---|
| Workhorse scalars | `bool`, `int`, `uint`, `int64`, `uint64`, `float`, `double`, `string` |
| Vectors | `int2/3/4`, `uint2/3/4`, `float2/3/4` |
| Ranges | `range`, `urange`, `range64`, `urange64` |
| Storage scalars | `int8`, `uint8`, `int16`, `uint16` — no arithmetic of their own |
| 16/8-bit lattice | `float16` (`half`), `half2/3/4/8`, `short2/3/4/8`, `ushort2/3/4/8`, `byte2/3/4/8/16`, `ubyte2/3/4/8/16` |
| Nominal | `enum`, `bitfield`, `struct`, `class`, `typedef distinct` |
| Structural | `tuple`, `variant`, fixed arrays `T[N]` |
| Containers | `array<T>`, `table<K;V>`, `table<T>` (set form) |
| Callable | `block`, `lambda`, `function` — see closures.md |
| Indirection | `T?` pointer, `T&` reference, `T?#` temporary, `smart_ptr<T>`, `iterator<T>` |
| Mutable string | `das_string`, a bound C++ `std::string`; compares with `string` directly, never wrapped in `string(...)` (probe-verified 2026-08-16) |

The first five groups pass by value, everything else by reference; `string` is immutable, a
`const char*`.

## Literals

| Literal | Type |
|---|---|
| `34` | `int` |
| `075` | `int` — NOT octal, this is 75; leading zeros are insignificant |
| `0xFF00A120` | `uint` — hex is unsigned by default; write `int(0x3F)` for `int` |
| `'a'` | `int` — character literals are integers |
| `34u` / `13l` / `0xFF00A120ul` / `32u8` | `uint` / `int64` / `uint64` / `uint8` |
| `1.52`, `1.0f`, `1.e-2` | `float` |
| `1.52d`, `1.e2lf` | `double` |
| `1.5h` | `float16` |

Strings use `"..."`, may span lines, escapes `\t \n \r \\ \" \' \b \f \v \xHH \uHHHH
\UHHHHHHHH`, and interpolate `{expr}` with an optional format spec (`"{pi:5.2f}"`). A nested
literal inside an interpolation stays plain — `"{s == "abc"}"`; escaping it (`"{s == \"abc\"}"`)
is a syntax error.

**Character literals take a smaller escape set** — `\b \t \n \f \r \\ \'` only. `'\v'` is
`error[30151] syntax error, unexpected invalid token` though `"\v"` works; write the number,
`11`. (probe-verified 2026-08-16)

## Conversions

**No implicit value-to-value conversion**: `int + float` is a compile error. Cast by calling the
target type — `float(i)`, `int(3.7)` truncates toward zero — any numeric to any numeric.

Bare *integer literals* promote to a known target type when the value fits: local/global
initializers, struct field declarations, struct-ctor field values, variant-arm values, `=`,
`:=`, compound assignment, either side of a binary operator, `return`. Call arguments,
parameter defaults, and `<-` do **not** promote — write `foo(1.0)`, never `foo(1)`.

Integer and bitfield targets are range-checked (`var d : uint8 = 256` is an error);
`float`/`double` targets always accept, so a literal above 2^24 silently loses precision in a
`float`. Enums cast out with `int(e)`; back in needs `unsafe(reinterpret<Color>(1))`.

To text: `string(x)` or interpolation. There is no cast back — `int("123")` and `bool(x)` do
**not** exist; use `to_int` / `try_to_int` (strings.md) and `x != 0`.

## Vectors, swizzles, and the 16/8-bit lattice

Swizzles read in any order (`v.zyx`); only sequential ones are writable — `w.xy =
float2(9.0, 8.0)` on a `float4 w`.

`xyzw` caps at 4 lanes. Vectors also take OpenCL `.s` swizzles — `s` plus one hex lane digit per
output lane, repeats allowed (`v.s3210`, `b16.sf`); 8- and 16-lane forms add `.lo` / `.hi`. The
two namespaces never mix in one mask.

Lattice vectors are tightly packed (`half3` is 6 bytes) and pass by value. `byte` is **signed**
int8, `ubyte` unsigned.

- **fp16 arithmetic is closed**: `half4 * half4 + half4` is a `half4`, bit-identical to native
  fp16. `half4(f4)` / `float4(h4)` convert; `half8(lo_float4, hi_float4)` packs.
- **The integer families are storage, converts, and bits only** — `byte4 + byte4` and ordering
  compares are compile errors. Widen (`int4(b4)` sign-extends, `uint4(us4)`) and narrow
  (`short4(i4)`, C truncation) with the target constructor, or saturate (`short4_sat`,
  `byte8_sat`). `<< >> & | ^` and compound assigns work, scalar `int` shift count masked to the
  lane width.
- 16-lane 8-bit forms carry exact dot products (`idot4(a, b)` -> `int4`, `idot4(acc, a, b)`
  accumulates, `idot(a, b)` reduces to one `int`) and `shuffle(lut, idx)` — bytes out of a
  16-entry `byte16` by `ubyte16` index.

## Enumerations

```das
enum Numbers {
    zero            // 0
    one             // 1
    ten = 9 + 1     // explicit compile-time constant
}
enum private Chars : uint8 { ch_a = 'A' }  // storage: int/int8/int16/uint/uint8/uint16
```

Values take a dot (`Numbers.one`). An enum name is its own strong type; out-of-range values
truncate to the storage type. `require daslib/enum_trait` enables `for (x in type<Chars>)`.
Enum and bitfield bodies separate entries by newline **or** comma (`enum E { A, B }`) but,
unlike struct and tuple bodies, reject `;`: `enum E { A; B }` is `error[30151] syntax error,
unexpected ';', expecting '}'`. (probe-verified 2026-08-16)

## Bitfields

Each named flag is one bit; storage is `uint` unless declared `uint8`/`uint16`/`uint64`.

```das
bitfield Perms {
    read
    write
    execute
    all  = Perms.read | Perms.write | Perms.execute
    none = bitfield(0)
}

var t : Perms = Perms.read | Perms.write
```

Flags read and write like `bool` fields: prefer `bf.flag` over `int(bf & T.flag) != 0`, and
`bf.flag = true` / `= false` over `|=` / `&= ~`. Build from an integer with `bitfield(1 << 1)` (or
`bitfield8` / `bitfield16` / `bitfield64`). Two bitfields are the same type only when flag list
**and** storage type match. Anonymous forms: `bitfield<one; two>`, `bitfield : uint8 <one; two>`.

A single flag carries the named type, but the **OR of two infers as a bare `bitfield`**:
`var t = Perms.read | Perms.write` then rejects `t.read` ("can't get bit field 'read' in
bitfield"). Annotate the declaration or assign into an already-typed variable.

## Tuples

```das
var a = (1, 2.0, "3")                  // tuple<int;float;string>
a._0 = 42                              // positional access
var named : tuple<i:int; f:float> = (i = 1, f = 2.0)
named.i = 5                            // named access; named._1 also works
let pair = "one" => 1                  // '=>' builds a 2-tuple, in any context
var (x, y, z) = (1, 2.0, "3")          // destructuring declaration
let (_, _, third) = (1, 2.0, "3")      // `_` discards a position
for ((p, q) in arr) { ... }            // destructuring iteration
```

Each destructured name binds like a `let`: a name already in scope, or repeated in a pattern, is
`error[30704] can't destructure into <name>` (`error[30708]` in the `for` form); only `_`
repeats freely. `options allow_local_variable_shadowing = true` does not lift it, and names
taken by an `assume` alias are refused too ("name already taken by alias").
(probe-verified 2026-08-16)

Field names are part of the type: `tuple<int;float>` and `tuple<i:int;f:float>` do not assign to
each other. A positional literal of *all* bare variable names may promote to a matching named
tuple (`(eid, distSq)` for `tuple<eid:int; distSq:float>`); a name mismatch is a compile error.
Mixing named and positional fields in one literal is unsupported.

## Variants

```das
variant Value {
    i : int
    f : float
    s : string
}

var v = Value(i = 42)
print("{v is i} {v as i}\n")        // true 42 — 'is' tests, 'as' extracts
v = Value(s = "hello")              // assigning a whole variant switches the case
let n = v ?as i ?? -1               // safe extract with fallback: -1
print("{variant_index(v)}\n")       // 2
```

`v as f` on the wrong case **panics**; `?as` yields a nullable, pair it with `??`. Compile-time
indices: `typeinfo variant_index<f>(v)` (compile error on an unknown name),
`typeinfo safe_variant_index<nope>(v)` (`-1` instead). A case-field write (`v.i = 7`) and
`set_variant_index(v, n)` each need `unsafe` and neither implies the other: a field write does
not change the index. Anonymous form `variant<i_value:uint; f_value:float>`; two variants match
when named cases, types, and order all match.

## Fixed arrays

`T[N]` is one type node per dimension, outermost first: `float[4][4]` is 4 rows of `float[4]`.

```das
var m : float[4][4]
m[1][2] = 5.0                      // m[1] is a float[4] row; 'for (row in m)' walks the 4 rows
typedef M4 = float[4][4]
var stack : M4[10]                 // float[10][4][4]
```

Operations act on the **outermost level only** (the one-peel rule): `typeinfo dim(x)` is the
outermost dimension; in generics `auto(TT)` binds the whole array while `auto(TT)[]` peels one
level and `TT - []` in a return type removes one; `safe_addr(arr)` decays to a pointer to the
first element (`int[4]` -> `int?#`).

Unlike `array<T>`, a fixed array of copyable elements copies with plain assignment. Build with
`fixed_array(1, 2, 3, 4)` or `fixed_array<float>(1.0, 2.0)`. Range indexing yields a dynamic
sub-array: `let b <- a[1..3]` is `array<int>`.

## Pointers

| Spelling | Meaning |
|---|---|
| `T?` | nullable pointer, `null` by default |
| `T const?` | the **pointee** is const |
| `T? const` | the **pointer** is const — this is what `let` / a non-`var` parameter adds |
| `T?#` | temporary pointer, cannot escape its scope |
| `void?` | untyped; `reinterpret` before use |

```das
var pt = new Point(x = 1.0, y = 2.0)   // heap allocation; pt is Point?
print("{pt.x}\n")                       // '.' auto-dereferences; '->' is only the method arrow
print("{*pt} {deref(pt)}\n")            // both panic on null
print("{pt?.x ?? -1.0}\n")              // '?.' yields null instead of panicking
unsafe { delete pt }                    // frees and nulls
```

Safe without `unsafe`: `new`, `*p` / `deref(p)`, `p.field`, `p?.field`, `p ?? default`,
`safe_addr(x)` (from `daslib/safe_addr`; `x` must be a local or global), `intptr(p)` (address as
`uint64`). Needs `unsafe`: `addr(x)`, `delete p`, `p[i]`, `++p` / `p += n`, `reinterpret<T>(x)`.

**A `void?` carries no stride, so arithmetic on one is refused outright** — `error[30950]
operations on 'void' pointers are prohibited`, even inside `unsafe`. Do byte math on `intptr(p)`,
or reinterpret to `uint8?` first. (probe-verified 2026-08-16)

**Writing through a pointer needs both const positions open** — a non-const pointee *and* a
`var` handle: `def f(var p : float?)` stores, while a plain `p : float?` parameter is
`float? const` and rejects the store. Const flows from the handle through deref, index, and
field access. Never take a writable pointer as `T const?` and `reinterpret` the const away —
the const type already licensed optimizations that can delete the write. Declare `var T?`
(memory.md).

`addr<T?>(x)` is sugar for `reinterpret<T?>(addr(x))` under one `unsafe` gate; the target must
be a pointer type. Casts are call-style — `reinterpret<int>(f)`, never `reinterpret<int> f` (a
syntax error). A non-const `T?` is accepted where the parameter is `T const?` (one-directional,
top level only; also for a pointer field of a tuple passed directly as an argument).

`smart_ptr<T>` is reference-counted, exists only for C++-registered handled types (never for
daslang structs or classes), moves with `<-`, and is declared `var inscope`. Compiler AST node
types are *not* smart pointers — they are garbage-collected raw pointers (macros.md).

## Temporary types

A `#` marks a value borrowed from C++ that must not outlive its scope — `string#`, `int?#`,
`array<uint8>#`. Temporaries cannot be copied (`s = boo` errors), moved, returned, or passed
where a regular value is expected; they *can* be cloned with `:=`, which is how a value escapes.
A parameter marked `implicit` takes both flavors, on the promise it never caches the value
(generics.md).

## Distinct types

A nominal newtype over a workhorse type: identical ABI, no interconversion, fully erased at
runtime.

```das
typedef distinct EntityId = int
typedef distinct Meters   = float

var id = EntityId(5)      // only way in: one arg of exactly the underlying type
print("{*id}\n")          // only way out: '*' peels one distinct level, yields a reference
*id = 7                   // a 'var' handle derefs writable

def operator + (a, b : Meters) : Meters => Meters(*a + *b)
```

Only `==` and `!=` are borrowed (same distinct both sides); define the rest yourself. Overloads
on a distinct and on its underlying type coexist. Two distincts over the same underlying type
are unrelated, as are same-named distincts in two modules. `var id : EntityId` and
`default<EntityId>` zero-initialize. Distinct types cannot be table keys but work as array
elements, struct fields, and tuple/variant members. Printing and RTTI show the underlying value.
For a distinct over a pointer, `*` peels the distinct, not the pointer.
`typedef private distinct Foo = int` limits visibility to the declaring module.

## Type aliases

`typedef Registry = table<string; array<int>>` is a transparent name, interchangeable with what
it names, not a new type. Without explicit `typedef public` / `typedef private` an alias inherits
the module's publicity. Aliases may be declared inside a function body (scoped to the block) or a
struct/class body. `tuple Name { ... }`, `variant Name { ... }`, and `bitfield Name { ... }` are
shorthand aliases to the corresponding anonymous forms.

## Compile-time type expressions

| Form | Meaning |
|---|---|
| `type<T>` | a no-storage type tag; may be passed on but not read ("type expression result is used, and not just passed") |
| `default<T>` | a real value: zeros **plus** field initializers (`default<Foo>.x` is `7` for `struct Foo { x : int = 7 }`); of a tuple/variant it is plain zero, not a deep initialize |
| `typedecl(expr)` | the type of an expression, usable in type positions: `var t : table<typedecl(key); string>` |
| `typeinfo <trait>(x)` | compile-time query; the trait name goes *outside* the parens: `typeinfo typename(x)`, `typeinfo sizeof(type<Foo>)`, `typeinfo dim(arr)` |

`var f : Foo` is rejected ("uninitialized variable is unsafe") when `Foo` has field initializers
— write `var f = Foo()`; a struct with none declares fine and is zeroed.

Trait catalog, `static_if`, and generic type contracts (`-const`, `-&`, `-[]`, `==const`,
`explicit`, `implicit`, OR-types): generics.md.
