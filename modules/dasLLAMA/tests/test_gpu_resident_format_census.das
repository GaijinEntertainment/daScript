options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // load_model_, create_session, moe_gpu_drop_model
require daslib/fio   // path_join
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_dn_on_gpu, set_gpu_resident_route: the per-op rail's marks and the route lever
require dasllama/dasllama_gpu_resident   // resident_layer_decline, perop_dn_decline
require _moe_resident   // MoeFixture, moe_fixture_admitted, MOE_LOGIT_BAR_REL
require _format_census

// The Vulkan arm of the weight-format census on Qwen3.6-35B-A3B-MTP-IQ4_XS-pure, every big plane IQ4_XS on disk (the
// deltanet beta/alpha included): each site's tag against the file's own tensor type, one soft check a site so the cell names
// every site still read as a copy; the whole-model driver armed on the file, a window prefill and fed decode steps against
// the CPU chain at the routed chain's bar with the IQ4_XS kernels' census rising; the same file with the whole-model route
// off, its deltanet layers on the per-op rail's IQ4_XS stacks; and a file whose beta/alpha ship F32 (Qwen3.6-35B-A3B
// UD-IQ2_XXS) read through the f32 GEMV and tile with no f16 router-form dispatch. Large tier (DASLLAMA_PARITY_FULL=1),
// DASLLAMA_GPU=1, -jit; skips without the file, the armed tier, or the expert tiles the routed block needs.

