options gen2
options persistent_heap
options gc
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require _dashv_test_common
require strings
require _server_limits_fixture
require daslib/json_boost

// Tests for advanced HTTP server features (Tutorial HV-04)
//
// Covers: STATIC, allow_cors, REDIRECT, SERVE_FILE, DATA,
//         set_content_type (on response), custom status codes
//         (201, 204, 404)



// ──────────────────────────────────────────────────────────────────────────
// Internal server with advanced features
// ──────────────────────────────────────────────────────────────────────────

class AdvancedTestServer : HvWebServer {
    static_dir : string

    def override onInit {
        // CORS
        allow_cors()

        // Redirect
        GET("/old-path") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> REDIRECT("/new-path", http_status.MOVED_PERMANENTLY)
        }
        GET("/new-path") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("arrived at new-path")
        }

        // Custom headers
        GET("/cached") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            set_header(resp, "Cache-Control", "max-age=3600")
            set_header(resp, "ETag", "\"v1\"")
            set_header(resp, "X-Server", "daslang-test")
            return resp |> TEXT_PLAIN("cached response")
        }

        // Binary data
        GET("/binary") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let payload = "BINARYDATA12"
            return resp |> DATA(payload, 12)
        }

        // HTML with custom content-type
        GET("/html") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            set_content_type(resp, "text/html")
            resp.body := "<h1>Hello</h1>"
            return http_status.OK
        }

        // 404 JSON error
        GET("/not-found") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> JSON("\{\"error\":\"not found\"\}", http_status.NOT_FOUND)
        }

        // 201 Created
        POST("/items") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            set_header(resp, "Location", "/items/42")
            return resp |> TEXT_PLAIN("created", http_status.CREATED)
        }

        // 204 No Content
        POST("/ack") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return http_status.NO_CONTENT
        }

        // Oversize buffered file — the buffered rail cannot cover a body past libhv's
        // write-buf cap, so SERVE_FILE must refuse loudly (500), never truncate a 200
        GET("/bigfile") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> SERVE_FILE("{self.static_dir}/big.bin")
        }
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Server lifecycle with static file setup
// ──────────────────────────────────────────────────────────────────────────

def with_advanced_test_server(blk : block<(base_url : string) : void>) {
    with_job_status(1) $(started) {
        with_job_status(1) $(finished) {
            with_atomic32() $(bound) {
            with_atomic32() $(stop_flag) {
            new_thread() @() {
                var server = new AdvancedTestServer()
                server->init(0)
                let port = server->bound_port()
                let static_root = path_join(test_temp_dir(), "_static_test_{port}")
                server.static_dir = static_root
                STATIC(server.server, "/static", static_root)
                var bound_to = -1
                if (server->start() == 0) {
                    bound_to = port
                }
                mkdir_rec(static_root)
                write_test_file("{static_root}/index.html", "<h1>Test</h1>")
                write_test_file("{static_root}/data.json", "\{\"key\":\"value\"\}")
                // 17MB — past the buffered rail's write-buf budget, for the oversize-refusal test
                write_test_file("{static_root}/big.bin", repeat("0123456789abcdef", 17 * 1024 * 1024 / 16))
                bound |> set(bound_to)
                started |> notify_and_release
                while (stop_flag |> get == 0) {
                    server->tick()
                    sleep(1u)
                }
                server->stop()
                server->cleanup()
                unsafe {
                    delete server
                }
                rmdir_rec(static_root)
                finished |> notify_and_release
            }
            started |> join
            let port = bound |> get
            invoke(blk, "http://127.0.0.1:{port}")
            stop_flag |> set(1)
            finished |> join
            }
            }
        }
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Tests
// ──────────────────────────────────────────────────────────────────────────

[test(description = "The server serves HTML and JSON files with the right content type and answers 404 for a missing file")]
def test_static_file_serving(t : T?) {
    with_advanced_test_server() $(base_url) {
        t |> run("serves HTML file") @(t : T?) {
            GET("{base_url}/static/index.html") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                t |> success(find(string(resp.body), "<h1>Test</h1>") >= 0, "body contains HTML")
            }
        }
        t |> run("serves JSON file with correct content-type") @(t : T?) {
            GET("{base_url}/static/data.json") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                t |> success(find(string(resp.body), "key") >= 0, "body contains JSON data")
            }
        }
        t |> run("returns 404 for missing static file") @(t : T?) {
            GET("{base_url}/static/nonexistent.txt") $(resp) {
                t |> success(int(resp.status_code) != int(http_status.OK), "non-200 for missing file")
            }
        }
    }
}

