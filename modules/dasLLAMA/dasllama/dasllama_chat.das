options gen2
options _comment_hygiene = true

module dasllama_chat shared public

require dasllama/dasllama_transformer   // the umbrella fires each arch [init] registration; requiring dasllama_common directly would drop them
require dasllama/dasllama_tokenizer  // TokKind — disambiguate the "llama" chat template by backend
require dasllama/dasllama_audio      // AudioTower + encoder: chat sessions can carry an audio tower
require daslib/strings_boost  // join() to assemble a segment / the streamed reply in O(n)
require math   // min/max in the reply-side reasoning matcher

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
//! running transcript. A session created with an audio tower (``create_chat(model, tower)``) also
//! owns the tower + encoder scratch and can take ``add_user_audio``.
struct ChatSession {
    session : Session = Session()
    tmpl : ChatTemplate = ChatTemplate()
    close_parts : array<ChatPart>   // how a turn terminates (the user template after its content slot)
    stop_ids : array<int64>         // resolved stop-token ids
    stop_ids_nothink : array<int64> // extra stops in force only while thinking is OFF (tmpl.stop_nothink)
    system : string = ""
    tools_json : array<string>      // verbatim tool JSONs; non-empty => the system turn carries the family's tool block
    pending : string = ""           // the user text awaiting the next respond()
    has_pending : bool = false
    pending_is_tool : bool = false  // pending is a tool-results block: marker-split at render
    first : bool = true             // first turn emits add_bos + the system turn
    max_new : int64 = 256l          // per-reply token budget
    enable_thinking : bool = true   // init from tmpl.think_default; off + a think_suppress form => instruct mode
    think_vocab : bool = false      // the vocab carries the reply-side reasoning markers (resolved at create)
    gate_ok : bool = false          // the think_gate specials resolve in the vocab (true when no gate declared)
    gate_rendered : bool = false    // a consumed turn actually rendered the gate — thinking is armed for the session
    history : array<Message>
    tower : AudioTower = AudioTower()     // d_model == 0 => text-only chat
    enc : EncoderState = EncoderState()   // encoder scratch (sized once at create)
    audio_rows : array<float>             // encoded soft tokens awaiting the next respond()
    n_audio_rows : int64                  // row count in audio_rows (each tower.proj_dim wide)
}

// Stated rather than inferred: the tower holds borrowed planes, and a generated teardown
// would force every module that deletes a ChatSession to see the plane finalizers too.
// Each field's own finalizer does the real work — this only names them in one place.
def finalize(var c : ChatSession) {
    delete c.session
    delete c.tmpl
    delete c.close_parts
    delete c.stop_ids
    delete c.stop_ids_nothink
    delete c.tools_json
    delete c.history
    delete c.tower
    delete c.enc
    delete c.audio_rows
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
    ct.assistant_close.parts <- [chat_special("</s>")]   // [INST] wraps USER turns; assistant turns end with EOS
    ct.stop <- ["</s>"]
    return <- ct
}

// The Mistral v7-tekken template (Voxtral, Mistral-Small-3.x): [INST]content[/INST] with NO
// spacing, [SYSTEM_PROMPT]…[/SYSTEM_PROMPT] system role. Detected BY NAME ("mistral-v7-tekken"),
// not Jinja. Audio spans open with [BEGIN_AUDIO] and have no closing marker (mtmd voxtral shape).
def private mistral_v7_tekken_template() : ChatTemplate {
    var ct = ChatTemplate()
    ct.add_bos = true   // <s>
    ct.system.parts <- [chat_special("[SYSTEM_PROMPT]"), chat_content(), chat_special("[/SYSTEM_PROMPT]")]
    ct.user.parts <- [chat_special("[INST]"), chat_content(), chat_special("[/INST]")]
    ct.assistant_close.parts <- [chat_special("</s>")]   // assistant turns close with EOS, not [/INST]
    ct.audio_pre.parts <- [chat_special("[BEGIN_AUDIO]")]
    ct.stop <- ["</s>"]
    return <- ct
}

// Name a template kind by sniffing the GGUF's embedded Jinja source for each family's distinctive
// marker — detection only, the Jinja is never executed. Resolves fine-tunes and arch-name
// collisions (a Mistral GGUF is arch "llama" but spells [INST]); checked most-specific first.
def private detect_chat_kind(src : string) : string {
    return "llama" if (contains(src, "<|start_header_id|>"))            // Llama-3 instruct
    return "gpt-oss" if (contains(src, "<|channel|>"))                  // gpt-oss Harmony channel format
    return "qwen2" if (contains(src, "<|im_start|>"))                   // ChatML (Qwen family, TinyLlama v0.3, ...)
    return "gemma4" if (contains(src, "<|turn>"))                       // Gemma-4 channel-based turn format
    return "gemma2" if (contains(src, "<start_of_turn>"))               // Gemma (2 and 3 share the turn shape)
    return "phi3" if (contains(src, "<|end|>"))                         // Phi-3
    // v7-tekken BY NAME (Voxtral stores the llama.cpp alias, not Jinja), or by SHAPE (Medium 3.5)
    return "mistral-v7-tekken" if (contains(src, "v7-tekken") || contains(src, "[SYSTEM_PROMPT]"))
    return "mistral-instruct" if (contains(src, "[INST]"))              // Mistral instruct
    return "glm4moe" if (contains(src, "[gMASK]<sop>"))                 // GLM-4.5 family (spells <|user|> too — before Zephyr)
    return "zephyr" if (contains(src, "<|user|>"))                      // Zephyr / TinyLlama-chat
    return ""
}

