options gen2
options persistent_heap   // the server thread loads/frees a multi-GB model once; explicit deletes below
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)
options fast_math = true  // ggml-parity FP laxity — this is an inference tool test, not the bit-exact suite
options _dasllama_internal = true

require dastest/testing_boost public
require openai_server                                   // set_model / set_model_audio / run_server (sibling)
require _server_rig                                     // file_base64 (sibling)
require dasllama/dasllama                               // load_model / QuantMode
require daslib/jobque_boost                             // with_job_status (server-thread lifecycle)
require daslib/json_boost                               // read_json
require daslib/base64                                   // input_audio.data payload
require daslib/fio
require strings
require daslib/strings_boost                            // count / join

// NATIVE AUDIO through the serving slot: input_audio part -> media worker gemma4a encode -> soft
// tokens spliced (causal) between the audio span markers -> the model transcribes/answers. ONE
// decoder, ONE mmproj (the E-series file carries vision AND audio) — no dedicated ASR model copy.
// Asserts: both arms on stats; a real transcription of the 4 s JFK clip with the family thinking
// default splitting reasoning from content; the armed-slot 400 shapes; the async decode-failure 400.
// Model-gated (E2B Q4_K_M, 2.9 GiB — inside the iteration tier) + JIT-only; skips cleanly otherwise.

let TEST_PORT = 18098

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
        let ready = serve_clients(port, 1200, "/v1/models", blk)   // up to ~120s: the load + both towers + JIT
        finished |> join
        assert(ready, "dasllama audio server became ready")
    }
}

// the wav's base64 (byte read: RIFF interior NULs would truncate a string read)
def private jfk_b64() : string => file_base64(JFK_PATH)

def private push_le16(var b : array<uint8>; v : int) {
    b |> push(uint8(v & 0xFF))
    b |> push(uint8((v >> 8) & 0xFF))
}

def private push_le32(var b : array<uint8>; v : int) {
    push_le16(b, v & 0xFFFF)
    push_le16(b, (v >> 16) & 0xFFFF)
}

def private push_ascii(var b : array<uint8>; s : string) {
    for (c in s) {
        b |> push(uint8(c))   // nolint:PERF006 — tiny_wav_b64 reserves the whole wav before any push
    }
}

// a valid PCM16 mono 16 kHz wav of n zero samples — decodes non-empty, sub-frame for the encoder
def private tiny_wav_b64(n_samples : int) : string {
    let data_bytes = n_samples * 2
    var b : array<uint8>
    b |> reserve(44 + data_bytes)
    push_ascii(b, "RIFF")
    push_le32(b, 36 + data_bytes)
    push_ascii(b, "WAVEfmt ")
    push_le32(b, 16)         // fmt chunk size
    push_le16(b, 1)          // PCM
    push_le16(b, 1)          // mono
    push_le32(b, 16000)      // sample rate
    push_le32(b, 16000 * 2)  // byte rate
    push_le16(b, 2)          // block align
    push_le16(b, 16)         // bits per sample
    push_ascii(b, "data")
    push_le32(b, data_bytes)
    for (_i in range(data_bytes)) {
        b |> push(uint8(0))
    }
    let out = base64_encode(b)
    delete b
    return out
}

def private audio_chat_body(b64 : string; text : string) : string =>
    "\{\"messages\":[\{\"role\":\"user\",\"content\":[\{\"type\":\"input_audio\",\"input_audio\":\{\"data\":\"{b64}\",\"format\":\"wav\"}},\{\"type\":\"text\",\"text\":\"{text}\"}]}]}"

//! what a buffered chat reply says: its prompt and cached position counts, its finish reason, its content
struct private ChatReply {
    prompt : int
    cached : int
    finish : string
    content : string
}

def private usage_of(t : T?; body : string; tag : string) : ChatReply {
    var err : string
    var js = read_json(body, err)
    t |> success(js != null, "{tag}: chat JSON parses")
    return ChatReply() if (js == null)
    let res = ChatReply(prompt = int(js?["usage"]?["prompt_tokens"] ?? 0l),
                        cached = int(js?["usage"]?["prompt_tokens_details"]?["cached_tokens"] ?? 0l),
                        finish = js?["choices"]?[0]?["finish_reason"] ?? "",
                        content = js?["choices"]?[0]?["message"]?["content"] ?? "")
    to_log(LOG_INFO, "{tag}: prompt {res.prompt}, cached {res.cached}, finish {res.finish}, reply '{res.content}'\n")
    unsafe {
        delete js
    }
    return res
}

