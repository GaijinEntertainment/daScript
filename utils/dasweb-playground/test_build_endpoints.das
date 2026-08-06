options gen2
options persistent_heap

require dastest/testing_boost public
require playground_server
require daslib/jobque_boost
require daslib/json_boost
require daslib/fio
require daslib/sha_256
require strings

let TEST_PORT = 19013   // 19011/19012 are test_playground_server's

let TEST_TOKEN = "test-build-token-3a"
let TC = "abc1234def567890abc1234def567890abc12345"

// Same local-harness rules as test_playground_server: all setup happens inside
// the server thread (module globals are per-context), shutdown goes through
// POST /shutdown. Strings cross into the thread as separate locals — the
// proven capture shape — and BuildOptions is assembled inside the thread.
def private with_build_server(token : string; blobs_dir : string; blk : block<(base_url : string) : void>) {
    with_page_build_server(token, blobs_dir, "", blk)
}

def private with_page_build_server(token : string; blobs_dir : string; page_origin : string; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        let token_copy = token
        let blobs_copy = blobs_dir
        let origin_copy = page_origin
        new_thread() @() {
            run_server(TEST_PORT, ":memory:", StorePolicy(), "https://test.example", "",
                BuildOptions(token = token_copy, blobs_dir = blobs_copy, page_origin = origin_copy))
            finished |> notify_and_release
        }
        let base_url = "http://127.0.0.1:{TEST_PORT}"
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
        assert(ready, "playground server became ready")
    }
}

def private authed_post(url : string; body : string; token : string; blk : block<(resp : HttpResponse?) : void>) {
    with_http_request() $(var req) {
        req.method = http_method.POST
        req.url := url
        set_header(req, "Authorization", "Bearer {token}")
        req.body := body
        request(req) $(resp) {
            invoke(blk, resp)
        }
    }
}

def private posted_hash(base_url : string; source : string) : string {
    var hash = ""
    POST("{base_url}/api/samples", source) $(resp) {
        if (resp != null && resp.status_code == http_status.OK) {
            var jerr = ""
            var doc = read_json(string(resp.body), jerr)
            hash = doc?["hash"] ?? ""
        }
    }
    return hash
}

def private json_of(resp : HttpResponse?) : JsonValue? {
    var jerr = ""
    return read_json(string(resp.body), jerr)
}

[test]
def test_disabled_surface(t : T?) {
    with_build_server("", "") $(base_url) {
        t |> run("builder endpoints are 503 without a configured token") @(tt : T?) {
            authed_post("{base_url}/api/build/next", TC, "whatever") $(resp) {
                tt |> equal(int(resp.status_code), 503)
            }
        }
        t |> run("build info reports the surface off") @(tt : T?) {
            GET("{base_url}/api/build/info") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var doc = json_of(resp)
                tt |> success(!(doc?["enabled"] ?? true), "enabled=false")
                tt |> equal(doc?["toolchain"] ?? "x", "")
            }
        }
        t |> run("public build request is 503 too") @(tt : T?) {
            let hash = posted_hash(base_url, "print(\"disabled build\")")
            POST("{base_url}/api/build/request/{hash}", "") $(resp) {
                tt |> equal(int(resp.status_code), 503)
            }
        }
    }
}

[test]
def test_bearer_auth(t : T?) {
    var err = ""
    let blobs = create_temp_directory("dasweb_ep_blobs_", err)
    with_build_server(TEST_TOKEN, blobs) $(base_url) {
        t |> run("missing token is refused") @(tt : T?) {
            POST("{base_url}/api/build/next", TC) $(resp) {
                tt |> equal(int(resp.status_code), 401)
                tt |> equal(get_header(resp, "WWW-Authenticate"), "Bearer")
            }
        }
        t |> run("wrong token is refused") @(tt : T?) {
            authed_post("{base_url}/api/build/next", TC, "not-the-token") $(resp) {
                tt |> equal(int(resp.status_code), 401)
            }
        }
        t |> run("right token claims from an empty queue: 204") @(tt : T?) {
            authed_post("{base_url}/api/build/next", TC, TEST_TOKEN) $(resp) {
                tt |> equal(int(resp.status_code), 204)
            }
        }
        t |> run("malformed toolchain announce is refused") @(tt : T?) {
            authed_post("{base_url}/api/build/toolchain", "NOT-A-SHA", TEST_TOKEN) $(resp) {
                tt |> equal(int(resp.status_code), 400)
            }
        }
    }
    rmdir_rec(blobs)
}

