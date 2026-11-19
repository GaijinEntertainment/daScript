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
require ../harness/_served_turn.das   // reply_rate
require ../harness/served_bench.das
require ../harness/model_bench.das   // the model bench's turn, cell and wave
require ../harness/model_bench_run.das   // its runner: the command lines, the run files, a model's starts
require daslib/command_line   // get_das_exe: the fake's child runs on this process's binary
require ../harness/_model_bench.das   // its settings
require ../harness/_corpus.das   // corpus_text
require ../harness/_prompt_files.das   // id_list, read_ids: the harness's comma lists of numbers
require ../../../tests/dasHV/_dashv_test_common.das   // with_test_server: a server on its own thread and port
require _fake_chat_server   // FakeChatServer and the SSE lines it answers with

// The served-turn instrument (harness/served_bench.das), with no model behind it: the stream reader's two clock marks
// and its usage read over hand-built SSE lines, the system prompt's nonce and size, the corpus filter, the row
// statistic, and whole turns against a fake chat server that answers by the request's "model" field - a complete
// stream, one with no usage chunk, one with no reply text, one of a single token, a failed request.

let TWO_PAUSES_US = 180000l   //! the chat fake's two 100 ms pauses between reply pieces, less what the first piece's delivery may lag
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

[test(description = "Against a fake speech server, a transcription reports its wall and text or the failure reason, a clip row refuses reps that read differing texts, every rep uploads an unseen clip copy, and a loaded row stands only inside the chat turn")]
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
                    t |> success(loaded_clip_row(Speech(url = speech_url), load, clip,
                                                 "asr clip loaded", "hello world", 2),
                                 "two fast reps inside the turn's 300 ms of decode")
                    t |> success(!loaded_clip_row(Speech(url = speech_url, model = "slow"), load, clip, "asr clip loaded", "hello world", 2), "two 300 ms reps outlive the turn: refused")
                    t |> success(!loaded_clip_row(Speech(url = speech_url), LoadJob(url = chat_url, model = "notext", question = "ask", budget = 8), clip, "asr clip loaded", "hello world", 2), "a turn with no reply text never starts the reps: refused")
                    t |> success(!loaded_clip_row(Speech(url = speech_url), load, clip, "asr clip loaded", "another text", 2), "a loaded row that reads another text than the idle row: refused")
                }
            }
        }
    }
}

[test(description = "WAV seconds are samples over rate (zero for non-WAV or missing files), a synthesis carries its wall and audio seconds or the failure reason, and a scene round lands one chat rate plus each lane, paced lanes once and failures below zero")]
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

[test(description = "Over fake servers the TTS and scene row counts come out exact: a failing model or lane refuses only its own row, a flooded lane keeps no pace, a paced TTS lane waits its audio second, a never-fitting lane is refused")]
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

[test(description = "A scene row needs two samples both alone and in the scene, `args_error` refuses flag sets that would report no row of the asked mode, and `scene_lanes` builds the ASR lanes in flag order then the TTS lane last")]
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

[test(description = "The system prompt leads with its nonce and cycles the corpus to the asked size, the corpus reader keeps non-comment %% segments, and the row statistic, server-clock bracket and decode rate match hand-computed values")]
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
        t |> equal(reply_rate(201l, 2.0e6), 100.0, "the first token sits outside the span")
        t |> equal(reply_rate(201l, 0.0), 0.0, "no span, no rate")
    }
}

