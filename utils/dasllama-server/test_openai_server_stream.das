options gen2
options persistent_heap   // the server thread loads/frees a model once; explicit deletes below
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // ggml-parity FP laxity — this is an inference tool test, not the bit-exact suite

require dastest/testing_boost public
require openai_server                                   // the scheduler-wired server (sibling — bare require, hyphenated dir)
require dasllama/dasllama                               // load_model / QuantMode
require daslib/jobque_boost                             // with_job_status / with_channel (threads + results)
require daslib/json_boost                               // read_json / from_JV
require daslib/strings_boost                            // split / trim_prefix
require daslib/fio                                      // get_env_variable / stat / path_join / sleep
require strings                                         // starts_with / build_string
require math                                            // min
require dasllama/dasllama_env

// S3/S4 conformance for the continuous-batching server wiring: SSE chunk framing on a streaming
// chat, the mandatory 400 on a prompt that cannot fit the context (the scheduler would panic), two
// client THREADS batching on one server (proven by /v1/stats peak_active >= 2), and disconnect
// eviction (an abandoned client's stream finishes "evicted", proven via stats), and graceful
// shutdown draining accepted work while rejecting new work. Same skip contract
// as test_openai_server; SmolLM2-135M — small, chatml template (real stop ids). In-dir like its
// siblings: a require path can't contain the '-' of "dasllama-server".

let TEST_PORT = 18101
let TEST_CTX = 1024l   // big enough that the eviction arm's 900-token budget outlasts a 1s client timeout

def private models_dir() : string {
    let env = g_env_harness.models_dir
    return empty(env) ? "/Users/borisbatkin/Work/llama.cpp/models/" : env
}
let MODEL_PATH = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")

def private model_missing(t : T?) : bool {
    if (!jit_enabled()) {
        t |> success(true, "skipped: interpreted (dasLLAMA server test is JIT-only)")
        return true
    }
    if (!stat(MODEL_PATH).is_valid) {
        t |> success(true, "skipped: {MODEL_PATH} not present")
        return true
    }
    return false
}

// Start the server on a dedicated thread (model loaded in-thread — g_model is per-context), wait
// until it answers /v1/models, run the block, POST /shutdown, join. Caps the context to TEST_CTX.
def private with_stream_server(t : T?; model_path : string; port : int; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            var m <- load_model(model_path, QuantMode.q8)
            m.config.seq_len = min(m.config.seq_len, TEST_CTX)
            set_model(m, "test-llm-stream")
            run_server(port)   // scheduler defaults: 4 streams, queue 32, chunk 64
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
        t |> success(ready, "dasllama server became ready")
    }
}

// Every `data: <payload>` line of an SSE body, in order ("[DONE]" included).
def private sse_datas(body : string) : array<string> {
    var lines <- split(body, "\n")
    var out <- [for (ln in lines); trim_prefix(ln, "data: "); where starts_with(ln, "data: ")]
    delete lines
    return <- out
}

// chat.completion.chunk mirror shapes (from_JV defaults the fields a chunk omits)
struct private ChunkDelta {
    @optional role : string
    @optional content : string
}
struct private ChunkChoice {
    delta : ChunkDelta
    @optional finish_reason : string
}
struct private Chunk {
    @rename = "object" _object : string
    choices : array<ChunkChoice>
}

// non-streaming chat.completion mirror shapes
struct private RespMsg {
    content : string
}
struct private RespChoice {
    message : RespMsg
    finish_reason : string
}
struct private RespUsage {
    prompt_tokens : int64
    completion_tokens : int64
    total_tokens : int64
}
struct private ChatResp {
    @rename = "object" _object : string
    choices : array<RespChoice>
    usage : RespUsage
}

// one concurrent client's verdict, shipped back over the channel (asserts live on the main thread)
struct ClientResult {
    status : int
    ok : int   // 1 = valid chat.completion with non-empty content
}

