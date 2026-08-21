options gen2
options persistent_heap   // the server thread loads/frees a multi-GB model once; explicit deletes below
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // ggml-parity FP laxity — this is an inference tool test, not the bit-exact suite
options _dasllama_internal = true

require dastest/testing_boost public
require openai_server                                   // the server: OpenAiServer + set_model + run_server (sibling)
require dasllama/dasllama                               // load_model / QuantMode / embed
require daslib/jobque_boost                             // with_job_status (server-thread lifecycle)
require daslib/json_boost                               // read_json / from_JV
require daslib/fio                                      // get_env_variable / stat / path_join / sleep / base_name
require daslib/toml                                     // read_toml: POST /config round-trip check
require math                                            // sqrt
require strings                                         // find (control-page content-type/body checks)
require dasllama/dasllama_env

// End-to-end conformance for the OpenAI-compatible server: spin it up on its own thread (a separate
// context — so the model loads INSIDE that thread, since g_model is a per-context module global),
// then drive /v1/models, /v1/embeddings and /v1/chat/completions over the real dashv HTTP client.
// Model-gated + JIT-only, so it's a real regression locally and a clean skip in CI (no GGUF).
// Lives here (not modules/dasLLAMA/tests/) because a require path can't contain the '-' in "dasllama-server":
// a require_module_name is NAME tokens joined by / . % .. — so hyphenated-dir tools keep tests in-dir
// and require the sibling module by BARE name (same convention as detect-dupe / find-dupe / mcp).

let TEST_PORT = 18099
// POST /config target; under the platform temp dir so a mid-test failure leaves no stray file
// in the repo. Module-level let evaluates per context, but the temp dir is stable — both agree.
def private test_save_path() : string {
    var err = ""
    let d = temp_directory(err)
    return path_join(empty(d) ? "/tmp" : d, "dasllama_test_config.toml")
}
let TEST_SAVE_PATH = test_save_path()

def private models_dir() : string {
    return models_dir_resolved()   // the ONE resolution: DASLLAMA_MODELS_DIR, else the rig default
}
let MODEL_PATH = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")

// Response shapes we assert on (from_JV ignores the object/model/usage fields we don't check).
struct private EmbItem {
    embedding : array<float>
    index : int
}
struct private EmbResp {
    data : array<EmbItem>
}

def private l2(v : array<float>) : float {
    var s = 0.0
    for (x in v) {
        s += x * x
    }
    return sqrt(s)
}

def private vecs_equal(a, b : array<float>) : bool {
    if (length(a) != length(b)) {
        return false
    }
    for (x, y in a, b) {
        if (x != y) {
            return false
        }
    }
    return true
}

// Start the server on a dedicated thread (model loaded in-thread), wait until it answers /v1/models,
// run the block, then POST /shutdown (flips the server context's stop flag) and join. `mode`
// arms the Metal pipeline in the server thread's context BEFORE the load (the --metal flag's
// engine effect — the != off backend pin must precede load_model).
def private with_llama_server(model_path : string; port : int; mode : MetalMode = MetalMode.off; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            set_metal_mode(mode)
            // the Metal-build CPU-prefill guard panics on an undeclared CPU prefill; the metal
            // arm keeps it ARMED — there it is the silent-GPU-fallback detector
            if (mode == MetalMode.off) {
                allow_cpu_prefill()
            }
            var m <- load_model(model_path, QuantMode.q8)
            set_model(m, "test-llm")
            set_default_max_tokens(4l)   // per-context global — exercised by the default-budget completion test
            // the /config surface as main.das registers it (per-context globals — set in-thread)
            var surf <- {
                "config" => JV((port = port, model = model_path, streams = 4)),
                "sources" => JV((port = "cli")),
                "authoritative" => JV(false),
                "config_path" => JV(""),
                "save_path" => JV(TEST_SAVE_PATH)
            }
            var sdoc = JV(surf)
            set_config_surface(write_json_compact(sdoc), TEST_SAVE_PATH, model_path)
            unsafe {
                delete sdoc
            }
            run_server(port)   // blocks (with_job_que + tick loop) until POST /shutdown
            finished |> notify_and_release
        }
        let base_url = "http://127.0.0.1:{port}"
        var ready = false
        for (_attempt in range(300)) {   // up to ~30s for load + JIT
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
        assert(ready, "dasllama server became ready")
    }
}

