options gen2
options persistent_heap   // each server thread loads/frees a multi-GB model once; explicit deletes below
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // ggml-parity FP laxity — this is an inference tool test, not the bit-exact suite

require dastest/testing_boost public
require openai_server                                   // the server under test (sibling — bare require, hyphenated dir)
require dasllama/dasllama                               // load_model / QuantMode
require daslib/jobque_boost                             // with_job_status (server-thread lifecycle)
require daslib/json_boost                               // read_json / from_JV
require daslib/strings_boost                            // split / trim_prefix / to_lower
require daslib/fio                                      // get_env_variable / stat / path_join / sleep
require strings                                         // find / starts_with
require math                                            // min
require dasllama/dasllama_env

// Live thinking + tool-use conformance, one small model per thinking family: the model actually
// THINKS (or CALLS) and the server demonstrably recognizes it — reasoning_content separated from
// content, finish_reason="tool_calls" with the parsed call. Greedy sampling and single-obvious-tool
// prompts keep the sub-1B models reliable. Model-gated + JIT-only like the sibling tests; the 11 GB
// gpt-oss-20b additionally rides the >6 GiB tier gate (DASLLAMA_PARITY_FULL=1). GLM-4 (the other
// symmetric-<think> family) has no small model — its live leg runs on the zen2 box (see THINKING.md);
// the symmetric matcher itself is pinned model-free in modules/dasLLAMA/tests/test_think_split.das.

let TEST_PORT = 18120
let TEST_CTX = 2048l   // room for a thinking span + the answer; keeps the KV cache small

def private models_dir() : string {
    return g_env_harness.models_dir   // empty -> paths are relative -> the tests skip
}
let QWEN3_PATH = path_join(models_dir(), "Qwen3-0.6B-Q8_0.gguf")
let GEMMA4_PATH = path_join(models_dir(), "gemma-4-E2B-it-Q4_K_M.gguf")
let GPTOSS_PATH = path_join(models_dir(), "gpt-oss-20b-mxfp4.gguf")

def private model_missing(t : T?; path : string) : bool {
    if (!jit_enabled()) {
        t |> success(true, "skipped: interpreted (dasLLAMA server test is JIT-only)")
        return true
    }
    if (!stat(path).is_valid) {
        t |> success(true, "skipped: {base_name(path)} not present (set DASLLAMA_MODELS_DIR)")
        return true
    }
    return false
}

