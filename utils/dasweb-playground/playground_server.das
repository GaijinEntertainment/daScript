options gen2
options indenting = 4

module playground_server public

require dashv/dashv_boost public
require daslib/json_boost
require daslib/logger
require samples_store public
require strings

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
var private g_stop_requested = false
var private g_server_started = false

def private client_ip_of(var req : HttpRequest?) : string {
    //! First hop of X-Forwarded-For — Caddy is the only caller in production,
    //! so the header is trustworthy; absent (direct loopback callers, tests)
    //! degrades to "local".
    let fwd = get_header(req, "X-Forwarded-For")
    if (empty(fwd)) {
        return "local"
    }
    let comma = find(fwd, ",")
    return comma < 0 ? fwd : slice(fwd, 0, comma)
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

def private log_request(method : string; path : string; status : int; ip : string; bytes_in : int; bytes_out : int) {
    logger_info("playground.req", "{method} {path} {status}",
        JV((method = method, path = path, status = status, ip = ip,
            bytes_in = bytes_in, bytes_out = bytes_out)))
}

class PlaygroundServer : HvWebServer {
    def override onInit {  // the route table: one registration per endpoint
        allow_cors()
        GET("/healthz") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("ok")
        }
        POST("/api/samples") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_put(req, resp)
        }
        GET("/api/samples/:hash") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_get_source(req, resp)
        }
        GET("/s/:hash") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return handle_share_link(req, resp)
        }
        POST("/shutdown") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            logger_info("playground", "shutdown requested")
            g_stop_requested = true
            return resp |> TEXT_PLAIN("bye")
        }
        ANY("*") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let body = write_json(JV((error = "not found")))
            log_request("{req.method}", string(req.path), 404, client_ip_of(req), 0, length(body))
            return resp |> JSON(body, http_status.NOT_FOUND)
        }
    }
}

def private handle_put(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let source = string(req.body)
    let ip = client_ip_of(req)
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
    log_request("POST", "/api/samples", int(status), ip, length(source), length(body))
    return resp |> JSON(body, status)
}

def private handle_get_source(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let hash = get_param(req, "hash")
    let ip = client_ip_of(req)
    if (!is_valid_hash(hash)) {
        log_request("GET", "/api/samples/:hash", 400, ip, 0, 0)
        return resp |> JSON(write_json(JV((error = "malformed hash"))), http_status.BAD_REQUEST)
    }
    let source = get_source(g_db, hash)
    if (empty(source)) {
        log_request("GET", "/api/samples/:hash", 404, ip, 0, 0)
        return resp |> JSON(write_json(JV((error = "unknown sample"))), http_status.NOT_FOUND)
    }
    // content-addressed => immutable forever
    resp |> set_header("Cache-Control", "public, max-age=31536000, immutable")
    log_request("GET", "/api/samples/:hash", 200, ip, 0, length(source))
    resp |> set_content_type("text/plain; charset=utf-8")
    resp.body := source
    return http_status.OK
}

def private handle_share_link(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
    let hash = get_param(req, "hash")
    let ip = client_ip_of(req)
    if (!is_valid_hash(hash) || !sample_exists(g_db, hash)) {
        log_request("GET", "/s/:hash", 404, ip, 0, 0)
        return resp |> TEXT_PLAIN("unknown sample", http_status.NOT_FOUND)
    }
    log_request("GET", "/s/:hash", 302, ip, 0, 0)
    return resp |> REDIRECT("/playground/index.html?s={hash}", http_status.FOUND)
}

def init_server(port : int; db_path : string; policy : StorePolicy; base_url : string) : bool {
    //! Open the store, migrate to latest, bind loopback, start listening.
    //! Runs in the serving thread's context — module globals bind here.
    g_policy = policy
    g_base_url = base_url
    g_stop_requested = false
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
            delete g_server
        }
        g_server = null
        g_server_started = false
    }
    if (g_db_open) {
        delete g_db
        g_db_open = false
    }
}

def run_server(port : int; db_path : string; policy : StorePolicy; base_url : string) {
    //! Blocking convenience wrapper (tests): init, tick until /shutdown, teardown.
    if (init_server(port, db_path, policy, base_url)) {
        while (update_server()) {
        }
    }
    shutdown_server()
}

def private current_unix_time : int64 {
    return int64(get_clock())
}
