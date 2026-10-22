options gen2

require dastest/testing_boost public
require ladder_store
require _ladder_test_common
require daslib/sql_linq
require daslib/json_boost
require daslib/strings_boost   // replace — the re-measure fixture edit

// Store-level coverage: policy gates, source stamping, the sidecar lookup ladder,
// official replace-by-identity, and the admin levers — no HTTP anywhere.

def private test_policy : LadderPolicy =>
    LadderPolicy(max_records_bytes = 65536, max_sidecar_bytes = 4096, max_submissions_per_ip_hour = 2)

def private ident_a : BoxIdentity {
    var b : BoxIdentity
    parse_box_string(BOX_A, b)
    return b
}

def private audio_cells(text : string) : array<AudioCell> {
    var jerr = ""
    var js = read_json(text, jerr)
    var out : array<AudioCell>
    if (js != null && js is _array) {
        out <- [for (c in js as _array); AudioCell(test = c?["test"] ?? "", ms = c?["ms"] ?? 0.0lf, audio_s = c?["audio_s"] ?? -1.0lf, voided = c?["voided"] ?? false)]
    }
    delete_json(js)
    return <- out
}

//! a v1 store (migration 1 stamped applied, every run in the v1 row shape) - the input the later migrations' backfills read
def private seed_v1_store(db : SqlRunner) {
    ladder_migration_001(db)
    baseline(db, 1)
    db |> exec("INSERT INTO submissions (Id, Kind, DocSha, Doc, CreatedAt, ClientIp) VALUES (7, 'records', 'sha7', ?, 1000, '10.0.0.1')", records_doc())
    db |> exec("INSERT INTO submissions (Id, Kind, DocSha, Doc, CreatedAt, ClientIp) VALUES (8, 'official', 'sha8', ?, 1000, 'importer')", official_doc())
    for (sid in [7, 8]) {
        for (ri in range(2)) {
            db |> exec("INSERT INTO runs (SubmissionId, ModelIdx, RunIdx, Gguf, Engine, Backend, Flavor, Box, Threads, RunDate, Source, CreatedAt) " +
                "VALUES (?, 0, ?, 'x.gguf', ?, 'cpu', 'tuned', 'testbox', 8, '2026-08-10', 'community', 1000)",
                sid, ri, ri == 0 ? "das" : "llama.cpp")
        }
    }
    db |> exec("INSERT INTO submissions (Id, Kind, DocSha, Doc, CreatedAt, ClientIp) VALUES (9, 'official', 'sha9', ?, 1000, 'importer')", audio_doc())
    for (slot in [(0, 0), (0, 1), (0, 2), (1, 0), (1, 1)]) {
        db |> exec("INSERT INTO runs (SubmissionId, ModelIdx, RunIdx, Gguf, Engine, Backend, Flavor, Box, Threads, RunDate, Source, CreatedAt) " +
            "VALUES (9, ?, ?, 'a.bin', 'das', 'cpu', 'tuned', 'm5', 6, '2026-09-23', 'official', 1000)",
            slot._0, slot._1)
    }
    db |> exec("INSERT INTO submissions (Id, Kind, DocSha, Doc, CreatedAt, ClientIp) VALUES (10, 'official', 'sha10', ?, 1000, 'importer')", audio_outlier_doc())
    for (ri in range(4)) {
        db |> exec("INSERT INTO runs (SubmissionId, ModelIdx, RunIdx, Gguf, Engine, Backend, Flavor, Box, Threads, RunDate, Source, CreatedAt) " +
            "VALUES (10, 0, ?, 'o.gguf', 'das', 'cpu', 'tuned', 'm5', 8, '2026-10-21', 'official', 1000)", ri)
    }
    db |> exec("INSERT INTO submissions (Id, Kind, DocSha, Doc, CreatedAt, ClientIp) VALUES (11, 'official', 'sha11', ?, 1000, 'importer')", image_doc())
    for (ri in range(2)) {
        db |> exec("INSERT INTO runs (SubmissionId, ModelIdx, RunIdx, Gguf, Engine, Backend, Flavor, Box, Threads, RunDate, Source, CreatedAt) " +
            "VALUES (11, 0, ?, 'v.gguf', 'das', 'metal', 'tuned', 'm5', 6, '2026-10-21', 'official', 1000)", ri)
    }
}

