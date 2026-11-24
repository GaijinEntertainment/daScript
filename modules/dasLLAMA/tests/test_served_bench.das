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
require ../harness/_model_bench_report.das   // its page
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

//! the rules of the bench's own passes: the eyeball's checks, the ultralong pass's lengths and corpus
def private rules_passes_cell(t : T?) {
    t |> run("the eyeball's checks: the count runs on in order, a reply holds one of its words, an unserved turn names its failure") @(t : T?) {
        t |> success(count_runs_on("6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20"), "the count from six")
        t |> success(count_runs_on("1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11"), "the count from one, the reply opening on the sequence given")
        t |> success(count_runs_on("6 7 8 9 10 11 1"), "the last number, with no character after it, may be cut: not read")
        t |> success(!count_runs_on("6, 7, 8, 9"), "a count short of ten")
        t |> success(!count_runs_on("6, 7, 9, 10, 11"), "a number skipped")
        t |> success(!count_runs_on("2, 3, 4, 5, 6, 7, 8, 9, 10, 11"), "a count from neither one nor six")
        t |> success(!count_runs_on("the sky is blue"), "no number at all")
        let sky = EYEBALL_ASKS[0]
        let served = BenchTurn(ttft_us = 1000l, answer = "Because of RAYLEIGH scattering.")
        t |> equal(eyeball_why(sky, served), "", "a word of the ask in any case")
        t |> success(find(eyeball_why(sky, BenchTurn(ttft_us = 1000l, answer = "one two three")), "holds none of") >= 0, "a reply with none of the words")
        let thought = BenchTurn(ttft_us = 1000l, completion_tokens = 48l, reply_head = "Rayleigh scattering, so one sentence")
        t |> success(find(eyeball_why(sky, thought), "48 tokens were reasoning") >= 0, "a reply that never left its reasoning, the ask's word in the reasoning or not")
        t |> equal(text_head("abc", 8), "abc", "a text under the limit, whole")
        t |> equal(text_head("ab\xd0\xb1c", 3), "ab", "a cut inside a two-byte character steps back to its first byte")
        t |> equal(text_head("ab\xd0\xb1c", 4), "ab\xd0\xb1", "a cut after it keeps it")
        t |> equal(eyeball_why(sky, BenchTurn(error = "http 500 down")), "http 500 down", "a turn that was not served")
        t |> success(find(eyeball_why(EYEBALL_ASKS[1], BenchTurn(ttft_us = 1000l, answer = "6, 6, 6")), "do not run on") >= 0, "the count's own check")
        let alone = row_kinds_of("eyeball")
        t |> success(!alone.k.pp && !alone.k.tg && !alone.k.width && empty(alone.unknown), "the eyeball is a kind of --only that names no timed cell")
    }
    t |> run("the ultralong pass: its lengths under a context, and a corpus repeated to a prompt's length") @(t : T?) {
        var at_128k <- ultralong_lengths(131072)
        var at_256k <- ultralong_lengths(262144)
        var at_32k <- ultralong_lengths(32768)
        var at_none <- ultralong_lengths(400)
        t |> success(length(at_128k) == 2 && at_128k[0] == 65536 && at_128k[1] == 130560, "a context of 131072: 65536 and the context less 512")
        t |> success(length(at_256k) == 3 && at_256k[1] == 131072 && at_256k[2] == 261632, "a context of 262144 holds the 131072 row too")
        t |> success(length(at_32k) == 1 && at_32k[0] == 32256, "a context under 65536: the context less 512 alone")
        t |> success(empty(at_none), "a context under the tail holds no row")
        let twice = repeated_corpus("alpha", 9)
        t |> success(length(twice) >= 9 && starts_with(twice, "alpha") && find(twice, "copy 2") > 0 && find(twice, "copy 3") < 0, "the corpus, then one marked copy: {twice}")
        t |> equal(repeated_corpus("alpha", 3), "alpha", "a corpus long enough is itself")
        delete at_128k
        delete at_256k
        delete at_32k
        delete at_none
    }
    t |> run("the behind pass: its bases under a context, a base with no id, a timed message under one, and the turn its cell refuses") @(t : T?) {
        var at_140k <- behind_lengths(140000)
        var at_70k <- behind_lengths(70144)
        var at_short <- behind_lengths(36000)
        t |> success(length(at_140k) == 4 && at_140k[0] == 32768 && at_140k[2] == 131072 && at_140k[3] == 135392, "a context of 140000 holds every base and the deepest that fits: {at_140k}")
        t |> success(length(at_70k) == 2 && at_70k[1] == 65536, "a base needs the timed turn's 4096 and the tail's 512 after it: 70144 just holds 65536, once")
        t |> success(length(at_short) == 1 && at_short[0] == 31392, "a context of 36000 holds its deepest base alone: {at_short}")
        var at_none <- behind_lengths(4000)
        t |> success(empty(at_none), "a context under the timed turn holds no base")
        delete at_none
        let corpus = "def alpha():\n    return 1\ndef beta():\n    return 2\n"
        t |> equal(behind_base(corpus, 12), behind_base(corpus, 12), "a base is the same text on any run")
        t |> success(find(behind_base(corpus, 12), "Reference id") < 0 && ends_with(behind_base(corpus, 12), "def alpha():"), "it carries no id and 12 characters of the corpus: {behind_base(corpus, 12)}")
        let extra = behind_timed_message(corpus, "n7", 26, 13)
        t |> success(starts_with(extra, "Reference id n7 ") && find(extra, "def beta():\n ") > 0 && find(extra, "def alpha") < 0 && find(extra, "The task: ") > 0, "a timed message leads with its id, cuts the corpus from its offset and ends on the task: {extra}")
        t |> success(find(behind_timed_message(corpus, "n8", 1000, 13), "return 2") > 0, "an offset past the corpus takes its last characters")
        let served = BenchTurn(ttft_us = 1000l, gen_us = 5000l, prompt_tokens = 140000l, cached_tokens = 135000l, completion_tokens = 128l)
        t |> equal(refusal_of(served, 128, true), "", "a turn that prefilled 5000 tokens past the cache enters its cell")
        var cold = served
        cold.cached_tokens = 100l
        t |> success(find(refusal_of(cold, 128, true), "the base did not come off the cache: 139900 of 140000") >= 0, "a turn whose base was prefilled is refused: {refusal_of(cold, 128, true)}")
        var cut = served
        cut.completion_tokens = 40l
        t |> success(find(refusal_of(cut, 128, true), "ended at 40 of 128") >= 0, "a reply cut short is refused")
        t |> success(find(refusal_of(BenchTurn(error = "http 500 down"), 128, true), "http 500 down") >= 0, "a turn that was not served is refused by its error")
        var mute = served
        mute.prompt_tokens = 0l
        t |> success(find(refusal_of(mute, 128, true), "no usage chunk") >= 0, "a turn with no usage chunk is refused")
        var single = served
        single.completion_tokens = 1l
        t |> success(find(refusal_of(single, 1, true), "no decode span") >= 0, "a reply of one token is refused")
        delete at_140k
        delete at_70k
        delete at_short
    }
}

