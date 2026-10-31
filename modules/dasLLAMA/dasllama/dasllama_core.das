options gen2
options _comment_hygiene = true
options _dasllama_internal = true

//! dasLLAMA — CPU LLM inference over GGUF models, in pure daslang. This module is the public API:
//! ``load_model`` -> ``create_session`` -> ``encode`` / ``eval`` / ``sample`` (or the one-call
//! streaming ``generate``), plus the chat layer (``create_chat`` / ``add_user`` / ``respond``).
//! Requiring it also registers every supported architecture (Llama-2/3, Mistral-3, the
//! Qwen-2/3 dense, VL and MoE families, the Qwen3.5/3.6 DeltaNet hybrids, GLM-4.5 MoE,
//! Phi-3, Gemma-2/3/4, gpt-oss) — ``load_model`` dispatches on GGUF metadata, so the same
//! program runs any of them; ``arch_names()`` lists the live registry.
//!
//! Run model programs with ``-jit``: interpreted inference is far too slow. Wrap the model work
//! in ``with_job_que()`` from ``daslib/jobque_boost`` so the kernels can thread. Call
//! ``setup_dasllama_jobque()`` inside it, before the first ``load_model``, ``generate`` or ``eval``:
//! the engine panics on a queue nobody configured, because a bare queue runs ~200x slower.
//! ``with_dasllama_jobque()`` does both in one call.
//! Declare ``options stack = 524288``: the forward chain outgrows the 16K default, and a required
//! module cannot raise the program's stack for you. See ``examples/dasLLAMA/run.das`` and
//! ``chat.das`` for the canonical shape.
//!
//! This is the engine with no C++ module beyond the daslang builtins. ``dasllama/dasllama`` adds
//! the audio and image file decoders, which need the ``audio`` and ``stbimage`` modules.

module dasllama_core shared public

require dasllama/dasllama_lint public
require dasllama/dasllama_transformer public   // the engine + arch registry; every arch [init] fires
require dasllama/dasllama_image   // the prepared-image cache: load_model maps it when present
require dasllama/dasllama_prefix public        // page-granular prefix cache over the paged pool
require dasllama/dasllama_chat public          // layer 2: chat sessions over the same engine
require dasllama/dasllama_tower public         // shared encoder-tower pieces (row math, activations, prof rail)
require dasllama/dasllama_audio public         // audio tower (mel frontend + whisper encoder + projectors)
require dasllama/dasllama_vision public        // image preprocessing rail (geometry, letterbox, normalize)
require dasllama/dasllama_vision_embedder public   // the vision embedder of whatever family (gemma4uv / gemma4v) — image soft tokens
require dasllama/dasllama_audio_embedder public    // the audio encoder of whatever family (gemma4a) — audio soft tokens
require dasllama/dasllama_asr public           // uniform ASR surface (whisper today; parakeet/qwen3-asr land behind the same sniff)
require dasllama/dasllama_vad public           // Silero-VAD (voice-activity detection): segments + streaming iterator
require dasllama/dasllama_wav public           // mono f32 PCM to WAV / PCM16 bytes and files
require dasllama/dasllama_tts public           // uniform TTS surface (KittenTTS, Kokoro, Pocket TTS): load_tts_model / synthesize / synthesize_stream / tts_register_voice
require dasllama/dasllama_translate public     // the translation surface (Bergamot): load_translator / translate / translator_langs
require dasllama/dasllama_config public        // the explicit .dlim bake configuration — the --config / identity contract
require dasllama/dasllama_math                 // engine-level knobs (wrapped below, not re-exported)
require dasllama/dasllama_par                  // the single-thread gate of the dispatch macros (wrapped below, not re-exported)
require dasllama/dasllama_gpu_tier             // GPU tier status + marks (wrapped below, not re-exported)
require dasllama/dasllama_gpu_resident         // resident slot-switch/pin surface (wrapped below, not re-exported)

// This facade owns the good names; the engine spells each wrapped function with a trailing
// underscore (load_model_, eval_, ...) so the public re-export above and the documented stub
// below never collide at a call site.

// ===== Model, session, tokenizer =====

def setup_dasllama_jobque() {
    //! Configure the job queue for dasLLAMA's fork/join matmul dispatch: pooled fork contexts, batched dispatch, the worker spin-before-park window (``jobque_spin_us``; 0 disables).
    //! Call INSIDE ``with_job_que()``, before the first ``generate``/``eval``. The fork-context pool is PER CONTEXT:
    //! a ``new_thread`` body that runs inference calls this again, or every parallel kernel clones the whole program (90x slower).
    setup_dasllama_jobque_()
}

def with_dasllama_jobque(blk : block<void>) {
    //! Run ``blk`` inside a fresh ``with_job_que()`` that ``setup_dasllama_jobque()`` has configured — the one-call form of the pair.
    //! The queue lives for the block only; a program that keeps its own queue calls ``setup_dasllama_jobque()`` inside it instead.
    with_dasllama_jobque_(blk)
}

def load_model(path : string; mode : QuantMode = QuantMode.fp32) : Model {
    //! Load a model AND its tokenizer from a GGUF file — architecture and tokenizer backend are
    //! auto-selected from metadata; ``mode`` picks the weight quantization. Q8 loads cache a PREPARED
    //! IMAGE beside the gguf for millisecond reloads (``DASLLAMA_IMAGE=0`` disables); under an active Metal mode the image is the BLOB-ONLY metal flavor (CPU inference against it panics).
    return <- load_model_cached(path, mode)
}

def create_session(model : Model; kv_dtype : KVDtype = KVDtype.f16) : Session {
    //! Create a fresh session (KV cache + scratch) sized to ``model.config.seq_len`` — one model, many independent conversations.
    //! ``kv_dtype`` picks the KV-cache codec: ``f16`` (default), ``q8_0`` (whole 32-blocks a head), ``tq4`` (a power-of-two head).
    //! A codec the model cannot serve steps down to ``q8_0``, else ``f16``, with one logged line - ``kv_codec_served`` says which.
    return <- create_session_(model, kv_dtype, kv_dtype)
}

def create_device_session(model : Model; kv_dtype : KVDtype = KVDtype.f16) : Session {
    //! Create a DEVICE-HOME session: its K/V cache lives only in the armed GPU driver's region - no host cache, nothing over the bus per token.
    //! Hold at most ``gpu_device_sessions()`` live ones, in the driver's codec (``gpu_device_session_dtype``).
    //! A codec the model cannot serve steps down as ``create_session``'s does. A call the device cannot serve panics.
    return <- create_device_session_(model, kv_dtype)
}

