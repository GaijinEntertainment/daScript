options gen2
options persistent_heap
options gc

require dastest/testing_boost public
require daslib/jobque_boost
require daslib/fio
require strings
require audio/audio_wav   // write_wav: the speech fake's answer
require openai/openai_vision   // image_file_data_uri
require math
require daslib/strings_boost   // count
require ../harness/_row_stats.das
require ../harness/served_bench.das
require ../../../tests/dasHV/_dashv_test_common.das   // with_test_server: a server on its own thread and port

// The served-turn instrument (harness/served_bench.das), with no model behind it: the stream reader's two clock marks
// and its usage read over hand-built SSE lines, the system prompt's nonce and size, the corpus filter, the row
// statistic, and whole turns against a fake chat server that answers by the request's "model" field - a complete
// stream, one with no usage chunk, one with no reply text, one of a single token, a failed request.

let ROLE = "data: \{\"choices\":[\{\"index\":0,\"delta\":\{\"role\":\"assistant\"}}]}"
let USAGE = "data: \{\"choices\":[],\"usage\":\{\"prompt_tokens\":100,\"completion_tokens\":3,\"total_tokens\":103,\"prompt_tokens_details\":\{\"cached_tokens\":40}},\"timings\":\{\"ttft_ms\":12.5,\"gen_ms\":20}}"

let SEEN = "\{\"id\":\"c\",\"object\":\"chat.completion\",\"created\":0,\"model\":\"m\",\"choices\":[\{\"index\":0,\"message\":\{\"role\":\"assistant\",\"content\":\"cats\"},\"finish_reason\":\"length\"}],\"usage\":\{\"prompt_tokens\":300,\"completion_tokens\":1,\"total_tokens\":301}}"

def private piece(key, text : string) : string => "data: \{\"choices\":[\{\"index\":0,\"delta\":\{\"{key}\":\"{text}\"}}]}"

class FakeChatServer : HvWebServer {
    def override onInit {
        STREAM("/v1/chat/completions") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            let body = string(req.body)
            if (find(body, "\"fail\"") >= 0) {   // the model named fail, in either client's JSON spacing
                respond(server, writer, int(http_status.INTERNAL_SERVER_ERROR), "application/json", "\{\"error\":\"down\"}")
                return
            }
            if (find(body, "image_url") >= 0) {   // the image question: one buffered completion, the image's rows in its prompt count
                respond(server, writer, int(http_status.OK), "application/json", find(body, "\"nousage\"") >= 0 ? replace(SEEN, "\"prompt_tokens\":300", "\"prompt_tokens\":0") : SEEN)
                return
            }
            sse_event(server, writer, slice(ROLE, 6), "")
            let oneword = find(body, "\"model\":\"oneword\"") >= 0
            if (find(body, "\"model\":\"notext\"") < 0) {
                for (word in ["one", " two", " three"]) {
                    sleep(30u)
                    sse_event(server, writer, slice(piece("content", word), 6), "")
                    break if (oneword)
                }
            }
            if (find(body, "\"model\":\"nousage\"") < 0) {
                sse_event(server, writer, slice(oneword ? replace(USAGE, "\"completion_tokens\":3", "\"completion_tokens\":1") : USAGE, 6), "")
            }
            sse_event(server, writer, "[DONE]", "")
            close_writer(server, writer)
        }
    }
}

var g_takes = 0   //! the speech fake's request count, in its own thread's context
var g_heard : table<string>   //! every upload the speech fake's `seen` model was sent, in its own thread's context

//! what tells one uploaded clip from another: the indices of its samples that are not silence, "" for an upload that is no WAV
def private upload_key(var req : HttpRequest?) : string {
    var err = ""
    let path = create_temp_file("served_bench_upload", ".wav", err)
    save_form_file(req, "file", path)
    var samples : array<float>
    var rate = 0
    var channels = 0
    read_wav(path, samples, rate, channels)
    remove(path)
    let key = build_string() $(writer) {
        for (i, s in count(), samples) {
            if (s != 0.0) {
                writer |> write("{i} ")
            }
        }
    }
    delete samples
    return key
}

