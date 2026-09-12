options gen2
options _comment_hygiene = true
options _dasllama_internal = true

module dasllama_vulkan_prefill shared public
options _cyclomatic_complexity = 0   // batched record/setup walks — split queued for the dedup phase
options _function_length = 0   // same

require dasllama/dasllama_lint public
require dasllama/dasllama_vulkan_common   // g_gpu/g_rd state, plumbing, hazard rail, fill metas
require dasllama/dasllama_vulkan_classes   // the class-kernel rail (generated set_/enc_ + gemv_cls_*)
require dasllama/dasllama_vulkan_decode   // vk_moe_dn_step — the dn chain's npos==1 arm routes through the decode step
require dasllama/dasllama_kqformat   // KqFmt — the arena fmt slots are its ordinals
require dasllama/dasllama_env   // g_env_vulkan — the vk_kv_merge / vk_overlap / vk_fuse A/B hatches
require dasllama/dasllama_gpu_tier   // moe_gpu_stream_need — the stream-slot carve's source of truth
require vulkan
require vulkan/vulkan_boost
require dasllama/dasllama_math
require math

// The Vulkan PREFILL + batch driver (dasllama_metal_prefill's twin): the resident batched
// prefill (pf_setup / vk_rdec_prefill), the arena FFN/GEMM/GEMV seams, the per-format batched
// GEMM chains (FFN batch + dense), deltanet + attention prefill dispatch, and the streamed
// pinned-mirror rail.
//
// STREAMED layers (DASLLAMA_GPU_MOE_STREAM=N, the N layers below the resident set): the loader
// keeps their device-layout planes in pinned host mirrors; per prefill each layer's stacks DMA
// into one of two device slots whose pseudo-stacks re-base onto the layer, so the batch rail
// runs verbatim. After a layer's fenced chain, the NEXT layer's copy submits UNFENCED and rides
// the GPU-idle window while the CPU runs attention — the following chain's transfer->compute
// barrier orders it (in-order queue). Decode of streamed layers stays on the CPU planes.

// ===== the resident prefill driver (batched, npos rows, one submit) =====
// Batch-kernel twin of vk_rdec_token; only the LAST row's logits are produced (reuses the decode
// tail). PF_ROLES prefill sets/layer + the buffers are built once; metas fill per call (npos varies).

let private PF_ROLES = 16l   // quant_xb q k v rope attn quant_at wo addr_ffn quant_xb2 gate up actrq down addr_next qkn

var private @scratch g_pf_ids_stage : array<uint>

//! whether the prefill's f16 feed admits a plane of format `f` on the armed coopmat mode - the tests read the same answer the driver acts on
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice")]
def public pf_f16_feed(f : int) : bool {
    if (g_gpu.coopmat_mode == COOPMAT_CM2) {
        return f == int(KqFmt.q8) || kq_sb(f)
    }
    return g_gpu.coopmat_mode == COOPMAT_MM && kq_sb(f) && khr_kq_tile_on()
}

[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice")]
def private pf_qkv6(l : int64) : bool => (pf_f16_feed(g_rd.layers[l].fq)
    && pf_f16_feed(g_rd.layers[l].fk) && pf_f16_feed(g_rd.layers[l].fv))
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice")]
def private pf_gu6(l : int64) : bool => g_env_vulkan.vk_f16_ffn && pf_f16_feed(g_rd.layers[l].f1) && pf_f16_feed(g_rd.layers[l].f3)
//! the down GEMM's feed: the hidden rows as f16 where the tile admits the plane and the FFN lever leaves the group on it (off it the rows quantize to the Q8 blocks the CPU chain uses)
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice")]
def private pf_dn6(l : int64) : bool => g_env_vulkan.vk_f16_ffn && pf_f16_feed(g_rd.layers[l].f2)
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-dn-block")]
def private pf_dnx6(l : int64) : bool => pf_f16_feed(g_rd.layers[l].fqkv) && pf_f16_feed(g_rd.layers[l].fz)
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-dn-block")]
def private pf_dno6(l : int64) : bool => pf_f16_feed(g_rd.layers[l].fout)

[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
def private pf_qkv_feed_fused(l : int64) : bool => !g_rd.layers[l].recurrent && g_env_vulkan.vk_fuse && (pf_qkv6(l) || !kq_sb(g_rd.layers[l].fq))

let private PF_CHUNK_MAX = 8l   // the overlap ramp doubles 1,2,4 then holds here; the cmd ring is sized from it

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#khr-mm-kq-tile")]
def khr_cls_ensure(fmt : int) : bool {
    if (fmt == int(KqFmt.k4)) return ensure_kq_batch_k4_khr_cls()
    if (fmt == int(KqFmt.k5)) return ensure_kq_batch_k5_khr_cls()
    if (fmt == int(KqFmt.k6)) return ensure_kq_batch_k6_khr_cls()
    if (fmt == int(KqFmt.q40)) return ensure_kq_batch_q40_khr_cls()
    if (fmt == int(KqFmt.iq4xs)) return ensure_kq_batch_iq4xs_khr_cls()
    if (fmt == int(KqFmt.k3)) return ensure_kq_batch_k3_khr_cls()
    if (fmt == int(KqFmt.iq3s)) return ensure_kq_batch_iq3s_khr_cls()
    if (fmt == int(KqFmt.iq3xxs)) return ensure_kq_batch_iq3xxs_khr_cls()
    if (fmt == int(KqFmt.iq4nl)) return ensure_kq_batch_iq4nl_khr_cls()
    if (fmt == int(KqFmt.k2)) return ensure_kq_batch_k2_khr_cls()
    if (fmt == int(KqFmt.iq2s)) return ensure_kq_batch_iq2s_khr_cls()
    if (fmt == int(KqFmt.iq2xs)) return ensure_kq_batch_iq2xs_khr_cls()
    verify(fmt == int(KqFmt.iq2xxs), "vk prefill: the KHR ensure fell through for a format the f16 feed admits")
    return ensure_kq_batch_iq2xxs_khr_cls()
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#khr-mm-kq-tile")]
def khr_cls_set(fmt : int; bufs : uint64 const[5]; sizes : int64 const[5]; gbits : uint const[5]) : VkDescriptorSet {
    if (fmt == int(KqFmt.k4)) return set_kq_batch_k4_khr_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.k5)) return set_kq_batch_k5_khr_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.k6)) return set_kq_batch_k6_khr_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.q40)) return set_kq_batch_q40_khr_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.iq4xs)) return set_kq_batch_iq4xs_khr_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.k3)) return set_kq_batch_k3_khr_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.iq3s)) return set_kq_batch_iq3s_khr_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.iq3xxs)) return set_kq_batch_iq3xxs_khr_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.iq4nl)) return set_kq_batch_iq4nl_khr_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.k2)) return set_kq_batch_k2_khr_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.iq2s)) return set_kq_batch_iq2s_khr_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.iq2xs)) return set_kq_batch_iq2xs_khr_cls(bufs, sizes, gbits)
    verify(fmt == int(KqFmt.iq2xxs), "vk prefill: the KHR set fell through for a format the f16 feed admits")
    return set_kq_batch_iq2xxs_khr_cls(bufs, sizes, gbits)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#khr-mm-kq-tile")]
