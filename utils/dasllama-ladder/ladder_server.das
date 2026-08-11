options gen2
options indenting = 4

module ladder_server public

require dashv/dashv_boost public
require daslib/json_boost
require daslib/logger
require daslib/fio
require daslib/strings_boost
require daslib/strings_convert
require ladder_store public
require strings

//! The HTTP surface: route table and handlers. A handler validates
//! transport-level shape, translates HTTP to one ladder_store call, and
//! formats the response. Policy (size, rate, source stamping) lives in the store.

// Lifecycle-owned state. Module-global: the server runs on its own context
// (per-thread in tests), and JIT heap collection cannot see stack locals as
// GC roots — handlers and the tick loop reach everything through these.
var private g_server : LadderServer?
var private g_db = SqlRunner()
var private g_db_open = false
var private g_policy = LadderPolicy()
var private g_stop_requested = false
var private g_server_started = false

def private client_ip_of(var req : HttpRequest?) : string {
    //! RIGHTMOST X-Forwarded-For hop. A reverse proxy APPENDS the peer it
    //! accepted, so only the last hop is proxy-authored — taking the first
    //! would let a caller pick its own rate-limit bucket.
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
    //! Exact-match only: an unknown or empty peer counts as remote.
    return peer == "127.0.0.1" || peer == "::1" || peer == "::ffff:127.0.0.1"
}

def private is_loopback_caller(var req : HttpRequest?) : bool {
    //! Operator-surface gate on the transport peer — the one identity a caller
    //! cannot forge through the proxy.
    return is_loopback_peer(client_ip(req))
}

def is_valid_sha(h : string) : bool {
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

def body_is_byte_faithful(buffered_bytes : int; text : string) : bool {
    //! A das string ends at the first NUL, so `text` can be a silent prefix of
    //! what arrived — a truncated body would be validated and stored as if it
    //! were the whole document. Only the buffered byte count knows.
    return buffered_bytes == length(text)
}

def private log_request(method : string; path : string; status : int; ip : string; bytes_in : int; bytes_out : int; t0 : int64) {
    logger_info("ladder.req", "{method} {path} {status}",
        JV((method = method, path = path, status = status, ip = ip,
            bytes_in = bytes_in, bytes_out = bytes_out,
            duration_ms = get_time_usec(t0) / 1000)))
}

class LadderServer : HvWebServer {
    def override onInit {  // the route table: one registration per endpoint
        // No allow_cors(): the site is same-origin behind the dasllama.io vhost.
        GET("/healthz") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("ok")
        }
        GET("/api/versions") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_versions(req, resp)
        }
        GET("/api/runs") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_runs(req, resp)
        }
        GET("/api/submission/:id") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_submission_doc(req, resp)
        }
        GET("/api/sidecars") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_find_sidecars(req, resp)
        }
        GET("/api/sidecar/:sha") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_sidecar_doc(req, resp)
        }
        POST("/api/submit/sidecar") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_submit_sidecar(req, resp)
        }
        POST("/api/submit/records") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_submit_records(req, resp)
        }
        POST("/admin/import-official") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_import_official(req, resp)
        }
        POST("/shutdown") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let t0 = ref_time_ticks()
            if (!is_loopback_caller(req)) {
                log_request("POST", "/shutdown", 403, client_ip_of(req), 0, 0, t0)
                return resp |> TEXT_PLAIN("forbidden", http_status.FORBIDDEN)
            }
            logger_info("ladder", "shutdown requested")
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

def private handle_versions(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let vs <- list_versions(g_db)
    var arr <- [for (v in vs); JV(v)]
    let body = write_json(JV((versions = JV(arr))))
    resp |> set_header("Cache-Control", "no-store")
    resp |> set_header("X-Content-Type-Options", "nosniff")
    log_request("GET", "/api/versions", 200, client_ip_of(req), 0, length(body), t0)
    return resp |> JSON(body)
}

def private run_to_jv(r : Run) : JsonValue? {
    return JV((
        id = r.Id, submission_id = r.SubmissionId, model_idx = r.ModelIdx, run_idx = r.RunIdx,
        gguf = r.Gguf, model_arch = r.ModelArch, quant = r.Quant,
        params_b = r.ParamsB, active_b = r.ActiveB, size_bytes = r.SizeBytes,
        engine = r.Engine, backend = r.Backend, flavor = r.Flavor, workload = r.Workload,
        box = r.Box, threads = r.Threads, date = r.RunDate,
        version = r.Version, dasllama_version = r.DasllamaVersion,
        engine_sha = r.EngineSha, tune_sha = r.TuneSha,
        source = r.Source, verified = r.Verified != 0))
}

