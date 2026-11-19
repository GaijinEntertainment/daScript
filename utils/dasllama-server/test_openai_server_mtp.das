options gen2
options persistent_heap   // each server thread loads/frees a model once; explicit deletes below
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // ggml-parity FP laxity — this is an inference tool test, not the bit-exact suite
options _dasllama_internal = true

require dastest/testing_boost public
require openai_server                                   // the server under test (sibling — bare require, hyphenated dir)
require _server_rig                                     // models_dir / model_missing - the suites' one model root and gate
require dasllama/dasllama                               // load_model / QuantMode
require daslib/jobque_boost                             // with_job_status (server-thread lifecycle)
require daslib/json_boost                               // read_json
require daslib/fio                                      // path_join / sleep
require math                                            // min
require strings                                         // repeat
require dasllama/dasllama_env                           // g_env_gpu: the GPU home the environment asks for

// The self-speculation default: unset, a slot runs MTP when it serves ONE stream on Metal and
// plain decode on the CPU or when it serves several; an explicit setting wins; a model with no
// NextN head serves plain either way. Read off /v1/stats's mtp_drafted counter after one greedy
// chat per boot. Model-gated + JIT-only like the sibling tests; needs
// Qwen3.5-0.8B-MTP-Q8_0.gguf (the NextN head) and tinyllama-1.1b-chat-v1.0.Q8_0.gguf (no head). The block drafter
// cell needs gemma-4-E2B-it-Q8_0.gguf with its DFlash2 drafter and a GPU home the environment asks for.
//! the Metal cells skip on a box with no Metal backend; the block drafter cell skips without DASLLAMA_GPU=1

let TEST_PORT = 18150   // 18150-18160: the eleven cells' boots; 18097-18147 belong to the sibling suites (test_openai_server*.das, uncategorized_persistent_heap_stack_524288.das)
let TEST_CTX = 1024l

let MTP_PATH = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
let PLAIN_PATH = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")
let E2B_PATH = path_join(models_dir(), "gemma-4-E2B-it-Q8_0.gguf")
let E2B_DRAFTER = path_join(models_dir(), "drafters/gemma-4-E2B-it-dflash2-GGUF/gemma-4-E2B-it-dflash2-Q8_0.gguf")
let COUNT_TURN = "Count from one to ten."
let LONG_TURN_REPS = 520   //! the counting turn this many times: a prompt whose one prefill window holds more of the drafter's layer rows than an unreserved array resize takes
let LONG_TURN_ROWS = 2800l   //! past the 2731 rows of four 1536-wide layers an unreserved resize takes (64 MB)

// Start the server on a dedicated thread with `streams` scheduler slots and the mtp knob
// `mtp` (-1 unset, 0, 1), on Metal or on the CPU, wait until it answers /v1/models, run the
// block, shut it down.
def private with_mtp_server(t : T?; model_path : string; port : int; streams : int64; mtp : int; metal : bool; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            if (metal) {
                set_metal_mode(MetalMode.when_available)   // before the load: the slot maps the Metal flavor
            } else {
                allow_cpu_prefill()   // deliberate CPU serving: deterministic across boxes, no blob twin needed
            }
            var m <- load_model(model_path, QuantMode.q8)
            m.config.seq_len = min(m.config.seq_len, TEST_CTX)
            set_model(m, "test-llm-mtp")
            run_server(port, streams, [mtp = mtp])   // blocks (with_job_que + tick loop) until POST /shutdown
            finished |> notify_and_release
        }
        let ready = serve_clients(port, 600, "/v1/models", blk)
        finished |> join
        t |> success(ready, "dasllama server became ready")
    }
}

//! the slot's drafted and accepted token counters off `/v1/stats`, -1 for one that did not read
def private slot_drafts(t : T?; base_url : string) : tuple<drafted : int64; accepted : int64> {
    var drafted = -1l
    var accepted = -1l
    GET("{base_url}/v1/stats") $(resp) {
        t |> equal(int(resp.status_code), int(http_status.OK))
        var perr = ""
        var js = read_json(string(resp.body), perr)
        t |> success(js != null, "stats JSON parses: {perr}")
        if (js != null) {
            drafted = int64(js?["mtp_drafted"] ?? -1.0lf)
            accepted = int64(js?["mtp_accepted"] ?? -1.0lf)
            unsafe {
                delete js
            }
        }
    }
    return (drafted = drafted, accepted = accepted)
}

//! one greedy chat over `content`: its usage's prompt tokens and the slot's draft counters after it, each -1 where not read
def private chat_then_drafted(t : T?; base_url : string; content : string) : tuple<prompt : int64; drafted : int64; accepted : int64> {
    let usage = chat_usage(base_url, "\{\"messages\":[\{\"role\":\"user\",\"content\":\"{content}\"}],\"max_tokens\":24,\"temperature\":0}")
    t |> success(usage.prompt >= 0, "the chat answered with its usage")
    let drafts = slot_drafts(t, base_url)
    return (prompt = int64(usage.prompt), drafted = drafts.drafted, accepted = drafts.accepted)
}

