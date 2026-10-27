options gen2
options persistent_heap   // the server thread loads and frees the models once
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

require dastest/testing_boost public
require openai_server                                   // configure_translators / add_asr_model / run_server / sentence_pieces (sibling)
require main                                            // translate_paths: the --translate / translate value read (sibling)
require _server_rig                                     // models_dir / model_missing / serve_clients (sibling)
require dashv/dashv_boost                               // with_http_request: the form and audio requests
require openai/openai_common                            // openai_client / post_json / get_json: the JSON requests
require daslib/jobque_boost                             // with_job_status / new_thread: the server thread
require daslib/json_boost
require daslib/fio
require strings
require daslib/strings_boost                            // join: the roster as the flag spells it

// Translation on the server: the LibreTranslate routes (POST /translate as JSON and as a form, a string `q` and an array,
// a repeated form `q` as an array, GET /languages, the 400s for an unserved pair, html, `auto` and a missing `q`) on two
// Bergamot students (ru-en and en-ru) and on the MiLMMT-46 translation LLM (its src == trg refusal); the translators on
// /v1/models and the `translate` block of /v1/stats; and /v1/audio/translations with `target` chaining GigaAM's Russian
// transcript through the ru-en student (json, text and verbose_json, with and without `language`), the 400s naming an
// unserved pair. Model-free: the sentence splitter, the flag's path list, a repeated id left out, and a server whose one
// translation model cannot load - the request refusals that come before any model (missing source and target, a
// non-string `q` element, the 20000-character limit over a repeated form `q`) and the 503 while no model is ready.
// The model cells (the bergamot base-memory students, MiLMMT-46-1B Q4_K_M, gigaam-v3-e2e-rnnt-f16 with the ru_short
// clip) skip without their files and run JIT-only.

let TEST_PORT = 18210   // 18210 .. 18213: this file's four boots

let RU_EN = path_join(models_dir(), "bergamot/ru-en-base-memory/model.ruen.intgemm.alphas.bin")
let EN_RU = path_join(models_dir(), "bergamot/en-ru-base-memory/model.enru.intgemm.alphas.bin")
let MILMMT = path_join(models_dir(), "milmmt/MiLMMT-46-1B-v0.1.Q4_K_M.gguf")
let GIGAAM = path_join(models_dir(), "gigaam-v3-e2e-rnnt-f16.gguf")
let RU_SHORT = "modules/dasLLAMA/benchmarks/asr/corpus/ru_short.wav"
let PHARMACY_RU = "Где ближайшая аптека?"

def private with_translate_server(port : int; paths : array<string>; asr : string; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        let roster = join(paths, ",")
        new_thread() <| @capture(= finished, = roster, = asr) {
            configure_translators(translate_paths(roster))
            add_asr_model(asr, "")
            run_server(port)   // blocks until POST /shutdown
            finished |> notify_and_release
        }
        let ready = serve_clients(port, 1200, "/v1/models", blk)   // up to ~120 s: the loads + the JIT
        finished |> join
        assert(ready, "the translation server became ready")
    }
}

//! POST /translate as the urlencoded form LibreTranslate's own page sends; `fields` in order, a name repeated as given
def private post_form(client : OpenAIClient; fields : array<tuple<string; string>>) : HttpOutcome {
    let body = join([for (f in fields); "{url_encode(f._0)}={url_encode(f._1)}"], "&")
    var out = HttpOutcome()
    with_http_request() $(var hreq : HttpRequest?#) {
        hreq.method = http_method.POST
        hreq.url := "{client.base_url}/translate"
        hreq.body := body
        set_content_type(hreq, "application/x-www-form-urlencoded")
        request(hreq) $(var resp : HttpResponse?) {
            if (resp != null) {
                out = HttpOutcome(status = int(resp.status_code), body = "{resp.body}")
            }
        }
    }
    return out
}

