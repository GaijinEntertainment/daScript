options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/clargs
require daslib/strings_boost
require strings
require _host_fixture

def run_policy_child(host_flags, script_args : array<string>) : Captured {
    var argv <- [get_host_binary(), "-dasroot", get_das_root(), "-no-module-cache"]
    argv |> push_from(host_flags)
    argv |> push(path_join(tests_dir(), "_fixture_policy_child.das"))
    argv |> push_from(script_args)
    return run_daslang(argv)
}

[test(description = "-policy sets a compile policy by name, a bare name turns a bool on, a later one overrides an earlier one, a -policy after the script's -- is the script's own argument, and a name or a value the policies do not take, or a -policy with nothing after it, ends the host with an error naming it", timeout = 1200)]
def test_policy_flag(t : T?) {
    t |> run("off by default, on by a bare name") @(t : T?) {
        t |> equal(strip(run_policy_child([], []).out), "pod off")
        t |> equal(strip(run_policy_child(["-policy", "force_inscope_pod"], []).out), "pod on")
    }
    t |> run("a later -policy overrides an earlier one") @(t : T?) {
        let res = run_policy_child(["-policy", "force_inscope_pod=true",
            "-policy", "force_inscope_pod=false"], [])
        t |> equal(strip(res.out), "pod off")
    }
    t |> run("after the script's -- it is the script's argument") @(t : T?) {
        let res = run_policy_child([], ["--", "-policy", "force_inscope_pod"])
        t |> equal(strip(res.out), "pod off")
    }
    t |> run("an unknown name") @(t : T?) {
        let res = run_policy_child(["-policy", "no_such_policy"], [])
        t |> equal(res.rc, 1, "the host's exit code, got:\n{res.out}")
        t |> success(res.out |> contains("unknown policy 'no_such_policy'"), res.out)
    }
    t |> run("a value the field does not take") @(t : T?) {
        let res = run_policy_child(["-policy", "stack=many"], [])
        t |> equal(res.rc, 1, "the host's exit code, got:\n{res.out}")
        let refusal = "policy 'stack' expects a 32-bit unsigned integer, got 'many'"
        t |> success(res.out |> contains(refusal), res.out)
    }
    t |> run("nothing after the flag") @(t : T?) {
        let res = run_daslang([get_host_binary(), "-dasroot", get_das_root(), "-policy"])
        t |> success(res.rc != 0, "the host's exit code, got:\n{res.out}")
        t |> success(res.out |> contains("-policy requires name=value"), res.out)
    }
}

[test(description = "a project file's project_policies set the policies of the compile, one item a line or between semicolons, a -policy on the command line overrides them, and an item the policies do not take ends the host with 1 naming it", timeout = 1200)]
def test_project_policies(t : T?) {
    let project = path_join(tests_dir(), "_fixture_policy.das_project")
    t |> equal(strip(run_policy_child(["-project", project], []).out), "pod on")
    let overridden = run_policy_child(["-project", project,
        "-policy", "force_inscope_pod=false"], [])
    t |> equal(strip(overridden.out), "pod off")
    let bad_project = path_join(tests_dir(), "_fixture_policy_bad.das_project")
    let bad = run_policy_child(["-project", bad_project], [])
    t |> equal(bad.rc, 1, "the host's exit code, got:\n{bad.out}")
    t |> success(bad.out |> contains("project_policies: unknown policy 'no_such_policy'"), bad.out)
}

def literal_program() : string => path_join(tests_dir(), "_fixture_policy_literal.das")

//! the aot tool over the literal program; `tool_args` follow the
//! program's path
def run_aot_tool(host_flags, tool_args : array<string>) : Captured {
    var argv <- [get_host_binary(), "-dasroot", get_das_root()]
    let tool <- [path_join(get_das_root(), "utils/aot/main.das"), "--", "-force-overwrite",
        "-aot", literal_program()]
    argv |> push_from(host_flags, tool, tool_args)
    return run_daslang(argv)
}

def emits_stack_literal(cpp_path : string) : bool {
    let text = fread(cpp_path)
    return (text |> contains("to_array_temp")) && !(text |> contains("to_array_move"))
}

def emits_heap_literal(cpp_path : string) : bool {
    let text = fread(cpp_path)
    return (text |> contains("to_array_move")) && !(text |> contains("to_array_temp"))
}

[test(description = "a -policy reaches the program an AOT emission compiles: after the aot tool's own -- and on daslang -aot an array literal argument is built on the stack under force_inscope_pod, a -policy ahead of the tool's -- is the host's and leaves the literal on the heap, and a name that is no policy stops the tool", timeout = 1200)]
def test_aot_policy(t : T?) {
    t |> run("the aot tool, no policy") @(t : T?) {
        let out = path_join(test_temp_dir(), "plain.cpp")
        t |> equal(run_aot_tool([], [out]).rc, 0)
        t |> success(emits_heap_literal(out), "without the policy the literal is a heap array")
    }
    t |> run("the aot tool, -policy after its --") @(t : T?) {
        let out = path_join(test_temp_dir(), "stacked.cpp")
        t |> equal(run_aot_tool([], [out, "-policy", "force_inscope_pod"]).rc, 0)
        t |> success(emits_stack_literal(out), "under the policy the literal is a stack array")
    }
    t |> run("the aot tool, -policy ahead of its --") @(t : T?) {
        let out = path_join(test_temp_dir(), "host_policy.cpp")
        t |> equal(run_aot_tool(["-policy", "force_inscope_pod"], [out]).rc, 0)
        t |> success(emits_heap_literal(out), "the host's policy is not the emitted program's")
    }
    t |> run("the aot tool, a name that is no policy") @(t : T?) {
        let out = path_join(test_temp_dir(), "bad.cpp")
        let bad = run_aot_tool([], [out, "-policy", "no_such_policy"])
        t |> success(bad.rc != 0, "the tool's exit code, got:\n{bad.out}")
        t |> success(bad.out |> contains("unknown policy 'no_such_policy'"), bad.out)
    }
    t |> run("daslang -aot") @(t : T?) {
        let out = path_join(test_temp_dir(), "direct.cpp")
        let res = run_daslang([get_host_binary(), "-aot", literal_program(), out,
            "-dasroot", get_das_root(), "-policy", "force_inscope_pod"])
        t |> equal(res.rc, 0, res.out)
        t |> success(emits_stack_literal(out), "daslang -aot takes the policy too")
    }
}
