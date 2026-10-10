options gen2

require strings
require daslib/fio

//! `daslang -tool` with no name lands here: the tools `daslang -tool <name>` runs in this install.

[export]
def main() {
    let root = get_das_root()
    var tools : array<string>
    dir(path_join(root, "utils")) $(name) {
        if (!starts_with(name, ".") && !empty(get_tool_script(name, root))) {
            tools |> push(name)
        }
    }
    if (!empty(get_tool_script("dastest", root))) {
        tools |> push("dastest")
    }
    sort(tools)
    print("daslang -tool <name> [arguments] runs the tool <name> from {root}:\n")
    for (n in tools) {
        print("    {n}\n")
    }
}