//! POST /v1/audio/translations with the clip and the form fields given
def private post_audio(url, clip : string; fields : array<tuple<string; string>>) : HttpOutcome {
    var out = HttpOutcome()
    with_http_request() $(var hreq : HttpRequest?#) {
        hreq.method = http_method.POST
        hreq.url := url
        set_form_file(hreq, "file", clip)
        for (f in fields) {
            set_form_data(hreq, f._0, f._1)
        }
        request(hreq) $(var resp : HttpResponse?) {
            if (resp != null) {
                out = HttpOutcome(status = int(resp.status_code), body = "{resp.body}")
            }
        }
    }
    return out
}

//! a JSON answer's string member, "" when the body or the member is not there
def private json_member(body, key : string) : string {
    var err = ""
    var js = read_json(body, err)
    let v = js?[key] ?? ""
    unsafe {
        delete js
    }
    return v
}

//! a JSON array's items as strings, none when `v` is not an array
def private string_items(v : JsonValue?) : array<string> {
    var out : array<string>
    if (v != null && (v.value is _array)) {
        out |> reserve(length(v.value as _array))
        for (e in v.value as _array) {
            out |> push(e ?? "")
        }
    }
    return <- out
}

//! a JSON answer's string-array member
def private json_member_list(body, key : string) : array<string> {
    var err = ""
    var js = read_json(body, err)
    var out <- string_items(js?[key])
    unsafe {
        delete js
    }
    return <- out
}

//! the codes GET /languages lists, each with its targets
def private language_targets(body : string) : table<string; array<string>> {
    var err = ""
    var js = read_json(body, err)
    var out : table<string; array<string>>
    if (js != null && (js.value is _array)) {
        for (row in js.value as _array) {
            out[row?["code"] ?? ""] <- string_items(row?["targets"])
        }
    }
    unsafe {
        delete js
    }
    return <- out
}

def private latin_letters(s : string) : int {
    var n = 0
    for (c in s) {
        if (c < 128 && is_alpha(c)) {   // the bytes of a Cyrillic letter sit above 127
            n++
        }
    }
    return n
}

[test]
def test_sentence_pieces(t : T?) {
    t |> run("a sentence ends at . ! ? before a space and at a newline; the separators are kept") @(t : T?) {
        var inscope pieces <- sentence_pieces("Hi there. How are you?  Fine!\nNext line\n\nLast")
        t |> equal(length(pieces), 5, "five pieces")
        t |> equal(pieces[0].sentence, "Hi there.", "the punctuation stays with its sentence")
        t |> equal(pieces[0].sep, " ", "a single space between sentences")
        t |> equal(pieces[1].sentence, "How are you?", "a question")
        t |> equal(pieces[1].sep, "  ", "a double space kept verbatim")
        t |> equal(pieces[2].sentence, "Fine!", "an exclamation")
        t |> equal(pieces[2].sep, "\n", "the newline is the separator")
        t |> equal(pieces[3].sep, "\n\n", "a blank line kept")
        t |> equal(pieces[4].sentence, "Last", "the tail with no end mark")
        t |> equal(pieces[4].sep, "", "no separator after the last")
    }
    t |> run("a dot inside a word or a number does not end a sentence") @(t : T?) {
        var inscope pieces <- sentence_pieces("Version 1.5 is out.")
        t |> equal(length(pieces), 1, "one piece")
        t |> equal(pieces[0].sentence, "Version 1.5 is out.", "the whole text")
    }
    t |> run("the --translate value is a comma-separated path list") @(t : T?) {
        var inscope paths <- translate_paths(" a.bin, ,b.gguf ")
        t |> equal(length(paths), 2, "blank entries dropped")
        t |> equal(paths[0], "a.bin", "trimmed")
        t |> equal(paths[1], "b.gguf", "in order")
    }
    t |> run("a translation model whose file is missing is left out") @(t : T?) {
        configure_translators([path_join(models_dir(), "no-such-translator.bin")])
        t |> success(!server_has_speech(), "nothing configured serves")
        configure_translators([])
    }
}

