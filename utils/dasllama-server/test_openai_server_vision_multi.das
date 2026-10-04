options gen2
options persistent_heap   // the server thread loads and frees two models once
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // an inference tool test, not the bit-exact suite
options _dasllama_internal = true

require dastest/testing_boost public
require openai_server                                   // add_model / set_model_vision / run_server (sibling)
require _server_rig                                     // models_dir / image_data_uri (sibling)
require dasllama/dasllama                               // load_model / QuantMode
require daslib/jobque_boost                             // the server thread, the two clients at once
require daslib/json_boost
require daslib/fio
require strings

// Two vision models of different families on ONE server - gemma-4-E2B (the gemma4v tower) and
// Qwen3-VL-4B (the qwen3v tower, grid positions) - each slot with its own mmproj: an image request
// routes by its `model` field to that slot's encoder and decoder, each alone and both at once, and
// both captions name the cats. Model-gated (both decoders, both mmprojs, the cats photo) and
// JIT-only; skips cleanly otherwise.

let TEST_PORT = 18162   // 18097-18161 belong to the sibling suites

let GEMMA = "gemma-4-E2B-it-Q8_0.gguf"
let GEMMA_MM = "mmproj-gemma-4-E2B-it-bf16.gguf"
let QWEN = "Qwen3VL-4B-Instruct-Q8_0.gguf"
let QWEN_MM = "mmproj-Qwen3VL-4B-Instruct-F16.gguf"
let CATS = "coco-val2017-000000039769.jpg"

def private with_two_vision_models(port : int; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            set_metal_mode(MetalMode.off)
            allow_cpu_prefill()   // both slots serve on the CPU arm: the cell is the routing, not a backend
            var ma <- load_model_(path_join(models_dir(), GEMMA), QuantMode.q8)
            add_model(ma, "gemma")
            set_model_vision("gemma", path_join(models_dir(), GEMMA_MM))
            var mb <- load_model_(path_join(models_dir(), QWEN), QuantMode.q8)
            add_model(mb, "qwen", SlotOpts(backend = "cpu"))
            set_model_vision("qwen", path_join(models_dir(), QWEN_MM))
            set_default_model("gemma")
            set_default_max_tokens(32l)
            run_server(port)   // blocks until POST /shutdown
            finished |> notify_and_release
        }
        let ready = serve_clients(port, 2400, "/v1/models", blk)   // up to ~240 s: two loads, two mmprojs, the JIT
        finished |> join
        assert(ready, "the two-vision-model server became ready")
    }
}

//! one image question to `model`: the reply's content, lowered, "" with the status in `why` when it failed
//! (the body is built here, not by the client library's `chat_vision`: the cell pins greedy sampling and thinking off, which that call does not send)
def private caption(base_url, model, uri : string; var why : string&) : string {
    var body = "\{\"model\":\"{model}\",\"messages\":[\{\"role\":\"user\",\"content\":["
    body += "\{\"type\":\"image_url\",\"image_url\":\{\"url\":\"{uri}\"}},"
    body += "\{\"type\":\"text\",\"text\":\"What animals are in this picture? Answer in one short sentence.\"}"
    body += "]}],\"max_tokens\":32,\"temperature\":0,\"enable_thinking\":false}"
    var reply = ""
    POST("{base_url}/v1/chat/completions", body) $(resp) {
        if (resp == null || resp.status_code != http_status.OK) {
            why = "status {resp != null ? int(resp.status_code) : 0}: {resp != null ? string(resp.body) : ""}"
            return
        }
        var err = ""
        var js = read_json(string(resp.body), err)
        reply = to_lower(js?["choices"]?[0]?["message"]?["content"] ?? "")
        let served = js?["model"] ?? ""
        if (served != model) {
            why = "answered by '{served}'"
        }
        unsafe {
            delete js
        }
    }
    return reply
}

// both models asked at once, a thread each; the count of captions that name the cats
def private both_at_once(base_url, uri : string) : int {
    var named = 0
    with_stream() $(var jobs : Stream?) {
        var ask = (base = base_url, uri = uri)
        jobs |> push_archive(ask)
        jobs |> push_archive(ask)
        with_atomic32() $(good) {
            with_job_status(2) $(done) {
                for (k in range(2)) {
                    new_thread() @() {
                        jobs |> pop_archive() $(var job : tuple<base : string; uri : string>) {
                            var why = ""
                            let reply = caption(job.base, k == 0 ? "gemma" : "qwen", job.uri, why)
                            if (empty(why) && find(reply, "cat") >= 0) {
                                good |> inc
                            }
                        }
                        jobs |> release
                        done |> notify_and_release
                    }
                }
                done |> join
            }
            named = good |> get
        }
    }
    return named
}

[test]
def test_two_vision_models_serve_at_once(t : T?) {
    var inscope files <- [for (f in [GEMMA, GEMMA_MM, QWEN, QWEN_MM, CATS]); path_join(models_dir(), f)]
    return if (model_missing(t, files))
    let uri = image_data_uri(path_join(models_dir(), CATS))
    with_two_vision_models(TEST_PORT) $(base_url) {
        t |> run("each model captions the photo through its own encoder, and an id no slot carries is a 404") @(t : T?) {
            for (model in ["gemma", "qwen"]) {
                var why = ""
                let reply = caption(base_url, model, uri, why)
                t |> equal(why, "", "{model} answered")
                t |> success(find(reply, "cat") >= 0, "{model} calls the cats cats: '{reply}'")
            }
            var why = ""
            caption(base_url, "nobody", uri, why)
            t |> success(find(why, "status 404") >= 0, "an id no slot carries: {why}")
        }
        t |> run("both models caption the photo when asked at once") @(t : T?) {
            t |> equal(both_at_once(base_url, uri), 2, "gemma and qwen each named the cats")
        }
    }
}
