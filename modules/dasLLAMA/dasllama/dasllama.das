options gen2
options _comment_hygiene = true

//! dasLLAMA — CPU LLM inference over GGUF models, in pure daslang. This module is the public API:
//! ``load_model`` -> ``create_session`` -> ``encode`` / ``eval`` / ``sample`` (or the one-call
//! streaming ``generate``), plus the chat layer (``create_chat`` / ``add_user`` / ``respond``).
//! Requiring it also registers every supported architecture (Llama-2/3, Mistral, Qwen, Phi-3,
//! Gemma-2/3/4, Qwen-MoE, gpt-oss) — ``load_model`` dispatches on GGUF metadata, so the same
//! program runs any of them.
//!
//! Run model programs with ``-jit`` (interpreted inference is far too slow), wrap the
//! generation in ``with_job_que()`` from ``daslib/jobque_boost`` so the kernels can thread, and
//! declare ``options stack = 65536`` — the forward chain outgrows the 16K default on arm64, and
//! a required module cannot raise the program's stack for you. See ``examples/dasLLAMA/run.das``
//! and ``chat.das`` for the canonical shape.

module dasllama shared public

require dasllama/dasllama_transformer public   // the engine + arch registry; every arch [init] fires
require dasllama/dasllama_image   // the prepared-image cache: load_model maps it when present
require dasllama/dasllama_prefix public        // page-granular prefix cache over the paged pool
require dasllama/dasllama_chat public          // layer 2: chat sessions over the same engine
require dasllama/dasllama_audio public         // audio tower (mel frontend + whisper encoder + projectors)
require dasllama/dasllama_asr public           // uniform ASR surface (whisper today; parakeet/qwen3-asr land behind the same sniff)
require dasllama/dasllama_vad public           // Silero-VAD (voice-activity detection): segments + streaming iterator
require dasllama/dasllama_math                 // setup_dasllama_jobque_ (engine-level, wrapped below, not re-exported)

// This facade owns the good names; the engine spells each wrapped function with a trailing
// underscore (load_model_, eval_, ...) so the public re-export above and the documented stub
// below never collide at a call site.

// ===== Model, session, tokenizer =====

def setup_dasllama_jobque() {
    //! Configure the job queue for dasLLAMA's fork/join matmul dispatch: pooled fork contexts,
    //! batched dispatch, and the worker spin-before-park window (``jobque_spin_us``; 0 disables).
    //! Call INSIDE ``with_job_que()``, before the first ``generate``/``eval``.
    setup_dasllama_jobque_()
}

def load_model(path : string; mode : QuantMode = QuantMode.fp32) : Model {
    //! Load a model AND its tokenizer from a GGUF file — architecture and tokenizer backend are
    //! auto-selected from GGUF metadata; ``mode`` picks the weight quantization. Q8 loads cache a
    //! PREPARED IMAGE beside the gguf (box-specific, ``DASLLAMA_IMAGE=0`` disables) for millisecond reloads.
    return <- load_model_cached(path, mode)
}

def create_session(model : Model; kv_dtype : KVDtype = KVDtype.f16) : Session {
    //! Create a fresh session (KV cache + scratch) sized to ``model.config.seq_len`` — one model,
    //! many independent conversations. ``kv_dtype`` picks the KV-cache codec: ``f16`` (default,
    //! near-lossless) halves bytes, ``q8_0`` halves again (needs head_size/kv_dim % 32 == 0).
    return <- create_session_(model, kv_dtype, kv_dtype)
}

def create_kv_pool(model : Model; page_rows : int64 = 64l; kv_dtype : KVDtype = KVDtype.f16) : KVPool {
    //! Create a caller-owned PAGED KV pool over ``model``'s cache geometry. Sessions created over it
    //! allocate cache pages of ``page_rows`` positions on demand instead of the full ``seq_len`` slab
    //! up front, so many sessions share one elastic pool. Keep the pool alive as long as sessions live.
    return <- create_kv_pool_(model.config, page_rows, kv_dtype, kv_dtype)
}

def create_session(model : Model; var pool : KVPool) : Session {
    //! Create a session whose KV cache is PAGED out of ``pool`` (see ``create_kv_pool``) — pages
    //! allocate on demand as the context grows. Call ``release_kv_pages`` before ``delete``, or its
    //! pages stay checked out of the pool. Otherwise behaves exactly like a flat session.
    return <- create_session_(model, pool)
}

def release_kv_pages(var session : Session) {
    //! Return ``session``'s KV pages to its pool (no-op on flat sessions). The normal shape is
    //! release + ``delete``; a released session stays alive but loses its cached context — to reuse
    //! it, also reset ``session.n_past`` to 0.
    release_kv_pages_(session)
}

def create_prefix_cache(max_groups : int64 = 0l) : PrefixCache {
    //! Create a prefix cache for the paged sessions of one ``create_kv_pool`` pool: finished
    //! streams donate KV pages (``prefix_insert``), later requests with the same prefix attach them
    //! (``prefix_attach``) instead of re-prefilling. ``max_groups`` caps retained groups (0 = unbounded).
    return <- create_prefix_cache_(max_groups)
}