def private chat_template_of_kind(kind : string) : ChatTemplate {
    return <- mistral_v7_tekken_template() if (kind == "mistral-v7-tekken")
    return <- mistral_instruct_template() if (kind == "mistral-instruct")
    return <- zephyr_chat_template() if (kind == "zephyr")
    return <- arch_chat(kind)
}

//! The chat template for a loaded model: sniff the GGUF's embedded `tokenizer.chat_template`
//! first, falling back to the arch heuristic. GGUF general.architecture is "llama" for both
//! Llama-3 (BPE) and Llama-2 (SentencePiece), so the fallback disambiguates by tokenizer backend.
def chat_template(m : Model) : ChatTemplate {
    let kind = detect_chat_kind(m.chat_template_str)
    if (!empty(kind)) {
        return <- chat_template_of_kind(kind)
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
                out |> push_from(encode_(m, join(seg, ""), false, false))
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
        out |> push_from(encode_(m, join(seg, ""), false, false))
    }
}

// Split `s` into template parts at every tool marker that resolves in the vocab — HF added-token
// splitting: vocab markers tokenize as their ids, the prose around them as text (encode() does not
// match added tokens inline). Markers absent from the vocab stay in the text run.
def private split_on_tool_markers(m : Model; c : ChatSession; s : string; var parts : array<ChatPart>) {
    let markers = fixed_array(c.tmpl.tool_call_open, c.tmpl.tool_call_close,
                              c.tmpl.tool_resp_open, c.tmpl.tool_resp_close)
    var pos = 0
    let n = length(s)
    while (pos < n) {
        var best = -1
        var best_mark = ""
        for (mk in markers) {
            if (!empty(mk) && special_id(m, mk) >= 0l) {
                let f = find(s, mk, pos)
                if (f >= 0 && (best < 0 || f < best)) {
                    best = f
                    best_mark = mk
                }
            }
        }
        if (best < 0) {
            break
        }
        if (best > pos) {
            parts |> push(chat_text(slice(s, pos, best)))
        }
        parts |> push(chat_special(best_mark))
        pos = best + length(best_mark)
    }
    if (pos < n) {
        parts |> push(chat_text(slice(s, pos, n)))
    }
}

// Render a turn with its content slot replaced by pre-split parts.
def private render_parts_spliced(m : Model; turn_parts : array<ChatPart>; spliced : array<ChatPart>; var out : array<int64>) {
    var parts : array<ChatPart>
    parts |> reserve(length(turn_parts) + length(spliced))
    for (p in turn_parts) {
        if (p.kind == ChatPartKind.content) {
            parts |> push_from(spliced)
        } else {
            parts |> push(p)
        }
    }
    render_parts(m, parts, "", out)
    delete parts
}

// The vocab carries the reply-side reasoning markers — resolved once at create. A thinking
// declaration on a vocab without the markers (qwen2/2.5 share chatml_chat) stays INERT: the
// reply matcher must not arm, or a literal "<think>" echo reclassifies a non-thinking reply.
def private think_vocab_ok(m : Model; tmpl : ChatTemplate) : bool {
    if (tmpl.think_mode == ThinkMode.none) {
        return false
    }
    if (tmpl.think_mode == ThinkMode.channel_switch) {
        return special_id(m, HARMONY_CH) >= 0l
    }
    if (tmpl.think_mode == ThinkMode.symmetric) {
        return special_id(m, tmpl.think_open) >= 0l && special_id(m, tmpl.think_close) >= 0l
    }
    return special_id(m, tmpl.think_close) >= 0l   // asymmetric: the closer special fingerprints the family
}

def private gate_specials_ok(m : Model; tmpl : ChatTemplate) : bool {
    for (p in tmpl.think_gate.parts) {
        if (p.kind == ChatPartKind.special && special_id(m, p.s) < 0l) {
            return false
        }
    }
    return true
}

// The thinking gate (gemma-4's <|think|> opening the system turn) renders THIS turn: first turn,
// thinking on, gate declared with its specials in the vocab, and a system turn to carry it.
def private think_gate_now(c : ChatSession) : bool {
    return (c.first && c.enable_thinking && c.gate_ok
        && !empty(c.tmpl.think_gate.parts) && !empty(c.tmpl.system.parts))
}

// The NEXT turn is a THINKING turn: toggle on, the vocab carries the reply markers, and — for
// gate families (gemma-4) — the gate rendered on a consumed turn or renders this turn. A gate
// that never entered the context (mid-conversation toggle) stays instruct.
def private think_turn_active_(c : ChatSession) : bool {
    if (!c.enable_thinking || !c.think_vocab) {
        return false
    }
    if (empty(c.tmpl.think_gate.parts)) {
        return true
    }
    return c.gate_rendered || think_gate_now(c)
}

