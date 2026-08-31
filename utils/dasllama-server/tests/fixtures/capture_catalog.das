options gen2
options persistent_heap
options stack = 524288   // every dasLLAMA program root takes this budget (options stack does not unify up from libs)

// The scripted regeneration rail for catalog_downloading.json / catalog_done.json: boot the
// real setup-mode server on a scratch catalog dir, download the SMALLEST card for real, and
// snapshot /catalog mid-flight and after it lands - bytes as served, machine-local paths
// normalized to the fixture convention (the one permitted edit). Run from the repo root:
//   bin/daslang -jit utils/dasllama-server/tests/fixtures/capture_catalog.das
// The download is a few GB from the pinned url; the scratch dir is deleted at the end.

require ../../openai_server.das
require ../../model_catalog.das
require dashv/dashv_boost
require daslib/jobque_boost
require daslib/json_boost
require daslib/fio
require strings
require daslib/strings_boost

let PORT = 18140   // the test suites own 18097..18135; this rail runs by hand, never in CI

let FIXTURE_DIR = "utils/dasllama-server/tests/fixtures"
let WIN_DIR_FILE = "C:\\\\Users\\\\user\\\\.dasllama\\\\models"   // as BYTES in the fixture (JSON-escaped)

def private normalize_paths(text, cat_dir : string) : string {
    // values arrive as JSON strings holding POSIX paths; the fixture convention is the
    // windows dir with escaped backslashes, so the dir prefix AND its separator both map
    var out = replace(text, "{cat_dir}/", "{WIN_DIR_FILE}\\\\")
    out = replace(out, cat_dir, WIN_DIR_FILE)
    return out
}

def private fetch_catalog(base_url : string) : string {
    var body = ""
    GET("{base_url}/catalog") $(resp) {
        if (resp != null && resp.status_code == http_status.OK) {
            body = clone_string(string(resp.body))
        }
    }
    return body
}

// the download.state + got fields of one /catalog poll
def private poll_state(base_url : string; var got : int64&) : tuple<state : string; body : string> {
    let body = fetch_catalog(base_url)
    var perr = ""
    var pj = read_json(body, perr)
    let state = "{pj?["download"]?["state"] ?? ""}"
    got = int64(pj?["download"]?["got"] ?? 0.0lf)
    unsafe {
        delete pj
    }
    return (state = state, body = body)
}

def private pick_total(body : string) : int64 {
    var perr = ""
    var pj = read_json(body, perr)
    let total = int64(pj?["download"]?["total"] ?? 0.0lf)
    unsafe {
        delete pj
    }
    return total > 0l ? total : 1l
}

// the smallest card - every entry reads absent against the scratch dir
def private pick_smallest(base_url : string) : string {
    var jerr = ""
    var js = read_json(fetch_catalog(base_url), jerr)
    var pick = ""
    var pick_bytes = 0l
    let entries = js?["entries"]
    if (entries != null) {
        for (e in entries.value as _array) {
            let b = int64(e?["bytes"] ?? 0.0lf)
            if (empty(pick) || b < pick_bytes) {
                pick = "{e?["name"] ?? ""}"
                pick_bytes = b
            }
        }
    }
    unsafe {
        delete js
    }
    to_log(LOG_INFO, "capture_catalog: smallest card is '{pick}' ({pick_bytes} bytes)\n")
    return pick
}

def private capture_both(base_url, cat_dir : string) {
    let pick = pick_smallest(base_url)
    if (empty(pick)) {
        to_log(LOG_ERROR, "capture_catalog: no catalog entries\n")
        return
    }
    POST("{base_url}/catalog/download", "\{\"name\":\"{pick}\"}") $(resp) {
        // the route answers 202 Accepted - the worker downloads on
        if (resp == null || (resp.status_code != http_status.OK && resp.status_code != http_status.ACCEPTED)) {
            to_log(LOG_ERROR, "capture_catalog: download refused ({resp != null ? int(resp.status_code) : 0})\n")
        }
    }
    var rolling = ""   // mid-flight snapshot: real bytes moving, state == downloading
    for (_attempt in range(6000)) {
        var got = 0l
        let p = poll_state(base_url, got)
        // wait past the first chunk: a 32KB snapshot pins 0% and the progress-render
        // assertion downstream loses its teeth - 5% is a real mid-flight picture
        if (p.state == "downloading" && got * 20l >= pick_total(p.body)) {
            rolling = p.body
            break
        }
        break if (p.state == "failed")
        sleep(100u)
    }
    if (empty(rolling)) {
        to_log(LOG_ERROR, "capture_catalog: never observed a mid-flight state\n")
        return
    }
    fwrite("{FIXTURE_DIR}/catalog_downloading.json", normalize_paths(rolling, cat_dir))
    to_log(LOG_INFO, "capture_catalog: catalog_downloading.json captured\n")
    var done = ""   // then let it land: the server's terminal state is "done", the entry present
    for (_attempt in range(36000)) {
        var got = 0l
        let p = poll_state(base_url, got)
        if (p.state == "done" || p.state == "idle") {
            done = p.body
            break
        }
        if (p.state == "failed") {
            to_log(LOG_ERROR, "capture_catalog: download FAILED - no done snapshot\n")
            break
        }
        sleep(500u)
    }
    if (!empty(done)) {
        fwrite("{FIXTURE_DIR}/catalog_done.json", normalize_paths(done, cat_dir))
        to_log(LOG_INFO, "capture_catalog: catalog_done.json captured\n")
    }
}

[export]
def main {
    var terr : string
    let tmp = temp_directory(terr)
    let cat_dir = path_join(empty(tmp) ? "." : tmp, "dasllama_capture_{ref_time_ticks()}")
    with_job_status(1) $(finished) {
        new_thread() @() {
            set_setup_mode(true)
            set_catalog_dir(cat_dir)
            var surf <- {
                "config" => JV((port = PORT, model = "", streams = 4)),
                "sources" => JV((port = "cli")),
                "authoritative" => JV(false),
                "config_path" => JV(""),
                "save_path" => JV("")
            }
            var sdoc = JV(surf)
            set_config_surface(write_json_compact(sdoc), "", "")
            unsafe {
                delete sdoc
            }
            run_server(PORT)   // blocks until POST /shutdown
            finished |> notify_and_release
        }
        let base_url = "http://127.0.0.1:{PORT}"
        var ready = false
        for (_attempt in range(300)) {
            if (!empty(fetch_catalog(base_url))) {
                ready = true
                break
            }
            sleep(100u)
        }
        if (ready) {
            capture_both(base_url, cat_dir)
        } else {
            to_log(LOG_ERROR, "capture_catalog: server never served /catalog\n")
        }
        POST("{base_url}/shutdown", "") $(_resp) {}
        finished |> join
    }
    rmdir_rec_result(cat_dir)   // the multi-GB scratch download goes with it
    to_log(LOG_INFO, "capture_catalog: done\n")
}
