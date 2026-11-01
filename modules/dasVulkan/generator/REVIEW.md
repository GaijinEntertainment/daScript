# dasVulkan Generator Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`. Planned work: `../ROADMAP.md`.

**A diff that adds a name to any section of `skip_report.txt` names in its commit message the
generated wrapper that still exposes the newly skipped struct or command - or states that none
does.** `check_generated.cmake` catches the changed report, not whether the loss is intended.

**A diff that adds a name to `copyability_report.txt` states in its commit message that no
hand-written array literal of the newly non-copyable struct anywhere in the repository is
followed by a read of a local the literal moved from.** das moves a non-copyable element out of an
array literal and zeroes the caller's local, so a struct newly listed here changes those call
sites silently.