def create_kv_pool(model : Model; page_rows : int64 = 64l; kv_dtype : KVDtype = KVDtype.f16) : KVPool {
    //! Create a caller-owned PAGED KV pool over ``model``'s cache geometry: sessions over it allocate pages of ``page_rows``
    //! positions on demand instead of the full ``seq_len`` slab, so many sessions share one elastic pool; keep it alive as
    //! long as sessions live. A codec the model cannot serve steps down as ``create_session``'s does.
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

def create_prefix_cache(max_groups : int64 = 0l; max_states : int64 = DEFAULT_PREFIX_STATES; max_state_bytes : int64 = 0l) : PrefixCache {
    //! A prefix cache for one ``create_kv_pool`` pool: streams donate KV pages (``prefix_insert``), later requests
    //! with the same prefix attach them (``prefix_attach``); ``max_groups`` caps the pages (0 = unbounded). A recurrent
    //! model keeps whole checkpoints instead: ``max_states`` within ``max_state_bytes`` (0 = the count alone).
    return <- create_prefix_cache_(max_groups, max_states, max_state_bytes)
}

def prefix_checkpoint_at(cache : PrefixCache; pool : KVPool; session : Session; prompt : array<int64>; matched : int64; stable_at : int64 = 0l) : int64 {
    //! Where a recurrent ``session``'s prefill of ``prompt`` should stop for a ``prefix_insert``, after ``prefix_attach``
    //! matched ``matched`` tokens: ``stable_at`` (``render_turn_marked``'s opening) when the caller knows it, else the
    //! longest opening an earlier checkpoint shares. 0 = nowhere - always on a model with no recurrent state.
    return prefix_checkpoint_at_(cache, pool, session, prompt, matched, stable_at)
}

def prefix_match_len(cache : PrefixCache; pool : KVPool; prompt : array<int64>) : int64 {
    //! How many leading positions of ``prompt`` a ``prefix_attach`` would attach right now, attaching nothing: a probe
    //! for a caller deciding what a request must bring (a media span's rows). 0 on a recurrent model's cache.
    return prefix_match_len_(cache, pool, prompt)
}

def prefix_attach(var cache : PrefixCache; var pool : KVPool; var session : Session; prompt : array<int64>) : int64 {
    //! Attach the longest cached prefix of ``prompt`` to a FRESH paged ``session`` of ``pool``:
    //! matched pages join the session's block table and ``n_past`` advances past them, so the
    //! caller prefills only the tail. Returns the matched count, capped one token short of the prompt.
    return prefix_attach_(cache, pool, session, prompt)
}

def prefix_insert(var cache : PrefixCache; var pool : KVPool; var session : Session; tokens : array<int64>; preview : string = ""; opening : bool = false) : string {
    //! Donate a session's KV pages to the cache: ``tokens`` is its EVALED history (the first ``n_past`` rows exist), every
    //! full page not already cached survives ``release_kv_pages``, ``preview`` labels the chain. A recurrent model donates one
    //! checkpoint at ``n_past`` (``opening``: taken between prefill calls, kept ahead of tails); "" = held, else why not.
    return prefix_insert_(cache, pool, session, tokens, preview, opening)
}

def prefix_chain_list(cache : PrefixCache) : array<PrefixChain> {
    //! Snapshot of the cache's donated chains for dashboards: per donation — page-covered token
    //! count, live pages, hit count, born/last-hit ticks, and the caller-provided preview.
    return <- prefix_chain_list_(cache)
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

def eval_segments(model : Model; var sessions : array<Session?>; toks : array<array<int64>>) {
    //! Several sessions' prompt chunks as ONE prefill: segment i runs ``toks[i]`` at ``sessions[i]``'s position and
    //! advances it, each segment's logits landing in its own session. Where the serving GPU prefills, the chunks share
    //! one window; elsewhere each runs as ``eval`` would. Distinct sessions, no empty chunk; no segments is a no-op.
    eval_segments_(model, sessions, toks)
}

def eval_embd(model : Model; var session : Session; embd : array<float>; npos : int64; non_causal : bool = false) {
    //! ``eval``'s embedding-input twin: prefill ``npos`` pre-built embedding rows (``npos × dim``,
    //! token-major) at the session's current position and advance it — the multimodal splice entry.
    //! ``non_causal`` marks the whole call as one image span (gemma vision); audio rows stay causal.
    eval_embd_(model, session, embd, npos, non_causal)
}

def eval_embd_body(model : Model; var session : Session; embd : array<float>; npos, span_lo, span_hi : int64; grid : int2) {
    //! Prefill ``npos`` rows built by ``media_body_rows``: [span_lo, span_hi) a non-causal span (empty = every row
    //! causal), roped on a merged image ``grid`` where one is given.
    eval_embd_body_(model, session, embd, npos, span_lo, span_hi, grid)
}

def media_body_rows(model : Model; var session : Session; head : array<int64>; rows : array<float>; n_media, src_w : int64;
                    tail : array<int64>; var embd : array<float>) {
    //! One prefill body's rows for ``eval_embd_body`` on ``session``: ``head`` text, ``n_media`` media rows of width ``src_w``, ``tail`` text.
    media_body_rows_(model, session, head, rows, n_media, src_w, tail, embd)
}

def eval_embd_span(model : Model; var session : Session; embd : array<float>; npos, span_lo, span_hi : int64) {
    //! ``eval_embd`` for a prompt carrying one NON-CAUSAL image span: rows ``[span_lo, span_hi)``
    //! prefill with every query attending the whole span, the text around them causally — the
    //! gemma vision decode shape (non-causal media chunk).
    eval_embd_span_(model, session, embd, npos, span_lo, span_hi)
}

def eval_embd_span_mrope(model : Model; var session : Session; embd : array<float>; npos, span_lo, span_hi : int64; grid : int2) {
    //! ``eval_embd_span``'s qwen mrope twin: the span rows rope as a ``grid``-shaped merged image
    //! (position advance ``max(grid.x, grid.y)``, tracked on the session for every later eval);
    //! same span mask, only the angles differ. Panics without ``rope.dimension_sections``.
    eval_embd_span_mrope_(model, session, embd, npos, span_lo, span_hi, int64(grid.x), int64(grid.y))
}

def create_batch_workspace(model : Model) : BatchWorkspace {
    //! Create the caller-owned scratch that ``eval_batch`` steps through — one per concurrent batch,
    //! reused across calls (buffers grow to the largest batch seen). Holds no session state: the
    //! sessions keep their own positions, caches and logits.
    return <- create_batch_workspace_(model)
}

def eval_batch(model : Model; var ws : BatchWorkspace; var sessions : array<Session?>; tokens : array<int64>) {   // `var sessions`: rows mutate THROUGH the pointers; a const array constifies the pointees
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
    return generate_embd_(model, session, embd, npos, params, max_tokens, 0l, 0l, int2(0), blk)
}

def generate_embd(model : Model; var session : Session; embd : array<float>; npos : int64;
                  params : SamplingParams; max_tokens : int64; span_lo, span_hi : int64;
                  blk : block<(id : int64; piece : string) : bool>) : int64 {
    //! ``generate_embd`` with a NON-CAUSAL span: rows ``[span_lo, span_hi)`` prefill as an image
    //! span (every query attends the whole span), the text around them causally — the gemma
    //! vision decode shape. The chat layer's image turns run on this.
    return generate_embd_(model, session, embd, npos, params, max_tokens, span_lo, span_hi, int2(0), blk)
}

def generate_embd(model : Model; var session : Session; embd : array<float>; npos : int64;
                  params : SamplingParams; max_tokens : int64; span_lo, span_hi : int64; grid : int2;
                  blk : block<(id : int64; piece : string) : bool>) : int64 {
    //! The span form with an mrope GRID (qwen vision): the span rows rope at grid positions
    //! (merged nx×ny; the sequence position advances by max(nx, ny)) — the qwen image shape.
    return generate_embd_(model, session, embd, npos, params, max_tokens, span_lo, span_hi, grid, blk)
}

// ===== Embeddings =====

def embed(model : Model; text : string; pooling : int64 = -1l; dims : int64 = 0l) : array<float> {
    //! L2-normalized sentence embedding of ``text`` (``model.config.dim`` floats, or the leading ``dims`` renormalized - the
    //! Matryoshka read) from the decoder's post-final-norm hidden rows, pooled the way the file's ``pooling_type`` asks (mean; the
    //! last row for Qwen3-Embedding) unless ``pooling`` names one (``POOLING_MEAN`` / ``POOLING_CLS`` / ``POOLING_LAST``; -1 = the file's).
    return <- embed_(model, text, pooling < 0l ? model.config.pooling : pooling, dims, embed_session_dtype(model), moe_gpu_resident_scratch_serves(model))
}

def embed_window_rows(model : Model) : int64 {
    //! The rows one ``embed_batch`` window takes on ``model``: its texts share the armed driver's scratch region where the
    //! driver serves the model, so a window holds at most the region's context; ``EMBED_WINDOW_ROWS`` elsewhere.
    return embed_window_rows_(model)
}

def gpu_free_regions() : int64 {
    //! The armed driver's K/V regions no live device-home session holds - where an in-handler forward (an embedding on a chat
    //! model) can take one; 0 with no driver armed.
    return gpu_free_regions_()
}

def embed_session_dtype(model : Model) : KVDtype {
    //! The K/V codec ``embed`` and ``embed_batch`` make ``model``'s sessions in: the armed whole-model driver's mirror codec where
    //! that driver serves ``model``, so it serves the forward (it serves no session of another codec); ``f32`` elsewhere.
    return embed_session_dtype_(model)
}

def embed_batch(model : Model; texts : array<string>; pooling : int64 = -1l; dims : int64 = 0l) : array<array<float>> {
    //! ``embed`` over many texts at once, one unit vector a text in input order: the texts' forwards share a prefill window,
    //! so a batch costs far less than a call a text. ``pooling`` and ``dims`` read as in ``embed``.
    return <- embed_batch_(model, texts, pooling < 0l ? model.config.pooling : pooling, dims, embed_window_rows(model), embed_session_dtype(model), moe_gpu_resident_scratch_serves(model))
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

def create_chat(model : Model; var embedder : VisionEmbedder; system : string = ""; max_new : int64 = 256l; kv_dtype : KVDtype = KVDtype.f16) : ChatSession {
    //! Start a VISION-CAPABLE conversation: takes ownership of ``embedder`` (moved in — load with
    //! ``load_vision_embedder``, which sniffs the mmproj's family) so ``add_user_image`` can splice
    //! image soft tokens into its turns. Panics on a mismatched pair or a family with no image markers.
    return <- create_chat_(model, embedder, system, max_new, kv_dtype)
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

def add_user_image(var chat : ChatSession; img : VisionImage) {
    //! Queue an image for the next ``respond`` — geometry, letterbox and the embedder run NOW,
    //! spliced at the head of the next user turn. ONE image per turn; needs a chat created with
    //! ``create_chat(model, embedder)``; call inside ``with_job_que()`` (decode: dasllama_vision_io).
    add_user_image_(chat, img)
}

def add_user_image_rows(model : Model; var chat : ChatSession; var rows : array<float>; n_rows : int64; grid : int2) {
    //! Queue PRE-ENCODED image soft-token rows (what ``encode_image`` emits — deepstack models:
    //! ``(1+n)·dim``-wide, length-checked) with the family's mrope ``grid`` ((0,0) = sequential)
    //! for the next ``respond``. MOVES ``rows``; works on a plain chat — no embedder needed.
    add_user_image_rows_(model, chat, rows, n_rows, grid)
}

def set_chat_date(date : string) {
    //! Pin the date a Llama-3.1+ system turn states (``"26 Jul 2024"``); ``""`` returns it to the day the turn
    //! renders. For a prompt that must not move with the calendar - a pinned test stream, a benchmark.
    set_chat_date_(date)
}

def add_user_span(model : Model; var chat : ChatSession; key : uint64; n_rows : int64; text_before : int; is_image : bool; bare : bool = false) {
    //! Mark the pending user turn as carrying a media span inline: ``render_turn`` and ``render_assistant`` then lay
    //! ``n_rows`` position ids (``media_position_id`` over the media's content ``key``) where the media sits,
    //! ``text_before`` bytes into the turn's text. The rows themselves are the caller's to eval at those positions.
    add_user_span_(model, chat, key, n_rows, text_before, is_image, bare)
}

def add_user_audio_rows(model : Model; var chat : ChatSession; var rows : array<float>; n_rows : int64; bare : bool = false) {
    //! Queue PRE-ENCODED audio soft-token rows (what ``encode_audio`` emits — ``dim``-wide, length-checked) for the
    //! next ``respond``. MOVES ``rows``; works on a plain chat, no tower needed. Pass the encoder's
    //! ``audio_span_bare`` as ``bare``: an ultravox span takes no marker, so a stock template serves it.
    add_user_audio_rows_(model, chat, rows, n_rows, bare)
}

def encode_image(embedder : VisionEmbedder; var scratch : VisionState; img : VisionImage;
                 tag : string; var out : array<float>) : int64 {
    //! The whole image path in one call — geometry, letterbox, normalize, encode — into ``out`` =
    //! npos × proj_dim soft tokens (returns npos). ``add_user_image`` is this plus queueing onto a
    //! chat turn; a scheduler owning its own embedder calls this. ``tag`` names the vision dump.
    return encode_image_(embedder, scratch, img, tag, out)
}

def encode_audio(embedder : AudioEmbedder; var scratch : AudioState; samples : array<float> | #;
                 var out : array<float>) : int64 {
    //! ``encode_image``'s audio twin: 16 kHz mono samples through the carried family's encoder
    //! into ``out`` = npos × proj_dim soft tokens (returns npos). A scheduler owning its own
    //! embedder calls this; the server's media worker is the worked example.
    return encode_audio_(embedder, scratch, samples, out)
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

def render_turn_marked(model : Model; chat : ChatSession) : RenderedTurn {
    //! ``render_turn`` with its opening marked: ``opening`` leading tokens - BOS, prelude and the system turn of a
    //! conversation's first turn - are the same for any user text, so a prefix cache can checkpoint there.
    return <- render_turn_marked_(model, chat)
}

def render_turn_image(model : Model; chat : ChatSession; var head : array<int64>; var tail : array<int64>; text_before : int = 0) {
    //! ``render_turn``'s IMAGE twin: the two token spans that bracket the image soft-token splice — ``head`` before the
    //! rows, ``tail`` after. Render-only (no KV, no embedder): the rows come from ``encode_image``.
    //! ``text_before`` bytes of the user text render ahead of the image (0: the image leads the turn).
    render_turn_image_(model, chat, head, tail, text_before)
}

def render_turn_audio(model : Model; chat : ChatSession; var head : array<int64>; var tail : array<int64>; text_before : int = 0) {
    //! ``render_turn``'s AUDIO twin: the same two-span contract around the audio soft-token splice
    //! (the template's audio span markers). Render-only (no KV, no encoder): the rows come from
    //! ``encode_audio``, and a scheduler prefills head, rows, tail. ``text_before`` as in ``render_turn_image``.
    render_turn_audio_(model, chat, head, tail, text_before)
}

def render_assistant(model : Model; var chat : ChatSession; text : string; var out : array<int64>; reasoning : string = "") {
    //! ``add_assistant``'s render half: appends the exact token stream a known reply prefills to ``out``
    //! WITHOUT running the model, advancing the transcript like ``add_assistant`` (a ``create_chat_renderer``
    //! chat replays history with no KV). ``reasoning`` renders in its think block under ``set_preserve_thinking``.
    render_assistant_(model, chat, text, out, reasoning)
}

def render_user(model : Model; var chat : ChatSession; var out : array<int64>) {
    //! Render the pending user turn alone and consume it, with no generation prompt: the turn a user message
    //! that follows another user message makes, as every template writes them one after the other. Precondition:
    //! a user message pending; a no-op otherwise.
    render_user_(model, chat, out)
}

def render_close(model : Model; chat : ChatSession) : array<int64> {
    //! The tokens that TERMINATE an assistant turn (what ``respond`` evals after the reply) — for
    //! schedulers that close a finished stream's turn themselves.
    return <- render_close_(model, chat)
}

def set_tools(var chat : ChatSession; var tools : array<string>) {
    //! Declare the conversation's tools (verbatim OpenAI ``tools[]`` JSON objects, moved in)
    //! BEFORE the first turn renders — the system turn carries the family's tool block. Families
    //! with no tool format (``tmpl.tool_mode`` none) ignore them.
    set_tools_(chat, tools)
}

def add_tool_results(var chat : ChatSession; results : array<string>) {
    //! Queue tool results as the next pending turn — the reply to an assistant turn that called
    //! tools. Call in place of ``add_user``, then ``respond``/``render_turn`` as usual.
    add_tool_results_(chat, results)
}

def add_tool_results(var chat : ChatSession; results : array<string>; names : array<string>) {
    //! ``add_tool_results`` with each result's function NAME known (e.g. mapped back from its
    //! ``tool_call_id``) — the name-spelling framings (harmony, gemma-4) then pair results
    //! correctly even out of call order. An empty name slot falls back to the last call set.
    add_tool_results_(chat, results, names)
}

def parse_calls(chat : ChatSession; reply : string) : ToolReply {
    //! Parse a complete reply per the model family's wire format into a ToolReply — the reasoning
    //! span, the content, and the calls with arguments normalized to JSON object text. A family with
    //! no tool format returns the reasoning/content split alone (safe on every reply); the buffered twin of the server's streaming parse.
    return <- parse_calls_(chat, reply)
}

def set_thinking(var chat : ChatSession; on : bool) {
    //! Toggle reasoning for a hybrid thinking model (Qwen3 family): ``false`` appends the
    //! template's empty think block so the model answers directly. No-op without a suppress
    //! form or think specials in the vocab; default is on.
    set_thinking_(chat, on)
}

def set_preserve_thinking(var chat : ChatSession; on : bool) {
    //! ``true`` makes every replayed assistant turn carry its think block - its ``reasoning`` between the
    //! template's markers, the empty block when it had none - so a replayed transcript renders the tokens
    //! the model generated and a prefix cache matches through it. No-op where ``set_thinking`` is; default off.
    set_preserve_thinking_(chat, on)
}

def set_reasoning_effort(var chat : ChatSession; effort : string) {
    //! The reasoning level a harmony (gpt-oss) system turn asks for: ``low``, ``medium`` or ``high``. Unset, the
    //! level follows ``set_thinking``: ``medium`` on, ``low`` off - the template has no off, and ``low`` is the
    //! shortest analysis it writes. No-op on every other family.
    set_reasoning_effort_(chat, effort)
}

def render_assistant_calls(model : Model; var chat : ChatSession; text : string; calls : array<string>; var out : array<int64>; reasoning : string = "") {
    //! ``render_assistant``'s tool-calling twin: replay an assistant turn that emitted tool calls
    //! (verbatim ``\{"name":…,"arguments":…}`` objects) plus any ``text`` alongside.
    render_assistant_calls_(model, chat, text, calls, out, reasoning)
}

def effective_stop_ids(chat : ChatSession) : array<int64> {
    //! Every stop id for the NEXT generation as one list: the template's stops plus the
    //! thinking-off extras while thinking is off. A scheduler that stops streams itself reads
    //! ``turn_stop_ids`` and runs the extras through ``nothink_stop_here`` instead.
    return <- effective_stop_ids_(chat)
}

def turn_stop_ids(chat : ChatSession) : array<int64> {
    //! The template's own stop ids for the NEXT generation (``<turn|>``, ``<eos>`` and their
    //! kin): a token among them ends the turn outright. The thinking-off extras are the
    //! ``make_nothink_guard`` guard's.
    return <- turn_stop_ids_(chat)
}

def make_nothink_guard(chat : ChatSession) : NothinkStopGuard {
    //! The instruct-mode stop guard for ``chat``'s next turn over the family's channel markers
    //! (gemma-4): unarmed on a thinking turn. Feed every sampled token through
    //! ``nothink_stop_here`` after the ``turn_stop_ids`` check.
    return <- make_nothink_guard_(chat)
}

def nothink_stop_here(var guard : NothinkStopGuard; id : int64; piece : string) : bool {
    //! true = ``id`` ends the turn: a guarded channel marker sampled after the reply's first
    //! content piece. A marker before content opens a leading thought the reply matcher splits.
    return nothink_stop_here_(guard, id, piece)
}

def split_reasoning(chat : ChatSession; reply : string) : ThinkSplit {
    //! Split a complete reply at its reasoning boundary per the model family's reply format
    //! (``<think>`` pair, Harmony channels, gemma-4's thought channel). Both halves come back
    //! stripped when a reasoning span is found; a reply with no reasoning passes through untouched.
    return split_reasoning_(chat, reply)
}

def make_think_stream(chat : ChatSession) : ThinkStream {
    //! The incremental reasoning/content splitter for ``chat``'s next turn (``think_feed`` per
    //! piece, ``think_finish`` at the end). Armed when the turn thinks, and on an instruct turn of
    //! a family with guarded channel markers (gemma-4); else a pass-through stream.
    return make_think_stream_(chat)
}

def think_budget_marks(model : Model; chat : ChatSession) : ThinkBudgetMarks {
    //! The token marks a reasoning budget acts on for ``chat``'s next turn (``ThinkBudgetMarks``): the span's open token,
    //! the close tokens a scheduler forces at a stream's ``think_budget`` (the family's close special; harmony's end mark
    //! and final-channel header), and whether the prompt opened the span. Empty where the turn does not think.
    return <- think_budget_marks_(model, chat)
}

def think_drain(var ts : ThinkStream; full : string) : ThinkSplit {
    //! Drain a COMPLETE reply through the splitter in one call: feed + finish + the strip rule
    //! (both halves strip when a reasoning span was consumed). The buffered-response twin of the
    //! ``think_feed``/``think_finish`` streaming pair.
    return think_drain_(ts, full)
}

def think_feed(var ts : ThinkStream; piece : string; var reasoning, content : string&) {
    //! Feed one streamed piece through the splitter; the out-strings are OVERWRITTEN with this
    //! piece's reasoning/content deltas (either may be empty while a partial marker is held).
    think_feed_(ts, piece, reasoning, content)
}

def think_finish(var ts : ThinkStream; var reasoning, content : string&) {
    //! Flush the splitter at end-of-generation (OVERWRITES the out-strings with the final deltas):
    //! an unclosed reasoning span classifies as reasoning — the truncated-tail rule.
    think_finish_(ts, reasoning, content)
}

def respond(model : Model; var chat : ChatSession; params : SamplingParams;
            blk : block<(piece : string) : bool>) : string {
    //! Generate the assistant's reply to the queued user message, streaming pieces through the
    //! trailing block (return ``false`` to stop early). Terminates the turn in the KV cache and
    //! appends both turns to ``chat.history``; returns the full reply text.
    return respond_(model, chat, params, blk)
}

// ===== Operations: dlim images, GPU tier, dispatch =====

typedef DlimImageInfo = dasllama_image::DlimImageInfo
typedef GpuTierStatus = dasllama_gpu_tier::GpuTierStatus
typedef GpuTierWant = dasllama_gpu_tier::GpuTierWant
typedef GpuModelMarks = dasllama_gpu_resident::GpuModelMarks

def gpu_model_marks_init() : dasllama_gpu_resident::GpuModelMarks {
    //! A fresh no-model marks holder — one per model slot; the marks save/restore pair swaps
    //! tier state across switches. (A generated constructor is not callable through the
    //! facade typedef, so the holder is minted here.)
    return <- gpu_model_marks_init_()
}

def dlim_inventory(gguf_path : string) : array<dasllama_image::DlimImageInfo> {
    //! List the prepared images (``.dlim``) minted beside ``gguf_path`` — per image: file, lane (the file name's
    //! ``metal`` / ``cpu-<class>`` / ``vulkan`` / family segment, "" for a hash-named legacy image), bytes, version,
    //! identity, verdict (``CURRENT`` loads; ``STALE vN`` an older version; ``OTHER`` another bake config or box; ``FOREIGN`` another flavor).
    return <- dlim_inventory_(gguf_path)
}

def dlim_clean(gguf_path : string; apply : bool; keep_other : bool = false) : tuple<total : int; stale : int; removed : int; freed : int64> {
    //! Garbage-collect ``STALE`` images beside ``gguf_path`` and hash-named legacy images of another identity; a
    //! lane-named ``OTHER`` and every ``FOREIGN`` image stay (their own loads re-bake them in place). ``apply = false``
    //! only reports, ``true`` removes; ``keep_other`` spares the legacy others too. Returns (total, stale, removed, freed bytes).
    return dlim_clean_(gguf_path, apply, keep_other)
}

def gpu_tier_status() : dasllama_gpu_tier::GpuTierStatus {
    //! Snapshot of the GPU tier in force: VRAM budget, resident/streamed layer counts, dense
    //! planes, engage/decline state — the server's GPU badge reads this.
    return gpu_tier_status_()
}

def gpu_resident_prefills() : int64 {
    //! How many prompt windows the armed whole-model driver has prefilled in this context - the device's served witness.
    return gpu_resident_prefills_()
}

def gpu_cpu_passes() : array<tuple<reason : string; words : string; count : int64>> {
    //! Calls the armed GPU path handed back to the CPU rails since the model armed, one row per
    //! reason that fired: its name, the same in words a status page prints, the count. Empty means the device served every call.
    return <- gpu_cpu_passes_()
}

def set_dispatch_worker_limit(v : int) {
    //! Cap the kernel-dispatch worker count (0 = no limit, all job-que workers). Latched by
    //! ``setup_dasllama_jobque`` — set it before that call.
    set_dispatch_worker_limit_(v)
}

def get_dispatch_worker_limit() : int {
    //! The dispatch worker cap in force (0 = no limit) — ``set_dispatch_worker_limit``'s read half.
    return get_dispatch_worker_limit_()
}

def set_jobque_spin_us(v : int64) {
    //! The workers' spin-before-park window in microseconds (0 = park at once). Latched by
    //! ``setup_dasllama_jobque`` — set it before that call; the default is 30 ms on every platform.
    set_jobque_spin_us_(v)
}

def get_jobque_spin_us() : int64 {
    //! The CPU spin window — ``set_jobque_spin_us``'s read half. The window the pool runs at
    //! the moment is ``get_jobque_spin_in_force``.
    return get_jobque_spin_us_()
}

def set_jobque_spin_gpu_us(v : int64) {
    //! The workers' spin-before-park window while the whole-model GPU driver serves the loaded
    //! model (default 500 us; a worker still spinning at the step's submit starves the driver).
    //! A live queue takes it the moment the driver arms or drops; a later queue at its setup.
    set_jobque_spin_gpu_us_(v)
}

def get_jobque_spin_gpu_us() : int64 {
    //! The GPU-served spin window — ``set_jobque_spin_gpu_us``'s read half.
    return get_jobque_spin_gpu_us_()
}

def get_jobque_spin_in_force() : int64 {
    //! The spin window the pool runs right now: the GPU window while the whole-model driver
    //! serves the loaded model, the CPU window otherwise.
    return get_jobque_spin_in_force_()
}

def set_single_thread(on : bool) {
    //! Run every kernel on the calling thread: no dispatch to the job queue at all, whatever queue exists.
    //! Takes effect at the next kernel, on THIS context (a ``new_thread`` body sets its own; ``DASLLAMA_SINGLE_THREAD=1``
    //! sets every context). The browser build's baseline, and the figure that says what the pool buys on any box.
    set_single_thread_(on)
}

def get_single_thread() : bool {
    //! Whether every kernel runs on the calling thread — ``set_single_thread``'s read half.
    return get_single_thread_()
}

def select_matmul_backend_for_load() : bool {
    //! Select the best matmul backend for the NEXT model load (honors a pin). Returns true when
    //! the choice needs the loader to repack weights into its interleaved layout.
    return select_matmul_backend_for_load_()
}

def kernel_backend_available(name : string) : bool {
    //! True when kernel backend ``name`` is registered AND its availability witness passes on
    //! this box — the detection probe behind defaults-first backend selection (the vulkan
    //! witness enumerates devices once and caches its verdict). Unknown names are false.
    return kernel_backend_available_(name)
}

def set_gpu_tier_want(want : dasllama_gpu_tier::GpuTierWant) {
    //! Record the GPU tier request the NEXT ``moe_gpu_tier_arm``/``load_model`` honors — the
    //! programmatic form of the core ``DASLLAMA_GPU_*`` knobs (an env var present overrides its
    //! field; the classifier/dense-arm knobs are env-only). A zero want keeps the load off the device.
    set_gpu_tier_want_(want)
}

def moe_gpu_tier_arm() : bool {
    //! Arm the recorded tier want so a GPU backend can install its hooks — call between
    //! ``set_gpu_tier_want`` and ``load_model``. False when no backend claims the request.
    return moe_gpu_tier_arm_()
}

def gpu_home() : string {
    //! The discrete GPU home whose seats the tier holds, ``"vulkan"`` or ``"cuda"``: the environment's
    //! spelling (``DASLLAMA_CUDA``) until ``set_gpu_home`` moves it.
    return gpu_home_()
}

def gpu_homes() : array<string> {
    //! The discrete homes this build carries - the names ``set_gpu_home`` takes.
    return <- gpu_homes_()
}

def set_gpu_home(name : string) : bool {
    //! Move the tier onto the named home before it arms - the program's twin of ``DASLLAMA_CUDA`` (dasllama-server's
    //! ``--gpu vulkan | cuda`` key): every seat back to its default, the shared installers and then the home's entry filling
    //! them. False, said in the log, for a home this build does not carry or a tier that already armed on another home.
    return set_gpu_home_(name)
}

def moe_gpu_weight_budget() : int64 {
    //! The armed backend's resident-weight VRAM budget in bytes (0 = no GPU backend armed).
    //! CAVEAT: the hook device-inits on first call — a reporting caller gates on
    //! ``gpu_tier_status().device`` being non-empty first (``supported`` must not be the gate).
    return moe_gpu_weight_budget_()
}

def mtp_drafter_sidecar(model_path : string) : string {
    //! The assistant drafter file a model ships beside it (gemma-4: `mtp-<stem>-Q8_0.gguf`, `<stem>`
    //! the file name before its quant token, an unsloth `-UD` tag dropped where the head has none), ""
    //! where there is none. A model with no NextN head of its own speculates through it once attached.
    return mtp_drafter_sidecar_(model_path)
}

def attach_mtp_drafter(model : Model; path : string) : bool {
    //! Attach the assistant drafter at `path` to `model` for speculative decode on the GPU driver that
    //! serves it (Metal or the Vulkan resident driver); false where every driver refused it (a drafter
    //! made for another target, a model the driver does not serve whole), each refusal logged.
    return empty(attach_mtp_drafter_(model, path))
}

def attach_mtp_sidecar(model : Model; model_path : string) : string {
    //! Attach the assistant drafter that sits beside the model at `model_path` (`mtp_drafter_sidecar`): the drafter's path
    //! where it attached, "" where none sits there or every driver refused it (each refusal logged).
    return attach_mtp_sidecar_(model, model_path)
}

def detach_mtp_drafter(model : Model) {
    //! Detach `model`'s assistant drafter, if one is attached; its rounds stop and plain decode serves.
    detach_mtp_drafter_(model)
}

def mtp_drafter_stats(model : Model) : MtpAssistantStats {
    //! The attached drafter's device rounds since its attach: drafts made, drafts accepted, rounds,
    //! and the verifies whose rows carried two streams or more. Zeros with no drafter attached.
    return mtp_drafter_stats_(model)
}

def mtp_capable(model : Model) : bool {
    //! Whether `model` can run a speculative round: a NextN head of its own, or an assistant
    //! drafter attached - what a scheduler's `mtp` asks for.
    return mtp_capable_(model)
}

def device_kv_max_rows(model : Model; kdt : KVDtype = KVDtype.f16; vdt : KVDtype = KVDtype.f16) : int64 {
    //! The rows of context the armed GPU driver holds a session's K/V for under these codecs - the
    //! model's own context where no driver bounds it. A server sizes its context by it: a blob-only
    //! model's session past it has no CPU stack to fall back to.
    return device_kv_max_rows_(model, kdt, vdt)
}

def gpu_dn_room(n : int64) {
    //! Tell the GPU driver how many sessions' recurrent state it keeps device-resident at once:
    //! a scheduler names its stream count before the first prefill, so no stream's deltanet state
    //! evicts under the batched step it rides. A driver with no such cache ignores it.
    gpu_dn_room_(n)
}

def gpu_slot_capture(var st : dasllama_gpu_resident::GpuModelMarks) : string {
    //! Capture the INSTALLED model's tier state into ``st`` right after its ``load_model`` and
    //! classify it — "gpu:resident" | "gpu:rails" | "cpu" — from the CAPTURED marks, never the
    //! process-global VRAM counter (it still holds the previous load's bytes on a later CPU load).
    return gpu_slot_capture_(st)
}

def gpu_slot_rearm(want : dasllama_gpu_tier::GpuTierWant; t : Model) : string {
    //! Re-arm the INSTALLED model onto the GPU tier — want, arm, resident upload (bake-slice
    //! path for a mapped vulkan-flavor ``t``). Its marks are installed and the device holds no other
    //! model (``moe_gpu_drop_model`` first). Returns "gpu:resident" | "gpu:rails" | "cpu".
    return gpu_slot_rearm_(want, t)
}

def moe_gpu_model_marks_save(var st : dasllama_gpu_resident::GpuModelMarks) {
    //! Save the INSTALLED model's GPU tier state into ``st`` and disarm it — one half of the
    //! multi-model slot switch (the engine's tier state is per-process, not per-model).
    moe_gpu_model_marks_save_(st)
}

def moe_gpu_model_marks_restore(var st : dasllama_gpu_resident::GpuModelMarks) {
    //! Install ``st`` as the per-model GPU tier state — ``moe_gpu_model_marks_save``'s inverse;
    //! ``st`` reads as no-model after.
    moe_gpu_model_marks_restore_(st)
}

def moe_gpu_hydrate_session(t : Model; var s : Session) {
    //! Pull ``s``'s host KV back from the device mirror when it owns live mirror rows (no-op
    //! otherwise). Run over every live session of the outgoing model BEFORE
    //! ``moe_gpu_drop_model`` — the drop destroys the mirror.
    moe_gpu_hydrate_session_(t, s)
}

def moe_gpu_drop_model() {
    //! Drop the INSTALLED model's whole device state (resident stacks, mirrors, VRAM) — the
    //! evict half of a slot switch. Caller guarantees quiescence: no step in flight, every live
    //! session hydrated first.
    moe_gpu_drop_model_()
}

def set_gpu_resident_regions(n : int64) {
    //! Ask the whole-model GPU driver for ``n`` K/V regions on the models loaded next, so ``n``
    //! sessions keep their K/V on the device at once - a server's stream count. One by default.
    set_gpu_resident_regions_(n)
}

def set_gpu_ctx_max(n : int64) {
    //! Cap the whole-model GPU driver's K/V context at ``n`` positions per region for the models
    //! loaded next (0 lifts it): a server's ``ctx``. The card's room still bounds it; a cap under the arming floor lowers
    //! the floor to the cap (never under 256): the cap is the shape the caller serves.
    set_gpu_ctx_max_(n)
}

def set_gpu_host_layers(n : int64) {
    //! Serve at least ``n`` routed layers' experts on the host - the first in layer order - on the whole-model GPU driver
    //! of a plain MoE loaded next, whatever the card holds (DASLLAMA_GPU_HOST_LAYERS in the environment): the host-layers
    //! form reached on any card. A negative ``n`` lifts the pin; ``gpu_host_layers_pin`` reads it.
    set_gpu_host_layers_(n)
}

def gpu_host_layers_pin() : int64 {
    //! The host-layers pin as set (``set_gpu_host_layers``), or -1 where none is - what a caller reads before it
    //! pins and hands back to ``set_gpu_host_layers`` after.
    return gpu_host_layers_pin_()
}

def set_gpu_kv_dtype(dt : KVDtype) {
    //! Ask the whole-model GPU driver to hold its K/V mirror in codec ``dt`` for the models loaded next - the codec of
    //! their sessions (``gpu_device_session_dtype`` reads what armed; ``q8_0`` is about half the ``f16`` mirror's bytes,
    //! ``tq4`` a quarter; ``f32`` reads as ``f16``). A model that cannot serve it serves what it can (``kv_codec_served``).
    set_gpu_kv_dtype_(dt)
}

def gpu_kv_dtype_pin() : tuple<pinned : bool; dt : KVDtype> {
    //! The in-process ask as ``set_gpu_kv_dtype`` left it: ``pinned`` false where no caller asked and ``DASLLAMA_GPU_KV``
    //! decides. A fixture reads it before it pins, then puts it back with ``set_gpu_kv_dtype`` or ``reset_gpu_kv_dtype``.
    return gpu_kv_dtype_pin_()
}

def reset_gpu_kv_dtype() {
    //! Lift the in-process ask ``set_gpu_kv_dtype`` made: ``DASLLAMA_GPU_KV`` decides the next load's mirror codec again.
    reset_gpu_kv_dtype_()
}
def gpu_ctx_pins() : tuple<ctx_max : int64; strict : bool> {
    //! The in-process context pins as set (``set_gpu_ctx_max``, ``set_gpu_ctx_strict``), apart from the
    //! environment's knobs: a fixture reads them before it pins and puts them back after its load.
    return gpu_ctx_pins_()
}

def set_gpu_ctx_strict(on : bool) {
    //! The ``set_gpu_ctx_max`` pin is the context asked, not a cap, for the models loaded next: a K/V
    //! mirror the card cannot hold at it declines the whole-model GPU driver instead of shortening the
    //! context, and the per-op rails serve the asked context (DASLLAMA_GPU_CTX_STRICT in the environment).
    set_gpu_ctx_strict_(on)
}

def gpu_resident_decline() : string {
    //! Why the whole-model GPU driver does not serve the model loaded last: its decline reason (with
    //! the remedy where one exists), or why it was never attempted. Empty when it serves.
    return gpu_resident_decline_()
}

def gpu_resident_experts_host() : bool {
    //! Whether the whole-model driver serving the model loaded last sums some routed layers' experts on the host (a
    //! hyper-connection MoE, a plain MoE the card does not hold whole, or one whose host-layers ask - ``set_gpu_host_layers``,
    //! ``DASLLAMA_GPU_HOST_LAYERS`` - moved them): the form whose speculative round pays at the NextN depth alone (`mtp_auto_arms`).
    return gpu_resident_experts_host_()
}

def gpu_device_sessions() : int64 {
    //! How many DEVICE-HOME sessions (``create_device_session``) the installed model serves at once:
    //! the armed driver's region count, 0 when the model is not served whole from the device.
    return gpu_device_sessions_()
}

def gpu_resident_mtp_device_home(model : Model) : bool {
    //! Whether ``model``'s self-speculation (MTP/NextN) serves device-home sessions: the draft head, its verify
    //! and a rejected round's rollback all run on the device, so a speculating scheduler may keep its streams'
    //! caches there. False keeps a speculating slot's sessions host-cached.
    return gpu_resident_mtp_device_home_(model)
}

def gpu_device_prefill_continues(model : Model) : bool {
    //! A device-home session of ``model`` prefills from a position past zero - a prompt in chunks,
    //! a turn over the turns before it. True for every model the driver serves: a recurrent hyper-connection
    //! model's n-gram ring rides its checkpoints and snapshots, so its continuation reads the history too.
    return gpu_device_prefill_continues_(model)
}

def gpu_device_prefill_segments(model : Model) : bool {
    //! A device-home scheduler's tick hands ``model``'s prefilling streams to one segmented window
    //! (``eval_segments``): the armed driver has the form and the model's chain holds several
    //! sessions a window. False where the driver would run each stream's chunk as its own window.
    return gpu_device_prefill_segments_(model)
}

def gpu_device_kv_park(var session : Session) : int64 {
    //! A FINISHED device-home session lets go of its K/V region and answers the claim its rows stay
    //! under (0 = none) - call it before every such session's ``delete``. Keep the claim and
    //! ``gpu_device_kv_adopt`` hands the rows to a later session: a prefix cache with no copy.
    return gpu_device_kv_park_(session)
}

def gpu_device_kv_adopt(var session : Session; claim : int64; npos : int64) : bool {
    //! Hand the first ``npos`` rows a kept ``claim`` holds to the fresh device-home ``session``, whose
    //! ``n_past`` becomes ``npos``. False when the rows are gone (another session took the region).
    return gpu_device_kv_adopt_(session, claim, npos)
}

def gpu_device_kv_parked_pos(claim : int64) : int64 {
    //! The position a kept ``claim``'s recurrent state stands at - the one row count
    //! ``gpu_device_kv_adopt`` may take on a recurrent model; 0 where the claim is gone or the
    //! model keeps no recurrent state.
    return gpu_device_kv_parked_pos_(claim)
}

def gpu_device_kv_checkpoint(session : Session) : bool {
    //! Keep a device-home recurrent ``session``'s state at its position beside its rows, so a later
    //! request that opens with exactly those tokens adopts it from the parked claim. False where the
    //! model keeps no recurrent state or the driver no checkpoints.
    return gpu_device_kv_checkpoint_(session)
}

def gpu_device_kv_checkpoints(claim : int64) : array<int64> {
    //! The positions a parked ``claim``'s checkpoints hold its recurrent state at (empty where it is dead).
    return <- gpu_device_kv_checkpoints_(claim)
}

def gpu_device_kv_export(model : Model; claim : int64; var host : Session; lo : int64; hi : int64) : bool {
    //! Copy the rows ``[lo, hi)`` the region of a live or parked ``claim`` holds down into the paged ``host``
    //! session's pages (grown to ``hi``), so a prefix cache keeps them past the region. False where the claim is
    //! gone, holds fewer rows, or the pool is not the driver's codec.
    return gpu_device_kv_export_(model, claim, host, lo, hi)
}

def gpu_device_kv_claim(session : Session) : int64 {
    //! The claim a live device-home ``session`` holds its region under (0 = none).
    return gpu_device_kv_claim_(session)
}

def gpu_device_kv_victim_claims(n : int) : array<int64> {
    //! The claims whose regions the next ``n`` fresh device-home sessions bind - the least recently used regions
    //! no live session holds - and so lose.
    return <- gpu_device_kv_victim_claims_(n)
}

def gpu_device_kv_parked_snapshot(claim : int64; at : int64; like : Session; var into : DnSnapshot) : DnSnapshot {
    //! The recurrent state a parked ``claim`` keeps at position ``at`` (where its state stands, or a checkpoint),
    //! copied off the device into ``into`` (moved in; ``prefix_take_spare`` hands one whose arrays keep their room)
    //! shaped as ``like``; ``pos`` -1 where none stands at ``at``.
    return <- gpu_device_kv_parked_snapshot_(claim, at, like, into)
}

def prefix_take_spare(var cache : PrefixCache) : DnSnapshot {
    //! A recurrent checkpoint ``cache`` dropped, to refill in place of a fresh snapshot (its arrays keep their
    //! room), or an empty snapshot where none waits.
    return <- prefix_take_spare_(cache)
}

def gpu_device_kv_import(model : Model; var session : Session; var host : Session; npos : int64) : bool {
    //! Bring the first ``npos`` rows of the paged ``host`` session up into a region for the fresh device-home
    //! ``session``, whose ``n_past`` becomes ``npos``; a recurrent state goes in the session's host arrays first.
    //! False where no region is free or the pool is not the driver's codec.
    return gpu_device_kv_import_(model, session, host, npos)
}

def gpu_device_kv_pool_armed() : bool {
    //! Whether a paged scheduler keeps its device-home streams' openings in its prefix cache too
    //! (``DASLLAMA_DEVICE_POOL``, on unless set to 0).
    return gpu_device_kv_pool_armed_()
}

def gpu_device_kv_pool_moves() : tuple<exports : int64; rows_down : int64; down_us : int64; imports : int64; rows_up : int64; up_us : int64; state_us : int64> {
    //! The rows ``gpu_device_kv_export`` brought down and ``gpu_device_kv_import`` took up since the process
    //! started, with the wall each direction took, and the wall ``gpu_device_kv_snapshot`` spent on states.
    let m = gpu_device_kv_pool_moves_()
    return (exports = m.exports, rows_down = m.rows_down, down_us = m.down_us, imports = m.imports, rows_up = m.rows_up, up_us = m.up_us, state_us = m.state_us)
}

def gpu_device_kv_snapshot(var session : Session; at : int64; var into : DnSnapshot) : DnSnapshot {
    //! The recurrent state a live device-home ``session`` held at ``at`` (where it stands, or a kept checkpoint) copied
    //! off its region into ``into`` (moved in), the region left as it stands - a session the driver does not hold
    //! flushes as ``dn_snapshot_take`` does; ``pos`` -1 where no state stands at ``at``.
    return <- gpu_device_kv_snapshot_(session, at, into)
}

def prefix_state_match_len(cache : PrefixCache; prompt : array<int64>) : int64 {
    //! How many leading positions of ``prompt`` a recurrent checkpoint in ``cache`` would attach, touching
    //! nothing (0 where none opens it).
    return prefix_state_match_len_(cache, prompt)
}

def prefix_attach_state_split(var cache : PrefixCache; var pool : KVPool; var pages : Session; var state : Session; prompt : array<int64>) : int64 {
    //! Attach the deepest recurrent checkpoint opening ``prompt`` apart: its pages shared into ``pages`` (a
    //! fresh session of ``pool`` that only reads them), its state into ``state``. Answers the positions, 0 = none.
    return prefix_attach_state_split_(cache, pool, pages, state, prompt)
}

def prefix_state_touch(var cache : PrefixCache; tokens : array<int64>; n : int64; opening : bool) : bool {
    //! Whether ``cache`` keeps a checkpoint of the first ``n`` of ``tokens``; one that does is used now, and
    //! ``opening`` makes it an opening from here on.
    return prefix_state_touch_(cache, tokens, n, opening)
}

def prefix_share_pages(var cache : PrefixCache; var pool : KVPool; var session : Session; tokens : array<int64>; n : int64) : int64 {
    //! Share the whole pages of the first ``n`` of ``tokens`` ``cache`` already holds into the fresh paged
    //! ``session``; answers the positions they cover, where the rows still to write start.
    return prefix_share_pages_(cache, pool, session, tokens, n)
}

def prefix_insert_snapshot(var cache : PrefixCache; var pool : KVPool; session : Session; tokens : array<int64>; var snap : DnSnapshot; opening : bool = false; upto : int64 = 0l) {
    //! A recurrent checkpoint of the first ``n_past`` of ``tokens`` out of ``session``'s pages, its state ``snap``
    //! (moved in) taken elsewhere - a device's slots, or a window's capture at ``upto`` (0 = the tokens' end).
    //! ``opening`` kinds it as shared, else as a conversation's tail.
    prefix_insert_snapshot_(cache, pool, session, tokens, snap, opening, upto)
}

def gpu_device_session_ctx() : int64 {
    //! The positions one device-home session holds - the armed driver's per-region context.
    return gpu_device_session_ctx_()
}

def gpu_device_prefill_window() : int64 {
    //! The positions the device prefills in one window - the shortest chunk worth handing a device-home session.
    return gpu_device_prefill_window_()
}

def gpu_device_session_dtype() : KVDtype {
    //! The K/V codec a device-home session must carry: the armed driver's.
    return gpu_device_session_dtype_()
}

def set_resident_prefill_allowed(allowed : bool) {
    //! Allow or pin out the resident prefill arm for HOST-CACHED sessions: more of them than the driver has
    //! K/V regions would strand device-only rows. A device-home session prefills on the device whatever the pin says.
    set_resident_prefill_allowed_(allowed)
}

def resident_prefill_allowed() : bool {
    //! The pin as ``set_resident_prefill_allowed`` left it. A fixture reads it before it pins, then puts it back.
    return resident_prefill_allowed_()
}