def private handle_runs(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let ip = client_ip_of(req)
    // ?version=N scopes to one dasllama version; absent = the whole board
    let vparam = get_param(req, "version")
    var version = 0
    if (!empty(vparam)) {
        let parsed = try_to_int(vparam)
        if (!(parsed |> is_ok)) {
            log_request("GET", "/api/runs", 400, ip, 0, 0, t0)
            return resp |> JSON(write_json(JV((error = "malformed version"))), http_status.BAD_REQUEST)
        }
        version = parsed ?? 0
    }
    let rows <- list_runs(g_db, version)
    var arr <- [for (r in rows); run_to_jv(r)]
    let body = write_json(JV((version = version, runs = JV(arr))))
    resp |> set_header("Cache-Control", "no-store")
    resp |> set_header("X-Content-Type-Options", "nosniff")
    log_request("GET", "/api/runs", 200, ip, 0, length(body), t0)
    return resp |> JSON(body)
}

def private handle_submission_doc(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let ip = client_ip_of(req)
    let parsed = try_to_int(get_param(req, "id"))
    if (!(parsed |> is_ok) || (parsed ?? 0) < 1) {
        log_request("GET", "/api/submission/:id", 400, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "malformed id"))), http_status.BAD_REQUEST)
    }
    let doc = get_submission_doc(g_db, parsed ?? 0)
    if (empty(doc)) {
        log_request("GET", "/api/submission/:id", 404, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "unknown submission"))), http_status.NOT_FOUND)
    }
    // bounded cache: an admin delete must eventually reach cached copies
    resp |> set_header("Cache-Control", "public, max-age=3600")
    resp |> set_header("X-Content-Type-Options", "nosniff")
    log_request("GET", "/api/submission/:id", 200, ip, 0, length(doc), t0)
    return resp |> JSON(doc)
}

def private handle_find_sidecars(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let ip = client_ip_of(req)
    let parsed = try_to_int(get_param(req, "version"))
    if (!(parsed |> is_ok) || (parsed ?? 0) < 1) {
        log_request("GET", "/api/sidecars", 400, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "version is required (the exchange keys on it)"))), http_status.BAD_REQUEST)
    }
    var ident : BoxIdentity
    if (!parse_box_string(get_param(req, "box"), ident)) {
        log_request("GET", "/api/sidecars", 400, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "box must be the five-field platform|arch|model_id|os_build|cpu form"))), http_status.BAD_REQUEST)
    }
    let hits <- find_sidecars(g_db, parsed ?? 0, ident)
    var arr <- [for (h in hits); JV((sha = h.sha, box = h.box, tier = h.tier,
        verified = h.verified, noise = h.noise, written = h.written,
        kernel_count = h.kernel_count, url = "/api/sidecar/{h.sha}"))]
    let body = write_json(JV((version = parsed ?? 0, hits = JV(arr))))
    resp |> set_header("Cache-Control", "no-store")
    resp |> set_header("X-Content-Type-Options", "nosniff")
    log_request("GET", "/api/sidecars", 200, ip, 0, length(body), t0)
    return resp |> JSON(body)
}

def private handle_sidecar_doc(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let ip = client_ip_of(req)
    let sha = get_param(req, "sha")
    if (!is_valid_sha(sha)) {
        log_request("GET", "/api/sidecar/:sha", 400, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "malformed sha"))), http_status.BAD_REQUEST)
    }
    let doc = get_sidecar_doc(g_db, sha)
    if (empty(doc)) {
        log_request("GET", "/api/sidecar/:sha", 404, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "unknown sidecar"))), http_status.NOT_FOUND)
    }
    // bounded cache despite content addressing: an admin delete of a hostile
    // sidecar must eventually reach cached copies
    resp |> set_header("Cache-Control", "public, max-age=3600")
    resp |> set_header("X-Content-Type-Options", "nosniff")
    resp |> set_header("Content-Disposition", "attachment; filename=\"{sha}.tune.json\"")
    log_request("GET", "/api/sidecar/:sha", 200, ip, 0, length(doc), t0)
    return resp |> JSON(doc)
}

def private submit_body_of(var req : HttpRequest?; route : string; var resp : HttpResponse?;
                           var status : http_status&; t0 : int64) : string {
    //! Shared NUL guard: returns "" and fills `status` when the body cannot be
    //! taken at face value ("" never validates downstream, so it is safe).
    let raw_bytes = length(req.body)
    let text = string(req.body)
    if (!body_is_byte_faithful(raw_bytes, text)) {
        let body = write_json(JV((error = "document contains NUL bytes")))
        log_request("POST", route, 400, client_ip_of(req), raw_bytes, length(body), t0)
        status = resp |> JSON(body, http_status.BAD_REQUEST)
        return ""
    }
    return text
}

def private put_status_of(status : PutStatus) : http_status {
    if (status == PutStatus.too_large) {
        return http_status.PAYLOAD_TOO_LARGE
    }
    if (status == PutStatus.rate_limited) {
        return http_status.TOO_MANY_REQUESTS
    }
    if (status == PutStatus.ok) {
        return http_status.OK
    }
    return http_status.BAD_REQUEST
}