[test]
def test_migration(t : T?) {
    t |> run("fresh db migrates to latest") @(t : T?) {
        with_latest_sqlite(":memory:") $(db) {
            t |> equal(current_schema_version(db), 9)
        }
    }
    t |> run("migrations 2 to 9 backfill the cells of rows inserted before the columns existed") @(tt : T?) {
        with_sqlite(":memory:") $(db) {
            seed_v1_store(db)
            tt |> equal(migrate_to_latest(db), 8, "eight pending migrations applied")
            tt |> equal(current_schema_version(db), 9)
            let rows <- _sql(db |> select_from(type<Run>) |> _order_by(_.Id))
            tt |> equal(length(rows), 15)
            tt |> equal(rows[13].ImgEnc, 58.0lf, "migration 9 fills the image cells")
            tt |> equal(rows[13].ImgPp, 4211.5lf)
            tt |> equal(rows[13].ImgTg, 113.9lf)
            tt |> equal(rows[14].ImgTg, 0.0lf, "a cell the reference did not measure stays 0")
            tt |> equal(rows[0].ImgEnc, 0.0lf, "an LLM run carries no image cells")
            tt |> equal(rows[0].Audio, "", "an LLM run carries no audio cells")
            let asr <- audio_cells(rows[4].Audio)
            tt |> equal(length(asr), 2, "migration 4 fills the asr cells from the document")
            tt |> equal(asr[0].test, "gb1.wav")
            tt |> equal(asr[1].ms, 158.0lf)
            tt |> success(!asr[0].voided && !asr[1].voided, "two rated clips void nothing")
            let tts <- audio_cells(rows[7].Audio)
            tt |> equal(length(tts), 2, "... and the tts cells")
            tt |> equal(tts[1].audio_s, 4.0lf)
            let cold <- audio_cells(rows[10].Audio)
            tt |> equal(length(cold), 3, "migration 6 fills the reference's cells with the verdict")
            tt |> equal(cold[0].test, "gb1.wav")
            tt |> equal(cold[2].test, "jfk3.wav")
            tt |> success(!cold[0].voided && !cold[2].voided, "the even clips stand")
            tt |> equal(cold[1].test, "jfk.wav")
            tt |> success(cold[1].voided, "the cold-start clip is voided off the das run's lengths")
            tt |> success(!audio_cells(rows[9].Audio)[1].voided, "the das run's own jfk.wav is even")
            tt |> equal(rows[4].Pp512, 0.0lf, "migration 2 leaves an audio run's LLM cells at 0")
            tt |> equal(rows[0].Pp512, 100.0lf)
            tt |> equal(rows[0].Tg128, 25.0lf)
            tt |> equal(rows[0].Tg128b4, 60.0lf, "the batched cell is filled by migration 3")
            tt |> equal(rows[1].Pp512, 80.0lf)
            tt |> equal(rows[1].Tg128, 0.0lf, "a cell the run did not measure stays 0")
            tt |> equal(rows[1].Tg128b4, 0.0lf, "... the batched cell too")
            tt |> equal(rows[0].Cpu, "Test CPU 9000")
            tt |> equal(rows[2].Pp512, 259.6lf, "the official submission's rows are filled too")
            tt |> equal(rows[3].Pp512, 145.0lf)
            tt |> equal(rows[2].Cpu, "Apple M1 Max")
            tt |> equal(rows[2].Gpu, "Apple M1 Max (32-core)", "migration 7 fills the gpu label")
            tt |> equal(rows[3].Gpu, "", "a run whose hardware names no gpu stays empty")
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
        t |> run("a slash in the cpu brand string does not strip the box") @(tt : T?) {
            // zen4's box: read "w/ Radeon" as a path and the identity is stripped away, so the plant refuses with 400
            let apu_box = "linux|x86_64||6.1.0-49-amd64|AMD Ryzen 7 PRO 8700GE w/ Radeon 780M Graphics"
            let put = plant_sidecar(db, sidecar_doc(apu_box, "4", "vec16"), test_policy(), 3100l)
            tt |> success(put.status == PutStatus.ok, "planted: {put.error}")
            tt |> success(find(get_sidecar_doc(db, put.sha), "w/ Radeon") >= 0, "box survived the strip")
            var ident : BoxIdentity
            tt |> success(parse_box_string(apu_box, ident), "the box string parses")
            var hits <- find_sidecars(db, 4, ident)
            tt |> equal(length(hits), 1, "indexed under its own box")
            tt |> success(hits[0].verified && hits[0].tier == "exact", "verified, exact tier")
            delete hits
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
        t |> run("delete also drops the verbatim submission, so the content can be resubmitted") @(tt : T?) {
            let doc = sidecar_doc(BOX_C, "3", "vec8_solo")
            let first = put_sidecar(db, doc, "10.1.9.1", pol, 5000l)
            tt |> success(first.created, "stored")
            // no orphaned submission row: the DocSha is gone, so the unique index is clear
            tt |> success(delete_sidecar(db, first.sha), "deleted")
            let orphans <- _sql(db |> select_from(type<Submission>) |> _where(_.DocSha == first.sha) |> _select(_.Id))
            tt |> equal(length(orphans), 0, "no orphaned submission")
            // the exact same content resubmits cleanly instead of panicking the unique index
            let again = put_sidecar(db, doc, "10.1.9.2", pol, 6000l)
            tt |> success(again.created, "resubmit succeeds after delete")
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
            // the das row carries its stamps into the filter columns, and its cells + cpu label
            let das_rows <- list_runs(db, 3)
            tt |> equal(length(das_rows), 1)
            tt |> equal(das_rows[0].Engine, "das")
            tt |> equal(das_rows[0].TuneSha, "cafe0123beef")
            tt |> equal(das_rows[0].Threads, 8)
            tt |> equal(das_rows[0].Pp512, 100.0lf)
            tt |> equal(das_rows[0].Tg128, 25.0lf)
            tt |> equal(das_rows[0].Tg128b4, 60.0lf, "the insert path fills the batched cell like the migration does")
            tt |> equal(das_rows[0].Cpu, "Test CPU 9000")
            // the reference row carries its own cells: the board pairs from this table alone
            for (r in rows) {
                if (r.Engine == "llama.cpp") {
                    tt |> equal(r.Pp512, 80.0lf)
                }
            }
            // put_records redacts paths, which re-serializes the JSON — so the stored copy is
            // the redacted form (records_doc carries no paths, so redaction only normalizes it)
            tt |> equal(get_submission_doc(db, put.submission_id), redact_record_paths(doc))
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
def test_records_redacted_server_side(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("submitted records lose their home-layout paths") @(tt : T?) {
            // inject a home path into the das run's cmd, then submit
            var jerr = ""
            var jdoc = read_json(records_doc(), jerr)   // top is [ {model} ]
            var models & = unsafe(jdoc.value as _array)   // nolint:LINT003 — mutated in place below
            var model = models[0]
            var mruns = model?["runs"]
            var rarr & = unsafe(mruns.value as _array)
            update(rarr[0], "cmd", JV("/home/boris/llama.cpp/bin/llama-bench -m /home/boris/models/x.gguf"))
            let dirty = write_json(jdoc)
            delete_json(jdoc)
            let put = put_records(db, dirty, "10.2.9.1", test_policy(), 5000l)
            tt |> success(put.status == PutStatus.ok, "accepted ({put.error})")
            let stored = get_submission_doc(db, put.submission_id)
            tt |> success(find(stored, "boris") < 0, "no username in the stored record")
            tt |> success(find(stored, "llama-bench -m x.gguf") >= 0, "cmd redacted to basenames")
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

def private run_of(rows : array<Run>; engine, workload : string) : Run {
    for (r in rows) {
        return r if (r.Engine == engine && r.Workload == workload)
    }
    return Run()
}

[test]
def test_image_cells(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("an image-chat import lands the encode, prefill and decode cells") @(tt : T?) {
            let res = import_official_store(db, image_doc(), 1000l)
            tt |> success(res.ok, "imported ({res.error})")
            let das = run_of(list_runs(db, 0), "das", "image-chat")
            tt |> equal(das.ImgEnc, 58.0lf)
            tt |> equal(das.ImgPp, 4211.5lf)
            tt |> equal(das.ImgTg, 113.9lf)
            tt |> equal(das.Pp512, 0.0lf, "the image cells are not the LLM cells")
            let lcpp = run_of(list_runs(db, 0), "llama.cpp", "image-chat")
            tt |> equal(lcpp.ImgEnc, 48.0lf)
            tt |> equal(lcpp.ImgTg, 0.0lf, "a cell the run did not measure stays 0")
        }
    }
}

[test]
def test_voids(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("a void blanks the named cells of the runs it selects, the rest stand") @(tt : T?) {
            tt |> success(import_official_store(db, image_doc(), 1000l).ok, "image imported")
            tt |> success(import_official_store(db, audio_doc(), 1001l).ok, "audio imported")
            let notes = ("[\{\"gguf\":\"Qwen3VL-4B-Instruct-Q8_0.gguf\",\"engine\":\"llama.cpp\",\"workload\":\"image-chat\"," +
                "\"comment\":\"x\",\"void_clips\":\"img:pp\"\}," +
                "\{\"gguf\":\"ggml-parakeet-tdt-0.6b-v3-f32.bin\",\"engine\":\"das\",\"comment\":\"x\",\"void_clips\":\"jfk.wav\"\}]")
            let r = import_annotations(db, notes)
            tt |> success(r.ok, "imported ({r.error})")
            tt |> equal(r.voids, 2)
            tt |> equal(r.holds, 0)
            let rows <- list_runs(db, 0)
            tt |> equal(length(rows), 7, "a void withholds no run")
            let lcpp = run_of(rows, "llama.cpp", "image-chat")
            tt |> equal(lcpp.ImgPp, 0.0lf, "the voided reference cell reads unmeasured")
            tt |> equal(lcpp.ImgEnc, 48.0lf, "the cells the void does not name stand")
            tt |> equal(run_of(rows, "das", "image-chat").ImgPp, 4211.5lf, "the das run the void does not select stands")
            let asr <- audio_cells(run_of(rows, "das", "asr").Audio)
            tt |> success(asr[1].test == "jfk.wav" && asr[1].voided, "the named clip reads voided")
            tt |> success(!asr[0].voided, "the other clip stands")
            tt |> success(!audio_cells(run_of(rows, "parakeet-cli", "asr").Audio)[0].voided, "a run of another engine stands")
        }
    }
}

[test]
def test_holds(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("a hold withholds every run of its model on its workload, references included") @(tt : T?) {
            tt |> success(import_official_store(db, audio_doc(), 1000l).ok, "imported")
            tt |> equal(length(list_runs(db, 0)), 5, "no hold: the board holds every run (the control)")
            tt |> equal(length(list_runs(db, 17)), 1, "the kokoro das run is the fixture's one release-17 run")
            let annotations = ("[" +
                "\{\"gguf\":\"kokoro-82m.gguf\",\"engine\":\"das\",\"workload\":\"tts\",\"held\":\"followup_general.md 219\"\}," +
                "\{\"gguf\":\"ggml-parakeet-tdt-0.6b-v3-f32.bin\",\"comment\":\"a note, not a hold\"\}]")
            let h = import_annotations(db, annotations)
            tt |> success(h.ok, "holds imported ({h.error})")
            tt |> equal(h.holds, 1, "an entry with no held field holds nothing")
            let rows <- list_runs(db, 0)
            tt |> equal(length(rows), 3, "both kokoro runs leave, the mlx-audio reference too")
            for (r in rows) {
                tt |> success(r.Gguf != "kokoro-82m.gguf", "no held model on the board ({r.Engine})")
            }
            tt |> equal(length(list_runs(db, 17)), 0, "the version filter composes with the hold")
        }
        t |> run("an import replaces the holds whole; a malformed one changes nothing") @(tt : T?) {
            tt |> success(!import_annotations(db, "not json").ok, "refused")
            tt |> equal(length(list_runs(db, 0)), 3, "the refusal left the hold standing")
            tt |> success(import_annotations(db, "[]").ok, "an empty document clears the holds")
            tt |> equal(length(list_runs(db, 0)), 5, "the held runs are back")
        }
        t |> run("a hold on another workload or box leaves the run") @(tt : T?) {
            tt |> success(import_annotations(db, "[\{\"gguf\":\"kokoro-82m.gguf\",\"workload\":\"asr\",\"held\":\"x\"\}," +
                "\{\"gguf\":\"kokoro-82m.gguf\",\"box\":\"zen4\",\"held\":\"x\"\}]").ok, "imported")
            tt |> equal(length(list_runs(db, 0)), 5, "neither hold selects the m5 tts runs")
        }
    }
}

[test]
def test_audio_cells(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("an official asr/tts import fills the audio cells") @(tt : T?) {
            let res = import_official_store(db, audio_doc(), 1000l)
            tt |> success(res.ok, "imported ({res.error})")
            tt |> equal(res.runs_added, 5)
            let rows <- list_runs(db, 0)
            let das_asr <- audio_cells(run_of(rows, "das", "asr").Audio)
            tt |> equal(length(das_asr), 2, "zero-ms, unprefixed and other-workload tests are not cells")
            tt |> equal(das_asr[0].test, "gb1.wav", "sorted by test, the prefix stripped")
            tt |> equal(das_asr[0].audio_s, 198.734375lf)
            tt |> equal(das_asr[1].test, "jfk.wav")
            tt |> equal(das_asr[1].audio_s, 11.0lf, "an integer audio_s reads as its number")
            let ref_asr <- audio_cells(run_of(rows, "parakeet-cli", "asr").Audio)
            tt |> equal(length(ref_asr), 1)
            tt |> equal(ref_asr[0].ms, 159.5lf)
            tt |> equal(ref_asr[0].audio_s, 0.0lf, "a run that did not record the audio length carries 0")
            let mlx_asr = run_of(rows, "mlx-audio", "asr")
            tt |> equal(mlx_asr.Backend, "metal", "an mlx engine row lands like any other engine")
            tt |> equal(audio_cells(mlx_asr.Audio)[0].ms, 90.0lf)
            let das_tts <- audio_cells(run_of(rows, "das", "tts").Audio)
            tt |> equal(length(das_tts), 2, "tts cells key on the tts: prefix")
            tt |> equal(das_tts[0].test, "s01")
            tt |> equal(das_tts[0].audio_s, 2.5lf)
            tt |> equal(length(audio_cells(run_of(rows, "mlx-audio", "tts").Audio)), 1)
        }
        t |> run("an audio-chat run's cells are its asr-keyed clips") @(tt : T?) {
            let res = import_official_store(db, audio_chat_doc(), 1001l)
            tt |> success(res.ok, "imported ({res.error})")
            let rows <- list_runs(db, 0)
            let das_chat <- audio_cells(run_of(rows, "das", "audio-chat").Audio)
            tt |> equal(length(das_chat), 1, "the asr: clip is the cell, the tts: key is not")
            tt |> equal(das_chat[0].test, "jfk.wav", "the asr: prefix stripped")
            tt |> equal(das_chat[0].audio_s, 11.0lf)
            tt |> equal(audio_cells(run_of(rows, "llama-mtmd-cli", "audio-chat").Audio)[0].ms, 900.0lf, "the reference's clip too")
        }
        t |> run("an LLM run carries no audio cells") @(tt : T?) {
            import_official_store(db, official_doc(), 2000l)
            for (r in list_runs(db, 0)) {
                if (r.Workload == "") {
                    tt |> equal(r.Audio, "")
                }
            }
        }
        t |> run("a community asr submission lands its cells under the stamped-das rule") @(tt : T?) {
            var doc = %json~
            [ { "gguf" : "ggml-parakeet-tdt-0.6b-v3-f32.bin", "runs" : [ {
                "engine" : "das", "backend" : "cpu", "flavor" : "tuned", "workload" : "asr",
                "box" : "b", "threads" : 8, "date" : "2026-10-21", "cmd" : "dasllama-bench --asr",
                "sha" : "abc123def", "version" : "0.6.5", "dasllama_version" : 17,
                "tune" : "k4q8_tile_gen=mr8 (manifest)", "tune_sha" : "cafe0123beef", "exec_fmt" : "f32",
                "hardware" : { "cpu" : "x" },
                "tests" : { "asr:jfk.wav" : { "ms" : 200.0, "audio_s" : 11 } }
            } ] } ]
            %%
            let put = put_records(db, write_json(doc), "10.5.0.1", test_policy(), 3000l)
            tt |> success(put.status == PutStatus.ok, "accepted ({put.error})")
            let row = run_of(list_runs(db, 17), "das", "asr")
            tt |> equal(row.Source, "community")
            tt |> equal(audio_cells(row.Audio)[0].ms, 200.0lf, "the community row carries its cells")
        }
    }
}

def private outlier_run_cells(rows : array<Run>; engine, backend : string) : array<AudioCell> {
    for (r in rows) {
        if (r.Gguf == "Qwen3-Omni-30B-A3B-Instruct-Q8_0.gguf" && r.Engine == engine && r.Backend == backend) {
            return <- audio_cells(r.Audio)
        }
    }
    return <- array<AudioCell>()
}

[test]
def test_audio_cells_voided(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("a cell past the outlier rule, or named by the run's void_clips, rides voided") @(tt : T?) {
            let res = import_official_store(db, audio_outlier_doc(), 1002l)
            tt |> success(res.ok, "imported ({res.error})")
            let rows <- list_runs(db, 0)
            let cold <- outlier_run_cells(rows, "llama-mtmd-cli", "metal")
            tt |> equal(length(cold), 3)
            tt |> success(cold[1].test == "jfk.wav" && cold[1].voided, "3178 ms a second against a median of 36, rated over the das run's lengths")
            tt |> success(!cold[0].voided && !cold[2].voided, "the reference's even clips stand")
            tt |> equal(cold[1].audio_s, 0.0lf, "the borrowed length rates the cell and is not written onto it")
            for (c in outlier_run_cells(rows, "llama-mtmd-cli", "cpu")) {
                tt |> success(!c.voided, "an even run voids nothing ({c.test})")
            }
            for (c in outlier_run_cells(rows, "das", "metal")) {
                tt |> success(!c.voided, "the das run is even ({c.test})")
            }
            let named <- outlier_run_cells(rows, "llama-mtmd-cli", "vulkan")
            tt |> success(named[0].test == "gb1.wav" && named[0].voided, "a clip the run's own void_clips names rides voided")
            tt |> success(!named[1].voided && !named[2].voided, "... and its other clips stand")
        }
    }
}

[test]
def test_plant_sidecar(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("a planted sidecar stores verified with the importer stamp") @(tt : T?) {
            let doc = sidecar_doc(BOX_A, "4", "vec8_u8")
            let put = plant_sidecar(db, doc, test_policy(), 1000l)
            tt |> success(put.status == PutStatus.ok, "stored")
            tt |> success(put.created, "created")
            tt |> equal(get_sidecar_doc(db, put.sha), doc)
            tt |> equal(get_submission_doc(db, 1), doc, "the receipt row landed")
            let rows <- admin_list_sidecars(db)
            tt |> equal(length(rows), 1)
            tt |> success(rows[0].verified, "verified from birth")
            tt |> equal(rows[0].client_ip, "importer")
            tt |> equal(rows[0].box, BOX_A)
            tt |> equal(rows[0].dasllama_version, 4)
            let hits <- find_sidecars(db, 4, ident_a())
            tt |> equal(length(hits), 1)
            tt |> success(hits[0].verified, "a planted row serves on the lookup ladder")
        }
        t |> run("re-planting an existing sha promotes it") @(tt : T?) {
            // a community upload of the SAME content enters unverified; the re-plant flips it
            let doc = sidecar_doc(BOX_B, "4", "vec8_u8")
            let community = put_sidecar(db, doc, "10.0.0.9", test_policy(), 2000l)
            tt |> success(community.status == PutStatus.ok && community.created, "community stored")
            let put = plant_sidecar(db, doc, test_policy(), 2001l)
            tt |> success(put.status == PutStatus.ok && !put.created, "dedup hit")
            let rows <- admin_list_sidecars(db)
            tt |> equal(length(rows), 2)
            tt |> equal(rows[0].sha, put.sha, "newest first — the 2000l insert leads the 1000l plant")
            tt |> success(rows[0].verified, "the existing row is now verified")
        }
        t |> run("a plant of a non-strip-invariant doc still dedups its community twin") @(tt : T?) {
            // the community path hashes the STRIPPED text; plant must hash the same text
            var jerr = ""
            var jdoc = read_json(sidecar_doc(BOX_C, "4", "vec8_u8"), jerr)
            var prov = jdoc?["provenance"]
            update(prov, "binary", JV("/Users/someone/bin/daslang"))
            let dirty = write_json(jdoc)
            delete_json(jdoc)
            let community = put_sidecar(db, dirty, "10.0.0.10", test_policy(), 2100l)
            tt |> success(community.status == PutStatus.ok && community.created, "dirty community upload stored")
            let put = plant_sidecar(db, dirty, test_policy(), 2101l)
            tt |> success(put.status == PutStatus.ok && !put.created, "plant hashed the stripped text - dedup, not a second row")
            tt |> equal(put.sha, community.sha)
        }
        t |> run("plant validates and bounds like any import") @(tt : T?) {
            let none = plant_sidecar(db, "", test_policy(), 3000l)
            tt |> success(none.status == PutStatus.empty_doc, "empty doc refused at the store gate")
            let bad = plant_sidecar(db, "not json", test_policy(), 3001l)
            tt |> success(bad.status == PutStatus.invalid, "garbage refused")
            tt |> success(!empty(bad.error), "error named")
            let stale = plant_sidecar(db, sidecar_doc(BOX_C, "0", "vec8"), test_policy(), 3002l)
            tt |> success(stale.status == PutStatus.invalid, "unstamped sidecar refused - it could never be served")
            let tiny = LadderPolicy(max_sidecar_bytes = 64)
            let huge = plant_sidecar(db, sidecar_doc(BOX_C, "4", "vec8"), tiny, 3003l)
            tt |> success(huge.status == PutStatus.too_large, "size cap holds")
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
