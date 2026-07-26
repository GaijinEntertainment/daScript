options gen2

// Test intermediary: spawns the PTY host DETACHED and exits immediately, so
// the test can prove the host survives its spawner's death. Arguments pass
// through to the host after "--host-args"; DASHERD_HOST_TOKEN passes via env.

require terminal/terminal
require strings

[export]
def main : int {
    var host_args : array<string>
    var collecting = false
    for (argument in get_command_line_arguments()) {
        if (collecting) {
            host_args |> push(argument)
        } elif (argument == "--host-args") {
            collecting = true
        }
    }
    if (empty(host_args)) {
        print("host_launcher: --host-args is required\n")
        return 2
    }
    var spawned = 0
    var spawn_error = ""
    let _ = spawn_detached(host_args, "", [
        "DASHERD_HOST_TOKEN={get_env_variable("DASHERD_HOST_TOKEN")}",
        "DASHERD_CHILD_ENV_KEYS={get_env_variable("DASHERD_CHILD_ENV_KEYS")}",
        "DASHERD_CHILD_PTYHOST_TEST={get_env_variable("DASHERD_CHILD_PTYHOST_TEST")}"
    ]) $(pid, launch_error) {
        spawned = pid
        spawn_error := launch_error
    }
    print("launched pid={spawned} error='{spawn_error}'\n")
    return spawned != 0 ? 0 : 1
}
