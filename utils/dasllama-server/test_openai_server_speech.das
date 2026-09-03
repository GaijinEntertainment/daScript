options gen2
options persistent_heap   // the server thread loads and frees the model once; explicit deletes below
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require openai_server                                   // configure_tts / run_server (sibling)
require dasllama/dasllama_common
require daslib/jobque_boost                             // with_job_status / with_channel (server-thread lifecycle, the burst fan-out)
require daslib/fio
require daslib/rtti                                     // this_context().last_exception — proving configure_tts does NOT panic
require strings
require dasllama/dasllama_env

// /v1/audio/speech on a TTS-only boot (no LLM slot): a WAV answer of speech length with a RIFF
// header, the raw pcm form, the declined mp3, the missing input, the unknown voice, the TTS id
// on /v1/models, the input cap and the speed range, and the queue's refusal past its cap.
// Model-gated (kitten-nano.gguf + tts_g2p.bin + tts_postag.bin beside it) and JIT-only; skips
// cleanly otherwise. The boot degrade and the no-TTS 503 are model-free and always run.

let TEST_PORT = 18136   // 18097/18098/18099/18101/18102/18120/18121/18131/18135 belong to the sibling suites
let SPEECHLESS_PORT = 18137
let TTS_BURST = 24      // past the server's queue cap (16): the overflow must be refused, not queued

def private models_dir() : string {
    return models_dir_resolved()
}
let TTS_PATH = path_join(models_dir(), "kitten-nano.gguf")
let BOGUS_TTS_PATH = path_join(models_dir(), "no-such-tts-model.gguf")   // never on disk: the degrade path

struct private BurstAnswer {
    status : int
    body : string
}

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

// a slotless boot whose --tts names a file set that is not there: configure_tts drops the route
// instead of panicking the boot, so this doubles as the TTS-less server (the speech route is
// registered on every server, and it is the one place the not-ready arm is reachable without a
// model) and as the degraded one - a panic here would strand the join below
def private with_speechless_server(t : T?; port : int; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            configure_tts(BOGUS_TTS_PATH)
            set_setup_mode(true)
            run_server(port)   // blocks until POST /shutdown
            finished |> notify_and_release
        }
        let base_url = "http://127.0.0.1:{port}"
        var ready = false
        for (_attempt in range(300)) {
            GET("{base_url}/v1/stats") $(resp) {
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
        t |> success(ready, "the speechless server became ready")
    }
}

def private panic_text_of(blk : block<() : void>) : string {
    var why = ""
    try {
        invoke(blk)
    } recover {
        why = this_context().last_exception |> rtrim
    }
    return why
}

[test]
def test_configure_tts_degrades_a_boot_it_cannot_serve(t : T?) {
    t |> run("a missing model file drops the route instead of panicking") @(t : T?) {
        t |> equal(tts_missing_file(BOGUS_TTS_PATH), BOGUS_TTS_PATH, "the probe names the missing model")
        t |> success(!tts_files_present(BOGUS_TTS_PATH), "the boot's setup-mode probe reads unservable")
        let why = panic_text_of() {
            configure_tts(BOGUS_TTS_PATH)
        }
        t |> equal(why, "", "configure_tts panicked instead of degrading")
        t |> success(!server_has_tts(), "the speech route is dropped, not armed")
        configure_tts("")
    }
    t |> run("a missing front-end pack drops the route instead of panicking") @(t : T?) {
        var terr : string
        let tmp = temp_directory(terr)
        let dir = path_join(empty(tmp) ? "." : tmp, "dasllama_speech_packless_{ref_time_ticks()}")
        mkdir_rec(dir)
        let packless = path_join(dir, "packless.gguf")
        fopen(packless, "wb") $(f) {
            if (f != null) {
                f |> fwrite("GGUF")
            }
        }
        let missing = tts_missing_file(packless)
        t |> success(find(missing, "tts_g2p.bin") >= 0, "the probe names the missing pack: {missing}")
        t |> success(!tts_files_present(packless), "the boot's setup-mode probe reads unservable")
        let why = panic_text_of() {
            configure_tts(packless)
        }
        t |> equal(why, "", "configure_tts panicked instead of degrading")
        t |> success(!server_has_tts(), "the speech route is dropped, not armed")
        configure_tts("")
        remove(packless)
        rmdir(dir)
    }
}