[test]
def test_constant_time_equal(t : T?) {
    t |> run("equality is exact") @(tt : T?) {
        tt |> success(constant_time_equal("secret-token", "secret-token"), "equal strings")
        tt |> success(!constant_time_equal("secret-token", "secret-tokeN"), "last byte differs")
        tt |> success(!constant_time_equal("secret-token", "Xecret-token"), "first byte differs")
        tt |> success(!constant_time_equal("secret", "secret-token"), "prefix is not equal")
        tt |> success(!constant_time_equal("secret-token", ""), "empty right")
        tt |> success(!constant_time_equal("", "secret-token"), "empty left")
        tt |> success(constant_time_equal("", ""), "both empty")
    }
}

[test]
def test_build_flow_end_to_end(t : T?) { // nolint:STYLE038 — sequential pipeline scenario; each step consumes the previous step's state
    var err = ""
    let blobs = create_temp_directory("dasweb_ep_blobs_", err)
    let upload_src = create_temp_file("dasweb_ep_wasm_", ".bin", err)
    fwrite(upload_src, "fake wasm payload for the pipeline test")
    let payload_sha = sha256_hex("fake wasm payload for the pipeline test")
    with_build_server(TEST_TOKEN, blobs) $(base_url) {
        // announce the toolchain — the go-live for TC
        authed_post("{base_url}/api/build/toolchain", TC, TEST_TOKEN) $(resp) {
            assert(int(resp.status_code) == 200, "toolchain announced")
        }
        let hash = posted_hash(base_url, "print(\"build me\")")
        t |> run("build info reports the surface on, with its toolchain") @(tt : T?) {
            // the playground asks this once at load to decide whether to offer
            // the wasm engine at all
            GET("{base_url}/api/build/info") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var doc = json_of(resp)
                tt |> success(doc?["enabled"] ?? false, "enabled=true")
                tt |> equal(doc?["toolchain"] ?? "", TC)
            }
        }
        t |> run("request enqueues at position 1") @(tt : T?) {
            POST("{base_url}/api/build/request/{hash}", "") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var doc = json_of(resp)
                tt |> equal(doc?["state"] ?? "", "queued")
                tt |> equal(doc?["position"] ?? 0, 1)
            }
        }
        t |> run("request for an unknown sample is 404") @(tt : T?) {
            let unknown = sha256_hex("never stored")
            POST("{base_url}/api/build/request/{unknown}", "") $(resp) {
                tt |> equal(int(resp.status_code), 404)
            }
        }
        t |> run("claim hands out the job with its source") @(tt : T?) {
            authed_post("{base_url}/api/build/next", TC, TEST_TOKEN) $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var doc = json_of(resp)
                tt |> equal(doc?["hash"] ?? "", hash)
                tt |> equal(doc?["toolchain_id"] ?? "", TC)
                tt |> equal(doc?["mode"] ?? "", "module")
                tt |> equal(doc?["source"] ?? "", "print(\"build me\")")
            }
        }
        t |> run("status shows building") @(tt : T?) {
            GET("{base_url}/api/build/status/{hash}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var doc = json_of(resp)
                tt |> equal(doc?["state"] ?? "", "building")
            }
        }
        t |> run("a result with a wrong sha is refused and stored nowhere") @(tt : T?) {
            with_http_request() $(var req) {
                req.method = http_method.POST
                req.url := "{base_url}/api/build/result"
                set_header(req, "Authorization", "Bearer {TEST_TOKEN}")
                set_form_data(req, "hash", hash)
                set_form_data(req, "toolchain", TC)
                set_form_data(req, "ok", "1")
                set_form_data(req, "manifest", "[\{\"name\":\"sample.wasm\",\"sha256\":\"{sha256_hex("some other bytes")}\"}]")
                set_form_file(req, "sample.wasm", upload_src)
                request(req) $(resp) {
                    tt |> equal(int(resp.status_code), 400)
                }
            }
            GET("{base_url}/api/build/artifact/{TC}/{hash}/sample.wasm") $(resp) {
                tt |> equal(int(resp.status_code), 404)
            }
        }
        t |> run("a good result lands the artifact and resolves the job") @(tt : T?) {
            with_http_request() $(var req) {
                req.method = http_method.POST
                req.url := "{base_url}/api/build/result"
                set_header(req, "Authorization", "Bearer {TEST_TOKEN}")
                set_form_data(req, "hash", hash)
                set_form_data(req, "toolchain", TC)
                set_form_data(req, "ok", "1")
                set_form_data(req, "manifest", "[\{\"name\":\"sample.wasm\",\"sha256\":\"{payload_sha}\"}]")
                set_form_file(req, "sample.wasm", upload_src)
                request(req) $(resp) {
                    tt |> equal(int(resp.status_code), 200)
                    var doc = json_of(resp)
                    tt |> equal(doc?["state"] ?? "", "done")
                }
            }
        }
        t |> run("status carries the artifact urls") @(tt : T?) {
            GET("{base_url}/api/build/status/{hash}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var doc = json_of(resp)
                tt |> equal(doc?["state"] ?? "", "done")
                let files = doc?["files"]
                tt |> success(files != null && files.value is _array, "files array present")
                let first : string = (files.value as _array)[0] ?? ""
                tt |> equal(first, "/api/build/artifact/{TC}/{hash}/sample.wasm")
            }
        }
        t |> run("the artifact serves byte-exact with immutable headers") @(tt : T?) {
            GET("{base_url}/api/build/artifact/{TC}/{hash}/sample.wasm") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                tt |> equal(string(resp.body), "fake wasm payload for the pipeline test")
                tt |> success(find(get_header(resp, "Cache-Control"), "immutable") >= 0, "immutable")
                tt |> equal(get_header(resp, "X-Content-Type-Options"), "nosniff")
            }
        }
        t |> run("artifact requests with bad components are 404") @(tt : T?) {
            GET("{base_url}/api/build/artifact/{TC}/{hash}/other.wasm") $(resp) {
                tt |> equal(int(resp.status_code), 404)
            }
            GET("{base_url}/api/build/artifact/{TC}/{hash}/x.exe") $(resp) {
                tt |> equal(int(resp.status_code), 404)
            }
        }
        t |> run("a late result for a resolved job is 409") @(tt : T?) {
            with_http_request() $(var req) {
                req.method = http_method.POST
                req.url := "{base_url}/api/build/result"
                set_header(req, "Authorization", "Bearer {TEST_TOKEN}")
                set_form_data(req, "hash", hash)
                set_form_data(req, "toolchain", TC)
                set_form_data(req, "ok", "0")
                set_form_data(req, "error", "too late")
                request(req) $(resp) {
                    tt |> equal(int(resp.status_code), 409)
                }
            }
        }
    }
    remove(upload_src)
    rmdir_rec(blobs)
}