[test]
def test_openai_server_audio(t : T?) {   // nolint:STYLE038 — one live server session per test: every endpoint assertion runs against that one boot
    t |> run("native audio: input_audio -> gemma4a soft tokens -> a transcription (needs E2B + its mmproj)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA server tests are JIT-only)")
            return
        }
        for (p in [DEC_PATH, MM_PATH, JFK_PATH]) {
            if (!stat(p).is_valid) {
                t |> skip("{base_name(p)} not present (set DASLLAMA_MODELS_DIR)")
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
            // an image AND audio on the ARMED slot: both parts reach the worker, and the bytes that are no image answer the 400
            POST("{base_url}/v1/chat/completions",
                 "\{\"messages\":[\{\"role\":\"user\",\"content\":[\{\"type\":\"image_url\",\"image_url\":\{\"url\":\"data:image/png;base64,AAAA\"}},\{\"type\":\"input_audio\",\"input_audio\":\{\"data\":\"{slice(b64, 0, 64)}\",\"format\":\"wav\"}}]}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST))
                t |> success(find(string(resp.body), "did not decode") >= 0 || find(string(resp.body), "decode") >= 0, "the 400 names the part that does not decode")
            }
            // the clip's cost is paid once: asked again, and continued with the clip in the history, the request attaches
            // the clip's span off the prefix cache and never reaches the tower
            let ask = "\{\"max_tokens\":8,\"messages\":[\{\"role\":\"user\",\"content\":[\{\"type\":\"input_audio\",\"input_audio\":\{\"data\":\"{b64}\",\"format\":\"wav\"}},\{\"type\":\"text\",\"text\":\"Transcribe this audio.\"}]}"
            var first_prompt = 0
            POST("{base_url}/v1/chat/completions", "{ask}]}") $(resp) {
                let u = usage_of(t, string(resp.body), "the clip asked again")
                first_prompt = u.prompt
                t |> success(u.cached >= u.prompt - 1 && u.prompt > 40, "asked again, all but the last position attaches ({u.cached} of {u.prompt})")
            }
            POST("{base_url}/v1/chat/completions", "{ask},\{\"role\":\"assistant\",\"content\":\"ok\"},\{\"role\":\"user\",\"content\":\"Thanks.\"}]}") $(resp) {
                let u = usage_of(t, string(resp.body), "the chat continued")
                t |> success(u.prompt > first_prompt, "the continued chat keeps the clip in its transcript ({u.prompt} > {first_prompt} positions)")
                t |> success(u.cached > 40 && u.cached <= first_prompt, "and attaches the first turn, the clip's span in it ({u.cached} of the first turn's {first_prompt})")
            }
            GET("{base_url}/v1/stats") $(resp) {
                t |> success(find(string(resp.body), "media") >= 0, "stats still answers after the media turns")
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
            // a sub-frame clip serves without a panic or decode failure; the >= 1-token padding
            // contract itself is pinned by test_audio_embedder's encode cell
            POST("{base_url}/v1/chat/completions", audio_chat_body(tiny_wav_b64(320), "hi")) $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK), "a sub-frame clip pads to a soft token and serves")
            }
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

// A WHISPER-CLASS tower on a stock decoder: the ultravox mmproj arms Llama-3.2-1B's audio through the audio flag alone
// (no vision tower in the file), its rows splice with no span marker, after the text the message puts ahead of them.

let TOWER_PORT = 18177
let UV_DEC_PATH = path_join(models_dir(), "Llama-3.2-1B-Instruct-Q8_0.gguf")
let UV_MM_PATH = path_join(models_dir(), "mmproj-ultravox-v0_5-llama-3_2-1b-f16.gguf")
let UV_ROWS = 187   // a chunk's soft tokens: 1500 encoder positions over the stack of 8

