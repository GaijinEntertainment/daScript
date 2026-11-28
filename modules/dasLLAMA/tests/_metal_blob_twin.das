options gen2
options _dasllama_internal = true

module _metal_blob_twin shared public

require dastest/testing_boost public
require dasllama/dasllama   // load_model_ - the engine load (a tokenizer rides it; load_gguf has none)
require dasllama/dasllama_layout   // convert_model_to_metal_blob - the blob-flavor transform
require dasllama/dasllama_common   // get_metal_mode / set_metal_mode: the home's mode a Metal form puts back
require dasllama/dasllama_math   // kernel_backend_seat: the backend pin a Metal mode leaves
require ?das_metal dasllama/dasllama_metal_common   // metal_kernel_coverage_of - the dispatch census
require ?das_metal dasllama/dasllama_metal_decode   // the fused attention's knob
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_segment_windows, metal_prefill_stats - the segmented window's witnesses
require ?das_metal dasllama/dasllama_metal_kernels   // b32_moe_wide_index, moe_aff_pair_on - the routed stamps a member's steps take
require ?das_metal dasllama/dasllama_metal_tower   // metal_tower_shutdown: a form's close
require ?das_metal dasllama/dasllama_metal_dflash   // metal_dflash_shutdown: a form's close
require ?das_metal metal/das_metal_boost   // metal_live_object_count: a form's leak check
require math
require strings
require daslib/strings_boost   // split / join: the crown list a cell narrows
require _compares   // argmax_of: a batched row's greedy pick

//! the comma list `crowns` with `drop`'s names left out - the box profile's crowns less the family a leg wants on its base form
def crowns_without(crowns : string; drop : array<string>) : string {
    var inscope parts <- split(crowns, ",")
    var inscope kept <- [for (c in parts); c; where find_index(drop, c) < 0]
    return join(kept, ",")
}

//! one Metal kernel's dispatch count since the census reset - 0 for a compiled kernel that never ran, a panic for a name no
//! compiled kernel carries (`metal_kernel_coverage_of`), and 0 on a build without das_metal
def metal_kernel_count(name : string) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_kernel_coverage_of(name)
    } else {
        return 0l
    }
}

//! the same count for a kernel a lever may never have compiled (a tensor twin under the dev-W panel lever): 0 where no
//! compiled kernel carries the name, never a panic
def metal_kernel_count_compiled(name : string) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope cov <- metal_kernel_coverage()
        return cov?[name] ?? 0l
    } else {
        return 0l
    }
}

//! the dispatches since the census reset of every compiled kernel whose name opens with `prefix` - a template's every
//! stamp at once (the base tile, its split and tensor arms), 0 where none is compiled
def metal_kernel_count_prefix(prefix : string) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope cov <- metal_kernel_coverage()
        var n = 0l
        for (name, cnt in keys(cov), values(cov)) {
            if (starts_with(name, prefix)) {
                n += cnt
            }
        }
        return n
    } else {
        return 0l
    }
}

//! a per-32 member's expert-indexed decode dispatches since the census reset, counted in routed stacks served, `cls` its
//! class stem (`Q51`, `Mlx4b`): the two-row routed GEMV and, where the member has it (`B32_MOE_WIDE_FMTS`), its wide stamp
//! a stack each; where the routed pair's affine arms serve the member (`moe_aff_pair_on`), the fused gate-and-up stamp two
//! stacks and the down stamp one
def routed_gemv_dispatches(f : KqFmt; cls : string) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let wide = b32_moe_wide_index(f) >= 0 ? metal_kernel_count("MetalMoeGemv{cls}W_metal_moe_gemv_w") : 0l
        var n = metal_kernel_count("metal_moe_gemv_{f}") + wide
        if (moe_aff_pair_on(f)) {
            n += 2l * metal_kernel_count_prefix("MetalMoeGu{cls}_") + metal_kernel_count_prefix("metal_moe_down2_{f}")
        }
        return n
    } else {
        return 0l
    }
}

