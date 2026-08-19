options gen2
options persistent_heap

require dastest/testing_boost public
require ladder_server
require _ladder_test_common
require daslib/jobque_boost
require daslib/json_boost
require daslib/sha_256
require daslib/fio   // sleep — the ready-spin
require strings

let TEST_PORT = 19015   // 19011..19013 playground, 19014 buildd

// HTTP coverage: transport shape, status mapping, headers — every policy
// behavior itself is proven in test_ladder_store; here each route is exercised
// through a live loopback server.

// Local harness (playground pattern): ladder_server keeps lifecycle state in
// module globals, which are per-context — so ALL setup happens inside the
// server thread, and shutdown goes through POST /shutdown.
def private with_ladder_server(policy : LadderPolicy; blk : block<(base_url : string) : void>) {
    with_job_status(1) $(finished) {
        new_thread() @() {
            run_server(TEST_PORT, ":memory:", policy, "")
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
        assert(ready, "ladder server became ready")
    }
}

def private default_policy : LadderPolicy {
    return LadderPolicy(submit_open = true)   // the submit-path tests need an open board; the gate itself has its own test
}

def private json_of(var resp : HttpResponse?) : JsonValue? {
    var jerr = ""
    return read_json(string(resp.body), jerr)
}

[test]
def test_health_and_unknown(t : T?) {
    with_ladder_server(default_policy()) $(base_url) {
        t |> run("healthz") @(tt : T?) {
            GET("{base_url}/healthz") $(resp) {
                tt |> success(resp != null, "response arrived")
                tt |> equal(int(resp.status_code), 200)
                tt |> equal(resp.body, "ok")
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
def test_sidecar_routes(t : T?) {
    with_ladder_server(default_policy()) $(base_url) {
        let doc = sidecar_doc(BOX_A, "3", "vec8_u8")
        // run-arm lambdas capture outer locals by value at creation, so nothing
        // written inside one arm reaches the next - every arm derives its own state
        let sha = sha256_hex(doc)
        t |> run("submit mints the content sha") @(tt : T?) {
            POST("{base_url}/api/submit/sidecar", doc) $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var js = json_of(resp)
                tt |> equal(js?["sha"] ?? "", sha)
                tt |> success(js?["created"] ?? false, "first submit creates")
                tt |> equal(js?["url"] ?? "", "/api/sidecar/{sha}")
            }
        }
        t |> run("download returns the exact document") @(tt : T?) {
            GET("{base_url}/api/sidecar/{sha}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                tt |> equal(resp.body, doc)
                tt |> success(find(get_header(resp, "Content-Disposition"), "{sha}.tune.json") >= 0, "download filename")
            }
        }
        t |> run("lookup ladders by version and box") @(tt : T?) {
            GET("{base_url}/api/sidecars?version=3&box={url_encode(BOX_A)}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var js = json_of(resp)
                let hits = js?["hits"]
                tt |> success(hits != null && hits is _array, "hits array")
                tt |> equal(length(hits as _array), 1)
                tt |> equal(hits?[0]?["tier"] ?? "", "exact")
                tt |> equal(hits?[0]?["sha"] ?? "", sha)
            }
            GET("{base_url}/api/sidecars?version=9&box={url_encode(BOX_A)}") $(resp) {
                var js = json_of(resp)
                tt |> equal(length(js?["hits"] as _array), 0, "no hits on an unseen version")
            }
        }
        t |> run("no box = the browse listing") @(tt : T?) {
            GET("{base_url}/api/sidecars") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var js = json_of(resp)
                let rows = js?["sidecars"]
                tt |> success(rows != null && rows is _array, "sidecars array")
                tt |> equal(length(rows as _array), 1)
                tt |> equal(rows?[0]?["sha"] ?? "", sha)
                tt |> equal(rows?[0]?["tier"] ?? "?", "")
                tt |> equal(rows?[0]?["dasllama_version"] ?? 0, 3)
                tt |> equal(rows?[0]?["engine_sha"] ?? "", "8a538d795")
            }
            GET("{base_url}/api/sidecars?version=9") $(resp) {
                tt |> equal(length(json_of(resp)?["sidecars"] as _array), 0, "version scopes the listing")
            }
            GET("{base_url}/api/sidecars?version=junk") $(resp) {
                tt |> equal(int(resp.status_code), 400, "malformed version refused")
            }
        }
        t |> run("transport-shape rejections") @(tt : T?) {
            GET("{base_url}/api/sidecar/nothex") $(resp) {
                tt |> equal(int(resp.status_code), 400)
            }
            GET("{base_url}/api/sidecar/{sha256_hex("absent")}") $(resp) {
                tt |> equal(int(resp.status_code), 404)
            }
            GET("{base_url}/api/sidecars?version=3&box={url_encode("three|fields|only")}") $(resp) {
                tt |> equal(int(resp.status_code), 400)
            }
            GET("{base_url}/api/sidecars?box={url_encode(BOX_A)}") $(resp) {
                tt |> equal(int(resp.status_code), 400, "version is required")
            }
            POST("{base_url}/api/submit/sidecar", "\{}") $(resp) {
                tt |> equal(int(resp.status_code), 400)
                tt |> success(!empty(json_of(resp)?["error"] ?? ""), "error named")
            }
            POST("{base_url}/api/submit/sidecar", "") $(resp) {
                tt |> equal(int(resp.status_code), 400, "an empty body is a named refusal, not a bare 200")
            }
        }
    }
}

[test]
def test_records_routes(t : T?) {
    with_ladder_server(default_policy()) $(base_url) {
        let doc = records_doc()
        t |> run("submit lands community rows") @(tt : T?) {
            POST("{base_url}/api/submit/records", doc) $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var js = json_of(resp)
                tt |> success((js?["submission_id"] ?? 0) >= 1, "submission id minted")
                tt |> equal(js?["runs_added"] ?? 0, 2)
            }
        }
        t |> run("versions and runs surface the board") @(tt : T?) {
            GET("{base_url}/api/versions") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                let body = string(resp.body)
                tt |> success(find(body, "3") >= 0, "das version listed")
            }
            GET("{base_url}/api/runs?version=3") $(resp) {
                var js = json_of(resp)
                tt |> equal(length(js?["runs"] as _array), 1)
                tt |> equal(js?["runs"]?[0]?["engine"] ?? "", "das")
                tt |> equal(js?["runs"]?[0]?["source"] ?? "", "community")
                tt |> success(!(js?["runs"]?[0]?["verified"] ?? true), "unverified")
            }
            GET("{base_url}/api/runs") $(resp) {
                var js = json_of(resp)
                tt |> equal(length(js?["runs"] as _array), 2, "both rows on the full board")
            }
            GET("{base_url}/api/runs?version=abc") $(resp) {
                tt |> equal(int(resp.status_code), 400)
            }
        }
        t |> run("the receipt is the verbatim submission") @(tt : T?) {
            // a byte-identical resubmit dedups onto the original id - the arm's
            // own way to learn it (cross-arm state does not survive the lambdas)
            var sid = 0
            POST("{base_url}/api/submit/records", doc) $(resp) {
                tt |> success(!(json_of(resp)?["created"] ?? true), "dedup hit")
                sid = json_of(resp)?["submission_id"] ?? 0
            }
            GET("{base_url}/api/submission/{sid}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                // records are path-redacted server-side, so the stored receipt is the redacted form
                tt |> equal(resp.body, redact_record_paths(doc))
            }
            GET("{base_url}/api/submission/0") $(resp) {
                tt |> equal(int(resp.status_code), 400)
            }
            GET("{base_url}/api/submission/99999") $(resp) {
                tt |> equal(int(resp.status_code), 404)
            }
        }
        t |> run("official import over loopback admin") @(tt : T?) {
            POST("{base_url}/admin/import-official", official_doc()) $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var js = json_of(resp)
                tt |> equal(js?["runs_added"] ?? 0, 2)
            }
            GET("{base_url}/api/runs") $(resp) {
                var js = json_of(resp)
                tt |> equal(length(js?["runs"] as _array), 4, "official rows joined the board")
            }
        }
    }
}

