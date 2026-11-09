options gen2
options persistent_heap
options indenting = 4
options _dasllama_internal = true
options _comment_hygiene = true

// The CUDA stamps of the audio towers' block classes and the whisper-class chat tails (`dasllama_cuda_classes_blocks`) on the
// case bodies their Vulkan stamps' cells in `test_vulkan_tower_kernels.das` run (`_discrete_cells.das`): the Conformer rows
// (the GLU, the causal depthwise conv, the windowed RPE attention on both head widths), the FastConformer conv module tails,
// gigaam's f16-tap conv, the chat tails' pair pool and swapped SwiGLU, the rel-position table, the bias-less and plain
// residual seams, the `rsqrt` norm stamps of `dasllama_cuda_classes` (the weighted rms, the gemma4v and qwen25v seams, the
// layernorm and the biased seam into halves), canary's rel-plane attention, and the tower rail's f16 GEMM s column; then the
// tower's GEMM column picks - the f16 tile's column rule over literal SM counts, the Q8_0 cm2 pick at the shapes the fc2 split
// gate's race measured with its seams, and a family's CUDA flavor refusals with the words a load says. Model-free; every cell
// but the f16 column rule, the refusals and the closing leak gate skips without a CUDA device and NVRTC, and the cm2 pick also
// skips under DASLLAMA_CM2_TILE or DASLLAMA_CM2_SPLITK and on a card outside 35 to 48 SMs. The leak gate never skips.

module test_cuda_tower_blocks

require dastest/testing_boost public
require strings   // find: the refusal lines in the child's log
require daslib/defer   // the pick cell's seams put back on every exit
require dasllama/dasllama_lint public
require dasllama/dasllama_cuda_classes   // the f16 GEMM's s and m column stamps
require dasllama/dasllama_cuda_classes_blocks
require dasllama/dasllama_cuda_tower_seat   // cut_cm2_tc, cut_pick and their seams, cut_lane_arms
require dasllama/dasllama_gpu_math   // f16_cm2_col
require dasllama/dasllama_env   // g_env_vulkan: the column and split-count knobs the pick reads
require _cuda_cell
require _discrete_cells
require _child_run   // a load's flavor refusal prints through to_log, which no in-process cell reads

[test(description = "The CUDA Conformer row stamps (GLU, causal depthwise conv, windowed RPE attention at both head widths) match the CPU forms on the shared discrete-cell case")]
def test_cud_tower_conformer(t0 : T?) {
    t0 |> run("CUDA Conformer row classes (GLU, causal depthwise conv, windowed RPE attention on both head widths) == the CPU forms") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_glu_sig_cls() && ensure_tower_dw_conv5_cls() && ensure_tower_g4a_attn64_cls() && ensure_tower_g4a_attn_cls(),
            "the Conformer stamps"))
        with_cu_planes() $(var cp) {
            conformer_case(t, cp)
        }
    }
}

[test(description = "The CUDA FastConformer conv module tails (centered depthwise conv with folded BN and silu, tap-major, and gigaam biased form) match the CPU form")]
def test_cud_tower_fastconformer(t0 : T?) {
    t0 |> run("CUDA FastConformer conv module tails (the centered depthwise conv + folded BN + silu, tap-major, gigaam's biased form) == the CPU form") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_cn_dw_cls() && ensure_tower_pk_dw_cls() && ensure_tower_ga_dw_cls(), "the conv-tail stamps"))
        with_cu_planes() $(var cp) {
            fastconformer_case(t, cp)
        }
    }
}

//! gigaam's f16-tap depthwise conv (`CuTowerGaDwH`) vs the CPU halfword loop, itself bit-exact to the f32 loop; a poisoned tap the control
[test(description = "The CUDA gigaam f16-tap depthwise conv matches the CPU loop over the same halfwords, itself bit-exact to the f32 loop, and a poisoned tap reds it")]
def test_cud_tower_ga_dwh(t0 : T?) {
    t0 |> run("CUDA f16-tap depthwise conv == the CPU loop over the same halfwords") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_ga_dwh_cls(), "tower_ga_dwh"))
        with_cu_planes() $(var cp) {
            ga_dwh_case(t, cp)
        }
    }
}

