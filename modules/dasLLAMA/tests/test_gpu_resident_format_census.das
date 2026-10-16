options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes in the cells: the model and sessions free before exit
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // load_model_, create_session, moe_gpu_drop_model
require daslib/fio   // path_join
require dasllama/dasllama_math   // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // moe_gpu_dn_on_gpu, set_gpu_resident_route: the per-op rail's marks and the route lever
require dasllama/dasllama_gpu_resident   // resident_layer_decline, perop_dn_decline, perop_shexp_native
require dasllama/dasllama_blocks   // shexp_gpu_prefill_layers, span_tokens: the per-op shared expert's device witnesses
require dasllama/dasllama_moe   // shexp_gpu_decode_rows
require _moe_resident   // MoeFixture, moe_fixture_admitted, MOE_LOGIT_BAR_REL
require _format_census
require ?vulkan dasllama/dasllama_vulkan_decode   // vk_rdec_token_gathered: the token command's rows gathered off their ids, where the module exists

// The Vulkan arm of the weight-format census on Qwen3.6-35B-A3B-MTP-IQ4_XS-pure, every big plane IQ4_XS on disk (the
// deltanet beta/alpha included): each site's tag against the file's own tensor type, one soft check a site so the cell names
// every site still read as a copy; the whole-model driver armed on the file, a window prefill and fed decode steps against
// the CPU chain at the routed chain's bar with the IQ4_XS kernels' census rising, the untied IQ4_XS token table gathered on
// the device by the window and by every step; the routed NextN head on the device - its prompt rows warmed by the window
// chain, its draft against the CPU head's, every speculative round drafted and verified there, a device-home session's
// rounds token for token the host-cached session's; the same file with the whole-model route off, its deltanet layers on
// the per-op rail's IQ4_XS stacks; and a file whose beta/alpha ship F32 (Qwen3.6-35B-A3B UD-IQ2_XXS) read through the f32
// GEMV and tile with no f16 router-form dispatch. Large tier (DASLLAMA_PARITY_FULL=1), DASLLAMA_GPU=1, -jit; skips without
// the file, the armed tier, or the expert tiles the routed block needs. The tied K-quant tables' gather rides two smaller
// files (Qwen3-4B-Instruct-2507 Q4_K_M, Llama-3.2-3B-Instruct Q4_K_M), the window's and the steps' rows off the classifier's
// plane against the CPU chain at the split bar.