//! the steps' routed form held by the census: under the routed pair's affine arms every step's gate-and-up rode the fused
//! stamp, else - the member on the wide stamp's list and the knob on - every step's expert rows rode the wide form
def routed_steps_form_cell(t : T?; f : KqFmt; cls : string; tag : string; steps : int64; n_layers : int64) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (moe_aff_pair_on(f)) {
            let gp = metal_kernel_count_prefix("MetalMoeGu{cls}_")
            t |> success(gp >= steps * n_layers, "{tag}: every step's routed gate and up rode the fused affine pair ({gp} dispatches, {steps * n_layers} layers x steps)")
        } else {
            let gw = metal_kernel_count("MetalMoeGemv{cls}W_metal_moe_gemv_w")
            t |> success(gw >= steps * 3l * n_layers || !get_metal_b32_moe_wide(), "{tag}: every step's expert rows rode the wide form ({gw} dispatches)")
        }
    }
}

//! `body` under a knob moved to `to` through `set`, the knob put back to `prev` after
def private under_knob(prev, to : auto(TT); set : function<(v : TT -const) : void>; body : block<void>) {
    invoke(set, to)
    invoke(body)
    invoke(set, prev)
}

//! `body` under the fused single-pass attention's knob pinned to `on`, the knob put back after: a census that reads the
//! fused form pins the knob, never inherits it. The body may move the knob itself (the knob-off control leg).
def with_metal_attn_d(on : bool; body : block<void>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        under_knob(get_metal_attn_d(), on, @@set_metal_attn_d, body)
    } else {
        invoke(body)
    }
}

//! `body` under the tile attention's far count pinned to `far` (0 = every depth, negative = never), the count put back after
def with_metal_attn_tile_far(far : int64; body : block<void>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        under_knob(get_metal_attn_tile_far(), far, @@set_metal_attn_tile_far, body)
    } else {
        invoke(body)
    }
}

//! the overrides a Metal cell selects and the prefill's npos floor it lowers, as the process held them before the cell's first
//! write: `metal_overrides_seat` reads them, `restore_metal_overrides` puts them back
struct MetalOverridesSeat {
    prefill : string
    decode : string
    min_npos : int64
}

def metal_overrides_seat() : MetalOverridesSeat {
    var seat = MetalOverridesSeat(prefill = active_prefill_override(), decode = active_decode_override())
    static_if (typeinfo builtin_module_exists(das_metal)) {
        seat.min_npos = get_metal_prefill_min_npos()
    }
    return seat
}

def restore_metal_overrides(seat : MetalOverridesSeat) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_prefill_min_npos(seat.min_npos)
    }
    select_decode_override(seat.decode)
    select_prefill_override(seat.prefill)
}

//! which overrides a Metal cell selects, to which rail, and the
//! prefill floor it lowers to (0 leaves it)
struct MetalRails {
    prefill : bool = true
    decode : bool = true
    rail : string = "metal"
    min_npos : int64 = 1l
}

//! what the selects of `select_rails` answered
struct private RailsSelected {
    prefill : bool
    decode : bool
}

def private select_rails(rails : MetalRails) : RailsSelected {
    var got = RailsSelected(prefill = true, decode = true)
    if (rails.prefill) {
        got.prefill = select_prefill_override(rails.rail)
    }
    if (rails.decode) {
        got.decode = select_decode_override(rails.rail)
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (rails.min_npos > 0l) {
            set_metal_prefill_min_npos(rails.min_npos)
        }
    }
    return got
}

//! `body` under `rails`: both overrides and the floor put back from
//! the seat after, whichever `rails` wrote
def with_metal_rails(rails : MetalRails; body : block<void>) {
    let seat = metal_overrides_seat()
    select_rails(rails)
    invoke(body)
    restore_metal_overrides(seat)
}

//! the same, each select `rails` makes asserted under `tag`
def with_metal_rails(t : T?; tag : string; rails : MetalRails; body : block<void>) {
    let seat = metal_overrides_seat()
    let got = select_rails(rails)
    t |> success(got.prefill, "{tag}: prefill override registered")
    t |> success(got.decode, "{tag}: decode override registered")
    invoke(body)
    restore_metal_overrides(seat)
}

