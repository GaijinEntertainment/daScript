options gen2
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // ggml-parity FP laxity; drop for bit-exact work

require dasllama/dasllama
require daslib/jobque_boost
require strings

// Tutorial dasLLAMA-14: Vision Chat
//
// This tutorial covers:
//   - load_image_rgb: any image file to an RGB8 VisionImage
//   - load_gemma4uv_embedder reads a vision multimodal projector (mmproj) GGUF
//   - create_chat(model, embedder) — a conversation that can see
//   - add_user_image + add_user composing one turn; respond() streams the reply
//   - Under the hood: encode_image soft tokens, render_turn_image head/tail,
//     eval_embd_span (the image span) and eval_embd (its causal twin)
//
// A vision chat model pairs a normal text decoder with an image encoder: the
// encoder turns pixels into "soft tokens" — embedding rows the decoder reads
// inline with text, one row per image patch group. The supported family is
// gemma-4 dense (decoder GGUF + its vision mmproj GGUF); the chat template
// frames the image span for you, so the calls below stay family-agnostic.
//
// Prerequisites: a gemma-4 decoder GGUF + its vision mmproj GGUF + an image
// file (PNG/JPG/BMP — anything stbimage decodes).
//
// Run: daslang.exe -jit tutorials/dasLLAMA/14_vision_chat.das -- <decoder.gguf> <mmproj.gguf> <image> [prompt]

let DEFAULT_PROMPT = "Describe this image in one sentence."
let N_GEN = 96l

// ──────────────────────────────────────────────────────────────────────────
// Section 1 — A conversation that can see
// ──────────────────────────────────────────────────────────────────────────
//
// The easy path is four calls. load_image_rgb decodes the file to RGB8.
// load_gemma4uv_embedder reads the mmproj — the vision half of the model,
// shipped as its own GGUF. create_chat(model, embedder) moves the embedder
// into the chat, and add_user_image queues the picture for the next turn:
// geometry, letterbox and the encoder run right there, so respond() only
// prefills and generates. One image per turn; text via add_user rides along.

def section_easy(m : Model; decoder_mmproj : string; image_path : string; prompt : string) {
    var inscope img <- load_image_rgb(image_path)
    if (img.width == 0) {
        panic("could not decode image '{image_path}'")
    }
    print("image: {img.width}x{img.height}\n")
    var emb <- load_gemma4uv_embedder(decoder_mmproj)
    var chat <- create_chat(m, emb, "", N_GEN)
    add_user_image(chat, img)
    add_user(chat, prompt)
    respond(m, chat, SamplingParams()) $(piece) {
        print("{piece}")
        return true
    }
    let st = stats(chat.session)
    print("\n[prompt {st.n_prompt} tok | gen {st.n_gen} tok | gen {int(st.gen_tps)} t/s]\n\n")
    delete chat
}

// ──────────────────────────────────────────────────────────────────────────
// Section 2 — Soft tokens by hand: the image span
// ──────────────────────────────────────────────────────────────────────────
//
// Now we build the same turn ourselves — the shape a scheduler uses when the
// encoder runs on a worker and the decoder session lives elsewhere.
//
// encode_image is the whole image path in one call: geometry, letterbox,
// normalize, encode. It appends npos rows of model.config.dim floats to `out`
// and returns npos. render_turn_image renders the turn's TOKENS as two spans —
// `head` before the image rows, `tail` after — split exactly at the template's
// image marker, so tokenizer merges never cross the picture.
//
// The prompt we prefill is head rows + image rows + tail rows, one array.
// embed_text_rows fills the text parts: prefilling tokens IS prefilling their
// embedding rows (tutorial 08 proves that identity). eval_embd_span runs the
// whole thing at the session's position, with one twist — the rows in
// [span_lo, span_hi) are NON-CAUSAL: every query in the span attends the whole
// span, because a picture has no left-to-right order. The text around it stays
// causal. That is the gemma vision decode shape.
//
// eval_embd is the same call without a span — every row causal. Audio rows
// keep their time order, so the audio family prefills through it.

