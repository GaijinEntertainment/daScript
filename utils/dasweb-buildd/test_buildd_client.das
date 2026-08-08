options gen2
options persistent_heap

require dastest/testing_boost public
require buildd_client
require buildd_core
require daslib/jobque_boost
require daslib/json_boost
require daslib/sha_256
require daslib/fio

let STUB_PORT = 19014   // 19011..19013 are dasweb-playground's

let STUB_TOKEN = "stub-token"
let TC_JOB = "feedfeedfeedfeedfeedfeedfeedfeedfeedfeed"
let JOB_HASH = "abababababababababababababababababababababababababababababababab"

// The upload landing path is computed the same way in the stub's context and
// the test's context — that file is how multipart bytes are verified across
// the thread boundary.
def private upload_landing : string {
    var err = ""
    return path_join(temp_directory(err), "dasweb_buildd_client_upload.bin")
}

def private stub_bearer_ok(var req : HttpRequest?) : bool {
    return get_header(req, "Authorization") == "Bearer {STUB_TOKEN}"
}

// Canned dasweb-playground build surface: enough contract to prove the client
// (bearer header, claim shapes, multipart result with sha verification).
class StubPlayground : HvWebServer {
    def override onInit {
        GET("/healthz") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("ok")
        }
        POST("/shutdown") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            g_stub_stop = true
            return resp |> TEXT_PLAIN("bye")
        }
        POST("/api/build/toolchain") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!stub_bearer_ok(req)) {
                return resp |> TEXT_PLAIN("unauthorized", http_status.UNAUTHORIZED)
            }
            if (req.body != TC_JOB) {
                return resp |> TEXT_PLAIN("bad toolchain", http_status.BAD_REQUEST)
            }
            return resp |> JSON("\{\"toolchain\":\"{TC_JOB}\"}")
        }
        POST("/api/build/next") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!stub_bearer_ok(req)) {
                return resp |> TEXT_PLAIN("unauthorized", http_status.UNAUTHORIZED)
            }
            if (req.body != TC_JOB) {
                return http_status.NO_CONTENT
            }
            return resp |> JSON(write_json(JV((hash = JOB_HASH, toolchain_id = TC_JOB,
                mode = "module", source = "print(42)"))))
        }
        POST("/api/build/result") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (!stub_bearer_ok(req)) {
                return resp |> TEXT_PLAIN("unauthorized", http_status.UNAUTHORIZED)
            }
            if (get_form_data(req, "hash") != JOB_HASH || get_form_data(req, "toolchain") != TC_JOB) {
                return resp |> TEXT_PLAIN("bad job key", http_status.BAD_REQUEST)
            }
            let ok_field = get_form_data(req, "ok")
            if (ok_field == "0") {
                if (empty(get_form_data(req, "error"))) {
                    return resp |> TEXT_PLAIN("missing error", http_status.BAD_REQUEST)
                }
                return resp |> TEXT_PLAIN("ok")
            }
            var jerr = ""
            var manifest = read_json(get_form_data(req, "manifest"), jerr)
            if (manifest == null || !(manifest.value is _array) || length(manifest.value as _array) != 1) {
                return resp |> TEXT_PLAIN("bad manifest", http_status.BAD_REQUEST)
            }
            let entry = (manifest.value as _array)[0]
            let name : string = entry?["name"] ?? ""
            let sha : string = entry?["sha256"] ?? ""
            if (name != "sample.wasm") {
                return resp |> TEXT_PLAIN("bad name", http_status.BAD_REQUEST)
            }
            let landing = upload_landing()
            if (save_form_file(req, name, landing) != 200) {
                return resp |> TEXT_PLAIN("missing part", http_status.BAD_REQUEST)
            }
            if (sha256_file_hex(landing) != sha) {
                return resp |> TEXT_PLAIN("sha mismatch", http_status.BAD_REQUEST)
            }
            return resp |> TEXT_PLAIN("ok")
        }
    }
}

var private g_stub_stop = false

def private run_stub(port : int) {
    var server = new StubPlayground()
    server->init(port)
    server->set_bind_host("127.0.0.1")
    server->start()
    while (!g_stub_stop) {
        server->tick()
        sleep(2u)
    }
    server->stop()
    server->cleanup()
    unsafe {
        delete server
    }
}

def private with_stub(blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            run_stub(STUB_PORT)
            finished |> notify_and_release
        }
        let base_url = "http://127.0.0.1:{STUB_PORT}"
        var ready = false
        for (_attempt in range(100)) {
            GET("{base_url}/healthz") $(resp) {
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
        assert(ready, "stub playground became ready")
    }
}

[test]
def test_claim_and_announce(t : T?) {
    with_stub() $(base_url) {
        t |> run("claim parses the job payload") @(tt : T?) {
            let job = claim_build(base_url, STUB_TOKEN, TC_JOB)
            tt |> success(job.ok, "claimed")
            tt |> equal(job.hash, JOB_HASH)
            tt |> equal(job.toolchain, TC_JOB)
            tt |> equal(job.mode, "module")
            tt |> equal(job.source, "print(42)")
        }
        t |> run("an idle queue (204) claims nothing") @(tt : T?) {
            let job = claim_build(base_url, STUB_TOKEN, "0123456789abcdef0123456789abcdef01234567")
            tt |> success(!job.ok, "nothing claimed")
        }
        t |> run("a wrong token claims nothing") @(tt : T?) {
            let job = claim_build(base_url, "wrong", TC_JOB)
            tt |> success(!job.ok, "refused")
        }
        t |> run("announce round-trips, wrong token refused") @(tt : T?) {
            tt |> success(announce_toolchain(base_url, STUB_TOKEN, TC_JOB), "announced")
            tt |> success(!announce_toolchain(base_url, "wrong", TC_JOB), "refused")
        }
    }
}

[test]
def test_result_uploads(t : T?) {
    var terr = ""
    let src = create_temp_file("buildd_client_wasm_", ".bin", terr)
    fwrite(src, "fake wasm content ABC")
    remove(upload_landing())
    with_stub() $(base_url) {
        t |> run("a success upload carries the file bytes intact") @(tt : T?) {
            let files <- [BuiltFile(name = "sample.wasm", sha256 = sha256_hex("fake wasm content ABC"), path = src)]
            tt |> success(upload_success(base_url, STUB_TOKEN, JOB_HASH, TC_JOB, files), "accepted")
            tt |> equal(fread(upload_landing()), "fake wasm content ABC")
        }
        t |> run("a manifest sha that does not match the bytes is refused") @(tt : T?) {
            let files <- [BuiltFile(name = "sample.wasm", sha256 = sha256_hex("other bytes"), path = src)]
            tt |> success(!upload_success(base_url, STUB_TOKEN, JOB_HASH, TC_JOB, files), "refused")
        }
        t |> run("a failure upload carries its error") @(tt : T?) {
            tt |> success(upload_failure(base_url, STUB_TOKEN, JOB_HASH, TC_JOB, "error[30101]: nope"), "accepted")
        }
    }
    remove(src)
    remove(upload_landing())
}