[test(description = "CORS headers are present on the server's response")]
def test_cors(t : T?) {
    with_advanced_test_server() $(base_url) {
        t |> run("CORS headers present on response") @(t : T?) {
            with_http_request() $(var req) {
                req.method = http_method.OPTIONS
                req.url := "{base_url}/cached"
                set_header(req, "Origin", "http://example.com")
                set_header(req, "Access-Control-Request-Method", "GET")
                request(req) $(resp) {
                    let cors_origin = get_header(resp, "Access-Control-Allow-Origin")
                    t |> success(!empty(cors_origin), "Access-Control-Allow-Origin present")
                }
            }
        }
    }
}

[test(description = "REDIRECT answers 301 with a Location header and a client that follows it reaches the destination")]
def test_redirect(t : T?) {
    with_advanced_test_server() $(base_url) {
        t |> run("REDIRECT returns 301 with Location header") @(t : T?) {
            with_http_request() $(var req) {
                req.method = http_method.GET
                req.url := "{base_url}/old-path"
                allow_redirect(req, false)
                request(req) $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.MOVED_PERMANENTLY))
                    let location = get_header(resp, "Location")
                    t |> success(!empty(location), "Location header present")
                }
            }
        }
        t |> run("redirect followed arrives at destination") @(t : T?) {
            GET("{base_url}/old-path") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                t |> equal(resp.body, "arrived at new-path")
            }
        }
    }
}

[test(description = "Custom Cache-Control and ETag headers reach the client")]
def test_custom_headers_caching(t : T?) {
    with_advanced_test_server() $(base_url) {
        t |> run("custom Cache-Control and ETag headers") @(t : T?) {
            GET("{base_url}/cached") $(resp) {
                let cc = get_header(resp, "Cache-Control")
                t |> equal(cc, "max-age=3600")
                let etag = get_header(resp, "ETag")
                t |> equal(etag, "\"v1\"")
                let xs = get_header(resp, "X-Server")
                t |> equal(xs, "daslang-test")
            }
        }
    }
}

[test(description = "DATA returns binary content with the octet-stream content type")]
def test_binary_data(t : T?) {
    with_advanced_test_server() $(base_url) {
        t |> run("DATA returns binary with octet-stream content-type") @(t : T?) {
            GET("{base_url}/binary") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                t |> equal(resp.content_length, 12)
                let ct = get_header(resp, "Content-Type")
                t |> success(find(ct, "octet-stream") >= 0, "Content-Type is octet-stream")
            }
        }
    }
}

[test(description = "An HTML response carries the text/html content type")]
def test_custom_content_type(t : T?) {
    with_advanced_test_server() $(base_url) {
        t |> run("HTML response with text/html content-type") @(t : T?) {
            GET("{base_url}/html") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.OK))
                let ct = get_header(resp, "Content-Type")
                t |> success(find(ct, "text/html") >= 0, "Content-Type is text/html")
                t |> success(find(string(resp.body), "<h1>Hello</h1>") >= 0, "body is HTML")
            }
        }
    }
}

[test(description = "A 404 answer carries its JSON body")]
def test_status_404(t : T?) {
    with_advanced_test_server() $(base_url) {
        t |> run("404 error with JSON body") @(t : T?) {
            GET("{base_url}/not-found") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.NOT_FOUND))
                let ct = get_header(resp, "Content-Type")
                t |> success(find(ct, "application/json") >= 0, "Content-Type is JSON")
                t |> success(find(string(resp.body), "not found") >= 0, "body contains error message")
            }
        }
    }
}

[test(description = "A 201 Created answer carries a Location header")]
def test_status_201_created(t : T?) {
    with_advanced_test_server() $(base_url) {
        t |> run("201 Created with Location header") @(t : T?) {
            POST("{base_url}/items", "new item") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.CREATED))
                let location = get_header(resp, "Location")
                t |> equal(location, "/items/42")
            }
        }
    }
}

[test(description = "A 204 No Content answer has no body")]
def test_status_204_no_content(t : T?) {
    with_advanced_test_server() $(base_url) {
        t |> run("204 No Content") @(t : T?) {
            POST("{base_url}/ack", "done") $(resp) {
                t |> equal(int(resp.status_code), int(http_status.NO_CONTENT))
            }
        }
    }
}

[test(description = "A buffered SERVE_FILE past the write-buffer budget answers 500 and never a truncated 200")]
def test_buffered_oversize_refused(t : T?) {
    with_advanced_test_server() $(base_url) {
        t |> run("a buffered SERVE_FILE past the write-buf budget answers 500, never a truncated 200") @(t : T?) {
            GET("{base_url}/bigfile") $(resp) {
                t |> equal(int(resp.status_code), 500)
                t |> success(find(string(resp.body), "STREAM route") >= 0, "the refusal names the writer rail")
            }
        }
    }
}

// Limits must reject bodies before routing, including cumulative chunked bodies.

