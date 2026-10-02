options gen2
options indenting = 4
require daslib/ansi_colors
require daslib/clargs

[export]
def main {
    let args <- get_user_args()
    if (length(args) != 1 || (args[0] != "enabled" && args[0] != "disabled")) { panic("expected color mode") }
    use_tty_colors = args[0] != "enabled"
    init_ansi_colors(["exe", "script.das", "--color"])
    if (use_tty_colors != (args[0] == "enabled")) { panic("unexpected color result") }
}
