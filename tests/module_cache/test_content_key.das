options gen2
options indenting = 4

require dastest/testing_boost public

require strings
require daslib/fio
require _mc_common


def fixture(name : string) : string =>
    path_join(get_das_root(), "tests/module_cache/_fixtures/{name}")


def with_fixtures(t : T?; names : array<string>;
                  blk : block<(dir : string) : void>) {
    let args <- get_command_line_arguments()
    if (find_index(args, "--use-aot") >= 0) {
        t |> skip("an AOT test host cannot run child scripts")
        return
    }
    var error : string
    let dir = create_temp_directory("das_mc_key", error)
    if (empty(dir)) {
        t |> failure("create_temp_directory: {error}")
        return
    }
    var copied = true
    for (name in names) {
        copied = fwrite(path_join(dir, name), fread(fixture(name))) && copied
    }
    t |> success(copied, "the fixtures are copied")
    if (copied) {
        invoke(blk, dir)
    }
    rmdir_rec(dir, error)
}


def stage_same_stamp_edit(path : string) : bool {
    let text = fread(path)
    let edit = replace(text, "1111", "2222")
    var attempts = 0
    while (attempts < 10) {
        attempts++
        if (fwrite(path, text) && fwrite("{path}.edit", edit)
                && (int64(stat(path).mtime)
                    == int64(stat("{path}.edit").mtime))) {
            return length(edit) == length(text)
        }
    }
    return false
}


def apply_edit(t : T?; path : string) {
    let before = stat(path)
    remove(path)
    t |> success(rename("{path}.edit", path), "the edit moves into place")
    let after = stat(path)
    t |> success(int64(after.mtime) == int64(before.mtime)
            && after.size == before.size,
        "the edited file keeps its stamp")
}


def host_argv(dir, script : string) : array<string> {
    return <- [das_exe(), "-dasroot", get_das_root(), "-module-cache",
        path_join(dir, "mc.bin"), path_join(dir, script)]
}


def run_child(t : T?; phase : string; argv : array<string>;
              markers : array<string>) {
    var out : string
    let rc = run_argv(argv, out)
    for (marker in markers) {
        t |> success(report_child(t, phase, rc, out, marker),
            "{phase} run prints '{marker}'")
    }
}


[test(description = "A same-stamp edit to a module is caught by content and the warm run serves the new text", timeout = 1200)]
def test_module_edit_inside_its_stamp(t : T?) {
    with_fixtures(t, ["mc_key_mod.das", "mc_key_drv.das"]) $(dir) {
        let mod = path_join(dir, "mc_key_mod.das")
        t |> success(stage_same_stamp_edit(mod), "a same-stamp edit is staged")
        let argv <- host_argv(dir, "mc_key_drv.das")
        run_child(t, "cold", argv, ["key=1111", "ser: wrote"])
        apply_edit(t, mod)
        run_child(t, "warm", argv, ["key=2222"])
    }
}


[test(description = "A same-stamp edit to an included file is caught by content and the warm run serves the new text", timeout = 1200)]
def test_included_file_edit_inside_its_stamp(t : T?) {
    with_fixtures(t, ["mc_key_body.inc", "mc_key_inc_drv.das"]) $(dir) {
        let body = path_join(dir, "mc_key_body.inc")
        t |> success(stage_same_stamp_edit(body),
            "a same-stamp edit is staged")
        let argv <- host_argv(dir, "mc_key_inc_drv.das")
        run_child(t, "cold", argv, ["key=1111", "ser: wrote"])
        run_child(t, "unchanged", argv,
            ["key=1111", "deser: clean"])
        apply_edit(t, body)
        run_child(t, "warm", argv, ["key=2222"])
    }
}

[test(description = "The included files' stamps survive a cache writeback, so the next run still serves clean", timeout = 1200)]
def test_included_stamps_survive_writeback(t : T?) {
    with_fixtures(t, ["mc_key_body.inc", "mc_key_inc_mod.das",
            "mc_key_inc_mod_drv.das"]) $(dir) {
        let body = path_join(dir, "mc_key_body.inc")
        let driver = path_join(dir, "mc_key_inc_mod_drv.das")
        t |> success(stage_same_stamp_edit(body), "an include edit is staged")
        t |> success(stage_same_stamp_edit(driver), "a driver edit is staged")
        let argv <- host_argv(dir, "mc_key_inc_mod_drv.das")
        run_child(t, "cold", argv, ["key=1111", "driver=1111", "ser: wrote"])
        apply_edit(t, driver)
        run_child(t, "rewrite", argv, ["key=1111", "driver=2222",
            "2 module(s) served from cache", "ser: wrote"])
        apply_edit(t, body)
        run_child(t, "edited include", argv, ["key=2222", "driver=2222",
            "ser: included source changed '{body}'\n",
            "(included source '{body}' changed)\n", "ser: wrote"])
        run_child(t, "rewritten cache", argv, ["key=2222", "deser: clean"])
    }
}


[test(description = "A driver with 4095 included files round-trips through the cache, and one more include makes the read fail and fall back to a rewrite", timeout = 1200)]
def test_included_source_count_bound(t : T?) {
    with_fixtures(t, ["mc_key_body.inc", "mc_key_inc_drv.das"]) $(dir) {
        let driver = path_join(dir, "mc_key_inc_drv.das")
        let original = fread(driver)
        var created = true
        let prefix = build_string() $(var writer) {
            for (i in range(4095)) {
                let name = "empty_{i}.inc"
                created = fwrite(path_join(dir, name), "") && created
                write(writer, "include {name}\n")
            }
        }
        t |> success(created, "the empty includes are written")
        if (!created) return
        let boundary = replace(original, "include mc_key_body.inc",
            prefix + "include mc_key_body.inc")
        t |> success(fwrite(driver, boundary), "4096 includes are written")
        let argv <- host_argv(dir, "mc_key_inc_drv.das")
        run_child(t, "boundary cold", argv, ["key=1111", "ser: wrote"])
        run_child(t, "boundary warm", argv, ["key=1111", "deser: clean"])
        t |> success(fwrite(path_join(dir, "extra.inc"), ""))
        t |> success(fwrite(driver, replace(boundary,
            "include mc_key_body.inc",
            "include extra.inc\ninclude mc_key_body.inc")))
        t |> success(remove(path_join(dir, "mc.bin")))
        run_child(t, "oversized cold", argv, ["key=1111", "ser: wrote"])
        run_child(t, "oversized warm", argv, ["key=1111",
            "ser: read failed '{driver}'", "deser: FALLBACK", "ser: wrote"])
    }
}


[test(description = "A cold run still writes its record when the shared temporary name is already taken, so the writer uses a private temporary", timeout = 1200)]
def test_writer_temp_is_private(t : T?) {
    with_fixtures(t, ["mc_key_mod.das", "mc_key_drv.das"]) $(dir) {
        t |> success(mkdir(path_join(dir, "mc.bin.tmp")),
            "the shared temp name is taken")
        run_child(t, "cold", host_argv(dir, "mc_key_drv.das"),
            ["key=1111", "ser: wrote"])
    }
}
