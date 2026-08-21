options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_math_default        // registers the "portable" backend at [init]
require dasllama/dasllama_math_aarch64_neon   // registers "arm64-sdot" on arm64+jit
require ?llvm/daslib/llvm_tune dasllama/dasllama_math_gen

// The kernel-backend registry must self-populate at [init]: the portable Q8·Q8 backend is always
// registered (the fallback everywhere), and one is auto-activated as the row-major default. A repack
// backend (arm64-gen / x64-gen) is only activated by select_matmul_backend_for_load_. No model file needed.

def private registered(name : string) : bool {
    for (n in kernel_backend_names()) {
        if (n == name) {
            return true
        }
    }
    return false
}

[test]
def test_portable_backend_always_registered(t : T?) {
    t |> run("portable backend registered + a row-major default auto-activated") @(t : T?) {
        t |> success(registered("portable"), "portable backend registered at [init]")
        t |> success(active_kernel_backend() != "", "a backend auto-activated (non-empty active)")
        t |> success(registered(active_kernel_backend()), "the active backend is a registered one")
        t |> success(!registered("bogus-backend"), "unknown backend not registered")
    }
}

[test]
def test_select_and_pin(t : T?) {
    t |> run("select / pin / for-load pick a registered backend and restore") @(t : T?) {
        let saved = active_kernel_backend()
        // explicit select by name
        select_kernel_backend("portable")
        t |> equal(active_kernel_backend(), "portable")
        // selecting an unknown name is a no-op (portable stays active)
        select_kernel_backend("does-not-exist")
        t |> equal(active_kernel_backend(), "portable")
        // for-load picks the best registered backend (a repack backend where available); its bool
        // return (needs_repack) drives whether the loader repacks — exercised for real by test_forward.
        select_matmul_backend_for_load_()
        t |> success(registered(active_kernel_backend()), "for-load activated a registered backend")
        // pin forces a specific backend, overriding auto-select
        pin_kernel_backend("portable")
        t |> equal(active_kernel_backend(), "portable")
        // an unknown pin name is a no-op that keeps the current backend (regression: Copilot #3340 —
        // it must not stick an invalid pin that wedges the active kernels)
        pin_kernel_backend("no-such-backend")
        t |> equal(active_kernel_backend(), "portable", "unknown pin keeps the current backend")
        // restore the pre-test selection so later test files see the same active backend
        clear_kernel_backend_pin()
        if (saved != "") {
            select_kernel_backend(saved)
        }
    }
}

// dummy slots for the availability test — the backend must never activate, so every slot panics
[unused_argument(yp, wp, sp, xqp, xsp, n, d)]
def private unavailable_mm(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d : int64) {
    panic("test-unavailable backend activated")
}
[unused_argument(yp, wp, sp, xqp, xsp, n, d, ntok)]
def private unavailable_batch(var yp : float?; wp : int8 const?; sp : float const?; xqp : int8 const?; xsp : float const?; n, d, ntok : int64) {
    panic("test-unavailable backend activated")
}
[unused_argument(y0p, y1p, y2p, wp, sp, woff0, woff1, woff2, xqp, xsp, n, d0, d1, d2)]
def private unavailable_group3(var y0p : float?; var y1p : float?; var y2p : float?; wp : int8 const?; sp : float const?; woff0, woff1, woff2 : int64; xqp : int8 const?; xsp : float const?; n, d0, d1, d2 : int64) {
    panic("test-unavailable backend activated")
}
[unused_argument(wp, sp, n, d)]
def private unavailable_repack(var wp : int8?; var sp : float?; n, d : int64) {
    panic("test-unavailable backend activated")
}
def private never_available() : bool => false

[test]
def test_backend_availability_predicate(t : T?) {
    t |> run("an unavailable backend is skipped by for-load select and refused by pin") @(t : T?) {
        let saved = active_kernel_backend()
        // outranks everything (99) but reports unavailable; needs_repack keeps it out of the
        // registration-time auto-activate (which deliberately does NOT evaluate `available` —
        // it runs at [init], where a predicate reading JIT-stamped state answers wrong)
        register_kernel_backend(KernelBackend(name = "test-unavailable",
            mm = @@unavailable_mm, batch = @@unavailable_batch, group3 = @@unavailable_group3,
            repack = @@unavailable_repack, needs_repack = true, priority = 99,
            available = @@never_available))
        select_matmul_backend_for_load_()
        t |> success(active_kernel_backend() != "test-unavailable", "for-load select skips an unavailable backend")
        pin_kernel_backend("test-unavailable")
        t |> success(active_kernel_backend() != "test-unavailable", "pin refuses an unavailable backend")
        t |> success(!kernel_backend_available_("test-unavailable"), "the availability probe reports the witness verdict")
        t |> success(!kernel_backend_available_("no-such-backend"), "an unregistered name reads unavailable")
        t |> success(kernel_backend_available_("portable"), "the portable backend is available everywhere")
        if (saved != "") {
            select_kernel_backend(saved)
        }
    }
}

[test]
def test_batch_backend_pin(t : T?) {
    t |> run("batch pin overrides the batch slots, survives re-select, guards layout") @(t : T?) {
        let saved = active_kernel_backend()
        select_kernel_backend("portable")
        // no pin: batch backend follows the active backend
        t |> equal(active_batch_backend(), "portable", "batch backend defaults to the active one")
        // an unknown donor warns and keeps the active backend's batch slots
        select_batch_backend("no-such-backend")
        t |> equal(active_batch_backend(), "portable", "unknown batch donor keeps active batch slots")
        // a repack donor on a row-major active backend is layout-incompatible: warn + keep
        var tested_reject = false
        for (n in kernel_backend_names()) {
            if (n == "arm64-gen" || n == "x64-gen") {
                select_batch_backend(n)
                t |> equal(active_batch_backend(), "portable", "repack donor rejected on row-major active")
                tested_reject = true
            }
        }
        if (!tested_reject) {
            to_log(LOG_WARNING, "test_batch_backend_pin: no repack donor (arm64-gen/x64-gen) registered on this build - layout-guard path not exercised\n")
        }
        // a row-major donor takes the batch slots and survives re-activation
        var tested_accept = false
        for (n in kernel_backend_names()) {
            if (n == "arm64-sdot") {
                select_batch_backend(n)
                t |> equal(active_batch_backend(), n, "row-major donor takes the batch slots")
                select_kernel_backend("portable")   // re-activate: the standing pin must re-apply
                t |> equal(active_batch_backend(), n, "batch pin survives re-selection")
                tested_accept = true
            }
        }
        if (!tested_accept) {
            to_log(LOG_WARNING, "test_batch_backend_pin: no row-major donor (arm64-sdot; x64 has none - its row-major default is portable) registered on this build - batch-pin override/survival not exercised\n")
        }
        // clearing the pin restores whole-backend slots on the next activation
        select_batch_backend("")
        select_kernel_backend("portable")
        t |> equal(active_batch_backend(), "portable", "cleared pin restores whole-backend slots")
        if (saved != "") {
            select_kernel_backend(saved)
        }
    }
}