let private CENSUS = MoeFixture(file = "Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL)
let private BA_F32 = MoeFixture(file = "Qwen3.6-35B-A3B-UD-IQ2_XXS.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL)
let private PROMPT = 40l   //! one window, past the per-op deltanet chain's 32-row floor
let private STEPS = 6l

def private recurrent_layers(tr : Model) : int64 {
    var n = 0l
    for (l in range64(tr.config.n_layers)) {
        n += layer_is_recurrent(tr.config, l) ? 1l : 0l
    }
    return n
}

//! the census of the named kernels as one sum (NO_CENSUS when the module is absent)
def private census_sum(names : array<string>) : int64 {
    var n = 0l
    for (k in names) {
        let c = census(k)
        return NO_CENSUS if (c == NO_CENSUS)
        n += c
    }
    return n
}

[test]
def test_gpu_resident_format_census_iq4xs(t : T?) {
    t |> run("{CENSUS.file}: every site's tag is the file's format, the whole-model driver armed, and its window and steps hold the CPU chain on the IQ4_XS kernels") @(t : T?) {
        if (!family_on(t, "qwen35moe") || !arm_on(t, "vk-census")) return
        let path = path_join(models_dir(), CENSUS.file)
        if (!resident_ready(t, path)) return
        var tr <- load_resident(path, 1l)
        census_cell(t, tr, path, "vulkan load")
        for (l in range64(tr.config.n_layers)) {
            t |> equal(resident_layer_decline(tr, l), "", "layer {l} draws no decline from the whole-model driver")
        }
        if (moe_fixture_admitted(t, tr, CENSUS) >= 0l) {
            t |> equal(gpu_resident_decline_(), "", "the whole-model driver armed: no decline reason")
            let nrec = recurrent_layers(tr)
            var pf_tile = 0l
            var st_gemv = 0l
            with_dasllama_jobque_() {
                var prompt <- resident_prompt(PROMPT)
                var toks <- resident_feed(STEPS)
                let tile0 = census("kq_batch_iq4xs_cls_spv")
                var gemv0 = 0l
                var fed <- resident_then_cpu_feed(t, tr, prompt, toks, $ {
                    pf_tile = census("kq_batch_iq4xs_cls_spv") - tile0
                    gemv0 = census_sum(["kq_gemv_iq4xs_cls_spv", "kq_gemv_iq4xs_n_cls_spv"])
                }, $ {
                    st_gemv = census_sum(["kq_gemv_iq4xs_cls_spv", "kq_gemv_iq4xs_n_cls_spv"]) - gemv0
                    served_every_call(t)
                })
                feed_steps_within(t, tr, "IQ4_XS census ", PROMPT, fed.gpu, fed.cpu, toks, CENSUS.bar_rel)
                if (tile0 != NO_CENSUS) {
                    t |> success(pf_tile >= 2l * nrec, "the window read every recurrent layer's beta and alpha planes through the IQ4_XS tile ({pf_tile} dispatches, {2l * nrec} planes)")
                    t |> success(st_gemv >= 5l * nrec, "the token command read every recurrent layer's five planes through the IQ4_XS GEMV ({st_gemv} recorded, {5l * nrec} planes)")
                }
                delete fed
                delete prompt
                delete toks
            }
        }
        delete tr
    }
}

[test]
def test_gpu_resident_format_census_perop(t : T?) {
    t |> run("{CENSUS.file} with the whole-model route off: the per-op rails keep every deltanet layer on the device over its IQ4_XS planes, the window and steps against the CPU chain") @(t : T?) {
        if (!family_on(t, "qwen35moe") || !arm_on(t, "vk-census")) return
        let path = path_join(models_dir(), CENSUS.file)
        if (!resident_ready(t, path)) return
        let route = gpu_resident_route_lever()
        set_gpu_resident_route(false)
        var tr <- load_model_(path, QuantMode.q8)
        set_gpu_resident_route(route)
        t |> success(!moe_gpu_resident_active(), "the whole-model route is off: the per-op rails serve")
        let nrec = recurrent_layers(tr)
        var marked = 0l
        for (l in range64(tr.config.n_layers)) {
            continue if (!layer_is_recurrent(tr.config, l))
            t |> equal(perop_dn_decline(tr, l), "", "layer {l}'s deltanet planes are the per-op chain's formats")
            marked += moe_gpu_dn_on_gpu(l) ? 1l : 0l
        }
        t |> equal(marked, nrec, "the per-op deltanet rail holds every recurrent layer on the device ({marked} of {nrec}) - none left on the CPU rail")
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(PROMPT)
            var toks <- resident_feed(STEPS)
            let scan0 = census("dn_scan_cls_spv")
            let step0 = census("dn_step_cls_spv")
            let tile0 = census("kq_batch_iq4xs_cls_spv")
            var sg <- create_session(tr, mirror_kv_dtype())
            var gpu <- feed_logits(tr, sg, prompt, toks)
            let scan = census("dn_scan_cls_spv") - scan0
            let step = census("dn_step_cls_spv") - step0
            let tile = census("kq_batch_iq4xs_cls_spv") - tile0
            delete sg
            moe_gpu_drop_model()
            var cpu <- feed_alone(tr, mirror_kv_dtype(), prompt, toks)
            feed_steps_within(t, tr, "IQ4_XS per-op ", PROMPT, gpu, cpu, toks, CENSUS.bar_rel, "per-op")
            if (scan0 != NO_CENSUS) {
                t |> success(scan >= nrec, "the per-op window chain scanned every recurrent layer on the device ({scan} of {nrec})")
                t |> success(tile >= 3l * nrec, "its qkv, z and out GEMMs read the IQ4_XS stacks ({tile} dispatches, {3l * nrec} planes)")
                t |> success(step >= nrec, "the per-op decode step recorded every recurrent layer's chain ({step} of {nrec})")
            }
            delete gpu
            delete cpu
            delete prompt
            delete toks
        }
        delete tr
    }
}

[test]
def test_gpu_resident_format_census_ba_f32(t : T?) {
    t |> run("{BA_F32.file}: the F32 beta/alpha rows ride the f32 GEMV and the f32 tile, no f16 router-form dispatch, the window and steps against the CPU chain") @(t : T?) {
        if (!family_on(t, "qwen35moe") || !arm_on(t, "vk-census")) return
        let path = path_join(models_dir(), BA_F32.file)
        if (!resident_ready(t, path)) return
        var tr <- load_resident(path, 1l)
        t |> success(tr.dn_ba_f32, "the file ships its beta/alpha rows F32")
        if (moe_fixture_admitted(t, tr, BA_F32) >= 0l) {
            let nrec = recurrent_layers(tr)
            var pf_ba = 0l
            var st_rt = 0l
            with_dasllama_jobque_() {
                var prompt <- resident_prompt(PROMPT)
                var toks <- resident_feed(STEPS)
                let ba0 = census("dn_ba_cls_spv")
                let f16_0 = census("router_gemv_f16_cls_spv")
                var rt0 = 0l
                var fed <- resident_then_cpu_feed(t, tr, prompt, toks, $ {
                    pf_ba = census("dn_ba_cls_spv") - ba0
                    rt0 = census("router_gemv_cls_spv")
                }, $ {
                    st_rt = census("router_gemv_cls_spv") - rt0
                    served_every_call(t)
                })
                feed_steps_within(t, tr, "F32 beta/alpha ", PROMPT, fed.gpu, fed.cpu, toks, BA_F32.bar_rel)
                if (ba0 != NO_CENSUS) {
                    t |> equal(pf_ba, nrec, "the window's f32 beta/alpha tile ran once per recurrent layer")
                    t |> success(st_rt >= nrec, "the token command recorded an f32 router-form GEMV per recurrent layer at least ({st_rt} over {nrec})")
                    t |> equal(census("router_gemv_f16_cls_spv") - f16_0, 0l, "no f16 router-form GEMV ran: no f16 copy of the beta/alpha rows")
                }
                delete fed
                delete prompt
                delete toks
            }
        }
        delete tr
    }
}
