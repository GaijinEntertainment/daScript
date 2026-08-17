options gen2
options persistent_heap

require dastest/testing_boost public
require ../../modules/dasLLAMA/performance/exchange_client.das
require dasllama/dasllama_version   // DASLLAMA_VERSION — the fixture stamps
require dashv/dashv_boost
require llvm/daslib/llvm_tune
require daslib/jobque_boost
require daslib/json_boost
require daslib/sha_256
require daslib/fio
require strings

let TEST_PORT = 18131   // 18097/18099/18101/18120/18121 are the openai-server suites'

// The exchange client against a FAKE exchange whose response shapes mirror the repo's
// dasllama-ladder service (its contract is pinned by that dir's own suite): lookup parse,
// the accept-policy pick, the full fetch-and-apply gate (hash/schema/version), the
// origin/applied_box stamps and their staleness consequences, the privacy strip, and
// both submit rails. All server state lives in the server thread's context (dashv rule).

// deterministic fixture — both the server thread and the asserting test compute the
// same text, so the shas agree without crossing contexts
def fixture_sidecar(box : string; version : int; winner : string) : string {
    var doc = JV((
        kernels = (dot = winner, softmax = "vec8_u2"),
        runtime = (jobque_spin_us = 30000),
        provenance = (
            box = box,
            engine_sha = "8a538d795",
            written = "2026-08-11T00:00:00Z",
            noise = "ok",
            binary = "/Users/someone/bin/daslang",
            dasllama_version = "{version}")))
    let text = write_json(doc)
    delete_json(doc)
    return text
}

def sibling_box : string {
    // same platform/arch/cpu as this box, another machine — the ladder's same-cpu tier
    var ident : BoxIdentity
    verify(parse_box_string(tune_box_identity(), ident))
    return "{ident.platform}|{ident.arch}|other-machine|{ident.os_build}|{ident.cpu}"
}

def verified_doc : string
    => fixture_sidecar(tune_box_identity(), DASLLAMA_VERSION, "vec16")

def crossbox_doc : string
    => fixture_sidecar(sibling_box(), DASLLAMA_VERSION, "vec8_u4")

def oldversion_doc : string
    => fixture_sidecar(tune_box_identity(), DASLLAMA_VERSION + 1, "vec16")

def tamper_sha : string
    => sha256_hex("tampered")

// the fake exchange: /api/sidecars returns the unverified sibling FIRST so the pick has to
// skip it under accept=verified; /api/sidecar/:sha serves the seeded docs (tamper_sha serves
// the WRONG content on purpose); the submit routes echo back through /last/* for assertions.
// All state is module globals seeded in onInit — it runs in the server thread's context.
var private g_docs : table<string; string>
var private g_hits_body = ""
var private g_last_sidecar = ""
var private g_last_records = ""
var private g_stop = false

