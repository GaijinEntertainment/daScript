options gen2
options indenting = 4

module buildd_client public

require dashv/dashv_boost public
require daslib/json_boost
require daslib/logger
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

def claim_build(server_url : string; token : string; toolchain : string) : ClaimedBuild {
    //! POST /api/build/next with the builder's toolchain; 204 = queue idle.
    var out = ClaimedBuild()
    with_http_request() $(var req) {
        req.method = http_method.POST
        req.url := "{server_url}/api/build/next"
        set_header(req, "Authorization", "Bearer {token}")
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
        req.method = http_method.POST
        req.url := "{server_url}/api/build/toolchain"
        set_header(req, "Authorization", "Bearer {token}")
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

def upload_success(server_url : string; token : string; hash : string; toolchain : string; files : array<BuiltFile>) : bool {
    //! Multipart result: manifest + one file part per artifact, part name =
    //! artifact name. libhv reads the files itself, so the bytes never pass
    //! through a das string.
    var ok = false
    with_http_request() $(var req) {
        req.method = http_method.POST
        req.url := "{server_url}/api/build/result"
        set_header(req, "Authorization", "Bearer {token}")
        set_form_data(req, "hash", hash)
        set_form_data(req, "toolchain", toolchain)
        set_form_data(req, "ok", "1")
        set_form_data(req, "manifest", manifest_json(files))
        for (f in files) {
            set_form_file(req, f.name, f.path)
        }
        request(req) $(resp) {
            ok = resp != null && resp.status_code == http_status.OK
            if (!ok) {
                logger_error("buildd.client", "result upload refused: {resp == null ? -1 : int(resp.status_code)}",
                    JV((hash = hash, toolchain = toolchain)))
            }
        }
    }
    return ok
}

def upload_failure(server_url : string; token : string; hash : string; toolchain : string; error_text : string) : bool {
    //! ok=0 result: the compiler/build error becomes the user-facing failure.
    var ok = false
    with_http_request() $(var req) {
        req.method = http_method.POST
        req.url := "{server_url}/api/build/result"
        set_header(req, "Authorization", "Bearer {token}")
        set_form_data(req, "hash", hash)
        set_form_data(req, "toolchain", toolchain)
        set_form_data(req, "ok", "0")
        set_form_data(req, "error", error_text)
        request(req) $(resp) {
            ok = resp != null && resp.status_code == http_status.OK
            if (!ok) {
                logger_error("buildd.client", "failure upload refused: {resp == null ? -1 : int(resp.status_code)}",
                    JV((hash = hash, toolchain = toolchain)))
            }
        }
    }
    return ok
}
