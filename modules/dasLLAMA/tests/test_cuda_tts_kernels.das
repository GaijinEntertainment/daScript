options gen2
options persistent_heap
options indenting = 4
options _dasllama_internal = true

// The TTS chains' CUDA stamps (`dasllama_cuda_classes_tts`, and the f32 tile GEMM, its stored members and the row gather of
// `dasllama_cuda_classes`) on the case bodies the Vulkan twins' cells run (`_discrete_tts_cells.das`,
// `_discrete_tts_conv_cells.das`, `_discrete_tts_pocket_cells.das`, `_discrete_tts_source_cells.das`), one cell a stamp
// family. Model-free; every cell skips without a CUDA device and NVRTC, and a stamp that does not compile on a device reds
// its cell.

require dastest/testing_boost public
require dasllama/dasllama_lint public
require dasllama/dasllama_cuda_classes
require dasllama/dasllama_cuda_classes_tts
require dasllama/dasllama_cuda_rail   // rail_tts_shot: the seats' one-shot
require _cuda_cell
require _discrete_tts_cells
require _discrete_tts_conv_cells
require _discrete_tts_pocket_cells
require _discrete_tts_source_cells

//! the exact tile's five stored member stamps built
def private stored_stamps_built(t : T?) : bool
    => stamps_built(t, tts_stored_gemm_ready(CutRail()),
        "the f32_gemm stored member stamps")

//! the four column-statistics stamps built
def private colstats_ready : bool => tts_colstats_ready(CutRail())

//! the column statistics and both fused AdaIN stamps built
def private adain_ready : bool => tts_adain_ready(CutRail())

//! the harmonic source's seven stamps built
def private src_ready : bool => tts_src_ready(CutRail())

//! the row GEMV's five stamps built
def private pk_gemv_ready : bool => tts_pk_gemv_ready(CutRail())

//! the frame loop's four fused stamps built
def private pk_fused_ready : bool => tts_pk_fused_ready(CutRail())

[test(description = "The CUDA stamps of the f32 tile GEMM, plain and biased, at 37 x 100 x 96 off every tile multiple match the k-ordered sum at the approx bar, store every output element, and never read the poisoned slab pad past n")]
def test_cud_f32_gemm(t0 : T?) {
    t0 |> run("the f32 tile GEMM (CUDA), plain and biased, == the k-ordered fma sum at off-tile shapes") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_f32_gemm_cls() && ensure_f32_gemm_b_cls(), "f32_gemm_cls, f32_gemm_b_cls"))
        with_cu_planes() $(var cp) {
            f32_gemm_case(t, cp)
        }
    }
}

[test(description = "Each stored member of the f32 tile GEMM's CUDA stamps - Q8_0, Q4_K, Q5_K, Q6_K and half floats, in the discrete homes' device layout - matches the k-ordered sum over the rows the CPU reader widens the same planes to: biased, with no bias row named, and under x rows narrower than a weight row")]
def test_cud_f32_gemm_stored(t0 : T?) {
    t0 |> run("the f32 tile's stored members (CUDA) == the k-ordered sum over the CPU reader's rows") <| @(t : T?) {
        return if (!cuda_up(t) || !stored_stamps_built(t))
        with_cu_planes() $(var cp) {
            for (member in fixed_array("q8", "k4", "k5", "k6", "f16")) {
                f32_stored_case(t, cp, member)
            }
        }
    }
}

[test(description = "Each stored member of the f32 tile GEMM's CUDA stamps over a transposed conv's lift reads row tap * cout + co, column ci of the lift as the tile's row co, column tap * cin + ci, matching the k-ordered sum over the CPU reader's lift rows")]
def test_cud_f32_gemm_stored_lift(t0 : T?) {
    t0 |> run("the f32 tile's stored members (CUDA) over a transposed conv's lift == the k-ordered sum over the reader's lift rows") <| @(t : T?) {
        return if (!cuda_up(t) || !stored_stamps_built(t))
        with_cu_planes() $(var cp) {
            for (member in fixed_array("q8", "k4", "k5", "k6", "f16")) {
                f32_stored_lift_case(t, cp, member)
            }
        }
    }
}