[test]
def test_policy_status_mapping(t : T?) {
    with_ladder_server(LadderPolicy(max_sidecar_bytes = 64, max_submissions_per_ip_hour = 1, submit_open = true)) $(base_url) {
        t |> run("oversize is 413, throttle is 429") @(tt : T?) {
            POST("{base_url}/api/submit/sidecar", sidecar_doc(BOX_A, "3", "vec8")) $(resp) {
                tt |> equal(int(resp.status_code), 413, "the tiny cap rejects the real fixture")
            }
            POST("{base_url}/api/submit/records", records_doc()) $(resp) {
                tt |> equal(int(resp.status_code), 200, "first accepted submission")
            }
            POST("{base_url}/api/submit/records", "[]") $(resp) {
                tt |> equal(int(resp.status_code), 429, "ceiling of one reached")
            }
        }
    }
    // the attempt limiter caps REJECTED traffic too — the accepted ceiling never fires on it
    with_ladder_server(LadderPolicy(max_attempts_per_ip_hour = 2, submit_open = true)) $(base_url) {
        t |> run("invalid submissions are attempt-limited before parse") @(tt : T?) {
            // two malformed bodies (400, never stored, so the accepted ceiling is untouched)
            POST("{base_url}/api/submit/sidecar", "not json") $(resp) {
                tt |> equal(int(resp.status_code), 400, "first rejected")
            }
            POST("{base_url}/api/submit/sidecar", "not json") $(resp) {
                tt |> equal(int(resp.status_code), 400, "second rejected")
            }
            // the third attempt trips the transport-layer limiter regardless of validity
            POST("{base_url}/api/submit/sidecar", sidecar_doc(BOX_A, "3", "vec8")) $(resp) {
                tt |> equal(int(resp.status_code), 429, "attempt ceiling reached on rejected traffic")
            }
        }
    }
}

