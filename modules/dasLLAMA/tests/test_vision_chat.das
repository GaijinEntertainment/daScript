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
