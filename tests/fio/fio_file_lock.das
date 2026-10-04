options gen2
options no_aot // subprocess fixture needs the interpreter executable
require dastest/testing_boost public
require daslib/fio
require daslib/clargs

[test]
def file_lock_is_exclusive_and_released_by_close(t : T?) {
    let made = create_temp_directory_result("file-lock-")
    verify(made is value)
    let root = made as value
    {
        var first = fopen("{root}/owner.lock", "ab")
        let second = fopen("{root}/owner.lock", "ab")
        verify(first != null && second != null)
        {
            t |> success(try_lock_file(first))
            t |> success(!try_lock_file(second), "another open handle cannot take the lock")
            var output : string
            let blocked = run_and_capture([get_host_binary(), "{get_das_root()}/tests/fio/_fixture_file_lock.das", "--", "{root}/owner.lock"], output, 60.0)
            t |> equal(3, blocked, "another process is excluded: {output}")
            fclose(first); first = null
            t |> success(try_lock_file(second), "close releases ownership")
        } finally {
            if (first != null) { fclose(first) }
            fclose(second)
        }
        var output : string
        let acquired = run_and_capture([get_host_binary(), "{get_das_root()}/tests/fio/_fixture_file_lock.das", "--", "{root}/owner.lock"], output, 60.0)
        t |> equal(0, acquired, "a later process acquires after close: {output}")
        t |> success(!try_lock_file(null))
    } finally { rmdir_rec(root) }
}
