options gen2

require dastest/testing_boost public
require daslib/fio
require daslib/strings_boost
require strings

def private gate_findings_for(root : string; var rc : int&) : array<string> {
    let exe = get_command_line_arguments()[0]
    var output : string
    rc = run_and_capture([exe, "-dasroot", get_das_root(), "{get_das_root()}/tests/REVIEW.das", "--", "--root", root], output)
    var findings : array<string>
    for (line in split(output, "\n")) {
        let at = find(line, ": ")
        if (find(line, " writes `") >= 0 && at >= 0) {
            findings |> push(strip(slice(line, at + 2)))
        }
    }
    return <- findings
}

[test]
def test_tests_review_gate(tt : T?) {
    tt |> run("every write built from the repo root is a finding, and nothing else is") <| @(t : T?) {
        var rc = 0
        var inscope findings <- gate_findings_for("{get_das_root()}/tests/dastest/_tests_gate_fixture", rc)
        t |> equal(rc, 1)
        let expected = [
            "fopen(...) writes `out`",
            "fopen(...) writes `fixture_path(\"y.tmp\")`",
            "fopen(...) writes `\"relative.tmp\"`",
            "mkdir_rec(...) writes `GLOBAL_OUT`",
            "remove(...) writes `fixture_path(\"y.tmp\")`",
            "rename(...) writes `out`",
            "rename(...) writes `path_join(root, \"renamed.tmp\")`",
            "copy_file(...) writes `path_join(root, \"copied.tmp\")`",
            "fwrite(...) writes `path_join(get_das_root(), \"written.tmp\")`",
            "mkdir_rec(...) writes `server.upload_dir`",
            "save(...) writes `fixture_path(\"image.png\")`"
        ]
        for (e in expected) {
            t |> success(findings |> has_value_prefix(e), "missing: {e}\nfound:\n{join(findings, "\n")}")
        }
        t |> equal(length(findings), length(expected), "found:\n{join(findings, "\n")}")
    }
}

def private has_value_prefix(lines : array<string>; prefix : string) : bool {
    for (l in lines) {
        return true if (starts_with(l, prefix))
    }
    return false
}
