---
slug: site-playground-main-js-is-build-artifact-edit-web-ui-src
title: Which files under site/playground/ are gitignored / are build artifacts, and where do I edit the real source?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

# site/playground/ — which files are gitignored

`site/.gitignore` (lines 15-20) marks these as build artifacts copied at deploy time, NOT tracked source:

```
playground/jquery-3.6.0.min.js
playground/main.css
playground/main.js
playground/samples/
playground/daslang_static.js
playground/daslang_static.wasm
```

Their **real source** lives in `web/ui/src/` (`main.js`, `main.css`, `jquery-3.6.0.min.js`) and `web/output/` (`daslang_static.{js,wasm}`). `pages.yml` and the `playground/samples/` build step generate or copy them into `site/playground/` on deploy.

**Files in `site/playground/` that ARE tracked** (edit these in place):

- `index.html`
- `forge-skin.css`
- `playground-init.js`
- `playground-share.js`
- `playground-tabs.js`
- `placeholder.html`

## How to spot the mistake

Edit lands fine, page works locally (Python http.server serves whatever's on disk), but `git status` doesn't list the file. Catch with `git check-ignore -v <path>` or `git status --ignored`.

## Why this layout exists

`web/ui/src/` is shared with the WASM embedded build target. Keeping `site/playground/main.js` as a copy lets the daslang.io site stay self-contained for deploy (`actions/deploy-pages@v4` publishes the full `_site/` snapshot — see [[daslang-io-redesign-landed]]'s notes on snapshot vs overlay), while edits go through the same source the embedded build consumes.

## Fix if you've already edited the artifact

The edit you made to `site/playground/main.js` works for local testing but won't survive the next deploy. Re-apply it to `web/ui/src/main.js` — the same patch usually applies cleanly since `pages.yml` does a straight copy. Then verify with `diff site/playground/main.js web/ui/src/main.js` — should be identical (or empty if the copy step has already re-run).

## Questions
- Which files under site/playground/ are gitignored / are build artifacts, and where do I edit the real source?
