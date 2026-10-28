options gen2
options persistent_heap   // the server thread loads and frees the models once
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require openai_server                                   // set_model / add_embedding_model / run_server (sibling)
require _server_rig                                     // models_dir / model_missing / serve_clients / stream_tally (sibling)
require model_catalog                                   // embedding_gguf: the roster's embedding-model sniff (sibling)
require dasllama/dasllama                               // load_model / embed: the reference vectors
require openai/openai_common                            // openai_client / post_json / get_json
require daslib/jobque_boost                             // with_job_status / new_thread: the server thread and the concurrent chat
require daslib/json_boost
require daslib/fio
require daslib/env_registry                              // env_value_of: the DASLLAMA_GPU the cell unsets and puts back
require strings
require math

// An embedding model on the server: a GGUF that names its pooling (Qwen3-Embedding-0.6B) is served by a worker thread of
// its own beside a chat slot (Qwen3-0.6B) - it is on /v1/models and the `embed` block of /v1/stats, never among the engine
// slots, its vectors equal the facade `embed` called here, a chat stream asked beside it finishes, the request
// refusals keep their texts (the context check on the worker under a 64-token context), and a chat model named in
// /v1/embeddings still answers on the engine. The worker's queue, wake and failure arms run under the server's worker seam
// (`set_worker_seam`), and main.das's roster boot is spawned as a child. The cells skip without the two files and run JIT-only.

let TEST_PORT = 18220   // 18220 .. 18226: this file's seven boots; 18097-18213 belong to the sibling suites

let EMB_PATH = path_join(models_dir(), "Qwen3-Embedding-0.6B-Q8_0.gguf")
let CHAT_PATH = path_join(models_dir(), "Qwen3-0.6B-Q8_0.gguf")
let EMB_CTX = 64l   // the embedder's context: an input past it is refused on the worker
let CHAT_CTX = 4096l   //! the chat slot's region context on a Vulkan box: four streams' mirrors fit beside the embedder's
let TEXT = "the quick brown fox jumps over the lazy dog"

//! the server's own Vulkan ask before the chat model loads: a region a stream, so the chat slot's streams are device-home
def private chat_residency_ask {
    if (gpu_tier_status().backend == "vulkan") {
        gpu_residency_ask(4l, CHAT_CTX)
    }
}

//! the chat-plus-embedder boot; `blk` learns whether the chat model's lane is planes-trimmed (no CPU planes)
def private with_embed_server(port : int; blk : block<(base_url : string; chat_trimmed : bool) : void>) {
    with_job_status(1) $(finished) {
        with_atomic32() $(trimmed) {
            new_thread() <| @capture(= finished, = trimmed) {
                allow_cpu_prefill()
                chat_residency_ask()
                var m <- load_model(CHAT_PATH, QuantMode.q8)
                trimmed |> set(m.planes_trimmed ? 1 : 0)
                set_model(m, "chat")
                set_default_max_tokens(8l)
                add_embedding_model(EMB_PATH, "embedder", QuantMode.q8, EMB_CTX)
                run_server(port)   // blocks until POST /shutdown
                finished |> notify_and_release
            }
            let ready = serve_clients(port, 1200, "/v1/models") $(base_url) {   // up to ~120 s: the loads + the JIT
                invoke(blk, base_url, trimmed |> get == 1)
            }
            finished |> join
            assert(ready, "the embedding server became ready")
        }
    }
}

//! the vectors of an embeddings answer, in order; none when the body does not parse
def private answer_vectors(body : string) : array<array<float>> {
    var err = ""
    var js = read_json(body, err)
    var out : array<array<float>>
    let data = js?["data"]
    if (data != null && (data.value is _array)) {
        out |> reserve(length(data.value as _array))
        for (row in data.value as _array) {
            var vec : array<float>
            let e = row?["embedding"]
            if (e != null && (e.value is _array)) {
                vec |> reserve(length(e.value as _array))
                for (x in e.value as _array) {
                    vec |> push(float(x ?? 0.0lf))
                }
            }
            out |> emplace(vec)
        }
    }
    unsafe {
        delete js
    }
    return <- out
}

