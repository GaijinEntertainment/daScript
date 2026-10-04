options gen2
options persistent_heap   // the server thread loads and frees the models once
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require openai_server                                   // add_asr_model / add_tts_model / run_server (sibling)
require _server_rig                                     // models_dir (sibling)
require openai/openai_audio                             // transcribe / speech: the two speech requests
require dashv/dashv_boost                               // GET / POST: the readiness poll, /v1/models, the shutdown
require daslib/jobque_boost                             // with_job_status / new_thread: the server thread, the four clients at once
require daslib/json_boost
require daslib/fio
require strings
require daslib/strings_boost   // count

// Several speech models on ONE server (no LLM slot): two ASR models and two TTS models, each
// routed by the request's `model` field. Every id is on /v1/models; each model answers under its
// own id and the first of a kind answers a request that names none; an id nothing serves is a 404;
// and the four models answer four requests sent at once - each model's worker is its own thread,
// so the four overlap instead of queueing behind one another.
// Model-gated (Qwen3-ASR 0.6B and 1.7B with their mmprojs, kitten-nano with the front-end packs,
// pocket-tts-en-q8) and JIT-only; skips cleanly otherwise.

let TEST_PORT = 18170   // 18170, 18173, 18174: this file's three boots; 18097-18163 belong to the sibling suites

let ASR_A = "Qwen3-ASR-0.6B-Q8_0"
let ASR_B = "Qwen3-ASR-1.7B-Q8_0"
let TTS_A = "kitten-nano"
let TTS_B = "pocket-tts-en-q8"
let JFK_PATH = "modules/dasLLAMA/models/jfk_ask_not.wav"   // in-tree, 4 s, 16 kHz mono

def private asr_mmproj(id : string) : string
    => path_join(models_dir(), "mmproj-{replace(id, "-Q8_0", "")}-bf16.gguf")

//! every file the roster needs, by path
def private roster_files : array<string> {
    var files <- [for (f in ["{ASR_A}.gguf", "{ASR_B}.gguf", "{TTS_A}.gguf", "{TTS_B}.gguf", "tts_g2p.bin", "tts_postag.bin"]); path_join(models_dir(), f)]
    files |> push(asr_mmproj(ASR_A))
    files |> push(asr_mmproj(ASR_B))
    return <- files
}

def private with_speech_roster(port : int; asr_models : int; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() <| @capture(= finished) {
            for (id, _k in [ASR_A, ASR_B], range(asr_models)) {
                add_asr_model(path_join(models_dir(), "{id}.gguf"), asr_mmproj(id))
            }
            for (id in [TTS_A, TTS_B, TTS_A]) {   // the third names an id already served: it is left out, not served twice
                add_tts_model(path_join(models_dir(), "{id}.gguf"))
            }
            run_server(port)   // blocks until POST /shutdown
            finished |> notify_and_release
        }
        let ready = serve_clients(port, 1200, "/v1/models", blk)   // up to ~120 s: four loads + the JIT
        finished |> join
        assert(ready, "the speech roster server became ready")
    }
}

//! the model of the newest job on the poll surface's `asr` list
def private last_asr_model(base_url : string) : string {
    var model = ""
    GET("{base_url}/v1/streams") $(resp) {
        var err = ""
        var js = read_json(resp != null ? string(resp.body) : "", err)
        let jobs = js?["asr"]
        if (jobs != null && (jobs.value is _array) && !empty(jobs.value as _array)) {
            var newest = -1l
            for (job in jobs.value as _array) {
                if ((job?["id"] ?? 0l) > newest) {
                    newest = job?["id"] ?? 0l
                    model = job?["model"] ?? ""
                }
            }
        }
        unsafe {
            delete js
        }
    }
    return model
}

//! the uploads the server's front holds in the temp directory
def private held_uploads : int {
    var n = 0
    var err = ""
    dir(temp_directory(err)) $(name) {
        if (starts_with(name, "dasllama_upload_")) {
            n++
        }
    }
    return n
}

def private heard_jfk(text : string) : bool
    => find(to_lower(text), "fellow americans") >= 0

def private said(client : OpenAIClient; model : string) : SpeechResult
    => speech(client, SpeechRequest(model = model, input = "Two models speak at once.", voice = "", response_format = "wav"))

// one request per model, the four on four threads; the count of the ones that answered right
def private four_at_once(base_url : string) : int {
    var ok = 0
    with_atomic32() $(good_count) {
        with_job_status(4) $(done) {
            for (k in range(4)) {
                new_thread() <| @capture(= base_url, = good_count, = done, = k) {
                    let client = openai_client("{base_url}/v1")
                    var good = false
                    if (k < 2) {
                        let res = transcribe(client, k == 0 ? ASR_A : ASR_B, JFK_PATH)
                        good = res.ok && heard_jfk(res.text)
                    } else {
                        var res <- said(client, k == 2 ? TTS_A : TTS_B)
                        good = res.ok && length(res.audio) > 44
                        delete res
                    }
                    if (good) {
                        good_count |> inc
                    }
                    done |> notify_and_release
                }
            }
            done |> join
        }
        ok = good_count |> get
    }
    return ok
}

