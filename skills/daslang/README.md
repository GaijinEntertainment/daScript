# The daslang agent-skill bundle

This directory is the **distributable, SDK-free daslang language skill** for AI coding
agents (Claude Code / claude.ai skill format): `SKILL.md` plus the reference files under
`references/`. Third parties drop the whole `daslang/` folder into their agent's skills
directory; nothing in it assumes this repository, the SDK layout, MCP tools, or any
particular embedding.

It is NOT one of the repo's task skills (`skills/*.md`) and is not read by in-repo agent
sessions — the repo's own CLAUDE.md supersedes it here.

## Rules for editing

- **Every code example is probe-verified**: it was compiled (and, for semantic claims, run)
  against the current binary before landing. Keep that bar — no example or behavioral claim
  goes in without a probe. Probes live in session scratchpads, not in the tree.
- **SDK-free**: no repo paths, no `bin/`, no MCP, no CI, no lint rule IDs, no tutorials
  paths. Idioms are stated as best practice, not as house lint policy.
- **gen2 only.** gen1 gets one mention (the `options gen2 = false` opt-out) and no examples.
- When the grammar or stdlib changes (new syntax, renamed daslib symbols, changed defaults),
  update the affected file in the same arc — this bundle rots exactly like the scraped
  third-party bundle it replaced, unless it is maintained with the language.
- The version/date stamp at the bottom of `SKILL.md` is updated whenever content is
  re-verified against a new daslang version.

## Distribution

Zip the `daslang/` directory itself (so the archive root is `daslang/SKILL.md`):

```powershell
Compress-Archive -Path skills/daslang -DestinationPath daslang-skill.zip
```

(When packaging, exclude this README — it is repo-facing; SKILL.md and references/ are the
product.)