def private cosine(a, b : array<float>) : float {
    return 0.0 if (length(a) != length(b) || empty(a))
    var dot, na, nb = 0.0lf
    for (x, y in a, b) {
        dot += double(x) * double(y)
        na += double(x) * double(x)
        nb += double(y) * double(y)
    }
    return float(dot / sqrt(na * nb))
}

def private json_string(body : string; path : array<string>) : string {
    var err = ""
    var js = read_json(body, err)
    var at = js
    for (key in path) {
        at = at?[key]
    }
    let v = at ?? ""
    unsafe {
        delete js
    }
    return v
}

//! the `embed` block of /v1/stats: its done_jobs, whether its first model is ready, and whether the engine slots list `id`
struct EmbedStats {
    done_jobs : int64
    ready : bool
    id : string
    slot_named : bool
    backend : string          //! where the worker serves the embedding model
    device_prefills : int64   //! the worker device's prompt windows
    cpu_passes : int64        //! the calls its device handed to the CPU rails
    chat_backend : string     //! the `chat` slot's arm outcome
    chat_holds_gpu : bool     //! ... and whether it holds the engine's device tier
    active : int64            //! the engine's live streams
}

def private embed_stats(client : OpenAIClient; id : string) : EmbedStats {
    var err = ""
    var js = read_json((client |> get_json("/v1/stats")).body, err)
    var st = EmbedStats(done_jobs = js?["embed"]?["done_jobs"] ?? -1l, ready = js?["embed"]?["models"]?[0]?["ready"] ?? false,
        id = js?["embed"]?["models"]?[0]?["id"] ?? "", backend = js?["embed"]?["models"]?[0]?["backend"] ?? "",
        device_prefills = js?["embed"]?["models"]?[0]?["device_prefills"] ?? -1l, cpu_passes = js?["embed"]?["models"]?[0]?["cpu_passes"] ?? -1l,
        active = js?["active"] ?? -1l)
    let slots = js?["models"]
    if (slots != null && (slots.value is _array)) {
        for (s in slots.value as _array) {
            if ((s?["name"] ?? "") == id) {
                st.slot_named = true
            }
            if ((s?["name"] ?? "") == "chat") {
                st.chat_backend = s?["backend_effective"] ?? ""
                st.chat_holds_gpu = s?["holds_gpu"] ?? false
            }
        }
    }
    unsafe {
        delete js
    }
    return st
}

//! the facade's vectors of TEXT, whole and cut to 8, the reference the worker's answers hold to; `device_box` where this box's
//! own load served the model whole off its device tier - the worker's context must too
struct RefVectors {
    want : array<float>
    cut : array<float>
    device_box : bool
}

def private reference_vectors : RefVectors {
    var m <- load_model(EMB_PATH, QuantMode.q8)
    var r = RefVectors(device_box = gpu_device_sessions() > 0l)
    r.want <- embed(m, TEXT)
    r.cut <- embed(m, TEXT, -1l, 8l)
    moe_gpu_drop_model()   //! the reference's device state leaves the card before the server's two tiers arm
    delete m
    return <- r
}

//! where the worker served a request, `before` and `after` its stats: on a box whose own load armed the device (`device_box`) the
//! worker's tier serves the model whole beside the chat slot's resident one, with no call passed to the CPU rails; elsewhere the CPU
def private served_beside_chat(t : T?; before, after : EmbedStats; device_box : bool) {
    if (!device_box) {
        t |> equal(after.backend, "cpu", "a box with no device tier serves the embedding model on the CPU")
        return
    }
    t |> equal(after.backend, "gpu:resident", "the worker's device tier serves the embedding model whole")
    t |> success(after.device_prefills > before.device_prefills, "the request's windows ran on the worker's device ({before.device_prefills} -> {after.device_prefills})")
    t |> equal(after.cpu_passes, 0l, "no call of the worker's passed to the CPU rails")
    t |> success(after.chat_backend == "gpu:resident" && after.chat_holds_gpu, "beside the chat slot's model, resident on the engine's tier ({after.chat_backend}, holds_gpu {after.chat_holds_gpu})")
}

