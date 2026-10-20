options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_transformer
require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require dasllama/dasllama_math
require dasllama/dasllama_gpu_tier
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_gguf
require dasllama/dasllama_sampling
require dasllama/dasllama_env   // pf_window_rows, PF_ROWS_WIDE: the window the plan picks with no pin
require dasllama/dasllama_scheduler   // chunk_defaults, GPU_IDLE_CHUNK_TOKENS: the idle chunk follows the driver's window
require _compares
require _model_tier
require _resident_feed
require _child_run

// The hyper-connection chain's k-deep speculative round on a carrier this box holds whole: Lynxpda's micro-qwen4exp (random
// weights in the real 4-layer cycle, hc 4, the PLE 3-gram side input, 8 experts top-2, a NextN head; 416 MB trunk + 149 MB
// head, `micro-qwen4exp/` under the models dir). The head shard as published carries no head mixer (nextn.hc_head_*); the
// `-hcmixer` twin the cells load adds one as a copy of the shard's own output_hc_* mixer (the same geometry, random either way).
// Random weights say nothing about text, so the cells read the driver's counters and the n-gram conv ring: a round of four
// rows forced to reject at row 0 rolls the ring back on the device - the rejected rows' slots hold their pre-round bytes again
// and the committed row's slot its row - with no verify passed to the CPU. The trunk and the published head are the
// `performance/model_specs.das` row's file and companion; the `-hcmixer` twin is minted by
// `harness/micro_qwen4exp_hcmixer.py`. Skips: no trunk or head file under the models dir, the GPU tier not armed
// (DASLLAMA_GPU=1 on a device that serves the whole-model driver), not `-jit`.

let private MICRO_TRUNK = "micro-qwen4exp/micro-qwen4exp-realqsa.gguf"
let private MICRO_HEAD = "micro-qwen4exp/micro-qwen4exp-realqsa-mtp-hcmixer.gguf"
let private MICRO_PROMPT = 40l
let private DEPTH = 3l

def private micro_ready(t : T?) : bool => resident_ready(t, path_join(models_dir(), MICRO_TRUNK)) && model_available(t, path_join(models_dir(), MICRO_HEAD))

//! the trunk loaded at q8 with the `-hcmixer` head pinned as its head, the head pin put back as it was found
def private load_micro() : Model {
    let head_pin = mtp_head_path_pin()
    set_mtp_head_path(path_join(models_dir(), MICRO_HEAD))
    var tr <- load_model_(path_join(models_dir(), MICRO_TRUNK), QuantMode.q8)
    if (head_pin.set) {
        set_mtp_head_path(head_pin.path)
    } else {
        clear_mtp_head_path()
    }
    return <- tr
}

//! the live region's n-gram conv ring off the device: [hist_rows x hc x dim] floats
def private live_ring(tr : Model) : array<float> {
    let c = tr.config
    var ring : array<float>
    ring |> resize(ple_hist_rows(c) * hc_dim(c))
    verify(rdec_dn_snap_ring(-1l, unsafe(addr(ring[0]))), "the driver landed no ring (no n-gram side input, or the driver is off)")
    return <- ring
}

//! the largest difference between the two rings' slot of position `pos` (the slots wrap at the ring's rows)
def private slot_diff(c : Config; a, b : array<float>; pos : int64) : float {
    let hcd = int(hc_dim(c))
    let slot = int(pos % ple_hist_rows(c))
    return span_maxdiff(a, b, slot * hcd, slot * hcd, hcd).diff
}

