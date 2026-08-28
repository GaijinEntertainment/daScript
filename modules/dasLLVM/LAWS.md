# LAWS - ruling provenance (append-only; not a rule document)

- 2026-08-27 - `REVIEW.md` - Boris kept the broad codegen-version trigger against a proposal
  to narrow it to cached artifacts: "i'd rather bump. sometimes hard to say if it changes
  emission or not. and cache is not my concern." He also deleted the re-pin carve-out clause
  outright rather than trimming it: "we are saving very little time. jited dll is not exactly
  a lot of value... world won't end." Platform clause on the module-suite rule: "yes. its a
  good rule." Routing line for the pinned emitter hash accepted with the small/ pin-rule
  rebind ("sure.").

- 2026-08-27 - `REVIEW.md` - the `LLVM_JIT_EMITTER_HASH` routing line added earlier today
  was removed with the pinned-value arm it routed (dragon duplicate finding; Boris: "okie").
  The `[tune]` tail of the codegen-version rule was re-mooded from an exemption to
  trigger-narrowing (dragon REMOVE EXCEPTIONS; wording applied verbatim).
