options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require dasllama/dasllama
require dasllama/dasllama_vision_io
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require daslib/strings_boost
require strings
require math
require _model_tier   // models_dir() + model_available() gate
require ?das_metal dasllama/dasllama_metal_tower   // the tower engage/knob legs (the facade registers the hook)

// The image chat turn: the prompt stream shape around the soft-token splice (tier 2) and the
// greedy caption (tier 3). Pinned: marker ids, media-first order, span length from the
// geometry, and that the model describes what is in the picture. NOT token-parity with
// llama-mtmd-cli — tests/CLAUDE.md carries why.

let MM = "mmproj-gemma-4-12B-it-BF16.gguf"
let DEC = "gemma-4-12B-it-Q4_K_M.gguf"
let MM_E2B = "mmproj-gemma-4-E2B-it-bf16.gguf"   // the gemma4v ViT pair (E-series)
let DEC_E2B = "gemma-4-E2B-it-Q8_0.gguf"
let CATS = "coco-val2017-000000039769.jpg"

// the pinned marker ids (gemma-4 vocab; ggml-vocab-gemma-4.gguf)
let IMAGE_OPEN = 255999l
let IMAGE_CLOSE = 258882l
// 640x480 letterboxes to 624x480 = 13x10 superpatches (mtmd oracle: image_tokens->nx = 130)
let CATS_ROWS = 130l
let MM_G3 = "mmproj-gemma-3-4b-it-F16.gguf"   // the gemma3 SigLIP pair
let DEC_G3 = "gemma-3-4b-it-Q8_0.gguf"
let MM_G3_12B = "mmproj-gemma-3-12b-it-F16.gguf"   // the same tower at projection width 3840
let DEC_G3_12B = "gemma-3-12b-it-Q4_K_M.gguf"      // large tier: DASLLAMA_PARITY_FULL gates it
// gemma3 vocab: <start_of_image> / <end_of_image>; the fixed 896² canvas always yields 256 rows
let G3_IMAGE_OPEN = 255999l
let G3_IMAGE_CLOSE = 256000l
let G3_ROWS = 256l
let MM_OMNI = "mmproj-Qwen3-Omni-30B-A3B-Instruct-bf16.gguf"   // the qwen3v pair (mrope decoder)
let DEC_OMNI = "Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf"         // large tier: DASLLAMA_PARITY_FULL gates it
let MM_4B = "mmproj-Qwen3VL-4B-Instruct-F16.gguf"              // the deepstack pair (small tier)
let DEC_4B = "Qwen3VL-4B-Instruct-Q8_0.gguf"
let MM_25O = "mmproj-Qwen2.5-Omni-3B-f16.gguf"                 // the qwen25v pair (small tier; qwen2vl MROPE decoder)
let DEC_25O = "Qwen2.5-Omni-3B-Q8_0.gguf"
// qwen vocab: <|vision_start|> / <|vision_end|>; 640x480 is 32-aligned = a 20x15 merged grid
let Q_IMAGE_OPEN = 151652l
let Q_IMAGE_CLOSE = 151653l
let Q_CATS_ROWS = 300l

