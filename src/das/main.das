require ast/ast_aot_cpp
require ast/standalone_contexts

require fio
require strings


def find_argument(args : array<string>; key : string) {
    for (i in range(length(args) - 1)) {
        if (args[i] == key) {
            return args[i + 1]
        }
    }
    panic("expected {key} argument!")
    return ""
}

[export]
def main() {
    let args <- get_command_line_arguments()

    let mode = find_argument(args, "--mode");
    if (mode == "aot") {
        let input = find_argument(args, "--input");
        let output = find_argument(args, "--output");
        print("aot {input} to {output}\n")
        let res = aot(input, false, false)
        if (res |> length() > 0) {
            fopen(output, "wb") <| $(fw) {
                if (fw != null) {
                    fwrite(fw, res)
                } else {
                    print("Couldn't create output file {output}")
                }
            }
        }
    } elif (mode == "standalone_ctx") {
        let input = find_argument(args, "--input");
        let output = find_argument(args, "--output");
        let cfg = StandaloneContextCfg(context_name = "context",
                                       class_name = "Standalone",
                                       cpp_output_dir = output)
        print("standalone ctx {input} to {output}\n")
        let res = standalone_aot(input, cfg, false, false)
        if (res |> length() > 0) {
            fopen(output, "wb") <| $(fw) {
                if (fw != null) {
                    fwrite(fw, res)
                } else {
                    print("Couldn't create output file {output}")
                }
            }
        }
    } elif (mode == "ast_print") {
        panic("unimplemented")
    } else {
        panic("Unknown mode {mode}")
    }
}
