// Tutorial OPENAI-09: Image Generation
//
// This tutorial covers generating images from a text prompt:
//   - Building an ImageRequest (prompt, size, count, response_format)
//   - generate_image and the ImageResult / ImageResponse shape
//   - Reading the returned url / revised_prompt
//
// Prerequisites: OPENAI-01 (client + result handling)
//
// Every dasOPENAI script needs `options rtti` — see OPENAI-01. To generate real
// images, point base_url at a backend whose /images/generations endpoint is
// OpenAI-compatible (e.g. OpenAI or OpenRouter with a key).
//
// Run: daslang.exe tutorials/dasOPENAI/09_image_generation.das

options gen2
options rtti
options persistent_heap
options gc

require openai/openai_images
require tutorial_openai_server

let SERVER_PORT = 18192

[export]
def main() {
    with_openai_server(SERVER_PORT) $(base_url) {
        let client = openai_client(base_url)

        // ──────────────────────────────────────────────────────────────────
        // Build the request. Only `prompt` is required; the rest are optional
        // and are omitted from the wire payload when left unset (thanks to
        // `options rtti` honoring @optional).
        // ──────────────────────────────────────────────────────────────────
        let req = ImageRequest(prompt = "a serene mountain lake at dawn",
            size = "1024x1024", n = 1, response_format = "url")
        let res = generate_image(client, req)
        if (!res.ok) {
            print("error [{res.error.kind}/{res.error.status}]: {res.error.message}\n")
        }
        assert(res.ok, "image generation failed")
        assert(!empty(res.response.data), "no images returned")

        // ──────────────────────────────────────────────────────────────────
        // Read each generated image. With response_format "url" the bytes live
        // at `url`; with "b64_json" they arrive base64-encoded in `b64_json`.
        // `revised_prompt` is the prompt the model actually used, if it rewrote
        // yours.
        // ──────────────────────────────────────────────────────────────────
        for (img in res.response.data) {
            print("url:            {img.url}\n")
            print("revised prompt: {img.revised_prompt}\n")
            assert(!empty(img.url), "image url is empty")
        }
    }
}
