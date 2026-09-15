options gen2
options indenting = 4
options no_aot // compiles fixtures through the real compiler at runtime

require dastest/testing_boost
require daslib/ast
require daslib/ast_boost
require daslib/rtti
require strings
require daslib/strings_boost
require daslib/fio

// The declarative catalog must reduce the built-in identity shapes exactly as the C++ arms do.
// Both fixtures pull the same targets; one leaves the built-in arms on, the other stands them down
// and arms the rules instead.

def fixture(name : string) : string {
    return "{get_das_root()}/tests/fold_rules/{name}"
}

def collect_bodies(t : T?; name : string; var out : table<string; string>) {
    var inscope access <- make_file_access("")
    using() $(var mg : ModuleGroup) {
        using() $(var cop : CodeOfPolicies) {
            cop.export_all = true
            compile_file(fixture(name), access, unsafe(addr(mg)), cop) $(ok; program; issues) {
                if (!ok) {
                    t |> failure("{name} failed to compile: {issues}")
                    return
                }
                for_each_module(get_ptr(program)) $(mod) {
                    for_each_function(mod, "") $(func) {
                        let nm = "{func.name}"
                        if (nm |> starts_with("p_")) {
                            out[nm] = "{describe(func.body)}"
                        }
                    }
                }
            }
        }
    }
}

def target_lines(t : T?; name : string) : array<string> {
    var text = ""
    fopen(fixture(name), "rb") $(f) {
        if (f == null) {
            t |> failure("cannot read {name}")
            return
        }
        fmap(f) $(data) {
            text = string(data)
        }
    }
    return <- [for (ln in split(text, "\n")); ln; where ln |> starts_with("[export, never_inline]")]
}

[test]
def test_both_fixtures_hold_the_same_targets(t : T?) {
    var a <- target_lines(t, "_fixture_parity_builtin.das")
    var b <- target_lines(t, "_fixture_parity_rules.das")
    t |> success(length(a) > 20, "the builtin fixture declares {length(a)} targets")
    t |> equal(join(b, "\n"), join(a, "\n"), "the two parity fixtures drifted apart")
    delete a
    delete b
}

[test]
def test_the_catalog_matches_the_builtin_fold(t : T?) {
    var builtin : table<string; string>
    var rules : table<string; string>
    collect_bodies(t, "_fixture_parity_builtin.das", builtin)
    collect_bodies(t, "_fixture_parity_rules.das", rules)
    t |> success(length(builtin) > 20, "the builtin fixture produced {length(builtin)} targets")
    t |> equal(length(rules), length(builtin), "both fixtures produce the same target set")
    for (nm, body in keys(builtin), values(builtin)) {
        let mine = rules?[nm] ?? "<missing>"
        t |> equal(mine, body, "{nm} differs")
    }
    delete builtin
    delete rules
}
