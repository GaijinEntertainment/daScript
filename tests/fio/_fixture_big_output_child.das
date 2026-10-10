options gen2
options no_aot // a subprocess fixture with no [test] - an AOT build of it tests nothing

require daslib/clargs
require strings

// Child for test_process.das: prints the given number of 100-byte lines, far more than a pipe
// holds, then exits 0.
[export]
def main() : int {
    let args <- get_cli_arguments()
    if (length(args) != 1) return 2
    let pad = repeat("x", 99)
    for (_i in range(to_int(args[0]))) {
        print("{pad}\n")
    }
    return 0
}
