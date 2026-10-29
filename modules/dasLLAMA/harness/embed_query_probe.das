options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap
options _dasllama_internal = true

require dasllama/dasllama   // nolint:STYLE029,STYLE030,LINT019 - used only inside the das_metal static_if half
require dasllama/dasllama_common   // set_metal_mode, apply_box_profile_runtime
require ?das_metal dasllama/dasllama_metal_prefill   // the split-K seat and the window's stage report
require ../performance/profile_common.das   // nolint:STYLE029,LINT019 - tune_gate, used only inside the das_metal static_if half
require llvm/daslib/llvm_tune   // nolint:STYLE030,LINT019 - tune_median, tune_cv_pct, used only inside the das_metal static_if half
require daslib/clargs
require daslib/json_boost
require daslib/fio

// The embedding query probe: one process embeds the same `-n` distinct instruction-prefixed queries a round, the
// in-process wall a query (the facade's `embed`, tokenize to the pooled vector) and the Metal window's GPU time, over
// `-r` rounds after one warmup round. With `--ab split` the rounds alternate the short window's split-K tiles on
// (arm A) and off (arm B), with `--ab tail` the GEMV-tail peel. Each row is the median over the rounds of the round's mean per query, and the rounds' cv;
// a row past 3% is noise. The corpus is a JSON file holding `query_instruction` and a `phrases` string array, read
// as probe input and printed nowhere.
//
//   bin/daslang -jit modules/dasLLAMA/harness/embed_query_probe.das -- -m <embed.gguf> --corpus <corpus.json> [-n 60] [-r 9] [--ab split]

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "Embedding model GGUF path"
    model : string
    @clarg_doc = "corpus JSON: `query_instruction` and a `phrases` string array"
    corpus : string
    @clarg_short = "n"
    @clarg_doc = "distinct queries a round (default 60)"
    queries : int = 60
    @clarg_short = "r"
    @clarg_doc = "timed rounds after the warmup round (default 9)"
    reps : int = 9
    @clarg_doc = "`split`: alternate the split-K tiles on (A) and off (B) by round; `tail`: the GEMV-tail peel on (A) and off (B)"
    ab : string = ""
    @clarg_short = "?"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private load_queries(path : string; n : int) : array<string> {
    var out : array<string>
    var err = ""
    let js = read_json(fread(path), err)
    if (js == null) {
        print("embed_query_probe: corpus unreadable ({err})\n")
        return <- out
    }
    let instr = js?.query_instruction ?? ""
    let phrases = js?.phrases
    if (phrases == null || !(phrases.value is _array)) {
        return <- out
    }
    for (p in phrases.value as _array) {
        break if (length(out) >= n)
        out |> push("Instruct: {instr}\nQuery: {p ?? ""}")
    }
    return <- out
}

def private probe(a : Args) : int {
    static_if (typeinfo builtin_module_exists(das_metal)) {
        var inscope queries <- load_queries(a.corpus, a.queries)
        if (length(queries) < a.queries) {
            print("embed_query_probe: the corpus holds {length(queries)} of the {a.queries} queries asked for\n")
            return 1
        }
        if (!empty(a.ab) && a.ab != "split" && a.ab != "tail") {
            print("embed_query_probe: --ab takes `split` or `tail`, not `{a.ab}`\n")
            return 2
        }
        set_metal_mode(MetalMode.when_available)
        apply_box_profile_runtime()
        return 1 if (!tune_gate())
        var code = 0
        with_dasllama_jobque() {
            var m <- load_model(a.model, QuantMode.q8)
            var inscope wall_a, wall_b, gpu_a, gpu_b : array<double>
            var inscope first_a, first_b : array<float>
            for (r in range(1 + (empty(a.ab) ? a.reps : 2 * a.reps))) {
                let arm_b = !empty(a.ab) && r > 0 && (r - 1) % 2 == 1
                if (a.ab == "split") {
                    set_metal_prefill_split_k(!arm_b)
                } elif (a.ab == "tail") {
                    set_metal_prefill_mm_tail(!arm_b)
                }
                var gpu_us = 0.0lf
                let t0 = ref_time_ticks()
                for (qi, q in count(), queries) {
                    metal_prefill_stage_reset()
                    var inscope v <- embed(m, q)
                    gpu_us += metal_prefill_stage_stats().gpu_us
                    if (qi == 0 && r > 0) {
                        if (arm_b) {
                            first_b := v
                        } else {
                            first_a := v
                        }
                    }
                }
                let wall = double(get_time_nsec(t0)) / 1.0e6lf / double(length(queries))
                let gpu = gpu_us / 1000.0lf / double(length(queries))
                if (r == 0) {
                    continue
                }
                if (arm_b) {
                    wall_b |> push(wall)
                    gpu_b |> push(gpu)
                } else {
                    wall_a |> push(wall)
                    gpu_a |> push(gpu)
                }
            }
            set_metal_prefill_split_k(true)
            set_metal_prefill_mm_tail(true)
            print("embed query x{length(queries)} ({m.config.dim}d x {m.config.n_layers}L), medians of {a.reps} rounds a arm\n")
            print("A: wall {tune_median(wall_a)} ms/query (cv {tune_cv_pct(wall_a)}%)  window gpu {tune_median(gpu_a)} ms (cv {tune_cv_pct(gpu_a)}%)\n")
            if (!empty(wall_b)) {
                print("B: wall {tune_median(wall_b)} ms/query (cv {tune_cv_pct(wall_b)}%)  window gpu {tune_median(gpu_b)} ms (cv {tune_cv_pct(gpu_b)}%)\n")
            }
            print("split-K sites served since load: {metal_prefill_split_k_sites()}\n")
            if (!empty(first_b)) {
                var dot = 0.0lf
                for (x, y in first_a, first_b) {
                    dot += double(x) * double(y)
                }
                print("query 0's vector, arm A against arm B: cosine {dot}\n")
            }
            if (tune_median(gpu_a) <= 0.0lf) {
                print("embed_query_probe: no Metal window served - no number\n")
                code = 1
            }
            delete m
        }
        return code
    } else {
        print("embed_query_probe: no das_metal in this build\n")
        return 1
    }
}

[export]
def main : int {
    var parsed <- parse_args(type<Args>)
    if (parsed |> is_err) {
        print("error: {parsed |> unwrap_err}\n")
        print_help(get_command_info(type<Args>), "embed_query_probe")
        return 2
    }
    let a <- parsed |> move_unwrap
    if (a.help || empty(a.model) || empty(a.corpus)) {
        print_help(get_command_info(type<Args>), "embed_query_probe")
        return a.help ? 0 : 2
    }
    return probe(a)
}