[test(description = "Against a fake chat server a complete SSE stream yields exact token counts, cache hit, both clock spans and the server's own TTFT, image and audio rows stand or refuse per file, and every incomplete turn or dead address names its reason")]
def test_turns_against_a_fake_server(t : T?) {
    with_test_server(type<FakeChatServer>) $(base_url) {
        t |> run("a complete stream is a turn: counts, the cache hit, both spans, the server's own clock") @(t : T?) {
            let turn = run_turn(Plan(url = base_url, model = "ok"), "sys", "ask", 8)
            t |> equal(turn.why, "")
            t |> equal(turn.prompt_tokens, 100l)
            t |> equal(turn.completion_tokens, 3l)
            t |> equal(turn.cached_tokens, 40l)
            t |> success(turn.ttft_us >= 100000l,
                         "first text after the server's 100 ms pause ({turn.ttft_us} us)")
            t |> success(turn.gen_us >= TWO_PAUSES_US && turn.gen_us < 5000000l,
                         "the decode span covers the two later pieces ({turn.gen_us} us)")
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
                let heard = media_rows(Plan(url = base_url, model = "ok", reps = 2), files, "run", true)
                t |> success(heard.rows == 1 && heard.refused == 0, "one row a chat clip: the file goes up as an input_audio part")
                let unheard = media_rows(Plan(url = base_url, model = "ok", reps = 2), gone, "run", true)
                t |> success(unheard.rows == 0 && unheard.refused == 1, "a missing clip refuses its row")
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

[test(description = "The model bench's prompt leads with an id it names as no part of the task, a turn is refused for each reason it cannot enter a cell, a model's settings lay its entry over the defaults, and the row flags fire on a noisy, fallen, stalled, unscaled or unfair row")]
def test_model_bench_rules(t : T?) {
    t |> run("a prompt opens on its id and ends on the task; the task alone carries no context") @(t : T?) {
        let base = "def alpha():\n    return 1\n"
        let short = prompt_of(base, "n1", 0)
        let long = prompt_of(base, "n2", 12)
        t |> success(starts_with(short, "Reference id n1 (not part of the task)."), "the id leads, named as no task: {short}")
        t |> success(find(short, "def alpha") < 0 && ends_with(short, "no introduction."), "the task alone ends the short prompt, asking for code and nothing ahead of it")
        t |> success(find(long, "def alpha():") >= 0 && find(long, "return") < 0, "12 characters of context, no more: {long}")
        t |> success(slice(short, 0, 16) != slice(long, 0, 16), "two ids part inside the first line")
    }
    t |> run("a turn enters a cell only whole: each refusal names its reason") @(t : T?) {
        let whole = BenchTurn(ttft_us = 1000l, gen_us = 2000l, prompt_tokens = 100l, cached_tokens = 40l, completion_tokens = 3l)
        t |> equal(refusal_of(whole, 3), "")
        t |> equal(refusal_of(BenchTurn(error = "http 500 down"), 3), "http 500 down")
        t |> success(find(refusal_of(BenchTurn(ttft_us = -1l), 3), "no reply text") >= 0)
        t |> success(find(refusal_of(BenchTurn(ttft_us = 1000l), 3), "no usage chunk") >= 0)
        t |> success(find(refusal_of(BenchTurn(ttft_us = 1000l, gen_us = 2000l, prompt_tokens = 100l, cached_tokens = 90l, completion_tokens = 3l), 3), "came off a cache") >= 0, "90 cached tokens are a cache hit")
        t |> success(find(refusal_of(whole, 8), "ended at 3 of 8") >= 0, "a reply short of its budget")
        t |> success(find(refusal_of(BenchTurn(ttft_us = 1000l, prompt_tokens = 100l, completion_tokens = 1l), 1), "no decode span") >= 0)
        t |> equal(prefill_rate(whole), 60000.0, "60 uncached tokens over one millisecond")
        t |> equal(reply_rate(whole.completion_tokens, 2000.0), 1000.0, "the first token sits outside the span")
    }
    t |> run("a model's settings are the defaults with its own fields over them") @(t : T?) {
        var config : BenchConfig
        t |> success(sscan_json("\{\"defaults\":\{\"lengths\":[0,2048],\"reps\":3,\"reply_tokens\":128,\"server\":\{\"ctx\":32768,\"streams\":4}},\"models\":\{\"small\":\{\"reps\":5,\"server\":\{\"streams\":1},\"drafter\":\"d.gguf\"}}}", config), "the config reads")
        var own <- settings_of(config, "small")
        t |> success(own.known && own.s.reps == 5 && own.s.reply_tokens == 128 && length(own.s.lengths) == 2, "its reps, the defaults' reply and lengths")
        t |> success(own.s.server.streams == 1 && own.s.server.ctx == 32768 && own.s.drafter == "d.gguf", "its streams, the defaults' context, its drafter")
        var other <- settings_of(config, "absent")
        t |> success(!other.known && other.s.reps == 3, "control: an entry the config lacks runs the defaults and says so")
        delete own
        delete other
        delete config
    }
    t |> run("the flags of a row") @(t : T?) {
        let calm = (best = 99.0, median = 100.0, cv = 1.0, mean = 100.0, worst = 101.0)
        t |> equal(flags_of(calm, 120.0, 0.6, true, false), "", "control: a calm row five sixths of the rung before it")
        t |> equal(flags_of(calm, 200.0, 0.6, true, false), "cliff", "half the rung before it, at twice the length")
        t |> equal(flags_of(calm, 200.0, 0.6, false, false), "", "the same fall with no doubling is no cliff")
        t |> equal(flags_of((best = 80.0, median = 100.0, cv = 9.0, mean = 100.0, worst = 120.0), 0.0, 0.6, false, true), "noisy,stall")
        t |> equal(wave_flags_of(calm, 105.0, false), "", "control: a summed rate level with the width before it")
        t |> equal(wave_flags_of(calm, 130.0, true), "scaling,unfair", "under nine tenths of the width before, and the wave unfair")
    }
}

[test(description = "Against a fake chat server a model bench turn carries its counts, clock marks and draft counts, a cell reports both rates over its reps with the cut scaled by its sizing turn and refuses on a cache hit or a failed turn, and a wave of eight streams sums its rows with every turn's text intact")]
def test_model_bench_against_a_fake_server(t : T?) {
    with_test_server(type<FakeChatServer>) $(base_url) {
        t |> run("a turn: counts, both spans, the gaps, the server's clock, the draft counts where a server reports them") @(t : T?) {
            let turn = bench_turn(base_url, "drafts", "ask", 3, 30)
            t |> equal(turn.error, "")
            t |> success(turn.prompt_tokens == 100l && turn.completion_tokens == 3l && turn.cached_tokens == 40l, "the usage chunk's counts")
            t |> success(turn.ttft_us >= 100000l && turn.gen_us >= TWO_PAUSES_US, "first text after one pause, the span over two more ({turn.ttft_us} and {turn.gen_us} us)")
            t |> success(turn.median_gap_us >= 90000l && turn.longest_gap_us >= turn.median_gap_us, "the gaps between reply chunks")
            t |> success(turn.draft_n == 8l && turn.draft_n_accepted == 5l && turn.server_ttft_ms == 12.5, "the server's own clock and its draft counts")
            t |> equal(bench_turn(base_url, "ok", "ask", 3, 30).draft_n, 0l, "control: a plain server reports none")
            t |> success(!empty(bench_turn(base_url, "fail", "ask", 3, 30).error), "a failed request carries its error")
        }
        t |> run("a cell: the sizing turn scales the cut, the reps give both rates, a refusal names its rep") @(t : T?) {
            let base = repeat("0123456789", 100)
            let cell = run_cell(BenchArgs(url = base_url, model_id = "ok"), "pp80", base, "run", 400, 2, 3, 30, 10.0, 80l, 60l)
            t |> equal(cell.why, "")
            t |> equal(cell.cut_chars, 200, "100 tokens read where 80 were wanted, 60 of them the task: the cut halves")
            t |> success(cell.prefill.median > 0.0 && cell.prefill.median <= 600.0, "60 uncached tokens over at least a tenth of a second ({cell.prefill.median})")
            t |> success(cell.decode.median > 0.0 && cell.decode.median <= 10.0, "two tokens over at least two tenths ({cell.decode.median})")
            t |> equal(cell.server_prefill, 4800.0, "the server's own clock: 60 tokens in 12.5 ms")
            let hit = run_cell(BenchArgs(url = base_url, model_id = "cached"), "pp80", base, "run", 400, 2, 3, 30, 10.0, 80l, 60l)
            t |> success(find(hit.why, "at rep 0") >= 0 && find(hit.why, "came off a cache") >= 0, "a cache hit refuses the cell: {hit.why}")
            let down = run_cell(BenchArgs(url = base_url, model_id = "fail"), "pp80", base, "run", 400, 2, 3, 30, 10.0, 80l, 60l)
            t |> equal(down.why, "the sizing turn failed")
        }
        t |> run("a wave of eight: the rows are the streams summed, and every turn's text crossed its thread whole") @(t : T?) {
            let raw = path_join(test_temp_dir(), "model_bench_wave_{ref_time_ticks()}.jsonl")
            let cell = run_wave_cell(BenchArgs(url = base_url, model_id = "ok", raw = raw), "tg3@8", "ctx", "run", 0, 8, 2, 3, 30, 2.0)
            t |> equal(cell.why, "")
            t |> success(cell.decode.median > 4.0 * cell.stream_decode, "eight streams' decode rates summed ({cell.decode.median} over {cell.stream_decode} a stream)")
            let lone = run_wave_cell(BenchArgs(url = base_url, model_id = "ok"), "tg3@1", "ctx", "run", 0, 1, 1, 3, 30, 2.0)
            t |> success(lone.decode.median < 4.0 * lone.stream_decode, "control: a wave of one sums to its one stream ({lone.decode.median} over {lone.stream_decode})")
            t |> success(cell.prefill.median > 0.0 && cell.last_ttft_ms >= 100.0, "the wave's prompts over its last first text")
            let lines = fread(raw)
            t |> equal(count(lines, "\"cell\":\"tg3@8\""), 16, "two waves of eight, a line a turn")
            t |> equal(count(lines, "\"reply_head\":\"one two three\""), 16, "each turn's reply read back from its line")
            remove(raw)
            let down = run_wave_cell(BenchArgs(url = base_url, model_id = "fail"), "tg3@2", "ctx", "run", 0, 2, 1, 3, 30, 2.0)
            t |> success(find(down.why, "the warm-up turn") >= 0, "a server that fails every turn refuses the wave at its warm-up: {down.why}")
        }
    }
}

[test(description = "`id_list` keeps a comma list's numbers in order, dropping an empty part and one that is not a number, and under bounds drops a number below `lo` or past `hi` while keeping both bounds themselves; an absent file's ids are none")]
def test_prompt_files_id_list(t : T?) {
    t |> equal(id_list(" 4, 2,,x, 7 "), [4l, 2l, 7l], "the numbers in order, an empty part and a word dropped")
    t |> equal(id_list("-3,0,9223372036854775807"), [-3l, 0l, 9223372036854775807l], "control: unbounded, every number stays")
    t |> equal(id_list("0,1,2,3", 2l), [2l, 3l], "under lo = 2 the numbers below it are dropped, 2 kept")
    t |> equal(id_list("0,1,4,5,-3", 1l, 4l), [1l, 4l], "in 1 .. 4 both bounds stay and every number outside goes")
    t |> equal(length(read_ids(path_join(test_temp_dir(), "no_such_ids.txt"))), 0, "an absent file holds no ids")
}

//! a config the driver and runner cells run under: three lengths, widths of one and two, a wide pass of two and four, replies
//! of three tokens (the fake's reply), an entry `stub` with one arm `x`
let BENCH_TEST_CONFIG = ("\{\"defaults\":\{\"lengths\":[0,400,800],\"widths\":[1,2],\"wide_widths\":[2,4],\"width_lengths\":[0,400],\"reply_tokens\":3,\"arm_reply_tokens\":3," +
    "\"reps\":1,\"cell_seconds\":120,\"cliff_share\":0.6,\"stall_times\":10,\"unfair_times\":2,\"arms\":[\"off\",\"x\"],\"server\":\{\"ctx\":4096,\"streams\":2}," +
    "\"wide_server\":\{\"ctx\":2048,\"streams\":4}},\"arm_flags\":\{\"x\":\{\"das\":[\"--arm-mark\"],\"llama\":[\"--spec\",\"\{drafter\}\"]}},\"models\":\{\"stub\":\{\"drafter\":\"d.gguf\"}}}")

//! `BENCH_TEST_CONFIG` written under the test's temp folder; its path
def private bench_test_config(t : T?; name : string) : string {
    let path = path_join(test_temp_dir(), "{name}_{ref_time_ticks()}.json")
    t |> success(fwrite(path, BENCH_TEST_CONFIG), "the config is written at {path}")
    return path
}

//! the row names of `res`, sorted
def private row_names(res : RunOut) : array<string> {
    var names <- [for (name in keys(res.tests)); name]
    sort(names)
    return <- names
}

[test(description = "Against a fake server whose prompt tokens follow the request's size, a model bench run climbs its config's lengths and widths into named rows, stops at --max-length, records an arm's rows and the plain rows under --arm off, sends no wave wider than the server's streams in the wide pass, and exits 1 on a config that does not read, a failed calibration and a refused cell", timeout = 1200)]
def test_model_bench_driver(t : T?) {
    let config = bench_test_config(t, "model_bench_driver")
    with_test_server(type<FakeChatServer>) $(base_url) {
        t |> run("the plain run: the ladder's rows, then the width rows at the width lengths") @(t : T?) {
            var res : RunOut
            let out = path_join(test_temp_dir(), "model_bench_driver_{ref_time_ticks()}.json")
            t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", out = out), res), 0, "every cell reported")
            var names <- row_names(res)
            t |> equal(join(names, " "), "pp400 pp400@2 pp800 tg3 tg3+400 tg3+400@2 tg3+800 tg3@2", "the rows a ladder of three lengths and a width of two name")
            t |> success(empty(res.refused) && empty(res.bench.stopped), "nothing refused, the ladder ran to its end ({res.bench.stopped})")
            var sized_tokens = 0l
            for (name, row in keys(res.tests), values(res.tests)) {
                sized_tokens = name == "pp800" ? row.prompt_tokens : sized_tokens
            }
            t |> success(sized_tokens > 700l && sized_tokens < 900l, "the 800 rung's prompts are sized near their length ({sized_tokens} tokens)")
            var back : RunOut
            t |> success(sscan_json(fread(out), back) && length(back.tests) == length(res.tests), "the run file reads back with every row")
            remove(out)
            delete back
            delete names
            delete res
        }
        t |> run("--max-length stops the ladder at its rung") @(t : T?) {
            var res : RunOut
            run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", max_length = 400), res)
            t |> success(key_exists(res.tests, "pp400") && !key_exists(res.tests, "pp800"), "the 400 rung ran, the 800 one did not")
            t |> success(find(res.bench.stopped, "--max-length") >= 0, "the stop names its flag: {res.bench.stopped}")
            delete res
        }
        t |> run("an arm's run: the decode rows alone at the width lengths, suffixed; --arm off the same rows unsuffixed") @(t : T?) {
            var arm : RunOut
            t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", arm = "x"), arm), 0)
            var arm_names <- row_names(arm)
            t |> equal(join(arm_names, " "), "tg3+400:x tg3:x", "no prefill row and no width row under an arm")
            var plain : RunOut
            t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", arm = "off"), plain), 0)
            var plain_names <- row_names(plain)
            t |> equal(join(plain_names, " "), "tg3 tg3+400", "the rows an arm's are read against")
            delete arm_names
            delete plain_names
            delete arm
            delete plain
        }
        t |> run("the wide pass: the wide widths alone, none wider than the server's streams") @(t : T?) {
            var res : RunOut
            t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", wide = true, streams = 2), res), 0)
            var names <- row_names(res)
            t |> equal(join(names, " "), "pp400@2 tg3+400@2 tg3@2", "a wave of two; four streams would time the server's queue, and the one-stream cells are its base alone")
            delete names
            delete res
        }
        t |> run("a run that cannot start, and one whose cell is refused, exit 1") @(t : T?) {
            var res : RunOut
            t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = path_join(test_temp_dir(), "absent.json")), res), 1, "a config that does not read")
            t |> equal(run_bench(BenchArgs(url = base_url, model_id = "ok", config = config, entry = "stub"), res), 1, "a server whose two calibration prompts count alike")
            t |> success(empty(res.tests), "... and it reported no row")
            t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized-fakefailreps", config = config, entry = "stub"), res), 1, "a server that fails every timed turn")
            t |> success(length(res.refused) == 1 && find(res.refused[0].why, "at rep 0") >= 0, "its first cell is refused behind a served warm-up")
            t |> success(find(res.bench.stopped, "was refused") >= 0, "... and the ladder stops there: {res.bench.stopped}")
            delete res
        }
    }
    remove(config)
}

