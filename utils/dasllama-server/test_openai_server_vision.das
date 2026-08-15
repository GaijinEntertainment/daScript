options gen2
options persistent_heap   // the server thread loads/frees a multi-GB model once; explicit deletes below
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // ggml-parity FP laxity — this is an inference tool test, not the bit-exact suite

require dastest/testing_boost public
require openai_server                                   // set_model / set_model_vision / run_server (sibling)
require dasllama/dasllama                               // load_model / QuantMode
require daslib/jobque_boost                             // with_job_status (server-thread lifecycle)
require daslib/json_boost                               // read_json
require daslib/base64                                   // the request's data: URI payload
require daslib/fio
require strings
require dasllama/dasllama_env

// The image route end to end: data URI -> vision worker -> soft tokens spliced between the two
// rendered spans -> a caption.
// Asserts: 200; usage counts the soft-token rows as prompt positions; the reply says "cat".
// Splice arithmetic is pinned unit-side (dasLLAMA tests/test_scheduler.das media arms).
// Model-gated (DASLLAMA_PARITY_FULL tier: 6.87 GiB decoder) + JIT-only; skips cleanly otherwise.

let TEST_PORT = 18097

def private models_dir() : string {
    return models_dir_resolved()   // the ONE resolution: DASLLAMA_MODELS_DIR, else the rig default
}
let DEC_PATH = path_join(models_dir(), "gemma-4-12B-it-Q4_K_M.gguf")
let MM_PATH = path_join(models_dir(), "mmproj-gemma-4-12B-it-BF16.gguf")
let CATS_PATH = path_join(models_dir(), "coco-val2017-000000039769.jpg")

// 640x480 letterboxes to 624x480 = 13x10 superpatches (mtmd oracle: image_tokens->nx = 130)
let CATS_ROWS = 130

def private with_vision_server(port : int; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            allow_cpu_prefill()   // the Metal-build CPU-prefill guard panics on an undeclared CPU prefill; this test serves on the CPU
            var m <- load_model(DEC_PATH, QuantMode.q8)
            set_model(m, "test-vlm")
            set_model_vision("test-vlm", MM_PATH)
            set_default_max_tokens(24l)
            run_server(port)   // blocks until POST /shutdown
            finished |> notify_and_release
        }
        let base_url = "http://127.0.0.1:{port}"
        var ready = false
        for (_attempt in range(1200)) {   // up to ~120s: a 12B load + mmproj + JIT
            GET("{base_url}/v1/models") $(resp) {
                if (resp != null && resp.status_code == http_status.OK) {
                    ready = true
                }
            }
            if (ready) {
                break
            }
            sleep(100u)
        }
        if (ready) {
            invoke(blk, base_url)
        }
        POST("{base_url}/shutdown", "") $(_resp) {}
        finished |> join
        assert(ready, "dasllama vision server became ready")
    }
}

// the cats jpeg as the data: URI a browser's FileReader would produce (byte read, not fread(path):
// a jpeg carries interior NULs, which a string read would truncate at)
def private cats_data_uri() : string {
    var inscope bytes : array<uint8>
    fopen(CATS_PATH, "rb") $(f) {
        if (f != null) {
            bytes |> resize(int64(fstat(f).size))
            fread(f, bytes)
        }
    }
    return empty(bytes) ? "" : "data:image/jpeg;base64,{base64_encode(bytes)}"
}

