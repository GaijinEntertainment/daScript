# dasVulkan Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`.

**A diff that adds a `[test]` under `integration/` reaches `volkInitialize()` before its first
Vulkan call - in the `[test]` function, or in a helper that function calls first.** Nothing in
the harness calls it, and every Vulkan entry point is null until it runs.
