options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

// The parity dump tool: one arm's forced-feed run to a file, or two files compared. A model past the box with both arms up,
// or a CPU reference too slow to run beside every device run, compares as files: the device arm dumps first and records its
// routing; the CPU arm runs once with `--ref` (the same prompt, feed and routing tape, replayed) and dumps; `--compare` reads
// both and applies the row bar with the one-step-off control.
//
//   DASLLAMA_GPU=1 daslang -jit modules/dasLLAMA/harness/parity_dump.das -- --model <gguf> --out dev.pd [--prompt "..."] [--steps N]
//   daslang -jit modules/dasLLAMA/harness/parity_dump.das -- --model <gguf> --ref dev.pd --out cpu.pd
//   daslang -jit modules/dasLLAMA/harness/parity_dump.das -- --compare-a dev.pd --compare-b cpu.pd [--bar 0.2]

require dasllama/dasllama_transformer
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_tokenizer
require dasllama/dasllama_kv_dtype
require _parity_dump
require ../performance/model_specs.das   // counting_prompt: the confident prompt family the parity suites encode
require daslib/clargs
require daslib/fio

let private EXIT_FAILED = 1       //! a compare past the bar, or a run that could not start
let private EXIT_REFUSED = 2      //! a compare of two dumps that do not share a prompt, feed and tape
let private EXIT_TAPE_SHORT = 3   //! a recording run whose tape a replay on another arm would run out of

[CommandLineArgs]
struct Args {
    @clarg_doc = "GGUF path (a dump run)"
    model : string

    @clarg_doc = "The dump to write"
    out : string

    @clarg_doc = "A dump to take the prompt, the fed tokens and the routing tape from (the replaying arm)"
    ref : string

    @clarg_doc = "The prompt text where no --ref gives one (default: a counting prompt)"
    prompt : string

    @clarg_doc = "The fed steps where no --ref gives them: the run's own greedy continuation"
    steps : int = 8

    @clarg_doc = "The KV codec: f16 | f32 | q8_0 | tq4"
    kv : KVDtype = KVDtype.f16

    @clarg_name = "compare-a"
    @clarg_doc = "The dump under test (a compare run)"
    compare_a : string

    @clarg_name = "compare-b"
    @clarg_doc = "The reference dump"
    compare_b : string

    @clarg_doc = "The row bar as a share of the reference row's live max"
    bar : float = 0.2

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

def private compare_files(a_path, b_path : string; rel : float) : int {
    var a <- parity_dump_read(a_path)
    var b <- parity_dump_read(b_path)
    let a_replayed = a.tape_seen > 0l
    let seen = a_replayed ? a.tape_seen : b.tape_seen
    let miss = a_replayed ? a.tape_miss : b.tape_miss
    print("PARITY {a.arm} '{a.model}' against {b.arm} '{b.model}': {length(b.rows)} rows, prompt {length(b.prompt)}, feed {length(b.feed)}, tape {length(b.tape)} picks ({b.picks_per_select} a select); the replaying arm ({a_replayed ? a.arm : b.arm}) saw {seen} selects, {miss} own picks off the tape\n")
    var rc = EXIT_FAILED
    with_dasllama_jobque_() {   // the rows' argmaxes split across the job queue
        var r <- parity_dump_compare(a, b, rel)
        if (!empty(r.why)) {
            print("PARITY REFUSED: {r.why}\n")
            rc = EXIT_REFUSED
        } else {
            for (row in r.rows) {
                let what = row.row == 0l ? "prefill" : "step {row.row - 1l} (fed {b.feed[row.row - 1l]})"
                print("PARITY {what}: maxdiff {row.diff} (bar {row.bar}) one-step-off {row.ctrl}, argmax {row.argmax_a} against {row.argmax_b}{row.ok ? "" : "  <-- MISS"}\n")
            }
            print(r.ok ? "PARITY OK\n" : "PARITY FAILED\n")
            rc = r.ok ? 0 : EXIT_FAILED
        }
        delete r
    }
    return rc
}

//! the recording arm's tape against the model's routed layers: a layer the device serves whole picks in its kernel and lands
//! nothing, so its selects are off the tape and a replay on another arm runs out - a replayable dump pins the routed layers
//! to the host (DASLLAMA_GPU_HOST_LAYERS)
def private tape_coverage(tr : Model; d : ParityDump) : int {
    let c = tr.config
    let routed = c.moe ? c.n_layers - c.n_layer_dense_lead : 0l
    return 0 if (routed <= 0l || d.picks_per_select <= 0l)
    let positions = long_length(d.prompt) + long_length(d.feed)
    let per_pos = long_length(d.tape) / (positions * d.picks_per_select)
    print("PARITY tape: {per_pos} of {routed} routed layers' selects a position\n")
    return 0 if (per_pos >= routed)
    print("PARITY tape SHORT: {routed - per_pos} routed layers picked on the device and landed nothing - a replay on another arm runs out; dump with DASLLAMA_GPU_HOST_LAYERS={routed} for a replayable tape\n")
    return EXIT_TAPE_SHORT
}

[export, cold_path]
def main() : int {
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "parity_dump")
        return EXIT_FAILED
    }
    let cfg <- r |> move_unwrap
    if (cfg.help || (empty(cfg.model) && empty(cfg.compare_a))) {
        print_help(get_command_info(type<Args>), "parity_dump")
        return cfg.help ? 0 : EXIT_FAILED
    }
    if (!empty(cfg.compare_a)) {
        return compare_files(cfg.compare_a, cfg.compare_b, cfg.bar)
    }
    if (empty(cfg.out)) {
        print("error: a dump run needs --out\n")
        return EXIT_FAILED
    }
    var tr <- load_model_(cfg.model, QuantMode.q8)
    var prompt, feed, tape : array<int64>
    var steps = int64(cfg.steps)
    if (!empty(cfg.ref)) {
        var ref <- parity_dump_read(cfg.ref)
        prompt |> push_from(ref.prompt)
        feed |> push_from(ref.feed)
        tape |> push_from(ref.tape)
        steps = long_length(feed)
        print("PARITY replaying '{cfg.ref}' ({ref.arm}): prompt {length(prompt)}, feed {length(feed)}, tape {length(tape)} picks\n")
        delete ref
    } else {
        var enc <- encode(tr.tok, empty(cfg.prompt) ? "Count from one to thirty: {counting_prompt(1, 24)}" : cfg.prompt, true, false)
        prompt |> push_from(enc)
        delete enc
    }
    var rc = 0
    with_dasllama_jobque_() {
        var d <- parity_dump_run(tr, base_name(cfg.model), cfg.kv, prompt, feed, steps, tape)
        parity_dump_write(cfg.out, d)
        let replayed = empty(cfg.ref) ? "" : " ({d.tape_seen} selects replayed, {d.tape_miss} own picks off the tape)"
        print("PARITY wrote '{cfg.out}': {d.arm} arm, {length(d.rows)} rows, prompt {length(d.prompt)}, feed {length(d.feed)}, tape {length(d.tape)} picks{replayed}\n")
        if (empty(cfg.ref)) {
            rc = tape_coverage(tr, d)
        }
        delete d
    }
    delete tr
    return rc
}