class FakeSpeechServer : HvWebServer {
    def override onInit {
        POST("/v1/audio/transcriptions") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let model = get_form_data(req, "model")
            if (model == "fail") {
                return resp |> JSON("\{\"error\":\"down\"}", http_status.INTERNAL_SERVER_ERROR)
            }
            if (model == "empty") {
                return resp |> JSON("\{\"text\":\"  \"}")
            }
            if (model == "drift") {
                g_takes++
                return resp |> JSON("\{\"text\":\"take {g_takes}\"}")
            }
            if (model == "slow") {
                sleep(300u)
            }
            if (model == "seen") {
                let upload = upload_key(req)
                let again = g_heard |> key_exists(upload)
                g_heard |> insert(upload)
                return resp |> JSON("\{\"text\":\"{again ? "heard before" : "new"}\"}")
            }
            return resp |> JSON("\{\"text\":\"hello world\"}")
        }
        POST("/v1/audio/speech") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let body = string(req.body)
            if (find(body, "\"model\":\"fail\"") >= 0) {
                return resp |> JSON("\{\"error\":\"down\"}", http_status.INTERNAL_SERVER_ERROR)
            }
            if (find(body, "\"model\":\"text\"") >= 0) {
                return resp |> JSON("\{\"text\":\"not audio\"}")
            }
            if (empty(g_wav)) {
                g_wav = silent_wav(16000)
            }
            return resp |> SERVE_FILE(g_wav)
        }
    }
}

var g_wav = ""   //! the speech fake's answer, written on its first request in its own thread's context

//! a temp file holding `samples` samples of silence as a 16-bit mono 16 kHz WAV
def private silent_wav(samples : int) : string {
    var err = ""
    let path = create_temp_file("served_bench_said", ".wav", err)
    var silence : array<float>
    silence |> resize(samples)
    write_wav(path, silence, 16000u, 1)
    delete silence
    return path
}

def private with_clip(blk : block<(clip : string) : void>) {
    var err = ""
    let clip = create_temp_file("served_bench_clip", ".wav", err)
    fopen(clip, "wb") $(f) {
        f |> fwrite("RIFF not really audio")
    }
    invoke(blk, clip)
    remove(clip)
}

[test]
def test_speech_rows_against_fakes(t : T?) {
    with_test_server(type<FakeSpeechServer>) $(speech_url) {
        with_clip() $(clip) {
            t |> run("a transcription is the request's wall and its text; a failed or empty answer says why") @(t : T?) {
                let heard = transcribe(Speech(url = speech_url), clip)
                t |> equal(heard.why, "")
                t |> equal(heard.text, "hello world")
                t |> success(heard.wall_us > 0l, "the wall is the client's ({heard.wall_us} us)")
                t |> success(transcribe(Speech(url = speech_url, model = "slow"), clip).wall_us >= 300000l, "the server's 300 ms sit inside the wall")
                t |> equal(transcribe(Speech(url = speech_url, model = "fail"), clip).why, "HTTP 500")
                t |> success(find(transcribe(Speech(url = speech_url, model = "empty"), clip).why, "no text") >= 0, "blank text is no transcription")
            }
            t |> run("a row answers the text its reps read, and refuses reps that read different texts") @(t : T?) {
                t |> equal(clip_row(Speech(url = speech_url), clip, "asr clip", "", 3), "hello world")
                t |> equal(clip_row(Speech(url = speech_url), clip, "asr clip", "another text", 3), "", "the text the idle row read is the text a loaded row must read")
                t |> equal(clip_row(Speech(url = speech_url, model = "drift"), clip, "asr clip", "", 3), "", "three reps, three texts")
                t |> equal(clip_row(Speech(url = speech_url, model = "fail"), clip, "asr clip", "", 3), "", "a failed rep refuses the row")
            }
            t |> run("every request uploads a clip the server has not heard; a clip that is no WAV goes up as it is") @(t : T?) {
                let wav = silent_wav(160000)   // ten seconds: the copies differ in one sample picked off the clock
                t |> equal(clip_row(Speech(url = speech_url, model = "seen"), wav, "asr clip", "", 4), "new", "four reps, four uploads the fake never saw")
                let copy = unseen_clip(wav)
                t |> success(!empty(copy) && copy != wav && stat(copy).size == stat(wav).size, "the copy is a file of its own, the clip's size")
                remove(copy)
                remove(wav)
                t |> equal(unseen_clip(clip), "", "no WAV, no copy")
                t |> equal(clip_row(Speech(url = speech_url, model = "seen"), clip, "asr clip", "", 2), "", "the same bytes twice: the fake's second answer differs and the row is refused")
            }
            with_test_server(type<FakeChatServer>) $(chat_url) {
                t |> run("a loaded row stands while the chat turn outlasts its reps") @(t : T?) {
                    let load = LoadJob(url = chat_url, model = "ok", question = "ask", budget = 8)
                    t |> success(loaded_clip_row(Speech(url = speech_url), load, clip, "asr clip loaded", "hello world", 2), "two fast reps inside the turn's 90 ms of decode")
                    t |> success(!loaded_clip_row(Speech(url = speech_url, model = "slow"), load, clip, "asr clip loaded", "hello world", 2), "two 300 ms reps outlive the turn: refused")
                    t |> success(!loaded_clip_row(Speech(url = speech_url), LoadJob(url = chat_url, model = "notext", question = "ask", budget = 8), clip, "asr clip loaded", "hello world", 2), "a turn with no reply text never starts the reps: refused")
                    t |> success(!loaded_clip_row(Speech(url = speech_url), load, clip, "asr clip loaded", "another text", 2), "a loaded row that reads another text than the idle row: refused")
                }
            }
        }
    }
}

