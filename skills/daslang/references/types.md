# Types

daslang is strongly and statically typed: every value has a type, there are no implicit value
conversions, and all types are zero-initialized by default. Syntax below is gen2, the parser
default (legacy files opt out with `options gen2 = false`; new files conventionally still
start with `options gen2`).

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
| Mutable string | `das_string`, a bound C++ `std::string` |

The first five groups pass by value; everything else passes by reference. `string` behaves
like `const char*`: immutable, passed by value (the value *is* the reference). `length(s)` and
`empty(s)` need no `require`; the rest of the string surface needs `require strings`.
Container operations live in modules-and-stdlib.md.

## Literals

| Literal | Type |
|---|---|
| `34` | `int` |
| `075` | `int` — leading zeros insignificant, this is 75, NOT octal |
| `0xFF00A120` | `uint` — hex is unsigned by default; write `int(0x3F)` for `int` |
| `'a'` | `int` — character literals are integers |
| `34u` / `13l` / `0xFF00A120ul` / `32u8` | `uint` / `int64` / `uint64` / `uint8` |
| `1.52`, `1.0f`, `1.e-2` | `float` |
| `1.52d`, `1.e2lf` | `double` |
| `1.5h` | `float16` |

Strings use `"..."`, may span lines, and take escapes (`\t \n \r \\ \" \' \b \f \v \xHH
\uHHHH \UHHHHHHHH`). `{expr}` interpolates, with an optional format specifier
(`"{pi:5.2f}"`); literal braces must be escaped `\{` `\}`.

## Conversions

**No implicit value-to-value conversion.** With `i : int` and `f : float`, `i + f` is a
compile error. Cast by calling the target type: `float(i)`, `int(3.7)` (truncates toward
zero), `double(x)`, `uint(x)`, `int8(x)` — any numeric to any numeric.

Bare *integer literals* are the one exception: they promote to a known target type when the
value fits — local/global initializers, struct field declaration initializers, struct-ctor
field values, variant-arm values, `=`, `:=`, compound assignment, either side of a binary
operator, and `return`. Function-call arguments, parameter default values, and `<-` do
**not** promote.

```das
var a : float = 1                 // ok
struct Foo { x : float = 7 }      // ok
var f = Foo(x = 3)                // ok
def ret() : uint8 { return 200 }  // ok
// foo(1) where foo takes float   // NOT promoted - write foo(1.0)
```

Integer and bitfield targets are range-checked (`var d : uint8 = 256` is an error).
`float`/`double` targets always accept, so a literal above 2^24 silently loses precision in a
`float`. Enums cast out with `int(e)`; back in needs `unsafe(reinterpret<Color>(1))`.

Anything converts to text with `string(x)` or interpolation. Parsing back is not a cast:

- `int("123")` does **not** exist — it is a compile error. Use `to_int` from `strings`.
- `bool(x)` does **not** exist — write `x != 0`.
- `to_int` / `to_float` silently return `0` on garbage (`to_int("foo")` is `0`,
  `to_int("12abc")` is `12`). For untrusted input use `try_to_int` / `try_to_float` from
  `daslib/strings_convert`, which return a `Result` separating invalid / out-of-range /
  trailing-garbage.

## Vectors, swizzles, and the 16/8-bit lattice

```das
let v = float3(1.0, 2.0, 3.0)      // v.zyx is 3,2,1
var w = float4(1.0, 2.0, 3.0, 4.0)
w.xy = float2(9.0, 8.0)            // sequential swizzles are writable
```

`xyzw` caps at 4 lanes. Every vector also takes OpenCL-style `.s` swizzles — `s` plus one hex
lane digit per output lane, repeats allowed (`v.s0`, `v.s3210`, `b16.sf`). The 8- and 16-lane
forms add `.lo` / `.hi`. The two namespaces never mix in one mask.

Lattice vectors are tightly packed (`half3` is 6 bytes, `byte3` is 3) and pass by value.
`byte` is **signed** int8; `ubyte` is unsigned.

- **fp16 arithmetic is closed**: `half4 * half4 + half4` is a `half4` (each op promoted to
  float and rounded back, bit-identical to native fp16). `half4(f4)` / `float4(h4)` convert;
  `half8(lo_float4, hi_float4)` packs.
- **The integer families are storage, converts, and bits only** — `byte4 + byte4` and ordering
  compares are compile errors. Widen with the wider constructor (`int4(b4)` sign-extends,
  `uint4(us4)`); narrow with the target constructor (C truncation, `short4(i4)`) or a
  saturating form (`short4_sat(i4)`, `byte8_sat(s8)`). `<< >> & | ^` and compound assigns work,
  with a scalar `int` shift count masked to the lane width.
- 16-lane 8-bit forms carry exact dot products (`idot4(a, b)` -> `int4`, `idot4(acc, a, b)`
  accumulates, `idot(a, b)` reduces to one `int`) and `shuffle(lut, idx)`, which picks bytes
  from a 16-entry `byte16` table using a `ubyte16` index.

