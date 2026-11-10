options gen2
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require dasllama/dasllama_math
require dasllama/dasllama_math_default        // registers the "portable" backend at [init]
require dasllama/dasllama_math_aarch64_neon   // registers "arm64-sdot" on arm64+jit
require ?llvm dasllama/dasllama_math_gen
require ?das_metal dasllama/dasllama_metal_gemm   // registers the batch-only "metal" donor on Apple builds
require ?vulkan dasllama/dasllama_math_vulkan     // registers the batch-only "vulkan" donor where dasVulkan builds
require ?cuda dasllama/dasllama_math_cuda         // registers the batch-only "cuda" donor where dasCUDA builds
require dasllama/dasllama_kqformat            // KqFmt + kq_fmt_stamp: the registry cell walks every format
require ?vulkan dasllama/dasllama_env         // g_env_vulkan.vk_mac: the macOS decline's opt-out
require ?das_accelerate dasllama/dasllama_math_accelerate
require daslib/enum_trait                     // for (f in type<KqFmt>)
require daslib/jobque_boost
require daslib/math_bits   // nolint:STYLE030,LINT019 — used only inside the das_accelerate static_if half
require math
require _compares

// The kernel-backend registry must self-populate at [init]: the portable Q8·Q8 backend is always
// registered (the fallback everywhere), and one is auto-activated as the row-major default. A repack
// backend (arm64-gen / x64-gen) is only activated by select_matmul_backend_for_load_. No model file needed.
// test_kq_registries reads the [tune] registries of dasllama_math_gen and skips on a build without dasLLVM.

def private registered(name : string) : bool {
    for (n in kernel_backend_names()) {
        if (n == name) {
            return true
        }
    }
    return false
}

[test(description = "The kernel-backend registry self-populates at init: portable is registered, a non-empty registered backend is auto-activated, and an unknown name is not registered")]
def test_portable_backend_always_registered(t : T?) {
    t |> run("portable backend registered + a row-major default auto-activated") @(t : T?) {
        t |> success(registered("portable"), "portable backend registered at [init]")
        t |> success(active_kernel_backend() != "", "a backend auto-activated (non-empty active)")
        t |> success(registered(active_kernel_backend()), "the active backend is a registered one")
        t |> success(!registered("bogus-backend"), "unknown backend not registered")
    }
}

[test(description = "Select and pin by name activate portable, an unknown name in either is a no-op keeping the current backend, and the for-load pick activates a registered backend")]
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

[test(description = "`restore_kernel_backend` puts back exactly the seat it was given: an unpinned seat clears a later pin and reactivates its backend, a pinned seat restores pin and backend over a for-load reselect")]
def test_seat_restores_the_pin(t : T?) {
    t |> run("a restored seat keeps the pin it found, and no pin where there was none") @(t : T?) {
        let found = kernel_backend_seat()
        clear_kernel_backend_pin()
        let unpinned = kernel_backend_seat()
        t |> equal(unpinned.pin, "", "control: no pin recorded where none is set")
        pin_kernel_backend("portable")
        restore_kernel_backend(unpinned)
        t |> equal(kernel_backend_pin(), "", "a seat with no pin clears the cell's own")
        t |> equal(active_kernel_backend(), unpinned.active, "and the backend it found is active again")
        pin_kernel_backend("portable")
        let pinned = kernel_backend_seat()
        clear_kernel_backend_pin()
        select_matmul_backend_for_load_()
        restore_kernel_backend(pinned)
        t |> equal(kernel_backend_pin(), "portable", "a pin set before the cell survives its restore")
        t |> equal(active_kernel_backend(), "portable", "and its backend is active")
        restore_kernel_backend(found)
    }
}

//! An available registered backend other than `not`, "" when the build registers none.
def private other_available(not : string) : string {
    for (n in kernel_backend_names()) {
        return n if (n != not && !kernel_backend_batch_only(n) && kernel_backend_available_(n))
    }
    return ""
}

