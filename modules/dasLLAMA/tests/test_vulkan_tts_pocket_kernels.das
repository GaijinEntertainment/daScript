options gen2
options _comment_hygiene = true
options _dasllama_internal = true
options persistent_heap   // a dastest root frees what it deletes only on this heap; the linear heap holds every carrier a cell deleted until the context ends

require dastest/testing_boost public
require ?vulkan dasllama/dasllama_vulkan_common
require ?vulkan dasllama/dasllama_vulkan_classes
require ?vulkan _vkd_oracles
require _discrete_tts_pocket_cells   // nolint:STYLE030,LINT019 — the case bodies both discrete homes run, inside the vulkan arms alone

//! The Pocket TTS family's Vulkan classes against the CPU chain (a Vulkan device, else the cells skip, and a stamp that
//! does not build reds its cell): the codec stream's row copies with and without the ELU, the layer scale, the causal
//! cached attention row over the caches' rows, the row GEMV's five stamps and the frame loop's four fused stamps, and
//! the rows rope; every written-only output under a NaN fill, every compare with its poisoned element. Every cell runs
//! the case body the CUDA home's cell runs (`_discrete_tts_pocket_cells.das`).

//! every pk gemv stamp built
def private pk_gemv_ready : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return tts_pk_gemv_ready(VtRail())
    } else {
        return false
    }
}

//! every pk fused stamp built
def private pk_fused_ready : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return tts_pk_fused_ready(VtRail())
    } else {
        return false
    }
}

[test(description = "The Pocket row copy writes a 7 x 13 strided window bit for bit and its ELU twin matches `elu_rows` within 1e-6 relative, moving only the negative elements, with every element outside the window left at its sentinel")]
def test_vkt_pk_rows(t0 : T?) {
    t0 |> run("the row copies, plain and ELU, == the CPU window copy and elu_rows at strides and offsets off zero") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t))
            let built = ensure_tts_pk_rows_cls() && ensure_tts_pk_rows_elu_cls()
            return if (!stamps_built(t, built, "tts_pk_rows_cls and tts_pk_rows_elu_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_pk_rows_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The in-place layer scale over 37 x 72 rows with the scale row at an offset equals the CPU `layer_scale_rows` bit for bit, leaving the slack past the rows untouched")]
def test_vkt_pk_row_scale(t0 : T?) {
    t0 |> run("the layer scale == the CPU layer_scale_rows, exact, at t 37 x 72 with the scale row at an offset") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_pk_row_scale_cls(), "tts_pk_row_scale_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_pk_row_scale_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The causal cached attention row matches `attention_causal_rows` within 1e-5 relative on a 45-query prompt, a decode step over an 8-key window, the prompt over 16 keys and a decode step over 300 keys, past one 256-key stride of the scores a row stages, where the row over the first stride's keys alone lands outside the bar; a poisoned key before the window changes no bit")]
def test_vkt_pk_attn(t0 : T?) {
    t0 |> run("the causal attention row == the CPU attention_causal_rows: a 45-query prompt, a decode step over an 8-key window, the prompt over 16, a decode step over 300 keys") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_pk_attn_cls(), "tts_pk_attn_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_pk_attn_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The attention row with four query heads over two key-value heads matches `attention_causal_rows` within 1e-5 relative over every key and over an 8-key window, and the window measurably moves the late rows")]
def test_vkt_pk_attn_grouped(t0 : T?) {
    t0 |> run("the attention row over grouped keys == the CPU attention_causal_rows: four query heads a pair of key-value heads, every key, then an 8-key window") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_pk_attn_cls(), "tts_pk_attn_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_pk_attn_grouped_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The row GEMV's five stamps (dot, ln+modulate+silu, gated residual, add+silu, tail with latent) at nin 200, nout 45 match the CPU `linear_vec` chains within 2e-7 x nin relative with weight rows on and off the float4 lattice")]
def test_vkt_pk_gemv(t0 : T?) {
    t0 |> run("the row GEMV's five stamps == the CPU linear_vec chains at nin 200 / nout 45, the slab's runs at offsets, the rows on and off the four lattice") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, pk_gemv_ready(), "the row GEMV's five stamps"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_pk_gemv_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The table rope over the k span of 23 fused qkv rows from position 37 matches the CPU `rope_rows` within 1e-6 relative, with the q and v spans and the slack left exactly as they were")]
def test_vkt_pk_rope(t0 : T?) {
    t0 |> run("the rows rope == the CPU rope_rows on the k span of t 23 rows from position 37, the q and v spans untouched") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, ensure_tts_pk_rope_cls(), "tts_pk_rope_cls"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_pk_rope_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

[test(description = "The frame loop's fused stamps match the CPU chains at the approx bar: the normed qkv row roped into y and written to the caches' one row (wide and grouped forms), the residual adds with and without scale, and the normed GELU")]
def test_vkt_pk_gemv_fused(t0 : T?) {
    t0 |> run("the frame loop's fused stamps == the CPU chains: the normed qkv row roped into y and the caches, the residual adds, the normed GELU") <| @(t : T?) {
        static_if (typeinfo builtin_module_exists(vulkan)) {
            return if (!vk_up(t) || !stamps_built(t, pk_fused_ready(), "the frame loop's fused stamps"))
            with_cell_planes() $(var cp : CellPlanes) {
                tts_pk_gemv_fused_case(t, cp)
            }
        } else {
            t |> skip("dasVulkan not present")
        }
    }
}

//! the stored row GEMV's stamps built: every kind of every member
def private pkst_stamps_built : bool {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        var rl : VtRail
        return tts_pkst_ready(rl)
    } else {
        return false
    }
}

[test(description = "The five stored row-GEMV stamps over weight rows held as Q8_0, Q4_K, Q5_K, Q6_K and half floats in the device layout match the CPU `linear_vec` chains over the rows the reader widens the same planes to, within 2e-7 x nin relative, leave every slot outside a row at its sentinel, and land off the chain with its weight rows moved down one")]
def test_vkt_pk_gemv_stored(t0 : T?) {
    for (member in fixed_array("q8", "k4", "k5", "k6", "f16")) {
        t0 |> run("the stored row GEMV, {member} rows == the CPU chains over the reader's rows") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                return if (!vk_up(t) || !stamps_built(t, pkst_stamps_built(), "the stored row GEMV stamps"))
                with_cell_planes() $(var cp : CellPlanes) {
                    tts_pk_gemv_stored_case(t, cp, member)
                }
            } else {
                t |> skip("dasVulkan not present")
            }
        }
    }
}

[test(description = "The four fused stored frame-loop stamps over weight rows held as Q8_0, Q4_K, Q5_K, Q6_K and half floats in the device layout - the normed qkv row roped into y and written to the caches' one row, the residual adds with and without the scale, the normed GELU - match the CPU chains over the rows the reader widens the same planes to, within the approx bar, leave the caches' other rows untouched, and the scaled add reds against the chain with its weight rows moved down one")]
def test_vkt_pk_gemv_stored_fused(t0 : T?) {
    for (member in fixed_array("q8", "k4", "k5", "k6", "f16")) {
        t0 |> run("the frame loop's fused stored stamps, {member} rows == the CPU chains over the reader's rows") <| @(t : T?) {
            static_if (typeinfo builtin_module_exists(vulkan)) {
                return if (!vk_up(t))
                let built = pkst_stamps_built()
                return if (!stamps_built(t, built, "the frame loop's fused stored stamps"))
                with_cell_planes() $(var cp : CellPlanes) {
                    tts_pk_gemv_stored_fused_case(t, cp, member)
                }
            } else {
                t |> skip("dasVulkan not present")
            }
        }
    }
}
