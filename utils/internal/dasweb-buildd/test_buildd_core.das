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

[test(description = "only plain .das file names pass the source-name check; every other name is refused")]
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

[test(description = "a raw daslang source unpacks into main.das")]
def test_unpack_raw_source(t : T?) {
    with_temp_dir("buildd_src_") $(dir) {
        t |> run("raw das text becomes main.das") @(tt : T?) {
            let entry = unpack_sources(dir, "print(\"hi\")").entry
            tt |> equal(entry, "main.das")
            tt |> equal(fread(path_join(dir, "main.das")), "print(\"hi\")")
        }
    }
}

[test(description = "a source this service cannot write is its own failure, and a refused document is not")]
def test_unpack_write_failure(t : T?) {
    with_temp_dir("buildd_src_") $(dir) {
        let missing = path_join(dir, "no_such_directory")
        t |> run("a raw source with nowhere to land") @(tt : T?) {
            let unpacked = unpack_sources(missing, "print(\"hi\")")
            tt |> equal(unpacked.entry, "")
            tt |> success(unpacked.write_failed, "write failure reported")
        }
        t |> run("a bundle with nowhere to land") @(tt : T?) {
            var files_obj <- { "main.das" => JV("print(1)") }
            let unpacked = unpack_sources(missing,
                                          write_json(
                                                     JV(
                                                        (files = JV(files_obj),
                                                         active = "main.das"))))
            tt |> equal(unpacked.entry, "")
            tt |> success(unpacked.write_failed, "write failure reported")
        }
        t |> run("a refused document is not a write failure") @(tt : T?) {
            var files_obj <- { "a.das" => JV("print(1)") }
            let unpacked = unpack_sources(dir,
                                          write_json(JV((files = JV(files_obj), active = "c.das"))))
            tt |> equal(unpacked.entry, "")
            tt |> success(!unpacked.write_failed, "the document's own fault")
        }
    }
}

[test(description = "a bundle unpacks with its active entry, and one with a hostile name, an active outside its files, no active, or non-text content is refused whole")]
def test_unpack_bundle(t : T?) {
    with_temp_dir("buildd_src_") $(dir) {
        t |> run("a bundle with files and active materializes with its entry") @(tt : T?) {
            var files_obj <- { "main.das" => JV("require live_stub\nprint(1)"), "live_stub.das" => JV("// stub") }
            let bundle = write_json(JV((files = JV(files_obj), active = "main.das")))
            let entry = unpack_sources(dir, bundle).entry
            tt |> equal(entry, "main.das")
            tt |> equal(fread(path_join(dir, "live_stub.das")), "// stub")
        }
    }
    with_temp_dir("buildd_src_") $(dir) {
        t |> run("a bundle with a hostile file name is refused whole") @(tt : T?) {
            var files_obj <- { "main.das" => JV("print(1)"), "../../evil.das" => JV("boom") }
            let bundle = write_json(JV((files = JV(files_obj), active = "main.das")))
            tt |> equal(unpack_sources(dir, bundle).entry, "")
        }
        t |> run("a bundle whose active is not among its files is refused") @(tt : T?) {
            var files_obj <- { "a.das" => JV("print(1)"), "b.das" => JV("print(2)") }
            let bundle = write_json(JV((files = JV(files_obj), active = "c.das")))
            tt |> equal(unpack_sources(dir, bundle).entry, "")
        }
        t |> run("a bundle without active is refused") @(tt : T?) {
            var files_obj <- { "a.das" => JV("print(1)"), "b.das" => JV("print(2)") }
            let bundle = write_json(JV((files = JV(files_obj))))
            tt |> equal(unpack_sources(dir, bundle).entry, "")
        }
        t |> run("a file whose content is not text is refused, not emptied") @(tt : T?) {
            // POST /api/samples stores arbitrary text, so a stored bundle can
            // carry a number here. Coercing it to "" would compile an EMPTY
            // file under a hash describing something else, and the user would
            // read the compile error as a fault in code that was discarded.
            var files_obj <- { "main.das" => JV(123), "other.das" => JV("print(1)") }
            let bundle = write_json(JV((files = JV(files_obj), active = "main.das")))
            tt |> equal(unpack_sources(dir, bundle).entry, "")
            tt |> success(!stat(path_join(dir, "main.das")).is_reg ||
                !empty(fread(path_join(dir, "main.das"))), "no empty stand-in was compiled")
        }
    }
}