[test]
def test_speech_route_without_a_model(t : T?) {
    t |> run("a boot whose TTS file set is missing serves on and refuses the speech route") @(t : T?) {
        with_speechless_server(t, SPEECHLESS_PORT) $(base_url) {
            speech_request(base_url, "\{\"input\": \"Hello.\"}") $(status, _ctype, _bytes, text) {
                t |> equal(status, 503, text)
                t |> success(find(text, "--tts") >= 0, "the refusal names the flag that arms the route: {text}")
            }
        }
    }
}

def private check_input_cap_and_speed_range(t : T?; base_url : string) {
    let over_cap = build_string() $(w) {
        for (_i in range(120)) {   // 120 * 44 = 5280 characters, past the 4096 cap
            w |> write("the quick brown fox jumps over the lazy dog. ")
        }
    }
    speech_request(base_url, "\{\"input\": \"{over_cap}\"}") $(status, _ctype, _bytes, text) {
        t |> equal(status, 400, text)
        t |> success(find(text, "4096") >= 0, "the refusal names the cap: {text}")
    }
    for (bad_speed in [4.5, 0.1]) {
        speech_request(base_url, "\{\"input\": \"Hello.\", \"speed\": {bad_speed}}") $(status, _ctype, _bytes, text) {
            t |> equal(status, 400, text)
            t |> success(find(text, "speed") >= 0, "the refusal names the knob: {text}")
        }
    }
    speech_request(base_url, "\{\"input\": \"Hello.\", \"speed\": 4.0}") $(status, _ctype, _bytes, text) {
        t |> equal(status, 200, "4.0 is the top of the range, not past it: {text}")
    }
}

// one burst of concurrent requests, each longer than the worker can finish while they arrive:
// the first TTS_QUEUE_LIMIT queue, the rest must come back 503 rather than pile up
def private check_queue_cap(t : T?; base_url : string) {
    let url = "{base_url}/v1/audio/speech"
    let body = build_string() $(w) {
        w |> write("\{\"input\": \"")
        for (_i in range(3)) {   // long enough that no synthesis finishes while the burst arrives
            w |> write("the quick brown fox jumps over the lazy dog. ")
        }
        w |> write("\"}")
    }
    var inscope answers : array<BurstAnswer>
    answers |> reserve(TTS_BURST)
    with_channel(TTS_BURST) $(ch) {
        with_job_status(TTS_BURST) $(done) {
            for (_i in range(TTS_BURST)) {
                new_thread <| @capture(= url, = body) {
                    var status = 0
                    var text = ""
                    POST(url, body) $(resp) {
                        if (resp != null) {
                            status = int(resp.status_code)
                            text := string(resp.body)
                        }
                    }
                    ch |> push_clone(BurstAnswer(status = status, body = text))
                    ch |> notify()
                    ch |> release()
                    done |> notify_and_release()
                }
            }
            for (a in each_clone(ch, type<BurstAnswer>)) {
                answers |> push(BurstAnswer(status = a.status, body = clone_string(a.body)))
            }
            done |> join()
        }
    }
    var refused = 0
    var served = 0
    var other = 0
    var refusal_names_cap = true
    for (a in answers) {
        if (a.status == 503) {
            refused++
            refusal_names_cap = refusal_names_cap && find(a.body, "capacity") >= 0
        } elif (a.status == 200) {
            served++
        } else {
            other++
        }
    }
    t |> equal(length(answers), TTS_BURST, "every burst request answered")
    t |> equal(other, 0, "a burst answer was neither served nor refused")
    t |> success(refused > 0, "{refused} of {TTS_BURST} refused, {served} served - the cap must reject the overflow")
    t |> success(refusal_names_cap, "every refusal names the queue cap")
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
        t |> run("the input cap and the speed range") @(t : T?) {
            check_input_cap_and_speed_range(t, base_url)
        }
        t |> run("the queue refuses past its cap") @(t : T?) {
            check_queue_cap(t, base_url)
        }
    }
}
