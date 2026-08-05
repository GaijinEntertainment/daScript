options gen2
options indenting = 4

module playground_server public

require dashv/dashv_boost public
require daslib/json_boost
require daslib/logger
require daslib/fio
require daslib/strings_boost
require samples_store public
require build_queue public
require build_artifacts public
require curated_import
require strings
require math

//! The HTTP surface: route table and handlers. A handler validates
//! transport-level shape, translates HTTP to one samples_store call, and
//! formats the response. Policy (size, rate, listing) lives in the store.

// Lifecycle-owned state. Module-global: the server runs on its own context
// (per-thread in tests), and JIT heap collection cannot see stack locals as
// GC roots — handlers and the tick loop reach everything through these.
var private g_server : PlaygroundServer?
var private g_db = SqlRunner()
var private g_db_open = false
var private g_policy = StorePolicy()
var private g_base_url = "https://daslang.io"
var private g_curated_dir = ""
var private g_build = BuildOptions()
var private g_stop_requested = false
var private g_server_started = false

struct BuildOptions {
    //! Transport-level build-surface config; queue policy nests from build_queue.
    token : string = ""       // bearer for the remote builder; "" disables the build surface
    blobs_dir : string = ""   // artifact cache root; "" disables artifact storage and serving
    queue : BuildConfig = BuildConfig()
}

def private client_ip_of(var req : HttpRequest?) : string {
    //! RIGHTMOST X-Forwarded-For hop. A reverse proxy APPENDS the peer it
    //! accepted, so only the last hop is proxy-authored — every hop left of it
    //! is whatever the client sent, and taking the first lets a caller pick its
    //! own rate-limit bucket. No header (direct callers, tests) falls back to
    //! the transport peer.
    let fwd = get_header(req, "X-Forwarded-For")
    if (empty(fwd)) {
        let peer = client_ip(req)
        return empty(peer) ? "local" : peer
    }
    let comma = rfind(fwd, ",")
    let last = comma < 0 ? fwd : slice(fwd, comma + 1)
    let trimmed = strip(last)
    return empty(trimmed) ? "local" : trimmed
}

def is_loopback_peer(peer : string) : bool {
    //! Exact-match only: an unknown or empty peer counts as remote, and no
    //! prefix/suffix rule can be fooled by a hostname that merely contains a
    //! loopback address.
    return peer == "127.0.0.1" || peer == "::1" || peer == "::ffff:127.0.0.1"
}

def private is_loopback_caller(var req : HttpRequest?) : bool {
    //! Operator-surface gate. The transport peer is the only identity a caller
    //! cannot forge: the service sits behind a proxy, so any browser page can
    //! drive a cross-origin POST to these routes, and no header separates that
    //! from a local operator.
    return is_loopback_peer(client_ip(req))
}

def constant_time_equal(a : string; b : string) : bool {
    //! Byte compare that never exits at the first difference, so a token guess
    //! learns nothing from response timing. Length is not hidden.
    let la = length(a)
    let lb = length(b)
    var diff = la ^ lb
    peek_data(a) $(ab) {
        peek_data(b) $(bb) {
            for (i in range(max(la, lb))) {
                let x = i < la ? int(ab[i]) : 0
                let y = i < lb ? int(bb[i]) : 0
                diff |= x ^ y
            }
        }
    }
    return diff == 0
}

def private bearer_token_of(var req : HttpRequest?) : string {
    let auth = get_header(req, "Authorization")
    if (empty(auth) || !(auth |> starts_with("Bearer "))) {
        return ""
    }
    return slice(auth, 7)
}

def private build_auth_status(var req : HttpRequest?) : http_status {
    //! OK = the caller holds the builder token. The loopback gate cannot serve
    //! here — the builder is remote — so this surface authenticates by bearer
    //! token from config, compared in constant time and never logged.
    if (empty(g_build.token)) {
        return http_status.SERVICE_UNAVAILABLE
    }
    return constant_time_equal(bearer_token_of(req), g_build.token) ? http_status.OK : http_status.UNAUTHORIZED
}

