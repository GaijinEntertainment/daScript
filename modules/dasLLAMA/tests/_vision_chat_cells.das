options gen2
options _dasllama_internal = true

module _vision_chat_cells shared public

//! the image chat turn's cells on a tower and a decoder their caller loaded; no entry loads a model

require dastest/testing_boost public
require dasllama/dasllama
require dasllama/dasllama_common public   // QuantMode, Model: a decoder form's load mode
require dasllama/dasllama_vision_io
require daslib/fio
require daslib/strings_boost
require strings
require math
require _model_tier   // models_dir, family_on
require _model_form public   // ModelForm, cpu_cells_form: the decoder forms the cells run on
require _gpu_knobs   // the tower knobs a cell pins and puts back
require ../performance/box_ident.das   // box_ram_gb - the room a large decoder's served planes need
require ?das_metal dasllama/dasllama_metal_tower   // the tower engage/knob legs (the facade registers the hook)

// The image chat turn: the prompt stream shape around the soft-token splice (tier 2) and the
// greedy caption (tier 3). Pinned: marker ids, media-first order, span length from the
// geometry, and that the model describes what is in the picture. NOT token-parity with
// llama-mtmd-cli — tests/CLAUDE.md carries why.
// Each cell reads two model files, so it runs in the inner of two nested blocks: the tower's `image` form (its mmproj's
// `with_model_once` block, in the family's test file) and, nested in it, the decoder's CPU form (its
// `with_cpu_and_gpu_model` block, there too). A chat takes its tower (`create_chat_` moves it in), so a cell hands the
// tower back to the form before the chat goes. Every cell skips on its decoder's family filter, on the cats jpg being
// absent from the models dir, and where the box lacks the RAM a large decoder serves in.

let private CATS = "coco-val2017-000000039769.jpg"

// the pinned marker ids (gemma-4 vocab; ggml-vocab-gemma-4.gguf)
let private IMAGE_OPEN = 255999l
let private IMAGE_CLOSE = 258882l
// 640x480 letterboxes to 624x480 = 13x10 superpatches (mtmd oracle: nx = 130)
let private CATS_ROWS = 130l
// gemma3 vocab: <start_of_image> / <end_of_image>; the fixed 896² canvas always yields 256 rows
let private G3_IMAGE_OPEN = 255999l
let private G3_IMAGE_CLOSE = 256000l
let private G3_ROWS = 256l
// qwen vocab: <|vision_start|> / <|vision_end|>; 640x480 is 32-aligned = a 20x15 merged grid
let private Q_IMAGE_OPEN = 151652l
let private Q_IMAGE_CLOSE = 151653l
let private Q_CATS_ROWS = 300l
let HOST_CTX_4B = 4096l   //! every host session's K/V on the 4B decoder: its file's 262144 positions commit 38 GB a flat session, and an image turn here holds under a thousand
let VISION_CHAT_CTX = 1l << 40l   //! a decoder's block context that cuts nothing: the file's own positions, as the product's load serves them

//! Whether an image chat cell's decoder block may load: the run's family filter names `family` (the decoder's row of
//! `model_families()`; "" a decoder no row holds), the cats photo is stocked, and the box holds `need_gb` of RAM for the
//! decoder served (0: no claim - a 12B Q4_K_M serves its planes at f32 on the CPU chain: 45 GB off a 7 GB file, past a 62 GB
//! pod); else the cell has registered its skip
def vision_chat_gate(t : T?; family, decoder : string; need_gb : int) : bool {
    return false if (!empty(family) && !family_on(t, family))
    if (!stat(path_join(models_dir(), CATS)).is_valid) {
        t |> skip("{CATS} not present - fetch it into the models dir (gemma4-vision-oracle/mint.sh documents the URL)")
        return false
    }
    let gb = box_ram_gb()
    if (need_gb > 0 && gb > 0 && gb < need_gb) {
        t |> skip("{decoder} needs about {need_gb} GB of RAM served; this box holds {gb} GB")
        return false
    }
    return true
}

