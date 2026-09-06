# watchdog Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`.

**A diff that renames or removes a name the supervisor writes or reads - an `event` value on
the JSON-lines log, a field key on such a line, a startup stage name, or a `@tune` kind or key
on the `@tune <kind> k=v` lines `watchdog.das` reads from the child - updates every place that
name appears in the tree in the same change, and names any out-of-tree reader in the PR
description** - a reader that stops seeing the name it waits for never fails loudly.

**A diff that adds a field to `WatchdogConfig` makes it discoverable in the same change: a
`@clarg_doc` on a flag field, or - for a `@clarg_skip` field - its key exemption in
`config_to_args` and its line in `README.md`** - the flag table is the whole of `--help`, and
`watchdog.json` keys are validated against the same field list.

**A diff that removes or renames a `WatchdogConfig` field updates every `watchdog.json` in the
tree in the same change** - an unknown key refuses the start, so a stale key in a bundled
config is a supervisor that never comes up.

**A diff that adds a `require` to `watchdog.das` for a module `has_module` can report absent
adds that module to the `watchdog` arm of `tests/.das_test` (repo root) in the same change** -
without the entry the whole test suite fails to compile on a machine where that module is
missing.

**A diff that reaches the notification area or the desktop from the supervision path logs the
reason the call failed and starts and keeps supervising anyway** - the supervisor must run on
a machine with no desktop.
