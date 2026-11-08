options gen2
options no_aot

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require daslib/clargs

//! `fpoll` on a pipe: false while the child is silent, true once it wrote, true at its EOF;
//! `funbuffered` on the same stream is a no-op the read survives; a regular file is always ready

def das_exe() : string {
    let args <- get_command_line_arguments()
    return empty(args) ? "" : args[0]
}

[test(description = "fpoll on a child's pipe is false while silent and true once a line or EOF arrives")]
def test_fpoll_pipe(t : T?) {
    t |> run("a silent child polls false, a written line polls true, the EOF polls true") @(t : T?) {
        let tmp = create_temp_directory_result("das fio poll ")
        if (!(tmp is value)) {
            t |> failure("could not create temp directory: {tmp as error}")
            return
        }
        let root_dir = tmp as value
        let release = path_join(root_dir, "release")
        let fixture = path_join(get_das_root(), "tests/fio/_fixture_poll_child.das")
        let argv <- [das_exe(), "-dasroot", get_das_root(), fixture, "--", release]
        var line = ""
        var silent = false
        var ready = false
        var at_eof = false
        let rc = unsafe(popen_argv(argv, 0.0, $(f) {
            if (f == null) return
            funbuffered(f)
            silent = !fpoll(f, 200)
            fwrite(release, "go")
            ready = fpoll(f, int(child_deadline(30.0) * 1000.0))
            line = fgets(f)
            at_eof = fpoll(f, int(child_deadline(30.0) * 1000.0))
            let tail = fgets(f)
            at_eof = at_eof && empty(tail)
        }))
        t |> equal(rc, 0, "child exited 0")
        t |> success(silent, "nothing to read while the child waits on the release file")
        t |> success(ready, "readable once the child wrote its line")
        t |> equal(rtrim(line, "\r\n"), "polled", "the line the child wrote")
        t |> success(at_eof, "a closed pipe polls true and reads empty")
        let cleanup = rmdir_rec_result(root_dir)
        t |> success(cleanup is value && cleanup as value, "temp dir removed")
    }
}

[test(description = "fpoll on a regular file is always true")]
def test_fpoll_regular_file(t : T?) {
    t |> run("a regular file is always readable") @(t : T?) {
        var terr : string
        let tmp = temp_directory(terr)
        let fname = path_join(empty(tmp) ? "." : tmp, "fio_poll_{ref_time_ticks()}.txt")
        t |> success(fwrite(fname, "x"), "the probe file is written")
        var ready = false
        fopen(fname, "rb") $(f) {
            if (f != null) {
                funbuffered(f)
                ready = fpoll(f, 0)
            }
        }
        t |> success(ready, "fpoll reports a regular file readable with a zero timeout")
        t |> success(remove(fname))
    }
}

[test(description = "try_lock_file locks exclusively across handles and a child process, and closing the handle releases the lock")]
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
            let blocked = run_and_capture([get_host_binary(), "-dasroot", get_das_root(),
                    "{get_das_root()}/tests/fio/_fixture_file_lock.das",
                    "--", "{root}/owner.lock"], output, 60.0)
            t |> equal(3, blocked, "another process is excluded: {output}")
            fclose(first); first = null
            t |> success(try_lock_file(second), "close releases ownership")
        } finally {
            if (first != null) { fclose(first) }
            fclose(second)
        }
        var output : string
        let acquired = run_and_capture([get_host_binary(), "-dasroot", get_das_root(),
                "{get_das_root()}/tests/fio/_fixture_file_lock.das",
                "--", "{root}/owner.lock"], output, 60.0)
        t |> equal(0, acquired, "a later process acquires after close: {output}")
        t |> success(!try_lock_file(null))
    } finally { rmdir_rec(root) }
}