//! `cover_probe` false sends every known part to the scheduler rowless (`set_media_cover_probe`)
def private with_tower_audio_server(port : int; cover_probe : bool; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            allow_cpu_prefill()
            var m <- load_model(UV_DEC_PATH, QuantMode.q8)
            set_model(m, "test-uv")
            set_model_audio("test-uv", UV_MM_PATH)
            set_default_max_tokens(48l)
            set_media_cover_probe(cover_probe)
            run_server(port)
            finished |> notify_and_release
        }
        let ready = serve_clients(port, 1200, "/v1/models", blk)
        finished |> join
        assert(ready, "dasllama tower-audio server became ready")
    }
}

def private prompt_tokens_of(t : T?; body : string; tag : string) : int {
    let reply = usage_of(t, body, tag)
    t |> success(!empty(reply.content), "{tag}: the turn answers")
    return reply.prompt
}

//! the scheduler's streams admitted and the media rows prefilled since boot, off /v1/stats
def private stats_counts(t : T?; base_url : string) : tuple<admitted : int64; media_rows : int64> {
    var res = (admitted = -1l, media_rows = -1l)
    GET("{base_url}/v1/stats") $(resp) {
        var err : string
        var js = read_json(string(resp.body), err)
        t |> success(js != null, "stats JSON parses")
        if (js != null) {
            res = (admitted = js?["admitted"] ?? -1l, media_rows = js?["media_rows"] ?? -1l)
            unsafe {
                delete js
            }
        }
    }
    return res
}

//! a greedy chat under the system line `system`: the clip then `text` in one user message, or `text` alone for an empty `b64`
def private system_chat_body(system, b64, text : string; stream : bool) : string {
    let audio = empty(b64) ? "" : "\{\"type\":\"input_audio\",\"input_audio\":\{\"data\":\"{b64}\",\"format\":\"wav\"}},"
    return "\{\"temperature\":0,\"stream\":{stream},\"messages\":[\{\"role\":\"system\",\"content\":\"{system}\"},\{\"role\":\"user\",\"content\":[{audio}\{\"type\":\"text\",\"text\":\"{text}\"}]}]}"
}

//! one ask of the clip under `system`: the reply, and how far the admitted-stream and media-row counters moved for it
def private ask_counted(t : T?; base_url, system, b64, text : string; tag : string) : tuple<reply : ChatReply; admitted : int64; rows : int64> {
    let before = stats_counts(t, base_url)
    var reply = ChatReply()
    POST("{base_url}/v1/chat/completions", system_chat_body(system, b64, text, false)) $(resp) {
        t |> equal(int(resp.status_code), int(http_status.OK), "{tag}: {resp.body}")
        reply = usage_of(t, string(resp.body), tag)
    }
    let after = stats_counts(t, base_url)
    return (reply = reply, admitted = after.admitted - before.admitted, rows = after.media_rows - before.media_rows)
}

//! the prompt positions `text` alone takes under `system`
def private text_prompt(t : T?; base_url, system, text : string) : int {
    var n = 0
    POST("{base_url}/v1/chat/completions", system_chat_body(system, "", text, false)) $(resp) {
        t |> equal(int(resp.status_code), int(http_status.OK))
        n = prompt_tokens_of(t, string(resp.body), "text alone under '{system}'")
    }
    return n
}

