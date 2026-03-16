options gen2
require dastest/testing_boost public

require fio

// ---- path manipulation (correctness, no error reporting) ----

[test]
def test_extension(t : T?) {
    t |> run("returns extension with dot") @(t : T?) {
        t |> equal(extension("foo.txt"), ".txt")
    }
    t |> run("returns null for no extension") @(t : T?) {
        t |> success(empty(extension("foo")))
    }
    t |> run("handles double extension") @(t : T?) {
        t |> equal(extension("archive.tar.gz"), ".gz")
    }
}

[test]
def test_stem(t : T?) {
    t |> run("returns stem") @(t : T?) {
        t |> equal(stem("foo.txt"), "foo")
    }
    t |> run("returns stem without extension") @(t : T?) {
        t |> equal(stem("bar"), "bar")
    }
}

[test]
def test_replace_extension(t : T?) {
    t |> run("replaces extension") @(t : T?) {
        t |> equal(replace_extension("foo.txt", ".md"), "foo.md")
    }
    t |> run("adds extension to file without one") @(t : T?) {
        t |> equal(replace_extension("foo", ".md"), "foo.md")
    }
}

[test]
def test_path_join(t : T?) {
    t |> run("joins two components") @(t : T?) {
        let joined = path_join("a", "b")
        // Windows uses backslash, Unix uses forward slash
        t |> success(joined == "a/b" || joined == "a\\b", "expected a/b or a\\b, got {joined}")
    }
}

[test]
def test_normalize(t : T?) {
    t |> run("normalizes dot-dot") @(t : T?) {
        let n = normalize("a/b/../c")
        t |> success(n == "a/c" || n == "a\\c", "expected a/c or a\\c, got {n}")
    }
}

[test]
def test_is_absolute(t : T?) {
    t |> run("relative path is not absolute") @(t : T?) {
        t |> success(!is_absolute("foo/bar"))
    }
}

[test]
def test_parent(t : T?) {
    t |> run("returns parent directory") @(t : T?) {
        let p = parent("a/b/c")
        t |> success(p == "a/b" || p == "a\\b", "expected a/b or a\\b, got {p}")
    }
}

// ---- filesystem functions with error reporting ----

[test]
def test_file_size_error(t : T?) {
    t |> run("reports error for missing file") @(t : T?) {
        var error : string
        let sz = file_size("_no_such_file_99999.xyz", error)
        t |> equal(sz, int64(-1))
        t |> success(!empty(error), "error should be non-empty")
    }
    t |> run("returns valid size for existing file") @(t : T?) {
        var error : string
        let sz = file_size("{get_das_root()}/tests/fio/fio_errors.das", error)
        t |> success(sz > int64(0), "size should be positive")
        t |> success(empty(error), "error should be empty, got: {error}")
    }
}

[test]
def test_copy_file_error(t : T?) {
    t |> run("reports error for missing source") @(t : T?) {
        var error : string
        let ok = copy_file("_no_such_source_99999.xyz", "_dest.xyz", false, error)
        t |> success(!ok, "copy_file should fail")
        t |> success(!empty(error), "error should be non-empty")
    }
}

[test]
def test_equivalent_error(t : T?) {
    t |> run("reports error for missing file") @(t : T?) {
        var error : string
        let ok = equivalent("_no_such_a_99999.xyz", "_no_such_b_99999.xyz", error)
        t |> success(!ok, "equivalent should fail")
        t |> success(!empty(error), "error should be non-empty")
    }
}

[test]
def test_disk_space_error(t : T?) {
    t |> run("reports error for bad path") @(t : T?) {
        var error : string
        disk_space("Z:\\no_such_drive_99999", error)
        t |> success(!empty(error), "error should be non-empty")
    }
}

[test]
def test_remove_error(t : T?) {
    t |> run("reports error for missing file") @(t : T?) {
        var error : string
        let ok = remove("_no_such_file_to_remove_99999.xyz", error)
        t |> success(!ok, "remove should fail")
        t |> success(!empty(error), "error should be non-empty")
    }
}

[test]
def test_mkdir_error(t : T?) {
    t |> run("reports error for invalid path") @(t : T?) {
        var error : string
        let ok = mkdir("_no_such_parent_99999/child/deep", error)
        t |> success(!ok, "mkdir should fail")
        t |> success(!empty(error), "error should be non-empty")
    }
}

[test]
def test_rmdir_rec_error(t : T?) {
    t |> run("succeeds for missing directory") @(t : T?) {
        var error : string
        let ok = rmdir_rec("_no_such_dir_to_remove_99999", error)
        t |> success(ok, "rmdir_rec on non-existent path should succeed (remove_all semantics)")
        t |> success(empty(error), "error should be empty")
    }
}

[test]
def test_relative_error(t : T?) {
    t |> run("works for valid paths") @(t : T?) {
        var error : string
        let r = relative("{get_das_root()}/tests/fio/fio_errors.das", "{get_das_root()}/tests", error)
        t |> success(r == "fio/fio_errors.das" || r == "fio\\fio_errors.das", "expected fio/fio_errors.das, got {r}")
        t |> success(empty(error), "error should be empty, got: {error}")
    }
}
