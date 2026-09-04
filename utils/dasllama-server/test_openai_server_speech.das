options gen2
options persistent_heap   // the server thread loads and frees the model once; explicit deletes below
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options _dasllama_internal = true

require dastest/testing_boost public
require openai_server                                   // configure_tts / run_server (sibling)
require dasllama/dasllama_common
require daslib/jobque_boost                             // with_job_status / with_channel (server-thread lifecycle, the burst fan-out)
require daslib/json_boost                               // read_json — the /v1/stats tts block
require daslib/fio                                      // run_and_capture — the spawned speech-only boot
require daslib/module_path                              // get_this_module_dir: main.das sits beside this test
require daslib/rtti                                     // this_context().last_exception — proving configure_tts does NOT panic
require math                                            // max: the child's captured tail is clamped to its last bytes
require strings
require dasllama/dasllama_env
require dasllama/dasllama_tts                           // tts_normalize / tts_chunks — the /v1/audio/phonemes answer is compared against the facade itself

// /v1/audio/speech on a TTS-only boot (no LLM slot): a WAV answer of speech length with a RIFF
// header, the raw pcm form, the declined mp3, the missing input, the unknown voice, the TTS id
// on /v1/models, the input cap (counted in characters) and the speed range, the queue's refusal
// past its cap, the `tts` block on /v1/stats, the /v1/audio/phonemes document on the same queue -
// its `model` field, and the voice rule of a front end that phonemizes ONE language (every name
// reads in it) - and the weight lane the worker pins.
// Model-gated (kitten-nano.gguf + tts_g2p.bin + tts_postag.bin beside it) and JIT-only; skips
// cleanly otherwise. Two more model-gated tests spawn main.das: the speech-only boot, and the
// boot whose worker CANNOT load (a front-end pack from before this build), which must serve on
// with the reason in the `tts` block. The multi-language voice rule (a named voice's own dialect,
// and the refusals) is gated on kokoro-82m.gguf. The configure_tts degrade, the no-TTS 503, the
// lane normalization and the catalog's tts download arm are model-free and always run.

let TEST_PORT = 18136   // 18097/18098/18099/18101/18102/18120/18121/18131/18135 belong to the sibling suites
let SPEECHLESS_PORT = 18137
let LANE_PORT = 18138   // the f32-lane boot: a second worker, so it needs a port of its own
let BOOT_PORT = 18139   // the spawned main.das boot
let CATALOG_PORT = 18141   // 18140 belongs to the fixtures' capture_catalog rail
let KOKORO_PORT = 18142    // the multi-language boot: its own worker, so its own port
let STALE_PACK_PORT = 18143   // the spawned boot whose front-end pack predates this build
let TTS_BURST = 24      // past the server's queue cap (16): the overflow must be refused, not queued
let CHILD_TIMEOUT_SEC = 420.0   // the child parses, JITs and loads before it listens
let BOOT_POLL_ATTEMPTS = 720    // x 500 ms — inside the child's own timeout, so a hung boot still joins

def private models_dir() : string {
    return models_dir_resolved()
}
let TTS_PATH = path_join(models_dir(), "kitten-nano.gguf")
let KOKORO_PATH = path_join(models_dir(), "kokoro-82m.gguf")   // the multi-language front end: two dialects to be wrong about
let BOGUS_TTS_PATH = path_join(models_dir(), "no-such-tts-model.gguf")   // never on disk: the degrade path

struct private BurstAnswer {
    status : int
    body : string
}

struct private BootOutcome {
    rc : int
    tail : string
}

