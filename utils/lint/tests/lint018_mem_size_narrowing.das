//! LINT018 is reported for: narrowing the size/position argument of a
//! call that has a 64-bit overload, or of a helper that takes the
//! 64-bit value or forwards it to such a call.
options gen2
options auto_inline_functions = false   // lint fixtures assert SOURCE shapes; splices rewrite them
// LINT018: narrowing the size/position argument of a call that has a 64-bit overload
//
// Problem: int(n) truncates above 2^31, so the copy, compare, resize, or erase
// silently covers the wrong count. memcpy/memmove/memcmp carry uint / int64 / uint64
// size overloads; array resize / resize_no_init / resize_and_init / reserve /
// erase and table reserve carry int64 overloads - the value passes straight through.
//
// Bad pattern:
//   memcpy(dst, src, int(nbytes))   // nbytes : int64
//   arr |> resize(int(newSize))     // newSize : int64
//
// Good pattern:
//   memcpy(dst, src, nbytes)
//   arr |> resize(newSize)
//
// A das helper is reported the same way when it would take the
// 64-bit value as it is: its parameter is declared int | int64, or
// it forwards the parameter as a size and has an int64 overload, or
// it is generic in the parameter and reads it as nothing but a size.

expect 50503:21

require daslib/lint

// bad: int() narrowing of an int64 size (1 warning)
def bad_memcpy_int64(var dst : void?; src : void?; nbytes : int64) {
    unsafe(memcpy(dst, src, int(nbytes)))
}

// bad: int() narrowing of a uint64 size (1 warning)
def bad_memcpy_uint64(var dst : void?; src : void?; nbytes : uint64) {
    unsafe(memcpy(dst, src, int(nbytes)))
}

// bad: int() narrowing on memmove (1 warning)
def bad_memmove_int64(var dst : void?; src : void?; nbytes : int64) {
    unsafe(memmove(dst, src, int(nbytes)))
}

// bad: int() narrowing on memcmp (1 warning)
def bad_memcmp_int64(dst, src : void?; nbytes : int64) : int {
    return unsafe(memcmp(dst, src, int(nbytes)))
}

// bad: array resize with a narrowed int64 size (1 warning)
def bad_resize(var arr : array<float>; newSize : int64) {
    arr |> resize(int(newSize))
}

// bad: resize_no_init narrowing (1 warning)
def bad_resize_no_init(var arr : array<float>; newSize : int64) {
    unsafe(arr |> resize_no_init(int(newSize)))
}

// bad: resize_and_init narrowing (1 warning)
def bad_resize_and_init(var arr : array<float>; newSize : int64) {
    arr |> resize_and_init(int(newSize))
}

// bad: reserve narrowing a uint64 - the fix widens with int64(...) (1 warning)
def bad_reserve_uint64(var arr : array<float>; newSize : uint64) {
    arr |> reserve(int(newSize))
}

// bad: single-position erase narrowing (1 warning)
def bad_erase_at(var arr : array<float>; pos : int64) {
    arr |> erase(int(pos))
}

// bad: ranged erase - both bounds narrow, one warning per call (1 warning)
def bad_erase_range(var arr : array<float>; pos, cnt : int64) {
    arr |> erase(int(pos), int(cnt))
}

// bad: table reserve narrowing (1 warning)
def bad_table_reserve(var tab : table<int; int>; newSize : int64) {
    tab |> reserve(int(newSize))
}

// good: the 64-bit size goes straight to the 64-bit overload
def good_memcpy_int64(var dst : void?; src : void?; nbytes : int64) {
    unsafe(memcpy(dst, src, nbytes))
}

// good: an int-typed size needs no cast at all
def good_memcpy_int(var dst : void?; src : void?; nbytes : int) {
    unsafe(memcpy(dst, src, nbytes))
}

// good: int64 resize overload takes the value as-is
def good_resize_int64(var arr : array<float>; newSize : int64) {
    arr |> resize(newSize)
}

// good: float operand - a genuine conversion, not a 64-bit truncation
def good_resize_float(var arr : array<float>; f : float) {
    arr |> resize(int(f))
}

// good: compound argument - the cast is not the whole size expression
def good_resize_compound(var arr : array<float>; n : int64) {
    arr |> resize(int(n) + 1)
}

struct Widget {
    n : int
}

def resize(var w : Widget; sz : int) {
    w.n = sz
}

// good: non-container receiver - same gate that keeps string resize (int-only) silent
def good_user_resize(var w : Widget; n : int64) {
    w |> resize(int(n))
}

def grow_to(var arr : array<auto(numT)>; n : int | int64) {
    arr |> reserve(n)
    arr |> resize(n)
}

def grow_any(var arr : array<auto(numT)>; n) {
    arr |> resize(n)
}

def grow_and_fill(var arr : array<auto(numT)>; n) {
    arr |> resize(n)
    for (i in range(n)) {
        arr[i] = default<numT>
    }
}

def grow_narrow(var arr : array<float>; n : int) {
    arr |> resize(n)
}