def prefix_attach(var cache : PrefixCache; var pool : KVPool; var session : Session; prompt : array<int64>) : int64 {
    //! Attach the longest cached prefix of ``prompt`` to a FRESH paged ``session`` of ``pool``:
    //! matched pages join the session's block table and ``n_past`` advances past them, so the
    //! caller prefills only the tail. Returns the matched count, capped one token short of the prompt.
    return prefix_attach_(cache, pool, session, prompt)
}

def prefix_insert(var cache : PrefixCache; var pool : KVPool; session : Session; tokens : array<int64>) {
    //! Donate a finished session's KV pages to the cache. ``tokens`` is the session's full EVALED
    //! history (only the first ``n_past`` rows exist); every full page not already cached is
    //! registered and survives ``release_kv_pages``. Call right before releasing.
    prefix_insert_(cache, pool, session, tokens)
}

def prefix_release(var cache : PrefixCache; var pool : KVPool) {
    //! Release every cached page back to ``pool`` and clear the cache (pages still used by live
    //! sessions stay alive until those sessions release them). Call before deleting the pool.
    prefix_release_(cache, pool)
}

def prefix_held_groups(cache : PrefixCache) : int64 {
    //! Pages the cache currently holds (== pool groups retained for reuse).
    return prefix_held_groups_(cache)
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

def eval_embd(model : Model; var session : Session; embd : array<float>; npos : int64) {
    //! ``eval``'s embedding-input twin: prefill ``npos`` pre-built embedding rows (``npos × dim``,
    //! token-major) at the session's current position and advance it — the multimodal splice entry
    //! (text via ``embed_text_rows``, media via an encoder tower's soft tokens).
    eval_embd_(model, session, embd, npos)
}

def create_batch_workspace(model : Model) : BatchWorkspace {
    //! Create the caller-owned scratch that ``eval_batch`` steps through — one per concurrent batch,
    //! reused across calls (buffers grow to the largest batch seen). Holds no session state: the
    //! sessions keep their own positions, caches and logits.
    return <- create_batch_workspace_(model)
}

def eval_batch(model : Model; var ws : BatchWorkspace; var sessions : array<Session?>; tokens : array<int64>) {   // nolint:LINT014 — rows mutate THROUGH the pointers; a const array constifies the pointees
    //! One synchronous batched decode step: row i evals ``tokens[i]`` at ``sessions[i]``'s current
    //! position, advancing each by one — B conversations through ONE pass of the weights (GEMVs
    //! batch into GEMMs). Sessions must be distinct, same-geometry, one pool if paged.
    eval_batch_(model, ws, sessions, tokens)
}

def sample(var session : Session; params : SamplingParams) : int64 {
    //! Sample the next token from ``session.logits`` per ``params``: penalties, then
    //! temperature/top-k/top-p/min-p and a CDF draw — or greedy argmax when ``params.temp <= 0``
    //! (``SamplingParams()`` defaults are greedy).
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
    //! Stream-generate up to ``max_tokens`` from ``prompt``, invoking the trailing block per token
    //! with ``(id, piece)``; return ``false`` from the block to stop early. Prefills the prompt in
    //! one ``eval``, then samples one token at a time; returns the number of tokens emitted.
    return generate_(model, session, prompt, params, max_tokens, blk)
}

def generate_embd(model : Model; var session : Session; embd : array<float>; npos : int64;
                  params : SamplingParams; max_tokens : int64;
                  blk : block<(id : int64; piece : string) : bool>) : int64 {
    //! ``generate``'s embedding-prefill twin: prefill ``npos`` pre-built embedding rows (the
    //! multimodal splice — see ``eval_embd``), then stream-sample exactly like ``generate``. The
    //! chat layer's audio turns run on this; use it directly for custom multimodal prompts.
    return generate_embd_(model, session, embd, npos, params, max_tokens, blk)
}

// ===== Embeddings =====

def embed(model : Model; text : string) : array<float> {
    //! Mean-pooled, L2-normalized sentence embedding of ``text`` (``model.config.dim`` floats): the
    //! decoder's last-layer hidden state (post-final RMSNorm), averaged then unit-normalized. A
    //! decoder-only model used this way yields RAG-grade vectors, not a dedicated embedder's.
    return <- embed_(model, text)
}

// ===== Chat =====

def create_chat(model : Model; system : string = ""; max_new : int64 = 256l; kv_dtype : KVDtype = KVDtype.f16) : ChatSession {
    //! Start a conversation over ``model``: resolves the chat template (GGUF-embedded, falling back
    //! to the arch registry) and creates the session. ``system`` is the system prompt (empty =
    //! none); ``max_new`` caps each reply; ``kv_dtype`` is the session's KV-cache codec.
    return <- create_chat_(model, system, max_new, kv_dtype)
}

def create_chat(model : Model; var tower : AudioTower; system : string = ""; max_new : int64 = 256l; kv_dtype : KVDtype = KVDtype.f16) : ChatSession {
    //! Start an AUDIO-CAPABLE conversation: takes ownership of ``tower`` (moved in — load with
    //! ``load_audio_tower``) so ``add_user_audio`` can splice audio into its turns. Panics on a
    //! mismatched decoder/mmproj pair; ``kv_dtype`` is the session's KV-cache codec (f16 default).
    return <- create_chat_(model, tower, system, max_new, kv_dtype)
}

def create_chat_renderer(model : Model; system : string = ""; max_new : int64 = 256l) : ChatSession {
    //! ``create_chat``'s RENDER-ONLY twin: resolves the template/stop ids/turn close but creates NO
    //! KV session — a queued request can render its whole prompt holding tokens only, no cache
    //! memory. It cannot ``respond``/``eval``.
    return <- create_chat_renderer_(model, system, max_new)
}

def add_user(var chat : ChatSession; text : string) {
    //! Queue a user message for the next ``respond``.
    add_user_(chat, text)
}

def add_assistant(model : Model; var chat : ChatSession; text : string) {
    //! Inject a KNOWN assistant reply (no generation): prefill the pending user turn and ``text``
    //! into the KV cache, then close the turn — like ``respond`` but with a supplied reply. The
    //! shape a stateless server needs to replay history. Precondition: a user message is pending.
    add_assistant_(model, chat, text)
}

def add_user_audio(var chat : ChatSession; samples : array<float> | #) {
    //! Queue audio (16 kHz mono f32 PCM) for the next ``respond`` — encoded to soft tokens
    //! immediately and spliced at the head of the turn before any ``add_user`` text. Needs a chat
    //! created with ``create_chat(model, tower)``; call inside ``with_job_que()``.
    add_user_audio_(chat, samples)
}

def caps(model : Model) : LlmCaps {
    //! What ``model`` honestly supports at the chat layer (see LlmCaps) — e.g. gemma has no system
    //! role, so the chat layer folds the system prompt into the first user turn; ``system_prompt``
    //! is false there so callers can surface it instead of being silently absorbed.
    return caps_(model)
}

def render_turn(model : Model; chat : ChatSession) : array<int64> {
    //! Render the next turn's prefill token ids — BOS + system on the first turn, then the user turn
    //! and the generation prompt — WITHOUT running the model. For inspection, token budgeting, tests.
    return <- render_turn_(model, chat)
}

def render_assistant(model : Model; var chat : ChatSession; text : string; var out : array<int64>) {
    //! ``add_assistant``'s render half: appends the exact token stream a known reply prefills to
    //! ``out`` WITHOUT running the model, advancing the transcript like ``add_assistant``. Use on a
    //! ``create_chat_renderer`` chat to replay history with no KV memory. Precondition: user message pending.
    render_assistant_(model, chat, text, out)
}

def render_close(model : Model; chat : ChatSession) : array<int64> {
    //! The tokens that TERMINATE an assistant turn (what ``respond`` evals after the reply) — for
    //! schedulers that close a finished stream's turn themselves.
    return <- render_close_(model, chat)
}

def set_tools(var chat : ChatSession; var tools : array<string>) {
    //! Declare the conversation's tools (verbatim OpenAI ``tools[]`` JSON objects, moved in)
    //! BEFORE the first turn renders — the system turn carries the family's tool block. Families
    //! with no tool format (``tmpl.tool_call_open`` empty) ignore them.
    set_tools_(chat, tools)
}

def add_tool_results(var chat : ChatSession; results : array<string>) {
    //! Queue tool results as the next pending turn — the reply to an assistant turn that called
    //! tools. Call in place of ``add_user``, then ``respond``/``render_turn`` as usual.
    add_tool_results_(chat, results)
}

def set_thinking(var chat : ChatSession; on : bool) {
    //! Toggle reasoning for a hybrid thinking model (Qwen3 family): ``false`` appends the
    //! template's empty think block so the model answers directly. No-op without a suppress
    //! form or think specials in the vocab; default is on.
    set_thinking_(chat, on)
}

def render_assistant_calls(model : Model; var chat : ChatSession; text : string; calls : array<string>; var out : array<int64>) {
    //! ``render_assistant``'s tool-calling twin: replay an assistant turn that emitted tool calls
    //! (verbatim ``\{"name":…,"arguments":…}`` objects) plus any ``text`` alongside.
    render_assistant_calls_(model, chat, text, calls, out)
}

def respond(model : Model; var chat : ChatSession; params : SamplingParams;
            blk : block<(piece : string) : bool>) : string {
    //! Generate the assistant's reply to the queued user message, streaming pieces through the
    //! trailing block (return ``false`` to stop early). Terminates the turn in the KV cache and
    //! appends both turns to ``chat.history``; returns the full reply text.
    return respond_(model, chat, params, blk)
}
