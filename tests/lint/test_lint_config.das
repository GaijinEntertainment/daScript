options gen2

require dastest/testing_boost
require daslib/lint_config
require fio
require strings

def fixture_path() : string {
    return "{get_das_root()}/tests/lint/_lint_config_fixture.txt"
}

def write_fixture(content : string) {
    fopen(fixture_path(), "wb") $(f) {
        if (f != null) {
            fwrite(f, content)
        }
    }
}

[test]
def test_missing_file_no_op(t : T?) {
    t |> run("missing file leaves disabled_codes untouched") @@(t : T?) {
        var d : table<string>
        load_lint_config_from_path("/no/such/lint_config_path_xyz", d)
        t |> equal(length(d), 0)
    }
}

[test]
def test_off_inserts(t : T?) {
    t |> run("'STYLE100 off' inserts the code") @@(t : T?) {
        write_fixture("STYLE100 off\n")
        var d : table<string>
        load_lint_config_from_path(fixture_path(), d)
        t |> equal(length(d), 1)
        t |> success(key_exists(d, "STYLE100"))
    }
}

[test]
def test_on_erases_default(t : T?) {
    t |> run("'STYLE005 on' erases a preloaded default") @@(t : T?) {
        write_fixture("STYLE005 on\n")
        var d : table<string>
        d |> insert("STYLE005")
        load_lint_config_from_path(fixture_path(), d)
        t |> equal(length(d), 0)
        t |> success(!key_exists(d, "STYLE005"))
    }
}

[test]
def test_comments_and_blanks_skipped(t : T?) {
    t |> run("# comments, blank lines, and inline '#' are ignored") @@(t : T?) {
        write_fixture("# header line\n\nSTYLE100 off  # inline comment\n   \n")
        var d : table<string>
        load_lint_config_from_path(fixture_path(), d)
        t |> equal(length(d), 1)
        t |> success(key_exists(d, "STYLE100"))
    }
}

[test]
def test_malformed_lines_silently_skipped(t : T?) {
    t |> run("garbage / one-token / unknown-verb / extra-tokens are silently skipped") @@(t : T?) {
        write_fixture("garbage\nSTYLE100\nSTYLE100 maybe\nSTYLE100 on extra\n")
        var d : table<string>
        load_lint_config_from_path(fixture_path(), d)
        t |> equal(length(d), 0)
    }
}

[test]
def test_on_for_unset_code_is_noop(t : T?) {
    t |> run("'X on' when X is not in the set is harmless") @@(t : T?) {
        write_fixture("STYLE999 on\n")
        var d : table<string>
        load_lint_config_from_path(fixture_path(), d)
        t |> equal(length(d), 0)
    }
}