[test(description = "A body at the limit is served, one byte over gets 413 before any handler runs, and limits are immutable while workers run")]
def body_limit_preserves_boundary_and_rejects_before_handler(t : T?) {
    with_test_server(type<LimitedServer>) $(base_url) {
        POST("{base_url}/echo", "12345678") $(resp) {
            t |> equal(int(resp.status_code), 200)
            t |> equal(resp.body, "12345678")
        }
        POST("{base_url}/echo", "123456789") $(resp) {
            t |> equal(int(resp.status_code), 413)
        }
        GET("{base_url}/handled") $(resp) {
            t |> equal(resp.body, "1", "oversized body never reaches application code")
        }
        GET("{base_url}/locked") $(resp) {
            t |> equal(resp.body, "locked", "limits are immutable while workers run")
        }
    }
}

// Tests for HTTP server routing (Tutorial HV-03)
//
// Covers: GET/POST/PUT/DELETE/PATCH/HEAD/ANY routes, TEXT_PLAIN, JSON,
//         path params (:id), query params (get_param, each_param),
//         set_header on response, get_header on request,
//         wildcard ANY("*") priority vs literal routes (#2675 regression).



// ──────────────────────────────────────────────────────────────────────────
// Internal server with routes for all HTTP methods
// ──────────────────────────────────────────────────────────────────────────

class RouteTestServer : HvWebServer {
    def override onInit {
        // Basic GET
        GET("/hello") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("Hello, world!")
        }
        // POST echo
        POST("/echo") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN(string(req.body))
        }
        // PUT
        PUT("/resource") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("PUT:{req.body}")
        }
        // PATCH
        PATCH("/resource") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("PATCH:{req.body}")
        }
        // DELETE
        DELETE("/resource") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("deleted")
        }
        // HEAD
        HEAD("/resource") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            set_header(resp, "X-Resource-Exists", "true")
            return http_status.OK
        }
        // ANY
        ANY("/any-method") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("method:{req.method}")
        }
        // Path parameters
        GET("/users/:id") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let user_id = get_param(req, "id")
            return resp |> TEXT_PLAIN("user:{user_id}")
        }
        GET("/users/:id/posts/:post_id") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let user_id = get_param(req, "id")
            let post_id = get_param(req, "post_id")
            return resp |> TEXT_PLAIN("user={user_id},post={post_id}")
        }
        // Query parameters
        GET("/search") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            var result = "query:"
            each_param(req) $(key, value : string) {
                result = "{result} {key}={value}"
            }
            return resp |> TEXT_PLAIN(result)
        }
        // Response headers
        GET("/headers") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            set_header(resp, "X-Custom", "test-value")
            set_header(resp, "X-Request-Id", "12345")
            return resp |> TEXT_PLAIN("with-headers")
        }
        // JSON response
        GET("/api/status") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            let payload = (status = "ok", version = 3)
            return resp |> JSON(write_json(JV(payload)))
        }
    }
}

// ──────────────────────────────────────────────────────────────────────────
// Tests
// ──────────────────────────────────────────────────────────────────────────

[test(description = "A GET route returns its text")]
def test_get_route(t : T?) {
    with_test_server(type<RouteTestServer>) $(base_url) {
        t |> run("GET route returns text") @(t : T?) {
            GET("{base_url}/hello") $(resp) {
                t |> equal(resp.body, "Hello, world!")
                t |> equal(int(resp.status_code), int(http_status.OK))
            }
        }
    }
}

[test(description = "A POST route echoes its body")]
def test_post_route(t : T?) {
    with_test_server(type<RouteTestServer>) $(base_url) {
        t |> run("POST route echoes body") @(t : T?) {
            POST("{base_url}/echo", "hello server") $(resp) {
                t |> equal(resp.body, "hello server")
            }
        }
    }
}

[test(description = "A PUT route receives its body")]
def test_put_route(t : T?) {
    with_test_server(type<RouteTestServer>) $(base_url) {
        t |> run("PUT route receives body") @(t : T?) {
            PUT("{base_url}/resource", "updated") $(resp) {
                t |> equal(resp.body, "PUT:updated")
            }
        }
    }
}

[test(description = "A PATCH route receives its body")]
def test_patch_route(t : T?) {
    with_test_server(type<RouteTestServer>) $(base_url) {
        t |> run("PATCH route receives body") @(t : T?) {
            PATCH("{base_url}/resource", "partial") $(resp) {
                t |> equal(resp.body, "PATCH:partial")
            }
        }
    }
}

[test(description = "A DELETE route runs")]
def test_delete_route(t : T?) {
    with_test_server(type<RouteTestServer>) $(base_url) {
        t |> run("DELETE route works") @(t : T?) {
            with_http_request() $(var req) {
                req.method = http_method.DELETE
                req.url := "{base_url}/resource"
                request(req) $(resp) {
                    t |> equal(resp.body, "deleted")
                }
            }
        }
    }
}

