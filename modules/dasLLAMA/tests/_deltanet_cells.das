options gen2
options _dasllama_internal = true

module _deltanet_cells shared public

//! the deltanet chunked-against-recurrent prefill cells on Qwen3.5-0.8B; no entry loads a model

require dastest/testing_boost public
require dasllama/dasllama_transformer   // umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require daslib/jobque_boost
require dasllama/dasllama_math  // with_dasllama_jobque_ (the engine-standard jobque config)
require dasllama/dasllama_gpu_tier   // the deltanet chain and decode-step route pins
require _model_tier   // argmax_of, logits_maxdiff, logits_maxabs
require _cpu_gpu_model   // with_ctx_cut: the cell's 2048-position context

// Chunked-vs-recurrent deltanet prefill equivalence on Qwen3.5-0.8B. The 20-token parity fixture
// covers only a single partial chunk, so this probe prefills 150 synthetic tokens — two full
// 64-token chunks plus a partial tail — exercising chunk boundaries, partial-chunk math, and the
// batched-conv history handoff. The two forms are NOT bit-identical (chunk FP reassociation;
// upstream accepts the same divergence between its autoregressive and chunking paths), so the
// gate is the forced-feed logits-tolerance form: the prefill logits within a bar of each other,
// then the recurrent arm's greedy token fed to BOTH arms for 8 steps with the logits compared
// within the bar at every step — which also proves the conv/S state the chunked prefill leaves
// behind feeds the recurrent decode correctly. The claim is the CPU chunk algebra, so the cell
// pins the GPU tier's deltanet chain and decode step off along with the vector activation forms.

let DN_LOGIT_BAR_REL = 0.04   // of the step's max |logit|; the two forms measure ~0.015 of it apart

//! On Qwen3.5-0.8B with GPU routes and vector activations pinned off, a 150-token chunked deltanet prefill lands within 4 percent of max logit of the recurrent one, and stays there over 8 forced-feed decode steps, while a one-step-off compare reds the bar; `tr`'s context cut to 2048 for it
def deltanet_chunked_prefill_cell(t : T?; var tr : Model) {
    with_ctx_cut(tr, 2048l) {   // KV RAM cap, same as the parity fixtures
        unsafe {
            t |> run("deltanet chunked prefill == recurrent (Qwen3.5-0.8B, multi-chunk)") @capture(& tr)(t : T ?) {
                let c = tr.config
                let prev_dn = get_deltanet_prefill_mode()
                // pin the scalar activation forms: this gate isolates the CHUNK algebra, and the two forms
                // are only reassociation-apart — exp4's ~2ulp is enough to flip a near-tie greedy pick and
                // derail the id lockstep (the exp4 paths are parity-gated by the frozen fixtures instead)
                let prev_vec = get_act_vec()
                set_act_vec(false)
                let prev_dn_route = moe_gpu_dn_route()
                let prev_dnd_route = moe_gpu_dnd_route()
                set_moe_gpu_dn_route(false)
                set_moe_gpu_dnd_route(false)
                with_dasllama_jobque_() {
                    let n = 150l
                    var prompt <- [for (i in range64(n)); 1000l + (i * 37l) % 5000l]

                    set_deltanet_prefill_mode(DnPrefillMode.recurrent)
                    var s1 <- make_run_state(c)
                    forward_prefill(tr, s1, prompt, n, 0l)
                    var ref <- [for (i in range64(c.vocab_size)); s1.logits[i]]

                    set_deltanet_prefill_mode(DnPrefillMode.chunked)
                    var s2 <- make_run_state(c)
                    forward_prefill(tr, s2, prompt, n, 0l)
                    var got <- [for (i in range64(c.vocab_size)); s2.logits[i]]

                    var tok = argmax_of(ref)
                    let prefill_diff = logits_maxdiff(got, ref)
                    let prefill_bar = DN_LOGIT_BAR_REL * logits_maxabs(ref)
                    to_log(LOG_INFO, "prefill: argmax recurrent {tok} ({ref[tok]}), chunked {argmax_of(got)} ({got[argmax_of(got)]}); logits maxdiff {prefill_diff} (bar {prefill_bar})\n")
                    t |> success(prefill_diff <= prefill_bar, "prefill logits: chunked within {prefill_bar} of recurrent (maxdiff {prefill_diff})")
                    delete ref
                    delete got

                    var prev_l1 : array<float>
                    for (st in range64(8l)) {
                        forward(tr, s1, tok, n + st)
                        forward(tr, s2, tok, n + st)
                        var inscope l1 <- [for (i in range64(c.vocab_size)); s1.logits[i]]
                        var inscope l2 <- [for (i in range64(c.vocab_size)); s2.logits[i]]
                        let step_diff = logits_maxdiff(l2, l1)
                        let step_bar = DN_LOGIT_BAR_REL * logits_maxabs(l1)
                        let a1 = argmax_of(l1)
                        let a2 = argmax_of(l2)
                        to_log(LOG_INFO, "step {st}: fed {tok}; argmax recurrent {a1} ({l1[a1]}), chunked {a2} ({l2[a2]}); logits maxdiff {step_diff} (bar {step_bar})\n")
                        t |> success(step_diff <= step_bar, "step {st} logits: chunked within {step_bar} of recurrent (maxdiff {step_diff})")
                        if (st > 0l) {   // the control: the previous step's recurrent logits are a disconnected mechanism the bar must RED
                            let off_diff = logits_maxdiff(l2, prev_l1)
                            t |> success(off_diff > step_bar, "step {st} control: the bar discriminates a one-step-off compare (maxdiff {off_diff} vs bar {step_bar})")
                        }
                        tok = a1
                        delete prev_l1
                        prev_l1 <- l1
                        delete l2
                    }
                    delete prev_l1
                    delete prompt
                    delete s1
                    delete s2
                }
                set_moe_gpu_dn_route(prev_dn_route)
                set_moe_gpu_dnd_route(prev_dnd_route)
                set_act_vec(prev_vec)
                set_deltanet_prefill_mode(prev_dn)
            }
        }
    }
}
