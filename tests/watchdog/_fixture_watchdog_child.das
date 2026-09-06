options gen2
options no_aot

require daslib/clargs
require daslib/fio

// The supervised child for test_watchdog.das. `state_dir mode`: a run counter in state_dir
// tells the child which launch it is, and `mode` picks the story it acts out:
//   crash-then-ok    run 1 prints the ready line and exits 9; run 2 prints it and exits 0
//   tune-then-ok     run 1 prints the tune marker and exits 3; run 2 exits 0
//   config-restart   run 1 exits 4; run 2 exits 0
//   tune-abort       run 1 exits 3 with no restart marker; run 2 exits 0
//   park             prints a line, waits for the stop file named by CADMUS_STOP_FILE (or, with
//                    none named, to be terminated), exits 0
// Codes return from main: a das exit() is an abnormal termination and reports 1.
def run_number(state_dir : string) : int {
    let path = path_join(state_dir, "runs")
    var runs = 0
    if (fexist(path)) {
        fopen(path, "rb") $(f) {
            if (f != null) {
                runs = to_int(fread(f))
            }
        }
    }
    runs++
    fwrite(path, "{runs}")
    return runs
}

[export]
def main() : int {
    let args <- get_cli_arguments()
    if (length(args) != 2) return 2
    let run = run_number(args[0])
    let mode = args[1]
    if (mode == "crash-then-ok") {
        print("[I] LLVM JIT: DLL cache hit .jitted_scripts/sub/fake.dll\n")
        print("listening on http://127.0.0.1:1/\n")
        return run == 1 ? 9 : 0
    }
    if (mode == "tune-then-ok") {
        if (run == 1) {
            print("llvm_tune: tuning scope 'fixture'\n")
            print("llvm_tune: restart to apply the winners\n")
            return 3
        }
        return 0
    }
    if (mode == "config-restart") {
        return run == 1 ? 4 : 0
    }
    if (mode == "tune-abort") {
        return run == 1 ? 3 : 0
    }
    if (mode == "park") {
        print("parked\n")
        // parks until the stop file the supervisor named appears; with none named it parks until
        // terminated (the deadlock guard is the only other way out)
        let stop_file = has_env_variable("CADMUS_STOP_FILE") ? get_env_variable("CADMUS_STOP_FILE") : ""
        let started = ref_time_ticks()
        while (empty(stop_file) || !fexist(stop_file)) {
            if (get_time_usec(started) > 30000000) return 6
            sleep(5u)
        }
        return 0
    }
    return 7
}
