options gen2
options persistent_heap
options gc

module _fake_chat_server public

require ../../../tests/dasHV/_dashv_test_common.das public   // HvWebServer and its routes, the dasHV suites' helper
require daslib/fio
require strings
require daslib/strings_boost

// The fake chat server of the served instruments' cells: the SSE lines it answers with and the server itself, in the test
// process (`with_test_server`) or as a child (`_fake_chat_root.das`).

let ROLE = "data: \{\"choices\":[\{\"index\":0,\"delta\":\{\"role\":\"assistant\"}}]}"
let USAGE = "data: \{\"choices\":[],\"usage\":\{\"prompt_tokens\":100,\"completion_tokens\":3,\"total_tokens\":103,\"prompt_tokens_details\":\{\"cached_tokens\":40}},\"timings\":\{\"ttft_ms\":12.5,\"gen_ms\":20}}"

let SEEN = "\{\"id\":\"c\",\"object\":\"chat.completion\",\"created\":0,\"model\":\"m\",\"choices\":[\{\"index\":0,\"message\":\{\"role\":\"assistant\",\"content\":\"cats\"},\"finish_reason\":\"length\"}],\"usage\":\{\"prompt_tokens\":300,\"completion_tokens\":1,\"total_tokens\":301}}"

def private piece(key, text : string) : string => "data: \{\"choices\":[\{\"index\":0,\"delta\":\{\"{key}\":\"{text}\"}}]}"

var private g_fake_name = "fake"   //! the one id the model list answers
var private g_fake_sized = false    //! every request is the `fakesized` model's
var private g_fake_word_ms = 100u   //! the pause ahead of every word of a reply

//! what a process serving the fake sets before it starts: its model list's id, whether every request is sized, and
//! the pause ahead of a word
def set_fake_chat(name : string; sized : bool; word_ms : int = 100) {
    g_fake_name = name
    g_fake_sized = sized
    g_fake_word_ms = uint(word_ms)
}

//! a chat server that answers by the request's "model" field: `fail` every turn an error, `fakefailreps` every turn but
//! an 8-token one, `notext`, `oneword`, `nousage`, `cached` (90 cached tokens), `fakenocache` (90 unless the request says
//! `cache_prompt` false), `drafts`, `fakestall` (a pause ahead of the last word), `fakellama` (llama.cpp's timing names),
//! `fakesized` (prompt tokens a quarter of the request's bytes)
class FakeChatServer : HvWebServer {
    def override onInit {
        GET("/v1/models") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> JSON("\{\"object\":\"list\",\"data\":[\{\"id\":\"{g_fake_name}\"}]}")
        }
        STREAM("/v1/chat/completions") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            let body = string(req.body)
            let warm = find(body, "\"max_tokens\":8") >= 0 || find(body, "\"max_tokens\": 8") >= 0
            if (find(body, "\"fail\"") >= 0 || (find(body, "fakefailreps") >= 0 && !warm)) {   // the model named fail, in either client's JSON spacing
                respond(server, writer, int(http_status.INTERNAL_SERVER_ERROR), "application/json", "\{\"error\":\"down\"}")
                return
            }
            if (find(body, "image_url") >= 0 || find(body, "input_audio") >= 0) {   // the media question: one buffered completion, the part's rows in its prompt count
                respond(server, writer, int(http_status.OK), "application/json", find(body, "\"nousage\"") >= 0 ? replace(SEEN, "\"prompt_tokens\":300", "\"prompt_tokens\":0") : SEEN)
                return
            }
            sse_event(server, writer, slice(ROLE, 6), "")
            let oneword = find(body, "\"model\":\"oneword\"") >= 0
            if (find(body, "\"model\":\"notext\"") < 0) {
                for (word in ["one", " two", " three"]) {
                    sleep(word == " three" && find(body, "fakestall") >= 0 ? 700u : g_fake_word_ms)
                    sse_event(server, writer, slice(piece("content", word), 6), "")
                    break if (oneword)
                }
            }
            if (find(body, "\"model\":\"nousage\"") < 0) {
                var usage = oneword ? replace(USAGE, "\"completion_tokens\":3", "\"completion_tokens\":1") : USAGE
                if (find(body, "\"model\":\"cached\"") >= 0) {
                    usage = replace(usage, "\"cached_tokens\":40", "\"cached_tokens\":90")
                }
                if (find(body, "fakenocache") >= 0 && find(body, "\"cache_prompt\":false") < 0 && find(body, "\"cache_prompt\": false") < 0) {
                    usage = replace(usage, "\"cached_tokens\":40", "\"cached_tokens\":90")
                }
                if (g_fake_sized || find(body, "fakesized") >= 0) {
                    usage = replace(replace(usage, "\"prompt_tokens\":100", "\"prompt_tokens\":{length(body) / 4}"), "\"cached_tokens\":40", "\"cached_tokens\":0")
                }
                if (find(body, "fakellama") >= 0) {
                    usage = replace(usage, "\"ttft_ms\":12.5,\"gen_ms\":20", "\"prompt_ms\":12.5,\"predicted_ms\":20")
                }
                if (find(body, "\"model\":\"drafts\"") >= 0) {
                    usage = replace(usage, "\"timings\":\{", "\"timings\":\{\"draft_n\":8,\"draft_n_accepted\":5,")
                }
                sse_event(server, writer, slice(usage, 6), "")
            }
            sse_event(server, writer, "[DONE]", "")
            close_writer(server, writer)
        }
    }
}
