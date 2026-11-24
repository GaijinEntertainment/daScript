options gen2
options persistent_heap

require dastest/testing_boost public
require buildd_client
require buildd_core
require buildd_service
require daslib/jobque_boost
require daslib/json_boost
require daslib/sha_256
require daslib/fio

let STUB_PORT = 19014   // 19011..19013 are dasweb-playground's
let SERVICE_PORT = 19016   // the service under test's health port; 19015 is taken

let STUB_TOKEN = "stub-token"
let TC_JOB = "feedfeedfeedfeedfeedfeedfeedfeedfeedfeed"
let JOB_HASH = "abababababababababababababababababababababababababababababababab"
let TC_SERVICE = "5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e"
let SERVICE_HASH = "5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e5e"
let FAULT_HASH = "fafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafafa"
let FLAKY_HASH_ONE_TRY = "f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1"
let FLAKY_HASH_RETRIED = "f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2f2"

def private stub_bearer_ok(var req : HttpRequest?) : bool =>
    get_header(req, "Authorization") == "Bearer {STUB_TOKEN}"

def private stub_build_result(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    if (!stub_bearer_ok(req)) {
        return resp |> TEXT_PLAIN("unauthorized", http_status.UNAUTHORIZED)
    }
    let hash = get_form_data(req, "hash")
    if (hash == SERVICE_HASH) {
        let ok_sent = get_form_data(req, "ok")
        if (ok_sent == "1" && g_stub_refuse_artifact) {
            return resp |> TEXT_PLAIN("refused", http_status.BAD_REQUEST)
        }
        g_stub_job_result = write_json(JV((ok = ok_sent, fault = get_form_data(req, "fault"),
            error = get_form_data(req, "error"))))
        return resp |> TEXT_PLAIN("ok")
    }
    if (hash == FLAKY_HASH_ONE_TRY || hash == FLAKY_HASH_RETRIED) {
        if (g_stub_flaky_seen |> key_exists(hash)) {
            return resp |> TEXT_PLAIN("ok")
        }
        g_stub_flaky_seen |> insert(hash)
        return resp |> TEXT_PLAIN("warming up", http_status.SERVICE_UNAVAILABLE)
    }
    let fault_field = get_form_data(req, "fault")
    if (hash == FAULT_HASH) {
        if (get_form_data(req, "ok") == "0" && fault_field == "1") {
            return resp |> TEXT_PLAIN("ok")
        }
        return resp |> TEXT_PLAIN("a fault result must say so", http_status.BAD_REQUEST)
    }
    if (!empty(fault_field)) {
        return resp |> TEXT_PLAIN("unexpected fault flag", http_status.BAD_REQUEST)
    }
    if (hash != JOB_HASH || get_form_data(req, "toolchain") != TC_JOB) {
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
    let manifest = read_json(get_form_data(req, "manifest"), jerr)
    if (manifest == null || !(manifest.value is _array) || length(manifest.value as _array) != 1) {
        return resp |> TEXT_PLAIN("bad manifest", http_status.BAD_REQUEST)
    }
    let entry = (manifest.value as _array)[0]
    let name : string = entry?["name"] ?? ""
    let sha : string = entry?["sha256"] ?? ""
    if (name != "sample.wasm") {
        return resp |> TEXT_PLAIN("bad name", http_status.BAD_REQUEST)
    }
    let landing = path_join(g_stub_upload_dir, "upload.bin")
    if (save_form_file(req, name, landing) != 200) {
        return resp |> TEXT_PLAIN("missing part", http_status.BAD_REQUEST)
    }
    if (sha256_file_hex(landing) != sha) {
        return resp |> TEXT_PLAIN("sha mismatch", http_status.BAD_REQUEST)
    }
    return resp |> TEXT_PLAIN("ok")
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
        POST("/api/build/toolchain") <| @(var req : HttpRequest?;
                                          var resp : HttpResponse?) : http_status {
            if (!stub_bearer_ok(req)) {
                return resp |> TEXT_PLAIN("unauthorized", http_status.UNAUTHORIZED)
            }
            if (req.body != TC_JOB) {
                return resp |> TEXT_PLAIN("bad toolchain", http_status.BAD_REQUEST)
            }
            return resp |> JSON("\{\"toolchain\":\"{TC_JOB}\"}")
        }
        POST("/api/build/next") <| @(var req : HttpRequest?;
                                     var resp : HttpResponse?) : http_status {
            if (!stub_bearer_ok(req)) {
                return resp |> TEXT_PLAIN("unauthorized", http_status.UNAUTHORIZED)
            }
            if (req.body == TC_SERVICE && !empty(g_stub_job_source)) {
                let job = write_json(JV((hash = SERVICE_HASH, toolchain_id = TC_SERVICE,
                    mode = g_stub_job_mode, source = g_stub_job_source)))
                g_stub_job_source = ""
                return resp |> JSON(job)
            }
            if (req.body != TC_JOB) {
                return http_status.NO_CONTENT
            }
            return resp |> JSON(write_json(JV((hash = JOB_HASH, toolchain_id = TC_JOB,
                mode = "module", source = "print(42)"))))
        }
        POST("/test/job") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            var jerr = ""
            var job = read_json(string(req.body), jerr)
            g_stub_job_source = clone_string(job?["source"] ?? "")
            g_stub_job_mode = clone_string(job?["mode"] ?? "module")
            g_stub_refuse_artifact = job?["refuse_artifact"] ?? false
            g_stub_job_result = ""
            return resp |> TEXT_PLAIN("ok")
        }
        GET("/test/result") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (empty(g_stub_job_result)) {
                return http_status.NO_CONTENT
            }
            return resp |> JSON(g_stub_job_result)
        }
        POST("/api/build/result") <| @(var req : HttpRequest?;
                                       var resp : HttpResponse?) : http_status {
            return stub_build_result(req, resp)
        }
    }
}

