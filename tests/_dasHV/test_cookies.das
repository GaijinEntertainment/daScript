// Tests for cookie operations (Tutorial HV-05, Section 1)
//
// Covers: add_cookie (simple + extended), get_cookie, each_cookie
//         on both HttpRequest* and HttpResponse&

options gen2
options persistent_heap
options gc
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require _common
require strings

// ──────────────────────────────────────────────────────────────────────────
// Internal server for cookie tests
// ──────────────────────────────────────────────────────────────────────────

class CookieTestServer : HvWebServer {
    def override onInit {
        // Set simple cookies on response
        GET("/set-cookies") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            add_cookie(resp, "session", "abc123")
            add_cookie(resp, "theme", "dark")
            return resp |> TEXT_PLAIN("cookies set")
        }

        // Set extended cookie with attributes
        GET("/set-extended-cookie") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            add_cookie(resp, "prefs", "dark-mode", "", "/", 3600, false, true)
            return resp |> TEXT_PLAIN("extended cookie set")
        }

        // Read cookies from request
        GET("/read-cookies") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            let session = get_cookie(req, "session")
            let theme = get_cookie(req, "theme")
            var s = !empty(session) ? string(session) : "missing"
            var th = !empty(theme) ? string(theme) : "missing"
            return resp |> TEXT_PLAIN("session={s},theme={th}")
        }

        // Read a cookie that does not exist
        GET("/read-missing-cookie") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            let missing = get_cookie(req, "nonexistent")
            let is_empty = empty(missing)
            return resp |> TEXT_PLAIN(is_empty ? "empty" : "found")
        }

        // Iterate all cookies on the request
        GET("/list-cookies") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            var result = ""
            each_cookie(req) <| $(name, value) {
                if (!empty(result)) {
                    result = "{result},"
                }
                result = "{result}{name}={value}"
            }
            return resp |> TEXT_PLAIN(result)
        }
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Tests
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_add_cookie_response_simple(t : T?) {
    with_test_server(type<CookieTestServer>, PORT_COOKIES) <| $(base_url) {
        t |> run("add_cookie sets cookies on response") @(t : T?) {
            GET("{base_url}/set-cookies") <| $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                // libhv parses Set-Cookie headers into cookies vector, not headers map
                var found_session = false
                var found_theme = false
                each_cookie(resp) <| $(name, value) {
                    if (name == "session" && value == "abc123") {
                        found_session = true
                    }
                    if (name == "theme" && value == "dark") {
                        found_theme = true
                    }
                }
                t |> success(found_session, "session cookie found")
                t |> success(found_theme, "theme cookie found")
            }
        }
    }
}

[test]
def test_add_cookie_response_extended(t : T?) {
    with_test_server(type<CookieTestServer>, PORT_COOKIES) <| $(base_url) {
        t |> run("extended cookie is received") @(t : T?) {
            GET("{base_url}/set-extended-cookie") <| $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                var found_prefs = false
                each_cookie(resp) <| $(name, value) {
                    if (name == "prefs" && value == "dark-mode") {
                        found_prefs = true
                    }
                }
                // Note: each_cookie only exposes name/value; HttpOnly and Max-Age
                // attributes are parsed into HttpCookie struct fields but not
                // currently exposed through the daScript binding
                t |> success(found_prefs, "prefs cookie found")
            }
        }
    }
}

[test]
def test_add_cookie_request(t : T?) {
    with_test_server(type<CookieTestServer>, PORT_COOKIES) <| $(base_url) {
        t |> run("add_cookie on request (simple)") @(t : T?) {
            with_http_request() <| $(var req) {
                req.method = http_method.GET
                req.url := "{base_url}/read-cookies"
                add_cookie(req, "session", "xyz789")
                add_cookie(req, "theme", "light")
                request(req) <| $(resp) {
                    t |> equal(string(resp.body), "session=xyz789,theme=light")
                }
            }
        }
    }
}

[test]
def test_get_cookie_missing(t : T?) {
    with_test_server(type<CookieTestServer>, PORT_COOKIES) <| $(base_url) {
        t |> run("get_cookie returns empty for missing cookie") @(t : T?) {
            with_http_request() <| $(var req) {
                req.method = http_method.GET
                req.url := "{base_url}/read-missing-cookie"
                request(req) <| $(resp) {
                    t |> equal(string(resp.body), "empty")
                }
            }
        }
    }
}

[test]
def test_each_cookie_iterates_all(t : T?) {
    with_test_server(type<CookieTestServer>, PORT_COOKIES) <| $(base_url) {
        t |> run("each_cookie iterates all cookies") @(t : T?) {
            with_http_request() <| $(var req) {
                req.method = http_method.GET
                req.url := "{base_url}/list-cookies"
                add_cookie(req, "a", "1")
                add_cookie(req, "b", "2")
                add_cookie(req, "c", "3")
                request(req) <| $(resp) {
                    let body = string(resp.body)
                    t |> success(find(body, "a=1") >= 0, "cookie a found")
                    t |> success(find(body, "b=2") >= 0, "cookie b found")
                    t |> success(find(body, "c=3") >= 0, "cookie c found")
                }
            }
        }
    }
}

[test]
def test_each_cookie_empty(t : T?) {
    with_test_server(type<CookieTestServer>, PORT_COOKIES) <| $(base_url) {
        t |> run("each_cookie with no cookies") @(t : T?) {
            with_http_request() <| $(var req) {
                req.method = http_method.GET
                req.url := "{base_url}/list-cookies"
                request(req) <| $(resp) {
                    t |> equal(string(resp.body), "")
                }
            }
        }
    }
}