// The generation prompt: the thinking-turn alternate opener when the arch declares one (gemma-4's
// bare model header — the model emits its own thought channel), else the standard opener.
def private render_assistant_open(m : Model; c : ChatSession; var out : array<int64>) {
    if (think_turn_active_(c) && !empty(c.tmpl.assistant_open_think.parts)) {
        render_parts(m, c.tmpl.assistant_open_think.parts, "", out)
    } else {
        render_parts(m, c.tmpl.assistant_open.parts, "", out)
    }
}

// The system-turn content on the first turn: the user system prompt, and — when tools are
// declared and the family has a tool format — the tool block wrapping the verbatim tool JSONs
// (one per line, the Qwen/Hermes shape).
def private system_content(c : ChatSession) : string {
    if (empty(c.tools_json) || c.tmpl.tool_mode == ToolMode.none) {
        return c.system
    }
    var parts : array<string>
    parts |> push(empty(c.system) ? c.tmpl.tool_sys_prelude : "{c.system}\n\n{c.tmpl.tool_sys_prelude}")
    parts |> push_from(c.tools_json)
    parts |> push(c.tmpl.tool_sys_epilogue)
    let s = join(parts, "\n")
    delete parts
    return s
}

// The first-turn system block, shared by the text and audio prompt paths: the think gate spliced
// before the content, tool markers split where declared. Returns the text to FOLD into the user
// turn instead when the family has no system role (gemma-2/3); "" = rendered or nothing to do.
def private render_system_turn(m : Model; c : ChatSession; var out : array<int64>) : string {
    let sys = system_content(c)
    let gate = think_gate_now(c)   // gate implies a declared system turn — it renders even with no prompt text
    if (!gate && (empty(sys) || empty(c.tmpl.system.parts))) {
        return sys
    }
    var sp : array<ChatPart>
    if (gate) {
        sp := c.tmpl.think_gate.parts
    }
    if (empty(c.tools_json)) {
        if (!empty(sys)) {
            sp |> push(chat_text(sys))
        }
    } else {
        // the tool block quotes the call markers (format instructions) — marker-split
        split_on_tool_markers(m, c, sys, sp)
    }
    render_parts_spliced(m, c.tmpl.system.parts, sp, out)
    delete sp
    return ""
}

// Render this turn's prefill: (first turn) BOS + system, then the user turn and the generation prompt.
def private render_prompt(m : Model; c : ChatSession; var out : array<int64>) {
    if (c.first && c.tmpl.add_bos) {
        out |> push(bos_id(m))
    }
    if (c.first && !empty(c.tmpl.prelude.parts)) {   // conversation-start prelude (glm4moe: [gMASK]<sop>)
        render_parts(m, c.tmpl.prelude.parts, "", out)
    }
    var user_content = c.pending
    if (c.first) {
        let fold = render_system_turn(m, c, out)
        if (!empty(fold)) {
            user_content = "{fold}\n\n{c.pending}"   // no system role (Gemma): fold into the first user turn
        }
    }
    if (c.pending_is_tool) {
        // tool results ride the user turn frames; their response markers split like the system block's
        var tp : array<ChatPart>
        split_on_tool_markers(m, c, user_content, tp)
        render_parts_spliced(m, c.tmpl.user.parts, tp, out)
        delete tp
    } else {
        render_parts(m, c.tmpl.user.parts, user_content, out)
    }
    render_assistant_open(m, c, out)
}

//! Render an AUDIO turn's prefill as two spans around the soft-token splice point (head before,
//! tail after) — matches llama-mtmd-cli exactly, whose text chunks break at media boundaries so
//! BPE merges never cross the audio. Public with ``create_chat_renderer_``: the render-only seam.
def render_prompt_audio(m : Model; c : ChatSession; var head : array<int64>; var tail : array<int64>) {
    if (c.first && c.tmpl.add_bos) {
        head |> push(bos_id(m))
    }
    if (c.first && !empty(c.tmpl.prelude.parts)) {   // conversation-start prelude (glm4moe: [gMASK]<sop>)
        render_parts(m, c.tmpl.prelude.parts, "", head)
    }
    var user_content = c.pending
    if (c.first) {
        let fold = render_system_turn(m, c, head)
        if (!empty(fold)) {
            user_content = "{fold}\n\n{c.pending}"
        }
    }
    var before : array<ChatPart>
    var after : array<ChatPart>
    var seen = false
    for (p in c.tmpl.user.parts) {
        if (p.kind == ChatPartKind.content) {
            seen = true
        } elif (seen) {
            after |> push(p)
        } else {
            before |> push(p)
        }
    }
    render_parts(m, before, "", head)
    render_parts(m, c.tmpl.audio_pre.parts, "", head)
    var tail_parts := c.tmpl.audio_post.parts
    tail_parts |> push(chat_content())
    if (think_turn_active_(c) && !empty(c.tmpl.assistant_open_think.parts)) {
        tail_parts |> push_from(after, c.tmpl.assistant_open_think.parts)
    } else {
        tail_parts |> push_from(after, c.tmpl.assistant_open.parts)
    }
    render_parts(m, tail_parts, user_content, tail)
}