class FakeExchange : HvWebServer {
    def override onInit {
        let vdoc = verified_doc()
        let xdoc = crossbox_doc()
        let odoc = oldversion_doc()
        g_docs[sha256_hex(vdoc)] = vdoc
        g_docs[sha256_hex(xdoc)] = xdoc
        g_docs[sha256_hex(odoc)] = odoc
        g_docs[tamper_sha()] = vdoc
        var hits <- [
            JV((sha = sha256_hex(xdoc), box = sibling_box(), tier = "cpu",
                verified = false, noise = "ok", written = "2026-08-11T00:00:00Z",
                kernel_count = 2, engine_sha = "8a538d795",
                dasllama_version = DASLLAMA_VERSION, url = "/api/sidecar/{sha256_hex(xdoc)}")),
            JV((sha = sha256_hex(vdoc), box = tune_box_identity(), tier = "exact",
                verified = true, noise = "ok", written = "2026-08-11T00:00:00Z",
                kernel_count = 2, engine_sha = "8a538d795",
                dasllama_version = DASLLAMA_VERSION, url = "/api/sidecar/{sha256_hex(vdoc)}"))]
        var top = JV((version = DASLLAMA_VERSION, hits = JV(hits)))
        g_hits_body = write_json(top)
        delete_json(top)
        GET("/api/sidecars") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            if (empty(get_param(req, "box")) || empty(get_param(req, "version"))) {
                return resp |> JSON("\{\"error\":\"bad request\"}", http_status.BAD_REQUEST)
            }
            return resp |> JSON(g_hits_body)
        }
        GET("/api/sidecar/:sha") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let sha = get_param(req, "sha")
            if (!key_exists(g_docs, sha)) {
                return resp |> JSON("\{\"error\":\"unknown sidecar\"}", http_status.NOT_FOUND)
            }
            return resp |> JSON(g_docs?[sha] ?? "")
        }
        POST("/api/submit/sidecar") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            g_last_sidecar = clone_string(string(req.body))
            var doc = JV((sha = sha256_hex(g_last_sidecar), created = true, url = "/api/sidecar/x"))
            let body = write_json(doc)
            delete_json(doc)
            return resp |> JSON(body)
        }
        POST("/api/submit/records") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            g_last_records = clone_string(string(req.body))
            return resp |> JSON("\{\"ok\":true}")
        }
        GET("/last/sidecar") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN(g_last_sidecar)
        }
        GET("/last/records") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN(g_last_records)
        }
        GET("/healthz") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("ok")
        }
        POST("/shutdown") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            g_stop = true
            return resp |> TEXT_PLAIN("bye")
        }
    }
}

def private run_fake_exchange(port : int) {
    var server = new FakeExchange()
    server->init(port)
    server->start()
    while (!g_stop) {
        server->tick()
        sleep(1u)
    }
    server->stop()
    server->cleanup()
    unsafe {
        delete server
    }
}

def private with_fake_exchange(blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            run_fake_exchange(TEST_PORT)
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
        assert(ready, "fake exchange became ready")
    }
}

def private temp_manifest(t : T?; tag : string) : string {
    let res = create_temp_file_result("exchange_client_{tag}_", ".tune.json")
    if (res is error) {
        t |> failure("could not create a temp file: {res as error}")
        return ""
    }
    return res as value
}

def private read_provenance(t : T?; path : string; key : string) : string {
    var jerr = ""
    var doc = read_json(fread(path), jerr)
    t |> success(doc != null, "applied sidecar parses: {jerr}")
    let v : string = doc?["provenance"]?[key] ?? ""
    let r = "{v}"
    delete_json(doc)
    return r
}

[test]
def test_box_match_key(t : T?) {
    t |> run("the Windows UBR folds out of the match key, everything else stays") @(tt : T?) {
        let a = "windows|x86_64||10.0.26200.8875|AMD64 Family 23 Model 49 Stepping 0, AuthenticAMD"
        let patched = "windows|x86_64||10.0.26200.9999|AMD64 Family 23 Model 49 Stepping 0, AuthenticAMD"
        tt |> equal(box_match_key(a), box_match_key(patched), "a monthly patch keeps the same key")
        let osver = "windows|x86_64||10.0.27000.1|AMD64 Family 23 Model 49 Stepping 0, AuthenticAMD"
        tt |> success(box_match_key(a) != box_match_key(osver), "a real OS-version change differs")
        let othercpu = "windows|x86_64||10.0.26200.8875|GenuineIntel Family 6"
        tt |> success(box_match_key(a) != box_match_key(othercpu), "a different cpu differs")
    }
    t |> run("non-Windows os_build is not folded") @(tt : T?) {
        let mac = "darwin|arm64|MacBookPro18,2|25F84|Apple M1 Max"
        tt |> equal(box_match_key(mac), to_lower(mac), "macOS build passes through")
        let linux = "linux|arm64||6.1.0-49|Cortex-A76"
        tt |> equal(box_match_key(linux), to_lower(linux), "linux kernel passes through")
    }
}

