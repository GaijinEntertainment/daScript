options gen2
options stack = 65536   // the respond()/prefill chain + generated NEON kernels overflow the 16K default on arm64 (M1)
options persistent_heap   // + explicit deletes below: each test frees its model/session before the next loads, so multi-GB weights never accumulate in the process

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — public-path tests exercise the API through it (engine + arch registrations + chat)
require dasllama/dasllama_tokenizer
require dasllama/dasllama_bpe
require daslib/jobque_boost
require daslib/fio
require math

// The unified chat engine must render each arch's turn EXACTLY as the old per-model REPLs hand-rolled
// it — same special-token ids, same text segmentation — so collapsing the 5 REPLs into one changes no
// tokens. Each case rebuilds the old demo's first-turn prefill against the RAW tokenizer backend and
// compares it to render_turn(). GGUF models are gitignored, so each case skips cleanly when absent.

// override the machine-local model root with DASLLAMA_MODELS_DIR (e.g. on the x64 handoff box)
def private models_dir() : string {
    let env = get_env_variable("DASLLAMA_MODELS_DIR")
    return empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
}
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

// KV caches are codec-encoded byte blobs — byte equality IS bit-exactness for a same-dtype pair
def private same(a, b : array<uint8>) : bool {
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
    var out <- render_turn(m, c)
    delete c
    return <- out
}

[test]
def test_chat_tinyllama_zephyr(t : T?) {
    t |> run("TinyLlama (arch=llama + SPM) renders the Zephyr prefill") @(t : T?) {
        let path = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_capped(path)
        var raw <- load_tokenizer_gguf(path)
        // old chat.das: BOS, "<|system|>\n{sys}", </s>, "\n<|user|>\n{usr}", </s>, "\n<|assistant|>\n"
        var want : array<int64>
        want |> push(1l)
        want |> push_from(encode(raw, "<|system|>\n{SYS}", false, false))
        want |> push(2l)
        want |> push_from(encode(raw, "\n<|user|>\n{USR}", false, false))
        want |> push(2l)
        want |> push_from(encode(raw, "\n<|assistant|>\n", false, false))
        t |> success(same(engine_prefill(m, SYS, USR), want), "zephyr prefill token-for-token")
        delete raw
        delete m
    }
}