[test]
def test_openai_server(t : T?) {   // nolint:STYLE038 — one live server session per test: every endpoint assertion runs against that one boot
    t |> run("OpenAI server: models + embeddings + chat (needs tinyllama q8)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA server test is JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid) {
            t |> success(true, "skipped: {base_name(MODEL_PATH)} not present (set DASLLAMA_MODELS_DIR)")
            return
        }
        with_llama_server(MODEL_PATH, TEST_PORT) $(base_url) {
            // /v1/models
            GET("{base_url}/v1/models") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "models JSON parses")
                if (js != null) {
                    t |> equal(js?["object"] ?? "", "list")
                    unsafe {
                        delete js
                    }
                }
            }
            // GET / — the control page ships beside the server sources and serves as text/html
            GET("{base_url}/") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                t |> success(find(get_header(resp, "Content-Type"), "text/html") >= 0, "control page is text/html")
                t |> success(find(string(resp.body), "dasllama-server") >= 0, "control page body served")
            }
            // /v1/stats — identity fields the control page polls
            GET("{base_url}/v1/stats") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "stats JSON parses")
                if (js != null) {
                    t |> equal(js?["model"] ?? "", "test-llm")
                    t |> success((js?["ctx"] ?? 0l) > 0l, "stats reports ctx")
                    t |> success(!(js?["draining"] ?? true), "not draining while serving")
                    t |> success((js?["max_streams"] ?? 0l) > 0l, "stats reports max_streams")
                    unsafe {
                        delete js
                    }
                }
            }
            // /v1/images — the .dlim inventory surface (set_model registers no path, so the
            // slot reports an empty inventory; the SHAPE is what the page contracts on)
            GET("{base_url}/v1/images") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "images JSON parses")
                if (js != null) {
                    t |> equal(js?["baking"] ?? "?", "")
                    let ms = js?["models"]
                    t |> success(ms != null && ms is _array && length(ms as _array) == 1, "one slot listed")
                    if (ms != null && ms is _array && length(ms as _array) == 1) {
                        t |> equal((ms as _array)[0]?["model"] ?? "", "test-llm")
                    }
                    unsafe {
                        delete js
                    }
                }
            }
            // /bake — idle state; POST declines cleanly without a configured spawner
            GET("{base_url}/bake") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null && (js?["state"] ?? "") == "idle", "bake starts idle")
                unsafe {
                    delete js
                }
            }
            POST("{base_url}/bake", "\{}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
                t |> success(find(string(resp.body), "source-tree daslang") >= 0, "bake declines without a spawner")
            }
            // /v1/embeddings — single input: unit-normalized vector of the model's hidden dim
            POST("{base_url}/v1/embeddings", "\{\"input\":\"the quick brown fox\"}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "embeddings JSON parses")
                if (js != null) {
                    t |> equal(js?["object"] ?? "", "list")
                    let r = from_JV(js, type<EmbResp>)
                    t |> success(length(r.data) == 1, "one embedding returned")
                    if (length(r.data) == 1) {
                        t |> success(!empty(r.data[0].embedding), "embedding is non-empty")
                        t |> success(abs(l2(r.data[0].embedding) - 1.0) < 1.0e-4, "embedding is L2-normalized")
                    }
                    unsafe {
                        delete js
                    }
                }
            }
            // /v1/embeddings — batch: identical inputs give identical vectors (deterministic), distinct differ
            POST("{base_url}/v1/embeddings",
                 "\{\"input\":[\"hello world\",\"a different sentence\",\"hello world\"]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                if (js != null) {
                    let r = from_JV(js, type<EmbResp>)
                    t |> success(length(r.data) == 3, "three embeddings returned")
                    if (length(r.data) == 3) {
                        t |> success(vecs_equal(r.data[0].embedding, r.data[2].embedding),
                                     "identical inputs -> identical vectors")
                        t |> success(!vecs_equal(r.data[0].embedding, r.data[1].embedding),
                                     "distinct inputs -> distinct vectors")
                    }
                    unsafe {
                        delete js
                    }
                }
            }
            // /v1/chat/completions (non-streaming) — end-to-end generation smoke
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":\"Say hi.\"}],\"max_tokens\":8}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "chat JSON parses")
                if (js != null) {
                    t |> equal(js?["object"] ?? "", "chat.completion")
                    unsafe {
                        delete js
                    }
                }
            }
            // /v1/completions WITHOUT max_tokens — the configured default budget applies (4 in this
            // harness; llm-cli-style clients omit the field, raw completions run to the budget)
            POST("{base_url}/v1/completions", "\{\"prompt\":\"Once\"}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "default-budget completion JSON parses")
                if (js != null) {
                    t |> equal(int(js?["usage"]?["completion_tokens"] ?? 0l), 4)
                    unsafe {
                        delete js
                    }
                }
            }
            // content-parts array form (what pi/opencode send even for pure text): the text
            // must reach the prompt — an empty-parse would render a near-empty user turn
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":[\{\"type\":\"text\",\"text\":\"Please repeat the word strawberry twenty times, then count the letters in it, then explain why.\"}]}],\"max_tokens\":4}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "parts-form chat JSON parses")
                if (js != null) {
                    t |> success(int(js?["usage"]?["prompt_tokens"] ?? 0l) > 40, "parts text reached the prompt")
                    unsafe {
                        delete js
                    }
                }
            }
            // tools on a template with no tool format (TinyLlama resolves Zephyr) — honest 400
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":\"hi\"}],\"tools\":[\{\"type\":\"function\",\"function\":\{\"name\":\"f\"}}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
            }
            // an image_url part on a slot with NO vision arm: a named 400, never a silent drop
            let one_img = "\{\"role\":\"user\",\"content\":[\{\"type\":\"image_url\",\"image_url\":\{\"url\":\"data:image/png;base64,AAAA\"}},\{\"type\":\"text\",\"text\":\"what is this\"}]}"
            POST("{base_url}/v1/chat/completions", "\{\"messages\":[{one_img}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
                t |> success(find(string(resp.body), "no vision arm") >= 0, "the 400 names the missing vision arm")
            }
            // two images on a TEXT-ONLY slot: the missing arm is the more useful error, so it
            // outranks the count (the one-image rule is pinned on the ARMED slot in the vision test)
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":[\{\"type\":\"image_url\",\"image_url\":\{\"url\":\"data:image/png;base64,AAAA\"}},\{\"type\":\"image_url\",\"image_url\":\{\"url\":\"data:image/png;base64,BBBB\"}}]}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
                t |> success(find(string(resp.body), "no vision arm") >= 0, "the missing arm outranks the image count")
            }
            // an input_audio part on a slot with NO audio arm: same named-400 contract as vision
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":[\{\"type\":\"input_audio\",\"input_audio\":\{\"data\":\"AAAA\",\"format\":\"wav\"}},\{\"type\":\"text\",\"text\":\"what is said\"}]}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
                t |> success(find(string(resp.body), "no audio arm") >= 0, "the 400 names the missing audio arm")
            }
            // an image AND audio in one message: refused by shape before any arm check — the
            // scheduler takes ONE media splice per request
            POST("{base_url}/v1/chat/completions", "\{\"messages\":[\{\"role\":\"user\",\"content\":[\{\"type\":\"image_url\",\"image_url\":\{\"url\":\"data:image/png;base64,AAAA\"}},\{\"type\":\"input_audio\",\"input_audio\":\{\"data\":\"AAAA\",\"format\":\"wav\"}}]}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
                t |> success(find(string(resp.body), "image AND audio") >= 0, "the 400 names the two-media shape")
            }
            // /v1/stats declares the arms the page reads: the vision/audio flags and the thinking default
            GET("{base_url}/v1/stats") $(resp) {
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null && !(js?["vision"] ?? true), "stats reports no vision arm for a text-only slot")
                t |> success(js != null && !(js?["audio"] ?? true), "stats reports no audio arm for a text-only slot")
                // the VALUE, not just presence: tinyllama neither defaults to thinking nor has the
                // reply markers in its vocab, so a true here means the vocab gate fell off
                t |> success(js != null && js?["thinking_default"] != null, "stats declares the family thinking default")
                t |> success(js != null && !(js?["thinking_default"] ?? true), "the declared default is OFF for this family")
                if (js != null) {
                    unsafe {
                        delete js
                    }
                }
            }
            // unknown endpoint hits the ANY("*") catch-all: logged server-side, OpenAI-shaped 404 back
            POST("{base_url}/v1/responses", "\{\"input\":\"hi\"}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.NOT_FOUND))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null && js?["error"] != null, "404 body is an OpenAI error object")
                if (js != null) {
                    unsafe {
                        delete js
                    }
                }
            }
            // prefix-cache round trip: a >1-page completion donates its pages at reap; the SAME
            // prompt re-sent attaches them without prefill (the TTFT-collapse demo's backing)
            var long_prompt = ""
            for (_i in range(12)) {
                long_prompt += "The quick brown fox jumps over the lazy dog. "
            }
            let cache_body = "\{\"prompt\":\"{long_prompt}\",\"max_tokens\":4}"
            POST("{base_url}/v1/completions", cache_body) $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
            }
            POST("{base_url}/v1/completions", cache_body) $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
            }
            GET("{base_url}/v1/stats") $(resp) {
                var err : string
                var js = read_json(string(resp.body), err)
                if (js != null) {
                    t |> success((js?["cached_tokens"] ?? 0l) > 0l, "re-sent prompt hit the prefix cache")
                    unsafe {
                        delete js
                    }
                }
            }
            // GET /config — launcher-registered surface + live status (model picker, gpu badge)
            GET("{base_url}/config") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "config JSON parses")
                if (js != null) {
                    t |> equal(js?["surface"]?["config"]?["port"] ?? 0l, int64(TEST_PORT))
                    t |> equal(js?["surface"]?["sources"]?["port"] ?? "", "cli")
                    t |> success(js?["status"]?["gpu"] != null, "gpu status object present")
                    t |> success(find(string(resp.body), "model_files") >= 0, "model files enumerated")
                    unsafe {
                        delete js
                    }
                }
            }
            // POST /config — validated body writes an authoritative TOML to the registered path
            POST("{base_url}/config", "\{\"model\":\"m.gguf\",\"port\":8123,\"mtp\":true,\"gpu\":\"vulkan\",\"image_mmproj\":\"mm.gguf\"}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                t |> success(find(string(resp.body), "restart_required") >= 0, "save response flags restart")
            }
            {
                let saved = fread(TEST_SAVE_PATH)
                t |> success(!empty(saved), "saved config readable")
                var terr : string
                var troot = read_toml(saved, terr)
                t |> success(troot != null, "saved config re-parses as TOML")
                if (troot != null) {
                    t |> success(troot?["authoritative"] ?? false, "saved config is authoritative")
                    t |> equal(troot?["port"] ?? 0l, 8123l)
                    t |> equal(troot?["model"] ?? "", "m.gguf")
                    t |> success(troot?["mtp"] ?? false, "bool key round-trips")
                    t |> equal(troot?["image_mmproj"] ?? "", "mm.gguf")   // a save must not eat the vision arm
                    unsafe {
                        delete troot
                    }
                }
                remove(TEST_SAVE_PATH)
            }
            // POST /config validation — unknown keys and wrong types are honest 400s
            POST("{base_url}/config", "\{\"bogus\":1}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
            }
            POST("{base_url}/config", "\{\"port\":\"eight\"}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
            }
            // operator-route CSRF gate: a cross-site Origin is refused, and a spoofed non-loopback
            // Host is refused header-less (the DNS-rebinding defense — Origin==Host alone would pass)
            with_http_request() $(var req) {
                req.method = http_method.POST
                req.url := "{base_url}/config"
                set_header(req, "Origin", "http://evil.example")
                req.body := "\{\"port\":8123}"
                request(req) $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.FORBIDDEN), "cross-site /config refused")
                }
            }
            with_http_request() $(var req) {
                req.method = http_method.POST
                req.url := "{base_url}/config"
                set_header(req, "Host", "evil.example:{TEST_PORT}")
                req.body := "\{\"port\":8123}"
                request(req) $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.FORBIDDEN), "rebound non-loopback Host refused")
                }
            }
            // /v1/streams — finished chat streams linger ~10 s with counters + text tails
            GET("{base_url}/v1/streams") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "streams JSON parses")
                if (js != null) {
                    t |> success((js?["max_streams"] ?? 0l) > 0l, "streams reports max_streams")
                    var saw_finished = false
                    var saw_tail = false
                    if (js?["streams"] != null && ((js?["streams"]).value is _array)) {
                        for (e in ((js?["streams"]).value as _array)) {
                            if ((e?["state"] ?? "") == "finished") {
                                saw_finished = true
                                saw_tail = saw_tail || ((e?["n_gen"] ?? 0l) > 0l && !empty(e?["gen_tail"] ?? ""))
                            }
                        }
                    }
                    t |> success(saw_finished, "finished streams linger on the poll surface")
                    t |> success(saw_tail, "a finished stream carries generated text tail")
                    var cache_rows = 0l
                    var cache_hit = false
                    var cache_preview = false
                    if (js?["cache"] != null && ((js?["cache"]).value is _array)) {
                        for (e in ((js?["cache"]).value as _array)) {
                            cache_rows ++
                            cache_hit = cache_hit || (e?["hits"] ?? 0l) > 0l
                            cache_preview = cache_preview || !empty(e?["preview"] ?? "")
                        }
                    }
                    t |> success(cache_rows > 0l, "prefix-cache chains surfaced")
                    t |> success(cache_hit, "a cache chain records its hit")
                    t |> success(cache_preview, "a cache chain carries a detokenized preview")
                    t |> success(js?["asr"] != null && ((js?["asr"]).value is _array), "asr job array present (empty without --asr)")
                    unsafe {
                        delete js
                    }
                }
            }
            // /v1/stats S2 depth — after real work went through the scheduler
            GET("{base_url}/v1/stats") $(resp) {
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "stats JSON parses (post-work)")
                if (js != null) {
                    t |> success((js?["prefill_tokens"] ?? 0l) > 0l, "prefill tokens counted")
                    t |> success((js?["weights_bytes"] ?? 0l) > 100000000l, "weights bytes reported")
                    t |> success((js?["kv_bytes"] ?? 0l) > 0l, "kv pool bytes reported")
                    t |> success((js?["ttft_avg_ms"] ?? 0.0lf) > 0.0lf, "ttft aggregate reported")
                    t |> success(!empty(js?["hardware"] ?? ""), "hardware line composed")
                    t |> equal(js?["asr_done_jobs"] ?? -1l, 0l)
                    unsafe {
                        delete js
                    }
                }
            }
            POST("{base_url}/v1/models/unload", "\{\"model\":\"test-llm\"}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST), "unloading the default is refused")
            }
            // a load that panics inside load_model answers 400, not a dead server (the next cell
            // proves the tick thread survived)
            var tderr = ""
            let td = temp_directory(tderr)
            let junk = to_generic_path(path_join(empty(td) ? "/tmp" : td, "dasllama_test_junk.gguf"))
            fwrite(junk, "not a gguf at all")
            POST("{base_url}/v1/models/load", "\{\"path\":\"{junk}\"}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST), "a corrupt GGUF load answers 400")
                t |> success(find(string(resp.body), "load failed") >= 0, "the 400 names the load failure")
            }
            GET("{base_url}/v1/models") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK), "the server serves on after a failed load")
            }
            // runtime slot lifecycle: load a second model live (activate=false keeps the boot
            // slot default), route a completion to it BY NAME, then move the default onto it and
            // unload the BOOT slot — the branch where every index above the hole shifts down
            let smol = to_generic_path(path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf"))   // forward slashes: a backslash path dies inside the JSON body
            if (stat(smol).is_valid) {
                POST("{base_url}/v1/models/load", "\{\"path\":\"{smol}\",\"id\":\"smol-live\",\"activate\":false,\"ctx\":512}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.OK), "a live load answers 200")
                    t |> success(find(string(resp.body), "smol-live") >= 0, "the load echoes the new slot id")
                    t |> success(find(string(resp.body), "\"ctx\":512") >= 0, "the body ctx clamps the KV pool")
                }
                POST("{base_url}/v1/models/load", "\{\"path\":\"{smol}\",\"id\":\"smol-live\"}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.CONFLICT), "a taken id is a 409")
                }
                POST("{base_url}/v1/models/load", "\{\"path\":\"{smol}\",\"id\":\"smol-other\"}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.CONFLICT), "one slot per GGUF — a second id on the same file is a 409")
                    t |> success(find(string(resp.body), "smol-live") >= 0, "the 409 names the slot already serving the file")
                }
                POST("{base_url}/v1/completions", "\{\"model\":\"smol-live\",\"prompt\":\"The capital of France is\",\"max_tokens\":4}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.OK), "the runtime slot serves by name")
                }
                GET("{base_url}/v1/stats") $(resp) {
                    var err : string
                    var js = read_json(string(resp.body), err)
                    var boot_is_default = false
                    if (js != null && js?["models"] != null && ((js?["models"]).value is _array)) {
                        for (e in ((js?["models"]).value as _array)) {
                            if ((e?["name"] ?? "") == "test-llm") {
                                boot_is_default = e?["is_default"] ?? false
                            }
                        }
                    }
                    t |> success(boot_is_default, "activate=false leaves the boot slot default")
                    if (js != null) {
                        unsafe {
                            delete js
                        }
                    }
                }
                POST("{base_url}/v1/models/activate", "\{\"model\":\"smol-live\"}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.OK), "activate moves the default to the runtime slot")
                }
                POST("{base_url}/v1/models/unload", "\{\"model\":\"test-llm\"}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.OK), "the boot slot unloads once it is not default")
                }
                POST("{base_url}/v1/completions", "\{\"model\":\"smol-live\",\"prompt\":\"hi\",\"max_tokens\":2}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.OK), "the shifted-down slot still serves by name")
                }
                POST("{base_url}/v1/completions", "\{\"prompt\":\"hi\",\"max_tokens\":2}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.OK), "the model-less route follows the shifted default")
                }
                POST("{base_url}/v1/models/unload", "\{\"model\":\"smol-live\"}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST), "the shifted default still refuses to unload")
                }
                POST("{base_url}/v1/completions", "\{\"model\":\"test-llm\",\"prompt\":\"hi\",\"max_tokens\":2}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.NOT_FOUND), "the unloaded boot slot 404s")
                }
                // the tower probe panic is caught, not fatal
                POST("{base_url}/v1/models/load", "\{\"path\":\"{to_generic_path(MODEL_PATH)}\",\"id\":\"boot-back\",\"activate\":false,\"ctx\":512,\"image_mmproj\":\"{junk}\"}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.OK), "a load with a corrupt mmproj still loads the model")
                    t |> success(find(string(resp.body), "\"tower_note\":\"\"") < 0 && find(string(resp.body), "tower_note") >= 0,
                                 "the tower_note carries the arm failure")
                }
                POST("{base_url}/v1/completions", "\{\"model\":\"boot-back\",\"prompt\":\"hi\",\"max_tokens\":2}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.OK), "the text-only degraded slot serves")
                }
            } else {
                t |> success(true, "skipped the runtime-load cells: SmolLM2-135M not present")
            }
            // POST /restart — LAST in the block: the server starts draining after this. The exit-4
            // half of the contract lives in main.das + watchdog.py (not exercisable here).
            POST("{base_url}/restart", "") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                t |> success(find(string(resp.body), "\"restarting\":true") >= 0, "restart response flags restarting")
            }
        }
    }
}

