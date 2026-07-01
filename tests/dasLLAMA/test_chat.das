options gen2

require dastest/testing_boost public
require dasllama/dasllama_transformer
require dasllama/dasllama_chat
require dasllama/dasllama_tokenizer
require dasllama/dasllama_bpe
require daslib/jobque_boost
require daslib/fio
require math

// The unified chat engine must render each arch's turn EXACTLY as the old per-model REPLs hand-rolled
// it — same special-token ids, same text segmentation — so collapsing the 5 REPLs into one changes no
// tokens. Each case rebuilds the old demo's first-turn prefill against the RAW tokenizer backend and
// compares it to render_turn(). GGUF models are gitignored, so each case skips cleanly when absent.

let MODELS_DIR = "/Users/borisbatkin/Work/llama.cpp/models/"
let SYS = "You are a helpful, friendly assistant. Keep answers concise."
let USR = "Once upon a time"

def private same(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (i in range(length(a))) {
        if (a[i] != b[i]) {
            return false
        }
    }
    return true
}

// Load capped to a tiny context: these cases only render prompts, and the session's KV cache is sized
// to config.seq_len — Phi/Llama-3's native 131072 would allocate ~64GB.
def private load_capped(path : string) : Model {
    var m <- load_model(path, QuantMode.q8)
    m.config.seq_len = min(m.config.seq_len, 512l)
    return <- m
}

// Build the chat engine's rendered first-turn prefill for (system, user).
def private engine_prefill(m : Model; system, user : string) : array<int64> {
    var c = create_chat(m, system)
    add_user(c, user)
    return <- render_turn(m, c)
}

[test]
def test_chat_tinyllama_zephyr(t : T?) {
    t |> run("TinyLlama (arch=llama + SPM) renders the Zephyr prefill") @(t : T?) {
        let path = "{MODELS_DIR}tinyllama-1.1b-chat-v1.0.Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        let m <- load_capped(path)
        let raw <- load_tokenizer_gguf(path)
        // old chat.das: BOS, "<|system|>\n{sys}", </s>, "\n<|user|>\n{usr}", </s>, "\n<|assistant|>\n"
        var want : array<int64>
        want |> push(1l)
        want |> push_from(encode(raw, "<|system|>\n{SYS}", false, false))
        want |> push(2l)
        want |> push_from(encode(raw, "\n<|user|>\n{USR}", false, false))
        want |> push(2l)
        want |> push_from(encode(raw, "\n<|assistant|>\n", false, false))
        t |> success(same(engine_prefill(m, SYS, USR), want), "zephyr prefill token-for-token")
    }
}

[test]
def test_chat_gemma(t : T?) {
    t |> run("Gemma-2 (SPM, no system role) folds system into the first user turn") @(t : T?) {
        let path = "{MODELS_DIR}gemma-2-2b-it-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        let m <- load_capped(path)
        let raw <- load_tokenizer_gguf(path)
        // old gemma_chat.das: BOS=2, SOT=106, "user\n{sys}\n\n{usr}", EOT=107, "\n", SOT, "model\n"
        var want : array<int64>
        want |> push(2l)
        want |> push(106l)
        want |> push_from(encode(raw, "user\n{SYS}\n\n{USR}", false, false))
        want |> push(107l)
        want |> push_from(encode(raw, "\n", false, false))
        want |> push(106l)
        want |> push_from(encode(raw, "model\n", false, false))
        t |> success(same(engine_prefill(m, SYS, USR), want), "gemma folded prefill token-for-token")
    }
}

[test]
def test_chat_phi(t : T?) {
    t |> run("Phi-3.5 (SPM) renders the Phi-3 prefill") @(t : T?) {
        let path = "{MODELS_DIR}Phi-3.5-mini-instruct-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        let m <- load_capped(path)
        let raw <- load_tokenizer_gguf(path)
        // old phi_chat.das: SYS=32006, "\n{sys}", END=32007, "\n", USR=32010, "\n{usr}", END, "\n", ASST=32001, "\n"
        var want : array<int64>
        want |> push(32006l)
        want |> push_from(encode(raw, "\n{SYS}", false, false))
        want |> push(32007l)
        want |> push_from(encode(raw, "\n", false, false))
        want |> push(32010l)
        want |> push_from(encode(raw, "\n{USR}", false, false))
        want |> push(32007l)
        want |> push_from(encode(raw, "\n", false, false))
        want |> push(32001l)
        want |> push_from(encode(raw, "\n", false, false))
        t |> success(same(engine_prefill(m, SYS, USR), want), "phi prefill token-for-token")
    }
}

