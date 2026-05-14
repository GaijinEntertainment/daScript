---
slug: github-pages-deploy-pages-publishes-snapshot-not-overlay
title: Does GitHub Pages preserve previously-deployed files when actions/deploy-pages publishes a new artifact?
created: 2026-05-13
last_verified: 2026-05-13
links: []
---

**No — `actions/deploy-pages` publishes the artifact as a complete site snapshot, not a layer over the prior deploy.**

If you upload `_site/` and it's missing a directory (e.g. `_site/playground/`) that was present in the previous deploy, that path will 404 on the live site after this deploy completes. Pages does not retain or merge files from earlier deploys.

## Implication for CI staging

You cannot use "skip this directory when the source artifacts are missing" as a graceful-degradation strategy — that will 404 the route. Two viable options when an artifact is unbuildable:

1. **Stage a placeholder `index.html`** so the URL keeps resolving to something useful (status page, "rebuilding" notice, link back to the rest of the site). Keeps the route alive while you re-roll.
2. **Fail the deploy** when artifacts are missing. Couples the whole site publish to the failing component — only acceptable if the missing piece is critical.

The "graceful skip" intuition (let docs/blog ship even when WASM is broken) is correct in principle, but the implementation must explicitly stage *something* under the route — silent omission is the bug.

## Where this bit me

PR #2648 (daslang.io playground): WASM build had `continue-on-error: true` so emsdk hiccups wouldn't block docs/blog. Round-4 fix gated the playground staging on `daslang_static.{js,wasm}` existing — but with no `else` branch, `_site/playground/` was absent on WASM failure, which would 404 `/playground/` on the next deploy. Round-5 added the `site/playground/placeholder.html` fallback that explains the runtime is rebuilding.

## Quick check

`actions/deploy-pages@v4` reference: https://github.com/actions/deploy-pages — "deploys an artifact" (singular, replaces). The artifact is whatever `actions/upload-pages-artifact` packaged, which is the full `_site/` you staged.

## Questions
- Does GitHub Pages preserve previously-deployed files when actions/deploy-pages publishes a new artifact?
