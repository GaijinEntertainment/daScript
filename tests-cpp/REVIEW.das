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

//! every source a `set_source_files_properties` names exists beside its CMakeLists - a property naming a moved file applies to nothing and its test builds green without the flag
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

struct private SwitchedModule {
    folder : string
    switches : array<string>
    libs : array<string>
}

def private root_disable_switches : table<string> {
    var out : table<string>
    for (name in cmake_command_targets(fread("CMakeLists.txt"), "option")) {
        if (name |> starts_with("DAS_") && name |> ends_with("_DISABLED")) {
            out |> insert(name)
        }
    }
    return <- out
}

//! `Module_<name>` -> the module folder whose CMakeLists calls `ADD_MODULE_CPP(<name>)` and the
//! root switches that folder keys on; a module no root switch reaches is always built
def private switched_modules : table<string; SwitchedModule> {
    var inscope root_switches <- root_disable_switches()
    var out : table<string; SwitchedModule>
    for (f in cmakelists_under("modules")) {
        let text = fread(f)
        var folder_switches <- [for (tok in keys(prefixed_tokens(text, "DAS_"))); tok; where key_exists(root_switches, tok)]
        continue if (empty(folder_switches))
        folder_switches |> sort
        var folder_libs <- [for (tok in keys(prefixed_tokens(text, "libDasModule"))); tok]
        folder_libs |> sort
        for (name in cmake_command_targets(text, "add_module_cpp")) {
            out["Module_{name}"] <- SwitchedModule(folder = dir_name(f), switches := folder_switches, libs := folder_libs)
        }
    }
    return <- out
}

//! every `Module_X` token a C or C++ source names, counting `register_Module_X` as `Module_X`
def private registered_modules(text : string) : array<string> {
    var out <- [for (tok in keys(prefixed_tokens(text, "Module_"))); tok]
    out |> push_from([for (tok in keys(prefixed_tokens(text, "register_Module_"))); slice(tok, length("register_"), length(tok))])
    out |> sort
    return <- out
}

def private is_gated(switches_named_here : table<string>; targets_named_here : table<string>; switched : SwitchedModule) : bool {
    for (lib in switched.libs) {
        return true if (key_exists(targets_named_here, lib))
    }
    for (sw in switched.switches) {
        return true if (key_exists(switches_named_here, sw))
    }
    return false
}

//! a big test that registers a switched module is gated on that module's switch - configured
//! with `DAS_*_DISABLED=ON`, the test links an entry point nothing builds
def private check_big_test_gating {
    var inscope per_module <- switched_modules()
    for (f in cmakelists_under("tests-cpp/big")) {
        let text = fread(f)
        var inscope switches_named_here <- prefixed_tokens(text, "DAS_")
        var inscope targets_named_here <- prefixed_tokens(text, "libDasModule")
        let folder = dir_name(f)
        dir(folder) $(name) {
            if (name |> ends_with(".c") || name |> ends_with(".cpp")) {
                let src = path_join(folder, name)
                for (mod in registered_modules(fread(src))) {
                    per_module |> get(mod) $(switched) {
                        if (!is_gated(switches_named_here, targets_named_here, switched)) {
                            let sw = switched.switches[0]
                            let lib = empty(switched.libs) ? "its library" : switched.libs[0]
                            gate_finding(f, "{src} registers {mod}, which {switched.folder} builds only while {sw} is off - gate this folder on {sw} (if({sw}) return()) or on TARGET {lib}, or a configure with {sw}=ON fails at link")
                        }
                    }
                }
            }
        }
    }
}

[export]
def main() : int {
    if (!fexist("tests-cpp/REVIEW.das")) {
        to_log(LOG_ERROR, "tests-cpp/REVIEW.das: run from the repo root\n")
        return 2
    }
    check_lane_wiring()
    check_big_test_gating()
    return gate_verdict("tests-cpp")
}