//! the rules test's cells of a run's settings and the runner's pass flags
def private rules_settings_cell(t : T?) {
    t |> run("a run's settings: the prefill rows alone cut the reply and drop the task-alone rung, the width rows alone take the width lengths, an entry's ultralong server lies over the defaults'") @(t : T?) {
        let path = path_join(test_temp_dir(), "model_bench_settings_{ref_time_ticks()}.json")
        let text = (replace(replace(BENCH_TEST_CONFIG, "\"reply_tokens\":3,", "\"reply_tokens\":128,"), "\"wide_server\":", "\"ultralong_server\":\{\"ctx\":1000,\"streams\":1},\"wide_server\":")
            |> replace("\"stub\":\{", "\"stub\":\{\"ultralong_server\":\{\"ctx\":2000},"))
        t |> success(fwrite(path, text), "the config is written")
        var all <- run_settings(BenchArgs(config = path, entry = "stub"), RowKinds(pp = true, tg = true, width = true))
        var pp <- run_settings(BenchArgs(config = path, entry = "stub"), RowKinds(pp = true))
        var wd <- run_settings(BenchArgs(config = path, entry = "stub"), RowKinds(width = true))
        var long_pass <- run_settings(BenchArgs(config = path, entry = "stub", ultralong = 1700), RowKinds(pp = true, tg = true, width = true))
        t |> success(empty(all.unfit) && all.s.reply_tokens == 128 && "{all.s.lengths}" == "{[0, 400, 800]}", "control: a run of every kind keeps the config's reply and ladder ({all.s.lengths})")
        t |> success(pp.s.reply_tokens == PP_ONLY_REPLY && "{pp.s.lengths}" == "{[400, 800]}", "the prefill rows alone: a reply of {PP_ONLY_REPLY} tokens, no task-alone rung ({pp.s.reply_tokens}, {pp.s.lengths})")
        t |> success(wd.s.reply_tokens == 128 && "{wd.s.lengths}" == "{[0, 400]}", "the width rows alone climb the width lengths ({wd.s.lengths})")
        t |> success("{long_pass.s.lengths}" == "{[1188]}" && long_pass.s.cell_seconds == ULTRALONG_CELL_SECONDS, "the ultralong pass: its own lengths and a cell's hour ({long_pass.s.lengths})")
        t |> success(all.s.ultralong_server.ctx == 2000 && all.s.ultralong_server.streams == 1, "the entry's ultralong context over the defaults', the defaults' streams kept ({all.s.ultralong_server.ctx}, {all.s.ultralong_server.streams})")
        remove(path)
        delete long_pass
        delete wd
        delete pp
        delete all
    }
    t |> run("the runner's pass flags: the one-stream passes pin our draft head off, the behind pass names the slot folder, a reference server gets no flag of ours") @(t : T?) {
        let out = path_join(test_temp_dir(), "model_bench_pass_flags_{ref_time_ticks()}")
        var behind, ultra, off, theirs : array<string>
        pass_flags(RunnerArgs(), "behind", out, behind)
        pass_flags(RunnerArgs(), "ultralong", out, ultra)
        pass_flags(RunnerArgs(), "off", out, off)
        pass_flags(RunnerArgs(llama_server = "llama-server", slot_dir = "slots-here"), "behind", out, theirs)
        t |> equal(join(behind, " "), "--mtp=false --slot-save-path {path_join(out, "slots")}", "the behind pass on ours")
        t |> success(stat(path_join(out, "slots")).is_dir, "... its slot folder made")
        t |> equal(join(ultra, " "), "--mtp=false", "the ultralong pass on ours")
        t |> success(empty(off), "control: a plain start adds nothing")
        t |> equal(join(theirs, " "), "--slot-save-path slots-here", "the behind pass on the reference: the named slot folder alone")
        var err = ""
        rmdir_rec(out, err)
        delete behind
        delete ultra
        delete off
        delete theirs
    }
}

