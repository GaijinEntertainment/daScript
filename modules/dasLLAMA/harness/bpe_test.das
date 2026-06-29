options gen2

// Detector harness for the Llama-3 BPE tokenizer, run against the in-repo
// ggml-vocab-llama-bpe fixture (the exact tokenizer + .inp/.out round-trip
// corpus llama.cpp uses for test-tokenizer-0). No model download needed.
//
// Step 1: load the BPE vocab/merges, parse the corpus, confirm input<->output
// alignment, and (once bpe_encode lands) gate encode token-for-token.

require dasllama/dasllama_bpe
require daslib/fio
require strings
require daslib/strings_boost

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

def ids_equal(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        return false if (a[i] != b[i])
    }
    return true
}

// Independent cross-check: ids captured from `llama-tokenize` (BOS stripped, since
// bpe_encode is BOS-free) on sentences NOT in the fixture corpus.
def check_fresh(tk : BPETokenizer; text : string; want : array<int64>) : bool {
    let got <- bpe_encode(tk, text)
    let ok = ids_equal(got, want)
    if (!ok) {
        to_log(LOG_ERROR, "FRESH FAIL '{text}'\n    want {want}\n    got  {got}\n")
    }
    return ok
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

    var npass = 0
    var nfail = 0
    for (i in range(length(inputs))) {
        let want = parse_out_ids(outlines[i])
        let got <- bpe_encode(tk, inputs[i])
        if (ids_equal(got, want)) {
            npass++
        } else {
            nfail++
            to_log(LOG_ERROR, "FAIL [{i}] in='{inputs[i]}'\n    want {want}\n    got  {got}\n")
        }
    }
    to_log(LOG_INFO, "BPE corpus: {npass}/{length(inputs)} passed, {nfail} failed\n")

    var fresh_ok = true
    fresh_ok = fresh_ok && check_fresh(tk, "Hello, world! It's 2026.", [9906l, 11l, 1917l, 0l, 1102l, 596l, 220l, 2366l, 21l, 13l])
    fresh_ok = fresh_ok && check_fresh(tk, "dasLLAMA runs on daslang 🦙", [35989l, 4178l, 39200l, 8640l, 389l, 6754l, 5317l, 11410l, 99l, 247l])
    fresh_ok = fresh_ok && check_fresh(tk, "  spaces   and\ttabs", [220l, 12908l, 256l, 323l, 3324l, 3518l])
    to_log(LOG_INFO, "fresh cross-check: {fresh_ok ? "PASS" : "FAIL"}\n")

    // decode round-trip: byte-level BPE is lossless, so decode(encode(x)) == x
    var rt_ok = 0
    for (i in range(length(inputs))) {
        let rt = bpe_decode_ids(tk, bpe_encode(tk, inputs[i]))
        if (rt == inputs[i]) {
            rt_ok++
        } else {
            to_log(LOG_ERROR, "ROUNDTRIP FAIL [{i}] in='{inputs[i]}' got='{rt}'\n")
        }
    }
    to_log(LOG_INFO, "decode round-trip: {rt_ok}/{length(inputs)}\n")

    // special tokens resolve by surface form; control tokens decode to nothing
    let eot = special_id(tk, "<|eot_id|>")
    let bos = special_id(tk, "<|begin_of_text|>")
    to_log(LOG_INFO, "special: eot_id={eot} bos={bos} decode(9906)='{bpe_decode(tk, 9906l)}' decode(eot)='{bpe_decode(tk, eot)}'\n")
}