def private handle_submit_sidecar(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let ip = client_ip_of(req)
    var early = http_status.OK
    let doc = submit_body_of(req, "/api/submit/sidecar", resp, early, t0)
    if (empty(doc)) {
        return early
    }
    let r = put_sidecar(g_db, doc, ip, g_policy, current_unix_time())
    let status = put_status_of(r.status)
    var body = ""
    if (r.status == PutStatus.ok) {
        body = write_json(JV((sha = r.sha, created = r.created, url = "/api/sidecar/{r.sha}")))
    } elif (r.status == PutStatus.invalid) {
        body = write_json(JV((error = r.error)))
    } else {
        body = write_json(JV((error = "{r.status}")))
    }
    log_request("POST", "/api/submit/sidecar", int(status), ip, length(doc), length(body), t0)
    return resp |> JSON(body, status)
}

def private handle_submit_records(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let ip = client_ip_of(req)
    var early = http_status.OK
    let doc = submit_body_of(req, "/api/submit/records", resp, early, t0)
    if (empty(doc)) {
        return early
    }
    let r = put_records(g_db, doc, ip, g_policy, current_unix_time())
    let status = put_status_of(r.status)
    var body = ""
    if (r.status == PutStatus.ok) {
        body = write_json(JV((submission_id = r.submission_id, runs_added = r.runs_added, created = r.created)))
    } elif (r.status == PutStatus.invalid) {
        body = write_json(JV((error = r.error)))
    } else {
        body = write_json(JV((error = "{r.status}")))
    }
    log_request("POST", "/api/submit/records", int(status), ip, length(doc), length(body), t0)
    return resp |> JSON(body, status)
}

def private handle_import_official(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let t0 = ref_time_ticks()
    let ip = client_ip_of(req)
    if (!is_loopback_caller(req)) {
        log_request("POST", "/admin/import-official", 403, ip, 0, 0, t0)
        return resp |> JSON(write_json(JV((error = "forbidden"))), http_status.FORBIDDEN)
    }
    var early = http_status.OK
    let doc = submit_body_of(req, "/admin/import-official", resp, early, t0)
    if (empty(doc)) {
        return early
    }
    let r = import_official_store(g_db, doc, current_unix_time())
    let status = r.ok ? http_status.OK : http_status.BAD_REQUEST
    let body = (r.ok
        ? write_json(JV((runs_added = r.runs_added, runs_replaced = r.runs_replaced)))
        : write_json(JV((error = r.error))))
    log_request("POST", "/admin/import-official", int(status), ip, length(doc), length(body), t0)
    return resp |> JSON(body, status)
}

def import_official_dir(db : SqlRunner; dir_path : string; now : int64) : int {
    //! Startup import: every `records/<box>.json` store in the dir (single-dot
    //! names only — tune sidecars, annotations, and `_` scratch are not stores).
    //! Returns imported-store count; each file logs its own result.
    var imported = 0
    dir(dir_path) $(name) {
        let dotp = find(name, ".")
        let is_store = ((name |> ends_with(".json")) && dotp == length(name) - 5
            && name != "annotations.json" && !(name |> starts_with("_")))
        if (is_store) {
            let text = fread(path_join(dir_path, name))
            let r = import_official_store(db, text, now)
            if (r.ok) {
                imported++
                logger_info("ladder", "official import {name}",
                    JV((file = name, added = r.runs_added, replaced = r.runs_replaced)))
            } else {
                logger_error("ladder", "official import FAILED {name}: {r.error}")
            }
        }
    }
    return imported
}

def init_server(port : int; db_path : string; policy : LadderPolicy; official_dir : string = "") : bool {
    //! Open the store, migrate to latest, import the official boards (when
    //! configured), bind loopback, start listening. Runs in the serving
    //! thread's context — module globals bind here.
    g_policy = policy
    g_stop_requested = false
    var open_result <- try_open_sqlite(db_path)
    if (open_result |> is_err) {
        logger_error("ladder", "could not open {db_path}: {open_result |> unwrap_err}")
        return false
    }
    g_db <- open_result._value
    g_db_open = true
    g_db |> apply_recommended_pragmas()
    let applied = migrate_to_latest(g_db)
    if (applied > 0) {
        logger_info("ladder", "applied {applied} migration(s)")
    }
    if (!empty(official_dir)) {
        let n = import_official_dir(g_db, official_dir, current_unix_time())
        logger_info("ladder", "imported {n} official store(s)")
    }
    g_server = new LadderServer()
    g_server->init(port)
    if (!g_server->set_bind_host("127.0.0.1")) {
        logger_error("ladder", "could not bind loopback")
        shutdown_server()
        return false
    }
    let rc = g_server->start()
    if (rc != 0) {
        logger_error("ladder", "could not start on port {port} (rc {rc}; port in use?)")
        shutdown_server()
        return false
    }
    g_server_started = true
    logger_info("ladder", "listening on http://127.0.0.1:{port}")
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
    logger_info("ladder", "server stopped")
}

def run_server(port : int; db_path : string; policy : LadderPolicy; official_dir : string = "") {
    //! Blocking convenience wrapper (tests): init, tick until /shutdown, teardown.
    if (init_server(port, db_path, policy, official_dir)) {
        while (update_server()) {
        }
    }
    shutdown_server()
}

def private current_unix_time : int64 {
    return int64(get_clock())
}
