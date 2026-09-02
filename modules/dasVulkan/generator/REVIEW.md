# dasVulkan Generator Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`.

**A diff that grows `skip_report.txt` names in its commit message the generated wrapper that
still exposes the newly skipped struct or command - or states that none does.**
`.github/workflows/vulkan_checks.yml` checks only that the committed report matches a fresh run;
it does not judge whether the loss is intended.

**A diff that grows `copyability_report.txt` states in its commit message that every
array-literal call site of the newly non-copyable struct was re-checked.** das moves a
non-copyable element out of an array literal, so the struct's arrival changes those call sites
silently.