[test]
def test_lookup_and_pick(t : T?) {
    with_fake_exchange() $(base_url) {
        t |> run("lookup parses the ladder response") @(tt : T?) {
            var hits : array<ExchangeHit>
            tt |> success(exchange_lookup(base_url, DASLLAMA_VERSION, tune_box_identity(), hits))
            tt |> equal(length(hits), 2)
            tt |> equal(hits[0].tier, "cpu")
            tt |> success(!hits[0].verified, "sibling hit is unverified")
            tt |> equal(hits[1].tier, "exact")
            tt |> success(hits[1].verified, "exact hit is verified")
            tt |> equal(hits[1].sha, sha256_hex(verified_doc()))
            tt |> equal(hits[1].kernel_count, 2)
            tt |> equal(hits[1].dasllama_version, DASLLAMA_VERSION)
            // the pick: verified skips the unverified head; any takes it
            tt |> equal(exchange_pick(hits, "verified"), 1)
            tt |> equal(exchange_pick(hits, "any"), 0)
            delete hits
        }
        t |> run("lookup fails loudly on transport") @(tt : T?) {
            var hits : array<ExchangeHit>
            tt |> success(!exchange_lookup("http://127.0.0.1:1", DASLLAMA_VERSION, tune_box_identity(), hits))
            delete hits
        }
    }
}

[test]
def test_fetch_and_apply(t : T?) {
    with_fake_exchange() $(base_url) {
        t |> run("exact verified hit applies clean") @(tt : T?) {
            let manifest = temp_manifest(tt, "exact")
            return if (empty(manifest))
            let sha = sha256_hex(verified_doc())
            tt |> equal(exchange_fetch_and_apply(base_url, sha, manifest, tune_box_identity()), "")
            tt |> equal(read_provenance(tt, manifest, "origin"), "exchange")
            tt |> equal(read_provenance(tt, manifest, "exchange_sha"), sha)
            tt |> equal(read_provenance(tt, manifest, "applied_box"), "", "same box needs no applied_box")
            tt |> success(!tune_sidecar_stale(manifest), "an applied exact-box sidecar reads fresh")
            remove(manifest)
        }
        t |> run("sibling-box hit gets applied_box and reads fresh") @(tt : T?) {
            let manifest = temp_manifest(tt, "cross")
            return if (empty(manifest))
            let sha = sha256_hex(crossbox_doc())
            tt |> equal(exchange_fetch_and_apply(base_url, sha, manifest, sibling_box()), "")
            tt |> equal(read_provenance(tt, manifest, "box"), sibling_box(), "box keeps naming the measuring box")
            tt |> equal(read_provenance(tt, manifest, "applied_box"), tune_box_identity())
            tt |> success(!tune_sidecar_stale(manifest), "applied_box re-admits the cross-box mint")
            remove(manifest)
        }
        t |> run("a local tune over an adopted sidecar discards the adopted winners") @(tt : T?) {
            let manifest = temp_manifest(tt, "adopt")
            return if (empty(manifest))
            // adopt an exchange sidecar (origin=exchange, carries dot/softmax winners)
            tt |> equal(exchange_fetch_and_apply(base_url, sha256_hex(verified_doc()), manifest, tune_box_identity()), "")
            tt |> equal(read_provenance(tt, manifest, "origin"), "exchange")
            // a local tuner writing over it must NOT merge — the file would otherwise end up
            // half-adopted, half-local, provenance rewritten local, and wrongly re-submittable
            set_tune_manifest_runtime_path(manifest)
            tune_manifest_set("mykernel", "local1")
            set_tune_manifest_runtime_path("")
            var jerr = ""
            var doc = read_json(fread(manifest), jerr)
            let kernels = doc?["kernels"]
            tt |> success(kernels != null && kernels is _object, "kernels object")
            tt |> equal(length(kernels as _object), 1, "adopted dot/softmax discarded, only the local key")
            tt |> success(key_exists(kernels as _object, "mykernel"), "the local key is present")
            tt |> equal(read_provenance(tt, manifest, "origin"), "", "origin cleared by the local write")
            delete_json(doc)
            remove(manifest)
        }
        t |> run("version and tamper gates refuse") @(tt : T?) {
            let manifest = temp_manifest(tt, "gates")
            return if (empty(manifest))
            let overs = exchange_fetch_and_apply(base_url, sha256_hex(oldversion_doc()), manifest)
            tt |> success(find(overs, "version") >= 0, "version gate: {overs}")
            let tamper = exchange_fetch_and_apply(base_url, tamper_sha(), manifest)
            tt |> equal(tamper, "content hash mismatch")
            let missing = exchange_fetch_and_apply(base_url, sha256_hex("no such doc"), manifest)
            tt |> equal(missing, "download failed")
            let liar = exchange_fetch_and_apply(base_url, sha256_hex(verified_doc()), manifest, "some|other|listing|box|entirely")
            tt |> equal(liar, "box does not match the listing")
            tt |> success(empty(fread(manifest)), "nothing was written on a refusal")
            remove(manifest)
        }
    }
}