[test]
def test_asr_worker_panic_boundary(t : T?) {
    t |> run("ASR worker boundary returns normally after a panic") @(t : T?) {
        var failure = ""
        var failed = false
        with (module openai_server) {
            failed = catch_worker_panic(failure) {
                panic("forced ASR worker failure")
            }
        }
        t |> success(failed, "panic was caught")
        t |> equal(failure, "forced ASR worker failure")
    }
    t |> run("ASR worker boundary preserves successful execution") @(t : T?) {
        var invoked = false
        var failure = ""
        var failed = true
        with (module openai_server) {
            failed = catch_worker_panic(failure) {
                invoked = true
            }
        }
        t |> success(invoked, "work block ran")
        t |> success(!failed, "successful work was not reported as failed")
        t |> equal(failure, "")
    }
}

// ===== parse_tool_calls (model-free) =====

[test]
def test_openai_server_metal_mode(t : T?) {
    t |> run("OpenAI server: metal mode when_available serves (the --metal flag's engine effect)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA server test is JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid) {
            t |> success(true, "skipped: {base_name(MODEL_PATH)} not present (set DASLLAMA_MODELS_DIR)")
            return
        }
        // when_available: engages the GPU where the gates pass (Apple), quietly stays CPU
        // elsewhere — serving must work identically either way
        with_llama_server(MODEL_PATH, TEST_PORT + 2, MetalMode.when_available) $(base_url) {
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":\"Say hi\"}],\"max_tokens\":8}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "chat JSON parses under metal mode")
                if (js != null) {
                    t |> success(!empty(js?["choices"]?[0]?["message"]?["content"] ?? ""),
                        "chat completion returns content under metal mode")
                    unsafe {
                        delete js
                    }
                }
            }
        }
    }
}