[test]
def test_gpu_resident_hc_micro_round_depth(t : T?) {
    t |> run("micro-qwen4exp at depth 3: the hyper-connection verify rail serves four rows on the device, and a reject at row 0 puts the n-gram ring's rejected slots back and recommits the committed row") @(t : T?) {
        if (!micro_ready(t)) return
        let depth_pin = mtp_depth_pin()
        set_mtp_depth(DEPTH)
        var tr <- load_micro()
        let c = tr.config
        t |> success(c.hyper_conn && has_ple_ngram(c) && c.n_layer_nextn > 0l && c.recr_mask != 0ul, "the fixture is the hyper-connection chain with the n-gram side input, recurrent layers and a NextN head")
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the trunk and its head (the load log names any decline)")
        t |> equal(rdec_verify_rows(), DEPTH + 1l, "the driver prepared the depth-3 round's four verify rows, and the hyper-connection rail admits them")
        if (pf_window_rows_pin() <= 0l) {
            t |> equal(pf_window_rows(), PF_ROWS_WIDE, "with no window pinned the plan picks the wide window: the micro model fits it with its scratch (DASLLAMA_GPU_PF_ROWS unset)")
        }
        let defaults = chunk_defaults(tr)
        t |> equal(defaults.idle, max(GPU_IDLE_CHUNK_TOKENS, gpu_device_prefill_window()), "the idle chunk is the wider of the GPU floor and the driver's prompt window ({gpu_device_prefill_window()})")
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(MICRO_PROMPT)
            var s <- create_session(tr, mirror_kv_dtype())
            let spec0 = get_mtp_spec()
            set_mtp_spec(true)
            forward_prefill(tr, s, prompt, MICRO_PROMPT, 0l)
            s.n_past = MICRO_PROMPT
            var before <- live_ring(tr)
            let force0 = get_mtp_force_reject_every()
            set_mtp_force_reject_every(1l)   // every round rejects at row 0: the committed token's row stands, the three drafts' rows roll back
            let verifies0 = moe_gpu_resident_verifies()
            let rollbacks0 = moe_gpu_resident_rollbacks()
            let passes0 = pass_count("verify_rows")
            let tok = argmax_of(s.logits)
            let acc = mtp_spec_round(tr, s, tok)
            set_mtp_force_reject_every(force0)
            set_mtp_spec(spec0)
            t |> equal(acc, 0l, "the forced reject accepted no draft")
            t |> equal(moe_gpu_resident_verifies() - verifies0, 1l, "the round's verify ran on the device")
            t |> equal(moe_gpu_resident_verify_rows(), DEPTH + 1l, "the device verify landed the four rows")
            t |> equal(moe_gpu_resident_rollbacks() - rollbacks0, 1l, "the reject rolled the region back on the device")
            t |> equal(pass_count("verify_rows") - passes0, 0l, "no verify passed to the CPU as verify_rows")
            t |> equal(s.n_past, MICRO_PROMPT + 1l, "the position moved by the committed token alone")
            var after <- live_ring(tr)
            let committed = slot_diff(c, before, after, MICRO_PROMPT)
            var rejected = 0.0
            for (r in range64(1l, DEPTH + 1l)) {
                rejected = max(rejected, slot_diff(c, before, after, MICRO_PROMPT + r))
            }
            to_log(LOG_INFO, "micro hc round: the committed row's slot moved by {committed}, the rejected rows' slots by {rejected}\n")
            t |> success(is_finite(committed) && committed > 0.0, "the committed row's slot holds its row, not the pre-round bytes ({committed})")
            t |> equal(rejected, 0.0, "the three rejected rows' slots hold their pre-round bytes again, bit for bit")
            // the no-route leg: a plain step after the round verifies nothing and rolls nothing back
            let verifies1 = moe_gpu_resident_verifies()
            let rollbacks1 = moe_gpu_resident_rollbacks()
            forward(tr, s, argmax_of(s.logits), s.n_past)
            t |> equal(moe_gpu_resident_verifies() - verifies1, 0l, "control: a plain step runs no verify - the verify counter stands")
            t |> equal(moe_gpu_resident_rollbacks() - rollbacks1, 0l, "control: a plain step rolls nothing back - the rollback counter stands")
            delete before
            delete after
            delete s
            delete prompt
        }
        restore_mtp_depth(depth_pin)
        delete tr
    }
}

//! `rounds` greedy speculative rounds on a fresh session over `prompt` under the chain cut `cut`: the drafts each round made
def private drafts_per_round(tr : Model; prompt : array<int64>; rounds : int64; cut : float) : array<int64> {
    var out : array<int64>
    out |> reserve(rounds)
    set_mtp_chain_min_p(cut)
    var s <- create_session(tr, mirror_kv_dtype())
    let spec0 = get_mtp_spec()
    set_mtp_spec(true)
    forward_prefill(tr, s, prompt, long_length(prompt), 0l)
    s.n_past = long_length(prompt)
    var sp = SamplingParams()
    s.spec_params = unsafe(addr(sp))
    var tok = sample_(s, sp)
    for (_r in range64(rounds)) {
        let drafted0 = s.mtp_drafted
        mtp_spec_round(tr, s, tok)
        out |> push(s.mtp_drafted - drafted0)
        tok = sample_(s, sp)
    }
    s.spec_params = null
    set_mtp_spec(spec0)
    delete s
    return <- out
}

