options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes: the CPU cell frees the target before the run ends
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — engine + arch registrations
require dasllama/dasllama_mtp_gemma   // the subject: the gemma-4 assistant drafter loader + CPU oracle
require dasllama/dasllama_math   // setup_dasllama_jobque_ - the CPU prefill needs the engine's own worker pool
require dasllama/dasllama_image   // nolint:STYLE030,LINT019 — load_model_cached is called in the das_metal half
require ?das_metal dasllama/dasllama_metal_mtp_gemma   // the subject's GPU half: attach + one draft step
require ?das_metal dasllama/dasllama_metal_common   // finish_pending_step - land the pipelined decode before reading its carry
require ?das_metal dasllama/dasllama_metal_decode   // metal_decode_shutdown
require ?das_metal dasllama/dasllama_metal_prefill   // the metal prefill override + shutdown
require ?das_metal metal/das_metal_boost   // metal_live_object_count - the leak gate
require daslib/jobque_boost
require daslib/fio
require math
require _model_tier   // models_dir() + model_available() presence/size-tier gate (see the file's header)

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // CPU-oracle test — prefills on the CPU by design
}

// The gemma-4 assistant drafter (`dasllama/dasllama_mtp_gemma`): the sidecar loader's shape and
// refusal contract, and one step of the CPU reference forward against a live target session.
// The drafter sidecar is a 440 MiB small-tier file; the 15.8 GiB target is PARITY_FULL-gated.

let private TARGET_FILE = "gemma-4-26B-A4B-it-Q4_K_M.gguf"
let private TARGET_LAYERS = 30l

// a Q8_0 tensor's byte cost in the drafter blob: whole 34-byte blocks, padded to the next
// 32-byte tensor start
def private q8_bytes(n : int64) : int64 => ((n / 32l) * 34l + 31l) / 32l * 32l

var private _tmp_dir : string
var private _tmp_dir_inited = false

def private fixture_path(name : string) : string {
    if (!_tmp_dir_inited) {
        let r = create_temp_directory_result("dasllama_gdraft")
        if (!(r is value)) {   // "" would resolve to the CWD and defeat the hermetic goal
            panic("could not create a temp dir for the drafter refusal fixtures: {unsafe(r.error)}")
        }
        _tmp_dir = unsafe(r.value)
        _tmp_dir_inited = true
    }
    return path_join(_tmp_dir, name)
}

[finalize]
def cleanup_temp_dir {
    if (_tmp_dir_inited && !empty(_tmp_dir)) {
        rmdir_rec(_tmp_dir)
    }
}

def private all_finite(a : array<float>) : bool {
    for (v in a) {
        if (!(abs(v) <= 1.0e30)) {   // one test for NaN and for inf/overflow: NaN fails every compare
            return false
        }
    }
    return true
}

def private greedy_top(logits : array<float>; vocab : int64) : int64 {
    var best = 0l
    var bv = logits[0]
    for (v in range64(vocab)) {
        if (logits[v] > bv) {
            bv = logits[v]
            best = v
        }
    }
    return best
}

// ===== refusals (no model file needed) =====

[test]
def test_gemma_drafter_refusals(t : T?) {
    if (!arm_on(t, "mtp-gdraft-refuse")) {
        return
    }
    t |> run("a missing path refuses with a reason instead of panicking") @(t : T?) {
        var dr <- load_gemma_drafter(path_join(fixture_path("."), "no-such-drafter.gguf"))
        t |> success(!dr.ok, "ok is false")
        t |> success(!empty(dr.why), "why names the refusal: '{dr.why}'")
        t |> success(empty(dr.blob) && empty(dr.fblob), "a refused load carries no weights")
        delete dr
    }
    t |> run("a file with no GGUF magic refuses with a reason") @(t : T?) {
        let p = fixture_path("junk.gguf")
        fopen(p, "wb") $(f) {
            if (f != null) {
                fwrite(f, "not a gguf file at all, but long enough to pass the length gate")
            }
        }
        var dr <- load_gemma_drafter(p)
        t |> success(!dr.ok, "ok is false")
        t |> success(find(dr.why, "magic") >= 0, "why names the magic: '{dr.why}'")
        delete dr
    }
    t |> run("gemma_drafter_sidecar never recurses onto a head, and refuses an absent sibling") @(t : T?) {
        t |> equal(gemma_drafter_sidecar(path_join(models_dir(), "mtp-gemma-4-26B-A4B-it-Q8_0.gguf")), "")
        t |> equal(gemma_drafter_sidecar(path_join(fixture_path("."), "no-such-model-Q4_K_M.gguf")), "")
    }
}