[test(description = "only the verdict exit is the sample's own failure: success is no fault, and every other status - a shell error's 1, the recipe's, the sandbox's, a failed spawn, the timeout kill - is a build-service fault")]
def test_is_build_fault(t : T?) {
    t |> success(!is_build_fault(0), "success")
    t |> success(!is_build_fault(BUILD_VERDICT_EXIT), "the sandboxed build failed")
    t |> success(is_build_fault(1), "a shell error in the recipe")
    t |> success(is_build_fault(10), "the recipe's own check")
    t |> success(is_build_fault(20), "the sandbox could not run")
    t |> success(is_build_fault(-1), "a failed spawn")
    t |> success(is_build_fault(popen_timed_out), "the timeout kill")
}

[test(description = "a job that fails reports a fault for every cause but the sample's own: a sources write failure and the timeout kill are faults, a refused document and the verdict exit are not, and a recipe exit carries its fault flag")]
def test_job_failure_outcomes(t : T?) {
    t |> run("sources this service could not write") @(tt : T?) {
        let outcome = unpack_failure(UnpackedSources(write_failed = true))
        tt |> success(outcome.fault, "a fault")
        tt |> success(!empty(outcome.error), "with a reason")
    }
    t |> run("a refused document") @(tt : T?) {
        let outcome = unpack_failure(UnpackedSources())
        tt |> success(!outcome.fault, "the sample's own")
        tt |> equal(outcome.error, "malformed source document")
    }
    t |> run("the timeout kill names its budget and keeps the output") @(tt : T?) {
        let outcome = build_failure(
                                    BuildRunResult(fault = true, exit_code = popen_timed_out,
                                                   output = "half a log"), 600)
        tt |> success(outcome.fault, "a fault")
        tt |> equal(outcome.error, "build timed out after 600 seconds\nhalf a log")
    }
    t |> run("the verdict exit carries the compiler's output") @(tt : T?) {
        let outcome = build_failure(
                                    BuildRunResult(exit_code = BUILD_VERDICT_EXIT,
                                                   output = "error[30101]: nope"), 600)
        tt |> success(!outcome.fault, "the sample's own")
        tt |> equal(outcome.error, "error[30101]: nope")
    }
    t |> run("a recipe exit is a fault, and a silent one names its status") @(tt : T?) {
        let outcome = build_failure(BuildRunResult(fault = true, exit_code = 16), 600)
        tt |> success(outcome.fault, "a fault")
        tt |> equal(outcome.error, "build failed (exit code 16)")
    }
}

[test(description = "an empty build command reports itself unconfigured as a fault, a real one has its output captured, and a failing one keeps its exit code and is a fault unless that code is the verdict", timeout = 1200)]
def test_run_build_command(t : T?) {
    t |> run("empty build_cmd reports itself") @(tt : T?) {
        let r = run_build_command("", "s", "o", "module", "main.das", 5.0)
        tt |> success(!r.ok, "not ok")
        tt |> success(find(r.output, "not configured") >= 0, "explains why")
        tt |> success(r.fault, "the builder's own fault")
    }
    t |> run("a real command runs and its output is captured") @(tt : T?) {
        let r = run_build_command("git --version", "s", "o", "module", "main.das", 30.0)
        tt |> success(r.ok, "git --version exits 0")
        tt |> success(find(r.output, "git version") >= 0, "stdout captured")
    }
    t |> run("a failing command reports its exit code") @(tt : T?) {
        let r = run_build_command("git definitely-not-a-subcommand", "s", "o", "module", "main.das",
                                  30.0)
        tt |> success(!r.ok, "nonzero exit")
        tt |> success(r.exit_code != 0, "code preserved")
        tt |> equal(r.fault, r.exit_code != BUILD_VERDICT_EXIT)
    }
}