[test]
def test_admin_sidecar_curation(t : T?) {
    // gate CLOSED throughout: the operator surface is independent of public submissions
    with_ladder_server(LadderPolicy()) $(base_url) {
        let doc = sidecar_doc(BOX_A, "4", "vec8_u8")
        let sha = sha256_hex(doc)
        t |> run("plant stores verified while the board is closed") @(tt : T?) {
            POST("{base_url}/admin/import-sidecar", doc) $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var js = json_of(resp)
                tt |> equal(js?["sha"] ?? "", sha)
                tt |> success(js?["created"] ?? false, "first plant creates")
            }
            GET("{base_url}/api/sidecars") $(resp) {
                var js = json_of(resp)
                tt |> equal(length(js?["sidecars"] as _array), 1)
                tt |> success(js?["sidecars"]?[0]?["verified"] ?? false, "verified on the public listing")
            }
        }
        t |> run("the admin listing carries gate state and moderation columns") @(tt : T?) {
            GET("{base_url}/admin/sidecars") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var js = json_of(resp)
                tt |> success(!(js?["submit_open"] ?? true), "gate reads closed")
                tt |> equal(length(js?["sidecars"] as _array), 1)
                tt |> equal(js?["sidecars"]?[0]?["client_ip"] ?? "", "importer")
            }
        }
        t |> run("verify demotes and promotes by sha") @(tt : T?) {
            POST("{base_url}/admin/sidecar/verify", "\{\"sha\":\"{sha}\",\"verified\":false}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
            }
            GET("{base_url}/api/sidecars") $(resp) {
                tt |> success(!(json_of(resp)?["sidecars"]?[0]?["verified"] ?? true), "demoted")
            }
            POST("{base_url}/admin/sidecar/verify", "\{\"sha\":\"{sha}\",\"verified\":true}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
            }
            GET("{base_url}/api/sidecars") $(resp) {
                tt |> success(json_of(resp)?["sidecars"]?[0]?["verified"] ?? false, "promoted back")
            }
        }
        t |> run("an absent verified key demotes (fail-safe, like the gate's open key)") @(tt : T?) {
            POST("{base_url}/admin/sidecar/verify", "\{\"sha\":\"{sha}\"}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
            }
            GET("{base_url}/api/sidecars") $(resp) {
                tt |> success(!(json_of(resp)?["sidecars"]?[0]?["verified"] ?? true), "no key never promotes")
            }
            POST("{base_url}/admin/sidecar/verify", "\{\"sha\":\"{sha}\",\"verified\":true}") $(resp) {
                tt |> equal(int(resp.status_code), 200, "restored for the arms below")
            }
        }
        t |> run("delete removes the sidecar and its receipt") @(tt : T?) {
            POST("{base_url}/admin/sidecar/delete", "\{\"sha\":\"{sha}\"}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
            }
            GET("{base_url}/api/sidecar/{sha}") $(resp) {
                tt |> equal(int(resp.status_code), 404, "document gone")
            }
            POST("{base_url}/admin/sidecar/delete", "\{\"sha\":\"{sha}\"}") $(resp) {
                tt |> equal(int(resp.status_code), 404, "second delete finds nothing")
            }
        }
    }
}

