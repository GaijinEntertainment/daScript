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
let private HOST_CTX = 16384l   //! the context a 16 GB card holds beside the dense planes once the first routed layers' experts leave for the host

//! `fx`'s file on the whole-model driver at `regions`: whole where the card holds the experts (the pod's shape), else again at
//! `HOST_CTX` asked whole with the first routed layers' experts on the host (this card's shape, the 35B's served form), `fx`
//! then expecting them so the admission reads the shape the load took
def private load_census(path : string; regions : int64; var fx : MoeFixture) : Model {
    var tr <- load_resident(path, regions)
    if (!moe_gpu_resident_active() && moe_gpu_resident_memory_decline()) {
        delete tr
        tr <- load_resident(path, regions, HOST_CTX)
        fx.strict_ctx = HOST_CTX
    }
    fx.expect_host_layers = moe_gpu_resident_active() && gpu_resident_host_layers() > 0l
    return <- tr
}

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

//! one speculative round's outcome on a session: its accept, its draft, the position it left and its verify row 0's logits
struct private Round {
    hit : bool
    draft : int64
    n_past : int64
    row0 : array<float>
}

//! one speculative round a fed token on `s` (the token forced, the round's own draft and verify), each round's outcome
def private spec_rounds(tr : Model; var s : Session; toks : array<int64>) : array<Round> {
    var out : array<Round>
    out |> reserve(long_length(toks))
    let vocab = tr.config.vocab_size
    for (tok in toks) {
        var acc = 0l
        let hit = mtp_spec_eval(tr, s, tok, acc)
        out |> emplace <| Round(hit = hit, draft = s.mtp_vbatch[1], n_past = s.n_past, row0 <- [for (i in range64(vocab)); s.mtp_logits_b[i]])
    }
    return <- out
}

//! a round's two verify rows off the device: the logits and the post-norm carry a row, the draft, the accepted count
struct private VerifyRows {
    draft : int64
    accepted : int64
    logits0, logits1 : array<float>
    carry0, carry1 : array<float>
}

//! one speculative round on `s` at its position: the draft, the two-row verify, the walk - the rows landed on the device
[cold_path]
def private verify_rows(tr : Model; var s : Session; tok : int64) : VerifyRows {
    var r : VerifyRows
    let vocab = tr.config.vocab_size
    let dim = tr.config.dim
    r.accepted = mtp_spec_round(tr, s, tok)
    r.draft = s.mtp_vbatch[1]
    r.logits0 <- [for (i in range64(vocab)); s.mtp_logits_b[i]]
    r.logits1 <- [for (i in range64(vocab)); s.mtp_logits_b[vocab + i]]
    r.carry0 <- [for (i in range64(dim)); s.mtp_hrows[i]]
    r.carry1 <- [for (i in range64(dim)); s.mtp_hrows[dim + i]]
    return <- r
}

//! where a stream ends on a session: its logits and its carry
struct private StreamEnd {
    logits : array<float>
    carry : array<float>
}

def private stream_end(s : Session) : StreamEnd => <- StreamEnd(logits <- [for (x in s.logits); x], carry <- [for (x in s.mtp_h); x])

//! `rounds` speculative rounds on `s` over `fed`, each round's token then its accepted drafts committed onto `toks`; where `s` ends
[cold_path]
def private committed_rounds(tr : Model; var s : Session; fed : array<int64>; rounds : int64; var toks : array<int64>) : StreamEnd {
    toks |> reserve(rounds * MTP_MAX_ROWS)
    spec_rounds_into(tr, s, toks, $(round, _committed) => round < rounds, $(round) => fed[round])
    return <- stream_end(s)
}

