options gen2
options persistent_heap

// The download rail's companion start, model-free and offline: a drafter pinned to a local file:// source starts under the
// id "drafter:<name>" the /catalog document's machine then names, and the worker's verify refuses bytes off the pinned sha.
// A file of its own: the download machine is process state, and test_model_catalog's document cells read it idle.

require dastest/testing_boost public
require model_catalog
require daslib/fio
require daslib/json_boost
require strings

def private tmp_dir(tag : string) : string {
    var terr : string
    let base = temp_directory(terr)
    let dir = path_join(empty(base) ? "." : base, "dasllama_catalog_dl_{tag}_{ref_time_ticks()}")
    mkdir_rec(dir)
    return dir
}

[unused_argument(path)]
def private no_packs(path : string) : bool => false

//! the download machine's `download` record off the /catalog document
def private download_field(dir, key : string) : string {
    var jerr = ""
    var doc = read_json(catalog_state_json(dir, 0, 0l, @@no_packs), jerr)
    let v = "{doc?["download"]?[key] ?? ""}"
    unsafe {
        delete doc
    }
    return v
}

[test]
def test_companion_download_starts(t : T?) {
    t |> run("a pinned drafter starts its download under the id drafter:<name>, and the worker refuses bytes off the pinned sha") @(t : T?) {
        catalog_events_init()
        let src_dir = tmp_dir("src")
        let dir = tmp_dir("models")
        let src = path_join(src_dir, "head.bin")
        fopen(src, "wb") $(f) {
            if (f != null) {
                f |> fwrite("not the pinned head")
            }
        }
        var e = CatalogEntry(name = "x", file = "x.gguf", bytes = 4l, sha256 = "00", url = "https://example.invalid/x")
        e.drafter.file = "mtp-x-Q8_0.gguf"
        e.drafter.url = "file://{src}"
        e.drafter.sha256 = "0000000000000000000000000000000000000000000000000000000000000000"
        e.drafter.bytes = int64(stat(src).size)
        t |> equal(download_field(dir, "state"), "idle", "the machine is idle before the start")
        let refusal = catalog_download_start_companion(dir, e, CatalogCompanionKind.drafter)
        t |> equal(refusal, "", "the pinned drafter's download starts")
        t |> equal(download_field(dir, "name"), "drafter:x", "the machine names the download by the companion's id")
        t |> equal(download_field(dir, "name"), catalog_download_id(e, "drafter"), "... the id the ACCEPTED body echoes")
        for (_ in range(600)) {
            break if (!catalog_download_running())
            sleep(100u)
            catalog_download_tick()
        }
        t |> success(!catalog_download_running(), "the worker finished within a minute")
        t |> equal(download_field(dir, "state"), "failed", "bytes off the pinned sha fail the verify")
        t |> success(find(download_field(dir, "error"), "sha256 MISMATCH") >= 0, "the failure names the sha mismatch: {download_field(dir, "error")}")
        t |> success(!stat(path_join(dir, e.drafter.file)).is_valid, "no drafter file lands beside the models")
        catalog_events_shutdown()
        remove(src)
        rmdir(src_dir)
        rmdir(dir)
    }
}
