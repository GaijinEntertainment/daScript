options gen2

// Test intermediary: spawns the PTY host DETACHED and exits immediately, so
// tests can prove the host survives its spawner's death. Test-only tool: the
// token rides argv here (loopback throwaway), which production never does.
//   --token <t>            host API token (becomes DASHERD_HOST_TOKEN)
//   --child-env KEY=VALUE  forwarded child env entry (repeatable)
//   --host-args <argv...>  everything after this is the host command line

require terminal/terminal
require daslib/strings_boost
require strings

[export]
def main : int {
    var host_args : array<string>
    var child_pairs : array<string>
    var token = ""
    var collecting = false
    var expect_token = false
    var expect_child_env = false
    for (argument in get_command_line_arguments()) {
        if (collecting) {
            host_args |> push(argument)
        } elif (expect_token) {
            token = argument
            expect_token = false
        } elif (expect_child_env) {
            child_pairs |> push(argument)
            expect_child_env = false
        } elif (argument == "--token") {
            expect_token = true
        } elif (argument == "--child-env") {
            expect_child_env = true
        } elif (argument == "--host-args") {
            collecting = true
        }
    }
    if (empty(host_args)) {
        print("host_launcher: --host-args is required\n")
        return 2
    }
    var inscope env : array<string>
    var inscope key_parts : array<string>
    env |> push("DASHERD_HOST_TOKEN={token}")
    for (pair in child_pairs) {
        let eq = find(pair, "=")
        if (eq <= 0) continue
        key_parts |> push(slice(pair, 0, eq))
        env |> push("DASHERD_CHILD_{pair}")
    }
    let keys = join(key_parts, ";")
    env |> push("DASHERD_CHILD_ENV_KEYS={keys}")
    var spawned = 0
    var spawn_error = ""
    let _ = spawn_detached(host_args, "", env) $(pid, launch_error) {
        spawned = pid
        spawn_error := launch_error
    }
    print("launched pid={spawned} error='{spawn_error}'\n")
    return spawned != 0 ? 0 : 1
}
