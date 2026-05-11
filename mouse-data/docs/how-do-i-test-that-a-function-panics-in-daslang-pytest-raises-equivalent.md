---
slug: how-do-i-test-that-a-function-panics-in-daslang-pytest-raises-equivalent
title: How do I test that a function panics in daslang? (pytest.raises equivalent)
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

**Short answer: you don't.** daslang's `try { ... } recover { ... }` is for catching **panic**, not for exception-handling-style test assertions. It's banned for testing-panic-UX use.

**Why:**
- `panic` in daslang is "bad things just happened" — runtime equivalent of a C++ exception. It carries exception-unwinding cost, AOT footprint, perf weight. Using `try/recover` to verify "this helper panics on bad input" misuses the panic machinery.
- `recover` does **NOT** expose the panic message anyway — you can only detect that *some* panic fired, not what it said. So even the "did this raise" form has poor fidelity (no message-shape assertion).
- A lint rule is incoming that will warn on every `try/recover` use; suppression will require an explicit annotation. Don't add new sites.

**What to do instead:**
1. **For helpers that intentionally panic on timeout / bad input** (e.g. `expect_value` / `expect_render` / `await_quiescent` in dasImgui's playwright module): you don't unit-test the panic path. The happy-path test that DEPENDS on the helper failing-loudly will surface regressions — if the helper stops panicking, downstream assertions will see stale state and fail anyway.
2. **For functions where soft-failure IS a legitimate API**: design a non-panicking variant (`try_foo : Option<T>` or `try_foo : Result<T; E>`) and have the panic-throwing wrapper call it. Then unit-test the `Result` variant with normal `t |> equal(...)` calls. Match the existing convention: `try_*` for fallible primitive, `*_or` for unwrap-with-default sugar, plain `foo` for strict.
3. **Negative-path coverage** for a panic-on-bad-input helper becomes: a developer running dastest can VISUALLY confirm the panic message format on any genuine failure. dastest prints the panic + stack as part of the FAIL line.

**Anti-patterns to refuse:**
- `try { expect_value(...) } recover { caught = true }; t |> success(caught, ...)` — wrong tool. There is no daslang pytest.raises.
- `try { x = to_int(s) } recover { x = -1 }` — parse failure is normal; use `try_to_int` from `daslib/strings_convert`.
- `try { v = tab[k] } recover { v = default }` — use `tab?[k] ?? default` or `key_exists`.

**See also:** `feedback_no_try_recover_for_soft_fail` auto-memory entry; `daslib/strings_convert` for the `try_to_*` / `to_*_or` family pattern; `modules/dasImgui/widgets/imgui_playwright.das` for `expect_value` as a real "intentionally panic on timeout" helper.

**Found 2026-05-10**, dasImgui Phase 3 retro — attempted to use `try/recover` to test `expect_value` panic UX; Boris flagged the misuse and announced lint rule.

## Questions
- How do I test that a function panics in daslang? (pytest.raises equivalent)
