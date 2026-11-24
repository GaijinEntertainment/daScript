options gen2
options indenting = 4

module buildd_client public

require dashv/dashv_boost public
require daslib/json_boost
require daslib/logger
require math
require buildd_core

//! The HTTP side of dasweb-buildd: claim, result upload, toolchain announce —
//! each one call against the playground's bearer-authed build endpoints. The
//! token rides in the Authorization header and is never logged. Zero
//! filesystem beyond handing file paths to the multipart uploader.

struct ClaimedBuild {
    ok : bool   // false = nothing claimed (idle 204, or a logged error)
    hash : string
    toolchain : string
    mode : string
    source : string
}

let HTTP_NO_RESPONSE = -1

//! Every call here runs on the service's tick thread, so each one is
//! bounded.
let CONNECT_TIMEOUT_SECONDS = 10
let CONTROL_TIMEOUT_SECONDS = 30
let UPLOAD_TIMEOUT_SECONDS = 300

struct ResultRetry {
    //! How long a result POST keeps trying while its status is unsettled.
    attempts : int = 8
    first_pause_ms : uint = 2000u
    max_pause_ms : uint = 30000u
}

def retry_pause_ms(retry : ResultRetry; attempt : int) : uint {
    //! The pause after unsettled attempt number `attempt`, counted from
    //! 1.
    var pause = min(retry.first_pause_ms, retry.max_pause_ms)
    for (_doubling in range(1, attempt)) {
        pause = min(pause * 2u, retry.max_pause_ms)
    }
    return pause
}

def private prepare_post(var req : HttpRequest?#; url : string; token : string;
                         timeout_seconds : int) {
    req.method = http_method.POST
    req.url := url
    req.connect_timeout = uint16(CONNECT_TIMEOUT_SECONDS)
    req.timeout = uint16(timeout_seconds)
    set_bearer_token_auth(req, token)
}

def claim_build(server_url : string; token : string; toolchain : string) : ClaimedBuild {
    //! POST /api/build/next with the builder's toolchain; 204 = queue idle.
    var out = ClaimedBuild()
    with_http_request() $(var req) {
        prepare_post(req, "{server_url}/api/build/next", token, CONTROL_TIMEOUT_SECONDS)
        req.body := toolchain
        request(req) $(resp) {
            if (resp == null) {
                logger_error("buildd.client", "claim failed: no response from {server_url}")
            } elif (resp.status_code == http_status.OK) {
                var jerr = ""
                var doc = read_json(string(resp.body), jerr)
                out.hash = doc?["hash"] ?? ""
                out.toolchain = doc?["toolchain_id"] ?? ""
                out.mode = doc?["mode"] ?? ""
                out.source = doc?["source"] ?? ""
                // Shape-check what the network handed us before any of it
                // reaches a path: the hash names this job's scratch directory.
                if (!is_valid_job_hash(out.hash) || !is_valid_job_toolchain(out.toolchain)) {
                    logger_error("buildd.client", "claim rejected: malformed job identity")
                    out = ClaimedBuild()
                    return
                }
                out.ok = !empty(out.source)
            } elif (int(resp.status_code) != 204) {
                logger_error("buildd.client", "claim refused: {int(resp.status_code)}")
            }
        }
    }
    return out
}

def announce_toolchain(server_url : string; token : string; toolchain : string) : bool {
    //! The go-live for a toolchain id. Only call after the ABI canary ran clean.
    var ok = false
    with_http_request() $(var req) {
        prepare_post(req, "{server_url}/api/build/toolchain", token, CONTROL_TIMEOUT_SECONDS)
        req.body := toolchain
        request(req) $(resp) {
            ok = resp != null && resp.status_code == http_status.OK
            if (!ok) {
                logger_error("buildd.client", "toolchain announce refused: {resp == null ? -1 : int(resp.status_code)}")
            }
        }
    }
    return ok
}

def is_settled_status(status : int) : bool {
    //! A status the playground itself gave. No response and a 5xx are
    //! unsettled: the same POST can still land.
    return status != HTTP_NO_RESPONSE && status < 500
}

def status_text(status : int) : string => status == HTTP_NO_RESPONSE ? "no response" : "{status}"

def post_until_settled(retry : ResultRetry; what : string; post_once : block<() : int>) : int {
    //! Repeats `post_once` until it returns a settled status; returns the
    //! last unsettled one once `retry.attempts` are spent.
    var status = HTTP_NO_RESPONSE
    for (attempt in range(1, retry.attempts + 1)) {
        status = invoke(post_once)
        return status if (is_settled_status(status))
        if (attempt < retry.attempts) {
            logger_warning("buildd.client", "{what} unsettled ({status_text(status)}); retrying",
                JV((attempt = attempt, attempts = retry.attempts)))
            sleep(retry_pause_ms(retry, attempt))
        }
    }
    return status
}

def private post_result(server_url, token, hash, toolchain, what : string; retry : ResultRetry;
                        fill : block<(var req : HttpRequest?#) : void>) : bool {
    let status = post_until_settled(retry, "{what} {hash}") $() : int {
        var status_code = HTTP_NO_RESPONSE
        with_http_request() $(var req) {
            prepare_post(req, "{server_url}/api/build/result", token, UPLOAD_TIMEOUT_SECONDS)
            set_form_data(req, "hash", hash)
            set_form_data(req, "toolchain", toolchain)
            invoke(fill, req)
            request(req) $(resp) {
                if (resp != null) {
                    status_code = int(resp.status_code)
                }
            }
        }
        return status_code
    }
    let accepted = status == int(http_status.OK)
    if (!accepted) {
        logger_error("buildd.client", "{what} not accepted: {status_text(status)}",
            JV((hash = hash, toolchain = toolchain)))
    }
    return accepted
}

def upload_success(server_url : string; token : string; hash : string; toolchain : string;
                   files : array<BuiltFile>;
                   retry : ResultRetry = ResultRetry()) : bool {
    //! Multipart result: manifest + one file part per artifact, part name =
    //! artifact name. libhv reads the files itself, so the bytes never pass
    //! through a das string.
    return post_result(server_url, token, hash, toolchain, "success upload", retry) $(var req) {
        set_form_data(req, "ok", "1")
        set_form_data(req, "manifest", manifest_json(files))
        for (f in files) {
            set_form_file(req, f.name, f.path)
        }
    }
}

def upload_failure(server_url : string; token : string; hash : string; toolchain : string;
                   error_text : string;
                   fault : bool; retry : ResultRetry = ResultRetry()) : bool {
    //! ok=0 result. `fault` false: the failure is the sample's own, and
    //! the error is stored as its verdict. `fault` true: the build
    //! service failed, and the server re-queues the job instead of
    //! judging the sample.
    return post_result(server_url, token, hash, toolchain, "failure upload", retry) $(var req) {
        set_form_data(req, "ok", "0")
        set_form_data(req, "error", error_text)
        if (fault) {
            set_form_data(req, "fault", "1")
        }
    }
}