//! `toks` as one-row steps on `s` from its position: where `s` ends
[cold_path]
def private stepped(tr : Model; var s : Session; toks : array<int64>) : StreamEnd {
    for (tok in toks) {
        forward(tr, s, tok, s.n_past)
        s.n_past++
    }
    return <- stream_end(s)
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
        var fx = CENSUS
        var tr <- load_census(path, 1l, fx)
        census_cell(t, tr, path, "vulkan load", census_sites_all())
        for (l in range64(tr.config.n_layers)) {
            t |> equal(resident_layer_decline(tr, l), "", "layer {l} draws no decline from the whole-model driver")
        }
        if (moe_fixture_admitted(t, tr, fx) >= 0l) {
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
                let gathers = gpu_resident_embed_gathers()   //! read while the driver is armed: the CPU leg drops the model off the device
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
                    if (gathers) {
                        t |> success(pf_emb >= 1l, "the window gathered its token rows off the IQ4_XS table on the device ({pf_emb} dispatches)")
                        t |> equal(st_rows, STEPS, "every fed step's row gathered on the device off its token id, no host row uploaded")
                    } else {
                        t |> equal(st_rows, 0l, "the table's plane stayed on the host (the load log names the room): no step row gathered on the device")
                    }
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

//! the per-op FFN rail's carriers: a q8, a K-quant (Q8_K feed) and a per-32 (q51) dense FFN at the llama resident file's bar (of the
//! step's max |logit|), and a per-32 file whose table and classifier are per-32 too (iq4nl32) at 0.08: its per-op chain - the window's
//! GEMMs on the device's f16 tiles beside the CPU's int8 dots - reads 0.44-0.73 of a 10.7-19.6 max logit (3.7-6.1%), argmax flips only
//! at near-ties, the one-step-off control 7.4 and past
let private PEROP_FFN = fixed_array((file = "Llama-3.2-1B-Instruct-Q8_0.gguf", family = "llama", bar = 0.06),
    (file = "Llama-3.2-1B-Instruct-Q4_K_M.gguf", family = "llama", bar = 0.06),
    (file = "Llama-3.2-1B-Instruct-Q5_1-local.gguf", family = "llama", bar = 0.06),
    (file = "Qwen2.5-0.5B-Instruct-IQ4_NL-pure-local.gguf", family = "qwen2", bar = 0.08))

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
    for (spec in PEROP_FFN) {
        let file = spec.file
        t |> run("{file} with the whole-model route off: the per-op FFN rail runs every layer's FFN row on the device, the steps against the CPU chain") @(t : T?) {
            if (!family_on(t, spec.family) || !arm_on(t, "vk-perop-ffn")) return
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
                var qo_marked = 0l
                for (l in range64(tr.config.n_layers)) {
                    t |> equal(perop_dense_ffn_decline(tr, l), "", "layer {l}'s FFN triple is the per-op rail's formats")
                    let on_q = moe_gpu_dense_on_gpu(int(fmt_at(tr.wq_fmt, l)), tr.wq_offs[l])
                    qo_marked += on_q && moe_gpu_dense_on_gpu(int(fmt_at(tr.wo_fmt, l)), tr.wo_offs[l]) ? 1l : 0l
                }
                t |> equal(qo_marked, tr.config.n_layers, "the dense rail holds every layer's q/o pair in the file's format ({qo_marked} of {tr.config.n_layers})")
                let cls_fmt = tr.config.shared_weights ? tr.emb_fmt : tr.wcls_fmt
                t |> success(moe_gpu_cls_on_gpu(tr.wcls_off), "the classifier plane (format {int(cls_fmt)}) is the tier's")
                let rows0 = moe_gpu_dense_ffn_rows()
                let gemm0 = moe_gpu_dense_gemm_rows()
                var sg <- create_session(tr, mirror_kv_dtype())
                var gpu <- feed_logits(tr, sg, prompt, toks)
                let rows = moe_gpu_dense_ffn_rows() - rows0
                let gemm = moe_gpu_dense_gemm_rows() - gemm0
                t |> success(gemm >= 5l * tr.config.n_layers * PROMPT,
                    "the window's q/o and FFN GEMMs ran on the device over every layer's planes ({gemm} rows, {5l * tr.config.n_layers * PROMPT} owed)")
                delete sg
                moe_gpu_drop_model()
                let cpu_gemm0 = moe_gpu_dense_gemm_rows()
                var cpu <- feed_alone(tr, mirror_kv_dtype(), prompt, toks)
                t |> equal(moe_gpu_dense_gemm_rows() - cpu_gemm0, 0l, "the CPU chain with the model's device state dropped runs no dense GEMM row on the device (the GEMM counter's control)")
                feed_steps_within(t, tr, "{file} per-op FFN ", PROMPT, gpu, cpu, toks, spec.bar, "per-op")
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
        var fx = BA_F32
        var tr <- load_census(path, 1l, fx)
        t |> success(tr.dn_ba_f32, "the file ships its beta/alpha rows F32")
        if (moe_fixture_admitted(t, tr, fx) >= 0l) {
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
def test_gpu_resident_format_census_verify_rows(t : T?) {
    t |> run("{CENSUS.file}: the verify's rows through the split command's segments hold the one-row steps, and a run of rounds commits the stream the steps commit") @(t : T?) {
        if (!family_on(t, "qwen35moe") || !arm_on(t, "vk-census")) return
        let path = path_join(models_dir(), CENSUS.file)
        if (!resident_ready(t, path)) return
        let spec0 = get_mtp_spec()
        set_mtp_spec(true)
        var fx = CENSUS
        var tr <- load_census(path, 2l, fx)
        if (moe_fixture_admitted(t, tr, fx) >= 0l) {
            t |> success(rdec_verify_rows() >= 2l, "the verify seat holds two rows or more (the load log names any decline)")
            with_dasllama_jobque_() {
                var prompt <- resident_prompt(PROMPT)
                var toks <- resident_feed(ROUNDS + 1l)
                var s <- create_session(tr, mirror_kv_dtype())
                prefill_warm(tr, s, prompt)
                let verifies0 = moe_gpu_resident_verifies()
                var vr <- verify_rows(tr, s, toks[0])
                t |> equal(moe_gpu_resident_verifies(), verifies0 + 1l, "the round's verify ran on the device")
                let a0 = argmax_of(vr.logits0)
                let a1 = argmax_of(vr.logits1)
                to_log(LOG_INFO, "35B round: draft {vr.draft} '{decode_(tr, [vr.draft])}', row 0 argmax {a0} '{decode_(tr, [a0])}' ({a0 == vr.draft ? "accepts" : "rejects"}), row 1 argmax {a1} '{decode_(tr, [a1])}', accepted {vr.accepted}\n")
                t |> equal(vr.accepted, a0 == vr.draft ? 1l : 0l, "the round accepted the draft exactly where row 0's argmax is the draft")
                var sd <- create_session(tr, mirror_kv_dtype())
                prefill_warm(tr, sd, prompt)
                let verifies1 = moe_gpu_resident_verifies()
                forward(tr, sd, toks[0], PROMPT)
                var sl0 <- [for (x in sd.logits); x]
                var sh0 <- [for (x in sd.mtp_h); x]
                forward(tr, sd, vr.draft, PROMPT + 1l)
                var sl1 <- [for (x in sd.logits); x]
                var sh1 <- [for (x in sd.mtp_h); x]
                t |> equal(moe_gpu_resident_verifies(), verifies1, "control: the two one-row steps run no verify - the counter stands")
                row_within_ctrl(t, "verify row 0 against the split command's one-row step", vr.logits0, sl0, sl1, CENSUS.bar_rel, "(the step's row 1 as the control)")
                row_within_ctrl(t, "verify row 1 against the split command's one-row step", vr.logits1, sl1, sl0, CENSUS.bar_rel, "(the step's row 0 as the control)")
                row_within_ctrl(t, "verify carry row 0 against the split command's one-row step", vr.carry0, sh0, sh1, CENSUS.bar_rel, "(the step's carry row 1 as the control)")
                row_within_ctrl(t, "verify carry row 1 against the split command's one-row step", vr.carry1, sh1, sh0, CENSUS.bar_rel, "(the step's carry row 0 as the control)")
                delete sd
                delete s
                var sr <- create_session(tr, mirror_kv_dtype())
                prefill_warm(tr, sr, prompt)
                var fed <- [for (i in range64(ROUNDS)); toks[i]]
                var committed : array<int64>
                var rounds <- committed_rounds(tr, sr, fed, 4l, committed)
                var ss <- create_session(tr, mirror_kv_dtype())
                prefill_warm(tr, ss, prompt)
                var steps <- stepped(tr, ss, committed)
                var off <- stepped(tr, ss, [toks[ROUNDS]])
                let ar = argmax_of(rounds.logits)
                let ast = argmax_of(steps.logits)
                to_log(LOG_INFO, "35B rounds committed {long_length(committed)} tokens '{decode_(tr, committed)}'; next argmax rounds {ar} '{decode_(tr, [ar])}', steps {ast} '{decode_(tr, [ast])}'\n")
                t |> equal(sr.n_past, ss.n_past - 1l, "the rounds stand where the steps stand")
                row_within_ctrl(t, "the logits after four rounds against the same stream stepped", rounds.logits, steps.logits, off.logits, CENSUS.bar_rel, "(the steps one token further as the control)")
                row_within_ctrl(t, "the carry after four rounds against the same stream stepped", rounds.carry, steps.carry, off.carry, CENSUS.bar_rel, "(the steps' carry one token further as the control)")
                served_every_call(t)
                delete off
                delete steps
                delete ss
                delete rounds
                delete committed
                delete sr
                delete fed
                delete vr
                delete prompt
                delete toks
            }
        }
        delete tr
        set_mtp_spec(spec0)
    }
}

//! the device-home session's rounds `home` against the host-cached session's `host`, the forced-feed form: both took the same fed
//! token at the same position until their drafts part, so through the parting round each verify row 0 is one input on two K/V
//! homes - held within `bar_rel` with the host's previous round's row as the control; the drafts are the head's picks over the
//! whole vocabulary and can part on a near-tie, so the rounds compare exactly only while they agree
def private rounds_agree(t : T?; tr : Model; host, home : array<Round>; bar_rel : float) {
    var agreed = 0l
    for (r in range64(long_length(host))) {
        to_log(LOG_INFO, "round {r}: host-cached draft {host[r].draft} '{decode_(tr, [host[r].draft])}' hit {host[r].hit} at {host[r].n_past}, device-home draft {home[r].draft} '{decode_(tr, [home[r].draft])}' hit {home[r].hit} at {home[r].n_past}\n")
        let ctrl_round = r > 0l ? r - 1l : 1l
        row_within_ctrl(t, "round {r}: the device-home verify row 0 against the host-cached session's", home[r].row0, host[r].row0, host[ctrl_round].row0, bar_rel, "(the host's round {ctrl_round} as the control)")
        break if (home[r].draft != host[r].draft)
        t |> equal(home[r].hit, host[r].hit, "round {r}: both accept or both reject")
        t |> equal(home[r].n_past, host[r].n_past, "round {r}: both stand at one position")
        agreed++
    }
    to_log(LOG_INFO, "the device-home session rounds as the host-cached session for {agreed} of {long_length(host)} rounds before the drafts part\n")
}

[test]
def test_gpu_resident_format_census_nextn(t : T?) {
    t |> run("{CENSUS.file}: the routed NextN head on the device - its prompt rows warmed by the window chain, its draft against the CPU head's, every speculative round drafted and verified there, and a device-home session's rounds token for token the host-cached session's") @(t : T?) {
        if (!family_on(t, "qwen35moe") || !arm_on(t, "vk-census")) return
        let path = path_join(models_dir(), CENSUS.file)
        if (!resident_ready(t, path)) return
        let spec0 = get_mtp_spec()
        set_mtp_spec(true)
        var fx = CENSUS
        var tr <- load_census(path, 2l, fx)
        if (moe_fixture_admitted(t, tr, fx) >= 0l) {
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
                rounds_agree(t, tr, host, home, CENSUS.bar_rel)
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
