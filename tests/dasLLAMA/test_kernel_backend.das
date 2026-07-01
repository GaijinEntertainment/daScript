options gen2

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_math_default        // registers the "portable" backend at [init]
require dasllama/dasllama_math_aarch64_neon   // registers "arm64-sdot" / "arm64-laneq" on arm64+jit

// The kernel-backend registry must self-populate at [init]: the portable Q8·Q8 backend is always
// registered (the fallback everywhere), and one is auto-activated as the row-major default. A repack
// backend (arm64 laneq) is only activated by select_matmul_backend_for_load. No model file needed.

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
        select_matmul_backend_for_load()
        t |> success(registered(active_kernel_backend()), "for-load activated a registered backend")
        // pin forces a specific backend, overriding auto-select
        pin_kernel_backend("portable")
        t |> equal(active_kernel_backend(), "portable")
        // restore the pre-test selection so later test files see the same active backend
        clear_kernel_backend_pin()
        if (saved != "") {
            select_kernel_backend(saved)
        }
    }
}