[test(description = "The CUDA stamp of the embedding gather at element bases off zero equals the CPU word + position + type sum bit for bit, with neither add, each add alone and both, follows its ids, and leaves the prefix before out_off at its sentinel")]
def test_cud_row_gather(t0 : T?) {
    t0 |> run("the embedding gather (CUDA) == the CPU word + position + type sum, exact, at element bases off zero") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_row_gather_cls(), "tts_row_gather_cls"))
        with_cu_planes() $(var cp) {
            tts_row_gather_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of the tanh GELU over a 5203-element run at an offset matches the CPU `gelu` within 1e-5 relative, inputs at +-30 past the tanh clamp included, leaves the outside untouched, and the bar tells it from the f16 LUT form")]
def test_cud_gelu_tanh(t0 : T?) {
    t0 |> run("the tanh GELU (CUDA) == the CPU gelu at the f32 bar, the run's tail and the tanh clamp included") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_gelu_tanh_cls(), "tts_gelu_tanh_cls"))
        with_cu_planes() $(var cp) {
            tts_gelu_tanh_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of the leaky ReLU over a 5203-element run at an offset equals the CPU `leaky_relu` bit for bit at slope 0.2, the three-element tail included, with the elements outside the run untouched")]
def test_cud_leaky(t0 : T?) {
    t0 |> run("the leaky ReLU (CUDA) == the CPU leaky_relu, exact, the run's tail included") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_leaky_cls(), "tts_leaky_cls"))
        with_cu_planes() $(var cp) {
            tts_leaky_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of the TTS bidirectional attention matches the CPU `attention_rows` within 1e-5 relative at 37x4x64, 130x2x128 and 512x2x64 (two keys an invocation), and zeroing the last key moves the first query row, so every key is read")]
def test_cud_attn(t0 : T?) {
    t0 |> run("the bidirectional attention (CUDA) == the CPU attention_rows at hs 64 and 128, one and two keys an invocation") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_attn_cls(), "tts_attn_cls"))
        with_cu_planes() $(var cp) {
            tts_attn_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of the concat rows equals the CPU concat bit for bit with one broadcast style row, and with the f0 and noise columns read at their row strides plus a zero pad to width 48")]
def test_cud_concat(t0 : T?) {
    t0 |> run("the concat rows (CUDA) == the CPU concat, exact: one style row broadcast, then f0 and n at strides and a zero pad") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_concat_cls(), "tts_concat_cls"))
        with_cu_planes() $(var cp) {
            tts_concat_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of the duration sigmoid sums over the first 50 of 64 logit columns at speed 1.3 matches the CPU raw durations within 1e-5 relative, with the poisoned columns past nb kept out of the sum")]
def test_cud_sigsum(t0 : T?) {
    t0 |> run("the sigmoid sums (CUDA) == the CPU raw durations over 50 of 64 columns at speed 1.3") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_sigsum_cls(), "tts_sigsum_cls"))
        with_cu_planes() $(var cp) {
            tts_sigsum_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamps of the four-pass column statistics match the double-precision channel means and squared deviations at the approx bar at 45x72, 300x20 (a short last block) and 1000x300 (lanes taking two channels)")]
def test_cud_colstats(t0 : T?) {
    t0 |> run("the column statistics (CUDA) == the double-precision mean and squared deviations, one and two strides a lane") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, colstats_ready(), "the column-statistics stamps"))
        with_cu_planes() $(var cp) {
            tts_colstats_case(t, cp)
        }
    }
}

