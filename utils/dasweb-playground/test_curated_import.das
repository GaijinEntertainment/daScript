options gen2

require dastest/testing_boost public
require curated_import
require daslib/sql
require daslib/sql_linq
require daslib/fio
require daslib/json_boost

def private fixture_dir(t : T?; tag : string) : string {
    var err = ""
    let root = temp_directory(err)
    t |> success(!empty(root), "temp dir available")
    let dir = path_join(root, "pg_import_{tag}")
    mkdir_rec(path_join(dir, "examples"))
    return dir
}

def private write_manifest(dir : string; body : string) {
    fwrite(path_join(dir, "data.json"), body)
}

def private nuke(dir : string) {
    // best-effort cleanup; temp files are per-run uniques either way
    for (f in ["data.json", "examples/hello.das", "examples/multi_a.das", "examples/multi_b.das"]) {
        var err = ""
        remove(path_join(dir, f), err)
    }
    var derr = ""
    rmdir(path_join(dir, "examples"), derr)
    rmdir(dir, derr)
}

let MANIFEST_V1 = ("\{ \"examples\": [" +
    "\{ \"name\": \"Hello world\", \"files\": [\"examples/hello.das\"] \}," +
    "\{ \"name\": \"Multi (multi-file)\", \"slug\": \"multi\", \"files\": [\"examples/multi_a.das\", \"examples/multi_b.das\"] \}" +
    "] \}")

[test]
def test_import_and_idempotency(t : T?) {
    let dir = fixture_dir(t, "basic")
    write_manifest(dir, MANIFEST_V1)
    fwrite(path_join(dir, "examples/hello.das"), "print(\"hello\")")
    fwrite(path_join(dir, "examples/multi_a.das"), "require multi_b")
    fwrite(path_join(dir, "examples/multi_b.das"), "def helper \{ \}")
    with_latest_sqlite(":memory:") $(db) {
        t |> run("import lists both entries with metadata") @(tt : T?) {
            tt |> equal(import_curated(db, dir, 1000l), 2)
            let listed <- listed_samples(db)
            tt |> equal(length(listed), 2)
            tt |> equal(listed[0].title, "Hello world")
            tt |> equal(listed[0].path, "examples/hello.das")
            tt |> equal(listed[0].slug, "")
            tt |> equal(listed[1].title, "Multi (multi-file)")
            tt |> equal(listed[1].slug, "multi")
        }
        t |> run("single-file body is the raw source") @(tt : T?) {
            let listed <- listed_samples(db)
            tt |> equal(get_source(db, listed[0].hash), "print(\"hello\")")
        }
        t |> run("multi-file body is the share bundle shape") @(tt : T?) {
            let listed <- listed_samples(db)
            let src = get_source(db, listed[1].hash)
            var jerr = ""
            var doc = read_json(src, jerr)
            tt |> success(doc != null, "bundle parses as json")
            tt |> equal(doc?["active"] ?? "", "multi_a.das")
            tt |> equal(doc?["files"]?["multi_a.das"] ?? "", "require multi_b")
            tt |> equal(doc?["files"]?["multi_b.das"] ?? "", "def helper \{ \}")
        }
        t |> run("re-import is a no-op") @(tt : T?) {
            let before <- listed_samples(db)
            tt |> equal(import_curated(db, dir, 2000l), 2)
            let after <- listed_samples(db)
            tt |> equal(length(after), length(before))
            tt |> equal(after[0].hash, before[0].hash)
            tt |> equal(after[1].hash, before[1].hash)
        }
    }
    nuke(dir)
}

