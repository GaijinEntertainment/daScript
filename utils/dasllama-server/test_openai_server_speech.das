options gen2
options persistent_heap   // the server thread loads and frees the model once; explicit deletes below
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require openai_server                                   // configure_tts / run_server (sibling)
require dasllama/dasllama_common
require daslib/jobque_boost                             // with_job_status (server-thread lifecycle)
require daslib/json_boost                               // read_json / write_json
require daslib/fio
require strings
require dasllama/dasllama_env

// /v1/audio/speech on a TTS-only boot (no LLM slot): a WAV answer of speech length with a RIFF
// header, the raw pcm form, the declined mp3, the missing input, the unknown voice, the TTS id
// on /v1/models. Model-gated (kitten-nano.gguf + tts_g2p.bin + tts_postag.bin beside it) and
// JIT-only; skips cleanly otherwise.

let TEST_PORT = 18136   // 18097/18098/18099/18101/18102/18120/18121/18131/18135 belong to the sibling suites

def private models_dir() : string {
    return models_dir_resolved()
}
let TTS_PATH = path_join(models_dir(), "kitten-nano.gguf")

def private with_speech_server(port : int; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            configure_tts(TTS_PATH)
            run_server(port)   // blocks until POST /shutdown
            finished |> notify_and_release
        }
        let base_url = "http://127.0.0.1:{port}"
        var ready = false
        for (_attempt in range(600)) {   // up to ~60 s: the load + JIT
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
        assert(ready, "dasllama speech server became ready")
    }
}

def private speech_request(base_url : string; body : string; blk : block<(status : int; ctype : string; bytes : array<uint8>; text : string) : void>) {
    POST("{base_url}/v1/audio/speech", body) $(resp) {
        var none : array<uint8>
        if (resp == null) {
            invoke(blk, 0, "", none, "")
        } else {
            let bytes <- get_body_bytes(resp)
            invoke(blk, int(resp.status_code), get_header(resp, "Content-Type"), bytes, string(resp.body))
        }
    }
}

def private le32(b : array<uint8>; o : int) : int {
    return int(b[o]) | (int(b[o + 1]) << 8) | (int(b[o + 2]) << 16) | (int(b[o + 3]) << 24)
}

[test]
def test_speech_route(t : T?) {
    if (!jit_enabled()) {
        t |> skip("JIT-only: the interpreter is too slow to serve a synthesis")
        return
    }
    if (!stat(TTS_PATH).is_valid || !stat(path_join(models_dir(), "tts_g2p.bin")).is_valid || !stat(path_join(models_dir(), "tts_postag.bin")).is_valid) {
        t |> skip("kitten-nano.gguf or the front-end packs not present under {models_dir()}")
        return
    }
    with_speech_server(TEST_PORT) $(base_url) {
        t |> run("/v1/models lists the TTS id") @(t : T?) {
            GET("{base_url}/v1/models") $(resp) {
                t |> success(resp != null && resp.status_code == http_status.OK, "200")
                t |> success(find(string(resp.body), "\"kitten-nano\"") >= 0, "the id is the file's stem: {resp.body}")
            }
        }
        t |> run("a WAV of speech length") @(t : T?) {
            speech_request(base_url, "\{\"input\": \"The quick brown fox jumps over the lazy dog, twice.\", \"voice\": \"Bella\"}") $(status, ctype, bytes, text) {
                t |> equal(status, 200, text)
                t |> success(find(ctype, "audio/wav") >= 0, "content type {ctype}")
                t |> success(length(bytes) > 44 + 24000 * 2, "{length(bytes)} bytes: over a second of 16-bit 24 kHz audio")
                if (length(bytes) > 44) {
                    t |> equal(int(bytes[0]), 'R')
                    t |> equal(int(bytes[8]), 'W')
                    t |> equal(le32(bytes, 24), 24000, "sample rate")
                    t |> equal(le32(bytes, 40), length(bytes) - 44, "data chunk covers the body")
                }
            }
        }
        t |> run("the raw pcm form") @(t : T?) {
            speech_request(base_url, "\{\"input\": \"Hello there.\", \"response_format\": \"pcm\", \"speed\": 1.1}") $(status, ctype, bytes, text) {
                t |> equal(status, 200, text)
                t |> success(find(ctype, "audio/pcm") >= 0 || find(ctype, "octet-stream") >= 0, "content type {ctype}")
                t |> success(length(bytes) > 12000 && length(bytes) % 2 == 0, "{length(bytes)} bytes of 16-bit samples")
                t |> success(!(length(bytes) > 4 && int(bytes[0]) == 'R' && int(bytes[1]) == 'I'), "no RIFF header on pcm")
            }
        }
        t |> run("the declined and malformed requests") @(t : T?) {
            speech_request(base_url, "\{\"input\": \"Hello.\", \"response_format\": \"mp3\"}") $(status, _ctype, _bytes, text) {
                t |> equal(status, 400)
                t |> success(find(text, "wav") >= 0 && find(text, "pcm") >= 0, "the refusal names what is served: {text}")
            }
            speech_request(base_url, "\{\"voice\": \"Bella\"}") $(status, _ctype, _bytes, text) {
                t |> equal(status, 400)
                t |> success(find(text, "input") >= 0, text)
            }
            speech_request(base_url, "\{\"input\": \"Hello.\", \"voice\": \"nobody\"}") $(status, _ctype, _bytes, text) {
                t |> equal(status, 400)
                t |> success(find(text, "unknown voice") >= 0, text)
            }
            speech_request(base_url, "\{\"input\": \"Hello.\", \"model\": \"not-served\"}") $(status, _ctype, _bytes, _text) {
                t |> equal(status, 404)
            }
            speech_request(base_url, "not json") $(status, _ctype, _bytes, _text) {
                t |> equal(status, 400)
            }
        }
    }
}
