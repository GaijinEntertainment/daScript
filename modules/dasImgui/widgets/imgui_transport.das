options gen2
options _comment_hygiene = true
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

module imgui_transport shared public

require dashv/dashv_boost public
require daslib/json public
require daslib/json_boost public
require daslib/fio
require math

// Module overview lives in handmade/module-imgui_transport.rst.
// Wait helpers are client-side only — server-side blocking would deadlock
// the GLFW main thread, which also advances frames.

typedef Transport = lambda<(action : string; var payload : JsonValue?) : JsonValue?>

let DEFAULT_AWAIT_POLL_MS : uint = 50u
let DEFAULT_AWAIT_TIMEOUT_SEC : float = 5.0f

// Per-request HTTP timeout (seconds). Every client call sets it so a wedged host — one that accepts
// the connection but stops answering (e.g. the GLFW main thread, which also serves the live API,
// blocked mid-command) — fails in seconds instead of blocking the caller to the popen watchdog.
let public DEFAULT_REQUEST_TIMEOUT_SEC : int = 10

struct public HttpReply {
    status : int        //!< HTTP status code; 0 when there was no response (connect refused or request timed out).
    body : string       //!< Response body, or "" on no response.
}

def private clamp_timeout_sec(sec : int) : uint16 {
    // libhv's HttpRequest.timeout is uint16 seconds; clamp so a negative/oversized value can't wrap into a near-zero or multi-hour timeout that defeats fail-fast.
    return uint16(clamp(sec, 1, 65535))
}

def public http_get(url : string; timeout_sec : int = DEFAULT_REQUEST_TIMEOUT_SEC) : HttpReply {
    //! GET ``url`` with a hard per-request timeout. ``status == 0`` means no response (refused / timed out).
    var reply : HttpReply
    with_http_request() $(var req) {
        req.method = http_method.GET
        req.url := url
        req.timeout = clamp_timeout_sec(timeout_sec)
        request(req) $(resp) {
            if (resp != null) {
                reply.status = int(resp.status_code)
                reply.body = string(resp.body)
            }
        }
    }
    return reply
}

def public http_post(url, body : string; timeout_sec : int = DEFAULT_REQUEST_TIMEOUT_SEC) : HttpReply {
    //! POST ``body`` as ``application/json`` to ``url`` with a hard per-request timeout.
    var reply : HttpReply
    with_http_request() $(var req) {
        req.method = http_method.POST
        req.url := url
        req.body := body
        req.timeout = clamp_timeout_sec(timeout_sec)
        set_content_type(req, "application/json")
        request(req) $(resp) {
            if (resp != null) {
                reply.status = int(resp.status_code)
                reply.body = string(resp.body)
            }
        }
    }
    return reply
}

def public live_api_transport(url : string) : Transport {
    //! Default transport: POSTs ``{"name": action, "args": payload}`` to ``<url>/command`` and parses
    //! the JSON response. ``url`` is captured by value, so the returned lambda is safe to reuse. Uses
    //! ``http_post``'s per-request timeout, so a wedged host yields null rather than blocking the body.
    return <- @ capture(= url) (action : string; var payload : JsonValue ?) : JsonValue ? {
        var body_tab : table<string; JsonValue?>
        body_tab |> insert("name", JV(action))
        if (payload != null) {
            body_tab |> insert("args", payload)
        }
        let reply = http_post("{url}/command", write_json(JV(body_tab)))
        if (empty(reply.body)) return null
        var err_str : string
        return read_json(reply.body, err_str)
    }
}

def public send_imgui_command(transport : Transport; action : string; var payload : JsonValue?) : JsonValue? {
    //! Send one command through ``transport`` and return the parsed JSON response.
    return invoke(transport, action, payload)
}

def public receive_imgui_snapshot(transport : Transport) : JsonValue? {
    //! Convenience for ``send_imgui_command(transport, "imgui_snapshot", null)`` — fetches the full widget snapshot.
    return invoke(transport, "imgui_snapshot", null)
}

def public await_imgui(transport : Transport;
                       until : string = "quiescent";
                       timeout_sec : float = DEFAULT_AWAIT_TIMEOUT_SEC) : bool {
    //! Poll ``imgui_await`` until ``value.quiescent`` flips true; returns true on quiescence, false on timeout.
    // ref_time_ticks() is platform-specific (QPC on Windows, nanoseconds on
    // Linux/macOS) — use get_time_usec(start) for portable elapsed comparison.
    let start = ref_time_ticks()
    let timeout_usec = int(double(timeout_sec) * 1000000.0lf)
    var args_jv = JV((until = until))
    while (get_time_usec(start) < timeout_usec) {
        let r = invoke(transport, "imgui_await", args_jv)
        if (r?["value"]?["quiescent"] ?? false) return true
        sleep(DEFAULT_AWAIT_POLL_MS)
    }
    return false
}

def public await_imgui_frame(transport : Transport;
                             target_frame : int;
                             timeout_sec : float = DEFAULT_AWAIT_TIMEOUT_SEC) : bool {
    //! Poll ``imgui_await`` until server-side ``value.frame >= target_frame``; reuses the ``frame:`` arg so server flips ``quiescent`` at the right tick.
    let start = ref_time_ticks()
    let timeout_usec = int(double(timeout_sec) * 1000000.0lf)
    var args_jv = JV((frame = target_frame))
    while (get_time_usec(start) < timeout_usec) {
        let r = invoke(transport, "imgui_await", args_jv)
        if (r?["value"]?["quiescent"] ?? false) return true
        sleep(DEFAULT_AWAIT_POLL_MS)
    }
    return false
}

def public send_with_await(transport : Transport;
                           action : string;
                           var payload : JsonValue?;
                           timeout_sec : float = DEFAULT_AWAIT_TIMEOUT_SEC) : JsonValue? {
    //! Send ``action``/``payload`` and honor any ``await_*`` modifier the server merged into the response.
    //! Dispatches on ``await_frames`` / ``await_until: "quiescent"`` / ``fire_and_forget``; otherwise default-polls quiescence with ``timeout_sec``.
    var r = invoke(transport, action, payload)
    if (r == null) return null
    let af = r?["await_frames"]
    if (af != null) {
        let n = af ?? 0
        let base_frame = r?["value"]?["frame"] ?? 0
        let _ = await_imgui_frame(transport, base_frame + n, timeout_sec)
        return r
    }
    let au = r?["await_until"]
    if ((au ?? "") == "quiescent") {
        let _ = await_imgui(transport, "quiescent", timeout_sec)
        return r
    }
    let ts = r?["timeout_sec"]
    if (ts != null) {
        let _ = await_imgui(transport, "quiescent", timeout_sec)
    }
    return r
}