//! The refusals that come before any translation model, on a server whose one model cannot load (its bytes are not a
//! Marian model) - model-free: the request shape, the size limit counted over a repeated form `q`, then the 503 while no
//! model is ready; and a model named twice is left out the second time.
[test]
def test_refusals_with_no_model_ready(t : T?) {
    let dir = path_join(test_temp_dir(), "xx-en-broken")
    let broken = path_join(dir, "model.xxen.intgemm.alphas.bin")
    t |> success(mkdir_rec(dir) && fwrite(broken, "not a Marian model"), "planted the broken model")
    t |> run("a model named twice is left out the second time: its id is already served") @(t : T?) {
        configure_translators([broken, broken])
        t |> equal(length(g_translators), 1, "one slot")
        configure_translators([])
    }
    t |> run("the request refusals, then the 503 while no model is ready") @(t : T?) {
        with_translate_server(TEST_PORT + 3, [broken], "") $(base_url) {
            let client = openai_client(base_url)
            let big = repeat("a", 15000)
            for (refusal in [("\{\"q\":\"x\",\"target\":\"en\"}", "missing source"),
                             ("\{\"q\":\"x\",\"source\":\"ru\"}", "missing target"),
                             ("\{\"q\":[\"x\",7],\"source\":\"ru\",\"target\":\"en\"}", "missing q"),
                             ("\{\"q\":\"{repeat("a", 20001)}\",\"source\":\"ru\",\"target\":\"en\"}", "past the 20000")]) {
                let a = client |> post_json("/translate", refusal._0)
                t |> equal(a.status, 400, "{refusal._1}: 400")
                t |> success(find(json_member(a.body, "error"), refusal._1) >= 0, "the error names it: {tail_of(a.body)}")
            }
            let form = post_form(client, [("q", big), ("q", big), ("source", "ru"), ("target", "en")])
            t |> equal(form.status, 400, "a repeated form q counts every value toward the limit")
            t |> success(find(json_member(form.body, "error"), "30000 characters") >= 0, "both values counted: {tail_of(form.body)}")
            let none_ready = client |> post_json("/translate", "\{\"q\":\"x\",\"source\":\"xx\",\"target\":\"en\"}")
            t |> equal(none_ready.status, 503, "no model ready is a 503")
            t |> success(find(json_member(none_ready.body, "error"), "no translation model is ready") >= 0, "the error says so: {none_ready.body}")
            let langs = client |> get_json("/languages")
            t |> equal(strip(langs.body), "[]", "/languages lists nothing")
            var err = ""
            var js = read_json((client |> get_json("/v1/stats")).body, err)
            t |> success(!empty(js?["translate"]?["models"]?[0]?["error"] ?? ""), "the stats block carries the load failure")
            unsafe {
                delete js
            }
        }
    }
    remove(broken)
}