[test]
def test_chat_qwen(t : T?) {
    t |> run("Qwen2.5 (BPE) renders the ChatML prefill") @(t : T?) {
        let path = "{MODELS_DIR}Qwen2.5-1.5B-Instruct-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        let m <- load_capped(path)
        let raw <- load_bpe_tokenizer_gguf(path)
        let ims = special_id(raw, "<|im_start|>")
        let ime = special_id(raw, "<|im_end|>")
        // old qwen_chat.das: ims, "system\n{sys}", ime, "\n", ims, "user\n{usr}", ime, "\n", ims, "assistant\n"
        var want : array<int64>
        want |> push(ims)
        want |> push_from(bpe_encode(raw, "system\n{SYS}"))
        want |> push(ime)
        want |> push_from(bpe_encode(raw, "\n"))
        want |> push(ims)
        want |> push_from(bpe_encode(raw, "user\n{USR}"))
        want |> push(ime)
        want |> push_from(bpe_encode(raw, "\n"))
        want |> push(ims)
        want |> push_from(bpe_encode(raw, "assistant\n"))
        t |> success(same(engine_prefill(m, SYS, USR), want), "qwen chatml prefill token-for-token")
    }
}

[test]
def test_chat_llama3(t : T?) {
    t |> run("Llama-3 (arch=llama + BPE) renders the Llama-3 header prefill") @(t : T?) {
        let path = "{MODELS_DIR}Llama-3.2-1B-Instruct-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        let m <- load_capped(path)
        let raw <- load_bpe_tokenizer_gguf(path)
        let sh = special_id(raw, "<|start_header_id|>")
        let eh = special_id(raw, "<|end_header_id|>")
        let eot = special_id(raw, "<|eot_id|>")
        // old llama3_chat.das: BOS, header(system), "\n\n{sys}", eot, header(user), "\n\n{usr}", eot, header(assistant), "\n\n"
        var want : array<int64>
        want |> push(raw.bos_id)
        want |> push(sh); want |> push_from(bpe_encode(raw, "system")); want |> push(eh)
        want |> push_from(bpe_encode(raw, "\n\n{SYS}"))
        want |> push(eot)
        want |> push(sh); want |> push_from(bpe_encode(raw, "user")); want |> push(eh)
        want |> push_from(bpe_encode(raw, "\n\n{USR}"))
        want |> push(eot)
        want |> push(sh); want |> push_from(bpe_encode(raw, "assistant")); want |> push(eh)
        want |> push_from(bpe_encode(raw, "\n\n"))
        t |> success(same(engine_prefill(m, SYS, USR), want), "llama3 header prefill token-for-token")
    }
}

[test]
def test_chat_end_to_end(t : T?) {
    t |> run("respond() streams a reply, stops on a stop token, records stats + transcript") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = "{MODELS_DIR}gemma-2-2b-it-Q8_0.gguf"
        if (!stat(path).is_valid) {
            t |> success(true, "skipped: {path} not present")
            return
        }
        var m <- load_model(path, QuantMode.q8)
        m.config.seq_len = min(m.config.seq_len, 512l)
        var reply = ""
        var ntok = 0l
        with_job_que() {
            set_jobque_fork_pool(true, true)
            var c = create_chat(m, "", 24l)   // no system prompt, short budget
            add_user(c, "Say hello in one short sentence.")
            reply = respond(m, c, SamplingParams()) $(piece) {   // greedy
                ntok++
                return true
            }
            t |> success(!empty(reply), "reply non-empty")
            t |> success(length(c.history) == 2, "transcript has the user + assistant turns")
            t |> success(c.history[1].role == Role.assistant, "second turn is the assistant reply")
            let st = stats(c.session)
            t |> success(st.n_gen == ntok && st.n_gen > 0l, "stats.n_gen matches streamed tokens")
            t |> success(st.n_prompt > 0l, "stats recorded the prefill")
        }
    }
}