//! a cell of an image chat turn: `cell` on the decoder's form `m` and the tower `emb` with the cats photo, as the subtest
//! `name` under the CPU-prefill intent, where the form is the CPU load asked alone (`form.alone`) and `uncached` says
//! which loader's
def vision_chat_cell(t : T?; name : string; var m : Model; form : ModelForm; uncached : bool; var emb : VisionEmbedder;
                     cell : function<(t : T?; var m : Model; var emb : VisionEmbedder; img : VisionImage) : void>) {
    return if (!form.alone || form.uncached != uncached)
    unsafe {
        t |> run(name) @capture(& m, & emb)(t : T ?) {
            let img <- load_image_rgb(path_join(models_dir(), CATS))
            with_cpu_prefill(true) {
                invoke(cell, t, m, emb, img)
            }
        }
    }
}

// The gemma-4 12B turn, on the gemma4uv embedder: shape, caption, then the tower legs.
[cold_path]
def test_vision_chat(t : T?; var m : Model; var emb : VisionEmbedder; img : VisionImage) {
    t |> equal(img.width, 640, "cats fixture width")
    t |> equal(img.height, 480, "cats fixture height")
    let knobs = gpu_knobs()
    static_if (typeinfo builtin_module_exists(das_metal)) {
        set_metal_tower(true)
    }
    let tower0_encodes = gpu_tower_counters(TowerFamily.vision).encodes   // nolint:LINT002,LINT019 — read in the apple static_if half; the linux flavor sees it unused
    var chat <- create_chat_(m, emb, "", 96l)
    add_user_image_(chat, img)
    add_user_(chat, "describe this image")
    t |> equal(chat.n_image_rows, CATS_ROWS, "image span length from the geometry")
    // one media kind per turn: audio on a vision chat panics (no tower arm, and the
    // cross-guard behind it), never a silent drop of either modality's rows
    var mixed = false
    try {
        var pcm : array<float>
        pcm |> resize(1600)
        add_user_audio_(chat, pcm)
        delete pcm
    } recover {
        mixed = true
    }
    t |> success(mixed, "audio beside a queued image panics instead of dropping rows")
    // TIER 2 - the stream shape: head ends at the open marker, tail opens at the close one
    var head, tail : array<int64>
    render_prompt_image(m, chat, head, tail)
    t |> success(!empty(head) && !empty(tail), "both spans render")
    t |> equal(head[long_length(head) - 1l], IMAGE_OPEN, "head ends with <|image>")
    t |> equal(tail[0], IMAGE_CLOSE, "tail starts with <image|>")
    // media-first: the user text sits in the TAIL, after the span (the upstream order)
    let tail_text = decode_(m, tail)
    t |> success(find(tail_text, "describe this image") >= 0,
        "the user text follows the image span (tail: {replace(tail_text, "\n", "\\n")})")
    let head_text = decode_(m, head)
    t |> success(find(head_text, "describe this image") < 0,
        "the head carries no user text (head: {replace(head_text, "\n", "\\n")})")
    // TIER 3 - the caption. Greedy; the full text is logged either way (the eyeball rail)
    var params = SamplingParams()
    params.temp = 0.0
    let reply = respond_(m, chat, params) $(_piece : string) {
        return true
    }
    to_log(LOG_INFO, "vision caption: {replace(reply, "\n", "\\n")}\n")
    let low = to_lower(reply)
    t |> success(find(low, "cat") >= 0, "the cats are described as cats")
    t |> success(length(reply) > 40, "the caption is a description, not a fragment")
    emb <- chat.embedder   // the chat took the tower; the block's form gets it back
    delete chat
    // The tower legs. The caption is the FLOOR, not token parity
    // (freeform token cells are banned; the soft-token numeric bars live in the tier-1 GPU
    // and gemma4uv-metal gates) - here the contract is engage evidence plus "still cats"
    // with the knob off.
    static_if (typeinfo builtin_module_exists(das_metal)) {
        t |> equal(metal_tower_stats().encodes - tower0_encodes, 1l,
                    "the image encode above was GPU-served (encodes delta)")
        set_metal_tower(false)
        let sc0 = metal_tower_stats()
        var chat2 <- create_chat_(m, emb, "", 96l)   // the tower the first chat gave back
        add_user_image_(chat2, img)
        add_user_(chat2, "describe this image")
        let reply2 = respond_(m, chat2, params) $(_piece : string) {
            return true
        }
        to_log(LOG_INFO, "vision caption (tower off): {replace(reply2, "\n", "\\n")}\n")
        let low2 = to_lower(reply2)
        t |> success(find(low2, "cat") >= 0, "knob-off leg still describes the cats")
        t |> success(length(reply2) > 40, "knob-off caption is a description, not a fragment")
        let sc1 = metal_tower_stats()
        t |> equal(sc1.encodes, sc0.encodes, "knob-off leg never dispatched")
        t |> equal(sc1.declines - sc0.declines, 1l, "knob-off leg recorded the knob decline")
        emb <- chat2.embedder
        delete chat2
    }
    set_gpu_knobs(knobs)
}