[test]
def test_libre_routes_on_bergamot(t : T?) {
    return if (model_missing(t, [RU_EN, EN_RU]))
    with_translate_server(TEST_PORT, [RU_EN, EN_RU], "") $(base_url) {
        let client = openai_client(base_url)
        let one = client |> post_json("/translate", "\{\"q\":\"{PHARMACY_RU}\",\"source\":\"ru\",\"target\":\"en\",\"api_key\":\"ignored\"}")
        t |> equal(one.status, 200, "a string q answers 200: {one.body}")
        let text = json_member(one.body, "translatedText")
        t |> success(find(to_lower(text), "pharmacy") >= 0, "the English names the pharmacy: '{text}'")
        let many = client |> post_json("/translate", "\{\"q\":[\"{PHARMACY_RU} Спасибо.\",\"Привет\"],\"source\":\"ru\",\"target\":\"en\"}")
        t |> equal(many.status, 200, "an array q answers 200: {many.body}")
        var inscope list <- json_member_list(many.body, "translatedText")
        t |> equal(length(list), 2, "one translation an input")
        if (length(list) == 2) {
            t |> success(find(to_lower(list[0]), "pharmacy") >= 0 && find(list[0], "? ") >= 0, "two sentences, the separator kept: '{list[0]}'")
            t |> success(latin_letters(list[1]) > 0, "the second input translated: '{list[1]}'")
        }
        let back = client |> post_json("/translate", "\{\"q\":\"Where is the nearest pharmacy?\",\"source\":\"en\",\"target\":\"ru\"}")
        t |> equal(back.status, 200, "the other student serves its own pair: {back.body}")
        t |> success(latin_letters(json_member(back.body, "translatedText")) == 0, "the Russian is Cyrillic: {back.body}")
        let form = post_form(client, [("q", PHARMACY_RU), ("source", "ru"), ("target", "en")])
        t |> equal(form.status, 200, "a urlencoded form answers 200: {form.body}")
        t |> success(find(to_lower(json_member(form.body, "translatedText")), "pharmacy") >= 0, "the form translates: {form.body}")
        let repeated = post_form(client, [("q", PHARMACY_RU), ("q", "Привет"), ("source", "ru"), ("target", "en")])
        t |> equal(repeated.status, 200, "a repeated form q answers 200: {repeated.body}")
        var inscope rlist <- json_member_list(repeated.body, "translatedText")
        t |> success(length(rlist) == 2 && find(to_lower(rlist[0]), "pharmacy") >= 0, "a repeated q is an array, one translation a value: {repeated.body}")
        let langs = client |> get_json("/languages")
        t |> equal(langs.status, 200, "/languages answers 200")
        var inscope targets <- language_targets(langs.body)
        t |> success(find_index(targets?["ru"] ?? [""], "en") >= 0 && find_index(targets?["en"] ?? [""], "ru") >= 0, "ru -> en and en -> ru listed: {langs.body}")
        t |> success(find(langs.body, "\"name\":\"Russian\"") >= 0, "a language carries its name: {langs.body}")
        for (refusal in [("\{\"q\":\"x\",\"source\":\"ru\",\"target\":\"de\"}", "ru -> de"),
                         ("\{\"q\":\"x\",\"source\":\"ru\",\"target\":\"en\",\"format\":\"html\"}", "html"),
                         ("\{\"q\":\"x\",\"source\":\"auto\",\"target\":\"en\"}", "detection"),
                         ("\{\"source\":\"ru\",\"target\":\"en\"}", "missing q")]) {
            let a = client |> post_json("/translate", refusal._0)
            t |> equal(a.status, 400, "{refusal._1}: 400")
            t |> success(find(json_member(a.body, "error"), refusal._1) >= 0, "the error names it: {a.body}")
        }
        let models = client |> get_json("/v1/models")
        t |> success(find(models.body, "\"ru-en-base-memory\"") >= 0 && find(models.body, "\"en-ru-base-memory\"") >= 0, "both students on /v1/models: {models.body}")
        let stats = client |> get_json("/v1/stats")
        var err = ""
        var js = read_json(stats.body, err)
        let tblock = js?["translate"]
        t |> success(tblock != null && !(tblock is _null), "the translate block on /v1/stats")
        t |> success((tblock?["done_jobs"] ?? 0l) >= 4l, "the jobs counted")
        t |> equal(tblock?["models"]?[0]?["source"] ?? "", "ru", "the first student's pair")
        t |> equal(tblock?["models"]?[0]?["kind"] ?? "", "marian", "a Bergamot student")
        unsafe {
            delete js
        }
    }
}

[test]
def test_libre_routes_on_milmmt(t : T?) {
    return if (model_missing(t, MILMMT))
    with_translate_server(TEST_PORT + 1, [MILMMT], "") $(base_url) {
        let client = openai_client(base_url)
        let one = client |> post_json("/translate", "\{\"q\":\"{PHARMACY_RU}\",\"source\":\"ru\",\"target\":\"en\"}")
        t |> equal(one.status, 200, "a string q answers 200: {one.body}")
        t |> success(find(to_lower(json_member(one.body, "translatedText")), "pharmacy") >= 0, "the English names the pharmacy: {one.body}")
        let many = client |> post_json("/translate", "\{\"q\":[\"{PHARMACY_RU}\",\"Where is the station?\"],\"source\":\"ru\",\"target\":\"en\"}")
        t |> equal(many.status, 200, "an array q answers 200: {many.body}")
        var inscope list <- json_member_list(many.body, "translatedText")
        t |> equal(length(list), 2, "one translation an input")
        let de = client |> post_json("/translate", "\{\"q\":\"Where is the station?\",\"source\":\"en\",\"target\":\"de\"}")
        t |> equal(de.status, 200, "any two of its languages: {de.body}")
        let langs = client |> get_json("/languages")
        var inscope targets <- language_targets(langs.body)
        t |> success(length(targets) >= 40, "every language the model knows is listed ({length(targets)})")
        t |> success(find_index(targets?["ru"] ?? [""], "en") >= 0, "ru -> en listed")
        let bogus = client |> post_json("/translate", "\{\"q\":\"x\",\"source\":\"en\",\"target\":\"xx\"}")
        t |> equal(bogus.status, 400, "a language the model does not know is an unserved pair")
        let same = client |> post_json("/translate", "\{\"q\":\"x\",\"source\":\"en\",\"target\":\"en\"}")
        t |> equal(same.status, 400, "a language into itself is an unserved pair: {same.body}")
        t |> success(find(json_member(same.body, "error"), "en -> en") >= 0, "the error names it: {same.body}")
    }
}

