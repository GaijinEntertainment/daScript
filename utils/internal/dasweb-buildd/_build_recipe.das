options gen2

require daslib/fio
require strings

//! Stand-in for run_build.sh in the service test: a build that does
//! what the job's entry file says on its first line - `exit
//! <status>`, `artifact` (writes sample.wasm, exits 0) or `sleep`
//! (outlives the build timeout). Invoked as the service invokes a
//! recipe: `<this> -- <src_dir> <out_dir> <mode> <entry>`.

[export]
def main : int {
    let args <- get_command_line_arguments()
    let count = length(args)
    if (count < 4) {
        print("recipe: expected <src_dir> <out_dir> <mode> <entry>\n")
        return 2
    }
    let script = fread(path_join(args[count - 4], args[count - 1]))
    print("recipe ran: {script}\n")
    if (script |> starts_with("artifact")) {
        fwrite(path_join(args[count - 3], "sample.wasm"), "recipe wasm")
        return 0
    }
    if (script |> starts_with("sleep")) {
        sleep(60000u)
        return 0
    }
    if (script |> starts_with("exit ")) {
        return to_int(slice(script, 5))
    }
    return 0
}