// ===== loader shape (the drafter sidecar alone — no target) =====

[test]
def test_gemma_drafter_load(t : T?) {
    if (!family_on(t, "gemma4moe") || !arm_on(t, "mtp-gdraft-load")) {
        return
    }
    t |> run("the gemma-4 drafter sidecar loads with the shape the encoder contract names") @(t : T?) {
        let target = path_join(models_dir(), TARGET_FILE)
        let side = gemma_drafter_sidecar(target)
        if (empty(side)) {
            t |> skip("no mtp-* drafter sidecar beside {TARGET_FILE}")
            return
        }
        t |> equal(base_name(side), "mtp-gemma-4-26B-A4B-it-Q8_0.gguf", "the sidecar prefers the Q8_0 head")
        if (!model_available(t, side)) return
        var dr <- load_gemma_drafter(side, TARGET_LAYERS)
        t |> success(dr.ok, "load ok (why='{dr.why}')")
        if (!dr.ok) {
            delete dr
            return
        }
        t |> equal(dr.dim, 1024l, "dim")
        t |> equal(dr.tdim, 2816l, "tdim (the target's hidden width)")
        t |> equal(dr.hidden, 8192l, "hidden")
        t |> equal(dr.vocab, 262144l, "vocab")
        t |> equal(dr.n_layers, 4l, "n_layers")
        t |> equal(dr.n_heads, 16l, "n_heads")
        t |> equal(dr.sliding_window, 1024l, "sliding_window")
        t |> success(abs(dr.eps - 1.0e-6) < 1.0e-10, "eps is 1e-6 (got {dr.eps})")
        t |> success(dr.rope_theta == 1.0e6 && dr.rope_theta_swa == 1.0e4,
            "rope theta {dr.rope_theta} full / {dr.rope_theta_swa} sliding")
        t |> success(length(dr.hs) == 4 && dr.hs[0] == 256l && dr.hs[1] == 256l && dr.hs[2] == 256l && dr.hs[3] == 512l,
            "hs = 256,256,256,512 (got {dr.hs})")
        t |> success(length(dr.kvh) == 4 && dr.kvh[0] == 8l && dr.kvh[1] == 8l && dr.kvh[2] == 8l && dr.kvh[3] == 2l,
            "kvh = 8,8,8,2 (got {dr.kvh})")
        t |> success(dr.sliding[0] && dr.sliding[1] && dr.sliding[2] && !dr.sliding[3],
            "sliding = T,T,T,F (got {dr.sliding})")
        t |> success(dr.kv_bound, "kv_layer is bound by the load's target_n_layers")
        t |> success(dr.kv_layer[0] == 28l && dr.kv_layer[1] == 28l && dr.kv_layer[2] == 28l && dr.kv_layer[3] == 29l,
            "kv_layer = 28,28,28,29 (got {dr.kv_layer})")
        t |> success(abs(dr.layer_scale[0] - 0.29101562) < 1.0e-6 && abs(dr.layer_scale[1] - 0.50390625) < 1.0e-6
            && abs(dr.layer_scale[2] - 0.52734375) < 1.0e-6 && abs(dr.layer_scale[3] - 0.41210938) < 1.0e-6,
            "layer_output_scale = {dr.layer_scale}")
        // the blob is every Q8_0 weight, each tensor padded to a 32-byte start
        var want_blob = q8_bytes(2l * dr.tdim * dr.dim) + q8_bytes(dr.dim * dr.tdim) + q8_bytes(dr.dim * dr.vocab)
        var want_fblob = dr.dim + dr.rope_freqs_n
        for (l in range64(dr.n_layers)) {
            let qd = dr.n_heads * dr.hs[l]
            want_blob += q8_bytes(dr.dim * qd) + q8_bytes(qd * dr.dim) + 3l * q8_bytes(dr.dim * dr.hidden)
            want_fblob += 4l * dr.dim + dr.hs[l]
        }
        t |> equal(long_length(dr.blob), want_blob, "blob bytes = the Q8_0 tensor sum")
        t |> equal(long_length(dr.fblob), want_fblob, "fblob floats = the F32 norm/table sum")
        t |> equal(dr.rope_freqs_n, 256l, "rope_freqs is 256 wide (the full class's half-dim)")
        // p-RoPE: 64 rotating dims followed by the pass-through tail
        t |> success(dr.fblob[dr.rope_freqs_off] == 1.0 && dr.fblob[dr.rope_freqs_off + 63l] == 1.0
            && dr.fblob[dr.rope_freqs_off + 64l] > 1.0e29,
            "rope_freqs = 64 x 1.0 then the 1e30 tail")
        var aligned = true
        var offs <- [dr.pre_proj_off, dr.post_proj_off, dr.tok_emb_off]
        offs |> reserve(3l + 5l * dr.n_layers)
        for (l in range64(dr.n_layers)) {
            offs |> push(dr.wq_offs[l])
            offs |> push(dr.wo_offs[l])
            offs |> push(dr.w1_offs[l])
            offs |> push(dr.w3_offs[l])
            offs |> push(dr.w2_offs[l])
        }
        for (o in offs) {
            aligned = aligned && o >= 0l && o % 32l == 0l && o < long_length(dr.blob)
        }
        t |> success(aligned, "every blob tensor starts 32-byte aligned and inside the blob ({offs})")
        delete dr
    }
}