[test]
def test_audio_translation_target(t : T?) {
    return if (model_missing(t, [RU_EN, GIGAAM]))
    if (!stat(RU_SHORT).is_valid) {
        t |> skip("ru_short.wav not present")
        return
    }
    with_translate_server(TEST_PORT + 2, [RU_EN], GIGAAM) $(base_url) {
        let url = "{base_url}/v1/audio/translations"
        let heard = post_audio("{base_url}/v1/audio/transcriptions", RU_SHORT, [("language", "ru"), ("response_format", "json")])
        let transcript = json_member(heard.body, "text")
        t |> success(heard.status == 200 && !empty(transcript), "the clip transcribes: {heard.body}")
        let plain = post_audio(url, RU_SHORT, [("language", "ru"), ("target", "en"), ("response_format", "json")])
        t |> equal(plain.status, 200, "the chained translation answers 200: {plain.body}")
        let text = json_member(plain.body, "text")
        t |> success(latin_letters(text) * 2 > length(text), "the transcript '{transcript}' comes back in English: '{text}'")
        let verbose = post_audio(url, RU_SHORT, [("language", "ru"), ("target", "en"), ("response_format", "verbose_json")])
        t |> equal(verbose.status, 200, "verbose_json answers 200: {verbose.body}")
        var err = ""
        var js = read_json(verbose.body, err)
        t |> equal(js?["task"] ?? "", "translate", "the verbose task")
        t |> equal(js?["text"] ?? "", text, "the same translation in `text`")
        let seg = js?["segments"]?[0]
        t |> success(seg != null && !(seg is _null) && latin_letters(seg?["text"] ?? "") > 0, "the first segment keeps its timing and reads in English")
        unsafe {
            delete js
        }
        let as_text = post_audio(url, RU_SHORT, [("language", "ru"), ("target", "en"), ("response_format", "text")])
        t |> equal(as_text.status, 200, "text answers 200: {as_text.body}")
        t |> equal(strip(as_text.body), text, "the bare translation as the body")
        let heard_source = post_audio(url, RU_SHORT, [("target", "en"), ("response_format", "json")])
        t |> equal(heard_source.status, 200, "no language: the source is what the ASR model heard: {heard_source.body}")
        t |> equal(json_member(heard_source.body, "text"), text, "the same translation")
        let nobody_writes = post_audio(url, RU_SHORT, [("target", "de")])
        t |> equal(nobody_writes.status, 400, "no language and no model that writes the target is a 400")
        t |> success(find(nobody_writes.body, "into de") >= 0, "the 400 names the target: {nobody_writes.body}")
        let unserved = post_audio(url, RU_SHORT, [("language", "ru"), ("target", "de")])
        t |> equal(unserved.status, 400, "a pair no model serves is a 400")
        t |> success(find(unserved.body, "ru -> de") >= 0, "the 400 names the pair: {unserved.body}")
        let no_target = post_audio(url, RU_SHORT, [("language", "ru")])
        t |> equal(no_target.status, 400, "no target: the ASR model's own translation, which GigaAM's caps do not declare")
    }
}
