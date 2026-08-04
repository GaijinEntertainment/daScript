options gen2
options persistent_heap

require dastest/testing_boost public
require playground_server
require daslib/jobque_boost
require daslib/json_boost
require daslib/fio
require strings

let TEST_CURATED_PORT = 19012

let TEST_PORT = 19011   // 19001..19010 are taken by tests/dasHV

// Local harness: playground_server keeps lifecycle state in module globals,
// which are per-context — so ALL setup (store open included) happens inside
// the server thread, and shutdown goes through POST /shutdown (which flips
// the serving context's own flag).
def private with_playground_server(policy : StorePolicy; blk : block<(base_url : string) : void>) {
    with_playground_server_at(TEST_PORT, policy, "", blk)
}

def private with_playground_server_at(port : int; policy : StorePolicy; curated_dir : string; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        let port_copy = port
        let curated_copy = curated_dir
        new_thread() @() {
            run_server(port_copy, ":memory:", policy, "https://test.example", curated_copy)
            finished |> notify_and_release
        }
        let base_url = "http://127.0.0.1:{port}"
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
            headers_b |> insert("X-Forwarded-For", "203.0.113.8, 10.0.0.1")   // identity is 10.0.0.1, the proxy-appended hop
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
def test_forged_forwarded_hops_share_one_bucket(t : T?) {
    //! Regression: the rate ceiling keyed on the FIRST X-Forwarded-For hop,
    //! which a client authors. A proxy appends the peer it accepted, so only
    //! the last hop is trustworthy — keying on the first let one caller mint a
    //! fresh identity per request and never be throttled.
    with_playground_server(tiny_policy()) $(base_url) {
        t |> run("a caller cannot escape the ceiling by varying earlier hops") @(tt : T?) {
            for (i in range(3)) {
                var h : table<string; string>
                h |> insert("X-Forwarded-For", "198.51.100.{i}, 10.0.0.42")
                POST("{base_url}/api/samples", "forged {i}", h) $(resp) {
                    tt |> equal(int(resp.status_code), 200)
                }
            }
            var h4 : table<string; string>
            h4 |> insert("X-Forwarded-For", "198.51.100.99, 10.0.0.42")
            POST("{base_url}/api/samples", "forged 3", h4) $(resp) {
                tt |> equal(int(resp.status_code), 429)
            }
        }
    }
}

[test]
def test_body_byte_faithfulness_guard(t : T?) {
    //! Regression: the body was read through a NUL-terminated string, so
    //! everything past an embedded NUL was dropped before the size check, the
    //! validation, the hash and the stored row — two different bodies sharing a
    //! pre-NUL prefix would collapse onto one hash and one public URL.
    //! The guard is tested as a predicate rather than end-to-end: the dasHV
    //! client takes the body as a NUL-terminated string too, so no dastest
    //! client can put a NUL on the wire.
    t |> run("a body whose bytes survive the string view is accepted") @(tt : T?) {
        let text = "print(\"visible\")"
        tt |> success(body_is_byte_faithful(length(text), text), "faithful body")
        tt |> success(body_is_byte_faithful(0, ""), "empty body")
    }
    t |> run("a body longer than its string view is refused") @(tt : T?) {
        let text = "print(\"visible\")"
        // what a NUL looks like from the handler: more bytes arrived than the
        // string carries
        tt |> success(!body_is_byte_faithful(length(text) + 18, text), "truncated at a NUL")
        tt |> success(!body_is_byte_faithful(1, ""), "NUL-only body")
    }
}

[test]
def test_stored_sources_are_served_nosniff(t : T?) {
    //! Regression: stored anonymous input was served same-origin with a
    //! one-year immutable cache and no nosniff, so a browser could sniff a
    //! sample as script.
    with_playground_server(default_policy()) $(base_url) {
        let source = "print(\"sniff me\")"
        let hash = posted_hash(base_url, source)
        t |> run("GET carries X-Content-Type-Options") @(tt : T?) {
            GET("{base_url}/api/samples/{hash}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                tt |> equal(get_header(resp, "X-Content-Type-Options"), "nosniff")
            }
        }
    }
}

[test]
def test_loopback_peer_predicate(t : T?) {
    //! The operator-surface gate. Only the predicate is unit-testable here: the
    //! test client necessarily connects FROM loopback, so the allow path is
    //! what the harness exercises (every suite shuts the server down through
    //! POST /shutdown) and the deny path is proven on the peer string.
    t |> run("loopback forms are accepted") @(tt : T?) {
        tt |> success(is_loopback_peer("127.0.0.1"), "ipv4 loopback")
        tt |> success(is_loopback_peer("::1"), "ipv6 loopback")
        tt |> success(is_loopback_peer("::ffff:127.0.0.1"), "v4-mapped loopback")
    }
    t |> run("everything else is remote") @(tt : T?) {
        tt |> success(!is_loopback_peer(""), "unknown peer")
        tt |> success(!is_loopback_peer("203.0.113.5"), "public address")
        tt |> success(!is_loopback_peer("10.0.0.1"), "private LAN address")
        tt |> success(!is_loopback_peer("127.0.0.1.evil.example"), "address as a hostname prefix")
        tt |> success(!is_loopback_peer("127.0.0.2"), "other loopback-range address")
    }
}

[test]
def test_listed_and_reimport(t : T?) {
    t |> run("listed is empty without curated import; reimport 503s unconfigured") @(tt : T?) {
        with_playground_server(default_policy()) $(base_url) {
            GET("{base_url}/api/samples/listed") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                tt |> equal(string(resp.body), "[]")
            }
            POST("{base_url}/admin/reimport", "") $(resp) {
                tt |> equal(int(resp.status_code), 503)
            }
        }
    }
    t |> run("boot import feeds listed; reimport recounts") @(tt : T?) {
        var terr = ""
        let dir = path_join(temp_directory(terr), "pg_server_curated")
        mkdir_rec(path_join(dir, "examples"))
        fwrite(path_join(dir, "data.json"),
            "\{ \"examples\": [ \{ \"name\": \"Hello world\", \"files\": [\"examples/hello.das\"] \} ] \}")
        fwrite(path_join(dir, "examples/hello.das"), "print(\"curated hello\")")
        with_playground_server_at(TEST_CURATED_PORT, default_policy(), dir) $(base_url) {
            var hash = ""
            GET("{base_url}/api/samples/listed") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var jerr = ""
                var doc = read_json(string(resp.body), jerr)
                tt |> success(doc != null && (doc.value is _array), "listed is a json array")
                tt |> equal(length(doc.value as _array), 1)
                let entry = (doc.value as _array)[0]
                tt |> equal(entry?["title"] ?? "", "Hello world")
                tt |> equal(entry?["category"] ?? "", "examples")
                tt |> equal(entry?["path"] ?? "", "examples/hello.das")
                hash = entry?["hash"] ?? ""
            }
            GET("{base_url}/api/samples/{hash}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                tt |> equal(string(resp.body), "print(\"curated hello\")")
            }
            POST("{base_url}/admin/reimport", "") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var jerr = ""
                var doc = read_json(string(resp.body), jerr)
                tt |> equal(int(doc?["listed"] ?? 0l), 1)
            }
        }
        var rerr = ""
        remove(path_join(dir, "data.json"), rerr)
        remove(path_join(dir, "examples/hello.das"), rerr)
        rmdir(path_join(dir, "examples"), rerr)
        rmdir(dir, rerr)
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
