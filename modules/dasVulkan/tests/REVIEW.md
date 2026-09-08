# dasVulkan Tests Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture
doc: `../ARCHITECTURE.md`.

**A diff that adds a `[test]` under `integration/` reaches `volkInitialize()` before its first
Vulkan call - in the `[test]` function, or in a helper that function calls first.** Nothing in
the harness calls it, and every Vulkan entry point is null until it runs.

**A `[test]` under `integration/` compares the numbers a driver or OS call returned only to
each other and to the call's own no-answer value - never to a magnitude or a threshold.** They
differ per machine and moment; whether the call answered does not.