// Start the server on a dedicated thread (model loaded in-thread — slots are per-context), wait
// until it answers /v1/models, run the block, POST /shutdown, join. `ready_s` scales the wait to
// the model's load time (the 11 GB gpt-oss needs minutes where the 0.6B needs seconds).
def private with_think_server(t : T?; model_path : string; port : int; ready_s : int; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            allow_cpu_prefill()   // deliberate CPU serving: deterministic across boxes, no blob twin needed
            var m <- load_model(model_path, QuantMode.q8)
            m.config.seq_len = min(m.config.seq_len, TEST_CTX)
            set_model(m, "test-llm-think")
            run_server(port)   // blocks (with_job_que + tick loop) until POST /shutdown
            finished |> notify_and_release
        }
        let base_url = "http://127.0.0.1:{port}"
        var ready = false
        for (_attempt in range(ready_s * 10)) {
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

// ===== response mirror shapes (from_JV defaults whatever a reply omits) =====

struct private TcFn {
    name : string
    arguments : string
}
struct private Tc {
    id : string
    @rename = "type" _type : string
    @rename = "function" _function : TcFn
}
struct private RespMsg {
    @optional content : string
    @optional reasoning_content : string
    @optional tool_calls : array<Tc>
}
struct private RespChoice {
    message : RespMsg
    finish_reason : string
}
struct private ChatResp {
    @rename = "object" _object : string
    choices : array<RespChoice>
}

struct private ChunkDelta {
    @optional role : string
    @optional content : string
    @optional reasoning_content : string
}
struct private ChunkChoice {
    delta : ChunkDelta
    @optional finish_reason : string
}
struct private Chunk {
    @rename = "object" _object : string
    choices : array<ChunkChoice>
}

// Every `data: <payload>` line of an SSE body, in order ("[DONE]" included). Sibling copy of
// test_openai_server_stream's — the server test files stay self-contained by dir convention.
def private sse_datas(body : string) : array<string> {
    var lines <- split(body, "\n")
    var out <- [for (ln in lines); trim_prefix(ln, "data: "); where starts_with(ln, "data: ")]
    delete lines
    return <- out
}

// POST a chat request and hand the parsed ChatResp to the block (fails the test on a non-200 or
// unparseable body). Greedy requests are deterministic per box, so content asserts are stable.
def private chat_resp(t : T?; base_url, body : string; blk : block<(r : ChatResp) : void>) {
    POST("{base_url}/v1/chat/completions", body) $(resp) {
        t |> equal(int(resp.status_code), int(http_status.OK))
        var perr = ""
        var js = read_json(string(resp.body), perr)
        t |> success(js != null, "chat JSON parses")
        if (js != null) {
            var r = from_JV(js, type<ChatResp>)
            t |> equal(r._object, "chat.completion")
            t |> equal(length(r.choices), 1)
            if (length(r.choices) == 1) {
                invoke(blk, r)
            }
            delete r
            unsafe {
                delete js
            }
        }
    }
}

// ===== Qwen3-0.6B: symmetric <think> + Hermes tools (the hybrid-thinking flagship) =====

[test]
def test_think_qwen3(t : T?) {   // nolint:STYLE038 — one live server session: every thinking/tool assertion runs against that one boot
    if (model_missing(t, QWEN3_PATH)) {
        return
    }
    with_think_server(t, QWEN3_PATH, TEST_PORT, 60) $(base_url) {
        t |> run("default thinking: reasoning_content separated, content clean of <think>") @(t : T?) {
            chat_resp(t, base_url, "\{\"messages\":[\{\"role\":\"user\",\"content\":\"What is 2+2? Answer with just the number.\"}],\"max_tokens\":512,\"temperature\":0}") $(r) {
                let msg & = unsafe(r.choices[0].message)
                t |> success(!empty(msg.reasoning_content), "the model thought and the server separated it")
                t |> success(find(msg.content, "<think>") < 0 && find(msg.content, "</think>") < 0,
                             "content carries no think markers")
                t |> success(find(msg.reasoning_content, "<think>") < 0, "reasoning carries no markers either")
                t |> success(!empty(msg.content), "an answer followed the thinking")
            }
        }
        t |> run("enable_thinking=false: instruct mode, no reasoning_content") @(t : T?) {
            chat_resp(t, base_url, "\{\"messages\":[\{\"role\":\"user\",\"content\":\"What is 2+2? Answer with just the number.\"}],\"max_tokens\":64,\"temperature\":0,\"enable_thinking\":false}") $(r) {
                let msg & = unsafe(r.choices[0].message)
                t |> success(empty(msg.reasoning_content), "suppressed run has no reasoning_content")
                t |> success(find(msg.content, "<think>") < 0, "content clean")
                t |> success(!empty(msg.content), "answer present")
            }
        }
        t |> run("streaming: reasoning deltas precede content deltas, both clean") @(t : T?) {
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":\"What is 2+2? Answer with just the number.\"}],\"max_tokens\":512,\"temperature\":0,\"stream\":true}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var datas <- sse_datas(string(resp.body))
                t |> success(length(datas) >= 3, "role + deltas + [DONE] at minimum")
                var first_reason = -1
                var first_content = -1
                var reasoning = ""
                var content = ""
                var perr = ""
                for (i in range(length(datas) - 1)) {
                    var jsi = read_json(datas[i], perr)
                    if (jsi != null) {
                        let c = from_JV(jsi, type<Chunk>)
                        if (length(c.choices) == 1) {
                            if (!empty(c.choices[0].delta.reasoning_content)) {
                                reasoning = "{reasoning}{c.choices[0].delta.reasoning_content}"
                                if (first_reason < 0) {
                                    first_reason = i
                                }
                            }
                            if (!empty(c.choices[0].delta.content)) {
                                content = "{content}{c.choices[0].delta.content}"
                                if (first_content < 0) {
                                    first_content = i
                                }
                            }
                        }
                        unsafe {
                            delete jsi
                        }
                    }
                }
                t |> success(first_reason >= 0, "reasoning deltas streamed")
                t |> success(first_content >= 0, "content deltas streamed")
                t |> success(first_reason < first_content, "reasoning streamed before content")
                t |> success(find(content, "<think>") < 0 && find(content, "</think>") < 0,
                             "streamed content clean of think markers")
                t |> equal(datas[length(datas) - 1], "[DONE]")
                delete datas
            }
        }
        t |> run("tools + thinking compose: reasoning_content AND finish_reason=tool_calls") @(t : T?) {
            chat_resp(t, base_url,
                "\{\"messages\":[\{\"role\":\"user\",\"content\":\"What is the weather in Paris right now? Use the get_weather tool.\"}]," +
                "\"tools\":[\{\"type\":\"function\",\"function\":\{\"name\":\"get_weather\",\"description\":\"Get current weather for a city\"," +
                "\"parameters\":\{\"type\":\"object\",\"properties\":\{\"city\":\{\"type\":\"string\"}},\"required\":[\"city\"]}}}]," +
                "\"max_tokens\":512,\"temperature\":0}") $(r) {
                t |> equal(r.choices[0].finish_reason, "tool_calls")
                let msg & = unsafe(r.choices[0].message)
                t |> equal(length(msg.tool_calls), 1)
                if (length(msg.tool_calls) == 1) {
                    t |> equal(msg.tool_calls[0]._function.name, "get_weather")
                    t |> success(!empty(msg.tool_calls[0]._function.arguments), "call carries arguments")
                    var perr = ""
                    var av = read_json(msg.tool_calls[0]._function.arguments, perr)
                    t |> success(av != null, "arguments are valid JSON")
                    if (av != null) {
                        t |> success(find(to_lower(av?["city"] ?? ""), "paris") >= 0, "the city argument names Paris")
                        unsafe {
                            delete av
                        }
                    }
                }
                t |> success(!empty(msg.reasoning_content), "the model thought before calling (hybrid compose)")
                t |> success(find(msg.content, "<tool_call>") < 0, "no raw tool envelope leaked into content")
            }
        }
    }
}

