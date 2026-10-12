// The segmented window's race: S sessions' prompts of N tokens each as ONE window against the same S as
// S windows in sequence, on a blob twin, wall time a form. Prints a row a prompt length and stream count.
//
//   bin/daslang -jit modules/dasLLAMA/harness/seg_window_race.das -- --model <gguf> [--lens 3000,9000] [--streams 2] [--reps 3]
//
// DASLLAMA_METAL_TENSOR=0 is the M4 arm. The prompts differ, so a crossed segment would show in the logits
// (logged as the last-row max difference against the sequential run's, the widest over the streams).

options gen2
options stack = 524288
options _dasllama_internal = true

require daslib/clargs
require dasllama/dasllama
require dasllama/dasllama_common   // nolint:STYLE030,LINT019 - eval_, make_run_state, eval_segments_
require dasllama/dasllama_math     // with_dasllama_jobque_
require ../tests/_metal_blob_twin.das
require ../tests/_compares.das     // nolint:STYLE030,LINT019 - logits_maxdiff
require ?das_metal dasllama/dasllama_metal_prefill   // metal_prefill_segment_windows, metal_prefill_shutdown
require dastest/testing_boost
require _race_fixture   // int_list, counting_tokens

[CommandLineArgs]
struct RaceArgs {
    @clarg_doc = "The GGUF to race (its blob twin is minted in memory)"
    model : string
    @clarg_doc = "Comma list of prompt lengths in tokens"
    lens : string = "3000,9000"
    @clarg_doc = "Comma list of stream counts: S prompts as S windows in sequence against one window of S segments"
    streams : string = "2"
    @clarg_doc = "Timed repetitions a row (the fastest counts)"
    reps : int = 3
}

[export]
def main : int {
    var args = RaceArgs()
    let rc = parse_args_with_help(args, "seg_window_race")
    return rc if (rc >= 0)
    if (empty(args.model)) {
        to_log(LOG_ERROR, "seg_window_race: --model <gguf> is required\n")
        return 2
    }
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var lens <- int_list(args.lens)
        var streams <- int_list(args.streams)
        set_metal_mode(MetalMode.when_available)
        var t = new T(name = "seg_window_race")
        var tr <- blob_twin(t, args.model, 20000l)
        with_dasllama_jobque_() {
            for (n in lens) {
                for (ns in streams) {
                    var prompts <- [for (k in range64(ns)); counting_tokens(tr, n, 1l + k * 100000l)]
                    var best_seq = 1.0e30
                    var best_one = 1.0e30
                    var inscope seq_logits, one_logits : array<array<float>>
                    seq_logits |> resize(ns)
                    one_logits |> resize(ns)
                    for (_rep in range(args.reps)) {
                        var seq <- [for (_k in range64(ns)); make_run_state(tr.config, KVDtype.f16, KVDtype.f16)]
                        let t0 = ref_time_ticks()
                        for (k in range64(ns)) {
                            eval_(tr, seq[k], prompts[k])
                        }
                        best_seq = min(best_seq, float(get_time_usec(t0)) / 1000.0)
                        for (k in range64(ns)) {
                            seq_logits[k] := seq[k].logits
                        }
                        delete seq
                        var ones <- [for (_k in range64(ns)); make_run_state(tr.config, KVDtype.f16, KVDtype.f16)]
                        var sess <- [for (k in range64(ns)); unsafe(addr(ones[k]))]
                        var toks := prompts
                        let sw0 = metal_prefill_segment_windows()
                        let t1 = ref_time_ticks()
                        eval_segments_(tr, sess, toks)
                        best_one = min(best_one, float(get_time_usec(t1)) / 1000.0)
                        if (ns > 1l && metal_prefill_segment_windows() - sw0 != 1l) {
                            to_log(LOG_ERROR, "seg_window_race: the {ns} streams did not ride one window at n={n}\n")
                        }
                        for (k in range64(ns)) {
                            one_logits[k] := ones[k].logits
                        }
                        sess |> clear()
                        delete toks
                        delete ones
                    }
                    var dmax = 0.0
                    for (k in range64(ns)) {
                        dmax = max(dmax, logits_maxdiff(one_logits[k], seq_logits[k]))
                    }
                    print("n={n} x {ns} streams: {ns} windows {best_seq} ms, one window {best_one} ms ({best_one / best_seq * 100.0}%), last-row logits max difference {dmax}\n")
                    delete prompts
                }
            }
        }
        metal_prefill_shutdown()
        delete tr
        unsafe {
            delete t
        }
    } else {
        print("seg_window_race: dasMetal not built\n")
    }
    return 0
}