//! the chat tails' pair pool and swapped gate (`swiglu_swapped_rows`) at an off-multiple width; the unswapped gate the control
[test(description = "The CUDA chat-tail pair pool and swapped SwiGLU rows match the CPU forms at an off-multiple width, with the unswapped gate as the control")]
def test_cud_tower_tail_rows(t0 : T?) {
    t0 |> run("CUDA pair pool and swapped SwiGLU rows == the CPU forms") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_pool_rows_cls() && ensure_tower_swiglu_swapped_cls(), "the tail stamps"))
        with_cu_planes() $(var cp) {
            tail_rows_case(t, cp)
        }
    }
}

//! the Transformer-XL rel table vs the CPU loop in double: rel 0 exactly (0, 1), the pad rows past 2 tt - 1 exactly zero
[test(description = "The CUDA Transformer-XL rel-position sinusoid table matches the CPU double loop, rel 0 exactly (0, 1) and the pad rows past 2 tt - 1 exactly zero")]
def test_cud_tower_rel_sincos(t0 : T?) {
    t0 |> run("CUDA rel-position sinusoid table == the CPU loop, the pad rows zero") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_rel_sincos_cls(), "tower_rel_sincos"))
        with_cu_planes() $(var cp) {
            rel_sincos_case(t, cp)
        }
    }
}

//! the plain seam (`CuTowerPostAddPlain`) with the next rms, `ln_on = 0` the seam alone; the post-norm seam's CPU form the control
[test(description = "The CUDA plain residual seam with its next rms, at ascale 0.5 and with ln_on 0, matches the CPU form, and the post-norm seam CPU form is the control")]
def test_cud_tower_post_add_plain(t0 : T?) {
    t0 |> run("CUDA plain residual seam at ascale 0.5 and ln_on 0 == the CPU form") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_post_add_plain_cls(), "tower_post_add_plain"))
        with_cu_planes() $(var cp) {
            post_add_plain_case(t, cp)
        }
    }
}

//! the bias-less seam (`CuTowerPostAddLnNb`) with the next layernorm, `ln_on = 0` the seam alone; the biased seam's form the control
[test(description = "The CUDA bias-less residual seam with its next layernorm, and alone at ln_on 0, matches the CPU form, with the biased seam form as the control")]
def test_cud_tower_post_add_ln_nb(t0 : T?) {
    t0 |> run("CUDA bias-less residual seam and its next layernorm == the CPU form") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_post_add_ln_nb_cls(), "tower_post_add_ln_nb"))
        with_cu_planes() $(var cp) {
            post_add_ln_nb_case(t, cp)
        }
    }
}

//! the weighted rms rows (`CuTowerRms`, its `rsqrt` held by the case's `NORM_BAR`) vs `rms_rows`; the other weight row the control
[test(description = "The CUDA weighted rms rows match rms_rows within NORM_BAR despite rsqrt, and the other weight row lands outside as the control")]
def test_cud_tower_rms(t0 : T?) {
    t0 |> run("CUDA weighted rms rows == rms_rows") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_rms_cls(), "tower_rms"))
        with_cu_planes() $(var cp) {
            rms_case(t, cp)
        }
    }
}

//! the gemma4v seam (`CuTowerPostAdd`, `rsqrt` in both norms) at ascale 1, ln_on 0 and ascale 0.5; the add without the post-norm the control
[test(description = "The CUDA gemma4v post-normed residual seam and its next rms match the CPU form at ascale 1, at ln_on 0 and at ascale 0.5, with the add minus post-norm as the control")]
def test_cud_tower_post_add(t0 : T?) {
    t0 |> run("CUDA post-normed residual seam and its next rms == the CPU form") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_post_add_cls(), "tower_post_add"))
        with_cu_planes() $(var cp) {
            post_add_case(t, cp)
        }
    }
}

