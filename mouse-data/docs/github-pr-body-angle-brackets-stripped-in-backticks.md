---
slug: github-pr-body-angle-brackets-stripped-in-backticks
title: GitHub PR body strips angle brackets inside backticks — markdown body containing <vector> shows as empty code spans
created: 2026-05-18
last_verified: 2026-05-18
links: []
---

**Symptom**: PR body containing `` `<vector>` `` (header name in backticks) renders as empty backticks on GitHub. The angle-bracket content is gone from the stored body, not just visually hidden.

**Cause**: GitHub's markdown pre-processor treats `<vector>`, `<utility>`, `<string>` etc. as malformed HTML tags and silently strips them BEFORE the markdown's backtick-code-span wrapping fires. Affects both `gh pr create` and `mcp__github__update_pull_request`.

**Fix**: escape with HTML entities — `&lt;vector&gt;`, `&lt;utility&gt;` — even though they're inside backticks. Markdown will literalize the entities once you escape `<` and `>`.

Example of broken vs fixed:
```
broken: includes `<vector>`, `<utility>`, `<string>` etc.
fixed:  includes `&lt;vector&gt;`, `&lt;utility&gt;`, `&lt;string&gt;` etc.
```

The same gotcha doesn't apply to commit messages — those render fine.

**Discovered**: PR #2716, 2026-05-18. Initial body had 6 stdlib headers stripped from the rendered body until we caught it by re-reading via `mcp__github__pull_request_read`.

**Verification habit**: after `update_pull_request` or `create_pull_request` of a body with `<` or `>` characters, immediately call `pull_request_read method=get` and grep for empty backtick-pairs (``).

## Questions
- GitHub PR body strips angle brackets inside backticks — markdown body containing <vector> shows as empty code spans