[test(description = "`with_kernel_backend` runs its block on the pinned backend and puts the seat back, refuses an unknown backend without running the block, and an earlier pin survives a nested pin on a second backend")]
def test_with_kernel_backend(t : T?) {
    t |> run("with_kernel_backend runs its block under the pin, puts the seat back, and refuses a pin that does not take") @(t : T?) {
        let found = kernel_backend_seat()
        clear_kernel_backend_pin()
        let unpinned = kernel_backend_seat()
        var inside = ""
        var inside_pin = ""
        let took = with_kernel_backend("portable") $ {
            inside = active_kernel_backend()
            inside_pin = kernel_backend_pin()
        }
        t |> success(took, "the pin took")
        t |> equal(inside, "portable", "the block ran on the pinned backend")
        t |> equal(inside_pin, "portable", "and under its pin")
        t |> equal(kernel_backend_pin(), "", "no pin after the block where there was none")
        t |> equal(active_kernel_backend(), unpinned.active, "and the backend it found is active again")
        var ran = false
        let refused = !with_kernel_backend("no-such-backend") $ {
            ran = true
        }
        t |> success(refused && !ran, "an unknown backend is refused and its block does not run")
        t |> equal(active_kernel_backend(), unpinned.active, "a refusal leaves the backend it found")
        pin_kernel_backend("portable")
        let other = other_available("portable")
        if (other != "") {
            var other_inside = ""
            let other_took = with_kernel_backend(other) $ {
                other_inside = active_kernel_backend()
            }
            t |> success(other_took, "the pin on {other} took over an earlier pin")
            t |> equal(other_inside, other, "the block ran on {other}")
            t |> equal(kernel_backend_pin(), "portable", "the earlier pin survives the block")
            t |> equal(active_kernel_backend(), "portable", "and its backend is active")
        } else {
            to_log(LOG_WARNING, "test_with_kernel_backend: no second available backend on this build - the earlier-pin arm is not exercised\n")
        }
        restore_kernel_backend(found)
    }
}

//! the first row two formats share, as "<fmt> carries <other>'s row"; "" when every row is its format's own
def private crossed_row(rows : array<auto(R)>) : string {
    for (i in range(length(rows))) {
        for (j in range(i)) {
            return "{rows[i].fmt} carries {rows[j].fmt}'s row {j}" if (rows[i].fn == rows[j].fn && rows[i].fmt != rows[j].fmt)
        }
    }
    return ""
}

