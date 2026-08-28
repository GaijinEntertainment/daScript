# dasVulkan Generator Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`.

**A diff that grows `skip_report.txt` states in its commit message why losing the newly skipped
structs and commands is acceptable.** CI checks only that the committed report matches a fresh
run; it does not judge whether the loss is intended.