// ===== the CPU oracle against a live target session =====

[test]
def test_gemma_drafter_cpu_step(t : T?) {
    if (!family_on(t, "gemma4moe") || !arm_on(t, "mtp-gdraft-cpu")) {
        return
    }
    with_job_que() {
        setup_dasllama_jobque_()
        t |> run("one CPU drafter step over the target's live K/V produces a finite token and hidden") @(t : T?) {
            if (!jit_enabled()) {
                t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                return
            }
            let target = path_join(models_dir(), TARGET_FILE)
            if (!model_available(t, target)) return
            let side = gemma_drafter_sidecar(target)
            if (empty(side)) {
                t |> skip("no mtp-* drafter sidecar beside {TARGET_FILE}")
                return
            }
            var tr <- load_model_(target, QuantMode.q8)
            tr.config.seq_len = min(tr.config.seq_len, 512l)
            t |> equal(tr.config.n_layers, TARGET_LAYERS, "the target carries the 30-layer gemma4 trunk")
            var dr <- load_gemma_drafter(side, tr.config.n_layers)
            t |> success(dr.ok, "drafter load ok (why='{dr.why}')")
            if (!dr.ok) {
                delete dr
                delete tr
                return
            }
            var prompt <- encode(tr, "The capital of France is", true, false)
            let npos = long_length(prompt)
            t |> success(npos >= 4l, "the prompt is {npos} tokens")
            var s <- make_run_state(tr.config, KVDtype.f16, KVDtype.f16)
            // prefill everything but the last token, then ONE decode step: the drafter's anchor is a
            // decode position, and forward() is what leaves the trunk residual the hidden is read from
            var head <- [for (i in range64(npos - 1l)); prompt[i]]
            eval_(tr, s, head)
            forward(tr, s, prompt[npos - 1l], npos - 1l)
            let anchor = npos   // the seed's own position = n_past: it ropes there and sees keys [0, npos)
            let tgt_next = greedy_top(s.logits, tr.config.vocab_size)
            var h : array<float>
            gemma_target_hidden(tr, s, true, h)
            t |> success(all_finite(h) && long_length(h) == tr.config.dim, "the target hidden is {long_length(h)} finite floats")
            var out : GemmaDraftOut
            gemma_draft_step_cpu(tr, s, dr, tgt_next, h, anchor, out)
            t |> success(all_finite(out.logits) && long_length(out.logits) == dr.vocab,
                "the draft logits are {long_length(out.logits)} finite floats")
            t |> success(out.token >= 0l && out.token < dr.vocab, "the draft token {out.token} is in [0, {dr.vocab})")
            t |> success(long_length(out.h_next) == dr.tdim && all_finite(out.h_next),
                "h_next is {long_length(out.h_next)} finite floats")
            var hn_rms = 0.0
            for (v in out.h_next) {
                hn_rms += v * v
            }
            hn_rms = sqrt(hn_rms / float(dr.tdim))
            t |> success(hn_rms > 0.0, "h_next carries signal (rms {hn_rms})")
            to_log(LOG_INFO, "gdraft: anchor {anchor} seed '{eyeball_str(piece(tr, tgt_next))}' ({tgt_next}) -> draft '{eyeball_str(piece(tr, out.token))}' ({out.token}), h_next rms {hn_rms}\n")
            delete out
            delete h
            delete head
            delete prompt
            delete s
            delete dr
            delete tr
        }
    }
}