[test]
def test_gpu_resident_hc_micro_chain_cut(t : T?) {
    t |> run("micro-qwen4exp at depth 3 under a chain cut of 0.99: random weights hold every draft under the floor, so each round drafts one row; with the cut at 0 each round drafts three") @(t : T?) {
        if (!micro_ready(t)) return
        let depth_pin = mtp_depth_pin()
        let cut0 = get_mtp_chain_min_p()
        set_mtp_depth(DEPTH)
        var tr <- load_micro()
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the trunk and its head")
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(MICRO_PROMPT)
            var whole <- drafts_per_round(tr, prompt, 4l, 0.0)
            var cut <- drafts_per_round(tr, prompt, 4l, 0.99)
            to_log(LOG_INFO, "micro hc chain cut: drafts a round with the cut off {whole}, at 0.99 {cut}\n")
            for (r in range64(4l)) {
                t |> equal(whole[r], DEPTH, "round {r} with the cut off drafts the depth's three rows")
                t |> equal(cut[r], 1l, "round {r} under the 0.99 cut drafts one row: the first draft's probability under the head sits under the floor")
            }
            delete whole
            delete cut
            delete prompt
        }
        set_mtp_chain_min_p(cut0)
        restore_mtp_depth(depth_pin)
        delete tr
    }
}

[test]
def test_gpu_resident_hc_micro_sampled_selection(t : T?) {
    t |> run("micro-qwen4exp at depth 2: a sampled stream through the device's selection on the hyper-connection chain decodes the rows-landed stream token for token, the same drafts accepted") @(t : T?) {
        if (!micro_ready(t)) return
        let depth_pin = mtp_depth_pin()
        set_mtp_depth(2l)
        var tr <- load_micro()
        t |> success(moe_gpu_resident_active(), "the whole-model driver armed on the trunk and its head")
        with_dasllama_jobque_() {
            var prompt <- resident_prompt(MICRO_PROMPT)
            var sp = SamplingParams(temp = 0.7, top_p = 0.9)
            let sel0 = get_select_landing()
            set_select_landing(false)
            var rows <- sampled_stream(tr, prompt, 24l, sp, 4242)
            set_select_landing(true)
            var sel <- sampled_stream(tr, prompt, 24l, sp, 4242)
            set_select_landing(sel0)
            to_log(LOG_INFO, "micro hc sampled: {length(sel.ids)} tokens, {sel.selections} selections, {sel.late_landings} late landings, {sel.accepted} against {rows.accepted} drafts accepted\n")
            to_log(LOG_INFO, "micro hc sampled: the rows-landed stream {rows.ids}\n")
            to_log(LOG_INFO, "micro hc sampled: the selection-landed stream {sel.ids}\n")
            t |> equal(rows.selections, 0l, "the rows-landed stream asked no selection")
            t |> success(sel.selections > 0l, "the selection served the hyper-connection chain's steps ({sel.selections} selections)")
            // what makes the two streams draw alike: random weights give every row a flat nucleus that reaches past the landed
            // survivors, so every landing falls back to the rows and both streams draw off the same rows with the same seed - the
            // plumbing's witness, not the selection's (test_sampling_landed.das holds the landed draw's equivalence)
            t |> equal(sel.late_landings, sel.selections, "every landing fell back to the rows: a random row's nucleus reaches past the survivors ({sel.late_landings} late landings of {sel.selections} selections)")
            t |> success(ids_equal(rows.ids, sel.ids), "the two streams agree token for token ({length(sel.ids)} tokens)")
            t |> equal(sel.accepted, rows.accepted, "the walk accepts the same drafts under the selection")
            delete rows
            delete sel
            delete prompt
        }
        restore_mtp_depth(depth_pin)
        delete tr
    }
}

[test]
def test_gpu_resident_hc_micro_single_queue(t : T?) {
    t |> run("micro-qwen4exp with the transfer queue off (DASLLAMA_VK_XFERQ=0): the host-experts form's streamed bundles and token command run on the compute timeline alone - a child loads, prefills and steps") @(t : T?) {
        if (!micro_ready(t)) return
        var out : string
        let rc = run_child(child_cmd("_resident_load_root.das", [(name = "DASLLAMA_VK_XFERQ", value = "0"), (name = "DAS_LOG_LEVEL", value = "info")],
            "\"{path_join(models_dir(), MICRO_TRUNK)}\" 0 step", true), out)
        t |> equal(rc, 0, "the single-queue child ran to its end:\n{out}")
        t |> success(find(out, "RESIDENT_LOAD active=true") >= 0, "the whole-model driver armed without a transfer family:\n{out}")
        t |> success(find(out, "transfer queue armed") < 0, "no transfer queue served the child:\n{out}")
        t |> success(find(out, "RESIDENT_STEP prefill=") >= 0, "the child's prefill and token step ran on the compute timeline alone:\n{out}")
    }
}