//! `prompt` through `s` on the Metal prefill, its floor at 1: the
//! prefill override and the floor put back, the decode override
//! never written; nothing off das_metal
[unused_argument(m, s, prompt)]
def metal_prefill_eval(m : Model; var s : Session; prompt : array<int64>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let floor0 = get_metal_prefill_min_npos()
        let prefill0 = active_prefill_override()
        select_prefill_override("metal")
        set_metal_prefill_min_npos(1l)
        eval_(m, s, prompt)
        select_prefill_override(prefill0)
        set_metal_prefill_min_npos(floor0)
    }
}

//! the Metal decodes served so far, 0 off das_metal
def metal_decode_count() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_decode_stats().decodes
    } else {
        return 0l
    }
}

//! the Metal batched driver's steps so far, 0 off das_metal
def metal_batch_step_count() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_batch_decode_stats().steps
    } else {
        return 0l
    }
}

//! the Metal prefills served so far, 0 off das_metal
def metal_prefill_count() : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return metal_prefill_stats().prefills
    } else {
        return 0l
    }
}

//! the Metal prefill's declines for `why` so far, 0 off das_metal
[unused_argument(why)]
def metal_prefill_decline_count(why : string) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope d <- metal_prefill_declines()
        return d?[why] ?? 0l
    } else {
        return 0l
    }
}

//! the Metal batch rail's declines for `why` so far, 0 off das_metal
[unused_argument(why)]
def metal_batch_decline_count(why : string) : int64 {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope d <- metal_batch_decode_declines()
        return d?[why] ?? 0l
    } else {
        return 0l
    }
}

//! the per-32 dense formats, `kq_dense_b32`'s members
let PER32_FMTS = fixed_array(KqFmt.q51, KqFmt.iq4nl32, KqFmt.q4032, KqFmt.q5032, KqFmt.mlx4,
    KqFmt.mlx8, KqFmt.mlx4b, KqFmt.mlx8b, KqFmt.mlx6b, KqFmt.q20, KqFmt.q10)

//! the class stem a format's Metal stamps carry (`Q51`, `Mlx4b`)
def kq_stem(f : KqFmt) : string => capitalize("{f}")

//! `f`'s one-row GEMV planes read since the census reset: the
//! two-row and K-split stamps, the wide one, and twice each fused
//! gate|up dispatch - it reads two planes
def kq_gemv_dispatches(f : KqFmt) : int64 {
    let s = kq_stem(f)
    return (metal_kernel_count("metal_kq_gemv_{f}") + metal_kernel_count("metal_kq_gemv_{f}_ks")
        + metal_kernel_count("MetalKqGemv{s}W_metal_b32_gemv_w")
        + 2l * metal_kernel_count("MetalKqGemv{s}Glu_metal_b32_gemv_w"))
}

//! `f`'s small-batch dispatches since the census reset: the B2, B4
//! and B8 stamps, plain and K-split
def kq_mvb_dispatches(f : KqFmt) : int64 {
    let s = kq_stem(f)
    var n = 0l
    for (ks in ["", "Ks"]) {
        for (w in [2, 4, 8]) {
            n += metal_kernel_count("MetalB32MvB{w}{s}{ks}_metal_b32_mvb")
        }
    }
    return n
}

//! `f`'s dequant pass, tensor twins and double-buffered and split-K
//! GEMM stamps since the census reset; under `compiled` a twin or
//! GEMM stamp never compiled reads 0, else it panics
def kq_twin_dispatches(f : KqFmt; compiled : bool) : int64 {
    let s = kq_stem(f)
    var n = metal_kernel_count("MetalKqDequant{s}_metal_kq_dequant")
    for (v in ["T", "TH", "TH128"]) {
        let name = "MetalKqMulMm{s}{v}_metal_kq_mulmm_b32_t"
        n += compiled ? metal_kernel_count_compiled(name) : metal_kernel_count(name)
    }
    for (v in ["THDb", "THDb128", "THSk", "TSk", "THSk16", "TSk16"]) {
        let name = "MetalKqMulMm{s}{v}_metal_kq_mulmm_db"
        n += compiled ? metal_kernel_count_compiled(name) : metal_kernel_count(name)
    }
    return n
}