[test]
def test_embedding_gguf(t : T?) {
    return if (model_missing(t, [EMB_PATH, CHAT_PATH]))
    t |> run("a GGUF that names its pooling is an embedding model; a chat model's names none") @(t : T?) {
        t |> success(embedding_gguf(EMB_PATH), "Qwen3-Embedding carries <arch>.pooling_type")
        t |> success(!embedding_gguf(CHAT_PATH), "Qwen3-0.6B carries no pooling_type")
        t |> success(!embedding_gguf(path_join(models_dir(), "no-such-model.gguf")), "a missing file is not one")
    }
}

[test]
def test_embedding_worker(t : T?) {
    return if (model_missing(t, [EMB_PATH, CHAT_PATH]))
    allow_cpu_prefill()
    var inscope ref <- reference_vectors()
    with_embed_server(TEST_PORT) $(base_url; _chat_trimmed) {
        let client = openai_client(base_url)
        {   // the embedding model is a worker model, not an engine slot, and its vectors are the facade's
            let before = embed_stats(client, "embedder")
            t |> success(before.ready && before.id == "embedder", "the embed block lists the loaded model")
            t |> success(!before.slot_named, "the engine slots do not list it")
            let models = client |> get_json("/v1/models")
            t |> success(find(models.body, "\"embedder\"") >= 0 && find(models.body, "\"chat\"") >= 0, "both on /v1/models: {models.body}")
            let a = client |> post_json("/v1/embeddings", "\{\"model\":\"embedder\",\"input\":[\"{TEXT}\",\"{TEXT}\"]}")
            t |> equal(a.status, 200, "200: {tail_of(a.body)}")
            var inscope got <- answer_vectors(a.body)
            t |> equal(length(got), 2, "one vector an input")
            if (length(got) == 2) {
                t |> success(cosine(got[0], ref.want) > 0.9999, "the worker's vector is the facade's ({cosine(got[0], ref.want)})")
                t |> success(cosine(got[1], ref.want) > 0.9999, "the second input too")
            }
            t |> equal(json_string(a.body, ["model"]), "embedder", "the answer names the model")
            t |> success(find(a.body, "\"prompt_tokens\"") >= 0, "usage counted")
            let after = embed_stats(client, "embedder")
            t |> equal(after.done_jobs, before.done_jobs + 1l, "the worker's job counter advanced")
            served_beside_chat(t, before, after, ref.device_box)
            let cut = client |> post_json("/v1/embeddings", "\{\"model\":\"embedder\",\"input\":\"{TEXT}\",\"dimensions\":8}")
            var inscope cut_got <- answer_vectors(cut.body)
            t |> success(length(cut_got) == 1 && length(cut_got[0]) == 8 && cosine(cut_got[0], ref.cut) > 0.9999, "the Matryoshka cut on the worker: {tail_of(cut.body)}")
        }
        {   // a chat stream and an embeddings request asked together both finish
            with_job_status(1) $(chat_done) {
                with_atomic32() $(content_seen) {
                    new_thread() <| @capture(= base_url, = chat_done, = content_seen) {
                        let tally = stream_tally(base_url, "\{\"model\":\"chat\",\"stream\":true,\"max_tokens\":8,\"messages\":[\{\"role\":\"user\",\"content\":\"Say hello.\"}]}")
                        content_seen |> set(tally.n_content + tally.n_reasoning > 0 && !empty(tally.finish) ? 1 : 0)
                        chat_done |> notify_and_release
                    }
                    let a = client |> post_json("/v1/embeddings", "\{\"model\":\"embedder\",\"input\":\"{TEXT}\"}")
                    var inscope got <- answer_vectors(a.body)
                    t |> success(a.status == 200 && length(got) == 1 && cosine(got[0], ref.want) > 0.9999, "the embedding is right beside the stream: {tail_of(a.body)}")
                    chat_done |> join
                    t |> equal(content_seen |> get, 1, "the chat stream sent tokens and finished")
                    let both = embed_stats(client, "embedder")
                    t |> success(!ref.device_box || (both.backend == "gpu:resident" && both.chat_holds_gpu && both.cpu_passes == 0l),
                        "after the stream beside the embedding both models are still resident on their tiers, nothing swapped out")
                }
            }
        }
        {   // the refusals keep their texts
            let long_input = build_string() $(var w) {
                for (i in range(200)) {
                    w |> write("word{i} ")
                }
            }
            for (refusal in [("\{\"model\":\"embedder\"}", "'input' must be a non-empty string"),   // a missing key reads as a null value, as the chat path has always answered
                             ("\{\"model\":\"embedder\",\"input\":\"\"}", "'input' must be a non-empty string"),
                             ("\{\"model\":\"embedder\",\"input\":[]}", "'input' is empty"),
                             ("\{\"model\":\"embedder\",\"input\":\"x\",\"pooling\":\"median\"}", "'pooling' must be \\\"mean\\\""),
                             ("\{\"model\":\"embedder\",\"input\":\"x\",\"dimensions\":0}", "'dimensions' must be 1..1024"),
                             ("\{\"model\":\"embedder\",\"input\":\"x\",\"dimensions\":1000000}", "'dimensions' must be 1..1024"),
                             ("\{\"model\":\"embedder\",\"input\":\"{long_input}\"}", "the model context is {EMB_CTX} - shorten it")]) {
                let a = client |> post_json("/v1/embeddings", refusal._0)
                t |> equal(a.status, 400, "{refusal._1}: 400")
                t |> success(find(a.body, refusal._1) >= 0, "{refusal._1}: the error names it: {tail_of(a.body)}")
            }
            let nobody = client |> post_json("/v1/embeddings", "\{\"model\":\"nobody\",\"input\":\"x\"}")
            t |> equal(nobody.status, 404, "an unknown model is a 404")
        }
        {   // a chat model named in /v1/embeddings answers on the engine
            let before = embed_stats(client, "embedder")
            let a = client |> post_json("/v1/embeddings", "\{\"model\":\"chat\",\"input\":\"{TEXT}\"}")
            t |> equal(a.status, 200, "200: {tail_of(a.body)}")
            var inscope got <- answer_vectors(a.body)
            t |> success(length(got) == 1 && !empty(got[0]), "one vector")
            t |> equal(json_string(a.body, ["model"]), "chat", "the chat slot answered")
            t |> equal(embed_stats(client, "embedder").done_jobs, before.done_jobs, "the embedding worker took no job")
        }
    }
}