[test]
def test_vision_chat(t : T?) {   // nolint:STYLE038 — one sequential turn story: shape → caption → tower legs
    t |> run("the image turn: stream shape, then the caption") @(tt : T?) {
        let mm = path_join(models_dir(), MM)
        let dec = path_join(models_dir(), DEC)
        let cats = path_join(models_dir(), CATS)
        if (!model_available(tt, mm) || !model_available(tt, dec)) {
            return
        }
        if (!stat(cats).is_valid) {
            tt |> skip("{CATS} not present - fetch it into the models dir (gemma4-vision-oracle/mint.sh documents the URL)")
            return
        }
        let img <- load_image_rgb(cats)
        tt |> equal(img.width, 640, "cats fixture width")
        tt |> equal(img.height, 480, "cats fixture height")
        var m <- load_model_(dec)
        allow_cpu_prefill()
        with_job_que() {
            setup_dasllama_jobque_()
            var tower0_encodes = 0l   // nolint:LINT002,LINT019 — read in the apple static_if half; the linux flavor sees it unused
            static_if (typeinfo builtin_module_exists(das_metal)) {
                set_metal_tower(true)
                tower0_encodes = metal_tower_stats().encodes
            }
            var emb <- load_vision_embedder(mm)
            var chat <- create_chat_(m, emb, "", 96l)
            add_user_image_(chat, img)
            add_user_(chat, "describe this image")
            tt |> equal(chat.n_image_rows, CATS_ROWS, "image span length from the geometry")
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
            tt |> success(mixed, "audio beside a queued image panics instead of dropping rows")
            // TIER 2 - the stream shape: head ends at the open marker, tail opens at the close one
            var head : array<int64>
            var tail : array<int64>
            render_prompt_image(m, chat, head, tail)
            tt |> success(!empty(head) && !empty(tail), "both spans render")
            tt |> equal(head[long_length(head) - 1l], IMAGE_OPEN, "head ends with <|image>")
            tt |> equal(tail[0], IMAGE_CLOSE, "tail starts with <image|>")
            // media-first: the user text sits in the TAIL, after the span (llama.cpp's order)
            let tail_text = decode_(m, tail)
            tt |> success(find(tail_text, "describe this image") >= 0,
                "the user text follows the image span (tail: {replace(tail_text, "\n", "\\n")})")
            let head_text = decode_(m, head)
            tt |> success(find(head_text, "describe this image") < 0,
                "the head carries no user text (head: {replace(head_text, "\n", "\\n")})")
            // TIER 3 - the caption. Greedy; the full text is logged either way (the eyeball rail)
            var params = SamplingParams()
            params.temp = 0.0
            let reply = respond_(m, chat, params) $(piece : string) {
                return true
            }
            to_log(LOG_INFO, "vision caption: {replace(reply, "\n", "\\n")}\n")
            let low = to_lower(reply)
            tt |> success(find(low, "cat") >= 0, "the cats are described as cats")
            tt |> success(length(reply) > 40, "the caption is a description, not a fragment")
            delete chat
            // The tower legs. The caption is the FLOOR, not token parity
            // (freeform token cells are banned; the soft-token numeric bars live in the tier-1 GPU
            // and gemma4uv-metal gates) - here the contract is engage evidence plus "still cats"
            // with the knob off.
            static_if (typeinfo builtin_module_exists(das_metal)) {
                tt |> equal(metal_tower_stats().encodes - tower0_encodes, 1l,
                            "the image encode above was GPU-served (encodes delta)")
                set_metal_tower(false)
                let sc0 = metal_tower_stats()
                var emb2 <- load_vision_embedder(mm)   // fresh: create_chat_ consumed `emb`
                var chat2 <- create_chat_(m, emb2, "", 96l)
                add_user_image_(chat2, img)
                add_user_(chat2, "describe this image")
                let reply2 = respond_(m, chat2, params) $(piece : string) {
                    return true
                }
                to_log(LOG_INFO, "vision caption (tower off): {replace(reply2, "\n", "\\n")}\n")
                let low2 = to_lower(reply2)
                tt |> success(find(low2, "cat") >= 0, "knob-off leg still describes the cats")
                tt |> success(length(reply2) > 40, "knob-off caption is a description, not a fragment")
                let sc1 = metal_tower_stats()
                tt |> equal(sc1.encodes, sc0.encodes, "knob-off leg never dispatched")
                tt |> equal(sc1.declines - sc0.declines, 1l, "knob-off leg recorded the knob decline")
                set_metal_tower(true)
                delete chat2
            }
        }
        delete m
    }
}

