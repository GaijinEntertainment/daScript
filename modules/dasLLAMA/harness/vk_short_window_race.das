// The Vulkan short window's race: a continuation of N rows after a prompt through the token command's peel against the
// same rows through the window chain (the lever `set_vulkan_prefill_peel` flipped between the forms), in one process,
// interleaved, the fastest of the reps a form. Prints a row a row count with both times and the forms' last-row logits'
// widest difference.
//
//   DASLLAMA_GPU=1 bin/daslang -jit modules/dasLLAMA/harness/vk_short_window_race.das -- --model <gguf> [--rows 1,2,4,8] [--reps 5] [--prompt 40]

options gen2
options stack = 524288
options _dasllama_internal = true

require daslib/clargs
require dasllama/dasllama
require dasllama/dasllama_transformer   // the umbrella fires each arch [init] registration
require dasllama/dasllama_common        // nolint:STYLE030,LINT019 - forward_prefill, allow_cpu_prefill
require dasllama/dasllama_math          // with_dasllama_jobque_
require dasllama/dasllama_gpu_resident  // moe_gpu_resident_active, moe_gpu_resident_peels, the peel's lever
require dasllama/dasllama_gpu_tier      // gpu_device_session_dtype_, rdec_token_n_rows
require ../tests/_compares.das          // nolint:STYLE030,LINT019 - logits_maxdiff
require _window_race_fixture   // int_list, counting_tokens

[CommandLineArgs]
struct RaceArgs {
    @clarg_doc = "The GGUF to race on the resident driver"
    model : string
    @clarg_doc = "Comma list of continuation row counts"
    rows : string = "1,2,4,8"
    @clarg_doc = "Timed repetitions a form (the fastest counts)"
    reps : int = 5
    @clarg_doc = "The prompt's rows ahead of the continuation"
    prompt : int = 40
}

//! the continuation `tail` after `prompt` on a fresh session with the peel's lever at `peel`: the wall of the continuation's
//! call and its last-row logits
def private continuation(tr : Model; prompt, tail : array<int64>; peel : bool) : tuple<ms : float; logits : array<float>> {
    set_vulkan_prefill_peel(peel)
    var s <- create_session(tr, gpu_device_session_dtype_())
    let np = long_length(prompt)
    forward_prefill(tr, s, prompt, np, 0l)
    let t0 = ref_time_ticks()
    forward_prefill(tr, s, tail, long_length(tail), np)
    let ms = float(get_time_usec(t0)) / 1000.0
    var logits <- [for (i in range64(tr.config.vocab_size)); s.logits[i]]
    delete s
    return <- (ms = ms, logits <- logits)
}

[export, cold_path]
def main : int {
    var args = RaceArgs()
    let rc = parse_args_with_help(args, "vk_short_window_race")
    return rc if (rc >= 0)
    if (empty(args.model)) {
        to_log(LOG_ERROR, "vk_short_window_race: --model <gguf> is required\n")
        return 2
    }
    allow_cpu_prefill()
    var rows <- int_list(args.rows)
    var tr <- load_model_(args.model, QuantMode.q8)
    if (!moe_gpu_resident_active()) {
        to_log(LOG_ERROR, "vk_short_window_race: the resident driver did not arm on {args.model} (the load log names why)\n")
        delete tr
        return 3
    }
    let lever = vulkan_prefill_peel()
    with_dasllama_jobque_() {
        let np = int64(args.prompt)
        var all <- counting_tokens(tr, np + 64l, 1l)
        var prompt <- [for (k in range64(np)); all[k]]
        print("rows the token command steps at once: {rdec_token_n_rows()}\n")
        for (k in rows) {
            var tail <- [for (i in range64(k)); all[np + i]]
            var best_peel = 1.0e30
            var best_chain = 1.0e30
            var worst_peel = 0.0
            var worst_chain = 0.0
            var dmax = 0.0
            let peels0 = moe_gpu_resident_peels()
            for (_rep in range(args.reps)) {
                var a <- continuation(tr, prompt, tail, true)
                var b <- continuation(tr, prompt, tail, false)
                best_peel = min(best_peel, a.ms)
                best_chain = min(best_chain, b.ms)
                worst_peel = max(worst_peel, a.ms)
                worst_chain = max(worst_chain, b.ms)
                dmax = max(dmax, logits_maxdiff(a.logits, b.logits))
                delete a
                delete b
            }
            print("rows={k}: peel {best_peel} ms (slowest {worst_peel}), chain {best_chain} ms (slowest {worst_chain}) ({best_peel / best_chain * 100.0}%), {args.reps} reps a form, peels counted {moe_gpu_resident_peels() - peels0} of {args.reps}, last-row logits max difference {dmax}\n")
            delete tail
        }
        var passed = ""
        for (p in gpu_cpu_passes_()) {   //! a pass would make a row device-against-CPU: the forms are the driver's own only while this reads none
            passed += " {p.reason}:{p.count}"
        }
        print("calls the driver passed to the CPU rails:{empty(passed) ? " none" : passed}\n")
        delete prompt
        delete all
    }
    set_vulkan_prefill_peel(lever)
    delete tr
    return 0
}