[test]
def test_admin_transport_rejections(t : T?) {
    with_ladder_server(LadderPolicy()) $(base_url) {
        t |> run("malformed, unknown, and empty inputs are named refusals") @(tt : T?) {
            POST("{base_url}/admin/sidecar/verify", "\{\"sha\":\"nothex\",\"verified\":true}") $(resp) {
                tt |> equal(int(resp.status_code), 400, "malformed sha")
            }
            POST("{base_url}/admin/sidecar/verify", "\{\"sha\":\"{sha256_hex("absent")}\",\"verified\":true}") $(resp) {
                tt |> equal(int(resp.status_code), 404, "unknown sha")
            }
            POST("{base_url}/admin/sidecar/verify", "") $(resp) {
                tt |> equal(int(resp.status_code), 400, "empty body refused, not silently accepted")
            }
            POST("{base_url}/admin/sidecar/delete", "\{\"sha\":\"nothex\"}") $(resp) {
                tt |> equal(int(resp.status_code), 400, "delete rejects a malformed sha")
            }
            POST("{base_url}/admin/import-sidecar", "not json") $(resp) {
                tt |> equal(int(resp.status_code), 400)
                tt |> success(!empty(json_of(resp)?["error"] ?? ""), "plant names its error over HTTP")
            }
        }
    }
}

[test]
def test_admin_origin_gate_and_page(t : T?) {
    with_ladder_server(LadderPolicy()) $(base_url) {
        // 404 from the store is the pass signal - a request the gate blocks answers 403
        let absent = sha256_hex("absent")
        t |> run("every admin route refuses a cross-site Origin") @(tt : T?) {
            // every operator POST, including the two my rename moved off the peer-only gate
            // (import-official, shutdown) - a cross-origin shutdown must be REFUSED, so the
            // server survives the arm
            for (path in ["/admin/sidecar/verify", "/admin/sidecar/delete", "/admin/import-sidecar",
                          "/admin/submit", "/admin/import-official", "/shutdown"]) {
                with_http_request() $(var req) {
                    req.method = http_method.POST
                    req.url := "{base_url}{path}"
                    set_header(req, "Origin", "http://evil.example")
                    req.body := "\{\"sha\":\"{absent}\",\"verified\":true}"
                    request(req) $(resp) {
                        tt |> equal(int(resp.status_code), 403, "cross-site POST refused")
                    }
                }
            }
            for (path in ["/admin/", "/admin/sidecars"]) {
                with_http_request() $(var req) {
                    req.method = http_method.GET
                    req.url := "{base_url}{path}"
                    set_header(req, "Origin", "http://evil.example")
                    request(req) $(resp) {
                        tt |> equal(int(resp.status_code), 403, "cross-site GET refused")
                    }
                }
            }
            // Origin: null (sandboxed iframe) has no scheme separator - must still fail closed
            with_http_request() $(var req) {
                req.method = http_method.POST
                req.url := "{base_url}/admin/sidecar/verify"
                set_header(req, "Origin", "null")
                req.body := "\{\"sha\":\"{absent}\",\"verified\":true}"
                request(req) $(resp) {
                    tt |> equal(int(resp.status_code), 403, "Origin null refused")
                }
            }
            // DNS rebinding: peer is loopback (the tunnel) and Origin == Host, both attacker-owned.
            // Sent header-less so same-origin passes and ONLY the loopback-Host check can refuse it
            // - if this 404'd the host guard would be doing nothing (and libhv would be ignoring the
            // spoofed Host); a 403 proves the guard fires.
            with_http_request() $(var req) {
                req.method = http_method.POST
                req.url := "{base_url}/admin/sidecar/verify"
                set_header(req, "Host", "evil.example:{TEST_PORT}")
                req.body := "\{\"sha\":\"{absent}\",\"verified\":true}"
                request(req) $(resp) {
                    tt |> equal(int(resp.status_code), 403, "rebound non-loopback Host refused")
                }
            }
        }
        t |> run("a same-origin request passes the gate") @(tt : T?) {
            with_http_request() $(var req) {
                req.method = http_method.POST
                req.url := "{base_url}/admin/sidecar/verify"
                set_header(req, "Origin", "http://127.0.0.1:{TEST_PORT}")
                req.body := "\{\"sha\":\"{absent}\",\"verified\":true}"
                request(req) $(resp) {
                    tt |> equal(int(resp.status_code), 404, "reached the store")
                    tt |> success(find(string(resp.body), "unknown sidecar") >= 0, "the store's own 404, not the catch-all's")
                }
            }
        }
        t |> run("the IPv4-mapped IPv6 loopback Host is accepted (peer/host allowlists agree)") @(tt : T?) {
            // header-less, so only is_loopback_host decides: a 404 means it passed the gate
            with_http_request() $(var req) {
                req.method = http_method.POST
                req.url := "{base_url}/admin/sidecar/verify"
                set_header(req, "Host", "[::ffff:127.0.0.1]:{TEST_PORT}")
                req.body := "\{\"sha\":\"{absent}\",\"verified\":true}"
                request(req) $(resp) {
                    tt |> equal(int(resp.status_code), 404, "mapped loopback Host reaches the store")
                }
            }
        }
    }
}