def khr_cls_enc(fmt : int; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : BatchArgs; groups : int64) {
    if (fmt == int(KqFmt.k4)) {
        enc_kq_batch_k4_khr_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.k5)) {
        enc_kq_batch_k5_khr_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.k6)) {
        enc_kq_batch_k6_khr_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.q40)) {
        enc_kq_batch_q40_khr_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.iq4xs)) {
        enc_kq_batch_iq4xs_khr_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.k3)) {
        enc_kq_batch_k3_khr_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.iq3s)) {
        enc_kq_batch_iq3s_khr_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.iq3xxs)) {
        enc_kq_batch_iq3xxs_khr_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.iq4nl)) {
        enc_kq_batch_iq4nl_khr_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.k2)) {
        enc_kq_batch_k2_khr_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.iq2s)) {
        enc_kq_batch_iq2s_khr_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.iq2xs)) {
        enc_kq_batch_iq2xs_khr_cls(raw, h, s, pc, groups)
    } else {
        verify(fmt == int(KqFmt.iq2xxs), "vk prefill: the KHR enc fell through for a format the f16 feed admits")
        enc_kq_batch_iq2xxs_khr_cls(raw, h, s, pc, groups)
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2e_cls_ensure(fmt : int) : bool {
    if (fmt == int(KqFmt.k4)) return ensure_kq_batch_k4_cm2e_cls()
    if (fmt == int(KqFmt.k5)) return ensure_kq_batch_k5_cm2e_cls()
    if (fmt == int(KqFmt.k6)) return ensure_kq_batch_k6_cm2e_cls()
    if (fmt == int(KqFmt.q40)) return ensure_kq_batch_q40_cm2e_cls()
    if (fmt == int(KqFmt.iq4xs)) return ensure_kq_batch_iq4xs_cm2e_cls()
    if (fmt == int(KqFmt.k3)) return ensure_kq_batch_k3_cm2e_cls()
    if (fmt == int(KqFmt.iq3s)) return ensure_kq_batch_iq3s_cm2e_cls()
    if (fmt == int(KqFmt.iq3xxs)) return ensure_kq_batch_iq3xxs_cm2e_cls()
    if (fmt == int(KqFmt.iq4nl)) return ensure_kq_batch_iq4nl_cm2e_cls()
    if (fmt == int(KqFmt.k2)) return ensure_kq_batch_k2_cm2e_cls()
    if (fmt == int(KqFmt.iq2s)) return ensure_kq_batch_iq2s_cm2e_cls()
    if (fmt == int(KqFmt.iq2xs)) return ensure_kq_batch_iq2xs_cm2e_cls()
    if (fmt == int(KqFmt.iq2xxs)) return ensure_kq_batch_iq2xxs_cm2e_cls()
    verify(fmt == int(KqFmt.q8), "vk prefill: the e-column ensure fell through for a kq format - a format the feed admits must own a tile arm")
    return ensure_q8_batch_cm2e_cls()
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2e_cls_set(fmt : int; bufs : uint64 const[5]; sizes : int64 const[5]; gbits : uint const[5]) : VkDescriptorSet {
    if (fmt == int(KqFmt.k4)) return set_kq_batch_k4_cm2e_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.k5)) return set_kq_batch_k5_cm2e_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.k6)) return set_kq_batch_k6_cm2e_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.q40)) return set_kq_batch_q40_cm2e_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.iq4xs)) return set_kq_batch_iq4xs_cm2e_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.k3)) return set_kq_batch_k3_cm2e_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.iq3s)) return set_kq_batch_iq3s_cm2e_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.iq3xxs)) return set_kq_batch_iq3xxs_cm2e_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.iq4nl)) return set_kq_batch_iq4nl_cm2e_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.k2)) return set_kq_batch_k2_cm2e_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.iq2s)) return set_kq_batch_iq2s_cm2e_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.iq2xs)) return set_kq_batch_iq2xs_cm2e_cls(bufs, sizes, gbits)
    if (fmt == int(KqFmt.iq2xxs)) return set_kq_batch_iq2xxs_cm2e_cls(bufs, sizes, gbits)
    verify(fmt == int(KqFmt.q8), "vk prefill: the e-column set fell through for a kq format - a format the feed admits must own a tile arm")
    return set_q8_batch_cm2e_cls(bufs, sizes, gbits)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2e_cls_enc(fmt : int; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : BatchArgs; groups : int64) {
    if (fmt == int(KqFmt.k4)) {
        enc_kq_batch_k4_cm2e_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.k5)) {
        enc_kq_batch_k5_cm2e_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.k6)) {
        enc_kq_batch_k6_cm2e_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.q40)) {
        enc_kq_batch_q40_cm2e_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.iq4xs)) {
        enc_kq_batch_iq4xs_cm2e_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.k3)) {
        enc_kq_batch_k3_cm2e_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.iq3s)) {
        enc_kq_batch_iq3s_cm2e_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.iq3xxs)) {
        enc_kq_batch_iq3xxs_cm2e_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.iq4nl)) {
        enc_kq_batch_iq4nl_cm2e_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.k2)) {
        enc_kq_batch_k2_cm2e_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.iq2s)) {
        enc_kq_batch_iq2s_cm2e_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.iq2xs)) {
        enc_kq_batch_iq2xs_cm2e_cls(raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.iq2xxs)) {
        enc_kq_batch_iq2xxs_cm2e_cls(raw, h, s, pc, groups)
    } else {
        verify(fmt == int(KqFmt.q8), "vk prefill: the e-column enc fell through for a kq format - a format the feed admits must own a tile arm")
        enc_q8_batch_cm2e_cls(raw, h, s, pc, groups)
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2_cls_ensure(fmt : int; tc : int64) : bool {
    verify(fmt == int(KqFmt.q8) || kq_sb(fmt), "vk prefill: a cm2 tile for a format the f16 feed never admits")
    verify(tc == 256l || tc == 128l || tc == 32l || tc == CM2_TC_E, "vk prefill: a cm2 tile column the pick never returns")
    if (g_gpu.coopmat_mode == COOPMAT_MM) {
        return khr_cls_ensure(fmt)
    }
    if (tc == CM2_TC_E) {
        return cm2e_cls_ensure(fmt)
    }
    if (fmt == int(KqFmt.k4)) {
        return tc == 256l ? ensure_kq_batch_k4_cm2l_cls() : (tc == 128l ? ensure_kq_batch_k4_cm2m_cls() : ensure_kq_batch_k4_cm2s_cls())
    }
    if (fmt == int(KqFmt.k5)) {
        return tc == 256l ? ensure_kq_batch_k5_cm2l_cls() : (tc == 128l ? ensure_kq_batch_k5_cm2m_cls() : ensure_kq_batch_k5_cm2s_cls())
    }
    if (fmt == int(KqFmt.k6)) {
        return tc == 256l ? ensure_kq_batch_k6_cm2l_cls() : (tc == 128l ? ensure_kq_batch_k6_cm2m_cls() : ensure_kq_batch_k6_cm2s_cls())
    }
    if (fmt == int(KqFmt.q40)) {
        return tc == 256l ? ensure_kq_batch_q40_cm2l_cls() : (tc == 128l ? ensure_kq_batch_q40_cm2m_cls() : ensure_kq_batch_q40_cm2s_cls())
    }
    if (fmt == int(KqFmt.iq4xs)) {
        return tc == 256l ? ensure_kq_batch_iq4xs_cm2l_cls() : (tc == 128l ? ensure_kq_batch_iq4xs_cm2m_cls() : ensure_kq_batch_iq4xs_cm2s_cls())
    }
    if (fmt == int(KqFmt.k3)) {
        return tc == 256l ? ensure_kq_batch_k3_cm2l_cls() : (tc == 128l ? ensure_kq_batch_k3_cm2m_cls() : ensure_kq_batch_k3_cm2s_cls())
    }
    if (fmt == int(KqFmt.iq3s)) {
        return tc == 256l ? ensure_kq_batch_iq3s_cm2l_cls() : (tc == 128l ? ensure_kq_batch_iq3s_cm2m_cls() : ensure_kq_batch_iq3s_cm2s_cls())
    }
    if (fmt == int(KqFmt.iq3xxs)) {
        return tc == 256l ? ensure_kq_batch_iq3xxs_cm2l_cls() : (tc == 128l ? ensure_kq_batch_iq3xxs_cm2m_cls() : ensure_kq_batch_iq3xxs_cm2s_cls())
    }
    if (fmt == int(KqFmt.iq4nl)) {
        return tc == 256l ? ensure_kq_batch_iq4nl_cm2l_cls() : (tc == 128l ? ensure_kq_batch_iq4nl_cm2m_cls() : ensure_kq_batch_iq4nl_cm2s_cls())
    }
    if (fmt == int(KqFmt.k2)) {
        return tc == 256l ? ensure_kq_batch_k2_cm2l_cls() : (tc == 128l ? ensure_kq_batch_k2_cm2m_cls() : ensure_kq_batch_k2_cm2s_cls())
    }
    if (fmt == int(KqFmt.iq2s)) {
        return tc == 256l ? ensure_kq_batch_iq2s_cm2l_cls() : (tc == 128l ? ensure_kq_batch_iq2s_cm2m_cls() : ensure_kq_batch_iq2s_cm2s_cls())
    }
    if (fmt == int(KqFmt.iq2xs)) {
        return tc == 256l ? ensure_kq_batch_iq2xs_cm2l_cls() : (tc == 128l ? ensure_kq_batch_iq2xs_cm2m_cls() : ensure_kq_batch_iq2xs_cm2s_cls())
    }
    if (fmt == int(KqFmt.iq2xxs)) {
        return tc == 256l ? ensure_kq_batch_iq2xxs_cm2l_cls() : (tc == 128l ? ensure_kq_batch_iq2xxs_cm2m_cls() : ensure_kq_batch_iq2xxs_cm2s_cls())
    }
    verify(fmt == int(KqFmt.q8), "vk prefill: the cm2 ensure fell through for a kq format - a format the feed admits must own a tile arm")
    return tc == 256l ? ensure_q8_batch_cm2l_cls() : (tc == 128l ? ensure_q8_batch_cm2m_cls() : ensure_q8_batch_cm2s_cls())
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2_cls_set(fmt : int; tc : int64; bufs : uint64 const[5]; sizes : int64 const[5]; gbits : uint const[5]) : VkDescriptorSet {
    verify(tc == 256l || tc == 128l || tc == 32l || tc == CM2_TC_E, "vk prefill: a cm2 tile column the pick never returns")
    if (g_gpu.coopmat_mode == COOPMAT_MM) {
        return khr_cls_set(fmt, bufs, sizes, gbits)
    }
    if (tc == CM2_TC_E) {
        return cm2e_cls_set(fmt, bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.k4)) {
        if (tc == 256l) {
            return set_kq_batch_k4_cm2l_cls(bufs, sizes, gbits)
        }
        return tc == 128l ? set_kq_batch_k4_cm2m_cls(bufs, sizes, gbits) : set_kq_batch_k4_cm2s_cls(bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.k5)) {
        if (tc == 256l) {
            return set_kq_batch_k5_cm2l_cls(bufs, sizes, gbits)
        }
        return tc == 128l ? set_kq_batch_k5_cm2m_cls(bufs, sizes, gbits) : set_kq_batch_k5_cm2s_cls(bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.k6)) {
        if (tc == 256l) {
            return set_kq_batch_k6_cm2l_cls(bufs, sizes, gbits)
        }
        return tc == 128l ? set_kq_batch_k6_cm2m_cls(bufs, sizes, gbits) : set_kq_batch_k6_cm2s_cls(bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.q40)) {
        if (tc == 256l) {
            return set_kq_batch_q40_cm2l_cls(bufs, sizes, gbits)
        }
        return tc == 128l ? set_kq_batch_q40_cm2m_cls(bufs, sizes, gbits) : set_kq_batch_q40_cm2s_cls(bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.iq4xs)) {
        if (tc == 256l) {
            return set_kq_batch_iq4xs_cm2l_cls(bufs, sizes, gbits)
        }
        return tc == 128l ? set_kq_batch_iq4xs_cm2m_cls(bufs, sizes, gbits) : set_kq_batch_iq4xs_cm2s_cls(bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.k3)) {
        if (tc == 256l) {
            return set_kq_batch_k3_cm2l_cls(bufs, sizes, gbits)
        }
        return tc == 128l ? set_kq_batch_k3_cm2m_cls(bufs, sizes, gbits) : set_kq_batch_k3_cm2s_cls(bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.iq3s)) {
        if (tc == 256l) {
            return set_kq_batch_iq3s_cm2l_cls(bufs, sizes, gbits)
        }
        return tc == 128l ? set_kq_batch_iq3s_cm2m_cls(bufs, sizes, gbits) : set_kq_batch_iq3s_cm2s_cls(bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.iq3xxs)) {
        if (tc == 256l) {
            return set_kq_batch_iq3xxs_cm2l_cls(bufs, sizes, gbits)
        }
        return tc == 128l ? set_kq_batch_iq3xxs_cm2m_cls(bufs, sizes, gbits) : set_kq_batch_iq3xxs_cm2s_cls(bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.iq4nl)) {
        if (tc == 256l) {
            return set_kq_batch_iq4nl_cm2l_cls(bufs, sizes, gbits)
        }
        return tc == 128l ? set_kq_batch_iq4nl_cm2m_cls(bufs, sizes, gbits) : set_kq_batch_iq4nl_cm2s_cls(bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.k2)) {
        if (tc == 256l) {
            return set_kq_batch_k2_cm2l_cls(bufs, sizes, gbits)
        }
        return tc == 128l ? set_kq_batch_k2_cm2m_cls(bufs, sizes, gbits) : set_kq_batch_k2_cm2s_cls(bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.iq2s)) {
        if (tc == 256l) {
            return set_kq_batch_iq2s_cm2l_cls(bufs, sizes, gbits)
        }
        return tc == 128l ? set_kq_batch_iq2s_cm2m_cls(bufs, sizes, gbits) : set_kq_batch_iq2s_cm2s_cls(bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.iq2xs)) {
        if (tc == 256l) {
            return set_kq_batch_iq2xs_cm2l_cls(bufs, sizes, gbits)
        }
        return tc == 128l ? set_kq_batch_iq2xs_cm2m_cls(bufs, sizes, gbits) : set_kq_batch_iq2xs_cm2s_cls(bufs, sizes, gbits)
    }
    if (fmt == int(KqFmt.iq2xxs)) {
        if (tc == 256l) {
            return set_kq_batch_iq2xxs_cm2l_cls(bufs, sizes, gbits)
        }
        return tc == 128l ? set_kq_batch_iq2xxs_cm2m_cls(bufs, sizes, gbits) : set_kq_batch_iq2xxs_cm2s_cls(bufs, sizes, gbits)
    }
    verify(fmt == int(KqFmt.q8), "vk prefill: the cm2 set fell through for a kq format - a format the feed admits must own a tile arm")
    if (tc == 256l) {
        return set_q8_batch_cm2l_cls(bufs, sizes, gbits)
    }
    return tc == 128l ? set_q8_batch_cm2m_cls(bufs, sizes, gbits) : set_q8_batch_cm2s_cls(bufs, sizes, gbits)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def cm2_cls_enc(fmt : int; tc : int64; raw : VkCommandBuffer; var h : VkHaz; var s : VkDescriptorSet; var pc : BatchArgs; groups : int64) {
    verify(tc == 256l || tc == 128l || tc == 32l || tc == CM2_TC_E, "vk prefill: a cm2 tile column the pick never returns")
    if (g_gpu.coopmat_mode == COOPMAT_MM) {
        khr_cls_enc(fmt, raw, h, s, pc, groups)
    } elif (tc == CM2_TC_E) {
        cm2e_cls_enc(fmt, raw, h, s, pc, groups)
    } elif (fmt == int(KqFmt.k4)) {
        if (tc == 256l) {
            enc_kq_batch_k4_cm2l_cls(raw, h, s, pc, groups)
        } elif (tc == 128l) {
            enc_kq_batch_k4_cm2m_cls(raw, h, s, pc, groups)
        } else {
            enc_kq_batch_k4_cm2s_cls(raw, h, s, pc, groups)
        }
    } elif (fmt == int(KqFmt.k5)) {
        if (tc == 256l) {
            enc_kq_batch_k5_cm2l_cls(raw, h, s, pc, groups)
        } elif (tc == 128l) {
            enc_kq_batch_k5_cm2m_cls(raw, h, s, pc, groups)
        } else {
            enc_kq_batch_k5_cm2s_cls(raw, h, s, pc, groups)
        }
    } elif (fmt == int(KqFmt.k6)) {
        if (tc == 256l) {
            enc_kq_batch_k6_cm2l_cls(raw, h, s, pc, groups)
        } elif (tc == 128l) {
            enc_kq_batch_k6_cm2m_cls(raw, h, s, pc, groups)
        } else {
            enc_kq_batch_k6_cm2s_cls(raw, h, s, pc, groups)
        }
    } elif (fmt == int(KqFmt.q40)) {
        if (tc == 256l) {
            enc_kq_batch_q40_cm2l_cls(raw, h, s, pc, groups)
        } elif (tc == 128l) {
            enc_kq_batch_q40_cm2m_cls(raw, h, s, pc, groups)
        } else {
            enc_kq_batch_q40_cm2s_cls(raw, h, s, pc, groups)
        }
    } elif (fmt == int(KqFmt.iq4xs)) {
        if (tc == 256l) {
            enc_kq_batch_iq4xs_cm2l_cls(raw, h, s, pc, groups)
        } elif (tc == 128l) {
            enc_kq_batch_iq4xs_cm2m_cls(raw, h, s, pc, groups)
        } else {
            enc_kq_batch_iq4xs_cm2s_cls(raw, h, s, pc, groups)
        }
    } elif (fmt == int(KqFmt.k3)) {
        if (tc == 256l) {
            enc_kq_batch_k3_cm2l_cls(raw, h, s, pc, groups)
        } elif (tc == 128l) {
            enc_kq_batch_k3_cm2m_cls(raw, h, s, pc, groups)
        } else {
            enc_kq_batch_k3_cm2s_cls(raw, h, s, pc, groups)
        }
    } elif (fmt == int(KqFmt.iq3s)) {
        if (tc == 256l) {
            enc_kq_batch_iq3s_cm2l_cls(raw, h, s, pc, groups)
        } elif (tc == 128l) {
            enc_kq_batch_iq3s_cm2m_cls(raw, h, s, pc, groups)
        } else {
            enc_kq_batch_iq3s_cm2s_cls(raw, h, s, pc, groups)
        }
    } elif (fmt == int(KqFmt.iq3xxs)) {
        if (tc == 256l) {
            enc_kq_batch_iq3xxs_cm2l_cls(raw, h, s, pc, groups)
        } elif (tc == 128l) {
            enc_kq_batch_iq3xxs_cm2m_cls(raw, h, s, pc, groups)
        } else {
            enc_kq_batch_iq3xxs_cm2s_cls(raw, h, s, pc, groups)
        }
    } elif (fmt == int(KqFmt.iq4nl)) {
        if (tc == 256l) {
            enc_kq_batch_iq4nl_cm2l_cls(raw, h, s, pc, groups)
        } elif (tc == 128l) {
            enc_kq_batch_iq4nl_cm2m_cls(raw, h, s, pc, groups)
        } else {
            enc_kq_batch_iq4nl_cm2s_cls(raw, h, s, pc, groups)
        }
    } elif (fmt == int(KqFmt.k2)) {
        if (tc == 256l) {
            enc_kq_batch_k2_cm2l_cls(raw, h, s, pc, groups)
        } elif (tc == 128l) {
            enc_kq_batch_k2_cm2m_cls(raw, h, s, pc, groups)
        } else {
            enc_kq_batch_k2_cm2s_cls(raw, h, s, pc, groups)
        }
    } elif (fmt == int(KqFmt.iq2s)) {
        if (tc == 256l) {
            enc_kq_batch_iq2s_cm2l_cls(raw, h, s, pc, groups)
        } elif (tc == 128l) {
            enc_kq_batch_iq2s_cm2m_cls(raw, h, s, pc, groups)
        } else {
            enc_kq_batch_iq2s_cm2s_cls(raw, h, s, pc, groups)
        }
    } elif (fmt == int(KqFmt.iq2xs)) {
        if (tc == 256l) {
            enc_kq_batch_iq2xs_cm2l_cls(raw, h, s, pc, groups)
        } elif (tc == 128l) {
            enc_kq_batch_iq2xs_cm2m_cls(raw, h, s, pc, groups)
        } else {
            enc_kq_batch_iq2xs_cm2s_cls(raw, h, s, pc, groups)
        }
    } elif (fmt == int(KqFmt.iq2xxs)) {
        if (tc == 256l) {
            enc_kq_batch_iq2xxs_cm2l_cls(raw, h, s, pc, groups)
        } elif (tc == 128l) {
            enc_kq_batch_iq2xxs_cm2m_cls(raw, h, s, pc, groups)
        } else {
            enc_kq_batch_iq2xxs_cm2s_cls(raw, h, s, pc, groups)
        }
    } elif (tc == 256l) {
        verify(fmt == int(KqFmt.q8), "vk prefill: the cm2 enc fell through for a kq format - a format the feed admits must own a tile arm")
        enc_q8_batch_cm2l_cls(raw, h, s, pc, groups)
    } elif (tc == 128l) {
        verify(fmt == int(KqFmt.q8), "vk prefill: the cm2 enc fell through for a kq format - a format the feed admits must own a tile arm")
        enc_q8_batch_cm2m_cls(raw, h, s, pc, groups)
    } else {
        verify(fmt == int(KqFmt.q8), "vk prefill: the cm2 enc fell through for a kq format - a format the feed admits must own a tile arm")
        enc_q8_batch_cm2s_cls(raw, h, s, pc, groups)
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
def private pf_kv_merged(l, dim, kvd : int64) : bool {
    if (!g_env_vulkan.vk_kv_merge || g_rd.layers[l].fq != int(KqFmt.q8)
            || g_rd.layers[l].fk != int(KqFmt.q8) || g_rd.layers[l].fv != int(KqFmt.q8)) {
        return false
    }
    return g_rd.layers[l].bv == g_rd.layers[l].bk + kvd * (dim / 32l)
}

//! Test hook: false runs every layer's FFN over the whole window (the slice's A/B arm)
var g_pf_ffn_slice = true

//! The first row of a layer's FFN dispatches: the last layer's, on the f16-fed cm2 route, starts 32 rows below the window's end
[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
def pf_ffn_slice_row0(l, n_layers, wlen : int64; f16_gate, f16_down : bool) : int64 {
    return g_pf_ffn_slice && g_env_vulkan.vk_ffn_slice && l + 1l == n_layers && f16_gate && f16_down ? max(wlen - 32l, 0l) : 0l
}

//! A cm2 role's tile column and split pick; a region starting above row 0 never splits
[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
def pf_split_pick(fmt : int; d, rows, n, row0, sib_tiles : int64) : tuple<tc : int64; nsplit : int64; ksplit : int64> {
    let tc = batch_tile_edges(fmt, d, rows, true).tc
    if (row0 != 0l) {
        return (tc = tc, nsplit = 1l, ksplit = 0l)
    }
    let sp = cm2_split_k(d, rows, n, tc, sib_tiles)
    return (tc = tc, nsplit = sp.nsplit, ksplit = sp.ksplit)
}

// one prefill GEMM role on the class rail: schedule fill + a per-(role, variant) cached set +
// BatchArgs push. kq formats take the kq tile family, else the q8 router (whose variant moves
// with the dispatched row count — the last window is shorter, the last layer's FFN is sliced).
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice"), arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
def private pf_gemm_enc(raw : VkCommandBuffer; var h : VkHaz; idx : int; fmt : int;
                        wq, ws, ab, sb, yb : uint64; wqn, wsn, an, sn, yn : int64;
                        abit, ybit : uint; n, d, blk, rows : int64; f16feed : bool; sib_tiles : int64 = 0l; row0 : int64 = 0l) {
    // the tile pick and the split pick — computed once, fed to fill AND enc
    var nsplit = 1l
    var ksplit = 0l
    var cm2_tc = 0l
    if (f16feed) {
        let sp = pf_split_pick(fmt, d, rows, n, row0, sib_tiles)
        cm2_tc = sp.tc
        nsplit = sp.nsplit
        ksplit = sp.ksplit
    }
    let groups = int64(fill_arena_batch_sched(g_rd.pf_meta[idx], d, blk, rows, fmt, f16feed, nsplit, row0))
    var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u, ksplit = nsplit > 1l ? uint(ksplit) : 0u)
    if (f16feed) {
        // native fmt-0 decode-in-load; a split pick reroutes the GEMM into the scratch planes
        let split = nsplit > 1l
        if (split) {
            verify(nsplit * rows * d * 4l <= g_rd.pf_splitk_bytes, "vk prefill: split-k planes exceed the scratch bound")
        }
        let yob = split ? g_rd.pf_splitk : yb
        let yon = split ? g_rd.pf_splitk_bytes : yn
        let yog = split ? VHZ_SK : ybit
        let key = idx * 16 + (cm2_tc == 256l ? (split ? 9 : 8) : (cm2_tc == 128l ? (split ? 11 : 10) : (split ? 14 : 13)))
        if (!key_exists(g_rd.pf_cls_gemm, key)) {
            verify(cm2_cls_ensure(fmt, cm2_tc), "vk prefill: the cm2 tile class rail must engage")
            g_rd.pf_cls_gemm[key] = cm2_cls_set(fmt, cm2_tc,
                fixed_array(wq, ws, g_rd.pf_meta[idx].buf, ab, yob),
                fixed_array(wqn, wsn, BATCH_META_BYTES, an, yon),
                fixed_array(0u, 0u, 0u, abit, yog))
        }
        cm2_cls_enc(fmt, cm2_tc, raw, h, g_rd.pf_cls_gemm[key], pc, groups)
        if (split) {
            let rkey = idx * 16 + 12
            if (!key_exists(g_rd.pf_cls_gemm, rkey)) {
                verify(ensure_splitk_reduce_cls(), "vk prefill: the split-k reduce class rail must engage")
                g_rd.pf_cls_gemm[rkey] = set_splitk_reduce_cls(
                    fixed_array(g_rd.pf_splitk, yb),
                    fixed_array(g_rd.pf_splitk_bytes, yn),
                    fixed_array(VHZ_SK, ybit))
            }
            var pcr = SkRedArgs(nelem = uint(rows * d), k = uint(nsplit))
            enc_splitk_reduce_cls(raw, h, g_rd.pf_cls_gemm[rkey], pcr, (rows * d + 1_023l) / 1_024l)
        }
    } elif (kq_sb(fmt)) {
        // the role's format is static, so the slot never collides with the q8 router's variants
        let q40cm = fmt == int(KqFmt.q40) && g_gpu.coopmat_mode == COOPMAT_F16
        let key = idx * 16 + 6
        if (!key_exists(g_rd.pf_cls_gemm, key)) {
            verify(kq_batch_cls_ensure(fmt, q40cm), "vk prefill: the kq batch class rail must engage")
            g_rd.pf_cls_gemm[key] = kq_batch_cls_set_for(fmt, q40cm,
                fixed_array(wq, ws, g_rd.pf_meta[idx].buf, ab, sb, yb),
                fixed_array(wqn, wsn, BATCH_META_BYTES, an, sn, yn),
                fixed_array(0u, 0u, 0u, abit, abit, ybit))
        }
        kq_batch_cls_enc_for(fmt, q40cm, raw, h, g_rd.pf_cls_gemm[key], pc, groups)
    } else {
        let v = q8_batch_cls_variant(q8_batch_tile_for(d, rows), q8_batch_aligned(d, rows))
        let key = idx * 16 + v
        if (!key_exists(g_rd.pf_cls_gemm, key)) {
            verify(q8_batch_cls_ensure(v), "vk prefill: the q8 batch class rail must engage")
            g_rd.pf_cls_gemm[key] = q8_batch_cls_set(v,
                fixed_array(wq, ws, g_rd.pf_meta[idx].buf, ab, sb, yb),
                fixed_array(wqn, wsn, BATCH_META_BYTES, an, sn, yn),
                fixed_array(0u, 0u, 0u, abit, abit, ybit))
        }
        q8_batch_cls_enc(v, raw, h, g_rd.pf_cls_gemm[key], pc, groups)
    }
    pfq_ts(raw)
}

//! the small f16 GEMM route serves: the shape admits it and the split-k scratch holds a whole window's partial planes at the widest row it takes (the router's, the beta/alpha pair's)
[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def private pf_f16g_on() : bool => (g_rd.pf_splitk != 0ul && f16g_shape_ok(g_rd.dim)
    && F16G_SPLIT * PF_WINDOW * max(g_rd.ner, 2l * g_rd.dn_nvh) * 4l <= g_rd.pf_splitk_bytes)

//! one small f16 GEMM: y[rows x d] at ybase = x[rows x n] w[d x n]^T over the weight rows at woff, through the split-k planes
[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def private pf_f16g_enc(raw : VkCommandBuffer; var h : VkHaz; var gset, rset : VkDescriptorSet; n, d, rows, woff, ybase : int64) {
    verify(F16G_SPLIT * rows * d * 4l <= g_rd.pf_splitk_bytes, "vk prefill: the f16 GEMM's partial planes exceed the split-k scratch")
    var pc = F16GemmArgs(n = uint(n), d = uint(d), rows = uint(rows), woff = uint(woff), ybase = 0u, ystride = uint(d), ksplit = uint(n / F16G_SPLIT))
    enc_f16_gemm_cm2_cls(raw, h, gset, pc, ((rows + 127l) / 128l) * ((d + 127l) / 128l) * F16G_SPLIT)
    var pcr = SkRedArgs(nelem = uint(rows * d), k = uint(F16G_SPLIT), ybase = uint(ybase))
    enc_splitk_reduce_cls(raw, h, rset, pcr, (rows * d + 1_023l) / 1_024l)
}

//! one recurrent layer's prefill window sets; the o feed sets (`pf_dnof_set` / `pf_dnrq_set`) are shared by every recurrent layer
[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-dn-block")]
def private pf_dn_layer_sets(l, dim, cd, di : int64) {
    var L & = unsafe(g_rd.layers[l])
    let np = g_rd.pf_np
    L.pf_dn_conv_set = set_dn_conv_cls(
        fixed_array(g_rd.pf_dnqkv, g_rd.pf_dnconv, L.dn.smalls_dev),
        fixed_array(np * cd * 4l, np * cd * 4l, DND_SMALLS_BYTES),
        fixed_array(VHZ_DNP, VHZ_DNC, VHZ_DNSM))
    L.pf_dn_scan_set = set_dn_scan_cls(
        fixed_array(g_rd.pf_dnconv, L.dn.state_dev, L.dn.smalls_dev, g_gpu.dn_ws_dev),
        fixed_array(np * cd * 4l, L.dn.state_bytes, DND_SMALLS_BYTES, DN_WS_BYTES),
        fixed_array(VHZ_DNC, VHZ_DNST, VHZ_DNSM, VHZ_DNWS))
    L.pf_dn_scan_p3_set = set_dn_scan_p3_cls(
        fixed_array(g_rd.pf_dnz, g_gpu.dn_ws_dev, L.dn.smalls_dev),
        fixed_array(np * di * 4l, DN_WS_BYTES, DND_SMALLS_BYTES),
        fixed_array(VHZ_DNZ, VHZ_DNWS, VHZ_DNSM))
    L.pf_dn_tail_set = set_dn_tail_cls(
        fixed_array(g_rd.pf_dnqkv, L.dn.smalls_dev),
        fixed_array(np * cd * 4l, DND_SMALLS_BYTES),
        fixed_array(VHZ_DNP, VHZ_DNSM))
    if (g_rd.dn_ba_f32) {
        L.pf_dn_ba_set = set_dn_ba_cls(
            fixed_array(g_rd.dn_ba_dev, g_rd.pf_xb, L.dn.smalls_dev),
            fixed_array(g_rd.dn_ba_bytes, np * dim * 4l, DND_SMALLS_BYTES),
            fixed_array(0u, VHZ_XB, VHZ_DNSM))
    }
    if (pf_dnx6(l)) {
        L.pf_dn_cvt_set = set_f16cvt_cls(fixed_array(g_rd.pf_xb, g_rd.pf_xf),
            fixed_array(np * dim * 4l, np * dim * 2l), fixed_array(VHZ_XB, VHZ_XQ))
    }
    if (g_rd.dn_ba_f32 && pf_dnx6(l) && pf_f16g_on()) {
        L.pf_f16g_ba_set = set_f16_gemm_cm2_cls(fixed_array(g_rd.dn_ba_dev, g_rd.pf_xf, g_rd.pf_splitk),
            fixed_array(g_rd.dn_ba_bytes, np * dim * 2l, g_rd.pf_splitk_bytes), fixed_array(0u, VHZ_XQ, VHZ_SK))
        L.pf_f16g_ba_red_set = set_splitk_reduce_cls(fixed_array(g_rd.pf_splitk, L.dn.smalls_dev),
            fixed_array(g_rd.pf_splitk_bytes, DND_SMALLS_BYTES), fixed_array(VHZ_SK, VHZ_DNSM))
    }
}

def private pf_dn_zero(raw : VkCommandBuffer; var h : VkHaz) {
    let hist_bytes = g_rd.dn_cd * (g_rd.dn_dconv - 1l) * 4l
    vhz_dep(raw, h, 0u, VHZ_DNST | VHZ_DNSM, true)
    for (L in g_rd.layers) {
        if (L.recurrent) {
            cmd_fill_zero(raw, L.dn.state_dev, 0l, L.dn.state_bytes)
            cmd_fill_zero(raw, L.dn.smalls_dev, DN_SM_HIST * 4l, hist_bytes)
        }
    }
}

[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-dn-block"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def private rd_pf_recurrent(raw : VkCommandBuffer; var h : VkHaz; l : int64; b : int; wlen : int64; last : bool) {
    var L & = unsafe(g_rd.layers[l])
    let dim = g_rd.dim
    let cd = g_rd.dn_cd
    let kd = int(g_rd.dn_kd)
    let di = g_rd.dn_di
    let nvh = g_rd.dn_nvh
    let nkh = int(g_rd.dn_nkh)
    let ds = g_rd.dn_ds
    let dconv = g_rd.dn_dconv
    let taps = dconv - 1l
    let np = g_rd.pf_np
    let eps = g_rd.eps
    let x6 = pf_dnx6(l)
    let xq8k = !x6 && kq_sb(L.fqkv)
    if (x6) {
        var pcc0 = ActArgs(nelem = uint(wlen * dim), gelu = 0u, nblk = 0u)
        enc_f16cvt_cls(raw, h, L.pf_dn_cvt_set, pcc0, (wlen * dim / 4l + 255l) / 256l)
    }
    if (xq8k) {
        var pc0 = RqArgs(inbase = 0u, nblk = uint(wlen * dim / 256l))
        enc_cls_q8k_rq(raw, h, g_rd.pf_sets[b + 0], pc0, (wlen * dim / 8l + 255l) / 256l)
    } elif (!x6 || !g_rd.dn_ba_f32) {
        var pc0 = RqArgs(inbase = 0u, nblk = uint(wlen * dim / 32l))
        enc_cls_dn_rq(raw, h, g_rd.pf_sets[b + 0], pc0, (wlen * dim / 4l + 255l) / 256l)
    }
    pfq_ts(raw)
    let xqb = x6 ? g_rd.pf_xf : g_rd.pf_xq
    let xsb = x6 ? g_rd.dummy : g_rd.pf_xs
    let xqn = x6 ? np * dim * 2l : np * dim
    let xsn = x6 ? 256l : np * (dim / 32l) * 4l
    let pqkv = arena_planes(L.fqkv, L.bqkv)
    pf_gemm_enc(raw, h, b + 1, L.fqkv, pqkv.wq, pqkv.ws, xqb, xsb, g_rd.pf_dnqkv,
        pqkv.wqb, pqkv.wsb, xqn, xsn, np * cd * 4l, VHZ_XQ, VHZ_DNP, dim, cd, L.bqkv, wlen, x6, cm2_tiles(di, wlen))
    let pz = arena_planes(L.fz, L.bz)
    pf_gemm_enc(raw, h, b + 2, L.fz, pz.wq, pz.ws, xqb, xsb, g_rd.pf_dnz,
        pz.wqb, pz.wsb, xqn, xsn, np * di * 4l, VHZ_XQ, VHZ_DNZ, dim, di, L.bz, wlen, x6, cm2_tiles(cd, wlen))
    if (g_rd.dn_ba_f32) {
        if (x6 && pf_f16g_on()) {
            pf_f16g_enc(raw, h, L.pf_f16g_ba_set, L.pf_f16g_ba_red_set, dim, nvh, wlen, L.ba_off, DN_SM_BETA)
            pf_f16g_enc(raw, h, L.pf_f16g_ba_set, L.pf_f16g_ba_red_set, dim, nvh, wlen, L.ba_off + nvh * dim, DN_SM_G)
        } else {
            var pcb = DnBaArgs(npos = uint(wlen), dim = uint(dim), nvh = uint(nvh), woff = uint(L.ba_off), xstride = uint(dim),
                obase_b = uint(DN_SM_BETA), obase_g = uint(DN_SM_G), ostride = uint(nvh))
            enc_dn_ba_cls(raw, h, L.pf_dn_ba_set, pcb, dn_ba_wgs(wlen, nvh))
        }
        pfq_ts(raw)
    } else {
        if (xq8k) {   // the qkv/z GEMMs read the rows as Q8_K; the q8 beta/alpha GEMMs read them as Q8_0
            var pcq = RqArgs(inbase = 0u, nblk = uint(wlen * dim / 32l))
            enc_cls_dn_rq(raw, h, g_rd.pf_sets[b + 0], pcq, (wlen * dim / 4l + 255l) / 256l)
        }
        let pb = arena_planes(int(KqFmt.q8), L.bbeta)
        pf_gemm_enc(raw, h, b + 3, int(KqFmt.q8), pb.wq, pb.ws, g_rd.pf_xq, g_rd.pf_xs, g_rd.pf_dnbg,
            pb.wqb, pb.wsb, np * dim, np * (dim / 32l) * 4l, np * nvh * 4l, VHZ_XQ, VHZ_DNBG, dim, nvh, L.bbeta, wlen, false)
        vhz_dep(raw, h, VHZ_DNBG, VHZ_DNSM, true)
        cmd_copy_range(raw, g_rd.pf_dnbg, 0l, L.dn.smalls_dev, DN_SM_BETA * 4l, wlen * nvh * 4l)
        let pa = arena_planes(int(KqFmt.q8), L.balpha)
        pf_gemm_enc(raw, h, b + 9, int(KqFmt.q8), pa.wq, pa.ws, g_rd.pf_xq, g_rd.pf_xs, g_rd.pf_dnbg,
            pa.wqb, pa.wsb, np * dim, np * (dim / 32l) * 4l, np * nvh * 4l, VHZ_XQ, VHZ_DNBG, dim, nvh, L.balpha, wlen, false)
        vhz_dep(raw, h, VHZ_DNBG, VHZ_DNSM, true)
        cmd_copy_range(raw, g_rd.pf_dnbg, 0l, L.dn.smalls_dev, DN_SM_G * 4l, wlen * nvh * 4l)
    }
    var pcc = DnConvArgs(npos = uint(wlen), cd = uint(cd), kd = uint(kd), ds = uint(ds), dconv = uint(dconv),
        hist_mode = 2u, taps_off = uint(DN_SM_TAPS), hist_off = uint(DN_SM_HIST),
        eps = eps, qscale = 1.0 / sqrt(float(ds)))
    enc_dn_conv_cls(raw, h, L.pf_dn_conv_set, pcc, dn_conv_wgs(cd, wlen))
    pfq_ts(raw)
    var pcs = DnScanArgs(npos = uint(wlen), cd = uint(cd), kd = uint(kd), nvh = uint(nvh), nkh = uint(nkh), ds = uint(ds),
        beta_off = uint(DN_SM_BETA), g_off = uint(DN_SM_G), a_off = uint(DN_SM_A), dt_off = uint(DN_SM_DT),
        o_off = uint(DN_O_OFF), di = uint(di))
    enc_dn_scan_cls(raw, h, L.pf_dn_scan_set, pcs, dn_scan_wgs(nvh, ds))
    pfq_ts(raw)
    var pc3 = DnScanP3Args(npos = uint(wlen), nvh = uint(nvh), di = uint(di), ds = uint(ds), o_off = uint(DN_O_OFF),
        wnorm_off = uint(DN_SM_WNORM), eps = eps)
    enc_dn_scan_p3_cls(raw, h, L.pf_dn_scan_p3_set, pc3, wlen)
    pfq_ts(raw)
    if (last) {
        // a window shorter than the taps is window 0 alone, whose history is zero: the ring's leading rows read as zero
        var pct = DnTailArgs(cd = uint(cd), taps = uint(taps), row0 = uint(max(wlen - taps, 0l)), hist_off = uint(DN_SM_HIST),
            zero_rows = uint(max(taps - wlen, 0l)))
        vhz_dep(raw, h, VHZ_DNP, VHZ_DNSM, false)
        enc_dn_tail_cls(raw, h, L.pf_dn_tail_set, pct, (cd * taps + int64(WG_X) - 1l) / int64(WG_X))
    } else {
        vhz_dep(raw, h, VHZ_DNP, VHZ_DNSM, true)
        cmd_copy_range(raw, g_rd.pf_dnqkv, (wlen - taps) * cd * 4l, L.dn.smalls_dev, DN_SM_HIST * 4l, taps * cd * 4l)
    }
    pfq_ts(raw)
    let o6 = pf_dno6(l)
    if (o6) {
        var pco = ActArgs(nelem = uint(wlen * di), gelu = 0u, nblk = 0u)
        enc_f16cvt_cls(raw, h, g_rd.pf_dnof_set, pco, (wlen * di / 4l + 255l) / 256l)
    } elif (kq_sb(L.fout)) {
        var pcr = RqArgs(inbase = uint(DN_O_OFF), nblk = uint(wlen * di / 256l))
        enc_cls_q8k_rq(raw, h, g_rd.pf_dnrq_set, pcr, (wlen * di / 8l + 255l) / 256l)
    } else {
        var pcr = RqArgs(inbase = uint(DN_O_OFF), nblk = uint(wlen * di / 32l))
        enc_cls_dn_rq(raw, h, g_rd.pf_dnrq_set, pcr, (wlen * di / 4l + 255l) / 256l)
    }
    pfq_ts(raw)
    let pout = arena_planes(L.fout, L.bout)
    pf_gemm_enc(raw, h, b + 7, L.fout, pout.wq, pout.ws, o6 ? g_rd.pf_dnof : g_gpu.hq_dev, o6 ? g_rd.dummy : g_gpu.hs_dev, g_rd.pf_xb2,
        pout.wqb, pout.wsb, o6 ? np * di * 2l : BATCH_HQ_BYTES, o6 ? 256l : BATCH_HS_BYTES, np * dim * 4l, VHZ_HQ, VHZ_XB2, di, dim, L.bout, wlen, o6)
}

// the prefill profile's stamps per layer: the attention head's roles (+ qkn), or the recurrent head's
// (rq_x qkv z [beta alpha | ba] conv scan onorm tail rq_o out), then the shared FFN tail's seven; an MoE
// layer adds the routed block's eight and drops the dense block's four when it has no shared expert
def private pf_roles_per_layer(L : RLayer) : int64 {
    var n = L.recurrent ? 7l + (g_rd.dn_ba_f32 ? 10l : 11l) : (g_rd.qk_norm ? 16l : 15l)
    if (L.moe) {
        n += PF_MOE_ROLES - (has_dense_triple(L) ? 0l : 4l)
    }
    return n
}

//! the routed block's stamps of an MoE layer's tail past ar1 / rq_f at `m0`: the shared expert's four when the layer has one, then the block's eight and ar2
def private pf_moe_tail_text(acc : double[26]; m0 : int; shexp : bool) : string {
    let s = shexp ? "sh_gate {int(acc[m0])} sh_up {int(acc[m0 + 1])} sh_act {int(acc[m0 + 2])} sh_down {int(acc[m0 + 3])} " : ""
    let r = m0 + (shexp ? 4 : 0)
    return "{s}router {int(acc[r])} topk {int(acc[r + 1])} sched {int(acc[r + 2])} gather {int(acc[r + 3])} e_gate {int(acc[r + 4])} e_up {int(acc[r + 5])} e_act {int(acc[r + 6])} e_down {int(acc[r + 7])} ar2+comb {int(acc[r + 8])}"
}

let private PF_MOE_ROLES = 8l   //! router topk sched gather e_gate e_up e_act e_down (the combine rides the layer's ar2 stamp)
let private PF_MOE_MAP_OFF = 2_048l   //! the schedules' first per-wg map word offset: 4-word records for up to 256 experts, the s pieces' then the m pieces'

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def vk_rdec_moe_ok(f1, f3, f2 : int) : bool {
    if (!vk_moe_init() || !g_gpu.has_coopmat2 || g_gpu.coopmat_mode != COOPMAT_CM2) {
        return false
    }
    return pf_f16_feed(f1) && pf_f16_feed(f3) && pf_f16_feed(f2)
}

//! the expert tiles' dispatch bounds for a window of `nk` slot rows: the s dispatch takes at most
//! one tile per expert (a bucket within the s column, or the remainder past whole m columns), the
//! m dispatch every expert's whole columns plus a partial one
def private pf_moe_bounds(nk : int64) : tuple<gu : int64; dn : int64; mgu : int64; mdn : int64> {
    let wt_gu = (g_rd.nfe + 127l) / 128l
    let wt_dn = (g_rd.dim + 127l) / 128l
    let rt_m = nk / int64(SCHED_M_ROWS) + g_rd.ne
    return (gu = g_rd.ne * wt_gu, dn = g_rd.ne * wt_dn, mgu = rt_m * wt_gu, mdn = rt_m * wt_dn)
}

//! the window chain's MoE block buffers and class rails, once per residency
[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def private pf_moe_setup {
    let np = g_rd.pf_np
    let dim = g_rd.dim
    let nfe = g_rd.nfe
    let nk = np * g_rd.k
    g_rd.pf_nk = nk
    verify(ensure_router_gemm_cls() && ensure_topk_rows_cls() && ensure_moe_sched_cls() && ensure_moe_gather_f16_cls()
        && ensure_actf16_cls() && ensure_cls_ar_comb() && ensure_cls_ar_comb_f16_b(),
        "vk prefill: the MoE block's class rails must engage on a live device")
    let bounds = pf_moe_bounds(nk)
    assert((PF_MOE_MAP_OFF + max(bounds.gu + bounds.mgu, bounds.dn + bounds.mdn)) * 4l <= BATCH_META_BYTES, "dasLLAMA vulkan tier: the MoE block's schedule exceeds its plane")
    g_rd.pf_mlog = make_device_buf(np * g_rd.ner * 4l)
    g_rd.pf_midx = make_device_buf(nk * 4l)
    g_rd.pf_mw = make_device_buf(nk * 4l)
    g_rd.pf_minv = make_device_buf(nk * 4l)
    g_rd.pf_msch1 = make_device_buf(BATCH_META_BYTES)
    g_rd.pf_msch3 = make_device_buf(BATCH_META_BYTES)
    g_rd.pf_msch2 = make_device_buf(BATCH_META_BYTES)
    g_rd.pf_mxg = make_device_buf((nk + TILE_READ_SLACK) * dim * 2l)
    g_rd.pf_my1 = make_device_buf(nk * nfe * 4l)
    g_rd.pf_my2 = make_device_buf(nk * nfe * 4l)
    g_rd.pf_mhf = make_device_buf((nk + TILE_READ_SLACK) * nfe * 2l)
    g_rd.pf_mdn = make_device_buf(nk * dim * 4l)
    g_rd.pf_midx_host = make_host_buf(nk * 4l, false, [cached = true])
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def private pf_moe_layer_sets(l : int64) {
    var L & = unsafe(g_rd.layers[l])
    let np = g_rd.pf_np
    let dim = g_rd.dim
    let nfe = g_rd.nfe
    let nk = g_rd.pf_nk
    let ner = g_rd.ner
    let tcs = int64(SCHED_S_ROWS)
    let tce = CM2_TC_E
    let xg_bytes = (nk + TILE_READ_SLACK) * dim * 2l
    let y_bytes = nk * nfe * 4l
    let hf_bytes = (nk + TILE_READ_SLACK) * nfe * 2l
    let dn_bytes = nk * dim * 4l
    let pe1 = arena_planes(L.fe1, L.be1)
    let pe3 = arena_planes(L.fe3, L.be3)
    let pe2 = arena_planes(L.fe2, L.be2)
    verify(cm2_cls_ensure(L.fe1, tcs) && cm2_cls_ensure(L.fe3, tcs) && cm2_cls_ensure(L.fe2, tcs)
        && cm2_cls_ensure(L.fe1, tce) && cm2_cls_ensure(L.fe3, tce) && cm2_cls_ensure(L.fe2, tce), "vk prefill: the expert tile class rails must engage")
    L.pf_router_set = set_router_gemm_cls(fixed_array(g_rd.router_dev, g_rd.pf_xb, g_rd.pf_mlog),
        fixed_array(g_rd.router_bytes, np * dim * 4l, np * ner * 4l), fixed_array(0u, VHZ_XB, VHZ_MRT))
    if (pf_f16g_on()) {
        verify(ensure_f16_gemm_cm2_cls() && ensure_splitk_reduce_cls() && ensure_f16cvt_cls(), "vk prefill: the router's f16 GEMM rails must engage")
        L.pf_router_cvt_set = set_f16cvt_cls(fixed_array(g_rd.pf_xb, g_rd.pf_xf),
            fixed_array(np * dim * 4l, np * dim * 2l), fixed_array(VHZ_XB, VHZ_XQ))
        L.pf_f16g_router_set = set_f16_gemm_cm2_cls(fixed_array(g_rd.router16_dev, g_rd.pf_xf, g_rd.pf_splitk),
            fixed_array(g_rd.router16_bytes, np * dim * 2l, g_rd.pf_splitk_bytes), fixed_array(0u, VHZ_XQ, VHZ_SK))
        L.pf_f16g_router_red_set = set_splitk_reduce_cls(fixed_array(g_rd.pf_splitk, g_rd.pf_mlog),
            fixed_array(g_rd.pf_splitk_bytes, np * ner * 4l), fixed_array(VHZ_SK, VHZ_MRT))
    }
    L.pf_topk_set = set_topk_rows_cls(fixed_array(g_rd.pf_mlog, g_rd.pf_midx, g_rd.pf_mw),
        fixed_array(np * ner * 4l, nk * 4l, nk * 4l), fixed_array(VHZ_MRT, VHZ_MRT, VHZ_MRT))
    L.pf_sched_set = set_moe_sched_cls(fixed_array(g_rd.pf_midx, g_rd.pf_minv, g_rd.pf_msch1, g_rd.pf_msch3, g_rd.pf_msch2),
        fixed_array(nk * 4l, nk * 4l, BATCH_META_BYTES, BATCH_META_BYTES, BATCH_META_BYTES),
        fixed_array(VHZ_MRT, VHZ_MRT, VHZ_MRT, VHZ_MRT, VHZ_MRT))
    L.pf_gather_set = set_moe_gather_f16_cls(fixed_array(g_rd.pf_xb, g_rd.pf_minv, g_rd.pf_mxg),
        fixed_array(np * dim * 4l, nk * 4l, xg_bytes), fixed_array(VHZ_XB, VHZ_MRT, VHZ_MXG))
    L.pf_e1_set = cm2_cls_set(L.fe1, tcs, fixed_array(pe1.wq, pe1.ws, g_rd.pf_msch1, g_rd.pf_mxg, g_rd.pf_my1),
        fixed_array(pe1.wqb, pe1.wsb, BATCH_META_BYTES, xg_bytes, y_bytes), fixed_array(0u, 0u, VHZ_MRT, VHZ_MXG, VHZ_GATE))
    L.pf_e1m_set = cm2_cls_set(L.fe1, tce, fixed_array(pe1.wq, pe1.ws, g_rd.pf_msch1, g_rd.pf_mxg, g_rd.pf_my1),
        fixed_array(pe1.wqb, pe1.wsb, BATCH_META_BYTES, xg_bytes, y_bytes), fixed_array(0u, 0u, VHZ_MRT, VHZ_MXG, VHZ_GATE_M))
    L.pf_e3_set = cm2_cls_set(L.fe3, tcs, fixed_array(pe3.wq, pe3.ws, g_rd.pf_msch3, g_rd.pf_mxg, g_rd.pf_my2),
        fixed_array(pe3.wqb, pe3.wsb, BATCH_META_BYTES, xg_bytes, y_bytes), fixed_array(0u, 0u, VHZ_MRT, VHZ_MXG, VHZ_UP))
    L.pf_e3m_set = cm2_cls_set(L.fe3, tce, fixed_array(pe3.wq, pe3.ws, g_rd.pf_msch3, g_rd.pf_mxg, g_rd.pf_my2),
        fixed_array(pe3.wqb, pe3.wsb, BATCH_META_BYTES, xg_bytes, y_bytes), fixed_array(0u, 0u, VHZ_MRT, VHZ_MXG, VHZ_UP_M))
    L.pf_eact_set = set_actf16_cls(fixed_array(g_rd.pf_my1, g_rd.pf_my2, g_rd.pf_mhf),
        fixed_array(y_bytes, y_bytes, hf_bytes), fixed_array(VHZ_GATE | VHZ_GATE_M, VHZ_UP | VHZ_UP_M, VHZ_HQ))
    L.pf_e2_set = cm2_cls_set(L.fe2, tcs, fixed_array(pe2.wq, pe2.ws, g_rd.pf_msch2, g_rd.pf_mhf, g_rd.pf_mdn),
        fixed_array(pe2.wqb, pe2.wsb, BATCH_META_BYTES, hf_bytes, dn_bytes), fixed_array(0u, 0u, VHZ_MRT, VHZ_HQ, VHZ_MDN))
    L.pf_e2m_set = cm2_cls_set(L.fe2, tce, fixed_array(pe2.wq, pe2.ws, g_rd.pf_msch2, g_rd.pf_mhf, g_rd.pf_mdn),
        fixed_array(pe2.wqb, pe2.wsb, BATCH_META_BYTES, hf_bytes, dn_bytes), fixed_array(0u, 0u, VHZ_MRT, VHZ_HQ, VHZ_MDN_M))
    L.pf_arcomb_set = set_cls_ar_comb(fixed_array(g_rd.pf_x, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xb, g_rd.pf_minv, g_rd.pf_mw, g_rd.pf_mdn, g_rd.pf_mlog),
        fixed_array(np * dim * 4l, np * dim * 4l, g_rd.norms_bytes, np * dim * 4l, nk * 4l, nk * 4l, dn_bytes, np * ner * 4l),
        fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XB, VHZ_MRT, VHZ_MRT, VHZ_MDN | VHZ_MDN_M, VHZ_MRT))
    L.pf_arcomb_f16_set = set_cls_ar_comb_f16_b(fixed_array(g_rd.pf_x, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xf, g_rd.pf_minv, g_rd.pf_mw, g_rd.pf_mdn, g_rd.pf_mlog),
        fixed_array(np * dim * 4l, np * dim * 4l, g_rd.norms_bytes, np * dim * 2l, nk * 4l, nk * 4l, dn_bytes, np * ner * 4l),
        fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XQ, VHZ_MRT, VHZ_MRT, VHZ_MDN | VHZ_MDN_M, VHZ_MRT))
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def private pf_moe_block(raw : VkCommandBuffer; var h : VkHaz; l, wlen : int64) {
    var L & = unsafe(g_rd.layers[l])
    let dim = g_rd.dim
    let ne = g_rd.ne   // nolint:LINT021 — the push words take the int64 geometry narrowed
    let k = g_rd.k
    let nfe = g_rd.nfe
    let ner = g_rd.ner
    let nk = wlen * k
    if (pf_f16g_on()) {
        var pcc = ActArgs(nelem = uint(wlen * dim), gelu = 0u, nblk = 0u)
        enc_f16cvt_cls(raw, h, L.pf_router_cvt_set, pcc, (wlen * dim / 4l + 255l) / 256l)
        pf_f16g_enc(raw, h, L.pf_f16g_router_set, L.pf_f16g_router_red_set, dim, ner, wlen, L.router_slot * ner * dim, 0l)
    } else {
        var pcr = RouterGemmArgs(npos = uint(wlen), ne = uint(ner), dim = uint(dim), woff = uint(L.router_slot * ner * dim), ostride = uint(ner))
        enc_router_gemm_cls(raw, h, L.pf_router_set, pcr, ((wlen + int64(ROUTER_TP) - 1l) / int64(ROUTER_TP)) * ((ner + int64(ROUTER_TE) - 1l) / int64(ROUTER_TE)))
    }
    pfq_ts(raw)
    var pct = TopkRowsArgs(npos = uint(wlen), ne = uint(ne), k = uint(k), norm = g_rd.norm_topk ? 1u : 0u, lstride = uint(ner), wscale = g_rd.wscale)
    enc_topk_rows_cls(raw, h, L.pf_topk_set, pct, wlen)
    pfq_ts(raw)
    let bounds = pf_moe_bounds(nk)
    let tcs = int64(SCHED_S_ROWS)
    let tce = CM2_TC_E
    let map_m_gu = uint(PF_MOE_MAP_OFF + bounds.gu)   //! the m dispatch's maps follow the s dispatch's
    let map_m_dn = uint(PF_MOE_MAP_OFF + bounds.dn)
    var pcs = SchedArgs(nk = uint(nk), ne = uint(ne), wt_gu = uint((nfe + 127l) / 128l), wt_dn = uint((dim + 127l) / 128l),
        map_off = uint(PF_MOE_MAP_OFF), map_off_mgu = map_m_gu, map_off_mdn = map_m_dn,
        bound_gu = uint(bounds.gu), bound_dn = uint(bounds.dn), bound_mgu = uint(bounds.mgu), bound_mdn = uint(bounds.mdn),
        base1 = uint(arena_local_blk(L.be1)), stride1 = uint(nfe * dim / fmt_unit(L.fe1)),
        base3 = uint(arena_local_blk(L.be3)), stride3 = uint(nfe * dim / fmt_unit(L.fe3)),
        base2 = uint(arena_local_blk(L.be2)), stride2 = uint(nfe * dim / fmt_unit(L.fe2)))
    enc_moe_sched_cls(raw, h, L.pf_sched_set, pcs, 1l)
    pfq_ts(raw)
    var pcg = GatherArgs(n = uint(dim), k = uint(k), r0 = 0u, r1 = uint(nk))
    enc_moe_gather_f16_cls(raw, h, L.pf_gather_set, pcg, nk)
    pfq_ts(raw)
    var pc1 = BatchArgs(n = uint(dim), d = uint(nfe), map_off = uint(PF_MOE_MAP_OFF), ksplit = 0u)
    var pc1m = BatchArgs(n = uint(dim), d = uint(nfe), map_off = map_m_gu, ksplit = 0u)
    cm2_cls_enc(L.fe1, tce, raw, h, L.pf_e1m_set, pc1m, bounds.mgu)
    cm2_cls_enc(L.fe1, tcs, raw, h, L.pf_e1_set, pc1, bounds.gu)
    pfq_ts(raw)
    cm2_cls_enc(L.fe3, tce, raw, h, L.pf_e3m_set, pc1m, bounds.mgu)
    cm2_cls_enc(L.fe3, tcs, raw, h, L.pf_e3_set, pc1, bounds.gu)
    pfq_ts(raw)
    var pca = ActArgs(nelem = uint(nk * nfe), gelu = g_rd.is_gelu ? 1u : 0u, nblk = uint(nk * nfe / 32l), elem0 = 0u)
    enc_actf16_cls(raw, h, L.pf_eact_set, pca, (nk * nfe / 4l + 255l) / 256l)
    pfq_ts(raw)
    var pc2 = BatchArgs(n = uint(nfe), d = uint(dim), map_off = uint(PF_MOE_MAP_OFF), ksplit = 0u)
    var pc2m = BatchArgs(n = uint(nfe), d = uint(dim), map_off = map_m_dn, ksplit = 0u)
    cm2_cls_enc(L.fe2, tce, raw, h, L.pf_e2m_set, pc2m, bounds.mdn)
    cm2_cls_enc(L.fe2, tcs, raw, h, L.pf_e2_set, pc2, bounds.dn)
    pfq_ts(raw)
}

//! the next layer's fused Q8 head skips its own requant, so this step encodes it.
[arch(at="../ARCHITECTURE_GPU_VULKAN_MOE.md#vk-prefill-moe-block")]
def private pf_moe_addr_next(raw : VkCommandBuffer; var h : VkHaz; l, wlen, nxt : int64) {
    var L & = unsafe(g_rd.layers[l])
    let dim = g_rd.dim
    var pcc = ArArgs(dim = uint(dim), add_on = has_dense_triple(L) ? 1u : 0u, woff = uint(nxt), eps = g_rd.eps, ascale = 1.0,
        slots = uint(g_rd.k), lstride = uint(g_rd.ner), gated = g_rd.sh_gated ? 1u : 0u)
    let next_fused = l + 1l < g_rd.n_layers && pf_qkv_feed_fused(l + 1l)
    if (next_fused && pf_qkv6(l + 1l)) {
        enc_cls_ar_comb_f16_b(raw, h, L.pf_arcomb_f16_set, pcc, wlen)
        return
    }
    enc_cls_ar_comb(raw, h, L.pf_arcomb_set, pcc, wlen)
    if (next_fused) {   //! the Q8 feed's requant, the next layer's own quant_xb set
        let nb = int((l + 1l) * PF_ROLES)
        if (kq_sb(g_rd.layers[l + 1l].fq)) {
            var pcq = RqArgs(inbase = 0u, nblk = uint(wlen * dim / 256l))
            enc_cls_q8k_rq(raw, h, g_rd.pf_sets[nb], pcq, (wlen * dim / 8l + 255l) / 256l)
        } else {
            var pcq = RqArgs(inbase = 0u, nblk = uint(wlen * dim / 32l))
            enc_cls_dn_rq(raw, h, g_rd.pf_sets[nb], pcq, (wlen * dim / 4l + 255l) / 256l)
        }
    }
}

[cold_path, arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]   // once per residency — buffers, sets, fa arming
def private pf_setup {
    if (g_rd.pf_ready) {
        return
    }
    let np = PF_WINDOW
    g_rd.pf_np = np
    let dim = g_rd.dim
    let qd = g_rd.qd
    let kvd = g_rd.kv_dim
    let hid = g_rd.hidden
    let wide = max(max(dim, qd), hid)
    g_rd.pf_x = make_device_buf(np * dim * 4l)
    g_rd.pf_xb = make_device_buf(np * dim * 4l)
    g_rd.pf_xq = make_device_buf(np * wide)
    g_rd.pf_xs = make_device_buf(np * (wide / 32l) * 4l)
    let qrow = g_rd.q_gated ? 2l * qd : qd
    g_rd.pf_q = make_device_buf(np * qrow * 4l)
    g_rd.pf_kv = make_device_buf(np * 2l * kvd * 4l)
    g_rd.pf_v = make_device_buf(np * kvd * 4l)   // split-path v GEMM output (copied into pf_kv's v half); idle when the merged k|v GEMM serves the layer
    g_rd.pf_attn = make_device_buf(np * qd * 4l)
    g_rd.pf_aq = make_device_buf(np * qd)
    g_rd.pf_as = make_device_buf(np * (qd / 32l) * 4l)
    g_rd.pf_gate = make_device_buf(np * hid * 4l)
    g_rd.pf_up = make_device_buf(np * hid * 4l)
    g_rd.pf_ffnout = make_device_buf(np * dim * 4l)
    g_rd.pf_xb2 = make_device_buf(np * dim * 4l)
    g_rd.pf_cos = make_device_buf(np * g_rd.cos_elems * 4l)
    g_rd.pf_logits_host = make_host_buf(g_rd.vocab * 4l, false, [cached = true])
    g_rd.pf_cmd = alloc_cmd()
    if (g_env_vulkan.vk_overlap) {
        let nring = 3l + (g_rd.n_layers + PF_CHUNK_MAX - 1l) / PF_CHUNK_MAX
        g_rd.pf_cmd_ring |> reserve(nring)
        for (_i in range64(nring)) {
            g_rd.pf_cmd_ring |> push(alloc_cmd())
        }
    }
    if (g_rd.emb_blk >= 0l) {
        g_rd.pf_ids = make_device_buf(np * 4l)
        verify(ensure_emb_gather_cls(), "vk prefill: the embed-gather class rail must engage")
        let pe = arena_planes(int(KqFmt.q8), g_rd.emb_blk)
        g_rd.pf_emb_set = set_emb_gather_cls(
            fixed_array(pe.wq, pe.ws, g_rd.pf_ids, g_rd.pf_x),
            fixed_array(pe.wqb, pe.wsb, np * 4l, np * dim * 4l),
            fixed_array(0u, 0u, 0u, VHZ_X))
    } elif (g_rd.emb_f32 != 0ul) {
        g_rd.pf_ids = make_device_buf(np * 4l)
        verify(ensure_emb_gather_f32_cls(), "vk prefill: the f32 embed-gather class rail must engage")
        g_rd.pf_emb_set = set_emb_gather_f32_cls(
            fixed_array(g_rd.emb_f32, g_rd.pf_ids, g_rd.pf_x),
            fixed_array(g_rd.emb_f32_bytes, np * 4l, np * dim * 4l),
            fixed_array(0u, 0u, VHZ_X))
    }
    ensure_batch_state()   // hq_dev/hs_dev for the FFN act
    // f16-fed groups (pf_f16_feed) take f16 convert roles; kq consumers the Q8_K quant/act forms
    var any_f16_feed = false
    var anykq = kq_sb(g_rd.cls_fmt_marker)
    for (L in g_rd.layers) {
        any_f16_feed = (any_f16_feed || pf_f16_feed(L.fq) || pf_f16_feed(L.fo) || pf_f16_feed(L.f1) || pf_f16_feed(L.f2)
            || (L.recurrent && (pf_f16_feed(L.fqkv) || pf_f16_feed(L.fz) || pf_f16_feed(L.fout))))
        anykq = anykq || kq_sb(L.fq) || kq_sb(L.fo) || kq_sb(L.f1) || kq_sb(L.f2) || (L.recurrent && (kq_sb(L.fqkv) || kq_sb(L.fz) || kq_sb(L.fout)))
    }
    if (any_f16_feed) {
        assert(np % 32l == 0l, "dasLLAMA vulkan tier: the s tile's unclamped column needs a 32-row-multiple window")   // the resident planes carry no read slack
    g_rd.pf_xf = make_device_buf(np * dim * 2l)
        g_rd.pf_af = make_device_buf(np * qd * 2l)
        g_rd.pf_hf = make_device_buf(np * hid * 2l)
    }
    // cm2 split-k scratch (one SSBO range): cap at the device range HERE where the size is negotiated
    if (any_f16_feed && g_gpu.coopmat_mode == COOPMAT_CM2) {
        var skb = 262_144l * int64(g_gpu.sm_count)
        let fsk = cm2_splitk_env()
        if (fsk > 1l) {
            skb = max(skb, np * wide * fsk * 4l)
        }
        skb = min(skb, vk_max_storage_range())
        if (skb > 0l) {
            g_rd.pf_splitk = make_device_buf(skb)
            g_rd.pf_splitk_bytes = skb
        }
    }
    let nsets = int(g_rd.n_layers * PF_ROLES + 2l)   // + prologue, final quantize of the last row
    g_rd.pf_sets |> resize(nsets)
    g_rd.pf_arq_sets |> resize(g_rd.n_layers * 2l)
    g_rd.pf_meta |> resize(nsets)
    for (mi in range(nsets)) {
        g_rd.pf_meta[mi] = make_host_buf(BATCH_META_BYTES, true)
    }
    let mirbytes = g_rd.mir_elems * g_rd.kv_esz
    let normbytes = g_rd.norms_bytes
    let kv16 = g_rd.kv16
    // the fa pick is per layer: a model's attention classes may differ in head size, and a class with no fa stamp takes the chunked pair
    let fa_dev = g_gpu.has_coopmat2_fa && vk_fa_on() && kv16
    var fa_hs : table<int64>   //! the head sizes the fa tiles serve on this model
    var chunked_hs : table<int64>   //! ... and the ones the chunked pair serves
    g_rd.pf_facm2 = false
    for (L in g_rd.layers) {
        continue if (L.recurrent)
        L.pf_fa = fa_dev && (L.hs == 64l || L.hs == 128l || L.hs == 256l) && (!g_rd.q_gated || L.hs == 256l)
        g_rd.pf_facm2 = g_rd.pf_facm2 || L.pf_fa
        if (L.pf_fa) {
            fa_hs |> insert(L.hs)
        } else {
            chunked_hs |> insert(L.hs)
        }
    }
    let fa128s = !fa_dev && g_gpu.has_coopmat && vk_fa_on() && key_exists(chunked_hs, 128l)
    let chunked_other = !fa128s ? long_length(chunked_hs) > 0l : long_length(chunked_hs) > 1l
    verify(ensure_cls_dn_rq() && ensure_cls_ar() && ensure_cls_ar_rq_b()
        && (kv16 ? ensure_rope_kv_b_f16_cls() : ensure_rope_kv_b_cls())
        && (!fa128s || (kv16 ? ensure_da_attn_b_h128_f16_cls() : ensure_da_attn_b_h128_cls()))
        && (!chunked_other || (kv16 ? ensure_da_attn_b_f16_cls() : ensure_da_attn_b_cls()))
        && (!key_exists(fa_hs, 64l) || (ensure_fa_cm2_h64_cls() && (!any_f16_feed || ensure_fa_cm2_h64_f16_cls())))
        && (!key_exists(fa_hs, 128l) || (ensure_fa_cm2_h128_cls() && (!any_f16_feed || ensure_fa_cm2_h128_f16_cls())))
        && (!key_exists(fa_hs, 256l) || (g_rd.q_gated
            ? (ensure_fa_cm2_h256_g_cls() && (!any_f16_feed || ensure_fa_cm2_h256_g_f16_cls()))
            : (ensure_fa_cm2_h256_cls() && (!any_f16_feed || ensure_fa_cm2_h256_f16_cls()))))
        && ensure_q8_actrq_cls() && (!g_rd.qk_norm || ensure_qk_rms_cls())
        && (!any_f16_feed || (ensure_f16cvt_cls() && ensure_actf16_cls() && ensure_cls_ar_f16_b()))
        && (!anykq || (ensure_cls_q8k_rq() && ensure_q8k_actrq_cls())),
        "vk prefill: the class rails must engage on a live device")
    if (g_rd.pf_facm2) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: cm2 flash attention armed ({long_length(fa_hs)} head size(s), native f16 mirrors; {long_length(chunked_hs)} on the chunked pair)\n")
    }
    delete fa_hs
    delete chunked_hs
    var nkvm = 0l
    for (l in range64(g_rd.n_layers)) {
        if (pf_kv_merged(l, dim, kvd)) {
            nkvm++
        }
    }
    if (nkvm > 0l) {
        to_log(LOG_INFO, "dasLLAMA vulkan tier: merged k/v prefill GEMM armed ({nkvm} of {g_rd.n_layers} layers; DASLLAMA_VK_KV_MERGE=0 pins the split pair)\n")
    }
    if (g_rd.moe) {
        pf_moe_setup()
    }
    if (g_rd.hybrid) {
        ensure_dn_state()   // dn_ws_dev (the scan workspace) + the staging buffers the reset/handoff ride
        verify(ensure_dn_conv_cls() && ensure_dn_scan_cls() && ensure_dn_scan_p3_cls()
            && ensure_dn_tail_cls() && (!g_rd.dn_ba_f32 || ensure_dn_ba_cls())
            && (!(g_rd.dn_ba_f32 && pf_f16g_on()) || (ensure_f16_gemm_cm2_cls() && ensure_splitk_reduce_cls())),
            "vk prefill: the deltanet class rails must engage on a live device")
        let cd = g_rd.dn_cd
        let di = g_rd.dn_di
        g_rd.pf_dnqkv = make_device_buf(np * cd * 4l)
        g_rd.pf_dnconv = make_device_buf(np * cd * 4l)
        g_rd.pf_dnz = make_device_buf(np * di * 4l)
        g_rd.pf_dnbg = make_device_buf(np * g_rd.dn_nvh * 4l)
        g_rd.pf_dnrq_set = set_rq_cls(fixed_array(g_gpu.dn_ws_dev, g_gpu.hq_dev, g_gpu.hs_dev),
            fixed_array(DN_WS_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES),
            fixed_array(VHZ_DNWS, VHZ_HQ, VHZ_HQ))
        if (any_f16_feed) {
            g_rd.pf_dnof = make_device_buf(np * di * 2l)
            g_rd.pf_dnof_set = set_f16cvt_cls(fixed_array(g_gpu.dn_ws_dev, g_rd.pf_dnof),
                fixed_array(DN_WS_BYTES, np * di * 2l), fixed_array(VHZ_DNWS, VHZ_HQ))
        }
    }
    for (l in range64(g_rd.n_layers)) {
        var L & = unsafe(g_rd.layers[l])
        let b = int(l * PF_ROLES)
        if (L.recurrent) {
            pf_dn_layer_sets(l, dim, g_rd.dn_cd, g_rd.dn_di)
        }
        if (L.moe) {
            pf_moe_layer_sets(l)
        }
        let qkv6 = !L.recurrent && pf_qkv6(l)
        let wo6 = pf_f16_feed(L.fo)
        let gu6 = pf_gu6(l)
        let dn6 = pf_dn6(l)
        // 0 quant_xb: pf_xb -> pf_xq/pf_xs (f16-fed: f16 convert -> pf_xf)
        g_rd.pf_sets[b + 0] = (qkv6
            ? set_f16cvt_cls(fixed_array(g_rd.pf_xb, g_rd.pf_xf),
                fixed_array(np * dim * 4l, np * dim * 2l), fixed_array(VHZ_XB, VHZ_XQ))
            : set_rq_cls(fixed_array(g_rd.pf_xb, g_rd.pf_xq, g_rd.pf_xs),
                fixed_array(np * dim * 4l, np * dim, np * (dim / 32l) * 4l),
                fixed_array(VHZ_XB, VHZ_XQ, VHZ_XQ)))
        // (GEMM roles 1/2/3/7/10/11/13 build lazily per router variant at record time)
        if (!L.recurrent) {
            // 4 rope+store (class: q, kv, Kmir, Vmir, cos, bias)
            let rbufs = fixed_array(g_rd.pf_q, g_rd.pf_kv, g_rd.k_mirror, g_rd.v_mirror, g_rd.pf_cos, g_rd.has_bias ? g_rd.bias_dev : g_rd.norms_dev)
            let rsizes = fixed_array(np * qrow * 4l, np * 2l * kvd * 4l, mirbytes, mirbytes, np * g_rd.cos_elems * 4l, g_rd.has_bias ? g_rd.bias_bytes : g_rd.norms_bytes)
            let rgbits = fixed_array(VHZ_Q, VHZ_KVK | VHZ_KVV, VHZ_MIR, VHZ_MIR, VHZ_COS, 0u)
            g_rd.pf_sets[b + 4] = (kv16 ? set_rope_kv_b_f16_cls(rbufs, rsizes, rgbits)
                : set_rope_kv_b_cls(rbufs, rsizes, rgbits))
        // 5 attn (class: q, Kmir, Vmir, out) — the cm2 fa tile / h128 coopmat twin when they serve this layer's head size
        let fa128l = fa128s && L.hs == 128l
        if (L.pf_fa) {
            g_rd.pf_sets[b + 5] = (wo6
                ? set_fa_cm2_cls(fixed_array(g_rd.pf_q, g_rd.k_mirror, g_rd.v_mirror, g_rd.pf_af),
                    fixed_array(np * qrow * 4l, mirbytes, mirbytes, np * qd * 2l),
                    fixed_array(VHZ_Q, VHZ_MIR, VHZ_MIR, VHZ_AQ))
                : set_fa_cm2_cls(fixed_array(g_rd.pf_q, g_rd.k_mirror, g_rd.v_mirror, g_rd.pf_attn),
                    fixed_array(np * qrow * 4l, mirbytes, mirbytes, np * qd * 4l),
                    fixed_array(VHZ_Q, VHZ_MIR, VHZ_MIR, VHZ_ATT)))
        } else {
            let abufs5 = fixed_array(g_rd.pf_q, g_rd.k_mirror, g_rd.v_mirror, g_rd.pf_attn)
            let asizes5 = fixed_array(np * qrow * 4l, mirbytes, mirbytes, np * qd * 4l)
            let agbits5 = fixed_array(VHZ_Q, VHZ_MIR, VHZ_MIR, VHZ_ATT)
            g_rd.pf_sets[b + 5] = (fa128l
                ? (kv16 ? set_da_attn_b_h128_f16_cls(abufs5, asizes5, agbits5) : set_da_attn_b_h128_cls(abufs5, asizes5, agbits5))
                : (kv16 ? set_da_attn_b_f16_cls(abufs5, asizes5, agbits5) : set_da_attn_b_cls(abufs5, asizes5, agbits5)))
        }
        // 6 quant_at: pf_attn -> pf_aq/pf_as (f16-fed: f16 convert -> pf_af)
        g_rd.pf_sets[b + 6] = (wo6
            ? set_f16cvt_cls(fixed_array(g_rd.pf_attn, g_rd.pf_af),
                fixed_array(np * qd * 4l, np * qd * 2l), fixed_array(VHZ_ATT, VHZ_AQ))
            : set_rq_cls(fixed_array(g_rd.pf_attn, g_rd.pf_aq, g_rd.pf_as),
                fixed_array(np * qd * 4l, np * qd, np * (qd / 32l) * 4l),
                fixed_array(VHZ_ATT, VHZ_AQ, VHZ_AQ)))
        }
        // 8 addr_ffn (class: x, add partner, norms, xb out)
        g_rd.pf_sets[b + 8] = set_cls_ar(fixed_array(g_rd.pf_x, g_rd.pf_xb2, g_rd.norms_dev, g_rd.pf_xb),
            fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim * 4l),
            fixed_array(VHZ_X, VHZ_XB2, 0u, VHZ_XB))
        // 9 quant_xb2: pf_xb -> pf_xq (f16-fed: f16 convert -> pf_xf)
        g_rd.pf_sets[b + 9] = (gu6
            ? set_f16cvt_cls(fixed_array(g_rd.pf_xb, g_rd.pf_xf),
                fixed_array(np * dim * 4l, np * dim * 2l), fixed_array(VHZ_XB, VHZ_XQ))
            : set_rq_cls(fixed_array(g_rd.pf_xb, g_rd.pf_xq, g_rd.pf_xs),
                fixed_array(np * dim * 4l, np * dim, np * (dim / 32l) * 4l),
                fixed_array(VHZ_XB, VHZ_XQ, VHZ_XQ)))
        // 12 actrq (class: gate, up, quants out, scales out; f16-fed: actf16 -> pf_hf)
        g_rd.pf_sets[b + 12] = (dn6
            ? set_actf16_cls(fixed_array(g_rd.pf_gate, g_rd.pf_up, g_rd.pf_hf),
                fixed_array(np * hid * 4l, np * hid * 4l, np * hid * 2l),
                fixed_array(VHZ_GATE, VHZ_UP, VHZ_HQ))
            : set_actrq_cls(fixed_array(g_rd.pf_gate, g_rd.pf_up, g_gpu.hq_dev, g_gpu.hs_dev),
                fixed_array(np * hid * 4l, np * hid * 4l, BATCH_HQ_BYTES, BATCH_HS_BYTES),
                fixed_array(VHZ_GATE, VHZ_UP, VHZ_HQ, VHZ_HQ)))
        // 14 addr_next (class): x += ffnout, xb = rms_att[l+1]|final
        g_rd.pf_sets[b + 14] = set_cls_ar(fixed_array(g_rd.pf_x, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xb),
            fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim * 4l),
            fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XB))
        g_rd.pf_arq_sets[int(l * 2l)] = (gu6
            ? set_cls_ar_f16_b(fixed_array(g_rd.pf_x, g_rd.pf_xb2, g_rd.norms_dev, g_rd.pf_xf),
                fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim * 2l),
                fixed_array(VHZ_X, VHZ_XB2, 0u, VHZ_XQ))
            : set_cls_ar_rq_b(
                fixed_array(g_rd.pf_x, g_rd.pf_xb2, g_rd.norms_dev, g_rd.pf_xq, g_rd.pf_xs),
                fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim, np * (dim / 32l) * 4l),
                fixed_array(VHZ_X, VHZ_XB2, 0u, VHZ_XQ, VHZ_XQ)))
        let next_qkv6 = l + 1l < g_rd.n_layers && pf_qkv6(l + 1l)
        g_rd.pf_arq_sets[int(l * 2l + 1l)] = (next_qkv6
            ? set_cls_ar_f16_b(fixed_array(g_rd.pf_x, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xf),
                fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim * 2l),
                fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XQ))
            : set_cls_ar_rq_b(
                fixed_array(g_rd.pf_x, g_rd.pf_ffnout, g_rd.norms_dev, g_rd.pf_xq, g_rd.pf_xs),
                fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim, np * (dim / 32l) * 4l),
                fixed_array(VHZ_X, VHZ_FFO, 0u, VHZ_XQ, VHZ_XQ)))
        // 15 qk rmsnorm (qk_norm models; class: q, k rows, norm rows)
        if (g_rd.qk_norm && !L.recurrent) {
            g_rd.pf_sets[b + 15] = set_qk_rms_cls(fixed_array(g_rd.pf_q, g_rd.pf_kv, g_rd.norms_dev),
                fixed_array(np * qrow * 4l, np * 2l * kvd * 4l, normbytes),
                fixed_array(VHZ_Q, VHZ_KVK, 0u))
        }
    }
    let bp = int(g_rd.n_layers * PF_ROLES)
    // prologue: xb = rms_att[0](x), add off (the aa slot binds x itself — add_on 0 never reads it)
    g_rd.pf_sets[bp + 0] = set_cls_ar(fixed_array(g_rd.pf_x, g_rd.pf_x, g_rd.norms_dev, g_rd.pf_xb),
        fixed_array(np * dim * 4l, np * dim * 4l, normbytes, np * dim * 4l),
        fixed_array(VHZ_X, VHZ_X, 0u, VHZ_XB))
    // final quantize of the LAST row (into the decode driver's xq_dev/xs_dev) for the cls GEMV
    g_rd.pf_sets[bp + 1] = set_rq_cls(fixed_array(g_rd.pf_xb, g_rd.xq_dev, g_rd.xs_dev),
        fixed_array(np * dim * 4l, dim, (dim / 32l) * 4l),
        fixed_array(VHZ_XB, VHZ_XQ, VHZ_XQ))
    g_rd.pf_ready = true
}

//! Resident-driver prefill: `npos` embedded rows (x_batch, [npos x dim]) + per-position rope rows
//! (cos_batch, [npos x hs]) -> the last row's logits, filling the KV mirror at positions [0, npos).
[hot_path]   // the per-window resident prefill — encode + one submit per window
def vk_rdec_prefill(x_batch : array<float>; cos_batch : array<float>; npos : int64; var logits : array<float>) {
    let noids : array<int64>
    vk_demotion_check("prefill")
    pf_run(x_batch, noids, false, 1.0, cos_batch, npos, logits)
}

[hot_path, arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def vk_rdec_prefill_ids(tokens : array<int64>; emb_scale : float; cos_batch : array<float>; npos : int64; var logits : array<float>) {
    assert(g_rd != null && (g_rd.emb_blk >= 0l || g_rd.emb_f32 != 0ul), "vk_rdec_prefill_ids without a placed embd plane")
    let nox : array<float>
    vk_demotion_check("prefill")
    pf_run(nox, tokens, true, emb_scale, cos_batch, npos, logits)
}

//! After a resident prefill, hand recurrent layer `l`'s device slot to the session `stp`/`histp` name (valid, dirty, bound)
[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#hybrid-token-command")]
def vk_rdec_prefill_dn_own(l : int64; stp : float?; histp : float?) {
    if (g_gpu != null && g_gpu.dry) {
        return
    }
    var L & = unsafe(g_rd.layers[l])
    L.dn.parity = g_rd.dn_parity   // the last window's tail landed in the prefill's ring image
    L.dn.owner_state_addr = intptr(stp)
    L.dn.owner_hist_addr = intptr(histp)
    L.dn.valid = true
    L.dn.dirty = true
}

//! False when the gather rail cannot build on this device (the caller keeps the CPU embed)
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def vk_rdec_set_emb(emb_block : int64) : bool {
    if (g_gpu != null && g_gpu.dry) {
        return true
    }
    assert(g_rd != null, "vk_rdec_set_emb before prepare")
    if (!ensure_emb_gather_cls()) {
        return false
    }
    g_rd.emb_blk = emb_block
    return true
}

//! False past RDEC_EMB_F32_CAP, the device's SSBO range, or the weight budget, or when the gather
//! rail cannot build (the caller keeps the CPU embed)
[arch(at="../ARCHITECTURE_GPU.md#gpu-backends")]
def vk_rdec_upload_emb_f32(fblob : array<float>; off, vocab, dim : int64) : bool {
    if (g_gpu != null && g_gpu.dry) {
        return false
    }
    assert(g_rd != null, "vk_rdec_upload_emb_f32 before prepare")
    let bytes = vocab * dim * 4l
    if (bytes > min(RDEC_EMB_F32_CAP, vk_max_storage_range()) || g_gpu.resident_bytes + bytes > g_gpu.weight_budget
            || !ensure_emb_gather_f32_cls()) {
        return false
    }
    g_rd.emb_f32 = make_device_buf(bytes)
    g_rd.emb_f32_bytes = bytes
    g_gpu.resident_bytes += bytes
    unsafe {
        upload_region_at(g_rd.emb_f32, 0l, addr<uint8 const?>(fblob[int(off)]), bytes)
    }
    return true
}

[hot_path, arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain")]
def private pf_run(x_batch : array<float>; ids : array<int64>; use_ids : bool; emb_scale : float;
                   cos_batch : array<float>; npos : int64; var logits : array<float>) {
    assert(g_rd != null && g_rd.ready, "vk prefill before prepare/set_cls")
    pf_setup()
    let dim = g_rd.dim
    let nlfin = rd_norm_final()
    let hsmax = g_rd.head_size   // nolint:LINT021 — the q/k norm rows sit at the widest class's stride; the row offset multiplies run 64-bit before the push-field narrow
    let cos_elems = g_rd.cos_elems
    let fa128s = !(g_gpu.has_coopmat2_fa && vk_fa_on() && g_rd.kv16) && g_gpu.has_coopmat && vk_fa_on()   //! the h128 coopmat twin serves a 128-head layer where the cm2 tiles do not
    unsafe {
        var w0 = 0l
        var last_wlen = 0l
        let taps = max(g_rd.dn_dconv - 1l, 0l)
        while (w0 < npos) {
            let tp0 = ref_time_ticks()
            var wlen = min(g_rd.pf_np, npos - w0)
            let rem = npos - w0 - wlen
            if (rem > 0l && rem < taps) {
                wlen -= taps - rem   // the last window keeps at least the conv taps: only window 0 can be shorter, and its history is zero
            }
            assert(wlen >= taps || w0 == 0l, "vk prefill: a later window shorter than the conv taps")
            let last = w0 + wlen >= npos
            if (use_ids) {
                g_pf_ids_stage |> resize(wlen)
                for (i in range64(wlen)) {
                    let id = ids[w0 + i]
                    if (id < 0l || id >= g_rd.vocab) {   // fail closed loudly, as the CPU's dequant_q8_row does on the same input
                        panic("vk prefill: token id {id} outside the vocab ({g_rd.vocab}) at row {w0 + i}")
                    }
                    g_pf_ids_stage[int(i)] = uint(id)
                }
                upload_region_at(g_rd.pf_ids, 0l, addr<uint8 const?>(g_pf_ids_stage[0]), wlen * 4l)
            } else {
                upload_region_at(g_rd.pf_x, 0l, addr<uint8 const?>(x_batch[int(w0 * dim)]), wlen * dim * 4l)
            }
            upload_region_at(g_rd.pf_cos, 0l, addr<uint8 const?>(cos_batch[int(w0 * cos_elems)]), wlen * cos_elems * 4l)
            if (w0 == 0l && g_rd.hybrid) {
                g_rd.dn_parity = 0
            }
            let bp = int(g_rd.n_layers * PF_ROLES)
            let t_prep = get_time_usec(tp0)
            // record
            var raw = g_rd.pf_cmd
            let rf : VkCommandBufferResetFlags
            vk_check(vkResetCommandBuffer(raw, rf), null)
            let begin = VkCommandBufferBeginInfo()
            vk_check(vkBeginCommandBuffer(raw, begin), null)
            pfq_begin(raw)
            pfq_ts(raw)   // q0: submit start
            var h : VkHaz
            if (w0 == 0l && g_rd.hybrid) {
                pf_dn_zero(raw, h)
            }
            pfq_ts(raw)   // q1: kept for the role-aggregation stamp shape (params ride push now)
            if (use_ids) {
                // no extra pfq_ts - the gather cost folds into the prologue stamp
                var pce = EmbArgs(npos = uint(wlen), dim = uint(dim),
                    wblk0 = g_rd.emb_blk >= 0l ? uint(arena_local_blk(g_rd.emb_blk)) : 0u, embed_scale = emb_scale)
                if (g_rd.emb_blk >= 0l) {
                    enc_emb_gather_cls(raw, h, g_rd.pf_emb_set, pce, (wlen * dim / 4l + 255l) / 256l)
                } else {
                    enc_emb_gather_f32_cls(raw, h, g_rd.pf_emb_set, pce, (wlen * dim / 4l + 255l) / 256l)
                }
            }
            var pc_pro = ArArgs(dim = uint(dim), add_on = 0u, woff = 0u, eps = g_rd.eps, ascale = 1.0)
            enc_cls_ar(raw, h, g_rd.pf_sets[bp + 0], pc_pro, wlen)   // prologue norm
            pfq_ts(raw)
            let overlap = g_env_vulkan.vk_overlap && !vk_prof()
            let fuse_arq = g_env_vulkan.vk_fuse
            var nextb = 1l
            var cstep = 1l
            var nchunk = 0
            for (l in range64(g_rd.n_layers)) {
                if (overlap && l == nextb) {
                    vk_check(vkEndCommandBuffer(raw), null)
                    submit_nofence(raw)
                    assert(nchunk < length(g_rd.pf_cmd_ring), "vk prefill: overlap ring exhausted")
                    raw = g_rd.pf_cmd_ring[nchunk]
                    nchunk++
                    let rfc : VkCommandBufferResetFlags
                    vk_check(vkResetCommandBuffer(raw, rfc), null)
                    let beginc = VkCommandBufferBeginInfo()
                    vk_check(vkBeginCommandBuffer(raw, beginc), null)
                    cstep = min(cstep * 2l, PF_CHUNK_MAX)
                    nextb += cstep
                }
                let b = int(l * PF_ROLES)
                let np = g_rd.pf_np
                let gu6 = pf_gu6(l)
                let dn6 = pf_dn6(l)
                let guk = kq_sb(g_rd.layers[l].f1) && !gu6
                let dnk = kq_sb(g_rd.layers[l].f2) && !dn6
                let hid = g_rd.layers[l].hid
                if (g_rd.layers[l].recurrent) {
                    rd_pf_recurrent(raw, h, l, b, wlen, last)
                } else {
                // this layer's attention class: gemma-4's sliding and global layers differ in head size, kv width, rope and window
                let hs = g_rd.layers[l].hs
                let qd = g_rd.layers[l].qd
                let kvd = g_rd.layers[l].kvd
                let kv_mul = uint(g_rd.layers[l].kv_mul)
                let window = uint(g_rd.layers[l].window)
                let pairs = int(qd / 2l + kvd / 2l)
                let ropewg = (wlen * int64(pairs) + int64(WG_X) - 1l) / int64(WG_X)
                let fa128 = fa128s && hs == 128l
                let attnwg = fa128 ? g_rd.n_heads * ((wlen + 31l) / 32l) : g_rd.n_heads * ((wlen + 7l) / 8l)
                // f16-fed roles (mode 4's cm2 tiles): the f32->f16 convert feeds decode-in-load; the fill's tile rule keys off the same flag
                let fq6 = pf_qkv6(l)
                let fo6 = pf_f16_feed(g_rd.layers[l].fo)
                // kq consumers take the Q8_K quant/act forms — unless the f16 feed serves them (k4 cm2)
                let fqk = kq_sb(g_rd.layers[l].fq) && !fq6
                let fok = kq_sb(g_rd.layers[l].fo) && !fo6
                let mirb = uint(g_rd.layers[l].mir_base)
                let feed_already_fused = l > 0l && pf_qkv_feed_fused(l)
                if (!feed_already_fused) {
                    if (fq6) {
                        var pc0 = ActArgs(nelem = uint(wlen * dim), gelu = 0u, nblk = 0u)
                        enc_f16cvt_cls(raw, h, g_rd.pf_sets[b + 0], pc0, (wlen * dim / 4l + 255l) / 256l)
                    } elif (fqk) {
                        var pc0 = RqArgs(inbase = 0u, nblk = uint(wlen * dim / 256l))
                        enc_cls_q8k_rq(raw, h, g_rd.pf_sets[b + 0], pc0, (wlen * dim / 8l + 255l) / 256l)
                    } else {
                        var pc0 = RqArgs(inbase = 0u, nblk = uint(wlen * dim / 32l))
                        enc_cls_dn_rq(raw, h, g_rd.pf_sets[b + 0], pc0, (wlen * dim / 4l + 255l) / 256l)
                    }
                }
                pfq_ts(raw)
                let pq = arena_planes(g_rd.layers[l].fq, g_rd.layers[l].bq)
                let pk = arena_planes(g_rd.layers[l].fk, g_rd.layers[l].bk)
                let pv = arena_planes(g_rd.layers[l].fv, g_rd.layers[l].bv)
                let xqb = fq6 ? g_rd.pf_xf : g_rd.pf_xq
                let xsb = fq6 ? g_rd.dummy : g_rd.pf_xs
                let xqn = fq6 ? np * dim * 2l : np * dim
                let xsn = fq6 ? 256l : np * (dim / 32l) * 4l
                let kv_merged = pf_kv_merged(l, dim, kvd)
                let qrow = g_rd.q_gated ? 2l * qd : qd
                let qhs = int(g_rd.q_gated ? 2l * hs : hs)
                let tq = cm2_tiles(qrow, wlen)
                let tkv = cm2_tiles(kvd, wlen)
                pf_gemm_enc(raw, h, b + 1, g_rd.layers[l].fq, pq.wq, pq.ws, xqb, xsb, g_rd.pf_q,
                    pq.wqb, pq.wsb, xqn, xsn, np * qrow * 4l, VHZ_XQ, VHZ_Q, dim, qrow, g_rd.layers[l].bq, wlen, fq6,
                    kv_merged ? cm2_tiles(2l * kvd, wlen) : 2l * tkv)
                if (kv_merged) {
                    pf_gemm_enc(raw, h, b + 2, g_rd.layers[l].fk, pk.wq, pk.ws, xqb, xsb, g_rd.pf_kv,
                        pk.wqb, pk.wsb, xqn, xsn, np * 2l * kvd * 4l, VHZ_XQ, VHZ_KVK | VHZ_KVV, dim, 2l * kvd, g_rd.layers[l].bk, wlen, fq6, tq)
                    pfq_ts(raw)   // the v stamp: the merged GEMM's cost lands on k's delta
                } else {
                    pf_gemm_enc(raw, h, b + 2, g_rd.layers[l].fk, pk.wq, pk.ws, xqb, xsb, g_rd.pf_kv,
                        pk.wqb, pk.wsb, xqn, xsn, np * 2l * kvd * 4l, VHZ_XQ, VHZ_KVK, dim, kvd, g_rd.layers[l].bk, wlen, fq6, tq + tkv)
                    pf_gemm_enc(raw, h, b + 3, g_rd.layers[l].fv, pv.wq, pv.ws, xqb, xsb, g_rd.pf_v,
                        pv.wqb, pv.wsb, xqn, xsn, np * kvd * 4l, VHZ_XQ, VHZ_VST, dim, kvd, g_rd.layers[l].bv, wlen, fq6, tq + tkv)
                    vhz_dep(raw, h, VHZ_VST, VHZ_KVV, true)
                    cmd_copy_range(raw, g_rd.pf_v, 0l, g_rd.pf_kv, wlen * kvd * 4l, wlen * kvd * 4l)   // v into pf_kv's v half
                }
                if (g_rd.qk_norm) {
                    var pcn = QkRmsArgs(hs = uint(hs), nh = uint(g_rd.n_heads), nkvh = uint(kvd / hs),
                        qstride = uint(qrow), kstride = uint(kv_merged ? 2l * kvd : kvd), kbase = 0u,
                        qwoff = uint(nlfin + dim + l * 2l * hsmax), kwoff = uint(nlfin + dim + (l * 2l + 1l) * hsmax),
                        eps = g_rd.eps, qhs = uint(qhs))
                    enc_qk_rms_cls(raw, h, g_rd.pf_sets[b + 15], pcn, wlen * (g_rd.n_heads + kvd / hs))
                    pfq_ts(raw)
                }
                var pcr = RopeKvBArgs(qd = uint(qd), kvd = uint(kvd), hs = uint(hs), half = uint(g_rd.layers[l].rot / 2l),
                    neox = g_rd.neox ? 1u : 0u, kpair0 = uint(qd / 2l), ppp = uint(pairs),
                    layerbase = mirb, npos = uint(wlen), pos0 = uint(w0),
                    voff = uint(kv_merged ? kvd : wlen * kvd), kstride = uint(kv_merged ? 2l * kvd : kvd), qhs = uint(qhs),
                    boff = uint(g_rd.layers[l].boff), hasb = g_rd.has_bias ? 1u : 0u,
                    cstride = uint(cos_elems), cbase = uint(g_rd.layers[l].cbase))
                if (g_rd.kv16) {
                    enc_rope_kv_b_f16_cls(raw, h, g_rd.pf_sets[b + 4], pcr, ropewg)
                } else {
                    enc_rope_kv_b_cls(raw, h, g_rd.pf_sets[b + 4], pcr, ropewg)
                }
                pfq_ts(raw)
                if (g_rd.layers[l].pf_fa) {
                    // the cm2 fa tile reads the f16 mirrors directly
                    var pcf = FaCm2Args(rows = uint(wlen), w0 = uint(w0), qd = uint(qd),
                        kvd = uint(kvd), kv_mul = kv_mul, kvbase = mirb,
                        scale = g_rd.scale, qstride = uint(qrow), qhs = uint(qhs), window = window)
                    let fawgs = g_rd.n_heads * ((wlen + 63l) / 64l)
                    if (hs == 64l) {
                        if (fo6) {
                            enc_fa_cm2_h64_f16_cls(raw, h, g_rd.pf_sets[b + 5], pcf, fawgs)
                        } else {
                            enc_fa_cm2_h64_cls(raw, h, g_rd.pf_sets[b + 5], pcf, fawgs)
                        }
                    } elif (hs == 128l) {
                        if (fo6) {
                            enc_fa_cm2_h128_f16_cls(raw, h, g_rd.pf_sets[b + 5], pcf, fawgs)
                        } else {
                            enc_fa_cm2_h128_cls(raw, h, g_rd.pf_sets[b + 5], pcf, fawgs)
                        }
                    } elif (g_rd.q_gated) {
                        if (fo6) {
                            enc_fa_cm2_h256_g_f16_cls(raw, h, g_rd.pf_sets[b + 5], pcf, fawgs)
                        } else {
                            enc_fa_cm2_h256_g_cls(raw, h, g_rd.pf_sets[b + 5], pcf, fawgs)
                        }
                    } else {
                        if (fo6) {
                            enc_fa_cm2_h256_f16_cls(raw, h, g_rd.pf_sets[b + 5], pcf, fawgs)
                        } else {
                            enc_fa_cm2_h256_cls(raw, h, g_rd.pf_sets[b + 5], pcf, fawgs)
                        }
                    }
                } else {
                    var pca = DaAttnBArgs(npos = uint(wlen), nh = uint(g_rd.n_heads), kvd = uint(kvd),
                        hs = uint(hs), kv_mul = kv_mul, kbase = mirb, vbase = mirb,
                        w0 = uint(w0), scale = g_rd.scale, qhs = uint(qhs), gated = g_rd.q_gated ? 1u : 0u, window = window)
                    if (fa128) {
                        if (g_rd.kv16) {
                            enc_da_attn_b_h128_f16_cls(raw, h, g_rd.pf_sets[b + 5], pca, attnwg)
                        } else {
                            enc_da_attn_b_h128_cls(raw, h, g_rd.pf_sets[b + 5], pca, attnwg)
                        }
                    } else {
                        if (g_rd.kv16) {
                            enc_da_attn_b_f16_cls(raw, h, g_rd.pf_sets[b + 5], pca, attnwg)
                        } else {
                            enc_da_attn_b_cls(raw, h, g_rd.pf_sets[b + 5], pca, attnwg)
                        }
                    }
                }
                pfq_ts(raw)
                let attn_landed_f16 = fo6 && g_rd.layers[l].pf_fa
                if (attn_landed_f16) {
                } elif (fo6) {
                    var pc6 = ActArgs(nelem = uint(wlen * qd), gelu = 0u, nblk = 0u)
                    enc_f16cvt_cls(raw, h, g_rd.pf_sets[b + 6], pc6, (wlen * qd / 4l + 255l) / 256l)
                } elif (fok) {
                    var pc6 = RqArgs(inbase = 0u, nblk = uint(wlen * qd / 256l))
                    enc_cls_q8k_rq(raw, h, g_rd.pf_sets[b + 6], pc6, (wlen * qd / 8l + 255l) / 256l)
                } else {
                    var pc6 = RqArgs(inbase = 0u, nblk = uint(wlen * qd / 32l))
                    enc_cls_dn_rq(raw, h, g_rd.pf_sets[b + 6], pc6, (wlen * qd / 4l + 255l) / 256l)
                }
                pfq_ts(raw)
                let po = arena_planes(g_rd.layers[l].fo, g_rd.layers[l].bo)
                let aqb = fo6 ? g_rd.pf_af : g_rd.pf_aq
                let asb = fo6 ? g_rd.dummy : g_rd.pf_as
                let aqn = fo6 ? np * qd * 2l : np * qd
                let asn = fo6 ? 256l : np * (qd / 32l) * 4l
                pf_gemm_enc(raw, h, b + 7, g_rd.layers[l].fo, po.wq, po.ws, aqb, asb, g_rd.pf_xb2,
                    po.wqb, po.wsb, aqn, asn, np * dim * 4l, VHZ_AQ, VHZ_XB2, qd, dim, g_rd.layers[l].bo, wlen, fo6)
                }
                let pre_on = g_rd.layers[l].pre_post ? 1u : 0u
                var pc8 = ArArgs(dim = uint(dim), add_on = 1u, woff = uint(rd_norm_off(l, RD_NORM_FFN)), eps = g_rd.eps, ascale = 1.0,
                    pre_on = pre_on, pwoff = uint(rd_norm_off(l, RD_NORM_POST_ATT)))
                let moe = g_rd.layers[l].moe   //! the routed block reads the normed rows the fused twin never stores
                if (fuse_arq && !guk && !moe) {
                    if (gu6) {
                        enc_cls_ar_f16_b(raw, h, g_rd.pf_arq_sets[int(l * 2l)], pc8, wlen)
                    } else {
                        enc_cls_ar_rq_b(raw, h, g_rd.pf_arq_sets[int(l * 2l)], pc8, wlen)
                    }
                    pfq_ts(raw)
                } else {
                    enc_cls_ar(raw, h, g_rd.pf_sets[b + 8], pc8, wlen)
                    pfq_ts(raw)
                    let feed_read = !moe || has_dense_triple(g_rd.layers[l])   //! the routed block takes the f32 rows; only a dense triple reads the feed
                    if (feed_read && gu6) {
                        var pc9 = ActArgs(nelem = uint(wlen * dim), gelu = 0u, nblk = 0u)
                        enc_f16cvt_cls(raw, h, g_rd.pf_sets[b + 9], pc9, (wlen * dim / 4l + 255l) / 256l)
                    } elif (feed_read && guk) {
                        var pc9 = RqArgs(inbase = 0u, nblk = uint(wlen * dim / 256l))
                        enc_cls_q8k_rq(raw, h, g_rd.pf_sets[b + 9], pc9, (wlen * dim / 8l + 255l) / 256l)
                    } elif (feed_read) {
                        var pc9 = RqArgs(inbase = 0u, nblk = uint(wlen * dim / 32l))
                        enc_cls_dn_rq(raw, h, g_rd.pf_sets[b + 9], pc9, (wlen * dim / 4l + 255l) / 256l)
                    }
                }
                pfq_ts(raw)
                let ffn_row0 = pf_ffn_slice_row0(l, g_rd.n_layers, wlen, gu6 && !moe, dn6)
                let ffn_rows = wlen - ffn_row0
                if (has_dense_triple(g_rd.layers[l])) {
                    let p1 = arena_planes(g_rd.layers[l].f1, g_rd.layers[l].b1)
                    let p3 = arena_planes(g_rd.layers[l].f3, g_rd.layers[l].b3)
                    let gqb = gu6 ? g_rd.pf_xf : g_rd.pf_xq
                    let gsb = gu6 ? g_rd.dummy : g_rd.pf_xs
                    let gqn = gu6 ? np * dim * 2l : np * dim
                    let gsn = gu6 ? 256l : np * (dim / 32l) * 4l
                    let tgu = cm2_tiles(hid, ffn_rows)
                    pf_gemm_enc(raw, h, b + 10, g_rd.layers[l].f1, p1.wq, p1.ws, gqb, gsb, g_rd.pf_gate,
                        p1.wqb, p1.wsb, gqn, gsn, np * hid * 4l, VHZ_XQ, VHZ_GATE, dim, hid, g_rd.layers[l].b1, ffn_rows, gu6, tgu, ffn_row0)
                    pf_gemm_enc(raw, h, b + 11, g_rd.layers[l].f3, p3.wq, p3.ws, gqb, gsb, g_rd.pf_up,
                        p3.wqb, p3.wsb, gqn, gsn, np * hid * 4l, VHZ_XQ, VHZ_UP, dim, hid, g_rd.layers[l].b3, ffn_rows, gu6, tgu, ffn_row0)
                    let gelu = g_rd.layers[l].gelu ? 1u : 0u
                    if (dn6) {
                        var pc12 = ActArgs(nelem = uint(ffn_rows * hid), gelu = gelu, nblk = uint(ffn_rows * hid / 32l), elem0 = uint(ffn_row0 * hid))
                        enc_actf16_cls(raw, h, g_rd.pf_sets[b + 12], pc12, (ffn_rows * hid / 4l + 255l) / 256l)
                    } elif (dnk) {
                        var pc12 = ActArgs(nelem = uint(wlen * hid), gelu = gelu, nblk = uint(wlen * hid / 256l))
                        enc_q8k_actrq_cls(raw, h, g_rd.pf_sets[b + 12], pc12, (wlen * hid / 8l + 255l) / 256l)
                    } else {
                        var pc12 = ActArgs(nelem = uint(wlen * hid), gelu = gelu, nblk = uint(wlen * hid / 32l))
                        enc_q8_actrq_cls(raw, h, g_rd.pf_sets[b + 12], pc12, (wlen * hid / 4l + 255l) / 256l)
                    }
                    pfq_ts(raw)
                    let p2 = arena_planes(g_rd.layers[l].f2, g_rd.layers[l].b2)
                    let hqb = dn6 ? g_rd.pf_hf : g_gpu.hq_dev
                    let hsb = dn6 ? g_rd.dummy : g_gpu.hs_dev
                    let hqn = dn6 ? np * hid * 2l : np * hid
                    let hsn = dn6 ? 256l : np * (hid / 32l) * 4l
                    pf_gemm_enc(raw, h, b + 13, g_rd.layers[l].f2, p2.wq, p2.ws, hqb, hsb, g_rd.pf_ffnout,
                        p2.wqb, p2.wsb, hqn, hsn, np * dim * 4l, VHZ_HQ, VHZ_FFO, hid, dim, g_rd.layers[l].b2, ffn_rows, dn6, 0l, ffn_row0)
                }
                if (moe) {
                    pf_moe_block(raw, h, l, wlen)
                }
                let nxt = l + 1l < g_rd.n_layers ? rd_norm_off(l + 1l, RD_NORM_ATT) : nlfin
                var pc14 = ArArgs(dim = uint(dim), add_on = 1u, woff = uint(nxt), eps = g_rd.eps, ascale = 1.0, row0 = uint(ffn_row0),
                    pre_on = pre_on, pwoff = uint(rd_norm_off(l, RD_NORM_POST_FFN)))
                let next_qkv6 = l + 1l < g_rd.n_layers && pf_qkv6(l + 1l)
                if (moe) {
                    pf_moe_addr_next(raw, h, l, wlen, nxt)
                } elif (l + 1l < g_rd.n_layers && pf_qkv_feed_fused(l + 1l)) {
                    if (next_qkv6) {
                        enc_cls_ar_f16_b(raw, h, g_rd.pf_arq_sets[int(l * 2l + 1l)], pc14, wlen)
                    } else {
                        enc_cls_ar_rq_b(raw, h, g_rd.pf_arq_sets[int(l * 2l + 1l)], pc14, wlen)
                    }
                } else {
                    enc_cls_ar(raw, h, g_rd.pf_sets[b + 14], pc14, ffn_rows)
                }
                pfq_ts(raw)
            }
            if (last) {
                if (kq_sb(g_rd.cls_fmt_marker)) {
                    var pcf = RqArgs(inbase = uint((wlen - 1l) * dim), nblk = uint(dim / 256l))
                    enc_cls_q8k_rq(raw, h, g_rd.pf_sets[bp + 1], pcf, (dim / 8l + 255l) / 256l)
                } else {
                    var pcf = RqArgs(inbase = uint((wlen - 1l) * dim), nblk = uint(dim / 32l))
                    enc_cls_dn_rq(raw, h, g_rd.pf_sets[bp + 1], pcf, (dim / 4l + 255l) / 256l)
                }
                pfq_ts(raw)
                var pcc = GemvArgs(n = uint(dim), d = uint(g_rd.vocab), nreg = 1u, ybase = 0u)
                gemv_enc(g_rd.cls_fmt_marker, raw, h, g_rd.cls_set, pcc, g_rd.vocab)
                pfq_ts(raw)
                vhz_dep(raw, h, VHZ_LOG, 0u, true)
                cmd_copy_whole(raw, g_rd.logits_dev, g_rd.pf_logits_host.buf, g_rd.vocab * 4l)
            }
            if (vk_prof()) {
                to_log(LOG_INFO, "vk_rdpf hz: {h.disp} nodes, {h.barriers} barriers\n")   // nolint:PERF026,PERF028 — vk_prof()-gated report leg
                if (g_rd.moe) {
                    vhz_dep(raw, h, VHZ_MRT, 0u, true)
                    cmd_copy_whole(raw, g_rd.pf_midx, g_rd.pf_midx_host.buf, wlen * g_rd.k * 4l)
                }
            }
            pfq_end(raw)
            vk_check(vkEndCommandBuffer(raw), null)
            let t_record = get_time_usec(tp0) - t_prep
            submit_wait(raw)
            if (vk_prof()) {
                to_log(LOG_INFO, "vk_rdpf w0 {w0} rows {wlen} prep {t_prep} record {t_record} submit {get_time_usec(tp0) - t_prep - t_record}\n")   // nolint:PERF026,PERF028 — vk_prof()-gated report leg
            }
            w0 += wlen
            last_wlen = wlen
        }
        memcpy(addr<void?>(logits[0]), g_rd.pf_logits_host.mapped, g_rd.vocab * 4l)
        // per-role GPU aggregation, LAST window, only when every stamp landed; overlapped levels (q|k|v, gate|up) bill the level on the last member's delta
        if (vk_prof()) {
            pf_prof_report(npos)
            if (g_rd.moe) {
                pf_moe_bucket_report(last_wlen)
            }
        }
    }
}

//! the profile's bucket report: the last MoE layer's picks of the last window as per-expert row
//! counts - the shape the expert tiles run over - with the 32-row tile count the s column takes
//! and the count a 32 / 64 / 128 column ladder would take
[cold_path]
def private pf_moe_bucket_report(wlen : int64) {
    let nk = wlen * g_rd.k
    var cnt : array<int64>
    cnt |> reserve(g_rd.ne)
    cnt |> resize(g_rd.ne)
    unsafe {
        let picks = reinterpret<uint?>(g_rd.pf_midx_host.mapped)
        for (i in range64(nk)) {
            let e = int64(picks[i])
            if (e < g_rd.ne) {
                cnt[e]++
            }
        }
    }
    var used = 0l
    var largest = 0l
    var b32 = 0l
    var b64 = 0l
    var b128 = 0l
    var b_over = 0l
    var tiles_s = 0l
    var tiles_ladder = 0l
    for (c in cnt) {
        continue if (c == 0l)
        used++
        largest = max(largest, c)
        tiles_s += (c + 31l) / 32l
        if (c <= 32l) {
            b32++
            tiles_ladder++
        } elif (c <= 64l) {
            b64++
            tiles_ladder++
        } else {
            if (c <= 128l) {
                b128++
            } else {
                b_over++
            }
            tiles_ladder += (c + 127l) / 128l
        }
    }
    to_log(LOG_INFO, "vk_rdpf moe buckets (the last MoE layer, {nk} rows over {g_rd.ne} experts): {used} non-empty, the largest {largest}; sizes <=32: {b32} 33-64: {b64} 65-128: {b128} >128: {b_over}; 32-row tiles {tiles_s}, a 32/64/128 ladder {tiles_ladder}\n")
    delete cnt
}

[cold_path]   // DASLLAMA_GPU_PROF=1 report leg
def private pf_prof_report(npos : int64) {
    var want = 5u
    for (L in g_rd.layers) {
        want += uint(pf_roles_per_layer(L))
    }
    if (g_pfq_n != want) {
        to_log(LOG_INFO, "vk_rdpf gpu: roles unaggregated ({g_pfq_n} stamps, the table expects {want})\n")
        return
    }
    var att : double[16]
    var dn : double[18]
    var moe : double[26]
    var dnm : double[26]   //! a recurrent MoE layer: the deltanet head's stamps, then the routed tail's
    var natt = 0l
    var ndn = 0l
    var nmoe = 0l
    var ndnm = 0l
    var qi = 3u
    for (L in g_rd.layers) {
        let n = pf_roles_per_layer(L)
        for (s in range64(n)) {
            if (L.recurrent && L.moe) {
                dnm[s] += pfq_us(qi + uint(s))
            } elif (L.recurrent) {
                dn[s] += pfq_us(qi + uint(s))
            } elif (L.moe) {
                moe[s] += pfq_us(qi + uint(s))
            } else {
                att[s] += pfq_us(qi + uint(s))
            }
        }
        qi += uint(n)
        if (L.recurrent && L.moe) {
            ndnm++
        } elif (L.recurrent) {
            ndn++
        } elif (L.moe) {
            nmoe++
        } else {
            natt++
        }
    }
    var tot = pfq_us(2u) + pfq_us(qi) + pfq_us(qi + 1u)
    var tatt = 0.0lf
    for (v in att) {
        tatt += v
    }
    var tdn = 0.0lf
    for (v in dn) {
        tdn += v
    }
    var tmoe = 0.0lf
    for (v in moe) {
        tmoe += v
    }
    var tdnm = 0.0lf
    for (v in dnm) {
        tdnm += v
    }
    tot += tatt + tdn + tmoe + tdnm
    to_log(LOG_INFO, "vk_rdpf gpu ({npos} tokens, last window) prologue {int(pfq_us(2u))} fin_rq {int(pfq_us(qi))} cls {int(pfq_us(qi + 1u))} attention layers {natt}: {int(tatt)} recurrent layers {ndn}: {int(tdn)} MoE layers {nmoe}: {int(tmoe)} recurrent MoE layers {ndnm}: {int(tdnm)} total {int(tot)} us\n")
    var has_shexp = false
    for (L in g_rd.layers) {
        has_shexp ||= L.moe && has_dense_triple(L)
    }
    if (nmoe > 0l) {
        //! an MoE layer's stamps: the attention roles (the qkn one shifts them by one), ar1, rq_f, then the tail
        let q = g_rd.qk_norm ? 1 : 0
        let qkn = q > 0 ? "qkn {int(moe[4])} " : ""
        to_log(LOG_INFO, "vk_rdpf moe rq_x {int(moe[0])} q {int(moe[1])} k {int(moe[2])} v {int(moe[3])} {qkn}rope_kv {int(moe[4 + q])} attn {int(moe[5 + q])} rq_a {int(moe[6 + q])} wo {int(moe[7 + q])} ar1 {int(moe[8 + q])} rq_f {int(moe[9 + q])} {pf_moe_tail_text(moe, 10 + q, has_shexp)}\n")
    }
    if (ndnm > 0l) {
        //! a recurrent MoE layer's stamps: the deltanet head's (the f32 beta/alpha arm folds two into one), ar1, rq_f, then the tail
        let b = g_rd.dn_ba_f32 ? 0 : 1
        let ba = g_rd.dn_ba_f32 ? "ba {int(dnm[3])}" : "beta {int(dnm[3])} alpha {int(dnm[4])}"
        to_log(LOG_INFO, "vk_rdpf dnmoe rq_x {int(dnm[0])} qkv {int(dnm[1])} z {int(dnm[2])} {ba} conv {int(dnm[4 + b])} scan {int(dnm[5 + b])} onorm {int(dnm[6 + b])} tail {int(dnm[7 + b])} rq_o {int(dnm[8 + b])} out {int(dnm[9 + b])} ar1 {int(dnm[10 + b])} rq_f {int(dnm[11 + b])} {pf_moe_tail_text(dnm, 12 + b, has_shexp)}\n")
    }
    if (natt > 0l) {
        // the qkn dispatch stamps between v and rope_kv, shifting every later role by one
        if (g_rd.qk_norm) {
            to_log(LOG_INFO, "vk_rdpf attn rq_x {int(att[0])} q {int(att[1])} k {int(att[2])} v {int(att[3])} qkn {int(att[4])} rope_kv {int(att[5])} attn {int(att[6])} rq_a {int(att[7])} wo {int(att[8])} ar1 {int(att[9])} rq_f {int(att[10])} gate {int(att[11])} up {int(att[12])} act {int(att[13])} down {int(att[14])} ar2 {int(att[15])}\n")
        } else {
            to_log(LOG_INFO, "vk_rdpf attn rq_x {int(att[0])} q {int(att[1])} k {int(att[2])} v {int(att[3])} rope_kv {int(att[4])} attn {int(att[5])} rq_a {int(att[6])} wo {int(att[7])} ar1 {int(att[8])} rq_f {int(att[9])} gate {int(att[10])} up {int(att[11])} act {int(att[12])} down {int(att[13])} ar2 {int(att[14])}\n")
        }
    }
    if (ndn > 0l) {
        let ba_stamp_shift = g_rd.dn_ba_f32 ? 0l : 1l
        let ba = g_rd.dn_ba_f32 ? "ba {int(dn[3])}" : "beta {int(dn[3])} alpha {int(dn[4])}"
        to_log(LOG_INFO, "vk_rdpf dn rq_x {int(dn[0])} qkv {int(dn[1])} z {int(dn[2])} {ba} conv {int(dn[4 + ba_stamp_shift])} scan {int(dn[5 + ba_stamp_shift])} onorm {int(dn[6 + ba_stamp_shift])} tail {int(dn[7 + ba_stamp_shift])} rq_o {int(dn[8 + ba_stamp_shift])} out {int(dn[9 + ba_stamp_shift])} ar1 {int(dn[10 + ba_stamp_shift])} rq_f {int(dn[11 + ba_stamp_shift])} gate {int(dn[12 + ba_stamp_shift])} up {int(dn[13 + ba_stamp_shift])} act {int(dn[14 + ba_stamp_shift])} down {int(dn[15 + ba_stamp_shift])} ar2 {int(dn[16 + ba_stamp_shift])}\n")
    }
}

// The arena FFN chain's scratch + the slab-independent act set. The three GEMV sets build lazily
// per SLAB in vk_arena_ffn (bindings 0/1 are the blk's slab planes); each GEMV role carries its
// OWN regions buffer (the weight base pair).
def private ensure_af_state(f1, f3, f2 : int) {
    if (g_gpu.af_ready) {
        return
    }
    ensure_ffn_state()
    let dkq = !vk_fmt_b32(f2)
    verify(gemv_cls_ensure(f1) && gemv_cls_ensure(f3) && gemv_cls_ensure(f2)
        && (dkq ? ensure_q8k_actrq_cls() : ensure_q8_actrq_cls()),
        "vulkan arena: the class rails must engage on a live device")
    g_gpu.af_meta_g = make_host_buf(META_BYTES, true)
    g_gpu.af_meta_u = make_host_buf(META_BYTES, true)
    g_gpu.af_meta_d = make_host_buf(META_BYTES, true)
    g_gpu.af_xq = make_host_buf(XQ_BYTES, true)
    g_gpu.af_xs = make_host_buf(XS_BYTES, true)
    g_gpu.af_ya_dev = make_device_buf(Y_BYTES)
    g_gpu.af_yb_dev = make_device_buf(Y_BYTES)
    g_gpu.af_yo_dev = make_device_buf(Y_BYTES)
    g_gpu.af_yo_host = make_host_buf(Y_BYTES, false, [cached = true])
    // the requant form follows the DOWN plane; class bindings: gate@0, up@1, hq@2, hs@3
    let abufs = fixed_array(g_gpu.af_ya_dev, g_gpu.af_yb_dev, g_gpu.hq_dev, g_gpu.hs_dev)
    let asizes = fixed_array(Y_BYTES, Y_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES)
    let agbits = fixed_array(VHG_Y1, VHG_Y2, VHG_HQ, VHG_HQ)
    g_gpu.af_actrq_set = set_actrq_cls(abufs, asizes, agbits)   // one family set serves both forms
    g_gpu.af_ready = true
}

// one af GEMV set per (role cache, blk's slab) — the role fixes meta/activation/output bindings.
// xbit: the activation pair's hazard bits (0 for host-written xq/xs, VHG_HQ for the actrq-fed
// down chain — losing it would let the down GEMV race the act+requant write).
def private af_role_set(var sets : table<int; VkDescriptorSet>; fmt : int; blk : int64;
                        mbuf, xqbuf, xsbuf, ybuf : uint64; xqbytes, xsbytes : int64; xbit, ybit : uint) : VkDescriptorSet {
    let sl = arena_slab_idx(blk)
    if (!key_exists(sets, sl)) {
        let p = arena_planes(fmt, blk)
        sets[sl] = gemv_cls_set(fmt,
            fixed_array(p.wq, p.ws, mbuf, xqbuf, xsbuf, ybuf),
            fixed_array(p.wqb, p.wsb, META_BYTES, xqbytes, xsbytes, Y_BYTES),
            fixed_array(0u, 0u, 0u, xbit, xbit, ybit))
    }
    return sets[sl]
}

//! Arena seam: one dense FFN layer against arena-resident planes — gate GEMV, up GEMV, fused
//! act+requant, down GEMV, ONE submit. `blk1/blk3/blk2` are gate/up/down arena block indices.
//! Gate and up must share a format class (they share the activation image); down is free.
def vk_arena_ffn(var y : array<float>; blk1, blk3, blk2 : int64; n, nfe : int64;
                 f1, f3, f2 : int; xq : array<int8>; xs : array<float>; is_gelu : bool) {
    verify(vk_moe_init())
    assert(vk_fmt_b32(f3) == vk_fmt_b32(f1), "dasLLAMA vulkan arena: gate/up form mismatch - one activation image")
    assert(nfe * 4l <= Y_BYTES && n * 4l <= Y_BYTES, "dasLLAMA vulkan arena: ffn planes exceed y capacity")
    ensure_af_state(f1, f3, f2)
    let xunit = fmt_unit(f1)
    let dkq = !vk_fmt_b32(f2)
    var gate_set = af_role_set(g_gpu.af_gate_sets, f1, blk1, g_gpu.af_meta_g.buf,
        g_gpu.af_xq.buf, g_gpu.af_xs.buf, g_gpu.af_ya_dev, XQ_BYTES, XS_BYTES, 0u, VHG_Y1)
    var up_set = af_role_set(g_gpu.af_up_sets, f3, blk3, g_gpu.af_meta_u.buf,
        g_gpu.af_xq.buf, g_gpu.af_xs.buf, g_gpu.af_yb_dev, XQ_BYTES, XS_BYTES, 0u, VHG_Y2)
    var down_set = af_role_set(g_gpu.af_down_sets, f2, blk2, g_gpu.af_meta_d.buf,
        g_gpu.hq_dev, g_gpu.hs_dev, g_gpu.af_yo_dev, BATCH_HQ_BYTES, BATCH_HS_BYTES, VHG_HQ, VHG_YO)
    unsafe {
        // per-role regions: one (weight block, xblock) pair each
        fill_gemv_regions(g_gpu.af_meta_g, blk1)
        fill_gemv_regions(g_gpu.af_meta_u, blk3)
        fill_gemv_regions(g_gpu.af_meta_d, blk2)
        memcpy(g_gpu.af_xq.mapped, addr<void?>(xq[0]), n)
        memcpy(g_gpu.af_xs.mapped, addr<void?>(xs[0]), (n / xunit) * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        var pcg = GemvArgs(n = uint(n), d = uint(nfe), nreg = 1u, ybase = 0u)
        gemv_enc(f1, raw, h, gate_set, pcg, nfe)
        gemv_enc(f3, raw, h, up_set, pcg, nfe)
        // the requant form follows the DOWN plane — its GEMV consumes hq/hs
        var pca = ActArgs(nelem = uint(nfe), gelu = is_gelu ? 1u : 0u, nblk = uint(nfe / (dkq ? 256l : 32l)))
        let awgs = (nfe / (dkq ? 8l : 4l) + 255l) / 256l
        if (dkq) {
            enc_q8k_actrq_cls(raw, h, g_gpu.af_actrq_set, pca, awgs)
        } else {
            enc_q8_actrq_cls(raw, h, g_gpu.af_actrq_set, pca, awgs)
        }
        var pcd = GemvArgs(n = uint(nfe), d = uint(n), nreg = 1u, ybase = 0u)
        gemv_enc(f2, raw, h, down_set, pcd, n)
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, g_gpu.af_yo_dev, g_gpu.af_yo_host.buf, n * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(y[0]), g_gpu.af_yo_host.mapped, n * 4l)
    }
}

// single-region batch schedule over an arena block: 4-word record at 0, all tiles map to region
// 0 at map_off = 4. Returns the dispatch workgroup count. `cm2l` must match the dispatched
// class's tile rule; `nsplit` stacks split-k planes; `row0` (word 1) never pairs with `nsplit > 1`.
def private fill_arena_batch_sched(hb : HostBuf; d, blk, npos : int64; fmt : int; cm2l : bool = false; nsplit : int64 = 1l; row0 : int64 = 0l) : uint {
    let te = batch_tile_edges(fmt, d, npos, cm2l)
    let wtiles = (d + te.td - 1l) / te.td
    let rtiles = (npos + te.tc - 1l) / te.tc
    let total = rtiles * wtiles * nsplit
    assert((4l + total) * 4l <= BATCH_META_BYTES, "dasLLAMA vulkan arena: batch schedule exceeds capacity")
    unsafe {
        var mp = reinterpret<uint?>(hb.mapped)
        mp[0] = uint(arena_local_blk(blk))   // slab-local block base (already in fmt units)
        mp[1] = uint(row0)  // rb0 - r0
        mp[2] = uint(npos)  // cnt
        mp[3] = 0u          // wg base
        for (g in range64(total)) {
            mp[4l + g] = 0u
        }
    }
    return uint(total)
}

//! Arena seam: batch GEMM of the [d x n] tensor at block `blk` against `npos` quantized activation
//! rows (xq/xs, [npos x n]) into y[0..npos*d). The prefill twin of vk_arena_gemv (q8 only for now).
def vk_arena_gemm(var y : array<float>; blk : int64; n, d, npos : int64; fmt : int; xq : array<int8>; xs : array<float>) {
    verify(vk_moe_init())
    assert(fmt == int(KqFmt.q8), "dasLLAMA vulkan arena_gemm: q8 only")
    assert(npos * n <= BATCH_XQ_BYTES && npos * d * 4l <= BATCH_Y_BYTES, "arena_gemm: batch exceeds scratch")
    arena_ensure_batch(fmt)
    let v = q8_batch_cls_variant(q8_batch_tile_for(d, npos), q8_batch_aligned(d, npos))
    let skey = arena_slab_idx(blk) * 8 + v
    unsafe {
        var a & = g_gpu.arenas[fmt]
        if (!key_exists(a.cls_batch_sets, skey)) {   // dies with the arena on model drop
            verify(q8_batch_cls_ensure(v), "vulkan arena: the batch class rail must engage on a live device")
            let p = arena_planes(fmt, blk)
            a.cls_batch_sets[skey] = q8_batch_cls_set(v,
                fixed_array(p.wq, p.ws, a.batch_meta.buf, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, g_gpu.batch_y1_dev),
                fixed_array(p.wqb, p.wsb, BATCH_META_BYTES, BATCH_XQ_BYTES, BATCH_XS_BYTES, BATCH_Y_BYTES),
                fixed_array(0u, 0u, 0u, 0u, 0u, VHG_YO))
        }
        let groups = fill_arena_batch_sched(a.batch_meta, d, blk, npos, fmt)
        upload_region_at(g_gpu.batch_xq_dev, 0l, addr<uint8 const?>(xq[0]), npos * n)
        upload_region_at(g_gpu.batch_xs_dev, 0l, addr<uint8 const?>(xs[0]), npos * (n / 32l) * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        var pc = BatchArgs(n = uint(n), d = uint(d), map_off = 4u)
        q8_batch_cls_enc(v, raw, h, a.cls_batch_sets[skey], pc, int64(groups))
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, npos * d * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(addr<void?>(y[0]), g_gpu.batch_y1.mapped, npos * d * 4l)
    }
}

//! Arena seam: one GEMV of the [d x n] tensor at block index `blk` into y[0..d). Same kernels as
//! the per-op tier — only the weight base is an arena index, not stack-relative.
def vk_arena_gemv(var y : array<float>; blk : int64; n, d : int64; fmt : int; xq : array<int8>; xs : array<float>) {
    assert(g_gpu != null && key_exists(g_gpu.arenas, fmt), "dasLLAMA vulkan arena: format not reserved")
    var yp = unsafe(addr(y[0]))
    let xqp = unsafe(addr(xq[0]))
    let xsp = unsafe(addr(xs[0]))
    arena_ensure_scratch(fmt)
    let unit = fmt_unit(fmt)   // q8 takes 32-block Q8_0 activations
    let nb = n / unit
    assert(n <= XQ_BYTES && nb * 4l <= XS_BYTES, "dasLLAMA vulkan arena: activations exceed scratch capacity")
    assert(d * 4l <= Y_BYTES, "dasLLAMA vulkan arena: gemv output exceeds y capacity")
    let sl = arena_slab_idx(blk)
    unsafe {
        var a & = g_gpu.arenas[fmt]
        if (!key_exists(a.cls_sets, sl)) {   // dies with the arena on model drop — rebuilt over the new scratch
            verify(gemv_cls_ensure(fmt), "vulkan arena: the gemv class rail must engage on a live device")
            let p = arena_planes(fmt, blk)
            a.cls_sets[sl] = gemv_cls_set(fmt,
                fixed_array(p.wq, p.ws, a.meta.buf, a.xq.buf, a.xs.buf, a.y_dev),
                fixed_array(p.wqb, p.wsb, META_BYTES, XQ_BYTES, XS_BYTES, Y_BYTES),
                fixed_array(0u, 0u, 0u, 0u, 0u, VHG_YO))
        }
        var mp = reinterpret<uint?>(a.meta.mapped)
        mp[0] = uint(arena_local_blk(blk))   // one region: the whole tensor at its slab base
        mp[1] = 0u          // activation block base
        memcpy(a.xq.mapped, reinterpret<void?>(xqp), n)   // one int8 quant per weight
        memcpy(a.xs.mapped, reinterpret<void?>(xsp), nb * 4l)
        var raw = alloc_cmd()
        let begin = VkCommandBufferBeginInfo()
        vk_check(vkBeginCommandBuffer(raw, begin), null)
        var h : VkHaz
        var pc = GemvArgs(n = uint(n), d = uint(d), nreg = 1u, ybase = 0u)
        gemv_enc(fmt, raw, h, a.cls_sets[sl], pc, d)
        vhz_dep(raw, h, VHG_YO, 0u, true)
        cmd_copy_whole(raw, a.y_dev, a.y.buf, d * 4l)
        vk_check(vkEndCommandBuffer(raw), null)
        submit_wait(raw)
        memcpy(reinterpret<void?>(yp), a.y.mapped, d * 4l)
    }
}

// ===== prefill batch dispatch (the FFN's batched-GEMM arm) =====

// the fused act+requant class sets (both forms — the mid-step form follows the down stack)
def private ensure_batch_acts_cls {
    if (g_gpu.batch_acts_cls_ready) {
        return
    }
    verify(ensure_q8_actrq_cls() && ensure_q8k_actrq_cls(), "vk batch: the actrq class rails must engage on a live device")
    let abufs = fixed_array(g_gpu.batch_y1_dev, g_gpu.batch_y2_dev, g_gpu.hq_dev, g_gpu.hs_dev)
    let asizes = fixed_array(BATCH_Y_BYTES, BATCH_Y_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES)
    let agbits = fixed_array(VHB_Y1, VHB_Y2, VHB_HQ, VHB_HQ)
    g_gpu.batch_actrq_set = set_actrq_cls(abufs, asizes, agbits)   // one family set serves both forms
    g_gpu.batch_acts_cls_ready = true
}

def private ensure_comb_state {
    if (g_gpu.comb_ready) {
        return
    }
    g_gpu.comb_stage = make_host_buf(2l * COMB_WI_BYTES, false)   // transfer_src staging: w then inv
    g_gpu.comb_w_dev = make_device_buf(COMB_WI_BYTES)
    g_gpu.comb_inv_dev = make_device_buf(COMB_WI_BYTES)
    g_gpu.comb_y_dev = make_device_buf(BATCH_Y_BYTES)
    g_gpu.comb_y = make_host_buf(BATCH_Y_BYTES, true, [cached = true])
    // combine (class: accumulator rows, slot map, slot weights, down rows)
    verify(ensure_combine_cls(), "vk combine: the class rail must engage on a live device")
    g_gpu.comb_set = set_combine_cls(
        fixed_array(g_gpu.comb_y_dev, g_gpu.comb_inv_dev, g_gpu.comb_w_dev, g_gpu.batch_y1_dev),
        fixed_array(BATCH_Y_BYTES, COMB_WI_BYTES, COMB_WI_BYTES, BATCH_Y_BYTES),
        fixed_array(VHB_CY, VHB_COMB, VHB_COMB, VHB_Y1))
    g_gpu.comb_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: device-side moe combine engaged\n")
}

// batch bindings differ from decode's: meta is the stack's BATCH table, activations point at a
// role-fixed image (gate/up: the gathered x scratch; down: the requantized hidden planes), y one
// of the two shared device planes (gate/down write y1_dev, up y2_dev)
def private ensure_batch_set(si : int; ybuf, xqbuf, xsbuf : uint64; xqbytes, xsbytes : int64; xbit, ybit : uint; ybytes : int64 = BATCH_Y_BYTES) {
    if (g_gpu.stacks[si].batch_made && g_gpu.stacks[si].batch_y == ybuf && g_gpu.stacks[si].batch_xq == xqbuf) {
        return
    }
    if (!g_gpu.stacks[si].batch_made) {
        g_gpu.stacks[si].batch_meta = make_host_buf(BATCH_META_BYTES, false)   // transfer_src staging
        g_gpu.stacks[si].batch_meta_dev = make_device_buf(BATCH_META_BYTES)
        g_gpu.stacks[si].batch_made = true
    }
    // a plane rebind invalidates every class-variant set built over the old planes
    g_gpu.stacks[si].batch_cls_ready = 0u
    g_gpu.stacks[si].batch_y = ybuf
    g_gpu.stacks[si].batch_xq = xqbuf
    g_gpu.stacks[si].batch_xs = xsbuf
    g_gpu.stacks[si].batch_xq_bytes = xqbytes
    g_gpu.stacks[si].batch_xs_bytes = xsbytes
    g_gpu.stacks[si].batch_y_bytes = ybytes
    g_gpu.stacks[si].batch_xbit = xbit
    g_gpu.stacks[si].batch_ybit = ybit
}

// chunk row cap: the gathered x window, both hidden planes, the requant planes, and the down
// output (which reuses y1) must all fit their buffers. Two activation units: `in` = gate/up
// image, `mid` = intermediate (follows the DOWN stack, may differ). 32 for Q8_0, 256 for Q8_K.
def private ffn_chunk_rows(n, nfe, xunit_in, xunit_mid : int64) : int64 {
    var r = min(BATCH_XQ_BYTES / n, BATCH_XS_BYTES / ((n / xunit_in) * 4l))
    r = min(r, BATCH_Y_BYTES / (max(nfe, n) * 4l))
    r = min(r, min(BATCH_HQ_BYTES / nfe, BATCH_HS_BYTES / ((nfe / xunit_mid) * 4l)))
    // tile-aligned so a partial last tile never reads past a buffer-derived cap (cmi8 loads unstaged)
    return min(r, BATCH_CHUNK_ROWS) / BATCH_TILE * BATCH_TILE
}

// stage the chunk's activation rows [r0, r0+rows) into the staging window (xq at 0, xs at
// BATCH_XQ_BYTES); the staging->device copies ride the HEAD of the chain submit — a separate
// synchronous copy submit here measured a full extra round trip per chunk
def private upload_batch_acts(xqp : int8 const?; xsp : float const?; r0, rows, n, xunit : int64) {
    let xq_bytes = rows * n
    let xs_bytes = rows * (n / xunit) * 4l
    unsafe {
        var sp = reinterpret<uint8?>(g_gpu.staging.mapped)
        par_memcpy(g_gpu.staging.mapped, reinterpret<void?>(xqp + r0 * n), xq_bytes)
        par_memcpy(reinterpret<void?>(sp + BATCH_XQ_BYTES), reinterpret<void?>(xsp + r0 * (n / xunit)), xs_bytes)
    }
}

// the chain-head acts copies the fold above defers to record time
def private cmd_copy_batch_acts(raw : VkCommandBuffer; xq_bytes, xs_bytes : int64) {
    var qregions : array<BufferCopy>
    qregions |> push(BufferCopy(srcOffset = 0ul, dstOffset = 0ul, size = uint64(xq_bytes)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(g_gpu.staging.buf), nonowning_buf(g_gpu.batch_xq_dev), qregions)
    var sregions : array<BufferCopy>
    sregions |> push(BufferCopy(srcOffset = uint64(BATCH_XQ_BYTES), dstOffset = 0ul, size = uint64(xs_bytes)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(g_gpu.staging.buf), nonowning_buf(g_gpu.batch_xs_dev), sregions)
}

// build the chunk's schedule: regions clipped to the row window [r0, r1) (row0 re-based to the
// window), then the per-workgroup region map. Weight offsets follow the stack's format units.
// Returns the workgroup count; `n` rides the push block (param removal = the deletion sweep's)
[unused_argument(n), arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def private fill_batch_meta(si : int; offs : int64 const?; nregions : int64; n, d : int64; r0, r1 : int64; cm2 : bool = false) : uint {
    let fmt = g_gpu.stacks[si].fmt
    let unit = fmt_unit(fmt)
    unsafe {
        var mp = reinterpret<uint?>(g_gpu.stacks[si].batch_meta.mapped)
        var nrc = 0l
        var nrows = 0l
        var all128 = true   // every active region's cnt a 128-multiple -> aligned-L eligible
        for (r in range64(nregions)) {
            let b0 = max(offs[r * 3l + 1l], r0)
            let b1 = min(offs[r * 3l + 1l] + offs[r * 3l + 2l], r1)
            if (b1 > b0) {
                nrc++
                nrows += b1 - b0
                all128 = all128 && (b1 - b0) % 128l == 0l
            }
        }
        let te = batch_tile_edges(fmt, d, nrc > 0l ? nrows / nrc : r1 - r0, cm2)
        g_gpu.stacks[si].batch_tile = cm2 ? te.tc : te.tile
        g_gpu.stacks[si].batch_aligned = !cm2 && fmt == int(KqFmt.q8) && te.tile == 128l && d % 128l == 0l && all128
        let wtiles = (d + te.td - 1l) / te.td
        let wg_bound = ((r1 - r0 + te.tc - 1l) / te.tc + nrc) * wtiles
        assert((nrc * 4l + wg_bound) * 4l <= BATCH_META_BYTES, "dasLLAMA vulkan tier: batch schedule exceeds capacity")
        // class layout: 4-word records at 0, the per-wg map at map_off (n/d ride the push block)
        let map_off = nrc * 4l
        g_gpu.stacks[si].batch_map_off = map_off
        var ri = 0l
        var wg = 0l
        for (r in range64(nregions)) {
            let rb0 = max(offs[r * 3l + 1l], r0)
            let rb1 = min(offs[r * 3l + 1l] + offs[r * 3l + 2l], r1)
            if (rb1 <= rb0) {
                continue
            }
            let rel = offs[r * 3l] - g_gpu.stacks[si].base
            assert(rel % unit == 0l, "dasLLAMA vulkan tier: unaligned batch region weight offset")
            let mb = ri * 4l
            mp[mb] = uint(rel / unit)
            mp[mb + 1l] = uint(rb0 - r0)
            mp[mb + 2l] = uint(rb1 - rb0)
            mp[mb + 3l] = uint(wg)
            let rwg = ((rb1 - rb0 + te.tc - 1l) / te.tc) * wtiles
            for (g in range64(rwg)) {
                mp[map_off + wg + g] = uint(ri)
            }
            wg += rwg
            ri++
        }
        return uint(wg)
    }
}

let private BATCH_CLS_CM2S = 7   // batch_cls_sets slots of the cm2 s / m / l tiles
let private BATCH_CLS_CM2M = 8
let private BATCH_CLS_CM2L = 9

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def private stack_batch_enc_cm2(raw : VkCommandBuffer; var h : VkHaz; si : int; nwg : uint; n, d : int64) {
    unsafe {
        var st & = g_gpu.stacks[si]
        let tc = st.batch_tile
        let v = tc == 256l ? BATCH_CLS_CM2L : (tc == 128l ? BATCH_CLS_CM2M : BATCH_CLS_CM2S)
        if ((st.batch_cls_ready & (1u << uint(v))) == 0u) {
            verify(cm2_cls_ensure(st.fmt, tc), "vk batch: the cm2 tile class rail must engage")
            st.batch_cls_sets[v] = cm2_cls_set(st.fmt, tc,
                fixed_array(st.wqbuf, st.wsbuf, st.batch_meta_dev, st.batch_xq, st.batch_y),
                fixed_array(st.wqbytes, ws_range(si), BATCH_META_BYTES, st.batch_xq_bytes, st.batch_y_bytes),
                fixed_array(0u, 0u, VHB_META, st.batch_xbit, st.batch_ybit))
            st.batch_cls_ready |= 1u << uint(v)
        }
        var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(st.batch_map_off))
        cm2_cls_enc(st.fmt, tc, raw, h, st.batch_cls_sets[v], pc, int64(nwg))
    }
}

// one stack batch tile on the class rail: the class picked by the stack's format (q8 rides the
// tile router; kq formats are fixed), a per-(stack, variant) cached set over the planes
// ensure_batch_set recorded, BatchArgs push. n/d must MATCH the fill's values.
def private stack_batch_enc(raw : VkCommandBuffer; var h : VkHaz; si : int; nwg : uint; n, d : int64) {
    unsafe {
        var st & = g_gpu.stacks[si]
        let fmt = st.fmt
        // Q4_0 rides the f16 coopmat arm too (no s8-native for nibbles — mode 2 falls back to sdot4)
        let q40cm = fmt == int(KqFmt.q40) && g_gpu.coopmat_mode == COOPMAT_F16
        var v = 6   // the stack's fixed class (kq); 5 = the q40 f16 MMA twin
        if (fmt == int(KqFmt.q8)) {
            v = q8_batch_cls_variant(st.batch_tile, st.batch_aligned)
        } elif (q40cm) {
            v = 5
        }
        if ((st.batch_cls_ready & (1u << uint(v))) == 0u) {
            let bufs = fixed_array(st.wqbuf, st.wsbuf, st.batch_meta_dev, st.batch_xq, st.batch_xs, st.batch_y)
            let sizes = fixed_array(st.wqbytes, ws_range(si), BATCH_META_BYTES, st.batch_xq_bytes, st.batch_xs_bytes, st.batch_y_bytes)
            let gbits = fixed_array(0u, 0u, VHB_META, st.batch_xbit, st.batch_xbit, st.batch_ybit)
            if (fmt == int(KqFmt.q8)) {
                verify(q8_batch_cls_ensure(v), "vk batch: the q8 batch class rail must engage")
                st.batch_cls_sets[v] = q8_batch_cls_set(v, bufs, sizes, gbits)
            } elif (q40cm || kq_sb(fmt)) {
                verify(kq_batch_cls_ensure(fmt, q40cm), "vk batch: the kq batch class rail must engage")
                st.batch_cls_sets[v] = kq_batch_cls_set_for(fmt, q40cm, bufs, sizes, gbits)
            } else {
                panic("dasLLAMA vulkan tier: no batch class for stack format {fmt}")
            }
            st.batch_cls_ready |= 1u << uint(v)
        }
        var pc = BatchArgs(n = uint(n), d = uint(d), map_off = uint(st.batch_map_off))
        if (fmt == int(KqFmt.q8)) {
            q8_batch_cls_enc(v, raw, h, st.batch_cls_sets[v], pc, int64(nwg))
        } elif (q40cm || kq_sb(fmt)) {
            kq_batch_cls_enc_for(fmt, q40cm, raw, h, st.batch_cls_sets[v], pc, int64(nwg))
        } else {
            panic("dasLLAMA vulkan tier: no batch class for stack format {fmt}")
        }
    }
}

def private cmd_copy_batch_meta(raw : VkCommandBuffer; si : int) {
    var regions : array<BufferCopy>
    regions |> push(BufferCopy(srcOffset = 0ul, dstOffset = 0ul, size = uint64(BATCH_META_BYTES)))
    cmd_copy_buffer(vk_value_to_boost(raw), nonowning_buf(g_gpu.stacks[si].batch_meta.buf),
                    nonowning_buf(g_gpu.stacks[si].batch_meta_dev), regions)
}

// one reusable cmd buffer, re-recorded per submit — prefill submits are few and multi-ms, so
// recording cost is noise. comb_npos > 0: down rows stay device-side, accumulating into comb_y_dev;
// only the last chunk appends the k-fold-smaller combined DMA. Hidden planes never leave the device.
def private record_ffn_batch_cmd(s1, s3, s2 : int; nwg_gu, nwg_dn : uint; rows, n, nfe, gout_bytes : int64;
                                 axq_bytes, axs_bytes, mid_nblk : int64; is_gelu : bool;
                                 comb_npos : uint = 0u; comb_first : bool = false; comb_last : bool = false;
                                 comb_wi_bytes : int64 = 0l; comb_y_bytes : int64 = 0l;
                                 comb_n : int64 = 0l; comb_k : int64 = 0l; comb_r0 : int64 = 0l; comb_r1 : int64 = 0l) {
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    // one declaration per same-region copy GROUP — per-copy would fake WAW on the shared bit
    vhz_dep(raw, h, 0u, VHB_ACT, true)
    cmd_copy_batch_acts(raw, axq_bytes, axs_bytes)
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, s1)
    cmd_copy_batch_meta(raw, s3)
    cmd_copy_batch_meta(raw, s2)
    if (comb_npos > 0u && comb_first) {
        vhz_dep(raw, h, 0u, VHB_COMB, true)
        cmd_copy_whole(raw, g_gpu.comb_stage.buf, g_gpu.comb_w_dev, comb_wi_bytes)
        cmd_copy_range(raw, g_gpu.comb_stage.buf, COMB_WI_BYTES, g_gpu.comb_inv_dev, 0l, comb_wi_bytes)
    }
    stack_batch_enc(raw, h, s1, nwg_gu, n, nfe)
    stack_batch_enc(raw, h, s3, nwg_gu, n, nfe)
    // the mid-step form follows the DOWN stack — its GEMM is the consumer of the hidden plane
    var pca = ActArgs(nelem = uint(rows * nfe), gelu = is_gelu ? 1u : 0u, nblk = uint(mid_nblk))
    let dkq = g_gpu.stacks[s2].fmt != 0
    let awgs = (rows * nfe / (dkq ? 8l : 4l) + 255l) / 256l
    if (dkq) {
        enc_q8k_actrq_cls(raw, h, g_gpu.batch_actrq_set, pca, awgs)
    } else {
        enc_q8_actrq_cls(raw, h, g_gpu.batch_actrq_set, pca, awgs)
    }
    stack_batch_enc(raw, h, s2, nwg_dn, nfe, n)
    if (comb_npos > 0u) {
        var pcc = CombineArgs(n = uint(comb_n), k = uint(comb_k), r0 = uint(comb_r0), r1 = uint(comb_r1))
        enc_combine_cls(raw, h, g_gpu.comb_set, pcc, int64(comb_npos))
        if (comb_last) {
            vhz_dep(raw, h, VHB_CY, 0u, true)
            cmd_copy_whole(raw, g_gpu.comb_y_dev, g_gpu.comb_y.buf, comb_y_bytes)
        }
    } else {
        vhz_dep(raw, h, VHB_Y1, 0u, true)
        cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, gout_bytes)
    }
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_ffnb hz: {h.disp} nodes, {h.barriers} barriers\n")
    }
    vk_check(vkEndCommandBuffer(raw), null)
}

// batch arm: chunked over row windows, down GEMM reuses y1 for output. npos > 0: the combine
// kernel folds each chunk's down rows into comb_y_dev, read back once after the last chunk.
def vk_moe_ffn_batch(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    let s1 = find_stack(unsafe(offs1[0]), f1, "prefill ffn gate")
    let s3 = find_stack(unsafe(offs3[0]), f3, "prefill ffn up")
    let s2 = find_stack(unsafe(offs2[0]), f2, "prefill ffn down")
    // gate/up share one activation image (forms must agree); down's intermediate is its own unit
    let kq_in = !vk_fmt_b32(f1)
    assert(!vk_fmt_b32(f3) == kq_in,
        "dasLLAMA vulkan tier: gate/up form mismatch - they share one activation image")
    let xunit_in = fmt_unit(f1)
    let xunit_mid = fmt_unit(f2)
    ensure_batch_state()
    ensure_batch_acts_cls()
    ensure_batch_set(s1, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
    ensure_batch_set(s3, g_gpu.batch_y2_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y2)
    ensure_batch_set(s2, g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.hs_dev, BATCH_HQ_BYTES, BATCH_HS_BYTES, VHB_HQ, VHB_Y1)
    let comb = npos > 0l
    var comb_k = 0l
    if (comb) {
        ensure_comb_state()
        comb_k = nrows / npos
        assert(comb_k * npos == nrows, "dasLLAMA vulkan tier: combined rows must be npos x k")
        assert(nrows * 4l <= COMB_WI_BYTES && npos * n * 4l <= BATCH_Y_BYTES,
            "dasLLAMA vulkan tier: combined output exceeds capacity")
        unsafe {
            var cp = reinterpret<uint8?>(g_gpu.comb_stage.mapped)
            par_memcpy(g_gpu.comb_stage.mapped, reinterpret<void?>(wp), nrows * 4l)
            par_memcpy(reinterpret<void?>(cp + COMB_WI_BYTES), reinterpret<void?>(invp), nrows * 4l)
        }
    }
    let chunk = ffn_chunk_rows(n, nfe, xunit_in, xunit_mid)
    var r0 = 0l
    while (r0 < nrows) {
        let r1 = min(r0 + chunk, nrows)
        let rows = r1 - r0
        let ts0 = ref_time_ticks()
        let nwg1 = fill_batch_meta(s1, offs1, nregions, n, nfe, r0, r1)
        let nwg3 = fill_batch_meta(s3, offs3, nregions, n, nfe, r0, r1)
        assert(nwg1 == nwg3, "dasLLAMA vulkan tier: fused batch pair region mismatch")
        let nwg2 = fill_batch_meta(s2, offs2, nregions, nfe, n, r0, r1)
        let ts1 = ref_time_ticks()
        upload_batch_acts(xqp, xsp, r0, rows, n, xunit_in)
        let ts2 = ref_time_ticks()
        record_ffn_batch_cmd(s1, s3, s2, nwg1, nwg2, rows, n, nfe, rows * n * 4l, rows * n, rows * (n / xunit_in) * 4l,
            rows * nfe / xunit_mid, is_gelu,
            [comb_npos = comb ? uint(npos) : 0u, comb_first = r0 == 0l, comb_last = r1 == nrows,
             comb_wi_bytes = nrows * 4l, comb_y_bytes = npos * n * 4l,
             comb_n = comb ? n : 0l, comb_k = comb_k, comb_r0 = r0, comb_r1 = r1])
        let ts3 = ref_time_ticks()
        submit_wait(g_gpu.batch_cmd)
        let ts4 = ref_time_ticks()
        if (!comb) {
            unsafe {
                par_memcpy(reinterpret<void?>(goutp + r0 * n), g_gpu.batch_y1.mapped, rows * n * 4l)
            }
        }
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_ffn_batch acts {get_time_usec(ts1) - get_time_usec(ts2)} submit {get_time_usec(ts3) - get_time_usec(ts4)} copy {get_time_usec(ts4)}\n")
        }
        r0 = r1
    }
    if (comb) {
        unsafe {
            par_memcpy(reinterpret<void?>(goutp), g_gpu.comb_y.mapped, npos * n * 4l)
        }
    }
}

// one dense chunk: acts + meta staging copies, one tile GEMM over the single whole-plane
// region, y DMA
def private record_dense_cmd(si : int; nwg : uint; n, d, y_bytes : int64; axq_bytes, axs_bytes : int64) {
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    vhz_dep(raw, h, 0u, VHB_ACT, true)
    cmd_copy_batch_acts(raw, axq_bytes, axs_bytes)
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, si)
    stack_batch_enc(raw, h, si, nwg, n, d)
    vhz_dep(raw, h, VHB_Y1, 0u, true)
    cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, y_bytes)
    vk_check(vkEndCommandBuffer(raw), null)
}

//! MoeGpuDenseFn: dense batched GEMM — y [nrows x d] = X · Wᵀ over the resident plane
//! ([d x n] at element offset woff), a one-region ride on the batch tile rail. Chunked like
//! the FFN arm; activations arrive pre-quantized in the plane's own form.
def vk_moe_dense(var yp : float?; woff : int64; xqp : int8 const?; xsp : float const?; n, d, nrows : int64; fmt : int) {
    let si = find_stack(woff, fmt, "dense batch")
    let xunit = fmt_unit(fmt)
    ensure_batch_state()
    ensure_batch_set(si, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
    var chunk = min(BATCH_XQ_BYTES / n, BATCH_XS_BYTES / ((n / xunit) * 4l))
    chunk = min(chunk, BATCH_Y_BYTES / (d * 4l))
    chunk = min(chunk, BATCH_CHUNK_ROWS) / BATCH_TILE * BATCH_TILE
    var region = fixed_array(woff, 0l, nrows)
    var r0 = 0l
    while (r0 < nrows) {
        let r1 = min(r0 + chunk, nrows)
        let rows = r1 - r0
        let ts0 = ref_time_ticks()
        var nwg = 0u
        unsafe {
            nwg = fill_batch_meta(si, addr(region[0]), 1l, n, d, r0, r1)
        }
        let ts1 = ref_time_ticks()
        upload_batch_acts(xqp, xsp, r0, rows, n, xunit)
        let ts2 = ref_time_ticks()
        record_dense_cmd(si, nwg, n, d, rows * d * 4l, rows * n, rows * (n / xunit) * 4l)
        let ts3 = ref_time_ticks()
        submit_wait(g_gpu.batch_cmd)
        let ts4 = ref_time_ticks()
        unsafe {
            par_memcpy(reinterpret<void?>(yp + r0 * d), g_gpu.batch_y1.mapped, rows * d * 4l)
        }
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_dense acts {get_time_usec(ts1) - get_time_usec(ts2)} submit {get_time_usec(ts3) - get_time_usec(ts4)} copy {get_time_usec(ts4)}\n")
        }
        r0 = r1
    }
}

// ===== deltanet dispatch (the recurrent-layer attention chain) =====

// the window chain's class sets over the dn scratch (built here — common cannot require classes)
def private ensure_dn_cls_sets {
    if (g_gpu.dn_cls_ready) {
        return
    }
    verify(ensure_dn_conv_cls() && ensure_dn_scan_cls() && ensure_dn_scan_p3_cls() && ensure_cls_dn_rq(),
        "vk deltanet chain: the class rails must engage on a live device")
    g_gpu.dn_conv_set = set_dn_conv_cls(
        fixed_array(g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.dn_smalls_dev),
        fixed_array(BATCH_Y_BYTES, BATCH_HQ_BYTES, DN_SMALLS_BYTES),
        fixed_array(VHB_Y1, VHB_HQ, VHB_SM))
    g_gpu.dn_scan_set = set_dn_scan_cls(
        fixed_array(g_gpu.hq_dev, g_gpu.dn_state_dev, g_gpu.dn_smalls_dev, g_gpu.dn_ws_dev),
        fixed_array(BATCH_HQ_BYTES, DN_STATE_BYTES, DN_SMALLS_BYTES, DN_WS_BYTES),
        fixed_array(VHB_HQ, VHB_ST, VHB_SM, VHB_WS))
    g_gpu.dn_scan_p3_set = set_dn_scan_p3_cls(
        fixed_array(g_gpu.batch_y2_dev, g_gpu.dn_ws_dev, g_gpu.dn_smalls_dev),
        fixed_array(BATCH_Y_BYTES, DN_WS_BYTES, DN_SMALLS_BYTES),
        fixed_array(VHB_Y2, VHB_WS, VHB_SM))
    g_gpu.dn_rq_set = set_rq_cls(
        fixed_array(g_gpu.dn_ws_dev, g_gpu.hq_dev, g_gpu.hs_dev),
        fixed_array(DN_WS_BYTES, BATCH_HQ_BYTES, BATCH_HS_BYTES),
        fixed_array(VHB_WS, VHB_HQ, VHB_HQ))
    g_gpu.dn_cls_ready = true
}

// one deltanet window: acts/meta/smalls staging copies, qkv + z tiles, conv, scan, the conv-tail
// extraction (before out-proj overwrites the qkv rows in y1), o requant, out-proj tile, y DMA;
// the state uploads on the first window and reads back on the last
def private record_dn_cmd(s_qkv, s_z, s_o : int; nwg_qkv, nwg_z, nwg_out : uint;
                          rows, cd, kd, di, dim, taps, nvh, nkh, ds : int64;
                          axq_bytes, axs_bytes : int64; first, last : bool; eps : float) {
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    pfq_begin(raw)
    pfq_ts(raw)   // q0: submit start
    vhz_dep(raw, h, 0u, VHB_ACT, true)
    cmd_copy_batch_acts(raw, axq_bytes, axs_bytes)
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, s_qkv)
    cmd_copy_batch_meta(raw, s_z)
    cmd_copy_batch_meta(raw, s_o)
    // stops SHORT of the hist region — later windows read the device-copied tail rows there
    vhz_dep(raw, h, 0u, VHB_SM, true)
    cmd_copy_range(raw, g_gpu.dn_smalls.buf, 0l, g_gpu.dn_smalls_dev, 0l, DN_SM_HIST * 4l)
    if (first) {
        cmd_copy_range(raw, g_gpu.dn_smalls.buf, DN_SM_HIST * 4l, g_gpu.dn_smalls_dev, DN_SM_HIST * 4l, cd * taps * 4l)
        vhz_dep(raw, h, 0u, VHB_ST, true)
        cmd_copy_whole(raw, g_gpu.dn_state_stage.buf, g_gpu.dn_state_dev, nvh * ds * ds * 4l)
    }
    pfq_ts(raw)   // q1: uploads
    stack_batch_enc(raw, h, s_qkv, nwg_qkv, dim, cd)
    stack_batch_enc(raw, h, s_z, nwg_z, dim, di)
    pfq_ts(raw)   // q2: qkv+z GEMMs
    var pcc = DnConvArgs(npos = uint(rows), cd = uint(cd), kd = uint(kd), ds = uint(ds), dconv = uint(taps + 1l),
        hist_mode = first ? 1u : 2u, taps_off = uint(DN_SM_TAPS), hist_off = uint(DN_SM_HIST),
        eps = eps, qscale = 1.0 / sqrt(float(ds)))
    enc_dn_conv_cls(raw, h, g_gpu.dn_conv_set, pcc, dn_conv_wgs(cd, rows))
    pfq_ts(raw)   // q3: conv
    var pcs = DnScanArgs(npos = uint(rows), cd = uint(cd), kd = uint(kd), nvh = uint(nvh), nkh = uint(nkh), ds = uint(ds),
        beta_off = uint(DN_SM_BETA), g_off = uint(DN_SM_G), a_off = uint(DN_SM_A), dt_off = uint(DN_SM_DT),
        o_off = uint(DN_O_OFF), di = uint(di))
    enc_dn_scan_cls(raw, h, g_gpu.dn_scan_set, pcs, dn_scan_wgs(nvh, ds))
    pfq_ts(raw)   // q4: the scan
    var pc3 = DnScanP3Args(npos = uint(rows), nvh = uint(nvh), di = uint(di), ds = uint(ds), o_off = uint(DN_O_OFF),
        wnorm_off = uint(DN_SM_WNORM), eps = eps)
    enc_dn_scan_p3_cls(raw, h, g_gpu.dn_scan_p3_set, pc3, rows)
    pfq_ts(raw)   // q5: the out-norm
    // conv-tail extraction: last `taps` RAW qkv rows out of y1 before out-proj reuses it
    if (!last) {
        vhz_dep(raw, h, VHB_Y1, VHB_SM, true)
        cmd_copy_range(raw, g_gpu.batch_y1_dev, (rows - taps) * cd * 4l, g_gpu.dn_smalls_dev, DN_SM_HIST * 4l, taps * cd * 4l)
    } else {
        vhz_dep(raw, h, VHB_Y1, 0u, true)
        cmd_copy_range(raw, g_gpu.batch_y1_dev, (rows - taps) * cd * 4l, g_gpu.dn_tail_host.buf, 0l, taps * cd * 4l)
        vhz_dep(raw, h, VHB_ST, 0u, true)
        cmd_copy_whole(raw, g_gpu.dn_state_dev, g_gpu.dn_state_host.buf, nvh * ds * ds * 4l)
    }
    pfq_ts(raw)   // q6: tail copies
    var pcr = RqArgs(inbase = uint(DN_O_OFF), nblk = uint(rows * di / 32l))
    enc_cls_dn_rq(raw, h, g_gpu.dn_rq_set, pcr, (rows * di / 4l + 255l) / 256l)
    pfq_ts(raw)   // q7: o requant
    stack_batch_enc(raw, h, s_o, nwg_out, di, dim)
    pfq_ts(raw)   // q8: out GEMM
    vhz_dep(raw, h, VHB_Y1, 0u, true)
    cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, rows * dim * 4l)
    pfq_end(raw)
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_dn hz: {h.disp} nodes, {h.barriers} barriers\n")
    }
    vk_check(vkEndCommandBuffer(raw), null)
}