[test]
def test_two_asr_and_two_tts_models_serve_at_once(t : T?) {
    var inscope files <- roster_files()
    return if (model_missing(t, files))
    with_speech_roster(TEST_PORT, 2) $(base_url) {
        let client = openai_client("{base_url}/v1")
        t |> run("every speech model is listed under its own id") @(t : T?) {
            GET("{base_url}/v1/models") $(resp) {
                let body = resp != null ? string(resp.body) : ""
                for (id in [ASR_A, ASR_B, TTS_A, TTS_B]) {
                    t |> equal(count(body, "\"{id}\""), 1, "{id}, once, in {body}")
                }
            }
            GET("{base_url}/v1/stats") $(resp) {
                var err = ""
                var js = read_json(resp != null ? string(resp.body) : "", err)
                let both = write_json(js?["tts"]?["models"])
                t |> success(find(both, TTS_A) >= 0 && find(both, TTS_B) >= 0, "the tts block names both speaking models: {both}")
                unsafe {
                    delete js
                }
            }
        }
        t |> run("each ASR model answers under its id, the first with none, and an unserved id is a 404") @(t : T?) {
            for (id in [ASR_A, ASR_B, ""]) {
                let res = transcribe(client, id, JFK_PATH)
                t |> success(res.ok && heard_jfk(res.text), "'{id}': {res.text}{res.error.message}")
                t |> equal(last_asr_model(base_url), empty(id) ? ASR_A : id, "the job ran on the model the request named")
            }
            let uploads = held_uploads()
            for (id in ["nobody", "nobody-asr"]) {   // with several models a name is an id or nothing: no `-asr` name is an alias here
                let res = transcribe(client, id, JFK_PATH)
                t |> success(!res.ok)
                t |> equal(res.error.status, 404, "'{id}': {res.error.message}")
            }
            t |> equal(held_uploads(), uploads, "a refused request's upload is not left in the temp directory")
        }
        t |> run("each TTS model answers under its id, in its own voice, and an unserved id is a 404") @(t : T?) {
            var sizes : array<int>
            sizes |> reserve(2)
            for (id in [TTS_A, TTS_B]) {
                var res <- said(client, id)
                t |> success(res.ok && length(res.audio) > 44, "'{id}': {res.error.message}")
                sizes |> push(length(res.audio))
                delete res
            }
            t |> success(sizes[0] != sizes[1], "the two families say the line at their own lengths: {sizes}")
            var none <- said(client, "no-such-tts")
            t |> success(!none.ok)
            t |> equal(none.error.status, 404, none.error.message)
            delete none
        }
        t |> run("the four models answer four requests sent at once") @(t : T?) {
            t |> equal(four_at_once(base_url), 4, "ASR a, ASR b, TTS a and TTS b each answered")
        }
    }
}

//! One ASR model takes every transcription, whatever model the request names: a client written for another server
//! names a model of its own, and there is only one to mean.
[test]
def test_one_asr_model_takes_any_name(t : T?) {
    var inscope files <- roster_files()
    return if (model_missing(t, files))
    with_speech_roster(TEST_PORT + 3, 1) $(base_url) {
        let client = openai_client("{base_url}/v1")
        for (id in ["whisper-1", "", ASR_A]) {
            let res = transcribe(client, id, JFK_PATH)
            t |> success(res.ok && heard_jfk(res.text), "'{id}': {res.text}{res.error.message}")
        }
    }
}

//! An ASR model that cannot load fails the boot with the loader's reason: the speech thread reports it to the engine,
//! which was waiting on that thread's first word. The control is the return itself - a boot that hung never gets here.
[test]
def test_asr_load_failure_fails_the_boot(t : T?) {
    var inscope files <- roster_files()
    return if (model_missing(t, files))
    var panicked = 0
    with_job_status(1) $(finished) {
        with_atomic32() $(failed) {
            new_thread() @() {
                add_tts_model(path_join(models_dir(), "{TTS_A}.gguf"))   // a speech model that serves: the boot is a server's
                add_asr_model(path_join(models_dir(), "no-such-asr-model.gguf"), "")
                try {
                    run_server(TEST_PORT + 4)
                } recover {
                    failed |> set(1)
                }
                shutdown_server()   // the boot got as far as its job queue and channels
                finished |> notify_and_release
            }
            finished |> join
            panicked = failed |> get
        }
    }
    t |> equal(panicked, 1, "the boot failed instead of serving without its ASR model")
}