[test(description = "The fused AdaIN CUDA stamps over the CUDA column stats match the CPU `adain_rows_into` plus leaky ReLU or Snake within 1e-4 relative, and the in-place leaky stamp equals the out-of-place one bit for bit with the slack untouched")]
def test_cud_adain(t0 : T?) {
    t0 |> run("the column stats + fused AdaIN (CUDA), leaky and Snake stamps, == the CPU adain_rows_into, out of place and in place") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, adain_ready(), "the column-statistics and fused AdaIN stamps"))
        with_cu_planes() $(var cp) {
            tts_adain_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of the residual add-and-scale equals (a + b) / sqrt(2) in f32 bit for bit over 1030 elements, out of place and in place, with the elements past the run left as they were")]
def test_cud_add_scale(t0 : T?) {
    t0 |> run("the add-and-scale (CUDA) == (a + b) * 1/sqrt(2) in f32, exact, out of place and in place") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_add_scale_cls(), "tts_add_scale_cls"))
        with_cu_planes() $(var cp) {
            tts_add_scale_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamps of the f32 conv im2col fed through F32GemmB match the CPU `conv1d` at the approx bar for a forward k5 conv, a transposed k4 s2 conv and a forward conv off row-major x, at element offsets, with poisoned slab pad columns kept out")]
def test_cud_tts_im2col(t0 : T?) {
    t0 |> run("forward k5, transposed k4 s2 and forward k5 off row-major x (CUDA): im2col + F32GemmB == the CPU conv1d") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_im2col_cls() && ensure_f32_gemm_b_cls(), "tts_im2col_cls, f32_gemm_b_cls"))
        with_cu_planes() $(var cp) {
            tts_im2col_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of the LSTM direction at hidden 64 and 200 over 23 steps reproduces the CPU `bilstm` within 1e-4 relative for both directions into one row set and for the forward weights walked backward, and flipping `backward` moves the output")]
def test_cud_tts_lstm_dir(t0 : T?) {
    for (hd in fixed_array(64, 200)) {
        t0 |> run("one LSTM direction (CUDA) at hidden {hd}, both walks == the CPU bilstm") <| @(t : T?) {
            return if (!cuda_up(t) || !stamps_built(t, ensure_tts_lstm_dir_cls(), "tts_lstm_dir_cls"))
            with_cu_planes() $(var cp) {
                tts_lstm_case(t, cp, hd)
            }
        }
    }
}

[test(description = "Every CUDA TTS stamp leaves the shared-memory carveout to the driver, the LSTM direction's 5 KB of workgroup memory included, so the recurrence re-reads its matrix through a full L1; an LLM window class keeps the full carveout")]
def test_cud_tts_carveout(t : T?) {
    t |> success(cud_l1_scoped("dasllama_cuda_classes_tts", 0x1400ul),
        "the LSTM direction's carveout is the driver's")
    t |> success(cud_l1_scoped("dasllama_cuda_classes_tts", 0ul),
        "a TTS row stamp's carveout is the driver's")
    t |> success(!cud_l1_scoped("dasllama_cuda_classes", 0x4200ul),
        "the f32 tile's class module keeps the full carveout")
}

//! the launch mode a TTS shot's body reads under home mode `home`, and the home's mode right after the shot
def private shot_pdl(home : bool) : tuple<inside : bool; after : bool> {
    let was = cuda_launch_pdl()
    cuda_set_launch_pdl(home)
    var inside = !home
    var rl : CutRail
    rail_tts_shot(rl, "probe", 1l, 1l) $ {
        inside = cuda_launch_pdl()
    }
    let after = cuda_launch_pdl()
    cuda_set_launch_pdl(was)
    return (inside = inside, after = after)
}

[test(description = "A TTS seat's shot on the CUDA rail launches its chain in plain stream order: programmatic dependent launch reads off inside the shot and is put back after it")]
def test_cud_tts_shot_plain_order(t : T?) {
    let shot = shot_pdl(true)
    t |> success(!shot.inside, "the shot's launches run without programmatic dependent launch")
    t |> success(shot.after, "the home's launch mode is put back after the shot")
}

//! ... the body's mode with the shot seam at `seam`
def private shot_pdl_inside(home, seam : bool) : bool {
    let seam_was = set_cuda_tts_shot_pdl(seam)
    let inside = shot_pdl(home).inside
    set_cuda_tts_shot_pdl(seam_was)
    return inside
}

[test(description = "The race seam `set_cuda_tts_shot_pdl(true)` lets a TTS seat's shot launch under programmatic dependent launch where the home has it on and leaves it off where the home has it off, and false - the default - keeps the shot in plain stream order")]
def test_cud_tts_shot_pdl_seam(t : T?) {
    t |> success(!cuda_tts_shot_pdl_on(), "the seam's default is plain stream order")
    t |> success(shot_pdl_inside(true, true), "seam on, home on: the shot launches under PDL")
    t |> success(!shot_pdl_inside(false, true), "seam on, home off: the shot stays in plain order")
    t |> success(!shot_pdl_inside(true, false), "seam off, home on: the shot stays in plain order")
    t |> success(!cuda_tts_shot_pdl_on(), "the seam is put back after the cells")
}

[test(description = "The CUDA stamp of the fold over the lift rows of a transposed k4 s2 conv - x times the lift matrix - plus the bias matches the CPU `conv1d` at the approx bar, at element bases off zero, the prefix before the output rows kept")]
def test_cud_tts_fold(t0 : T?) {
    t0 |> run("tts_fold (CUDA) == the CPU conv1d over a transposed conv's lift rows") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_fold_cls(), "tts_fold_cls"))
        with_cu_planes() $(var cp) {
            tts_fold_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of the LSTM direction over the recurrence's stored Q8_0 plane at hidden 64 over 9 steps reproduces the CPU loop over the rows the reader widens that plane to, forward and backward")]
def test_cud_tts_lstm_dir_q8(t0 : T?) {
    t0 |> run("tts_lstm_dir_q8 (CUDA) == the CPU loop over the reader's rows, both directions") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_lstm_dir_q8_cls(), "tts_lstm_dir_q8_cls"))
        with_cu_planes() $(var cp) {
            lstm_q8_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of the depthwise transposed pool k3 s2 p1 with output padding over 22 channels and 19 steps produces 38 rows matching `conv1d_rows_transposed_depthwise` within 1e-5 relative, reading weights and bias between poisoned slots")]
def test_cud_tts_pool_dw(t0 : T?) {
    t0 |> run("the depthwise transposed pool (CUDA) k3 s2 p1 over 22 channels == the CPU conv1d_rows_transposed_depthwise") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_pool_dw_cls(), "tts_pool_dw_cls"))
        with_cu_planes() $(var cp) {
            tts_pool_dw_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamps of the Pocket row copy write a 7 x 13 strided window bit for bit and the ELU twin matches `elu_rows` within 1e-6 relative, moving only the negative elements, with every element outside the window left at its sentinel")]
def test_cud_pk_rows(t0 : T?) {
    t0 |> run("the row copies (CUDA), plain and ELU, == the CPU window copy and elu_rows at strides and offsets off zero") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_pk_rows_cls() && ensure_tts_pk_rows_elu_cls(), "tts_pk_rows_cls, tts_pk_rows_elu_cls"))
        with_cu_planes() $(var cp) {
            tts_pk_rows_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of the in-place layer scale over 37 x 72 rows with the scale row at an offset equals the CPU `layer_scale_rows` bit for bit, leaving the slack past the rows untouched")]
def test_cud_pk_row_scale(t0 : T?) {
    t0 |> run("the layer scale (CUDA) == the CPU layer_scale_rows, exact, at t 37 x 72 with the scale row at an offset") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_pk_row_scale_cls(), "tts_pk_row_scale_cls"))
        with_cu_planes() $(var cp) {
            tts_pk_row_scale_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of the causal cached attention row matches `attention_causal_rows` within 1e-5 relative on a 45-query prompt, a decode step over an 8-key window, the prompt over 16 keys and a decode step over 300 keys, past one 256-key stride of the scores a row stages, where the row over the first stride's keys alone lands outside the bar; a poisoned key before the window changes no bit")]
def test_cud_pk_attn(t0 : T?) {
    t0 |> run("the causal attention row (CUDA) == the CPU attention_causal_rows: a 45-query prompt, a decode step over an 8-key window, the prompt over 16, a decode step over 300 keys") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_pk_attn_cls(), "tts_pk_attn_cls"))
        with_cu_planes() $(var cp) {
            tts_pk_attn_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of the attention row with four query heads over two key-value heads matches `attention_causal_rows` within 1e-5 relative over every key and over an 8-key window, and the window measurably moves the late rows")]
def test_cud_pk_attn_grouped(t0 : T?) {
    t0 |> run("the attention row over grouped keys (CUDA) == the CPU attention_causal_rows: four query heads a pair of key-value heads, every key, then an 8-key window") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_pk_attn_cls(), "tts_pk_attn_cls"))
        with_cu_planes() $(var cp) {
            tts_pk_attn_grouped_case(t, cp)
        }
    }
}