//! MoeGpuDnFn: one recurrent layer's whole deltanet attention block as one submit chain per
//! DN_WINDOW rows. yp gets npos x dim rows; tailp gets the last taps qkv rows for the caller's history store.
def vk_moe_dn(var yp : float?; woq, woz, woo : int64;
              xqp : int8 const?; xsp : float const?; bp : float const?; gp : float const?;
              convw : float const?; wnormp : float const?; ap : float const?; dtp : float const?;
              var stp : float?; var tailp : float?; histp : float const?;
              npos, dim, cd, kd, di, nvh, nkh, ds, dconv : int64; eps : float) {
    let s_qkv = find_stack(woq, 0, "dn qkv")
    let s_z = find_stack(woz, 0, "dn z-gate")
    let s_o = find_stack(woo, 0, "dn out")
    let taps = dconv - 1l
    if (npos == 1l) {   // the decode step — device-resident state, one pre-recorded submit
        vk_moe_dn_step(yp, s_qkv, s_z, s_o, woq, woz, woo, xqp, xsp, bp, gp,
            convw, wnormp, ap, dtp, stp, histp, dim, cd, kd, di, nvh, nkh, ds, dconv, eps)
        return
    }
    assert(ds <= DN_MAX_DS && ds % 16l == 0l && (ds & (ds - 1l)) == 0l && nvh <= DN_MAX_HEADS && cd <= DN_MAX_CD
           && 2l * kd <= DN_MAX_QK && dconv <= DN_MAX_DCONV && di == nvh * ds && kd == nkh * ds && cd == 2l * kd + di
           && npos > taps && dim % 32l == 0l && di % 32l == 0l,
        "dasLLAMA vulkan tier: deltanet geometry outside chain capacity")
    ensure_batch_state()
    ensure_dn_state()
    ensure_dn_cls_sets()
    ensure_batch_set(s_qkv, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
    ensure_batch_set(s_z, g_gpu.batch_y2_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y2)
    ensure_batch_set(s_o, g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.hs_dev, BATCH_HQ_BYTES, BATCH_HS_BYTES, VHB_HQ, VHB_Y1)
    unsafe {
        var sp = reinterpret<float?>(g_gpu.dn_smalls.mapped)
        sp[0] = eps
        sp[1] = 1.0 / sqrt(float(ds))
        memcpy(reinterpret<void?>(sp + DN_SM_TAPS), reinterpret<void?>(convw), cd * dconv * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_WNORM), reinterpret<void?>(wnormp), ds * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_A), reinterpret<void?>(ap), nvh * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_DT), reinterpret<void?>(dtp), nvh * 4l)
        memcpy(reinterpret<void?>(sp + DN_SM_HIST), reinterpret<void?>(histp), cd * taps * 4l)
        memcpy(g_gpu.dn_state_stage.mapped, reinterpret<void?>(stp), nvh * ds * ds * 4l)
    }
    var qkv_region = fixed_array(woq, 0l, 0l)
    var z_region = fixed_array(woz, 0l, 0l)
    var o_region = fixed_array(woo, 0l, 0l)
    var r0 = 0l
    while (r0 < npos) {
        let rows = min(DN_WINDOW, npos - r0)
        let ts0 = ref_time_ticks()
        qkv_region[2] = rows
        z_region[2] = rows
        o_region[2] = rows
        var nwg_qkv = 0u
        var nwg_z = 0u
        var nwg_out = 0u
        unsafe {
            nwg_qkv = fill_batch_meta(s_qkv, addr(qkv_region[0]), 1l, dim, cd, 0l, rows)
            nwg_z = fill_batch_meta(s_z, addr(z_region[0]), 1l, dim, di, 0l, rows)
            nwg_out = fill_batch_meta(s_o, addr(o_region[0]), 1l, di, dim, 0l, rows)
        }
        upload_batch_acts(xqp, xsp, r0, rows, dim, 32l)
        unsafe {
            var sp = reinterpret<float?>(g_gpu.dn_smalls.mapped)
            par_memcpy(reinterpret<void?>(sp + DN_SM_BETA), reinterpret<void?>(bp + r0 * nvh), rows * nvh * 4l)
            par_memcpy(reinterpret<void?>(sp + DN_SM_G), reinterpret<void?>(gp + r0 * nvh), rows * nvh * 4l)
        }
        let ts1 = ref_time_ticks()
        record_dn_cmd(s_qkv, s_z, s_o, nwg_qkv, nwg_z, nwg_out, rows, cd, kd, di, dim, taps, nvh, nkh, ds,
            rows * dim, rows * (dim / 32l) * 4l, r0 == 0l, r0 + rows == npos, eps)
        submit_wait(g_gpu.batch_cmd)
        let ts2 = ref_time_ticks()
        unsafe {
            par_memcpy(reinterpret<void?>(yp + r0 * dim), g_gpu.batch_y1.mapped, rows * dim * 4l)
        }
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_dn prep {get_time_usec(ts0) - get_time_usec(ts1)} submit {get_time_usec(ts1) - get_time_usec(ts2)} copy {get_time_usec(ts2)}\n")
            to_log(LOG_INFO, "vk_dn gpu upl {int(pfq_us(1u))} qkv_z {int(pfq_us(2u))} conv {int(pfq_us(3u))} scan_p1 {int(pfq_us(4u))} scan_p2 {int(pfq_us(5u))} tailcp {int(pfq_us(6u))} rq {int(pfq_us(7u))} out {int(pfq_us(8u))}\n")
        }
        r0 += rows
    }
    unsafe {
        par_memcpy(reinterpret<void?>(stp), g_gpu.dn_state_host.mapped, nvh * ds * ds * 4l)
        memcpy(reinterpret<void?>(tailp), g_gpu.dn_tail_host.mapped, taps * cd * 4l)
    }
}