// ===== Public API =====

// Consume the pending user turn once its render is done — render_prompt_audio reads c.pending
// gated only on n_audio_rows, so a leftover would silently re-inject the text into a later
// audio-only turn.
def private consume_pending_(var c : ChatSession) {
    if (think_gate_now(c)) {   // this consumed turn rendered the gate — thinking arms for the session
        c.gate_rendered = true
    }
    c.first = false
    c.has_pending = false
    c.pending = ""
    c.pending_is_tool = false
}

//! `create_chat_`'s RENDER-ONLY twin: resolves the chat template, stop ids, and turn close
//! exactly like `create_chat_`, but creates NO KV session — a queued request can render its
//! whole prompt holding tokens only, no cache memory. Do not eval/respond on it.
def create_chat_renderer_(model : Model; system : string = ""; max_new : int64 = 256l) : ChatSession {
    var c = ChatSession(tmpl <- chat_template(model), system = system, max_new = max_new)
    c.enable_thinking = c.tmpl.think_default   // the family's default (gemma-4 E-series: off)
    c.think_vocab = think_vocab_ok(model, c.tmpl)
    c.gate_ok = gate_specials_ok(model, c.tmpl)
    for (sp in c.tmpl.stop) {
        let id = special_id(model, sp)
        if (id >= 0l) {
            c.stop_ids |> push(id)
        }
    }
    for (sp in c.tmpl.stop_nothink) {
        let id = special_id(model, sp)
        if (id >= 0l) {
            c.stop_ids_nothink |> push(id)
        }
    }
    // how the assistant turn terminates: the template's explicit assistant_close, else the
    // user-template parts after the content slot (right for chatml/llama-3-style templates)
    if (!empty(c.tmpl.assistant_close.parts)) {
        c.close_parts := c.tmpl.assistant_close.parts
    } else {
        var seen = false
        for (p in c.tmpl.user.parts) {
            if (p.kind == ChatPartKind.content) {
                seen = true
            } elif (seen) {
                c.close_parts |> push(p)
            }
        }
    }
    return <- c
}

//! Start a conversation over `model`. `system` is the system prompt (empty = none); `max_new` caps
//! each reply; `kv_dtype` is the session's KV-cache codec (f16 default, f32 = bit-exact reference).
//! A model can drive many independent ChatSessions.
def create_chat_(model : Model; system : string = ""; max_new : int64 = 256l; kv_dtype : KVDtype = KVDtype.f16) : ChatSession {
    var c <- create_chat_renderer_(model, system, max_new)
    c.session <- create_session_(model, kv_dtype, kv_dtype)
    return <- c
}

//! Start an AUDIO-CAPABLE conversation: like create_chat, but takes ownership of an audio tower
//! (moved in) so ``add_user_audio`` can splice soft tokens into its turns. A mismatched
//! decoder/mmproj pair panics here, not at respond time.
def create_chat_(model : Model; var tower : AudioTower; system : string = ""; max_new : int64 = 256l; kv_dtype : KVDtype = KVDtype.f16) : ChatSession {
    if (tower.proj_dim != model.config.dim) {
        panic("dasLLAMA chat: mmproj projects to {tower.proj_dim} but the decoder is {model.config.dim}-wide - mismatched pair")
    }
    var c <- create_chat_(model, system, max_new, kv_dtype)
    c.enc <- make_encoder_state(tower)
    c.tower <- tower
    return <- c
}

//! Queue audio (16 kHz mono f32 PCM) for the next respond() — encoded to soft tokens NOW,
//! spliced at the head of the next user turn (before its text). Composes with add_user in the
//! same turn; call inside with_job_que() (the encoder threads its kernels).
def add_user_audio_(var c : ChatSession; samples : array<float> | #) {
    if (c.tower.d_model == 0l) {
        panic("dasLLAMA chat: this chat has no audio tower - create it with create_chat(model, tower)")
    }
    let chunks <- log_mel_chunks(samples, c.tower.n_mel)
    let rows = audio_tokens_per_chunk(c.tower)
    let dim = c.tower.proj_dim
    c.audio_rows |> reserve(int((c.n_audio_rows + long_length(chunks) * rows) * dim))
    for (chunk in chunks) {
        audio_encode(c.tower, c.enc, chunk)
        for (i in range64(rows * dim)) {
            c.audio_rows |> push(c.enc.out[i])
        }
    }
    c.n_audio_rows += long_length(chunks) * rows
}

// The empty think block that turns a hybrid thinking model into an instruct model (Qwen3's
// enable_thinking=false). GENERATION prompt only — replayed history carries no think blocks.
// Skips silently when thinking is on or the vocab lacks the think specials.
def private render_think_suppress(m : Model; c : ChatSession; var out : array<int64>) {
    if (c.enable_thinking || empty(c.tmpl.think_suppress.parts)) {
        return
    }
    for (p in c.tmpl.think_suppress.parts) {
        if (p.kind == ChatPartKind.special && special_id(m, p.s) < 0l) {
            return
        }
    }
    render_parts(m, c.tmpl.think_suppress.parts, "", out)
}