## Enumerations

```das
enum Numbers {
    zero            // 0
    one             // 1
    ten = 9 + 1     // explicit compile-time constant
}
enum private Chars : uint8 { ch_a = 'A' }  // storage: int/int8/int16/uint/uint8/uint16
```

Values are reached with a dot — `Numbers.one`, never `Numbers one`. An enum name is its own
strong type; out-of-range values truncate to the storage type. `require daslib/enum_trait`
enables `for (x in type<Chars>) { ... }` over every value. Enum and bitfield bodies take one
entry per line — unlike struct and tuple bodies, they reject `;` separators.

## Bitfields

Each named flag is one bit; storage is `uint` unless declared `uint8`/`uint16`/`uint64`.

```das
bitfield Perms {
    read
    write
    execute
    all  = Perms.read | Perms.write | Perms.execute   // named constants are allowed
    none = bitfield(0)
}

var t : Perms = Perms.read | Perms.write   // spell the type - see below
print("{t.read} {t.execute}\n")            // true false
t.execute = true                            // set one named bit; t.read = false clears
```

Flags read like `bool` fields, so prefer `bf.flag` over `int(bf & T.flag) != 0` and
`bf.flag = true/false` over `|=` / `&= ~`. Build from an integer with `bitfield(1 << 1)` (or
`bitfield8` / `bitfield16` / `bitfield64`). Two bitfields are the same type only when the flag
list **and** the storage type match. Anonymous forms: `bitfield<one; two>`,
`bitfield : uint8 <one; two>`.

Gotcha: a single flag (`Perms.read`) carries the named type, but the **OR of two flags infers
as a bare `bitfield`** — `var t = Perms.read | Perms.write` then rejects `t.read` with
"can't get bit field 'read' in bitfield". Annotate the declaration (`var t : Perms = ...`) or
assign into an already-typed variable.

## Tuples

```das
var a = (1, 2.0, "3")                  // tuple<int;float;string>
a._0 = 42                              // positional access
var named : tuple<i:int; f:float> = (i = 1, f = 2.0)
named.i = 5                            // named access; named._1 also works
let pair = "one" => 1                  // '=>' builds a 2-tuple, in any context
var (x, y, z) = (1, 2.0, "3")          // destructuring declaration
for ((p, q) in arr) { ... }            // destructuring iteration
```

Field names are part of the type: `tuple<int;float>` and `tuple<i:int;f:float>` are different
types and do not assign to each other. A positional literal whose elements are *all* bare
variable names may be promoted to a matching named tuple (`(eid, distSq)` for
`tuple<eid:int; distSq:float>`); a name mismatch is a compile error, not a silent fallback.
Mixing named and positional fields in one literal is unsupported.
`tuple Vertex { position : float3; uv : float2 }` is shorthand for an alias to the anonymous
form.

## Variants

A variant holds a case index plus the value of exactly one named case.

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

`v as f` when the case is `i` **panics**; `?as` yields a nullable, so pair it with `??`.
Compile-time indices: `typeinfo variant_index<f>(v)` (compile error on an unknown name),
`typeinfo safe_variant_index<nope>(v)` (`-1` instead). A direct case-field write (`v.i = 7`)
and `set_variant_index(v, n)` each require `unsafe`, and neither implies the other — a raw
field write does not change the index. Anonymous form `variant<i_value:uint; f_value:float>`;
two variants match when named cases, types, and order all match.

## Fixed arrays

`T[N]` is a structural type, not a qualifier on the element: one node per dimension,
outermost first. `float[4][4]` is 4 rows of `float[4]`.

```das
var m : float[4][4]
m[1][2] = 5.0                      // m[1] is a float[4] row
for (row in m) { ... }             // iterates the 4 rows
typedef M4 = float[4][4]
var stack : M4[10]                 // float[10][4][4]
```

Operations act on the **outermost level only** (the one-peel rule): `typeinfo dim(x)` is the
outermost dimension; in generics `auto(TT)` binds the whole array while `auto(TT)[]` peels one
level and `TT - []` in a return type removes one; `safe_addr(arr)` decays to a pointer to the
first element (`int[4]` -> `int?#`), which is what makes C-interop calls work.

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
print("{pt.x}\n")                       // '.' auto-dereferences; no C-style 'p->field'
                                        // ('->' exists only as the method-call arrow)
