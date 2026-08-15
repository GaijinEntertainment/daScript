options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require dasllama/dasllama
require dasllama/dasllama_vision_io
require dasllama/dasllama_math   // setup_dasllama_jobque_
require daslib/jobque_boost
require daslib/fio
require daslib/strings_boost
require strings
require _model_tier   // models_dir() + model_available() gate

// The image chat turn: the prompt stream shape around the soft-token splice (tier 2) and the
// greedy caption (tier 3). Pinned: marker ids, media-first order, span length from the
// geometry, and that the model describes what is in the picture. NOT token-parity with
// llama-mtmd-cli — tests/CLAUDE.md carries why.

let MM = "mmproj-gemma-4-12B-it-BF16.gguf"
let DEC = "gemma-4-12B-it-Q4_K_M.gguf"
let CATS = "coco-val2017-000000039769.jpg"

// the pinned marker ids (gemma-4 vocab; ggml-vocab-gemma-4.gguf)
let IMAGE_OPEN = 255999l
let IMAGE_CLOSE = 258882l
// 640x480 letterboxes to 624x480 = 13x10 superpatches (mtmd oracle: image_tokens->nx = 130)
let CATS_ROWS = 130l

[test]
def test_vision_chat(t : T?) {
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
            var emb <- load_gemma4uv_embedder(mm)
            var chat <- create_chat_(m, emb, "", 96l)
            add_user_image_(chat, img)
            add_user_(chat, "describe this image")
            tt |> equal(chat.n_image_rows, CATS_ROWS, "image span length from the geometry")
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
        }
        delete m
    }
}
