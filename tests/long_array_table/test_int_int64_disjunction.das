options gen2

require dastest/testing_boost public

// Phase 8b spike: can a function accept `int | int64` as a single
// parameter signature and fork inside the body via `static_if`? If
// yes, PR-D (linq surface widening) can use one signature per
// function instead of doubling overloads. If no, PR-D falls back to
// separate `: int` / `: int64` pairs.
//
// Reference for the disjunction-parameter shape:
// tests/language/generics.das. `typeinfo is_int` / `typeinfo
// is_int64` are added in the same PR — string- compare
// `stripped_typename(x) == "int"` was the fallback while landing
// this.

def take_or(x : int | int64) : int64 {
    static_if (typeinfo is_int(x)) {
        return int64(x) + 1_l
    } else {
        return x + 1_l
    }
}

[test(description = "a function taking int or int64 forks to its int branch for an int argument")]
def test_int_branch(t : T?) {
    let r = take_or(40)
    t |> equal(r, 41_l)
}

[test(description = "a function taking int or int64 forks to its int64 branch for an int64 argument")]
def test_int64_branch(t : T?) {
    let r = take_or(40_l)
    t |> equal(r, 41_l)
}

// Type-trait contract: locks `is_int` / `is_int64` against silent
// reverts.
[test(description = "typeinfo is_int and is_int64 classify int and int64 types")]
def test_typeinfo_is_int_traits(t : T?) {
    static_assert(typeinfo is_int(type<int>), "is_int(int) must be true")
    static_assert(!typeinfo is_int(type<int64>), "is_int(int64) must be false")
    static_assert(!typeinfo is_int(type<uint>), "is_int(uint) must be false")
    static_assert(typeinfo is_int64(type<int64>), "is_int64(int64) must be true")
    static_assert(!typeinfo is_int64(type<int>), "is_int64(int) must be false")
    static_assert(!typeinfo is_int64(type<uint>), "is_int64(uint) must be false")
    t |> equal(0, 0)
}

// Symmetric is_float / is_double — added alongside is_int / is_int64
// for numeric struct_convert dispatch (sqlite_migrate
// `struct_convert_field`).
[test(description = "typeinfo is_float and is_double classify float and double types")]
def test_typeinfo_is_float_double_traits(t : T?) {
    static_assert(typeinfo is_float(type<float>), "is_float(float) must be true")
    static_assert(!typeinfo is_float(type<double>), "is_float(double) must be false")
    static_assert(!typeinfo is_float(type<int>), "is_float(int) must be false")
    static_assert(typeinfo is_double(type<double>), "is_double(double) must be true")
    static_assert(!typeinfo is_double(type<float>), "is_double(float) must be false")
    static_assert(!typeinfo is_double(type<int>), "is_double(int) must be false")
    t |> equal(0, 0)
}
