# dasVulkan Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`.

**A `[test]` the diff adds or edits must call `volkInitialize()` - itself, or in a helper it
calls first - before anything it calls reaches a `vk*` function.** Nothing in the harness calls
it, and every `vk*` function is null until it runs.

**A `[test]` pass/fail check on a value a property, feature, limit, item-count (what a
`vkEnumerate*` or `vkGet*` call reports) or memory-budget query returned compares it to another
query's value or to the value the query returns when it has nothing to report, never to a fixed
number - a fixed threshold the test needs decides a skip instead.** A fixed expected value fails
on the next machine.

**A `[test]` check on a value the device computed or rendered - a query-pool result is one -
whose rounding, accumulation order or precision the Vulkan spec leaves to the device, passes
every value between two bounds, and the test states how it derives each bound - never a list of
values seen on devices.** The next device rounds another way.