// The same turn through the gemma4v ViT tower (E-series): the carrier sniffs the family, the
// splice and the span are the shared rail, so tier 2 is the same shape and tier 3 the same floor.
[test]
def test_vision_chat_e2b(t : T?) {
    t |> run("the E2B image turn through the gemma4v tower: stream shape, then the caption") @(tt : T?) {
        let mm = path_join(models_dir(), MM_E2B)
        let dec = path_join(models_dir(), DEC_E2B)
        let cats = path_join(models_dir(), CATS)
        if (!model_available(tt, mm) || !model_available(tt, dec)) {
            return
        }
        if (!stat(cats).is_valid) {
            tt |> skip("{CATS} not present - fetch it into the models dir (gemma4-vision-oracle/mint.sh documents the URL)")
            return
        }
        let img <- load_image_rgb(cats)
        var m <- load_model_(dec, QuantMode.q8)   // E2B serves q8 (its PLE table rides the q8 rail - the fp32 expand has no PLE arm)
        allow_cpu_prefill()
        with_job_que() {
            setup_dasllama_jobque_()
            // a carrier with no vision arm is refused at create, by name
            var no_arm_tripped = false
            try {
                var none = VisionEmbedder()
                var bad <- create_chat_(m, none, "", 8l)
                delete bad
            } recover {
                no_arm_tripped = true
            }
            tt |> success(no_arm_tripped, "create_chat_ refuses an embedder with no vision arm")
            var emb <- load_vision_embedder(mm)
            tt |> equal(vision_family(emb), "gemma4v", "the E2B mmproj sniffs as gemma4v")
            tt |> equal(vision_align(emb), 48l, "48 px per soft token (patch 16 x merge 3)")
            var chat <- create_chat_(m, emb, "", 96l)
            add_user_image_(chat, img)
            add_user_(chat, "describe this image")
            tt |> equal(chat.n_image_rows, CATS_ROWS, "image span length from the geometry (mtmd: 130)")
            var head : array<int64>
            var tail : array<int64>
            render_prompt_image(m, chat, head, tail)
            tt |> success(!empty(head) && !empty(tail), "both spans render")
            tt |> equal(head[long_length(head) - 1l], IMAGE_OPEN, "head ends with <|image>")
            tt |> equal(tail[0], IMAGE_CLOSE, "tail starts with <image|>")
            var params = SamplingParams()
            params.temp = 0.0
            let reply = respond_(m, chat, params) $(piece : string) {
                return true
            }
            to_log(LOG_INFO, "vision caption (E2B/gemma4v): {replace(reply, "\n", "\\n")}\n")
            let low = to_lower(reply)
            tt |> success(find(low, "cat") >= 0, "the cats are described as cats")
            tt |> success(length(reply) > 40, "the caption is a description, not a fragment")
            delete chat
        }
        delete m
    }
}

// The same turn through the gemma3 SigLIP tower: a THIRD family through the same splice rail —
// its markers come from the gemma3 arch and its span is the fixed 256 (896² canvas).
[test]
def test_vision_chat_gemma3(t : T?) {
    t |> run("the gemma-3-4b image turn through the gemma3v tower: stream shape, then the caption") @(tt : T?) {
        let mm = path_join(models_dir(), MM_G3)
        let dec = path_join(models_dir(), DEC_G3)
        let cats = path_join(models_dir(), CATS)
        if (!model_available(tt, mm) || !model_available(tt, dec)) {
            return
        }
        if (!stat(cats).is_valid) {
            tt |> skip("{CATS} not present - fetch it into the models dir (gemma4-vision-oracle/mint.sh documents the URL)")
            return
        }
        let img <- load_image_rgb(cats)
        var m <- load_model_(dec, QuantMode.q8)
        allow_cpu_prefill()
        with_job_que() {
            setup_dasllama_jobque_()
            var emb <- load_vision_embedder(mm)
            tt |> equal(vision_family(emb), "gemma3v", "the gemma-3 mmproj sniffs as gemma3v")
            tt |> equal(vision_align(emb), 56l, "56 px per soft token (patch 14 x merge 4)")
            var chat <- create_chat_(m, emb, "", 96l)
            add_user_image_(chat, img)
            add_user_(chat, "describe this image")
            tt |> equal(chat.n_image_rows, G3_ROWS, "the fixed canvas always splices 256 rows")
            var head : array<int64>
            var tail : array<int64>
            render_prompt_image(m, chat, head, tail)
            tt |> success(!empty(head) && !empty(tail), "both spans render")
            tt |> equal(head[long_length(head) - 1l], G3_IMAGE_OPEN, "head ends with <start_of_image>")
            tt |> equal(tail[0], G3_IMAGE_CLOSE, "tail starts with <end_of_image>")
            var params = SamplingParams()
            params.temp = 0.0
            let reply = respond_(m, chat, params) $(piece : string) {
                return true
            }
            to_log(LOG_INFO, "vision caption (gemma-3-4b/gemma3v): {replace(reply, "\n", "\\n")}\n")
            let low = to_lower(reply)
            tt |> success(find(low, "cat") >= 0, "the cats are described as cats")
            tt |> success(length(reply) > 40, "the caption is a description, not a fragment")
            delete chat
        }
        delete m
    }
}