[test(description = "module mode collects its one artifact with its sha, and the manifest takes the upload shape")]
def test_collect_artifacts(t : T?) {
    with_temp_dir("buildd_out_") $(dir) {
        fwrite(path_join(dir, "sample.wasm"), "wasm bytes")
        t |> run("module mode collects its one artifact with a sha") @(tt : T?) {
            let files <- collect_artifacts(dir, "module")
            tt |> equal(length(files), 1)
            tt |> equal(files[0].name, "sample.wasm")
            tt |> equal(files[0].sha256, sha256_hex("wasm bytes"))
        }
        t |> run("the manifest is the upload shape") @(tt : T?) {
            let files <- collect_artifacts(dir, "module")
            var jerr = ""
            var doc = read_json(manifest_json(files), jerr)
            tt |> success(doc != null && doc.value is _array, "json array")
            tt |> equal((doc.value as _array)[0]?["name"] ?? "", "sample.wasm")
            tt |> equal((doc.value as _array)[0]?["sha256"] ?? "", sha256_hex("wasm bytes"))
        }
    }
}

[test(description = "a build cannot widen what it publishes: smuggled files are dropped, an undefined mode publishes nothing, and page mode publishes its html+js+wasm triple whole or not at all")]
def test_build_cannot_widen_its_own_output(t : T?) {
    // Regression, proven on zen4: a compile-time [init] in the user's own
    // source can fwrite into the output directory. The sandbox stops it
    // READING the box, and this stops what it writes from being published.
    with_temp_dir("buildd_out_") $(dir) {
        fwrite(path_join(dir, "sample.wasm"), "wasm bytes")
        fwrite(path_join(dir, "leak.js"), "// smuggled payload")
        fwrite(path_join(dir, "index.html"), "<script>evil()</script>")
        fwrite(path_join(dir, "sample.o"), "object file")
        t |> run("smuggled servable files are dropped, not uploaded") @(tt : T?) {
            let files <- collect_artifacts(dir, "module")
            tt |> equal(length(files), 1)
            tt |> equal(files[0].name, "sample.wasm")
        }
    }
    with_temp_dir("buildd_out_") $(dir) {
        fwrite(path_join(dir, "leak.js"), "// smuggled payload")
        t |> run("a build producing only smuggled files yields nothing") @(tt : T?) {
            let files <- collect_artifacts(dir, "module")
            tt |> success(empty(files), "no artifacts")
        }
    }
    t |> run("an undefined mode publishes nothing") @(tt : T?) {
        tt |> success(empty(expected_artifact_names("bogus")), "unknown mode")
        tt |> equal(length(expected_artifact_names("module")), 1)
    }
    t |> run("page mode publishes exactly the html+js+wasm triple") @(tt : T?) {
        let names <- expected_artifact_names("page")
        tt |> equal(length(names), 3)
        tt |> equal(names[0], "sample.html")
        tt |> equal(names[1], "sample.js")
        tt |> equal(names[2], "sample.wasm")
    }
    with_temp_dir("buildd_out_") $(dir) {
        fwrite(path_join(dir, "sample.html"), "<html></html>")
        fwrite(path_join(dir, "sample.js"), "// glue")
        fwrite(path_join(dir, "sample.wasm"), "wasm bytes")
        t |> run("page mode collects its triple in order") @(tt : T?) {
            let files <- collect_artifacts(dir, "page")
            tt |> equal(length(files), 3)
            tt |> equal(files[0].name, "sample.html")
            tt |> equal(files[1].name, "sample.js")
            tt |> equal(files[2].name, "sample.wasm")
            tt |> equal(files[2].sha256, sha256_hex("wasm bytes"))
        }
    }
    with_temp_dir("buildd_out_") $(dir) {
        fwrite(path_join(dir, "sample.html"), "<html></html>")
        fwrite(path_join(dir, "sample.wasm"), "wasm bytes")
        t |> run("a partial page set yields nothing — all or nothing") @(tt : T?) {
            let files <- collect_artifacts(dir, "page")
            tt |> success(empty(files), "missing sample.js drops the whole set")
        }
    }
}