[test]
def test_openai_server_stream(t : T?) {   // nolint:STYLE038 — one live server session: every SSE-framing assertion runs against that one boot
    if (model_missing(t)) {
        return
    }
    with_stream_server(t, MODEL_PATH, TEST_PORT) $(base_url) {
        t |> run("streaming chat: SSE framing (role delta, content chunks, finish, [DONE])") @(t : T?) {
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":\"Say hi.\"}],\"max_tokens\":6,\"stream\":true}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var datas <- sse_datas(string(resp.body))
                t |> success(length(datas) >= 3, "role + terminating chunk + [DONE] at minimum (got {length(datas)} events)")
                if (length(datas) < 3) {
                    return
                }
                t |> equal(datas[length(datas) - 1], "[DONE]")
                var perr = ""
                var js0 = read_json(datas[0], perr)
                t |> success(js0 != null, "first chunk parses")
                if (js0 != null) {
                    let c = from_JV(js0, type<Chunk>)
                    t |> equal(c._object, "chat.completion.chunk")
                    t |> success(length(c.choices) == 1, "one choice per chunk")
                    if (length(c.choices) == 1) {
                        t |> equal(c.choices[0].delta.role, "assistant")
                    }
                    unsafe {
                        delete js0
                    }
                }
                var content_events = 0
                var finish = ""
                for (i in range(1, length(datas) - 1)) {
                    var jsi = read_json(datas[i], perr)
                    if (jsi != null) {
                        let c = from_JV(jsi, type<Chunk>)
                        if (length(c.choices) == 1) {
                            if (!empty(c.choices[0].delta.content)) {
                                content_events ++
                            }
                            if (!empty(c.choices[0].finish_reason)) {
                                finish = c.choices[0].finish_reason
                            }
                        }
                        unsafe {
                            delete jsi
                        }
                    }
                }
                t |> success(content_events >= 1, "at least one content delta ({content_events})")
                t |> success(finish == "stop" || finish == "length", "terminating chunk carries finish_reason (got '{finish}')")
                delete datas
            }
        }
        t |> run("over-long prompt answers 400 before the scheduler (mandatory pre-check)") @(t : T?) {
            let big = build_string() $(w) {
                for (_i in range(1400)) {   // ~1400 tokens — far beyond the TEST_CTX-token context
                    w |> write("word ")
                }
            }
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":\"{big}\"}],\"max_tokens\":4}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
                var perr = ""
                var js = read_json(string(resp.body), perr)
                t |> success(js != null, "error body parses")
                if (js != null) {
                    t |> equal(js?["error"]?["type"] ?? "", "invalid_request_error")
                    unsafe {
                        delete js
                    }
                }
            }
        }
        t |> run("auto truncation drops oldest turns and reserves output space") @(t : T?) {
            let old_turn = build_string() $(w) {
                for (_i in range(1400)) {
                    w |> write("obsolete ")
                }
            }
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":\"{old_turn}\"}," +
                 "\{\"role\":\"assistant\",\"content\":\"Understood.\"}," +
                 "\{\"role\":\"user\",\"content\":\"Answer with OK.\"}]," +
                 "\"max_tokens\":4,\"truncation\":\"auto\"}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var perr = ""
                var js = read_json(string(resp.body), perr)
                t |> success(js != null, "response body parses")
                if (js != null) {
                    let cr = from_JV(js, type<ChatResp>)
                    t |> equal(cr._object, "chat.completion")
                    t |> equal(length(cr.choices), 1)
                    t |> success(cr.usage.prompt_tokens < 128l,
                                 "old turn was removed from the rendered prompt ({cr.usage.prompt_tokens} tokens remain)")
                    unsafe {
                        delete js
                    }
                }
            }
        }
        t |> run("two concurrent clients batch on one server (peak_active >= 2 via /v1/stats)") @(t : T?) {
            with_channel(2) $(ch) {
                for (ti in range(2)) {
                    let prompt_txt = ti == 0 ? "Once upon a time" : "The little robot"
                    new_thread() @() {
                        var r = ClientResult()
                        POST("{base_url}/v1/chat/completions",
                             "\{\"messages\":[\{\"role\":\"user\",\"content\":\"{prompt_txt}\"}],\"max_tokens\":48}") $(resp) {
                            if (resp != null) {
                                r.status = int(resp.status_code)
                                var perr = ""
                                var js = read_json(string(resp.body), perr)
                                if (js != null) {
                                    let cr = from_JV(js, type<ChatResp>)
                                    if (cr._object == "chat.completion" && length(cr.choices) == 1) {
                                        r.ok = !empty(cr.choices[0].message.content) ? 1 : 0
                                    }
                                    unsafe {
                                        delete js
                                    }
                                }
                            }
                        }
                        ch |> push_clone(r)
                        ch |> notify_and_release
                    }
                }
                var got = 0
                var all_ok = true
                ch |> for_each_clone() $(r : ClientResult#) {
                    got ++
                    all_ok &&= r.status == 200 && r.ok == 1
                }
                t |> equal(got, 2)
                t |> success(all_ok, "both concurrent chat requests returned a valid completion")
            }
            GET("{base_url}/v1/stats") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var perr = ""
                var js = read_json(string(resp.body), perr)
                t |> success(js != null, "stats parses")
                if (js != null) {
                    let peak = int64(js?["peak_active"] ?? 0l)
                    let steps = int64(js?["decode_steps"] ?? 0l)
                    let gen = int64(js?["gen_tokens"] ?? 0l)
                    let fin = int64(js?["finished"] ?? 0l)
                    t |> success(peak >= 2l, "peak_active >= 2 — the streams were concurrent (got {peak})")
                    t |> success(steps > 0l, "decode steps counted ({steps})")
                    t |> success(gen > 0l, "generated tokens counted ({gen})")
                    t |> success(fin >= 3l, "finished streams counted across the run ({fin})")
                    unsafe {
                        delete js
                    }
                }
            }
        }
        t |> run("disconnected client is evicted mid-generation (observed via /v1/stats)") @(t : T?) {
            // Non-streaming: the client receives ZERO bytes until finish, so its 1s idle-read
            // timeout fires deterministically while the server is still generating the 900-token
            // budget (a 135M model is nowhere near 900 tok/s); the serve loop's
            // is_writer_connected poll then evicts the stream and reaps its session.
            with_http_request() <| $(var req) {
                req.method = http_method.POST
                req.url := "{base_url}/v1/chat/completions"
                req.body := "\{\"messages\":[\{\"role\":\"user\",\"content\":\"Tell me a very long story.\"}],\"max_tokens\":900}"
                set_content_type(req, "application/json")
                set_timeout(req, 1)
                request(req) $(_resp) {}   // times out — the abort IS the test stimulus
            }
            var evicted = 0l
            for (_attempt in range(300)) {   // up to ~30s; generation stops at eviction, so usually instant
                GET("{base_url}/v1/stats") $(resp) {
                    if (resp != null && resp.status_code == http_status.OK) {
                        var perr = ""
                        var js = read_json(string(resp.body), perr)
                        if (js != null) {
                            evicted = int64(js?["evicted"] ?? 0l)
                            unsafe {
                                delete js
                            }
                        }
                    }
                }
                if (evicted >= 1l) {
                    break
                }
                sleep(100u)
            }
            t |> success(evicted >= 1l, "the abandoned stream was evicted (evicted = {evicted})")
            // the server survives the eviction and still serves
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":\"Hi\"}],\"max_tokens\":4}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
            }
        }
        t |> run("prefix cache: the same request twice returns the identical completion, warm run cached") @(t : T?) {
            // The rendered prompt must span at least one full page (server default P = 64 = the
            // prefill chunk, so warm chunk boundaries align with cold's and flash prefill repeats
            // the exact same evals — greedy output is bit-identical, not just similar).
            let user = ("Tell me about a little robot who lives in a small village near the mountains" +
                " and spends every evening counting the birds that cross the sky above the old bakery" +
                " while dreaming about the day it will finally learn to fly like them." +
                " The robot keeps a careful notebook of every sparrow, swallow and crow it has ever" +
                " counted, and the baker sometimes leaves a warm roll on the windowsill for it." +
                " In winter the birds are fewer, so the robot recounts the old pages and wonders" +
                " whether the mountains are lonely too. Please tell the story slowly and gently.")
            let body = "\{\"messages\":[\{\"role\":\"user\",\"content\":\"{user}\"}],\"max_tokens\":24}"
            var replies : array<string>
            for (_round in range(2)) {
                POST("{base_url}/v1/chat/completions", body) $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.OK))
                    var perr = ""
                    var js = read_json(string(resp.body), perr)
                    if (js != null) {
                        let cr = from_JV(js, type<ChatResp>)
                        if (length(cr.choices) == 1) {
                            replies |> push("{cr.choices[0].message.content}\t{cr.choices[0].finish_reason}")
                        }
                        unsafe {
                            delete js
                        }
                    }
                }
            }
            t |> equal(2, length(replies))
            if (length(replies) == 2) {
                t |> success(!empty(replies[0]), "cold reply non-empty")
                t |> equal(replies[0], replies[1])
            }
            GET("{base_url}/v1/stats") $(resp) {
                var perr = ""
                var js = read_json(string(resp.body), perr)
                t |> success(js != null, "stats parses")
                if (js != null) {
                    let cached = int64(js?["cached_tokens"] ?? 0l)
                    let pages = int64(js?["prefix_pages"] ?? 0l)
                    t |> success(cached >= 64l, "the warm run attached at least one full page ({cached} tokens)")
                    t |> success(pages > 0l, "the prefix cache retains pages ({pages})")
                    unsafe {
                        delete js
                    }
                }
            }
            delete replies
        }
        t |> run("graceful shutdown drains accepted work and rejects new work") @(t : T?) {
            with_channel(1) $(ch) {
                new_thread() @() {
                    var result = ClientResult()
                    POST("{base_url}/v1/completions",
                         "\{\"prompt\":\"Once upon a time\",\"max_tokens\":900}") $(resp) {
                        if (resp != null) {
                            result.status = int(resp.status_code)
                            result.ok = !empty(resp.body) ? 1 : 0
                        }
                    }
                    ch |> push_clone(result)
                    ch |> notify_and_release
                }
                var active = 0l
                for (_attempt in range(300)) {
                    GET("{base_url}/v1/stats") $(resp) {
                        if (resp != null && resp.status_code == http_status.OK) {
                            var perr = ""
                            var js = read_json(string(resp.body), perr)
                            if (js != null) {
                                active = int64(js?["active"] ?? 0l)
                                unsafe {
                                    delete js
                                }
                            }
                        }
                    }
                    if (active >= 1l) {
                        break
                    }
                    sleep(100u)
                }
                t |> success(active >= 1l, "a completion was active before shutdown")
                POST("{base_url}/shutdown", "") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.OK))
                }
                POST("{base_url}/v1/completions",
                     "\{\"prompt\":\"new work\",\"max_tokens\":4}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.SERVICE_UNAVAILABLE))
                }
                var completed = 0
                ch |> for_each_clone() $(result : ClientResult#) {
                    completed++
                    t |> equal(result.status, int(http_status.OK))
                    t |> equal(result.ok, 1)
                }
                t |> equal(completed, 1)
            }
        }
    }
}