//! every per-32 dispatch a window of `f` takes since the census
//! reset: the base GEMM, its split stamp (0 uncompiled), the twins
//! and dequant (a panic uncompiled where the box profile crowns the member's
//! tensor form, 0 where it does not - the twins compile under the crown),
//! the small-batch forms, the GEMV
def kq_window_dispatches(f : KqFmt) : int64 {
    let s = kq_stem(f)
    return (metal_kernel_count("MetalKqMulMm{s}_metal_kq_mulmm")
        + metal_kernel_count_compiled("MetalKqMulMm{s}Sk_metal_kq_mulmm")
        + kq_twin_dispatches(f, !metal_tensor_crowned("kq_mulmm_{f}")) + kq_mvb_dispatches(f) + kq_gemv_dispatches(f))
}

//! `f`'s routed tensor twins (`T`, `TH`, `TH128`, `THR`) on the
//! stamp body `body` since the census reset; under `compiled` a twin
//! never compiled reads 0, else it panics
def routed_twin_dispatches(f : KqFmt; body : string; compiled : bool) : int64 {
    var n = 0l
    for (v in ["T", "TH", "TH128", "THR"]) {
        let name = "MetalMoeMulMm{kq_stem(f)}{v}_{body}"
        n += compiled ? metal_kernel_count_compiled(name) : metal_kernel_count(name)
    }
    return n
}

def private prefill_driver_down(m : Model) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        if (m.metal_blob) {
            metal_prefill_shutdown()
        }
    }
}

//! each prompt's greedy picks over `nsteps` `eval_batch_` steps, a
//! row per prompt: row 1 retired after step `shrink_at`, every
//! `single_every`-th step one-row forwards in its place, the prefill
//! driver shut after the prompts under `pf_down`, and each row's
//! pick off its prompt first under `with_first`. Takes no job queue
def batch_greedy_rows(m : Model; prompts : array<array<int64>>; nsteps : int;
                      kdt : KVDtype = KVDtype.f16; shrink_at : int = -1;
                      single_every : int = 0; pf_down : bool = false;
                      with_first : bool = false) : array<array<int64>> {
    let c = m.config
    let vocab = c.vocab_size
    let nb = length(prompts)
    var got : array<array<int64>>
    got |> resize(nb)
    for (row in got) {
        row |> reserve(nsteps + 1)
    }
    var ss : array<Session>
    ss |> reserve(nb)
    for (i in range(nb)) {
        ss |> emplace <| make_run_state(c, kdt, kdt)
        eval_(m, ss[i], prompts[i])
    }
    if (pf_down) {
        prefill_driver_down(m)
    }
    var ws <- create_batch_workspace_(m)
    var sess : array<Session?>
    var cur : array<int64>
    var live : array<int>
    sess |> reserve(nb)
    cur |> reserve(nb)
    live |> reserve(nb)
    for (i in range(nb)) {
        sess |> push(unsafe(addr(ss[i])))
        cur |> push(argmax_of(ss[i].logits, vocab))
        live |> push(i)
        if (with_first) {
            got[i] |> push(cur[i])
        }
    }
    for (st in range(nsteps)) {
        if (single_every > 0 && (st + 1) % single_every == 0) {
            for (sp, pick in sess, cur) {
                forward(m, *sp, pick, sp.n_past)
                sp.n_past++
                pick = argmax_of(sp.logits, vocab)
            }
        } else {
            eval_batch_(m, ws, sess, cur)
            for (sp, pick in sess, cur) {
                pick = argmax_of(sp.logits, vocab)
            }
        }
        for (r, pick in live, cur) {
            got[r] |> push(pick)   // nolint:PERF006 - every row reserved above
        }
        if (st == shrink_at && length(sess) > 1) {
            sess |> erase(1)
            cur |> erase(1)
            live |> erase(1)
        }
    }
    sess |> clear()   //! borrowed: delete of array<T?> frees pointees
    unsafe {
        delete ws
        delete sess
    }
    delete cur
    delete live
    delete ss
    return <- got
}