[test]
def test_chat_gemma(t : T?) {
    t |> run("Gemma-2 (SPM, no system role) folds system into the first user turn") @(t : T?) {
        let path = path_join(models_dir(), "gemma-2-2b-it-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_capped(path)
        var raw <- load_tokenizer_gguf(path)
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
        delete raw
        delete m
    }
}

[test]
def test_chat_phi(t : T?) {
    t |> run("Phi-3.5 (SPM) renders the Phi-3 prefill") @(t : T?) {
        let path = path_join(models_dir(), "Phi-3.5-mini-instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_capped(path)
        var raw <- load_tokenizer_gguf(path)
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
        delete raw
        delete m
    }
}

[test]
def test_chat_qwen(t : T?) {
    t |> run("Qwen2.5 (BPE) renders the ChatML prefill") @(t : T?) {
        let path = path_join(models_dir(), "Qwen2.5-1.5B-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_capped(path)
        var raw <- load_bpe_tokenizer_gguf(path)
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
        delete raw
        delete m
    }
}

[test]
def test_chat_llama3(t : T?) {
    t |> run("Llama-3 (arch=llama + BPE) renders the Llama-3 header prefill") @(t : T?) {
        let path = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_capped(path)
        var raw <- load_bpe_tokenizer_gguf(path)
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
        delete raw
        delete m
    }
}

[test]
def test_chat_end_to_end(t : T?) {
    t |> run("respond() streams a reply, stops on a stop token, records stats + transcript") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "gemma-2-2b-it-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_model(path, QuantMode.q8)
        m.config.seq_len = min(m.config.seq_len, 512l)
        var reply = ""
        var ntok = 0l
        with_job_que() {
            setup_dasllama_jobque()
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
            delete c
        }
        delete m
    }
}

// ===== Hermes tool-calling render (Qwen family) =====

// The reference is the Qwen2.5/Qwen3 chat template + HF added-token tokenization: tool JSONs one
// per line inside <tools>, and EVERY <tool_call>/</tool_call> spelling — the prose mentions in the
// format instructions included — encodes as its single vocab id. <tools> and (on Qwen2.5, which
// lacks the token) <tool_response> stay in the text runs.

[test]
def test_chat_tools_hermes(t : T?) {
    t |> run("Qwen ChatML renders the Hermes tool block token-for-token (needs Qwen2.5-0.5B)") @(t : T?) {
        let path = path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_capped(path)
        var raw <- load_bpe_tokenizer_gguf(path)
        let ims = special_id(raw, "<|im_start|>")
        let ime = special_id(raw, "<|im_end|>")
        let tco = special_id(m, "<tool_call>")
        let tcc = special_id(m, "</tool_call>")
        t |> success(tco >= 0l && tcc >= 0l, "the vocab carries the tool-call markers")
        t |> equal(special_id(m, "<tool_response>"), -1l)   // NOT a Qwen2.5 token — stays text below

        let tooldef = "\{\"type\": \"function\", \"function\": \{\"name\": \"get_weather\", \"parameters\": \{}}}"
        var c = create_chat_renderer(m, SYS, 8l)
        var tools : array<string>
        tools |> push(tooldef)
        set_tools(c, tools)
        add_user(c, USR)
        let got <- render_turn(m, c)

        var want : array<int64>
        want |> push(ims)
        want |> push_from(bpe_encode(raw, "system\n{SYS}\n\n# Tools\n\nYou may call one or more functions to assist with the user query.\n\nYou are provided with function signatures within <tools></tools> XML tags:\n<tools>\n{tooldef}\n</tools>\n\nFor each function call, return a json object with function name and arguments within "))
        want |> push(tco)
        want |> push(tcc)
        want |> push_from(bpe_encode(raw, " XML tags:\n"))
        want |> push(tco)
        want |> push_from(bpe_encode(raw, "\n\{\"name\": <function-name>, \"arguments\": <args-json-object>}\n"))
        want |> push(tcc)
        want |> push(ime)
        want |> push_from(bpe_encode(raw, "\n"))
        want |> push(ims)
        want |> push_from(bpe_encode(raw, "user\n{USR}"))
        want |> push(ime)
        want |> push_from(bpe_encode(raw, "\n"))
        want |> push(ims)
        want |> push_from(bpe_encode(raw, "assistant\n"))
        t |> success(same(got, want), "system tool block token-for-token")
        delete c
        delete raw
        delete m
    }
    t |> run("tool-call replay + tool-results turn render token-for-token (needs Qwen2.5-0.5B)") @(t : T?) {
        let path = path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_capped(path)
        var raw <- load_bpe_tokenizer_gguf(path)
        let ims = special_id(raw, "<|im_start|>")
        let ime = special_id(raw, "<|im_end|>")
        let tco = special_id(m, "<tool_call>")
        let tcc = special_id(m, "</tool_call>")

        // no system, no tools: the replay shape alone. The assistant called get_weather...
        var c = create_chat_renderer(m, "", 8l)
        add_user(c, USR)
        var toks : array<int64>
        let call = "\{\"name\": \"get_weather\", \"arguments\": \{\"city\": \"Paris\"}}"
        var calls : array<string>
        calls |> push(call)
        render_assistant_calls(m, c, "", calls, toks)
        var want : array<int64>
        want |> push(ims)
        want |> push_from(bpe_encode(raw, "user\n{USR}"))
        want |> push(ime)
        want |> push_from(bpe_encode(raw, "\n"))
        want |> push(ims)
        want |> push_from(bpe_encode(raw, "assistant\n"))
        want |> push(tco)
        want |> push_from(bpe_encode(raw, "\n{call}\n"))
        want |> push(tcc)
        want |> push(ime)
        want |> push_from(bpe_encode(raw, "\n"))
        t |> success(same(toks, want), "assistant tool-call turn token-for-token")

        // ...and its result arrives: a user-framed turn, <tool_response> as plain text
        var results : array<string>
        results |> push("\{\"temp_c\": 21}")
        add_tool_results(c, results)
        let got2 <- render_turn(m, c)
        var want2 : array<int64>
        want2 |> push(ims)
        want2 |> push_from(bpe_encode(raw, "user\n<tool_response>\n\{\"temp_c\": 21}\n</tool_response>"))
        want2 |> push(ime)
        want2 |> push_from(bpe_encode(raw, "\n"))
        want2 |> push(ims)
        want2 |> push_from(bpe_encode(raw, "assistant\n"))
        t |> success(same(got2, want2), "tool-results turn token-for-token")
        t |> success(c.history[2].role == Role.tool, "tool result recorded in the transcript")
        delete c
        delete raw
        delete m
    }
}

// ===== Non-thinking mode (the Qwen3 family's enable_thinking=false) =====

[test]
def test_chat_think_suppress(t : T?) {
    t |> run("set_thinking(false) appends the empty think block to the generation prompt (needs Qwen3-0.6B)") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3-0.6B-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_capped(path)
        var raw <- load_bpe_tokenizer_gguf(path)
        let ims = special_id(raw, "<|im_start|>")
        let ime = special_id(raw, "<|im_end|>")
        let tho = special_id(m, "<think>")
        let thc = special_id(m, "</think>")
        t |> success(tho >= 0l && thc >= 0l, "the vocab carries the think markers")

        // thinking on (the default): the plain ChatML generation prompt, no think block
        var c = create_chat_renderer(m, "", 8l)
        add_user(c, USR)
        let got <- render_turn(m, c)
        var want : array<int64>
        want |> push(ims)
        want |> push_from(bpe_encode(raw, "user\n{USR}"))
        want |> push(ime)
        want |> push_from(bpe_encode(raw, "\n"))
        want |> push(ims)
        want |> push_from(bpe_encode(raw, "assistant\n"))
        t |> success(same(got, want), "thinking default renders unchanged")
        delete c

        // thinking off: the Jinja template's enable_thinking=false form
        var c2 = create_chat_renderer(m, "", 8l)
        set_thinking(c2, false)
        add_user(c2, USR)
        let got2 <- render_turn(m, c2)
        var want2 : array<int64>
        want2 |> push_from(want)
        want2 |> push(tho)
        want2 |> push_from(bpe_encode(raw, "\n\n"))
        want2 |> push(thc)
        want2 |> push_from(bpe_encode(raw, "\n\n"))
        t |> success(same(got2, want2), "set_thinking(false) appends <think>\\n\\n</think>\\n\\n")
        delete c2
        delete raw
        delete m
    }
    t |> run("suppress is a vocab-gated no-op without think tokens (needs Qwen2.5-0.5B)") @(t : T?) {
        let path = path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_capped(path)
        t |> equal(special_id(m, "<think>"), -1l)
        var a = create_chat_renderer(m, "", 8l)
        add_user(a, USR)
        let got_on <- render_turn(m, a)
        delete a
        var b = create_chat_renderer(m, "", 8l)
        set_thinking(b, false)
        add_user(b, USR)
        let got_off <- render_turn(m, b)
        t |> success(same(got_on, got_off), "no think tokens => prompt unchanged, no panic")
        delete b
        delete m
    }
}

// ===== Template detection (model-free) =====

// Distinctive first special of a template's user turn — "" when the user turn opens with plain
// text (Zephyr's role markers are text; its only special is </s>).
def private first_user_special(ct : ChatTemplate) : string {
    for (p in ct.user.parts) {
        if (p.kind == ChatPartKind.special) {
            return p.s
        }
    }
    return ""
}

def private detected_special(src : string) : string {
    var m = Model()
    m.arch = "llama"   // deliberately the colliding arch: detection must not consult it
    m.chat_template_str = src
    let ct <- chat_template(m)
    return first_user_special(ct)
}

[test]
def test_chat_template_detection(t : T?) {
    t |> run("template sniffed from GGUF tokenizer.chat_template") @(t : T?) {
        // fragments carry each family's distinctive marker (no braces — das strings interpolate {})
        t |> equal(detected_special("xx <|start_header_id|> role <|end_header_id|> xx"), "<|start_header_id|>")
        t |> equal(detected_special("xx <|im_start|> role xx <|im_end|>"), "<|im_start|>")
        t |> equal(detected_special("xx <|turn>model xx <turn|> xx <|channel>thought"), "<|turn>")   // Gemma-4 channel format
        t |> equal(detected_special("xx <start_of_turn> role xx <end_of_turn>"), "<start_of_turn>")
        t |> equal(detected_special("xx <|user|> xx <|end|> xx"), "<|user|>")      // Phi-3: both markers
        t |> equal(detected_special("bos_token xx [INST] xx [/INST] eos_token"), "[INST]")
        t |> equal(detected_special("xx <|user|> xx eos_token xx"), "</s>")        // Zephyr: text markers
    }
    t |> run("[INST]-family templates close the assistant turn with </s>, not the user tail") @(t : T?) {
        // [INST]…[/INST] wraps USER turns only; the assistant's reply ends with EOS. The
        // derived user-tail close (` [/INST]`) is wrong for this family — multi-turn history
        // must render `… reply</s>[INST] next …`.
        var m = Model()
        m.chat_template_str = "bos_token xx [INST] xx [/INST] eos_token"
        let ct <- chat_template(m)
        t |> equal(length(ct.assistant_close.parts), 1, "v0.3 has an explicit assistant close")
        t |> equal(ct.assistant_close.parts[0].s, "</s>")
        var m2 = Model()
        m2.chat_template_str = "mistral-v7-tekken"
        let ct2 <- chat_template(m2)
        t |> equal(length(ct2.assistant_close.parts), 1, "v7-tekken has an explicit assistant close")
        t |> equal(ct2.assistant_close.parts[0].s, "</s>")
    }
    t |> run("arch fallback when the GGUF carries no template") @(t : T?) {
        var m = Model()
        m.arch = "qwen2"
        let ct <- chat_template(m)
        t |> equal(first_user_special(ct), "<|im_start|>")
        var m2 = Model()
        m2.arch = "llama"
        m2.tok.kind = TokKind.spm
        let ct2 <- chat_template(m2)
        t |> equal(first_user_special(ct2), "</s>")   // llama + SPM => Zephyr
        var m3 = Model()
        m3.arch = "qwen3"
        let ct3 <- chat_template(m3)
        t |> equal(first_user_special(ct3), "<|im_start|>")   // Qwen3 registry template is ChatML too
    }
}

// ===== <think>-stripping (model-free) =====

[test]
def test_chat_strip_think(t : T?) {
    t |> run("leading think block is stripped from history content") @(t : T?) {
        t |> equal(strip_think("<think>\nreasoning here\n</think>\n\nThe answer is 42."), "The answer is 42.")
        t |> equal(strip_think("  \n<think>r</think>answer"), "answer")   // leading whitespace before the block is fine
        t |> equal(strip_think("<think>never closed — budget ran out"), "")
    }
    t |> run("replies without a leading think block pass through") @(t : T?) {
        t |> equal(strip_think("The answer is 42."), "The answer is 42.")
        t |> equal(strip_think(""), "")
        // a think tag mentioned mid-reply is content, not a reasoning block
        t |> equal(strip_think("use the <think> tag like this"), "use the <think> tag like this")
    }
}

// ===== pending consumption (regression: an audio-only turn must not resurrect prior text) =====

[test]
def test_chat_pending_cleared_on_consume(t : T?) {
    t |> run("respond()/add_assistant() clear the consumed user text (needs SmolLM2-135M)") @(t : T?) {
        let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        with_job_que() {
            setup_dasllama_jobque()
            var m <- load_capped(path)
            var c = create_chat(m, "", 4l)   // max_new=4 — the state machine is under test, not the reply
            add_user(c, "Count to three.")
            let reply = respond(m, c, SamplingParams()) $(piece) => true
            // pending must not survive its consumption: render_prompt_audio reads c.pending gated
            // only on n_audio_rows, so a leftover here silently re-injects THIS text into a later
            // audio-only turn's prompt
            t |> equal(c.pending, "", "respond() cleared the consumed text (reply: '{reply}')")
            add_user(c, "And now four.")
            add_assistant(m, c, "One, two, three, four.")
            t |> equal(c.pending, "", "add_assistant() cleared the consumed text")
            delete c
            delete m
        }
    }
}

// ===== render seam: render_assistant produces EXACTLY the tokens add_assistant evals =====

[test]
def test_chat_render_seam(t : T?) {
    t |> run("render_assistant ≡ add_assistant evals; renderer chat holds no KV (needs SmolLM2-135M)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA model tests are JIT-only)")
            return
        }
        let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        with_job_que() {
            setup_dasllama_jobque()
            var m <- load_capped(path)
            // classic prefill is bit-exact under ANY chunking (test_prefill's contract), so a
            // byte-equal cache proves the render and eval paths agree token for token
            let saved_mode = get_prefill_attn_mode()
            set_prefill_attn_mode(AttnPrefillMode.classic)
            var a = create_chat(m, SYS, 4l)   // the eval path: two replayed turns
            add_user(a, "Count to three.")
            add_assistant(m, a, "One, two, three.")
            add_user(a, "And four?")
            add_assistant(m, a, "Four.")
            var b = create_chat_renderer(m, SYS, 4l)   // the render path: same turns, tokens only
            var toks : array<int64>
            add_user(b, "Count to three.")
            render_assistant(m, b, "One, two, three.", toks)
            add_user(b, "And four?")
            render_assistant(m, b, "Four.", toks)
            t |> equal(length(b.session.key_cache), 0, "renderer chat allocates no KV")
            t |> equal(b.session.n_past, 0l, "renderer chat never evals")
            t |> equal(a.session.n_past, int64(length(toks)), "rendered token count == evaled positions")
            t |> equal(length(b.history), length(a.history), "transcripts advance in lockstep")
            t |> equal(b.pending, "", "render_assistant consumed the pending text")
            t |> success(same(b.stop_ids, a.stop_ids), "renderer resolves the same stop ids")
            // eval the rendered stream on a fresh session: byte-equal caches ⟺ identical tokens
            var s <- create_session(m)
            eval(m, s, toks)
            t |> success(same(a.session.key_cache, s.key_cache), "key cache byte-equal: rendered ≡ evaled")
            t |> success(same(a.session.value_cache, s.value_cache), "value cache byte-equal: rendered ≡ evaled")
            set_prefill_attn_mode(saved_mode)
            delete s
            delete b
            delete a
            delete m
        }
    }
}