[test]
def test_build_failure_paths(t : T?) { // nolint:STYLE038 — sequential refusal scenarios, each consuming the previous step's queue state
    var err = ""
    let blobs = create_temp_directory("dasweb_ep_blobs_", err)
    let upload_src = create_temp_file("dasweb_ep_wasm_", ".bin", err)
    fwrite(upload_src, "fake wasm payload for the pipeline test")
    let payload_sha = sha256_hex("fake wasm payload for the pipeline test")
    with_build_server(TEST_TOKEN, blobs) $(base_url) {
        authed_post("{base_url}/api/build/toolchain", TC, TEST_TOKEN) $(resp) {
            assert(int(resp.status_code) == 200, "toolchain announced")
        }
        t |> run("a failed build surfaces the compiler error") @(tt : T?) {
            let bad = posted_hash(base_url, "this does not compile")
            POST("{base_url}/api/build/request/{bad}", "") $(_resp) {}
            authed_post("{base_url}/api/build/next", TC, TEST_TOKEN) $(_resp) {}
            with_http_request() $(var req) {
                req.method = http_method.POST
                req.url := "{base_url}/api/build/result"
                set_header(req, "Authorization", "Bearer {TEST_TOKEN}")
                set_form_data(req, "hash", bad)
                set_form_data(req, "toolchain", TC)
                set_form_data(req, "ok", "0")
                set_form_data(req, "error", "error[30101]: no such thing")
                request(req) $(resp) {
                    tt |> equal(int(resp.status_code), 200)
                }
            }
            GET("{base_url}/api/build/status/{bad}") $(resp) {
                var doc = json_of(resp)
                tt |> equal(doc?["state"] ?? "", "failed")
                tt |> equal(doc?["error"] ?? "", "error[30101]: no such thing")
            }
        }
        t |> run("an upload carrying an extra file is refused") @(tt : T?) {
            // The server does not take the builder's word for the artifact
            // set: a build runs the user's compile-time code, so an extra
            // servable file would land on this origin.
            let extra = posted_hash(base_url, "print(\"extra artifact\")")
            POST("{base_url}/api/build/request/{extra}", "") $(_resp) {}
            authed_post("{base_url}/api/build/next", TC, TEST_TOKEN) $(_resp) {}
            with_http_request() $(var req) {
                req.method = http_method.POST
                req.url := "{base_url}/api/build/result"
                set_header(req, "Authorization", "Bearer {TEST_TOKEN}")
                set_form_data(req, "hash", extra)
                set_form_data(req, "toolchain", TC)
                set_form_data(req, "ok", "1")
                set_form_data(req, "manifest",
                    "[\{\"name\":\"sample.wasm\",\"sha256\":\"{payload_sha}\"},\{\"name\":\"leak.js\",\"sha256\":\"{payload_sha}\"}]")
                set_form_file(req, "sample.wasm", upload_src)
                set_form_file(req, "leak.js", upload_src)
                request(req) $(resp) {
                    tt |> equal(int(resp.status_code), 400)
                }
            }
            GET("{base_url}/api/build/artifact/{TC}/{extra}/leak.js") $(resp) {
                tt |> equal(int(resp.status_code), 404)
            }
        }
        t |> run("a manifest with a duplicate name is refused") @(tt : T?) {
            let dup = posted_hash(base_url, "print(\"dup manifest\")")
            POST("{base_url}/api/build/request/{dup}", "") $(_resp) {}
            authed_post("{base_url}/api/build/next", TC, TEST_TOKEN) $(_resp) {}
            with_http_request() $(var req) {
                req.method = http_method.POST
                req.url := "{base_url}/api/build/result"
                set_header(req, "Authorization", "Bearer {TEST_TOKEN}")
                set_form_data(req, "hash", dup)
                set_form_data(req, "toolchain", TC)
                set_form_data(req, "ok", "1")
                set_form_data(req, "manifest",
                    "[\{\"name\":\"sample.wasm\",\"sha256\":\"{payload_sha}\"},\{\"name\":\"sample.wasm\",\"sha256\":\"{payload_sha}\"}]")
                set_form_file(req, "sample.wasm", upload_src)
                request(req) $(resp) {
                    tt |> equal(int(resp.status_code), 400)
                }
            }
        }
        t |> run("a manifest naming a file the mode does not publish is refused") @(tt : T?) {
            let odd = posted_hash(base_url, "print(\"odd manifest\")")
            POST("{base_url}/api/build/request/{odd}", "") $(_resp) {}
            authed_post("{base_url}/api/build/next", TC, TEST_TOKEN) $(_resp) {}
            with_http_request() $(var req) {
                req.method = http_method.POST
                req.url := "{base_url}/api/build/result"
                set_header(req, "Authorization", "Bearer {TEST_TOKEN}")
                set_form_data(req, "hash", odd)
                set_form_data(req, "toolchain", TC)
                set_form_data(req, "ok", "1")
                set_form_data(req, "manifest", "[\{\"name\":\"other.wasm\",\"sha256\":\"{payload_sha}\"}]")
                set_form_file(req, "other.wasm", upload_src)
                request(req) $(resp) {
                    tt |> equal(int(resp.status_code), 400)
                }
            }
        }
        t |> run("status for a sample never requested is 404") @(tt : T?) {
            let never = posted_hash(base_url, "print(\"never built\")")
            GET("{base_url}/api/build/status/{never}") $(resp) {
                tt |> equal(int(resp.status_code), 404)
                var doc = json_of(resp)
                tt |> equal(doc?["state"] ?? "", "none")
            }
        }
    }
    remove(upload_src)
    rmdir_rec(blobs)
}

