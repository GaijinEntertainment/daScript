options gen2
options stack = 524288
options persistent_heap
options _dasllama_internal = true

// The W4A4 instrument: the same windows of a text through the CUDA home's FP4 tensor leaf (the window's activations
// quantized to e2m1 per token) and through the f16 tiles over the same nv4 weights, each window's next token scored off
// its last row - the perplexity of each arm, and the paired difference with its standard error: the activation
// quantization's own cost, the weights held equal. Needs DASLLAMA_GPU=1 DASLLAMA_CUDA=1 and an nv4 carrier the
// whole-model driver serves.
//   DASLLAMA_GPU=1 DASLLAMA_CUDA=1 daslang -jit modules/dasLLAMA/harness/w4a4_ppl.das -- -m <nv4.gguf> --text wiki.test.raw

require dasllama/dasllama_transformer   // the umbrella's [init] registrations arm the llama arch and the CUDA home
require dasllama/dasllama_common
require dasllama/dasllama_core   // create_session, forward_prefill
require dasllama/dasllama_math
require dasllama/dasllama_gpu_resident
require dasllama/dasllama_cuda_decode   // set_cuda_mmq_pin - the two arms' knob
require ../tests/_resident_feed.das   // load_resident
require ../tests/_model_tier.das      // mirror_kv_dtype, target_logp
require ../tests/_compares.das        // argmax_of
require daslib/fio
require daslib/clargs
require daslib/strings_boost   // split, join: the text tokenized a run of lines at a time
require math

[CommandLineArgs]
struct Args {
    @clarg_short = "m"
    @clarg_doc = "GGUF model path: an nv4 carrier the CUDA whole-model driver serves"
    model : string

    @clarg_doc = "The text scored (wikitext-2's wiki.test.raw)"
    text : string

    @clarg_doc = "Windows scored, window w's prompt starting at token w x stride"
    windows : int = 400

    @clarg_doc = "A window's prompt length in tokens"
    ctx : int = 512

    @clarg_doc = "The token stride between windows"
    stride : int = 256

    @clarg_short = "?"
    @clarg_name = "show-help"
    @clarg_doc = "Show this help and exit"
    help : bool
}

struct ArmScore {
    nll : array<float>   //! a window's negative log-likelihood of the token after its prompt
    hits : int64         //! the windows whose argmax was that token
}

//! every window's score under the MMQ knob at `pin`: 1 the FP4 leaf (W4A4), 0 the f16 tiles (weight-only)
def arm_score(tr : Model; ids : array<int64>; windows, ctx, stride, pin : int64) : ArmScore {
    set_cuda_mmq_pin(pin)
    var sc : ArmScore
    sc.nll |> reserve(windows)
    with_dasllama_jobque_() {
        for (w in range64(windows)) {
            let start = w * stride
            var prompt <- [for (i in range64(start, start + ctx)); ids[i]]
            var s <- create_session(tr, mirror_kv_dtype())
            forward_prefill(tr, s, prompt, ctx, 0l)
            let target = ids[start + ctx]
            sc.nll |> push(-target_logp(s.logits, target))
            if (argmax_of(s.logits) == target) {
                sc.hits++
            }
            delete s
            delete prompt
        }
    }
    set_cuda_mmq_pin(-1l)
    return <- sc
}

//! the text's ids, tokenized a run of lines at a time (the whole file as one string takes the tokenizer past the box's memory); the first run alone carries the BOS
def text_ids(tr : Model; text : string) : array<int64> {
    var ids : array<int64>
    var run : array<string>
    var run_bytes = 0
    var first = true
    let lines <- split(text, "\n")
    let last = length(lines) - 1
    for (line, i in lines, count()) {
        run |> push(line)
        run_bytes += length(line) + 1
        if (run_bytes >= 8192 || i == last) {
            var part <- encode_(tr, join(run, "\n"), first, false)
            ids |> push_from(part)
            delete part
            first = false
            run |> clear()
            run_bytes = 0
        }
    }
    return <- ids
}

def mean_of(v : array<float>) : double {
    var acc = 0.0lf
    for (x in v) {
        acc += double(x)
    }
    return acc / double(length(v))
}

[export]
def main() : int {
    if (!jit_enabled()) {
        print("This instrument is JIT-only - run with: daslang -jit <file>\n")
        return 1
    }
    var r <- parse_args(type<Args>)
    if (r |> is_err) {
        print("error: {r |> unwrap_err}\n\n")
        print_help(get_command_info(type<Args>), "w4a4_ppl")
        return 1
    }
    let cfg <- r |> move_unwrap
    if (cfg.help || empty(cfg.model) || empty(cfg.text)) {
        print_help(get_command_info(type<Args>), "w4a4_ppl")
        return cfg.help ? 0 : 1
    }
    var text : string
    fopen(cfg.text, "rb") $(f) {
        if (f == null) {
            panic("can't open {cfg.text}")
        }
        text = clone_string(fread(f))
    }
    var tr <- load_resident(cfg.model, 1l, int64(cfg.ctx) + 8l)   // the context capped at a window: a session's K/V at the file's full context is gigabytes a window
    if (!moe_gpu_resident_active()) {
        print("the whole-model driver declined {cfg.model} (the load log names why)\n")
        return 1
    }
    var ids <- text_ids(tr, text)
    let ctx = int64(cfg.ctx)
    let stride = int64(cfg.stride)
    let windows = int64(cfg.windows)
    let need = (windows - 1l) * stride + ctx + 1l
    if (long_length(ids) < need) {
        panic("{cfg.text} tokenizes to {long_length(ids)} ids, under the {need} the windows need")
    }
    var on <- arm_score(tr, ids, windows, ctx, stride, 1l)
    var off <- arm_score(tr, ids, windows, ctx, stride, 0l)
    let m_on = mean_of(on.nll)
    let m_off = mean_of(off.nll)
    var dsum = 0.0lf
    var dsq = 0.0lf
    for (a, b in on.nll, off.nll) {
        let d = double(a) - double(b)
        dsum += d
        dsq += d * d
    }
    let n = double(windows)
    let dmean = dsum / n
    let se = sqrt(max(dsq / n - dmean * dmean, 0.0lf) / n)
    print("W4A4 against weight-only on {cfg.model}: {cfg.windows} windows of {cfg.ctx} tokens, stride {cfg.stride}, the window's next token scored\n")
    print("  ppl  W4A4 (FP4 leaf) {exp(m_on)}  weight-only (f16 tiles) {exp(m_off)}  argmax hits {on.hits} / {off.hits}\n")
    print("  paired nll delta (W4A4 - weight-only) {dmean} +- {se} = a ppl ratio of {exp(dmean)}\n")
    delete on
    delete off
    delete ids
    delete tr
    return 0
}