[test(description = "A model bench cell keeps a turn out of the server's cache by its request field, sums its reps' draft counts, marks a reply that pauses, reads llama.cpp's timing names as the server's clock, and a wave is unfair past its bound; a turn with no decode span, a zero span and a noisy wave are named; the shipped config reads, and an entry's lists and drafters lay over the defaults")]
def test_model_bench_cell_reads(t : T?) {
    with_test_server(type<FakeChatServer>) $(base_url) {
        t |> run("cells against the fake") @(t : T?) {
            let base = "one two three four five six seven eight nine ten "
            let kept = run_cell(BenchArgs(url = base_url, model_id = "fakenocache"), "tg3", base, "run", 0, 1, 3, 30, 10.0, 0l, 60l)
            t |> equal(kept.why, "", "the request says cache_prompt false: a server that caches without it reports no hit")
            let drafted = run_cell(BenchArgs(url = base_url, model_id = "drafts"), "tg3", base, "run", 0, 2, 3, 30, 10.0, 0l, 60l)
            t |> success(drafted.draft_n == 16l && drafted.draft_n_accepted == 10l, "two reps of 8 drafted and 5 accepted sum ({drafted.draft_n}, {drafted.draft_n_accepted})")
            let paused = run_cell(BenchArgs(url = base_url, model_id = "fakestall"), "tg3", base, "run", 0, 1, 3, 30, 0.5, 0l, 60l)
            let even = run_cell(BenchArgs(url = base_url, model_id = "ok"), "tg3", base, "run", 0, 1, 3, 30, 0.5, 0l, 60l)
            t |> success(paused.stalled && !even.stalled, "a 700 ms pause ahead of the last word is a stall ({paused.stalled}); control: even gaps under the floor are none ({even.stalled})")
            let llama = bench_turn(base_url, "fakellama", "hi", 3, 30)
            t |> success(llama.server_ttft_ms == 12.5 && llama.server_gen_ms == 20.0, "prompt_ms and predicted_ms read as the server's clock ({llama.server_ttft_ms}, {llama.server_gen_ms})")
            let tight = run_wave_cell(BenchArgs(url = base_url, model_id = "ok"), "tg3@2", base, "run", 0, 2, 1, 3, 30, 0.01)
            let loose = run_wave_cell(BenchArgs(url = base_url, model_id = "ok"), "tg3@2", base, "run", 0, 2, 1, 3, 30, 1000.0)
            t |> success(tight.unfair && !loose.unfair, "a last first text past the bound is unfair; control: under a wide bound it is not")
        }
    }
    t |> run("the turn and row rules the fake never reaches") @(t : T?) {
        let chunked = BenchTurn(ttft_us = 1000l, gen_us = 0l, prompt_tokens = 100l, cached_tokens = 0l, completion_tokens = 3l)
        t |> success(find(refusal_of(chunked, 3), "no decode span") >= 0, "three tokens in one chunk have no span")
        t |> equal(reply_rate(chunked.completion_tokens, 0.0), 0.0, "a zero span is a rate of zero, not a division")
        t |> equal(unserved_why(BenchTurn(ttft_us = 1000l)), "", "control: a turn with reply text was served")
        t |> equal(wave_flags_of((best = 80.0, median = 100.0, cv = 7.0, mean = 100.0, worst = 120.0), 0.0, false), "noisy", "a wave's cv past 6%")
        t |> equal(wave_flags_of((best = 96.0, median = 100.0, cv = 5.0, mean = 100.0, worst = 104.0), 0.0, false), "", "control: a wave under it")
    }
    t |> run("the shipped config, and an entry's fields over the defaults") @(t : T?) {
        var shipped : BenchConfig
        t |> success(read_config(default_config_path(), shipped), "modules/dasLLAMA/harness/model_bench.json reads")
        var gemma <- settings_of(shipped, "gemma-4-26B-A4B-it-Q4_K_M")
        t |> success(gemma.known && join(gemma.s.arms, " ") == "off assistant dflash", "the 26B's arms ({join(gemma.s.arms, " ")})")
        t |> success(ends_with(drafter_of(gemma.s, "dflash"), "DFlash-Q8_0.gguf") && drafter_of(gemma.s, "dflash") != drafter_of(gemma.s, "assistant"), "an arm's own drafter")
        t |> success(key_exists(shipped.arm_flags, "dflash2") && length(gemma.s.lengths) > 2 && gemma.s.server.streams > 0, "the arm table and the defaults reach it")
        delete gemma
        delete shipped
        var config : BenchConfig
        t |> success(sscan_json(BENCH_TEST_CONFIG, config), "the test config parses")
        var stub <- settings_of(config, "stub")
        t |> success(stub.known && drafter_of(stub.s, "x") == "d.gguf" && join(stub.s.arms, " ") == "off x" && stub.s.wide_server.streams == 4, "the entry's drafter, the defaults' arms and wide server")
        var over : BenchConfig
        t |> success(sscan_json("\{\"defaults\":\{\"lengths\":[0],\"widths\":[1],\"wide_widths\":[8],\"width_lengths\":[0],\"reply_tokens\":4,\"arm_reply_tokens\":9,\"reps\":1,\"cell_seconds\":5,\"cliff_share\":0.5,\"stall_times\":3,\"unfair_times\":2,\"arms\":[\"off\"],\"server\":\{\"ctx\":8,\"streams\":1,\"flags\":[\"--a\"]}},\"models\":\{\"m\":\{\"lengths\":[0,16],\"widths\":[1,2],\"wide_widths\":[4],\"width_lengths\":[0,16],\"reply_tokens\":6,\"arm_reply_tokens\":12,\"cell_seconds\":7,\"cliff_share\":0.7,\"stall_times\":4,\"unfair_times\":3,\"arms\":[\"off\",\"y\"],\"drafters\":\{\"y\":\"y.gguf\"},\"server\":\{\"flags\":[\"--b\",\"--c\"]},\"wide_server\":\{\"streams\":8}}}}", over), "a config with every field overridden parses")
        var m <- settings_of(over, "m")
        t |> success(length(m.s.lengths) == 2 && length(m.s.widths) == 2 && m.s.wide_widths[0] == 4 && length(m.s.width_lengths) == 2, "its four lists")
        t |> success(m.s.reply_tokens == 6 && m.s.arm_reply_tokens == 12 && m.s.cell_seconds == 7 && m.s.cliff_share == 0.7 && m.s.stall_times == 4.0 && m.s.unfair_times == 3.0, "its scalars")
        t |> success(join(m.s.arms, " ") == "off y" && drafter_of(m.s, "y") == "y.gguf" && drafter_of(m.s, "z") == "" && join(m.s.server.flags, " ") == "--b --c" && m.s.server.ctx == 8 && m.s.wide_server.streams == 8, "its arms, an arm's drafter, its server flags over the defaults' context, its wide server")
        delete m
        delete over
        delete stub
        delete config
    }
}

