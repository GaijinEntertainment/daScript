# watchdog Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `README.md`.

**A diff that changes the names `find_daslang` (`watchdog.das`) tries, or the roots any caller
hands it - a new caller included - updates every text in `watchdog.das` and `README.md` that
names a path `find_daslang` tries, in the same change** - a text that names paths the
supervisor no longer tries sends the user to the wrong folder.

**A diff that adds a field key to a JSON-lines log line describes it in the `## The log`
section of `README.md`, in the same change** - that section is the list a rename sweep starts
from.

**A diff that adds an `event` value or a field key to the log from any `.das` in this folder
uses the name another file here already writes for the same thing** - one meaning under two
spellings matches a log reader on only one of the two, and the miss is silent.

**Weakening `REVIEW.das` (beside this file) is a defect: dropping a check, narrowing what a
check walks, or rewriting a finding text so it no longer names what failed.**

**A diff that renames or removes a name the supervisor writes or reads - an `event` value on
the JSON-lines log, a field key on such a line, a startup stage name, or a `@tune` kind or key
on the `@tune <kind> k=v` lines `watchdog.das` reads from the child - updates every place that
name appears in the tree in the same change, and names any out-of-tree reader in the PR
description** - a reader that stops seeing the name it waits for never fails loudly.

**A diff that adds a `WatchdogConfig` field makes it discoverable in the same change: a flag
field gets a `@clarg_doc`; a `@clarg_skip` field gets its key exemption in `config_to_args` and
its line in `README.md`.** `--help` lists a flag field only through its `@clarg_doc`, and
`watchdog.json` keys are validated against the same field list.

**A diff that adds a switch, or a value of a switch, read before the config (`wants_mode` in
`main.das`, `tool_front` in `watchdog.das`) adds or updates its line in `print_watchdog_help`
or `README.md`, in the same change.**

**A diff that removes or renames a `WatchdogConfig` field updates every `watchdog.json` in the
tree in the same change, and names any out-of-tree `watchdog.json` in the PR description** -
an unknown key refuses the start, so a stale key in a bundled config is a supervisor that never
comes up.

**A diff that adds a `require` to a `.das` in this folder for an optional module - one a build
can leave out, so `has_module` reports it absent - adds that module, in the same change, to the
`watchdog` arm of `tests/.das_test` and to the `if(TARGET ...)` guard and the link line of the
`watchdog` target in `utils/CMakeLists.txt` (both repo root)** - a build that leaves the module
out otherwise fails: the whole test suite at compile, the target at link instead of being
skipped.

**A diff that makes the tray or a notification depend on something the host machine may not
have - a call into `stddlg`, a spawned program that shows something on the desktop, or a file
the icon path reads - logs that dependency's failure as an event on the JSON-lines log and lets
the supervisor start and keep supervising** - the supervisor must run on a machine with no
desktop and with a missing or unreadable icon file.
