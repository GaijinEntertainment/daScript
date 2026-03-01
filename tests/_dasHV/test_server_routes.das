// Tests for HTTP server routing (Tutorial HV-03)
//
// Covers: GET/POST/PUT/DEL/PATCH/HEAD/ANY routes, TEXT_PLAIN, JSON,
//         path params (:id), query params (get_param, each_param),
//         set_status, set_header on response, get_header on request

options gen2
options persistent_heap
options gc
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require _common
require daslib/json_boost
require strings

// ──────────────────────────────────────────────────────────────────────────
// Internal server with routes for all HTTP methods
// ──────────────────────────────────────────────────────────────────────────

class RouteTestServer : HvWebServer {
    def override onInit {
        // Basic GET
        GET("/hello") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            return resp |> TEXT_PLAIN("Hello, world!")
        }
        // POST echo
        POST("/echo") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            return resp |> TEXT_PLAIN(string(req.body))
        }
        // PUT
        PUT("/resource") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            return resp |> TEXT_PLAIN("PUT:{req.body}")
        }
        // PATCH
        PATCH("/resource") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            return resp |> TEXT_PLAIN("PATCH:{req.body}")
        }
        // DELETE
        DEL("/resource") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            return resp |> TEXT_PLAIN("deleted")
        }
        // HEAD
        HEAD("/resource") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            set_header(resp, "X-Resource-Exists", "true")
            set_status(resp, http_status.OK)
            return int(http_status.OK)
        }
        // ANY
        ANY("/any-method") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            return resp |> TEXT_PLAIN("method:{req.method}")
        }
        // Path parameters
        GET("/users/:id") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            let user_id = get_param(req, "id")
            return resp |> TEXT_PLAIN("user:{user_id}")
        }
        GET("/users/:id/posts/:post_id") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            let user_id = get_param(req, "id")
            let post_id = get_param(req, "post_id")
            return resp |> TEXT_PLAIN("user={user_id},post={post_id}")
        }
        // Query parameters
        GET("/search") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            var result = "query:"
            each_param(req) <| $(key, value : string) {
                result = "{result} {key}={value}"
            }
            return resp |> TEXT_PLAIN(result)
        }
        // Response headers
        GET("/headers") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            set_header(resp, "X-Custom", "test-value")
            set_header(resp, "X-Request-Id", "12345")
            return resp |> TEXT_PLAIN("with-headers")
        }
        // JSON response
        GET("/api/status") <| @(req : HttpRequest?; var resp : HttpResponse) : int {
            let payload : tuple<status : string; version : int> = ("ok", 3)
            return resp |> JSON(write_json(JV(payload)))
        }
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Tests
// ──────────────────────────────────────────────────────────────────────────

[test]
def test_get_route(t : T?) {
    var server = new RouteTestServer()
    with_test_server(server, PORT_SERVER_ROUTES) <| $(base_url) {
        t |> run("GET route returns text") @(t : T?) {
            GET("{base_url}/hello") <| $(resp) {
                t |> equal(string(resp.body), "Hello, world!")
                t |> equal(int(resp.status_code), int(http_status.OK))
            }
        }
    }
}

[test]
def test_post_route(t : T?) {
    var server = new RouteTestServer()
    with_test_server(server, PORT_SERVER_ROUTES) <| $(base_url) {
        t |> run("POST route echoes body") @(t : T?) {
            POST("{base_url}/echo", "hello server") <| $(resp) {
                t |> equal(string(resp.body), "hello server")
            }
        }
    }
}

[test]
def test_put_route(t : T?) {
    var server = new RouteTestServer()
    with_test_server(server, PORT_SERVER_ROUTES) <| $(base_url) {
        t |> run("PUT route receives body") @(t : T?) {
            with_http_request() <| $(var req) {
                req.method = http_method.PUT
                req.url := "{base_url}/resource"
                req.body := "updated"
                request(req) <| $(resp) {
                    t |> equal(string(resp.body), "PUT:updated")
                }
            }
        }
    }
}