let RUNNER_TEST_PORT = 18171   //! the port the runner's child servers take in these cells; 18097-18160 belong to the server suites

//! the runner's flags for the fake as its server: the stub entry under `dir`, `more` after the fake's own arguments
def private runner_args(dir, config : string; more : array<string>) : RunnerArgs {
    var args <- RunnerArgs(models_dir = dir, out_dir = dir, config = config, quick = true, boot_seconds = 120, port = RUNNER_TEST_PORT,
        server_engine = "fake")
    args.model <- ["stub"]
    args.server_arg <- [get_das_exe(), path_join(get_das_root(), "modules/dasLLAMA/tests/_fake_chat_root.das"), "-dasroot", get_das_root(), "--",
        "--word-ms", "20", "--port", "\{port\}", "--name", "\{gguf\}"]
    args.server_arg |> push_from(more)
    return <- args
}

//! the runner's command lines for the bench and for each kind of server, from flags and a config
def private runner_lines_cell(t : T?; dir, config_path : string) {
    var config : BenchConfig
    t |> success(read_config(config_path, config), "the config reads")
    var flags <- RunnerArgs(quick = true, max_length = 4096, cell_seconds = 30, reps = 2, config = "c.json")
    var arm <- bench_argv(flags, "e", "stem", "dflash2", 8911, 4)
    t |> success(find(join(arm, " "), "--model e --out stem.bench.json --raw stem.requests.jsonl --arm dflash2 --quick --streams 4 --max-length 4096 --cell-seconds 30 --reps 2 --config c.json") >= 0, "an arm's bench: {join(arm, " ")}")
    var wide <- bench_argv(flags, "e", "stem", "wide", 8911, 0)
    var plain <- bench_argv(flags, "e", "stem", "off", 8911, 0)
    var long_plain <- bench_argv(flags, "e", "stem", "off", 8911, 0, true)
    t |> success(find(join(wide, " "), "--wide") >= 0 && find(join(wide, " "), "--arm") < 0 && find(join(wide, " "), "--streams") < 0, "the wide pass, no stream cap at 0")
    t |> success(find(join(plain, " "), "--arm") < 0 && find(join(plain, " "), "--wide") < 0 && find(join(long_plain, " "), "--arm off") >= 0, "the plain ladder, and the arms' plain rows")
    var ours <- arm_flags(config, "x", "D.gguf", false)
    var theirs <- arm_flags(config, "x", "D.gguf", true)
    var none <- arm_flags(config, "off", "D.gguf", false)
    var unknown <- arm_flags(config, "absent", "D.gguf", false)
    t |> success(join(ours, " ") == "--arm-mark" && join(theirs, " ") == "--spec D.gguf" && empty(none) && empty(unknown), "an arm's flags a side, the drafter filled in; none for off and for an arm the table lacks")
    var named <- entries_of(config, flags.model)
    t |> equal(join(named, " "), "stub", "every model of the config when none is named")
    let plan = ServerPlan(ctx = 7, streams = 3)
    var extra <- ["--e"]
    var asked <- RunnerArgs(server_engine = "other")
    asked.server_arg <- ["srv", "-m", "\{gguf\}", "-p", "\{port\}", "-c", "\{ctx\}", "-n", "\{streams\}"]
    var other <- server_of(asked, "g.gguf", 9, plan, extra)
    t |> success(line_of(other.argv) == "srv -m g.gguf -p 9 -c 7 -n 3 --e" && other.engine == "other" && !other.asks_stop, "another server's line, its placeholders filled: {line_of(other.argv)}")
    var tree <- RunnerArgs(server_tree = dir, gpu = "cuda")
    var missing <- server_of(tree, "g.gguf", 9, plan, extra)
    t |> success(empty(missing.argv), "a tree with no binary gives no line")
    mkdir_rec(path_join(dir, "bin"))
    fwrite(path_join(dir, "bin/daslang"), "x")
    var built <- server_of(tree, "g.gguf", 9, plan, extra)
    t |> success(find(line_of(built.argv), "-jit utils/dasllama-server/main.das -- -m g.gguf -p 9 --gpu cuda --ctx 7 --streams 3 --e") >= 0 && built.asks_stop, "ours: {line_of(built.argv)}")
    var ref <- RunnerArgs(llama_server = path_join(dir, "bin/daslang"))
    var reference <- server_of(ref, "g.gguf", 9, plan, extra)
    t |> success(find(line_of(reference.argv), "-m g.gguf --port 9 -ngl 99 -np 3 -c 21 --temp 0 --e") >= 0 && reference.engine == "llama.cpp" && reference_side(ref), "the reference: one context for its three slots: {line_of(reference.argv)}")
    t |> equal(line_of(["a b", "c"]), "\"a b\" c", "an argument with a blank is quoted")
    delete flags
    delete arm
    delete wide
    delete plain
    delete long_plain
    delete ours
    delete theirs
    delete none
    delete unknown
    delete named
    delete extra
    delete asked
    delete other
    delete tree
    delete missing
    delete built
    delete ref
    delete reference
    delete config
}

