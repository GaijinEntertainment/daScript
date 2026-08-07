options gen2

require dastest/testing_boost public
require verify_core
require daslib/fio
require strings

def private with_temp_dir(prefix : string; blk : block<(dir : string) : void>) {
    var err = ""
    let dir = create_temp_directory(prefix, err)
    assert(!empty(dir), "temp dir created")
    invoke(blk, dir)
    rmdir_rec(dir)
}

def private write_manifest(dir : string; text : string) : string {
    let path = path_join(dir, "data.json")
    fwrite(path, text)
    return path
}

[test]
def test_load_manifest(t : T?) {
    with_temp_dir("dasweb_verify_") $(dir) {
        t |> run("a well-formed manifest parses into entries") @(tt : T?) {
            var m = SamplesManifest()
            m.examples |> emplace(SampleEntry(name = "Hello", files <- ["examples/hello.das"]))
            m.examples |> emplace(SampleEntry(name = "Game", files <- ["examples/game/main.das", "examples/game/stub.das"]))
            let path = write_manifest(dir, sprint_json(m, true))
            var error = ""
            let samples <- load_manifest(path, error)
            tt |> equal(error, "")
            tt |> equal(length(samples), 2)
            tt |> equal(samples[0].name, "Hello")
            tt |> equal(samples[0].files[0], "examples/hello.das")
            tt |> equal(length(samples[1].files), 2)
        }
        t |> run("unknown keys like slug are ignored, as the shipped manifest carries them") @(tt : T?) {
            let path = write_manifest(dir,
                "\{ \"examples\" : [ \{ \"name\" : \"Hello\", \"slug\" : \"h\", \"files\" : [\"examples/hello.das\"] \} ] \}")
            var error = ""
            let samples <- load_manifest(path, error)
            tt |> equal(error, "")
            tt |> equal(length(samples), 1)
            tt |> equal(samples[0].files[0], "examples/hello.das")
        }
        t |> run("a missing manifest fails closed naming the path") @(tt : T?) {
            var error = ""
            let samples <- load_manifest(path_join(dir, "missing.json"), error)
            tt |> success(empty(samples), "no entries")
            tt |> success(find(error, "missing.json") >= 0, "error names the path")
        }
        t |> run("an unparseable manifest fails closed") @(tt : T?) {
            let path = write_manifest(dir, "not json at all")
            var error = ""
            let samples <- load_manifest(path, error)
            tt |> success(empty(samples), "no entries")
            tt |> success(!empty(error), "error is set")
        }
        t |> run("a sample without files fails closed naming the sample") @(tt : T?) {
            var m = SamplesManifest()
            m.examples |> emplace(SampleEntry(name = "Broken"))
            let path = write_manifest(dir, sprint_json(m, false))
            var error = ""
            let samples <- load_manifest(path, error)
            tt |> success(empty(samples), "no entries")
            tt |> success(find(error, "Broken") >= 0, "error names the sample")
        }
    }
}

[test]
def test_check_sample(t : T?) {
    with_temp_dir("dasweb_verify_") $(dir) {
        t |> run("a compiling sample passes") @(tt : T?) {
            fwrite(path_join(dir, "ok.das"), "options gen2\n[export]\ndef main() \{\n    to_log(LOG_INFO, \"ok\\n\")\n\}\n")
            let result = check_sample(dir, dir, SampleEntry(name = "OK", files <- ["ok.das"]))
            tt |> success(result.ok, "compiles")
            tt |> equal(result.entry, "ok.das")
        }
        t |> run("a sample requiring a same-directory sibling passes") @(tt : T?) {
            fwrite(path_join(dir, "sibling.das"), "options gen2\ndef helper() : int => 42\n")
            fwrite(path_join(dir, "entry.das"), "options gen2\nrequire sibling\n[export]\ndef main() \{\n    to_log(LOG_INFO, \"\{helper()\}\\n\")\n\}\n")
            let result = check_sample(dir, dir, SampleEntry(name = "Multi", files <- ["entry.das", "sibling.das"]))
            tt |> success(result.ok, "compiles through the bare-name require")
        }
        t |> run("a broken sample fails with the compiler's message") @(tt : T?) {
            fwrite(path_join(dir, "bad.das"), "options gen2\n[export]\ndef main() \{\n    let x = no_such_symbol_anywhere()\n\}\n")
            let result = check_sample(dir, dir, SampleEntry(name = "Bad", files <- ["bad.das"]))
            tt |> success(!result.ok, "fails")
            tt |> success(find(result.message, "compilation failed") >= 0, "message says why")
        }
        t |> run("a missing listed file fails before compiling, naming the file") @(tt : T?) {
            fwrite(path_join(dir, "present.das"), "options gen2\n[export]\ndef main() \{\n    pass\n\}\n")
            let result = check_sample(dir, dir, SampleEntry(name = "Rot", files <- ["present.das", "gone.das"]))
            tt |> success(!result.ok, "fails")
            tt |> success(find(result.message, "gone.das") >= 0, "message names the file")
        }
    }
}

[test]
def test_generated_samples(t : T?) {
    with_temp_dir("dasweb_verify_") $(samples) {
        with_temp_dir("dasweb_verify_repo_") $(repo) {
            t |> run("a stage-time-generated sample compiles from its canonical source") @(tt : T?) {
                var err = ""
                mkdir_rec(path_join(repo, "examples/graphics"), err)
                fwrite(path_join(repo, "examples/graphics/furier_opengl_imgui_example.das"),
                    "options gen2\n[export]\ndef main() \{\n    pass\n\}\n")
                let result = check_sample(samples, repo, SampleEntry(name = "Fourier", files <- ["examples/furier.das"]))
                tt |> success(result.ok, "compiles via the canonical mapping")
            }
            t |> run("a generated sample whose canonical source is gone fails naming both paths") @(tt : T?) {
                let result = check_sample(samples, repo, SampleEntry(name = "Phys", files <- ["examples/physarum_lab.das"]))
                tt |> success(!result.ok, "fails")
                tt |> success(find(result.message, "physarum_lab.das") >= 0, "names the manifest file")
                tt |> success(find(result.message, "physarum_lab_opengl_imgui_example.das") >= 0, "names the canonical source")
            }
        }
    }
}
