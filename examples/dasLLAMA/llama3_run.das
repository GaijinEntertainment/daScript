options gen2

require dasllama/dasllama_transformer
require dasllama/dasllama_bpe
require daslib/jobque_boost
require strings

// Step 6: Llama-3 end-to-end — the BPE tokenizer (dasllama_bpe) + the forward pass with
// θ=500000 RoPE + llama3 freq-scaling, token-for-token vs llama.cpp greedy.
//   oracle = instrumented llama-simple (scratchpad/simple_ids.cpp), CPU greedy.
// Run with JIT:  bin/daslang -jit modules/dasLLAMA/dasllama_llama3_run.das -- <model.gguf>

let DEFAULT_MODEL = "/Users/borisbatkin/Work/llama.cpp/models/Llama-3.2-1B-Instruct-Q8_0.gguf"

// Build [BOS] + BPE(text) — the prompt as llama.cpp tokenizes it (add_special=true).
def prompt_ids(tk : BPETokenizer; text : string) : array<int64> {
    var ids : array<int64>
    ids |> push(tk.bos_id)
    ids |> push_from(bpe_encode(tk, text))
    return <- ids
}

[export]
def main {
    var model = DEFAULT_MODEL
    let cmd <- get_command_line_arguments()
    if (!empty(cmd) && ends_with(cmd[length(cmd) - 1], ".gguf")) {
        model = cmd[length(cmd) - 1]
    }

    let prompt_text = "Once upon a time"
    // [BOS] + BPE("Once upon a time"): same prompt ids across the Llama-3 family
    let prompt_oracle = [128000l, 12805l, 5304l, 264l, 892l]
    // simple_ids CPU-greedy continuation (40 tokens), selected per model
    var oracle : array<int64>
    if (find(model, "8B") >= 0 || find(model, "8b") >= 0) {
        oracle <- [11l, 304l, 264l, 2678l, 14458l, 89777l, 304l, 279l, 20700l, 35231l,
            315l, 279l, 47967l, 11l, 1070l, 12439l, 264l, 3995l, 3828l, 7086l,
            71471l, 13l, 71471l, 574l, 264l, 22999l, 323l, 68188l, 1716l, 11l,
            449l, 264l, 4851l, 2539l, 315l, 5895l, 323l, 264l, 4059l, 2539l]
    } else {
        oracle <- [11l, 304l, 264l, 2678l, 14458l, 89777l, 304l, 279l, 20700l, 35231l,
            315l, 350l, 26089l, 3852l, 11l, 1070l, 12439l, 264l, 3995l, 3828l,
            7086l, 71471l, 13l, 71471l, 574l, 264l, 22999l, 323l, 68188l, 13836l,
            11l, 449l, 264l, 4851l, 2539l, 315l, 5895l, 323l, 264l, 4059l]
    }

    print("loading {model}\n")
    let t <- load_gguf(model, QuantMode.q8)
    let tok <- load_bpe_tokenizer_gguf(model)
    let c = t.config
    print("config: dim={c.dim} hidden={c.hidden_dim} layers={c.n_layers} ")
    print("heads={c.n_heads}/{c.n_kv_heads} (kv_mul={c.n_heads / c.n_kv_heads}) ")
    print("vocab={c.vocab_size} θ={c.rope_freq_base} rope_freqs={length(t.rope_freqs)}\n")

    // --- encode check ---
    let prompt <- prompt_ids(tok, prompt_text)
    var enc_ok = length(prompt) == length(prompt_oracle)
    if (enc_ok) {
        for (i in range(length(prompt))) {
            enc_ok = enc_ok && prompt[i] == prompt_oracle[i]
        }
    }
    print("encode('{prompt_text}') = {prompt} -> {enc_ok ? "OK" : "MISMATCH"}\n")

    // --- generate ---
    var s = make_run_state(c)
    let n_gen = length(oracle)
    with_job_que() {
        set_jobque_fork_pool(true, true)
        let ids <- generate(t, s, prompt, length(prompt) + n_gen)
        let text = build_string() $(var w) {
            let echo = length(prompt) - 1
            for (i in range(n_gen)) {
                w |> write(bpe_decode(tok, ids[echo + i]))
            }
        }
        print("OUTPUT: {prompt_text}{text}\n")
        let echo = length(prompt) - 1
        var matched = 0
        for (i in range(n_gen)) {
            if (ids[echo + i] == oracle[i]) {
                matched++
            }
        }
        print("continuation MATCH: {matched}/{n_gen} vs llama.cpp greedy\n")
        if (enc_ok && matched == n_gen) {
            print("PASS: Llama-3 encode + forward + decode all match llama.cpp\n")
        } else {
            print("FAIL\n")
        }
    }
}
