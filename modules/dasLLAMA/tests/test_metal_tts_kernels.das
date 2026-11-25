options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options indenting = 4
options _dasllama_internal = true

// The TTS chains' Metal rail stamps (`dasllama_metal_classes_tts`) on the case bodies the discrete homes' cells run
// (`_discrete_tts_cells.das`, `_discrete_tts_conv_cells.das`, `_discrete_tts_pocket_cells.das`,
// `_discrete_tts_source_cells.das`), one cell a stamp family. Model-free; every cell skips without a Metal device, and a
// stamp that does not compile on the device reds its cell. `test_mtl_tts_widened_counts` serves a hand-built frame model
// (`_tts_widened_cells.das`) through the home's registered seats, and skips too where the tower's pipelines do not stand up.

require dastest/testing_boost public
require dasllama/dasllama_lint public
require ?das_metal dasllama/dasllama_metal_classes_tts
require ?das_metal _metal_cell
require ?das_metal _discrete_tts_cells
require ?das_metal _discrete_tts_conv_cells
require ?das_metal _discrete_tts_pocket_cells
require ?das_metal _discrete_tts_source_cells
require ?das_metal _discrete_cells   // seats; the tower rows' cases
require ?das_metal _compares
require ?das_metal _kernel_oracles   // cell_role
require ?das_metal dasllama/dasllama_gpu_rail   // TtsMmSite, TtsConvSite, RailBuf
require ?das_metal dasllama/dasllama_kqformat
require ?das_metal dasllama/dasllama_metal_sites   // rail_tts_mm, rail_tts_conv: the home's own dense and conv tiles
require ?das_metal dasllama/dasllama_metal_tower   // metal_tower_init: the tile's pipelines
require ?das_metal dasllama/dasllama_common   // bump_weights_epoch: a model's discharge
require ?das_metal dasllama/dasllama_metal_tts   // nolint:STYLE030,LINT019 - the home's seats and its discharge hook register here
require ?das_metal _tts_widened_cells

def private src_ready : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return tts_src_ready(MtRail())
    } else {
        return false
    }
}

def private colstats_ready : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return tts_colstats_ready(MtRail())
    } else {
        return false
    }
}

def private adain_ready : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return tts_adain_ready(MtRail())
    } else {
        return false
    }
}

def private pk_gemv_ready : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return tts_pk_gemv_ready(MtRail())
    } else {
        return false
    }
}

def private pk_fused_ready : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return tts_pk_fused_ready(MtRail())
    } else {
        return false
    }
}

def private stored_stamps_built(t : T?) : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        return stamps_built(t, tts_stored_gemm_ready(MtRail()), "the f32_gemm stored member stamps")
    } else {
        return false
    }
}

//! the stored row GEMV's stamps built: every kind of every member, and this home's own-layout ones
def private pkst_stamps_built : bool {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var rl : MtRail
        return tts_pkst_ready(rl)
    } else {
        return false
    }
}

