options gen2

require dastest/testing_boost public
require ladder_store
require _ladder_test_common
require daslib/sql_linq
require daslib/json_boost
require daslib/strings_boost   // replace — the re-measure fixture edit

// Store-level coverage: policy gates, source stamping, the sidecar lookup ladder,
// official replace-by-identity, and the admin levers — no HTTP anywhere.

def private test_policy : LadderPolicy {
    return LadderPolicy(max_records_bytes = 65536, max_sidecar_bytes = 4096, max_submissions_per_ip_hour = 2)
}

def private ident_a : BoxIdentity {
    var b : BoxIdentity
    parse_box_string(BOX_A, b)
    return b
}

[test]
def test_migration(t : T?) {
    t |> run("fresh db migrates to latest") @(t : T?) {
        with_latest_sqlite(":memory:") $(db) {
            t |> equal(current_schema_version(db), 1)
        }
    }
}

[test]
def test_put_sidecar(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("a versioned sidecar stores and dedups") @(tt : T?) {
            let doc = sidecar_doc(BOX_A, "3", "vec8_u8")
            let put = put_sidecar(db, doc, "10.0.0.1", test_policy(), 1000l)
            tt |> success(put.status == PutStatus.ok, "stored")
            tt |> success(put.created, "created")
            tt |> equal(length(put.sha), 64)
            tt |> equal(get_sidecar_doc(db, put.sha), doc)
            let again = put_sidecar(db, doc, "10.0.0.2", test_policy(), 1001l)
            tt |> success(again.status == PutStatus.ok && !again.created, "content dedup")
        }
        t |> run("path-shaped provenance is stripped server-side") @(tt : T?) {
            var jerr = ""
            var jdoc = read_json(sidecar_doc(BOX_B, "3", "vec8_u8"), jerr)
            var prov = jdoc?["provenance"]
            update(prov, "binary", JV("/Users/someone/bin/daslang"))
            update(prov, "weird", JV("C:\\Users\\bob\\thing"))
            let dirty = write_json(jdoc)
            delete_json(jdoc)
            let put = put_sidecar(db, dirty, "10.0.0.6", test_policy(), 3000l)
            tt |> success(put.status == PutStatus.ok, "stored")
            let stored = get_sidecar_doc(db, put.sha)
            tt |> success(find(stored, "binary") < 0, "binary gone")
            tt |> success(find(stored, "bob") < 0, "path-shaped value gone")
            tt |> success(find(stored, "\"noise\"") >= 0, "plain provenance survives")
            tt |> equal(sha256_hex(stored), put.sha, "the sha is of the stripped text")
        }
        t |> run("an unversioned sidecar is refused") @(tt : T?) {
            var bad = %json~
            {
                "kernels" : { "dot" : "vec8" },
                "provenance" : { "box" : "a|b|c|d|e", "engine_sha" : "x" }
            }
            %%
            let put = put_sidecar(db, write_json(bad), "10.0.0.3", test_policy(), 1002l)
            tt |> success(put.status == PutStatus.invalid, "refused")
            tt |> success(!empty(put.error), "error named")
        }
        t |> run("size and rate gates") @(tt : T?) {
            let big = build_string() $(w) {
                w |> write("[")
                for (_i in range(600)) {
                    w |> write("\"aaaaaaaa\",")
                }
                w |> write("\"a\"]")
            }
            tt |> success(put_sidecar(db, big, "10.0.0.4", test_policy(), 1003l).status == PutStatus.too_large, "size gate")
            // the ip already accepted two stored submissions this hour -> throttled,
            // and per the cheapest-first discipline even a dedup body is throttled
            let p1 = put_sidecar(db, sidecar_doc(BOX_A, "3", "u2"), "10.0.0.5", test_policy(), 2000l)
            let p2 = put_sidecar(db, sidecar_doc(BOX_A, "3", "u4"), "10.0.0.5", test_policy(), 2001l)
            tt |> success(p1.created && p2.created, "two under the ceiling")
            let p3 = put_sidecar(db, sidecar_doc(BOX_A, "3", "u8"), "10.0.0.5", test_policy(), 2002l)
            tt |> success(p3.status == PutStatus.rate_limited, "third throttled")
        }
    }
}