[test]
def test_openai_server_audio_tower(t : T?) {
    t |> run("an ultravox mmproj arms a stock decoder's audio: bare rows after the text (needs Llama-3.2-1B + the ultravox mmproj)") @(t : T?) {
        if (!jit_enabled()) {
            t |> skip("interpreted (dasLLAMA server tests are JIT-only)")
            return
        }
        for (p in [UV_DEC_PATH, UV_MM_PATH, JFK_PATH]) {
            if (!stat(p).is_valid) {
                t |> skip("{base_name(p)} not present (set DASLLAMA_MODELS_DIR)")
                return
            }
        }
        let b64 = jfk_b64()
        let text = "Transcribe this audio."
        with_tower_audio_server(TOWER_PORT, true) $(base_url) {
            GET("{base_url}/v1/stats") $(resp) {
                var err : string
                var js = read_json(string(resp.body), err)
                t |> success(js != null && (js?["audio"] ?? false), "stats declares the audio arm")
                t |> success(js != null && !(js?["vision"] ?? true), "and no vision arm: the file carries none")
                if (js != null) {
                    unsafe {
                        delete js
                    }
                }
            }
            var plain = 0
            POST("{base_url}/v1/chat/completions", "\{\"temperature\":0,\"messages\":[\{\"role\":\"user\",\"content\":\"{text}\"}]}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                plain = prompt_tokens_of(t, string(resp.body), "text alone")
            }
            let text_first = "\{\"temperature\":0,\"messages\":[\{\"role\":\"user\",\"content\":[\{\"type\":\"text\",\"text\":\"{text}\"},\{\"type\":\"input_audio\",\"input_audio\":\{\"data\":\"{b64}\",\"format\":\"wav\"}}]}]}"
            POST("{base_url}/v1/chat/completions", text_first) $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                t |> equal(prompt_tokens_of(t, string(resp.body), "text, then audio"), plain + UV_ROWS, "the clip adds its rows and no marker token")
            }
            POST("{base_url}/v1/chat/completions", audio_chat_body(b64, text)) $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                t |> equal(prompt_tokens_of(t, string(resp.body), "audio, then text"), plain + UV_ROWS, "the same count with the clip ahead of the text")
            }
            check_known_clip_reencoded(t, base_url, b64, text)
            check_media_parts_limit(t, base_url)
            check_assistant_media_dropped(t, base_url, text)
        }
    }
}

//! The clip is known, but no cached prefix runs under a new system line to its span: the probe sends it to the encoder
//! before the submit, so one stream is admitted, carrying the clip's rows. Asked again, the cache holds the span.
def private check_known_clip_reencoded(t : T?; base_url, b64, text : string) {
    let sys_plain = text_prompt(t, base_url, SYS_A, text)
    let moved = ask_counted(t, base_url, SYS_A, b64, text, "a known clip under a new system line")
    t |> equal(moved.reply.prompt, sys_plain + UV_ROWS, "the clip adds its rows under the new system line")
    t |> equal(moved.admitted, 1l, "one stream admitted: the uncovered span was encoded before the submit")
    t |> equal(moved.rows, int64(UV_ROWS), "and it brought the clip's rows")
    t |> success(moved.reply.finish != "media_lost", "the reply finishes '{moved.reply.finish}'")
    let again = ask_counted(t, base_url, SYS_A, b64, text, "the same ask again")
    t |> equal(again.admitted, 1l, "one stream admitted")
    t |> equal(again.rows, 0l, "the cached span brings no rows")
    t |> equal(again.reply.prompt, moved.reply.prompt, "the same prompt")
}

//! A request's media parts are capped: one past the limit is refused by name before any part decodes; at the limit
//! the parts decode, and the first one's bad base64 answers.
def private check_media_parts_limit(t : T?; base_url : string) {
    for (n in [MEDIA_SPANS_LIMIT, MEDIA_SPANS_LIMIT + 1]) {
        let parts = join([for (_i in range(n)); "\{\"type\":\"input_audio\",\"input_audio\":\{\"data\":\"!!!not-base64!!!\",\"format\":\"wav\"}}"], ",")
        POST("{base_url}/v1/chat/completions", "\{\"messages\":[\{\"role\":\"user\",\"content\":[{parts}]}]}") $(resp) {
            let body = string(resp.body)
            t |> equal(int(resp.status_code), int(http_status.BAD_REQUEST), "{n} parts: {body}")
            let names_limit = find(body, "carries {n} media parts; the limit is {MEDIA_SPANS_LIMIT}") >= 0
            if (n > MEDIA_SPANS_LIMIT) {
                t |> success(names_limit, "{n} parts: the 400 names the limit ({body})")
            } else {
                t |> success(!names_limit && find(body, "not valid base64") >= 0, "{n} parts are within the limit ({body})")
            }
        }
    }
}

