options gen2
options stack = 524288
options _dasllama_internal = true

require gen_tune_probe
require tune_kernels
require llvm/daslib/llvm_tune
require daslib/tune
require daslib/export_c
require class_mint_text

//! The mint of a CPU class as one call, for a target with no tuner process to spawn and whose
//! file IO is its host's own - a console. Built as a library under `DAS_TUNE_MODE=tune`, the class's
//! `DAS_JIT_BASELINE` and the target's `--jit-target`, so the grid it carries is the class's rows.

[export_c, tune_policy(missing = "fallback")]
def class_mint() : string {
    //! Runs the generator tuner's test gate and race, then the loop-hint tuner, the sidecar
    //! kept in memory. The text is one JSON object: `profile`, the class profile to commit, and
    //! `sidecar`, the whole mint with its race tables; `error` alone where a step failed.
    set_tune_manifest_in_memory()
    return mint_failure_text("the generator test gate failed") if (!gen_tune_probe::test_mode())
    return mint_failure_text("the generator race failed") if (!gen_tune_probe::tune_mode_run())
    return mint_failure_text("the loop-hint tuner failed") if (tune_kernels::main() != 0)
    let profile = tune_profile_text(tune_mint_target().klass)
    return mint_failure_text("the mint wrote no kernels") if (empty(profile))
    return mint_result_text(profile, tune_manifest_text())
}