// The same turn through the gemma4v ViT tower (E-series): the carrier sniffs the family, the
// splice and the span are the shared rail, so tier 2 is the same shape and tier 3 the same floor.
[cold_path]
def test_vision_chat_e2b(t : T?; var m : Model; var emb : VisionEmbedder; img : VisionImage) {
    // a carrier with no vision arm is refused at create, by name
    var no_arm_tripped = false
    try {
        var none = VisionEmbedder()
        var bad <- create_chat_(m, none, "", 8l)
        delete bad
    } recover {
        no_arm_tripped = true
    }
    t |> success(no_arm_tripped, "create_chat_ refuses an embedder with no vision arm")
    t |> equal(vision_family(emb), "gemma4v", "the E2B mmproj sniffs as gemma4v")
    t |> equal(vision_align(emb), 48l, "48 px per soft token (patch 16 x merge 3)")
    var chat <- create_chat_(m, emb, "", 96l)
    add_user_image_(chat, img)
    add_user_(chat, "describe this image")
    t |> equal(chat.n_image_rows, CATS_ROWS, "image span length from the geometry (mtmd: 130)")
    var head, tail : array<int64>
    render_prompt_image(m, chat, head, tail)
    t |> success(!empty(head) && !empty(tail), "both spans render")
    t |> equal(head[long_length(head) - 1l], IMAGE_OPEN, "head ends with <|image>")
    t |> equal(tail[0], IMAGE_CLOSE, "tail starts with <image|>")
    var params = SamplingParams()
    params.temp = 0.0
    let reply = respond_(m, chat, params) $(_piece : string) {
        return true
    }
    to_log(LOG_INFO, "vision caption (E2B/gemma4v): {replace(reply, "\n", "\\n")}\n")
    let low = to_lower(reply)
    t |> success(find(low, "cat") >= 0, "the cats are described as cats")
    t |> success(length(reply) > 40, "the caption is a description, not a fragment")
    emb <- chat.embedder   // the chat took the tower; the block's form gets it back
    delete chat
}

// The E2B image turn with thinking OFF: instruct mode arms the channel markers as stops, and a
// model that opens the media turn with one must still answer - the leading block is tolerated.
// The Q4_K_M decoder under the chat sampler preset at the session's default seed draws a channel
// marker first on this box (greedy on the Q8_0 decoder does not), so the cell runs that shape;
// the assert is the answer, since the first draw follows the box's tower lane.
[cold_path]
def test_vision_chat_e2b_nothink(t : T?; var m : Model; var emb : VisionEmbedder; img : VisionImage) {
    var chat <- create_chat_(m, emb, "", 640l)
    set_thinking_(chat, false)
    set_seed_(chat.session, 1234)
    add_user_image_(chat, img)
    add_user_(chat, "describe this image")
    let params = SamplingParams(temp = 0.7, top_k = 20l, top_p = 0.95)
    var n_pieces = 0
    let reply = respond_(m, chat, params) $(_piece : string) {
        n_pieces++
        return true
    }
    // the model opens with a thought block despite the closed empty prefill; the armed
    // matcher files it as reasoning and the answer is the content half
    let sp = split_reasoning_(chat.tmpl, reply)
    to_log(LOG_INFO, "vision caption (E2B/gemma4v, thinking off): {stats(chat.session).n_gen} tokens, reasoning {length(sp.reasoning)} chars, content: {replace(sp.content, "\n", "\\n")}\n")
    t |> success(n_pieces > 0, "the turn generated tokens")
    t |> success(find(to_lower(sp.content), "cat") >= 0, "the content half describes the cats as cats")
    t |> success(find(sp.content, "<channel|>") < 0 && find(sp.content, "<|channel>") < 0, "no channel marker reaches the content half")
    t |> success(stats(chat.session).n_gen < 640l, "the turn ended on a stop, not the budget ({stats(chat.session).n_gen} tokens)")
    t |> equal(length(chat.history), 2, "the turn is on the history")
    t |> success(find(chat.history[1].content, "<|channel>") < 0, "history stores the answer without the thought")
    emb <- chat.embedder   // the chat took the tower; the block's form gets it back
    delete chat
}

