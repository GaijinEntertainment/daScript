options gen2

require dastest/testing_boost public
require bench_suite
require daslib/fio
require strings

def private with_temp_dir(prefix : string; blk : block<(dir : string) : void>) {
    var err = ""
    let dir = create_temp_directory(prefix, err)
    verify(!empty(dir), "temp dir created")
    invoke(blk, dir)
    rmdir_rec(dir)
}

def private touch(dir, rel : string) {
    let p = path_join(dir, rel)
    mkdir_rec(dir_name(p))
    fwrite(p, "options gen2\n")
}

let private GOOD_CONFIG = "\{ \"root\": \"benchmarks\", \"exclude\": [\"**/tests/**\", \"**/_*.das\"], \"lanes\": [\"interp\"], \"repeat\": 2, \"timeout_seconds\": 30, \"memory_limit_mb\": 512, \"baseline_runs\": 5, \"files\": \{ \"core/slow.das\": \{ \"skip\": \"too slow\" \}, \"core/big.das\": \{ \"timeout_seconds\": 60, \"memory_limit_mb\": 2048 \} \} \}"

[test]
def test_load_suite_config(t : T?) {
    with_temp_dir("bench_stand_") $(dir) {
        t |> run("a well-formed config parses with its overrides") @(tt : T?) {
            let path = path_join(dir, "suite.json")
            fwrite(path, GOOD_CONFIG)
            var error = ""
            let cfg <- load_suite_config(path, error)
            tt |> equal(error, "")
            tt |> equal(cfg.repeat, 2)
            tt |> equal(cfg.timeout_seconds, 30)
            tt |> equal(length(cfg.exclude), 2)
            tt |> equal(length(cfg.lanes), 1)
            cfg.files |> get("core/slow.das") $(ov) {
                tt |> equal(ov.skip, "too slow")
            }
            cfg.files |> get("core/big.das") $(ov) {
                tt |> equal(ov.timeout_seconds, 60)
            }
            tt |> equal(length(cfg.files), 2)
            tt |> success(cfg.regression_threshold > 0.09lf && cfg.regression_threshold < 0.11lf, "threshold keeps its default")
        }
        t |> run("a missing config fails closed naming the path") @(tt : T?) {
            var error = ""
            load_suite_config(path_join(dir, "missing.json"), error)
            tt |> success(find(error, "missing.json") >= 0, "error names the path")
        }
        t |> run("an unparseable config fails closed") @(tt : T?) {
            let path = path_join(dir, "bad.json")
            fwrite(path, "not json")
            var error = ""
            load_suite_config(path, error)
            tt |> success(find(error, "parse") >= 0, "error says it did not parse")
        }
        t |> run("no lanes, a zero repeat, or an unknown lane are errors") @(tt : T?) {
            let path = path_join(dir, "lanes.json")
            fwrite(path, "\{ \"lanes\": [] \}")
            var error = ""
            load_suite_config(path, error)
            tt |> success(find(error, "no lanes") >= 0, "empty lanes rejected")
            fwrite(path, "\{ \"lanes\": [\"interp\"], \"repeat\": 0 \}")
            load_suite_config(path, error)
            tt |> success(find(error, "positive") >= 0, "zero repeat rejected")
            fwrite(path, "\{ \"lanes\": [\"vulkan\"] \}")
            load_suite_config(path, error)
            tt |> success(find(error, "vulkan") >= 0, "unknown lane named")
            fwrite(path, "\{ \"lanes\": [\"interp\", \"jit\", \"aot\"] \}")
            load_suite_config(path, error)
            tt |> equal(error, "", "the three real lanes are accepted")
            fwrite(path, "\{ \"lanes\": [\"interp\"], \"baseline_runs\": 2 \}")
            load_suite_config(path, error)
            tt |> success(find(error, "baseline_runs") >= 0, "a baseline of two runs has no median worth gating on")
        }
    }
}

def private load_cfg(dir : string) : SuiteConfig {
    var error = ""
    return <- load_suite_config(path_join(dir, "suite.json"), error)
}

[test]
def test_discover_files(t : T?) {
    with_temp_dir("bench_stand_") $(dir) {
        fwrite(path_join(dir, "suite.json"), GOOD_CONFIG)
        touch(dir, "benchmarks/core/hash/test02.das")
        touch(dir, "benchmarks/core/hash/_common.das")
        touch(dir, "benchmarks/core/slow.das")
        touch(dir, "benchmarks/core/big.das")
        touch(dir, "benchmarks/sql/tests/test_update.das")
        touch(dir, "benchmarks/sort/sort.das")
        touch(dir, "benchmarks/sort/README.md")
        t |> run("helpers, test dirs and non-das files are excluded; the rest is sorted") @(tt : T?) {
            let cfg <- load_cfg(dir)
            var err = ""
            let files <- discover_files(dir, cfg, err)
            tt |> equal(err, "")
            tt |> equal(length(files), 4)
            tt |> equal(files[0].path, "core/big.das")
            tt |> equal(files[1].path, "core/hash/test02.das")
            tt |> equal(files[2].path, "core/slow.das")
            tt |> equal(files[3].path, "sort/sort.das")
        }
        t |> run("group and id derive from the path") @(tt : T?) {
            let cfg <- load_cfg(dir)
            var err = ""
            let files <- discover_files(dir, cfg, err)
            tt |> equal(files[1].group, "core/hash")
            tt |> equal(files[1].id, "core/hash/test02")
            tt |> equal(files[3].group, "sort")
            tt |> equal(group_of("top.das"), "root")
            tt |> equal(stem_path("top.das"), "top")
        }
        t |> run("overrides apply and defaults fill the rest") @(tt : T?) {
            let cfg <- load_cfg(dir)
            var err = ""
            let files <- discover_files(dir, cfg, err)
            tt |> equal(files[2].skip, "too slow")
            tt |> equal(files[2].timeout_seconds, 30)
            tt |> equal(files[0].timeout_seconds, 60)
            tt |> equal(files[0].memory_limit_mb, 2048)
            tt |> equal(files[1].memory_limit_mb, 512)
            tt |> equal(files[1].skip, "")
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
