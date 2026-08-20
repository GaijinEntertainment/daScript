# Generic programming

Omit an argument type and the function becomes *generic*: it is instantiated per distinct
combination of call-site argument types, as a private function in the **calling** module.

Branch on types with `static_if` plus `typeinfo`:

```das
def set_field(var obj; val) {
    static_if (typeinfo safe_has_field<x>(obj)) {
        obj.x = val
    }
}
```

## Declaring a generic

| Form | Meaning |
|---|---|
| `def f(a)` | `a` is any type |
| `def f(a, b)` | `a` and `b` are *independent* types |
| `def f(a, b : auto)` | `a` and `b` must be the **same** type |
| `def f(a : auto)` | same as `def f(a)` |
| `def f(a : auto(TT)) : TT` | names the bound type `TT`, usable in the signature and body |
| `def f(a : array<auto(TT)>; b : TT)` | `b` must match `a`'s element type |
| `def f(a : auto(TT)[])` | fixed array; `TT` binds the element, one level peeled |
| `[generic] def f(a : int)` | forces generic instantiation even with fully concrete arguments |

Constrain in the signature, not the body: `def set0(var a : array<auto(T)>; b : T; i : int)` fails
at the call with a clear message; unconstrained, it fails deep inside with an obscure one.

`type<TT>` refers to the bound alias as a type, and the strip contracts apply to it
(`typeinfo typename(type<TT -const>)` is `int` where `TT` is `int const`). A non-`var` parameter
binds the alias const, a `var` parameter mutable. Plain `auto(TT)` binds the **whole** matched type
including fixed-array dimensions (`int[4]` -> `TT` is `int const[4]`).

**Generic tuples work; generic structs do not.** A `tuple` may declare `auto(T)` members, and a
generic taking it refers to it as `type<Name>`; a `struct` with an `auto` field is a compile error
("structure field type can't be inferred").

```das
tuple Handle {
    h : auto(HandleType)
    i : int
}

def make_handle(t : auto(HandleType)) : Handle {
    var h : type<Handle>
    return h
}
```

## Type contracts

Contracts modify how a parameter type *matches*; they are consumed during inference, at no runtime
cost.

**They belong on a parameter, not on a cast.** `-const`, `-&`, `-[]`, `-#` and `==const` act on
the head of the type expression they sit on, and are load-bearing only while a generic alias is
binding. On a concrete cast target they are inert — `reinterpret<Foo? -const>(p)` is
`reinterpret<Foo?>(p)` — and never reach a nested qualifier: with `typedef CI = int const`,
`CI? -const` is still `int const?`. For a writable pointer declare the parameter `var T?` rather
than stripping `const` off a cast (memory.md). (probe-verified 2026-08-16)

| Contract | Accepts |
|---|---|
| `Foo` | `Foo` and anything inheriting from `Foo` (a non-`var` parameter also implies `const`) |
| `Foo const` | both `Foo` and `Foo const` |
| `Foo ==const` | exact constness match only |
| `auto -const` | matches, then removes `const` from the bound type |
| `Foo#` | temporary `Foo#` only |
| `auto -#` | matches, then removes the temporary qualifier |
| `Foo implicit` | both `Foo` and `Foo#`, treated as written — **unsafe** |
| `Foo \|#` | `Foo` then `Foo#`, in that order |
| `auto&` | passed by reference |
| `auto ==&` / `auto& ==&` | exact by-value / by-reference match |
| `auto -&` | matches, then removes the reference |
| `auto[]` | a fixed array of any element type and size |
| `TT -[]` | removes one fixed-array level |
| `Foo explicit` | exactly `Foo`; no substitution of derived types |
| `int \| float` | OR type (see below) |

`==const` is **exact** constness, not "accepts either": a non-`var` parameter already implies
`const`, so `def f(a : Foo ==const)` accepts only const arguments and `def f(var a : Foo ==const)`
only mutable ones. The idiom is a *pair* of such overloads, so the result's constness follows the
argument's.

`-#` lets a generic own a copy of a possibly-temporary argument: inside
`def own_copy(a : auto(TT))`, `var owned : TT -# := a` clones a temporary into an owned value.

Gotcha: `-#` with `==const` on **two or more** parameters sharing one alias breaks matching — a
plain `array<int>` is then rejected. For multi-source signatures use plain `array<TT>`.

## OR types

`def f(a : int | float | double)` accepts any listed alternative and always makes the function
generic. **Not** a runtime tagged union: the function is monomorphized per concrete argument type,
so inside the body `a` simply *is* an `int`, or a `float`. Alternatives are tried in the order
listed (`Bar explicit | Foo` tries exact `Bar` first).

