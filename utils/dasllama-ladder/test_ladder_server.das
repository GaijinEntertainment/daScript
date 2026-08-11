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
    return LadderPolicy()
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
                tt |> equal(resp.body, doc)
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
    with_ladder_server(LadderPolicy(max_sidecar_bytes = 64, max_submissions_per_ip_hour = 1)) $(base_url) {
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
}