//! two long prose prompts the Metal decode and MTP cells continue
let PROSE_LIGHTHOUSE = ("The old lighthouse keeper watched the storm roll in from the west, " +
    "counting the seconds between each flash and the thunder that followed. By midnight the " +
    "waves were breaking over the jetty and")
let PROSE_RIVER = ("In the spring of that year the river rose higher than anyone in the " +
    "valley could remember, and the farmers along the lower fields moved their animals to the " +
    "ridge while")

//! the Metal home as a block's Metal form found it: the mode, the overrides and the prefill floor a mode or a cell selects, and
//! the kernel backend with its pin - a Metal mode pins "portable", and the mode going back off leaves that pin standing
struct MetalHomeSeat {
    mode : MetalMode
    overrides : MetalOverridesSeat
    backend : KernelBackendSeat
}

def metal_home_seat() : MetalHomeSeat =>
    MetalHomeSeat(mode = get_metal_mode(), overrides = metal_overrides_seat(), backend = kernel_backend_seat())

//! the Metal home put back as `seat` recorded it: the mode first (it writes the overrides), then the overrides, then the backend
def restore_metal_home(seat : MetalHomeSeat) {
    set_metal_mode(seat.mode)
    restore_metal_overrides(seat.overrides)
    restore_kernel_backend(seat.backend)
}

//! the blob-flavor twin of a model file, its context capped: what every Metal parity and serving
//! cell loads as the device arm
def blob_twin(t : T?; path : string; seq_cap : int64) : Model {
    var tg <- load_model_(path, QuantMode.q8)
    t |> success(convert_model_to_metal_blob(tg), "metal-blob transform")
    tg.config.seq_len = min(tg.config.seq_len, seq_cap)
    return <- tg
}

//! a Metal form's model `m` closed: the decode, prefill and tower
//! drivers shut down, no Metal object live with `m` still loaded,
//! then `m` deleted and still none live, `what` naming the form
[unused_argument(t, what)]   // without das_metal the leak check is empty
def metal_form_close(t : T?; var m : Model; what : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        metal_tower_shutdown()
        metal_dflash_shutdown()
        metal_decode_shutdown()
        metal_prefill_shutdown()
        t |> equal(metal_live_object_count(), 0l,
            ("{what}: no Metal object left live after the drivers' shutdown, " +
            "the model still loaded: {metal_live_object_report()}"))
    }
    delete m
    static_if (typeinfo builtin_module_exists(das_metal)) {
        t |> equal(metal_live_object_count(), 0l,
            ("{what}: no Metal object left live after the model's delete: " +
            "{metal_live_object_report()}"))
    }
}

//! Two sessions' chunks `ta` and `tb` as ONE prefill window: the segmented window's witness rises by one and the
//! prefill count by one, both asserted under `tag`.
def segments_window(t : T?; tr : Model; var sa, sb : Session; ta, tb : array<int64>; tag : string) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var sess <- [unsafe(addr(sa)), unsafe(addr(sb))]
        var toks : array<array<int64>>
        toks |> resize(2)
        toks[0] := ta
        toks[1] := tb
        let sw0 = metal_prefill_segment_windows()
        let pf0 = metal_prefill_stats().prefills
        eval_segments(tr, sess, toks)
        t |> equal(metal_prefill_segment_windows() - sw0, 1l, "{tag}: both chunks rode one window")
        t |> equal(metal_prefill_stats().prefills - pf0, 1l, "{tag}: the window was one prefill the device served")
        sess |> clear()   //! borrowed pointers: the caller owns the sessions
        delete toks
    }
}

//! "1, 2, ..., n, " - the confident counting prompt, long enough that a window's sites ride the tiles
def counting_text(n : int) : string {
    return build_string() $(var w) {
        for (i in range(1, n + 1)) {
            w |> write(i)
            w |> write(", ")
        }
    }
}
