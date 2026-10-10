options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options persistent_heap   // + explicit deletes below: each test frees its model/session before the next loads, so multi-GB weights never accumulate in the process
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama_chat   // harmony_reasoning_level / set_reasoning_effort_: the reasoning-level rule on a bare session
require dasllama/dasllama   // the public facade — public-path tests exercise the API through it (engine + arch registrations + chat)

[init]
def private allow_cpu_prefill_intent() {
    allow_cpu_prefill()   // render/eval-seam test, CPU by design: on a Metal build the prefill guard panics otherwise
}
require dasllama/dasllama_tokenizer
require dasllama/dasllama_blocks   // embed_text_rows: the media body cell builds rows from the prompt's own tokens
require dasllama/dasllama_bpe
require daslib/jobque_boost
require daslib/fio
require daslib/linq   // sequence_equal: the KV byte blobs
require math
require _model_tier   // model_available: presence + the >6 GiB PARITY_FULL tier gate

// The unified chat engine must render each arch's turn EXACTLY as the old per-model REPLs hand-rolled
// it — same special-token ids, same text segmentation — so collapsing the 5 REPLs into one changes no
// tokens. Each case rebuilds the old demo's first-turn prefill against the RAW tokenizer backend and
// compares it to render_turn(). GGUF models are gitignored, so each case skips cleanly when absent.

let SYS = "You are a helpful, friendly assistant. Keep answers concise."
let USR = "Once upon a time"

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
        if (!model_available(t, path)) {
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
        t |> success(ids_equal(engine_prefill(m, SYS, USR), want), "zephyr prefill token-for-token")
        delete raw
        delete m
    }
}

