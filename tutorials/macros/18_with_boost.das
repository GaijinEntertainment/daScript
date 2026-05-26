// Macro Tutorial 18: with_ macro from daslib/with_boost
//
// The `with_` macro binds an array or table element by reference inside
// a block, with an automatic LOCK around the body so push/erase/resize
// on the underlying container panic at runtime instead of silently
// dangling the reference.
//
// Covers:
//   - Why `var a : A& = arr[0]` is rejected by daslang's typer
//   - Single-arg form with default `_` binding
//   - Named binding via $(name) on the block
//   - Multi-arg form (positional) for cross-container work
//   - Workhorse element types (int, float, ...) — bound by reference
//   - Table containers (one table-keyed arg per call)
//   - Runtime lock: mutation of the container inside the body panics
//
// Run: daslang.exe tutorials/macros/18_with_boost.das

options gen2

require daslib/with_boost

struct A {
    f1 : int
    f2 : int
}

[export]
def main {

    // ──────────────────────────────────────────────────────────────────
    // Section 1 — Why with_ exists
    // ──────────────────────────────────────────────────────────────────
    //
    // Plain `var a : A& = arr[0]` is rejected by daslang's typer:
    //
    //     error[31300]: local reference to non-local expression is unsafe
    //
    // The reason: between binding the ref and using it, code could push
    // or resize the array, leaving `a` dangling. with_ solves this by
    // locking the array around a block — push/resize inside the block
    // panic instead of corrupting memory.

    // ──────────────────────────────────────────────────────────────────
    // Section 2 — Single-arg, default `_` binding
    // ──────────────────────────────────────────────────────────────────

    var arr = [A(f1 = 1, f2 = 2), A(f1 = 3, f2 = 4)]
    with_(arr[0]) {
        _.f1 = 99
        _.f2 = 100
    }
    print("section 2: arr[0] = {arr[0].f1}, {arr[0].f2}\n")

    // ──────────────────────────────────────────────────────────────────
    // Section 3 — Named binding via $(name)
    // ──────────────────────────────────────────────────────────────────
    //
    // The block-param keyword (`var`/`let`) is optional; the macro
    // strips constness so mutations always persist.

    with_(arr[1]) $(elem) {
        elem.f1 = 555
    }
    print("section 3: arr[1].f1 = {arr[1].f1}\n")

    // ──────────────────────────────────────────────────────────────────
    // Section 4 — Multi-arg positional form
    // ──────────────────────────────────────────────────────────────────
    //
    // Pass multiple containers; the block params are positional.
    // Each container gets its own lock.

    var dst = [A(f1 = 0, f2 = 0)]
    var src = [A(f1 = 10, f2 = 20)]
    with_(dst[0], src[0]) $(d, s) {
        d.f1 = s.f1 + 1
        d.f2 = s.f2 + 2
    }
    print("section 4: dst[0] = {dst[0].f1}, {dst[0].f2}\n")

    // ──────────────────────────────────────────────────────────────────
    // Section 5 — Workhorse element types (int, float, ...)
    // ──────────────────────────────────────────────────────────────────
    //
    // The block-arg is bound by reference, so `_ = X` (or named `x = X`)
    // mutates the underlying slot. The macro relies on the helper's
    // `block<(var x : TT&) : void>` signature to drive daslang's
    // inference; no special-case in the macro itself.

    var ints = [1, 2, 3]
    with_(ints[1]) {
        _ = 222
    }
    print("section 5: ints = {ints}\n")

    // ──────────────────────────────────────────────────────────────────
    // Section 6 — Tables
    // ──────────────────────────────────────────────────────────────────
    //
    // Tables work the same way; `tab[key]` upserts (creates a default
    // entry if missing). At most ONE table-keyed arg per call — the
    // macro can't prove two table-keyed args refer to distinct tables,
    // and a 2nd upsert into the SAME table would rehash and invalidate
    // the first pinned entry. The rule is conservative across the board
    // (no alias analysis), so `with_(tab1[k1], tab2[k2])` is also refused.

    var tab : table<string; A>
    tab |> insert("k", A(f1 = 11, f2 = 22))
    with_(tab["k"]) $(v) {
        v.f1 = 777
    }
    let kf1 = tab["k"].f1
    print("section 6: tab[k].f1 = {kf1}\n")

    // ──────────────────────────────────────────────────────────────────
    // Section 7 — Lock is real (would-panic example, commented out)
    // ──────────────────────────────────────────────────────────────────
    //
    // Trying to mutate the container from inside the body panics at
    // runtime — the whole point of the lock. Daslang panic is fatal
    // (not a C++/JS exception), so we don't demonstrate it live in a
    // tutorial. The line below is what would panic:
    //
    //     with_(arr[0]) $(a) {
    //         arr |> push(A(f1 = 1000, f2 = 2000))   // panic: array is locked
    //     }
    //
    // See `tests/with_boost/test_with_lock_panics.das` for the
    // full set of runtime-locked behaviours.

    print("section 7: see comment for the lock-panic shape\n")
}