// ===== attention dispatch (the full-attention-layer chain) =====

def private ensure_at_state {
    if (g_gpu.at_ready) {
        return
    }
    ensure_batch_state()
    verify(ensure_at_prep_cls() && ensure_at_attn_cls() && ensure_cls_q8k_rq() && ensure_cls_dn_rq(),
        "vk attention chain: the class rails must engage on a live device")
    g_gpu.at_smalls = make_host_buf(AT_SMALLS_BYTES, false)
    g_gpu.at_smalls_dev = make_device_buf(AT_SMALLS_BYTES)
    g_gpu.at_q_dev = make_device_buf(AT_WINDOW * AT_MAX_QD * 4l)
    g_gpu.at_g_dev = make_device_buf(AT_WINDOW * AT_MAX_QD * 4l)
    g_gpu.at_k_dev = make_device_buf(AT_CTX * AT_MAX_KV * 4l)
    g_gpu.at_v_dev = make_device_buf(AT_CTX * AT_MAX_KV * 4l)
    g_gpu.at_vraw_dev = make_device_buf(AT_WINDOW * AT_MAX_KV * 4l)
    g_gpu.at_kv_host = make_host_buf(2l * AT_WINDOW * AT_MAX_KV * 4l, true, [cached = true])
    g_gpu.at_bias_dev = make_device_buf((AT_MAX_QD + 2l * AT_MAX_KV) * 4l)
    // prep (class: GEMM rows, prepped panel, gate stash, smalls, projection bias) — the k/v forms' gate view never fires
    g_gpu.at_prep_q_set = set_at_prep_cls(
        fixed_array(g_gpu.batch_y1_dev, g_gpu.at_q_dev, g_gpu.at_g_dev, g_gpu.at_smalls_dev, g_gpu.at_bias_dev),
        fixed_array(BATCH_Y_BYTES, AT_WINDOW * AT_MAX_QD * 4l, AT_WINDOW * AT_MAX_QD * 4l, AT_SMALLS_BYTES, (AT_MAX_QD + 2l * AT_MAX_KV) * 4l),
        fixed_array(VHB_Y1, VHB_ATQ, VHB_ATG, VHB_SM, VHB_ATBIAS))
    g_gpu.at_prep_k_set = set_at_prep_cls(
        fixed_array(g_gpu.batch_y2_dev, g_gpu.at_k_dev, g_gpu.at_g_dev, g_gpu.at_smalls_dev, g_gpu.at_bias_dev),
        fixed_array(BATCH_Y_BYTES, AT_CTX * AT_MAX_KV * 4l, AT_WINDOW * AT_MAX_QD * 4l, AT_SMALLS_BYTES, (AT_MAX_QD + 2l * AT_MAX_KV) * 4l),
        fixed_array(VHB_Y2, VHB_ATK, 0u, VHB_SM, VHB_ATBIAS))
    // IN PLACE on at_vraw: src and outp are the same buffer
    g_gpu.at_prep_v_set = set_at_prep_cls(
        fixed_array(g_gpu.at_vraw_dev, g_gpu.at_vraw_dev, g_gpu.at_g_dev, g_gpu.at_smalls_dev, g_gpu.at_bias_dev),
        fixed_array(AT_WINDOW * AT_MAX_KV * 4l, AT_WINDOW * AT_MAX_KV * 4l, AT_WINDOW * AT_MAX_QD * 4l, AT_SMALLS_BYTES, (AT_MAX_QD + 2l * AT_MAX_KV) * 4l),
        fixed_array(VHB_VRAW, VHB_VRAW, VHB_ATG, VHB_SM, VHB_ATBIAS))
    // attn (class: q panel, k panel, v rows, gate/out plane)
    g_gpu.at_attn_set = set_at_attn_cls(
        fixed_array(g_gpu.at_q_dev, g_gpu.at_k_dev, g_gpu.at_v_dev, g_gpu.at_g_dev),
        fixed_array(AT_WINDOW * AT_MAX_QD * 4l, AT_CTX * AT_MAX_KV * 4l, AT_CTX * AT_MAX_KV * 4l, AT_WINDOW * AT_MAX_QD * 4l),
        fixed_array(VHB_ATQ, VHB_ATK, VHB_ATV, VHB_ATG))
    // o requant: ONE family set serves both forms — the encoder picks the kernel per window
    g_gpu.at_rq_set = set_rq_cls(fixed_array(g_gpu.at_g_dev, g_gpu.hq_dev, g_gpu.hs_dev),
        fixed_array(AT_WINDOW * AT_MAX_QD * 4l, BATCH_HQ_BYTES, BATCH_HS_BYTES),
        fixed_array(VHB_ATG, VHB_HQ, VHB_HQ))
    g_gpu.at_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: attention chain engaged\n")
}

