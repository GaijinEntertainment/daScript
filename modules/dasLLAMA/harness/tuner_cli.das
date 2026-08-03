options gen2

module tuner_cli shared public

//! The flag spec shared by the dasllama tuner and both of its halves, so one declaration is
//! the contract for all three. Two race modes only: default and ``--tune-paranoid`` — a fast
//! RACE is a systematic failure to measure (12-sample medians cannot resolve sub-2% twins);
//! the debugging concession is `daspkg release --quick`, which accepts the existing sidecar
//! and races nothing.

require daslib/clargs

[CommandLineArgs]
struct TunerArgs {
    @clarg_name = "tune-paranoid"
    @clarg_doc = "Triple the round budget, tighten the noise gate to 1%, and tighten validation"
    tune_paranoid : bool
}

def tuner_paranoid_requested() : bool {
    //! True when ``--tune-paranoid`` is on this tuner's command line. A parse failure reads
    //! as "not paranoid" — the normal budget, not a silent 3x surprise.
    var r <- parse_args(type<TunerArgs>, get_user_args())
    if (r |> is_err) {
        return false
    }
    let cfg <- r |> move_unwrap
    return cfg.tune_paranoid
}