[test]
def test_privacy_strip(t : T?) {
    t |> run("binary and path-shaped provenance are stripped") @(tt : T?) {
        var doc = JV((
            kernels = (dot = "vec16"),
            provenance = (
                box = tune_box_identity(),
                engine_sha = "8a538d795",
                binary = "/Users/borisbatkin/bin/daslang",
                weird = "C:\\Users\\bob\\thing",
                noise = "ok",
                dasllama_version = "{DASLLAMA_VERSION}")))
        let text = write_json(doc)
        delete_json(doc)
        let stripped = exchange_strip_private(text)
        tt |> success(find(stripped, "binary") < 0, "binary is gone")
        tt |> success(find(stripped, "borisbatkin") < 0, "the username path is gone")
        tt |> success(find(stripped, "weird") < 0, "path-shaped values are gone")
        tt |> success(find(stripped, "\"noise\"") >= 0, "plain provenance survives")
        tt |> success(find(stripped, "vec16") >= 0, "kernels survive")
        var errs : array<string>
        tt |> success(validate_sidecar_submission(stripped, errs), "stripped doc stays submission-grade")
        delete errs
    }
    t |> run("garbage never leaves the box") @(tt : T?) {
        tt |> equal(exchange_strip_private("not json at all"), "")
    }
}

[test]
def test_submit_rails(t : T?) {
    with_fake_exchange() $(base_url) {
        t |> run("sidecar submit round-trips stripped") @(tt : T?) {
            let stripped = exchange_strip_private(verified_doc())
            var sha = ""
            tt |> equal(exchange_submit_sidecar_doc(base_url, stripped, sha), "")
            tt |> equal(sha, sha256_hex(stripped))
            GET("{base_url}/last/sidecar") $(resp) {
                tt |> equal(string(resp.body), stripped, "the exchange received the stripped doc verbatim")
            }
        }
        t |> run("unversioned sidecar is refused before the wire") @(tt : T?) {
            let bad = exchange_strip_private(fixture_sidecar(tune_box_identity(), 0, "vec16"))
            var sha = ""
            let err = exchange_submit_sidecar_doc(base_url, bad, sha)
            tt |> success(find(err, "not submission-grade") >= 0, "refused: {err}")
        }
        t |> run("submit_current guards origin and box") @(tt : T?) {
            let manifest = temp_manifest(tt, "cur")
            return if (empty(manifest))
            set_tune_manifest_runtime_path(manifest)
            let policy = ExchangePolicy(url = base_url)
            // an exchange-sourced sidecar never goes back up
            tt |> equal(exchange_fetch_and_apply(base_url, sha256_hex(verified_doc()), manifest), "")
            tt |> success(find(exchange_submit_current(policy), "came from the exchange") >= 0)
            // a foreign-box file never goes back up
            fwrite(manifest, fixture_sidecar("darwin|arm64|Mac|25F84|Apple M1 Max", DASLLAMA_VERSION, "vec16"))
            tt |> success(find(exchange_submit_current(policy), "not measured on this box") >= 0)
            // this box's own tune submits, and the state file remembers it
            let own = fixture_sidecar(tune_box_identity(), DASLLAMA_VERSION, "vec16")
            fwrite(manifest, own)
            tt |> equal(exchange_submit_current(policy), "")
            GET("{base_url}/last/sidecar") $(resp) {
                tt |> success(find(string(resp.body), "binary") < 0, "the submitted copy is stripped")
            }
            tt |> success(!empty(fread("{slice(manifest, 0, length(manifest) - 5)}.exchange.json")),
                "the state file landed beside the sidecar")
            set_tune_manifest_runtime_path("")
            remove("{slice(manifest, 0, length(manifest) - 5)}.exchange.json")
            remove(manifest)
        }
        t |> run("records submit redacts paths then posts") @(tt : T?) {
            let doc = fixture_records()
            tt |> equal(exchange_submit_records_doc(base_url, doc), "")
            GET("{base_url}/last/records") $(resp) {
                // the client redacts before POST, so the wire form is the redacted doc
                tt |> equal(string(resp.body), redact_record_paths(doc))
            }
            tt |> success(find(exchange_submit_records_doc(base_url, "[]"), "not submission-grade") >= 0, "empty store refused")
            tt |> success(find(exchange_submit_records_doc(base_url, "\{}"), "does not parse") >= 0, "non-array refused")
        }
    }
}

