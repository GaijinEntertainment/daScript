options gen2

//! dasLLAMA — CPU LLM inference over GGUF models, in pure daslang. This module is the public API:
//! ``load_model`` -> ``create_session`` -> ``encode`` / ``eval`` / ``sample`` (or the one-call
//! streaming ``generate``), plus the chat layer (``create_chat`` / ``add_user`` / ``respond``).
//! Requiring it also registers every supported architecture (Llama-2/3, Mistral, Qwen, Phi-3,
//! Gemma-2/3/4, Qwen-MoE, gpt-oss) — ``load_model`` dispatches on GGUF metadata, so the same
//! program runs any of them.
//!
//! Run model programs with ``-jit`` (interpreted inference is far too slow), and wrap the
//! generation in ``with_job_que()`` from ``daslib/jobque_boost`` so the kernels can thread —
//! see ``examples/dasLLAMA/run.das`` and ``chat.das`` for the canonical shape.

module dasllama shared public

require dasllama/dasllama_transformer public   // the engine + arch registry; every arch [init] fires
require dasllama/dasllama_chat public          // layer 2: chat sessions over the same engine

// This facade owns the good names; the engine spells each wrapped function with a trailing
// underscore (load_model_, eval_, ...) so the public re-export above and the documented stub
// below never collide at a call site.

// ===== Model, session, tokenizer =====

def load_model(path : string; mode : QuantMode = QuantMode.fp32) : Model {
    //! Load a model AND its tokenizer from a GGUF file — the one entry point. The architecture and
    //! tokenizer backend are auto-selected from GGUF metadata; ``mode`` picks the weight quantization
    //! (``QuantMode.fp32`` = the token-exact reference, ``QuantMode.q8`` = the fast int8 path).
    return <- load_model_(path, mode)
}

def create_session(model : Model) : Session {
    //! Create a fresh session (KV cache + scratch) sized to ``model.config.seq_len``. A model serves
    //! many sessions, each with its own position and cache — one model, many independent conversations.
    //! On large-context models cap ``model.config.seq_len`` BEFORE creating sessions to bound KV memory.
    return <- create_session_(model)
}

def encode(model : Model; text : string; add_special = true; parse_special = false) : array<int64> {
    //! Encode ``text`` to token ids with the model's tokenizer. ``add_special`` prepends BOS where the
    //! model expects one. ``parse_special`` is reserved and not yet honored — special tokens reach the
    //! model as atomic ids from the chat layer's template renderer, never by spelling them in text.
    return <- encode_(model, text, add_special, parse_special)
}

def decode(model : Model; ids : array<int64>) : string {
    //! Decode a token-id sequence back to text with the model's tokenizer.
    return decode_(model, ids)
}

def piece(model : Model; id : int64) : string {
    //! Decode a single token to its text piece — the streaming counterpart of ``decode``.
    return piece_(model, id)
}

// ===== Evaluation and sampling =====

def eval(model : Model; var session : Session; tokens : array<int64>) {
    //! THE eval primitive: run ``tokens`` at the session's current position and advance it. Prefill =
    //! ``eval(prompt)``; each generation step = ``eval([token])`` — the same call at different batch
    //! sizes. Logits land in ``session.logits``.
    eval_(model, session, tokens)
}

def sample(var session : Session; params : SamplingParams) : int64 {
    //! Sample the next token from ``session.logits`` per ``params``: repetition penalty over the recent
    //! window, then temperature + top-k + softmax + CDF — or greedy argmax when ``params.temp <= 0``.
    //! ``SamplingParams()`` defaults are greedy.
    return sample_(session, params)
}

def set_seed(var session : Session; seed : int) {
    //! Seed the session's sampling RNG for reproducible generation.
    set_seed_(session, seed)
}

def stats(session : Session) : Stats {
    //! Timing of the most recent ``generate``/``respond`` call on ``session``: prompt/generated token
    //! counts, time to first token, prefill and generation tok/s.
    return stats_(session)
}

// ===== Generation =====

def generate(model : Model; var session : Session; prompt : array<int64>; params : SamplingParams;
             max_tokens : int64; blk : block<(id : int64; piece : string) : bool>) : int64 {
    //! Stream-generate up to ``max_tokens`` from ``prompt``, invoking the trailing block per token with
    //! ``(id, piece)``; return ``false`` from the block to stop early (e.g. on a stop token). Prefills
    //! the prompt in one ``eval``, then samples one token at a time, advancing the session. Returns the
    //! number of tokens emitted; timing is available afterwards via ``stats``.
    return generate_(model, session, prompt, params, max_tokens, blk)
}

// ===== Chat =====

def create_chat(model : Model; system : string = ""; max_new : int64 = 256l) : ChatSession {
    //! Start a conversation over ``model``: resolves the model's chat template (sniffed from the GGUF's
    //! embedded template, falling back to the arch registry) and creates the session. ``system`` is the
    //! system prompt (empty = none); ``max_new`` caps each reply. One model can drive many chats.
    return <- create_chat_(model, system, max_new)
}

def add_user(var chat : ChatSession; text : string) {
    //! Queue a user message for the next ``respond``.
    add_user_(chat, text)
}

def render_turn(model : Model; chat : ChatSession) : array<int64> {
    //! Render the next turn's prefill token ids — BOS + system on the first turn, then the user turn
    //! and the generation prompt — WITHOUT running the model. For inspection, token budgeting, tests.
    return <- render_turn_(model, chat)
}

def respond(model : Model; var chat : ChatSession; params : SamplingParams;
            blk : block<(piece : string) : bool>) : string {
    //! Generate the assistant's reply to the queued user message: render the turn, prefill it, then
    //! stream pieces through the trailing block (return ``false`` to stop early) until a stop token or
    //! the ``max_new`` budget. Terminates the turn in the KV cache and appends both turns to
    //! ``chat.history``. Returns the full reply text; timing via ``stats(chat.session)``.
    return respond_(model, chat, params, blk)
}
