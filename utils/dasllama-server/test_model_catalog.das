options gen2
options persistent_heap

// The curated catalog + download rail, model-free: table invariants (pinned URLs, unique
// ids, one default), the models-dir precedence, local-presence detection, and the
// download-start refusals that need no network.

require dastest/testing_boost public
require model_catalog
require daslib/fio
require daslib/json_boost
require strings

def private tmp_models_dir(tag : string) : string {
    var terr : string
    let base = temp_directory(terr)
    let dir = path_join(empty(base) ? "." : base, "dasllama_catalog_test_{tag}_{ref_time_ticks()}")
    mkdir_rec(dir)
    return dir
}

[test]
def test_catalog_table(t : T?) {
    t |> run("entries are pinned, unique, and exactly one is the default") @(t : T?) {
        var names : table<string>
        var files : table<string>
        var defaults = 0
        var count = 0
        for (e in model_catalog()) {
            count++
            t |> success(!key_exists(names, e.name), "duplicate name {e.name}")
            names |> insert(e.name)
            t |> success(!key_exists(files, e.file), "duplicate file {e.file}")
            files |> insert(e.file)
            t |> equal(length(e.sha256), 64, "{e.name}: sha256 must be 64 hex chars")
            t |> success(e.bytes > 0l, "{e.name}: canonical size missing")
            t |> success(e.url |> starts_with("https://"), "{e.name}: url must be https")
            // commit-pinned: /resolve/<40-hex>/, never /resolve/main/ (a re-upload under the
            // same name would silently change the bytes the sha then rejects)
            t |> success(find(e.url, "/resolve/main/") < 0, "{e.name}: url is not commit-pinned")
            t |> success(e.ctx > 0, "{e.name}: ctx missing")
            if (e.is_default) {
                defaults++
            }
        }
        t |> equal(defaults, 1, "exactly one recommended default")
        t |> success(count >= 5, "curated list shrank: {count} entries, expected at least 5")
    }
}

[test]
def test_models_dir_resolution(t : T?) {
    t |> run("the pure core: env > config > home default, separators stripped, roots kept") @(t : T?) {
        t |> equal(catalog_models_dir_from("D:/somewhere/models/", "", "H:/u"), "D:/somewhere/models")
        t |> equal(catalog_models_dir_from("D:\\somewhere\\models\\", "", "H:/u"), "D:\\somewhere\\models")
        t |> equal(catalog_models_dir_from("D:/cfg-models", "E:/env-models/", "H:/u"), "E:/env-models")
        t |> equal(catalog_models_dir_from("", "", "H:/u"), path_join(path_join("H:/u", ".dasllama"), "models"))
        // bare roots must keep their separator - stripping "/" or "C:\\" retargets the cwd/drive
        t |> equal(catalog_models_dir_from("", "/", "H:/u"), "/")
        t |> equal(catalog_models_dir_from("", "C:\\", "H:/u"), "C:\\")
    }
    t |> run("the wrapper resolves through the env rail to a usable absolute default") @(t : T?) {
        let dflt = catalog_models_dir("")
        t |> success(!empty(dflt), "wrapper resolves a dir")
        if (empty(catalog_env_override())) {
            t |> success(find(dflt, ".dasllama") >= 0, "default lands under ~/.dasllama, got {dflt}")
        }
    }
}

[test]
def test_local_presence(t : T?) {
    t |> run("an absent file is not present; a planted one is, at its path") @(t : T?) {
        let dir = tmp_models_dir("presence")
        let probe = CatalogEntry(name = "probe", file = "probe.gguf")
        t |> equal(catalog_local_path(dir, probe), "")
        let planted = path_join(dir, "probe.gguf")
        fopen(planted, "wb") $(f) {
            if (f != null) {
                f |> fwrite("GGUF")
            }
        }
        t |> equal(catalog_local_path(dir, probe), planted)
        remove(planted)
        rmdir(dir)
    }
}

[test]
def test_download_refusals(t : T?) {
    t |> run("no events channel refuses instead of spawning a doomed worker") @(t : T?) {
        let dir = tmp_models_dir("chan")
        let e = CatalogEntry(name = "x", file = "x.gguf", bytes = 4l, sha256 = "00", url = "https://example.invalid/x")
        let refusal = catalog_download_start(dir, e)
        t |> success(find(refusal, "not initialized") >= 0, "got: {refusal}")
        rmdir(dir)
    }
    t |> run("an already-present file refuses the re-download") @(t : T?) {
        catalog_events_init()
        let dir = tmp_models_dir("present")
        let e = CatalogEntry(name = "x", file = "x.gguf", bytes = 4l, sha256 = "00", url = "https://example.invalid/x")
        let planted = path_join(dir, "x.gguf")
        fopen(planted, "wb") $(f) {
            if (f != null) {
                f |> fwrite("GGUF")
            }
        }
        let refusal = catalog_download_start(dir, e)
        t |> success(find(refusal, "already present") >= 0, "got: {refusal}")
        catalog_events_shutdown()
        remove(planted)
        rmdir(dir)
    }
}

[test]
def test_state_json(t : T?) {
    t |> run("the /catalog document carries every entry with presence and an idle machine") @(t : T?) {
        let dir = tmp_models_dir("doc")
        var jerr = ""
        var doc = read_json(catalog_state_json(dir), jerr)
        t |> success(doc != null, "state json parses: {jerr}")
        if (doc != null) {
            t |> equal(doc?["models_dir"] ?? "", dir)
            let entries = doc?["entries"]
            t |> success(entries != null && (entries.value is _array), "entries is an array")
            if (entries != null && (entries.value is _array)) {
                t |> equal(length(entries.value as _array), length(model_catalog()))
                for (e in entries.value as _array) {
                    t |> equal(e?["present"] ?? true, false, "nothing is present in a fresh dir")
                }
            }
            t |> equal(doc?["download"]?["state"] ?? "", "idle")
        }
        unsafe {
            delete doc
        }
        rmdir(dir)
    }
}
