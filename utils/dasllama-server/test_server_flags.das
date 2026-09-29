options gen2
options persistent_heap

require dastest/testing_boost public
require main                                            // the program root: cli_has_flag (sibling by bare name)
require daslib/clargs                                   // get_user_args / flag_present: the match cli_has_flag rests on
require strings

// Model-free unit coverage for the flag-presence test behind the config surface's provenance:
// a flag the process was given reads present, in both spellings, and an absent one reads absent.
// Runs fully in CI - no GGUF, no JIT.
//! the process-argument cell skips when the test process was given no `--` flag

[test]
def test_flag_match(t : T?) {
    t |> run("a flag matches bare and as flag=value, never as a prefix of another flag") @(t : T?) {
        let args <- ["-m", "model.gguf", "--flat", "--mtp=false", "--streams", "1"]
        t |> success(flag_present(args, "--flat"), "a bare flag")
        t |> success(flag_present(args, "--mtp"), "the flag=value spelling")
        t |> success(flag_present(args, "--streams"), "a flag whose value is the next argument")
        t |> success(flag_present(args, "-m"), "a short flag")
        t |> success(!flag_present(args, "--fla"), "a prefix of a given flag is absent")
        t |> success(!flag_present(args, "--mtp=true"), "a flag=value the list does not hold is absent")
        t |> success(!flag_present(args, "--gpu-dn"), "a flag the list does not hold is absent")
        let none : array<string>
        t |> success(!flag_present(none, "--flat"), "an empty list holds no flag")
    }
}

[test]
def test_cli_has_flag(t : T?) {
    t |> run("a flag this process was given reads present, one it was not given reads absent") @(t : T?) {
        var given = ""
        var args <- get_user_args()
        for (a in args) {
            if (empty(given) && a |> starts_with("--") && find(a, "=") < 0) {
                given = a
            }
        }
        delete args
        if (empty(given)) {
            t |> skip("the test process was given no -- flag")
            return
        }
        t |> success(cli_has_flag(given), "{given} is present")
        t |> success(!cli_has_flag("{given}-never-given"), "{given}-never-given is absent")
    }
}