[test]
def test_parse_tool_calls(t : T?) {
    t |> run("single call: content prefix, name, compact arguments, id/type") @(t : T?) {
        var calls : array<OaiToolCall>
        let pre = parse_tool_calls("Let me check.\n<tool_call>\n\{\"name\": \"get_weather\", \"arguments\": \{\"city\": \"Paris\"}}\n</tool_call>",
                                   "<tool_call>", "</tool_call>", "call_7", calls)
        t |> equal(pre, "Let me check.")
        t |> equal(length(calls), 1)
        if (length(calls) == 1) {
            t |> equal(calls[0]._function.name, "get_weather")
            t |> equal(calls[0]._function.arguments, "\{\"city\": \"Paris\"}")
            t |> equal(calls[0].id, "call_7_0")
            t |> equal(calls[0]._type, "function")
        }
        delete calls
    }
    t |> run("two calls parse in order; empty content strips clean") @(t : T?) {
        var calls : array<OaiToolCall>
        let pre = parse_tool_calls("<tool_call>\n\{\"name\": \"a\", \"arguments\": \{}}\n</tool_call>\n<tool_call>\n\{\"name\": \"b\", \"arguments\": \{\"n\": 1}}\n</tool_call>",
                                   "<tool_call>", "</tool_call>", "call_1", calls)
        t |> equal(pre, "")
        t |> equal(length(calls), 2)
        if (length(calls) == 2) {
            t |> equal(calls[0]._function.name, "a")
            t |> equal(calls[0]._function.arguments, "\{}")
            t |> equal(calls[1]._function.name, "b")
            t |> equal(calls[1].id, "call_1_1")
        }
        delete calls
    }
    t |> run("malformed spans keep the reply as content") @(t : T?) {
        var calls : array<OaiToolCall>
        // unterminated span (token budget cut): no calls, full text back
        let full1 = "text <tool_call>\n\{\"name\": \"a\""
        t |> equal(parse_tool_calls(full1, "<tool_call>", "</tool_call>", "c", calls), full1)
        t |> equal(length(calls), 0)
        // unparseable JSON inside a well-formed span: no calls, full text back
        let full2 = "<tool_call>\nnot json\n</tool_call>"
        t |> equal(parse_tool_calls(full2, "<tool_call>", "</tool_call>", "c", calls), full2)
        t |> equal(length(calls), 0)
        delete calls
    }
    t |> run("arguments emitted as a JSON string pass through verbatim") @(t : T?) {
        var calls : array<OaiToolCall>
        parse_tool_calls("<tool_call>\n\{\"name\": \"a\", \"arguments\": \"\{\\\"x\\\": 1}\"}\n</tool_call>",
                         "<tool_call>", "</tool_call>", "c", calls)
        t |> equal(length(calls), 1)
        if (length(calls) == 1) {
            t |> equal(calls[0]._function.arguments, "\{\"x\": 1}")
        }
        delete calls
    }
}