//! Media rides user messages: an image on an assistant turn - this slot has no vision arm - is dropped, and the
//! request answers as its text alone.
def private check_assistant_media_dropped(t : T?; base_url, text : string) {
    let turns_head = "\{\"temperature\":0,\"messages\":[\{\"role\":\"user\",\"content\":\"Hi.\"},\{\"role\":\"assistant\",\"content\":["
    let turns_tail = "\{\"type\":\"text\",\"text\":\"Hello.\"}]},\{\"role\":\"user\",\"content\":\"{text}\"}]}"
    var text_turns = 0
    POST("{base_url}/v1/chat/completions", "{turns_head}{turns_tail}") $(resp) {
        t |> equal(int(resp.status_code), int(http_status.OK))
        text_turns = prompt_tokens_of(t, string(resp.body), "the turns as text")
    }
    POST("{base_url}/v1/chat/completions", "{turns_head}\{\"type\":\"image_url\",\"image_url\":\{\"url\":\"data:image/png;base64,AAAA\"}},{turns_tail}") $(resp) {
        t |> equal(int(resp.status_code), int(http_status.OK), "an assistant turn's image is no vision request: {resp.body}")
        t |> equal(prompt_tokens_of(t, string(resp.body), "an image on the assistant turn"), text_turns, "and the prompt is the text turns alone")
    }
}

let SYS_A = "You are a careful listener."
let SYS_B = "You answer in one short sentence."
let SYS_C = "You are brief."

// A known clip under a system line the prefix cache holds no span for, sent to the scheduler rowless: the stream finishes
// `media_lost` on admission, and the server encodes the clip and submits once more - the client sees one answer.
let LOST_PORT = 18178

[test]
def test_openai_server_audio_lost_span(t : T?) {
    t |> run("a span the cache does not hold is encoded and resubmitted (needs Llama-3.2-1B + the ultravox mmproj)") @(t : T?) {
        var inscope files <- [UV_DEC_PATH, UV_MM_PATH, JFK_PATH]
        return if (model_missing(t, files))
        let b64 = jfk_b64()
        let text = "Transcribe this audio."
        with_tower_audio_server(LOST_PORT, false) $(base_url) {
            let cold = ask_counted(t, base_url, SYS_A, b64, text, "the clip, new to the slot")
            t |> equal(cold.admitted, 1l, "a new clip encodes before the submit: one stream")
            t |> equal(cold.rows, int64(UV_ROWS), "carrying the clip's rows")
            let held = ask_counted(t, base_url, SYS_A, b64, text, "the same ask: the cache holds the span")
            t |> equal(held.admitted, 1l, "a span the cache holds is admitted once")
            t |> equal(held.rows, 0l, "with no rows")
            let lost = ask_counted(t, base_url, SYS_B, b64, text, "the clip under a new system line")
            t |> success(lost.reply.finish != "media_lost" && !empty(lost.reply.finish), "the reply finishes '{lost.reply.finish}', never media_lost")
            t |> success(!empty(lost.reply.content), "and answers")
            t |> equal(lost.admitted, 2l, "two streams admitted: the rowless one lost its span, the resubmission carried the rows")
            t |> equal(lost.rows, int64(UV_ROWS), "the resubmission brought the clip's rows")
            let sys_plain = text_prompt(t, base_url, SYS_B, text)
            t |> equal(lost.reply.prompt, sys_plain + UV_ROWS, "the prompt a cold ask of the clip takes")
            // streamed, the retry is invisible on the wire: one opening role delta, one finish, one [DONE]
            let before = stats_counts(t, base_url)
            POST("{base_url}/v1/chat/completions", system_chat_body(SYS_C, b64, text, true)) $(resp) {
                let body = string(resp.body)
                t |> equal(int(resp.status_code), int(http_status.OK))
                t |> equal(count(body, "\"role\":\"assistant\""), 1, "one role delta across the retry: {body}")
                t |> equal(count(body, "[DONE]"), 1, "one [DONE]")
                t |> equal(count(body, "media_lost"), 0, "no media_lost on the wire")
                t |> success(count(body, "\"finish_reason\":\"stop\"") + count(body, "\"finish_reason\":\"length\"") == 1, "one finish chunk")
            }
            t |> equal(stats_counts(t, base_url).admitted - before.admitted, 2l, "the streamed ask took the retry too")
        }
    }
}

