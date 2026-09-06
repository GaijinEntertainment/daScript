# watchdog Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`.

**A diff that renames or removes a name on the JSON-lines log - an `event` value, a key of one,
or a startup stage name - updates every reader in the same change: the event list and the stage
list in `README.md`, `smoke_test.cmake`, and `tests/watchdog/test_watchdog.das` (repo root), and
names any out-of-tree reader in the PR description** - the log is the wire everything
downstream reads, and a reader that stops seeing the event it waits for never fails loudly.

**A diff that adds a field to `WatchdogConfig` makes it discoverable in the same change: a
`@clarg_doc` on a flag field, or - for a `@clarg_skip` field - its key exemption in
`config_to_args` and its line in `README.md`** - the flag table is the whole of `--help`, and
`watchdog.json` keys are validated against the same field list.

**A diff that removes or renames a `WatchdogConfig` field updates every `watchdog.json` in the
tree in the same change** - an unknown key refuses the start, so a stale key in a bundled
config is a supervisor that never comes up.