// The same turn through the gemma3 SigLIP tower: a THIRD family through the same splice rail —
// its markers come from the gemma3 arch and its span is the fixed 256 (896² canvas).
[cold_path]
def test_vision_chat_gemma3(t : T?; var m : Model; var emb : VisionEmbedder; img : VisionImage) {
    t |> equal(vision_family(emb), "gemma3v", "the gemma-3 mmproj sniffs as gemma3v")
    t |> equal(vision_align(emb), 56l, "56 px per soft token (patch 14 x merge 4)")
    var chat <- create_chat_(m, emb, "", 96l)
    add_user_image_(chat, img)
    add_user_(chat, "describe this image")
    t |> equal(chat.n_image_rows, G3_ROWS, "the fixed canvas always splices 256 rows")
    var head, tail : array<int64>
    render_prompt_image(m, chat, head, tail)
    t |> success(!empty(head) && !empty(tail), "both spans render")
    t |> equal(head[long_length(head) - 1l], G3_IMAGE_OPEN, "head ends with <start_of_image>")
    t |> equal(tail[0], G3_IMAGE_CLOSE, "tail starts with <end_of_image>")
    var params = SamplingParams()
    params.temp = 0.0
    let reply = respond_(m, chat, params) $(_piece : string) {
        return true
    }
    to_log(LOG_INFO, "vision caption (gemma-3-4b/gemma3v): {replace(reply, "\n", "\\n")}\n")
    let low = to_lower(reply)
    t |> success(find(low, "cat") >= 0, "the cats are described as cats")
    t |> success(length(reply) > 40, "the caption is a description, not a fragment")
    emb <- chat.embedder   // the chat took the tower; the block's form gets it back
    delete chat
}

// The 12B gemma3 pair: the SAME SigLIP tower at projection width 3840 — the one dimension the
// family varies per decoder; the gemma3-12b category's, so a `--category gemma3-12b` run opens it.
[cold_path]
def test_vision_chat_gemma3_12b(t : T?; var m : Model; var emb : VisionEmbedder; img : VisionImage) {
    t |> equal(vision_family(emb), "gemma3v", "the 12B mmproj sniffs as gemma3v")
    t |> equal(vision_proj_dim(emb), 3840l, "projection width matches the 12B decoder")
    var chat <- create_chat_(m, emb, "", 96l)
    add_user_image_(chat, img)
    add_user_(chat, "describe this image")
    t |> equal(chat.n_image_rows, G3_ROWS, "the fixed canvas always splices 256 rows")
    var params = SamplingParams()
    params.temp = 0.0
    let reply = respond_(m, chat, params) $(_piece : string) {
        return true
    }
    to_log(LOG_INFO, "vision caption (gemma-3-12b/gemma3v): {replace(reply, "\n", "\\n")}\n")
    let low = to_lower(reply)
    t |> success(find(low, "cat") >= 0, "the cats are described as cats")
    t |> success(length(reply) > 40, "the caption is a description, not a fragment")
    emb <- chat.embedder   // the chat took the tower; the block's form gets it back
    delete chat
}