def private build_auth_refused(path : string; var req : HttpRequest?; var resp : HttpResponse?; auth : http_status; t0 : int64) : http_status {
    if (auth == http_status.UNAUTHORIZED) {
        resp |> set_header("WWW-Authenticate", "Bearer")
    }
    let msg = auth == http_status.UNAUTHORIZED ? "unauthorized" : "build api disabled"
    log_request("POST", path, int(auth), client_ip_of(req), 0, 0, t0)
    return resp |> JSON(write_json(JV((error = msg))), auth)
}

def body_is_byte_faithful(buffered_bytes : int; text : string) : bool {
    //! Whether the string view of a request body still carries every byte that
    //! arrived. A das string ends at the first NUL, so `text` can be a silent
    //! prefix of what the client sent — only the buffered byte count knows the
    //! difference, and a truncated body would be hashed and stored as if it
    //! were the whole thing.
    return buffered_bytes == length(text)
}

def is_valid_hash(h : string) : bool {
    //! Transport-shape check: exactly 64 lowercase hex chars.
    if (length(h) != 64) {
        return false
    }
    var ok = true
    peek_data(h) $(bytes) {
        for (b in bytes) {
            let c = int(b)
            if (!(is_number(c) || (c >= 'a' && c <= 'f'))) {
                ok = false
            }
        }
    }
    return ok
}

def private log_request(method : string; path : string; status : int; ip : string; bytes_in : int; bytes_out : int; t0 : int64) {
    logger_info("playground.req", "{method} {path} {status}",
        JV((method = method, path = path, status = status, ip = ip,
            bytes_in = bytes_in, bytes_out = bytes_out,
            duration_ms = get_time_usec(t0) / 1000)))
}

class PlaygroundServer : HvWebServer {
    def override onInit {  // the route table: one registration per endpoint
        // No allow_cors(): libhv's CORS middleware reflects the caller's Origin
        // back on EVERY route, which would make each stored sample and the
        // operator surface cross-origin readable. The playground is same-origin.
        GET("/healthz") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("ok")
        }
        POST("/api/samples") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_put(req, resp)
        }
        GET("/api/samples/listed") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_listed(req, resp)
        }
        GET("/api/samples/:hash") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_get_source(req, resp)
        }
        POST("/admin/reimport") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_reimport(req, resp)
        }
        GET("/api/build/info") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_build_info(req, resp)
        }
        POST("/api/build/request/:hash") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_build_request(req, resp)
        }
        GET("/api/build/status/:hash") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_build_status(req, resp)
        }
        GET("/api/build/artifact/:toolchain/:hash/:file") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_build_artifact(req, resp)
        }
        POST("/api/build/toolchain") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_build_toolchain(req, resp)
        }
        POST("/api/build/next") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_build_next(req, resp)
        }
        POST("/api/build/result") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_build_result(req, resp)
        }
        GET("/s/:hash") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_share_link(req, resp)
        }
        POST("/shutdown") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let t0 = ref_time_ticks()
            if (!is_loopback_caller(req)) {
                log_request("POST", "/shutdown", 403, client_ip_of(req), 0, 0, t0)
                return resp |> TEXT_PLAIN("forbidden", http_status.FORBIDDEN)
            }
            logger_info("playground", "shutdown requested")
            g_stop_requested = true
            log_request("POST", "/shutdown", 200, client_ip_of(req), 0, 3, t0)
            return resp |> TEXT_PLAIN("bye")
        }
        ANY("*") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let t0 = ref_time_ticks()
            let body = write_json(JV((error = "not found")))
            log_request("{req.method}", string(req.path), 404, client_ip_of(req), 0, length(body), t0)
            return resp |> JSON(body, http_status.NOT_FOUND)
        }
    }
}