//! A worker that cannot load degrades its model: the boot serves on, the model is off /v1/models and serves nothing,
//! and the `embed` block carries the reason.
[test]
def test_embedding_worker_load_failure(t : T?) {
    return if (model_missing(t, CHAT_PATH))
    let broken = path_join(test_temp_dir(), "broken-embedder.gguf")
    t |> success(fwrite(broken, "not a GGUF"), "planted the broken model")
    with_job_status(1) $(finished) {
        new_thread() <| @capture(= finished, = broken) {
            allow_cpu_prefill()
            chat_residency_ask()
            var m <- load_model(CHAT_PATH, QuantMode.q8)
            set_model(m, "chat")
            add_embedding_model(broken, "broken", QuantMode.q8, 0l)
            run_server(TEST_PORT + 1)
            finished |> notify_and_release
        }
        let ready = serve_clients(TEST_PORT + 1, 1200, "/v1/models") $(base_url) {
            let client = openai_client(base_url)
            let models = client |> get_json("/v1/models")
            t |> success(find(models.body, "\"broken\"") < 0, "the broken model is off /v1/models: {models.body}")
            let a = client |> post_json("/v1/embeddings", "\{\"model\":\"broken\",\"input\":\"x\"}")
            t |> equal(a.status, 404, "it serves nothing: {a.body}")
            let stats = client |> get_json("/v1/stats")
            t |> success(!empty(json_string(stats.body, ["embed", "models"])) || find(stats.body, "\"embed\":") >= 0, "the embed block is there")
            var err = ""
            var js = read_json(stats.body, err)
            t |> success(!empty(js?["embed"]?["models"]?[0]?["error"] ?? ""), "the stats block carries the load failure: {tail_of(stats.body)}")
            t |> equal(js?["embed"]?["models"]?[0]?["ready"] ?? true, false, "not ready")
            unsafe {
                delete js
            }
        }
        finished |> join
        t |> success(ready, "the server booted past the failed load")
    }
    remove(broken)
}