// The 12B gemma3 pair: the SAME SigLIP tower at projection width 3840 — the one dimension the
// family varies per decoder; large tier, so DASLLAMA_PARITY_FULL gates it.
[test]
def test_vision_chat_gemma3_12b(t : T?) {
    t |> run("the gemma-3-12b image turn: the widened projection, then the caption") @(tt : T?) {
        let mm = path_join(models_dir(), MM_G3_12B)
        let dec = path_join(models_dir(), DEC_G3_12B)
        let cats = path_join(models_dir(), CATS)
        if (!model_available(tt, mm) || !model_available(tt, dec)) {
            return
        }
        if (!stat(cats).is_valid) {
            tt |> skip("{CATS} not present - fetch it into the models dir (gemma4-vision-oracle/mint.sh documents the URL)")
            return
        }
        let img <- load_image_rgb(cats)
        var m <- load_model_(dec)   // the Q4_K_M file picks its serving mode like the gemma-4 12B turn
        allow_cpu_prefill()
        with_job_que() {
            setup_dasllama_jobque_()
            var emb <- load_vision_embedder(mm)
            tt |> equal(vision_family(emb), "gemma3v", "the 12B mmproj sniffs as gemma3v")
            tt |> equal(vision_proj_dim(emb), 3840l, "projection width matches the 12B decoder")
            var chat <- create_chat_(m, emb, "", 96l)
            add_user_image_(chat, img)
            add_user_(chat, "describe this image")
            tt |> equal(chat.n_image_rows, G3_ROWS, "the fixed canvas always splices 256 rows")
            var params = SamplingParams()
            params.temp = 0.0
            let reply = respond_(m, chat, params) $(piece : string) {
                return true
            }
            to_log(LOG_INFO, "vision caption (gemma-3-12b/gemma3v): {replace(reply, "\n", "\\n")}\n")
            let low = to_lower(reply)
            tt |> success(find(low, "cat") >= 0, "the cats are described as cats")
            tt |> success(length(reply) > 40, "the caption is a description, not a fragment")
            delete chat
        }
        delete m
    }
}

