options gen2
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // load_model_ - the engine load (a tokenizer rides it; load_gguf has none)
require dasllama/dasllama_layout   // convert_model_to_metal_blob - the blob-flavor transform
require ?das_metal dasllama/dasllama_metal_common   // metal_kernel_coverage_of - the dispatch census
require ?das_metal dasllama/dasllama_metal_decode   // the fused attention's knob
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_segment_windows, metal_prefill_stats - the segmented window's witnesses
require ?das_metal dasllama/dasllama_metal_kernels   // b32_moe_wide_index, moe_aff_pair_on - the routed stamps a member's steps take
require math
require strings
require daslib/strings_boost   // split / join: the crown list a cell narrows

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

//! `body` under the fused single-pass attention's knob pinned to `on`, the knob put back after: a census that reads the
//! fused form pins the knob, never inherits it. The body may move the knob itself (the knob-off control leg).
def with_metal_attn_d(on : bool; body : block<void>) {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        let prev = get_metal_attn_d()
        set_metal_attn_d(on)
        invoke(body)
        set_metal_attn_d(prev)
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

//! the blob-flavor twin of a model file, its context capped: what every Metal parity and serving
//! cell loads as the device arm
def blob_twin(t : T?; path : string; seq_cap : int64) : Model {
    var tg <- load_model_(path, QuantMode.q8)
    t |> success(convert_model_to_metal_blob(tg), "metal-blob transform")
    tg.config.seq_len = min(tg.config.seq_len, seq_cap)
    return <- tg
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
