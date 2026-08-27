# Playground UI (web/examples/ui) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`site/README.md`.

**Every rule of `site/REVIEW.md` binds a change under `src/` and `samples/` as if the files
lived under `site/playground/`.** The deploy copies them into the site (`pages.yml`), so
`site/playground/` never shows the change.

**Never write a `// verify:` line into a sample source - put the budget or the expected pattern
in `utils/internal/dasweb-verify/browser/expectations.json` instead.** A sample-source change
voids that sample's nightly build-cache entry.

**A file of the boulder-dash sample - its `.das` sources, its dastest suites, its
`ARCHITECTURE.md` - answers to `samples/examples/boulder-dash/REVIEW.md` wherever the
diff puts it.**