// a minimal stamped community records store (the lcpp_bench shape), built programmatically —
// %json~ literals are off-limits here (daScript#3690 collapses them under -jit)
def private fixture_records : string {
    var run_das = JV((
        engine = "das", backend = "cpu", flavor = "tuned", box = "testbox",
        cmd = "daslang -jit lcpp_bench.das -- --model m.gguf", date = "2026-08-11",
        threads = 16, hardware = (cpu = "Test CPU"),
        tests = (pp512 = (tok_s = 100.0lf, stddev = 1.0lf)),
        source = "community", sha = "abc123", version = "0.6.4",
        exec_fmt = "interleaved", tune = "tuned", tune_sha = "deadbeef",
        dasllama_version = DASLLAMA_VERSION))
    var runs_arr : array<JsonValue?>
    runs_arr |> push(run_das)
    var model = JV((gguf = "test-model-Q8_0.gguf", arch = "Test 1B", quant = "Q8_0",
        size_bytes = 1234567l, runs = JV(runs_arr)))
    var models_arr : array<JsonValue?>
    models_arr |> push(model)
    var doc = JV(models_arr)
    let text = write_json(doc)
    delete_json(doc)
    return text
}

[test]
def test_policy_and_discovery(t : T?) {
    t |> run("policy: defaults, toml, and degradation") @(tt : T?) {
        let p0 = exchange_policy_from_config("")
        tt |> equal(p0.url, EXCHANGE_URL_DEFAULT)
        tt |> equal(p0.accept, "verified")
        tt |> equal(p0.submit, "ask")
        let res = create_temp_file_result("exchange_policy_", ".toml")
        if (res is error) {
            tt |> failure("could not create a temp file")
            return
        }
        let path = res as value
        fwrite(path, "exchange_url = \"http://127.0.0.1:9\"\nexchange_accept = \"any\"\nexchange_submit = \"never\"\n")
        let p1 = exchange_policy_from_config(path)
        tt |> equal(p1.url, "http://127.0.0.1:9")
        tt |> equal(p1.accept, "any")
        tt |> equal(p1.submit, "never")
        fwrite(path, "exchange_accept = \"yolo\"\nexchange_submit = \"whatever\"\n")
        let p2 = exchange_policy_from_config(path)
        tt |> equal(p2.accept, "verified", "a typo must never widen acceptance")
        tt |> equal(p2.submit, "ask")
        remove(path)
    }
}
