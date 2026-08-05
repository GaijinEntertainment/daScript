options gen2

require dastest/testing_boost public
require buildd_core
require daslib/sha_256
require daslib/json_boost
require daslib/fio
require strings

def private with_temp_dir(prefix : string; blk : block<(dir : string) : void>) {
    var err = ""
    let dir = create_temp_directory(prefix, err)
    assert(!empty(dir), "temp dir created")
    invoke(blk, dir)
    rmdir_rec(dir)
}

[test]
def test_source_filename_validation(t : T?) {
    t |> run("boring .das names pass, everything else is refused") @(tt : T?) {
        tt |> success(is_valid_source_filename("main.das"), "main.das")
        tt |> success(is_valid_source_filename("live_stub.das"), "underscores")
        tt |> success(!is_valid_source_filename("x.wasm"), "wrong suffix")
        tt |> success(!is_valid_source_filename("../evil.das"), "traversal")
        tt |> success(!is_valid_source_filename("a/b.das"), "separator")
        tt |> success(!is_valid_source_filename(".hidden.das"), "leading dot")
        tt |> success(!is_valid_source_filename(""), "empty")
    }
}

[test]
def test_unpack_raw_source(t : T?) {
    with_temp_dir("buildd_src_") $(dir) {
        t |> run("raw das text becomes main.das") @(tt : T?) {
            let entry = unpack_sources(dir, "print(\"hi\")")
            tt |> equal(entry, "main.das")
            tt |> equal(fread(path_join(dir, "main.das")), "print(\"hi\")")
        }
    }
}

[test]
def test_unpack_bundle(t : T?) {
    with_temp_dir("buildd_src_") $(dir) {
        t |> run("a bundle with files and active materializes with its entry") @(tt : T?) {
            var files_obj <- { "main.das" => JV("require live_stub\nprint(1)"), "live_stub.das" => JV("// stub") }
            let bundle = write_json(JV((files = JV(files_obj), active = "main.das")))
            let entry = unpack_sources(dir, bundle)
            tt |> equal(entry, "main.das")
            tt |> equal(fread(path_join(dir, "live_stub.das")), "// stub")
        }
    }
    with_temp_dir("buildd_src_") $(dir) {
        t |> run("a bundle with a hostile file name is refused whole") @(tt : T?) {
            var files_obj <- { "main.das" => JV("print(1)"), "../../evil.das" => JV("boom") }
            let bundle = write_json(JV((files = JV(files_obj), active = "main.das")))
            tt |> equal(unpack_sources(dir, bundle), "")
        }
        t |> run("a bundle whose active is not among its files is refused") @(tt : T?) {
            var files_obj <- { "a.das" => JV("print(1)"), "b.das" => JV("print(2)") }
            let bundle = write_json(JV((files = JV(files_obj), active = "c.das")))
            tt |> equal(unpack_sources(dir, bundle), "")
        }
        t |> run("a bundle without active is refused") @(tt : T?) {
            var files_obj <- { "a.das" => JV("print(1)"), "b.das" => JV("print(2)") }
            let bundle = write_json(JV((files = JV(files_obj))))
            tt |> equal(unpack_sources(dir, bundle), "")
        }
    }
}

[test]
def test_run_build_command(t : T?) {
    t |> run("empty build_cmd reports itself") @(tt : T?) {
        let r = run_build_command("", "s", "o", "module", "main.das", 5.0)
        tt |> success(!r.ok, "not ok")
        tt |> success(find(r.output, "not configured") >= 0, "explains why")
    }
    t |> run("a real command runs and its output is captured") @(tt : T?) {
        let r = run_build_command("git --version", "s", "o", "module", "main.das", 30.0)
        tt |> success(r.ok, "git --version exits 0")
        tt |> success(find(r.output, "git version") >= 0, "stdout captured")
    }
    t |> run("a failing command reports its exit code") @(tt : T?) {
        let r = run_build_command("git definitely-not-a-subcommand", "s", "o", "module", "main.das", 30.0)
        tt |> success(!r.ok, "nonzero exit")
        tt |> success(r.exit_code != 0, "code preserved")
    }
}

[test]
def test_collect_artifacts(t : T?) {
    with_temp_dir("buildd_out_") $(dir) {
        fwrite(path_join(dir, "sample.wasm"), "wasm bytes")
        fwrite(path_join(dir, "index.html"), "<html/>")
        fwrite(path_join(dir, "build.log"), "junk")
        fwrite(path_join(dir, ".hidden.wasm"), "junk")
        t |> run("only servable names are collected, sorted, with shas") @(tt : T?) {
            let files <- collect_artifacts(dir)
            tt |> equal(length(files), 2)
            tt |> equal(files[0].name, "index.html")
            tt |> equal(files[1].name, "sample.wasm")
            tt |> equal(files[1].sha256, sha256_hex("wasm bytes"))
            tt |> equal(files[0].sha256, sha256_file_hex(path_join(dir, "index.html")))
        }
        t |> run("the manifest is the upload shape") @(tt : T?) {
            let files <- collect_artifacts(dir)
            var jerr = ""
            var doc = read_json(manifest_json(files), jerr)
            tt |> success(doc != null && doc.value is _array, "json array")
            tt |> equal((doc.value as _array)[1]?["name"] ?? "", "sample.wasm")
            tt |> equal((doc.value as _array)[1]?["sha256"] ?? "", sha256_hex("wasm bytes"))
        }
    }
}

[test]
def test_derive_toolchain(t : T?) {
    t |> run("this repo's HEAD is a 40-hex sha") @(tt : T?) {
        let sha = derive_toolchain(".")
        tt |> equal(length(sha), 40)
    }
    t |> run("a non-repo dir derives nothing") @(tt : T?) {
        var err = ""
        let dir = create_temp_directory("buildd_norepo_", err)
        tt |> equal(derive_toolchain(dir), "")
        rmdir_rec(dir)
    }
}