// ===== gemma-4-E2B: asymmetric channels, thinking OFF by default (opt-in via <|think|>) =====

[test]
def test_think_gemma4(t : T?) {
    if (model_missing(t, GEMMA4_PATH)) {
        return
    }
    with_think_server(t, GEMMA4_PATH, TEST_PORT + 1, 180) $(base_url) {
        t |> run("default (family off): no reasoning_content, answer clean") @(t : T?) {
            chat_resp(t, base_url, "\{\"messages\":[\{\"role\":\"user\",\"content\":\"What is 2+2? Answer with just the number.\"}],\"max_tokens\":64,\"temperature\":0}") $(r) {
                let msg & = unsafe(r.choices[0].message)
                t |> success(empty(msg.reasoning_content), "the E-series default is instruct mode")
                t |> success(!empty(msg.content), "answer present")
                t |> success(find(msg.content, "<channel|>") < 0 && find(msg.content, "<|channel>") < 0,
                             "no channel markers in content")
            }
        }
        t |> run("enable_thinking=true: the model emits its thought channel and the server splits it") @(t : T?) {
            chat_resp(t, base_url, "\{\"messages\":[\{\"role\":\"user\",\"content\":\"What is 17 times 23? Think it through, then answer.\"}],\"max_tokens\":512,\"temperature\":0,\"enable_thinking\":true}") $(r) {
                let msg & = unsafe(r.choices[0].message)
                t |> success(!empty(msg.reasoning_content), "thinking-ON produced a reasoning span")
                t |> success(find(msg.content, "<channel|>") < 0 && find(msg.content, "<|channel>") < 0,
                             "content clean of channel markers")
                t |> success(find(msg.reasoning_content, "<|channel>") < 0, "reasoning clean of the opener")
            }
        }
    }
}

// ===== gpt-oss-20b: Harmony channel-switch (analysis -> final), 11 GB — large-tier gated =====

[test]
def test_think_gptoss(t : T?) {
    if (get_env_variable("DASLLAMA_PARITY_FULL") != "1") {
        t |> success(true, "skipped: gpt-oss-20b is large-tier (11 GB) — set DASLLAMA_PARITY_FULL=1 to run")
        return
    }
    if (model_missing(t, GPTOSS_PATH)) {
        return
    }
    with_think_server(t, GPTOSS_PATH, TEST_PORT + 2, 600) $(base_url) {
        t |> run("Harmony analysis channel lands in reasoning_content, final in content") @(t : T?) {
            chat_resp(t, base_url, "\{\"messages\":[\{\"role\":\"user\",\"content\":\"What is 2+2? Answer with just the number.\"}],\"max_tokens\":256,\"temperature\":0}") $(r) {
                let msg & = unsafe(r.choices[0].message)
                t |> success(!empty(msg.reasoning_content), "the analysis channel was recognized as reasoning")
                t |> success(find(msg.reasoning_content, "<|channel|>") < 0 && find(msg.reasoning_content, "<|message|>") < 0,
                             "reasoning clean of Harmony markers")
                if (!empty(msg.content)) {
                    t |> success(find(msg.content, "<|channel|>") < 0 && find(msg.content, "<|message|>") < 0,
                                 "content clean of Harmony markers")
                }
            }
        }
    }
}
