options gen2
require daslib/fio
require daslib/clargs
[export]
def main() : int {
    let args <- get_cli_arguments()
    if (length(args) != 1) { return 2 }
    let file = fopen(args[0], "ab")
    if (file == null) { return 2 }
    var acquired = false
    { acquired = try_lock_file(file) } finally { fclose(file) }
    return acquired ? 0 : 3
}
