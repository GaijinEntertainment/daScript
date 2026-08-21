options gen2
options persistent_heap   // the server thread loads/frees a multi-GB model once; explicit deletes below
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // ggml-parity FP laxity — this is an inference tool test, not the bit-exact suite
options _dasllama_internal = true

require dastest/testing_boost public
require openai_server                                   // set_model / set_model_audio / run_server (sibling)
require dasllama/dasllama                               // load_model / QuantMode
require daslib/jobque_boost                             // with_job_status (server-thread lifecycle)
require daslib/json_boost                               // read_json
require daslib/base64                                   // input_audio.data payload
require daslib/fio
require strings
require dasllama/dasllama_env

// NATIVE AUDIO through the serving slot: input_audio part -> media worker gemma4a encode -> soft
// tokens spliced (causal) between the audio span markers -> the model transcribes/answers. ONE
// decoder, ONE mmproj (the E-series file carries vision AND audio) — no dedicated ASR model copy.
// Asserts: both arms on stats; a real transcription of the 4 s JFK clip with the family thinking
// default splitting reasoning from content; the armed-slot 400 shapes; the async decode-failure 400.
// Model-gated (E2B Q4_K_M, 2.9 GiB — inside the iteration tier) + JIT-only; skips cleanly otherwise.

let TEST_PORT = 18098

def private models_dir() : string {
    return models_dir_resolved()   // the ONE resolution: DASLLAMA_MODELS_DIR, else the rig default
}
let DEC_PATH = path_join(models_dir(), "gemma-4-E2B-it-Q4_K_M.gguf")
let MM_PATH = path_join(models_dir(), "mmproj-gemma-4-E2B-it-bf16.gguf")
let JFK_PATH = "modules/dasLLAMA/models/jfk_ask_not.wav"   // in-tree, 4 s, 16 kHz mono

def private with_audio_server(port : int; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            allow_cpu_prefill()   // planar q8 model: every prefill is CPU here, span included
            var m <- load_model(DEC_PATH, QuantMode.q8)
            set_model(m, "test-alm")
            set_model_vision("test-alm", MM_PATH)   // the SAME file arms both towers — the one-download story
            set_model_audio("test-alm", MM_PATH)
            set_default_max_tokens(2048l)   // the default-armed thought needs room before the answer
            run_server(port)   // blocks until POST /shutdown
            finished |> notify_and_release
        }
        let base_url = "http://127.0.0.1:{port}"
        var ready = false
        for (_attempt in range(1200)) {   // up to ~120s: the load + both towers + JIT
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
        assert(ready, "dasllama audio server became ready")
    }
}

// the wav's base64 (byte read: RIFF interior NULs would truncate a string read)
def private jfk_b64() : string {
    var inscope bytes : array<uint8>
    fopen(JFK_PATH, "rb") $(f) {
        if (f != null) {
            bytes |> resize(int64(fstat(f).size))
            fread(f, bytes)
        }
    }
    return empty(bytes) ? "" : base64_encode(bytes)
}

def private audio_chat_body(b64 : string; text : string) : string {
    return "\{\"messages\":[\{\"role\":\"user\",\"content\":[\{\"type\":\"input_audio\",\"input_audio\":\{\"data\":\"{b64}\",\"format\":\"wav\"}},\{\"type\":\"text\",\"text\":\"{text}\"}]}]}"
}

