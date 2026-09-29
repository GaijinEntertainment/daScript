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

// The self-speculation default: unset, a slot runs MTP when it serves ONE stream on Metal and
// plain decode on the CPU or when it serves several; an explicit setting wins; a model with no
// NextN head serves plain either way. Read off /v1/stats's mtp_drafted counter after one greedy
// chat per boot. Model-gated + JIT-only like the sibling tests; needs
// Qwen3.5-0.8B-MTP-Q8_0.gguf (the NextN head) and tinyllama-1.1b-chat-v1.0.Q8_0.gguf (no head).
//! the Metal cells skip on a box with no Metal backend

let TEST_PORT = 18150   // 18150-18156: the seven cells' boots; 18097-18147 belong to the sibling suites (test_openai_server*.das, test_setup_mode.das)
let TEST_CTX = 1024l

let MTP_PATH = path_join(models_dir(), "Qwen3.5-0.8B-MTP-Q8_0.gguf")
let PLAIN_PATH = path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf")

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
def test_mtp_default_on_metal(t : T?) {
    if (model_missing(t, MTP_PATH)) {
        return
    }
    if (!kernel_backend_available("metal")) {
        t |> skip("no Metal backend on this box")
        return
    }
    t |> run("unset, one stream on Metal: the NextN-headed slot drafts") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT, 1l, -1, true) $(base_url) {
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

[test]
def test_mtp_default_on_cpu(t : T?) {
    if (model_missing(t, MTP_PATH)) {
        return
    }
    t |> run("unset, one stream on the CPU: the NextN-headed slot decodes plain") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT + 3, 1l, -1, false) $(base_url) {
            t |> equal(0l, drafted_after_chat(t, base_url), "no draft on the CPU")
        }
    }
    t |> run("explicit on, one stream on the CPU: the setting wins over the default") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT + 4, 1l, 1, false) $(base_url) {
            t |> success(drafted_after_chat(t, base_url) > 0l, "mtp = true drafts on the CPU")
        }
    }
    t |> run("explicit on, four streams on the CPU: the setting wins over the default") @(t : T?) {
        with_mtp_server(t, MTP_PATH, TEST_PORT + 5, 4l, 1, false) $(base_url) {
            t |> success(drafted_after_chat(t, base_url) > 0l, "mtp = true drafts at four streams")
        }
    }
}

[test]
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

// model-free: the config surface's tri-state seam and the resolved value GET /config reports
[test]
def test_mtp_setting_tri_state(t : T?) {
    t |> run("a key the TOML or --mtp set pins 0 / 1; unset is -1 and each slot decides") @(t : T?) {
        t |> equal(mtp_setting(true, true), 1, "mtp = true")
        t |> equal(mtp_setting(true, false), 0, "mtp = false")
        t |> equal(mtp_setting(false, false), -1, "unset: the slot decides")
        t |> equal(mtp_setting(false, true), -1, "an unsourced true is still unset")
    }
    t |> run("the unset default resolves on for one stream on a GPU, off on the CPU or at several streams") @(t : T?) {
        t |> success(mtp_default_resolved(1, "metal", ""), "one stream on Metal")
        t |> success(mtp_default_resolved(1, "metal-required", ""), "one stream on required Metal")
        t |> success(mtp_default_resolved(1, "vulkan", ""), "one stream on a Vulkan device")
        t |> success(mtp_default_resolved(1, "", "auto"), "one stream on the legacy --metal auto")
        t |> success(!mtp_default_resolved(1, "", ""), "one stream, no device")
        t |> success(!mtp_default_resolved(1, "off", "required"), "one stream on the CPU, whatever --metal says")
        t |> success(!mtp_default_resolved(4, "metal", ""), "four streams on Metal")
        t |> success(!mtp_default_resolved(4, "vulkan", ""), "four streams on a Vulkan device")
    }
    t |> run("a slot arms on one stream whose decode runs on a GPU") @(t : T?) {
        t |> success(mtp_auto_arms(1l, true, "cpu"), "the Metal flavor - its tier word reads cpu")
        t |> success(mtp_auto_arms(1l, false, "gpu:resident"), "the whole model resident on a Vulkan device")
        t |> success(!mtp_auto_arms(1l, false, "gpu:rails"), "a model the device serves in part")
        t |> success(!mtp_auto_arms(1l, false, "cpu"), "the CPU")
        t |> success(!mtp_auto_arms(4l, true, "cpu"), "four streams on Metal")
        t |> success(!mtp_auto_arms(4l, false, "gpu:resident"), "four streams on a Vulkan device")
    }
    t |> run("the Metal mode: the --gpu pick when there is one, else the legacy --metal mode") @(t : T?) {
        t |> success(resolved_metal_mode(MetalMode.when_available, "vulkan") == MetalMode.off, "a --gpu pick wins over --metal")
        t |> success(resolved_metal_mode(MetalMode.required, "off") == MetalMode.off, "gpu = off keeps Metal out")
        t |> success(resolved_metal_mode(MetalMode.off, "metal") == MetalMode.when_available, "gpu = metal")
        t |> success(resolved_metal_mode(MetalMode.off, "metal-required") == MetalMode.required, "gpu = metal-required")
        t |> success(resolved_metal_mode(MetalMode.required, "") == MetalMode.required, "no --gpu pick: the legacy mode stands")
    }
    t |> run("a --metal spelling names its mode in any case, and an unknown one names none") @(t : T?) {
        t |> success(metal_mode_of("Required").mode == MetalMode.required && metal_mode_of("Required").known)
        t |> success(metal_mode_of("auto").mode == MetalMode.when_available && metal_mode_of("when_available").known)
        t |> success(metal_mode_of("").mode == MetalMode.off && metal_mode_of("").known && metal_mode_of("off").known)
        t |> success(metal_mode_of("fast").mode == MetalMode.off && !metal_mode_of("fast").known)
    }
}
