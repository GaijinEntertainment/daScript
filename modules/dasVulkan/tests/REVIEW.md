# dasVulkan Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`.

**A diff that adds a `[test]` under `integration/` puts it in the existing category file for its
topic or in the folder's uncategorized file (`tests/ARCHITECTURE.md` names it), never in a new
file, with a `description`, and with `timeout = 1200` when the test builds a pipeline from source
or spawns a child; a tutorial's pixel oracle stays the one test beside its tutorial, marked
`standalone`** - the shape rules are `tests/REVIEW.md` (repo root) and `tests/ARCHITECTURE.md`;
`REVIEW.das` (beside this file) applies them here and, tests-only, to `../tutorials/`. The
generated indexes are `README.md` (beside this file) and `../tutorials/TESTS.md` (`daslang
utils/internal/test-audit/main.das -- --root modules/dasVulkan/tests --readme`, and `--root
modules/dasVulkan/tutorials --tests-only --index modules/dasVulkan/tutorials/TESTS.md --readme`);
the `check_tests_index` target verifies both.

**A diff that adds a `.das` file under `integration/` that is not itself a test - a shader
module or a scene helper the suites require - starts its name with `_`** - dastest skips a `_`
name when it scans the directory, so such a file never runs as a test of its own.

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