def private handle_put(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let ip = client_ip_of(req)
    // Transport shape first: a das string ends at the first NUL, so a body
    // carrying one would be size-checked, validated, hashed and stored
    // truncated — two different bodies sharing a pre-NUL prefix would collapse
    // onto one hash and one public URL. Compare the buffered byte count
    // (std::string size) against the string length and refuse the mismatch.
    let raw_bytes = length(req.body)
    let source = string(req.body)
    if (!body_is_byte_faithful(raw_bytes, source)) {
        let nul_body = write_json(JV((error = "source contains NUL bytes")))
        log_request("POST", "/api/samples", 400, ip, raw_bytes, length(nul_body), t0)
        return resp |> JSON(nul_body, http_status.BAD_REQUEST)
    }
    let r = put_sample(g_db, source, ip, g_policy, current_unix_time())
    var status = http_status.OK
    var body = ""
    if (r.status == PutStatus.ok) {
        body = write_json(JV((hash = r.hash, url = "{g_base_url}/s/{r.hash}", created = r.created)))
    } elif (r.status == PutStatus.too_large) {
        status = http_status.PAYLOAD_TOO_LARGE
        body = write_json(JV((error = "source too large")))
    } elif (r.status == PutStatus.rate_limited) {
        status = http_status.TOO_MANY_REQUESTS
        body = write_json(JV((error = "rate limited")))
    } else {
        status = http_status.BAD_REQUEST
        body = write_json(JV((error = r.status == PutStatus.empty_source ? "empty source" : "source is not valid utf-8")))
    }
    log_request("POST", "/api/samples", int(status), ip, length(source), length(body), t0)
    return resp |> JSON(body, status)
}

def private handle_get_source(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let hash = get_param(req, "hash")
    let ip = client_ip_of(req)
    if (!is_valid_hash(hash)) {
        log_request("GET", "/api/samples/:hash", 400, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "malformed hash"))), http_status.BAD_REQUEST)
    }
    let source = get_source(g_db, hash)
    if (empty(source)) {
        log_request("GET", "/api/samples/:hash", 404, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "unknown sample"))), http_status.NOT_FOUND)
    }
    // content-addressed => immutable forever
    resp |> set_header("Cache-Control", "public, max-age=31536000, immutable")
    // The body is anonymous user input served from the site's own origin, and
    // it is cached for a year: without nosniff a browser may sniff it as script
    // or HTML, which would make any stored sample a same-origin script source.
    resp |> set_header("X-Content-Type-Options", "nosniff")
    log_request("GET", "/api/samples/:hash", 200, ip, 0, length(source), t0)
    resp |> set_content_type("text/plain; charset=utf-8")
    resp.body := source
    return http_status.OK
}

def private handle_listed(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let listed <- listed_samples(g_db)
    var arr <- [for (l in listed); JV((hash = l.hash, title = l.title, category = l.category,
        path = l.path, slug = l.slug))]
    let body = write_json(JV(arr))
    resp |> set_header("Cache-Control", "public, max-age=60")
    resp |> set_header("X-Content-Type-Options", "nosniff")
    log_request("GET", "/api/samples/listed", 200, client_ip_of(req), 0, length(body), t0)
    return resp |> JSON(body)
}

def private handle_reimport(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    if (!is_loopback_caller(req)) {
        log_request("POST", "/admin/reimport", 403, client_ip_of(req), 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "forbidden"))), http_status.FORBIDDEN)
    }
    if (empty(g_curated_dir)) {
        log_request("POST", "/admin/reimport", 503, client_ip_of(req), 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "curated_dir not configured"))), http_status.SERVICE_UNAVAILABLE)
    }
    let listed = import_curated(g_db, g_curated_dir, current_unix_time())
    let status = listed >= 0 ? http_status.OK : http_status.INTERNAL_SERVER_ERROR
    let body = write_json(JV((listed = listed)))
    log_request("POST", "/admin/reimport", int(status), client_ip_of(req), 0, length(body), t0)
    return resp |> JSON(body, status)
}

def private handle_share_link(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let hash = get_param(req, "hash")
    let ip = client_ip_of(req)
    if (!is_valid_hash(hash) || !sample_exists(g_db, hash)) {
        log_request("GET", "/s/:hash", 404, ip, 0, 0, t0)
        return resp |> TEXT_PLAIN("unknown sample", http_status.NOT_FOUND)
    }
    log_request("GET", "/s/:hash", 302, ip, 0, 0, t0)
    return resp |> REDIRECT("/playground/index.html?s={hash}", http_status.FOUND)
}

def private body_toolchain_of(var req : HttpRequest?; var ok : bool&) : string {
    //! A builder body is a bare toolchain id. A das string ends at the first
    //! NUL, so "abc1234\0junk" would validate as a clean 7-hex id and be
    //! stored as one — the same asymmetry the sample route guards against.
    let raw_bytes = length(req.body)
    let text = string(req.body)
    ok = body_is_byte_faithful(raw_bytes, text)
    return strip(text)
}