[test(description = "The Metal rail stamp of the tanh GELU over a 5203-element run at an offset matches the CPU `gelu` within 1e-5 relative, inputs at +-30 past the tanh clamp included, leaves the outside untouched, and the bar tells it from the f16 LUT form")]
def test_mtl_gelu_tanh(t0 : T?) {
    t0 |> run("the tanh GELU (Metal rail) == the CPU gelu at the f32 bar, the run's tail and the tanh clamp included") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_gelu_tanh_cls(), "tts_gelu_tanh_cls"))
            with_mt_planes() $(var cp) {
                tts_gelu_tanh_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the leaky ReLU over a 5203-element run at an offset equals the CPU `leaky_relu` bit for bit at slope 0.2, the three-element tail included, with the elements outside the run untouched")]
def test_mtl_leaky(t0 : T?) {
    t0 |> run("the leaky ReLU (Metal rail) == the CPU leaky_relu, exact, the run's tail included") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_leaky_cls(), "tts_leaky_cls"))
            with_mt_planes() $(var cp) {
                tts_leaky_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the concat rows equals the CPU concat bit for bit with one broadcast style row, and with the f0 and noise columns read at their row strides plus a zero pad to width 48")]
def test_mtl_concat(t0 : T?) {
    t0 |> run("the concat rows (Metal rail) == the CPU concat, exact: one style row broadcast, then f0 and n at strides and a zero pad") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_concat_cls(), "tts_concat_cls"))
            with_mt_planes() $(var cp) {
                tts_concat_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the duration sigmoid sums over the first 50 of 64 logit columns at speed 1.3 matches the CPU raw durations within 1e-5 relative, with the poisoned columns past nb kept out of the sum")]
def test_mtl_sigsum(t0 : T?) {
    t0 |> run("the sigmoid sums (Metal rail) == the CPU raw durations over 50 of 64 columns at speed 1.3") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_sigsum_cls(), "tts_sigsum_cls"))
            with_mt_planes() $(var cp) {
                tts_sigsum_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the residual add-and-scale equals (a + b) / sqrt(2) in f32 bit for bit over 1030 elements, out of place and in place, with the elements past the run left as they were")]
def test_mtl_add_scale(t0 : T?) {
    t0 |> run("the add-and-scale (Metal rail) == (a + b) * 1/sqrt(2) in f32, exact, out of place and in place") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_add_scale_cls(), "tts_add_scale_cls"))
            with_mt_planes() $(var cp) {
                tts_add_scale_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the depthwise transposed pool k3 s2 p1 with output padding over 22 channels and 19 steps produces 38 rows matching `conv1d_rows_transposed_depthwise` within 1e-5 relative, reading weights and bias between poisoned slots")]
def test_mtl_tts_pool_dw(t0 : T?) {
    t0 |> run("the depthwise transposed pool (Metal rail) k3 s2 p1 over 22 channels == the CPU conv1d_rows_transposed_depthwise") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_pool_dw_cls(), "tts_pool_dw_cls"))
            with_mt_planes() $(var cp) {
                tts_pool_dw_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamps of the Pocket row copy write a 7 x 13 strided window bit for bit and the ELU twin matches `elu_rows` within 1e-6 relative, moving only the negative elements, with every element outside the window left at its sentinel")]
def test_mtl_pk_rows(t0 : T?) {
    t0 |> run("the row copies (Metal rail), plain and ELU, == the CPU window copy and elu_rows at strides and offsets off zero") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_pk_rows_cls() && ensure_tts_pk_rows_elu_cls(), "tts_pk_rows_cls, tts_pk_rows_elu_cls"))
            with_mt_planes() $(var cp) {
                tts_pk_rows_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the in-place layer scale over 37 x 72 rows with the scale row at an offset equals the CPU `layer_scale_rows` bit for bit, leaving the slack past the rows untouched")]
def test_mtl_pk_row_scale(t0 : T?) {
    t0 |> run("the layer scale (Metal rail) == the CPU layer_scale_rows, exact, at t 37 x 72 with the scale row at an offset") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_pk_row_scale_cls(), "tts_pk_row_scale_cls"))
            with_mt_planes() $(var cp) {
                tts_pk_row_scale_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the causal cached attention row matches `attention_causal_rows` within 1e-5 relative on a 45-query prompt, a decode step over an 8-key window, the prompt over 16 keys and a decode step over 300 keys, past one 256-key stride of the scores a row stages, where the row over the first stride's keys alone lands outside the bar; a poisoned key before the window changes no bit")]
def test_mtl_pk_attn(t0 : T?) {
    t0 |> run("the causal attention row (Metal rail) == the CPU attention_causal_rows: a 45-query prompt, a decode step over an 8-key window, the prompt over 16, a decode step over 300 keys") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_pk_attn_cls(), "tts_pk_attn_cls"))
            with_mt_planes() $(var cp) {
                tts_pk_attn_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the attention row with four query heads over two key-value heads matches `attention_causal_rows` within 1e-5 relative over every key and over an 8-key window, and the window measurably moves the late rows")]
def test_mtl_pk_attn_grouped(t0 : T?) {
    t0 |> run("the attention row over grouped keys (Metal rail) == the CPU attention_causal_rows: four query heads a pair of key-value heads, every key, then an 8-key window") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_pk_attn_cls(), "tts_pk_attn_cls"))
            with_mt_planes() $(var cp) {
                tts_pk_attn_grouped_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the table rope over the k span of 23 fused qkv rows from position 37 matches the CPU `rope_rows` within 1e-6 relative, with the q and v spans and the slack left exactly as they were")]
def test_mtl_pk_rope(t0 : T?) {
    t0 |> run("the rows rope (Metal rail) == the CPU rope_rows on the k span of t 23 rows from position 37, the q and v spans untouched") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_pk_rope_cls(), "tts_pk_rope_cls"))
            with_mt_planes() $(var cp) {
                tts_pk_rope_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the stage-sum axpy equals o + 0.25 y in f32 bit for bit over 1030 elements when accumulating, and 0.25 y over a NaN-filled o when overwriting, with the elements outside the run untouched")]
def test_mtl_axpy(t0 : T?) {
    t0 |> run("the axpy (Metal rail) == o + 0.25 y in f32, exact, accumulating (zero = 0) and overwriting (zero = 1)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_axpy_cls(), "tts_axpy_cls"))
            with_mt_planes() $(var cp) {
                tts_axpy_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The reflect pad built from two launches of the Metal rail row copy stamp equals `pad_reflect_left_rows_inplace` bit for bit: x [37][24] shifted down one row behind its own row 1, nothing written past the 38 rows")]
def test_mtl_reflect1(t0 : T?) {
    t0 |> run("the reflect pad (Metal rail) as the driver's two row copies: y [38][24] == x [37][24] behind x's row 1, exact") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_pk_rows_cls(), "tts_pk_rows_cls"))
            with_mt_planes() $(var cp) {
                tts_reflect1_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamps of the harmonic source on the torch and onnx laws hold phase frames to one ulp and the mixed signal to 2e-6 of the CPU `sine_source` (fast math off on this home), a poisoned f0 frame reds both; at up 300 the 40000-frame cumsum and the seeded own noise draw match too, the draw within 1e-5")]
def test_mtl_source(t0 : T?) {
    for (torch in fixed_array(true, false)) {
        t0 |> run("the harmonic source (Metal rail), {torch ? "torch" : "onnx"} law, up 300 x 700 frames: phase frames, mixed signal, long cumsum, own draw") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(das_metal)) {
                return if (!metal_up(t) || !stamps_built(t, src_ready(), "the harmonic source stamps"))
                with_mt_planes() $(var cp) {
                    tts_source_case(t, cp, torch, 300, 700, true)
                }
            } else {
                t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
            }
        }
        t0 |> run("the harmonic source (Metal rail), {torch ? "torch" : "onnx"} law, up 2 x 37 frames: the first taps read sample 0, the initial phases in") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(das_metal)) {
                return if (!metal_up(t) || !stamps_built(t, src_ready(), "the harmonic source stamps"))
                with_mt_planes() $(var cp) {
                    tts_source_case(t, cp, torch, 2, 37, false)
                }
            } else {
                t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
            }
        }
    }
}