//! Does the cm2 flash-attention tile serve this window? A coopmat2-fa device, the fa knob, head 64
//! or 128, and no gate (this chain wires neither the h256 stamps nor their gated epilogue - gated models keep at_attn).
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain")]
def private at_fa_serves(hs : int64; gated : bool) : bool {
    return !gated && g_gpu.has_coopmat2_fa && vk_fa_on() && (hs == 64l || hs == 128l)
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain")]
def private ensure_at_fa_state(hs : int64) {
    if (g_gpu.at_fa_ready) {
        return
    }
    verify(ensure_f16cvt_cls() && (hs == 64l ? ensure_fa_cm2_h64_cls() : ensure_fa_cm2_h128_cls()),
        "vk attention chain: the fa class rails must engage on a live device")
    let kvbytes = AT_CTX * AT_MAX_KV * 4l
    let kv16bytes = AT_CTX * AT_MAX_KV * 2l
    g_gpu.at_k16_dev = make_device_buf(kv16bytes)
    g_gpu.at_v16_dev = make_device_buf(kv16bytes)
    g_gpu.at_k16cvt_set = set_f16cvt_cls(fixed_array(g_gpu.at_k_dev, g_gpu.at_k16_dev),
        fixed_array(kvbytes, kv16bytes), fixed_array(VHB_ATK, VHB_ATK16))
    g_gpu.at_v16cvt_set = set_f16cvt_cls(fixed_array(g_gpu.at_v_dev, g_gpu.at_v16_dev),
        fixed_array(kvbytes, kv16bytes), fixed_array(VHB_ATV, VHB_ATV16))
    g_gpu.at_fa_set = set_fa_cm2_cls(
        fixed_array(g_gpu.at_q_dev, g_gpu.at_k16_dev, g_gpu.at_v16_dev, g_gpu.at_g_dev),
        fixed_array(AT_WINDOW * AT_MAX_QD * 4l, kv16bytes, kv16bytes, AT_WINDOW * AT_MAX_QD * 4l),
        fixed_array(VHB_ATQ, VHB_ATK16, VHB_ATV16, VHB_ATG))
    g_gpu.at_fa_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: attention chain on the cm2 flash tile (hs {hs})\n")
}

// one attention window: acts/meta/smalls staging copies, q|gate + k + v tiles, the prep pair
// (deinterleave/qk-rms/rope; k lands at absolute positions), v append, the flash attention pass,
// o requant, wo tile, y DMA
[arch(at="../ARCHITECTURE_GPU_VULKAN.md#vk-prefill-window-chain"), arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#decode-attention-block")]
def private record_at_cmd(s_q, s_k, s_v, s_o : int; nwg_q, nwg_k, nwg_v, nwg_o : uint;
                          rows, w0, qd, kv_dim, dim, n_heads, hs, half, n_kv, kv_mul, o_nblk : int64;
                          axq_bytes, axs_bytes, sm_bytes : int64; o_kq, gated, qknorm, fa, hasb : bool;
                          eps, scale : float; woq : int64) {
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    pfq_begin(raw)
    pfq_ts(raw)
    vhz_dep(raw, h, 0u, VHB_ACT, true)
    cmd_copy_batch_acts(raw, axq_bytes, axs_bytes)
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, s_q)
    cmd_copy_batch_meta(raw, s_k)
    cmd_copy_batch_meta(raw, s_v)
    cmd_copy_batch_meta(raw, s_o)
    vhz_dep(raw, h, 0u, VHB_SM, true)
    cmd_copy_range(raw, g_gpu.at_smalls.buf, 0l, g_gpu.at_smalls_dev, 0l, sm_bytes)
    pfq_ts(raw)
    stack_batch_enc(raw, h, s_q, nwg_q, dim, gated ? 2l * qd : qd)
    stack_batch_enc(raw, h, s_k, nwg_k, dim, kv_dim)
    stack_batch_enc(raw, h, s_v, nwg_v, dim, kv_dim)
    pfq_ts(raw)
    let pflags = uint((gated ? 1 : 0) | (qknorm ? 2 : 0))
    let hb = hasb ? 1u : 0u
    var pcq = AtPrepArgs(d = uint(qd), nh = uint(n_heads), hs = uint(hs), half = uint(half),
        flags = pflags, rms_off = uint(AT_SM_RMSQ), cos_off = uint(AT_SM_COS),
        sin_off = uint(AT_SM_COS + rows * half), obase = 0u, qsrc = 1u, boff = 0u, hasb = hb, eps = eps)
    enc_at_prep_cls(raw, h, g_gpu.at_prep_q_set, pcq, rows)
    var pck = AtPrepArgs(d = uint(kv_dim), nh = uint(n_kv), hs = uint(hs), half = uint(half),
        flags = pflags, rms_off = uint(AT_SM_RMSK), cos_off = uint(AT_SM_COS),
        sin_off = uint(AT_SM_COS + rows * half), obase = uint(w0), qsrc = 0u, boff = uint(qd), hasb = hb, eps = eps)
    enc_at_prep_cls(raw, h, g_gpu.at_prep_k_set, pck, rows)
    if (hasb) {
        var pcv = AtPrepArgs(d = uint(kv_dim), nh = uint(n_kv), hs = uint(hs), half = 0u,
            flags = 0u, rms_off = 0u, cos_off = 0u, sin_off = 0u, obase = 0u, qsrc = 0u,
            boff = uint(qd + kv_dim), hasb = 1u, eps = eps)
        enc_at_prep_cls(raw, h, g_gpu.at_prep_v_set, pcv, rows)
    }
    pfq_ts(raw)
    // append raw v at absolute positions
    vhz_dep(raw, h, VHB_VRAW, VHB_ATV, true)
    cmd_copy_range(raw, g_gpu.at_vraw_dev, 0l, g_gpu.at_v_dev, w0 * kv_dim * 4l, rows * kv_dim * 4l)
    if (fa) {
        var pcv = ActArgs(nelem = uint((w0 + rows) * kv_dim), gelu = 0u, nblk = 0u)
        let cvtwgs = ((w0 + rows) * kv_dim / 4l + 255l) / 256l
        enc_f16cvt_cls(raw, h, g_gpu.at_k16cvt_set, pcv, cvtwgs)
        enc_f16cvt_cls(raw, h, g_gpu.at_v16cvt_set, pcv, cvtwgs)
        if (w0 + rows <= DAT_MIRROR_ROWS) {
            dat_layer_ensure(woq, kv_dim)
            var DL & = unsafe(g_gpu.dat_layers[woq])
            vhz_dep(raw, h, VHB_ATK16 | VHB_ATV16, 0u, true)
            cmd_copy_range(raw, g_gpu.at_k16_dev, 0l, DL.k_mir, 0l, (w0 + rows) * kv_dim * 2l)
            cmd_copy_range(raw, g_gpu.at_v16_dev, 0l, DL.v_mir, 0l, (w0 + rows) * kv_dim * 2l)
            DL.cnt = w0 + rows
            DL.filled = true
            g_dat_mirror_fills++
        }
        var pcf = FaCm2Args(rows = uint(rows), w0 = uint(w0), qd = uint(qd), kvd = uint(kv_dim),
            kv_mul = uint(kv_mul), kvbase = 0u, scale = scale)
        let fawgs = n_heads * ((rows + 63l) / 64l)
        if (hs == 64l) {
            enc_fa_cm2_h64_cls(raw, h, g_gpu.at_fa_set, pcf, fawgs)
        } else {
            enc_fa_cm2_h128_cls(raw, h, g_gpu.at_fa_set, pcf, fawgs)
        }
    } else {
        var pca = AtAttnArgs(rows = uint(rows), w0 = uint(w0), qd = uint(qd), kvd = uint(kv_dim),
            hs = uint(hs), kv_mul = uint(kv_mul), flags = gated ? 1u : 0u, dpl = uint(hs / 32l), scale = scale)
        enc_at_attn_cls(raw, h, g_gpu.at_attn_set, pca, n_heads * ((rows + 7l) / 8l))
    }
    pfq_ts(raw)
    var pcr = RqArgs(inbase = 0u, nblk = uint(o_nblk))
    let rqwgs = (rows * qd / (o_kq ? 8l : 4l) + 255l) / 256l
    if (o_kq) {
        enc_cls_q8k_rq(raw, h, g_gpu.at_rq_set, pcr, rqwgs)
    } else {
        enc_cls_dn_rq(raw, h, g_gpu.at_rq_set, pcr, rqwgs)
    }
    pfq_ts(raw)
    stack_batch_enc(raw, h, s_o, nwg_o, qd, dim)
    pfq_ts(raw)
    vhz_dep(raw, h, VHB_Y1, 0u, true)
    cmd_copy_whole(raw, g_gpu.batch_y1_dev, g_gpu.batch_y1.buf, rows * dim * 4l)
    //! the K/V host DMAs stay at the END of the chain (ARCHITECTURE_GPU_VULKAN.md sec.2.2j: a driver drops the in-cmd barrier)
    vhz_dep(raw, h, VHB_ATK, 0u, true)
    cmd_copy_range(raw, g_gpu.at_k_dev, w0 * kv_dim * 4l, g_gpu.at_kv_host.buf, 0l, rows * kv_dim * 4l)
    vhz_dep(raw, h, VHB_VRAW, 0u, true)
    cmd_copy_range(raw, g_gpu.at_vraw_dev, 0l, g_gpu.at_kv_host.buf, AT_WINDOW * AT_MAX_KV * 4l, rows * kv_dim * 4l)
    pfq_ts(raw)
    pfq_end(raw)
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_attn hz: {h.disp} nodes, {h.barriers} barriers\n")
    }
    vk_check(vkEndCommandBuffer(raw), null)
}

