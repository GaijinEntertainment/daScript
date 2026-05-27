options gen2
options no_unused_function_arguments = false
options no_unused_block_arguments = false

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require strings

def dastest_root() : string {
    return "{get_das_root()}/dastest"
}

def fixture_dir() : string {
    return "{dastest_root()}/tests/_isolated_fixture"
}

def run_isolated(var output : string&; var exit_code : int&) {
    let exe = get_command_line_arguments()[0]
    let cmd = "{exe} {dastest_root()}/dastest.das -- --test {fixture_dir()} --isolated-mode --isolated-mode-threads 4"
    output = ""
    unsafe {
        exit_code = popen(cmd) $(f) {
            if (f != null) {
                while (!feof(f)) {
                    output += fgets(f)
                }
            }
        }
    }
}

[test]
def test_isolated_mode(tt : T?) {
    tt |> run("isolated mode spawns 4 workers, fixture suite all passes, --worker-index round-trips") <| @(t : T?) {
        var output : string
        var exit_code : int
        run_isolated(output, exit_code)
        t |> equal(exit_code, 0)
        t |> success(output |> contains("SUCCESS"), "expected SUCCESS in dastest output, got:\n{output}")
    }
}