[test(description = "The Metal rail stamps of the STFT on reflect and edge pads produce 10 frames x 4 bins of magnitude and phase within 1e-4 of the double form, the zero-imaginary bin reading +pi or 0, and a poisoned sample reds the compare")]
def test_mtl_stft(t0 : T?) {
    for (reflect in fixed_array(true, false)) {
        t0 |> run("the STFT (Metal rail), {reflect ? "reflect" : "edge"} pad, 10 frames x 4 bins == the double form") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(das_metal)) {
                return if (!metal_up(t) || !stamps_built(t, ensure_tts_stft_reflect_cls() && ensure_tts_stft_edge_cls(), "tts_stft_reflect_cls, tts_stft_edge_cls"))
                with_mt_planes() $(var cp) {
                    tts_stft_case(t, cp, reflect)
                }
            } else {
                t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
            }
        }
    }
}

[test(description = "The Metal rail stamp of the inverse STFT with and without the window envelope turns 9 frames x 3 bins into 36 samples within 1e-4 of each sample's envelope (the sum of its terms' magnitudes) of the double overlap-add, some samples under a zero envelope, and a poisoned magnitude reds the compare")]
def test_mtl_istft(t0 : T?) {
    for (envelope in fixed_array(true, false)) {
        t0 |> run("the inverse STFT (Metal rail), envelope {envelope ? "on" : "off"}, 9 frames x 3 bins into 36 samples == the double overlap-add") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(das_metal)) {
                return if (!metal_up(t) || !stamps_built(t, ensure_tts_istft_cls(), "tts_istft_cls"))
                with_mt_planes() $(var cp) {
                    tts_istft_case(t, cp, envelope)
                }
            } else {
                t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
            }
        }
    }
}