//! MoeGpuAttnFn: one full-attention layer's whole softmax attention block as one submit chain per
//! AT_WINDOW rows. yp gets npos x dim rows; kp/vp the roped-k / raw-v rows for the caller's cache store.
//! K/V persist device-side at absolute positions, so later windows attend the whole batch.
def vk_moe_attn(var yp : float?; var kp : float?; var vp : float?;
                woq, wok, wov, woo : int64; fq, fk, fv, fo : int;
                xqp : int8 const?; xsp : float const?;
                rmsqp : float const?; rmskp : float const?;
                bqp : float const?; bkp : float const?; bvp : float const?;
                cosp : float const?; sinp : float const?;
                npos, dim, qd, kv_dim, hs, rot, n_heads : int64;
                scale : float; eps : float; gated : bool; qknorm : bool) {
    let s_q = find_stack(woq, fq, "prefill attn q")
    let s_k = find_stack(wok, fk, "prefill attn k")
    let s_v = find_stack(wov, fv, "prefill attn v")
    let s_o = find_stack(woo, fo, "prefill attn o")
    // q/k/v share one activation image, so their format class must agree (the FFN triple rule);
    // the wo stack is free — the requant form follows it
    let kq = fq != 0
    assert((fk != 0) == kq && (fv != 0) == kq,
        "dasLLAMA vulkan tier: q/k/v stack format class mismatch in one attention chain")
    let half = rot / 2l
    let n_kv = kv_dim / hs
    let kv_mul = n_kv > 0l ? n_heads / n_kv : 0l
    assert(npos <= AT_CTX && qd <= AT_MAX_QD && kv_dim <= AT_MAX_KV
           && (hs == 128l || hs == 256l) && half >= 1l && 2l * half <= hs
           && n_heads * hs == qd && n_kv * hs == kv_dim && n_heads == n_kv * kv_mul
           && n_heads <= 64l && dim % 32l == 0l && kv_dim % 32l == 0l
           && qd % (fo != 0 ? 256l : 32l) == 0l
           && (gated ? 2l : 1l) * qd * AT_WINDOW * 4l <= BATCH_Y_BYTES
           && qd * AT_WINDOW <= BATCH_HQ_BYTES && npos * dim * 4l <= BATCH_Y_BYTES,
        "dasLLAMA vulkan tier: attention geometry outside chain capacity")
    let xunit = kq ? 256l : 32l
    ensure_batch_state()
    ensure_at_state()
    let fa = at_fa_serves(hs, gated)
    if (fa) {
        ensure_at_fa_state(hs)
    }
    ensure_batch_set(s_q, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
    ensure_batch_set(s_k, g_gpu.batch_y2_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y2)
    ensure_batch_set(s_v, g_gpu.at_vraw_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES,
        VHB_ACT, VHB_VRAW, [ybytes = AT_WINDOW * AT_MAX_KV * 4l])
    ensure_batch_set(s_o, g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.hs_dev, BATCH_HQ_BYTES, BATCH_HS_BYTES, VHB_HQ, VHB_Y1)
    unsafe {
        var sp = reinterpret<float?>(g_gpu.at_smalls.mapped)
        sp[0] = eps
        sp[1] = scale
        if (qknorm) {
            memcpy(reinterpret<void?>(sp + AT_SM_RMSQ), reinterpret<void?>(rmsqp), hs * 4l)
            memcpy(reinterpret<void?>(sp + AT_SM_RMSK), reinterpret<void?>(rmskp), hs * 4l)
        }
    }
    let hasb = bqp != null
    if (hasb) {
        unsafe {
            upload_region_at(g_gpu.at_bias_dev, 0l, reinterpret<uint8 const?>(bqp), qd * 4l)
            upload_region_at(g_gpu.at_bias_dev, qd * 4l, reinterpret<uint8 const?>(bkp), kv_dim * 4l)
            upload_region_at(g_gpu.at_bias_dev, (qd + kv_dim) * 4l, reinterpret<uint8 const?>(bvp), kv_dim * 4l)
        }
    }
    var q_region = fixed_array(woq, 0l, 0l)
    var k_region = fixed_array(wok, 0l, 0l)
    var v_region = fixed_array(wov, 0l, 0l)
    var o_region = fixed_array(woo, 0l, 0l)
    var r0 = 0l
    while (r0 < npos) {
        let rows = min(AT_WINDOW, npos - r0)
        let ts0 = ref_time_ticks()
        q_region[1] = r0
        q_region[2] = rows
        k_region[1] = r0
        k_region[2] = rows
        v_region[1] = r0
        v_region[2] = rows
        o_region[1] = r0
        o_region[2] = rows
        var nwg_q = 0u
        var nwg_k = 0u
        var nwg_v = 0u
        var nwg_o = 0u
        unsafe {
            nwg_q = fill_batch_meta(s_q, addr(q_region[0]), 1l, dim, gated ? 2l * qd : qd, r0, r0 + rows)
            nwg_k = fill_batch_meta(s_k, addr(k_region[0]), 1l, dim, kv_dim, r0, r0 + rows)
            nwg_v = fill_batch_meta(s_v, addr(v_region[0]), 1l, dim, kv_dim, r0, r0 + rows)
            nwg_o = fill_batch_meta(s_o, addr(o_region[0]), 1l, qd, dim, r0, r0 + rows)
        }
        upload_batch_acts(xqp, xsp, r0, rows, dim, xunit)
        unsafe {
            var sp = reinterpret<float?>(g_gpu.at_smalls.mapped)
            par_memcpy(reinterpret<void?>(sp + AT_SM_COS), reinterpret<void?>(cosp + r0 * half), rows * half * 4l)
            par_memcpy(reinterpret<void?>(sp + AT_SM_COS + rows * half), reinterpret<void?>(sinp + r0 * half), rows * half * 4l)
        }
        let ts1 = ref_time_ticks()
        record_at_cmd(s_q, s_k, s_v, s_o, nwg_q, nwg_k, nwg_v, nwg_o,
            rows, r0, qd, kv_dim, dim, n_heads, hs, half, n_kv, kv_mul,
            rows * qd / (fo != 0 ? 256l : 32l),
            rows * dim, rows * (dim / xunit) * 4l, (AT_SM_COS + 2l * rows * half) * 4l,
            fo != 0, gated, qknorm, fa, hasb, eps, scale, woq)
        submit_wait(g_gpu.batch_cmd)
        let ts2 = ref_time_ticks()
        unsafe {
            par_memcpy(reinterpret<void?>(yp + r0 * dim), g_gpu.batch_y1.mapped, rows * dim * 4l)
            var kvp8 = reinterpret<uint8?>(g_gpu.at_kv_host.mapped)
            par_memcpy(reinterpret<void?>(kp + r0 * kv_dim), g_gpu.at_kv_host.mapped, rows * kv_dim * 4l)
            par_memcpy(reinterpret<void?>(vp + r0 * kv_dim), reinterpret<void?>(kvp8 + AT_WINDOW * AT_MAX_KV * 4l), rows * kv_dim * 4l)
        }
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_attn prep {get_time_usec(ts0) - get_time_usec(ts1)} submit {get_time_usec(ts1) - get_time_usec(ts2)} copy {get_time_usec(ts2)}\n")
            if (g_pfq_n == 8u) {
                to_log(LOG_INFO, "vk_attn gpu copies {int(pfq_us(1u))} qkv {int(pfq_us(2u))} prep {int(pfq_us(3u))} attn {int(pfq_us(4u))} rq {int(pfq_us(5u))} wo {int(pfq_us(6u))} dma {int(pfq_us(7u))}\n")
            }
        }
        r0 += rows
    }
}

