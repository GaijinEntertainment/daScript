options gen2

require dastest/testing_boost public
require ../bench_suite.das
require _test_common
require daslib/fio
require daslib/json_boost
require daslib/strings_boost
require strings

def private touch(dir, rel : string) {
    let p = path_join(dir, rel)
    mkdir_rec(dir_name(p))
    fwrite(p, "options gen2\n")
}

def private good_config() : string {
    let files <- { "core/slow.das" => JV((skip = "too slow")), "core/big.das" => JV((timeout_seconds = 60)) }
    let globs <- { "sort/**" => "not here" }
    return write_json(JV((root = "benchmarks", exclude = JV(["**/tests/**", "**/_*.das"]), lanes = JV(["interp"]),
        repeat = 2, timeout_seconds = 30, lane_excludes = JV((interp = JV(globs))), files = JV(files))))
}

def private load_cfg(dir : string) : SuiteConfig {
    var error = ""
    return <- load_suite_config(path_join(dir, "suite.json"), error)
}

[test]
def test_load_suite_config(t : T?) {
    with_temp_dir("bench_stand_") $(dir) {
        t |> run("a well-formed config parses with its overrides") @(tt : T?) {
            fwrite(path_join(dir, "suite.json"), good_config())
            var error = ""
            let cfg <- load_suite_config(path_join(dir, "suite.json"), error)
            tt |> equal(error, "")
            tt |> equal(cfg.repeat, 2)
            tt |> equal(cfg.timeout_seconds, 30)
            tt |> equal(length(cfg.exclude), 2)
            tt |> equal(length(cfg.files), 2)
            cfg.files |> get("core/big.das") $(ov) {
                tt |> equal(ov.timeout_seconds, 60)
            }
        }
        t |> run("a missing, unparseable or out-of-range config fails closed, naming what is wrong") @(tt : T?) {
            var error = ""
            load_suite_config(path_join(dir, "missing.json"), error)
            tt |> success(find(error, "missing.json") >= 0, "error names the path")
            let path = path_join(dir, "bad.json")
            var bodies <- [
                "not a config at all",
                write_json(JV((lanes = JV(array<string>())))),
                write_json(JV((lanes = JV(["interp"]), repeat = 0))),
                write_json(JV((lanes = JV(["vulkan"])))),
                write_json(JV((lanes = JV(["interp"]), lane_excludes = JV((jit = JV(table<string; string>()))))))
            ]
            for (body, word in bodies, ["parse", "no lanes", "positive", "vulkan", "does not run"]) {
                fwrite(path, body)
                load_suite_config(path, error)
                tt |> success(find(error, word) >= 0, "{body} -> {error}")
            }
            fwrite(path, write_json(JV((lanes = JV(["interp", "jit"])))))
            load_suite_config(path, error)
            tt |> equal(error, "", "the two real lanes are accepted")
        }
    }
}

[test]
def test_discover_files(t : T?) {
    with_temp_dir("bench_stand_") $(dir) {
        fwrite(path_join(dir, "suite.json"), good_config())
        for (rel in ["core/hash/test02.das", "core/hash/_common.das", "core/slow.das", "core/big.das", "sql/tests/test_update.das", "sort/sort.das", "sort/README.md"]) {
            touch(dir, "benchmarks/{rel}")
        }
        t |> run("helpers, test dirs and non-das files are excluded; the rest is sorted, with identity from the path and overrides applied") @(tt : T?) {
            var err = ""
            let files <- discover_files(dir, load_cfg(dir), err)
            tt |> equal(err, "")
            tt |> equal(join([for (f in files); f.path], " "), "core/big.das core/hash/test02.das core/slow.das sort/sort.das")
            tt |> equal(files[1].group, "core/hash")
            tt |> equal(files[1].id, "core/hash/test02")
            tt |> equal(group_of("top.das"), "root")
            tt |> equal(stem_path("top.das"), "top")
            tt |> equal(files[2].skip, "too slow")
            tt |> equal(files[2].timeout_seconds, 30)
            tt |> equal(files[0].timeout_seconds, 60)
            tt |> equal(files[1].skip, "")
            tt |> equal(skip_reason(files[3], "interp"), "not here", "a lane exclude reaches the file it matches")
            tt |> equal(skip_reason(files[1], "interp"), "")
        }
        t |> run("an override naming a missing file is reported") @(tt : T?) {
            let cfg <- load_cfg(dir)
            remove(path_join(dir, "benchmarks/core/slow.das"))
            var err = ""
            let files <- discover_files(dir, cfg, err)
            tt |> equal(length(files), 3)
            tt |> success(find(err, "core/slow.das") >= 0, "error names the stale override")
        }
    }
}