// The same turn through the qwen3v tower (Qwen3-Omni): the mrope leg — grid rope positions
// through the uniform span, position advance by the grid's long side. Large tier.
[cold_path]
def test_vision_chat_omni(t : T?; var m : Model; var emb : VisionEmbedder; img : VisionImage) {
    t |> success(m.config.rope_sections.x + m.config.rope_sections.y + m.config.rope_sections.z > 0,
        "the decoder carries rope.dimension_sections ({m.config.rope_sections})")
    t |> equal(vision_family(emb), "qwen3v", "the Omni mmproj sniffs as qwen3v")
    t |> equal(vision_proj_dim(emb), 2048l, "projection width matches the Omni decoder")
    var chat <- create_chat_(m, emb, "", 96l)
    add_user_image_(chat, img)
    add_user_(chat, "describe this image")
    t |> equal(chat.n_image_rows, Q_CATS_ROWS, "640x480 is 32-aligned: 300 soft tokens")
    t |> equal(chat.image_grid, int2(20, 15), "the merged grid reaches the chat (position advance 20)")
    var head, tail : array<int64>
    render_prompt_image(m, chat, head, tail)
    t |> equal(head[long_length(head) - 1l], Q_IMAGE_OPEN, "head ends with <|vision_start|>")
    t |> equal(tail[0], Q_IMAGE_CLOSE, "tail starts with <|vision_end|>")
    let tail_text = decode_(m, tail)
    t |> success(find(tail_text, "describe this image") >= 0,
        "the user text follows the image span (tail: {replace(tail_text, "\n", "\\n")})")
    var params = SamplingParams()
    params.temp = 0.0
    let reply = respond_(m, chat, params) $(_piece : string) {
        return true
    }
    to_log(LOG_INFO, "vision caption (omni/qwen3v): {replace(reply, "\n", "\\n")}\n")
    let low = to_lower(reply)
    t |> success(find(low, "cat") >= 0, "the cats are described as cats")
    t |> success(length(reply) > 40, "the caption is a description, not a fragment")
    t |> equal(chat.session.rope_pos_delta, 20l - 300l, "the session's rope delta reflects the grid advance")
    emb <- chat.embedder   // the chat took the tower; the block's form gets it back
    delete chat
}

// The Qwen3-VL 4B turn: deepstack wide rows through the chat, the caption, then the zeroed-slices control.
[cold_path]
def test_vision_chat_deepstack(t : T?; var m : Model; var emb : VisionEmbedder; img : VisionImage) {
    t |> equal(m.config.n_deepstack, 3l, "the decoder carries n_deepstack_layers")
    t |> equal(vision_family(emb), "qwen3v", "the dense mmproj sniffs as qwen3v")
    t |> equal(vision_proj_dim(emb), 2560l, "projection width matches the 4B decoder")
    t |> equal(vision_deepstack(emb), 3l, "the tower carries three deepstack taps")
    t |> equal(vision_row_width(emb), 10240l, "wide rows: (1+3) x 2560")
    var chat <- create_chat_(m, emb, "", 96l)
    add_user_image_(chat, img)
    add_user_(chat, "describe this image")
    t |> equal(chat.n_image_rows, Q_CATS_ROWS, "640x480 is 32-aligned: 300 soft tokens")
    t |> equal(chat.image_grid, int2(20, 15), "the merged grid reaches the chat")
    t |> equal(long_length(chat.image_rows), Q_CATS_ROWS * 10240l, "the chat holds wide rows")
    var params = SamplingParams()
    params.temp = 0.0
    let reply = respond_(m, chat, params) $(_piece : string) {
        return true
    }
    to_log(LOG_INFO, "vision caption (qwen3vl-4b/deepstack): {replace(reply, "\n", "\\n")}\n")
    let low = to_lower(reply)
    t |> success(find(low, "cat") >= 0, "the cats are described as cats")
    t |> success(length(reply) > 40, "the caption is a description, not a fragment")
    t |> equal(chat.session.rope_pos_delta, 20l - 300l, "the session's rope delta reflects the grid advance")
    emb <- chat.embedder   // the chat took the tower; the block's form gets it back
    delete chat
    // the zeroed-slices decoder control: the SAME wide rows with their deepstack tails
    // zeroed must move the decoder's logits - a caption gate alone cannot see a decoder
    // that silently ignores the slices (the arc's slice-H requirement)
    var st = VisionState()
    var rows : array<float>
    let nr = encode_image(emb, st, img, "ds-control", rows)
    t |> equal(nr, Q_CATS_ROWS, "control encode row count")
    var zeroed := rows
    for (r in range64(nr)) {
        for (i in range64(2560l, 10240l)) {
            zeroed[r * 10240l + i] = 0.0
        }
    }
    var s1 <- create_session(m)
    eval_embd_span_mrope(m, s1, rows, nr, 0l, nr, int2(20, 15))
    var s2 <- create_session(m)
    eval_embd_span_mrope(m, s2, zeroed, nr, 0l, nr, int2(20, 15))
    var dmax = 0.0
    var a1 = 0l
    var a2 = 0l
    for (i in range64(long_length(s1.logits))) {
        dmax = max(dmax, nan_wide_diff(s1.logits[i], s2.logits[i]))
        if (s1.logits[i] > s1.logits[a1]) {
            a1 = i
        }
        if (s2.logits[i] > s2.logits[a2]) {
            a2 = i
        }
    }
    to_log(LOG_INFO, "deepstack zeroed-slices: wide picks '{replace(piece_(m, a1), "\n", "\\n")}' ({s1.logits[a1]}), zeroed picks '{replace(piece_(m, a2), "\n", "\\n")}' ({s2.logits[a2]}), logits maxdiff = {dmax}\n")
    t |> success(dmax > 1e-2, "the decoder consumes the deepstack slices ({dmax})")
    delete s1
    delete s2
    delete rows
    delete zeroed
    delete st
}