// ===== streamed-prefill dispatch (pinned-mirror layers) =====

def private ensure_stream_slots {
    if (g_gpu.stream_ready) {
        return
    }
    ensure_batch_state()
    g_gpu.stream_cmd = alloc_cmd()
    let nslots = stream_slot_count()
    g_gpu.stream_slots |> reserve(nslots)
    for (_slot in range(nslots)) {
        let s1 = make_stack_shell(-1l, 0l, 0, g_gpu.stream_max_gu_wq, g_gpu.stream_max_gu_ws, [xfer_shared = true])
        let s3 = make_stack_shell(-1l, 0l, 0, g_gpu.stream_max_gu_wq, g_gpu.stream_max_gu_ws, [xfer_shared = true])
        let s2 = make_stack_shell(-1l, 0l, 0, g_gpu.stream_max_dn_wq, g_gpu.stream_max_dn_ws, [xfer_shared = true])
        g_gpu.stream_slots |> push(StreamSlot(group = -1, s1 = s1, s3 = s3, s2 = s2))
    }
    g_gpu.stream_ready = true
    let slot_bytes = 2l * (g_gpu.stream_max_gu_wq + g_gpu.stream_max_gu_ws) + g_gpu.stream_max_dn_wq + g_gpu.stream_max_dn_ws
    // same read-time carve carved_budget() subtracts — overcommit silently hits the WDDM paging cliff
    let need = moe_gpu_stream_need()
    let reserve = need > 0l ? int64(nslots) * need : STREAM_RESERVE
    assert(int64(nslots) * slot_bytes <= reserve, "dasLLAMA vulkan tier: stream slots exceed the carved stream reserve")
    to_log(LOG_INFO, "dasLLAMA vulkan tier: streamed prefill engaged ({length(g_gpu.stream_stacks) / 3} layers, slot {slot_bytes} bytes x {length(g_gpu.stream_slots)})\n")
}