def private handle_build_toolchain(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let auth = build_auth_status(req)
    if (auth != http_status.OK) {
        return build_auth_refused("/api/build/toolchain", req, resp, auth, t0)
    }
    let ip = client_ip_of(req)
    // announcing a toolchain IS its go-live: the builder does this only after
    // the --jit-check-abi canary ran clean on that toolchain
    var faithful = false
    let toolchain = body_toolchain_of(req, faithful)
    if (!faithful || !is_valid_toolchain_id(toolchain)) {
        log_request("POST", "/api/build/toolchain", 400, ip, length(toolchain), 0, t0)
        return resp |> JSON(write_json(JV((error = "malformed toolchain id"))), http_status.BAD_REQUEST)
    }
    set_current_toolchain(g_db, toolchain)
    let body = write_json(JV((toolchain = toolchain)))
    log_request("POST", "/api/build/toolchain", 200, ip, length(toolchain), length(body), t0)
    return resp |> JSON(body)
}

def private handle_build_next(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let auth = build_auth_status(req)
    if (auth != http_status.OK) {
        return build_auth_refused("/api/build/next", req, resp, auth, t0)
    }
    let ip = client_ip_of(req)
    // the body names the BUILDER's toolchain — a claim only ever hands out
    // jobs minted for the toolchain that will actually build them
    var faithful = false
    let toolchain = body_toolchain_of(req, faithful)
    if (!faithful || !is_valid_toolchain_id(toolchain)) {
        log_request("POST", "/api/build/next", 400, ip, length(toolchain), 0, t0)
        return resp |> JSON(write_json(JV((error = "malformed toolchain id"))), http_status.BAD_REQUEST)
    }
    let job = claim_next_build(g_db, g_build.queue, toolchain, current_unix_time())
    if (empty(job.hash)) {
        log_request("POST", "/api/build/next", 204, ip, 0, 0, t0)
        return http_status.NO_CONTENT
    }
    let source = get_source(g_db, job.hash)
    if (empty(source)) {
        // sample rows are never deleted, so this cannot arise; fail the job
        // rather than let it ping-pong through the queue
        record_build_failed(g_db, job.hash, toolchain, "source missing from store", current_unix_time())
        log_request("POST", "/api/build/next", 204, ip, 0, 0, t0)
        return http_status.NO_CONTENT
    }
    let body = write_json(JV((hash = job.hash, toolchain_id = job.toolchain, mode = job.mode, source = source)))
    log_request("POST", "/api/build/next", 200, ip, 0, length(body), t0)
    return resp |> JSON(body)
}

def private parse_result_manifest(text : string) : array<ArtifactFile> {
    //! [{name, sha256}], every field validated, ≤ 16 unique names; empty
    //! result = malformed (an ok=1 result must carry at least one file).
    var out : array<ArtifactFile>
    if (empty(text)) {
        return <- out
    }
    var jerr = ""
    var doc = read_json(text, jerr)
    if (doc == null || !(doc.value is _array)) {
        return <- out
    }
    var seen : table<string>
    out |> reserve(length(doc.value as _array))
    for (e in doc.value as _array) {
        let name : string = e?["name"] ?? ""
        let sha : string = e?["sha256"] ?? ""
        if (!is_valid_artifact_filename(name) || !is_valid_artifact_hash(sha) || key_exists(seen, name)) {
            out |> clear()
            return <- out
        }
        seen |> insert(name)
        out |> push(ArtifactFile(name = name, sha256 = sha))
    }
    if (length(out) > MAX_ARTIFACT_MANIFEST_ENTRIES) {
        out |> clear()
    }
    return <- out
}

def manifest_matches_mode(files : array<ArtifactFile> const; mode : string) : bool {
    //! The uploaded set must be exactly the mode's published set — same names,
    //! no extras, no omissions.
    let wanted <- expected_artifact_names(mode)
    if (empty(wanted) || length(files) != length(wanted)) {
        return false
    }
    for (name in wanted) {
        var found = false
        for (f in files) {
            if (f.name == name) {
                found = true
            }
        }
        if (!found) {
            return false
        }
    }
    return true
}