[test(description = "A HEAD route sets headers and sends no body")]
def test_head_route(t : T?) {
    with_test_server(type<RouteTestServer>) $(base_url) {
        t |> run("HEAD route sets headers, no body") @(t : T?) {
            with_http_request() $(var req) {
                req.method = http_method.HEAD
                req.url := "{base_url}/resource"
                request(req) $(resp) {
                    t |> equal(int(resp.status_code), int(http_status.OK))
                    let exists = get_header(resp, "X-Resource-Exists")
                    t |> equal(exists, "true")
                }
            }
        }
    }
}

[test(description = "An ANY route handles both GET and POST")]
def test_any_route(t : T?) {
    with_test_server(type<RouteTestServer>) $(base_url) {
        t |> run("ANY route handles GET") @(t : T?) {
            GET("{base_url}/any-method") $(resp) {
                t |> success(find(string(resp.body), "method:") >= 0, "body contains method info")
            }
        }
        t |> run("ANY route handles POST") @(t : T?) {
            POST("{base_url}/any-method", "") $(resp) {
                t |> success(find(string(resp.body), "method:") >= 0, "body contains method info")
            }
        }
    }
}

[test(description = "Route path parameters bind, singly or several at once")]
def test_path_params(t : T?) {
    with_test_server(type<RouteTestServer>) $(base_url) {
        t |> run("single path param") @(t : T?) {
            GET("{base_url}/users/42") $(resp) {
                t |> equal(resp.body, "user:42")
            }
        }
        t |> run("multiple path params") @(t : T?) {
            GET("{base_url}/users/7/posts/99") $(resp) {
                t |> equal(resp.body, "user=7,post=99")
            }
        }
    }
}

[test(description = "Query parameters are visible to the server through each_param")]
def test_query_params_server(t : T?) {
    with_test_server(type<RouteTestServer>) $(base_url) {
        t |> run("query params accessible via each_param") @(t : T?) {
            GET("{base_url}/search?q=daslang&page=1") $(resp) {
                let body = string(resp.body)
                t |> success(find(body, "q=daslang") >= 0, "query contains q=daslang")
                t |> success(find(body, "page=1") >= 0, "query contains page=1")
            }
        }
    }
}

[test(description = "set_header on a response reaches the client")]
def test_response_headers_server(t : T?) {
    with_test_server(type<RouteTestServer>) $(base_url) {
        t |> run("set_header on response") @(t : T?) {
            GET("{base_url}/headers") $(resp) {
                let custom = get_header(resp, "X-Custom")
                t |> equal(custom, "test-value")
                let reqid = get_header(resp, "X-Request-Id")
                t |> equal(reqid, "12345")
            }
        }
    }
}

[test(description = "A JSON response carries the JSON content type")]
def test_json_response_server(t : T?) {
    with_test_server(type<RouteTestServer>) $(base_url) {
        t |> run("JSON response with correct content-type") @(t : T?) {
            GET("{base_url}/api/status") $(resp) {
                let ct = get_header(resp, "Content-Type")
                t |> success(find(ct, "application/json") >= 0, "Content-Type is JSON")
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

// ──────────────────────────────────────────────────────────────────────────
// Wildcard priority — regression for #2675
// ──────────────────────────────────────────────────────────────────────────
//
// Before libhv PR #836 (trie router), pathHandlers was an unordered_map
// and HttpService::GetRoute iterated in container order, taking the first
// wildcard or path match it found. ANY("*") could enumerate before
// specific paths like /status, so every request hit the catch-all
// (POSIX libstdc++ reliably, Windows MSVC intermittently). Symptom in
// daslang-live: /shutdown never reached request_exit(); driver popen
// hung the full 120s watchdog. See GaijinEntertainment/daScript#2675.
//
// libhv master (≥ 5da5fc22) replaces pathHandlers with a trie that
// enforces Literal > Param > Wildcard. This test pins the contract.

class WildcardPriorityServer : HvWebServer {
    def override onInit {
        GET("/status") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("specific")
        }
        ANY("*") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
            return resp |> TEXT_PLAIN("catchall")
        }
    }
}

[test(description = "A literal route outranks ANY * and ANY * catches unknown GET and POST paths")]
def test_wildcard_priority(t : T?) {
    with_test_server(type<WildcardPriorityServer>) $(base_url) {
        t |> run("literal GET /status outranks ANY *") @(t : T?) {
            GET("{base_url}/status") $(resp) {
                t |> equal(resp.body, "specific")
            }
        }
        t |> run("ANY * catches unknown GET paths") @(t : T?) {
            GET("{base_url}/unknown") $(resp) {
                t |> equal(resp.body, "catchall")
            }
        }
        t |> run("ANY * catches unknown POST paths") @(t : T?) {
            POST("{base_url}/unknown", "") $(resp) {
                t |> equal(resp.body, "catchall")
            }
        }
    }
}
