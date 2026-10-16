options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

// Load-serve-delete cycles of one model on Metal, each closed at the drivers' shutdown, the process footprint and the live
// Metal objects printed after the delete and after the shutdown: a device buffer left alive shows as a footprint category
// that climbs a model a cycle.
//   bin/daslang -jit modules/dasLLAMA/harness/metal_release_probe.das -- <model.gguf> <cycles>
require dasllama/dasllama_transformer   // nolint:STYLE029,LINT019 — umbrella fires each arch [init] registration
require dasllama/dasllama   // load_model_, encode, generate, with_dasllama_jobque
require dasllama/dasllama_common
require dasllama/dasllama_math
require dasllama/dasllama_layout
require metal/das_metal_boost
require dasllama/dasllama_metal_common
require dasllama/dasllama_metal_decode
require dasllama/dasllama_metal_prefill
require daslib/fio
require strings

def footprint_line() : string {
    var out = ""
    let args <- ["sh", "-c", "footprint -p $PPID | grep -E 'Footprint:|IOAccelerator|MALLOC_LARGE' | head -4"]
    run_and_capture(args, out)
    return out
}

def report(tag : string) {
    print("=== {tag}\n{footprint_line()}context heap {int64(heap_bytes_allocated()) / 1048576l} MB, live metal objects {metal_live_object_count()}\n{metal_live_object_report()}")
}

[export]
def main {
    let args <- get_command_line_arguments()
    let path = args[length(args) - 2]
    let cycles = to_int(args[length(args) - 1])
    pin_kernel_backend("portable")   // row-major weights: the blob transform's donor contract
    set_wscale_f16(false)
    report("baseline")
    for (c in range(cycles)) {
        with_dasllama_jobque() {
            var trg <- load_model_(path, QuantMode.q8)
            let blob = convert_model_to_metal_blob(trg)
            trg.config.seq_len = min(trg.config.seq_len, 512l)
            var inscope prompt <- encode(trg, "One, two, three, four, five, six,", true, false)
            select_decode_override("metal")
            select_prefill_override("metal")
            set_metal_prefill_min_npos(1l)
            var s = make_run_state(trg.config, KVDtype.f16, KVDtype.f16)
            var inscope ids <- generate(trg, s, prompt, length(prompt) + 8)
            print("cycle {c}: {length(ids) - length(prompt)} steps, blob {blob}\n")
            report("cycle {c} served, before delete")
            delete s
            set_metal_prefill_min_npos(64l)
            select_decode_override("")
            select_prefill_override("")
            delete trg
        }
        report("after cycle {c} delete")
        metal_decode_shutdown()
        metal_prefill_shutdown()
        report("after cycle {c} shutdown")
    }
}