//! the qwen25v seam (`CuTowerPostAddRms`, `rsqrt` in the next rms) and `ln_on = 0`; the add without the bias row the control
[test(description = "The CUDA qwen25v biased residual seam and its next rms, and the seam alone at ln_on 0, match the CPU form, with the add minus the bias row as the control")]
def test_cud_tower_post_add_rms(t0 : T?) {
    t0 |> run("CUDA biased residual seam and its next rms == the CPU form") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_post_add_rms_cls(), "tower_post_add_rms"))
        with_cu_planes() $(var cp) {
            post_add_rms_case(t, cp)
        }
    }
}

//! the layernorm into halves (`CuTowerLn16`) bit for bit its f32 twin's (`CuTowerLn`) rows rounded, both vs `layernorm_batch`
[test(description = "The CUDA layernorm into halves equals its f32 twin rows rounded to f16 bit for bit, and both match layernorm_batch")]
def test_cud_tower_ln16(t0 : T?) {
    t0 |> run("CUDA layernorm into halves == the f32 stamp rounded, and layernorm_batch") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_ln_cls() && ensure_tower_ln16_cls(), "tower_ln and tower_ln16"))
        with_cu_planes() $(var cp) {
            ln16_case(t, cp)
        }
    }
}

//! the seam with the next layernorm into halves (`CuTowerPostAddLn16`) bit for bit its f32 twin's (`CuTowerPostAddLn`), and `ln_on = 0`
[test(description = "The CUDA biased residual seam with its next layernorm into halves equals its f32 twin rounded bit for bit, also at ln_on 0, and matches the CPU form")]
def test_cud_tower_post_add_ln16(t0 : T?) {
    t0 |> run("CUDA biased residual seam with its next layernorm into halves == the f32 stamp rounded, and the CPU form") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_post_add_ln_cls() && ensure_tower_post_add_ln16_cls(), "tower_post_add_ln and tower_post_add_ln16"))
        with_cu_planes() $(var cp) {
            post_add_ln16_case(t, cp)
        }
    }
}

//! canary's rel-plane attention on both head stamps vs the rel-pos loop with u / v biases, idle lanes writing nothing; scaled keys and rel the controls
[test(description = "The CUDA canary rel-plane attention at both head widths matches the CPU rel-pos loop with u and v biases, idle lanes writing nothing, with scaled keys and rel planes as controls")]
def test_cud_tower_cn_attn_rel_plane(t0 : T?) {
    t0 |> run("CUDA rel-plane FastConformer attention (both head widths) over CPU-built planes == the CPU loop") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_cn_attn_r64_cls() && ensure_tower_cn_attn_r_cls(), "the rel-plane attention stamps"))
        with_cu_planes() $(var cp) {
            cn_attn_rel_plane_case(t, cp)
        }
    }
}

//! the f16 GEMM's s column (`CuF16GemmCm2WS`, the tower GEMM sites `cud_f16_tc` puts on the 32-position column) against the f32
//! dot of the same halves and bit for bit its m column twin (`CuF16GemmCm2W`)
[test(description = "The CUDA f16 GEMM s-column stamp matches the f32 dot of the same halves and equals its m-column twin bit for bit")]
def test_cud_f16_gemm_s(t0 : T?) {
    t0 |> run("CUDA f16 GEMM s column == the f32 dot of the same halves, bit for bit the m column") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_f16_gemm_cm2_ws_cls() && ensure_f16_gemm_cm2_w_cls(), "the f16 GEMM's s and m column stamps"))
        with_cu_planes() $(var cp) {
            f16_gemm_s_case(t, cp)
        }
    }
}

//! the f16 tile's column rule: s to 64 rows, m on a card of unknown SM count, past that the cheaper of the two waves
[test(description = "f16_cm2_col picks the 32-wide s column up to 64 rows, the 128-wide m column when the SM count is unknown, and the cheaper wave past that, pinned at literal shapes and SM counts")]
def test_cud_f16_cm2_col(t0 : T?) {
    t0 |> run("f16_cm2_col: the s column to 64 rows, m where the SM count is unknown, the cheaper wave past them") <| @(t : T?) {
        t |> equal(f16_cm2_col(1024l, 64l, 0l), 32l, "64 rows: s, whatever the card")
        t |> equal(f16_cm2_col(1024l, 65l, 0l), 128l, "65 rows on a card of unknown SM count: m")
        t |> equal(f16_cm2_col(1024l, 100l, 0l), 128l, "100 rows on a card of unknown SM count: m")
        t |> equal(f16_cm2_col(1024l, 100l, 36l), 32l, "100 rows x 1024 on 36 SMs: s")
        t |> equal(f16_cm2_col(1024l, 1500l, 36l), 32l, "1500 rows x 1024 on 36 SMs: s")
        t |> equal(f16_cm2_col(1280l, 1500l, 36l), 128l, "1500 rows x 1280 on 36 SMs: m")
        t |> equal(f16_cm2_col(1280l, 1500l, 60l), 128l, "1500 rows x 1280 on 60 SMs: m")
    }
}

