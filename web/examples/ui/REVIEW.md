# Playground UI (web/examples/ui) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`site/README.md`.

**A boulder-dash sample file - a `.das` source, a dastest suite, or that sample's
`ARCHITECTURE.md` - answers to `samples/examples/boulder-dash/REVIEW.md`, wherever the diff
puts it.**

**A diff that changes a file of this folder applies `site/REVIEW.md` (repo root) too.** The
deploy copies these files into the site (`.github/workflows/pages.yml`, repo root), so
`site/playground/` never shows the change.

**A PR whose diff changes `src/` (this folder) states, in the PR body, a run of the
WASM-staged Playwright suite (`site/tests/playground/`, repo root) against the branch tip,
naming the passes and any failures; a later edit to `src/` restates the run.** The no-WASM
lane cannot see a broken runtime path, and a run recorded mid-branch describes a tree that no
longer ships.

**A PR whose diff changes a bundle under `samples/` - the file set a `data.json` entry lists -
states, in the PR body, a run that loaded that bundle on a runtime built from the branch tip -
the dasweb-verify browser run (`utils/internal/dasweb-verify`, repo root), or the bundle's game
card or playground page opened and run for a few frames - naming the bundle and the
artifact.** The Playwright suite stubs the graphics and audio bundles, so a green suite says
nothing about them.

**A `.das` under `samples/` not written in gen2 is a defect.**

**A bundle under `samples/` that does not compile and run with the current toolchain is a
defect.**

**Never write a `// verify:` line into a `.das` file under `samples/` - put the budget or the expected pattern
in `utils/internal/dasweb-verify/browser/expectations.json` instead.**