//! whether this box's own load of the embedding model serves it whole off the device tier - the cells below hold what the
//! server does on such a box, and skip elsewhere
def private vulkan_device_box(t : T?) : bool {
    var m <- load_model(EMB_PATH, QuantMode.q8)
    let served = gpu_device_sessions() > 0l
    moe_gpu_drop_model()
    delete m
    if (!served) {
        t |> skip("no Vulkan device tier serves the embedding model on this box")
    }
    return served
}

//! With DASLLAMA_GPU unset the worker's context arms its device tier off the want the server hands it alone: the want the
//! roster's slot gets reaches the worker (`add_embedding_model`'s `gpu_want`), and the worker serves on the device.
[test]
def test_embedding_worker_gpu_want(t : T?) {
    return if (model_missing(t, EMB_PATH) || !vulkan_device_box(t))
    let env_was = env_value_of("DASLLAMA_GPU")
    set_env_variable("DASLLAMA_GPU", "")   //! read as unset by every context made from here on - the server's and its worker's
    with_job_status(1) $(finished) {
        new_thread() <| @capture(= finished) {
            add_embedding_model(EMB_PATH, "embedder", QuantMode.q8, EMB_CTX, GpuTierWant(auto_tier = true))
            run_server(TEST_PORT + 2)
            finished |> notify_and_release
        }
        let ready = serve_clients(TEST_PORT + 2, 1200, "/v1/models") $(base_url) {
            let client = openai_client(base_url)
            let before = embed_stats(client, "embedder")
            let a = client |> post_json("/v1/embeddings", "\{\"model\":\"embedder\",\"input\":\"{TEXT}\"}")
            t |> equal(a.status, 200, "200: {tail_of(a.body)}")
            let after = embed_stats(client, "embedder")
            t |> equal(after.backend, "gpu:resident", "the worker armed its device tier off the server's want with DASLLAMA_GPU unset")
            t |> success(after.device_prefills > before.device_prefills, "the request prefilled on the worker's device")
        }
        finished |> join
        t |> success(ready, "the embeddings-only server booted")
    }
    set_env_variable("DASLLAMA_GPU", env_was)
}

//! A chat slot whose model is a trimmed vulkan lane and whose streams cannot each hold a device region is refused at the
//! server's start, by name - never at its first prompt; the same boot with a region a stream starts (`test_embedding_worker`).
[test]
def test_trimmed_chat_refused_at_start(t : T?) {
    return if (model_missing(t, [EMB_PATH, CHAT_PATH]) || !vulkan_device_box(t))
    var refusal = ""
    var trimmed = false
    var served = false
    with_channel(1) $(ch) {
        new_thread() <| @capture(= ch) {
            allow_cpu_prefill()
            gpu_residency_ask(1l, CHAT_CTX)   //! one region under the server's four streams
            var m <- load_model(CHAT_PATH, QuantMode.q8)
            let lane_trimmed = m.planes_trimmed
            set_model(m, "chat")
            run_server(TEST_PORT + 3)   //! refused: returns without serving
            ch |> push_clone((why = clone_string(server_boot_refusal()), trimmed = lane_trimmed))
            ch |> notify_and_release
        }
        //! a boot that is not refused serves: the poll shuts it down, so the thread's answer lands either way
        served = serve_clients(TEST_PORT + 3, 300, "/v1/models") $(_base_url) {}
        ch |> for_each_clone() $(r : tuple<why : string; trimmed : bool>#) {
            refusal = clone_string(r.why)
            trimmed = r.trimmed
        }
    }
    if (!trimmed) {
        t |> skip("the chat model's lane is not trimmed on this box (DASLLAMA_TRIM)")
        return
    }
    t |> success(!served, "the refused boot never answered a request")
    t |> success(find(refusal, "slot 'chat'") >= 0 && find(refusal, "planes-trimmed") >= 0 && find(refusal, "region") >= 0,
        "the start is refused naming the slot, the trimmed lane and the regions: {refusal}")
}