[test(description = "the generated page package names the app sample and the job's entry, and declared assets become absolute-src embeds")]
def test_write_page_package(t : T?) {
    with_temp_dir("buildd_pkg_") $(dir) {
        t |> run("the generated package names the app sample and the job's entry") @(tt : T?) {
            let no_assets : array<string>
            tt |> success(write_page_package(dir, "prog.das", no_assets, ""), "written")
            let body = fread(path_join(dir, ".das_package"))
            tt |> success(find(body, "release_name(\"sample\")") >= 0, "app is sample")
            tt |> success(find(body, "release_main(\"prog.das\")") >= 0, "entry carried")
            tt |> success(find(body, "require daslib/daspkg") >= 0, "daspkg hook file")
            tt |> success(find(body, "release_embed_file") < 0, "no embeds without assets")
        }
        t |> run("declared assets become absolute-src embeds at /<path>") @(tt : T?) {
            mkdir_rec(path_join(dir, "wt/tutorials/_assets/gltf"))
            fwrite(path_join(dir, "wt/tutorials/_assets/gltf/BoomBox.glb"), "glb bytes")
            let wt = replace(path_join(dir, "wt"), "\\", "/")
            let assets <- [ "tutorials/_assets/gltf/BoomBox.glb" ]
            tt |> success(write_page_package(dir, "prog.das", assets, wt), "written")
            let body = fread(path_join(dir, ".das_package"))
            tt |> success(
                find(body, "release_embed_file(\"{wt}/tutorials/_assets/gltf/BoomBox.glb\", \"/tutorials/_assets/gltf/BoomBox.glb\")") >= 0,
                "embed line carries absolute src and MEMFS dst")
            // embedded files live in the data segment: the package must widen INITIAL_MEMORY
            tt |> success(find(body, "release_emcc_arg(\"-sINITIAL_MEMORY=") >= 0,
                          "initial memory covers embeds")
        }
    }
}

[test(description = "wasm-assets lines parse and nothing else does, asset paths must sit under the allowlist with plain components, and a bad one fails closed naming it")]
def test_declared_assets(t : T?) {
    t |> run("wasm-assets lines parse; everything else is ignored") @(tt : T?) {
        let src = ("options gen2\n" +
            "// wasm-assets: tutorials/_assets/gltf/BoomBox.glb\n" +
            "  // wasm-assets:   tutorials/_assets/hdri/cannon_2k.hdr  \n" +
            "// wasm-assets:\n" +
            "// not-assets: tutorials/_assets/x.bin\n" +
            "require math\n")
        let got <- collect_declared_assets(src)
        tt |> equal(length(got), 2)
        tt |> equal(got[0], "tutorials/_assets/gltf/BoomBox.glb")
        tt |> equal(got[1], "tutorials/_assets/hdri/cannon_2k.hdr")
    }
    t |> run("asset paths are shape-checked: allowlist prefix, boring components") @(tt : T?) {
        tt |> success(is_valid_asset_path("tutorials/_assets/cat/textures/concrete_cat_statue_diff_2k.jpg"), "real shape")
        tt |> success(!is_valid_asset_path("/etc/passwd"), "absolute")
        tt |> success(!is_valid_asset_path("tutorials/_assets/../../secrets.txt"), "traversal")
        tt |> success(!is_valid_asset_path("examples/hello.das"), "outside the allowlist")
        tt |> success(!is_valid_asset_path("tutorials/_assets/a\\b.bin"), "backslash")
        tt |> success(!is_valid_asset_path("tutorials/_assets//x.bin"), "empty component")
        tt |> success(!is_valid_asset_path("tutorials/_assets/"), "no file")
    }
    t |> run("validation fails closed with the offending path in the message") @(tt : T?) {
        with_temp_dir("buildd_assets_") $(wt) {
            mkdir_rec(path_join(wt, "tutorials/_assets/gltf"))
            fwrite(path_join(wt, "tutorials/_assets/gltf/BoomBox.glb"), "glb bytes")
            let ok_assets <- [ "tutorials/_assets/gltf/BoomBox.glb" ]
            var err = ""
            tt |> success(validate_asset_paths(ok_assets, wt, err), "existing asset passes")
            let missing <- [ "tutorials/_assets/gltf/NoSuch.glb" ]
            tt |> success(!validate_asset_paths(missing, wt, err), "missing asset refuses")
            tt |> success(find(err, "NoSuch.glb") >= 0, "message names the path")
            let bad <- [ "tutorials/_assets/../x" ]
            tt |> success(!validate_asset_paths(bad, wt, err), "bad shape refuses")
            remove(path_join(wt, "tutorials/_assets/gltf/BoomBox.glb"))
        }
    }
}