//! two bench run files joined by `take_run`
def private runner_join_cell(t : T?; dir : string) {
    var first : RunOut
    first.tests |> insert("tg3", TestRow(tok_s = 10.0lf))
    first.bench.cmd = "one"
    var second : RunOut
    second.tests |> insert("tg3:x", TestRow(tok_s = 20.0lf))
    second.refused |> push(Refusal(cell = "pp400", why = "down"))
    second.bench.cmd = "two"
    let fa = path_join(dir, "a.bench.json")
    let fb = path_join(dir, "b.bench.json")
    t |> success(fwrite(fa, sprint_json(first, false)) && fwrite(fb, sprint_json(second, false)), "two run files written")
    var all : RunOut
    t |> success(take_run(fa, all) && take_run(fb, all), "both read")
    t |> success(length(all.tests) == 2 && length(all.refused) == 1 && all.bench.cmd == "one ; two", "their rows, refusals and command lines ({all.bench.cmd})")
    t |> success(!take_run(path_join(dir, "absent.bench.json"), all), "control: a file that is not there joins nothing")
    delete first
    delete second
    delete all
}

//! a model run end to end against the fake as a child: its plain and arm starts, one card
def private runner_card_cell(t : T?; dir, config_path : string) {
    var config : BenchConfig
    read_config(config_path, config)
    var sized <- ["--sized"]
    var args <- runner_args(dir, config_path, sized)
    let long_log = path_join(dir, "stub.long.requests.jsonl")
    fwrite(long_log, "a line an earlier run left\n")
    t |> equal(run_models(args, config), 0, "the model ran")
    let card = fread(path_join(dir, "stub.card.json"))
    t |> success(find(card, "\"tg3\"") >= 0 && find(card, "\"pp400\"") >= 0 && find(card, "\"tg3:x\"") >= 0 && find(card, "\"tg3+400:x\"") >= 0, "the card carries the ladder's rows and the arm's")
    t |> success(find(card, "\"engine\":\"fake\"") >= 0 || find(card, "\"engine\": \"fake\"") >= 0, "... under the engine the flags name")
    t |> success(fexist(path_join(dir, "stub.server.log")) && fexist(path_join(dir, "stub.x.server.log")) && fexist(path_join(dir, "stub.long.bench.json")), "a log a server start, and the arms' plain rows' run file")
    t |> success(empty(server_models(RUNNER_TEST_PORT)), "no server is left on the port")
    t |> success(find(fread(long_log), "an earlier run left") < 0 && find(fread(long_log), "\"cell\"") >= 0, "the long plain run's request log holds this run's turns alone")
    delete sized
    delete args
    delete config
}

