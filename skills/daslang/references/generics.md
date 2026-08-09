# Generic programming

Omit an argument type and the function becomes *generic*: it is instantiated for each distinct
combination of argument types at the call site. Instances are created as private functions in the
**calling** module — this drives both name resolution (see `_::` below) and error reporting.

Unlike C++ SFINAE, branching on types uses ordinary conditionals — `static_if` with `typeinfo`.

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

Constrain in the signature rather than in the body: `def set0(var a; b; i : int) { a[i] = b }`
fails deep inside with an obscure message, while
`def set0(var a : array<auto(T)>; b : T; i : int)` fails at the call with a clear one.

`type<TT>` refers to the bound alias as a type; `TT -const`, `TT -#`, `TT -&`, `TT -[]` strip one
qualifier from it (`typeinfo typename(type<TT -const>)` is `int` where `TT` is `int const`).

A non-`var` parameter binds the alias const, a `var` parameter binds it mutable. A plain
`auto(TT)` binds the **whole** matched type including fixed-array dimensions (`int[4]` -> `TT` is
`int const[4]`), whereas `auto(TT)[]` peels one level.

**Generic tuples work; generic structs do not.** A `tuple` may declare `auto(T)` members, and a
generic taking it refers to it as `type<Name>`. A `struct` with an `auto` field is a compile error
("structure field type can't be inferred").

```das
tuple Handle {
    h : auto(HandleType)
    i : int
}

def make_handle(t : auto(HandleType)) : Handle {
    var h : type<Handle>            // type<> resolves from the argument
    return h
}
```

## Type contracts

Contracts modify how a parameter type *matches*. They are consumed during inference and have no
runtime cost.

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

`==const` means **exact** constness, not "accepts either". Since a non-`var` parameter already
implies `const`, `def f(a : Foo ==const)` accepts only const arguments and `def f(var a : Foo ==const)`
accepts only mutable ones. The idiom is a *pair* of overloads, so the result's constness follows
the argument's:

```das
def view(a : Foo ==const) : string {
    return "const view"
}

def view(var a : Foo ==const) : string {
    a.x += 1
    return "mutable view"
}
```

`-#` is what lets a generic own a copy of a possibly-temporary argument: inside
`def own_copy(a : auto(TT))`, `var owned : TT -# := a` clones a temporary into a regular, owned
value.

Gotcha: combining `-#` with `==const` on **two or more** parameters that share the same alias
breaks matching — a plain `array<int>` is then rejected. For multi-source signatures use the
plain `array<TT>` form.

## OR types

`def f(a : int | float | double)` accepts any listed alternative and always makes the function
generic. This is **not** a runtime tagged union: the function is monomorphized per concrete
argument type, so inside the body `a` simply *is* an `int`, or a `float` — no dispatch, no
unpacking. Alternatives are tried in the order listed (`Bar explicit | Foo` tries exact `Bar`
first).

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

The annotation traits are spelled with the `struct_` prefix — plain `has_annotation` does not
exist. The `safe_` forms return `false` / `-1` instead of raising a compile error, which is what
you want in a generic that must accept anything. `has_field` requires the argument to already be
a struct or handled type; `safe_has_field` does not.

**Existence** — `builtin_module_exists(name)`, `builtin_annotation_exists(type<T>)` (true for
registered native types), `builtin_function_exists(@@<sig> name)` (native functions only — a
daslang-defined function is a compile error here).

`static_if (typeinfo builtin_module_exists(some_optional_module)) { ... }` compiles its body only
when that module is linked in — the standard way to make a binding optional.

There is no `is_same_type(A, B)` — traits take one argument. Compare type identity with
`typeinfo stripped_typename(a) == typeinfo stripped_typename(b)`.

Any trait name not on this list is dispatched to the user-extensible typeinfo-macro system.

## static_if, static_assert, concept_assert

`static_if (cond) { } static_elif (cond) { } else { }` — parentheses required; only the selected
branch is compiled, so the other branches may be invalid for the current types.

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

Two compile-time asserts turn a deep instantiation failure into one readable message:

- `static_assert(cond, "message")` — reports **at the assert**, inside the generic.
- `concept_assert(cond, "message")` — reports **at the call site**. Prefer it for preconditions
  a caller can act on.

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

A bare untyped parameter is *less* specialized than `array<numT>`, so the concrete overload keeps
winning for flat sources and the recursive one only fires for deeper nesting.

## Specialization and ambiguity

`array<numT>` (an already-bound alias) and `array<auto(TT)>` (a fresh binding) are **equally**
specialized. Two same-name overloads differing only in that produce
`error[30341] too many matching functions`. Distinguish them by a concrete element type, or make
one of them the untyped form above.

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
    _::write(val)                       // picks up the caller's write() overloads
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

Plain `write(val)` there would fail with "module is not visible directly from serializer". This is
why `:=` and `delete` are emitted as `_::clone` and `_::finalize` — so user-defined `clone` and
`finalize` overloads are found at the instantiation site.

## Types built from expressions

`typedecl(expr)` yields the compile-time type of an expression and can be used anywhere a type is
expected:

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
that compare a value against structural patterns and bind parts of it. Each arm is an `if (pattern)`;
`_` is the catch-all; `$v(name)` declares a variable bound to the matched part.

```das
require daslib/match

def enum_match(color : Color) : int {
    match (color) {
        if (Color.Red) {
            return 1
        }
        if (_) {
            return -1
        }
    }
}
```

Enum cases use the dotted `Color.Red` form.

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
it compiles for any type — the tool for matching inside a generic:

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
`match_copy` function that attempts the conversion and returns `true` on success.

`match` also decomposes compiler AST node pointers by node type and field, with nested patterns —
the workhorse for macro code.
