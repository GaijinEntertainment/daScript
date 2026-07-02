options gen2
options persistent_heap   // so Section 4's deletes really free (the default linear heap frees only at context teardown)

require dasllama/dasllama
require daslib/jobque_boost
require daslib/fio
require strings
require math

// Tutorial dasLLAMA-04: Sessions, the KV Cache, and Memory
//
// This tutorial covers:
//   - What a Session is, and sizing its KV cache by capping config.seq_len
//   - One model, many independent sessions
//   - eval / sample by hand — the loop inside generate()
//   - Freeing models and sessions: options persistent_heap + delete
//
// Prerequisites: tutorial 01.
//
// Run: daslang.exe -jit tutorials/dasLLAMA/04_sessions_and_memory.das -- <path/to/model.gguf>
//
// Each section is its own function, and that's load-bearing, not just style: a daslang
// function's stack frame is statically sized for ALL its locals, and the default 16KB
// context stack must also fit the model's whole forward-pass call chain. Piling every
// section's locals into one fat main() overflows it; per-section functions pop their
// frames between sections. (The alternative is raising `options stack`.)

let PROMPT = "Once upon a time"
let CTX = 1024l

// Resolve the model path: last command-line argument ending in .gguf, else $DASLLAMA_MODEL.
def model_path() : string {
    let cmd <- get_command_line_arguments()
    if (!empty(cmd) && ends_with(cmd[length(cmd) - 1], ".gguf")) {
        return cmd[length(cmd) - 1]
    }
    return get_env_variable("DASLLAMA_MODEL")
}

def mb(bytes : int64) : int {
    return int(bytes / 1000000l)
}

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — The KV cache, and why you cap seq_len
// ──────────────────────────────────────────────────────────────────────────
//
// A Session is one generation stream: the attention KV cache plus scratch buffers.
// The KV cache is sized to config.seq_len AT create_session TIME — roughly
// 2 * n_layers * seq_len * kv_dim floats. Models ship with big native contexts
// (Llama-3's native seq_len is 131072 => tens of GB of fp32 KV), so cap seq_len
// to the context you actually need BEFORE creating sessions.

def section_kv_cache(var m : Model) {
    let c = m.config
    let native_kv = 2l * c.n_layers * c.seq_len * c.kv_dim * 4l
    print("native ctx {c.seq_len} -> KV cache would be ~{mb(native_kv)} MB per session\n")
    m.config.seq_len = min(m.config.seq_len, CTX)
    var s = create_session(m)
    let kv_bytes = int64(length(s.key_cache) + length(s.value_cache)) * 4l
    print("capped ctx {m.config.seq_len} -> this session's KV cache is {mb(kv_bytes)} MB\n\n")
    delete s   // sessions are freed as soon as a section is done with them — Section 4 explains
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — One model, many sessions
// ──────────────────────────────────────────────────────────────────────────
//
// The model is the expensive, read-only part (the weights). Sessions are
// per-conversation state: each has its own position (n_past), KV cache, and
// sampling RNG. Two sessions over one model don't see each other.

def continue_text(m : Model; var s : Session; text : string) : string {
    var out = ""
    let prompt <- encode(m, text)
    generate(m, s, prompt, SamplingParams(temp = 0.8, top_k = 40l), 12l) $(_id, piece) {
        out += piece
        return true
    }
    return out
}

def section_many_sessions(m : Model) {
    var s1 = create_session(m)
    var s2 = create_session(m)
    set_seed(s1, 1)
    set_seed(s2, 2)
    let t1 = continue_text(m, s1, "The dragon")
    let t2 = continue_text(m, s2, "The library")
    print("s1: The dragon{t1}\n")
    print("s2: The library{t2}\n")
    print("independent positions: s1.n_past={s1.n_past} s2.n_past={s2.n_past}\n\n")
    delete s1
    delete s2
}

// ──────────────────────────────────────────────────────────────────────────
// Section 3 — eval and sample by hand
// ──────────────────────────────────────────────────────────────────────────
//
// This is the loop inside generate(): eval() runs tokens at the session's
// current position and advances it (prefill = the whole prompt in one call);
// sample() picks the next token from session.logits. generate() adds the
// timing stats and the repetition-penalty window — this skeleton is all the
// model ever does.

def section_eval_by_hand(m : Model) {
    var s = create_session(m)
    let prompt <- encode(m, PROMPT)
    print("n_past before prefill: {s.n_past}\n")
    eval(m, s, prompt)
    print("n_past after prefill:  {s.n_past}\n")
    print("{PROMPT}")
    var one : array<int64>
    one |> resize(1)
    for (_step in range64(16l)) {
        let tok = sample(s, SamplingParams())
        print("{piece(m, tok)}")
        one[0] = tok
        eval(m, s, one)   // feed the sampled token back in
    }
    print("\nn_past after 16 steps: {s.n_past}\n\n")
    delete s
}

[export]
def main() {
    let path = model_path()
    if (empty(path)) {
        print("usage: daslang -jit tutorials/dasLLAMA/04_sessions_and_memory.das -- <model.gguf>\n")
        return
    }
    var m <- load_model(path, QuantMode.q8)
    with_job_que() {
        set_jobque_fork_pool(true, true)
        section_kv_cache(m)
        section_many_sessions(m)
        section_eval_by_hand(m)
    }

    // ──────────────────────────────────────────────────────────────────────
    // Section 4 — Freeing: persistent_heap + delete
    // ──────────────────────────────────────────────────────────────────────
    //
    // A one-shot script can just exit — the context frees everything. A long-lived
    // process that loads model after model must free explicitly, and that takes both
    // ingredients: `options persistent_heap` at the top of the file (a malloc-backed
    // heap where delete returns memory immediately — on the default linear heap a
    // mid-context delete is a no-op), and an explicit `delete` of every Session and
    // Model you created, like the sections above did.

    print("heap before deleting the model: {mb(int64(heap_bytes_allocated()))} MB\n")
    delete m
    print("heap after deleting the model:  {mb(int64(heap_bytes_allocated()))} MB\n")
}