def private drafted_after_chat(t : T?; base_url : string) : int64 => chat_then_drafted(t, base_url, COUNT_TURN).drafted

//! the draft counts in the `timings` of a streamed greedy chat's usage chunk, -1 for a key the chunk does not carry
def private streamed_draft_counts(t : T?; base_url : string) : tuple<drafted : int64; accepted : int64> {
    var drafted = -1l
    var accepted = -1l
    POST("{base_url}/v1/chat/completions",
         "\{\"messages\":[\{\"role\":\"user\",\"content\":\"{COUNT_TURN}\"}],\"max_tokens\":24,\"temperature\":0," +
         "\"stream\":true,\"stream_options\":\{\"include_usage\":true},\"cache_prompt\":false}") $(resp) {
        t |> equal(int(resp.status_code), int(http_status.OK))
        var inscope datas <- sse_datas(string(resp.body))
        t |> success(length(datas) >= 2, "a usage chunk ahead of [DONE] ({length(datas)} events)")
        if (length(datas) >= 2) {
            var perr = ""
            var js = read_json(datas[length(datas) - 2], perr)
            t |> success(js != null, "the usage chunk parses: {perr}")
            if (js != null) {
                drafted = js?["timings"]?["draft_n"] ?? -1l
                accepted = js?["timings"]?["draft_n_accepted"] ?? -1l
                unsafe {
                    delete js
                }
            }
        }
    }
    return (drafted = drafted, accepted = accepted)
}

[test(description = "on Metal the NextN-headed slot decodes plain when unset or at four streams and drafts only when explicitly on", timeout = 1200)]
def test_mtp_default_on_metal(t : T?) {
    if (model_missing(t, MTP_PATH)) {
        return
    }
    if (!kernel_backend_available("metal")) {
        t |> skip("no Metal backend on this box")
        return
    }
    t |> run("unset, one stream on Metal: the NextN-headed slot decodes plain, its file has no catalog row listing Metal") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT, 1l, -1, true) $(base_url) {
            t |> equal(0l, drafted_after_chat(t, base_url), "no draft under the default without a serve_mtp platform")
        }
    }
    t |> run("explicit on, one stream on Metal: the NextN-headed slot drafts") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT + 8, 1l, 1, true) $(base_url) {
            t |> success(drafted_after_chat(t, base_url) > 0l, "mtp_drafted rose after one chat")
        }
    }
    t |> run("unset, four streams on Metal: the same slot decodes plain") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT + 1, 4l, -1, true) $(base_url) {
            t |> equal(0l, drafted_after_chat(t, base_url), "no draft at several streams")
        }
    }
    t |> run("explicit off, one stream on Metal: the setting wins over the default") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT + 2, 1l, 0, true) $(base_url) {
            t |> equal(0l, drafted_after_chat(t, base_url), "mtp = false keeps plain decode")
        }
    }
}

[test(description = "on the CPU the unset default decodes plain, an explicit setting wins and a drafting turn's timings carry its draft counts, and a constrained reply decodes plain beside drafting streams", timeout = 1200)]
def test_mtp_default_on_cpu(t : T?) {
    if (model_missing(t, MTP_PATH)) {
        return
    }
    t |> run("unset, one stream on the CPU: the NextN-headed slot decodes plain") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT + 3, 1l, -1, false) $(base_url) {
            t |> equal(0l, drafted_after_chat(t, base_url), "no draft on the CPU")
            t |> equal(streamed_draft_counts(t, base_url).drafted, -1l, "a plain turn's timings carry no draft count")
        }
    }
    t |> run("explicit on, one stream on the CPU: the setting wins over the default") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT + 4, 1l, 1, false) $(base_url) {
            t |> success(drafted_after_chat(t, base_url) > 0l, "mtp = true drafts on the CPU")
            let before = slot_drafts(t, base_url)
            let turn = streamed_draft_counts(t, base_url)
            t |> success(turn.drafted > 0l && turn.accepted > 0l && turn.accepted <= turn.drafted,
                "a drafting turn's timings carry its own counts ({turn.accepted} accepted of {turn.drafted})")
            let after = slot_drafts(t, base_url)
            t |> equal(after.drafted - before.drafted, turn.drafted, "... the drafts the slot's counter rose by over that turn")
            t |> equal(after.accepted - before.accepted, turn.accepted, "... and the accepted ones its accepted counter rose by")
        }
    }
    t |> run("explicit on, four streams on the CPU: the setting wins over the default") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT + 5, 4l, 1, false) $(base_url) {
            t |> success(drafted_after_chat(t, base_url) > 0l, "mtp = true drafts at four streams")
        }
    }
    t |> run("explicit on: a constrained reply decodes plain beside the drafting streams") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT + 7, 2l, 1, false) $(base_url) {
            let before = drafted_after_chat(t, base_url)
            t |> success(before > 0l, "an unconstrained chat drafted")
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":\"How many legs does a cat have? Answer in JSON.\"}],\"max_tokens\":24,\"temperature\":0," +
                 "\"response_format\":\{\"type\":\"json_schema\",\"json_schema\":\{\"name\":\"legs\",\"strict\":true,\"schema\":" +
                 "\{\"type\":\"object\",\"properties\":\{\"legs\":\{\"type\":\"integer\",\"minimum\":0,\"maximum\":9}},\"required\":[\"legs\"],\"additionalProperties\":false}}}}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var perr = ""
                var js = read_json(string(resp.body), perr)
                t |> success(js != null, "the constrained chat answers")
                if (js != null) {
                    let content = js?["choices"]?[0]?["message"]?["content"] ?? ""
                    var cerr = ""
                    var cj = read_json(content, cerr)
                    t |> success(cj != null && cj.value is _object && cj?["legs"] != null && !(cj?["legs"].value is _null), "the reply fits the schema: {content}")
                    unsafe {
                        delete cj
                        delete js
                    }
                }
            }
            var after = -1l
            GET("{base_url}/v1/stats") $(resp) {
                var perr = ""
                var js = read_json(string(resp.body), perr)
                if (js != null) {
                    after = int64(js?["mtp_drafted"] ?? -1.0lf)
                    unsafe {
                        delete js
                    }
                }
            }
            t |> equal(before, after, "the constrained stream drafted nothing")
        }
    }
}