def section_by_hand(m : Model; decoder_mmproj : string; image_path : string; prompt : string) {
    var inscope img <- load_image_rgb(image_path)
    var emb <- load_gemma4uv_embedder(decoder_mmproj)
    var scratch = Gemma4uvState()

    // the encoder half: pixels to soft tokens
    var img_rows : array<float>
    let n_img = encode_image(emb, scratch, img, "tut14", img_rows)
    print("encoded {n_img} soft tokens of {m.config.dim} floats each\n")

    // the template half: the turn's tokens, split at the image marker
    var chat = create_chat_renderer(m, "", N_GEN)
    add_user(chat, prompt)
    var head : array<int64>
    var tail : array<int64>
    render_turn_image(m, chat, head, tail)
    let n_head = long_length(head)
    let n_tail = long_length(tail)
    let np = n_head + n_img + n_tail
    print("turn: {n_head} head + {n_img} image + {n_tail} tail rows\n")

    // one row array: text rows around the image rows
    var rows : array<float>
    rows |> resize(np * m.config.dim)
    embed_text_rows(m, head, rows, 0l)
    for (i in range64(n_img * m.config.dim)) {
        rows[n_head * m.config.dim + i] = img_rows[i]
    }
    embed_text_rows(m, tail, rows, n_head + n_img)

    // prefill with the image span non-causal, then sample like any session
    var s = create_session(m)
    eval_embd_span(m, s, rows, np, n_head, n_head + n_img)
    let stop_ids <- effective_stop_ids(chat)
    var one : array<int64>
    one |> resize(1)
    for (_step in range64(N_GEN)) {
        let tok = sample(s, SamplingParams())
        if (find_index(stop_ids, tok) >= 0) {
            break
        }
        print("{piece(m, tok)}")
        one[0] = tok
        eval(m, s, one)
    }
    print("\n\n")

    // the causal twin: eval_embd prefills rows with no span — text-only here,
    // and the audio family's turns run through exactly this call
    var s2 = create_session(m)
    let ids <- encode(m, "The capital of France is")
    let n = long_length(ids)
    var text_rows : array<float>
    text_rows |> resize(n * m.config.dim)
    embed_text_rows(m, ids, text_rows, 0l)
    eval_embd(m, s2, text_rows, n)
    print("eval_embd prefilled {n} causal rows; next token: '{piece(m, sample(s2, SamplingParams()))}'\n\n")

    delete s
    delete s2
    delete chat
    delete emb
}

def is_image_path(p : string) : bool {
    let low = to_lower(p)
    return ends_with(low, ".png") || ends_with(low, ".jpg") || ends_with(low, ".jpeg") || ends_with(low, ".bmp")
}

[export]
def main() {
    let cmd <- get_command_line_arguments()
    var ggufs : array<string>
    var image_path = ""
    var prompt = DEFAULT_PROMPT
    for (a in cmd) {
        if (ends_with(a, ".gguf")) {
            ggufs |> push(a)
        } elif (is_image_path(a)) {
            image_path = a
        } elif (!empty(image_path) && !starts_with(a, "-")) {
            prompt = a
        }
    }
    if (length(ggufs) < 2 || empty(image_path)) {
        print("usage: daslang.exe -jit tutorials/dasLLAMA/14_vision_chat.das -- <decoder.gguf> <mmproj.gguf> <image> [prompt]\n")
        return
    }
    var m <- load_model(ggufs[0], QuantMode.q8)
    allow_cpu_prefill()   // a CPU tutorial, said explicitly — on a Mac, set_metal_mode(MetalMode.when_available) picks the GPU rails instead
    with_job_que() {
        setup_dasllama_jobque()
        section_easy(m, ggufs[1], image_path, prompt)
        section_by_hand(m, ggufs[1], image_path, prompt)
    }
    delete m
}