//! A chat model named in /v1/embeddings whose lane is planes-trimmed embeds in a free device region: with every region
//! held by a live stream the request is refused 503, by reason, and once a stream finishes the same request answers 200.
[test]
def test_trimmed_chat_embedding_waits_for_a_region(t : T?) {
    return if (model_missing(t, [EMB_PATH, CHAT_PATH]) || !vulkan_device_box(t))
    let ask = "\{\"model\":\"chat\",\"input\":\"{TEXT}\"}"
    with_embed_server(TEST_PORT + 4) $(base_url; chat_trimmed) {
        if (!chat_trimmed) {
            t |> skip("the chat model's lane is not trimmed on this box (DASLLAMA_TRIM)")
            return
        }
        let client = openai_client(base_url)
        let streams = 4   //! chat_residency_ask's region count: one live stream a region
        with_job_status(streams) $(done) {
            for (_ in range(streams)) {
                new_thread() <| @capture(= base_url, = done) {
                    stream_tally(base_url, "\{\"model\":\"chat\",\"stream\":true,\"max_tokens\":1500,\"messages\":[\{\"role\":\"user\",\"content\":\"Count from 1 to 2000, one number a line.\"}]}")
                    done |> notify_and_release
                }
            }
            var live = 0l
            for (_ in range(500)) {
                live = embed_stats(client, "embedder").active
                break if (live == int64(streams))
                sleep(10u)
            }
            t |> equal(live, int64(streams), "every region holds a live stream")
            let held = client |> post_json("/v1/embeddings", ask)
            t |> equal(embed_stats(client, "embedder").active, int64(streams), "... still, after the request answered")
            t |> equal(held.status, 503, "a trimmed chat model with no free region refuses the embedding: {tail_of(held.body)}")
            t |> success(find(held.body, "holds a live stream") >= 0, "the refusal names the held regions: {tail_of(held.body)}")
            done |> join
        }
        let freed = client |> post_json("/v1/embeddings", ask)
        t |> equal(freed.status, 200, "with the streams done the same request answers: {tail_of(freed.body)}")
    }
}

let FAULT_TEXT = "the fixture's fault input"   //! the input whose job the worker seam panics
let WIDE_CTX = 16384l                          //! an explicit embedder context past EMB_SCRATCH_TOKENS

//! the embedding models' rows of /v1/stats, by id: each one's context; and the queue's pending count
struct EmbedQueue {
    pending : int64
    ctx : table<string; int64>
}

def private embed_queue(client : OpenAIClient) : EmbedQueue {
    var err = ""
    var js = read_json((client |> get_json("/v1/stats")).body, err)
    var q = EmbedQueue(pending = js?["embed"]?["pending"] ?? -1l)
    let rows = js?["embed"]?["models"]
    if (rows != null && (rows.value is _array)) {
        for (r in rows.value as _array) {
            q.ctx[r?["id"] ?? ""] = r?["ctx"] ?? -1l
        }
    }
    unsafe {
        delete js
    }
    return <- q
}

//! the embedding model's own context, as a load reads it
def private trained_ctx : int64 {
    var m <- load_model(EMB_PATH, QuantMode.q8)
    let ctx = m.config.seq_len
    moe_gpu_drop_model()
    delete m
    return ctx
}

//! the embeddings-only boot under the worker seam: `first` with no context of its own, `wide` with WIDE_CTX; every
//! worker parks while `hold` is non-zero, the server holds as a running benchmark while `bench_hold` is, the engine counts
//! its wakes in `wakes`, and FAULT_TEXT panics its job
def private with_seam_server(port : int; var hold, bench_hold, wakes : Atomic32?; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        var seam = WorkerSeam(hold = hold, bench_hold = bench_hold, wakes = wakes, embed_fault = FAULT_TEXT)
        new_thread() <| @capture(= finished, = seam) {
            var seam_was = worker_seam()
            set_worker_seam(seam)
            add_embedding_model(EMB_PATH, "first", QuantMode.q8, 0l)
            add_embedding_model(EMB_PATH, "wide", QuantMode.q8, WIDE_CTX)
            run_server(port)
            set_worker_seam(seam_was)
            finished |> notify_and_release
        }
        let ready = serve_clients(port, 1200, "/v1/models", blk)
        finished |> join
        assert(ready, "the embeddings-only seam server became ready")
    }
}