[test]
def test_admin_page_serves(t : T?) {
    with_ladder_server(LadderPolicy()) $(base_url) {
        t |> run("the operator page serves on the loopback surface") @(tt : T?) {
            GET("{base_url}/admin/") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                tt |> success(find(string(resp.body), "dasllama-ladder admin") >= 0, "the page arrived")
            }
            GET("{base_url}/admin") $(resp) {
                tt |> equal(int(resp.status_code), 200, "the no-slash spelling serves it too")
            }
        }
    }
}

[test]
def test_admin_plant_promotes_community(t : T?) {
    // gate OPEN: a community upload is the same content a later plant promotes
    with_ladder_server(default_policy()) $(base_url) {
        let doc = sidecar_doc(BOX_B, "4", "vec8_u8")
        t |> run("planting a community upload promotes it in place") @(tt : T?) {
            POST("{base_url}/api/submit/sidecar", doc) $(resp) {
                tt |> equal(int(resp.status_code), 200)
            }
            GET("{base_url}/api/sidecars") $(resp) {
                tt |> success(!(json_of(resp)?["sidecars"]?[0]?["verified"] ?? true), "community entry is unverified")
            }
            POST("{base_url}/admin/import-sidecar", doc) $(resp) {
                tt |> equal(int(resp.status_code), 200)
                tt |> success(!(json_of(resp)?["created"] ?? true), "dedup hit, not a second row")
            }
            GET("{base_url}/api/sidecars") $(resp) {
                var js = json_of(resp)
                tt |> equal(length(js?["sidecars"] as _array), 1, "still one row")
                tt |> success(js?["sidecars"]?[0]?["verified"] ?? false, "now verified")
            }
            GET("{base_url}/admin/sidecars") $(resp) {
                tt |> success(json_of(resp)?["submit_open"] ?? false, "the admin listing reads the gate open too")
            }
        }
    }
}

[test]
def test_submit_gate(t : T?) {
    // default policy = submit CLOSED: the read board serves, writes are refused until opened, and
    // the loopback admin flip is reversible (the test-window switch)
    with_ladder_server(LadderPolicy()) $(base_url) {
        let doc = sidecar_doc(BOX_A, "3", "vec8_u8")
        t |> run("a closed board refuses submissions with 403") @(tt : T?) {
            POST("{base_url}/api/submit/sidecar", doc) $(resp) {
                tt |> equal(int(resp.status_code), 403, "sidecar refused")
            }
            POST("{base_url}/api/submit/records", records_doc()) $(resp) {
                tt |> equal(int(resp.status_code), 403, "records refused")
            }
        }
        t |> run("the loopback admin toggle opens submissions") @(tt : T?) {
            POST("{base_url}/admin/submit", "\{\"open\":true}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var js = json_of(resp)
                tt |> success(js?["submit_open"] ?? false, "reports open")
            }
            POST("{base_url}/api/submit/sidecar", doc) $(resp) {
                tt |> equal(int(resp.status_code), 200, "sidecar now accepted")
            }
        }
        t |> run("the toggle closes them again") @(tt : T?) {
            POST("{base_url}/admin/submit", "\{\"open\":false}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var js = json_of(resp)
                tt |> success(!(js?["submit_open"] ?? true), "reports closed")
            }
            POST("{base_url}/api/submit/records", records_doc()) $(resp) {
                tt |> equal(int(resp.status_code), 403, "records refused again")
            }
        }
        t |> run("a malformed admin body cannot leave the gate open (fail-safe)") @(tt : T?) {
            POST("{base_url}/admin/submit", "\{\"open\":true}") $(resp) {
                tt |> equal(int(resp.status_code), 200)
            }
            // garbage must resolve to closed: read_json -> null -> ?? false. (A non-loopback
            // caller is also refused, but every in-process peer is 127.0.0.1 so that path
            // can't be exercised here — it is covered by is_operator_caller's exact-match.)
            POST("{base_url}/admin/submit", "not json at all") $(resp) {
                tt |> equal(int(resp.status_code), 200)
                var js = json_of(resp)
                tt |> success(!(js?["submit_open"] ?? true), "garbage body closed the gate")
            }
            POST("{base_url}/api/submit/sidecar", doc) $(resp) {
                tt |> equal(int(resp.status_code), 403, "submit refused after the garbage close")
            }
        }
    }
}
