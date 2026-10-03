options gen2
require dastest/testing_boost public
require daslib/fio

[test]
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