[test]
def test_synthesis_and_scene_against_fakes(t : T?) {
    t |> run("a WAV answer's seconds are its samples over its rate; other bytes carry none") @(t : T?) {
        let half = silent_wav(8000)
        t |> equal(wav_file_seconds(half), 0.5)
        remove(half)
        var junk <- [for (ch in "RIFF not really audio, only text of some length past the header"); uint8(ch)]
        t |> equal(wav_seconds(junk), 0.0)
        t |> equal(wav_file_seconds("{half}.absent"), 0.0, "a file that is not there carries none")
        delete junk
    }
    with_test_server(type<FakeSpeechServer>) $(speech_url) {
        t |> run("a synthesis is the request's wall and the speech it carries; a failed or audio-less answer says why") @(t : T?) {
            let said = say(Speech(url = speech_url), "", "hello")
            t |> equal(said.why, "")
            t |> equal(said.audio_s, 1.0)
            t |> success(said.wall_us > 0l)
            t |> equal(say(Speech(url = speech_url, model = "fail"), "", "hello").why, "HTTP 500")
            t |> success(find(say(Speech(url = speech_url, model = "text"), "", "hello").why, "WAV") >= 0, "a JSON answer is no audio")
        }
        with_clip() $(clip) {
            with_test_server(type<FakeChatServer>) $(chat_url) {
                let load = LoadJob(url = chat_url, model = "ok", question = "ask", budget = 8)
                t |> run("a round lands the turn's rate and every lane's requests inside it") @(t : T?) {
                    var lanes <- [SceneJob(lane = 0, url = speech_url, payload = clip), SceneJob(lane = 1, tts = true, url = speech_url, payload = "hello")]
                    var samples : array<SceneSample>
                    scene_round(load, lanes, samples)
                    var chat = 0
                    var per_lane = [0, 0]
                    for (smp in samples) {
                        if (smp.lane == SCENE_CHAT) {
                            chat++
                            t |> success(smp.value > 0.0, "the turn's decode rate ({smp.value} tok/s)")
                        } else {
                            per_lane[smp.lane]++
                            t |> success(smp.value > 0.0, "lane {smp.lane}'s wall ({smp.value} ms)")
                        }
                    }
                    t |> equal(chat, 1, "one rate a round")
                    t |> success(per_lane[0] >= 1 && per_lane[1] >= 1, "both lanes answered inside the turn's 60 ms of decode: {per_lane}")
                    delete samples
                    delete lanes
                }
                t |> run("a paced lane sends one request a period, a failed request reads below zero") @(t : T?) {
                    var lanes <- [SceneJob(lane = 0, url = speech_url, payload = clip, period_ms = 5000), SceneJob(lane = 1, tts = true, url = speech_url, model = "fail", payload = "hello", period_ms = 5000)]
                    var samples : array<SceneSample>
                    scene_round(load, lanes, samples)
                    var paced <- [for (smp in samples); smp.value; where smp.lane == 0]
                    var failed <- [for (smp in samples); smp.value; where smp.lane == 1]
                    t |> equal(length(paced), 1, "one request, then the wait the turn ends")
                    t |> success(length(failed) == 1 && failed[0] < 0.0, "the failed request: {failed}")
                    delete paced
                    delete failed
                    delete samples
                    delete lanes
                }
            }
        }
    }
}

