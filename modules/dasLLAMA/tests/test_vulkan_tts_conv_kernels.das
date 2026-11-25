options gen2
options _comment_hygiene = true
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require ?vulkan vulkan
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require _compares   // nolint:STYLE030,LINT019 — the compares, inside the vulkan arms alone
require dasllama/dasllama_convert   // nolint:STYLE030,LINT019 — cvt_f32_to_f16_sat, inside the vulkan arms alone
require dasllama/dasllama_tts_blocks   // nolint:STYLE030,LINT019 — conv1d_out_len, inside the vulkan arm alone
require dasllama/dasllama_math   // nolint:STYLE030,LINT019 — transpose_ct, inside the vulkan arm alone
require dasllama/dasllama_gpu_math   // nolint:STYLE030,LINT019 - the row stamps' activation codes, inside the device arms
require _discrete_tts_cells   // nolint:STYLE030,LINT019 — the stored recurrence's case body both discrete homes run, inside the vulkan arm alone
require _discrete_tts_conv_cells   // nolint:STYLE030,LINT019 — the sequence classes' case bodies both discrete homes run and the conv fixture, inside the vulkan arms alone

//! The TTS tower's sequence classes on Vulkan against the CPU chain (a Vulkan device, else the cells skip, and a stamp
//! that does not build reds its cell; the half im2col cell also skips where the device serves no f16 tile): the conv
//! im2col in both directions, fed through F32GemmB and held to the CPU `conv1d` end to end, the half im2col through the
//! f16 tile and the bias pass, one LSTM direction held to the CPU `bilstm` on both walks, and the depthwise transposed
//! pool held to its CPU rows form, at channel and hidden widths off every lattice, with the poisoned-element control on
//! every compare. The cells but the half im2col run the case bodies the CUDA home's cells run
//! (`_discrete_tts_conv_cells.das`, `_discrete_tts_cells.das`); the half im2col reads the Vulkan home's f16 tile pick
//! (cm2 or the KHR twin) and stays here.


let private COL16_SLACK_ROWS = 64   //! the half column plane's rows past the conv's: the KHR tile loads whole 16-row blocks
let private F16_LATTICE = 1.0 / 64.0   //! the half route's operand step: sixty-fourths within F16_LATTICE_OFF of zero, every product exact on the f16 tile
let private F16_LATTICE_OFF = 0.75     //! the half route's operand offset: `row_fill`'s sixty-fourths less it span [-0.75, 0.75]

//! one conv's half im2col through the f16 tile and the bias pass vs the CPU conv1d at `approx` (2e-3), on f16-exact operands
def private im2col16_arm(t : T?; tag : string; g : ConvGeom) {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        let khr = g_gpu.coopmat_mode != COOPMAT_CM2   // the pick the driver makes where an f16 tile serves at all (the half cell skipped otherwise)
        var c <- make_conv(g, F16_LATTICE, F16_LATTICE_OFF)
        let tout = int(conv1d_out_len(c, int64(g.tin)))
        t |> equal(tout, g.tout, "{tag}: the output length")
        let kdim = (g.k * g.cin + 31) / 32 * 32
        var x <- row_fill(g.cin * g.tin, 41, 7, F16_LATTICE, F16_LATTICE_OFF)
        var x_rows : array<float>
        if (g.rows_x) {
            transpose_ct(x, int64(g.cin), int64(g.tin), x_rows)
        }
        var y_ref <- conv_ref(c, x, g.tin)
        var w <- slab_rows(c, kdim)
        var w16 : array<uint16>
        w16 |> resize(length(w))
        unsafe {
            cvt_f32_to_f16_sat(addr(w16[0]), addr(w[0]), long_length(w))
        }
        let ny = tout * g.cout
        with_cell_planes() $(var cp : CellPlanes) {
            let xp = cp |> in_(g.rows_x ? x_rows : x, g.xoff)
            let colp = cp |> out_((tout + COL16_SLACK_ROWS) * kdim / 2)   // the half plane: two halfwords a float slot
            let wp = cp |> in_plain(w16)
            let bp = cp |> in_(c.b)
            let yp = cp |> out_(ny)
            var pc = GkIm2col1dArgs(cin = uint(g.cin), tin = uint(g.tin), tout = uint(tout), k = uint(g.k), stride = uint(g.stride),
                pad_l = uint(g.pad), dilation = 1u, transposed = g.transposed ? 1u : 0u, kdim = uint(kdim), xoff = uint(g.xoff),
                coloff = uint(g.coloff), row_major = g.rows_x ? 1u : 0u, r0 = 0u, mp = uint(tout))
            var s_col = cp |> cls_set(fixed_array(xp, colp), @@set_tts_im2col16_cls)
            let mm = fixed_array(wp, colp, yp)
            var s_mm = f16_gemm_set(khr, gather_planes(cp.dev, mm), gather_planes(cp.bytes, mm), gather_planes(cp.hazard_bit, mm))
            var s_b = cp |> cls_set(fixed_array(yp, bp), @@set_tower_bias_act_cls)
            var pm = F16GemmArgs(n = uint(kdim), d = uint(g.cout), rows = uint(tout), woff = 0u, ybase = 0u, ystride = uint(g.cout), ksplit = 0u)
            var pb = GkBiasActArgs(d = uint(g.cout), nelem = uint(ny), boff = 0u, act = BIAS_ACT_NONE)
            cp |> cell_record() $(raw : VkCommandBuffer; var h : VkHaz) {
                enc_tts_im2col16_cls(raw, h, s_col, pc, int64(tout * kdim))
                f16_gemm_enc(khr, raw, h, s_mm, pm, int64(tout), int64(g.cout))
                enc_tower_bias_act_cls(raw, h, s_b, pb, (int64(ny) / 4l + 255l) / 256l)
            }
            let route = "im2col16 + the {khr ? "KHR" : "cm2"} f16 tile + the bias"
            check_rows(t, "{tag} {route} over {tout} x {g.cout} vs the CPU conv1d", cp |> plane_run(yp), y_ref, BAR_APPROX, g.cout)
        }
        delete c
        delete w
        delete w16
        delete x
        delete x_rows
        delete y_ref
    }
}