def grow_twin(var arr : array<float>; n : int) {
    arr |> resize(n)
}

def grow_twin(var arr : array<float>; n : int64) {
    arr |> resize(n)
}

def keep(var arr : array<int>; v : int) {
    arr |> push(v)
}

def keep(var arr : array<int>; v : int64) {
    arr |> push(int(v % 256l))
}

// bad: the helper declares its size int | int64 (1 warning)
def bad_helper_option(var arr : array<float>; newSize : int64) {
    arr |> grow_to(int(newSize))
}

// bad: a uint64 into an int | int64 parameter - the fix widens
// with int64(...) (1 warning)
def bad_helper_option_uint64(var arr : array<float>; newSize : uint64) {
    arr |> grow_to(int(newSize))
}

// bad: the helper is generic in its size and only resizes with it
// (1 warning)
def bad_helper_generic(var arr : array<float>; newSize : int64) {
    arr |> grow_any(int(newSize))
}

// bad: the helper forwards its size to resize and has an int64
// overload (1 warning)
def bad_helper_twin(var arr : array<float>; newSize : int64) {
    arr |> grow_twin(int(newSize))
}

// bad: ensure_capacity carries an int64 overload (1 warning)
def bad_ensure_capacity(var arr : array<float>; newSize : int64) {
    arr |> ensure_capacity(int(newSize))
}

// good: the helper takes int alone - the narrowing is its signature
def good_helper_int_only(var arr : array<float>; newSize : int64) {
    arr |> grow_narrow(int(newSize))
}

// good: the generic helper also reads its size as a range bound,
// which int64 would not compile
def good_helper_generic_other_reads(var arr : array<float>; newSize : int64) {
    arr |> grow_and_fill(int(newSize))
}

// good: an int64 overload exists, but the parameter is a value,
// not a size
def good_twin_not_a_size(var arr : array<int>; v : int64) {
    arr |> keep(int(v))
}

// bad: scratch_reserve carries an int64 overload (1 warning)
def bad_scratch_reserve(var arr : array<float>; newSize : int64) {
    unsafe(arr |> scratch_reserve(int(newSize)))
}

// bad: scratch_ensure_capacity carries an int64 overload (1 warning)
def bad_scratch_ensure_capacity(var arr : array<float>; newSize : int64) {
    unsafe(arr |> scratch_ensure_capacity(int(newSize)))
}

// bad: scratch_resize carries an int64 overload (1 warning)
def bad_scratch_resize(var arr : array<float>; newSize : int64) {
    unsafe(arr |> scratch_resize(int(newSize)))
}

// good: the int64 goes to the int | int64 parameter as it is
def good_helper_option(var arr : array<float>; newSize : int64) {
    arr |> grow_to(newSize)
}

// good: a uint64 widened for the int | int64 parameter
def good_helper_option_uint64(var arr : array<float>; newSize : uint64) {
    arr |> grow_to(int64(newSize))
}

// good: the generic helper takes the int64 as it is
def good_helper_generic(var arr : array<float>; newSize : int64) {
    arr |> grow_any(newSize)
}

// good: the int64 overload of the helper takes the value as it is
def good_helper_twin(var arr : array<float>; newSize : int64) {
    arr |> grow_twin(newSize)
}

// good: the capacity helpers take the int64 as it is
def good_capacity_int64(var arr : array<float>; newSize : int64) {
    arr |> ensure_capacity(newSize)
    unsafe {
        arr |> scratch_reserve(newSize)
        arr |> scratch_ensure_capacity(newSize)
        arr |> scratch_resize(newSize)
    }
}

def grow_via(var arr : array<auto(numT)>; n) {
    arr |> grow_any(n)
}

def grow_named(var arr : array<float>; n : auto(sizeT)) {
    arr |> resize(n)
}

def grow_again(var arr : array<auto(numT)>; n) {
    if (length(arr) > 100) {
        arr |> grow_again(n)
    }
    arr |> resize(n)
}

def grow_generic_twin(var arr : array<auto(numT)>; n : int) {
    arr |> resize(n)
}

def grow_generic_twin(var arr : array<auto(numT)>; n : int64) {
    arr |> resize(n)
}

// bad: the helper forwards its size to another helper that resizes
// with it (1 warning)
def bad_helper_forwards_twice(var arr : array<float>; newSize : int64) {
    arr |> grow_via(int(newSize))
}

// bad: a generic helper with an int64 overload of the same signature
// (1 warning)
def bad_helper_generic_twin(var arr : array<float>; newSize : int64) {
    arr |> grow_generic_twin(int(newSize))
}

// good: the helper names its size type, and the cast picks it
def good_helper_named_type(var arr : array<float>; newSize : int64) {
    arr |> grow_named(int(newSize))
}

// good: a recursive helper also passes its size to itself, which is
// not a size slot
def good_helper_recursive(var arr : array<float>; newSize : int64) {
    arr |> grow_again(int(newSize))
}
