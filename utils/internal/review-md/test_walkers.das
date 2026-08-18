options gen2

require dastest/testing_boost public
require strings
require daslib/strings_boost
require daslib/fio

// End-to-end walker coverage against planted fixture repos, spawned exactly as CI runs
// them: the pilot gate's rejection branches, all.das's rc wiring, main.das's gate-red arm.

def write_at(t : T?; root, rel, text : string) {
    let p = path_join(root, rel)
    let d = dir_name(p)
    if (!empty(d)) {
        mkdir_rec(d)
    }
    t |> success(fwrite(p, text), "wrote {rel}")
}

def run_in(cwd_root : string; args : array<string>) : tuple<rc : int; out : string> {
    let prev = getcwd()
    if (!chdir(cwd_root)) {
        return (rc = -1, out = "chdir failed: {cwd_root}")
    }
    var out : string
    let rc = run_and_capture(args, out, 120.0)
    chdir(prev)
    return (rc = rc, out = out)
}

def spawn_at(cwd_root : string; args : array<string>) : tuple<rc : int; out : string> {
    let argv <- get_command_line_arguments()
    var full : array<string>
    full |> push(normalize(argv[0]))
    full |> push_from(args)
    return run_in(cwd_root, full)
}

def make_pilot_fixture(t : T?; tree, root : string) {
    write_at(t, root, "CMakeLists.txt", "install(FILES src/utils/good-tool/main.das DESTINATION utils/good-tool)\n")
    write_at(t, root, "utils/CMakeLists.txt", "set(DAS_UTILS\n    good-tool\n)\nset(DAS_UTILS_SHIPPED_EXES good-tool)\n")
    write_at(t, root, "utils/good-tool/main.das", "options gen2\n")
    write_at(t, root, "utils/good-tool/REVIEW.md", "stays in DAS_UTILS_SHIPPED_EXES\n")
    write_at(t, root, "utils/internal/tidy-tool/main.das", "options gen2\n")
    write_at(t, root, "doc/source/reference/utils/good_tool.rst", "page\n")
    write_at(t, root, "doc/source/reference/utils.rst", "   utils/good_tool.rst\n")
    write_at(t, root, "install/CLAUDE.md", "- `utils/good-tool/` - tool\n- `bin/` - exes (`good-tool.exe`)\n")
    var err : string
    t |> success(copy_file(path_join(tree, "utils/REVIEW.das"), path_join(root, "utils/REVIEW.das"), true, err), "copied the real gate")
}

[test]
def test_pilot_gate_fixtures(t : T?) {
    let tree = getcwd()
    if (!fexist(path_join(tree, "utils/REVIEW.das"))) {
        t |> skip("not run from the repo root")
        return
    }
    let fixture_r = create_temp_directory_result("review_pilot")
    t |> success(fixture_r is value, "temp fixture dir created")
    if (!(fixture_r is value)) return
    let fixture = unsafe(fixture_r.value)
    make_pilot_fixture(t, tree, fixture)

    let green = spawn_at(fixture, [path_join(fixture, "utils/REVIEW.das")])
    t |> equal(0, green.rc, "consistent fixture is green: {green.out}")
    t |> success(find(green.out, "REVIEW.das utils: OK") >= 0, "green verdict printed")

    // one violation per check class
    write_at(t, fixture, "utils/internal/Bad_Name/x.txt", "x\n")
    write_at(t, fixture, "utils/rogue-tool/main.das", "options gen2\n")
    write_at(t, fixture, "utils/good-tool/notes.md", "see utils/internal for helpers\n")
    write_at(t, fixture, "utils/CMakeLists.txt", "set(DAS_UTILS\n    good-tool\n)\nset(DAS_UTILS_SHIPPED_EXES good-tool ghost internal/sneak)\n")
    write_at(t, fixture, "install/CLAUDE.md", "- `utils/good-tool/` - tool\n- `bin/` - exes (`good-tool.exe`, `gone.exe`)\n")

    let red = spawn_at(fixture, [path_join(fixture, "utils/REVIEW.das")])
    t |> equal(1, red.rc, "planted fixture is red")
    for (needle in ["not kebab-case", "has no install rule", "no doc/source/reference/utils/rogue_tool.rst",
                    "no toctree row utils/rogue_tool.rst", "no layout row for external tool utils/rogue-tool/",
                    "references utils/internal", "must record the DAS_UTILS_SHIPPED_EXES decision",
                    "does not name ghost.exe", "has no DAS_UTILS entry", "is a path",
                    "is not an external tool directory", "names gone.exe"]) {
        t |> success(find(red.out, needle) >= 0, "finding fired: {needle}")
    }

    t |> success(rmdir_rec_result(fixture) is value, "fixture removed")
}