[test(description = "Where dasLLVM mounts, every kq format carries all eight tune registries or none with the reference row first, every format but q8 has rm rows with a witness keyed alike per perm, and no format's kernel row is another's"), arch(at = "../ARCHITECTURE_MEASUREMENT_KERNEL_RACE.md#rm-family-race")]
def test_kq_registries(t : T?) {
    static_if (typeinfo module_exists(dasllama_math_gen)) {
        t |> run("kq_registries: a kq family's eight registries are all present with the reference row first, or all absent") @(t : T?) {
            var families = 0
            for (kf in type<KqFmt>) {
                kq_fmt_stamp(kf) $(F) {
                    let r <- kq_registries(F)
                    let lens = fixed_array(length(r.tile), length(r.tile4), length(r.gemv), length(r.layout),
                        length(r.tileform), length(r.amx_cfg), length(r.witness), length(r.panel))
                    var present = 0
                    for (l in lens) {
                        present += l > 0 ? 1 : 0
                    }
                    t |> success(present == 0 || present == 8, "{kf}: all eight registries or none (lengths {lens})")
                    if (present == 8) {
                        t |> equal(r.tile[0]._0, "reference", "{kf}: the tile registry's first row is the reference body")
                    }
                    if (present == 8) {
                        families++
                        t |> equal(r.gemv[0]._0, "reference", "{kf}: the gemv registry's first row is the reference body")
                        t |> equal(r.panel[0]._0, "reference", "{kf}: the panel registry's first row is the reference body")
                    }
                }
            }
            t |> success(families > 0, "at least one format carries a kq family ({families})")
            let q8 <- kq_registries(KqTag_q8())
            t |> equal(length(q8.tile), 0, "control: q8 has no kq family (its tile family is the q8q8 one)")
        }
        t |> run("kq_registries: every format but q8 carries rm rows, one witness row per perm, keyed alike, the reference row first") @(t : T?) {
            var fmts <- kq_family_fmts() $(regs : KqRegistries) => !empty(regs.rm_gemv)
            var members = 0
            for (_kf in type<KqFmt>) {
                members++
            }
            t |> equal(length(fmts), members - 1, "every format but q8 (its own rows type) carries rm rows ({fmts})")
            for (kf in fmts) {
                var r <- kq_registries_of(kf)
                t |> equal(length(r.rm_witness), length(r.rm_gemv), "{kf}: a witness row per rm perm")
                t |> equal(r.rm_gemv[0]._0, "reference", "{kf}: the reference row first")
                for (i in range(length(r.rm_gemv))) {
                    t |> equal(r.rm_witness[i]._0, r.rm_gemv[i]._0, "{kf}: perm {i} keyed alike")
                }
                delete r
            }
            delete fmts
        }
        t |> run("kq_registries: every format's kernel rows are its own - no row is another format's") @(t : T?) {
            var tiles : array<tuple<fmt : KqFmt; fn : KqTileGenFn>>
            var gemvs : array<tuple<fmt : KqFmt; fn : MatmulKqRowsFn>>
            var panels : array<tuple<fmt : KqFmt; fn : KqPanelGenFn>>
            for (kf in type<KqFmt>) {
                kq_fmt_stamp(kf) $(F) {
                    let r <- kq_registries(F)
                    let again <- kq_registries(F)
                    if (!empty(r.tile)) {
                        t |> success(r.tile[0]._1 == again.tile[0]._1, "control: {kf}'s tile row compares equal to itself")
                    }
                    for (row in r.tile) {
                        tiles |> push((fmt = kf, fn = row._1))
                    }
                    for (row in r.tile4) {
                        tiles |> push((fmt = kf, fn = row._1))
                    }
                    for (row in r.gemv) {
                        gemvs |> push((fmt = kf, fn = row._1))
                    }
                    for (row in r.panel) {
                        panels |> push((fmt = kf, fn = row._1))
                    }
                }
            }
            t |> success(!empty(tiles) && !empty(gemvs) && !empty(panels), "the walk collected rows of every kind")
            t |> equal(crossed_row(tiles), "", "no format's tile or tile4 row is another format's")
            t |> equal(crossed_row(gemvs), "", "no format's gemv row is another format's")
            t |> equal(crossed_row(panels), "", "no format's panel row is another format's")
        }
    } else {
        t |> skip("dasllama_math_gen is not mounted (no dasLLVM) - no [tune] registries to read")
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

[test(description = "A priority-99 backend whose availability predicate answers false is skipped by the for-load select and refused by pin, and the probe reads unregistered names unavailable and portable available")]
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

[test(description = "The batch backend follows the active one unpinned, an unknown or repack donor on a row-major active backend is rejected, a row-major donor's pin survives reselection, and clearing restores whole-backend slots")]
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

[test(description = "Every batch-only backend, including each metal, vulkan or cuda donor the build registers, leaves the active backend and pin unchanged under pin or select, and on macOS vulkan is unavailable without DASLLAMA_VK_MAC")]
def test_batch_only_backends_refuse_the_whole_seat(t : T?) {
    t |> run("a batch-only backend is never pinned or selected as the whole backend") @(t : T?) {
        t |> success(!kernel_backend_batch_only("portable"), "the portable backend serves every slot")
        let seat = kernel_backend_seat()
        var checked = 0
        for (n in kernel_backend_names()) {
            continue if (!kernel_backend_batch_only(n))
            let before = active_kernel_backend()
            pin_kernel_backend(n)
            t |> equal(active_kernel_backend(), before, "pin_kernel_backend('{n}') keeps the active backend")
            t |> equal(kernel_backend_pin(), seat.pin, "... and sets no pin")
            select_kernel_backend(n)
            t |> equal(active_kernel_backend(), before, "select_kernel_backend('{n}') keeps it too")
            checked++
        }
        restore_kernel_backend(seat)
        var donors = 0
        static_if (typeinfo builtin_module_exists(das_metal)) {
            t |> success(registered("metal") && kernel_backend_batch_only("metal"), "the metal donor registers batch-only")
            donors++
        }
        static_if (typeinfo builtin_module_exists(vulkan)) {
            t |> success(registered("vulkan") && kernel_backend_batch_only("vulkan"), "the vulkan donor registers batch-only")
            if (get_platform_name() == "darwin" && !g_env_vulkan.vk_mac) {
                t |> success(!kernel_backend_available_("vulkan"), "on macOS the vulkan donor declines before any probe unless DASLLAMA_VK_MAC asks")
            }
            donors++
        }
        static_if (typeinfo builtin_module_exists(cuda)) {
            t |> success(registered("cuda") && kernel_backend_batch_only("cuda"), "the cuda donor registers batch-only")
            donors++
        }
        if (donors > 0) {
            t |> success(checked >= donors, "every donor this build registers was walked: {checked} of {donors}")
        } elif (checked == 0) {
            t |> skip("this build registers no batch-only backend (no das_metal, no dasVulkan)")
        }
    }
}

// whether pinning `name` panicked, with DASLLAMA_PIN_BACKEND read as `env_pin` for the call
def private pin_panics(name, env_pin : string) : bool {
    var tripped = false
    try {
        pin_kernel_backend_env(name, env_pin)
    } recover {
        tripped = true
    }
    return tripped
}

[test(description = "A pin that cannot take, unregistered or batch-only, is refused by name and panics only when DASLLAMA_PIN_BACKEND named it, warning when pinned in process, and sets no pin")]
def test_env_pin_refuses_what_cannot_take(t : T?) {
    t |> run("a pin that cannot take: the reason named, the environment's pin panics, an in-process one warns") @(t : T?) {
        t |> equal(kernel_backend_pin_refusal("portable"), "", "the portable backend pins")
        t |> success(!empty(kernel_backend_pin_refusal("no-such-backend")), "an unregistered name is refused by name")
        let seat = kernel_backend_seat()
        t |> success(pin_panics("no-such-backend", "no-such-backend"), "the environment's unregistered pin panics")
        t |> success(!pin_panics("no-such-backend", ""), "the same pin in process warns (the control)")
        t |> success(!pin_panics("no-such-backend", "portable"), "a pin the environment did not name warns")
        t |> success(!pin_panics("portable", "portable"), "the environment's pin that takes runs")
        restore_kernel_backend(seat)
        var checked = 0
        for (n in kernel_backend_names()) {
            continue if (!kernel_backend_batch_only(n))
            t |> success(!empty(kernel_backend_pin_refusal(n)), "batch-only '{n}' is refused by name")
            t |> success(pin_panics(n, n), "the environment's pin of batch-only '{n}' panics, never measuring the default under its name")
            t |> equal(kernel_backend_pin(), seat.pin, "... and sets no pin")
            checked++
        }
        restore_kernel_backend(seat)
        to_log(LOG_INFO, "batch-only backends checked: {checked}\n")
    }
}

// The "+AMX" float-batch override (Accelerate sgemm — AMX on M1-M3, SME on M4+) vs the portable
// fp32/bf16 batch matmuls on deterministic data. sgemm accumulates f32 in coprocessor order —
// relative-tolerance compare, never bit-exact. Below the min-ntok gate the override must fall
// through to the portable body and match it exactly. Off-Apple every check feints cleanly.

let N = 96          // K: weight row length
let D = 48          // output rows
let NTOK = 40       // above the armed min-ntok gate (32)
let SMALL_NTOK = 4  // below the gate — must fall through to the portable body

def private fill01(var a : array<float>; salt : uint) {
    for (i in range(length(a))) {
        a[i] = float(int((uint(i) * 2654435761u + salt * 40503u) & 0xFFFFu)) / 65536.0 - 0.5
    }
}

def private max_rel_err(a, b : array<float>) : float {
    var m = 0.0
    for (va, vb in a, b) {
        m = max(m, nan_wide_diff(va, vb) / max(1.0, abs(va)))
    }
    return m
}

//! `acc` inside `bar` of `ref`, relative to ref's magnitude floored at 1; a NaN on either side is outside
def private rel_ok(ref, acc, bar : float) : bool => abs(ref - acc) / max(1.0, abs(ref)) < bar

//! the tolerance verdict: the max difference logged at its token and row, the last element poisoned by an added 0.5
def private check_rel(t : T?; tag : string; yref, yacc : array<float>; bar : float) {
    var bad = 0
    for (vr, va in yref, yacc) {
        if (!rel_ok(vr, va, bar)) {
            bad++
        }
    }
    let mi = maxdiff_at(yacc, yref)
    to_log(LOG_INFO, "{tag}: max |acc - ref| {abs(yacc[mi] - yref[mi])} at token {mi / D} row {mi % D}\n")
    t |> success(bad == 0, "{tag}: every element within {bar} of the portable form ({bad} of {length(yref)} off)")
    let n = length(yref) - 1
    t |> success(!rel_ok(yref[n] + 0.5, yacc[n], bar), "{tag}: the bar reds a poisoned element")
}

// static_if shims: every driver symbol reference lives behind builtin_module_exists so the
// FILE compiles off-Apple (the [test] arm feints there, but helper bodies still typecheck)
def private t_accel_enable() {
    static_if (typeinfo builtin_module_exists(das_accelerate)) {
        dasllama_accelerate_enable(32l)
    }
}

def private t_accel_disable() {
    static_if (typeinfo builtin_module_exists(das_accelerate)) {
        dasllama_accelerate_disable()
    }
}

def private t_accel_pin_floor() {
    static_if (typeinfo builtin_module_exists(das_accelerate)) {
        dasllama_accelerate_set_min_mmac(0l)
    }
}

[unused_argument(on)]  // off-Apple the else arm never reads it
def private t_accel_set_f16(on : bool) : bool {
    static_if (typeinfo builtin_module_exists(das_accelerate)) {
        return dasllama_accelerate_set_f16(on)
    } else {
        return false
    }
}

def private t_accel_engaged() : int64 {
    static_if (typeinfo builtin_module_exists(das_accelerate)) {
        return dasllama_accelerate_engaged()
    } else {
        return 0l
    }
}

def private t_accel_reset_engaged() {
    static_if (typeinfo builtin_module_exists(das_accelerate)) {
        dasllama_accelerate_reset_engaged()
    }
}

// run the same mm twice — portable (override off) into yref, accelerate-armed into yacc
def private ref_and_accel(ntok : int; run_mm : block<(var y : array<float>) : void>) : tuple<yref : array<float>; yacc : array<float>> {
    var res : tuple<yref : array<float>; yacc : array<float>>
    res.yref |> resize(ntok * D)
    res.yacc |> resize(ntok * D)
    clear_float_batch_override()
    invoke(run_mm, res.yref)
    t_accel_enable()
    invoke(run_mm, res.yacc)
    t_accel_disable()
    return <- res
}

//! The float batch over f32 weights `w` (matmul_batch) or bf16 words (matmul_bf16_batch), portable against armed: bit-exact
//! under `expect_exact`, else within the tolerance. `f16_lane` routes the accelerate half through the BNNS f16 lane (weights
//! bf16->f16 exact, activations f32->f16 rounded, f32 accumulate) — one x rounding step, so a looser envelope.
def private check_batch(t : T?; w : array<auto(WT)>; ntok : int; expect_exact : bool; f16_lane : bool) {
    var x : array<float>
    x |> resize(ntok * N)
    fill01(x, 7u)
    if (f16_lane && !t_accel_set_f16(true)) {
        to_log(LOG_WARNING, "BNNS probe failed on this OS - f16 lane arm feints\n")
        t |> success(true)
        return
    }
    let r <- ref_and_accel(ntok) $(var y) {
        static_if (typeinfo is_float(type<WT>)) {
            matmul_batch(y, w, 0l, x, int64(N), int64(D), int64(ntok))
        } else {
            matmul_bf16_batch(y, w, 0l, x, int64(N), int64(D), int64(ntok))
        }
    }
    static_if (typeinfo is_float(type<WT>)) {
        if (f16_lane) {
            t_accel_set_f16(false)
        }
    } else {
        t_accel_set_f16(false)
    }
    if (expect_exact) {
        for (va, vb in r.yref, r.yacc) {
            t |> equal(va, vb)
        }
    } else {
        let kind = typeinfo is_float(type<WT>) ? "fp32" : "bf16"
        check_rel(t, "{kind} batch ntok {ntok}{f16_lane ? " f16 lane" : ""}", r.yref, r.yacc, f16_lane ? 1e-2 : 1e-4)
    }
}

def private check_fp32(t : T?; ntok : int; expect_exact : bool; f16_lane : bool = false) {
    var w : array<float>
    w |> resize(D * N)
    fill01(w, 3u)
    check_batch(t, w, ntok, expect_exact, f16_lane)
}

def private check_bf16(t : T?; ntok : int; f16_lane : bool) {
    var w : array<float>
    w |> resize(D * N)
    fill01(w, 3u)
    let wbf <- pack_bf16(w)
    check_batch(t, wbf, ntok, false, f16_lane)
}

// the counter a driver reads to tell an armed run that served from one that fell through
def private check_engaged_witness(t : T?) {
    var w : array<float>
    w |> resize(D * N)
    fill01(w, 3u)
    var x : array<float>
    x |> resize(NTOK * N)
    fill01(x, 7u)
    var y : array<float>
    y |> resize(NTOK * D)
    t_accel_reset_engaged()
    clear_float_batch_override()
    matmul_batch(y, w, 0l, x, int64(N), int64(D), int64(NTOK))
    t |> equal(t_accel_engaged(), 0l, "the portable path serves no call")
    t_accel_enable()
    matmul_batch(y, w, 0l, x, int64(N), int64(D), int64(NTOK))
    t |> equal(t_accel_engaged(), 1l, "an armed batch call counts once")
    var ys : array<float>
    ys |> resize(SMALL_NTOK * D)
    matmul_batch(ys, w, 0l, x, int64(N), int64(D), int64(SMALL_NTOK))
    t |> equal(t_accel_engaged(), 1l, "a below-gate call falls through before the counter")
    t_accel_disable()
    t_accel_reset_engaged()
    t |> equal(t_accel_engaged(), 0l, "the reset reads 0")
}

[test(description = "The Accelerate float-batch override (fp32, bf16, BNNS f16 lane, strips) matches the portable batch matmul within a relative bar that reds a poisoned element, and below the min-ntok gate falls through bit-exact"), unused_argument(t)]  // off-Apple the feint arm never reads t
def test_accel_float_batch(t : T?) {
    static_if (typeinfo builtin_module_exists(das_accelerate)) {
        with_dasllama_jobque_() {
            t_accel_pin_floor()  // tiny test shapes must engage, not fall through
            t_accel_set_f16(false)  // pin: env DASLLAMA_ACCEL_F16 must not leak into the sgemm arms
            t |> run("fp32 batch vs portable (tolerance)") @(t : T?) {
                check_fp32(t, NTOK, false)
            }
            t |> run("bf16 batch vs portable (tolerance)") @(t : T?) {
                check_bf16(t, NTOK, false)
            }
            t |> run("bf16 BNNS f16 lane vs portable (tolerance)") @(t : T?) {
                check_bf16(t, NTOK, true)
            }
            t |> run("below-gate ntok falls through bit-exact, and the engagement witness counts what served") @(t : T?) {
                check_fp32(t, SMALL_NTOK, true)
                check_engaged_witness(t)
            }
            t |> run("fp32 BNNS f16 lane vs portable (tolerance)") @(t : T?) {
                check_fp32(t, NTOK, false, true)
            }
            t |> run("bf16 widen scratch clears the unreserved-size guard (E4B PLE-projection class)") @(t : T?) {
                // the widen scratch is d*n floats: past max_unreserved_size (64 MiB) a bare
                // resize panics - E4B's bf16 PLE projection is 27.5M elements (over the line),
                // E2B's 15.7M is under
                let bd = 4200l
                let bn = 4096l
                var w : array<float>
                w |> reserve(bd * bn)
                w |> resize(bd * bn)
                fill01(w, 3u)
                var wbf : array<uint16>
                wbf |> reserve(bd * bn)
                wbf |> resize(bd * bn)
                for (dst, src in wbf, w) {
                    dst = uint16(uint(float_bits_to_int(src)) >> 16u)
                }
                delete w
                var x : array<float>
                x |> resize(NTOK * int(bn))
                fill01(x, 7u)
                var yref : array<float>
                yref |> resize(NTOK * int(bd))
                var yacc : array<float>
                yacc |> resize(NTOK * int(bd))
                clear_float_batch_override()
                matmul_bf16_batch(yref, wbf, 0l, x, bn, bd, int64(NTOK))
                t_accel_enable()
                matmul_bf16_batch(yacc, wbf, 0l, x, bn, bd, int64(NTOK))
                t_accel_disable()
                t |> success(max_rel_err(yref, yacc) < 1e-4)
            }
            t |> run("strips override serves correctly, incl. rows < strips") @(t : T?) {
                dasllama_accelerate_set_strips(8l)
                check_fp32(t, NTOK, false)
                var w : array<float>
                w |> resize(4 * N)
                fill01(w, 5u)
                var x : array<float>
                x |> resize(NTOK * N)
                fill01(x, 9u)
                var yref : array<float>
                yref |> resize(NTOK * 4)
                var yacc : array<float>
                yacc |> resize(NTOK * 4)
                clear_float_batch_override()
                matmul_batch(yref, w, 0l, x, int64(N), 4l, int64(NTOK))
                t_accel_enable()
                matmul_batch(yacc, w, 0l, x, int64(N), 4l, int64(NTOK))
                t_accel_disable()
                dasllama_accelerate_set_strips(0l)
                t |> success(max_rel_err(yref, yacc) < 1e-4)
            }
        }
    } else {
        feint("das_accelerate is not built on this platform; nothing to compare\n")
    }
}
