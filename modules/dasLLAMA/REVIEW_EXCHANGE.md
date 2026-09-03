# dasLLAMA Sidecar-Exchange Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `ARCHITECTURE_ENGINE.md`.

**Routed from `REVIEW.md`: a change to the sidecar-exchange client
(`dasllama/dasllama_exchange.das`), or to a tune-boot path that reaches it, applies this
list together with `REVIEW.md`'s.**

**Never add a second HTTP path to the sidecar exchange - every HTTP call (lookup, download,
submit) goes through `dasllama/dasllama_exchange.das`.** The mechanical half (no second
`dashv` requirer in the engine; `harness/` is out of its scope, since a measurement harness
talks HTTP to a reference server, never to the exchange) is `performance/REVIEW.das`'s to
enforce.

**Weakening the exchange download gate, the submission strip, or the submit rails is a
defect.** The download gate checks content sha, schema, and `DASLLAMA_RELEASE`. The submit
rails stop a sidecar that came from the exchange, or was minted on another box, from being
submitted. `utils/dasllama-server/test_exchange_client.das` enforces the download gate, the
strip, and the rails.

**A diff that adds a submission path around `exchange_strip_private` is a defect, even where
the strip itself is intact.**

**A tune-boot path (`exchange_scope_resolver` / `exchange_boot_submit_check`,
`dasllama/dasllama_exchange.das`) that fails when `exchange_lookup` fails is a defect - it
falls through to the local sidecar and the baked winners.**

**A diff that adds an outbound exchange request reachable from a tune-boot path
(`exchange_scope_resolver` / `exchange_boot_submit_check`) without routing it through
`exchange_may_contact` (`dasllama/dasllama_exchange.das`) is a defect.**

**A diff that adds a tune-boot-path (`exchange_scope_resolver` /
`exchange_boot_submit_check`) consent question with no terminal to ask on also emits that
question as a `@sidecar` event, in the same change** - the watchdog dialog and the control
page are the answer surfaces a supervised boot has.

**A diff that changes the exchange consent notice wording updates every checked-in copy in
the same change, and a diff that adds a copy names it here in the same change:
`EXCHANGE_CONSENT_NOTICE` (`dasllama/dasllama_exchange.das`), `CONSENT_TITLE` /
`CONSENT_TEXT` (`utils/watchdog/watchdog.py`, repo root), and the captured
`utils/dasllama-server/tests/fixtures/exchange.json` (repo root).** The console prompt and the
control page render the served constant, so they are not copies.