// POST /v1/models/load arms a runtime-loaded slot's audio from the body's `audio_mmproj`; a file that arms nothing serves
// the model as text and says why in `tower_note`.
let LOAD_PORT = 18183
let SMOL_PATH = path_join(models_dir(), "SmolLM2-135M-Instruct-Q8_0.gguf")

def private with_text_server(port : int; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            allow_cpu_prefill()
            var m <- load_model(SMOL_PATH, QuantMode.q8)
            set_model(m, "test-smol")
            set_default_max_tokens(16l)
            run_server(port)
            finished |> notify_and_release
        }
        let ready = serve_clients(port, 1200, "/v1/models", blk)
        finished |> join
        assert(ready, "dasllama text server became ready")
    }
}

//! /v1/stats' default-slot audio arm, and the arm of the slot named `id`
def private audio_arms(t : T?; base_url, id : string) : tuple<default_arm : bool; slot_arm : bool> {
    var res = (default_arm = false, slot_arm = false)
    GET("{base_url}/v1/stats") $(resp) {
        var err : string
        var js = read_json(string(resp.body), err)
        t |> success(js != null, "stats JSON parses")
        if (js != null) {
            res.default_arm = js?["audio"] ?? false
            for (mv in js?["models"].value as _array) {
                if ((mv?["name"] ?? "") == id) {
                    res.slot_arm = mv?["audio"] ?? false
                }
            }
            unsafe {
                delete js
            }
        }
    }
    return res
}

[test]
def test_openai_server_audio_runtime_load(t : T?) {
    t |> run("a runtime load arms audio from audio_mmproj (needs SmolLM2-135M, Llama-3.2-1B + the ultravox mmproj)") @(t : T?) {
        var inscope files <- [SMOL_PATH, UV_DEC_PATH, UV_MM_PATH, JFK_PATH]
        return if (model_missing(t, files))
        let dec = to_generic_path(UV_DEC_PATH)
        let missing = to_generic_path(path_join(test_temp_dir(), "no-such-mmproj.gguf"))
        with_text_server(LOAD_PORT) $(base_url) {
            t |> success(!audio_arms(t, base_url, "test-smol").default_arm, "the boot slot has no audio arm")
            POST("{base_url}/v1/models/load", "\{\"path\":\"{dec}\",\"id\":\"uv-bad\",\"ctx\":2048,\"activate\":false,\"audio_mmproj\":\"{missing}\"}") $(resp) {
                let body = string(resp.body)
                t |> equal(int(resp.status_code), int(http_status.OK), "the model serves without the arm: {body}")
                var err : string
                var js = read_json(body, err)
                t |> success(!empty(js?["tower_note"] ?? ""), "and tower_note says why: {body}")
                unsafe {
                    delete js
                }
            }
            t |> success(!audio_arms(t, base_url, "uv-bad").slot_arm, "the slot a missing mmproj names has no audio arm")
            POST("{base_url}/v1/models/unload", "\{\"model\":\"uv-bad\"}") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
            }
            POST("{base_url}/v1/models/load", "\{\"path\":\"{dec}\",\"id\":\"uv-live\",\"ctx\":2048,\"audio_mmproj\":\"{to_generic_path(UV_MM_PATH)}\"}") $(resp) {
                let body = string(resp.body)
                t |> equal(int(resp.status_code), int(http_status.OK), body)
                var err : string
                var js = read_json(body, err)
                t |> equal(js?["tower_note"] ?? "?", "", "the arm took: {body}")
                unsafe {
                    delete js
                }
            }
            let arms = audio_arms(t, base_url, "uv-live")
            t |> success(arms.slot_arm, "the loaded slot declares the audio arm")
            t |> success(arms.default_arm, "and, made the default, so does the server")
            POST("{base_url}/v1/chat/completions", audio_chat_body(jfk_b64(), "Transcribe this audio.")) $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK), "the armed slot takes input_audio: {resp.body}")
                t |> success(prompt_tokens_of(t, string(resp.body), "the clip on the runtime-armed slot") > UV_ROWS, "the clip's rows are in the prompt")
            }
        }
    }
}
