options gen2

require strings
require daslib/fio
require dastest/review_gate

// The mechanical half of tests-cpp/REVIEW.md (contract: REVIEW_COMMON.md at the repo root).
// Run from the repo root: bin/daslang tests-cpp/REVIEW.das — exit 0 clean, 1 with findings.

def private lanes_of(text : string; command : string) : array<string> {
    var inscope names <- cmake_command_targets(text, command)
    return <- [for (n in names); slice(n, length("test-"), length(n)); where n |> starts_with("test-")]
}

def private cmakelists_under(folder : string) : array<string> {
    var out : array<string>
    dir(folder) $(name) {
        let p = "{folder}/{name}/CMakeLists.txt"
        if (name != "." && name != ".." && fexist(p)) {
            out |> push(p)
        }
    }
    out |> sort
    return <- out
}

// Targets named by `add_dependencies(test-<lane> ...)`, whichever lane.
def private lane_wired_targets(text : string) : array<string> {
    var out : array<string>
    for (blk in cmake_command_blocks(text, "add_dependencies")) {
        var inscope args <- cmake_args(blk)
        continue if (empty(args) || !starts_with(args[0], "test-"))
        out |> reserve(length(out) + length(args) - 1)
        for (i in range(1, length(args))) {
            out |> push(args[i])
        }
    }
    return <- out
}

// The target an `add_test` COMMAND runs, or "" when it is not a target this file builds -
// an interpreter or a plain path is nobody's build dependency.
def private runs_target(command : string; local_exes : array<string>) : string {
    let genex = "$<TARGET_FILE:"
    if (command |> starts_with(genex) && command |> ends_with(">")) {
        return slice(command, length(genex), length(command) - 1)
    }
    return find_index(local_exes, command) >= 0 ? command : ""
}

// Every test binary a file registers is a binary its lane builds. `ctest -L <lane>` runs
// whatever carries the label, while `test-<lane>` builds only what is wired to it, so an
// unwired target runs stale or missing while the lane reports success.
def private check_test_targets(f : string; text : string) {
    var inscope local_exes <- cmake_command_targets(text, "add_executable")
    var inscope wired <- lane_wired_targets(text)
    for (command in cmake_test_commands(text)) {
        let target = runs_target(command, local_exes)
        continue if (empty(target) || find_index(wired, target) >= 0)
        gate_finding(f, "test target {target} is wired into no test-<lane> here - add_dependencies(test-<lane> {target}) or the lane runs a binary it never built")
    }
}

// Every source a `set_source_files_properties` names exists beside its CMakeLists. The
// property applies to nothing when the file moves, so a test whose compile flag went
// missing builds the ordinary way and stays green.
def private check_source_property_paths(f : string; text : string) {
    let folder = dir_name(f)
    for (blk in cmake_command_blocks(text, "set_source_files_properties")) {
        var inscope args <- cmake_args(blk)
        for (a in args) {
            break if (a == "PROPERTIES" || a == "DIRECTORY" || a == "TARGET_DIRECTORY")
            continue if (a |> starts_with("$"))
            if (!fexist(path_join(folder, a))) {
                gate_finding(f, "set_source_files_properties names {a}, which does not exist beside {f} - the property applies to nothing and its test builds without the flag")
            }
        }
    }
}

// Every lane a file labels tests into is a lane that file also builds for.
def private check_lane_wiring {
    let top_cmake = "tests-cpp/CMakeLists.txt"
    var inscope aggregate_lanes <- lanes_of(fread(top_cmake), "add_custom_target")
    var files <- [top_cmake]
    files |> push_from(cmakelists_under("tests-cpp/big"))
    for (f in files) {
        let text = fread(f)
        var inscope wired_lanes <- lanes_of(text, "add_dependencies")
        // a file that defines an aggregate wires its dependencies at the definition site
        wired_lanes |> push_from(lanes_of(text, "add_custom_target"))
        var inscope labels <- cmake_test_labels(text)
        for (lab in labels) {
            // a label with no test-<lane> target is a ctest filter — nothing to wire to
            continue if (find_index(aggregate_lanes, lab) < 0 || find_index(wired_lanes, lab) >= 0)
            gate_finding(f, "labels a test \"{lab}\" but wires nothing into test-{lab} — wire it with add_dependencies(test-{lab} <target>), or the lane runs a binary it never built")
        }
        check_test_targets(f, text)
        check_source_property_paths(f, text)
    }
}

[export]
def main() : int {
    if (!fexist("tests-cpp/REVIEW.das")) {
        to_log(LOG_ERROR, "tests-cpp/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_lane_wiring()
    return gate_verdict("tests-cpp")
}