var private g_stub_stop = false
var private g_stub_flaky_seen : table<string>
var private g_stub_upload_dir = ""
var private g_stub_job_source = ""
var private g_stub_job_mode = ""
var private g_stub_refuse_artifact = false
var private g_stub_job_result = ""

def private run_stub(port : int) {
    var err = ""
    g_stub_upload_dir = create_temp_directory("buildd_stub_", err)
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
    rmdir_rec(g_stub_upload_dir)
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

[test(description = "against the stub playground a claim parses the job payload, an idle queue or a wrong token claims nothing, and announce round-trips with the right token only", timeout = 1200)]
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

[test(description = "a success upload lands with the sha of the bytes sent, a manifest sha that does not match them is refused, a failure upload carries its error, and a build-service fault carries its flag", timeout = 1200)]
def test_result_uploads(t : T?) {
    var terr = ""
    let src_dir = create_temp_directory("buildd_client_", terr)
    let src = path_join(src_dir, "sample.bin")
    fwrite(src, "fake wasm content ABC")
    with_stub() $(base_url) {
        t |> run("a success upload lands with the sha of the bytes sent") @(tt : T?) {
            let files <- [
                          BuiltFile(name = "sample.wasm",
                                    sha256 = sha256_hex("fake wasm content ABC"), path = src)]
            tt |> success(upload_success(base_url, STUB_TOKEN, JOB_HASH, TC_JOB, files), "accepted")
        }
        t |> run("a manifest sha that does not match the bytes is refused") @(tt : T?) {
            let files <- [
                          BuiltFile(name = "sample.wasm", sha256 = sha256_hex("other bytes"),
                                    path = src)]
            tt |> success(!upload_success(base_url, STUB_TOKEN, JOB_HASH, TC_JOB, files), "refused")
        }
        t |> run("a failure upload carries its error") @(tt : T?) {
            tt |> success(
                          upload_failure(base_url, STUB_TOKEN, JOB_HASH, TC_JOB,
                                         "error[30101]: nope", false), "accepted")
        }
        t |> run("a build-service fault is flagged, and only then") @(tt : T?) {
            let once = ResultRetry(attempts = 1, first_pause_ms = 0u)
            tt |> success(
                          upload_failure(base_url, STUB_TOKEN, FAULT_HASH, TC_JOB,
                                         "could not publish", true, once), "flagged")
            tt |> success(
                          !upload_failure(base_url, STUB_TOKEN, FAULT_HASH, TC_JOB,
                                          "could not publish", false, once), "not flagged")
        }
    }
    rmdir_rec(src_dir)
}

[test(description = "a result POST is repeated while its status is unsettled - no response or a 5xx - with a pause that doubles up to a cap, and stops at the first settled status or when the attempts are spent")]
def test_post_until_settled(t : T?) {
    t |> run("the pause doubles from the first one and stops at the cap") @(tt : T?) {
        let retry = ResultRetry()
        tt |> equal(retry_pause_ms(retry, 1), 2000u)
        tt |> equal(retry_pause_ms(retry, 2), 4000u)
        tt |> equal(retry_pause_ms(retry, 4), 16000u)
        tt |> equal(retry_pause_ms(retry, 5), 30000u)
        tt |> equal(retry_pause_ms(retry, 7), 30000u)
        tt |> equal(retry_pause_ms(ResultRetry(first_pause_ms = 0u), 3), 0u)
        tt |> equal(retry_pause_ms(ResultRetry(first_pause_ms = 50000u), 1), 30000u)
    }
    t |> run("only no response and 5xx are unsettled") @(tt : T?) {
        tt |> success(!is_settled_status(HTTP_NO_RESPONSE), "no response")
        tt |> success(!is_settled_status(500), "500")
        tt |> success(!is_settled_status(503), "503")
        tt |> success(is_settled_status(200), "200")
        tt |> success(is_settled_status(409), "409")
        tt |> success(is_settled_status(499), "499")
    }
    t |> run("no response, then a 502, then accepted: three posts") @(tt : T?) {
        let script = [HTTP_NO_RESPONSE, 502, 200, 400]
        var posts = 0
        let status = post_until_settled(ResultRetry(attempts = 4, first_pause_ms = 0u),
                                        "test post") $() : int {
            posts ++
            return script[posts - 1]
        }
        tt |> equal(status, 200)
        tt |> equal(posts, 3)
    }
    t |> run("a refusal is settled: one post") @(tt : T?) {
        var posts = 0
        let status = post_until_settled(ResultRetry(attempts = 4, first_pause_ms = 0u),
                                        "test post") $() : int {
            posts ++
            return 400
        }
        tt |> equal(status, 400)
        tt |> equal(posts, 1)
    }
    t |> run("a server that never answers spends every attempt") @(tt : T?) {
        var posts = 0
        let status = post_until_settled(ResultRetry(attempts = 4, first_pause_ms = 0u),
                                        "test post") $() : int {
            posts ++
            return HTTP_NO_RESPONSE
        }
        tt |> equal(status, HTTP_NO_RESPONSE)
        tt |> equal(posts, 4)
    }
    t |> run("no response is unsettled, and one attempt of it does not pause") @(tt : T?) {
        let started = ref_time_ticks()
        tt |> success(
                      !upload_failure("http://256.0.0.1:80", STUB_TOKEN, JOB_HASH, TC_JOB, "boom",
                                      false,
            ResultRetry(attempts = 1, first_pause_ms = 5000u)), "not accepted")
        tt |> success(get_time_usec(started) < 4000000, "no pause after the last attempt")
    }
    t |> run("the last unsettled status is the one reported") @(tt : T?) {
        let script = [HTTP_NO_RESPONSE, 503]
        var posts = 0
        let status = post_until_settled(ResultRetry(attempts = 2, first_pause_ms = 0u),
                                        "test post") $() : int {
            posts ++
            return script[posts - 1]
        }
        tt |> equal(status, 503)
        tt |> equal(posts, 2)
    }
}

[test(description = "a result the stub playground answers 503 once is lost with one attempt and accepted on the retry", timeout = 1200)]
def test_result_retry(t : T?) {
    with_stub() $(base_url) {
        t |> run("one attempt loses a result the server answers 503") @(tt : T?) {
            tt |> success(
                          !upload_failure(base_url, STUB_TOKEN, FLAKY_HASH_ONE_TRY, TC_JOB, "boom",
                                          false,
                ResultRetry(attempts = 1, first_pause_ms = 0u)), "lost")
        }
        t |> run("the retry lands it") @(tt : T?) {
            tt |> success(
                          upload_failure(base_url, STUB_TOKEN, FLAKY_HASH_RETRIED, TC_JOB, "boom",
                                         false,
                ResultRetry(attempts = 2, first_pause_ms = 0u)), "accepted")
        }
    }
}

def private scripted_job(base_url : string; script : string; mode : string = "module";
                         refuse_artifact : bool = false) {
    POST("{base_url}/test/job",
         write_json(
                    JV(
                       (source = script, mode = mode,
                        refuse_artifact = refuse_artifact)))) $(_resp) {}
}

// Ticks the service until the stub has the scripted job's result;
// null when none arrives.
def private service_result(base_url : string) : JsonValue? {
    var result : JsonValue?
    for (_tick in range(600)) {
        update_service()
        GET("{base_url}/test/result") $(resp) {
            if (resp != null && resp.status_code == http_status.OK) {
                var jerr = ""
                result = read_json(string(resp.body), jerr)
            }
        }
        if (result != null) break
    }
    return result
}

//! The service spawns its build command by contract, so the stand-in
//! recipe runs as a daslang child: a running process cannot hand back
//! an exit status, a timeout kill or a missing artifact.
[test(description = "a job driven through the service is reported by what went wrong: a verdict exit and a refused asset declaration go up unflagged, a recipe fault, the timeout kill, a build with no artifact, an artifact the server refuses and a missing scratch directory go up as faults, and a published build reports nothing failed", timeout = 1200)]
def test_service_reports_faults(t : T?) {
    var terr = ""
    let work = create_temp_directory("buildd_service_", terr)
    let recipe = path_join(get_das_root(), "utils/internal/dasweb-buildd/_build_recipe.das")
    let host = "{get_command_line_arguments()[0]} -no-module-cache -dasroot {get_das_root()}"
    let build_cmd = "{host} {recipe} --"
    with_stub() $(base_url) {
        var cfg = BuilddArgs(port = SERVICE_PORT, server_url = base_url, token = STUB_TOKEN,
                             toolchain = TC_SERVICE,
            build_cmd = build_cmd, scratch_dir = path_join(work, "scratch"),
            poll_interval_seconds = 1,
            build_timeout_seconds = 5, announce = false)
        t |> success(init_service(cfg), "service started")
        t |> run("the verdict exit goes up unflagged with the build's output") @(tt : T?) {
            scripted_job(base_url, "exit 65")
            var result = service_result(base_url)
            tt |> equal(result?["ok"] ?? "", "0")
            tt |> equal(result?["fault"] ?? "?", "")
            tt |> success(find(result?["error"] ?? "",
                          "recipe ran: exit 65") >= 0, "the build's output is the error")
        }
        t |> run("a recipe exit that is not the verdict is a fault") @(tt : T?) {
            scripted_job(base_url, "exit 16")
            var result = service_result(base_url)
            tt |> equal(result?["ok"] ?? "", "0")
            tt |> equal(result?["fault"] ?? "", "1")
        }
        t |> run("the timeout kill is a fault that names the budget") @(tt : T?) {
            scripted_job(base_url, "sleep")
            var result = service_result(base_url)
            tt |> equal(result?["fault"] ?? "", "1")
            tt |> success(find(result?["error"] ?? "",
                          "build timed out after 5 seconds") >= 0, "names the budget")
        }
        t |> run("a build that passed and left no artifact is a fault") @(tt : T?) {
            scripted_job(base_url, "exit 0")
            var result = service_result(base_url)
            tt |> equal(result?["fault"] ?? "", "1")
            tt |> success(find(result?["error"] ?? "",
                          "build produced no artifacts") >= 0, "says so")
        }
        t |> run("an artifact the server refuses is a fault") @(tt : T?) {
            scripted_job(base_url, "artifact", "module", true)
            var result = service_result(base_url)
            tt |> equal(result?["ok"] ?? "", "0")
            tt |> equal(result?["fault"] ?? "", "1")
            tt |> success(find(result?["error"] ?? "", "could not publish") >= 0, "says so")
        }
        t |> run("a published build reports nothing failed") @(tt : T?) {
            scripted_job(base_url, "artifact")
            var result = service_result(base_url)
            tt |> equal(result?["ok"] ?? "", "1")
        }
        t |> run("an asset declaration the builder refuses goes up unflagged") @(tt : T?) {
            scripted_job(base_url, "// wasm-assets: ../../etc/passwd\nexit 0", "page")
            var result = service_result(base_url)
            tt |> equal(result?["ok"] ?? "", "0")
            tt |> equal(result?["fault"] ?? "?", "")
        }
        shutdown_service()
        fwrite(path_join(work, "not_a_directory"), "x")
        cfg.scratch_dir = path_join(path_join(work, "not_a_directory"), "scratch")
        t |> success(init_service(cfg), "service started again")
        t |> run("a scratch directory that cannot be made is a fault") @(tt : T?) {
            scripted_job(base_url, "artifact")
            var result = service_result(base_url)
            tt |> equal(result?["fault"] ?? "", "1")
            tt |> success(find(result?["error"] ?? "", "scratch space") >= 0, "says so")
        }
        shutdown_service()
    }
    rmdir_rec(work)
}