def private with_speech_server(model_path : string; port : int; lane : string; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() <| @capture(= model_path, = lane) {
            configure_tts(model_path, lane)   // the server thread is its own context: the lane pin has to be requested HERE
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

def private phonemes_request(base_url : string; body : string; blk : block<(status : int; text : string) : void>) {
    POST("{base_url}/v1/audio/phonemes", body) $(resp) {
        if (resp == null) {
            invoke(blk, 0, "")
        } else {
            invoke(blk, int(resp.status_code), string(resp.body))
        }
    }
}

def private le32(b : array<uint8>; o : int) : int {
    return int(b[o]) | (int(b[o + 1]) << 8) | (int(b[o + 2]) << 16) | (int(b[o + 3]) << 24)
}

// the `tts` member of /v1/stats — a MISS node when the boot serves no speech model, since a
// missing key reads back as a shared _null node rather than a null pointer
def private with_tts_stats(base_url : string; blk : block<(tts : JsonValue?) : void>) {
    GET("{base_url}/v1/stats") $(resp) {
        var err = ""
        var js = read_json(resp == null ? "" : string(resp.body), err)
        invoke(blk, js?["tts"])
        if (js != null) {
            unsafe {
                delete js
            }
        }
    }
}

def private json_present(v : JsonValue?) : bool {
    return v != null && !(v is _null)
}

def private json_strings(v : JsonValue?) : array<string> {
    var out : array<string>
    if (v != null && (v.value is _array)) {
        out |> reserve(length(v.value as _array))
        for (e in v.value as _array) {
            out |> push(e ?? "")
        }
    }
    return <- out
}

// one field lifted out of every object in a JSON array — the phonemes document's chunk rows
def private json_row_field(v : JsonValue?; field : string) : array<string> {
    var out : array<string>
    if (v != null && (v.value is _array)) {
        out |> reserve(length(v.value as _array))
        for (e in v.value as _array) {
            out |> push(e?[field] ?? "")
        }
    }
    return <- out
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
            // the phonemes route rides the same worker, so it refuses the same way
            phonemes_request(base_url, "\{\"input\": \"Hello.\"}") $(status, text) {
                t |> equal(status, 503, text)
                t |> success(find(text, "--tts") >= 0, "the refusal names the flag that arms the route: {text}")
            }
            // the page gates its speech studio on the key existing, so a speechless boot must not carry it
            with_tts_stats(base_url) $(tts) {
                t |> success(!json_present(tts), "a boot with no speech model carries no tts block on /v1/stats")
            }
        }
    }
}

def private catalog_download(base_url, body : string; blk : block<(status : int; text : string) : void>) {
    POST("{base_url}/catalog/download", body) $(resp) {
        if (resp == null) {
            invoke(blk, 0, "")
        } else {
            invoke(blk, int(resp.status_code), string(resp.body))
        }
    }
}

// The tts arm of POST /catalog/download - the control page's speech card addresses one FILE of
// the pinned set, not a catalog entry name. Only the refusals are exercised: an accepted start
// pulls a real file from the pinned upstream, which no test may do.
[test]
def test_catalog_tts_download_arm(t : T?) {
    t |> run("a tts start with no file, and one naming a file outside the set, refuse in words") @(t : T?) {
        with_speechless_server(t, CATALOG_PORT) $(base_url) {
            catalog_download(base_url, "\{\"tower\": \"tts\"}") $(status, text) {
                t |> equal(status, 400, text)
                t |> success(find(text, "file") >= 0, "the refusal names the field it wants: {text}")
            }
            catalog_download(base_url, "\{\"tower\": \"tts\", \"file\": \"no-such-speech-model.gguf\"}") $(status, text) {
                t |> equal(status, 409, text)
                t |> success(find(text, "text-to-speech set") >= 0, "the refusal says what the file is not in: {text}")
            }
            catalog_download(base_url, "\{\"name\": \"gemma-4-e2b\", \"tower\": \"speech\"}") $(status, text) {
                t |> equal(status, 400, text)
                t |> success(find(text, "tts") >= 0, "the unknown-tower refusal lists the tts arm: {text}")
            }
        }
    }
}

