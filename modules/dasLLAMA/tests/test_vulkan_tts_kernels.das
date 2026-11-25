options gen2
options _comment_hygiene = true
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

module test_vulkan_tts_kernels

require dastest/testing_boost public
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require _discrete_tts_cells   // nolint:STYLE030,LINT019 — the case bodies both discrete homes run, inside the vulkan arms alone

//! The TTS tower's Vulkan kernel classes against their CPU oracles (a Vulkan device, else the cells skip, and a stamp
//! that does not build reds its cell): the f32-exact tile GEMM the front end feeds on, in both its stamps and its
//! stored members, at shapes off every tile and workgroup multiple, and the PL-BERT seat's row classes - the embedding
//! gather, the tanh GELU, the leaky ReLU and the bidirectional attention - and the predictor and decoder row classes -
//! the concat, the duration sigmoid sums, the column statistics with the fused AdaIN stamps and the residual join -
//! with the poisoned-element control on every compare. Every cell runs the case body the CUDA home's cell runs
//! (`_discrete_tts_cells.das`).

//! the four column-statistics stamps built
def private colstats_ready : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return tts_colstats_ready(VtRail())
    } else {
        return false
    }
}

//! the column statistics and both fused AdaIN stamps built
def private adain_ready : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return tts_adain_ready(VtRail())
    } else {
        return false
    }
}

[test(description = "The f32 tile GEMM, plain and biased, at 37 x 100 x 96 off every tile multiple matches the k-ordered sum at the approx bar, stores every output element, and never reads the poisoned slab pad past n")]
def test_vkt_f32_gemm(t0 : T?) {
    t0 |> run("the f32 tile GEMM, plain and biased, == the k-ordered fma sum at off-tile shapes") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t))
            let built = ensure_f32_gemm_cls() && ensure_f32_gemm_b_cls()
            return if (!stamps_built(t, built, "f32_gemm_cls and f32_gemm_b_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                f32_gemm_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "Each stored member of the f32 tile GEMM - Q8_0, Q4_K, Q5_K, Q6_K and half floats, in the discrete homes' device layout - matches the k-ordered sum over the rows the CPU reader widens the same planes to: biased, with no bias row named, under x rows narrower than a weight row, and over a transposed conv's lift (row tap * cout + co, column ci as the tile's row co, column tap * cin + ci)")]
def test_vkt_f32_gemm_stored(t0 : T?) {
    t0 |> run("the f32 tile's stored members == the k-ordered sum over the CPU reader's rows, dense and over a lift") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t))
            let built = tts_stored_gemm_ready(VtRail())
            return if (!stamps_built(t, built, "the f32_gemm stored member stamps"))
            with_cell_planes() $(var cp : CellPlanes) {
                for (member in fixed_array("q8", "k4", "k5", "k6", "f16")) {
                    f32_stored_case(t, cp, member)
                    f32_stored_lift_case(t, cp, member)
                }
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The embedding gather at element bases off zero equals the CPU word + position + type sum bit for bit, with neither add, each add alone and both, follows its ids, and leaves the prefix before out_off at its sentinel")]
def test_vkt_row_gather(t0 : T?) {
    t0 |> run("the embedding gather == the CPU word + position + type sum, exact, at element bases off zero") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_row_gather_cls(), "tts_row_gather_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_row_gather_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The tanh GELU over a 5203-element run at an offset matches the CPU `gelu` within 1e-5 relative, inputs at +-30 past the tanh clamp included, leaves the outside untouched, and the bar tells it from the f16 LUT form")]
def test_vkt_gelu_tanh(t0 : T?) {
    t0 |> run("the tanh GELU == the CPU gelu at the f32 bar, the run's tail and the tanh clamp included") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_gelu_tanh_cls(), "tts_gelu_tanh_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_gelu_tanh_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The leaky ReLU over a 5203-element run at an offset equals the CPU `leaky_relu` bit for bit at slope 0.2, the three-element tail included, with the elements outside the run untouched")]
def test_vkt_leaky(t0 : T?) {
    t0 |> run("the leaky ReLU == the CPU leaky_relu, exact, the run's tail included") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_leaky_cls(), "tts_leaky_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_leaky_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The TTS bidirectional attention matches the CPU `attention_rows` within 1e-5 relative at 37x4x64, 130x2x128 and 512x2x64 (two keys an invocation), and zeroing the last key moves the first query row, so every key is read")]
def test_vkt_attn(t0 : T?) {
    t0 |> run("the bidirectional attention == the CPU attention_rows at hs 64 and 128, one and two keys an invocation") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_attn_cls(), "tts_attn_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_attn_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The concat rows equal the CPU concat bit for bit with one broadcast style row, and with the f0 and noise columns read at their row strides plus a zero pad to width 48")]
def test_vkt_concat(t0 : T?) {
    t0 |> run("the concat rows == the CPU concat, exact: one style row broadcast, then f0 and n at strides and a zero pad") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_concat_cls(), "tts_concat_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_concat_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The duration sigmoid sums over the first 50 of 64 logit columns at speed 1.3 match the CPU raw durations within 1e-5 relative, with the poisoned columns past nb kept out of the sum")]
def test_vkt_sigsum(t0 : T?) {
    t0 |> run("the sigmoid sums == the CPU raw durations over 50 of 64 columns at speed 1.3") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_sigsum_cls(), "tts_sigsum_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_sigsum_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The four-pass column statistics match the double-precision channel means and squared deviations at the approx bar at 45x72, 300x20 (a short last block) and 1000x300 (lanes taking two channels)")]
def test_vkt_colstats(t0 : T?) {
    t0 |> run("the column statistics == the double-precision mean and squared deviations, one and two strides a lane") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, colstats_ready(), "the column statistics stamps"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_colstats_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The fused AdaIN stamps over device column stats match the CPU `adain_rows_into` plus leaky ReLU or Snake within 1e-4 relative, and the in-place leaky stamp equals the out-of-place one bit for bit with the slack untouched")]
def test_vkt_adain(t0 : T?) {
    t0 |> run("the column stats + fused AdaIN, leaky and Snake stamps, == the CPU adain_rows_into, out of place and in place") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t))
            let built = adain_ready()
            return if (!stamps_built(t, built, "the column statistics and fused AdaIN stamps"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_adain_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The residual add-and-scale equals (a + b) / sqrt(2) in f32 bit for bit over 1030 elements, out of place and in place, with the elements past the run left as they were")]
def test_vkt_add_scale(t0 : T?) {
    t0 |> run("the add-and-scale == (a + b) * 1/sqrt(2) in f32, exact, out of place and in place") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_add_scale_cls(), "tts_add_scale_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_add_scale_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The row-broadcast bias adds the bias row at an element base to 434 rows of 12 in place - alone it equals x + b bit for bit, under relu max(x + b, 0) - leaves the elements past the rows untouched, lands off the input, and the two arms differ")]
def test_vkt_bias_act(t0 : T?) {
    t0 |> run("the bias row and the relu == x + b and max(x + b, 0), exact") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tower_bias_act_cls(), "tower_bias_act_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tower_bias_act_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}