//! the runs a model fails: a server that exits, one that names another model, a taken port, a missing file
def private runner_failures_cell(t : T?; dir, config_path : string) {
    var config : BenchConfig
    read_config(config_path, config)
    var leaves <- ["--exit"]
    var gone <- runner_args(dir, config_path, leaves)
    t |> equal(run_models(gone, config), 1, "a server that exits before it answers")
    var renamed <- runner_args(dir, config_path, leaves)
    renamed.server_arg |> pop()
    renamed.server_arg |> pop()
    renamed.server_arg |> push_from(["other.gguf", "--sized"])
    t |> equal(run_models(renamed, config), 1, "a server whose list names another model")
    t |> success(empty(server_models(RUNNER_TEST_PORT)), "... and neither is left on the port")
    var absent <- runner_args(dir, config_path, leaves)
    absent.model |> clear()
    absent.model |> push("nofile")
    t |> equal(run_models(absent, config), 1, "a model whose file is not there")
    with_test_server(type<FakeChatServer>) $(taken_url) {
        var busy <- runner_args(dir, config_path, leaves)
        busy.port = to_int(slice(taken_url, find(taken_url, ":", 6) + 1))
        t |> equal(run_models(busy, config), 1, "a port something already answers on")
        delete busy
    }
    delete leaves
    delete gone
    delete renamed
    delete absent
    delete config
}

