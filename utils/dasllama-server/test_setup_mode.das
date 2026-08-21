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

def private check_tower_routes(t : T?; base_url, cat_dir, asr_file : string) {
    POST("{base_url}/catalog/download", "\{\"name\":\"x\",\"tower\":\"bogus\"}") $(resp) {
        t |> success(resp != null && resp.status_code == http_status.BAD_REQUEST, "an unknown tower kind is a 400")
    }
    t |> success(!empty(cat_dir) && !empty(asr_file), "the catalog names its dir and the asr tower file")
    mkdir_rec(cat_dir)
    let planted_asr = path_join(cat_dir, asr_file)
    fopen(planted_asr, "wb") $(f) {
        if (f != null) {
            f |> fwrite("BIN")
        }
    }
    POST("{base_url}/catalog/download", "\{\"tower\":\"asr\"}") $(resp) {
        t |> success(resp != null && resp.status_code == http_status.CONFLICT, "a present asr tower refuses the re-download")
    }
    GET("{base_url}/catalog") $(resp) {
        var jerr = ""
        var js = read_json(resp != null ? string(resp.body) : "", jerr)
        t |> equal(js?["asr"]?["present"] ?? false, true, "the planted asr tower reads present")
        t |> equal(js?["asr"]?["path"] ?? "", planted_asr)
        unsafe {
            delete js
        }
    }
    remove(planted_asr)
    GET("{base_url}/favicon.ico") $(resp) {
        t |> success(resp != null && resp.status_code == http_status.OK, "the tray mark serves as the tab icon (404 = tray.ico missing)")
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
            POST("{base_url}/v1/models/load", "\{\"path\":\"whatever.gguf\"}") $(resp) {
                t |> success(resp != null && resp.status_code == http_status.CONFLICT, "runtime loads refuse setup mode (409)")
                t |> success(resp != null && find(string(resp.body), "serving server") >= 0, "the 409 names the serving-server precondition")
            }
            GET("{base_url}/v1/models") $(resp) {
                t |> success(resp != null && resp.status_code == http_status.OK, "models lists")
                t |> success(resp != null && find(string(resp.body), "\"data\":[]") >= 0, "empty served set")
            }
            var cat_dir = ""
            var asr_file = ""
            GET("{base_url}/catalog") $(resp) {
                t |> success(resp != null && resp.status_code == http_status.OK, "catalog answers")
                var jerr = ""
                var js = read_json(resp != null ? string(resp.body) : "", jerr)
                let entries = js?["entries"]
                t |> success(entries != null && (entries.value is _array)
                    && length(entries.value as _array) == length(model_catalog()), "every curated entry served")
                t |> equal(js?["download"]?["state"] ?? "", "idle")
                t |> success(int(js?["box"]?["ram_gb"] ?? -1l) >= 0, "the box RAM fact is present")
                t |> equal(js?["box"]?["vram_mb"] ?? -1l, 0l, "no armed tier reports no VRAM budget")
                t |> equal(js?["asr"]?["present"] ?? true, false, "the asr tower is absent in a fresh dir")
                cat_dir = js?["models_dir"] ?? ""
                asr_file = js?["asr"]?["file"] ?? ""
                unsafe {
                    delete js
                }
            }
            POST("{base_url}/catalog/download", "\{\"name\":\"no-such-entry\"}") $(resp) {
                t |> success(resp != null && resp.status_code == http_status.NOT_FOUND, "unknown catalog entry is a 404")
            }
            check_tower_routes(t, base_url, cat_dir, asr_file)
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