[test(description = "The Metal rail stamp of the embedding gather at element bases off zero equals the CPU word + position + type sum bit for bit, with neither add, each add alone and both, follows its ids, and leaves the prefix before out_off at its sentinel")]
def test_mtl_row_gather(t0 : T?) {
    t0 |> run("the embedding gather (Metal rail) == the CPU word + position + type sum, exact, at element bases off zero") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_row_gather_cls(), "tts_row_gather_cls"))
            with_mt_planes() $(var cp) {
                tts_row_gather_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the TTS bidirectional attention matches the CPU `attention_rows` within 1e-5 relative at 37x4x64, 130x2x128 and 512x2x64 (two keys an invocation), and zeroing the last key moves the first query row, so every key is read")]
def test_mtl_attn(t0 : T?) {
    t0 |> run("the bidirectional attention (Metal rail) == the CPU attention_rows at hs 64 and 128, one and two keys an invocation") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_attn_cls(), "tts_attn_cls"))
            with_mt_planes() $(var cp) {
                tts_attn_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamps of the four-pass column statistics match the double-precision channel means and squared deviations at the approx bar at 45x72, 300x20 (a short last block) and 1000x300 (lanes taking two channels)")]
def test_mtl_colstats(t0 : T?) {
    t0 |> run("the column statistics (Metal rail) == the double-precision mean and squared deviations, one and two strides a lane") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, colstats_ready(), "the column-statistics stamps"))
            with_mt_planes() $(var cp) {
                tts_colstats_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The fused AdaIN Metal rail stamps over the Metal rail column stats match the CPU `adain_rows_into` plus leaky ReLU or Snake within 1e-4 relative, and the in-place leaky stamp equals the out-of-place one bit for bit with the slack untouched")]
def test_mtl_adain(t0 : T?) {
    t0 |> run("the column stats + fused AdaIN (Metal rail), leaky and Snake stamps, == the CPU adain_rows_into, out of place and in place") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, adain_ready(), "the column-statistics and fused AdaIN stamps"))
            with_mt_planes() $(var cp) {
                tts_adain_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the fold over the lift rows of a transposed k4 s2 conv - x times the lift matrix - plus the bias matches the CPU `conv1d` at the approx bar, at element bases off zero, the prefix before the output rows kept")]
def test_mtl_tts_fold(t0 : T?) {
    t0 |> run("tts_fold (Metal rail) == the CPU conv1d over a transposed conv's lift rows") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_fold_cls(), "tts_fold_cls"))
            with_mt_planes() $(var cp) {
                tts_fold_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the LSTM direction at hidden 64 and 200 over 23 steps reproduces the CPU `bilstm` within 1e-4 relative for both directions into one row set and for the forward weights walked backward, and flipping `backward` moves the output")]
def test_mtl_tts_lstm_dir(t0 : T?) {
    for (hd in fixed_array(64, 200)) {
        t0 |> run("one LSTM direction (Metal rail) at hidden {hd}, both walks == the CPU bilstm") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(das_metal)) {
                return if (!metal_up(t) || !stamps_built(t, ensure_tts_lstm_dir_cls(), "tts_lstm_dir_cls"))
                with_mt_planes() $(var cp) {
                    tts_lstm_case(t, cp, hd)
                }
            } else {
                t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
            }
        }
    }
}