//! Render the next turn's prefill token ids — BOS + system on the first turn, then the user turn and
//! the generation prompt — WITHOUT running the model. For inspection, token counting, and tests.
def render_turn_(m : Model; c : ChatSession) : array<int64> {
    var out : array<int64>
    render_prompt(m, c, out)
    render_think_suppress(m, c, out)
    return <- out
}

//! Render the tokens that TERMINATE an assistant turn — the template's explicit assistant_close,
//! else the user template's post-content parts — exactly what respond_/add_assistant_ eval after
//! the reply. For schedulers that close a finished stream's turn themselves.
def render_close_(m : Model; c : ChatSession) : array<int64> {
    var out : array<int64>
    render_parts(m, c.close_parts, "", out)
    return <- out
}

//! `add_assistant_`'s render half: appends the exact token stream a KNOWN reply prefills to
//! `out` WITHOUT running the model, advancing the transcript like `add_assistant_`. Replays a
//! stateless request's history on a `create_chat_renderer_` chat with no KV memory.
def render_assistant_(m : Model; var c : ChatSession; text : string; var out : array<int64>) {
    if (!c.has_pending) {
        return
    }
    render_prompt(m, c, out)
    out |> push_from(encode_(m, text, false, false))
    consume_pending_(c)
    render_parts(m, c.close_parts, "", out)
    c.history |> push(Message(role = Role.assistant, content = text))
}

//! Toggle reasoning for a hybrid thinking model: `false` renders the template's think_suppress
//! block on every generation prompt (the Jinja enable_thinking=false form); a no-op for templates
//! with no suppress form or vocabs without the think specials.
def set_thinking_(var c : ChatSession; on : bool) {
    c.enable_thinking = on
}

//! The stop ids in force for the NEXT generation: the template's stops, plus its thinking-off
//! extras while the next turn is NOT a thinking turn (gemma-4 cuts at a stray channel marker in
//! instruct mode — including "thinking on" that never armed: missing vocab, un-rendered gate).
def effective_stop_ids_(c : ChatSession) : array<int64> {
    var out := c.stop_ids
    if (!think_turn_active_(c)) {
        out |> push_from(c.stop_ids_nothink)
    }
    return <- out
}

//! Queue a user message for the next respond().
def add_user_(var c : ChatSession; text : string) {
    c.pending = text
    c.has_pending = true
    c.pending_is_tool = false
    c.history |> push(Message(role = Role.user, content = text))
}

//! Declare the conversation's tools (verbatim JSON objects, one per tool — the OpenAI ``tools[]``
//! entries, moved in). The first rendered turn then carries the family's tool block in its system
//! turn. Families with no tool format (``tmpl.tool_call_open`` empty) ignore them.
def set_tools_(var c : ChatSession; var tools : array<string>) {
    c.tools_json <- tools
}

//! Queue TOOL RESULTS as the next pending turn — the reply to an assistant turn that called
//! tools. Each result wraps in the family's tool-response markers, riding the user turn frames
//! (ChatML/Hermes convention). Call in place of ``add_user_``, then respond/render as usual.
def add_tool_results_(var c : ChatSession; results : array<string>) {
    var parts : array<string>
    parts |> reserve(length(results))
    c.history |> reserve(length(c.history) + length(results))
    for (r in results) {
        parts |> push("{c.tmpl.tool_resp_open}\n{r}\n{c.tmpl.tool_resp_close}")
        c.history |> push(Message(role = Role.tool, content = r))
    }
    c.pending = join(parts, "\n")
    c.has_pending = true
    c.pending_is_tool = true
    delete parts
}

//! `render_assistant_`'s tool-calling twin: replay an assistant turn that emitted tool calls.
//! ``calls`` are verbatim ``\{"name":…,"arguments":…}`` JSON objects; ``text`` is any content
//! alongside. Precondition: a pending user or tool turn; a no-op otherwise.
def render_assistant_calls_(m : Model; var c : ChatSession; text : string; calls : array<string>; var out : array<int64>) {
    if (!c.has_pending) {
        return
    }
    render_prompt(m, c, out)
    var parts : array<string>
    parts |> reserve(length(calls) + 1)
    if (!empty(text)) {
        parts |> push(text)
    }
    for (cj in calls) {
        parts |> push("{c.tmpl.tool_call_open}\n{cj}\n{c.tmpl.tool_call_close}")
    }
    let body = join(parts, "\n")
    delete parts
    var bp : array<ChatPart>
    split_on_tool_markers(m, c, body, bp)   // call markers tokenize as their ids where the vocab has them
    render_parts(m, bp, "", out)
    delete bp
    consume_pending_(c)
    render_parts(m, c.close_parts, "", out)
    c.history |> push(Message(role = Role.assistant, content = body))
}

//! Inject a KNOWN assistant reply into the conversation WITHOUT generating: prefills the pending
//! user turn and this text into the KV cache, then closes the turn — like respond() but with a
//! given reply. Precondition: a user message is pending; text turns only, no pending audio.
def add_assistant_(m : Model; var c : ChatSession; text : string) {
    if (!c.has_pending) {
        return
    }
    // in lockstep with render_assistant_ (rendered ≡ evaled; test_chat's render-seam case proves
    // it) — but evaled as the SAME three spans as always, so flash-prefill numerics never shift
    var pre : array<int64>
    render_prompt(m, c, pre)   // user turn + the assistant-open generation prompt
    eval_(m, c.session, pre)
    let body <- encode_(m, text, false, false)
    eval_(m, c.session, body)
    consume_pending_(c)
    var close : array<int64>
    render_parts(m, c.close_parts, "", close)
    eval_(m, c.session, close)
    c.history |> push(Message(role = Role.assistant, content = text))
}