[test(description = "The row GEMV's five CUDA stamps (dot, ln+modulate+silu, gated residual, add+silu, tail with latent) at nin 200, nout 45 match the CPU `linear_vec` chains within 2e-7 x nin relative with weight rows on and off the float4 lattice")]
def test_cud_pk_gemv(t0 : T?) {
    t0 |> run("the row GEMV's five stamps (CUDA) == the CPU linear_vec chains at nin 200 / nout 45, the slab's runs at offsets, the rows on and off the four lattice") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, pk_gemv_ready(), "the row GEMV stamps"))
        with_cu_planes() $(var cp) {
            tts_pk_gemv_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of the table rope over the k span of 23 fused qkv rows from position 37 matches the CPU `rope_rows` within 1e-6 relative, with the q and v spans and the slack left exactly as they were")]
def test_cud_pk_rope(t0 : T?) {
    t0 |> run("the rows rope (CUDA) == the CPU rope_rows on the k span of t 23 rows from position 37, the q and v spans untouched") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_pk_rope_cls(), "tts_pk_rope_cls"))
        with_cu_planes() $(var cp) {
            tts_pk_rope_case(t, cp)
        }
    }
}

[test(description = "The frame loop's fused CUDA stamps match the CPU chains at the approx bar: the normed qkv row roped into y and written to the caches' one row (wide and grouped forms), the residual adds with and without scale, and the normed GELU")]
def test_cud_pk_gemv_fused(t0 : T?) {
    t0 |> run("the frame loop's fused stamps (CUDA) == the CPU chains: the normed qkv row roped into y and the caches, the residual adds, the normed GELU") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, pk_fused_ready(), "the frame loop fused stamps"))
        with_cu_planes() $(var cp) {
            tts_pk_gemv_fused_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamp of the stage-sum axpy equals o + 0.25 y in f32 bit for bit over 1030 elements when accumulating, and 0.25 y over a NaN-filled o when overwriting, with the elements outside the run untouched")]
def test_cud_axpy(t0 : T?) {
    t0 |> run("the axpy (CUDA) == o + 0.25 y in f32, exact, accumulating (zero = 0) and overwriting (zero = 1)") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_axpy_cls(), "tts_axpy_cls"))
        with_cu_planes() $(var cp) {
            tts_axpy_case(t, cp)
        }
    }
}