[test(description = "a model with no NextN head ignores the speculation default and the chat serves", timeout = 1200)]
def test_mtp_default_no_head(t : T?) {
    if (model_missing(t, PLAIN_PATH)) {
        return
    }
    let metal = kernel_backend_available("metal")   // the backend the default would arm on
    t |> run("unset, one stream, no NextN head: the default is ignored and the chat serves") @(t : T?) {
        with_mtp_server(t, PLAIN_PATH, TEST_PORT + 6, 1l, -1, metal) $(base_url) {
            t |> equal(0l, drafted_after_chat(t, base_url), "a head-less model drafts nothing and answers")
        }
    }
}

def private e2b_gpu_slot_flags(home, drafter : string) : array<string> {
    var flags <- ["--model", E2B_PATH, "--gpu", home, "--ctx", "4096"]
    if (!empty(drafter)) {
        flags |> push_from(["--drafter", drafter])
    }
    return <- flags
}

[test(description = "on the GPU home the environment asks for a slot with the DFlash2 block drafter boots off the card alone, a turn over a prompt past 2800 tokens is served, and the slot's draft counter rises; the same boot with no drafter drafts nothing", timeout = 1200)]
def test_block_drafter_on_gpu(t : T?) {
    if (model_missing(t, [E2B_PATH, E2B_DRAFTER])) {
        return
    }
    if (!(g_env_gpu.gpu ?? false)) {
        t |> skip("no GPU home is asked for - needs DASLLAMA_GPU=1 (DASLLAMA_CUDA=1 beside it for the CUDA home)")
        return
    }
    let home = g_env_gpu.cuda ? "cuda" : "vulkan"
    let long_turn = repeat("{COUNT_TURN} ", LONG_TURN_REPS)
    t |> run("{home}: the control - the slot with no drafter serves the long turn and reports no draft") @(t : T?) {
        var flags <- e2b_gpu_slot_flags(home, "")
        with_spawned_server(t, "the plain {home} boot", TEST_PORT + 9, flags, "streams = 1\n") $(base_url) {
            let turn = chat_then_drafted(t, base_url, long_turn)
            t |> success(turn.prompt > LONG_TURN_ROWS, "the prompt is one window past the unreserved rows ({turn.prompt} tokens)")
            t |> equal(turn.drafted, 0l, "the slot with no drafter drafted nothing")
        }
        delete flags
    }
    t |> run("{home}: the slot with the block drafter boots, serves the long turn and reports its drafts") @(t : T?) {
        var drafting <- e2b_gpu_slot_flags(home, E2B_DRAFTER)
        with_spawned_server(t, "the drafting {home} boot", TEST_PORT + 10, drafting, "streams = 1\n") $(base_url) {
            let turn = chat_then_drafted(t, base_url, long_turn)
            t |> success(turn.prompt > LONG_TURN_ROWS, "the prompt is one window past the unreserved rows ({turn.prompt} tokens)")
            t |> success(turn.drafted > 0l, "the slot drafted over the long turn ({turn.drafted} tokens)")
            t |> success(turn.accepted > 0l, "and some drafts were accepted ({turn.accepted} of {turn.drafted}): a drafter reading the wrong keys of a long context gets none")
            t |> success(drafted_after_chat(t, base_url) > turn.drafted, "a short turn after it drafts too: the server lived through the long window")
        }
        delete drafting
    }
}