// The same turn through the qwen3v tower (Qwen3-Omni): the mrope leg — grid rope positions
// through the uniform span, position advance by the grid's long side. Large tier.
[test]
def test_vision_chat_omni(t : T?) {
    t |> run("the Omni image turn through the qwen3v tower: stream shape, the grid, the caption") @(tt : T?) {
        let mm = path_join(models_dir(), MM_OMNI)
        let dec = path_join(models_dir(), DEC_OMNI)
        let cats = path_join(models_dir(), CATS)
        if (!model_available(tt, mm) || !model_available(tt, dec)) {
            return
        }
        if (!stat(cats).is_valid) {
            tt |> skip("{CATS} not present - fetch it into the models dir (gemma4-vision-oracle/mint.sh documents the URL)")
            return
        }
        let img <- load_image_rgb(cats)
        var m <- load_model_(dec, QuantMode.q8)
        tt |> success(m.config.rope_sections.x + m.config.rope_sections.y + m.config.rope_sections.z > 0,
            "the decoder carries rope.dimension_sections ({m.config.rope_sections})")
        allow_cpu_prefill()
        with_job_que() {
            setup_dasllama_jobque_()
            var emb <- load_vision_embedder(mm)
            tt |> equal(vision_family(emb), "qwen3v", "the Omni mmproj sniffs as qwen3v")
            tt |> equal(vision_proj_dim(emb), 2048l, "projection width matches the Omni decoder")
            var chat <- create_chat_(m, emb, "", 96l)
            add_user_image_(chat, img)
            add_user_(chat, "describe this image")
            tt |> equal(chat.n_image_rows, Q_CATS_ROWS, "640x480 is 32-aligned: 300 soft tokens")
            tt |> equal(chat.image_grid, int2(20, 15), "the merged grid reaches the chat (position advance 20)")
            var head : array<int64>
            var tail : array<int64>
            render_prompt_image(m, chat, head, tail)
            tt |> equal(head[long_length(head) - 1l], Q_IMAGE_OPEN, "head ends with <|vision_start|>")
            tt |> equal(tail[0], Q_IMAGE_CLOSE, "tail starts with <|vision_end|>")
            let tail_text = decode_(m, tail)
            tt |> success(find(tail_text, "describe this image") >= 0,
                "the user text follows the image span (tail: {replace(tail_text, "\n", "\\n")})")
            var params = SamplingParams()
            params.temp = 0.0
            let reply = respond_(m, chat, params) $(piece : string) {
                return true
            }
            to_log(LOG_INFO, "vision caption (omni/qwen3v): {replace(reply, "\n", "\\n")}\n")
            let low = to_lower(reply)
            tt |> success(find(low, "cat") >= 0, "the cats are described as cats")
            tt |> success(length(reply) > 40, "the caption is a description, not a fragment")
            tt |> equal(chat.session.rope_pos_delta, 20l - 300l, "the session's rope delta reflects the grid advance")
            delete chat
        }
        delete m
    }
}

