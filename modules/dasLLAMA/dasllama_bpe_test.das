options gen2

// Detector harness for the Llama-3 BPE tokenizer, run against the in-repo
// ggml-vocab-llama-bpe fixture (the exact tokenizer + .inp/.out round-trip
// corpus llama.cpp uses for test-tokenizer-0). No model download needed.
//
// Step 1: load the BPE vocab/merges, parse the corpus, confirm input<->output
// alignment, and (once bpe_encode lands) gate encode token-for-token.

require dasllama_bpe
require daslib/fio
require strings
require daslib/strings_boost
require math

let MODEL = "/Users/borisbatkin/Work/llama.cpp/models/ggml-vocab-llama-bpe.gguf"
let SEP = "\n__ggml_vocab_test__\n"

// Parse one .out line (" 1142 220 19 ...") into token ids; tolerant of the leading space.
def parse_out_ids(line : string) : array<int64> {
    return <- [for (tok in split(line, " ")); int64(to_int(tok)); where !empty(tok)]
}

// Drop a single trailing empty element (both fixture files end with a trailing
// delimiter / newline, producing one artifact piece after split).
def drop_trailing_empty(var xs : array<string>) {
    if (!empty(xs) && empty(xs[length(xs) - 1])) {
        xs |> erase(length(xs) - 1)
    }
}

[export]
def main() {
    let tk = load_bpe_tokenizer_gguf(MODEL)
    to_log(LOG_INFO, "BPE loaded: vocab={length(tk.vocab)} merges={length(tk.merge_rank)} pre='{tk.pre}' bos={tk.bos_id} eos={tk.eos_id}\n")

    var inputs <- split(fread(MODEL + ".inp"), SEP)
    var outlines <- split(fread(MODEL + ".out"), "\n")
    drop_trailing_empty(inputs)
    drop_trailing_empty(outlines)

    to_log(LOG_INFO, "corpus: {length(inputs)} inputs, {length(outlines)} outputs\n")
    if (length(inputs) != length(outlines)) {
        to_log(LOG_ERROR, "MISALIGNED corpus — input/output counts differ\n")
        return
    }

    // Sample the first few aligned pairs to eyeball the parse before encode exists.
    for (i in range(min(3, length(inputs)))) {
        let want = parse_out_ids(outlines[i])
        to_log(LOG_INFO, "  [{i}] in='{inputs[i]}' -> expect {length(want)} ids: {want}\n")
    }

    to_log(LOG_INFO, "harness ready — {length(inputs)} test cases; bpe_encode gate lands next step\n")
}