//! under the seam's bench hold an embedding is refused 503 by reason; released, the same request answers
def private bench_hold_refuses(t : T?; client : OpenAIClient; var bench_hold : Atomic32?) {
    let ask = "\{\"model\":\"first\",\"input\":\"{TEXT}\"}"
    bench_hold |> set(1)
    let held = client |> post_json("/v1/embeddings", ask)
    bench_hold |> set(0)
    t |> equal(held.status, 503, "an embedding asked under the bench hold is held: {tail_of(held.body)}")
    t |> success(find(held.body, "a benchmark is running") >= 0, "the hold says why: {tail_of(held.body)}")
    let freed = client |> post_json("/v1/embeddings", ask)
    t |> equal(freed.status, 200, "released, the same request answers: {tail_of(freed.body)}")
}

//! An embeddings-only server: a benchmark's hold refuses an embedding 503; a request naming no model goes to the first
//! embedding model; an embedder with no context of its own serves min(trained, EMB_SCRATCH_TOKENS) and an explicit one past
//! it serves whole; a worker's answer wakes the engine; a job that panics answers 500 and the worker serves on; past
//! EMB_QUEUE_LIMIT pending a request is refused 503.
[test]
def test_embedding_worker_seams(t : T?) {
    return if (model_missing(t, EMB_PATH))
    let trained = trained_ctx()
    with_atomic32() $(hold) {
        with_atomic32() $(bench_hold) {
            with_atomic32() $(wakes) {
                with_seam_server(TEST_PORT + 5, hold, bench_hold, wakes) $(base_url) {
                    let client = openai_client(base_url)
                    t |> run("a benchmark holding the server refuses an embedding; released, the same request answers") @(t : T?) {
                        bench_hold_refuses(t, client, bench_hold)
                    }
                    t |> run("an embedder's context: min(trained, EMB_SCRATCH_TOKENS) unset, the explicit one past it whole") @(t : T?) {
                        t |> success(trained > WIDE_CTX, "Qwen3-Embedding's trained context {trained} is past {WIDE_CTX}")
                        var inscope q <- embed_queue(client)
                        t |> equal(q.ctx?["first"] ?? -1l, min(trained, EMB_SCRATCH_TOKENS), "no ctx: the scratch budget")
                        t |> equal(q.ctx?["wide"] ?? -1l, WIDE_CTX, "an explicit ctx past the budget serves whole")
                    }
                    t |> run("a request naming no model goes to the first embedding model") @(t : T?) {
                        let a = client |> post_json("/v1/embeddings", "\{\"input\":\"{TEXT}\"}")
                        t |> equal(a.status, 200, "200: {tail_of(a.body)}")
                        t |> equal(json_string(a.body, ["model"]), "first", "the first embedding model answered")
                    }
                    t |> run("a worker's answer wakes the engine") @(t : T?) {
                        let before = wakes |> get
                        let a = client |> post_json("/v1/embeddings", "\{\"model\":\"wide\",\"input\":\"{TEXT}\"}")
                        t |> equal(a.status, 200, "200: {tail_of(a.body)}")
                        for (_ in range(2000)) {   // the wake rides the front channel behind the answer: wait for it, no deadline asserted
                            break if ((wakes |> get) > before)
                            sleep(1u)
                        }
                        t |> equal(wakes |> get, before + 1, "the engine took one wake for the one answer")
                    }
                    t |> run("a job that panics answers 500, and the worker serves the next") @(t : T?) {
                        let bad = client |> post_json("/v1/embeddings", "\{\"model\":\"first\",\"input\":[\"{TEXT}\",\"{FAULT_TEXT}\"]}")
                        t |> equal(bad.status, 500, "the panicked job is a 500: {tail_of(bad.body)}")
                        t |> success(find(bad.body, "the fixture's embedding fault") >= 0, "naming the failure: {tail_of(bad.body)}")
                        let next = client |> post_json("/v1/embeddings", "\{\"model\":\"first\",\"input\":\"{TEXT}\"}")
                        t |> equal(next.status, 200, "the same worker answers the next request: {tail_of(next.body)}")
                    }
                    t |> run("past EMB_QUEUE_LIMIT pending, a request is refused at capacity") @(t : T?) {
                        hold |> set(1)
                        with_job_status(EMB_QUEUE_LIMIT) $(done) {
                            with_atomic32() $(answered) {
                                for (_ in range(EMB_QUEUE_LIMIT)) {
                                    new_thread() <| @capture(= base_url, = done, = answered) {
                                        let a = openai_client(base_url) |> post_json("/v1/embeddings", "\{\"model\":\"first\",\"input\":\"{TEXT}\"}")
                                        if (a.status == 200) {
                                            answered |> inc
                                        }
                                        done |> notify_and_release
                                    }
                                }
                                var pending = 0l
                                for (_ in range(3000)) {   // the parked worker holds every job: wait until the queue is full
                                    pending = embed_queue(client).pending
                                    break if (pending >= int64(EMB_QUEUE_LIMIT))
                                    sleep(10u)
                                }
                                t |> equal(pending, int64(EMB_QUEUE_LIMIT), "the parked worker's queue filled")
                                let over = client |> post_json("/v1/embeddings", "\{\"model\":\"first\",\"input\":\"{TEXT}\"}")
                                hold |> set(0)
                                t |> equal(over.status, 503, "the request past the limit is refused: {tail_of(over.body)}")
                                t |> success(find(over.body, "at capacity") >= 0, "the refusal says the queue is at capacity: {tail_of(over.body)}")
                                done |> join
                                t |> equal(answered |> get, EMB_QUEUE_LIMIT, "every queued request answered once the worker went on")
                            }
                        }
                    }
                    hold |> set(0)
                }
            }
        }
    }
}