## typeinfo

`typeinfo <trait>(expr)` or `typeinfo <trait>(type<T>)` — compile-time reflection, folded to a
constant. Every trait takes exactly one argument; a few take extra names in angle brackets.

**Names** — `typename`, `fulltypename`, `stripped_typename`, `undecorated_typename`, `modulename`,
`struct_name`, `struct_modulename`, `mangled_name`

**Size and layout** — `sizeof`, `alignof`, `dim`, `dim_table_value`, `vector_dim`, `offsetof<field>`

**Kind queries** — `is_pod`, `is_raw`, `is_struct`, `is_class`, `is_handle`, `is_distinct`,
`is_tuple`, `is_variant`, `is_enum`, `is_bitfield`, `is_lambda`, `is_function`, `is_iterator`,
`is_iterable`, `is_string`, `is_numeric`, `is_numeric_comparable`, `is_int`, `is_int64`, `is_float`,
`is_double`, `is_vector`, `is_any_vector`, `is_array`, `is_table`, `is_dim`, `is_pointer`,
`is_smart_ptr`, `is_void`, `is_void_pointer`, `is_ref`, `is_ref_type`, `is_ref_value`, `is_const`,
`is_temp`, `is_temp_type`, `is_local`, `is_workhorse`, `is_argument`

`is_int`/`is_int64`/`is_float`/`is_double` are exact scalar checks, not "numeric".

**Capabilities** — `can_copy`, `can_move`, `can_clone`, `can_clone_from_const`, `can_new`,
`can_delete`, `can_delete_ptr`, `can_be_placed_in_container`, `need_delete`, `need_inscope`,
`has_nontrivial_ctor`, `has_nontrivial_dtor`, `has_nontrivial_copy`,
`is_unsafe_when_uninitialized`, `is_safe_to_delete`, `is_pod_delete`, `needs_nontrivial_init`,
`needs_container_init`, `needs_container_finalize`

**Fields, annotations, variants** — `has_field<name>`, `safe_has_field<name>`,
`struct_has_annotation<ann>`, `struct_safe_has_annotation<ann>`,
`struct_has_annotation_argument<ann; arg>`, `struct_safe_has_annotation_argument<ann; arg>`,
`struct_get_annotation_argument<ann; arg>`, `variant_index<name>`, `safe_variant_index<name>`

`is_unsafe_when_uninitialized` tells a generic whether a bare `var x : TT` is legal for the type it
bound: `static_if (typeinfo is_unsafe_when_uninitialized(type<TT>)) { unsafe { ... } } else { ... }`.
The `@safe_when_uninitialized` field annotation and the `[safe_when_uninitialized]` struct
annotation flip it (structs-and-classes.md).

Annotation traits all carry the `struct_` prefix — plain `has_annotation` does not exist. The
`safe_` forms return `false` / `-1` instead of raising a compile error; `has_field` requires the
argument to already be a struct or handled type, `safe_has_field` does not.

**Existence** — `builtin_module_exists(name)`, `builtin_annotation_exists(type<T>)` (true for
registered native types), `builtin_function_exists(@@<sig> name)` (native functions only — a
daslang-defined function is a compile error here).

`static_if (typeinfo builtin_module_exists(name))` compiles its body only when that module is
linked in — how a binding is made optional.

No `is_same_type(A, B)` — traits take one argument; compare identity with
`typeinfo stripped_typename(a) == typeinfo stripped_typename(b)`.

Any trait name not on this list is dispatched to the user-extensible typeinfo-macro system.

## static_if, static_assert, concept_assert

`static_if (cond) { } static_elif (cond) { } else { }` — parentheses required; only the selected
branch is compiled, so the others may be invalid for the current types.

```das
def kind(a) : string {
    static_if (typeinfo is_array(a)) {
        return "array"
    } static_elif (typeinfo is_numeric(a)) {
        return "number"
    } else {
        return "other"
    }
}
```

- `static_assert(cond, "message")` — reports **at the assert**, inside the generic.
- `concept_assert(cond, "message")` — reports **at the call site**; prefer it for preconditions a
  caller can act on.

```das
def sum_all(a : array<auto(TT)>) : TT {
    concept_assert(typeinfo is_numeric(type<TT>), "sum_all requires a numeric element type")
    var total : TT
    for (x in a) {
        total += x
    }
    return total
}
// sum_all(["a"]) -> error at the call: "sum_all requires a numeric element type"
```

