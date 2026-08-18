options gen2

module tuner_phrases shared public

// Sidecar provenance phrases (zero-require leaf, like dasllama_version): every value written
// into provenance reaches the exchange verbatim, so the validator's render charset binds the
// WRITER — test_exchange_schema pins these through validate_sidecar_submission.

//! The noise probe's settle note: the probe opened at `cv0`, re-settled at `cv`.
def settle_note(tag : string; cv0, cv : double) : string {
    return "{tag} cv {cv0:0.2f}% settled to {cv:0.2f}%"
}
