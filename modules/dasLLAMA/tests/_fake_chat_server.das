options gen2
options persistent_heap
options gc

module _fake_chat_server public

require ../../../tests/dasHV/_dashv_test_common.das public   // HvWebServer and its routes, the dasHV suites' helper
require daslib/fio
require strings
require math   // max
require daslib/strings_boost
require daslib/json_boost   // a slot route's body

// The fake chat server of the served instruments' cells: the SSE lines it answers with and the server itself, in the test
// process (`with_test_server`) or as a child (`_fake_chat_root.das`).

let ROLE = "data: \{\"choices\":[\{\"index\":0,\"delta\":\{\"role\":\"assistant\"}}]}"
let USAGE = "data: \{\"choices\":[],\"usage\":\{\"prompt_tokens\":100,\"completion_tokens\":3,\"total_tokens\":103,\"prompt_tokens_details\":\{\"cached_tokens\":40}},\"timings\":\{\"ttft_ms\":12.5,\"gen_ms\":20}}"

let SEEN = "\{\"id\":\"c\",\"object\":\"chat.completion\",\"created\":0,\"model\":\"m\",\"choices\":[\{\"index\":0,\"message\":\{\"role\":\"assistant\",\"content\":\"cats\"},\"finish_reason\":\"length\"}],\"usage\":\{\"prompt_tokens\":300,\"completion_tokens\":1,\"total_tokens\":301}}"

def private piece(key, text : string) : string => "data: \{\"choices\":[\{\"index\":0,\"delta\":\{\"{key}\":\"{text}\"}}]}"

var private g_fake_name = "fake"   //! the one id the model list answers
var private g_fake_sized = false    //! every request is the `fakesized` model's
var private g_fake_word_ms = 100u   //! the pause ahead of every word of a reply
var private g_fake_sky_asks = 0     //! the eyeball's sky asks answered: `fakedrift` ends each reply on its count

//! what a process serving the fake sets before it starts: its model list's id, whether every request is sized, and
//! the pause ahead of a word
def set_fake_chat(name : string; sized : bool; word_ms : int = 100) {
    g_fake_name = name
    g_fake_sized = sized
    g_fake_word_ms = uint(word_ms)
}

//! which of the model bench's eyeball asks a request body carries: the sky 0, the count 1, the capital 2, the sum 3; -1 for none
def private eyeball_ask_of(body : string) : int {
    for (mark, at in ["sky blue", "Count from 1 to 20", "capital of France", "2 + 2"], count()) {
        return at if (find(body, mark) >= 0)
    }
    return -1
}

//! a reply's three words: a working model's to eyeball ask `at`, the stock ones to any other request (-1)
def private eyeball_words(at : int) : array<string> {
    if (at == 0) {
        return <- ["Rayleigh", " scattering", " wins"]
    } elif (at == 1) {
        return <- ["1, 2, 3, 4,", " 5, 6, 7, 8,", " 9, 10, 11"]
    } elif (at == 2) {
        return <- ["Paris", " of", " course"]
    } elif (at == 3) {
        return <- ["4", " is", " it"]
    }
    return <- ["one", " two", " three"]
}

var private g_fake_slots : table<string>   //! the slot files this server's life saved

//! a slot route's answer to `action` over the request `body`: a save of any file name but one holding "noslot" is kept and
//! answered, a restore answers a name this life saved, anything else is a 400
def private fake_slot_answer(action, body : string; var status : http_status&) : string {
    var err = ""
    var js = read_json(body, err)
    let name = js?["filename"] ?? ""
    unsafe {
        delete js
    }
    status = http_status.OK
    if (action == "save" && !empty(name) && find(name, "noslot") < 0) {
        g_fake_slots |> insert(name)
        return "\{\"id_slot\":0,\"filename\":\"{name}\",\"n_saved\":120,\"n_written\":480,\"timings\":\{\"save_ms\":2.0}}"
    }
    if (action == "restore" && key_exists(g_fake_slots, name)) {
        return "\{\"id_slot\":0,\"filename\":\"{name}\",\"n_restored\":120,\"n_read\":480,\"timings\":\{\"restore_ms\":1.0}}"
    }
    status = http_status.BAD_REQUEST
    return "\{\"error\":\{\"message\":\"slot 0 did not {action}: '{name}'\"}}"
}

//! the prompt tokens a sized turn reads off the cache: the request's bytes ahead of a replayed assistant message, four a
//! token - none under `fakecoldbase`, none where the request says `cache_prompt` false, none for a turn that replays nothing
def private fake_replayed_tokens(body : string) : int {
    let at = max(find(body, "\"role\":\"assistant\""), find(body, "\"role\": \"assistant\""))   // either client's JSON spacing
    let uncached = find(body, "fakecoldbase") >= 0 || find(body, "\"cache_prompt\":false") >= 0 || find(body, "\"cache_prompt\": false") >= 0
    return at < 0 || uncached ? 0 : at / 4
}

//! a chat server that answers by the request's "model" field: `fail` every turn an error, `fakefailreps` every turn but
//! an 8-token one, `notext`, `oneword`, `nousage`, `cached` (90 cached tokens), `fakenocache` (90 unless the request says
//! `cache_prompt` false), `drafts`, `fakestall` (a pause ahead of the last word), `fakellama` (llama.cpp's timing names),
//! `fakesized` (prompt tokens a quarter of the request's bytes); the model bench's eyeball asks get a working model's replies,
//! but under `fakewrongeyes`, `fakefailreps` serves them, and `fakedrift` words every sky reply apart from the one before
class FakeChatServer : HvWebServer {
    def override onInit {
        POST("/slots/0") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            var status = http_status.OK
            let text = fake_slot_answer(get_param(req, "action"), string(req.body), status)
            return resp |> JSON(text, status)
        }
        GET("/v1/models") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> JSON("\{\"object\":\"list\",\"data\":[\{\"id\":\"{g_fake_name}\"}]}")
        }
        STREAM("/v1/chat/completions") <| @(var req : HttpRequest?; var writer : HttpResponseWriter?) {
            let body = string(req.body)
            let warm = find(body, "\"max_tokens\":8") >= 0 || find(body, "\"max_tokens\": 8") >= 0
            let sees = find(body, "fakewrongeyes") >= 0 ? -1 : eyeball_ask_of(body)
            if (find(body, "\"fail\"") >= 0 || (find(body, "fakefailreps") >= 0 && !warm && sees < 0)) {   // the model named fail, in either client's JSON spacing
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
                var inscope words <- eyeball_words(sees)
                if (sees == 0 && find(body, "fakedrift") >= 0) {
                    words[2] = " wins {g_fake_sky_asks++}"
                }
                for (word in words) {
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
                    usage = replace(replace(usage, "\"prompt_tokens\":100", "\"prompt_tokens\":{length(body) / 4}"), "\"cached_tokens\":40", "\"cached_tokens\":{fake_replayed_tokens(body)}")
                }
                if (find(body, "fakellama") >= 0) {
                    usage = replace(usage, "\"ttft_ms\":12.5,\"gen_ms\":20", "\"prompt_ms\":12.5,\"predicted_ms\":20")
                }
                if (find(body, "\"model\":\"drafts\"") >= 0 || find(body, "fakedrafts") >= 0) {
                    usage = replace(usage, "\"timings\":\{", "\"timings\":\{\"draft_n\":8,\"draft_n_accepted\":5,")
                }
                sse_event(server, writer, slice(usage, 6), "")
            }
            sse_event(server, writer, "[DONE]", "")
            close_writer(server, writer)
        }
    }
}