print("{*pt} {deref(pt)}\n")            // both panic on null
print("{pt?.x ?? -1.0}\n")              // '?.' yields null instead of panicking
unsafe { delete pt }                    // frees and nulls
```

Safe without `unsafe`: `new`, `*p` / `deref(p)`, `p.field`, `p?.field`, `p ?? default`,
`safe_addr(x)` (from `daslib/safe_addr`), `intptr(p)` (address as `uint64`).
Needs `unsafe`: `addr(x)`, `delete p`, `p[i]`, `++p` / `p += n`, `reinterpret<T>(x)`.

**The two const positions are independent, and writing through a pointer needs both open** —
a non-const pointee *and* a `var` handle:

```das
def write_through(var p : float?) { unsafe { p[0] = 42.0 } }   // ok
def read_only(p : float const?)   { return unsafe(p[0]) }      // read only
// def bad(p : float?) { unsafe { p[0] = 1.0 } }   // error: p is 'float? const'
```

Dereference, index, and field access all flow the handle's const onto the result, so a `let`
handle gives const access to everything reachable through it. Never take a writable pointer as
`T const?` and `reinterpret` the const away: the const type already licensed optimizations
that can delete the write. Declare `var T?`. Rationale in memory.md.

`addr<T?>(x)` is sugar for `reinterpret<T?>(addr(x))` under a single `unsafe` gate; the target
must be a pointer type. Casts are call-style and self-delimiting — `reinterpret<int>(f)`,
never `reinterpret<int> f` (a syntax error). A non-const `T?` argument is accepted where the
parameter is `T const?` (one-directional, top level only; also for a pointer field of a tuple
passed directly as an argument).

`smart_ptr<T>` is reference-counted, exists only for C++-registered handled types (never for
daslang structs or classes), moves with `<-`, and is declared `var inscope`. Compiler AST node
types are *not* smart pointers — they are garbage-collected raw pointers (macros.md).

## Temporary types

A `#` suffix marks a value borrowed from C++ that must not outlive the scope it was handed to —
`string#`, `int?#`, `array<uint8>#`.

```das
def use(var ds : das_string) {
    peek(ds) $ (boo : string#) {
        var cloned : string := boo     // clone escapes; a plain copy would not
    }
}
```

Temporaries cannot be copied (`s = boo` is an error), moved, returned, or passed where a
regular value is expected; they *can* be cloned with `:=`. A function that must accept both
flavors marks the parameter `implicit`, promising not to cache it:
`def accept_any(s : string implicit)`. `safe_addr(x)` yields a `T?#` without `unsafe`; the
compiler checks the argument is a local or global.

## Distinct types

A nominal newtype over a workhorse type: identical ABI, no interconversion, fully erased at
runtime.

```das
typedef distinct EntityId = int
typedef distinct Meters   = float

var id = EntityId(5)      // only way in: one arg of exactly the underlying type
print("{*id}\n")          // only way out: '*' peels one distinct level, yields a reference
*id = 7                   // const flows from the handle, so a 'var' derefs writable

def operator + (a, b : Meters) : Meters => Meters(*a + *b)
def describe(x : int)      : string => "int {x}"
def describe(x : EntityId) : string => "entity {*x}"   // overloads coexist
```

Only `==` and `!=` are borrowed (same distinct both sides); define every other operator
yourself. Two distincts over the same underlying type are unrelated, and so are same-named
distincts in two modules. `var id : EntityId` and `default<EntityId>` zero-initialize.
Distinct types cannot be table keys but work as array elements, struct fields, and
tuple/variant members. At runtime, printing and RTTI show the underlying value. For a distinct
over a pointer, `*` peels the distinct, not the pointer. `typedef private distinct Foo = int`
limits visibility to the declaring module. `distinct` is not a reserved word, so ordinary
identifiers named `distinct` still work.

## Type aliases

`typedef` creates a transparent name — interchangeable with what it names, not a new type.

```das
typedef Vec3     = float3
typedef Callback = function<(x, y : int) : bool>
typedef Registry = table<string; array<int>>
typedef private Internal = int      // also 'typedef public'
```

Without explicit `public`/`private` an alias inherits the module's publicity. Aliases may also
be declared inside a function body (scoped to the block) or inside a struct/class body.
`tuple Name { ... }`, `variant Name { ... }`, and `bitfield Name { ... }` are shorthand alias
declarations for the corresponding anonymous forms.

## Compile-time type expressions

| Form | Meaning |
|---|---|
| `type<T>` | a no-storage type tag; may be passed on but not read (reading errors at the call site: "type expression result is used, and not just passed") |
| `default<T>` | a real default value: zeros **plus** field initializers, so `default<Foo>.x` is `7` for `struct Foo { x : int = 7 }`. `default<>` of a tuple/variant is zero — not a deep initialize |
| `typedecl(expr)` | the type of an expression, usable in type positions: `var t : table<typedecl(key); string>` |
| `typeinfo <trait>(x)` | compile-time query; the trait name goes *outside* the parens: `typeinfo typename(x)`, `typeinfo sizeof(type<Foo>)`, `typeinfo dim(arr)` |

Pick `type<T>` when the body only needs the type, `default<T>` when it needs a value.
Declaring an uninitialized local of a struct that carries field initializers is rejected
(`var f : Foo` -> "uninitialized variable is unsafe"); write `var f = Foo()`. A struct with no
field initializers declares fine and is zeroed.

The trait catalog, `static_if`, and generic type contracts (`-const`, `-&`, `-[]`, `==const`,
`explicit`, `implicit`, OR-types) are in generics.md.
