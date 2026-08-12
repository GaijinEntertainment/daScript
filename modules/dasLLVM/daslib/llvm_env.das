options gen2

//! dasLLVM's environment knobs, declared as `[EnvConfig]` structs (daslib/clargs) — one field
//! per variable, the doc line on the declaration. `ENVIRONMENT.md` is GENERATED from these
//! (`harness/gen_env_doc.das`) and `tests/llvm_env_registry.das` fails when a variable is read
//! anywhere in the module without a declaration here.
//!
//! Two loading disciplines, split by struct: `JitEnv` loads ONCE at context init into
//! `g_env_jit` (the knobs are process-stable and the DLL/obj cache keys fold them). `TuneEnv`
//! deliberately has NO loaded global: the tuner's contract is set-then-latch — a harness (and
//! the tune tests) may `set_env_variable` a `DAS_TUNE_*` knob in-process before the first read
//! latches it — so its reads stay live through `env_value_of`, and the struct exists for the
//! doc rows and the coverage scan.

module llvm_env shared public

require daslib/clargs
require daslib/env_registry public
require daslib/rtti

// ===== JIT =====

[EnvConfig(env_prefix = "DAS")]
struct public JitEnv {
    @clarg_doc = "Split-JIT LTO probe: partitions emit bitcode and the link runs lld LTO. Dev instrument for measuring cross-module-inlining recovery; needs --jit-split-modules + --jit-obj-cache=0, announces itself, and folds into the cache keys so a probe artifact never serves a normal run."
    jit_probe_lto : bool = false

    @clarg_doc = "Split-JIT key forensics: log every (partition, mangled name, aot hash) the obj-cache chain folds. Diff two runs to locate WHERE a key diverged - distinguishes a changed hash from a changed fold order."
    jit_dump_hashes : bool = false

    @clarg_doc = "Comma-separated x64 CPU features to force on (e.g. avx2,f16c), bypassing detection; LLVM target-feature spellings. Also satisfies cpu_supports-based tune eligibility. Executing a forced instruction the host lacks is an illegal instruction, not a diagnostic."
    jit_x64_force_features : string = ""

    @clarg_doc = "The arm64 twin (e.g. dotprod,i8mm)."
    jit_arm64_force_features : string = ""
}

let public g_env_jit = env_config(type<JitEnv>)

// ===== Kernel tuning =====
// NO g_env_tune on purpose - see the module docstring. Read these via env_value_of/env_is_set
// so a harness's in-process set_env_variable is still seen by the first latching read.

[EnvConfig(env_prefix = "DAS")]
struct public TuneEnv {
    @clarg_default_doc = "normal"
    @clarg_doc = "Compile-time tune mode: normal, tune, or test."
    tune_mode : string = ""

    @clarg_path
    @clarg_default_doc = "<app>.tune.json beside the app"
    @clarg_doc = "The tune sidecar to read and write - how you tune when the application directory is read-only. Setting it also gates the tune policy off (a redirect means manual)."
    tune_manifest : string = ""

    @clarg_path
    @clarg_default_doc = "<home>/.tune-history/<box>/"
    @clarg_doc = "Where mint archives land. Every mint archives (failures kept, marked .FAILED.json); the history is never deleted."
    tune_history : string = ""

    @clarg_default_doc = "2 normal / 1 paranoid"
    @clarg_doc = "The noise gate's probe-cv ceiling in percent - a calibration lever. Garbage or <= 0 reads as unset."
    tune_noise_cv : float = 0.0

    @clarg_doc = "Mint through a refusal at either gate - a failing noise probe or a failing validation verdict. The sidecar is stamped overridden, so the escape always leaves a mark."
    tune_noise_override : bool = false

    @clarg_default_doc = "normal"
    @clarg_doc = "What a tune shows: silent, normal, or verbose. Anything unrecognized reads as normal, so a typo never silences a tune."
    tune_verbosity : string = ""

    @clarg_default_doc = "declared by [tune_policy]"
    @clarg_doc = "Override the missing-scope policy: fallback, warn, error, auto, or restart. The announce line says when the environment shaped the policy."
    tune_policy : string = ""
}

// ===== the generated documentation (ENVIRONMENT.md) =====

// Variables dasLLVM reads but does not own - documented so the coverage scan knows them.
def private ambient_rows() : array<EnvDocRow> {
    return <- [
        EnvDocRow(name = "HOME", kind = "path", dflt = "unset",
            doc = "The tune-history home (USERPROFILE on Windows when unset)."),
        EnvDocRow(name = "USERPROFILE", kind = "path", dflt = "unset",
            doc = "Windows fallback for the tune-history home."),
        EnvDocRow(name = "PROCESSOR_IDENTIFIER", kind = "text", dflt = "unset",
            doc = "Half of the Windows tune box identity (measurements are a property of the box AND its OS)."),
        EnvDocRow(name = "OS", kind = "text", dflt = "unset",
            doc = "The other half of the Windows tune box identity.")
    ]
}

//! Renders every declared knob as the `ENVIRONMENT.md` body. `modules/dasLLVM/ENVIRONMENT.md`
//! is this output verbatim - `harness/gen_env_doc.das` writes it and
//! `tests/llvm_env_registry.das` fails when the checked-in file drifts from it.
def public env_markdown() : string {
    return build_string() $(var w) {
        w |> write("# dasLLVM environment variables\n\n")
        w |> write("GENERATED from the `[EnvConfig]` declarations in `daslib/llvm_env.das` - do not edit by\n")
        w |> write("hand. Regenerate with `daslang modules/dasLLVM/harness/gen_env_doc.das`; a variable read\n")
        w |> write("anywhere in the module without a declaration fails `tests/llvm_env_registry.das`.\n\n")
        write_env_doc_semantics(w)
        write_env_section(w, "JIT",
            "Loaded once at context init into `g_env_jit`. The force-features pair exists to reproduce another box's codegen locally or check that a feature-gated kernel is the one being measured; both override `cpu_supports`, which `requires=` on a `[tune_perm]` and `suffix:requires` fallback entries match against.")
        write_env_info_rows(w, get_env_config_info(type<JitEnv>))
        write_env_section(w, "Kernel tuning",
            "LIVE-READ, not context-init loaded: the tuner's set-then-latch contract lets a harness `set_env_variable` a knob in-process before the first read latches it. The framework itself is documented in `skills/llvm_tune.md`.")
        write_env_info_rows(w, get_env_config_info(type<TuneEnv>))
        write_env_section(w, "Ambient variables dasLLVM reads but does not own", "")
        var extras <- ambient_rows()
        for (e in extras) {
            write_env_row(w, e)
        }
        delete extras
    }
}

//! Every declared name, in documentation order - the coverage test consumes this.
def public registered_env_names() : array<string> {
    var out : array<string>
    out |> reserve(16)
    push_info_names(out, get_env_config_info(type<JitEnv>))
    push_info_names(out, get_env_config_info(type<TuneEnv>))
    var extras <- ambient_rows()
    for (e in extras) {
        out |> push(e.name)
    }
    delete extras
    return <- out
}