//! `reply` with a leading `<think>…</think>` reasoning block removed (the Qwen3 thinking format).
//! Replies that don't open with `<think>` pass through unchanged; an unterminated block strips
//! to empty, since the reasoning never reached an answer.
def strip_think(reply : string) : string {
    let open = find(reply, "<think>")
    if (open < 0 || !is_null_or_whitespace(slice(reply, 0, open))) {
        return reply
    }
    let close = find(reply, "</think>")
    return close < 0 ? "" : strip(slice(reply, close + length("</think>"), length(reply)))
}

// ===== Reply-side reasoning matcher (per-arch, streaming-capable) =====

// gpt-oss Harmony markers — the channel_switch grammar is the format, not per-arch data.
let private HARMONY_CH = "<|channel|>"
let private HARMONY_MSG = "<|message|>"
let private HARMONY_END = "<|end|>"

// ThinkStream.state values (shared by every mode; HEADER/BETWEEN are channel_switch-only).
// Public: the ThinkStream field default references them, so cross-module construction needs them.
let TS_START = 0     // deciding whether the reply opens with the reasoning opener
let TS_REASON = 1    // inside the reasoning span
let TS_WS = 2        // swallowing whitespace between the closer and the content
let TS_CONTENT = 3   // past the reasoning: everything passes through
let TS_HEADER = 4    // channel_switch: reading a channel header up to <|message|>
let TS_BETWEEN = 5   // channel_switch: framing glue between <|end|> and the next header

//! One reply split at its reasoning boundary (`split_reasoning_`).
struct ThinkSplit {
    reasoning : string = ""
    content : string = ""
}

//! Incremental reasoning/content splitter for a streamed reply: `make_think_stream_` builds it,
//! `think_feed_` yields per-piece deltas, `think_finish_` flushes the tail at end-of-generation.
//! An unarmed stream (`ThinkMode.none`) passes everything to content.
struct ThinkStream {
    mode : ThinkMode = ThinkMode.none
    open_mark : string = ""
    close_mark : string = ""
    state : int = TS_CONTENT   // pass-through by default — only make_think_stream_ arms TS_START
    hold : string = ""      // undecided tail: leading whitespace, a partial marker, or header glue
    hdr : string = ""       // channel_switch: the channel header accumulating up to <|message|>
    consumed : bool = false // a reasoning span was entered — the drain's strip rule keys on this
}

//! The reply matcher for `tmpl`, armed (`thinking` = the caller's own arming decision — the
//! low-level form for tests; sessions use the ChatSession overload below, which folds in the
//! vocab and gate state). Unarmed or `ThinkMode.none` streams pass everything to content.
def make_think_stream_(tmpl : ChatTemplate; thinking : bool) : ThinkStream {
    if (!thinking || tmpl.think_mode == ThinkMode.none) {
        return ThinkStream()
    }
    return ThinkStream(mode = tmpl.think_mode, open_mark = tmpl.think_open, close_mark = tmpl.think_close,
                       state = TS_START)
}

//! The reply matcher for `c`'s NEXT turn: armed only when the turn is a thinking turn
//! (`think_turn_active_` — toggle on, markers in the vocab, gate rendered or rendering).
def make_think_stream_(c : ChatSession) : ThinkStream {
    return make_think_stream_(c.tmpl, think_turn_active_(c))
}

// Longest k < length(mark) where s ends with mark's first k characters — the hold-back length
// that keeps a marker split across feed boundaries matchable.
def private tail_prefix_len(s : string; mark : string) : int {
    let n = length(s)
    var k = min(n, length(mark) - 1)
    while (k > 0) {
        if (slice(s, n - k, n) == slice(mark, 0, k)) {
            return k
        }
        k --
    }
    return 0
}

// One whitespace-or-marker probe at TS_START: -1 = need more input, 0 = no opener (content),
// 1 = opener matched at `cut` (first index past it, leading whitespace included).
def private probe_opener(work : string; mark : string; var cut : int&) : int {
    let stripped = strip_left(work)
    if (empty(stripped)) {
        return -1
    }
    let m = length(mark)
    let avail = min(length(stripped), m)
    if (slice(stripped, 0, avail) != slice(mark, 0, avail)) {
        return 0
    }
    if (avail < m) {
        return -1
    }
    cut = length(work) - length(stripped) + m
    return 1
}

// Route one channel header by its NAME token — the text up to the first space, since Harmony
// headers carry recipients (`commentary to=functions.finalize_report`): only the `final` channel
// is content; `analysis`, `commentary`, and anything unrecognized classify as reasoning.
def private harmony_header_is_content(hdr : string) : bool {
    let name = strip(hdr)
    let sp = find(name, " ")
    return (sp < 0 ? name : slice(name, 0, sp)) == "final"
}