[test]
def test_reimport_repoints_and_demotes(t : T?) {
    let dir = fixture_dir(t, "repoint")
    write_manifest(dir, MANIFEST_V1)
    fwrite(path_join(dir, "examples/hello.das"), "print(\"v1\")")
    fwrite(path_join(dir, "examples/multi_a.das"), "require multi_b")
    fwrite(path_join(dir, "examples/multi_b.das"), "def helper \{ \}")
    with_latest_sqlite(":memory:") $(db) {
        t |> equal(import_curated(db, dir, 1000l), 2)
        var old_hash = ""
        {
            let listed <- listed_samples(db)
            old_hash = listed[0].hash
        }
        t |> run("changed source repoints the listing, keeps the old permalink") @(tt : T?) {
            fwrite(path_join(dir, "examples/hello.das"), "print(\"v2\")")
            tt |> equal(import_curated(db, dir, 2000l), 2)
            let listed <- listed_samples(db)
            tt |> success(listed[0].hash != old_hash, "listing points at the new hash")
            tt |> equal(get_source(db, listed[0].hash), "print(\"v2\")")
            tt |> equal(get_source(db, old_hash), "print(\"v1\")")
        }
        t |> run("curated entry deduping onto a user row still demotes when dropped") @(tt : T?) {
            // regression for the Origin-filter miss: the USER stores bytes the
            // importer has never seen; the importer then lists that same content
            // (dedup keeps Origin "user"); dropping it from the manifest must
            // still unlist it.
            let user_put = put_sample(db, "print(\"user-first\")", "10.0.0.7", StorePolicy(), 2500l)
            tt |> success(user_put.created, "user stores these bytes first")
            fwrite(path_join(dir, "examples/hello.das"), "print(\"user-first\")")
            tt |> equal(import_curated(db, dir, 2600l), 2)
            let row <- _sql(db |> select_from(type<Sample>) |> _where(_.Hash == user_put.hash))
            tt |> equal(length(row), 1)
            tt |> equal(row[0].Origin, "user")
            tt |> equal(row[0].Listed, 1)   // importer listed the user-origin row
            write_manifest(dir, "\{ \"examples\": [ \{ \"name\": \"Multi (multi-file)\", \"slug\": \"multi\", \"files\": [\"examples/multi_a.das\", \"examples/multi_b.das\"] \} ] \}")
            tt |> equal(import_curated(db, dir, 2700l), 1)
            var titles : array<string>
            for (l in listed_samples(db)) {
                titles |> push(l.title)
            }
            tt |> success(find_index(titles, "Hello world") < 0, "user-origin curated listing demoted")
            tt |> equal(get_source(db, user_put.hash), "print(\"user-first\")")   // permalink survives
            // restore the fixture state the next subtest expects
            write_manifest(dir, MANIFEST_V1)
            fwrite(path_join(dir, "examples/hello.das"), "print(\"v2\")")
            tt |> equal(import_curated(db, dir, 2800l), 2)
        }
        t |> run("entry removed from the manifest gets demoted, user promotions survive") @(tt : T?) {
            let user = put_sample(db, "let mine = 1", "10.0.0.1", StorePolicy(), 3000l)
            tt |> success(promote_sample(db, user.hash, "Mine", "community"), "user promote")
            write_manifest(dir, "\{ \"examples\": [ \{ \"name\": \"Hello world\", \"files\": [\"examples/hello.das\"] \} ] \}")
            tt |> equal(import_curated(db, dir, 4000l), 1)
            let listed <- listed_samples(db)
            tt |> equal(length(listed), 2)   // hello + the user promotion
            var titles : array<string>
            for (l in listed) {
                titles |> push(l.title)
            }
            tt |> success(find_index(titles, "Mine") >= 0, "user promotion still listed")
            tt |> success(find_index(titles, "Multi (multi-file)") < 0, "vanished entry demoted")
        }
    }
    nuke(dir)
}

[test]
def test_import_failure_modes(t : T?) {
    with_latest_sqlite(":memory:") $(db) {
        t |> run("missing manifest returns -1") @(tt : T?) {
            tt |> equal(import_curated(db, "no/such/dir", 1000l), -1)
        }
        t |> run("bad json returns -1") @(tt : T?) {
            let dir = fixture_dir(tt, "badjson")
            write_manifest(dir, "\{ not json")
            tt |> equal(import_curated(db, dir, 1000l), -1)
            nuke(dir)
        }
        t |> run("entry with a missing file is skipped, the rest import") @(tt : T?) {
            let dir = fixture_dir(tt, "missing")
            write_manifest(dir, MANIFEST_V1)
            fwrite(path_join(dir, "examples/hello.das"), "print(\"hello\")")
            // multi_a/multi_b deliberately absent
            tt |> equal(import_curated(db, dir, 1000l), 1)
            let listed <- listed_samples(db)
            tt |> equal(length(listed), 1)
            tt |> equal(listed[0].title, "Hello world")
            nuke(dir)
        }
    }
}