[test(description = "The row GEMV's five Metal rail stamps (dot, ln+modulate+silu, gated residual, add+silu, tail with latent) at nin 200, nout 45 match the CPU `linear_vec` chains within 2e-7 x nin relative with weight rows on and off the float4 lattice")]
def test_mtl_pk_gemv(t0 : T?) {
    t0 |> run("the row GEMV's five stamps (Metal rail) == the CPU linear_vec chains at nin 200 / nout 45, the slab's runs at offsets, the rows on and off the four lattice") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, pk_gemv_ready(), "the row GEMV stamps"))
            with_mt_planes() $(var cp) {
                tts_pk_gemv_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The frame loop's fused Metal rail stamps match the CPU chains at the approx bar: the normed qkv row roped into y and written to the caches' one row (wide and grouped forms), the residual adds with and without scale, and the normed GELU")]
def test_mtl_pk_gemv_fused(t0 : T?) {
    t0 |> run("the frame loop's fused stamps (Metal rail) == the CPU chains: the normed qkv row roped into y and the caches, the residual adds, the normed GELU") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, pk_fused_ready(), "the frame loop fused stamps"))
            with_mt_planes() $(var cp) {
                tts_pk_gemv_fused_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the row-broadcast bias adds the bias row at an element base to 434 rows of 12 in place - alone it equals x + b bit for bit, under relu max(x + b, 0) - leaves the elements past the rows untouched, lands off the input, and the two arms differ")]
def test_mtl_bias_act(t0 : T?) {
    t0 |> run("the bias row and the relu (Metal rail) == x + b and max(x + b, 0), exact") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tower_bias_act_cls(), "tower_bias_act_cls"))
            with_mt_planes() $(var cp) {
                tower_bias_act_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the half im2col writes a conv's k3 columns as f16 on both source layouts and strides, off and on the four-channel lattice, with x and the panel at element bases, bit for bit the reference columns, the halves before the panel and the lanes past it left at the sentinel")]
def test_mtl_im2col16(t0 : T?) {
    t0 |> run("the half im2col (Metal rail) == the reference columns as halves") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_im2col16_cls(), "tts_im2col16_cls"))
            with_mt_planes() $(var cp) {
                im2col16_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamps of the layernorm rows match `layernorm_batch` within the norm bar as f32 and as halves, the halves bit for bit the f32 rows rounded, the slack past the rows left at the sentinel, and the layernorm without its bias row lands off them")]
def test_mtl_ln16(t0 : T?) {
    t0 |> run("the layernorm rows (Metal rail), f32 and halves == layernorm_batch") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tower_ln_cls() && ensure_tower_ln16_cls(), "tower_ln_cls, tower_ln16_cls"))
            with_mt_planes() $(var cp) {
                ln16_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamps of the residual seam with the next layernorm match x + y + b and `layernorm_batch` within the norm bar as f32 and as halves, the halves bit for bit the f32 rows rounded, the half output untouched with the norm off, and the add without the bias lands off them")]
def test_mtl_post_add_ln16(t0 : T?) {
    t0 |> run("the seam and the next layernorm (Metal rail), f32 and halves == the CPU forms") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tower_post_add_ln_cls() && ensure_tower_post_add_ln16_cls(), "tower_post_add_ln_cls, tower_post_add_ln16_cls"))
            with_mt_planes() $(var cp) {
                post_add_ln16_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail's plane zero runs in the order of the rail's dispatches: a leaky map over 5300 elements and then a zero of 5203 of them from element 7, recorded on one command buffer, leave zeros in the run and the map's values on both sides of it")]
def test_mtl_rail_zero(t0 : T?) {
    t0 |> run("the plane zero (Metal rail) lands after the dispatch recorded ahead of it, the run alone") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_rail_zero_cls() && ensure_tts_leaky_cls(), "rail_zero_cls, tts_leaky_cls"))
            with_mt_planes() $(var cp) {
                let n = 5300
                let lo = 7
                let run = 5203
                var inscope x <- [for (i in range(n)); float(i % 37) - 18.5]
                let xp = cp |> io_(x)
                var pl = TtsElemArgs(nelem = uint(n), off = 0u, slope = 0.5)
                cp |> cell_launch(t, "rail_zero") $(var rl) {
                    let roles = seats(fixed_array(xp))
                    enc_tts_leaky_cls(rl, roles, pl, int64(n))
                    rail_zero(rl, roles[0], int64(lo) * 4l, int64(run) * 4l)
                }
                var inscope got <- cp |> plane_run(xp)
                var inscope mapped <- [for (v in x); v > 0.0 ? v : v * 0.5]
                var inscope want <- [for (i in range(n)); i >= lo && i < lo + run ? 0.0 : mapped[i]]
                check_rows(t, "rail_zero after the map: zeros in the run, the map's values outside it", got, want, BAR_EXACT, FLAT_ROWS)
                t |> success(mismatch_exact(got, mapped) > 0, "rail_zero control: the run is off the map's values (the zero ran after the map)")
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal home's own dense tile over a Q8_0 leaf whose rows are wider than the layer's inputs zeroes the columns past the inputs: after a call that leaves NaN in the tile's scratch rows, a 40-input call over 64-wide weight rows lands the sum over the 40 inputs, every output finite")]
def test_mtl_mm_own_pad(t0 : T?) {
    t0 |> run("the home's dense tile (Metal), x narrower than a weight row: the scratch columns past the inputs read zero") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, metal_tower_init() && rail_tts_sites_ensure(MtRail()), "the tower's pipelines and the sites' stamps"))
            with_mt_planes() $(var cp) {
                let rows = 5
                let d = 64
                let k = 64
                let nin = 40
                var inscope w <- row_fill(d * k, 17, 3, 0.04, 1.9)
                var inscope m <- stored_mat("q8", w, d, k, true)
                var inscope x <- row_fill(rows * nin, 41, 7, 0.05, 2.1)
                var inscope poison <- nan_plane(rows * k)
                let wp = cp |> in_plain(m.wq)
                let pp = cp |> in_(poison)
                let xp = cp |> in_(x)
                let y0p = cp |> out_(rows * d)
                let yp = cp |> out_(rows * d)
                var site = TtsMmSite(nin = int64(k), nout = int64(d), rows = int64(rows), cout_p = int64(d), kdim_p = int64(k), stored = true, native = true, fmt = KqFmt.q8)
                var served = true
                cp |> cell_launch(t, "st2_q8_exact_mm") $(var rl) {
                    let at = $(pl : int) => rail_buf(rl, cell_role(pl))   // read before the first dispatch re-seats the rail's first roles
                    let pl = fixed_array(at(wp), at(pp), at(xp), at(y0p), at(yp))
                    served = rail_tts_mm(rl, pl[1], pl[0], pl[0], site, pl[1], pl[3]) && served   // the whole-width call: NaN into every scratch column
                    site.nin = int64(nin)
                    served = rail_tts_mm(rl, pl[2], pl[0], pl[0], site, pl[2], pl[4]) && served
                }
                t |> success(served, "the home's tile takes the Q8_0 leaf in its own layout")
                var inscope want <- stored_mm_ref(x, x, rows, d, nin, nin, false) $(o, c) => m.rows[o * k + c]
                var inscope primed <- cp |> plane_run(y0p)
                t |> success(nan_count(primed) > 0, "the control: the whole-width call over NaN rows lands NaN - the scratch columns held it")
                var inscope got <- cp |> plane_run(yp)
                check_rows(t, "st2_q8_exact_mm over 40 of 64 columns vs the sum over the 40 inputs", got, want, BAR_APPROX, d)
                var inscope moved <- stored_mm_ref(x, x, rows, d, nin, nin, false) $(o, c) => m.rows[((o + 1) % d) * k + c]
                check_exceeds(t, "st2_q8_exact_mm control: the sum over the weight rows moved down one lands off the device rows", got, moved, BAR_APPROX)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal home's own conv tile follows the site's half flag over a Q4_K leaf in the home's layout: a k1 conv of 40 rows over 256 inputs off the half lattice lands the k-ordered sum within 1e-4 on the float tile, and under the half flag lands within the half tile's bar and outside the float tile's")]
def test_mtl_conv_tile_follows_half(t0 : T?) {
    t0 |> run("the home's conv tile (Metal), a Q4_K leaf: the float tile, and the f16-staged tile under the site's half flag") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, metal_tower_init() && rail_tts_sites_ensure(MtRail()), "the tower's pipelines and the sites' stamps"))
            with_mt_planes() $(var cp) {
                let tout = 40
                let mp = 64
                let d = 64
                let k = 256
                var inscope w <- row_fill(d * k, 17, 3, 0.04, 1.9)
                var inscope m <- stored_mat("k4", w, d, k, true)
                var inscope x <- [for (i in range(tout * k)); 0.093 * float((i * 5 + 1) % 41) - 1.87 + 2.1e-4 * float(i % 13)]
                var inscope bias <- plane_of(d, 0.0)
                let bp = cp |> in_(bias)
                let qp = cp |> in_plain(m.wq)
                let sp = cp |> in_plain(m.ws)
                let xp = cp |> in_(x)
                let yxp = cp |> out_(mp * d)
                let yhp = cp |> out_(mp * d)
                var site = TtsConvSite(cin = int64(k), k = 1l, stride = 1l, pad_l = 0l, dilation = 1l, tin = int64(tout), tout = int64(tout), cout = int64(d),
                    cout_p = int64(d), kdim_p = int64(k), stored = true, native = true, fmt = KqFmt.k4)
                var served = true
                cp |> cell_launch(t, "st2_k4_conv") $(var rl) {
                    let at = $(pl : int) => rail_buf(rl, cell_role(pl))   // read before the first dispatch re-seats the rail's first roles
                    let pl = fixed_array(at(bp), at(qp), at(sp), at(xp), at(yxp), at(yhp))
                    served = rail_tts_conv(rl, pl[0], pl[1], pl[2], site, pl[3], pl[4]) && served
                    site.half = true
                    served = rail_tts_conv(rl, pl[0], pl[1], pl[2], site, pl[3], pl[5]) && served
                }
                t |> success(served, "the home's conv tile takes the Q4_K leaf in its own layout under either flag")
                var inscope want <- stored_mm_ref(x, x, tout, d, k, k, false) $(o, c) => m.rows[o * k + c]
                let tight = Bar(rel = 1e-4, abs_floor = 1e-4)
                var inscope yx <- cp |> plane_run(yxp)
                var inscope got <- copy_span(yx, 0, tout * d)
                check_rows(t, "st2_k4_conv_exact_mm (the site's half flag off) vs the k-ordered sum", got, want, tight, d)
                var inscope yh <- cp |> plane_run(yhp)
                var inscope staged <- copy_span(yh, 0, tout * d)
                check_rows(t, "st2_k4_conv_mm (the site's half flag on) vs the k-ordered sum at the half tile's bar", staged, want, cm_bar(want), d)
                check_exceeds(t, "st2_k4_conv_mm control: the f16-staged tile over operands off the half lattice lands outside the float tile's bar", staged, want, tight)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamps of the f32 tile GEMM, plain and biased, at 37 x 100 x 96 off every tile multiple match the k-ordered sum at the approx bar, store every output element, and never read the poisoned slab pad past n")]
def test_mtl_f32_gemm(t0 : T?) {
    t0 |> run("the f32 tile GEMM (Metal rail), plain and biased, == the k-ordered fma sum at off-tile shapes") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_f32_gemm_cls() && ensure_f32_gemm_b_cls(), "f32_gemm_cls, f32_gemm_b_cls"))
            with_mt_planes() $(var cp) {
                f32_gemm_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "Each stored member of the f32 tile GEMM's Metal rail stamps - Q8_0, Q4_K, Q5_K, Q6_K and half floats, in the discrete homes' device layout - matches the k-ordered sum over the rows the CPU reader widens the same planes to: biased, with no bias row named, and under x rows narrower than a weight row")]