let private PICK_SM_LO = 35   //! the fc2 race's picks hold from 35 SMs: under them qwen3a's 52-row site splits four ways
let private PICK_SM_HI = 48   //! ... to 48: past them that site splits seven ways

//! a cm2 pick as `<column>x<k splits>x<chunk>/<workgroups>`
def private pick_words(p : CutPick) : string => "{p.tc}x{p.nsplit}x{p.ksplit}/{p.groups}"

//! `cut_pick` for a [rows x d] output over `n` on a fresh residency of `cap` rows: its words and the split planes it grew
def private tower_pick(d, rows, n : int64; split_ok : bool; cap : int64) : tuple<words : string; parts : int64> {
    var inscope r = CutResident(family = "pick", cap_rows = cap)
    let p = cut_pick(r, d, rows, n, split_ok)
    let parts = r.splitk_parts.bytes
    cut_release(r)
    return (words = pick_words(p), parts = parts)
}

//! the tower's Q8_0 cm2 pick at the fc2 split gate race's shapes (`PERF_LEDGER.md`), each seam answering the value it replaced
[test(description = "On a 35 to 48 SM card the CUDA tower Q8_0 cm2 pick reads the pinned column, split and tile counts at the fc2 race shapes, each seam answers the value it replaced, and the seams are put back")]
def test_cud_tower_cm2_pick(t0 : T?) {
    t0 |> run("the CUDA tower's cm2 pick at the fc2 race's shapes, the forced column held at a split site, each seam put back") <| @(t : T?) {
        return if (!cuda_up(t))
        if (g_env_vulkan.cm2_tile != 0l || g_env_vulkan.cm2_splitk != 0l) {
            t |> skip("DASLLAMA_CM2_TILE={g_env_vulkan.cm2_tile} DASLLAMA_CM2_SPLITK={g_env_vulkan.cm2_splitk} force the pick the cell holds unforced")
            return
        }
        let sms = g_cud.sm_count
        if (sms < PICK_SM_LO || sms > PICK_SM_HI) {
            t |> skip("a card of {sms} SMs: the race's picks hold from {PICK_SM_LO} to {PICK_SM_HI} SMs, where every pinned shape reads one pick")
            return
        }
        let col0 = cuda_tower_cm2_col()
        let fill0 = cuda_tower_splitk_fill()
        let floor0 = cuda_tower_splitk_min_rows()
        {
            defer() {
                set_cuda_tower_cm2_col(col0)
                set_cuda_tower_splitk_fill(fill0)
                set_cuda_tower_splitk_min_rows(floor0)
            }
            t |> equal(set_cuda_tower_cm2_col(0l), col0, "the column seat answers the column it replaced")
            t |> equal(cuda_tower_cm2_col(), 0l, "... no column pinned")
            t |> equal(set_cuda_tower_splitk_fill(4l), fill0, "the fill seat answers the bound it replaced")
            t |> equal(cuda_tower_splitk_fill(), 4l, "... the shipped bound")
            t |> equal(set_cuda_tower_splitk_min_rows(0l), floor0, "the floor seat answers the floor it replaced")
            t |> equal(cuda_tower_splitk_min_rows(), 0l, "... no row floor")
            t |> equal(cut_cm2_tc(8192l, 64l), 32l, "64 rows: the s column, where the wave model would take m")
            t |> equal(cut_cm2_tc(384l, 65l), 128l, "65 rows: the m column, where the wave model would take s")
            t |> equal(cut_cm2_tc(384l, 128l), 128l, "128 rows: the m column")
            t |> equal(cut_cm2_tc(384l, 129l), 32l, "129 rows: the wave model's column, s")
            let tiny = tower_pick(384l, 1500l, 1536l, true, 1500l)
            t |> equal(tiny.words, "128x1x0/36", "whisper tiny's fc2 site at 1500 rows: m whole, 36 tiles")
            t |> equal(tiny.parts, 0l, "... a whole pick grows no split planes")
            t |> equal(tower_pick(1280l, 1500l, 5120l, true, 1500l).words, "128x1x0/120", "whisper turbo's fc2 site at 1500 rows: m whole, 120 tiles")
            let q104 = tower_pick(896l, 104l, 3584l, true, 104l)
            t |> equal(q104.words, "256x7x512/49", "qwen3a 0.6B's fc2 site at its 104-row window: split 7 ways onto the l column")
            t |> equal(q104.parts, 7l * 104l * 896l * 4l, "... its split planes grown to seven of the window's rows")
            t |> equal(tower_pick(896l, 52l, 3584l, true, 52l).words, "32x5x768/70", "... at its 52-row window: the s column split 5 ways")
            t |> equal(tower_pick(896l, 104l, 3584l, false, 104l).words, "128x1x0/7", "a site that takes no split: the wave model's m whole, never the widened column")
            t |> equal(pick_words(cut_pick_whole(896l, 104l)), "128x1x0/7", "the whole pick at the same shape")
            t |> equal(set_cuda_tower_splitk_fill(0l), 4l, "the fill seat answers the shipped bound")
            t |> equal(tower_pick(384l, 1500l, 1536l, true, 1500l).words, "256x3x512/54", "no fill bound: tiny's fc2 site splits 3 ways onto l, the race's no-gate arm")
            t |> equal(set_cuda_tower_splitk_min_rows(2000l), 0l, "the floor seat answers no floor")
            t |> equal(tower_pick(384l, 1500l, 1536l, true, 1500l).words, "128x1x0/36", "a floor past the site's rows: whole")
            t |> equal(set_cuda_tower_splitk_min_rows(-1l), 2000l, "the floor seat answers the floor it replaced")
            t |> equal(cuda_tower_splitk_min_rows(), 0l, "... a negative floor clamps to none")
            t |> equal(set_cuda_tower_splitk_fill(-3l), 0l, "the fill seat answers no bound")
            t |> equal(cuda_tower_splitk_fill(), 0l, "... a negative bound clamps to none")
            t |> equal(set_cuda_tower_splitk_fill(4l), 0l, "the shipped bound back")
            t |> equal(set_cuda_tower_cm2_col(128l), 0l, "the column seat answers no pin")
            t |> equal(cuda_tower_cm2_col(), 128l, "... the m column pinned")
            t |> equal(tower_pick(896l, 104l, 3584l, true, 104l).words, "128x7x512/49", "the forced m column holds at a split site the pick widens to l")
            t |> equal(cut_cm2_tc(384l, 64l), 128l, "a forced column beats the rows rule")
            t |> equal(set_cuda_tower_cm2_col(32l), 128l, "the column seat answers the m pin")
            t |> equal(tower_pick(896l, 104l, 3584l, true, 104l).words, "32x1x0/28", "a forced s column whose 28 tiles the fill bound keeps whole")
            t |> equal(set_cuda_tower_cm2_col(64l), 32l, "the column seat answers the s pin")
            t |> equal(cuda_tower_cm2_col(), 0l, "... a column no tile has pins none")
        }
        t |> equal(cuda_tower_cm2_col(), col0, "the column seat put back as found")
        t |> equal(cuda_tower_splitk_fill(), fill0, "the fill seat put back as found")
        t |> equal(cuda_tower_splitk_min_rows(), floor0, "the floor seat put back as found")
    }
}

