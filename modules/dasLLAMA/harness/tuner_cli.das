options gen2

module tuner_cli shared public

//! The flag spec shared by the dasllama tuner and both of its halves, so one declaration is the
//! contract for all three. `daspkg release --tune-fast` and `dasllama_tuner` both emit
//! ``--tune-fast``; before this module each consumer hand-rolled its own scan of argv, and
//! ``gen_tune_probe`` had no scan at all, so it silently ignored the flag it was passed.

require daslib/clargs

[CommandLineArgs]
struct TunerArgs {
    @clarg_name = "tune-fast"
    @clarg_doc = "Screen progressively and cap the round budget instead of the full sweep"
    tune_fast : bool
}

def tuner_fast_requested() : bool {
    //! True when ``--tune-fast`` is on this tuner's command line. A parse failure reads as
    //! "not fast" — the slower sweep is always the safe answer.
    var r <- parse_args(type<TunerArgs>, get_user_args())
    if (r |> is_err) {
        return false
    }
    let cfg <- r |> move_unwrap
    return cfg.tune_fast
}
