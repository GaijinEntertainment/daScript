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
require dasllama/dasllama_bpe
require dasllama/dasllama_image   // load_model_uncached: the product's load with no image
require daslib/jobque_boost
require daslib/fio
require math
require _model_tier   // model_available: presence + the >6 GiB PARITY_FULL tier gate
require _cpu_gpu_model   // with_cpu_and_gpu_model, cpu_cells_form: the one-load blocks
require _chat_cells   // the cells per (model, form), CHAT_SYS / CHAT_USR, chat_engine_prefill, chat_detok

// The unified chat engine must render each arch's turn EXACTLY as the old per-model REPLs hand-rolled
// it — same special-token ids, same text segmentation — so collapsing the 5 REPLs into one changes no
// tokens. Each case rebuilds the old demo's first-turn prefill against the RAW tokenizer backend and
// compares it to render_turn(). GGUF models are gitignored, so each case skips cleanly when absent.

// Load capped to a tiny context: these cases only render prompts, and the session's KV cache is sized
// to config.seq_len — Phi/Llama-3's native 131072 would allocate ~64GB.
def private load_capped(path : string) : Model {
    var m <- load_model_uncached(path, QuantMode.q8)
    m.config.seq_len = min(m.config.seq_len, 512l)
    return <- m
}

[test(description = "The Qwen2.5 prefill equals the hand-built ChatML sequence token for token, and the marked render reports the system turn as the first turn opening and none on a later turn")]
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
        want |> push_from(bpe_encode(raw, "system\n{CHAT_SYS}"))
        want |> push(ime)
        want |> push_from(bpe_encode(raw, "\n"))
        want |> push(ims)
        want |> push_from(bpe_encode(raw, "user\n{CHAT_USR}"))
        want |> push(ime)
        want |> push_from(bpe_encode(raw, "\n"))
        want |> push(ims)
        want |> push_from(bpe_encode(raw, "assistant\n"))
        t |> success(ids_equal(chat_engine_prefill(m, CHAT_SYS, CHAT_USR), want), "qwen chatml prefill token-for-token")
        // the marked render: a first turn's opening is its system turn, a later turn has none
        var c = create_chat(m, CHAT_SYS)
        add_user(c, CHAT_USR)
        var marked <- render_turn_marked(m, c)
        var inscope sys_toks <- bpe_encode(raw, "system\n{CHAT_SYS}")
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

[test(description = "A template with parameter blocks sniffs to the Qwen3.6 xml_function tool format, and on Qwen3.5 its first turn, call replay, results, thinking replay and back-to-back user turns render the template exact text")]
def test_chat_xml_function(t : T?) {
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
}

// ===== qwen35 Hermes tools (the qwen2/qwen3 block, kept by the 3.5 template) =====

[test(description = "The qwen35 registration declares the Hermes tool markers and prelude, and on Qwen3.5-0.8B the system tool block renders those markers as single vocab ids")]
def test_chat_qwen35_tools(t : T?) {
    t |> run("the qwen35 registration declares the Hermes tool format (model-free)") @(t : T?) {
        var ct <- arch_chat("qwen35")
        t |> equal(ct.tool_call_open, "<tool_call>")
        t |> equal(ct.tool_resp_open, "<tool_response>")
        t |> success(!empty(ct.tool_sys_prelude), "tool block prelude declared")
        delete ct
    }
}

// ===== ToolMode declarations + the gemma-4 tool wire (the tool-formats arc) =====

[test(description = "Each family declares its ToolMode by registration or template detection, harmony reasoning level follows an explicit effort else the thinking flag, and a mode missing its control specials demotes to none")]
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

[test(description = "Template fragments sniff to each family distinctive user-turn special regardless of arch, INST-family templates close the assistant turn with EOS, and a template-less GGUF falls back by arch and tokenizer")]
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

[test(description = "strip_think removes a leading think block, whitespace before it included, empties an unclosed one, and passes through replies with no leading block or a mid-reply think tag")]
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

//! On voxtral an inline audio span renders as the splice head, the span position ids and the tail at any text offset, keeps its span in a replayed turn, and position ids differ by row and key
def test_chat_inline_span(t : T?; m : Model) {
    unsafe {
        t |> run("an inline span renders the turn the splice pair renders, its position ids where the rows go - in a replayed turn too") @capture(& m)(t : T ?) {
            let key = 0x1234abcdul
            for (before in [0, 4, length(CHAT_USR)]) {
                var c <- create_chat_renderer_(m)
                add_user(c, CHAT_USR)
                c.n_audio_rows = 3l
                var inscope head, tail : array<int64>
                render_turn_audio_(m, c, head, tail, before)
                var inscope ids <- [for (r in range64(3l)); media_position_id(key, r)]
                var inscope want := head
                want |> push_from(ids, tail)
                var ci <- create_chat_renderer_(m)
                add_user(ci, CHAT_USR)
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
        }
    }
}

// A gemma-4 template's non-thinking generation prompt is read off the model's own template: the larger models close
// an empty thought channel after the model turn's opener, the E-series opens the turn bare.
[test(description = "A gemma-4 template non-thinking opener follows the template: the 12B form closes an empty thought channel after the model turn, the E-series form opens bare, and both thinking openers are bare")]
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
[test(description = "The Llama-3 system header is read off the template (fixed date, clock or none), and on Llama-3.2 a tool turn adds the environment line before the cutoff while a pinned date overrides the template")]
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
}