// The bench shape: pre-encoded rows injected into a PLAIN chat (no embedder attached) must
// walk the very turn the embedder chat walks — lcpp_bench prices this seam, and a hand-splice
// here once fed deepstack decoders a narrow scrambled span that still captioned the cats.
[cold_path]
def test_vision_chat_rows_seam(t : T?; var m : Model; var emb : VisionEmbedder; img : VisionImage) {
    var params = SamplingParams()
    params.temp = 0.0
    var chat1 <- create_chat_(m, emb, "", 24l)
    add_user_image_(chat1, img)
    add_user_(chat1, "describe this image")
    let r1 = respond_(m, chat1, params) $(_piece : string) {
        return true
    }
    emb <- chat1.embedder   // the chat took the tower; the block's form gets it back
    delete chat1
    var st = VisionState()
    var rows : array<float>
    let nr = encode_image(emb, st, img, "rows-seam", rows)
    var chat2 <- create_chat_(m, "", 24l)
    add_user_image_rows_(m, chat2, rows, nr, vision_mrope_grid(emb, st))
    add_user_(chat2, "describe this image")
    let r2 = respond_(m, chat2, params) $(_piece : string) {
        return true
    }
    to_log(LOG_INFO, "rows-seam replies: embedder '{replace(r1, "\n", "\\n")}' vs rows '{replace(r2, "\n", "\\n")}'\n")
    t |> equal(r1, r2, "greedy replies match token-for-token")
    // the seam's guards: a wrong-width quantum and a second image both refuse by panic
    var tripped = false
    try {
        var chat3 <- create_chat_(m, "", 8l)
        var bad : array<float>
        bad |> resize(nr * m.config.dim)   // narrow rows on a deepstack decoder - the old bench defect's exact shape
        add_user_image_rows_(m, chat3, bad, nr, int2(20, 15))
        delete bad
        delete chat3
    } recover {
        tripped = true
    }
    t |> success(tripped, "narrow rows on a deepstack decoder panic at the seam")
    tripped = false
    try {
        var rows3 : array<float>
        let nr3 = encode_image(emb, st, img, "rows-seam-2", rows3)
        var chat4 <- create_chat_(m, "", 8l)
        add_user_image_rows_(m, chat4, rows3, nr3, vision_mrope_grid(emb, st))
        var rows4 : array<float>
        let nr4 = encode_image(emb, st, img, "rows-seam-3", rows4)
        add_user_image_rows_(m, chat4, rows4, nr4, vision_mrope_grid(emb, st))
        delete rows4
        delete rows3
        delete chat4
    } recover {
        tripped = true
    }
    t |> success(tripped, "a second image before respond() panics at the seam")
    delete chat2
    delete rows
    delete st
}