let private CENSUS = MoeFixture(file = "Qwen3.6-35B-A3B-MTP-IQ4_XS-pure.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL)
let private BA_F32 = MoeFixture(file = "Qwen3.6-35B-A3B-UD-IQ2_XXS.gguf", shexp = true, bar_rel = MOE_LOGIT_BAR_REL)
let private TIED_QWEN = "Qwen3-4B-Instruct-2507-Q4_K_M.gguf"
let private TIED_LLAMA = "Llama-3.2-3B-Instruct-Q4_K_M.gguf"
let private PROMPT = 40l   //! one window, past the per-op deltanet chain's 32-row floor
let private ROUNDS = 6l   //! the speculative rounds a NextN cell runs a session
let private STEPS = 6l

//! the one-row steps whose row the device gathered off the step's token id so far (NO_CENSUS when the module is absent)
def private gathered_rows() : int64 {
    static_if (typeinfo builtin_module_exists(vulkan)) {
        return vk_rdec_token_gathered()
    } else {
        return NO_CENSUS
    }
}

//! the census name of the token-table gather's stamp for a table in `f`
def private emb_kernel(f : KqFmt) : string => f == KqFmt.q8 ? "emb_gather_cls_spv" : (kq_block32(f) ? "{f}_emb_cls_spv" : "kq_emb_{f}_cls_spv")

//! one speculative round's outcome on a session: its accept, its draft and the position it left
struct private Round {
    hit : bool
    draft : int64
    n_past : int64
}

//! one speculative round a fed token on `s` (the token forced, the round's own draft and verify), each round's outcome
def private spec_rounds(tr : Model; var s : Session; toks : array<int64>) : array<Round> {
    var out : array<Round>
    out |> reserve(long_length(toks))
    for (tok in toks) {
        var acc = 0l
        let hit = mtp_spec_eval(tr, s, tok, acc)
        out |> push(Round(hit = hit, draft = s.mtp_vbatch[1], n_past = s.n_past))
    }
    return <- out
}

//! `prompt` prefilled on `s` with the speculative warm on, the session's position at its end
def private prefill_warm(tr : Model; var s : Session; prompt : array<int64>) {
    forward_prefill(tr, s, prompt, long_length(prompt), 0l)
    s.n_past = long_length(prompt)
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
        census_cell(t, tr, path, "vulkan load", census_sites_all())
        for (l in range64(tr.config.n_layers)) {
            t |> equal(resident_layer_decline(tr, l), "", "layer {l} draws no decline from the whole-model driver")
        }
        if (moe_fixture_admitted(t, tr, CENSUS) >= 0l) {
            t |> equal(gpu_resident_decline_(), "", "the whole-model driver armed: no decline reason")
            let nrec = recurrent_layer_count(tr.config)
            t |> success(!tr.config.shared_weights && emb_kq(tr) && tr.emb_fmt == KqFmt.iq4xs, "the file's token table is untied and kept as its own IQ4_XS plane ({tr.emb_fmt})")
            var pf_tile = 0l
            var st_gemv = 0l
            var pf_emb = 0l
            var st_rows = 0l
            with_dasllama_jobque_() {
                var prompt <- resident_prompt(PROMPT)
                var toks <- resident_feed(STEPS)
                let tile0 = census("kq_batch_iq4xs_cls_spv")
                let emb0 = census(emb_kernel(KqFmt.iq4xs))
                var gemv0 = 0l
                var rows0 = 0l
                var fed <- resident_then_cpu_feed(t, tr, prompt, toks, $ {
                    pf_tile = census("kq_batch_iq4xs_cls_spv") - tile0
                    pf_emb = census(emb_kernel(KqFmt.iq4xs)) - emb0
                    gemv0 = census_sum(["kq_gemv_iq4xs_cls_spv", "kq_gemv_iq4xs_n_cls_spv"])
                    rows0 = gathered_rows()
                }, $ {
                    st_gemv = census_sum(["kq_gemv_iq4xs_cls_spv", "kq_gemv_iq4xs_n_cls_spv"]) - gemv0
                    st_rows = gathered_rows() - rows0
                    served_every_call(t)
                })
                feed_steps_within(t, tr, "IQ4_XS census ", PROMPT, fed.gpu, fed.cpu, toks, CENSUS.bar_rel)
                if (tile0 != NO_CENSUS) {
                    t |> success(pf_tile >= 2l * nrec, "the window read every recurrent layer's beta and alpha planes through the IQ4_XS tile ({pf_tile} dispatches, {2l * nrec} planes)")
                    t |> success(st_gemv >= 5l * nrec, "the token command read every recurrent layer's five planes through the IQ4_XS GEMV ({st_gemv} recorded, {5l * nrec} planes)")
                    t |> success(pf_emb >= 1l, "the window gathered its token rows off the IQ4_XS table on the device ({pf_emb} dispatches)")
                    t |> equal(st_rows, STEPS, "every fed step's row gathered on the device off its token id, no host row uploaded")
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
        let nrec = recurrent_layer_count(tr.config)
        var marked = 0l
        for (l in range64(tr.config.n_layers)) {
            continue if (!layer_is_recurrent(tr.config, l))
            t |> equal(perop_dn_decline(tr, l), "", "layer {l}'s deltanet planes are the per-op chain's formats")
            marked += moe_gpu_dn_on_gpu(l) ? 1l : 0l
        }
        t |> equal(marked, nrec, "the per-op deltanet rail holds every recurrent layer on the device ({marked} of {nrec}) - none left on the CPU rail")
        var shexp_native = 0l
        for (l in range64(tr.config.n_layers)) {
            shexp_native += moe_gpu_shexp_on_gpu(l) && perop_shexp_native(tr, l) ? 1l : 0l
        }
        t |> equal(shexp_native, tr.config.n_layers, "the per-op rail holds every layer's shared expert in the file's own format ({shexp_native} of {tr.config.n_layers}) - no q8 transcode")
        let sh_pf0 = shexp_gpu_prefill_layers()
        let sh_ovl0 = shexp_gpu_decode_rows()
        let sh_gemm0 = shexp_gpu_gemm_rows()
        let sh_aft0 = shexp_gpu_after_rows()
        let sh_dec0 = sh_ovl0 + sh_aft0 + shexp_gpu_gemm_rows() + span_tokens()
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
            t |> success(shexp_gpu_prefill_layers() - sh_pf0 >= tr.config.n_layers, "the window ran every layer's shared expert on the device over its own planes ({shexp_gpu_prefill_layers() - sh_pf0} layers)")
            let sh_dec = shexp_gpu_decode_rows() + shexp_gpu_after_rows() + shexp_gpu_gemm_rows() + span_tokens() - sh_dec0
            t |> success(sh_dec >= STEPS, "every fed step ran the shared expert on the device - the span or the per-layer chain ({sh_dec} rows)")
            to_log(LOG_INFO, "per-op shared expert rows: overlapped chain {shexp_gpu_decode_rows() - sh_ovl0}, after the routed chain {shexp_gpu_after_rows() - sh_aft0}, dense GEMMs {shexp_gpu_gemm_rows() - sh_gemm0}\n")
            t |> equal(shexp_gpu_gemm_rows() - sh_gemm0, 0l, "one activation form a layer: no layer took the dense GEMMs - the overlapped chain or the span serves every one")
            delete gpu
            delete cpu
            delete prompt
            delete toks
        }
        delete tr
    }
}

//! the per-op FFN rail's carriers: a q8, a K-quant (Q8_K feed) and a per-32 (q51) dense FFN
let private PEROP_FFN = fixed_array("Llama-3.2-1B-Instruct-Q8_0.gguf", "Llama-3.2-1B-Instruct-Q4_K_M.gguf", "Llama-3.2-1B-Instruct-Q5_1-local.gguf")
let private PEROP_FFN_BAR = 0.06   //! of the step's max |logit|: the llama resident file's bar

//! `path` loaded with the whole-model route off and the per-op FFN rail pinned to `ffn` (0 or 1), both levers put back
def private load_perop(path : string; ffn : int) : Model {
    let route = gpu_resident_route_lever()
    let pin = gpu_dense_ffn_pin()
    set_gpu_resident_route(false)
    set_gpu_want_dense_ffn(ffn)
    var tr <- load_model_(path, QuantMode.q8)
    set_gpu_want_dense_ffn(pin)
    set_gpu_resident_route(route)
    return <- tr
}

[test]
def test_gpu_perop_dense_ffn(t : T?) {
    for (file in PEROP_FFN) {
        t |> run("{file} with the whole-model route off: the per-op FFN rail runs every layer's FFN row on the device, the steps against the CPU chain") @(t : T?) {
            if (!family_on(t, "llama") || !arm_on(t, "vk-perop-ffn")) return
            let path = path_join(models_dir(), file)
            if (!resident_ready(t, path)) return
            with_dasllama_jobque_() {
                var prompt <- resident_prompt(PROMPT)
                var toks <- resident_feed(STEPS)
                var off <- load_perop(path, 0)
                let off0 = moe_gpu_dense_ffn_rows()
                var so <- create_session(off, mirror_kv_dtype())
                var ctl <- feed_logits(off, so, prompt, toks)
                t |> equal(moe_gpu_dense_ffn_rows() - off0, 0l, "with the rail off no FFN row runs on the device (the counter's control)")
                delete so
                delete ctl
                delete off
                var tr <- load_perop(path, 1)
                t |> success(!moe_gpu_resident_active(), "the whole-model route is off: the per-op rails serve")
                for (l in range64(tr.config.n_layers)) {
                    t |> equal(perop_dense_ffn_decline(tr, l), "", "layer {l}'s FFN triple is the per-op rail's formats")
                }
                let rows0 = moe_gpu_dense_ffn_rows()
                var sg <- create_session(tr, mirror_kv_dtype())
                var gpu <- feed_logits(tr, sg, prompt, toks)
                let rows = moe_gpu_dense_ffn_rows() - rows0
                delete sg
                moe_gpu_drop_model()
                var cpu <- feed_alone(tr, mirror_kv_dtype(), prompt, toks)
                feed_steps_within(t, tr, "{file} per-op FFN ", PROMPT, gpu, cpu, toks, PEROP_FFN_BAR, "per-op")
                t |> equal(rows, STEPS * tr.config.n_layers, "every fed step ran each layer's FFN row on the device ({rows} rows)")
                delete gpu
                delete cpu
                delete tr
                delete prompt
                delete toks
            }
        }
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
            let nrec = recurrent_layer_count(tr.config)
            var pf_ba = 0l
            var st_rt = 0l
            with_dasllama_jobque_() {
                var prompt <- resident_prompt(PROMPT)
                var toks <- resident_feed(STEPS)
                let ba0 = census("dn_ba_cls_spv")
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
def test_gpu_resident_format_census_nextn(t : T?) {
    t |> run("{CENSUS.file}: the routed NextN head on the device - its prompt rows warmed by the window chain, its draft against the CPU head's, every speculative round drafted and verified there, and a device-home session's rounds token for token the host-cached session's") @(t : T?) {
        if (!family_on(t, "qwen35moe") || !arm_on(t, "vk-census")) return
        let path = path_join(models_dir(), CENSUS.file)
        if (!resident_ready(t, path)) return
        let spec0 = get_mtp_spec()
        set_mtp_spec(true)
        var tr <- load_resident(path, 2l)
        if (moe_fixture_admitted(t, tr, CENSUS) >= 0l) {
            t |> equal(tr.config.n_layer_nextn, 1l, "the -MTP- file carries its NextN head")
            t |> success(layer_is_moe(tr, tr.config.n_layers), "the head's FFN is routed")
            t |> success(rdec_verify_rows() >= 2l, "the routed head rides the driver: its verify seat holds two rows or more (the load log names any decline)")
            t |> success(gpu_resident_mtp_device_home_(tr), "every seat of the round runs on the device, so a device-home session speculates")
            with_dasllama_jobque_() {
                var prompt <- resident_prompt(PROMPT)
                var toks <- resident_feed(ROUNDS + 1l)
                var fed <- [for (i in range64(ROUNDS)); toks[i]]
                var s <- create_session(tr, mirror_kv_dtype())
                prefill_warm(tr, s, prompt)
                t |> equal(moe_gpu_resident_head_rows(s), PROMPT - 1l, "the window chain warmed the head's {PROMPT - 1l} prompt rows on the device")
                var h0 <- [for (x in s.mtp_h); x]
                let row = PROMPT - 1l
                let drafts0 = moe_gpu_resident_drafts()
                var dev <- draft_from(tr, s, toks[0], row, h0)
                t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the routed head drafted on the device")
                let seat = unset_rdec_draft()
                var cpu <- draft_from(tr, s, toks[0], row, h0)
                var ctrl <- draft_from(tr, s, toks[ROUNDS], row, h0)
                reinstall_rdec_draft(seat)
                t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l, "the unset seat left both CPU drafts to forward_mtp")
                row_within_ctrl(t, "the routed head's draft", dev.logits, cpu.logits, ctrl.logits, CENSUS.bar_rel, "(the CPU draft of another token as the control)")
                pick_agrees(t, tr, "the routed head's draft", argmax_of(dev.logits), cpu.logits, CENSUS.bar_rel)
                copy_floats(h0, 0l, s.mtp_h, 0l, long_length(h0))
                s.mtp_h_pos1 = PROMPT
                let verifies0 = moe_gpu_resident_verifies()
                var host <- spec_rounds(tr, s, fed)
                t |> equal(moe_gpu_resident_drafts(), drafts0 + 1l + ROUNDS, "every round drafted on the device")
                t |> equal(moe_gpu_resident_verifies(), verifies0 + ROUNDS, "every round verified on the device")
                t |> equal(moe_gpu_resident_head_rows(s), s.n_past - 1l, "the region holds the head's rows through the session's position less one")
                var sd <- create_device_session(tr, mirror_kv_dtype())
                t |> success(sd.device_kv, "the second session is device-home: its K/V lives in the driver's region alone")
                prefill_warm(tr, sd, prompt)
                t |> equal(moe_gpu_resident_head_rows(sd), PROMPT - 1l, "the device-home session's head rows warmed on the device, with no host copy")
                let drafts1 = moe_gpu_resident_drafts()
                let verifies1 = moe_gpu_resident_verifies()
                var home <- spec_rounds(tr, sd, fed)
                t |> equal(moe_gpu_resident_drafts(), drafts1 + ROUNDS, "every device-home round drafted on the device")
                t |> equal(moe_gpu_resident_verifies(), verifies1 + ROUNDS, "every device-home round verified on the device")
                for (r in range64(ROUNDS)) {
                    to_log(LOG_INFO, "round {r}: host-cached draft {host[r].draft} '{decode_(tr, [host[r].draft])}' hit {host[r].hit} at {host[r].n_past}, device-home draft {home[r].draft} '{decode_(tr, [home[r].draft])}' hit {home[r].hit} at {home[r].n_past}\n")
                    t |> equal(home[r].draft, host[r].draft, "round {r}: the device-home session drafts the host-cached session's token - one command over the same rows")
                    t |> equal(home[r].hit, host[r].hit, "round {r}: both accept or both reject")
                    t |> equal(home[r].n_past, host[r].n_past, "round {r}: both stand at one position")
                }
                served_every_call(t)
                delete sd
                delete home
                delete host
                delete s
                delete dev
                delete cpu
                delete ctrl
                delete h0
                delete fed
                delete prompt
                delete toks
            }
        }
        delete tr
        set_mtp_spec(spec0)
    }
}

//! `file` on the whole-model driver: the window's token rows and every fed step's row gathered on the device off the table's
//! plane (the tied classifier's where the table is tied), the forced feed against the CPU chain at the split bar
def private emb_gather_cell(t : T?; file : string; bar_rel : float) {
    let path = path_join(models_dir(), file)
    if (!resident_ready(t, path)) return
    var tr <- load_resident(path, 1l)
    t |> success(moe_gpu_resident_active(), "{file}: the whole-model driver armed (the load log names any decline)")
    if (moe_gpu_resident_active()) {
        t |> success(tr.config.shared_weights && emb_kq(tr), "{file}: the table is tied and kept as the classifier's {tr.emb_fmt} plane")
        let kernel = emb_kernel(tr.emb_fmt)
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(PROMPT)
            var toks <- resident_feed(STEPS)
            let emb0 = census(kernel)
            var pf_emb = 0l
            var rows0 = 0l
            var st_rows = 0l
            var fed <- resident_then_cpu_feed(t, tr, prompt, toks, $ {
                pf_emb = census(kernel) - emb0
                rows0 = gathered_rows()
            }, $ {
                st_rows = gathered_rows() - rows0
                served_every_call(t)
            })
            feed_steps_within(t, tr, "{file} gather ", PROMPT, fed.gpu, fed.cpu, toks, bar_rel)
            if (emb0 != NO_CENSUS) {
                t |> success(pf_emb >= 1l, "{file}: the window gathered its token rows off the tied {tr.emb_fmt} plane ({kernel}, {pf_emb} dispatches)")
                t |> equal(st_rows, STEPS, "{file}: every fed step's row gathered on the device off its token id, no host row uploaded")
            }
            delete fed
            delete prompt
            delete toks
        }
    }
    delete tr
}

//! the Qwen3-4B Q4_K_M chain's device-vs-CPU distance: its K-quant sites read 0.38-0.62 of a 7.3-11.3 max logit (0.073 at its
//! widest step, the argmax the CPU's at every row) - the chain's rounding: the gathered rows are the class lane decode
//! (`test_vkd_emb_gather_family`); the one-step-off control reads 5.4 and past
let private QWEN_KQ_BAR_REL = 0.10

[test]
def test_gpu_resident_gather_tied_qwen3(t : T?) {
    t |> run("{TIED_QWEN}: the tied K-quant table gathered on the device by the window and by every decode step") @(t : T?) {
        if (!family_on(t, "qwen3") || !arm_on(t, "vk-gather")) return
        emb_gather_cell(t, TIED_QWEN, QWEN_KQ_BAR_REL)
    }
}

//! the per-32 table carriers' bar: the 0.5B qwen2 file's forced-feed reading (test_gpu_resident_qwen2.das' LOGIT_BAR_REL)
let private PER32_TABLE_BAR_REL = 0.06

//! a per-32 carrier on the whole-model driver: every site the file carries read in its own format (the census), the tied per-32
//! table gathered on the device by the window and every step, the forced feed against the CPU chain
def private per32_table_cell(t : T?; file : string; bar_rel : float) {
    let path = path_join(models_dir(), file)
    if (!resident_ready(t, path)) return
    {
        var tr <- load_model_(path, QuantMode.q8)
        t |> success(kq_block32(cls_fmt_of(tr)) && emb_kq(tr), "{file}: the tied table loads in its per-32 form ({tr.emb_fmt})")
        census_cell(t, tr, path, "vulkan load", ["attention q/k/v/o", "token table", "classifier"])
        delete tr
    }
    emb_gather_cell(t, file, bar_rel)
}

[test]
def test_gpu_resident_per32_tables(t : T?) {
    for (spec in [(file = "Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf", arm = "vk-per32-iq4nl32"),
                  (file = "Qwen2.5-0.5B-Instruct-Q4_0-local.gguf", arm = "vk-per32-q4032"),
                  (file = "Qwen2.5-0.5B-Instruct-Q5_0-pure-local.gguf", arm = "vk-per32-q5032")]) {
        t |> run("{spec.file}: the per-32 token table and classifier on the device, census and forced feed") @(t : T?) {
            if (!family_on(t, "qwen2") || !arm_on(t, spec.arm)) return
            per32_table_cell(t, spec.file, PER32_TABLE_BAR_REL)
        }
    }
    t |> run("Qwen3.5-0.8B-MTP-Q5_1-local.gguf: the q51 table, classifier, deltanet planes and eh_proj on the device") @(t : T?) {
        if (!family_on(t, "qwen35") || !arm_on(t, "vk-per32-q51")) return
        let path = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q5_1-local.gguf")
        if (!resident_ready(t, path)) return
        var tr <- load_model_(path, QuantMode.q8)
        census_cell(t, tr, path, "vulkan load", ["deltanet qkv/gate/out", "deltanet beta/alpha", "token table", "classifier", "NextN eh_proj"])
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the per-32 hybrid (the load log names any decline)")
        t |> success(rdec_verify_rows() >= 2l, "the NextN head - its q51 eh_proj among its planes - rides the driver (the load log names any decline)")
        delete tr
        emb_gather_cell(t, "Qwen3.5-0.8B-MTP-Q5_1-local.gguf", 0.06)
    }
}

[test]
def test_gpu_resident_gather_tied_llama(t : T?) {
    t |> run("{TIED_LLAMA}: the tied Q6_K table gathered on the device by the window and by every decode step") @(t : T?) {
        if (!family_on(t, "llama") || !arm_on(t, "vk-gather")) return
        emb_gather_cell(t, TIED_LLAMA, SPLIT_BAR_REL)
    }
}
