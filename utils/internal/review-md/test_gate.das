options gen2

require dastest/testing_boost public
require daslib/fio
require gate

[test]
def test_run_gates_spawns_with_repo_root_cwd(t : T?) {
    let fixture_r = create_temp_directory_result("review_md_run")
    t |> success(fixture_r is value, "temp fixture dir created")
    if (!(fixture_r is value)) return
    let fixture = unsafe(fixture_r.value)
    t |> success(mkdir_rec("{fixture}/green"))
    t |> success(mkdir_rec("{fixture}/red"))
    // the green gate proves the cwd contract: it resolves its own path relative to the root
    let green_src = "options gen2\n\nrequire daslib/fio\n\n[export]\ndef main() : int \{\n    return fexist(\"green/REVIEW.das\") ? 0 : 1\n\}\n"
    let red_src = "options gen2\n\n[export]\ndef main() : int \{\n    return 1\n\}\n"
    t |> success(fwrite("{fixture}/green/REVIEW.das", green_src))
    t |> success(fwrite("{fixture}/red/REVIEW.das", red_src))

    let cwd_before = getcwd()
    var inscope none : array<string>
    t |> equal(0, run_gates(fixture, none))
    t |> equal(1, run_gates(fixture, ["green/REVIEW.das", "red/REVIEW.das"]))
    t |> equal(cwd_before, getcwd(), "run_gates restores the caller's cwd")
    t |> equal(2, run_gates(path_join(fixture, "no/such/dir"), ["green/REVIEW.das", "red/REVIEW.das"]),
               "an unreachable root fails every gate rather than passing silently")

    t |> success(rmdir_rec_result(fixture) is value, "fixture removed")
}