def test_mtl_f32_gemm_stored(t0 : T?) {
    t0 |> run("the f32 tile's stored members (Metal rail) == the k-ordered sum over the CPU reader's rows") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stored_stamps_built(t))
            with_mt_planes() $(var cp) {
                for (member in fixed_array("q8", "k4", "k5", "k6", "f16")) {
                    f32_stored_case(t, cp, member)
                }
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "Each stored member of the f32 tile GEMM's Metal rail stamps over a transposed conv's lift reads row tap * cout + co, column ci of the lift as the tile's row co, column tap * cin + ci, matching the k-ordered sum over the CPU reader's lift rows")]
def test_mtl_f32_gemm_stored_lift(t0 : T?) {
    t0 |> run("the f32 tile's stored members (Metal rail) over a transposed conv's lift == the k-ordered sum over the reader's lift rows") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stored_stamps_built(t))
            with_mt_planes() $(var cp) {
                for (member in fixed_array("q8", "k4", "k5", "k6", "f16")) {
                    f32_stored_lift_case(t, cp, member)
                }
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamps of the f32 conv im2col fed through F32GemmB match the CPU `conv1d` at the approx bar for a forward k5 conv, a transposed k4 s2 conv and a forward conv off row-major x, at element offsets, with poisoned slab pad columns kept out")]
def test_mtl_tts_im2col(t0 : T?) {
    t0 |> run("forward k5, transposed k4 s2 and forward k5 off row-major x (Metal rail): im2col + F32GemmB == the CPU conv1d") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_im2col_cls() && ensure_f32_gemm_b_cls(), "tts_im2col_cls, f32_gemm_b_cls"))
            with_mt_planes() $(var cp) {
                tts_im2col_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail stamp of the LSTM direction over the recurrence's stored Q8_0 plane at hidden 64 over 9 steps reproduces the CPU loop over the rows the reader widens that plane to, forward and backward")]
def test_mtl_tts_lstm_dir_q8(t0 : T?) {
    t0 |> run("tts_lstm_dir_q8 (Metal rail) == the CPU loop over the reader's rows, both directions") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t) || !stamps_built(t, ensure_tts_lstm_dir_q8_cls(), "tts_lstm_dir_q8_cls"))
            with_mt_planes() $(var cp) {
                lstm_q8_case(t, cp)
            }
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal rail's five stored row-GEMV stamps over weight rows held as Q8_0, Q4_K, Q5_K, Q6_K and half floats in the device layout match the CPU `linear_vec` chains over the rows the reader widens the same planes to, within 2e-7 x nin relative, leave every slot outside a row at its sentinel, and land off the chain with its weight rows moved down one")]
def test_mtl_pk_gemv_stored(t0 : T?) {
    for (member in fixed_array("q8", "k4", "k5", "k6", "f16")) {
        t0 |> run("the stored row GEMV (Metal rail), {member} rows == the CPU chains over the reader's rows") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(das_metal)) {
                return if (!metal_up(t) || !stamps_built(t, pkst_stamps_built(), "the stored row GEMV stamps"))
                with_mt_planes() $(var cp) {
                    tts_pk_gemv_stored_case(t, cp, member)
                }
            } else {
                t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
            }
        }
    }
}