//! a family's CUDA flavor stood down at load: the tower knob off, then no home armed - the line a load says read off a child's log
[test(description = "qwen3a takes no CUDA flavor with the tower knob off or with no home armed, and a child load logs the exact line naming each condition")]
def test_cud_tower_flavor_refusals(t0 : T?) {
    t0 |> run("a family takes no CUDA flavor with the tower knob off or no home armed, its line naming the condition") <| @(t : T?) {
        let was = cuda_tower_on()
        set_cuda_tower(false)
        let knob = cut_lane_arms("qwen3a", true)
        set_cuda_tower(was)
        t |> equal(knob, false, "the tower knob off: no CUDA flavor")
        t |> equal(cuda_tower_on(), was, "the knob put back as found")
        var out : string
        let envs <- [(name = "DAS_LOG_LEVEL", value = "info"), (name = "DASLLAMA_GPU", value = ""), (name = "DASLLAMA_CUDA", value = "")]
        let rc = run_child(child_cmd("_flavor_decline_root.das", envs, "cut"), out)
        t |> equal(rc, 0, "the decline root ran:\n{out}")
        t |> success(find(out, "ROOT_DONE") >= 0, "the decline root reached its end:\n{out}")
        t |> success(find(out, "CUT knob false") >= 0 && find(out, "CUT device false") >= 0, "both arms refuse:\n{out}")
        t |> success(find(out, "dasLLAMA cuda tower: qwen3a takes no CUDA flavor - the tower knob is off (set_cuda_tower(false)); the load maps the CPU image") >= 0,
            "the knob's line")
        t |> success(find(out, "dasLLAMA cuda tower: qwen3a takes no CUDA flavor - the home has no device (DASLLAMA_GPU=1 DASLLAMA_CUDA=1 arm it), or a class failed to build; the load maps the CPU image") >= 0,
            "with no home armed, the device's line")
    }
}

