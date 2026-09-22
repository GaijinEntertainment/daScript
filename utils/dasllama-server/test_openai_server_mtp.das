options gen2
options persistent_heap   // each server thread loads/frees a model once; explicit deletes below
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // ggml-parity FP laxity — this is an inference tool test, not the bit-exact suite
options _dasllama_internal = true

require dastest/testing_boost public
require openai_server                                   // the server under test (sibling — bare require, hyphenated dir)
require main                                            // mtp_setting / mtp_default_resolved - the config surface's tri-state seam
require _server_rig                                     // models_dir / model_missing - the suites' one model root and gate
require dasllama/dasllama                               // load_model / QuantMode
require daslib/jobque_boost                             // with_job_status (server-thread lifecycle)
require daslib/json_boost                               // read_json
require daslib/fio                                      // path_join / sleep
require math                                            // min

// The self-speculation default: unset, a slot runs MTP when it serves ONE stream and plain
// decode when it serves several; an explicit setting wins; a model with no NextN head serves
// plain either way. Read off /v1/stats's mtp_drafted counter after one greedy chat per boot.
// Model-gated + JIT-only like the sibling tests; needs Qwen3.5-0.8B-MTP-Q8_0.gguf (the NextN
// head) and tinyllama-1.1b-chat-v1.0.Q8_0.gguf (no head).

let TEST_PORT = 18150   // 18150-18154: the five cells' boots; 18097-18147 belong to the sibling suites (test_openai_server*.das, test_setup_mode.das)
let TEST_CTX = 1024l

let MTP_PATH = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
let PLAIN_PATH = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")

// Start the server on a dedicated thread with `streams` scheduler slots and the mtp knob
// `mtp` (-1 unset, 0, 1), wait until it answers /v1/models, run the block, shut it down.
def private with_mtp_server(t : T?; model_path : string; port : int; streams : int64; mtp : int; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            allow_cpu_prefill()   // deliberate CPU serving: deterministic across boxes, no blob twin needed
            var m <- load_model(model_path, QuantMode.q8)
            m.config.seq_len = min(m.config.seq_len, TEST_CTX)
            set_model(m, "test-llm-mtp")
            run_server(port, streams, [mtp = mtp])   // blocks (with_job_que + tick loop) until POST /shutdown
            finished |> notify_and_release
        }
        let base_url = "http://127.0.0.1:{port}"
        var ready = false
        for (_attempt in range(600)) {
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

def private drafted_after_chat(t : T?; base_url : string) : int64 {
    POST("{base_url}/v1/chat/completions", "\{\"messages\":[\{\"role\":\"user\",\"content\":\"Count from one to ten.\"}],\"max_tokens\":24,\"temperature\":0}") $(resp) {
        t |> equal(int(resp.status_code), int(http_status.OK))
    }
    var drafted = -1l
    GET("{base_url}/v1/stats") $(resp) {
        t |> equal(int(resp.status_code), int(http_status.OK))
        var perr = ""
        var js = read_json(string(resp.body), perr)
        t |> success(js != null, "stats JSON parses: {perr}")
        if (js != null) {
            drafted = int64(js?["mtp_drafted"] ?? -1.0lf)
            unsafe {
                delete js
            }
        }
    }
    return drafted
}

[test]
def test_mtp_default_single_stream(t : T?) {
    if (model_missing(t, MTP_PATH)) {
        return
    }
    t |> run("unset, one stream: the NextN-headed slot drafts") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT, 1l, -1) $(base_url) {
            t |> success(drafted_after_chat(t, base_url) > 0l, "mtp_drafted rose after one chat")
        }
    }
    t |> run("unset, four streams: the same slot decodes plain") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT + 1, 4l, -1) $(base_url) {
            t |> equal(0l, drafted_after_chat(t, base_url), "no draft at several streams")
        }
    }
    t |> run("explicit off, one stream: the setting wins over the default") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT + 2, 1l, 0) $(base_url) {
            t |> equal(0l, drafted_after_chat(t, base_url), "mtp = false keeps plain decode")
        }
    }
    t |> run("explicit on, four streams: the setting wins over the default") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT + 3, 4l, 1) $(base_url) {
            t |> success(drafted_after_chat(t, base_url) > 0l, "mtp = true drafts at four streams")
        }
    }
}

[test]
def test_mtp_default_no_head(t : T?) {
    if (model_missing(t, PLAIN_PATH)) {
        return
    }
    t |> run("unset, one stream, no NextN head: the default is ignored and the chat serves") @(t : T?) {
        with_mtp_server(t, PLAIN_PATH, TEST_PORT + 4, 1l, -1) $(base_url) {
            t |> equal(0l, drafted_after_chat(t, base_url), "a head-less model drafts nothing and answers")
        }
    }
}

// model-free: the config surface's tri-state seam and the resolved value GET /config reports
[test]
def test_mtp_setting_tri_state(t : T?) {
    t |> run("a key the TOML or --mtp set pins 0 / 1; unset is -1 and each slot decides") @(t : T?) {
        t |> equal(mtp_setting(true, true), 1, "mtp = true")
        t |> equal(mtp_setting(true, false), 0, "mtp = false")
        t |> equal(mtp_setting(false, false), -1, "unset: the slot decides")
        t |> equal(mtp_setting(false, true), -1, "an unsourced true is still unset")
    }
    t |> run("the unset default resolves on for one host-cached stream, off at several streams or on a Vulkan-resident server") @(t : T?) {
        t |> success(mtp_default_resolved(1, ""), "one stream, no device")
        t |> success(mtp_default_resolved(1, "metal"), "one stream on Metal - the rows stay host-cached there")
        t |> success(!mtp_default_resolved(4, ""), "four streams")
        t |> success(!mtp_default_resolved(1, "vulkan"), "a Vulkan-resident slot keeps its device home")
    }
}
