options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: each test frees its model/session before the next loads, so multi-GB weights never accumulate in the process

require dastest/testing_boost public
require dasllama/dasllama   // the public facade — public-path tests exercise the API through it (engine + arch registrations + chat)

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // render/eval-seam test, CPU by design: on a Metal build the prefill guard panics otherwise
}
require dasllama/dasllama_tokenizer
require dasllama/dasllama_bpe
require daslib/jobque_boost
require daslib/fio
require math
require _model_tier   // model_available: presence + the >6 GiB PARITY_FULL tier gate

// The unified chat engine must render each arch's turn EXACTLY as the old per-model REPLs hand-rolled
// it — same special-token ids, same text segmentation — so collapsing the 5 REPLs into one changes no
// tokens. Each case rebuilds the old demo's first-turn prefill against the RAW tokenizer backend and
// compares it to render_turn(). GGUF models are gitignored, so each case skips cleanly when absent.

let SYS = "You are a helpful, friendly assistant. Keep answers concise."
let USR = "Once upon a time"

def private same(a, b : array<int64>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (va, vb in a, b) {
        if (va != vb) {
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
    for (va, vb in a, b) {
        if (va != vb) {
            return false
        }
    }
    return true
}

// Load capped to a tiny context: these cases only render prompts, and the session's KV cache is sized
// to config.seq_len — Phi/Llama-3's native 131072 would allocate ~64GB.
def private load_capped(path : string) : Model {
    var m <- load_model_(path, QuantMode.q8)
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
        var want <- [2l, 106l]
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
        let path = path_join(models_dir(), "Phi-3.5-mini-instruct-Q4_K_M.gguf")
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
        var want <- [raw.bos_id, sh]
        want |> push_from(bpe_encode(raw, "system")); want |> push(eh)
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
        var m <- load_model_(path, QuantMode.q8)
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
def test_chat_tools_hermes(t : T?) {   // nolint:STYLE038 — flat expected-token stream, one push per run
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
        var want2 := want
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
        // the reply matcher is vocab-gated the same way: a chatml family without the think
        // tokens (qwen2.5) must NOT arm, or a literal "<think>" echo reclassifies the reply
        let ts = make_think_stream(b)
        t |> success(ts.mode == ThinkMode.none, "no think vocab => the reply matcher stays unarmed")
        delete b
        delete m
    }
}

// ===== gemma-4 E-series thinking (opt-in via the <|think|> gate) =====

[test]
def test_chat_gemma4_thinking(t : T?) {   // nolint:STYLE038 — flat expected-token stream, one push per run
    t |> run("default = closed empty thought; thinking-ON = gate + bare opener (needs gemma-4-E2B)") @(t : T?) {
        let path = path_join(models_dir(), "gemma-4-E2B-it-Q4_K_M.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_capped(path)
        let trn = special_id(m, "<|turn>")
        let trc = special_id(m, "<turn|>")
        let cho = special_id(m, "<|channel>")
        let chc = special_id(m, "<channel|>")
        let thk = special_id(m, "<|think|>")
        t |> success(trn >= 0l && trc >= 0l && cho >= 0l && chc >= 0l, "the vocab carries the turn/channel markers")
        t |> success(thk >= 0l, "the vocab carries the <|think|> gate")

        // thinking OFF (the E-series default): the closed empty thought prefill — the exact
        // pre-thinking-arc token stream, pinned so the assistant_open restructure changes nothing
        var c = create_chat_renderer(m, "", 8l)
        t |> success(!c.enable_thinking, "gemma-4 defaults to thinking off")
        // instruct mode stops at a stray channel marker; thinking-ON must NOT (they frame the thought)
        let base_stops = length(c.stop_ids)
        var eff_off <- effective_stop_ids(c)
        t |> equal(length(eff_off), base_stops + 2, "thinking-off adds the two channel-marker stops")
        t |> success(find_index(eff_off, cho) >= 0 && find_index(eff_off, chc) >= 0, "the extras are the channel markers")
        delete eff_off
        set_thinking(c, true)
        var eff_on <- effective_stop_ids(c)
        t |> equal(length(eff_on), base_stops, "thinking-on keeps only the template stops")
        delete eff_on
        set_thinking(c, false)
        add_user(c, USR)
        let got <- render_turn(m, c)
        var want <- [bos_id(m), trn]
        want |> push_from(encode(m, "user\n{USR}", false, false))
        want |> push(trc)
        want |> push_from(encode(m, "\n", false, false))
        want |> push(trn)
        want |> push_from(encode(m, "model\n", false, false))
        want |> push(cho)
        want |> push_from(encode(m, "thought\n", false, false))
        want |> push(chc)
        t |> success(same(got, want), "default prefill token-for-token (instruct shape)")
        delete c

        // thinking ON: the <|think|> gate opens a system turn (even with no system prompt) and
        // the generation prompt is the bare model header — the model emits its own thought channel
        var c2 = create_chat_renderer(m, "", 8l)
        set_thinking(c2, true)
        add_user(c2, USR)
        let got2 <- render_turn(m, c2)
        var want2 <- [bos_id(m), trn]
        want2 |> push_from(encode(m, "system\n", false, false))
        want2 |> push(thk)
        want2 |> push(trc)
        want2 |> push_from(encode(m, "\n", false, false))
        want2 |> push(trn)
        want2 |> push_from(encode(m, "user\n{USR}", false, false))
        want2 |> push(trc)
        want2 |> push_from(encode(m, "\n", false, false))
        want2 |> push(trn)
        want2 |> push_from(encode(m, "model\n", false, false))
        t |> success(same(got2, want2), "thinking-ON prefill: <|think|> system gate + bare opener")
        delete c2

        // thinking ON with a system prompt: the gate precedes the prompt text in the same turn
        var c3 = create_chat_renderer(m, SYS, 8l)
        set_thinking(c3, true)
        add_user(c3, USR)
        let got3 <- render_turn(m, c3)
        var want3 <- [bos_id(m), trn]
        want3 |> push_from(encode(m, "system\n", false, false))
        want3 |> push(thk)
        want3 |> push_from(encode(m, SYS, false, false))
        want3 |> push(trc)
        want3 |> push_from(encode(m, "\n", false, false))
        want3 |> push(trn)
        want3 |> push_from(encode(m, "user\n{USR}", false, false))
        want3 |> push(trc)
        want3 |> push_from(encode(m, "\n", false, false))
        want3 |> push(trn)
        want3 |> push_from(encode(m, "model\n", false, false))
        t |> success(same(got3, want3), "thinking-ON + system prompt: gate precedes the prompt text")
        delete c3

        // a mid-conversation set_thinking(true) cannot inject the gate (the system turn already
        // rendered) — the prompt stays instruct-shaped and the channel-marker stops stay armed
        var c5 = create_chat_renderer(m, "", 8l)
        add_user(c5, USR)
        var replay : array<int64>
        render_assistant(m, c5, "The answer.", replay)   // consumes turn 1, thinking off — no gate
        delete replay
        set_thinking(c5, true)
        add_user(c5, "and again?")
        let got5 <- render_turn(m, c5)
        t |> success(find_index(got5, thk) < 0, "no late gate materializes")
        t |> equal(got5[length(got5) - 1], chc, "the opener keeps the closed empty thought")
        var eff5 <- effective_stop_ids(c5)
        t |> success(find_index(eff5, chc) >= 0, "the channel-marker stops stay armed (thinking never armed)")
        delete eff5
        delete c5
        delete m
    }
}

// ===== qwen35 Hermes tools (the qwen2/qwen3 block, kept by the 3.5 template) =====

[test]
def test_chat_qwen35_tools(t : T?) {
    t |> run("the qwen35 registration declares the Hermes tool format (model-free)") @(t : T?) {
        var ct <- arch_chat("qwen35")
        t |> equal(ct.tool_call_open, "<tool_call>")
        t |> equal(ct.tool_resp_open, "<tool_response>")
        t |> success(!empty(ct.tool_sys_prelude), "tool block prelude declared")
        delete ct
    }
    t |> run("tool markers resolve in the 3.5 vocab and render as single ids (needs Qwen3.5-0.8B)") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_capped(path)
        let tco = special_id(m, "<tool_call>")
        let tcc = special_id(m, "</tool_call>")
        t |> success(tco >= 0l && tcc >= 0l, "the vocab carries the tool-call markers")
        var c = create_chat_renderer(m, "", 8l)
        var tools : array<string>
        tools |> push("\{\"type\": \"function\", \"function\": \{\"name\": \"get_weather\", \"parameters\": \{}}}")
        set_tools(c, tools)
        add_user(c, USR)
        let got <- render_turn(m, c)
        // the format instructions quote the markers — each must encode as its single vocab id
        var n_open = 0
        var n_close = 0
        for (id in got) {
            if (id == tco) {
                n_open ++
            } elif (id == tcc) {
                n_close ++
            }
        }
        t |> success(n_open >= 1 && n_close >= 1, "system tool block renders the markers as vocab ids")
        delete c
        delete m
    }
}

// ===== ToolMode declarations + the gemma-4 tool wire (the tool-formats arc) =====

[test]
def test_chat_tool_modes(t : T?) {
    t |> run("each family's registration/template declares its ToolMode (model-free)") @(t : T?) {
        var g <- arch_chat("gpt-oss")
        t |> success(g.tool_mode == ToolMode.harmony, "gpt-oss declares harmony")
        delete g
        var g4 <- arch_chat("gemma4")
        t |> success(g4.tool_mode == ToolMode.gemma4, "gemma4 declares the DSL mode")
        delete g4
        var l <- arch_chat("llama")
        t |> success(l.tool_mode == ToolMode.llama_json, "llama declares the whole-reply JSON mode")
        delete l
        var q <- arch_chat("qwen3")
        t |> success(q.tool_mode == ToolMode.hermes, "the qwen family declares hermes")
        delete q
        // mistral rides template DETECTION (arch is "llama" for those GGUFs), not a registration
        var m = Model()
        m.arch = "llama"
        m.chat_template_str = "bos [INST] u [/INST] eos"
        let ct <- chat_template(m)
        t |> success(ct.tool_mode == ToolMode.mistral, "[INST] detection declares the mistral mode")
    }
    t |> run("a declared mode without its control specials demotes to none at create") @(t : T?) {
        // an [INST] template over a vocab with NO [TOOL_CALLS] family (v0.1/v0.2-class): the
        // template says mistral, the session honestly says none — the server 400s instead of
        // panicking on the missing specials at render
        var m = Model()
        m.arch = "llama"
        m.chat_template_str = "bos [INST] u [/INST] eos"
        var c = create_chat_renderer(m, "", 8l)
        t |> success(c.tmpl.tool_mode == ToolMode.none, "missing [TOOL_CALLS] family demotes to none")
        delete c
    }
}

[test]
def test_chat_gemma4_tool_wire(t : T?) {   // nolint:STYLE038 — flat expected-token stream, one push per run
    t |> run("defs turn + call replay + result turn token-for-token (needs gemma-4-E2B)") @(t : T?) {
        let path = path_join(models_dir(), "gemma-4-E2B-it-Q4_K_M.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_capped(path)
        let trn = special_id(m, "<|turn>")
        let trc = special_id(m, "<turn|>")
        let cho = special_id(m, "<|channel>")
        let chc = special_id(m, "<channel|>")
        let tlo = special_id(m, "<|tool>")
        let tlc = special_id(m, "<tool|>")
        let tco = special_id(m, "<|tool_call>")
        let tcc = special_id(m, "<tool_call|>")
        let tro = special_id(m, "<|tool_response>")
        let trr = special_id(m, "<tool_response|>")
        let q = special_id(m, "<|\"|>")
        t |> success(tlo >= 0l && tlc >= 0l && tco >= 0l && tcc >= 0l && tro >= 0l && trr >= 0l && q >= 0l,
                     "the vocab carries the tool + quote specials")

        // the defs turn: one <|tool>declaration DSL block in a forced system turn, quote tokens as specials
        let tooldef = ("\{\"type\": \"function\", \"function\": \{\"name\": \"get_weather\", " +
            "\"description\": \"Get current weather\", \"parameters\": \{\"type\": \"object\", " +
            "\"properties\": \{\"city\": \{\"type\": \"string\"}}, \"required\": [\"city\"]}}}")
        var c = create_chat_renderer(m, "", 8l)
        var tools : array<string>
        tools |> push(tooldef)
        set_tools(c, tools)
        add_user(c, USR)
        let got <- render_turn(m, c)
        var want <- [bos_id(m), trn]
        want |> push_from(encode(m, "system\n", false, false))
        want |> push(tlo)
        want |> push_from(encode(m, "declaration:get_weather\{description:", false, false))
        want |> push(q)
        want |> push_from(encode(m, "Get current weather", false, false))
        want |> push(q)
        want |> push_from(encode(m, ",parameters:\{properties:\{city:\{type:", false, false))
        want |> push(q)
        want |> push_from(encode(m, "STRING", false, false))
        want |> push(q)
        want |> push_from(encode(m, "}},required:[", false, false))
        want |> push(q)
        want |> push_from(encode(m, "city", false, false))
        want |> push(q)
        want |> push_from(encode(m, "],type:", false, false))
        want |> push(q)
        want |> push_from(encode(m, "OBJECT", false, false))
        want |> push(q)
        want |> push_from(encode(m, "}}", false, false))
        want |> push(tlc)
        want |> push(trc)
        want |> push_from(encode(m, "\n", false, false))
        want |> push(trn)
        want |> push_from(encode(m, "user\n{USR}", false, false))
        want |> push(trc)
        want |> push_from(encode(m, "\n", false, false))
        want |> push(trn)
        want |> push_from(encode(m, "model\n", false, false))
        want |> push(cho)
        want |> push_from(encode(m, "thought\n", false, false))
        want |> push(chc)
        t |> success(same(got, want), "defs turn token-for-token")
        delete c

        // call replay: the DSL block continues the model turn, which stays OPEN (no close parts)
        var c2 = create_chat_renderer(m, "", 8l)
        add_user(c2, USR)
        var calls : array<string>
        calls |> push("\{\"name\": \"get_weather\", \"arguments\": \{\"city\": \"Paris\"}}")
        var toks : array<int64>
        render_assistant_calls(m, c2, "", calls, toks)
        var want2 <- [bos_id(m), trn]
        want2 |> push_from(encode(m, "user\n{USR}", false, false))
        want2 |> push(trc)
        want2 |> push_from(encode(m, "\n", false, false))
        want2 |> push(trn)
        want2 |> push_from(encode(m, "model\n", false, false))
        want2 |> push(cho)
        want2 |> push_from(encode(m, "thought\n", false, false))
        want2 |> push(chc)
        want2 |> push(tco)
        want2 |> push_from(encode(m, "call:get_weather\{city:", false, false))
        want2 |> push(q)
        want2 |> push_from(encode(m, "Paris", false, false))
        want2 |> push(q)
        want2 |> push_from(encode(m, "}", false, false))
        want2 |> push(tcc)
        t |> success(same(toks, want2), "call replay token-for-token, turn left open")
        t |> success(c2.turn_open, "the model turn is recorded open")

        // the result continues the OPEN turn in-turn — no user frame, no close before it
        var results : array<string>
        results |> push("\{\"temp\": 21}")
        var names : array<string>
        names |> push("get_weather")
        add_tool_results(c2, results, names)
        let got3 <- render_turn(m, c2)
        var want3 : array<int64>
        want3 |> push(tro)
        want3 |> push_from(encode(m, "response:get_weather\{temp:21}", false, false))
        want3 |> push(trr)
        t |> success(same(got3, want3), "result turn token-for-token (in-turn, named)")
        delete c2
        delete m
    }
    t |> run("displaced results + leaked open turn both heal on the next render (needs gemma-4-E2B)") @(t : T?) {
        let path = path_join(models_dir(), "gemma-4-E2B-it-Q4_K_M.gguf")
        if (!stat(path).is_valid) {
            t |> skip("{path} not present")
            return
        }
        var m <- load_capped(path)
        let trn = special_id(m, "<|turn>")
        let trc = special_id(m, "<turn|>")
        let cho = special_id(m, "<|channel>")
        let chc = special_id(m, "<channel|>")
        let tro = special_id(m, "<|tool_response>")
        let trr = special_id(m, "<tool_response|>")

        // [user, asst(calls), tool, user]: the displaced results render FIRST (in the open turn),
        // then the turn closes, then the user turn — transcript order on the wire
        var c = create_chat_renderer(m, "", 8l)
        add_user(c, USR)
        var calls : array<string>
        calls |> push("\{\"name\": \"get_weather\", \"arguments\": \{\"city\": \"Paris\"}}")
        var toks : array<int64>
        render_assistant_calls(m, c, "", calls, toks)
        delete toks
        var results : array<string>
        results |> push("\{\"temp\": 21}")
        var names : array<string>
        names |> push("get_weather")
        add_tool_results(c, results, names)
        add_user(c, "and in Rome?")   // displaces the queued results turn
        let got <- render_turn(m, c)
        var want : array<int64>
        want |> push(tro)
        want |> push_from(encode(m, "response:get_weather\{temp:21}", false, false))
        want |> push(trr)
        want |> push(trc)   // the open model turn closes
        want |> push_from(encode(m, "\n", false, false))
        want |> push(trn)
        want |> push_from(encode(m, "user\nand in Rome?", false, false))
        want |> push(trc)
        want |> push_from(encode(m, "\n", false, false))
        want |> push(trn)
        want |> push_from(encode(m, "model\n", false, false))
        want |> push(cho)
        want |> push_from(encode(m, "thought\n", false, false))
        want |> push(chc)
        t |> success(same(got, want), "displaced results + close + user turn token-for-token")
        delete c

        // [user, asst(calls), user] with NO results: only the leaked close heals
        var c2 = create_chat_renderer(m, "", 8l)
        add_user(c2, USR)
        var toks2 : array<int64>
        render_assistant_calls(m, c2, "", calls, toks2)
        delete toks2
        add_user(c2, "never mind")
        let got2 <- render_turn(m, c2)
        var want2 : array<int64>
        want2 |> push(trc)
        want2 |> push_from(encode(m, "\n", false, false))
        want2 |> push(trn)
        want2 |> push_from(encode(m, "user\nnever mind", false, false))
        want2 |> push(trc)
        want2 |> push_from(encode(m, "\n", false, false))
        want2 |> push(trn)
        want2 |> push_from(encode(m, "model\n", false, false))
        want2 |> push(cho)
        want2 |> push_from(encode(m, "thought\n", false, false))
        want2 |> push(chc)
        t |> success(same(got2, want2), "the leaked open turn closes before the user turn")
        delete c2
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
        t |> equal(strip_think("<think>never closed — budget ran out"), "")  // nolint:STYLE039 - the em-dash IS the test input: multibyte content inside an unclosed think block
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
            // queued media dies with the turn it was queued for: rows an assistant-path consume
            // leaves armed would silently splice a stale image into a later unrelated turn
            c.n_image_rows = 2l   // planted: content is never read on this text path
            c.image_rows |> resize(8l)
            add_assistant(m, c, "One, two, three, four.")
            t |> equal(c.pending, "", "add_assistant() cleared the consumed text")
            t |> equal(c.n_image_rows, 0l, "add_assistant() cleared the queued media rows")
            t |> success(empty(c.image_rows), "the row buffer went with the count")
            delete c
            delete m
        }
    }
}

// ===== render seam: render_assistant produces EXACTLY the tokens add_assistant evals =====

[test]
def test_chat_render_seam(t : T?) {
    t |> run("render_assistant == add_assistant evals; renderer chat holds no KV (needs SmolLM2-135M)") @(t : T?) {
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
            t |> equal(a.session.n_past, long_length(toks), "rendered token count == evaled positions")
            t |> equal(length(b.history), length(a.history), "transcripts advance in lockstep")
            t |> equal(b.pending, "", "render_assistant consumed the pending text")
            t |> success(same(b.stop_ids, a.stop_ids), "renderer resolves the same stop ids")
            // eval the rendered stream on a fresh session: byte-equal caches ⟺ identical tokens
            var s <- create_session(m)
            eval(m, s, toks)
            t |> success(same(a.session.key_cache, s.key_cache), "key cache byte-equal: rendered == evaled")
            t |> success(same(a.session.value_cache, s.value_cache), "value cache byte-equal: rendered == evaled")
            set_prefill_attn_mode(saved_mode)
            delete s
            delete b
            delete a
            delete m
        }
    }
}

// detokenize a rendered span for the eyeball line; specials have no piece bytes, so they
// render as <id> — the markers under test must be VISIBLE in the log
def private detok(m : Model; ids : array<int64>) : string {
    let raw = build_string() $(writer) {
        for (id in ids) {
            let p = piece_(m, id)
            writer |> write(empty(p) ? "<{id}>" : p)
        }
    }
    return replace(raw, "\n", "\\n")
}

// The chat-audio marker gate: render_prompt_audio must bracket the soft-token splice in the
// family's audio markers — the render-side half of the chat-audio rail (encoder numerics are
// test_audio's; nothing else pins these template tags against a real vocab).
[test]
def test_chat_audio_markers(t : T?) {
    t |> run("voxtral audio turn opens with [BEGIN_AUDIO] (no closing marker - mtmd shape)") @(t : T?) {
        let path = path_join(models_dir(), "voxtral-mini-3b-q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        var c <- create_chat_renderer_(m)
        add_user(c, USR)
        c.n_audio_rows = 3l   // planted span: render only brackets it, the splice happens at eval
        var head : array<int64>
        var tail : array<int64>
        render_prompt_audio(m, c, head, tail)
        to_log(LOG_INFO, "voxtral audio-turn eyeball: '{detok(m, head)}' [audio x3] '{detok(m, tail)}'\n")
        let ba = special_id(m, "[BEGIN_AUDIO]")
        t |> success(ba >= 0l, "[BEGIN_AUDIO] resolves in the vocab")
        t |> equal(head[length(head) - 1], ba, "head closes with the opening audio marker")
        t |> success(!empty(tail) && tail[0] != ba, "no closing marker - the tail starts with text")
        delete c
        delete m
    }
    t |> run("qwen2audio audio turn is bracketed by <|audio_bos|>/<|audio_eos|>") @(t : T?) {
        let path = path_join(models_dir(), "qwen2audio-7b-q8_0.gguf")
        if (!model_available(t, path)) {   // 7.7 GB — runs under DASLLAMA_PARITY_FULL=1
            return
        }
        var m <- load_capped(path)
        var c <- create_chat_renderer_(m)
        add_user(c, USR)
        c.n_audio_rows = 3l
        var head : array<int64>
        var tail : array<int64>
        render_prompt_audio(m, c, head, tail)
        to_log(LOG_INFO, "qwen2audio audio-turn eyeball: '{detok(m, head)}' [audio x3] '{detok(m, tail)}'\n")
        let bos = special_id(m, "<|audio_bos|>")
        let eos = special_id(m, "<|audio_eos|>")
        t |> success(bos >= 0l && eos >= 0l, "audio markers resolve in the vocab")
        t |> equal(head[length(head) - 1], bos, "head closes with <|audio_bos|>")
        t |> equal(tail[0], eos, "tail opens with <|audio_eos|>")
        delete c
        delete m
    }
}
