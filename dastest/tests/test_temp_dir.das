options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require strings

[test]
def test_temp_dir_is_private(tt : T?) {
    tt |> run("one directory per run, outside the repo tree, writable") <| @(t : T?) {
        let dir = test_temp_dir()
        t |> equal(test_temp_dir(), dir)
        t |> success(stat(dir).is_dir, "{dir} exists")
        t |> success(find(dir, get_das_root()) != 0, "{dir} is not under the repo root")
        let probe = path_join(dir, "probe.txt")
        t |> success(fwrite(probe, "probe"), "{probe} is writable")
        t |> equal(fread(probe), "probe")
    }
}

[test]
def test_temp_dir_removed_when_the_run_ends(tt : T?) {
    tt |> run("a run's directory and its files are gone once the run ends") <| @(t : T?) {
        let exe = get_command_line_arguments()[0]
        var output : string
        let rc = run_and_capture([exe, "-dasroot", get_das_root(), "{get_das_root()}/dastest/tests/_temp_dir_child.das"], output)
        t |> equal(rc, 0, output)
        let marker = "TEMP_DIR="
        let at = find(output, marker)
        t |> success(at >= 0, "the child printed its directory:\n{output}")
        return if (at < 0)
        let dir = strip(slice(output, at + length(marker)))
        t |> success(!empty(dir) && find(dir, test_temp_dir()) < 0, "the child's directory is its own: {dir}")
        t |> success(!stat(dir).is_valid, "{dir} was removed when the child ended")
    }
}

[test]
def test_temp_dir_panics_without_a_temp_root(tt : T?) {
    tt |> run("no usable temp root: the first call panics and names itself") <| @(t : T?) {
        let exe = get_command_line_arguments()[0]
        let missing = path_join(test_temp_dir(), "no_such_dir")
        var lines : array<string>
        var rc = 0
        let argv <- [exe, "-dasroot", get_das_root(), "{get_das_root()}/dastest/tests/_temp_dir_child.das"]
        with_process(argv, "", ["TMP={missing}", "TEMP={missing}", "TMPDIR={missing}"]) $(var p) {
            rc = unsafe(process_wait(p, 60.0))
            unsafe(process_drain(p) $(line) {
                lines |> push(clone_string(line))
            })
        }
        t |> success(rc != 0, "the child failed: rc={rc}")
        t |> success(find(join(lines, "\n"), "test_temp_dir: ") >= 0, "the panic names test_temp_dir:\n{join(lines, "\n")}")
    }
}