[test]
def test_synthesis_and_scene_rows_against_fakes(t : T?) {
    with_test_server(type<FakeSpeechServer>) $(speech_url) {
        with_clip() $(clip) {
            with_test_server(type<FakeChatServer>) $(chat_url) {
                let load = LoadJob(url = chat_url, model = "ok", question = "ask", budget = 8)
                t |> run("the synthesis and scene rows stand over the fakes, and a failed lane refuses its row alone") @(t : T?) {
                    var models <- ["", "fail"]
                    let said = tts_rows(speech_url, "tag", models, "", "hello", 2)
                    t |> success(said.rows == 1 && said.refused == 1, "the default model's row stands, the failing model's is refused: {said.rows} / {said.refused}")
                    delete models
                    var lanes <- [SceneJob(lane = 0, url = speech_url, payload = clip), SceneJob(lane = 1, tts = true, url = speech_url, payload = "hello")]
                    let acted = scene_rows("tag", load, lanes, 2, true)
                    t |> success(acted.rows == 3 && acted.refused == 0, "the turn's rate and both lanes, back to back: {acted.rows} / {acted.refused}")
                    t |> equal(lanes[0].period_ms, 0, "a flooded lane keeps no pace")
                    var broken <- [SceneJob(lane = 0, url = speech_url, payload = clip), SceneJob(lane = 1, tts = true, url = speech_url, model = "fail", payload = "hello")]
                    let part = scene_rows("tag", load, broken, 2, true)
                    t |> success(part.rows == 2 && part.refused == 1, "the failing TTS lane is refused, the others stand: {part.rows} / {part.refused}")
                    var paced <- [SceneJob(lane = 0, tts = true, url = speech_url, payload = "hello")]
                    let slow = scene_rows("tag", load, paced, 2, false)
                    t |> equal(paced[0].period_ms, 1000, "a paced TTS lane waits the second of speech its answer carried")
                    t |> success(slow.rows == 2 && slow.refused == 0, "one request a round is a sample a round: the row stands over two rounds ({slow.rows} / {slow.refused})")
                    var late <- [SceneJob(lane = 0, url = speech_url, model = "slow", payload = clip)]
                    let none = scene_rows("tag", load, late, 2, true)
                    t |> success(none.rows == 1 && none.refused == 1, "a lane whose 300 ms request never fits the turn has a round with no sample: refused, the turn's rate stands ({none.rows} / {none.refused})")
                    delete late
                    delete lanes
                    delete broken
                    delete paced
                }
            }
        }
    }
}