[test]
def test_parse_tool_calls_auto(t : T?) {
    t |> run("Mistral TOOL_CALLS array normalizes to OpenAI calls") @(t : T?) {
        var calls : array<OaiToolCall>
        let pre = parse_tool_calls_auto("checking\n[TOOL_CALLS][\{\"name\":\"search_messages\",\"arguments\":\{\"query\":\"zephyr\"}}]",
                                        "", "", "m", calls)
        t |> equal(pre, "checking")
        t |> equal(length(calls), 1)
        if (length(calls) == 1) {
            t |> equal(calls[0]._function.name, "search_messages")
            t |> equal(calls[0]._function.arguments, "\{\"query\": \"zephyr\"}")
        }
        delete calls
    }
    t |> run("Llama python tag maps parameters to arguments") @(t : T?) {
        var calls : array<OaiToolCall>
        let pre = parse_tool_calls_auto("<|python_tag|>\{\"name\":\"message_timeline\",\"parameters\":\{\"query\":\"x\"}}",
                                        "", "", "l", calls)
        t |> equal(pre, "")
        t |> equal(length(calls), 1)
        if (length(calls) == 1) {
            t |> equal(calls[0]._function.name, "message_timeline")
            t |> equal(calls[0]._function.arguments, "\{\"query\": \"x\"}")
        }
        delete calls
    }
    t |> run("strict bare OpenAI call object is accepted") @(t : T?) {
        var calls : array<OaiToolCall>
        let pre = parse_tool_calls_auto("\{\"function\":\{\"name\":\"get_message_context\",\"arguments\":\{\"message_id\":7}}}",
                                        "", "", "b", calls)
        t |> equal(pre, "")
        t |> equal(length(calls), 1)
        delete calls
    }
    t |> run("ordinary JSON remains assistant content") @(t : T?) {
        var calls : array<OaiToolCall>
        let full = "\{\"answer\":42}"
        t |> equal(parse_tool_calls_auto(full, "", "", "n", calls), full)
        t |> equal(length(calls), 0)
        delete calls
    }
}

