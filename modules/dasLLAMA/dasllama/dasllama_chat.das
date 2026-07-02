options gen2

module dasllama_chat shared public

require dasllama/dasllama_transformer   // nolint:STYLE029 — umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_tokenizer  // TokKind — disambiguate the "llama" chat template by backend
require daslib/strings_boost  // join() to assemble a segment / the streamed reply in O(n)

// The chat layer (layer 2). One engine drives every model: create_chat resolves the arch's chat
// template (from the transformer's registry), add_user queues a message, respond renders the turn,
// prefills it, streams the reply, and keeps the whole conversation in the session's KV cache. The
// per-model chat format is data — a ChatTemplate part list — so there is no per-model code here.

//! Who authored a message.
enum Role {
    system
    user
    assistant
    tool
}

//! One conversation turn.
struct Message {
    role : Role = Role.user
    content : string = ""
}

//! A chat conversation over a model: the KV-cache session, the resolved chat template, and the
//! running transcript. A model serves many independent ChatSessions.
struct ChatSession {
    session : Session = Session()
    tmpl : ChatTemplate = ChatTemplate()
    close_parts : array<ChatPart>   // how a turn terminates (the user template after its content slot)
    stop_ids : array<int64>         // resolved stop-token ids
    system : string = ""
    pending : string = ""           // the user text awaiting the next respond()
    has_pending : bool = false
    first : bool = true             // first turn emits add_bos + the system turn
    max_new : int64 = 256l          // per-reply token budget
    history : array<Message>
}

// ===== Chat-template resolution =====

// The Zephyr / TinyLlama-chat template. Its <|system|>/<|user|>/<|assistant|> markers are plain
// SentencePiece text (not vocab tokens — only </s> is a real id), so they render as text segments.
def private zephyr_chat_template() : ChatTemplate {
    var ct = ChatTemplate()
    ct.add_bos = true   // <s>
    ct.system.parts <- [chat_text("<|system|>\n"), chat_content(), chat_special("</s>")]
    ct.user.parts <- [chat_text("\n<|user|>\n"), chat_content(), chat_special("</s>")]
    ct.assistant_open.parts <- [chat_text("\n<|assistant|>\n")]
    ct.stop <- ["</s>"]
    return <- ct
}

// The Mistral-Instruct template (v0.3). [INST] / [/INST] are control tokens in the v0.3 vocab, so
// they render as specials. No system role — the renderer folds it into the first user turn.
def private mistral_instruct_template() : ChatTemplate {
    var ct = ChatTemplate()
    ct.add_bos = true   // <s>
    ct.user.parts <- [chat_special("[INST]"), chat_text(" "), chat_content(), chat_text(" "), chat_special("[/INST]")]
    ct.stop <- ["</s>"]
    return <- ct
}

// Pick a named template by sniffing the GGUF's embedded Jinja source for each family's distinctive
// marker — detection only, the Jinja is never executed. This is what resolves fine-tunes and the
// arch-name collisions (a Mistral GGUF is arch "llama" but spells [INST]). Checked most-specific
// first: Phi-3's source spells both <|user|> and <|end|>, Zephyr's only <|user|>.
def private detect_chat_template(m : Model; var out : ChatTemplate) : bool {
    let src = m.chat_template_str
    if (empty(src)) {
        return false
    }
    if (contains(src, "<|start_header_id|>")) {   // Llama-3 instruct
        out <- arch_chat("llama")
    } elif (contains(src, "<|im_start|>")) {      // ChatML (Qwen family, TinyLlama v0.3, ...)
        out <- arch_chat("qwen2")
    } elif (contains(src, "<start_of_turn>")) {   // Gemma
        out <- arch_chat("gemma2")
    } elif (contains(src, "<|end|>")) {           // Phi-3
        out <- arch_chat("phi3")
    } elif (contains(src, "[INST]")) {            // Mistral instruct
        out <- mistral_instruct_template()
    } elif (contains(src, "<|user|>")) {          // Zephyr / TinyLlama-chat
        out <- zephyr_chat_template()
    } else {
        return false
    }
    return true
}

//! The chat template for a loaded model: sniff the GGUF's embedded `tokenizer.chat_template` first
//! (see detect_chat_template). When the GGUF carries none, fall back to the arch heuristic — GGUF
//! general.architecture is "llama" for BOTH Llama-3 (byte-level BPE) and Llama-2/TinyLlama
//! (SentencePiece), so disambiguate by tokenizer backend: BPE => the registry's Llama-3 instruct
//! template, SPM => Zephyr.
def chat_template(m : Model) : ChatTemplate {
    var det = ChatTemplate()
    if (detect_chat_template(m, det)) {
        return <- det
    }
    if (m.arch == "llama" && m.tok.kind == TokKind.spm) {
        return <- zephyr_chat_template()
    }
    return <- arch_chat(m.arch)
}

// ===== Renderer: ChatTemplate part list -> token ids =====