// The system text a ChatML template states for a conversation with no system message, read off the three shapes the
// stocked templates take; a span that carries an expression is the caller's own system turn and yields none.
[test(description = "A ChatML template default system text is its literal system string, from template text or a string literal, and empty where the system span is only an expression or absent")]
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
    add_user(c, CHAT_USR)
    c.n_audio_rows = 3l
    var inscope head, tail : array<int64>
    render_prompt_audio(m, c, head, tail, before)
    let h = chat_detok(m, head)
    let tl = chat_detok(m, tail)
    let lead = slice(CHAT_USR, 0, max(before, 4))
    let rest = slice(CHAT_USR, before)
    to_log(LOG_INFO, "voxtral audio at {before}: '{h}' [audio x3] '{tl}'\n")
    t |> equal(head[length(head) - 1], special_id(m, "[BEGIN_AUDIO]"), "{before}: the head still closes with the opening marker")
    t |> success(before == 0 ? find(h, lead) < 0 : find(h, lead) >= 0, "{before}: the head carries exactly the text ahead of the audio")
    t |> success(before == length(CHAT_USR) ? find(tl, "time") < 0 : find(tl, rest) >= 0, "{before}: the tail carries the text after it")
    delete c
}

// The chat-audio marker gate: render_prompt_audio must bracket the soft-token splice in the
// family's audio markers — the render-side half of the chat-audio rail (encoder numerics are
// test_audio's; nothing else pins these template tags against a real vocab).
[test(description = "render_prompt_audio ends the voxtral head on BEGIN_AUDIO with no closing marker and splices at the asked text offset, and brackets qwen2audio rows in audio_bos and audio_eos")]
def test_chat_audio_markers(t : T?) {
    t |> run("qwen2audio audio turn is bracketed by <|audio_bos|>/<|audio_eos|>") @(t : T?) {
        let path = path_join(models_dir(), "qwen2audio-7b-q8_0.gguf")
        if (!model_available(t, path)) {
            return
        }
        var m <- load_capped(path)
        var c <- create_chat_renderer_(m)
        add_user(c, CHAT_USR)
        c.n_audio_rows = 3l
        var head, tail : array<int64>
        render_prompt_audio(m, c, head, tail)
        to_log(LOG_INFO, "qwen2audio audio-turn eyeball: '{chat_detok(m, head)}' [audio x3] '{chat_detok(m, tail)}'\n")
        let bos = special_id(m, "<|audio_bos|>")
        let eos = special_id(m, "<|audio_eos|>")
        t |> success(bos >= 0l && eos >= 0l, "audio markers resolve in the vocab")
        t |> equal(head[length(head) - 1], bos, "head closes with <|audio_bos|>")
        t |> equal(tail[0], eos, "tail opens with <|audio_eos|>")
        delete c
        delete m
    }
}

//! render_prompt_audio ends the voxtral head on BEGIN_AUDIO with no closing marker and splices at the asked text offset, and brackets qwen2audio rows in audio_bos and audio_eos
def test_chat_audio_markers_voxtral(t : T?; m : Model) {
    unsafe {
        t |> run("voxtral audio turn opens with [BEGIN_AUDIO] (no closing marker - mtmd shape)") @capture(& m)(t : T ?) {
            var c <- create_chat_renderer_(m)
            add_user(c, CHAT_USR)
            c.n_audio_rows = 3l   // planted span: render only brackets it, the splice happens at eval
            var head, tail : array<int64>
            render_prompt_audio(m, c, head, tail)
            to_log(LOG_INFO, "voxtral audio-turn eyeball: '{chat_detok(m, head)}' [audio x3] '{chat_detok(m, tail)}'\n")
            let ba = special_id(m, "[BEGIN_AUDIO]")
            t |> success(ba >= 0l, "[BEGIN_AUDIO] resolves in the vocab")
            t |> equal(head[length(head) - 1], ba, "head closes with the opening audio marker")
            t |> success(!empty(tail) && tail[0] != ba, "no closing marker - the tail starts with text")
            delete c
        }
    }
    unsafe {
        t |> run("the audio splices where the message put it: ahead of the text, after it, inside it") @capture(& m)(t : T ?) {
            for (before in [0, 4, length(CHAT_USR)]) {
                audio_splice_cell(t, m, before)
            }
        }
    }
}

[test(description = "Voxtral-mini-3B on one load renders the inline audio span and the BEGIN_AUDIO head spliced at the asked offset")]
def test_chat_voxtral_cells(t : T?) {
    return if (!family_on(t, "voxtral"))
    with_cpu_and_gpu_model(t, "llm", "voxtral-mini-3b-q8_0.gguf", 512l, [cpu_cells_form(uncached = true)]) $(m, _form) {
        test_chat_inline_span(t, m)
        test_chat_audio_markers_voxtral(t, m)
    }
}
