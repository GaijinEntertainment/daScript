# dasVulkan Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`.

**A `[test]` the diff adds or edits calls `volkInitialize()` - itself, or in a helper it calls
first - before anything it calls reaches a `vk*` function.** Nothing in the harness calls it, and
every `vk*` function is null until it runs.

**A `[test]` check compares a number a driver or OS query reported - count, limit, version or
size - only to another reported number or its no-answer value (zero, an empty list), never to a
threshold.** Machines differ; whether the query answered does not.
