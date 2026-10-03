options gen2
options persistent_heap
options gc

require dastest/testing_boost public
require daslib/jobque_boost
require daslib/fio
require strings
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

def private piece(key, text : string) : string => "data: \{\"choices\":[\{\"index\":0,\"delta\":\{\"{key}\":\"{text}\"}}]}"

class FakeChatServer : HvWebServer {
    def override onInit {
        STREAM("/v1/chat/completions") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            let body = string(req.body)
            if (find(body, "\"model\":\"fail\"") >= 0) {
                respond(server, writer, int(http_status.INTERNAL_SERVER_ERROR), "application/json", "\{\"error\":\"down\"}")
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
            return resp |> JSON("\{\"text\":\"hello world\"}")
        }
    }
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
