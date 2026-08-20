options gen2

module tuner_cli shared public

//! The flag spec shared by the dasllama tuner and both of its halves, so one declaration is
//! the contract for all three. One race protocol: ``--tune-paranoid`` is accepted and runs
//! it; the debugging concession is `daspkg release --quick`, which accepts the existing
//! sidecar and races nothing.

require dasllama/dasllama_lint public
require daslib/clargs

[CommandLineArgs]
struct TunerArgs {
    @clarg_name = "tune-paranoid"
    @clarg_doc = "Accepted for compatibility; the tuner runs one margin-decided protocol and this flag no longer changes the budget"
    tune_paranoid : bool
}

def tuner_paranoid_requested() : bool {
    //! True when ``--tune-paranoid`` is on this tuner's command line (a parse failure reads
    //! as not set). The flag changes nothing today; the halves only note it.
    var r <- parse_args(type<TunerArgs>, get_user_args())
    if (r |> is_err) {
        return false
    }
    let cfg <- r |> move_unwrap
    return cfg.tune_paranoid
}