[test]
def test_vision_chat_deepstack(t : T?) {
    t |> run("the Qwen3-VL 4B image turn: deepstack wide rows through the chat, then the caption") @(tt : T?) {
        let mm = path_join(models_dir(), MM_4B)
        let dec = path_join(models_dir(), DEC_4B)
        let cats = path_join(models_dir(), CATS)
        if (!model_available(tt, mm) || !model_available(tt, dec)) {
            return
        }
        if (!stat(cats).is_valid) {
            tt |> skip("{CATS} not present - fetch it into the models dir (gemma4-vision-oracle/mint.sh documents the URL)")
            return
        }
        let img <- load_image_rgb(cats)
        var m <- load_model_(dec, QuantMode.q8)
        tt |> equal(m.config.n_deepstack, 3l, "the decoder carries n_deepstack_layers")
        allow_cpu_prefill()
        with_job_que() {
            setup_dasllama_jobque_()
            var emb <- load_vision_embedder(mm)
            tt |> equal(vision_family(emb), "qwen3v", "the dense mmproj sniffs as qwen3v")
            tt |> equal(vision_proj_dim(emb), 2560l, "projection width matches the 4B decoder")
            tt |> equal(vision_deepstack(emb), 3l, "the tower carries three deepstack taps")
            tt |> equal(vision_row_width(emb), 10240l, "wide rows: (1+3) x 2560")
            var chat <- create_chat_(m, emb, "", 96l)
            add_user_image_(chat, img)
            add_user_(chat, "describe this image")
            tt |> equal(chat.n_image_rows, Q_CATS_ROWS, "640x480 is 32-aligned: 300 soft tokens")
            tt |> equal(chat.image_grid, int2(20, 15), "the merged grid reaches the chat")
            tt |> equal(long_length(chat.image_rows), Q_CATS_ROWS * 10240l, "the chat holds wide rows")
            var params = SamplingParams()
            params.temp = 0.0
            let reply = respond_(m, chat, params) $(piece : string) {
                return true
            }
            to_log(LOG_INFO, "vision caption (qwen3vl-4b/deepstack): {replace(reply, "\n", "\\n")}\n")
            let low = to_lower(reply)
            tt |> success(find(low, "cat") >= 0, "the cats are described as cats")
            tt |> success(length(reply) > 40, "the caption is a description, not a fragment")
            tt |> equal(chat.session.rope_pos_delta, 20l - 300l, "the session's rope delta reflects the grid advance")
            delete chat
            // the zeroed-slices decoder control: the SAME wide rows with their deepstack tails
            // zeroed must move the decoder's logits - a caption gate alone cannot see a decoder
            // that silently ignores the slices (the plan's slice-H requirement)
            var emb2 <- load_vision_embedder(mm)   // create_chat_ moved `emb` into the session
            var st = VisionState()
            var rows : array<float>
            let nr = encode_image(emb2, st, img, "ds-control", rows)
            tt |> equal(nr, Q_CATS_ROWS, "control encode row count")
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
                dmax = max(dmax, abs(s1.logits[i] - s2.logits[i]))
                if (s1.logits[i] > s1.logits[a1]) {
                    a1 = i
                }
                if (s2.logits[i] > s2.logits[a2]) {
                    a2 = i
                }
            }
            to_log(LOG_INFO, "deepstack zeroed-slices: wide picks '{replace(piece_(m, a1), "\n", "\\n")}' ({s1.logits[a1]}), zeroed picks '{replace(piece_(m, a2), "\n", "\\n")}' ({s2.logits[a2]}), logits maxdiff = {dmax}\n")
            tt |> success(dmax > 1e-2, "the decoder consumes the deepstack slices ({dmax})")
            delete s1
            delete s2
            delete rows
            delete zeroed
            delete st
            delete emb2
        }
        delete m
    }
}

[test]
def test_vision_chat_qwen25o(t : T?) {
    t |> run("the Qwen2.5-Omni image turn: window ViT + the NON-interleaved MROPE decoder, then the caption") @(tt : T?) {
        let mm = path_join(models_dir(), MM_25O)
        let dec = path_join(models_dir(), DEC_25O)
        let cats = path_join(models_dir(), CATS)
        if (!model_available(tt, mm) || !model_available(tt, dec)) {
            return
        }
        if (!stat(cats).is_valid) {
            tt |> skip("{CATS} not present - fetch it into the models dir (gemma4-vision-oracle/mint.sh documents the URL)")
            return
        }
        let img <- load_image_rgb(cats)
        var m <- load_model_(dec, QuantMode.q8)
        tt |> equal(m.config.rope_sections, int4(16, 24, 24, 0), "the qwen2vl decoder's sections")
        tt |> success(!m.config.mrope_interleaved, "qwen2vl ropes the sections as contiguous ranges")
        allow_cpu_prefill()
        with_job_que() {
            setup_dasllama_jobque_()
            var emb <- load_vision_embedder(mm)
            tt |> equal(vision_family(emb), "qwen25v", "the dual-tower Omni mmproj sniffs as qwen25v")
            tt |> equal(vision_proj_dim(emb), 2048l, "projection width matches the 3B decoder")
            var chat <- create_chat_(m, emb, "", 96l)
            add_user_image_(chat, img)
            add_user_(chat, "describe this image")
            tt |> equal(chat.n_image_rows, 391l, "640x480 letterboxes to 644x476 at align 28: 391 soft tokens")
            tt |> equal(chat.image_grid, int2(23, 17), "the merged grid reaches the chat (position advance 23)")
            var params = SamplingParams()
            params.temp = 0.0
            let reply = respond_(m, chat, params) $(piece : string) {
                return true
            }
            to_log(LOG_INFO, "vision caption (qwen2.5-omni/qwen25v): {replace(reply, "\n", "\\n")}\n")
            let low = to_lower(reply)
            tt |> success(find(low, "cat") >= 0, "the cats are described as cats")
            tt |> success(length(reply) > 40, "the caption is a description, not a fragment")
            tt |> equal(chat.session.rope_pos_delta, 23l - 391l, "the session's rope delta reflects the grid advance")
            delete chat
        }
        delete m
    }
}