[test]
def test_sidecar_ladder(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        var pol = test_policy()
        pol.max_submissions_per_ip_hour = 100
        let exact1 = put_sidecar(db, sidecar_doc(BOX_A, "3", "vec8_u8"), "10.1.0.1", pol, 1000l)
        let exact2 = put_sidecar(db, sidecar_doc(BOX_A, "3", "vec8_u4"), "10.1.0.1", pol, 2000l)
        let cpu_hit = put_sidecar(db, sidecar_doc(BOX_B, "3", "vec8_u2"), "10.1.0.2", pol, 3000l)
        let arch_hit = put_sidecar(db, sidecar_doc(BOX_C, "3", "vec4_u2"), "10.1.0.3", pol, 4000l)
        let other_ver = put_sidecar(db, sidecar_doc(BOX_A, "2", "vec16"), "10.1.0.4", pol, 5000l)
        t |> run("tiers ladder exact -> cpu -> arch, version-scoped") @(tt : T?) {
            let hits <- find_sidecars(db, 3, ident_a())
            tt |> equal(length(hits), 4)
            tt |> equal(hits[0].tier, "exact")
            tt |> equal(hits[1].tier, "exact")
            tt |> equal(hits[2].tier, "cpu")
            tt |> equal(hits[2].sha, cpu_hit.sha)
            tt |> equal(hits[3].tier, "arch")
            tt |> equal(hits[3].sha, arch_hit.sha)
            // newest-first within the unverified exact tier
            tt |> equal(hits[0].sha, exact2.sha)
            let v2 <- find_sidecars(db, 2, ident_a())
            tt |> equal(length(v2), 1)
            tt |> equal(v2[0].sha, other_ver.sha)
            let v9 <- find_sidecars(db, 9, ident_a())
            tt |> equal(length(v9), 0)
        }
        t |> run("a verified sidecar surfaces first within its tier") @(tt : T?) {
            tt |> success(set_sidecar_verified(db, exact1.sha, true), "flag set")
            let hits <- find_sidecars(db, 3, ident_a())
            tt |> equal(hits[0].sha, exact1.sha)
            tt |> success(hits[0].verified, "verified first")
            tt |> equal(hits[1].sha, exact2.sha)
        }
        t |> run("delete removes the sidecar from the ladder") @(tt : T?) {
            tt |> success(delete_sidecar(db, arch_hit.sha), "deleted")
            let hits <- find_sidecars(db, 3, ident_a())
            tt |> equal(length(hits), 3)
            tt |> success(!delete_sidecar(db, arch_hit.sha), "second delete finds nothing")
        }
    }
}

[test]
def test_list_sidecars(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        var pol = test_policy()
        pol.max_submissions_per_ip_hour = 100
        let v3a = put_sidecar(db, sidecar_doc(BOX_A, "3", "vec8_u8"), "10.5.0.1", pol, 1000l)
        let v3b = put_sidecar(db, sidecar_doc(BOX_C, "3", "vec4_u2"), "10.5.0.2", pol, 2000l)
        let v2 = put_sidecar(db, sidecar_doc(BOX_B, "2", "vec16"), "10.5.0.3", pol, 3000l)
        t |> run("version 0 lists every sidecar newest first, tier empty") @(tt : T?) {
            let all <- list_sidecars(db, 0)
            tt |> equal(length(all), 3)
            tt |> equal(all[0].sha, v2.sha)
            tt |> equal(all[1].sha, v3b.sha)
            tt |> equal(all[2].sha, v3a.sha)
            tt |> equal(all[0].tier, "")
            tt |> equal(all[0].dasllama_version, 2)
            tt |> equal(all[0].engine_sha, "8a538d795")
        }
        t |> run("a version scopes the listing") @(tt : T?) {
            let v3 <- list_sidecars(db, 3)
            tt |> equal(length(v3), 2)
            tt |> equal(length(list_sidecars(db, 9)), 0)
        }
    }
}

[test]
def test_put_records(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("a stamped submission lands as unverified community rows") @(tt : T?) {
            let doc = records_doc()
            let put = put_records(db, doc, "10.2.0.1", test_policy(), 1000l)
            tt |> success(put.status == PutStatus.ok, "accepted ({put.error})")
            tt |> equal(put.runs_added, 2)
            var rows <- list_runs(db, 0)
            tt |> equal(length(rows), 2)
            for (r in rows) {
                tt |> equal(r.Source, "community")
                tt |> equal(r.Verified, 0)
                tt |> equal(r.Gguf, "test-model-Q8_0.gguf")
            }
            // the das row carries its stamps into the filter columns
            let das_rows <- list_runs(db, 3)
            tt |> equal(length(das_rows), 1)
            tt |> equal(das_rows[0].Engine, "das")
            tt |> equal(das_rows[0].TuneSha, "cafe0123beef")
            tt |> equal(das_rows[0].Threads, 8)
            tt |> equal(get_submission_doc(db, put.submission_id), doc)
        }
        t |> run("a byte-identical resubmit dedups") @(tt : T?) {
            let put = put_records(db, records_doc(), "10.2.0.9", test_policy(), 1100l)
            tt |> success(put.status == PutStatus.ok && !put.created, "dedup hit")
            tt |> equal(length(list_runs(db, 0)), 2)
        }
        t |> run("an unstamped submission is refused") @(tt : T?) {
            var bad = %json~
            [ { "gguf" : "m.gguf", "runs" : [ {
                "engine" : "das", "backend" : "cpu", "flavor" : "tuned",
                "box" : "b", "threads" : 8, "date" : "2026-08-10", "cmd" : "c",
                "hardware" : { "cpu" : "x" },
                "tests" : { "pp512" : { "tok_s" : 1.0 } }
            } ] } ]
            %%
            let put = put_records(db, write_json(bad), "10.2.0.2", test_policy(), 1200l)
            tt |> success(put.status == PutStatus.invalid, "refused")
            tt |> success(!empty(put.error), "error named")
        }
    }
}

