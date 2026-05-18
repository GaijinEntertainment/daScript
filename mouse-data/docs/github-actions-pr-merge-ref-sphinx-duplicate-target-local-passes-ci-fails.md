---
slug: github-actions-pr-merge-ref-sphinx-duplicate-target-local-passes-ci-fails
title: CI sphinx-build fails with "Duplicate explicit target name" but local + WSL repro builds clean — what's going on?
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

When local `sphinx-build -W` AND a WSL-Ubuntu-24.04 mirror of the CI pipeline both succeed, but GitHub Actions CI fails with `Duplicate explicit target name: "alias-X"` at lines you didn't write — the cause is almost always **GitHub Actions checking out the PR merge ref, not the branch HEAD**.

`actions/checkout@v4` on pull-request events checks out a SYNTHESIZED MERGE COMMIT (PR-HEAD merged into base). Real `git merge origin/master` runs server-side. If master and the PR independently added the SAME RST explicit target (`.. _alias-foo:`) in DIFFERENT line ranges of the same file, git's recursive auto-merge cleanly combines both without conflict markers (because the edit hunks don't overlap). Sphinx then sees two definitions for the same label and bails with "Duplicate explicit target name".

**Diagnostic** — add a debug step to the docs workflow that dumps the file right before sphinx-build:

```yaml
- name: "DEBUG: dump file + check duplicate sources"
  run: |
    nl -ba doc/source/stdlib/external_types.rst | sed -n '50,130p'
    grep -rn '_alias-imvec\|_alias-ImVec' doc/source/
    sha256sum doc/source/stdlib/external_types.rst
```

The grep on CI will show duplicate anchor lines (e.g. lines 61+62 AND 125+126). Locally the same grep returns only one set.

**Fix**: `git fetch origin master && git merge origin/master` on the PR branch, then reconcile the duplicated content manually. Push. CI then sees the resolved version.

**Catch this earlier**: a clean WSL repro that mirrors CI's `actions/checkout` would also need to merge master before building. Most WSL repros clone the branch and skip the merge step, which is why they silently miss this class of bug. If you want a faithful CI repro, do `git checkout pr-branch && git merge origin/master` in WSL too.

Encountered 2026-05-18 on dasImgui PR #48 (drawlist-rail). PR #49 had landed on master 4h earlier adding `_alias-imvec2`/`_alias-imvec4` anchors to the same file my PR was anchoring `_alias-imcolor`. CI saw both blocks; local saw only mine. Resolved by merging master + folding `_alias-imcolor` into the existing block.

## Questions
- CI sphinx-build fails with "Duplicate explicit target name" but local + WSL repro builds clean — what's going on?