[test(description = "The reflect pad built from two launches of the CUDA row copy stamp equals `pad_reflect_left_rows_inplace` bit for bit: x [37][24] shifted down one row behind its own row 1, nothing written past the 38 rows")]
def test_cud_reflect1(t0 : T?) {
    t0 |> run("the reflect pad (CUDA) as the driver's two row copies: y [38][24] == x [37][24] behind x's row 1, exact") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tts_pk_rows_cls(), "tts_pk_rows_cls"))
        with_cu_planes() $(var cp) {
            tts_reflect1_case(t, cp)
        }
    }
}

[test(description = "The CUDA stamps of the harmonic source on the torch and onnx laws hold phase frames to one ulp and the mixed signal to 1e-5 of the CPU `sine_source` (the home's fast-math sin and tanh), a poisoned f0 frame reds both; at up 300 the 40000-frame cumsum and the seeded own noise draw match too")]
def test_cud_source(t0 : T?) {
    for (torch in fixed_array(true, false)) {
        t0 |> run("the harmonic source (CUDA), {torch ? "torch" : "onnx"} law, up 300 x 700 frames: phase frames, mixed signal, long cumsum, own draw") <| @(t : T?) {
            return if (!cuda_up(t) || !stamps_built(t, src_ready(), "the harmonic source stamps"))
            with_cu_planes() $(var cp) {
                tts_source_case(t, cp, torch, 300, 700, true)
            }
        }
        t0 |> run("the harmonic source (CUDA), {torch ? "torch" : "onnx"} law, up 2 x 37 frames: the first taps read sample 0, the initial phases in") <| @(t : T?) {
            return if (!cuda_up(t) || !stamps_built(t, src_ready(), "the harmonic source stamps"))
            with_cu_planes() $(var cp) {
                tts_source_case(t, cp, torch, 2, 37, false)
            }
        }
    }
}