def private handle_build_result(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let auth = build_auth_status(req)
    if (auth != http_status.OK) {
        return build_auth_refused("/api/build/result", req, resp, auth, t0)
    }
    let ip = client_ip_of(req)
    let raw_bytes = length(req.body)
    let hash = get_form_data(req, "hash")
    let toolchain = get_form_data(req, "toolchain")
    if (!is_valid_hash(hash) || !is_valid_toolchain_id(toolchain)) {
        log_request("POST", "/api/build/result", 400, ip, raw_bytes, 0, t0)
        return resp |> JSON(write_json(JV((error = "malformed hash or toolchain"))), http_status.BAD_REQUEST)
    }
    let pre = build_status(g_db, hash, toolchain)
    if (!pre.found || pre.state != BUILD_STATE_BUILDING) {
        // a result for a job that was requeued (or never claimed) is refused;
        // the artifact is content-addressed, so discarded work is cheap
        log_request("POST", "/api/build/result", 409, ip, raw_bytes, 0, t0)
        return resp |> JSON(write_json(JV((error = "job is not building"))), http_status.CONFLICT)
    }
    let ok_field = get_form_data(req, "ok")
    if (ok_field == "0") {
        // compiler error text is line-oriented; a das string ends at the first
        // NUL, which such text cannot contain, so get_form_data is byte-faithful here
        record_build_failed(g_db, hash, toolchain, get_form_data(req, "error"), current_unix_time())
        let fail_body = write_json(JV((state = BUILD_STATE_FAILED)))
        log_request("POST", "/api/build/result", 200, ip, raw_bytes, length(fail_body), t0)
        return resp |> JSON(fail_body)
    }
    if (ok_field != "1") {
        log_request("POST", "/api/build/result", 400, ip, raw_bytes, 0, t0)
        return resp |> JSON(write_json(JV((error = "ok must be \"0\" or \"1\""))), http_status.BAD_REQUEST)
    }
    if (empty(g_build.blobs_dir)) {
        log_request("POST", "/api/build/result", 503, ip, raw_bytes, 0, t0)
        return resp |> JSON(write_json(JV((error = "artifact storage not configured"))), http_status.SERVICE_UNAVAILABLE)
    }
    let files <- parse_result_manifest(get_form_data(req, "manifest"))
    if (empty(files)) {
        log_request("POST", "/api/build/result", 400, ip, raw_bytes, 0, t0)
        return resp |> JSON(write_json(JV((error = "malformed manifest"))), http_status.BAD_REQUEST)
    }
    // The build ran the user's own compile-time code, so the artifact set is
    // not the builder's to choose: it must be exactly what this mode
    // publishes, or a build could smuggle an extra file onto this origin.
    if (!manifest_matches_mode(files, pre.mode)) {
        logger_error("playground.build", "refused unexpected artifact set",
            JV((hash = hash, toolchain = toolchain, mode = pre.mode, files = length(files))))
        log_request("POST", "/api/build/result", 400, ip, raw_bytes, 0, t0)
        return resp |> JSON(write_json(JV((error = "unexpected artifact set"))), http_status.BAD_REQUEST)
    }
    let stage = create_stage_dir(g_build.blobs_dir)
    if (empty(stage)) {
        log_request("POST", "/api/build/result", 500, ip, raw_bytes, 0, t0)
        return resp |> JSON(write_json(JV((error = "cannot stage upload"))), http_status.INTERNAL_SERVER_ERROR)
    }
    for (f in files) {
        // multipart part content is binary; save_form_file writes the raw part
        // bytes — never read a file part through a das string. The destination
        // path comes from build_artifacts, which owns cache path assembly.
        let dest = stage_file_path(stage, f.name)
        if (empty(dest) || save_form_file(req, f.name, dest) != 200) {
            discard_stage(stage)
            log_request("POST", "/api/build/result", 400, ip, raw_bytes, 0, t0)
            return resp |> JSON(write_json(JV((error = "missing file part {f.name}"))), http_status.BAD_REQUEST)
        }
    }
    if (!finalize_artifact(g_build.blobs_dir, toolchain, hash, stage, files)) {
        log_request("POST", "/api/build/result", 400, ip, raw_bytes, 0, t0)
        return resp |> JSON(write_json(JV((error = "artifact verification failed"))), http_status.BAD_REQUEST)
    }
    let names <- [for (f in files); JV(f.name)]
    record_build_done(g_db, hash, toolchain, artifact_rel_dir(toolchain, hash), write_json(JV(names)), current_unix_time())
    let body = write_json(JV((state = BUILD_STATE_DONE, files = length(files))))
    log_request("POST", "/api/build/result", 200, ip, raw_bytes, length(body), t0)
    return resp |> JSON(body)
}