[test]
def test_openai_server_vision(t : T?) {   // nolint:STYLE038 — one server boot, every route-proof cell against it
    t |> run("the image route: data URI -> soft tokens -> a caption (needs gemma-4-12B + its mmproj)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA server tests are JIT-only)")
            return
        }
        for (p in [DEC_PATH, MM_PATH, CATS_PATH]) {
            if (!stat(p).is_valid) {
                t |> success(true, "skipped: {base_name(p)} not present (set DASLLAMA_MODELS_DIR)")
                return
            }
        }
        if (!g_env_test.parity_full) {   // 6.87 GiB decoder: over the 6 GiB tier line
            t |> success(true, "skipped: the 12B pair runs under DASLLAMA_PARITY_FULL=1 (final pre-PR gate, not the iteration loop)")
            return
        }
        let uri = cats_data_uri()
        t |> success(length(uri) > 1000, "the cats jpeg encoded to a data URI ({length(uri)} chars)")
        with_vision_server(TEST_PORT) $(base_url) {
            // the arm is declared on the stats surface (the page gates its attach button on this)
            GET("{base_url}/v1/stats") $(resp) {
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null && (js?["vision"] ?? false), "stats reports the vision arm")
                if (js != null) {
                    unsafe {
                        delete js
                    }
                }
            }
            var body = "\{\"messages\":[\{\"role\":\"user\",\"content\":["
            body += "\{\"type\":\"image_url\",\"image_url\":\{\"url\":\"{uri}\"}},"
            body += "\{\"type\":\"text\",\"text\":\"What animals are in this picture? Answer in one short sentence.\"}"
            body += "]}],\"max_tokens\":32,\"temperature\":0}"
            POST("{base_url}/v1/chat/completions", body) $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "the image chat response parses")
                if (js != null) {
                    // usage counts the soft-token rows as prompt positions: a text-only render of
                    // this turn is well under 100 tokens, so 130 rows have to be in there
                    let np = int(js?["usage"]?["prompt_tokens"] ?? 0l)
                    t |> success(np > CATS_ROWS, "prompt_tokens {np} includes the {CATS_ROWS} image rows")
                    let reply = to_lower(js?["choices"]?[0]?["message"]?["content"] ?? "")
                    t |> success(!empty(reply), "the reply is not empty")
                    t |> success(find(reply, "cat") >= 0, "the model calls the cats cats: '{reply}'")
                    unsafe {
                        delete js
                    }
                }
            }
            // a malformed data URI is a 400 from the decode arm, not a panic on the tick thread
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":[\{\"type\":\"image_url\",\"image_url\":\{\"url\":\"data:image/png;base64,bm90YW5pbWFnZQ==\"}},\{\"type\":\"text\",\"text\":\"hi\"}]}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
                t |> success(find(string(resp.body), "could not decode") >= 0, "the 400 names the decode failure")
            }
            // a remote URL is refused by name — the server never fetches on its request thread
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":[\{\"type\":\"image_url\",\"image_url\":\{\"url\":\"https://example.com/cat.png\"}},\{\"type\":\"text\",\"text\":\"hi\"}]}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
                t |> success(find(string(resp.body), "data: URI") >= 0, "the 400 names the data-URI requirement")
            }
            // a data URI with no base64 marker, and one with garbage payload — each its own 400
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":[\{\"type\":\"image_url\",\"image_url\":\{\"url\":\"data:image/png,rawbytes\"}},\{\"type\":\"text\",\"text\":\"hi\"}]}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
                t |> success(find(string(resp.body), "base64-encoded") >= 0, "the 400 names the missing base64 marker")
            }
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":[\{\"type\":\"image_url\",\"image_url\":\{\"url\":\"data:image/png;base64,@@@@\"}},\{\"type\":\"text\",\"text\":\"hi\"}]}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
                t |> success(find(string(resp.body), "not valid base64") >= 0, "the 400 names the bad payload")
            }
            // an image on an EARLIER message drops with a warning; the request still serves as text
            var early = "\{\"messages\":[\{\"role\":\"user\",\"content\":["
            early += "\{\"type\":\"image_url\",\"image_url\":\{\"url\":\"{uri}\"}},"
            early += "\{\"type\":\"text\",\"text\":\"look at this\"}]},"
            early += "\{\"role\":\"assistant\",\"content\":\"ok\"},"
            early += "\{\"role\":\"user\",\"content\":\"say hi\"}],\"max_tokens\":8,\"temperature\":0}"
            POST("{base_url}/v1/chat/completions", early) $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                if (js != null) {
                    let np = int(js?["usage"]?["prompt_tokens"] ?? 0l)
                    t |> success(np < CATS_ROWS, "no rows spliced for a dropped earlier-message image (prompt_tokens {np})")
                    unsafe {
                        delete js
                    }
                }
            }
            // the per-slot stats carry the arm and the thinking default the page's roster reads
            GET("{base_url}/v1/stats") $(resp) {
                var err : string
                var js = read_json(string(resp.body), err)
                let slot0 = js?["models"]?[0]
                t |> success(slot0?["vision"] ?? false, "the slot row reports its vision arm")
                t |> success(slot0?["thinking_default"] != null, "the slot row reports its thinking default")
                if (js != null) {
                    unsafe {
                        delete js
                    }
                }
            }
        }
    }
}
