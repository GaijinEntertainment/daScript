options gen2
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends
options indenting = 4
options _comment_hygiene = true
options _dasllama_internal = true

// The CUDA home's ASR decoder and VAD stamps (`dasllama_cuda_classes_asr.das`, the oracle-only combine in `_cuda_cell.das`) on
// the case bodies the Vulkan cells of `test_vulkan_tower_kernels.das` run (`_discrete_cells.das`): the whisper decoder's K/V
// store, chunked attention and fused Q8_0 row passes with the clamped Q8_0 feed beside them, the transducer's gates,
// projection, joint and pick, the CTC head's pick and the silero VAD's encoder and recurrence. Model-free; every cell but the
// closing leak gate skips without a CUDA device and NVRTC, and a stamp NVRTC refuses on a box with a device reds its cell. The
// leak gate never skips.

require dastest/testing_boost public
require dasllama/dasllama_lint public
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_cuda_classes_asr
require _cuda_cell
require _discrete_cells

[test(description = "The whisper decoder K/V store in its three layouts and the chunked causal and cross attention pair on the CUDA home match the CPU decoder forms, with causal-versus-cross and scaled-keys controls")]
def test_cud_wdec_classes(t0 : T?) {
    t0 |> run("the whisper decoder's K/V store and chunked attention pair on the CUDA home == the CPU decoder forms") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_wdec_kv_cls() && ensure_tower_wdec_kv16_cls() && ensure_tower_wdec_attn_part_cls() && ensure_tower_wdec_attn_comb_cls(), "the K/V store and attention stamps"))
        with_cu_planes() $(var cp) {
            wdec_classes_case(t, cp)
        }
    }
}

[test(description = "The layernorm, post-add, bias-activation and attention combine fused with the Q8_0 requant on the CUDA home match the separate pass then requant and the CPU forms at three widths incl. whisper tiny and turbo")]
def test_cud_wdec_fused_rq(t0 : T?) {
    t0 |> run("the layernorm, the post-add, the bias-activation and the attention combine fused with the Q8_0 requant on the CUDA home == the pass then the requant and the CPU forms, at the other cells' width, whisper tiny's and turbo's") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_ln_rq_cls() && ensure_tower_post_add_ln_rq_cls() && ensure_tower_bias_act_rq_cls() && ensure_tower_wdec_attn_comb_rq_cls()
            && ensure_tower_ln_cls() && ensure_tower_post_add_ln_cls() && ensure_tower_bias_act_cls() && ensure_tower_clamp_rq_cls() && ensure_tower_wdec_attn_comb_cls(), "the fused row passes and their separate twins"))
        with_cu_planes() $(var cp) {
            wdec_fused_rq_case(t, cp)
        }
    }
}

//! the clamped Q8_0 feed (`CuTowerClampRq`) against the CPU clamp and requant; a poisoned scale reds it, the unclamped requant misses
[test(description = "tower_clamp_rq on the CUDA home reproduces the CPU clamp then Q8_0 requant of the rows, a poisoned scale reds it, and the unclamped requant misses as the control")]
def test_cud_tower_clamp_rq(t0 : T?) {
    t0 |> run("tower_clamp_rq on the CUDA home == the clamped rows requantized as the CPU does") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_clamp_rq_cls(), "tower_clamp_rq_cls"))
        with_cu_planes() $(var cp) {
            clamp_rq_case(t, cp)
        }
    }
}

//! the silero VAD's two stamps against `_vad_oracle.das` at the approx bar: the encoder over three chunks, the recurrence at one and five
[test(description = "The silero VAD encoder over three chunks and its recurrence at one and five chunks from a nonzero state on the CUDA home match the CPU VAD oracle at the approx bar")]
def test_cud_tower_vad(t0 : T?) {
    t0 |> run("the silero VAD's encoder and recurrence on the CUDA home == the CPU references") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_vad_enc_cls() && ensure_tower_vad_lstm_cls(), "the VAD stamps"))
        with_cu_planes() $(var cp) {
            vad_case(t, cp)
        }
    }
}

//! the transducer decode's four stamps on the Vulkan cell's legs (`test_vkt_rnnt`) at both `rc_shapes`, against `_rnnt_oracle.das`
[test(description = "The transducer decode gates, projection, joint and pick stamps on the CUDA home match the CPU RNNT oracle at both rc_shapes")]
def test_cud_rnnt(t0 : T?) {
    t0 |> run("the transducer decode's gates, projection, joint and pick on the CUDA home == the CPU references") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_rnnt_gates_cls() && ensure_rnnt_proj_cls() && ensure_rnnt_joint_cls() && ensure_rnnt_pick_cls(), "the transducer stamps"))
        with_cu_planes() $(var cp) {
            rnnt_case(t, cp)
        }
    }
}

//! the CTC head's pick on `ctc_pick_cases` (the Vulkan cell's widths, tie, NaN pads, all-NaN row) against the CPU row loop
[test(description = "The CTC pick on the CUDA home writes every record with the CPU class exactly and plogs within 1e-5 relative over three widths, a tie taking the lower class and an all-NaN row class 0, a poisoned bias the control")]
def test_cud_ctc_pick(t0 : T?) {
    t0 |> run("the CTC head's pick on the CUDA home == the CPU row loop, a tie picking the lower class, a NaN row picking class 0") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_ctc_pick_cls(), "the CTC pick stamp"))
        with_cu_planes() $(var cp) {
            ctc_pick_case(t, cp)
        }
    }
}

//! the leak gate: every device buffer the cells above made is freed, every module released at the shutdown, nothing counted live
[test(description = "After the ASR stamp cells, the CUDA tier shutdown leaves no CUDA object or device allocation live, as counted by the leak log")]
def test_cud_asr_shutdown_leaks(t0 : T?) {
    t0 |> run("the CUDA tier shuts down with nothing live after the ASR stamps' cells") <| @(t : T?) {
        cud_init()
        cud_shutdown()
        t |> equal(cuda_log_leaks_if_any(), 0l, "no CUDA object or allocation outlives the tier")
    }
}