// ===== multi-model: two slots, "model" routing, per-slot caches surviving interleaved traffic =====

let MODEL2_PATH = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")
let TEST_PORT_MM = 18121

// Two-slot variant of with_llama_server: tinyllama as the default slot "tiny", SmolLM2-135M as
// "smol" (backend pinned cpu — asserts the requested-vs-effective surface). Same thread/context
// discipline: both models load INSIDE the server thread.
def private with_llama_server2(path_a, path_b : string; port : int; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            set_metal_mode(MetalMode.off)
            allow_cpu_prefill()   // multi-slot routing test: both slots serve on the CPU arm
            var ma <- load_model(path_a, QuantMode.q8)
            add_model(ma, "tiny")
            var mb <- load_model(path_b, QuantMode.q8)
            add_model(mb, "smol", SlotOpts(backend = "cpu"))
            set_default_model("tiny")
            set_default_max_tokens(4l)
            var surf <- {
                "config" => JV((port = port, model = path_a, streams = 4)),
                "sources" => JV((port = "cli")),
                "authoritative" => JV(false),
                "config_path" => JV(""),
                "save_path" => JV(TEST_SAVE_PATH)
            }
            var sdoc = JV(surf)
            set_config_surface(write_json_compact(sdoc), TEST_SAVE_PATH, path_a)
            unsafe {
                delete sdoc
            }
            run_server(port)   // blocks until POST /shutdown
            finished |> notify_and_release
        }
        let base_url = "http://127.0.0.1:{port}"
        var ready = false
        for (_attempt in range(600)) {   // up to ~60s: two loads + JIT
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
        assert(ready, "dasllama two-slot server became ready")
    }
}