//! main.das's boot from a config roster holding a chat GGUF and an embedding GGUF - the embedding one marked default: the
//! embedding model lands on a worker (the `embed` block, at the scratch budget's context), never an engine slot, and the
//! chat model takes the default, so a request naming no model embeds on the chat slot.
[test]
def test_roster_routes_an_embedding_gguf(t : T?) {
    return if (model_missing(t, [EMB_PATH, CHAT_PATH]))
    let config = build_string() $(var w) {
        w |> write("[[models]]\nname = \"chat\"\npath = \"{CHAT_PATH}\"\nctx = 4096\n\n")
        w |> write("[[models]]\nname = \"emb\"\npath = \"{EMB_PATH}\"\ndefault = true\n")
    }
    var inscope no_flags : array<string>
    with_spawned_server(t, "the roster boot", TEST_PORT + 6, no_flags, config) $(base_url) {
        let client = openai_client(base_url)
        let models = client |> get_json("/v1/models")
        t |> success(find(models.body, "\"emb\"") >= 0 && find(models.body, "\"chat\"") >= 0, "both on /v1/models: {models.body}")
        let st = embed_stats(client, "emb")
        t |> success(st.ready && st.id == "emb", "the embedding model is the worker's: {st.id}")
        t |> success(!st.slot_named, "never an engine slot")
        t |> success(!empty(st.chat_backend), "the chat model is the engine's")
        var inscope q <- embed_queue(client)
        t |> equal(q.ctx?["emb"] ?? -1l, EMB_SCRATCH_TOKENS, "no ctx in the roster: the scratch budget")
        let named = client |> post_json("/v1/embeddings", "\{\"model\":\"emb\",\"input\":\"{TEXT}\"}")
        t |> equal(named.status, 200, "the worker answers: {tail_of(named.body)}")
        let unnamed = client |> post_json("/v1/embeddings", "\{\"input\":\"{TEXT}\"}")
        t |> equal(unnamed.status, 200, "200: {tail_of(unnamed.body)}")
        t |> equal(json_string(unnamed.body, ["model"]), "chat", "the chat model is the default, the default mark on the embedding model notwithstanding")
    }
}