[test]
def test_admin_levers(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        let put = put_records(db, records_doc(), "10.3.0.1", test_policy(), 1000l)
        t |> run("submission verify flips every run") @(tt : T?) {
            tt |> equal(set_submission_verified(db, put.submission_id, true), 2)
            for (r in list_runs(db, 0)) {
                tt |> equal(r.Verified, 1)
            }
            tt |> equal(set_submission_verified(db, put.submission_id, false), 2)
        }
        t |> run("submission delete removes its runs") @(tt : T?) {
            tt |> equal(delete_submission(db, put.submission_id), 2)
            tt |> equal(length(list_runs(db, 0)), 0)
            tt |> equal(get_submission_doc(db, put.submission_id), "")
            tt |> equal(delete_submission(db, put.submission_id), 0)
        }
        t |> run("sidecar submission delete removes the sidecar row") @(tt : T?) {
            let sc = put_sidecar(db, sidecar_doc(BOX_A, "3", "vec8"), "10.3.0.2", test_policy(), 2000l)
            tt |> success(sc.created, "stored")
            let hits <- find_sidecars(db, 3, ident_a())
            tt |> equal(length(hits), 1)
            // a sidecar submission's DocSha IS the sidecar's content sha
            let sids <- _sql(db |> select_from(type<Submission>)
                |> _where(_.DocSha == sc.sha)
                |> _select(_.Id))
            tt |> equal(length(sids), 1)
            tt |> success(delete_submission(db, sids[0]) > 0, "submission deleted")
            let after <- find_sidecars(db, 3, ident_a())
            tt |> equal(length(after), 0)
        }
    }
}

[test]
def test_official_import(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        let text1 = official_doc()
        t |> run("official rows import verified") @(tt : T?) {
            let res = import_official_store(db, text1, 1000l)
            tt |> success(res.ok, "imported")
            tt |> equal(res.runs_added, 2)
            for (r in list_runs(db, 0)) {
                tt |> equal(r.Source, "official")
                tt |> equal(r.Verified, 1)
            }
        }
        t |> run("a byte-identical re-import is a no-op") @(tt : T?) {
            let res = import_official_store(db, text1, 2000l)
            tt |> success(res.ok, "ok")
            tt |> equal(res.runs_added, 0)
            tt |> equal(length(list_runs(db, 0)), 2)
        }
        t |> run("a re-measure replaces on the identity, never duplicates") @(tt : T?) {
            // the re-measure: same identity, one number moved
            let text2 = replace(text1, "259.6", "261.0")
            let res = import_official_store(db, text2, 3000l)
            tt |> success(res.ok, "ok")
            tt |> equal(res.runs_added, 2)
            tt |> equal(res.runs_replaced, 2)
            tt |> equal(length(list_runs(db, 0)), 2)
        }
        t |> run("garbage is refused") @(tt : T?) {
            let res = import_official_store(db, "not json", 4000l)
            tt |> success(!res.ok, "refused")
            tt |> success(!empty(res.error), "error named")
        }
    }
}

[test]
def test_list_versions(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        var pol = test_policy()
        pol.max_submissions_per_ip_hour = 100
        put_records(db, records_doc(), "10.4.0.1", pol, 1000l)
        put_sidecar(db, sidecar_doc(BOX_A, "5", "vec8"), "10.4.0.1", pol, 1001l)
        t |> run("versions merge from runs and sidecars, newest first") @(tt : T?) {
            // runs carry 3 (das) and 0 (reference); the sidecar adds 5
            let vs <- list_versions(db)
            tt |> equal(length(vs), 3)
            tt |> equal(vs[0], 5)
            tt |> equal(vs[1], 3)
            tt |> equal(vs[2], 0)
        }
    }
}
