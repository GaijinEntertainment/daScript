# Playground UI (web/examples/ui) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`site/README.md`.

**`src/` and `samples/` are the deployed playground's UI source - every rule of
`site/REVIEW.md` binds a change under them as if the files lived under `site/playground/`.**
The deploy copies them into the site (`pages.yml`), so `site/playground/` never shows the
change and only this line makes that checklist apply.

**A file of the boulder-dash sample - its `.das` sources, its dastest suites, its
`ARCHITECTURE.md` - answers to `samples/examples/boulder-dash/REVIEW.md` wherever the
diff puts it.**
