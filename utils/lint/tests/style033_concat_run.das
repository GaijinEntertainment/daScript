options gen2
// STYLE033: a run of >=2 bulk `push_from` / `push_clone_from` from arrays.
//
//   * into a FRESH-EMPTY array<T> (empty `var c : array<T>` then the run) it is a
//     concatenation — `var c <- concat(s1, s2, ...)` says it in one expression
//     (needs `require daslib/linq`). This is the N>=2 generalization of STYLE032
//     (which handles the single-source case with `var c := SRC`).
//   * into an EXISTING (already-live) array it collapses to a single variadic call
//     of the same name — `dst |> push_from(...)` or `dst |> push_clone_from(...)`
//     (the builtin fixed-arity overloads).
//
// Caps: concat tops out at 8 sources, the variadic push overloads at 4 — a run
// longer than its target shape can express stays silent (no invalid suggestion).
// A C-array (fixed-array) source has no clean array<T> rewrite, so it stays silent.
// A nested source (array<array<E>> into a flat array<E>) binds the recursive-flatten
// push_from overload — that is a flatten, not a clone/concat, so it stays silent too.

expect 31209:4

require daslib/style_lint
require daslib/linq

// --- Bad: fresh-empty array filled by a run (>=2) — concat ---

def bad_fresh_concat_clone(a, b : array<int>) : array<int> {
    var c : array<int>                        // STYLE033 — concat(a, b)
    c |> push_clone_from(a)
    c |> push_clone_from(b)
    return <- c
}

def bad_fresh_concat_copy(a, b, c : array<int>) : array<int> {
    var d : array<int>                        // STYLE033 — concat(a, b, c)
    d |> push_from(a)
    d |> push_from(b)
    d |> push_from(c)
    return <- d
}

// --- Bad: existing array filled by a run (>=2) — variadic collapse ---

def bad_existing_variadic_clone(var dst : array<int>; a, b : array<int>) {
    dst |> push_clone_from(a)                 // STYLE033 — dst |> push_clone_from(a, b)
    dst |> push_clone_from(b)
}

def bad_existing_variadic_copy(var dst : array<int>; a, b : array<int>) {
    dst |> push_from(a)                       // STYLE033 — dst |> push_from(a, b)
    dst |> push_from(b)
}

// --- Good: already the suggested forms / out of scope (must NOT fire) ---

def good_concat_form(a, b : array<int>) : array<int> {
    // Already a concat.
    return <- concat(a, b)
}

def good_already_variadic(a, b : array<int>) : array<int> {
    // Already collapsed to one variadic call.
    var c : array<int>
    c |> push_clone_from(a, b)
    return <- c
}

def good_c_array_run() : array<int> {
    // C-array sources — no clean array<T> concat rewrite, stays silent.
    let c1 = fixed_array(1, 2)
    let c2 = fixed_array(3, 4)
    var w : array<int>
    w |> push_from(c1)
    w |> push_from(c2)
    return <- w
}

def good_existing_too_long(var dst : array<int>; a, b, c, d, e : array<int>) {
    // A run of 5 has no single variadic overload (cap is 4) — stays silent.
    dst |> push_clone_from(a)
    dst |> push_clone_from(b)
    dst |> push_clone_from(c)
    dst |> push_clone_from(d)
    dst |> push_clone_from(e)
}

def good_fresh_flatten(n1, n2 : array<array<int>>) : array<int> {
    // Nested sources flatten via the recursive push_from overload — NOT a clone, so
    // `concat(n1, n2)` (array<array<int>>) would be the wrong type. Must stay silent.
    var w : array<int>
    w |> push_from(n1)
    w |> push_from(n2)
    return <- w
}

def good_single_flatten(n1 : array<array<int>>) : array<int> {
    // Single nested source flattens — STYLE032's `var w := n1` would be a type error.
    var w : array<int>
    w |> push_from(n1)
    return <- w
}

def good_existing_flatten(var dst : array<int>; n1, n2 : array<array<int>>) {
    // Nested sources flatten into a flat dst — not a flat-array bulk append; stays silent.
    dst |> push_from(n1)
    dst |> push_from(n2)
}