def private build_status_payload(hash : string; toolchain : string) : string {
    //! One JSON shape for request and status responses: the state plus
    //! whatever that state carries (position / error / artifact urls).
    let st = build_status(g_db, hash, toolchain)
    if (!st.found) {
        return write_json(JV((state = "none")))
    }
    if (st.state == BUILD_STATE_QUEUED) {
        return write_json(JV((state = st.state, position = st.position)))
    }
    if (st.state == BUILD_STATE_FAILED) {
        return write_json(JV((state = st.state, error = st.error)))
    }
    if (st.state == BUILD_STATE_DONE) {
        var urls : array<JsonValue?>
        var jerr = ""
        var names = read_json(st.artifact_files, jerr)
        if (names != null && names.value is _array) {
            urls |> reserve(length(names.value as _array))
            for (n in names.value as _array) {
                let name : string = n ?? ""
                urls |> push(JV("/api/build/artifact/{toolchain}/{hash}/{name}"))
            }
        }
        return write_json(JV((state = st.state, toolchain = toolchain, files = JV(urls))))
    }
    return write_json(JV((state = st.state)))   // building
}

def private builds_enabled : bool {
    return !empty(g_build.token) && !empty(g_build.blobs_dir)
}

def private handle_build_info(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    // The playground asks this once at load to decide whether the wasm engine
    // is offerable at all, instead of inferring it from a failed build. The
    // toolchain is a public git sha and carries no secret.
    let toolchain = current_toolchain(g_db)
    let enabled = builds_enabled() && !empty(toolchain)
    let body = write_json(JV((enabled = enabled, toolchain = toolchain)))
    resp |> set_header("Cache-Control", "no-store")
    log_request("GET", "/api/build/info", 200, client_ip_of(req), 0, length(body), t0)
    return resp |> JSON(body)
}

def private handle_build_request(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let ip = client_ip_of(req)
    let hash = get_param(req, "hash")
    if (!is_valid_hash(hash)) {
        log_request("POST", "/api/build/request/:hash", 400, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "malformed hash"))), http_status.BAD_REQUEST)
    }
    let toolchain = current_toolchain(g_db)
    if (!builds_enabled() || empty(toolchain)) {
        log_request("POST", "/api/build/request/:hash", 503, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "builds are not available yet"))), http_status.SERVICE_UNAVAILABLE)
    }
    if (!sample_exists(g_db, hash)) {
        log_request("POST", "/api/build/request/:hash", 404, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "unknown sample"))), http_status.NOT_FOUND)
    }
    // no extra limiter by design: the queue plus baking time is the throttle
    enqueue_build(g_db, hash, toolchain, BUILD_MODE_MODULE, current_unix_time())
    let body = build_status_payload(hash, toolchain)
    resp |> set_header("Cache-Control", "no-store")
    // the payload carries the stored compiler error, which is user-derived text
    resp |> set_header("X-Content-Type-Options", "nosniff")
    log_request("POST", "/api/build/request/:hash", 200, ip, 0, length(body), t0)
    return resp |> JSON(body)
}

def private handle_build_status(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let ip = client_ip_of(req)
    let hash = get_param(req, "hash")
    if (!is_valid_hash(hash)) {
        log_request("GET", "/api/build/status/:hash", 400, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "malformed hash"))), http_status.BAD_REQUEST)
    }
    let toolchain = current_toolchain(g_db)
    if (!builds_enabled() || empty(toolchain)) {
        log_request("GET", "/api/build/status/:hash", 503, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "builds are not available yet"))), http_status.SERVICE_UNAVAILABLE)
    }
    let st = build_status(g_db, hash, toolchain)
    let body = build_status_payload(hash, toolchain)
    resp |> set_header("Cache-Control", "no-store")
    // the payload carries the stored compiler error, which is user-derived text
    resp |> set_header("X-Content-Type-Options", "nosniff")
    let status = st.found ? http_status.OK : http_status.NOT_FOUND
    log_request("GET", "/api/build/status/:hash", int(status), ip, 0, length(body), t0)
    return resp |> JSON(body, status)
}