def private think_feed_symmetric(var ts : ThinkStream; var work : string; var rp, cp : array<string>) {
    while (!empty(work)) {
        if (ts.state == TS_START) {
            var cut = 0
            let r = probe_opener(work, ts.open_mark, cut)
            if (r < 0) {
                ts.hold = work
                return
            }
            if (r == 0) {
                ts.state = TS_CONTENT
                continue
            }
            // asymmetric openers spell an optional trailing newline (gemma-4's `thought\n`)
            if (ts.mode == ThinkMode.asymmetric && cut < length(work) && slice(work, cut, cut + 1) == "\n") {
                cut ++
            } elif (ts.mode == ThinkMode.asymmetric && cut >= length(work)) {
                ts.hold = work   // the optional newline may still arrive with the next piece
                return
            }
            work = slice(work, cut, length(work))
            ts.state = TS_REASON
            ts.consumed = true
        } elif (ts.state == TS_REASON) {
            let f = find(work, ts.close_mark)
            if (f >= 0) {
                rp |> push(slice(work, 0, f))
                work = slice(work, f + length(ts.close_mark), length(work))
                ts.state = TS_WS
            } else {
                let h = tail_prefix_len(work, ts.close_mark)
                let n = length(work)
                rp |> push(slice(work, 0, n - h))
                ts.hold = slice(work, n - h, n)
                return
            }
        } elif (ts.state == TS_WS) {
            let stripped = strip_left(work)
            if (empty(stripped)) {
                return   // still whitespace: drop it (content leading-trim)
            }
            work = stripped
            ts.state = TS_CONTENT
        } else {
            cp |> push(work)
            return
        }
    }
}

def private think_feed_channel(var ts : ThinkStream; var work : string; var rp, cp : array<string>) {
    while (!empty(work)) {
        if (ts.state == TS_START) {
            var cut = 0
            let r = probe_opener(work, HARMONY_CH, cut)
            if (r < 0) {
                ts.hold = work
                return
            }
            if (r == 0) {
                ts.state = TS_CONTENT
                continue
            }
            work = slice(work, cut, length(work))
            ts.state = TS_HEADER
            ts.hdr = ""
            ts.consumed = true
        } elif (ts.state == TS_HEADER) {
            let f = find(work, HARMONY_MSG)
            if (f < 0) {
                let h = tail_prefix_len(work, HARMONY_MSG)
                let n = length(work)
                ts.hdr = "{ts.hdr}{slice(work, 0, n - h)}"   // headers are a few chars; no O(n^2) risk
                ts.hold = slice(work, n - h, n)
                return
            }
            ts.hdr = "{ts.hdr}{slice(work, 0, f)}"
            work = slice(work, f + length(HARMONY_MSG), length(work))
            ts.state = harmony_header_is_content(ts.hdr) ? TS_CONTENT : TS_REASON
            ts.hdr = ""
        } elif (ts.state == TS_REASON) {
            let fe = find(work, HARMONY_END)
            let fc = find(work, HARMONY_CH)
            if (fe >= 0 && (fc < 0 || fe <= fc)) {
                rp |> push(slice(work, 0, fe))
                work = slice(work, fe + length(HARMONY_END), length(work))
                ts.state = TS_BETWEEN
            } elif (fc >= 0) {
                rp |> push(slice(work, 0, fc))
                work = slice(work, fc + length(HARMONY_CH), length(work))
                ts.state = TS_HEADER
                ts.hdr = ""
            } else {
                let h = max(tail_prefix_len(work, HARMONY_END), tail_prefix_len(work, HARMONY_CH))
                let n = length(work)
                rp |> push(slice(work, 0, n - h))
                ts.hold = slice(work, n - h, n)
                return
            }
        } elif (ts.state == TS_BETWEEN) {
            // framing glue (<|start|>assistant etc.) up to the next channel header is dropped
            let f = find(work, HARMONY_CH)
            if (f < 0) {
                let h = tail_prefix_len(work, HARMONY_CH)
                let n = length(work)
                ts.hold = slice(work, n - h, n)   // the glue before it is framing, not content
                return
            }
            work = slice(work, f + length(HARMONY_CH), length(work))
            ts.state = TS_HEADER
            ts.hdr = ""
        } else {
            cp |> push(work)
            return
        }
    }
}

//! Feed one decoded piece through the splitter. The out-strings are OVERWRITTEN with this piece's
//! reasoning/content deltas (either may come back empty — e.g. while a partial marker is held).
//! Pieces may split markers at any byte; the stream holds partial matches back across calls.
def think_feed_(var ts : ThinkStream; piece : string; var reasoning, content : string&) {
    let work = empty(ts.hold) ? piece : "{ts.hold}{piece}"
    ts.hold = ""
    var rp : array<string>
    var cp : array<string>
    if (ts.mode == ThinkMode.channel_switch) {
        think_feed_channel(ts, work, rp, cp)
    } else {
        think_feed_symmetric(ts, work, rp, cp)
    }
    reasoning = join(rp, "")
    content = join(cp, "")
    delete rp
    delete cp
}