// re-base one slot pseudo-stack onto a stream stack (descriptors stay — the slot buffers are
// the same; only the meta-resolution identity and the kernel choice follow the layer)
def private rebase_pseudo(si : int; ss : StreamStack const) {
    g_gpu.stacks[si].base = ss.base
    g_gpu.stacks[si].elems = ss.elems
    g_gpu.stacks[si].fmt = ss.fmt
}


//! a stream copy's recording head: every prior transfer-queue copy retired, the shared cmd reset
//! (shared cmds are safe: a wait always intervenes between recordings)
def private stream_copy_begin : VkCommandBuffer {
    let xfer = g_gpu.xfer_fam >= 0
    if (xfer && g_gpu.xfer_value > 0ul) {
        xfer_host_wait(g_gpu.xfer_value)   // every prior transfer-queue copy retired
    }
    var raw = xfer ? g_gpu.xfer_cmd : g_gpu.stream_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    return raw
}

//! The streamed-layer split's copy: only the regions' experts cross the bus (region r's expert =
//! (woff - stack base) / ege), each at its own offset inside the slot's planes; the slot is marked
//! EMPTY afterwards, so a later window never mistakes the partial fill for the whole group.
[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#streamed-layer-split")]
def private copy_stream_group_partial(si, g : int; offs1 : int64 const?; nregions, ege : int64) {
    let slot = g_gpu.stream_slots[si]
    var raw = stream_copy_begin()
    for (k in range(3)) {
        let sk = g * 3 + k
        let psi = k == 0 ? slot.s1 : (k == 1 ? slot.s3 : slot.s2)
        let nexp = g_gpu.stream_stacks[sk].elems / ege
        let qlen = g_gpu.stream_stacks[sk].wqbytes / nexp
        let slen = g_gpu.stream_stacks[sk].wsbytes / nexp
        for (r in range64(nregions)) {
            let e = (unsafe(offs1[r * 3l]) - g_gpu.stream_stacks[g * 3].base) / ege
            cmd_copy_range(raw, g_gpu.stream_stacks[sk].hwq.buf, e * qlen, g_gpu.stacks[psi].wqbuf, e * qlen, qlen)
            if (slen > 0l) {
                cmd_copy_range(raw, g_gpu.stream_stacks[sk].hws.buf, e * slen, g_gpu.stacks[psi].wsbuf, e * slen, slen)
            }
        }
    }
    stream_copy_end(si, raw, false)
    g_gpu.stream_slots[si].group = -1
}

def private copy_stream_group(si, g : int; sync : bool) {
    let slot = g_gpu.stream_slots[si]
    var raw = stream_copy_begin()
    for (k in range(3)) {
        let sk = g * 3 + k
        let psi = k == 0 ? slot.s1 : (k == 1 ? slot.s3 : slot.s2)
        cmd_copy_whole(raw, g_gpu.stream_stacks[sk].hwq.buf, g_gpu.stacks[psi].wqbuf, g_gpu.stream_stacks[sk].wqbytes)
        if (g_gpu.stream_stacks[sk].wsbytes > 0l) {
            cmd_copy_whole(raw, g_gpu.stream_stacks[sk].hws.buf, g_gpu.stacks[psi].wsbuf, g_gpu.stream_stacks[sk].wsbytes)
        }
    }
    stream_copy_end(si, raw, sync)
    g_gpu.stream_slots[si].group = g
}

//! a stream copy's submit: the transfer queue signals the timeline (the slot's consumer attaches the
//! GPU-side wait), single-queue mode keeps the old unfenced rail; `sync` blocks the host either way
def private stream_copy_end(si : int; var raw : VkCommandBuffer; sync : bool) {
    let xfer = g_gpu.xfer_fam >= 0
    vk_check(vkEndCommandBuffer(raw), null)
    if (xfer) {
        g_gpu.xfer_value++
        var sig = g_gpu.xfer_value
        var tss = VkTimelineSemaphoreSubmitInfo()
        var sem = g_gpu.xfer_sem
        var submit = VkSubmitInfo()
        submit.commandBufferCount = 1u
        submit.signalSemaphoreCount = 1u
        let nofence : VkFence
        unsafe {
            tss.signalSemaphoreValueCount = 1u
            tss.pSignalSemaphoreValues = addr(sig)
            submit.pNext = addr<void?>(tss)
            submit.pCommandBuffers = addr(raw)
            submit.pSignalSemaphores = addr(sem)
            vk_check(vkQueueSubmit(g_gpu.xfer_queue, 1u, addr(submit), nofence), null)
        }
        if (sync) {
            xfer_host_wait(g_gpu.xfer_value)
        }
        // the consumer still attaches the GPU-side wait — cross-queue visibility by the book
        g_gpu.stream_slots[si].wait_value = g_gpu.xfer_value
    } elif (sync) {
        submit_wait(raw)
    } else {
        var submit = VkSubmitInfo()
        submit.commandBufferCount = 1u
        let nofence : VkFence
        unsafe {
            submit.pCommandBuffers = addr(raw)
            vk_check(vkQueueSubmit(g_gpu.queue, 1u, addr(submit), nofence), null)
        }
    }
}

//! bind group g's slot for a batch dispatch: prefetch miss cold-copies the slot, the pseudo stacks
//! rebase onto it, and the slot's transfer-queue fill hands off — the next compute submit waits the timeline
def private stream_group_bind(g : int; nrows : int64; f3, f2 : int) {
    assert(nrows >= BATCH_TILE, "dasLLAMA vulkan tier: streamed layers serve prefill batches only")
    assert(g_gpu.stream_stacks[g * 3 + 1].fmt == f3 && g_gpu.stream_stacks[g * 3 + 2].fmt == f2,
        "dasLLAMA vulkan tier: streamed group format mismatch")
    ensure_stream_slots()
    var si = -1
    for (i in range(length(g_gpu.stream_slots))) {
        if (g_gpu.stream_slots[i].group == g) {
            si = i
        }
    }
    if (si < 0) {
        si = g % length(g_gpu.stream_slots)
        copy_stream_group(si, g, true)
    }
    rebase_pseudo(g_gpu.stream_slots[si].s1, g_gpu.stream_stacks[g * 3])
    rebase_pseudo(g_gpu.stream_slots[si].s3, g_gpu.stream_stacks[g * 3 + 1])
    rebase_pseudo(g_gpu.stream_slots[si].s2, g_gpu.stream_stacks[g * 3 + 2])
    if (g_gpu.stream_slots[si].wait_value > 0ul) {
        g_gpu.pend_wait = g_gpu.stream_slots[si].wait_value
        g_gpu.stream_slots[si].wait_value = 0ul
    }
}

var private g_split_announced = false

[arch(at="../ARCHITECTURE_GPU_VULKAN_DECODE.md#streamed-layer-split")]
def stream_group_bind_partial(g : int; nrows : int64; f3, f2 : int; offs1 : int64 const?; nregions, ege : int64) {
    if (!g_split_announced) {
        g_split_announced = true
        to_log(LOG_INFO, "dasLLAMA vulkan tier: streamed prefill split engaged (DASLLAMA_GPU_MOE_SPLIT=0 streams whole layers)\n")
    }
    assert(nrows >= BATCH_TILE, "dasLLAMA vulkan tier: streamed layers serve prefill batches only")
    assert(g_gpu.stream_stacks[g * 3 + 1].fmt == f3 && g_gpu.stream_stacks[g * 3 + 2].fmt == f2,
        "dasLLAMA vulkan tier: streamed group format mismatch")
    ensure_stream_slots()
    let si = g % length(g_gpu.stream_slots)
    copy_stream_group_partial(si, g, offs1, nregions, ege)
    rebase_pseudo(g_gpu.stream_slots[si].s1, g_gpu.stream_stacks[g * 3])
    rebase_pseudo(g_gpu.stream_slots[si].s3, g_gpu.stream_stacks[g * 3 + 1])
    rebase_pseudo(g_gpu.stream_slots[si].s2, g_gpu.stream_stacks[g * 3 + 2])
    if (g_gpu.stream_slots[si].wait_value > 0ul) {
        g_gpu.pend_wait = g_gpu.stream_slots[si].wait_value
        g_gpu.stream_slots[si].wait_value = 0ul
    }
}

//! prefetch of the next group runs descending while prefill runs ascending
def private stream_group_prefetch(g : int) {
    let nslots = stream_slot_count()
    if (nslots > 1 && g > 0 && g_gpu.stream_slots[(g - 1) % nslots].group != g - 1) {
        copy_stream_group((g - 1) % nslots, g - 1, false)
    }
}

def vk_moe_ffn_streamed(g : int; var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xqp : int8 const?; xsp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    stream_group_bind(g, nrows, f3, f2)
    vk_moe_ffn_batch(goutp, offs1, offs3, offs2, nregions, xqp, xsp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    stream_group_prefetch(g)
}

def vk_moe_ffn_streamed_xf(g : int; var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    stream_group_bind(g, nrows, f3, f2)
    vk_moe_ffn_batch_xf(goutp, offs1, offs3, offs2, nregions, xp, wp, invp, n, nfe, nrows, npos, f1, f3, f2, is_gelu)
    stream_group_prefetch(g)
}

// ===== the cm2 expert chain: the batch arm over the f16 feed =====

//! MoeGpuFfnXfOkFn: does the cm2 chain serve a layer whose triple is (f1, f3, f2) at this window?
//! Mode 4 on a coopmat2 device, every stack a format the f16 feed admits, and the window
//! inside the f32 x plane's cap. The combined (npos > 0) form only.
[arch(at="../ARCHITECTURE_RUNTIME.md#activation-scale-lattice"), arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain")]
def vk_moe_ffn_xf_ok(npos, n : int64; f1, f3, f2 : int) : bool {
    if (g_gpu == null || g_gpu.dry || !g_gpu.has_coopmat2) {
        return false
    }
    return pf_f16_feed(f1) && pf_f16_feed(f3) && pf_f16_feed(f2) && npos * n * 4l <= BATCH_Y_BYTES
}

def private ensure_xf_state {
    if (g_gpu.xf_ready) {
        return
    }
    ensure_batch_state()
    ensure_comb_state()
    g_gpu.batch_xf_dev = make_device_buf(BATCH_Y_BYTES)
    g_gpu.resident_bytes += BATCH_Y_BYTES
    verify(ensure_moe_gather_f16_cls() && ensure_actf16_cls(), "vk batch: the cm2 feed class rails must engage on a live device")
    g_gpu.batch_gather_set = set_moe_gather_f16_cls(
        fixed_array(g_gpu.batch_xf_dev, g_gpu.comb_inv_dev, g_gpu.batch_xq_dev),
        fixed_array(BATCH_Y_BYTES, COMB_WI_BYTES, BATCH_XQ_BYTES),
        fixed_array(VHB_XF, VHB_COMB, VHB_ACT))
    g_gpu.batch_actf16_set = set_actf16_cls(
        fixed_array(g_gpu.batch_y1_dev, g_gpu.batch_y2_dev, g_gpu.hq_dev),
        fixed_array(BATCH_Y_BYTES, BATCH_Y_BYTES, BATCH_HQ_BYTES),
        fixed_array(VHB_Y1, VHB_Y2, VHB_HQ))
    g_gpu.xf_ready = true
    to_log(LOG_INFO, "dasLLAMA vulkan tier: cm2 expert chain engaged\n")
}

//! chunk row cap of the cm2 chain: the gathered f16 image (batch_xq window), the f16 hidden plane
//! (hq_dev) and the f32 output planes must all fit; 128-aligned like the tiles' columns
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-tile-pick-and-default")]
def private ffn_cm2_chunk_rows(n, nfe : int64) : int64 {
    var r = min(BATCH_XQ_BYTES / (n * 2l), BATCH_HQ_BYTES / (nfe * 2l)) - TILE_READ_SLACK
    r = min(r, BATCH_Y_BYTES / (max(nfe, n) * 4l))
    let rows = min(r, BATCH_CHUNK_ROWS - TILE_READ_SLACK) / 128l * 128l
    assert(rows >= 128l, "dasLLAMA vulkan tier: the cm2 chain's planes hold no chunk at this width")
    return rows
}

[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain")]
def private record_ffn_cm2_cmd(s1, s3, s2 : int; nwg_gu, nwg_dn : uint; rows, n, nfe, nk, npos : int64;
                               r0, r1 : int64; is_gelu : bool) {
    let raw = g_gpu.batch_cmd
    let rf : VkCommandBufferResetFlags
    vk_check(vkResetCommandBuffer(raw, rf), null)
    let begin = VkCommandBufferBeginInfo()
    vk_check(vkBeginCommandBuffer(raw, begin), null)
    var h : VkHaz
    if (r0 == 0l) {
        vhz_dep(raw, h, 0u, VHB_XF, true)
        cmd_copy_whole(raw, g_gpu.staging.buf, g_gpu.batch_xf_dev, npos * n * 4l)
        vhz_dep(raw, h, 0u, VHB_COMB, true)
        cmd_copy_whole(raw, g_gpu.comb_stage.buf, g_gpu.comb_w_dev, nk * 4l)
        cmd_copy_range(raw, g_gpu.comb_stage.buf, COMB_WI_BYTES, g_gpu.comb_inv_dev, 0l, nk * 4l)
    }
    vhz_dep(raw, h, 0u, VHB_META, true)
    cmd_copy_batch_meta(raw, s1)
    cmd_copy_batch_meta(raw, s3)
    cmd_copy_batch_meta(raw, s2)
    let k = uint(nk / npos)
    var pcg = GatherArgs(n = uint(n), k = k, r0 = uint(r0), r1 = uint(r1))
    enc_moe_gather_f16_cls(raw, h, g_gpu.batch_gather_set, pcg, nk)
    stack_batch_enc_cm2(raw, h, s1, nwg_gu, n, nfe)
    stack_batch_enc_cm2(raw, h, s3, nwg_gu, n, nfe)
    var pca = ActArgs(nelem = uint(rows * nfe), gelu = is_gelu ? 1u : 0u, nblk = uint(rows * nfe / 32l))
    enc_actf16_cls(raw, h, g_gpu.batch_actf16_set, pca, (rows * nfe / 4l + 255l) / 256l)
    stack_batch_enc_cm2(raw, h, s2, nwg_dn, nfe, n)
    var pcc = CombineArgs(n = uint(n), k = k, r0 = uint(r0), r1 = uint(r1))
    enc_combine_cls(raw, h, g_gpu.comb_set, pcc, npos)
    if (r1 == nk) {
        vhz_dep(raw, h, VHB_CY, 0u, true)
        cmd_copy_whole(raw, g_gpu.comb_y_dev, g_gpu.comb_y.buf, npos * n * 4l)
    }
    if (vk_prof()) {
        to_log(LOG_INFO, "vk_ffnc hz: {h.disp} nodes, {h.barriers} barriers\n")
    }
    vk_check(vkEndCommandBuffer(raw), null)
}

//! the chain's stacks, sets and host staging (x rows, the combine's w/inv planes) - shared by the
//! synchronous chunk loop and the async single-chunk begin
def private xf_prepare(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; xp : float const?; wp : float const?; invp : uint const?; n, nrows, npos : int64; f1, f3, f2 : int) : tuple<s1 : int; s3 : int; s2 : int> {
    let s1 = find_stack(unsafe(offs1[0]), f1, "prefill ffn gate")
    let s3 = find_stack(unsafe(offs3[0]), f3, "prefill ffn up")
    let s2 = find_stack(unsafe(offs2[0]), f2, "prefill ffn down")
    assert(npos > 0l && nrows % npos == 0l, "dasLLAMA vulkan tier: the cm2 expert chain serves the combined form only")
    assert(nrows * 4l <= COMB_WI_BYTES && npos * n * 4l <= BATCH_Y_BYTES,
        "dasLLAMA vulkan tier: combined output exceeds capacity")
    ensure_xf_state()
    ensure_batch_set(s1, g_gpu.batch_y1_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y1)
    ensure_batch_set(s3, g_gpu.batch_y2_dev, g_gpu.batch_xq_dev, g_gpu.batch_xs_dev, BATCH_XQ_BYTES, BATCH_XS_BYTES, VHB_ACT, VHB_Y2)
    ensure_batch_set(s2, g_gpu.batch_y1_dev, g_gpu.hq_dev, g_gpu.hs_dev, BATCH_HQ_BYTES, BATCH_HS_BYTES, VHB_HQ, VHB_Y1)
    unsafe {
        par_memcpy(g_gpu.staging.mapped, reinterpret<void?>(xp), npos * n * 4l)
        var cp = reinterpret<uint8?>(g_gpu.comb_stage.mapped)
        par_memcpy(g_gpu.comb_stage.mapped, reinterpret<void?>(wp), nrows * 4l)
        par_memcpy(reinterpret<void?>(cp + COMB_WI_BYTES), reinterpret<void?>(invp), nrows * 4l)
    }
    return (s1 = s1, s3 = s3, s2 = s2)
}

//! one chunk's metas + record; returns after the record so the caller picks wait or no-wait
def private xf_record_chunk(s1, s3, s2 : int; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; n, nfe, nrows, npos, r0, r1 : int64; is_gelu : bool) {
    let nwg1 = fill_batch_meta(s1, offs1, nregions, n, nfe, r0, r1, true)
    let nwg3 = fill_batch_meta(s3, offs3, nregions, n, nfe, r0, r1, true)
    assert(nwg1 == nwg3, "dasLLAMA vulkan tier: fused batch pair region mismatch")
    let nwg2 = fill_batch_meta(s2, offs2, nregions, nfe, n, r0, r1, true)
    record_ffn_cm2_cmd(s1, s3, s2, nwg1, nwg2, r1 - r0, n, nfe, nrows, npos, r0, r1, is_gelu)
}

//! MoeGpuFfnXfFn: the cm2 route of the batch arm. `xp` = the window's f32 rows (npos x n, one
//! per position); `invp`/`wp` = the combine's slot map and weights over nrows = npos x k bucket
//! rows. Chunked over bucket-row windows like the quant arm; combined rows read back once at the end.
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain")]
def vk_moe_ffn_batch_xf(var goutp : float?; offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    let st = xf_prepare(offs1, offs3, offs2, xp, wp, invp, n, nrows, npos, f1, f3, f2)
    let chunk = ffn_cm2_chunk_rows(n, nfe)
    var r0 = 0l
    while (r0 < nrows) {
        let r1 = min(r0 + chunk, nrows)
        let ts0 = ref_time_ticks()
        xf_record_chunk(st.s1, st.s3, st.s2, offs1, offs3, offs2, nregions, n, nfe, nrows, npos, r0, r1, is_gelu)
        let ts1 = ref_time_ticks()
        submit_wait(g_gpu.batch_cmd)
        if (vk_prof()) {
            to_log(LOG_INFO, "vk_ffn_cm2 prep {get_time_usec(ts0) - get_time_usec(ts1)} submit {get_time_usec(ts1)}\n")
        }
        r0 = r1
    }
    unsafe {
        par_memcpy(reinterpret<void?>(goutp), g_gpu.comb_y.mapped, npos * n * 4l)
    }
}

var private g_xf_pending = false   // ONE async cm2 chain in flight (its combined rows wait in comb_y)

//! the model drop's twin of the FFN-in-flight assert: an async cm2 chain must have been joined
def vk_xf_drop_check {
    assert(!g_xf_pending, "dasLLAMA vulkan tier: model drop with an async cm2 chain in flight")
    g_xf_pending = false
}
var private g_xf_pend_bytes = 0l

//! MoeGpuFfnXfRowsFn: the async arm serves one chunk - this many bucket rows at most.
def vk_moe_ffn_xf_async_rows(n, nfe : int64) : int64 => ffn_cm2_chunk_rows(n, nfe)

//! MoeGpuFfnXfBeginFn: the cm2 chain over the given regions, submitted and left in flight - the
//! streamed-layer split runs its CPU regions meanwhile; vk_moe_ffn_batch_xf_join collects.
[arch(at="../ARCHITECTURE_GPU_VULKAN_GEMM.md#cm2-expert-chain")]
def vk_moe_ffn_batch_xf_begin(offs1 : int64 const?; offs3 : int64 const?; offs2 : int64 const?; nregions : int64; xp : float const?; wp : float const?; invp : uint const?; n, nfe, nrows, npos : int64; f1, f3, f2 : int; is_gelu : bool) {
    assert(!g_xf_pending, "dasLLAMA vulkan tier: async cm2 chain begin while one is already in flight")
    let cap = ffn_cm2_chunk_rows(n, nfe)
    assert(nrows <= cap, "dasLLAMA vulkan tier: the async cm2 chain serves one chunk")
    let st = xf_prepare(offs1, offs3, offs2, xp, wp, invp, n, nrows, npos, f1, f3, f2)
    xf_record_chunk(st.s1, st.s3, st.s2, offs1, offs3, offs2, nregions, n, nfe, nrows, npos, 0l, nrows, is_gelu)
    submit_nowait(g_gpu.batch_cmd)
    g_xf_pending = true
    g_xf_pend_bytes = npos * n * 4l
}

//! MoeGpuFfnXfJoinFn: spin the pending chain's fence and copy its combined rows out.
def vk_moe_ffn_batch_xf_join(var goutp : float?) {
    assert(g_xf_pending, "dasLLAMA vulkan tier: async cm2 chain join with nothing in flight")
    wait_fence()
    unsafe {
        par_memcpy(reinterpret<void?>(goutp), g_gpu.comb_y.mapped, g_xf_pend_bytes)
    }
    g_xf_pending = false
}
