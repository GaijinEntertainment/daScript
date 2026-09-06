# watchdog Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`.

**A diff that renames or removes a log `event`, one of its keys, or a startup stage name in
`watchdog.das` names every reader of it in the same change** - the JSON-lines log is the wire
the deploy scripts, the control pages and the crash bundles read, and a reader that stops
seeing `crash` never fails loudly.

**A diff that adds a field to `WatchdogConfig` gives it a `@clarg_doc`** - the flag table is the
whole of `--help`, and `watchdog.json` keys are validated against the same field list, so an
undocumented field is a key an operator cannot discover.