[test(description = "The model bench's prompt leads with an id it names as no part of the task, a turn is refused for each reason it cannot enter a cell, the eyeball's checks pass a count that runs on in order and a reply holding one of its ask's words and fail the rest, the ultralong pass names its lengths under a context and repeats a short corpus, a model's settings lay its entry over the defaults, and the row flags fire on a noisy, fallen, stalled, unscaled or unfair row")]
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
    t |> run("a wave's streams each take a task of their own, every one asking for code alone") @(t : T?) {
        var seen : table<string>
        for (i in range(32)) {
            let task = wave_task(i)
            t |> success(!key_exists(seen, task), "stream {i}'s task is no earlier stream's: {task}")
            t |> success(ends_with(task, "no introduction.") && find(task, "at least 600 lines") >= 0, "stream {i} asks for a long reply of code alone")
            seen |> insert(task)
        }
        t |> success(find(wave_task(0), "Python") >= 0 && find(wave_task(0), "token-bucket") >= 0, "stream 0 builds the one-stream rows' subject in their language")
        t |> success(ends_with(prompt_of("x", "n3", 0, wave_task(5)), wave_task(5)), "a wave prompt ends on its stream's task")
        t |> success(ends_with(prompt_of("x", "n4", 0), TASK), "control: a prompt with no task named ends on the one-stream task")
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
    rules_settings_cell(t)
    t |> run("--only names the row kinds: every kind where it names none, its first word that is no kind reported") @(t : T?) {
        let all = row_kinds_of("")
        t |> success(all.k.pp && all.k.tg && all.k.width && empty(all.unknown), "no word: every kind")
        let two = row_kinds_of("pp,width")
        t |> success(two.k.pp && !two.k.tg && two.k.width && empty(two.unknown), "pp,width: those two and not the decode rows")
        let bad = row_kinds_of("tg,rows,more")
        t |> success(bad.k.tg && !bad.k.pp && !bad.k.width && bad.unknown == "rows", "the first word that is no kind: {bad.unknown}")
        t |> success(pp_only_reply(128) == PP_ONLY_REPLY && pp_only_reply(3) == 3, "the prefill rows alone cut a long reply and keep a shorter one")
    }
    rules_passes_cell(t)
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
            t |> success(cell.done_ms > 0.0 && cell.done_ms < 30000.0 && cell.done_ms > cell.last_ttft_ms, "the wave's end is past its last first text, inside the turn's 30 s: the last stream's reply is in it ({cell.done_ms} ms over {cell.last_ttft_ms})")
            t |> success(lone.done_ms < cell.done_ms + cell.last_ttft_ms, "control: a wave of one ends no later than the wave of eight's end and wait together ({lone.done_ms} ms)")
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

//! the driver's eyeball against the fake server at `base_url`: the replies a plain run and an arm's run ask for, and a
//! server whose replies read wrong
def private driver_eyeball_cell(t : T?; base_url, config : string) {
    var plain : RunOut
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", only = "eyeball"), plain), 0, "the eyeball alone: every reply reads right")
    var modes <- [for (r in plain.eyeball); "{r.mode} {r.ask}"]
    sort(modes)
    t |> equal(join(modes, ", "), "@2 count, @2 sky, @4 capital, @4 count, @4 sky, @4 sum, plain count, plain sky", "one stream's two asks, a wave of two, a wave of four")
    t |> success(empty(plain.tests) && empty(plain.refused), "... and no timed cell ran")
    var drift : RunOut
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized-fakedrift", config = config, entry = "stub", only = "eyeball"), drift), 0, "a wave's reply worded apart from the one-stream reply still reads right")
    var noted <- [for (r in drift.eyeball); "{r.mode} {r.ask}"; where !empty(r.note)]
    var plain_noted <- [for (r in plain.eyeball); r.ask; where !empty(r.note)]
    t |> equal(join(noted, ", "), "@2 sky, @4 sky", "... and says it is not the one-stream reply")
    t |> success(empty(plain_noted), "control: a server that words a reply alike in every mode notes nothing")
    delete noted
    delete plain_noted
    delete drift
    var two : RunOut
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", only = "eyeball", streams = 2), two), 0)
    t |> equal(length(two.eyeball), 4, "a server of two streams gets no wave of four")
    var arm : RunOut
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", only = "eyeball", arm = "x"), arm), 0)
    t |> success(length(arm.eyeball) == 2 && arm.eyeball[0].mode == ":x" && arm.eyeball[1].ask == "count", "an arm's run: the two asks at one stream, on the speculative path")
    var long_plain : RunOut
    run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", arm = "off"), long_plain)
    t |> success(empty(long_plain.eyeball), "the arms' plain rows ride a server the plain run has read: no eyeball")
    var wrong : RunOut
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized-fakewrongeyes", config = config, entry = "stub"), wrong), 1, "a server whose replies read wrong")
    t |> equal(length(wrong.refused), 8, "every reply is a refusal of the run")
    t |> success(empty(wrong.tests) && find(wrong.bench.stopped, "at the eyeball") >= 0, "... which stops ahead of its first timed cell: {wrong.bench.stopped}")
    t |> success(find(wrong.eyeball[0].why, "holds none of") >= 0 && wrong.eyeball[0].reply == "one two three", "a wrong reply carries its reason and its text: {wrong.eyeball[0].why}")
    delete modes
    delete plain
    delete two
    delete arm
    delete long_plain
    delete wrong
}

//! the driver's --only and --lengths runs against the fake server at `base_url`
def private driver_only_cell(t : T?; base_url, config : string) {
    var pp : RunOut
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", only = "pp"), pp), 0)
    var pp_names <- row_names(pp)
    t |> equal(join(pp_names, " "), "pp400 pp800", "the prefill rows, no task-alone turn and no decode row")
    t |> equal(pp.bench.reply_tokens, 3, "... the config's reply, already under the cut")
    var tg : RunOut
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", only = "tg"), tg), 0)
    var tg_names <- row_names(tg)
    t |> equal(join(tg_names, " "), "tg3 tg3+400 tg3+800", "the one-stream decode rows at the config's reply")
    var wd : RunOut
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", only = "width"), wd), 0)
    var wd_names <- row_names(wd)
    t |> equal(join(wd_names, " "), "pp400@2 tg3+400@2 tg3@2", "the waves alone: their one-stream turns run at the width lengths and record no row")
    var ln : RunOut
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", lengths = "0,800"), ln), 0)
    var ln_names <- row_names(ln)
    t |> equal(join(ln_names, " "), "pp800 tg3 tg3+800 tg3@2", "the named lengths as the ladder; the width rows at the one width length it reached")
    var none : RunOut
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", only = "rows"), none), 1, "a word that is no kind")
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", only = "pp", arm = "x"), none), 1, "an arm's run takes the eyeball alone")
    t |> success(empty(none.tests), "... and neither ran a cell")
    var long_run : RunOut
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", ultralong = 1700), long_run), 0, "the ultralong pass at a context of 1700")
    var long_names <- row_names(long_run)
    t |> equal(join(long_names, " "), "pp1188 tg3+1188", "the context less 512 alone, one stream: no width row")
    t |> equal(long_run.bench.cell_seconds, ULTRALONG_CELL_SECONDS, "... a cell given its hour")
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", ultralong = 1700, arm = "x"), none), 1, "the ultralong pass is a run of its own")
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", ultralong = 1700, wide = true), none), 1, "... not the wide pass's")
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", ultralong = 1700, only = "pp"), none), 1, "... and takes no --only")
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", wide = true, only = "pp"), none), 1, "the wide pass takes no --only")
    t |> success(empty(none.tests) && empty(none.eyeball), "... and none of them sent a turn")
    let raw = path_join(test_temp_dir(), "model_bench_only_pp_{ref_time_ticks()}.jsonl")
    var quiet, whole : RunOut
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", only = "pp", raw = raw), quiet), 0)
    let pp_alone = fread(raw)
    remove(raw)
    t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", raw = raw), whole), 0)
    t |> success(count(pp_alone, "\"cell\":\"short\"") == 0 && count(fread(raw), "\"cell\":\"short\"") > 0, "the prefill rows alone send no task-alone turn; control: a whole run does")
    remove(raw)
    delete quiet
    delete whole
    delete long_names
    delete long_run
    delete pp_names
    delete tg_names
    delete wd_names
    delete ln_names
    delete pp
    delete tg
    delete wd
    delete ln
    delete none
}

[test(description = "Against a fake server whose prompt tokens follow the request's size, a model bench run climbs its config's lengths and widths into named rows, stops at --max-length, records an arm's rows and the plain rows under --arm off, runs the row kinds --only names and the lengths --lengths names and refuses a word that is no kind, opens on an eyeball whose every reply it checks and stops at one that reads wrong, runs the ultralong pass's one row under a context, sends no wave wider than the server's streams in the wide pass, and exits 1 on a config that does not read, a failed calibration and a refused cell", timeout = 1200)]
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
        t |> run("--only: the prefill rows alone, the decode rows alone, the width rows over the width lengths; --lengths names the ladder") @(t : T?) {
            driver_only_cell(t, base_url, config)
        }
        t |> run("the eyeball: obvious asks at one stream, in waves and on an arm, a wrong reply stopping the run") @(t : T?) {
            driver_eyeball_cell(t, base_url, config)
        }
        t |> run("the wide pass: the wide widths alone, none wider than the server's streams") @(t : T?) {
            var res : RunOut
            t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", wide = true, streams = 2), res), 0)
            var names <- row_names(res)
            t |> equal(join(names, " "), "pp400@2 tg3+400@2 tg3@2", "a wave of two; four streams would time the server's queue, and the one-stream cells are its base alone")
            t |> success(empty(res.eyeball), "the wide pass asks no eyeball: the plain start of the same server did")
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

[test(description = "Against the fake server the behind pass prefills a base cold once and saves it, restores it on the next run in place of the prefill, and reports the timed rows over the tokens past the cache with the cache's share beside them; a server whose slot routes refuse still reports its rows with no slot row, a turn whose base did not come off the cache refuses its cell, and the pass takes no arm")]
def test_model_bench_behind(t : T?) {
    let config = bench_test_config(t, "model_bench_behind")
    with_test_server(type<FakeChatServer>) $(base_url) {
        let tag = "behind{ref_time_ticks()}"
        var first : RunOut
        t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", behind = 20000, lengths = "6000", slot_tag = tag), first), 0, "the first run")
        var first_names <- row_names(first)
        t |> equal(join(first_names, " "), "cold6000 pp4096+6000 save6000 tg3+4096+6000", "the base prefilled cold and saved, then the timed rows")
        let timed = first.tests?["pp4096+6000"] ?? TestRow()
        t |> success(timed.cached_tokens > 5000l && timed.prompt_tokens - timed.cached_tokens < int64(BEHIND_FRESH_MAX * BEHIND_EXTRA), "the timed row names the base read off the cache ({timed.cached_tokens} of {timed.prompt_tokens})")
        t |> equal((first.tests?["save6000"] ?? TestRow()).prompt_tokens, 120l, "the save row carries the tokens the route answered")
        var second : RunOut
        t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", behind = 20000, lengths = "6000", slot_tag = tag), second), 0, "the second run")
        var second_names <- row_names(second)
        t |> equal(join(second_names, " "), "pp4096+6000 restore6000 tg3+4096+6000", "the base restored off its file: no cold prefill, no save")
        t |> equal((second.tests?["restore6000"] ?? TestRow()).tok_s, 120000.0lf, "the restore row is the route's tokens over its own clock")
        var refused : RunOut
        t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", behind = 20000, lengths = "6000", slot_tag = "noslot{tag}"), refused), 0, "a server whose slot routes refuse")
        var refused_names <- row_names(refused)
        t |> equal(join(refused_names, " "), "cold6000 pp4096+6000 tg3+4096+6000", "... still reports its rows, with no slot row")
        var cold : RunOut
        t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized fakecoldbase", config = config, entry = "stub", behind = 20000, lengths = "6000", slot_tag = "cold{tag}"), cold), 1, "a base that never comes off the cache")
        t |> success(length(cold.refused) == 1 && cold.refused[0].cell == "pp4096+6000" && find(cold.refused[0].why, "the base did not come off the cache") >= 0, "... refuses the timed cell by name: {cold.refused}")
        var unserved : RunOut
        t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized-fakefailreps", config = config, entry = "stub", behind = 20000, lengths = "6000", slot_tag = "unserved{tag}"), unserved), 1, "a server that fails the cold base turn")
        t |> success(length(unserved.refused) == 1 && unserved.refused[0].cell == "cold6000" && empty(unserved.tests), "... refuses the base's cell and times no turn after it: {unserved.refused}")
        delete unserved
        var none : RunOut
        t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", behind = 20000, wide = true), none), 1, "the behind pass is not the wide pass's")
        t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", behind = 20000, only = "pp"), none), 1, "... and takes no --only")
        t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", behind = 20000, arm = "x"), none), 1, "the behind pass is a run of its own")
        t |> equal(run_bench(BenchArgs(url = base_url, model_id = "fakesized", config = config, entry = "stub", behind = 20000, ultralong = 20000), none), 1, "... and not the ultralong pass's")
        delete none
        delete cold
        delete refused_names
        delete refused
        delete second_names
        delete second
        delete first_names
        delete first
    }
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
        t |> success(stub.known && drafter_of(stub.s, "x") == "d.gguf" && join(stub.s.arms, " ") == "off x" && stub.s.wide_server.streams == 4 && stub.s.eyeball_reply_tokens == 0, "the entry's drafter, the defaults' arms and wide server, no eyeball reply of its own")
        var over : BenchConfig
        t |> success(sscan_json("\{\"defaults\":\{\"lengths\":[0],\"widths\":[1],\"wide_widths\":[8],\"width_lengths\":[0],\"reply_tokens\":4,\"arm_reply_tokens\":9,\"reps\":1,\"cell_seconds\":5,\"cliff_share\":0.5,\"stall_times\":3,\"unfair_times\":2,\"arms\":[\"off\"],\"server\":\{\"ctx\":8,\"streams\":1,\"flags\":[\"--a\"]}},\"models\":\{\"m\":\{\"eyeball_reply_tokens\":96,\"lengths\":[0,16],\"widths\":[1,2],\"wide_widths\":[4],\"width_lengths\":[0,16],\"reply_tokens\":6,\"arm_reply_tokens\":12,\"cell_seconds\":7,\"cliff_share\":0.7,\"stall_times\":4,\"unfair_times\":3,\"arms\":[\"off\",\"y\"],\"drafters\":\{\"y\":\"y.gguf\"},\"server\":\{\"flags\":[\"--b\",\"--c\"]},\"wide_server\":\{\"streams\":8}}}}", over), "a config with every field overridden parses")
        var m <- settings_of(over, "m")
        t |> success(length(m.s.lengths) == 2 && length(m.s.widths) == 2 && m.s.wide_widths[0] == 4 && length(m.s.width_lengths) == 2, "its four lists")
        t |> success(m.s.reply_tokens == 6 && m.s.arm_reply_tokens == 12 && m.s.eyeball_reply_tokens == 96 && m.s.cell_seconds == 7 && m.s.cliff_share == 0.7 && m.s.stall_times == 4.0 && m.s.unfair_times == 3.0, "its scalars")
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

//! the runner's --only: what reaches each bench's command line, and which arms start
def private runner_only_cell(t : T?) {
    var picked <- RunnerArgs(only = "pp,long,dflash2", lengths = "0,16384")
    var picked_plain <- bench_argv(picked, "e", "stem", "off", 8911, 0)
    var picked_long <- bench_argv(picked, "e", "stem", "off", 8911, 0, true)
    var picked_arm <- bench_argv(picked, "e", "stem", "dflash2", 8911, 0)
    t |> success(find(join(picked_plain, " "), "--only pp --lengths 0,16384") >= 0, "--only's row kinds reach the plain start's main run, --lengths every run: {join(picked_plain, " ")}")
    t |> success(find(join(picked_long, " "), "--only") < 0 && find(join(picked_arm, " "), "--only") < 0 && find(join(picked_arm, " "), "--arm dflash2 --lengths 0,16384") >= 0, "the long-reply run and an arm's take the lengths and no kinds")
    t |> equal(bench_kinds_of("width,long,pp,x"), "width,pp", "the bench's kinds among the words")
    t |> success(arm_runs("", "off") && arm_runs("", "x"), "no --only: every arm starts")
    t |> success(arm_runs("pp", "off") && arm_runs("long", "off") && !arm_runs("x", "off"), "the plain start for a row kind or the long-reply rows, not for an arm's name")
    t |> success(arm_runs("x", "x") && !arm_runs("pp,long", "x"), "another arm by its name alone")
    t |> success(arm_runs("eyeball", "x") && arm_runs("eyeball", "off"), "the eyeball starts every arm")
    var eyes <- RunnerArgs(only = "eyeball")
    var eyes_arm <- bench_argv(eyes, "e", "stem", "dflash2", 8911, 0)
    var eyes_plain <- bench_argv(eyes, "e", "stem", "off", 8911, 0)
    t |> success(find(join(eyes_arm, " "), "--arm dflash2 --only eyeball") >= 0 && find(join(eyes_plain, " "), "--only eyeball") >= 0, "... each start's bench told to run it alone: {join(eyes_arm, " ")}")
    delete eyes
    delete eyes_arm
    delete eyes_plain
    var listed <- ["off", "x"]
    t |> equal(only_unknown("pp,tg,width,long,x", listed), "", "control: every word names something")
    t |> equal(only_unknown("pp,y,z", listed), "y", "the first word that is no kind, not long and no arm")
    t |> equal(only_unknown("off", listed), "off", "the plain start is named by its rows, not as an arm")
    t |> equal(only_unknown("ultralong", listed), "", "the ultralong pass is a word of --only")
    var bare : RunnerArgs
    var whole <- bench_argv(bare, "e", "stem", "ultralong", 8911, 1, false, 131072)
    t |> success(find(join(whole, " "), "--ultralong 131072 --streams 1") >= 0 && find(join(whole, " "), "--arm") < 0 && find(join(whole, " "), "--only") < 0, "the ultralong pass's bench takes its server's context and no kinds: {join(whole, " ")}")
    t |> equal(only_unknown("behind", listed), "", "the behind pass is a word of --only")
    var behind <- bench_argv(bare, "e", "dir/e.ref.behind", "behind", 8911, 1, false, 131072)
    t |> success(find(join(behind, " "), "--behind 131072 --slot-tag e.ref.behind") >= 0 && find(join(behind, " "), "--ultralong") < 0, "the behind pass's bench takes its server's context and its side's slot tag: {join(behind, " ")}")
    delete behind
    var shipped : BenchConfig
    t |> success(read_config(default_config_path(), shipped), "the shipped config reads")
    var e2b <- settings_of(shipped, "gemma-4-E2B-it-Q8_0")
    t |> success(e2b.s.ultralong_server.ctx == 131072 && e2b.s.ultralong_server.streams == 1, "a model takes the defaults' ultralong server: the whole context, one stream")
    delete e2b
    delete shipped
    delete whole
    delete bare
    delete listed
    delete picked
    delete picked_plain
    delete picked_long
    delete picked_arm
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
    runner_only_cell(t)
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

//! the lines of `page` that open a row named `name`, joined; "" where it has none
def private row_lines_of(page, name : string) : string {
    var lines <- split(page, "\n")
    var hits <- [for (l in lines); l; where starts_with(l, "| {name} ")]
    let text = join(hits, "\n")
    delete lines
    delete hits
    return text
}

//! a row's name read back, and the page of two sides: a cell of both, a row one side lacks, an arm against its plain row,
//! a refusal, the sections' order
def private runner_page_cell(t : T?) {
    let arm = row_key("tg512+4096:dflash2")
    t |> success(arm.known && arm.decode && arm.reply == 512 && arm.length == 4096 && arm.width == 1 && arm.arm == "dflash2", "an arm's decode row reads back")
    let wave = row_key("pp4096@4")
    t |> success(wave.known && !wave.decode && wave.length == 4096 && wave.width == 4 && empty(wave.arm), "a wave's prefill row reads back")
    t |> success(!row_key("warm").known && !row_key("pp12abc").known && !row_key("pp4096+x").known && !row_key("tg8@x").known && !row_key("restorex").known, "control: a name of another shape is not known")
    let behind_pp = row_key("pp4096+131072")
    t |> success(behind_pp.known && !behind_pp.decode && behind_pp.length == 4096 && behind_pp.behind == 131072 && !behind_pp.slot, "a behind pass's prefill row reads back its base")
    let behind_tg = row_key("tg128+4096+131072")
    t |> success(behind_tg.known && behind_tg.decode && behind_tg.reply == 128 && behind_tg.length == 4096 && behind_tg.behind == 131072, "and its decode row")
    let after = row_key("tg128+4096")
    t |> success(after.known && after.behind == 0 && after.length == 4096, "control: a decode row behind a plain prompt names no base")
    let restored = row_key("restore131072")
    t |> success(restored.known && restored.slot && restored.behind == 131072 && row_key("save65536").behind == 65536, "a slot route's row reads back its base")
    t |> success(row_key("cold32768").slot && row_key("cold32768").behind == 32768 && row_key("pp32768").behind == 0, "the cold base's row is the behind pass's, the ladder's row of that length is not")
    var ours : ReportSide
    ours.engine = "das"
    ours.run.tests |> insert("pp4096", TestRow(tok_s = 2069.4lf, prompt_tokens = 4104l))
    ours.run.tests |> insert("pp16384", TestRow(tok_s = 1527.2lf, prompt_tokens = 16379l))
    ours.run.tests |> insert("tg512", TestRow(tok_s = 20.0lf, ttft_ms = 110.0lf))
    ours.run.tests |> insert("tg512:x", TestRow(tok_s = 40.0lf, draft_n = 200l, draft_n_accepted = 100l, flags = "noisy"))
    ours.run.tests |> insert("tg128@2", TestRow(tok_s = 46.7lf, stream_tok_s = 23.5lf, done_ms = 7100.0lf))
    ours.run.tests |> insert("warm", TestRow(tok_s = 5.0lf))
    var theirs : ReportSide
    theirs.engine = "llama.cpp"
    theirs.run.tests |> insert("pp4096", TestRow(tok_s = 426.5lf, prompt_tokens = 4104l))
    theirs.run.tests |> insert("tg512", TestRow(tok_s = 24.0lf, ttft_ms = 320.0lf))
    theirs.run.tests |> insert("tg512:x", TestRow(tok_s = 36.0lf, draft_n = 400l, draft_n_accepted = 100l))
    ours.run.tests |> insert("tg128@4", TestRow(tok_s = 80.0lf, stream_tok_s = 20.0lf, done_ms = 7100.0lf))
    ours.run.tests |> insert("pp4096@4", TestRow(tok_s = 4000.0lf, prompt_tokens = 4104l))
    theirs.run.tests |> insert("tg128@4", TestRow(tok_s = 90.0lf, stream_tok_s = 22.5lf, done_ms = 9000.0lf))
    theirs.run.refused |> push(Refusal(cell = "pp16384", why = "past the cell's time | cut"))
    ours.run.tests |> insert("pp4096+131072", TestRow(tok_s = 3000.0lf, prompt_tokens = 135200l, cached_tokens = 131100l, flags = "noisy"))
    theirs.run.tests |> insert("pp4096+131072", TestRow(tok_s = 2000.0lf, prompt_tokens = 135300l, cached_tokens = 131200l, flags = "cliff"))
    ours.run.tests |> insert("tg128+4096+131072", TestRow(tok_s = 30.0lf, prompt_tokens = 135200l, ttft_ms = 1500.0lf))
    theirs.run.tests |> insert("tg128+4096+131072", TestRow(tok_s = 40.0lf, prompt_tokens = 135300l, ttft_ms = 2500.0lf, flags = "noisy"))
    ours.run.tests |> insert("restore131072", TestRow(tok_s = 500000.0lf, prompt_tokens = 131100l))
    ours.run.tests |> insert("cold131072", TestRow(tok_s = 1200.0lf, prompt_tokens = 131100l))
    let page = model_report("stub", ours, theirs)
    let deep = row_lines_of(page, "pp4096+131072")
    t |> success(find(deep, "| 135200 ") >= 0 && find(deep, "| 131100 / 131200 ") >= 0 && find(deep, "| 3000 / 2000 ") >= 0 && find(deep, "| 1.50 ") >= 0, "a row behind a long context: its prompt tokens, each side's tokens off the cache, the rates ({deep})")
    t |> success(find(deep, "| das: noisy; llama.cpp: cliff ") >= 0, "... both sides' flags, each under its name")
    t |> success(find(row_lines_of(page, "tg128+4096+131072"), "| llama.cpp: noisy ") >= 0 && find(row_lines_of(page, "tg128+4096+131072"), "| 1.50 / 2.50 ") >= 0, "the reference's flags alone under its name; the time to the first text")
    t |> success(find(row_lines_of(page, "cold131072"), "| 1200 / - ") >= 0 && find(row_lines_of(page, "restore131072"), "| 500000 / - ") >= 0, "the base's own rows: the cold prefill and the restore, each its rate")
    t |> success(find(page, "## Behind a long context") > find(page, "## Speculative") && find(page, "off the cache") >= 0, "the section after the arms', its cache column named")
    var eyes : ReportSide
    eyes.engine = "das"
    eyes.run.eyeball <- [EyeballReply(mode = "plain", ask = "sky", reply = "scatter"),
        EyeballReply(mode = "@2", ask = "count", why = "the numbers stop at 4", reply = "1 2 3 4")]
    var no_side : ReportSide
    let eyed = model_report("stub", eyes, no_side)
    t |> success(find(row_lines_of(eyed, "eyeball replies, wrong"), "| 2, 1 ") >= 0 && find(row_lines_of(eyed, "eyeball replies, wrong"), "| 0, 0 ") >= 0, "each side's eyeball replies and how many read wrong")
    t |> success(find(eyed, "eyeball @2 count") >= 0 && find(eyed, "the numbers stop at 4") >= 0 && find(eyed, "eyeball plain sky") < 0, "a wrong reply among the refusals; control: a right one is not")
    delete eyes
    delete no_side
    t |> success(find(page, "`das / llama.cpp`") >= 0, "the page says which side a cell names first")
    let pp = row_lines_of(page, "pp4096")
    t |> success(find(pp, "| 2069 / 426.5 ") >= 0 && find(pp, "| 4.85 ") >= 0 && find(pp, "| 4104 ") >= 0, "a prefill row: ours / theirs, ours over theirs, its prompt tokens ({pp})")
    t |> success(find(row_lines_of(page, "pp16384"), "| 1527 / - ") >= 0, "a row the reference did not run names it with a dash")
    let tg = row_lines_of(page, "tg512")
    t |> success(find(tg, "| 20.0 / 24.0 ") >= 0 && find(tg, "| 0.83 ") >= 0 && find(tg, "| 0.11 / 0.32 ") >= 0, "a decode row and its time to the first text ({tg})")
    let spec = row_lines_of(page, "tg512:x")
    t |> success(find(spec, "| 40.0 / 36.0 ") >= 0 && find(spec, "| 2.00x / 1.50x ") >= 0 && find(spec, "| 50% / 25% ") >= 0 && find(spec, "| das: noisy ") >= 0, "an arm's row: its rate over the plain row of its reply, accepted of drafted, its flags under its side ({spec})")
    let two = row_lines_of(page, "tg128@2")
    t |> success(find(two, "| 36.1 / - ") >= 0 && find(two, "| 23.5 / - ") >= 0 && find(two, "| 7.10 / - ") >= 0, "a wave's reply row: 256 reply tokens over the wave's 7.1 s, one stream's rate, the wave's end ({two})")
    let four = row_lines_of(page, "tg128@4")
    t |> success(find(four, "| 72.1 / 56.9 ") >= 0 && find(four, "| 1.27 ") >= 0 && find(four, "| 7.10 / 9.00 ") >= 0, "the wave that ends first leads, its streams' summed rate under the reference's or not ({four})")
    t |> success(find(row_lines_of(page, "pp4096@4"), "| 4000 / - ") >= 0, "control: a wave's prompt row keeps the bench's own rate")
    t |> success(find(row_lines_of(page, "pp4096"), "7.10") < 0, "control: a one-stream row carries no wave's end")
    t |> success(find(page, "| llama.cpp | pp16384 | past the cell's time \\| cut |") >= 0, "a refused cell under its side, a bar in its reason escaped")
    let at = [find(page, "## Prefill"), find(page, "## Decode"), find(page, "## Streams"), find(page, "## Speculative"), find(page, "## Other rows"), find(page, "## Refused")]
    t |> success(at[0] >= 0 && at[0] < at[1] && at[1] < at[2] && at[2] < at[3] && at[3] < at[4] && at[4] < at[5], "the sections in order, a name of another shape under the other rows")
    t |> success(find(page, "| pp4096 ") < find(page, "| pp16384 "), "the rows by prompt length, not by spelling")
    var empty_side : ReportSide
    let alone = model_report("stub", ours, empty_side)
    t |> success(find(alone, "`das / reference`") >= 0 && find(alone, "## Refused") < 0 && find(row_lines_of(alone, "pp4096"), "| 2069 / - ") >= 0, "control: one side alone - no reference rate, no refusals' table")
    delete ours
    delete theirs
    delete empty_side
}

//! a folder's run files as the model's page: both sides' plain files and an arm's, and a model with no file
def private runner_page_files_cell(t : T?; dir, config_path : string) {
    var config : BenchConfig
    read_config(config_path, config)
    var picked <- settings_of(config, "stub")
    var plain : RunOut
    plain.tests |> insert("tg3", TestRow(tok_s = 10.0lf))
    plain.bench.reps = 7
    plain.bench.cmd = "plain"
    var spec : RunOut
    spec.bench.cmd = "arm"
    spec.tests |> insert("tg3:x", TestRow(tok_s = 30.0lf))
    var ref : RunOut
    ref.tests |> insert("tg3", TestRow(tok_s = 20.0lf))
    t |> success((fwrite(path_join(dir, "paged.bench.json"), sprint_json(plain, false)) && fwrite(path_join(dir, "paged.x.bench.json"), sprint_json(spec, false))
        && fwrite(path_join(dir, "paged.ref.bench.json"), sprint_json(ref, false))), "three run files written")
    t |> success(write_report(dir, "paged", picked.s), "the page writes")
    let page = fread(path_join(dir, "paged.md"))
    t |> success(find(row_lines_of(page, "tg3"), "| 10.0 / 20.0 ") >= 0 && find(row_lines_of(page, "tg3"), "| 0.50 ") >= 0, "both sides' plain rows in one cell")
    t |> success(find(row_lines_of(page, "tg3:x"), "| 30.0 / - ") >= 0 && find(row_lines_of(page, "tg3:x"), "| 3.00x / - ") >= 0, "the arm's file joins its side")
    t |> success(find(page, "`ours / reference`") >= 0 && find(row_lines_of(page, "reps a cell"), "| 7 ") >= 0, "sides with no card are named by their place; the first file's reps stand for the side")
    t |> success(!write_report(dir, "nofile", picked.s) && !fexist(path_join(dir, "nofile.md")), "control: a model with no run file gets no page")
    delete plain
    delete spec
    delete ref
    delete picked
    delete config
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
    let page = fread(path_join(dir, "stub.md"))
    t |> success(find(page, "`fake / reference`") >= 0 && !empty(row_lines_of(page, "pp400")) && !empty(row_lines_of(page, "tg3+400:x")), "the model's page beside the card: the ladder's rows and the arm's under the card's engine")
    t |> success(find(fread(long_log), "an earlier run left") < 0 && find(fread(long_log), "\"cell\"") >= 0, "the long plain run's request log holds this run's turns alone")
    var eyes <- runner_args(dir, config_path, sized)
    eyes.only = "eyeball"
    remove(path_join(dir, "stub.card.json"))
    t |> equal(run_models(eyes, config), 0, "the eyeball alone: every start asked and read right, a pass with no timed row")
    t |> success(!fexist(path_join(dir, "stub.card.json")), "... which writes no card")
    delete eyes
    delete sized
    delete args
    delete config
}

//! the runner's passes end to end against the fake as a child, under a config whose entry names an ultralong server: each
//! pass a start of its own that `--only` picks alone, and `--report` writing the page with no start
def private runner_passes_cell(t : T?; dir : string) {
    let config_path = path_join(dir, "passes.json")
    fwrite(config_path, replace(BENCH_TEST_CONFIG, "\"wide_server\":", "\"ultralong_server\":\{\"ctx\":12000,\"streams\":1},\"wide_server\":"))
    var config : BenchConfig
    t |> success(read_config(config_path, config), "the passes' config reads")
    var sized <- ["--sized"]
    var deep <- runner_args(dir, config_path, sized)
    deep.only = "ultralong"
    t |> equal(run_models(deep, config), 0, "the ultralong pass alone")
    t |> success(fexist(path_join(dir, "stub.ultralong.bench.json")) && find(fread(path_join(dir, "stub.card.json")), "\"pp11488\"") >= 0, "its run file, and the card's row at the context less 512")
    t |> success(!fexist(path_join(dir, "stub.server.log")) && !fexist(path_join(dir, "stub.x.server.log")), "--only ultralong starts neither the plain server nor an arm's")
    var behind <- runner_args(dir, config_path, sized)
    behind.only = "behind"
    t |> equal(run_models(behind, config), 0, "the behind pass alone")
    let card = fread(path_join(dir, "stub.card.json"))
    t |> success(fexist(path_join(dir, "stub.behind.bench.json")) && find(card, "\"cold7392\"") >= 0 && find(card, "\"save7392\"") >= 0 && find(card, "\"pp4096+7392\"") >= 0, "its run file, and the card's base and timed rows at the deepest base the context holds")
    t |> success(stat(path_join(dir, "slots")).is_dir, "the slot folder the pass names to its server, under the output folder")
    let page_path = path_join(dir, "stub.md")
    t |> success(find(fread(page_path), "## Behind a long context") >= 0, "the page gains the section of rows behind a long context")
    remove(page_path)
    var report <- runner_args(dir, config_path, sized)
    report.report = true
    t |> equal(run_models(report, config), 0, "--report")
    t |> success(find(fread(page_path), "## Behind a long context") >= 0 && !fexist(path_join(dir, "stub.server.log")), "... writes the page again from the folder's files and starts no server")
    t |> success(empty(server_models(RUNNER_TEST_PORT)), "no server is left on the port")
    delete report
    delete behind
    delete deep
    delete sized
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

[test(description = "The model bench runner builds each server's and the bench's command lines from its flags and the config, joins the bench's run files, and writes a model's page with each row's cell reading ours / the reference's")]
def test_model_bench_runner(t : T?) {
    let dir = runner_test_dir("model_bench_runner")
    let config_path = bench_test_config(t, "model_bench_runner")
    t |> run("the command lines") @(t : T?) {
        runner_lines_cell(t, dir, config_path)
    }
    t |> run("two run files join into one") @(t : T?) {
        runner_join_cell(t, dir)
    }
    t |> run("the page of two sides") @(t : T?) {
        runner_page_cell(t)
    }
    t |> run("a folder's run files as a page") @(t : T?) {
        runner_page_files_cell(t, dir, config_path)
    }
    runner_test_done(dir, config_path)
}

[test(description = "Against the fake chat server as a child process, the model bench runner runs a model's plain and arm starts into one card carrying every row, and writes the model's page beside it", timeout = 1200)]
def test_model_bench_runner_card(t : T?) {
    let dir = runner_test_dir("model_bench_runner_card")
    let config_path = bench_test_config(t, "model_bench_runner_card")
    runner_card_cell(t, dir, config_path)
    runner_test_done(dir, config_path)
}

[test(description = "Against the fake chat server as a child process, the model bench runner starts the ultralong pass and the behind pass each alone under --only, starting no arm, and --report writes the model's page from the folder's files with no start", timeout = 600)]
def test_model_bench_runner_passes(t : T?) {
    let dir = runner_test_dir("model_bench_runner_passes")
    runner_passes_cell(t, dir)
    runner_test_done(dir, path_join(dir, "passes.json"))
}

[test(description = "Against the fake chat server as a child process, the model bench runner fails a model whose server exits, names another model, finds its port taken, or whose file is missing", timeout = 1200)]
def test_model_bench_runner_failures(t : T?) {
    let dir = runner_test_dir("model_bench_runner_failures")
    let config_path = bench_test_config(t, "model_bench_runner_failures")
    runner_failures_cell(t, dir, config_path)
    runner_test_done(dir, config_path)
}