// The Qwen2.5-Omni turn: window ViT + the NON-interleaved MROPE decoder, then the caption.
[cold_path]
def test_vision_chat_qwen25o(t : T?; var m : Model; var emb : VisionEmbedder; img : VisionImage) {
    t |> equal(m.config.rope_sections, int4(16, 24, 24, 0), "the qwen2vl decoder's sections")
    t |> success(!m.config.mrope_interleaved, "qwen2vl ropes the sections as contiguous ranges")
    t |> equal(vision_family(emb), "qwen25v", "the dual-tower Omni mmproj sniffs as qwen25v")
    t |> equal(vision_proj_dim(emb), 2048l, "projection width matches the 3B decoder")
    var chat <- create_chat_(m, emb, "", 96l)
    add_user_image_(chat, img)
    add_user_(chat, "describe this image")
    t |> equal(chat.n_image_rows, 391l, "640x480 letterboxes to 644x476 at align 28: 391 soft tokens")
    t |> equal(chat.image_grid, int2(23, 17), "the merged grid reaches the chat (position advance 23)")
    var params = SamplingParams()
    params.temp = 0.0
    let reply = respond_(m, chat, params) $(_piece : string) {
        return true
    }
    to_log(LOG_INFO, "vision caption (qwen2.5-omni/qwen25v): {replace(reply, "\n", "\\n")}\n")
    let low = to_lower(reply)
    t |> success(find(low, "cat") >= 0, "the cats are described as cats")
    t |> success(length(reply) > 40, "the caption is a description, not a fragment")
    t |> equal(chat.session.rope_pos_delta, 23l - 391l, "the session's rope delta reflects the grid advance")
    emb <- chat.embedder   // the chat took the tower; the block's form gets it back
    delete chat
}

// The bench/scheduler shape: a PLAIN chat (no embedder attached) renders the image span, so
// the Omni vocab-spelling fallback must live at the renderer, not only in the vision
// constructor (lcpp_bench --image builds exactly this turn and panicked on <|vision_start|>).
// It reads the decoder's uncached form alone: the tower and the photo are its block's, not its own.
[cold_path, unused_argument(emb, img)]
def test_render_image_span_qwen25o(t : T?; var m : Model; var emb : VisionEmbedder; img : VisionImage) {   // nolint:LINT014 - the image chat cells' one signature
    var chat <- create_chat_(m, "", 8l)
    add_user_(chat, "describe this image")
    var head, tail : array<int64>
    render_turn_image_(m, chat, head, tail)
    t |> success(!empty(head) && !empty(tail), "both spans render")
    t |> equal(head[length(head) - 1], special_id(m, "<|vision_bos|>"), "the head closes on the vocab's bos marker")
    delete head
    delete tail
    delete chat
}

// The multi-turn showcase: ONE Omni session — an image turn, then a text follow-up whose
// answer requires the image turn's history across the mrope position delta. Audio-in-chat
// for the qwen3a conformer is a standing gap (the chat AudioTower serves the whisper-class
// families only) — the full three-modality showcase lands with that slice.
[cold_path]
def test_omni_showcase(t : T?; var m : Model; var emb : VisionEmbedder; img : VisionImage) {
    var chat <- create_chat_(m, emb, "", 96l)
    var params = SamplingParams()
    params.temp = 0.0
    add_user_image_(chat, img)
    add_user_(chat, "describe this image in one sentence")
    let r1 = respond_(m, chat, params) $(_piece : string) {
        return true
    }
    to_log(LOG_INFO, "omni showcase turn 1 (image): {replace(r1, "\n", "\\n")}\n")
    t |> success(find(to_lower(r1), "cat") >= 0, "turn 1 describes the cats")
    // the follow-up decodes across the mrope delta: its prefill and every decode step
    // rope at grid-advanced positions, and the answer needs the image turn's content
    add_user_(chat, "was my previous question about animals? answer yes or no")
    let r2 = respond_(m, chat, params) $(_piece : string) {
        return true
    }
    to_log(LOG_INFO, "omni showcase turn 2 (text): {replace(r2, "\n", "\\n")}\n")
    t |> success(find(to_lower(r2), "yes") >= 0, "turn 2 recalls the image turn across the delta'd history")
    t |> success(chat.session.rope_pos_delta == 20l - 300l, "the delta persists across turns")
    emb <- chat.embedder   // the chat took the tower; the block's form gets it back
    delete chat
}
