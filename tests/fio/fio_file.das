options gen2

require dastest/testing_boost public
require daslib/faker
require daslib/fuzzer
require daslib/strings_boost
require daslib/fio
require daslib/defer

let TOTAL_FUZZ_FILES = 100

def operator ==(a, b : array<uint8> implicit) {
    if (length(a) != length(b)) {
        return false
    }
    for (A, B in a, b) {
        if (A != B) {
            return false
        }
    }
    return true
}

[test(description = "fwrite and fread of an empty array are 0-byte no-ops and not out-of-bounds addresses")]
def test_empty_array_io(t : T?) {
    t |> run("fwrite/fread of an empty array is a 0-byte no-op, not an OOB addr") @(t : T?) {
        let fname = path_join(test_temp_dir(), "empty_array.bin")
        var none : array<int>
        var wrote = false
        fopen(fname, "wb") $(f) {
            if (f != null) {
                t |> equal(fwrite(f, none), 0)
                t |> equal(long_fwrite(f, none), 0l)
                wrote = true
            }
        }
        t |> success(wrote, "opened for write")
        var read_back = false
        fopen(fname, "rb") $(f) {
            if (f != null) {
                t |> equal(fread(f, none), 0)
                t |> equal(long_fread(f, none), 0l)
                read_back = true
            }
        }
        t |> success(read_back, "opened for read")
        t |> success(remove(fname))
        delete none
    }
}

[test(description = "stat reports the execute bit: a data file has none and an executable has it")]
def test_stat_is_exec(t : T?) {
    t |> run("stat reports the execute bit: a data file has none, an executable has it") @(t : T?) {
        let dir = test_temp_dir()
        let plain = path_join(dir, "exec_probe.txt")
        t |> success(fwrite(plain, "x"), "the probe file is written")
        let st_plain = stat(plain)
        t |> success(st_plain.is_valid && st_plain.is_reg && !st_plain.is_exec, "a data file is not executable")
        if (get_platform_name() == "windows") {
            let exe = path_join(dir, "exec_probe.exe")
            t |> success(fwrite(exe, "x"), "the .exe probe is written")
            t |> success(stat(exe).is_exec, "Windows: the .exe extension is the execute bit")
            t |> success(remove(exe))
        } else {
            var out = ""
            t |> equal(0, run_and_capture(["chmod", "755", plain], out, 10.0), "chmod runs")
            t |> success(stat(plain).is_exec, "chmod 755 sets the bit stat reports")
            t |> equal(0, run_and_capture(["chmod", "610", plain], out, 10.0), "chmod runs")
            t |> success(stat(plain).is_exec, "a group-only execute bit counts too")
            t |> equal(0, run_and_capture(["chmod", "600", plain], out, 10.0), "chmod runs")
            t |> success(!stat(plain).is_exec, "and no execute bit at all reads false")
            t |> success(stat(dir).is_exec && stat(dir).is_dir, "a directory carries the bit, so a program test pairs it with is_reg")
        }
        t |> success(remove(plain))
    }
}

[test(description = "fopen, fread and fmap survive fuzzed arguments without crashing")]
def test_fopen(t : T?) {
    t |> run("fuzz fopen") @@(t : T?) {
        var fake <- Faker()
        fuzz(TOTAL_FUZZ_FILES) {
            let fname = path_join(test_temp_dir(), fake |> any_file_name)
            var stat : FStat
            if (!stat(fname, stat) && !stat.is_valid) {
                var f = fopen(fname, "w")
                if (f != null) {
                    fclose(f)
                    var written = false
                    fopen(fname, "a") $(ff) {
                        fwrite(ff, "test")
                        written = true
                    }
                    t |> success(written)
                    t |> success(remove(fname))
                }
            }
        }
    }

    t |> run("fuzz fread") @@(t : T?) {
        var fake <- Faker()
        fuzz(TOTAL_FUZZ_FILES) {
            let fname = path_join(test_temp_dir(), fake |> any_file_name)
            var stat : FStat
            if (!stat(fname, stat) && !stat.is_valid) {
                fopen(fname, "wb") $(f) {
                    fwrite(f, "test")
                }
                fopen(fname, "rb") $(f) {
                    t |> equal(fread(f), "test")
                }
                t |> success(remove(fname))
            }
        }
    }

    t |> run("fuzz fmap") @@(t : T?) {
        var fake <- Faker()
        fuzz(TOTAL_FUZZ_FILES) {
            let fname = path_join(test_temp_dir(), fake |> any_file_name)
            var stat : FStat
            if (!stat(fname, stat) && !stat.is_valid) {
                fopen(fname, "wb") $(f) {
                    fwrite(f, "test")
                }
                fopen(fname, "rb") $(f) {
                    fmap(f) $(arr1) {
                        peek_data("test") $(arr2) {
                            t |> equal(arr1, arr2)
                        }
                    }
                }
                t |> success(remove(fname))
            }
        }
    }
}

[test(description = "stat and fstat agree on the mode and type of a directory and an open file")]
def stat_mode_matches_open_file_and_type(t : T?) {
    let made = create_temp_directory_result("stat-mode-")
    verify(made is value)
    let root = made as value
    {
        var directory : FStat
        t |> success(stat(root, directory))
        t |> success(directory.is_dir)
        t |> success(directory.mode != 0u)
        fopen("{root}/file", "wb") $(file) {
            verify(file != null)
            var opened, named : FStat
            fstat(file, opened)
            t |> success(stat("{root}/file", named))
            t |> success(opened.is_reg && named.is_reg)
            t |> equal(opened.mode, named.mode)
            t |> success(opened.mode != directory.mode, "file type bits are retained")
        }
    } finally { rmdir_rec(root) }
}

def private scratch(name : string) : string {
    var terr : string
    let tmp = temp_directory(terr)
    return path_join(empty(tmp) ? "." : tmp, "{name}_{ref_time_ticks()}.txt")
}

[test(description = "fwrite_if_changed leaves a file and its timestamp alone when the text is identical")]
def test_identical_text_is_not_written(t : T?) {
    let path = scratch("fio_wic_same")
    defer() {
        var err : string
        remove(path, err)
    }
    t |> success(fwrite_if_changed(path, "one\n"), "a file that does not exist yet is written")
    let stamp = stat(path).mtime
    t |> success(!fwrite_if_changed(path, "one\n"), "the same text is not written again")
    t |> equal(stat(path).mtime, stamp, "so the timestamp does not move")
    t |> equal(fread(path), "one\n", "and the text is still there")
}

[test(description = "fwrite_if_changed replaces a file whose text differs")]
def test_different_text_is_written(t : T?) {
    let path = scratch("fio_wic_diff")
    defer() {
        var err : string
        remove(path, err)
    }
    fwrite(path, "one\n")
    t |> success(fwrite_if_changed(path, "two\n"), "different text is written")
    t |> equal(fread(path), "two\n", "and it replaces what was there")
}