//! the compile gate: every stamp of `dasllama_cuda_classes_blocks` compiles through NVRTC and loads
[test(description = "Every cuda_dispatch stamp of the tower blocks file compiles through NVRTC and loads, none refused")]
def test_cud_blocks_compile_gate(t0 : T?) {
    t0 |> run("every [cuda_dispatch] stamp of the block file compiles and loads") <| @(t : T?) {
        return if (!cuda_up(t))
        let built <- [
            ("tower_rel_sincos_cls", ensure_tower_rel_sincos_cls()), ("tower_cn_dw_cls", ensure_tower_cn_dw_cls()),
            ("tower_pk_dw_cls", ensure_tower_pk_dw_cls()), ("tower_ga_dw_cls", ensure_tower_ga_dw_cls()),
            ("tower_ga_dwh_cls", ensure_tower_ga_dwh_cls()), ("tower_dw_conv5_cls", ensure_tower_dw_conv5_cls()),
            ("tower_glu_sig_cls", ensure_tower_glu_sig_cls()), ("tower_swiglu_swapped_cls", ensure_tower_swiglu_swapped_cls()),
            ("tower_pool_rows_cls", ensure_tower_pool_rows_cls()), ("tower_post_add_ln_nb_cls", ensure_tower_post_add_ln_nb_cls()),
            ("tower_post_add_plain_cls", ensure_tower_post_add_plain_cls()), ("tower_g4a_attn_cls", ensure_tower_g4a_attn_cls()),
            ("tower_g4a_attn64_cls", ensure_tower_g4a_attn64_cls()), ("tower_cn_attn_r_cls", ensure_tower_cn_attn_r_cls()),
            ("tower_cn_attn_r64_cls", ensure_tower_cn_attn_r64_cls())]
        let refused <- [for (b in built); b._0; where !b._1]
        t |> equal(length(refused), 0, "every stamp compiles and loads; refused: {refused} ({cuda_last_error()})")
    }
}

//! the leak gate: every device buffer the cells uploaded freed, every module released at the shutdown, nothing counted live
[test(description = "After the block cells, the CUDA tier shutdown leaves no CUDA object or device allocation live, as counted by the leak log")]
def test_cud_blocks_shutdown_leaks(t0 : T?) {
    t0 |> run("the CUDA tier shuts down with nothing live after the block cells") <| @(t : T?) {
        cud_shutdown()
        t |> equal(cuda_log_leaks_if_any(), 0l, "no CUDA object or allocation outlives the tier")
    }
}