def private handle_build_artifact(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let ip = client_ip_of(req)
    // resolve_artifact_file revalidates every request-derived component; a path
    // is only ever built from parts that passed
    let path = resolve_artifact_file(g_build.blobs_dir,
        get_param(req, "toolchain"), get_param(req, "hash"), get_param(req, "file"))
    if (empty(path)) {
        log_request("GET", "/api/build/artifact", 404, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "unknown artifact"))), http_status.NOT_FOUND)
    }
    // content-addressed => immutable forever; artifacts are derived from user
    // input, so nosniff rides along like it does on stored sources
    resp |> set_header("Cache-Control", "public, max-age=31536000, immutable")
    resp |> set_header("X-Content-Type-Options", "nosniff")
    let rc = resp |> SERVE_FILE(path)
    log_request("GET", "/api/build/artifact", int(rc), ip, 0, artifact_size(path), t0)
    return rc
}

def init_server(port : int; db_path : string; policy : StorePolicy; base_url : string; curated_dir : string = ""; build : BuildOptions = BuildOptions()) : bool {
    //! Open the store, migrate to latest, import the curated set (when
    //! configured), bind loopback, start listening. Runs in the serving
    //! thread's context — module globals bind here.
    g_policy = policy
    g_base_url = base_url
    g_curated_dir = curated_dir
    g_build = build
    g_stop_requested = false
    if (!empty(g_build.blobs_dir)) {
        clear_stage_dirs(g_build.blobs_dir)   // a crash mid-upload leaks a stage dir
    }
    var open_result <- try_open_sqlite(db_path)
    if (open_result |> is_err) {
        logger_error("playground", "could not open {db_path}: {open_result |> unwrap_err}")
        return false
    }
    g_db <- open_result._value
    g_db_open = true
    g_db |> apply_recommended_pragmas()
    let applied = migrate_to_latest(g_db)
    if (applied > 0) {
        logger_info("playground", "applied {applied} migration(s)")
    }
    if (!empty(g_curated_dir)) {
        import_curated(g_db, g_curated_dir, current_unix_time())   // logs its own counts; -1 keeps prior listings
    }
    g_server = new PlaygroundServer()
    g_server->init(port)
    if (!g_server->set_bind_host("127.0.0.1")) {
        logger_error("playground", "could not bind loopback")
        shutdown_server()
        return false
    }
    let rc = g_server->start()
    if (rc != 0) {
        logger_error("playground", "could not start on port {port} (rc {rc}; port in use?)")
        shutdown_server()
        return false
    }
    g_server_started = true
    logger_info("playground", "listening on http://127.0.0.1:{port}")
    return true
}

def update_server : bool {
    //! One tick. Returns false once shutdown was requested.
    return false if (g_server == null)
    g_server->tick()
    if (g_stop_requested) {
        return false
    }
    sleep(2u)
    return true
}

def shutdown_server {
    //! Release everything in dependency order; safe after a partially failed init.
    if (g_server != null) {
        if (g_server_started) {
            g_server->stop()
        }
        g_server->cleanup()
        unsafe {
            delete g_server   // class-instance delete requires unsafe; sole owner, tick loop already stopped
        }
        g_server = null
        g_server_started = false
    }
    if (g_db_open) {
        delete g_db
        g_db_open = false
    }
    logger_info("playground", "server stopped")
}

def run_server(port : int; db_path : string; policy : StorePolicy; base_url : string; curated_dir : string = ""; build : BuildOptions = BuildOptions()) {
    //! Blocking convenience wrapper (tests): init, tick until /shutdown, teardown.
    if (init_server(port, db_path, policy, base_url, curated_dir, build)) {
        while (update_server()) {
        }
    }
    shutdown_server()
}

def private current_unix_time : int64 {
    return int64(get_clock())
}