## Recursive (variadic) generics

To accept `array<T>`, `array<array<T>>`, ... at any nesting, add an **untyped** source parameter
next to the concrete one and recurse under `static_if`:

```das
def flat_push(var dest : array<auto(numT)>; src : array<numT>) {    // flat fast path
    dest |> push_from(src)
}

def flat_push(var dest : array<auto(numT)>; src) {                  // deeper nesting, or a scalar
    static_if (typeinfo is_array(src)) {
        for (x in src) {
            dest |> flat_push(x)
        }
    } else {
        dest |> push(numT(src))
    }
}
```

A bare untyped parameter is *less* specialized than `array<numT>`, so the concrete overload wins
for flat sources and the recursive one fires only for deeper nesting.

## Specialization and ambiguity

`array<numT>` (an already-bound alias) and `array<auto(TT)>` (a fresh binding) are **equally**
specialized: two same-name overloads differing only in that produce
`error[30341] too many matching functions`. Distinguish them by a concrete element type, or make
one the untyped form above.

## Name resolution inside a generic

| Prefix | Resolves in |
|---|---|
| *(none)* | the module where the generic is **defined**; the caller's overloads are invisible |
| `_::` | as if written in the **instantiating** module — the caller's overloads *and* everything it requires |
| `__::` | the instantiating module's **own** symbols only, nothing imported |

```das
module serializer                       // --- library ---

[generic]
def save(val) {
    _::write(val)
}
```

```das
require serializer                      // --- user code ---

struct Color {
    r, g, b : float
}

def write(c : Color) {
    print("{c.r},{c.g},{c.b}")
}

[export]
def main() {
    save(Color(r = 1.0))                // calls the user's write(Color)
}
```

Plain `write(val)` there fails with "module is not visible directly from serializer". `:=` and
`delete` are emitted as `_::clone` / `_::finalize` for the same reason.

## Types built from expressions

`typedecl(expr)` yields the compile-time type of an expression, usable anywhere a type is expected:

```das
def table_by_id(t : auto(T)) {
    var tab : table<typedecl(t.id); T>
    return <- tab
}
// for struct A { id : string } -> table<string const; A const>
// for struct B { id : int }    -> table<int const; B const>
```

## Pattern matching

`require daslib/match` adds `match`, `static_match`, `multi_match`, `static_multi_match` — macros
that compare a value against structural patterns and bind parts of it. Each arm is an
`if (pattern)`; enum cases use the dotted `Color.Red` form.

| Pattern | Matches |
|---|---|
| `_` | anything (also any single element inside a sequence) |
| `...` | zero or more elements inside a sequence |
| `$v(n)` | binds the matched value to a new variable `n` |
| `_ as f` / `$v(n) as f` | variant case `f`, optionally binding it |
| `IF(f = $v(x))` | variant case by constructor syntax |
| `Foo(a = 13)` / `Foo(a = $v(n))` | struct field equal to a value / bound |
| `(1, _, "3")`, `(13, ...)`, `(..., "13")` | tuple by position |
| `fixed_array($v(a), $v(b))`, `fixed_array(0, ...)` | fixed-size array |
| `[$v(a), $v(b)]`, `[..., 1, 2]` | dynamic array (element count is checked) |
| `pattern && cond` | guard — extra condition using the bound variables |
| `pattern \|\| pattern` | either; both sides must bind the same variables |
| `match_expr(x + 1)` | element equals an expression over already-bound variables |
| `match_type(type<int>, $v(e))` | matches on the type of the expression |

```das
def guards_match(ab : AB) : string {
    match (ab) {
        if (AB(a = $v(a), b = $v(b)) && (b > a)) {
            return "{b} > {a}"
        }
        if (_) {
            return "other"
        }
    }
}
```

**`static_match`** silently drops arms whose pattern cannot possibly match the argument type, so
it compiles for any type — the matcher to use inside a generic:

```das
def static_kind(what) : string {
    static_match (what) {
        if (Color.Red) {
            return "red"
        }
        if (match_type(type<int>, $v(n))) {
            return "int {n}"
        }
        if (_) {
            return "other"
        }
    }
}
```

**`multi_match`** runs *every* matching arm instead of stopping at the first;
`static_multi_match` is its type-tolerant form.

Matching values of unrelated struct types requires opting the struct in: `[match_as_is]` plus
user-defined `operator is` / `operator as`, or `[match_copy, safe_when_uninitialized]` plus a
`match_copy` function returning `true` on a successful conversion.

`match` also decomposes compiler AST node pointers by node type and field, with nested patterns.