[test(description = "The Metal rail's four fused stored frame-loop stamps over weight rows held as Q8_0, Q4_K, Q5_K, Q6_K and half floats in the device layout - the normed qkv row roped into y and written to the caches' one row, the residual adds with and without the scale, the normed GELU - match the CPU chains over the rows the reader widens the same planes to, within the approx bar, leave the caches' other rows untouched, and the scaled add reds against the chain with its weight rows moved down one")]
def test_mtl_pk_gemv_stored_fused(t0 : T?) {
    for (member in fixed_array("q8", "k4", "k5", "k6", "f16")) {
        t0 |> run("the frame loop's fused stored stamps (Metal rail), {member} rows == the CPU chains over the reader's rows") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(das_metal)) {
                return if (!metal_up(t) || !stamps_built(t, pkst_stamps_built(), "the frame loop's fused stored stamps"))
                with_mt_planes() $(var cp) {
                    tts_pk_gemv_stored_fused_case(t, cp, member)
                }
            } else {
                t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
            }
        }
    }
}

[test(description = "The Metal rail's five stored row-GEMV stamps over Q8_0 and Q4_K weight rows in the Metal home's own layout - 34-byte Q8_0 blocks, Q4_K quants in disk order beside their scale strips - match the CPU `linear_vec` chains over the rows the reader widens the same planes to, within 2e-7 x nin relative, leave every slot outside a row at its sentinel, and land off the chain with its weight rows moved down one")]
def test_mtl_pk_gemv_own(t0 : T?) {
    for (member in fixed_array("q8", "k4")) {
        t0 |> run("the stored row GEMV (Metal rail), {member} rows in the home's own layout == the CPU chains over the reader's rows") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(das_metal)) {
                return if (!metal_up(t) || !stamps_built(t, pkst_stamps_built(), "the stored row GEMV stamps"))
                with_mt_planes() $(var cp) {
                    tts_pk_gemv_stored_case(t, cp, member, true)
                }
            } else {
                t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
            }
        }
    }
}

