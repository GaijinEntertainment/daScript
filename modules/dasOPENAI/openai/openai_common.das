options gen2
options indenting = 4
options rtti  // honor @optional / @rename in this module's structs (consumer roots must also set it)

//! OpenAI-compatible API — shared core: client config, the unified error type, and the HTTP
//! transport (JSON POST/GET, POST-for-bytes) reused by every endpoint module.
//!
//! Pure-das over ``dashv`` (HTTP transport) + ``daslib/json_boost`` (serialization).
//!
//! IMPORTANT: consumer root scripts MUST set ``options rtti`` — otherwise the ``@optional`` /
//! ``@rename`` field annotations are ignored and requests serialize every field (e.g. ``"n":0``),
//! which real OpenAI rejects.

module openai_common shared public

require dashv/dashv_boost public
require daslib/json_boost public
require strings public

// ===== Client =====

struct OpenAIClient {
    //! Connection + auth config. Override ``base_url`` to target any OpenAI-compatible server.
    base_url : string = "https://api.openai.com/v1"
    api_key : string
    organization : string
    timeout_seconds : int = 60
    default_model : string
}

def openai_client(base_url : string; api_key : string = "") : OpenAIClient {
    //! Builds a client for the given base URL (no trailing slash) and optional API key.
    return OpenAIClient(base_url = base_url, api_key = api_key)
}

// ===== Errors =====

struct OpenAIError {
    //! Unified error. ``kind`` is "transport" | "http" | "api" | "decode".
    kind : string
    status : int
    message : string
    _type : string
    code : string
}

struct private OpenAIErrorBody {
    message : string
    @rename = "type" _type : string
    @optional param : string
    @optional code : string
}

struct private OpenAIErrorEnvelope {
    error : OpenAIErrorBody
}

// ===== Shared types =====

struct Usage {
    //! Token accounting returned with most responses.
    prompt_tokens : int
    completion_tokens : int
    total_tokens : int
}

// ===== Transport =====
// Public so endpoint modules (openai_chat, openai_embeddings, ...) can reuse them.

struct HttpOutcome {
    //! Internal transport result of a JSON request (``post_json`` / ``get_json``).
    ok : bool
    status : int
    body : string
    error : OpenAIError
}

struct BytesOutcome {
    //! Internal transport result of a binary request (``post_for_bytes``).
    ok : bool
    status : int
    bytes : array<uint8>
    error : OpenAIError
}

def parse_error_body(status : int; body : string) : OpenAIError {
    //! Decodes an OpenAI ``{"error":{...}}`` body into an ``OpenAIError`` (falls back to a plain HTTP error).
    var err = OpenAIError(kind = "http", status = status, message = "HTTP {status}")
    var env : OpenAIErrorEnvelope
    if (sscan_json(body, env) && !empty(env.error.message)) {
        err.kind = "api"
        err.message = env.error.message
        err._type = env.error._type
        err.code = env.error.code
    }
    return err
}

def configure_request(var req : HttpRequest?#; client : OpenAIClient) {
    //! Applies auth (Bearer), the optional OpenAI-Organization header, and the client timeout to a
    //! manually-built request — shared by every transport path so they behave identically.
    if (!empty(client.api_key)) {
        set_bearer_token_auth(req, client.api_key)
    }
    if (!empty(client.organization)) {
        set_header(req, "OpenAI-Organization", client.organization)
    }
    if (client.timeout_seconds > 0) {
        set_timeout(req, client.timeout_seconds)
    }
}

def percent_encode(s : string) : string {
    //! Percent-encodes a string for safe use as a single URL path segment (e.g. a model id like
    //! "google/gemma-..." whose '/' would otherwise be parsed as extra path segments).
    let HEX = "0123456789ABCDEF"
    var out = ""
    peek_data(s) $(bytes) {
        for (i in range(length(bytes))) {
            let c = int(bytes[i])
            // unreserved per RFC 3986: ALPHA / DIGIT / '-' / '_' / '.' / '~'
            if (is_alpha(c) || is_number(c) || c == 45 || c == 95 || c == 46 || c == 126) {
                out += slice(s, i, i + 1)
            } else {
                out += "%"
                out += slice(HEX, c / 16, c / 16 + 1)
                out += slice(HEX, c % 16, c % 16 + 1)
            }
        }
    }
    return out
}

