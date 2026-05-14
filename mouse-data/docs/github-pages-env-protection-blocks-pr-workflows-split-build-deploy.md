---
slug: github-pages-env-protection-blocks-pr-workflows-split-build-deploy
title: Why does my GitHub Actions Pages workflow fail instantly on PRs with "Branch is not allowed to deploy to github-pages due to environment protection rules"?
created: 2026-05-14
last_verified: 2026-05-14
links: []
---

**The `environment:` block at the job level is the culprit. Attaching `environment: github-pages` to a job causes GitHub to evaluate the environment's deployment-branch protection rule *before* any step runs — and PR branches don't match the rule's "only deploy from default branch" default.**

Symptoms:
- Job conclusion: `failure`
- `started_at` ≈ `completed_at` (1-2 seconds)
- `runner_id: 0`, `runner_name: ""` (no runner ever assigned)
- `steps: []` (no step recorded)
- Annotation: `Branch "refs/pull/N/merge" is not allowed to deploy to github-pages due to environment protection rules.`

The check fires for `refs/pull/N/merge` (the synthetic merge ref PRs use), `workflow_dispatch` on a non-default branch, and any other non-master push. The deploy steps inside the job are correctly gated by `if: github.ref == 'refs/heads/master'`, but that gate never gets to fire because the job-level `environment:` declaration is what triggers the protection check.

## Two fixes

**Defense-in-depth (correct):** split into separate jobs.

```yaml
jobs:
  build:           # runs everywhere, no environment attached
    steps:
      - ... real build steps ...
      - if: github.ref == 'refs/heads/master'
        uses: actions/upload-pages-artifact@v3

  deploy:          # only on master, environment attached, env rule satisfied
    if: github.ref == 'refs/heads/master'
    needs: build
    environment:
      name: github-pages
      url: ${{ steps.deployment.outputs.page_url }}
    steps:
      - id: deployment
        uses: actions/deploy-pages@v4
```

PR builds now run to completion (real CI signal), the env protection rule still gates the actual publish, and the workflow's master-only `if:` is the primary gate.

**Quick unblock (defense-out):** disable the environment's branch protection.

```bash
gh api -X PUT repos/OWNER/REPO/environments/github-pages \
  --input - <<<'{"deployment_branch_policy": null}'
```

Trade-off: the workflow's step-level `if:` checks become the only barrier. For personal-project repos, fine. For shared infra, prefer the split.

**Why `*` wildcard policy doesn't work:** GitHub's deployment-branch policies match against branch names like `master` / `feature/x`. The synthetic `refs/pull/N/merge` ref is neither a branch nor a tag — `*` matches nothing in this case.

## Where this bit me

dasImgui PR #24 (2026-05-13): the docs.yml workflow had `environment: github-pages` at the job level, so every PR build instant-failed. First docs PR on the repo, so the latent issue surfaced. PR #25 (split build/deploy) fixed the structure; the protection rule was restored to default-branch-only after the refactor landed.

## Questions
- Why does my GitHub Actions Pages workflow fail instantly on PRs with "Branch is not allowed to deploy to github-pages due to environment protection rules"?