[test]
def test_scene_rows_flags_and_lanes(t : T?) {
    t |> run("a scene row needs two samples alone and two in the scene") @(t : T?) {
        var two <- [10.0, 12.0]
        var one <- [10.0]
        t |> success(scene_row("tag", "asr x", "ms", two, two), "two of each stand")
        t |> success(!scene_row("tag", "asr x", "ms", two, one), "one sample in the scene has no spread")
        t |> success(!scene_row("tag", "asr x", "ms", one, two), "one sample alone has none")
        delete two
        delete one
    }
    t |> run("the flags that would report no row of a mode they ask for are refused, and the scene's lanes follow the flags") @(t : T?) {
        t |> equal(args_error(Args(url = "u"), 3), "")
        t |> success(find(args_error(Args(url = "u"), 1), "one rep") >= 0, "one rep has no spread")
        var img <- ["a.jpg"]
        t |> success(find(args_error(Args(image <- img), 3), "--image") >= 0, "an image row needs the chat server")
        t |> success(find(args_error(Args(follow_up = true), 3), "--follow-up") >= 0, "the follow-up row needs the chat server")
        t |> success(find(args_error(Args(url = "u", follow_up = true, no_chat = true), 3), "--follow-up") >= 0, "and its chat rows")
        var clips <- ["c.wav"]
        t |> success(find(args_error(Args(url = "u", asr_url = "a", clip <- clips, scene = true), 3), "--scene") >= 0, "a scene needs a speaking server")
        var clip2 <- ["c.wav"]
        t |> success(find(args_error(Args(asr_url = "a", clip <- clip2, under_load = true), 3), "--under-load") >= 0, "a loaded row needs the chat server")
        var clip3 <- ["c.wav"]
        var ids <- ["one", "two"]
        var cfg = Args(url = "u", asr_url = "a", tts_url = "s", clip <- clip3, scene_asr <- ids, tts_voice = "v")
        var lanes <- scene_lanes(cfg, "tts-id", "say this")
        t |> equal(length(lanes), 3, "a lane an ASR model, then the TTS lane")
        t |> success(lanes[0].model == "one" && lanes[1].model == "two" && !lanes[1].tts && lanes[1].payload == "c.wav" && lanes[1].lane == 1, "the ASR lanes in flag order on the first clip")
        t |> success(lanes[2].tts && lanes[2].model == "tts-id" && lanes[2].payload == "say this" && lanes[2].voice == "v" && lanes[2].lane == 2, "the TTS lane last")
        delete lanes
        delete cfg
        var clip4 <- ["c.wav"]
        var plain = Args(url = "u", asr_url = "a", tts_url = "s", clip <- clip4, asr_model = "only")
        var one_lane <- scene_lanes(plain, "", "say")
        t |> success(length(one_lane) == 2 && one_lane[0].model == "only", "no --scene-asr: the --asr-model one")
        delete one_lane
        delete plain
    }
}

[test]
def test_prompt_and_rows(t : T?) {
    t |> run("a system prompt opens on its nonce and holds the asked characters of the corpus, cycled") @(t : T?) {
        let base = "alpha beta gamma delta."
        let a = system_prompt(base, "n1", 60)
        let b = system_prompt(base, "n2", 60)
        t |> success(starts_with(a, "Session n1. "), "the nonce leads: {a}")
        t |> success(slice(a, 0, 12) != slice(b, 0, 12), "two nonces part within the first dozen characters")
        t |> equal(system_prompt(base, "n1", 60), a, "one nonce, one prompt")
        t |> equal(count(a, base), 2, "60 characters hold the corpus twice whole")
        t |> success(find(a, "[part 3]\nalpha beta gam") >= 0, "and its first 14 characters once more: {a}")
        t |> success(length(system_prompt(base, "n1", 600)) > length(a) + 500, "the size follows the ask")
    }
    t |> run("the corpus is its %% segments, the comment ones dropped, a blank line between them") @(t : T?) {
        var err = ""
        let path = create_temp_file("served_bench_corpus", ".txt", err)
        fopen(path, "wb") $(f) {
            f |> fwrite("# a header\n%%\nfirst article\nits second line\n\n%%\nsecond article\n")
        }
        t |> equal(corpus_text(path), "first article\nits second line\n\nsecond article")
        remove(path)
        t |> equal(corpus_text(path), "", "a missing corpus reads empty")
    }
    t |> run("the row statistic and the brackets beside it") @(t : T?) {
        var vals <- [3.0, 1.0, 2.0]
        let s = spread_of(vals)
        t |> equal(s.best, 1.0)
        t |> equal(s.median, 2.0)
        t |> success(abs(s.cv - 40.8248) < 0.001, "population cv in percent ({s.cv})")
        var all_own <- [61.0, 60.0, 62.0]
        t |> equal(own(all_own, "TTFT", "ms"), "  [server's own TTFT 61.0 ms]")
        var one_missing <- [61.0, 0.0, 62.0]
        t |> equal(own(one_missing, "TTFT", "ms"), "", "one turn without the server's clock drops the bracket")
        t |> equal(rate(Turn(completion_tokens = 201l), 2.0e6), 100.0, "the first token sits outside the span")
        t |> equal(rate(Turn(completion_tokens = 201l), 0.0), 0.0, "no span, no rate")
    }
}

