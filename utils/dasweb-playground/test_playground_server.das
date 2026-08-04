options gen2
options persistent_heap

require dastest/testing_boost public
require playground_server
require daslib/jobque_boost
require daslib/json_boost
require daslib/fio
require strings

let TEST_PORT = 19011   // 19001..19010 are taken by tests/dasHV

// Local harness: playground_server keeps lifecycle state in module globals,
// which are per-context — so ALL setup (store open included) happens inside
// the server thread, and shutdown goes through POST /shutdown (which flips
// the serving context's own flag).
def private with_playground_server(policy : StorePolicy; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            run_server(TEST_PORT, ":memory:", policy, "https://test.example")
            finished |> notify_and_release
        }
        let base_url = "http://127.0.0.1:{TEST_PORT}"
        var ready = false
        for (_attempt in range(100)) {
            GET("{base_url}/healthz") $(resp) {
                if (resp != null && resp.status_code == http_status.OK) {
                    ready = true
                }
            }
            if (ready) {
                break
            }
            sleep(100u)
        }
        if (ready) {
            invoke(blk, base_url)
        }
        POST("{base_url}/shutdown", "") $(_resp) {}
        finished |> join
        assert(ready, "playground server became ready")
    }
}

def private default_policy : StorePolicy {
    return StorePolicy()
}

def private tiny_policy : StorePolicy {
    return StorePolicy(max_source_bytes = 64, max_inserts_per_ip_hour = 3)
}

def private posted_hash(base_url : string; source : string) : string {
    var hash = ""
    POST("{base_url}/api/samples", source) $(resp) {
        if (resp != null && resp.status_code == http_status.OK) {
            var jerr = ""
            var doc = read_json(string(resp.body), jerr)
            hash = doc?["hash"] ?? ""
        }
    }
    return hash
}

[test]
def test_health_and_unknown_routes(t : T?) {
    with_playground_server(default_policy()) $(base_url) {
        t |> run("healthz") @(tt : T?) {
            GET("{base_url}/healthz") $(resp) {
                tt |> success(resp != null, "response arrived")
                tt |> equal(int(resp.status_code), 200)
                tt |> equal(string(resp.body), "ok")
            }
        }
        t |> run("unknown route is a json 404") @(tt : T?) {
            GET("{base_url}/no/such/route") $(resp) {
                tt |> equal(int(resp.status_code), 404)
                tt |> success(find(string(resp.body), "not found") >= 0, "json error body")
            }
        }
    }
}

[test]
def test_put_get_roundtrip(t : T?) {
    with_playground_server(default_policy()) $(base_url) {
        let source = "print(\"shared via /s/\")"
        t |> run("post mints hash and url") @(tt : T?) {
            POST("{base_url}/api/samples", source) $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var jerr = ""
                var doc = read_json(string(resp.body), jerr)
                let hash : string = doc?["hash"] ?? ""
                tt |> equal(hash, sha256_hex(source))
                tt |> equal(doc?["url"] ?? "", "https://test.example/s/{hash}")
                tt |> success(doc?["created"] ?? false, "first post creates")
            }
        }
        t |> run("second post dedups") @(tt : T?) {
            POST("{base_url}/api/samples", source) $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var jerr = ""
                var doc = read_json(string(resp.body), jerr)
                tt |> success(!(doc?["created"] ?? true), "created=false on dedup")
            }
        }
        t |> run("get returns the exact source") @(tt : T?) {
            let hash = sha256_hex(source)
            GET("{base_url}/api/samples/{hash}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                tt |> equal(string(resp.body), source)
                tt |> success(find(get_header(resp, "Content-Type"), "text/plain") >= 0, "text/plain")
                tt |> success(find(get_header(resp, "Cache-Control"), "immutable") >= 0, "immutable cache header")
            }
        }
    }
}

[test]
def test_transport_rejections(t : T?) {
    with_playground_server(tiny_policy()) $(base_url) {
        t |> run("empty body is 400") @(tt : T?) {
            POST("{base_url}/api/samples", "") $(resp) {
                tt |> equal(int(resp.status_code), 400)
            }
        }
        t |> run("oversized body is 413") @(tt : T?) {
            var big = ""
            for (_i in range(65)) {
                big += "y"
            }
            POST("{base_url}/api/samples", big) $(resp) {
                tt |> equal(int(resp.status_code), 413)
            }
        }
        t |> run("malformed hash is 400") @(tt : T?) {
            GET("{base_url}/api/samples/nothex") $(resp) {
                tt |> equal(int(resp.status_code), 400)
            }
            GET("{base_url}/api/samples/DEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEFDEADBEEF") $(resp) {
                tt |> equal(int(resp.status_code), 400)
            }
        }
        t |> run("unknown well-formed hash is 404") @(tt : T?) {
            GET("{base_url}/api/samples/{sha256_hex("no such sample")}") $(resp) {
                tt |> equal(int(resp.status_code), 404)
            }
        }
    }
}

[test]
def test_rate_limiting_by_forwarded_ip(t : T?) {
    with_playground_server(tiny_policy()) $(base_url) {
        t |> run("fourth insert from one ip is 429; another ip passes") @(tt : T?) {
            var headers_a : table<string; string>
            headers_a |> insert("X-Forwarded-For", "203.0.113.7")
            var headers_b : table<string; string>
            headers_b |> insert("X-Forwarded-For", "203.0.113.8, 10.0.0.1")
            for (i in range(3)) {
                POST("{base_url}/api/samples", "sample {i}", headers_a) $(resp) {
                    tt |> equal(int(resp.status_code), 200)
                }
            }
            POST("{base_url}/api/samples", "sample 3", headers_a) $(resp) {
                tt |> equal(int(resp.status_code), 429)
            }
            POST("{base_url}/api/samples", "sample 4", headers_b) $(resp) {
                tt |> equal(int(resp.status_code), 200)
            }
        }
    }
}

[test]
def test_share_link(t : T?) {
    with_playground_server(default_policy()) $(base_url) {
        let source = "let shared = true"
        let hash = posted_hash(base_url, source)
        t |> run("known hash redirects to the playground") @(tt : T?) {
            tt |> equal(hash, sha256_hex(source))
            GET("{base_url}/s/{hash}") $(resp) {
                tt |> success(resp != null, "response arrived")
                // libhv's client may follow the 302: accept either the raw
                // redirect or the followed-to-playground 404 on this bare API
                // server (no static files here).
                let sc = int(resp.status_code)
                if (sc == 302) {
                    tt |> success(find(get_header(resp, "Location"), hash) >= 0, "Location carries the hash")
                } else {
                    tt |> equal(sc, 404)
                }
            }
        }
        t |> run("unknown hash is 404") @(tt : T?) {
            GET("{base_url}/s/{sha256_hex("never stored")}") $(resp) {
                tt |> equal(int(resp.status_code), 404)
            }
        }
    }
}