[test(description = "The half im2col through the f16 GEMM tile plus the bias pass matches the CPU `conv1d` at the approx bar on f16-exact operands, for a forward k5, a transposed k4 s2 and a forward k3 over row-major x")]
def test_vkt_tts_im2col16(t0 : T?) {
    t0 |> run("the f16 tile: forward k5 s1 p2 over 22 channels into 40, transposed k4 s2 p1 over 16 into 32, forward k3 over 32 into 64 (a 96-wide row)") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t))
            if (g_gpu.coopmat_mode != COOPMAT_CM2 && !khr_kq_tile_on()) {
                t |> skip("no Vulkan device with an f16 tile (neither cm2 nor the KHR tile at subgroup 32)")
                return
            }
            let built = ensure_tts_im2col16_cls() && ensure_tower_bias_act_cls() && f16_gemm_ensure(g_gpu.coopmat_mode != COOPMAT_CM2)
            return if (!stamps_built(t, built, "the f16 tile's classes"))
            im2col16_arm(t, "im2col16 forward k5 cin22", ConvGeom(cin = 22, cout = 40, k = 5, stride = 1, pad = 2, tin = 37, tout = 37,
                xoff = 0, coloff = 0, transposed = false))
            im2col16_arm(t, "im2col16 transposed k4 s2 cin16", ConvGeom(cin = 16, cout = 32, k = 4, stride = 2, pad = 1, tin = 19, tout = 38,
                xoff = 5, coloff = 0, transposed = true))
            im2col16_arm(t, "im2col16 forward k3 cin32 rows", ConvGeom(cin = 32, cout = 64, k = 3, stride = 1, pad = 1, tin = 50, tout = 50,
                xoff = 0, coloff = 0, transposed = false, rows_x = true))
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The f32 conv im2col fed through F32GemmB matches the CPU `conv1d` at the approx bar for a forward k5 conv, a transposed k4 s2 conv and a forward conv off row-major x, at element offsets, with poisoned slab pad columns kept out")]
def test_vkt_tts_im2col(t0 : T?) {
    t0 |> run("forward k5 s1 p2 over 22 channels, transposed k4 s2 p1 over 16, forward k5 off row-major x: im2col + F32GemmB == the CPU conv1d") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t))
            let built = ensure_tts_im2col_cls() && ensure_f32_gemm_b_cls()
            return if (!stamps_built(t, built, "tts_im2col_cls and f32_gemm_b_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_im2col_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The LSTM direction stamp at hidden 64 and 200 over 23 steps reproduces the CPU `bilstm` within 1e-4 relative for both directions into one row set and for the forward weights walked backward, and flipping `backward` moves the output")]
def test_vkt_tts_lstm_dir(t0 : T?) {
    for (hd in fixed_array(64, 200)) {
        t0 |> run("one LSTM direction at hidden {hd}, both walks == the CPU bilstm") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                return if (!vk_up(t) || !stamps_built(t, ensure_tts_lstm_dir_cls(), "tts_lstm_dir_cls"))
                with_cell_planes() $(var cp : CellPlanes) {
                    tts_lstm_case(t, cp, hd)
                }
            } else {
                t |> skip("dasVulkan not present")
            }
        }
    }
}

[test(description = "The fold stamp over the lift rows of a transposed k4 s2 conv - x times the lift matrix - plus the bias matches the CPU `conv1d` at the approx bar, at element bases off zero, the prefix before the output rows kept")]
def test_vkt_tts_fold(t0 : T?) {
    t0 |> run("the transposed conv's overlap-add over its lift rows == the CPU conv1d") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_fold_cls(), "tts_fold_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_fold_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The LSTM direction stamp over the recurrence's stored Q8_0 plane at hidden 64 over 9 steps reproduces the CPU loop over the rows the reader widens that plane to, forward and backward")]
def test_vkt_tts_lstm_dir_q8(t0 : T?) {
    t0 |> run("the LSTM direction over its stored Q8_0 recurrence == the CPU loop over the reader's rows, both directions") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_lstm_dir_q8_cls(), "tts_lstm_dir_q8_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                lstm_q8_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The depthwise transposed pool k3 s2 p1 with output padding over 22 channels and 19 steps produces 38 rows matching `conv1d_rows_transposed_depthwise` within 1e-5 relative, reading weights and bias between poisoned slots")]
def test_vkt_tts_pool_dw(t0 : T?) {
    t0 |> run("the depthwise transposed pool k3 s2 p1 over 22 channels == the CPU conv1d_rows_transposed_depthwise") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_pool_dw_cls(), "tts_pool_dw_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_pool_dw_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
