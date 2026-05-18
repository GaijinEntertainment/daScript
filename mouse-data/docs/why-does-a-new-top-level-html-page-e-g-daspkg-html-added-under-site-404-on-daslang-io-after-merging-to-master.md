---
slug: why-does-a-new-top-level-html-page-e-g-daspkg-html-added-under-site-404-on-daslang-io-after-merging-to-master
title: Why does a new top-level HTML page (e.g. /daspkg.html) added under site/ 404 on daslang.io after merging to master?
created: 2026-05-17
last_verified: 2026-05-17
links: []
---

The Pages deploy workflow (`.github/workflows/pages.yml`, "Stage site for deployment" step) stages root-level HTML via an **explicit allowlist**, not a glob:

```yaml
cp site/index.html _site/
cp site/downloads.html _site/
cp site/daspkg.html _site/      # add a new line per new top-level page
cp site/robots.txt _site/
cp -r site/files _site/files
```

Only `index.html`, `downloads.html`, `daspkg.html`, `robots.txt`, and `files/` are explicitly copied. Subdirectories with their own staging (`build/site` → `/doc`, `web/output/*` → `/playground`, `_news/` + `blog/_posts/` → blog via `build_blog.py`) are independent of this allowlist.

**Failure mode**: add `site/foo.html` to a commit, merge to master, watch /foo.html return 404 indefinitely because no `cp site/foo.html _site/` line was added. The page sits in the repo but never reaches `_site/`, and since `actions/deploy-pages` publishes `_site` as a complete snapshot ([[github-pages-deploy-pages-publishes-snapshot-not-overlay]]) there's no carryover from a prior deploy either.

**Fix**: edit pages.yml, add the `cp site/<name>.html _site/` line, commit. Re-running the workflow without the edit will not help — the cp list is the source of truth.

**Diagnose with**: `grep "cp site/" .github/workflows/pages.yml` shows the current allowlist.

The 2026-05-17 daspkg page (PR #2703, commit 281ac2e28) shipped without this line and 404'd in production until the workflow was patched.</body>
<parameter name="slug">pages-yml-explicit-cp-allowlist-new-html-needs-line

## Questions
- Why does a new top-level HTML page (e.g. /daspkg.html) added under site/ 404 on daslang.io after merging to master?