def post_json(client : OpenAIClient; path : string; body_json : string) : HttpOutcome {
    //! POSTs a JSON body and returns the raw outcome (status + body, or error).
    var out : HttpOutcome
    let url = "{client.base_url}{path}"
    with_http_request() $(var req : HttpRequest?#) {
        req.method = http_method.POST
        req.url := url
        req.body := body_json
        set_content_type(req, "application/json")
        configure_request(req, client)
        request(req) $(var resp : HttpResponse?) {
            if (resp == null) {
                out.error = OpenAIError(kind = "transport", message = "no response from {url}")
                return
            }
            out.status = int(resp.status_code)
            out.body = "{resp.body}"
            if (out.status >= 200 && out.status < 300) {
                out.ok = true
            } else {
                out.error = parse_error_body(out.status, out.body)
            }
        }
    }
    return out
}

def get_json(client : OpenAIClient; path : string) : HttpOutcome {
    //! GETs ``path`` (with auth + org header + timeout) and returns the raw outcome.
    var out : HttpOutcome
    let url = "{client.base_url}{path}"
    with_http_request() $(var req : HttpRequest?#) {
        req.method = http_method.GET
        req.url := url
        configure_request(req, client)
        request(req) $(var resp : HttpResponse?) {
            if (resp == null) {
                out.error = OpenAIError(kind = "transport", message = "no response from {url}")
                return
            }
            out.status = int(resp.status_code)
            out.body = "{resp.body}"
            if (out.status >= 200 && out.status < 300) {
                out.ok = true
            } else {
                out.error = parse_error_body(out.status, out.body)
            }
        }
    }
    return out
}

def post_for_bytes(client : OpenAIClient; path : string; body_json : string) : BytesOutcome {
    //! POSTs a JSON body and returns the raw response bytes (e.g. audio), or a parsed error.
    var out : BytesOutcome
    let url = "{client.base_url}{path}"
    with_http_request() $(var req : HttpRequest?#) {
        req.method = http_method.POST
        req.url := url
        req.body := body_json
        set_content_type(req, "application/json")
        configure_request(req, client)
        request(req) $(var resp : HttpResponse?) {
            if (resp == null) {
                out.error = OpenAIError(kind = "transport", message = "no response from {url}")
                return
            }
            out.status = int(resp.status_code)
            if (out.status >= 200 && out.status < 300) {
                out.ok = true
                out.bytes <- get_body_bytes(resp)
            } else {
                out.error = parse_error_body(out.status, "{resp.body}")
            }
        }
    }
    return <- out
}

// ===== Decode helpers =====
// The typed "send request -> decode JSON response into a struct" wrappers in each endpoint
// module are identical except for the path and response type; these capture that body once.

def post_and_decode(client : OpenAIClient; path, body_json : string; var response : auto(RT)) : OpenAIError {
    //! POSTs a JSON body, then decodes a 2xx response into ``response``.
    //! Returns a default (empty-``kind``) OpenAIError on success, or the transport/decode error.
    let out = post_json(client, path, body_json)
    if (!out.ok) return out.error
    if (!sscan_json(out.body, response)) {
        return OpenAIError(kind = "decode", status = out.status, message = "failed to decode {path} response")
    }
    return OpenAIError()
}

def get_and_decode(client : OpenAIClient; path : string; var response : auto(RT)) : OpenAIError {
    //! GETs ``path``, then decodes a 2xx response into ``response`` (see ``post_and_decode``).
    let out = get_json(client, path)
    if (!out.ok) return out.error
    if (!sscan_json(out.body, response)) {
        return OpenAIError(kind = "decode", status = out.status, message = "failed to decode {path} response")
    }
    return OpenAIError()
}