[test]
def test_openai_server_audio(t : T?) {   // nolint:STYLE038 — one live server session per test: every endpoint assertion runs against that one boot
    t |> run("native audio: input_audio -> gemma4a soft tokens -> a transcription (needs E2B + its mmproj)") @(t : T?) {
        if (!jit_enabled()) {
            t |> success(true, "skipped: interpreted (dasLLAMA server tests are JIT-only)")
            return
        }
        for (p in [DEC_PATH, MM_PATH, JFK_PATH]) {
            if (!stat(p).is_valid) {
                t |> success(true, "skipped: {base_name(p)} not present (set DASLLAMA_MODELS_DIR)")
                return
            }
        }
        let b64 = jfk_b64()
        t |> success(length(b64) > 1000, "the jfk wav encoded to base64 ({length(b64)} chars)")
        with_audio_server(TEST_PORT) $(base_url) {
            // both arms on the stats surface — the page gates its mic/image buttons on these
            GET("{base_url}/v1/stats") $(resp) {
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null && (js?["audio"] ?? false), "stats declares the audio arm")
                t |> success(js != null && (js?["vision"] ?? false), "stats declares the vision arm off the same mmproj")
                if (js != null) {
                    unsafe {
                        delete js
                    }
                }
            }
            // the transcription, under the family thinking DEFAULT: reasoning splits into
            // reasoning_content, and content is the clean transcript of the 4 s clip
            POST("{base_url}/v1/chat/completions", audio_chat_body(b64, "Transcribe this audio.")) $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null, "chat JSON parses")
                if (js != null) {
                    let content = to_lower(js?["choices"]?[0]?["message"]?["content"] ?? "")
                    t |> success(find(content, "fellow americans") >= 0, "the transcript hears the clip (content: {content})")
                    let reasoning = js?["choices"]?[0]?["message"]?["reasoning_content"] ?? ""
                    t |> success(!empty(reasoning), "the default-armed thought landed in reasoning_content, not the transcript")
                    t |> success(int(js?["usage"]?["prompt_tokens"] ?? 0l) > 40, "the audio soft-token rows count as prompt positions")
                    unsafe {
                        delete js
                    }
                }
            }
            // an image AND audio on the ARMED slot: still refused by shape — one media splice per request
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":[\{\"type\":\"image_url\",\"image_url\":\{\"url\":\"data:image/png;base64,AAAA\"}},\{\"type\":\"input_audio\",\"input_audio\":\{\"data\":\"{slice(b64, 0, 64)}\",\"format\":\"wav\"}}]}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
                t |> success(find(string(resp.body), "image AND audio") >= 0, "the 400 names the two-media shape")
            }
            // input_audio.data that is not base64: the synchronous 400
            POST("{base_url}/v1/chat/completions", audio_chat_body("!!!not-base64!!!", "hi")) $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
                t |> success(find(string(resp.body), "not valid base64") >= 0, "the 400 names the base64 failure")
            }
            // valid base64 of NON-audio bytes: decodes in the WORKER, so the 400 proves the async
            // error path (park -> media worker decode failure -> drain answers the parked wire)
            var junk : array<uint8>
            junk |> resize(64)
            for (i in range(64)) {
                junk[i] = uint8(i)
            }
            POST("{base_url}/v1/chat/completions", audio_chat_body(base64_encode(junk), "hi")) $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
                t |> success(find(string(resp.body), "did not decode") >= 0, "the async decode failure reaches the client as a named 400")
            }
            delete junk
            // live unload of the towers-armed slot: the async drop job frees its encoders in the
            // media worker (name-keyed tables, no bounce), and the tower-less new default 400s audio
            let tiny = to_generic_path(path_join(models_dir(), "tinyllama-1.1b-chat-v1.0.Q8_0.gguf"))
            if (stat(tiny).is_valid) {
                POST("{base_url}/v1/models/load", "\{\"path\":\"{tiny}\",\"id\":\"tiny-live\",\"ctx\":512}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.OK), "a second slot live-loads beside the armed one")
                }
                POST("{base_url}/v1/models/unload", "\{\"model\":\"test-alm\"}") $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.OK), "the towers-armed slot unloads once tiny-live is default")
                }
                POST("{base_url}/v1/chat/completions", audio_chat_body(slice(b64, 0, 64), "hi")) $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST), "audio on the tower-less default is a 400")
                    t |> success(find(string(resp.body), "no audio arm") >= 0, "the 400 names the missing arm")
                }
            } else {
                t |> success(true, "skipped the live-unload cells: tinyllama not present")
            }
        }
    }
}