def private maxd_of(a, b : array<float>) : float {
    var mx = 0.0
    for (x, y in a, b) {
        mx = max(mx, abs(x - y))
    }
    return mx
}

// the GPU drafter step against the CPU oracle on the served twin: same seed token, same carried
// hidden, same anchor, so the two must agree on the argmax and sit within GPU-vs-CPU f32 noise
[test]
def test_gemma_drafter_gpu_step(t : T?) {   // nolint:STYLE038 - one fixture: load, prefill, step, compare, teardown
    if (!family_on(t, "gemma4moe") || !arm_on(t, "mtp-gdraft-gpu")) {
        return
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        with_job_que() {
            setup_dasllama_jobque_()
            t |> run("the GPU drafter step matches the CPU oracle (argmax, logits, h_next)") @(t : T?) {
                if (!jit_enabled()) {
                    t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
                    return
                }
                let target = path_join(models_dir(), TARGET_FILE)
                if (!model_available(t, target)) return
                let side = gemma_drafter_sidecar(target)
                if (empty(side)) {
                    t |> skip("no mtp-* drafter sidecar beside {TARGET_FILE}")
                    return
                }
                let prior = get_metal_mode()
                set_metal_mode(MetalMode.required)
                var trg <- load_model_cached(target, QuantMode.q8)
                set_metal_mode(prior)
                trg.config.seq_len = min(trg.config.seq_len, 512l)
                if (!trg.metal_blob) {
                    t |> skip("the served image is not the metal-blob flavor on this box")
                    delete trg
                    return
                }
                var dr <- load_gemma_drafter(side, trg.config.n_layers)
                t |> success(dr.ok, "the oracle's drafter copy loads (why='{dr.why}')")
                t |> success(attach_gemma_drafter(trg, side), "the GPU drafter attaches to the served twin")
                var prompt <- encode(trg, "The capital of France is", true, false)
                let npos = long_length(prompt)
                var s <- make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
                var head <- [for (i in range64(npos - 1l)); prompt[i]]
                select_prefill_override("metal")
                set_metal_prefill_min_npos(1l)
                eval_(trg, s, head)
                select_prefill_override("")
                set_metal_prefill_min_npos(64l)
                select_decode_override("metal")
                var one <- [prompt[npos - 1l]]
                eval_(trg, s, one)   // one GPU decode step: lands the post-norm hidden the drafter feeds on
                finish_pending_step()
                t |> equal(s.mtp_h_pos1, s.n_past, "the decode carried the post-norm hidden for the drafter")
                let anchor = s.n_past
                let seed = greedy_top(s.logits, trg.config.vocab_size)
                var h := s.mtp_h
                var dg = 0l
                var hg : array<float>
                hg |> resize(dr.tdim)
                t |> success(gemma_draft_step_gpu(trg, s, seed, anchor, h, dg, hg), "the GPU draft step served")
                var lg : array<float>
                gemma_drafter_last_logits(lg)
                var out : GemmaDraftOut
                gemma_draft_step_cpu(trg, s, dr, seed, h, anchor, out)
                let dl = maxd_of(lg, out.logits)
                let dh = maxd_of(hg, out.h_next)
                t |> equal(out.token, dg, "GPU and CPU drafts agree on the token")
                t |> success(dl <= 0.25, "draft logits within 0.25 of the oracle (maxd {dl})")
                t |> success(dh <= 0.05, "h_next within 0.05 of the oracle (maxd {dh})")
                to_log(LOG_INFO, "gdraft gpu: anchor {anchor} seed {seed} -> gpu {dg} cpu {out.token}; logits maxd {dl}, h_next maxd {dh}\n")
                select_decode_override("")
                detach_gemma_drafter()
                metal_decode_shutdown()
                metal_prefill_shutdown()
                delete out
                delete lg
                delete hg
                delete h
                delete one
                delete head
                delete prompt
                delete s
                delete dr
                delete trg
                t |> equal(metal_live_object_count(), 0l, "no leaked Metal objects after shutdown")
            }
        }
    } else {
        t |> skip("no das_metal on this build")
    }
}