//! a folder of its own for a runner test, the stub model's and its drafter's files in it
def private runner_test_dir(name : string) : string {
    let dir = path_join(test_temp_dir(), "{name}_{ref_time_ticks()}")
    mkdir_rec(dir)
    fwrite(path_join(dir, "stub.gguf"), "x")
    fwrite(path_join(dir, "d.gguf"), "x")
    return dir
}

def private runner_test_done(dir, config_path : string) {
    remove(config_path)
    var err = ""
    rmdir_rec(dir, err)
}

[test(description = "The model bench runner builds each server's and the bench's command lines from its flags and the config, and joins the bench's run files")]
def test_model_bench_runner(t : T?) {
    let dir = runner_test_dir("model_bench_runner")
    let config_path = bench_test_config(t, "model_bench_runner")
    t |> run("the command lines") @(t : T?) {
        runner_lines_cell(t, dir, config_path)
    }
    t |> run("two run files join into one") @(t : T?) {
        runner_join_cell(t, dir)
    }
    runner_test_done(dir, config_path)
}

[test(description = "Against the fake chat server as a child process, the model bench runner runs a model's plain and arm starts into one card carrying every row", timeout = 1200)]
def test_model_bench_runner_card(t : T?) {
    let dir = runner_test_dir("model_bench_runner_card")
    let config_path = bench_test_config(t, "model_bench_runner_card")
    runner_card_cell(t, dir, config_path)
    runner_test_done(dir, config_path)
}

[test(description = "Against the fake chat server as a child process, the model bench runner fails a model whose server exits, names another model, finds its port taken, or whose file is missing", timeout = 1200)]
def test_model_bench_runner_failures(t : T?) {
    let dir = runner_test_dir("model_bench_runner_failures")
    let config_path = bench_test_config(t, "model_bench_runner_failures")
    runner_failures_cell(t, dir, config_path)
    runner_test_done(dir, config_path)
}
