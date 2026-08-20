options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

// Setup mode end to end, model-free: a slotless server boots, the page surface reports
// setup, every inference route fails closed, and the catalog routes serve.

require dastest/testing_boost public
require openai_server
require model_catalog
require dashv/dashv_boost
require daslib/jobque_boost
require daslib/json_boost
require daslib/fio
require strings

let TEST_PORT = 18135   // 18097/18099/18101/18120/18121/18131 belong to the sibling suites

// the test_openai_server harness minus the model: setup mode is exactly a slotless boot
var private g_picker_dir = ""   // planted per run, removed by with_setup_server's teardown

def private plant_picker_dir : string {
    var terr : string
    let tmp = temp_directory(terr)
    g_picker_dir = path_join(empty(tmp) ? "." : tmp, "dasllama_setup_picker_{ref_time_ticks()}")
    mkdir_rec(g_picker_dir)
    for (fname in ["planted.gguf", "planted.gguf.0xdead.dlim"]) {
        fopen(path_join(g_picker_dir, fname), "wb") $(f) {
            if (f != null) {
                f |> fwrite("GGUF")
            }
        }
    }
    return path_join(g_picker_dir, "planted.gguf")
}

def private remove_picker_dir {
    if (empty(g_picker_dir)) {
        return
    }
    for (fname in ["planted.gguf", "planted.gguf.0xdead.dlim"]) {
        remove(path_join(g_picker_dir, fname))
    }
    rmdir(g_picker_dir)
    g_picker_dir = ""
}

def private with_setup_server(port : int; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            set_setup_mode(true)
            var terr : string
            let tmp = temp_directory(terr)
            set_catalog_dir(path_join(empty(tmp) ? "." : tmp, "dasllama_setup_test_{ref_time_ticks()}"))
            let picker_gguf = plant_picker_dir()
            var surf <- {
                "config" => JV((port = port, model = "", streams = 4)),
                "sources" => JV((port = "cli")),
                "authoritative" => JV(false),
                "config_path" => JV(""),
                "save_path" => JV("")
            }
            var sdoc = JV(surf)
            set_config_surface(write_json_compact(sdoc), "", picker_gguf)
            unsafe {
                delete sdoc
            }
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
        remove_picker_dir()
        assert(ready, "setup-mode server became ready")
    }
}

[test]
def test_setup_mode(t : T?) {
    t |> run("a slotless boot serves setup stats, fail-closed inference, and the catalog") @(t : T?) {
        // no jit gate: setup mode never infers, so the interpreted tier serves it fine -
        // this is the one server suite CI runs END TO END rather than compile-only
        with_setup_server(TEST_PORT) $(base_url) {
            GET("{base_url}/v1/stats") $(resp) {
                t |> success(resp != null && resp.status_code == http_status.OK, "stats answers")
                var jerr = ""
                var js = read_json(resp != null ? string(resp.body) : "", jerr)
                t |> equal(js?["setup"] ?? false, true, "stats carries setup:true")
                t |> equal(js?["model"] ?? "", "(no model)")
                unsafe {
                    delete js
                }
            }
            POST("{base_url}/v1/chat/completions", "\{\"messages\":[\{\"role\":\"user\",\"content\":\"hi\"}]}") $(resp) {
                t |> success(resp != null && resp.status_code == http_status.NOT_FOUND, "chat fails closed (404), server survives")
            }
            GET("{base_url}/v1/models") $(resp) {
                t |> success(resp != null && resp.status_code == http_status.OK, "models lists")
                t |> success(resp != null && find(string(resp.body), "\"data\":[]") >= 0, "empty served set")
            }
            GET("{base_url}/catalog") $(resp) {
                t |> success(resp != null && resp.status_code == http_status.OK, "catalog answers")
                var jerr = ""
                var js = read_json(resp != null ? string(resp.body) : "", jerr)
                let entries = js?["entries"]
                t |> success(entries != null && (entries.value is _array)
                    && length(entries.value as _array) == length(model_catalog()), "every curated entry served")
                t |> equal(js?["download"]?["state"] ?? "", "idle")
                unsafe {
                    delete js
                }
            }
            POST("{base_url}/catalog/download", "\{\"name\":\"no-such-entry\"}") $(resp) {
                t |> success(resp != null && resp.status_code == http_status.NOT_FOUND, "unknown catalog entry is a 404")
            }
            GET("{base_url}/v1/stats") $(resp) {
                t |> success(resp != null && resp.status_code == http_status.OK, "still serving after the fail-closed round")
            }
            GET("{base_url}/config") $(resp) {
                t |> success(resp != null && resp.status_code == http_status.OK, "config answers")
                let body = resp != null ? string(resp.body) : ""
                t |> success(find(body, "planted.gguf") >= 0, "the picker lists the planted gguf")
                t |> success(find(body, ".dlim") < 0, "a .dlim beside it never enters the model picker")
            }
        }
    }
}