def private upload_page_triple(base_url : string; hash : string; html_src, js_src, wasm_src : string; blk : block<(resp : HttpResponse?) : void>) {
    with_http_request() $(var req) {
        req.method = http_method.POST
        req.url := "{base_url}/api/build/result"
        set_header(req, "Authorization", "Bearer {TEST_TOKEN}")
        set_form_data(req, "hash", hash)
        set_form_data(req, "toolchain", TC)
        set_form_data(req, "ok", "1")
        set_form_data(req, "manifest", ("[" +
            "\{\"name\":\"sample.html\",\"sha256\":\"{sha256_file_hex(html_src)}\"}," +
            "\{\"name\":\"sample.js\",\"sha256\":\"{sha256_file_hex(js_src)}\"}," +
            "\{\"name\":\"sample.wasm\",\"sha256\":\"{sha256_file_hex(wasm_src)}\"}]"))
        set_form_file(req, "sample.html", html_src)
        set_form_file(req, "sample.js", js_src)
        set_form_file(req, "sample.wasm", wasm_src)
        request(req) $(resp) {
            invoke(blk, resp)
        }
    }
}

let GL_SOURCE = "require glfw/glfw_boost\nrequire opengl/opengl_boost\n[export]\ndef main \{\n\}\n"

[test]
def test_page_build_flow(t : T?) {
    var err = ""
    let blobs = create_temp_directory("dasweb_ep_blobs_", err)
    let html_src = create_temp_file("dasweb_ep_page_", ".html", err)
    let js_src = create_temp_file("dasweb_ep_page_", ".js", err)
    let wasm_src = create_temp_file("dasweb_ep_page_", ".bin", err)
    fwrite(html_src, "<html>page shell</html>")
    fwrite(js_src, "// emscripten glue")
    fwrite(wasm_src, "fake page wasm")
    // page_origin == this harness's own host:port, so the Host header the das
    // client sends is exactly the run origin — the gate's ALLOW side.
    with_page_build_server(TEST_TOKEN, blobs, "http://127.0.0.1:{TEST_PORT}") $(base_url) {
        authed_post("{base_url}/api/build/toolchain", TC, TEST_TOKEN) $(resp) {
            assert(int(resp.status_code) == 200, "toolchain announced")
        }
        let hash = posted_hash(base_url, GL_SOURCE)
        t |> run("a graphics sample enqueues as a page build") @(tt : T?) {
            POST("{base_url}/api/build/request/{hash}", "") $(resp) {
                tt |> equal(int(resp.status_code), 200)
            }
            authed_post("{base_url}/api/build/next", TC, TEST_TOKEN) $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var doc = json_of(resp)
                tt |> equal(doc?["mode"] ?? "", "page")
                tt |> equal(doc?["source"] ?? "", GL_SOURCE)
            }
        }
        t |> run("the page triple lands and status reports kind=page with absolute urls") @(tt : T?) {
            upload_page_triple(base_url, hash, html_src, js_src, wasm_src) $(resp) {
                tt |> equal(int(resp.status_code), 200)
            }
            GET("{base_url}/api/build/status/{hash}") $(resp) {
                var doc = json_of(resp)
                tt |> equal(doc?["state"] ?? "", "done")
                tt |> equal(doc?["kind"] ?? "", "page")
                let files = doc?["files"]
                tt |> success(files != null && files.value is _array, "files array present")
                let first : string = (files.value as _array)[0] ?? ""
                tt |> equal(first, "http://127.0.0.1:{TEST_PORT}/api/build/artifact/{TC}/{hash}/sample.html")
            }
        }
        t |> run("the page document serves with the isolation headers") @(tt : T?) {
            GET("{base_url}/api/build/artifact/{TC}/{hash}/sample.html") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                tt |> equal(string(resp.body), "<html>page shell</html>")
                tt |> equal(get_header(resp, "Cross-Origin-Resource-Policy"), "cross-origin")
                tt |> equal(get_header(resp, "Cross-Origin-Embedder-Policy"), "require-corp")
                // COOP + COEP is what makes crossOriginIsolated true for a page
                // opened directly; without it a -pthread program never gets its
                // SharedArrayBuffer and hangs on `loading-workers`.
                tt |> equal(get_header(resp, "Cross-Origin-Opener-Policy"), "same-origin")
                tt |> equal(get_header(resp, "X-Content-Type-Options"), "nosniff")
            }
        }
    }
    // Same blobs, new server whose run origin is elsewhere: html/js must refuse
    // on this Host (the gate does not trust Caddy's routing), wasm still serves.
    with_page_build_server(TEST_TOKEN, blobs, "https://run.test.example") $(base_url) {
        let hash = sha256_hex(GL_SOURCE)
        t |> run("html and js artifacts answer only on the run origin's Host") @(tt : T?) {
            GET("{base_url}/api/build/artifact/{TC}/{hash}/sample.html") $(resp) {
                tt |> equal(int(resp.status_code), 404)
            }
            GET("{base_url}/api/build/artifact/{TC}/{hash}/sample.js") $(resp) {
                tt |> equal(int(resp.status_code), 404)
            }
            GET("{base_url}/api/build/artifact/{TC}/{hash}/sample.wasm") $(resp) {
                tt |> equal(int(resp.status_code), 200)
            }
        }
    }
    remove(html_src)
    remove(js_src)
    remove(wasm_src)
    rmdir_rec(blobs)
}