[test]
def test_patch_route(t : T?) {
    var server = new RouteTestServer()
    with_test_server(server, PORT_SERVER_ROUTES) <| $(base_url) {
        t |> run("PATCH route receives body") @(t : T?) {
            with_http_request() <| $(var req) {
                req.method = http_method.PATCH
                req.url := "{base_url}/resource"
                req.body := "partial"
                request(req) <| $(resp) {
                    t |> equal(string(resp.body), "PATCH:partial")
                }
            }
        }
    }
}

[test]
def test_delete_route(t : T?) {
    var server = new RouteTestServer()
    with_test_server(server, PORT_SERVER_ROUTES) <| $(base_url) {
        t |> run("DEL route works") @(t : T?) {
            with_http_request() <| $(var req) {
                req.method = http_method.DELETE
                req.url := "{base_url}/resource"
                request(req) <| $(resp) {
                    t |> equal(string(resp.body), "deleted")
                }
            }
        }
    }
}

[test]
def test_head_route(t : T?) {
    var server = new RouteTestServer()
    with_test_server(server, PORT_SERVER_ROUTES) <| $(base_url) {
        t |> run("HEAD route sets headers, no body") @(t : T?) {
            with_http_request() <| $(var req) {
                req.method = http_method.HEAD
                req.url := "{base_url}/resource"
                request(req) <| $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.OK))
                    let exists = get_header(resp, "X-Resource-Exists")
                    t |> equal(string(exists), "true")
                }
            }
        }
    }
}

[test]
def test_any_route(t : T?) {
    var server = new RouteTestServer()
    with_test_server(server, PORT_SERVER_ROUTES) <| $(base_url) {
        t |> run("ANY route handles GET") @(t : T?) {
            GET("{base_url}/any-method") <| $(resp) {
                t |> success(find(string(resp.body), "method:") >= 0, "body contains method info")
            }
        }
        t |> run("ANY route handles POST") @(t : T?) {
            POST("{base_url}/any-method", "") <| $(resp) {
                t |> success(find(string(resp.body), "method:") >= 0, "body contains method info")
            }
        }
    }
}

[test]
def test_path_params(t : T?) {
    var server = new RouteTestServer()
    with_test_server(server, PORT_SERVER_ROUTES) <| $(base_url) {
        t |> run("single path param") @(t : T?) {
            GET("{base_url}/users/42") <| $(resp) {
                t |> equal(string(resp.body), "user:42")
            }
        }
        t |> run("multiple path params") @(t : T?) {
            GET("{base_url}/users/7/posts/99") <| $(resp) {
                t |> equal(string(resp.body), "user=7,post=99")
            }
        }
    }
}

[test]
def test_query_params_server(t : T?) {
    var server = new RouteTestServer()
    with_test_server(server, PORT_SERVER_ROUTES) <| $(base_url) {
        t |> run("query params accessible via each_param") @(t : T?) {
            GET("{base_url}/search?q=daslang&page=1") <| $(resp) {
                let body = string(resp.body)
                t |> success(find(body, "q=daslang") >= 0, "query contains q=daslang")
                t |> success(find(body, "page=1") >= 0, "query contains page=1")
            }
        }
    }
}

[test]
def test_response_headers_server(t : T?) {
    var server = new RouteTestServer()
    with_test_server(server, PORT_SERVER_ROUTES) <| $(base_url) {
        t |> run("set_header on response") @(t : T?) {
            GET("{base_url}/headers") <| $(resp) {
                let custom = get_header(resp, "X-Custom")
                t |> equal(string(custom), "test-value")
                let reqid = get_header(resp, "X-Request-Id")
                t |> equal(string(reqid), "12345")
            }
        }
    }
}

[test]
def test_json_response_server(t : T?) {
    var server = new RouteTestServer()
    with_test_server(server, PORT_SERVER_ROUTES) <| $(base_url) {
        t |> run("JSON response with correct content-type") @(t : T?) {
            GET("{base_url}/api/status") <| $(resp) {
                let ct = get_header(resp, "Content-Type")
                t |> success(find(string(ct), "application/json") >= 0, "Content-Type is JSON")
                // Parse the body
                var err : string
                var parsed = read_json(string(resp.body), err)
                t |> success(parsed != null, "JSON parses successfully")
                if (parsed != null) {
                    unsafe {
                        delete parsed
                    }
                }
            }
        }
    }
}