[test]
def test_tts_lane_normalizes_at_config_time(t : T?) {
    t |> run("q8 is the default and f32 is the other served lane") @(t : T?) {
        configure_tts(BOGUS_TTS_PATH)
        t |> equal(server_tts_lane(), "q8", "an unspecified lane serves q8")
        configure_tts(BOGUS_TTS_PATH, "f32")
        t |> equal(server_tts_lane(), "f32", "f32 is taken as asked")
        configure_tts(BOGUS_TTS_PATH, "Q8")
        t |> equal(server_tts_lane(), "q8", "the spelling is case-insensitive, like every other enum-ish key")
    }
    t |> run("an unknown lane is refused and the default stands") @(t : T?) {
        configure_tts(BOGUS_TTS_PATH, "int4")
        t |> equal(server_tts_lane(), "q8", "an unserved lane must not reach the worker")
        configure_tts("")
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

def private speech_model_missing(t : T?) : bool {
    if (!jit_enabled()) {
        t |> skip("JIT-only: the interpreter is too slow to serve a synthesis")
        return true
    }
    if (!stat(TTS_PATH).is_valid || !stat(path_join(models_dir(), "tts_g2p.bin")).is_valid || !stat(path_join(models_dir(), "tts_postag.bin")).is_valid) {
        t |> skip("kitten-nano.gguf or the front-end packs not present under {models_dir()}")
        return true
    }
    return false
}

// the block the control page's speech studio gates on: the worker's own answers about the model
// it loaded, beside the counters the two syntheses above moved
def private check_tts_stats_block(t : T?; base_url : string; want_lane : string) {
    with_tts_stats(base_url) $(tts) {
        t |> success(json_present(tts), "a TTS boot carries the tts block on /v1/stats")
        return if (!json_present(tts))
        t |> equal(tts?["id"] ?? "", "kitten-nano", "the block names the served TTS id")
        t |> success(tts?["ready"] ?? false, "the worker answered ready before the route did")
        t |> equal(int(tts?["pending"] ?? -1l), 0, "no synthesis is in flight")
        t |> success((tts?["done_jobs"] ?? 0l) > 0l, "the syntheses above are counted")
        t |> success(json_present(tts?["audio_s"]), "the block carries the cumulative speech seconds")
        t |> equal(int(tts?["sample_rate"] ?? 0l), 24000, "the model's rate — the same one the wav header carries")
        t |> equal(tts?["lane"] ?? "", want_lane, "the block reports the lane the worker pinned")
        // the CANONICAL names caps() declares, not the friendly aliases the route also accepts:
        // the page's voice picker offers exactly what the model calls its voices
        var inscope voices <- json_strings(tts?["voices"])
        t |> equal(length(voices), 8, "kitten-nano's eight voices")
        t |> equal(voices[0], "expr-voice-2-m", "the first is the one an omitted voice field lands on")
    }
}

// The front-end view the speech studio draws under its waveform. The expected values are the
// facade's OWN normalizer and chunker run here, so the route is proved to answer what a synthesis
// of the same text would actually say — not a transcription of today's tables.
def private check_phonemes(t : T?; base_url : string) {
    let input = "Dr. Chen paid $12."
    let want_norm = tts_normalize(input)
    // kitten-nano is the fixture, and a Kitten model's chunker appends the driver's comma to a
    // bare chunk: the rule is passed here rather than left to the default the model would pick
    var inscope want <- tts_chunks(want_norm, TTS_CHUNK_CHARS, true)
    t |> equal(want_norm, "Doctor Chen paid twelve dollars.", "the normalizer's spoken form of the probe")
    phonemes_request(base_url, "\{\"input\": \"{input}\"}") $(status, text) {
        t |> equal(status, 200, text)
        var err = ""
        var js = read_json(text, err)
        t |> success(js != null, "the answer parses as JSON: {err}")
        if (js != null) {
            t |> equal(js?["normalized"] ?? "", want_norm, "the document carries the normalized text")
            t |> equal(js?["lang"] ?? "", "en-us", "no voice named: the model's first language")
            var inscope texts <- json_row_field(js?["chunks"], "text")
            var inscope phons <- json_row_field(js?["chunks"], "phonemes")
            t |> equal(length(texts), length(want), "one row per chunk the synthesis would take")
            for (i in range(min(length(texts), length(want)))) {
                t |> equal(texts[i], want[i], "row {i} carries the chunk itself")
                t |> success(!empty(phons[i]), "row {i} carries a phoneme string")
            }
            unsafe {
                delete js
            }
        }
    }
}

// The voice rule on a model that phonemizes ONE language: there is no second dialect to be wrong
// about, so every name reads in it - the alias the speech route resolves, and a name nobody
// carries alike. The multi-language arm is the Kokoro test below.
def private check_phonemes_voice_rule(t : T?; base_url : string) {
    for (voice in ["Bella", "nobody-at-all"]) {
        phonemes_request(base_url, "\{\"input\": \"Hello.\", \"voice\": \"{voice}\"}") $(status, text) {
            t |> equal(status, 200, "'{voice}' on a one-language front end: {text}")
            var err = ""
            var js = read_json(text, err)
            t |> equal(js?["lang"] ?? "", "en-us", "the only language it phonemizes")
            if (js != null) {
                unsafe {
                    delete js
                }
            }
        }
    }
    // the cap counts CHARACTERS, the way the page's text box does: 1500 em dashes are 1500 of
    // them in 4500 bytes, well under the 4096 the route takes - a byte cap would refuse them
    let dashes = build_string() $(w) {
        for (_i in range(1500)) {
            w |> write("—")
        }
    }
    phonemes_request(base_url, "\{\"input\": \"{dashes}\"}") $(status, text) {
        t |> equal(status, 200, "1500 characters in 4500 bytes: {slice(text, 0, min(160, length(text)))}")
    }
    phonemes_request(base_url, "\{\"input\": \"Hello.\", \"model\": \"kitten-nano\"}") $(status, text) {
        t |> equal(status, 200, "the served id is accepted: {text}")
    }
    phonemes_request(base_url, "\{\"input\": \"Hello.\", \"model\": \"not-served\"}") $(status, text) {
        t |> equal(status, 404, "the phonemes route reads `model` the way the speech route does: {text}")
        t |> success(find(text, "kitten-nano") >= 0, "the refusal names the id that IS served: {text}")
    }
}

[test]
def test_speech_route(t : T?) {
    if (speech_model_missing(t)) {
        return
    }
    with_speech_server(TTS_PATH, TEST_PORT, "q8") $(base_url) {
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
        t |> run("the tts block on /v1/stats") @(t : T?) {
            check_tts_stats_block(t, base_url, "q8")
        }
        t |> run("the phonemes route answers the normalized text and a phoneme string per chunk") @(t : T?) {
            check_phonemes(t, base_url)
        }
        t |> run("a one-language front end phonemizes any voice name, and reads the model field") @(t : T?) {
            check_phonemes_voice_rule(t, base_url)
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

// A front end with TWO dialects has to be told which voice, because either answer would be a
// claim about a sound: a voice it carries reads that voice's language, and one it does not is
// refused in the speech route's own words rather than answered in the wrong accent.
def private check_multi_language_phonemes(t : T?; base_url : string) {
    var inscope langs <- { "bf_emma" => "en-gb", "af_heart" => "en-us", "bm_george" => "en-gb" }
    for (voice, want in keys(langs), values(langs)) {
        phonemes_request(base_url, "\{\"input\": \"The water tower.\", \"voice\": \"{voice}\"}") $(status, text) {
            t |> equal(status, 200, "{voice}: {text}")
            var err = ""
            var js = read_json(text, err)
            t |> equal(js?["lang"] ?? "", want, "{voice} reads {want}")
            if (js != null) {
                unsafe {
                    delete js
                }
            }
        }
    }
    // a British and an American reading of one line are different strings, or the lang field
    // above is decoration
    var gb = ""
    var us = ""
    phonemes_request(base_url, "\{\"input\": \"The water tower.\", \"voice\": \"bf_emma\"}") $(_status, text) {
        gb = clone_string(text)
    }
    phonemes_request(base_url, "\{\"input\": \"The water tower.\", \"voice\": \"af_heart\"}") $(_status, text) {
        us = clone_string(text)
    }
    t |> success(!empty(gb) && gb != us, "the two dialects answer different documents")
    phonemes_request(base_url, "\{\"input\": \"Hello.\", \"voice\": \"nobody-at-all\"}") $(status, text) {
        t |> equal(status, 400, "an unknown voice cannot be phonemized in a chosen dialect: {text}")
        t |> success(find(text, "unknown voice") >= 0, "the speech route's own refusal: {text}")
    }
    phonemes_request(base_url, "\{\"input\": \"Hello.\", \"voice\": \"jf_alpha\"}") $(status, text) {
        t |> equal(status, 400, "a pack whose language the front end lacks: {text}")
        t |> success(find(text, "speaks ja") >= 0, "the refusal names the language the pack speaks: {text}")
    }
}

[test]
def test_phonemes_on_a_multi_language_model(t : T?) {
    if (!jit_enabled()) {
        t |> skip("JIT-only: the interpreter is too slow to load a TTS model")
        return
    }
    if (!stat(KOKORO_PATH).is_valid || !stat(path_join(models_dir(), "tts_g2p.bin")).is_valid ||
        !stat(path_join(models_dir(), "tts_postag.bin")).is_valid) {
        t |> skip("kokoro-82m.gguf or the front-end packs not present under {models_dir()}")
        return
    }
    with_speech_server(KOKORO_PATH, KOKORO_PORT, "q8") $(base_url) {
        t |> run("the phonemes route reads the voice's own dialect and refuses a voice it cannot") @(t : T?) {
            check_multi_language_phonemes(t, base_url)
        }
    }
}

// the child boots from the SAME tree this test runs from: the parent's -dasroot / -module-cache
// carry over, so a worktree run never compiles main.das against the binary's tree
def private child_launch_args(server_das : string; port : int; tts_path : string; config : string) : array<string> {
    var out : array<string>
    out |> reserve(14)   // argv[0] + -jit + at most two forwarded flag pairs + the script + seven own args
    var carry = ""   // a forwarded flag, held until its value arrives on the next argv entry
    for (a in get_command_line_arguments()) {   // a LOCKED array: iterate it, never bind or delete it
        if (empty(out)) {
            out |> push(clone_string(a))   // argv[0] = the running daslang binary
            out |> push("-jit")
        } elif (!empty(carry)) {
            out |> push(clone_string(carry))
            out |> push(clone_string(a))
            carry = ""
        } elif (a == "-dasroot" || a == "-module-cache") {
            carry = clone_string(a)
        }
    }
    out |> push(clone_string(server_das))
    // --config PINS the child's configuration: main.das otherwise auto-discovers a
    // dasllama-server.toml in the cwd or beside itself, and a developer's own (which may be
    // `authoritative = true`, outranking the flags) would decide what this child serves
    for (a in ["--", "--tts", tts_path, "--port", "{port}", "--config", config]) {
        out |> push(clone_string(a))
    }
    return <- out
}

// a config carrying one harmless key: the child needs a READABLE one to be pinned to it, and
// every value that matters rides its flags
def private write_pinned_config(path : string) : bool {
    var ok = false
    fopen(path, "wb") $(f) {
        if (f != null) {
            f |> fwrite("streams = 4\n")
            ok = true
        }
    }
    return ok
}

// main.das spawned as a child on `port` with `--tts model_path`, held while `blk` runs and shut
// down after. A [tune] bootstrap exit relaunches once.
def private with_spawned_boot(t : T?; what : string; port : int; model_path : string; blk : block<(base_url : string) : void>) {
    let server_das = path_join(get_this_module_dir(), "main.das")
    if (!stat(server_das).is_valid) {
        t |> skip("main.das is not beside this test")
        return
    }
    var terr : string
    let tmp = temp_directory(terr)
    let config = path_join(empty(tmp) ? "." : tmp, "dasllama_boot_{port}_{ref_time_ticks()}.toml")
    if (!write_pinned_config(config)) {
        t |> skip("could not write the child's config at {config}")
        return
    }
    let base_url = "http://127.0.0.1:{port}"
    var ready = false
    var why = ""
    with_channel(1) $(ch) {
        with_job_status(1) $(done) {
            var inscope args <- child_launch_args(server_das, port, model_path, config)
            new_thread <| @capture(<- args) {
                var out = ""
                var rc = run_and_capture(args, out, CHILD_TIMEOUT_SEC)
                if (rc == 3 && find(out, "restart to apply") >= 0) {
                    out = ""   // [tune] bootstrap: the child adopted this box's kernels and exited with the restart code
                    rc = run_and_capture(args, out, CHILD_TIMEOUT_SEC)
                }
                ch |> push_clone(BootOutcome(rc = rc, tail = slice(out, max(0, length(out) - 600))))
                ch |> notify()
                ch |> release()
                done |> notify_and_release()
            }
            var died = false
            for (_attempt in range(BOOT_POLL_ATTEMPTS)) {
                GET("{base_url}/v1/stats") $(resp) {
                    if (resp != null && resp.status_code == http_status.OK) {
                        ready = true
                    }
                }
                break if (ready)
                died = ch |> try_pop_clone() $(o : BootOutcome#) {
                    why = "the boot exited rc={o.rc} before it listened - tail: {clone_string(o.tail)}"
                }
                break if (died)
                sleep(500u)
            }
            if (ready) {
                invoke(blk, base_url)
            }
            POST("{base_url}/shutdown", "") $(_resp) {}
            done |> join()
        }
    }
    remove(config)
    t |> success(ready, "{what} listened. {why}")
}

// A --tts boot with no --model is the ONE start shape only main.das can prove: its roster is
// empty, so every roster-indexing step of the boot has to survive that, and the in-process
// harness above never runs them (it calls configure_tts + run_server directly).
[test]
def test_speech_only_boot_through_main(t : T?) {
    if (speech_model_missing(t)) {
        return
    }
    with_spawned_boot(t, "the speech-only boot", BOOT_PORT, TTS_PATH) $(base_url) {
        t |> run("a --tts boot with no --model serves the speech route") @(t : T?) {
            speech_request(base_url, "\{\"input\": \"The speech-only boot speaks.\"}") $(status, ctype, bytes, text) {
                t |> equal(status, 200, text)
                t |> success(find(ctype, "audio/wav") >= 0, "content type {ctype}")
                t |> success(length(bytes) > 44, "{length(bytes)} bytes of wav")
            }
            check_tts_stats_block(t, base_url, "q8")
        }
    }
}

// the ten bytes load_g2p reads before it refuses: the "TG2P" magic and a version byte from before
// the British tier (test_tts_g2p.das writes the same header for its own version cell)
def private write_v1_g2p_pack(path : string) : bool {
    var ok = false
    fopen(path, "wb") $(f) {
        if (f != null) {
            var inscope bytes <- [uint8(0x54), uint8(0x47), uint8(0x32), uint8(0x50),
                uint8(1), uint8(0), uint8(0), uint8(0), uint8(0), uint8(0)]
            fwrite(f, bytes)
            ok = true
        }
    }
    return ok
}

// A directory holding the real model beside a front-end pack from before this build. The GGUF is
// copied rather than named in place, because load_tts_model reads the packs from the MODEL's own
// directory - which is exactly why an upgrade can leave a stale one there. "" back = skipped.
def private stage_stale_pack_dir(t : T?) : string {
    var terr : string
    let tmp = temp_directory(terr)
    let dir_path = path_join(empty(tmp) ? "." : tmp, "dasllama_stale_g2p_{ref_time_ticks()}")
    mkdir_rec(dir_path)
    let gguf = copy_file_result(TTS_PATH, path_join(dir_path, "kitten-nano.gguf"), true)
    let tagger = copy_file_result(path_join(models_dir(), "tts_postag.bin"), path_join(dir_path, "tts_postag.bin"), true)
    if (!(gguf is value) || !(tagger is value) || !write_v1_g2p_pack(path_join(dir_path, "tts_g2p.bin"))) {
        t |> skip("could not stage a stale front-end pack under {dir_path}")
        return ""
    }
    return dir_path
}

// everything, because the failed boot bakes its prepared image beside the copied GGUF first
def private drop_staged_dir(dir_path : string) {
    var inscope names : array<string>
    dir(dir_path) $(name) {
        if (name != "." && name != "..") {
            names |> push(clone_string(name))
        }
    }
    for (n in names) {
        remove(path_join(dir_path, n))
    }
    rmdir(dir_path)
}

// The upgrade this degrade exists for: a new binary beside a tts_g2p.bin from before it. The
// worker's load panics on the pack version - a failure no file check can see ahead of time - and
// the boot has to come up anyway, because a crash-looping boot under a watchdog never gets a page
// to fix the config from. Spawned through main.das: only the real boot path runs the fallback.
[test]
def test_boot_degrades_when_the_worker_cannot_load(t : T?) {
    if (speech_model_missing(t)) {
        return
    }
    let staged = stage_stale_pack_dir(t)
    if (empty(staged)) {
        return
    }
    with_spawned_boot(t, "the boot with a stale front-end pack", STALE_PACK_PORT, path_join(staged, "kitten-nano.gguf")) $(base_url) {
        t |> run("the speech route drops, the boot serves on, and /v1/stats says why") @(t : T?) {
            with_tts_stats(base_url) $(tts) {
                t |> success(json_present(tts), "the tts block survives the failed load")
                return if (!json_present(tts))
                t |> equal(tts?["id"] ?? "", "kitten-nano", "named by the file's stem, the way it was configured")
                t |> success(!(tts?["ready"] ?? true), "the worker never armed")
                t |> equal(int(tts?["pending"] ?? -1l), 0, "and nothing is queued behind it")
                t |> equal(length(json_strings(tts?["voices"])), 0, "no voice list to offer")
                t |> equal(tts?["lane"] ?? "", "q8", "the lane the boot asked for")
                let err = tts?["error"] ?? ""
                t |> success(!empty(err), "the block carries the reason the page draws")
                t |> success(find(err, "version 1") >= 0 && find(err, "tts_g2p.bin") >= 0,
                    "and the reason is the stale pack: {err}")
            }
            speech_request(base_url, "\{\"input\": \"Hello.\"}") $(status, _ctype, _bytes, text) {
                t |> equal(status, 503, "the route is dropped, not half-armed: {text}")
            }
            GET("{base_url}/v1/stats") $(resp) {
                let body = resp == null ? "" : string(resp.body)
                t |> success(find(body, "\"setup\":true") >= 0,
                    "with no model behind it the boot answers as setup mode, so the page can fix the config: {body}")
            }
        }
    }
    drop_staged_dir(staged)
}

[test]
def test_speech_route_on_the_f32_lane(t : T?) {
    if (speech_model_missing(t)) {
        return
    }
    with_speech_server(TTS_PATH, LANE_PORT, "f32") $(base_url) {
        t |> run("the configured lane reaches the worker and comes back on /v1/stats") @(t : T?) {
            speech_request(base_url, "\{\"input\": \"The quick brown fox.\", \"voice\": \"Bella\"}") $(status, _ctype, bytes, text) {
                t |> equal(status, 200, text)
                t |> success(length(bytes) > 44, "{length(bytes)} bytes of wav off the f32 planes")
            }
            check_tts_stats_block(t, base_url, "f32")
        }
    }
}