// Encode each maximal run of text/content between special tokens as ONE segment — matches how
// llama.cpp tokenizes a templated prompt (BPE/SPM merges do not cross special-token boundaries).
def private render_parts(m : Model; parts : array<ChatPart>; content : string; var out : array<int64>) {
    var seg : array<string>   // the current text run, flushed (encoded as one segment) at each special
    for (p in parts) {
        if (p.kind == ChatPartKind.special) {
            if (!empty(seg)) {
                out |> push_from(encode(m, join(seg, ""), false, false))
                seg |> clear()
            }
            let id = special_id(m, p.s)
            if (id < 0l) {
                panic("dasLLAMA chat: template special token '{p.s}' not in the model vocab")
            }
            out |> push(id)
        } elif (p.kind == ChatPartKind.content) {
            seg |> push(content)
        } else {
            seg |> push(p.s)
        }
    }
    if (!empty(seg)) {
        out |> push_from(encode(m, join(seg, ""), false, false))
    }
}

// Render this turn's prefill: (first turn) BOS + system, then the user turn and the generation prompt.
def private render_prompt(m : Model; c : ChatSession; var out : array<int64>) {
    if (c.first && c.tmpl.add_bos) {
        out |> push(bos_id(m))
    }
    var user_content = c.pending
    if (c.first && !empty(c.system)) {
        if (!empty(c.tmpl.system.parts)) {
            render_parts(m, c.tmpl.system.parts, c.system, out)
        } else {
            user_content = "{c.system}\n\n{c.pending}"   // no system role (Gemma): fold into the first user turn
        }
    }
    render_parts(m, c.tmpl.user.parts, user_content, out)
    render_parts(m, c.tmpl.assistant_open.parts, "", out)
}

// ===== Public API =====

//! Start a conversation over `model`. `system` is the system prompt (empty = none); `max_new` caps
//! each reply. A model can drive many independent ChatSessions.
def create_chat(model : Model; system : string = ""; max_new : int64 = 256l) : ChatSession {
    var c = ChatSession(session <- create_session(model), tmpl <- chat_template(model),
                        system = system, max_new = max_new)
    for (sp in c.tmpl.stop) {
        let id = special_id(model, sp)
        if (id >= 0l) {
            c.stop_ids |> push(id)
        }
    }
    // the assistant turn terminates like a user turn: the user-template parts after the content slot
    var seen = false
    for (p in c.tmpl.user.parts) {
        if (p.kind == ChatPartKind.content) {
            seen = true
        } elif (seen) {
            c.close_parts |> push(p)
        }
    }
    return <- c
}

//! Render the next turn's prefill token ids — BOS + system on the first turn, then the user turn and
//! the generation prompt — WITHOUT running the model. For inspection, token counting, and tests.
def render_turn(m : Model; c : ChatSession) : array<int64> {
    var out : array<int64>
    render_prompt(m, c, out)
    return <- out
}

//! Queue a user message for the next respond().
def add_user(var c : ChatSession; text : string) {
    c.pending = text
    c.has_pending = true
    c.history |> push(Message(role = Role.user, content = text))
}

//! `reply` with a leading `<think>…</think>` reasoning block removed (the Qwen3 thinking format) —
//! the reasoning of prior turns is excluded from chat history per the Qwen3 protocol. Replies that
//! don't open with `<think>` pass through unchanged; an unterminated block (the token budget ran out
//! mid-think) strips to empty, since the reasoning never reached an answer.
def strip_think(reply : string) : string {
    let open = find(reply, "<think>")
    if (open < 0 || !is_null_or_whitespace(slice(reply, 0, open))) {
        return reply
    }
    let close = find(reply, "</think>")
    return close < 0 ? "" : strip(slice(reply, close + length("</think>"), length(reply)))
}

//! Generate the assistant's reply to the queued user message: render the turn, prefill it, then
//! stream tokens through `blk(piece)` (return false to stop early) until a stop token or the budget.
//! Terminates the turn in the KV cache and appends it to the transcript (with any `<think>` block
//! stripped — see strip_think). Returns the full reply text, thinking included.
//! Timing is available afterwards via stats(c.session).
def respond(m : Model; var c : ChatSession; params : SamplingParams;
            blk : block<(piece : string) : bool>) : string {
    if (!c.has_pending) {
        return ""
    }
    var pre : array<int64>
    render_prompt(m, c, pre)
    c.first = false
    c.has_pending = false
    // local clone so the streaming callback captures no part of `c` (c.session is passed to generate)
    var stops := c.stop_ids
    var reply_parts : array<string>
    generate(m, c.session, pre, params, c.max_new) $(id, piece) {
        for (sid in stops) {
            return false if (id == sid)
        }
        reply_parts |> push(piece)
        return invoke(blk, piece)
    }
    // close the assistant turn so the next turn continues cleanly
    var close : array<int64>
    render_parts(m, c.close_parts, "", close)
    eval(m, c.session, close)
    let reply = join(reply_parts, "")
    c.history |> push(Message(role = Role.assistant, content = strip_think(reply)))
    return reply
}
