# dasVulkan Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`.

**A `[test]` the diff adds or edits calls `volkInitialize()` - itself, or in a helper it calls
first - before anything it calls reaches a `vk*` function.** Nothing in the harness calls it, and
every `vk*` function is null until it runs.

**A `[test]` check compares a value a driver or OS query returned - a property, feature bit,
limit, count or budget, never a rendered or computed result - only to another query's value or to
its no-answer value (zero, an empty list); a fixed value may pick a skip or another query's
expected answer.** Machines differ; whether a query answered does not.