[test]
def test_turns_against_a_fake_server(t : T?) {
    with_test_server(type<FakeChatServer>) $(base_url) {
        t |> run("a complete stream is a turn: counts, the cache hit, both spans, the server's own clock") @(t : T?) {
            let turn = run_turn(Plan(url = base_url, model = "ok"), "sys", "ask", 8)
            t |> equal(turn.why, "")
            t |> equal(turn.prompt_tokens, 100l)
            t |> equal(turn.completion_tokens, 3l)
            t |> equal(turn.cached_tokens, 40l)
            t |> success(turn.ttft_us >= 30000l, "the first reply text arrived after the server's 30 ms pause ({turn.ttft_us} us)")
            t |> success(turn.gen_us >= 40000l && turn.gen_us < 5000000l, "the decode span covers the two later pieces ({turn.gen_us} us)")
            t |> equal(turn.self_ttft_ms, 12.5)
        }
        t |> run("the follow-up row stands over complete turns and is refused by a failed one") @(t : T?) {
            t |> success(followup_row(Plan(url = base_url, model = "ok", reply = 8, reps = 2), "run"))
            t |> success(!followup_row(Plan(url = base_url, model = "fail", reply = 8, reps = 2), "run"), "a failed first request refuses the row")
        }
        t |> run("an image question is its wall and its prompt tokens; an image row stands over a file and is refused without one") @(t : T?) {
            with_clip() $(file) {   // any bytes: the fake reads the part, not the picture
                let uri = image_file_data_uri(file)
                t |> success(starts_with(uri, "data:image/jpeg;base64,") && length(uri) > 30, "a file is a data URI: {uri}")
                t |> equal(image_file_data_uri("{file}.absent"), "", "a file that is not there is no URI")
                let seen = see(Plan(url = base_url, model = "ok"), "sys", "what?", uri)
                t |> equal(seen.why, "")
                t |> equal(seen.prompt_tokens, 300l)
                t |> success(seen.wall_us > 0l)
                t |> equal(see(Plan(url = base_url, model = "fail"), "sys", "what?", uri).why, "HTTP 500")
                t |> success(find(see(Plan(url = base_url, model = "nousage"), "sys", "what?", uri).why, "no usage") >= 0, "an answer with no prompt count cannot enter a row")
                var files <- [file]
                let looked = image_rows(Plan(url = base_url, model = "ok", reps = 2), files, "run")
                t |> success(looked.rows == 1 && looked.refused == 0, "one row an image")
                var gone <- ["{file}.absent"]
                let missed = image_rows(Plan(url = base_url, model = "ok", reps = 2), gone, "run")
                t |> success(missed.rows == 0 && missed.refused == 1, "a missing file refuses its row")
                delete files
                delete gone
            }
        }
        t |> run("a turn that cannot enter a row says why") @(t : T?) {
            t |> success(find(run_turn(Plan(url = base_url, model = "nousage"), "sys", "ask", 8).why, "include_usage") >= 0, "no usage chunk")
            t |> success(find(run_turn(Plan(url = base_url, model = "notext"), "sys", "ask", 8).why, "no reply text") >= 0, "no reply text")
            t |> success(find(run_turn(Plan(url = base_url, model = "oneword"), "sys", "ask", 8).why, "no decode span") >= 0, "a reply of one token has no decode span to time")
            t |> equal(run_turn(Plan(url = base_url, model = "fail"), "sys", "ask", 8).why, "HTTP 500")
        }
    }
    t |> run("no server at the address is a refused turn, not a hang") @(t : T?) {
        t |> equal(run_turn(Plan(url = "http://127.0.0.1:9"), "sys", "ask", 8).why, "HTTP 0")
    }
}