let private PROF_N = 4099   //! four whole 1024-element workgroups and three more

//! the cell's plane through one TTS shot: the tanh GELU marked `tts_act`, the leaky ReLU marked
//! `tts_rows`, a second leaky ReLU past the last mark, and with `extra` a third mark after it
def private prof_shot(var cp : auto(C); x : array<float>; extra : bool) : array<float> {
    cp |> cell_clear()
    let xp = cp |> io_(x)
    var rl <- cell_rail(cp)
    var pg = TtsElemArgs(nelem = uint(PROF_N), off = 0u, slope = 0.0)
    var pl = TtsElemArgs(nelem = uint(PROF_N), off = 0u, slope = 0.2)
    rail_tts_shot(rl, "prof cell", 1l, int64(PROF_N)) {
        enc_tts_gelu_tanh_cls(rl, seats(fixed_array(xp)), pg, int64(PROF_N))
        rail_pt(rl, RailProfRole.tts_act)
        enc_tts_leaky_cls(rl, seats(fixed_array(xp)), pl, int64(PROF_N))
        rail_pt(rl, RailProfRole.tts_rows)
        enc_tts_leaky_cls(rl, seats(fixed_array(xp)), pl, int64(PROF_N))
        if (extra) {
            rail_pt(rl, RailProfRole.tts_rows)
        }
    }
    return <- cp |> plane_run(xp)
}

[test(description = "A Metal TTS shot under the profile ledger's pin runs a command buffer a mark, charges each marked role nonzero device time over its one stamp, leaves the dispatch past the last mark uncharged, and computes bit for bit what the unprofiled shot computes; with the pin off a shot reports nothing, and a model's discharge drops the last report")]
def test_mtl_tts_prof_ledger(t0 : T?) {
    t0 |> run("the Metal shot's profile ledger: a role a mark, device time, the rows unchanged") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            let built = ensure_tts_gelu_tanh_cls() && ensure_tts_leaky_cls()
            return if (!metal_up(t) || !stamps_built(t, built, "tts_gelu_tanh_cls and tts_leaky_cls"))
            var inscope x <- [for (i in range(PROF_N)); float(i % 61 - 30) * 0.37]
            let was = set_rail_prof_pin(true)
            with_mt_planes() $(var cp) {
                var inscope profiled <- prof_shot(cp, x, false)
                let s = rail_prof_last()
                set_rail_prof_pin(false)
                var inscope plain <- prof_shot(cp, x, true)
                let act = int(RailProfRole.tts_act)
                let rows = int(RailProfRole.tts_rows)
                t |> equal(s.stamps, 2)
                t |> equal(s.n[act], 1)
                t |> equal(s.n[rows], 1)
                t |> success(s.us[act] > 0.0lf && s.us[rows] > 0.0lf,
                    "both marked roles took device time ({s.us[act]} and {s.us[rows]} us)")
                t |> success(s.tot == s.us[act] + s.us[rows], "the total is the roles' sum")
                t |> equal(rail_prof_last().stamps, 2)   // the pin-off shot's marks reported nothing
                check_rows(t, "the profiled shot vs the one-buffer shot", profiled, plain, BAR_EXACT,
                    FLAT_ROWS)
            }
            bump_weights_epoch()
            t |> equal(rail_prof_last().stamps, 0, "a model's discharge drops the last report: the next model's reader finds none")
            set_rail_prof_pin(was)
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}

[test(description = "The Metal home counts its dense dispatches over widened rows on a hand-built Pocket frame model 36 wide with a 20-wide latent: over a backbone projection held as half floats off the 32 lattice the prompt seat's rows GEMM counts once for each of the 2 layers and the frames seat's fused projection once a layer for each of the 3 frames, over an input linear of halves the frame's row GEMV counts once a frame, and the same model of floats counts nothing on either seat")]
def test_mtl_tts_widened_counts(t0 : T?) {
    t0 |> run("the Metal home's widened-dispatch count, a site a leaf of halves; the model of floats counts none") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(das_metal)) {
            return if (!metal_up(t))
            tts_widened_counts_case(t)
        } else {
            t |> skip("das_metal is not built on this platform; the Metal rail is not compiled")
        }
    }
}
