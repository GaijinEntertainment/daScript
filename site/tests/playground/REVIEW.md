# Playground Playwright suite Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`site/README.md`.

**A diff that adds a test in this folder needing the daslang runtime, or that makes an existing
test here need it, puts `@wasm` in that test's title, in the same change.** The per-PR lane
stages the site without WASM artifacts and runs the suite with `--grep-invert '@wasm'`
(`playground-e2e.yml`), so an untagged runtime-dependent test fails every PR; the tagged specs
run only in `nightly_playground.yml`'s `wasm_specs` job, against the DEPLOYED site.

**A diff that adds an `@wasm` test whose assertion depends on a runtime change in the same
diff states in its PR body that `nightly_playground.yml` drives the deployed site and the test
stays red until the rebuilt artifact ships.** Without the statement, the first post-merge
nightly reads as a mystery regression to whoever is on it.

**A diff that makes a test - or a whole `*.spec.js` file - skip itself or change its assertions
based on what the browser engine supports is a defect.** The shipped artifacts run on every
engine: stub `/api/build/info` to say whether the wasm engine is available.