//! Flush the splitter at end-of-generation, OVERWRITING the out-strings with the final deltas:
//! an unclosed reasoning span stays reasoning (the truncated-tail rule); an opener probe that
//! never completed was ordinary content; whitespace/framing tails (TS_WS/HEADER/BETWEEN) drop.
def think_finish_(var ts : ThinkStream; var reasoning, content : string&) {
    reasoning = ""
    content = ""
    if (ts.state == TS_REASON) {
        reasoning = ts.hold
    } elif (ts.state == TS_CONTENT) {
        content = ts.hold
    } elif (ts.state == TS_START && !empty(ts.hold)) {
        // EOG mid-probe: a COMPLETED opener means generation truncated at the opener — the tail
        // past it (gemma-4's undecided optional newline hold) is reasoning; an incomplete opener
        // was ordinary content all along
        var cut = 0
        let mark = ts.mode == ThinkMode.channel_switch ? HARMONY_CH : ts.open_mark
        if (probe_opener(ts.hold, mark, cut) == 1) {
            reasoning = ts.mode == ThinkMode.channel_switch ? "" : slice(ts.hold, cut, length(ts.hold))
            ts.consumed = true   // generation truncated AT the opener — a reasoning span was entered
        } else {
            content = ts.hold
        }
    }
    ts.hold = ""
    ts.hdr = ""
    ts.state = TS_CONTENT
}

//! Drain a COMPLETE reply through `ts` — feed, finish, and the strip rule in one place: both
//! halves strip when a reasoning span was CONSUMED (an empty think block strips too — the
//! `strip_think` parity rule). The one engine behind `split_reasoning_` and the server.
def think_drain_(var ts : ThinkStream; full : string) : ThinkSplit {
    var res = ThinkSplit()
    think_feed_(ts, full, res.reasoning, res.content)
    var rtail = ""
    var ctail = ""
    think_finish_(ts, rtail, ctail)
    res.reasoning = "{res.reasoning}{rtail}"
    res.content = "{res.content}{ctail}"
    if (ts.consumed) {
        res.reasoning = strip(res.reasoning)
        res.content = strip(res.content)
    }
    return res
}

//! Split a complete reply at its reasoning boundary per the template's reply-side format —
//! `strip_think`'s per-arch generalization (see think_drain_ for the strip rule).
def split_reasoning_(tmpl : ChatTemplate; reply : string) : ThinkSplit {
    var ts = make_think_stream_(tmpl, true)
    return think_drain_(ts, reply)
}

//! Generate the assistant's reply to the queued user message: render the turn, prefill it, then
//! stream tokens through `blk(piece)` (return false to stop early) until a stop token or the
//! budget. Terminates the turn; history stores the reply reasoning-stripped (split_reasoning_).
def respond_(m : Model; var c : ChatSession; params : SamplingParams;
             blk : block<(piece : string) : bool>) : string {
    if (!c.has_pending && c.n_audio_rows == 0l) {
        return ""
    }
    // local clone so the streaming callback captures no part of `c` (c.session is passed to generate)
    var stops <- effective_stop_ids_(c)
    var reply_parts : array<string>
    if (c.n_audio_rows > 0l) {
        // audio turn: token spans around the soft-token splice, prefilled as embedding rows
        var head : array<int64>
        var tail : array<int64>
        render_prompt_audio(m, c, head, tail)
        render_think_suppress(m, c, tail)
        let dim = m.config.dim
        let n_head = long_length(head)
        let n_tail = long_length(tail)
        let npos = n_head + c.n_audio_rows + n_tail
        var embd : array<float>
        embd |> resize(npos * dim)
        embed_text_rows(m, head, embd, 0l)
        for (i in range64(c.n_audio_rows * dim)) {
            embd[n_head * dim + i] = c.audio_rows[i]
        }
        embed_text_rows(m, tail, embd, n_head + c.n_audio_rows)
        generate_embd_(m, c.session, embd, npos, params, c.max_new) $(id, piece) {
            for (sid in stops) {
                return false if (id == sid)
            }
            reply_parts |> push(piece)
            return invoke(blk, piece)
        }
        c.audio_rows |> clear()
        c.n_audio_rows = 0l
    } else {
        var pre : array<int64>
        render_prompt(m, c, pre)
        render_think_suppress(m, c, pre)
        generate_(m, c.session, pre, params, c.max_new) $(id, piece) {
            for (sid in stops) {
                return false if (id == sid)
            }
            reply_parts |> push(piece)
            return invoke(blk, piece)
        }
    }
    consume_pending_(c)
    // close the assistant turn so the next turn continues cleanly
    var close : array<int64>
    render_parts(m, c.close_parts, "", close)
    eval_(m, c.session, close)
    let reply = join(reply_parts, "")
    // history holds only the answer — the per-arch reasoning span strips; families with no
    // declared reply format keep the legacy unconditional <think> strip (a thinking fine-tune
    // served under a non-thinking arch — R1-distills on `llama` — must not bloat history)
    let stored = (c.tmpl.think_mode == ThinkMode.none ? strip_think(reply)
        : split_reasoning_(c.tmpl, reply).content)
    c.history |> push(Message(role = Role.assistant, content = stored))
    return reply
}