[test]
def test_openai_server_multimodel(t : T?) {   // nolint:STYLE038 — one live two-slot server session per test: the routing assertions share that one boot
    t |> run("multi-model: routing + per-slot caches (needs tinyllama + SmolLM2-135M q8)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA server test is JIT-only)")
            return
        }
        if (!stat(MODEL_PATH).is_valid || !stat(MODEL2_PATH).is_valid) {
            t |> success(true, "skipped: tinyllama/SmolLM2-135M not present (set DASLLAMA_MODELS_DIR)")
            return
        }
        with_llama_server2(MODEL_PATH, MODEL2_PATH, TEST_PORT_MM) $(base_url) {
            // /v1/models lists both slots
            GET("{base_url}/v1/models") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                let body = string(resp.body)
                t |> success(find(body, "\"tiny\"") >= 0, "default slot listed")
                t |> success(find(body, "\"smol\"") >= 0, "second slot listed")
            }
            // routed by "model": the response echoes the slot id
            POST("{base_url}/v1/chat/completions",
                 "\{\"model\":\"smol\",\"messages\":[\{\"role\":\"user\",\"content\":\"Say hi.\"}],\"max_tokens\":4}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "routed chat JSON parses")
                if (js != null) {
                    t |> equal(js?["model"] ?? "", "smol")
                    unsafe {
                        delete js
                    }
                }
            }
            // no "model" field -> the default slot serves and is echoed
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":\"Say hi.\"}],\"max_tokens\":4}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                if (js != null) {
                    t |> equal(js?["model"] ?? "", "tiny")
                    unsafe {
                        delete js
                    }
                }
            }
            // unknown model -> OpenAI-shaped 404
            POST("{base_url}/v1/chat/completions",
                 "\{\"model\":\"nope\",\"messages\":[\{\"role\":\"user\",\"content\":\"hi\"}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.NOT_FOUND))
                t |> success(find(string(resp.body), "model_not_found") >= 0, "404 carries model_not_found")
            }
            // POST /v1/models/activate: admin warm-switch — 200 flips the active slot; 404 on an
            // unknown id. The GPU rebind inside is a no-op on this rig (no tier armed).
            POST("{base_url}/v1/models/activate", "\{\"model\":\"smol\"}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                t |> success(find(string(resp.body), "\"model\":\"smol\"") >= 0, "activate echoes the slot")
            }
            GET("{base_url}/v1/stats") $(resp) {
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "post-activate stats JSON parses")
                if (js != null) {
                    t |> equal(js?["active_model"] ?? "", "smol")
                    unsafe {
                        delete js
                    }
                }
            }
            POST("{base_url}/v1/models/activate", "\{\"model\":\"nope\"}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.NOT_FOUND))
                t |> success(find(string(resp.body), "model_not_found") >= 0, "activate 404 carries model_not_found")
            }
            // THE directive test: slot caches survive interleaved traffic on another slot.
            // Same >1-page prompt on tiny twice, a smol request between them — the re-send must
            // hit tiny's prefix cache even though smol served in between.
            var long2 = ""
            for (_i in range(12)) {
                long2 += "Pack my box with five dozen liquor jugs. "
            }
            let body_a = "\{\"model\":\"tiny\",\"prompt\":\"{long2}\",\"max_tokens\":4}"
            POST("{base_url}/v1/completions", body_a) $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
            }
            POST("{base_url}/v1/completions", "\{\"model\":\"smol\",\"prompt\":\"Hello\",\"max_tokens\":2}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
            }
            POST("{base_url}/v1/completions", body_a) $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
            }
            // per-slot stats: roster present, the activation moved the default to smol,
            // smol's requested backend recorded, tiny's cache hit counted, smol's cache untouched
            GET("{base_url}/v1/stats") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "stats JSON parses")
                if (js != null) {
                    let mv = js?["models"]
                    t |> success(mv != null && mv.value is _array, "stats carries a models roster")
                    if (mv != null && mv.value is _array) {
                        t |> equal(length(mv.value as _array), 2)
                        for (e in mv.value as _array) {
                            let nm = e?["name"] ?? ""
                            if (nm == "tiny") {
                                t |> success(!(e?["is_default"] ?? true), "the activate moved the default off tiny")
                                t |> success((e?["cached_tokens"] ?? 0l) > 0l, "tiny's prefix cache hit across the smol interleave")
                            } elif (nm == "smol") {
                                t |> success(e?["is_default"] ?? false, "the activate made smol the default")
                                t |> equal(e?["backend"] ?? "", "cpu")
                                t |> equal(e?["backend_effective"] ?? "", "cpu")
                                t |> equal(e?["cached_tokens"] ?? -1l, 0l)
                                t |> equal(e?["holds_gpu"] ?? true, false)
                            }
                        }
                    }
                    unsafe {
                        delete js
                    }
                }
            }
            // /v1/streams entries carry the model tag
            GET("{base_url}/v1/streams") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                t |> success(find(string(resp.body), "\"model\":\"tiny\"") >= 0, "stream cards tag their slot")
            }
            // POST /config with a [[models]] roster — validated, TOML round-trips as array-of-tables
            POST("{base_url}/config",
                 "\{\"port\":8123,\"models\":[\{\"name\":\"a\",\"path\":\"a.gguf\",\"backend\":\"cpu\",\"default\":true},\{\"name\":\"b\",\"path\":\"b.gguf\"}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
            }
            {
                let saved = fread(TEST_SAVE_PATH)
                t |> success(!empty(saved), "saved roster config readable")
                var terr : string
                var troot = read_toml(saved, terr)
                t |> success(troot != null, "saved roster config re-parses as TOML")
                if (troot != null) {
                    let mv = troot?["models"]
                    t |> success(mv != null && mv.value is _array, "[[models]] survives the TOML round-trip")
                    if (mv != null && mv.value is _array) {
                        t |> equal(length(mv.value as _array), 2)
                        t |> equal((mv.value as _array)[0]?["name"] ?? "", "a")
                        t |> equal((mv.value as _array)[0]?["backend"] ?? "", "cpu")
                    }
                    unsafe {
                        delete troot
                    }
                }
                remove(TEST_SAVE_PATH)
            }
            // roster validation: duplicate names, bad backend, missing path — honest 400s
            POST("{base_url}/config",
                 "\{\"models\":[\{\"name\":\"x\",\"path\":\"p.gguf\"},\{\"name\":\"x\",\"path\":\"q.gguf\"}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
            }
            POST("{base_url}/config",
                 "\{\"models\":[\{\"name\":\"x\",\"path\":\"p.gguf\",\"backend\":\"turbo\"}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
            }
            POST("{base_url}/config", "\{\"models\":[\{\"name\":\"x\"}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
            }
        }
    }
}