[test]
def test_all_and_main_walker_wiring(t : T?) {
    let tree = getcwd()
    let all_das = path_join(tree, "utils/internal/review-md/all.das")
    let main_das = path_join(tree, "utils/internal/review-md/main.das")
    if (!fexist(all_das)) {
        t |> skip("not run from the repo root")
        return
    }
    let fixture_r = create_temp_directory_result("review_walk")
    t |> success(fixture_r is value, "temp fixture dir created")
    if (!(fixture_r is value)) return
    let fixture = unsafe(fixture_r.value)
    let git = run_in(fixture, ["git", "init", "-q"])
    if (git.rc != 0) {
        t |> skip("git init unavailable: {git.out}")
        return
    }
    let green_gate = "options gen2\n\n[export]\ndef main() : int \{\n    return 0\n\}\n"
    let red_gate = "options gen2\n\n[export]\ndef main() : int \{\n    return 1\n\}\n"
    write_at(t, fixture, "a/REVIEW.md", "rules\n")
    write_at(t, fixture, "a/REVIEW.das", green_gate)

    let ok = spawn_at(fixture, [all_das])
    t |> equal(0, ok.rc, "green tree passes: {ok.out}")
    t |> success(find(ok.out, "1 gate(s), 0 red, 0 orphan(s)") >= 0, "summary line printed")

    write_at(t, fixture, "b/REVIEW.das", green_gate)   // gate with no checklist
    let orphaned = spawn_at(fixture, [all_das])
    t |> equal(1, orphaned.rc, "orphan turns the walk red")
    t |> success(find(orphaned.out, "b/REVIEW.das: REVIEW.das without a sibling REVIEW.md") >= 0, "orphan named")

    write_at(t, fixture, "a/REVIEW.das", red_gate)
    let failing = spawn_at(fixture, [all_das])
    t |> equal(1, failing.rc, "red gate turns the walk red")
    t |> success(find(failing.out, "1 red") >= 0, "red count in summary")

    // main.das: positional changed path, gate red -> exit 1 and no listing
    let gated = spawn_at(fixture, [main_das, "--", "a/x.das"])
    t |> equal(1, gated.rc, "red gate blocks the listing")
    t |> success(find(gated.out, "gate(s) red") >= 0, "fail-fix message printed")

    let listed = spawn_at(fixture, [main_das, "--", "a/x.das", "--list-only"])
    t |> equal(0, listed.rc, "--list-only ignores the red gate")
    t |> success(find(listed.out, "a/REVIEW.md") >= 0, "checklist listed")

    write_at(t, fixture, "a/REVIEW.das", green_gate)
    let clean = spawn_at(fixture, [main_das, "--", "a/x.das"])
    t |> equal(0, clean.rc, "green gate lets the listing through")
    t |> success(find(clean.out, "a/REVIEW.md") >= 0, "checklist listed after green gate")

    // a changed orphan REVIEW.das is a structural defect — red even under --list-only
    let orphan_hit = spawn_at(fixture, [main_das, "--", "b/REVIEW.das"])
    t |> equal(1, orphan_hit.rc, "changed orphan gate fails the walk")
    t |> success(find(orphan_hit.out, "without a sibling REVIEW.md") >= 0, "orphan named by main.das")
    let orphan_list = spawn_at(fixture, [main_das, "--", "b/REVIEW.das", "--list-only"])
    t |> equal(1, orphan_list.rc, "--list-only does not bypass the orphan check")

    t |> success(rmdir_rec_result(fixture) is value, "fixture removed")
}