// The bench/scheduler shape: a PLAIN chat (no embedder attached) renders the image span, so
// the Omni vocab-spelling fallback must live at the renderer, not only in the vision
// constructor (lcpp_bench --image builds exactly this turn and panicked on <|vision_start|>).
[test]
def test_render_image_span_qwen25o(t : T?) {
    t |> run("a plain chat renders the Omni image span via the vocab's own spelling") @(tt : T?) {
        let dec = path_join(models_dir(), DEC_25O)
        if (!model_available(tt, dec)) {
            return
        }
        var m <- load_model_(dec, QuantMode.q8)
        var chat <- create_chat_(m, "", 8l)
        add_user_(chat, "describe this image")
        var head : array<int64>
        var tail : array<int64>
        render_turn_image_(m, chat, head, tail)
        tt |> success(!empty(head) && !empty(tail), "both spans render")
        tt |> equal(head[length(head) - 1], special_id(m, "<|vision_bos|>"), "the head closes on the vocab's bos marker")
        delete head
        delete tail
        delete chat
        delete m
    }
}

// The multi-turn showcase: ONE Omni session — an image turn, then a text follow-up whose
// answer requires the image turn's history across the mrope position delta. Audio-in-chat
// for the qwen3a conformer is a standing gap (the chat AudioTower serves the whisper-class
// families only) — the full three-modality showcase lands with that slice.
[test]
def test_omni_showcase(t : T?) {
    t |> run("one Omni session: image turn, then a text turn recalling it") @(tt : T?) {
        let mm = path_join(models_dir(), MM_OMNI)
        let dec = path_join(models_dir(), DEC_OMNI)
        let cats = path_join(models_dir(), CATS)
        if (!model_available(tt, mm) || !model_available(tt, dec)) {
            return
        }
        if (!stat(cats).is_valid) {
            tt |> skip("cats fixture not present in the models dir")
            return
        }
        let img <- load_image_rgb(cats)
        var m <- load_model_(dec, QuantMode.q8)
        allow_cpu_prefill()
        with_job_que() {
            setup_dasllama_jobque_()
            var emb <- load_vision_embedder(mm)
            var chat <- create_chat_(m, emb, "", 96l)
            var params = SamplingParams()
            params.temp = 0.0
            add_user_image_(chat, img)
            add_user_(chat, "describe this image in one sentence")
            let r1 = respond_(m, chat, params) $(piece : string) {
                return true
            }
            to_log(LOG_INFO, "omni showcase turn 1 (image): {replace(r1, "\n", "\\n")}\n")
            tt |> success(find(to_lower(r1), "cat") >= 0, "turn 1 describes the cats")
            // the follow-up decodes across the mrope delta: its prefill and every decode step
            // rope at grid-advanced positions, and the answer needs the image turn's content
            add_user_(chat, "was my previous question about animals? answer yes or no")
            let r2 = respond_(m, chat, params) $(piece : string) {
                return true
            }
            to_log(LOG_INFO, "omni showcase turn 2 (text): {replace(r2, "\n", "\\n")}\n")
            tt |> success(find(to_lower(r2), "yes") >= 0, "turn 2 recalls the image turn across the delta'd history")
            tt |> success(chat.session.rope_pos_delta == 20l - 300l, "the delta persists across turns")
            delete chat
        }
        delete m
    }
}