[test(description = "The CUDA stamps of the STFT on reflect and edge pads produce 10 frames x 4 bins of magnitude and phase within 1e-4 of the double form, the zero-imaginary bin reading +pi or 0, and a poisoned sample reds the compare")]
def test_cud_stft(t0 : T?) {
    for (reflect in fixed_array(true, false)) {
        t0 |> run("the STFT (CUDA), {reflect ? "reflect" : "edge"} pad, 10 frames x 4 bins == the double form") <| @(t : T?) {
            return if (!cuda_up(t) || !stamps_built(t, ensure_tts_stft_reflect_cls() && ensure_tts_stft_edge_cls(), "tts_stft_reflect_cls, tts_stft_edge_cls"))
            with_cu_planes() $(var cp) {
                tts_stft_case(t, cp, reflect)
            }
        }
    }
}

[test(description = "The CUDA stamp of the inverse STFT with and without the window envelope turns 9 frames x 3 bins into 36 samples within 1e-4 of each sample's envelope (the sum of its terms' magnitudes) of the double overlap-add, some samples under a zero envelope, and a poisoned magnitude reds the compare")]
def test_cud_istft(t0 : T?) {
    for (envelope in fixed_array(true, false)) {
        t0 |> run("the inverse STFT (CUDA), envelope {envelope ? "on" : "off"}, 9 frames x 3 bins into 36 samples == the double overlap-add") <| @(t : T?) {
            return if (!cuda_up(t) || !stamps_built(t, ensure_tts_istft_cls(), "tts_istft_cls"))
            with_cu_planes() $(var cp) {
                tts_istft_case(t, cp, envelope)
            }
        }
    }
}

[test(description = "The CUDA stamp: The row-broadcast bias adds the bias row at an element base to 434 rows of 12 in place - alone it equals x + b bit for bit, under relu max(x + b, 0) - leaves the elements past the rows untouched, lands off the input, and the two arms differ")]
def test_cud_bias_act(t0 : T?) {
    t0 |> run("the bias row and the relu (CUDA) == x + b and max(x + b, 0), exact") <| @(t : T?) {
        return if (!cuda_up(t) || !stamps_built(t, ensure_tower_bias_act_cls(), "tower_bias_act_cls"))
        with_cu_planes() $(var cp) {
            tower_bias_act_case(t, cp)
        }
    }
}

//! the stored row GEMV's stamps built: every kind of every member
def private pkst_stamps_built : bool => tts_pkst_ready(CutRail())

[test(description = "The CUDA home's five stored row-GEMV stamps over weight rows held as Q8_0, Q4_K, Q5_K, Q6_K and half floats in the device layout match the CPU `linear_vec` chains over the rows the reader widens the same planes to, within 2e-7 x nin relative, leave every slot outside a row at its sentinel, and land off the chain with its weight rows moved down one")]
def test_cud_pk_gemv_stored(t0 : T?) {
    for (member in fixed_array("q8", "k4", "k5", "k6", "f16")) {
        t0 |> run("the stored row GEMV (CUDA), {member} rows == the CPU chains over the reader's rows") <| @(t : T?) {
            return if (!cuda_up(t) || !stamps_built(t, pkst_stamps_built(), "the stored row GEMV stamps"))
            with_cu_planes() $(var cp) {
                tts_pk_gemv_stored_case(t, cp, member)
            }
        }
    }
}

[test(description = "The CUDA home's four fused stored frame-loop stamps over weight rows held as Q8_0, Q4_K, Q5_K, Q6_K and half floats in the device layout - the normed qkv row roped into y and written to the caches' one row, the residual adds with and without the scale, the normed GELU - match the CPU chains over the rows the reader widens the same planes to, within the approx bar, leave the caches' other rows untouched, and the scaled add reds against the chain with its weight rows moved down one")]
def test_cud_pk_gemv_stored_fused(t0 : T?) {
    for (member in fixed_array("q8", "k4", "k5", "k6", "f16")) {
        t0 |> run("the frame loop's fused stored stamps (CUDA), {member} rows == the CPU chains over the reader's rows") <| @(t : T?) {
            return if (!cuda_up(t) || !stamps_built(t, pkst_stamps_built(), "the frame loop's fused stored stamps"))
            with_cu_planes() $(var cp) {
                tts_pk_gemv_stored_fused_case(t, cp, member)
            }
        }
    }
}
