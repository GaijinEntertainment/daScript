options gen2

//! prints the host binary, then every argument the tool received after daslang's `--`
[export]
def main() {
    let args <- get_command_line_arguments()
    print("host={args[0]}\n")
    var past = false
    for (a in args) {
        if (past) {
            print("arg={a}\n")
        }
        past ||= a == "--"
    }
}
