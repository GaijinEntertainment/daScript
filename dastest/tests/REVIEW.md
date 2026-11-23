# dastest Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`../README.md`.

**A `[test]` in a file here whose path holds no `_`-leading name answers to `tests/REVIEW.md` (repo
root) too, whose shape rules `REVIEW.das` (beside this file) applies.**

**A diff that adds a `[test]` to a file here whose path holds no `_`-leading name puts it in the
existing file for its topic, or in `uncategorized.das` where no file holds that topic, never in a
new file.**

**A diff that adds a `[test]` here that spawns a child dastest declares `timeout = 1200` on it** -
a run started with `--test-wall` ends a test that declares no longer budget at that wall, and a
child dastest compiles a program of its own.

**A diff that adds a `.das` file here that is not itself a test - a fixture another test runs, or a
module a fixture requires - starts its name, or the name of a directory on its path under this
folder, with `_`** - dastest skips a `_` name anywhere on the path when it scans a directory, so
such a file never runs as a test of its own.

**A diff that adds a test for dastest itself, or changes how one spawns a process or checks an
exit code, runs it on Windows and Linux before it lands, or the PR body names each of those two
it was not run on** - the `extended_checks` workflow runs the dastest tests on Linux alone per
pull request; Windows sees them nightly.
