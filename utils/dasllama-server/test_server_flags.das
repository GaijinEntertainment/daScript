options gen2
options persistent_heap

require dastest/testing_boost public
require main                                            // the program root: cli_has_flag, mtp_setting, mtp_default_resolved (sibling by bare name)
require openai_server                                   // mtp_auto_arms, and serving_knobs' Metal-mode pair through it
require dasllama/dasllama                               // nolint:STYLE029 - MetalMode through the facade, the one door into the engine
require daslib/clargs                                   // get_user_args / flag_present: the match cli_has_flag rests on
require strings

// Model-free unit coverage for the flag-presence test behind the config surface's provenance:
// a flag the process was given reads present, in both spellings, and an absent one reads absent;
// and for the self-speculation default's rule, as the slot and the config surface read it.
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

[test]
def test_mtp_setting_tri_state(t : T?) {
    t |> run("a key the TOML or --mtp set pins 0 / 1; unset is -1 and each slot decides") @(t : T?) {
        t |> equal(mtp_setting(true, true), 1, "mtp = true")
        t |> equal(mtp_setting(true, false), 0, "mtp = false")
        t |> equal(mtp_setting(false, false), -1, "unset: the slot decides")
        t |> equal(mtp_setting(false, true), -1, "an unsourced true is still unset")
    }
    t |> run("the unset default resolves on for one stream on a GPU, off on the CPU or at several streams") @(t : T?) {
        t |> success(mtp_default_resolved(1, "metal", ""), "one stream on Metal")
        t |> success(mtp_default_resolved(1, "metal-required", ""), "one stream on required Metal")
        t |> success(mtp_default_resolved(1, "vulkan", ""), "one stream on a Vulkan device")
        t |> success(mtp_default_resolved(1, "", "auto"), "one stream on the legacy --metal auto")
        t |> success(!mtp_default_resolved(1, "", ""), "one stream, no device")
        t |> success(!mtp_default_resolved(1, "off", "required"), "one stream on the CPU, whatever --metal says")
        t |> success(!mtp_default_resolved(4, "metal", ""), "four streams on Metal")
        t |> success(!mtp_default_resolved(4, "vulkan", ""), "four streams on a Vulkan device")
    }
    t |> run("a slot arms on one stream whose decode runs on a GPU") @(t : T?) {
        t |> success(mtp_auto_arms(1l, true, "cpu"), "the Metal flavor - its tier word reads cpu")
        t |> success(mtp_auto_arms(1l, false, "gpu:resident"), "the whole model resident on a Vulkan device")
        t |> success(!mtp_auto_arms(1l, false, "gpu:rails"), "a model the device serves in part")
        t |> success(!mtp_auto_arms(1l, false, "cpu"), "the CPU")
        t |> success(!mtp_auto_arms(4l, true, "cpu"), "four streams on Metal")
        t |> success(!mtp_auto_arms(4l, false, "gpu:resident"), "four streams on a Vulkan device")
    }
    t |> run("the Metal mode: the --gpu pick when there is one, else the legacy --metal mode") @(t : T?) {
        t |> success(resolved_metal_mode(MetalMode.when_available, "vulkan") == MetalMode.off, "a --gpu pick wins over --metal")
        t |> success(resolved_metal_mode(MetalMode.required, "off") == MetalMode.off, "gpu = off keeps Metal out")
        t |> success(resolved_metal_mode(MetalMode.off, "metal") == MetalMode.when_available, "gpu = metal")
        t |> success(resolved_metal_mode(MetalMode.off, "metal-required") == MetalMode.required, "gpu = metal-required")
        t |> success(resolved_metal_mode(MetalMode.required, "") == MetalMode.required, "no --gpu pick: the legacy mode stands")
    }
    t |> run("a --metal spelling names its mode in any case, and an unknown one names none") @(t : T?) {
        t |> success(metal_mode_of("Required").mode == MetalMode.required && metal_mode_of("Required").known)
        t |> success(metal_mode_of("auto").mode == MetalMode.when_available && metal_mode_of("when_available").known)
        t |> success(metal_mode_of("").mode == MetalMode.off && metal_mode_of("").known && metal_mode_of("off").known)
        t |> success(metal_mode_of("fast").mode == MetalMode.off && !metal_mode_of("fast").known)
    }
}
