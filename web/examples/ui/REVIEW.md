# Playground UI (web/examples/ui) Code Review Checklist

**Read `REVIEW_COMMON.md` (repo root) first - its contract binds this checklist.** Architecture doc:
`site/README.md`.

**A file of the boulder-dash sample - a `.das` the `boulder-dash` entry of `samples/data.json`
lists, a dastest suite over one of those files, or that sample's `ARCHITECTURE.md` - answers to
`samples/examples/boulder-dash/REVIEW.md`, wherever the diff puts it.**

**A diff that changes a file of this folder applies `site/REVIEW.md` (repo root) too.** The
deploy publishes this folder's `src/` as the site's playground, so a change here changes the
site with no diff under `site/`.

**A PR whose diff changes `src/` (this folder) states, in the PR body, a run of the full
Playwright suite (`site/tests/playground/`, repo root) with WASM artifacts staged at
`site/playground/` - the deployed artifacts count, `src/` is not compiled into them - against
the branch tip, naming the passes and any failures; a later edit to `src/` restates the run.** A run without those artifacts cannot see a broken runtime path, and a
run recorded mid-branch describes a tree that no longer ships.

**A PR whose diff changes a bundle under `samples/` - the file set a `samples/data.json` entry lists -
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