[test(description = "a job hash must be exactly 64 lowercase hex characters and a toolchain id 7 to 64")]
def test_job_identity_validation(t : T?) {
    // The builder assembles a scratch path from the hash the SERVER sent, so
    // its shape is checked on arrival — "the server said so" is not validation.
    let good = "1111111111111111111111111111111111111111111111111111111111111111"
    t |> run("hashes are exactly 64 lowercase hex") @(tt : T?) {
        tt |> success(is_valid_job_hash(good), "64 hex")
        tt |> success(!is_valid_job_hash(slice(good, 0, 63)), "too short")
        tt |> success(!is_valid_job_hash("ABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABAB"), "uppercase")
        tt |> success(!is_valid_job_hash("../../../etc"), "traversal")
        tt |> success(!is_valid_job_hash(""), "empty")
    }
    t |> run("toolchains are 7..64 lowercase hex") @(tt : T?) {
        tt |> success(is_valid_job_toolchain("abc1234"), "short sha")
        tt |> success(!is_valid_job_toolchain("abc123"), "too short")
        tt |> success(!is_valid_job_toolchain(".."), "traversal")
    }
}

[test(description = "an artifact that is a symlink to a host file is refused, so nothing is uploaded", timeout = 1200)]
def test_symlinked_artifact_is_refused(t : T?) {
    // A build writes its output directory with the user's own compile-time
    // code. Everything downstream reads the artifact path on the HOST, outside
    // the sandbox — so a symlink there would publish a host file. Refuse it.
    with_temp_dir("buildd_link_") $(dir) {
        var terr = ""
        let secret = path_join(dir, "secret.txt")
        fwrite(secret, "host-only content")
        let out_dir = path_join(dir, "out")
        mkdir_rec(out_dir)
        let link = path_join(out_dir, "sample.wasm")
        // daslang binds no symlink CREATION, so make one the POSIX way.
        let rc = unsafe(popen_argv(["ln", "-s", secret, link], 30.0) $(f) {
            if (f != null) {
                terr = fread(f)
            }
        })
        // Gate on what actually landed, not on the exit code: on Windows `ln`
        // reports success while COPYING the file, so an exit-code-only guard
        // would assert against a premise that is false there. The refusal is
        // still compiled everywhere and exercised wherever a real link is made.
        var link_err = ""
        if (rc != 0 || !is_symlink(link, link_err)) {
            print("skipping symlink case: this platform did not create a symlink\n")
            return
        }
        t |> run("a symlinked artifact yields nothing to upload") @(tt : T?) {
            let files <- collect_artifacts(out_dir, "module")
            tt |> success(empty(files), "refused, so nothing is published")
        }
    }
}

[test(description = "a git checkout derives its 40-hex HEAD sha as the toolchain id and a non-repo directory derives nothing", timeout = 1200)]
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