[test]
def test_chat_gemma(t : T?) {
    t |> run("Gemma-2 (SPM, no system role) folds system into the first user turn") @(t : T?) {
        let path = path_join(models_dir(), "gemma-2-2b-it-Q8_0.gguf")
        if (!model_available(t, path)) {
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
        t |> success(ids_equal(engine_prefill(m, SYS, USR), want), "gemma folded prefill token-for-token")
        delete raw
        delete m
    }
}

[test]
def test_chat_phi(t : T?) {
    t |> run("Phi-3.5 (SPM) renders the Phi-3 prefill") @(t : T?) {
        let path = path_join(models_dir(), "Phi-3.5-mini-instruct-Q4_K_M.gguf")
        if (!model_available(t, path)) {
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
        t |> success(ids_equal(engine_prefill(m, SYS, USR), want), "phi prefill token-for-token")
        delete raw
        delete m
    }
}

[test]
def test_chat_qwen(t : T?) {
    t |> run("Qwen2.5 (BPE) renders the ChatML prefill") @(t : T?) {
        let path = path_join(models_dir(), "Qwen2.5-1.5B-Instruct-Q8_0.gguf")
        if (!model_available(t, path)) {
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
        t |> success(ids_equal(engine_prefill(m, SYS, USR), want), "qwen chatml prefill token-for-token")
        // the marked render: a first turn's opening is its system turn, a later turn has none
        var c = create_chat(m, SYS)
        add_user(c, USR)
        var marked <- render_turn_marked(m, c)
        var inscope sys_toks <- bpe_encode(raw, "system\n{SYS}")
        var inscope nl_toks <- bpe_encode(raw, "\n")
        t |> equal(marked.opening, 2l + long_length(sys_toks) + long_length(nl_toks), "the opening is the system turn, marker to newline")
        t |> success(marked.opening > 0l && marked.opening < long_length(marked.toks), "and stops short of the user turn ({marked.opening} of {long_length(marked.toks)})")
        t |> success(ids_equal(marked.toks, want), "the marked render's tokens are render_turn's")
        c.first = false
        var later <- render_turn_marked(m, c)
        t |> equal(later.opening, 0l, "a later turn has no opening")
        delete later
        delete marked
        delete c
        delete raw
        delete m
    }
}

[test]
def test_chat_llama3(t : T?) {
    t |> run("Llama-3 (arch=llama + BPE) renders the Llama-3 header prefill") @(t : T?) {
        let path = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        var raw <- load_bpe_tokenizer_gguf(path)
        let sh = special_id(raw, "<|start_header_id|>")
        let eh = special_id(raw, "<|end_header_id|>")
        let eot = special_id(raw, "<|eot_id|>")
        // the model's own template: every system turn opens with its knowledge-cutoff line and a date line, the
        // caller's system text after them - and a conversation with no system message still carries the two lines
        set_chat_date("26 Jul 2024")
        let header = "Cutting Knowledge Date: December 2023\nToday Date: 26 Jul 2024\n\n"
        var want <- [raw.bos_id, sh]
        want |> push_from(bpe_encode(raw, "system")); want |> push(eh)
        want |> push_from(bpe_encode(raw, "\n\n{header}{SYS}"))
        want |> push(eot)
        want |> push(sh); want |> push_from(bpe_encode(raw, "user")); want |> push(eh)
        want |> push_from(bpe_encode(raw, "\n\n{USR}"))
        want |> push(eot)
        want |> push(sh); want |> push_from(bpe_encode(raw, "assistant")); want |> push(eh)
        want |> push_from(bpe_encode(raw, "\n\n"))
        t |> success(ids_equal(engine_prefill(m, SYS, USR), want), "llama3 header prefill token-for-token")
        var inscope bare <- engine_prefill(m, "", USR)
        var inscope lead <- bpe_encode(raw, "\n\n{header}")
        t |> success(length(bare) > 4 + length(lead) && ids_equal([for (k in range(length(lead))); bare[4 + k]], lead),
            "with no system message the turn still states the cutoff and the date")
        set_chat_date("")
        var inscope today <- engine_prefill(m, "", USR)
        t |> success(!ids_equal(today, bare), "unpinned, the date is the day the turn renders")
        delete raw
        delete m
    }
}

[test]
def test_chat_end_to_end(t : T?) {
    t |> run("respond() streams a reply, stops on a stop token, records stats + transcript") @(t : T?) {
        return if (!jit_only(t))
        let path = path_join(models_dir(), "gemma-2-2b-it-Q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_model_(path, QuantMode.q8)
        m.config.seq_len = min(m.config.seq_len, 512l)
        var reply = ""
        var ntok = 0l
        with_dasllama_jobque() {
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
        if (!model_available(t, path)) {
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
        t |> success(ids_equal(got, want), "system tool block token-for-token")
        delete c
        delete raw
        delete m
    }
    t |> run("tool-call replay + tool-results turn render token-for-token (needs Qwen2.5-0.5B)") @(t : T?) {
        let path = path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        var raw <- load_bpe_tokenizer_gguf(path)
        let ims = special_id(raw, "<|im_start|>")
        let ime = special_id(raw, "<|im_end|>")
        let tco = special_id(m, "<tool_call>")
        let tcc = special_id(m, "</tool_call>")

        // no system, no tools: the replay shape alone - the conversation opens with the system turn the model's own
        // template states for one that carries none. The assistant called get_weather...
        var c = create_chat_renderer(m, "", 8l)
        t |> equal(c.tmpl.default_system, "You are Qwen, created by Alibaba Cloud. You are a helpful assistant.", "the template's default system text, read off its escaped string literal")
        add_user(c, USR)
        var toks : array<int64>
        let call = "\{\"name\": \"get_weather\", \"arguments\": \{\"city\": \"Paris\"}}"
        var calls : array<string>
        calls |> push(call)
        render_assistant_calls(m, c, "", calls, toks)
        var want : array<int64>
        want |> push(ims)
        want |> push_from(bpe_encode(raw, "system\n{c.tmpl.default_system}"))
        want |> push(ime)
        want |> push_from(bpe_encode(raw, "\n"))
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
        t |> success(ids_equal(toks, want), "assistant tool-call turn token-for-token")

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
        t |> success(ids_equal(got2, want2), "tool-results turn token-for-token")
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
        if (!model_available(t, path)) {
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
        t |> success(ids_equal(got, want), "thinking default renders unchanged")
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
        t |> success(ids_equal(got2, want2), "set_thinking(false) appends <think>\\n\\n</think>\\n\\n")
        delete c2
        delete raw
        delete m
    }
    t |> run("suppress is a vocab-gated no-op without think tokens (needs Qwen2.5-0.5B)") @(t : T?) {
        let path = path_join(models_dir(), "Qwen2.5-0.5B-Instruct-Q8_0.gguf")
        if (!model_available(t, path)) {
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
        t |> success(ids_equal(got_on, got_off), "no think tokens => prompt unchanged, no panic")
        // the reply matcher is vocab-gated the same way: a chatml family without the think
        // tokens (qwen2.5) must NOT arm, or a literal "<think>" echo reclassifies the reply
        let ts = make_think_stream(b)
        t |> success(ts.mode == ThinkMode.none, "no think vocab => the reply matcher stays unarmed")
        delete b
        delete m
    }
}

// ===== preserve_thinking: a replayed assistant turn renders the think block the model generated =====

// one replay of "Four." after USR on a fresh renderer, preserve set as asked, the reasoning handed in
def private replay_four(m : Model; preserve : bool; reasoning : string) : array<int64> {
    var c = create_chat_renderer(m, "", 8l)
    set_preserve_thinking(c, preserve)
    add_user(c, USR)
    var got : array<int64>
    render_assistant(m, c, "Four.", got, reasoning)
    delete c
    return <- got
}

[test]
def test_chat_preserve_thinking(t : T?) {
    t |> run("preserve_thinking replays an assistant turn with its think block - empty, or around its reasoning (needs Qwen3-0.6B)") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3-0.6B-Q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        var raw <- load_bpe_tokenizer_gguf(path)
        let ims = special_id(raw, "<|im_start|>")
        let ime = special_id(raw, "<|im_end|>")
        let tho = special_id(m, "<think>")
        let thc = special_id(m, "</think>")
        // every replay opens with the user turn and the assistant header, and ends with the content and its close
        var head : array<int64>
        head |> push(ims)
        head |> push_from(bpe_encode(raw, "user\n{USR}"))
        head |> push(ime)
        head |> push_from(bpe_encode(raw, "\n"))
        head |> push(ims)
        head |> push_from(bpe_encode(raw, "assistant\n"))
        var tail := bpe_encode(raw, "Four.")
        tail |> push(ime)
        tail |> push_from(bpe_encode(raw, "\n"))
        // off (the default): no think block, whatever reasoning the turn carried
        let got <- replay_four(m, false, "Two and two.")
        var want := head
        want |> push_from(tail)
        t |> success(ids_equal(got, want), "preserve off: header, content, close")
        // on, no reasoning: the empty block, the tokens a thinking-off generation prompt put there
        let got2 <- replay_four(m, true, "")
        var want2 := head
        want2 |> push(tho)
        want2 |> push_from(bpe_encode(raw, "\n\n"))
        want2 |> push(thc)
        want2 |> push_from(bpe_encode(raw, "\n\n"), tail)
        t |> success(ids_equal(got2, want2), "preserve on, no reasoning: <think>\\n\\n</think>\\n\\n ahead of the content")
        // on, with reasoning: the reasoning between the markers
        let got3 <- replay_four(m, true, "Two and two.")
        var want3 := head
        want3 |> push(tho)
        want3 |> push_from(bpe_encode(raw, "\nTwo and two.\n"))
        want3 |> push(thc)
        want3 |> push_from(bpe_encode(raw, "\n\n"), tail)
        t |> success(ids_equal(got3, want3), "preserve on, reasoning: <think>\\n...\\n</think>\\n\\n ahead of the content")
        delete raw
        delete m
    }
}

// ===== Qwen3.6's tool format: sniffed off the template, rendered as the template's own text =====

//! the Qwen3.6 chat template's text for two tools, a system line and a user line, thinking off (its own Jinja rendering)
let private XMLF_FIRST = "<|im_start|>system\n# Tools\n\nYou have access to the following functions:\n\n<tools>\n\{\"type\": \"function\", \"function\": \{\"name\": \"control\", \"description\": \"Switch a device.\", \"parameters\": \{\"type\": \"object\", \"properties\": \{\"point\": \{\"type\": \"string\", \"description\": \"the device id\"}, \"value\": \{\"type\": \"string\"}}, \"required\": [\"point\"]}}}\n\{\"type\": \"function\", \"function\": \{\"name\": \"not_for_me\", \"description\": \"The phrase was not addressed to the assistant.\", \"parameters\": \{\"type\": \"object\", \"properties\": \{}, \"required\": []}}}\n</tools>\n\nIf you choose to call a function ONLY reply in the following format with NO suffix:\n\n<tool_call>\n<function=example_function_name>\n<parameter=example_parameter_1>\nvalue_1\n</parameter>\n<parameter=example_parameter_2>\nThis is the value for the second parameter\nthat can span\nmultiple lines\n</parameter>\n</function>\n</tool_call>\n\n<IMPORTANT>\nReminder:\n- Function calls MUST follow the specified format: an inner <function=...></function> block must be nested within <tool_call></tool_call> XML tags\n- Required parameters MUST be specified\n- You may provide optional reasoning for your function call in natural language BEFORE the function call, but NOT after\n- If there is no function call available, answer the question like normal with your current knowledge and do not tell the user about function calls\n</IMPORTANT>\n\nYou are a terse assistant.<|im_end|>\n<|im_start|>user\nTurn it off.<|im_end|>\n<|im_start|>assistant\n<think>\n\n</think>\n\n"
//! ... and the same conversation continued: the call turn replayed, two results, a user line, thinking off
let private XMLF_REPLAY = "<|im_start|>assistant\n<think>\n\n</think>\n\n<tool_call>\n<function=control>\n<parameter=point>\nlight.1\n</parameter>\n<parameter=value>\nfalse\n</parameter>\n</function>\n</tool_call><|im_end|>\n<|im_start|>user\n<tool_response>\nok\n</tool_response>\n<tool_response>\ndone\n</tool_response><|im_end|>\n<|im_start|>user\nThanks.<|im_end|>\n<|im_start|>assistant\n<think>\n\n</think>\n\n"

// the rendered tokens as the template's text: piece by piece, the template's markers by their spelling, since
// decode drops the specials
def private pieces_text(m : Model; toks : array<int64>) : string {
    var marks : table<int64; string>
    for (s in fixed_array("<|im_start|>", "<|im_end|>", "<think>", "</think>", "<tool_call>", "</tool_call>", "<tool_response>", "</tool_response>")) {
        let id = special_id(m, s)
        if (id >= 0l) {
            marks[id] = s
        }
    }
    return build_string() $(var w) {
        for (id in toks) {
            w |> write(marks?[id] ?? piece(m, id))
        }
    }
}

[test]
def test_chat_xml_function(t : T?) {   // nolint:STYLE038 - one template, every form of its turns held to its own text
    t |> run("a template with <parameter= blocks installs the Qwen3.6 tool format (model-free)") @(t : T?) {
        var m = Model()
        m.chat_template_str = "xx <|im_start|> role xx <|im_end|> xx <tool_call>\n<function=example_function_name>\n<parameter=example_parameter_1>\nvalue_1 xx"
        let ct <- chat_template(m)
        t |> success(ct.tool_mode == ToolMode.xml_function, "the Qwen3.6 format")
        t |> equal("<tool_call>", ct.tool_call_open)
        t |> success(starts_with(ct.tool_sys_prelude, "# Tools\n\nYou have access"), "its tool block opener")
        t |> equal(4, length(ct.assistant_open_think.parts), "a thinking turn's generation prompt ends in an open think line")
        var m2 = Model()
        m2.chat_template_str = "xx <|im_start|> role xx <|im_end|> xx <tool_call> xx"
        let ct2 <- chat_template(m2)
        t |> success(ct2.tool_mode == ToolMode.hermes, "the ChatML template without them keeps Hermes")
    }
    t |> run("the first turn and a replayed call turn render the template's own text (needs Qwen3.5-0.8B)") @(t : T?) {
        let path = path_join(models_dir(), "Qwen3.5-0.8B-Q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        // the definitions as the server hands them over: `write_json_compact`'s one-line `tojson` shape
        var tools <- ["\{\"type\": \"function\", \"function\": \{\"name\": \"control\", \"description\": \"Switch a device.\", \"parameters\": \{\"type\": \"object\", \"properties\": \{\"point\": \{\"type\": \"string\", \"description\": \"the device id\"}, \"value\": \{\"type\": \"string\"}}, \"required\": [\"point\"]}}}",
                      "\{\"type\": \"function\", \"function\": \{\"name\": \"not_for_me\", \"description\": \"The phrase was not addressed to the assistant.\", \"parameters\": \{\"type\": \"object\", \"properties\": \{}, \"required\": []}}}"]
        var c = create_chat_renderer(m, "You are a terse assistant.", 8l)
        xml_function_tools(c.tmpl)   // what the 0.8B's own template sniffs to, pinned here so the cell holds the format, not the sniff
        set_thinking(c, false)
        set_preserve_thinking(c, true)
        var tools_copy := tools
        set_tools(c, tools_copy)
        add_user(c, "Turn it off.")
        let first <- render_turn_marked(m, c)
        t |> equal(XMLF_FIRST, pieces_text(m, first.toks), "the first turn: the tool block ahead of the system text, the template's instruction, the closed think block")
        // the opening's shared head: the tool block, which every request on these tools shares whatever its system text
        t |> success(first.system_head > 16l && first.system_head < first.opening, "the shared head is a proper part of the opening ({first.system_head} of {first.opening} tokens)")
        let head_toks <- [for (i in range64(first.system_head)); first.toks[i]]
        let head = pieces_text(m, head_toks)
        t |> success(find(head, "</tools>") >= 0 && find(head, "terse assistant") < 0, "... the tool block, none of the system text")
        var replay : array<int64>
        let calls <- ["\{\"name\": \"control\", \"arguments\": \{\"point\": \"light.1\", \"value\": \"false\"}}"]
        render_assistant_calls(m, c, "", calls, replay)
        let results <- ["ok", "done"]
        add_tool_results(c, results)
        add_user(c, "Thanks.")
        let next <- render_turn_marked(m, c)
        replay |> push_from(next.toks)
        // the replay re-renders the first turn's prompt, then the call turn, the results and the next turn
        let gen = "<|im_start|>assistant\n<think>\n\n</think>\n\n"
        t |> equal("{XMLF_FIRST}{slice(XMLF_REPLAY, length(gen))}", pieces_text(m, replay), "the call turn as <function=> blocks, the results in one user turn, the next turn")
        t |> success(find_index(first.toks, special_id(m, "<tool_call>")) >= 0, "the instruction's call markers render as the vocab's special tokens, not as text")
        // a call turn with text alongside: the text, a blank line, the blocks
        var c3 = create_chat_renderer(m, "You are a terse assistant.", 8l)
        xml_function_tools(c3.tmpl)
        set_thinking(c3, false)
        var tools3 := tools
        set_tools(c3, tools3)
        add_user(c3, "Turn it off.")
        var with_text : array<int64>
        render_assistant_calls(m, c3, "Sure.", calls, with_text)
        t |> success(find(pieces_text(m, with_text), "Sure.\n\n<tool_call>\n<function=control>") >= 0, "the text ahead of the block, a blank line between")
        // thinking on: the generation prompt opens the think block, a replayed turn writes its own closed block once
        var c4 = create_chat_renderer(m, "You are a terse assistant.", 8l)
        xml_function_tools(c4.tmpl)
        set_thinking(c4, true)
        set_preserve_thinking(c4, true)
        add_user(c4, "Turn it off.")
        let gen4 <- render_turn_marked(m, c4)
        let gen4_text = pieces_text(m, gen4.toks)
        t |> success(ends_with(gen4_text, "<|im_start|>assistant\n<think>\n"), "the thinking generation prompt ends inside the think block")
        let sp = split_reasoning(c4, "I should switch it.\n</think>\n\nDone.")
        t |> equal("I should switch it.", sp.reasoning, "a reply to that prompt opens with reasoning and never writes the opener")
        t |> equal("Done.", sp.content)
        var rep4 : array<int64>
        render_assistant(m, c4, "Done.", rep4, "I should switch it.")
        let rep4_text = pieces_text(m, rep4)
        t |> success(find(rep4_text, "<|im_start|>assistant\n<think>\nI should switch it.\n</think>\n\nDone.<|im_end|>") >= 0, "the replayed turn: the plain header, the think block once, the answer")
        var openers = 0
        var at = find(rep4_text, "<think>")
        while (at >= 0) {
            openers ++
            at = find(rep4_text, "<think>", at + 1)
        }
        t |> equal(1, openers, "one think opener in the replay, not the generation prompt's and the block's")
        delete c4
        delete c3
        // two user messages in a row: the earlier renders as its own turn (render_user), the later gets the generation prompt
        var c2 = create_chat_renderer(m, "You are a terse assistant.", 8l)
        set_thinking(c2, false)
        add_user(c2, "A house note.")
        var two : array<int64>
        render_user(m, c2, two)
        add_user(c2, "Turn it off.")
        let last <- render_turn_marked(m, c2)
        two |> push_from(last.toks)
        t |> equal("<|im_start|>system\nYou are a terse assistant.<|im_end|>\n<|im_start|>user\nA house note.<|im_end|>\n<|im_start|>user\nTurn it off.<|im_end|>\n<|im_start|>assistant\n<think>\n\n</think>\n\n",
            pieces_text(m, two), "two user turns, the system turn once, the generation prompt after the last")
        delete c2
        delete c
        delete m
    }
}

// ===== gemma-4 E-series thinking (ON by default; instruct = the opt-out) =====

// The instruct-mode TEXT turn through respond: the E-series rambles past its answer through a
// stray channel marker, and the guard ends the turn there - the content half carries no marker
// and the turn does not run to the budget.
[test]
def test_chat_gemma4_instruct_text_turn(t : T?) {
    t |> run("thinking off on E2B: '2+2' answers and a stray channel marker ends the turn") @(t : T?) {
        let path = path_join(models_dir(), "gemma-4-E2B-it-Q4_K_M.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        allow_cpu_prefill()
        with_dasllama_jobque() {
            var c = create_chat(m, "", 64l)
            set_thinking(c, false)
            add_user(c, "What is 2+2? Answer with just the number.")
            let reply = respond(m, c, SamplingParams()) $(_piece) {
                return true
            }
            let sp = split_reasoning(c, reply)
            to_log(LOG_INFO, "instruct text turn: '{replace(reply, "\n", "\\n")}' ({stats(c.session).n_gen} tokens)\n")
            t |> success(find(sp.content, "4") >= 0, "the answer is 4: '{sp.content}'")
            t |> success(find(sp.content, "<channel|>") < 0 && find(sp.content, "<|channel>") < 0, "no channel marker reaches the content half")
            t |> success(stats(c.session).n_gen < 64l, "the turn ended on a stop, not the budget ({stats(c.session).n_gen} tokens)")
            delete c
        }
        delete m
    }
}

//! the last row's logits of `toks` prefilled with rows [lo, hi) as embedding rows: one fused body, or text / rows / text as three calls
def private e_series_body_logits(m : Model; toks : array<int64>; lo, hi : int64; fused, with_ids : bool) : array<float> {
    var head <- [for (k in range64(lo)); toks[k]]
    var mid <- [for (k in range64(lo, hi)); toks[k]]
    var tail <- [for (k in range64(hi, long_length(toks))); toks[k]]
    var rows : array<float>
    rows |> resize((hi - lo) * m.config.dim)
    embed_text_rows(m, mid, rows, 0l)
    var s <- create_session(m)
    if (fused) {
        var body : array<float>
        media_body_rows(m, s, head, rows, hi - lo, m.config.dim, tail, body)
        if (!with_ids) {
            s.embd_ids |> clear()
        }
        eval_embd_body(m, s, body, long_length(toks), 0l, 0l, int2(0))
        delete body
    } else {
        eval(m, s, head)
        eval_embd(m, s, rows, hi - lo)
        eval(m, s, tail)
    }
    var out := s.logits
    delete s
    delete head
    delete mid
    delete tail
    delete rows
    return <- out
}

//! one side's top predicted token beside a logits compare, its id and its piece
def private log_top_token(m : Model; side : string; logits : array<float>) {
    let top = argmax_of(logits)
    to_log(LOG_INFO, "E-series media body, {side}: top token {top} '{piece_(m, top)}'\n")
}

let private E_BODY_NEAR = 0.25  //! of the logits' peak: one body against three calls differs by call shape alone - 0.11 here, and the same tokens as ids read 0.14 apart in one call and in three
let private E_BODY_FAR = 1.0     //! of the peak: text rows under the padding token's per-layer input are another prompt - 1.9 here

[test]
def test_gemma4_e_media_body(t : T?) {
    t |> run("an E-series media body gives its text rows their own tokens' per-layer input: one call == text / rows / text (needs gemma-4-E2B)") @(t : T?) {
        let path = path_join(models_dir(), "gemma-4-E2B-it-Q4_K_M.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        allow_cpu_prefill()
        with_dasllama_jobque() {
            var toks <- encode(m, "The quick brown fox jumps over the lazy dog while the little robot watches from a small hill near the old village bakery", true, false)
            let np = long_length(toks)
            t |> success(np > 16l, "the prompt holds a head, a span and a tail ({np} tokens)")
            var split <- e_series_body_logits(m, toks, 5l, 11l, false, true)
            var fused <- e_series_body_logits(m, toks, 5l, 11l, true, true)
            var padded <- e_series_body_logits(m, toks, 5l, 11l, true, false)
            let peak = logits_maxabs(split)
            let near = logits_maxdiff(fused, split)
            let far = logits_maxdiff(padded, split)
            to_log(LOG_INFO, "E-series media body: one call {near} from three, the padding-id body {far}, at a {peak} peak\n")
            log_top_token(m, "three calls", split)
            log_top_token(m, "one call", fused)
            log_top_token(m, "the padding-id body", padded)
            t |> success(near < E_BODY_NEAR * peak, "one call reads as three ({near} at a {peak} peak)")
            t |> success(far > E_BODY_FAR * peak, "the ids are load-bearing: a body without them reads another prompt ({far} at a {peak} peak)")
            delete split
            delete fused
            delete padded
            delete toks
        }
        delete m
    }
}

[test]
def test_chat_gptoss_budget_marks(t : T?) {
    t |> run("gpt-oss: the reasoning budget's marks are harmony's analysis channel and its end (needs gpt-oss-20b, large tier)") @(t : T?) {
        let path = path_join(models_dir(), "gpt-oss-20b-mxfp4.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        let ch = special_id(m, "<|channel|>")
        let fin = special_id(m, "<|end|>")
        let start = special_id(m, "<|start|>")
        let msg = special_id(m, "<|message|>")
        t |> success(ch >= 0l && fin >= 0l && start >= 0l && msg >= 0l, "the vocab carries the harmony channel, end, start and message markers")
        var c = create_chat_renderer(m, "", 8l)
        var marks <- think_budget_marks(m, c)
        var analysis <- encode(m, "analysis", false, false)
        let nopen = length(marks.open)
        t |> success(nopen >= 3 && marks.open[0] == ch && marks.open[nopen - 1] == msg, "the open is the channel mark, `analysis` and the message mark ({nopen} tokens)")
        t |> success(nopen == length(analysis) + 2 && ids_equal(analysis, marks.open[1 .. nopen - 1]), "the word between the marks is `analysis` as the tokenizer writes it")
        t |> equal(fin, marks.close_tok, "the budget leaves the span on the message end the model writes")
        let nclose = length(marks.close)
        t |> success(nclose >= 3 && marks.close[0] == fin && marks.close[1] == start && marks.close[nclose - 1] == msg, "the forced close ends the message and opens the final channel up to its message mark ({nclose} tokens)")
        t |> success(length(marks.reopen) == 1 && marks.reopen[0] == ch, "a channel mark written after the forced close ends the turn")
        delete analysis
        delete marks
        delete c
        delete m
    }
}

[test]
def test_chat_gemma4_thinking(t : T?) {   // nolint:STYLE038 — flat expected-token stream, one push per run
    t |> run("default = thinking; instruct opt-out = the template's bare model turn (needs gemma-4-E2B)") @(t : T?) {
        let path = path_join(models_dir(), "gemma-4-E2B-it-Q4_K_M.gguf")
        if (!model_available(t, path)) {
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

        var c = create_chat_renderer(m, "", 8l)
        t |> success(c.enable_thinking, "gemma-4 defaults to thinking on (thinking-trained family)")
        // the reasoning budget's marks: gemma-4 opens its thought with the channel mark and the word `thought` - a
        // sequence, which no single special lookup resolves - and leaves it on the channel close
        var marks <- think_budget_marks(m, c)
        var thought <- encode(m, "thought", false, false)
        t |> success(length(marks.open) == length(thought) + 1 && marks.open[0] == cho && ids_equal(thought, marks.open[1 .. length(marks.open)]), "the budget's open sequence is the channel mark followed by `thought` as the tokenizer writes it ({length(marks.open)} tokens)")
        delete thought
        t |> equal(chc, marks.close_tok, "the budget leaves the span on the channel close the model writes")
        t |> success(!empty(marks.close) && marks.close[0] == chc, "the forced close is the channel close")
        t |> success(find_index(marks.reopen, cho) >= 0 && find_index(marks.reopen, chc) >= 0, "both channel markers end the turn once the forced close is out ({length(marks.reopen)} markers)")
        delete marks
        // instruct opt-out: the E-series template opens the model turn bare
        set_thinking(c, false)
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
        t |> success(ids_equal(got, want), "default prefill token-for-token (instruct shape)")
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
        t |> success(ids_equal(got2, want2), "thinking-ON prefill: <|think|> system gate + bare opener")
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
        t |> success(ids_equal(got3, want3), "thinking-ON + system prompt: gate precedes the prompt text")
        delete c3

        // a mid-conversation set_thinking(true) cannot inject the gate (the system turn already
        // rendered) — the prompt stays instruct-shaped and the channel-marker stops stay armed
        var c5 = create_chat_renderer(m, "", 8l)
        set_thinking(c5, false)
        add_user(c5, USR)
        var replay : array<int64>
        render_assistant(m, c5, "The answer.", replay)   // consumes turn 1, thinking off — no gate
        delete replay
        set_thinking(c5, true)
        add_user(c5, "and again?")
        let got5 <- render_turn(m, c5)
        t |> success(find_index(got5, thk) < 0, "no late gate materializes")
        t |> success(find_index(got5, chc) < 0, "the opener is the E-series template's bare model turn: no closed thought ahead of the answer")
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
        if (!model_available(t, path)) {
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
        var m7 = Model()
        m7.arch = "llama"
        m7.chat_template_str = "bos [SYSTEM_PROMPT]s[/SYSTEM_PROMPT][INST]u[/INST] eos"
        let ct7 <- chat_template(m7)
        t |> success(ct7.tool_mode == ToolMode.mistral, "the v7-tekken shape (Mistral-Small 3.x) declares the mistral mode too")
        var m3 <- arch_chat("mistral3")
        t |> success(m3.tool_mode == ToolMode.mistral, "the mistral3 arch declares it")
        delete m3
    }
    t |> run("harmony's Reasoning level: the one set, else medium on a thinking turn and low off it (model-free)") @(t : T?) {
        var c = ChatSession(tmpl <- arch_chat("gpt-oss"))
        c.enable_thinking = c.tmpl.think_default
        c.think_vocab = true
        t |> equal(harmony_reasoning_level(c), "medium", "thinking on, nothing set")
        set_thinking_(c, false)
        t |> equal(harmony_reasoning_level(c), "low", "thinking off, nothing set")
        set_reasoning_effort_(c, "high")
        t |> equal(harmony_reasoning_level(c), "high", "the level set wins over the flag")
        set_reasoning_effort_(c, "minimal")
        t |> equal(harmony_reasoning_level(c), "low", "minimal reads as low")
        set_reasoning_effort_(c, "")
        t |> equal(harmony_reasoning_level(c), "low", "unset returns to the flag")
        var tripped = false
        try {
            set_reasoning_effort_(c, "max")
        } recover {
            tripped = true
        }
        t |> success(tripped, "a level that is not one of the three panics")
        delete c
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
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        let trn = special_id(m, "<|turn>")
        let trc = special_id(m, "<turn|>")
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
        set_thinking(c, false)
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
        t |> success(ids_equal(got, want), "defs turn token-for-token")
        delete c

        // call replay: the DSL block continues the model turn, which stays OPEN (no close parts)
        var c2 = create_chat_renderer(m, "", 8l)
        set_thinking(c2, false)
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
        want2 |> push(tco)
        want2 |> push_from(encode(m, "call:get_weather\{city:", false, false))
        want2 |> push(q)
        want2 |> push_from(encode(m, "Paris", false, false))
        want2 |> push(q)
        want2 |> push_from(encode(m, "}", false, false))
        want2 |> push(tcc)
        t |> success(ids_equal(toks, want2), "call replay token-for-token, turn left open")
        t |> success(c2.turn_open, "the model turn is recorded open")

        // the result continues the OPEN turn in-turn — no user frame, no close before it; with thinking off the turn then
        // closes and a new model turn opens (off the canonical template, which leaves the model nothing to continue and
        // an E-series model ends the turn at once)
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
        want3 |> push(trc)
        want3 |> push_from(encode(m, "\n", false, false))
        want3 |> push(trn)
        want3 |> push_from(encode(m, "model\n", false, false))
        t |> success(ids_equal(got3, want3), "result turn token-for-token (in-turn, named), then the closed and reopened model turn")
        let marked3 <- render_turn_marked(m, c2)
        let gen_prompt = 2l + long_length(encode(m, "\n", false, false)) + long_length(encode(m, "model\n", false, false))
        t |> equal(marked3.messages_end, long_length(want3) - gen_prompt, "the last message closes at the response block's end; the turn close and the new opening are the generation prompt")
        delete c2
        // a thinking conversation (decided at its first turn): the thought channel re-opens in the open turn after the
        // results, and the reply splits as one that begins inside it
        var c3 = create_chat_renderer(m, "", 8l)
        set_thinking(c3, true)
        add_user(c3, USR)
        var toks3 : array<int64>
        render_assistant_calls(m, c3, "", calls, toks3)
        add_tool_results(c3, results, names)
        let got4 <- render_turn(m, c3)
        var want4 : array<int64>
        want4 |> push(tro)
        want4 |> push_from(encode(m, "response:get_weather\{temp:21}", false, false))
        want4 |> push(trr)
        want4 |> push(special_id(m, "<|channel>"))
        want4 |> push_from(encode(m, "thought\n", false, false))
        t |> success(ids_equal(got4, want4), "a thinking turn re-opens the thought channel in the open turn ({length(got4)} vs {length(want4)} tokens)")
        let sp4 = split_reasoning(c3, "The result says 21 degrees.<channel|>It is 21 degrees in Paris.")
        t |> equal(sp4.reasoning, "The result says 21 degrees.", "the reply's head is the thought the prompt opened")
        t |> equal(sp4.content, "It is 21 degrees in Paris.", "... and the content follows the channel close")
        delete c3
        delete m
    }
    t |> run("displaced results + leaked open turn both heal on the next render (needs gemma-4-E2B)") @(t : T?) {
        let path = path_join(models_dir(), "gemma-4-E2B-it-Q4_K_M.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        let trn = special_id(m, "<|turn>")
        let trc = special_id(m, "<turn|>")
        let tro = special_id(m, "<|tool_response>")
        let trr = special_id(m, "<tool_response|>")

        // [user, asst(calls), tool, user]: the displaced results render FIRST (in the open turn),
        // then the turn closes, then the user turn — transcript order on the wire
        var c = create_chat_renderer(m, "", 8l)
        set_thinking(c, false)
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
        t |> success(ids_equal(got, want), "displaced results + close + user turn token-for-token")
        delete c

        // [user, asst(calls), user] with NO results: only the leaked close heals
        var c2 = create_chat_renderer(m, "", 8l)
        set_thinking(c2, false)
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
        t |> success(ids_equal(got2, want2), "the leaked open turn closes before the user turn")
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
        if (!model_available(t, path)) {
            return
        }
        with_dasllama_jobque() {
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
            c.n_audio_rows = 3l   // both modalities: the consume clears each independently
            c.audio_rows |> resize(12l)
            add_assistant(m, c, "One, two, three, four.")
            t |> equal(c.pending, "", "add_assistant() cleared the consumed text")
            t |> equal(c.n_image_rows, 0l, "add_assistant() cleared the queued image rows")
            t |> success(empty(c.image_rows), "the image row buffer went with the count")
            t |> equal(c.n_audio_rows, 0l, "add_assistant() cleared the queued audio rows")
            t |> success(empty(c.audio_rows), "the audio row buffer went with the count")
            delete c
            delete m
        }
    }
}

// ===== render seam: render_assistant produces EXACTLY the tokens add_assistant evals =====

[test]
def test_chat_render_seam(t : T?) {
    t |> run("render_assistant == add_assistant evals; renderer chat holds no KV (needs SmolLM2-135M)") @(t : T?) {
        return if (!jit_only(t))
        let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        with_dasllama_jobque() {
            var m <- load_capped(path)
            moe_gpu_drop_model()   // one lane: under DASLLAMA_GPU=1 the two paths would split between the device prefill and the CPU continuation
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
            t |> success(ids_equal(b.stop_ids, a.stop_ids), "renderer resolves the same stop ids")
            // eval the rendered stream on a fresh session: byte-equal caches ⟺ identical tokens
            var s <- create_session(m)
            eval(m, s, toks)
            //! the KV caches are codec-encoded byte blobs: byte equality IS bit-exactness for a same-dtype pair
            t |> success(sequence_equal(a.session.key_cache, s.key_cache), "key cache byte-equal: rendered == evaled")
            t |> success(sequence_equal(a.session.value_cache, s.value_cache), "value cache byte-equal: rendered == evaled")
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

[test]
def test_chat_inline_span(t : T?) {
    t |> run("an inline span renders the turn the splice pair renders, its position ids where the rows go - in a replayed turn too") @(t : T?) {
        let path = path_join(models_dir(), "voxtral-mini-3b-q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        let key = 0x1234abcdul
        for (before in [0, 4, length(USR)]) {
            var c <- create_chat_renderer_(m)
            add_user(c, USR)
            c.n_audio_rows = 3l
            var inscope head, tail : array<int64>
            render_turn_audio_(m, c, head, tail, before)
            var inscope ids <- [for (r in range64(3l)); media_position_id(key, r)]
            var inscope want := head
            want |> push_from(ids, tail)
            var ci <- create_chat_renderer_(m)
            add_user(ci, USR)
            add_user_span_(m, ci, key, 3l, before, false, false)
            var inscope got <- render_turn_(m, ci)
            t |> success(ids_equal(got, want), "{before}: the inline turn is head + ids + tail ({length(got)} vs {length(want)} positions)")
            // the same turn replayed as history: the span stays in the transcript, the reply and the close after it
            var inscope hist : array<int64>
            render_assistant_(m, ci, "ok", hist)
            t |> success(length(hist) > length(head) + 3 && ids_equal([for (k in range(length(head) + 3)); hist[k]], [for (k in range(length(head) + 3)); want[k]]),
                "{before}: a replayed turn keeps its span")
            t |> success(empty(ci.pending_spans), "{before}: the consumed turn leaves no span queued")
            delete ci
            delete c
        }
        t |> success(is_media_position(media_position_id(key, 0l)) && media_position_id(key, 0l) != media_position_id(key, 1l)
            && media_position_id(key, 0l) != media_position_id(key + 1ul, 0l), "a position id is no token id, and differs by row and by key")
        delete m
    }
}

//! whether `add_user_span_` refuses the span by panic, on a fresh chat that already holds the `queued` spans' text offsets
def private span_refused(m : Model; queued : array<int>; n_rows : int64; text_before : int; is_image, bare : bool) : bool {
    var c <- create_chat_renderer_(m)
    add_user(c, USR)
    for (at in queued) {
        add_user_span_(m, c, 0x77ul, 1l, at, false, true)
    }
    var refused = false
    try {
        add_user_span_(m, c, 0x77ul, n_rows, text_before, is_image, bare)
    } recover {
        refused = true
    }
    delete c
    return refused
}

// A template with no media markers takes a span only where the encoder's rows need none (a bare audio span); every span
// has rows, and spans queue in text order.
[test]
def test_chat_span_refusals(t : T?) {
    t |> run("add_user_span_ refuses a span the template cannot mark, an empty span, and one out of text order") @(t : T?) {
        let path = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        var none : array<int>
        t |> success(!span_refused(m, none, 2l, 4, false, true), "a bare audio span needs no markers (the control)")
        t |> success(span_refused(m, none, 0l, 4, false, true), "a span of no rows panics")
        t |> success(span_refused(m, none, -1l, 4, false, true), "a span of negative rows panics")
        t |> success(span_refused(m, none, 2l, 4, true, false), "an image span on a template with no image markers panics")
        t |> success(span_refused(m, none, 2l, 4, false, false), "a marked audio span on a template with no audio markers panics")
        t |> success(!span_refused(m, [4], 2l, 4, false, true), "a span at the offset of the one before it queues")
        t |> success(span_refused(m, [4], 2l, 2, false, true), "a span ahead of the one queued before it panics")
        delete none
        delete m
    }
}

// A gemma-4 template's non-thinking generation prompt is read off the model's own template: the larger models close
// an empty thought channel after the model turn's opener, the E-series opens the turn bare.
[test]
def test_gemma4_instruct_opener(t : T?) {
    t |> run("the non-thinking opener follows the template: a closed thought where it writes one, bare where it does not") @(t : T?) {
        let cases = [
            (name = "the 12B form", parts = 5,
             src = "\{\{- '<|turn>model\\n' -}}\{%- if not enable_thinking -%}\{\{- '<|channel>thought\\n<channel|>' -}}\{%- endif -%}"),
            (name = "the E-series form", parts = 2,
             src = "\{\{- '<|turn>model\\n' -}}\{%- elif enable_thinking -%}\{\{- '<|channel>thought\\n' -}}\{%- endif -%}")
        ]
        for (c in cases) {
            var m = Model()
            m.chat_template_str = c.src
            var inscope tmpl <- chat_template(m)
            t |> equal(length(tmpl.assistant_open.parts), c.parts, c.name)
            t |> equal(length(tmpl.assistant_open_think.parts), 2, "{c.name}: the thinking opener is the bare model turn")
            delete m
        }
    }
}

// A Llama-3.1+ template's system header is read off the template: the cutoff line, and the date line's date where the
// template fixes one; a template with a clock states the day, and one with no header (Llama-3.0) states nothing.
[test]
def test_llama_system_header(t : T?) {
    t |> run("the dated system header follows the template") @(t : T?) {
        let head = "\{\{- \"<|start_header_id|>system<|end_header_id|>\\n\\n\" }}\{\{- \"Cutting Knowledge Date: December 2023\\n\" }}\{\{- \"Today Date: \" + date_string + \"\\n\\n\" }}"
        let cases = [
            (name = "a fixed date (3.1)", dated = true, date = "26 Jul 2024",
             src = "\{%- set date_string = \"26 Jul 2024\" %}{head}"),
            (name = "a clock (3.2)", dated = true, date = "",
             src = "\{%- if strftime_now is defined %}\{%- set date_string = strftime_now(\"%d %b %Y\") %}\{%- else %}\{%- set date_string = \"26 Jul 2024\" %}\{%- endif %}{head}"),
            (name = "no header (3.0)", dated = false, date = "",
             src = "\{\{- '<|start_header_id|>' + message['role'] + '<|end_header_id|>\\n\\n' }}")
        ]
        for (c in cases) {
            var m = Model()
            m.chat_template_str = c.src
            var inscope tmpl <- chat_template(m)
            t |> equal(tmpl.system_dated, c.dated, c.name)
            t |> equal(tmpl.system_date, c.date, "{c.name}: the date the template fixes")
            t |> equal(tmpl.system_lead, c.dated ? "Cutting Knowledge Date: December 2023\n" : "", "{c.name}: the cutoff line")
            delete m
        }
    }
    t |> run("the rendered header: a tool-carrying turn opens with the environment line, and a pinned date wins over the template's") @(t : T?) {
        let path = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        set_chat_date("")
        let plain = system_header_text(m, "01 Jan 2030", false)
        let tooled = system_header_text(m, "01 Jan 2030", true)
        set_chat_date("26 Jul 2024")
        let pinned = system_header_text(m, "01 Jan 2030", false)
        set_chat_date("")
        to_log(LOG_INFO, "llama header, the template's date: '{plain}'\nwith a tool: '{tooled}'\nthe date pinned: '{pinned}'\n")
        t |> success(find(plain, "\\n\\nCutting Knowledge Date: December 2023\\nToday Date: 01 Jan 2030\\n\\n") >= 0, "unpinned, the turn states the date the template fixes")
        t |> success(find(plain, "Environment: ipython") < 0, "a turn with no tools states no environment line")
        t |> success(find(tooled, "\\n\\nEnvironment: ipython\\nCutting Knowledge Date: December 2023\\nToday Date: 01 Jan 2030\\n\\n") >= 0, "a tool-carrying turn opens with the environment line, ahead of the cutoff")
        t |> success(find(pinned, "Today Date: 26 Jul 2024\\n") >= 0 && find(pinned, "2030") < 0, "a pinned date wins over the template's own")
        delete m
    }
}

//! the first turn's detokenized render on a Llama-3.2 chat whose template fixes `date` (as 3.1's does), with a tool or none
def private system_header_text(m : Model; date : string; with_tool : bool) : string {
    var c <- create_chat_renderer_(m)
    c.tmpl.system_date = date
    if (with_tool) {
        var tools <- ["\{\"type\": \"function\", \"function\": \{\"name\": \"get_time\", \"parameters\": \{\"type\": \"object\", \"properties\": \{}}}}"]
        set_tools_(c, tools)
        delete tools
    }
    add_user(c, USR)
    var inscope toks <- render_turn_(m, c)
    delete c
    return detok(m, toks)
}

// The system text a ChatML template states for a conversation with no system message, read off the three shapes the
// stocked templates take; a span that carries an expression is the caller's own system turn and yields none.
[test]
def test_chatml_default_system(t : T?) {
    t |> run("the default system text is the template's literal, never one of its expressions") @(t : T?) {
        let cases = [
            (name = "template text (Qwen2.5-Omni)", want = "You are a helpful assistant.",
             src = "\{% if loop.first and message['role'] != 'system' %}<|im_start|>system\nYou are a helpful assistant.<|im_end|>\n\{% endif %}<|im_start|>\{\{ message['role'] }}"),
            (name = "a string literal (Qwen2.5)", want = "You are Qwen, created by Alibaba Cloud. You are a helpful assistant.",
             src = "\{\{- '<|im_start|>system\\n' + messages[0]['content'] + '<|im_end|>\\n' }}\{%- else %}\{\{- '<|im_start|>system\\nYou are Qwen, created by Alibaba Cloud. You are a helpful assistant.<|im_end|>\\n' }}"),
            (name = "an expression alone (Qwen3.5)", want = "",
             src = "\{\{- '<|im_start|>system\\n' }}\{\{- \"# Tools\" }}<|im_end|>\{\{- '<|im_start|>system\\n' + content + '<|im_end|>\\n' }}"),
            (name = "no system literal at all", want = "",
             src = "\{\{- '<|im_start|>' + message.role + '\\n' + content + '<|im_end|>' }}")
        ]
        for (c in cases) {
            var m = Model()
            m.chat_template_str = c.src
            var inscope tmpl <- chat_template(m)
            t |> equal(tmpl.default_system, c.want, c.name)
            delete m
        }
    }
}

//! one audio turn rendered with `before` bytes of the user text ahead of the splice: the head closes with the opening marker
//! and carries exactly that text, the tail the rest
def private audio_splice_cell(t : T?; m : Model; before : int) {
    var c <- create_chat_renderer_(m)
    add_user(c, USR)
    c.n_audio_rows = 3l
    var inscope head, tail : array<int64>
    render_prompt_audio(m, c, head, tail, before)
    let h = detok(m, head)
    let tl = detok(m, tail)
    let lead = slice(USR, 0, max(before, 4))
    let rest = slice(USR, before)
    to_log(LOG_INFO, "voxtral audio at {before}: '{h}' [audio x3] '{tl}'\n")
    t |> equal(head[length(head) - 1], special_id(m, "[BEGIN_AUDIO]"), "{before}: the head still closes with the opening marker")
    t |> success(before == 0 ? find(h, lead) < 0 : find(h, lead) >= 0, "{before}: the head carries exactly the text ahead of the audio")
    t |> success(before == length(USR) ? find(tl, "time") < 0 : find(tl, rest) >= 0, "{before}: the tail carries the text after it")
    delete c
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
        var head, tail : array<int64>
        render_prompt_audio(m, c, head, tail)
        to_log(LOG_INFO, "voxtral audio-turn eyeball: '{detok(m, head)}' [audio x3] '{detok(m, tail)}'\n")
        let ba = special_id(m, "[BEGIN_AUDIO]")
        t |> success(ba >= 0l, "[BEGIN_AUDIO] resolves in the vocab")
        t |> equal(head[length(head) - 1], ba, "head closes with the opening audio marker")
        t |> success(!empty(tail) && tail[0] != ba, "no closing marker - the tail starts with text")
        delete c
        delete m
    }
    t |> run("the audio splices where the message put it: ahead of the text, after it, inside it") @(t : T?) {
        let path = path_join(models_dir(), "voxtral-mini-3b-q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        for (before in [0, 4, length(USR)]) {
            audio_splice_cell(t, m, before)
        }
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
        var head, tail : array<int64>
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
