# mouse

Personal Q&A cache MCP server. `.md` answers backed by SQLite/FTS5 retrieval. Long vision: [OVERVIEW.md](OVERVIEW.md).

## Quick start

```bash
# Rebuild the index from <root>/docs/ (defaults to ./mouse-data, override via --root or $MOUSE_ROOT)
daslang utils/mouse/main.das -- rebuild

# Search
daslang utils/mouse/main.das -- ask "how do I X"

# Add a Q&A (dupe-gated by default)
daslang utils/mouse/main.das -- add "how do I X" --body "answer body"

# Run as MCP stdio server
daslang utils/mouse/main.das -- serve
```

## MCP wiring

Add to your `.mcp.json`:

```json
{
  "mcpServers": {
    "mouse": {
      "command": "daslang.exe",
      "args": ["utils/mouse/main.das", "--", "serve", "--root", "./mouse-data"]
    }
  }
}
```

Tools exposed: `mouse__ask`, `mouse__add`, `mouse__get`, `mouse__rebuild`.

## Layout

```
mouse-data/
  docs/<slug>.md       -- one doc per atomic Q&A (source of truth)
  index.db             -- SQLite, rebuildable from docs/
```

`.md` files are checked-in-friendly. `git pull` + `mouse rebuild` re-syncs the index.

## Development

Run the test suite:

```bash
daslang dastest/dastest.das -- --test utils/mouse/tests/
```

Format `.das` files via the daslang MCP `format_file` tool (`mcp__daslang__format_file` when invoking from an agent) — there is no shell equivalent, so the snippet above is shell-runnable but the formatter is not.

CLAUDE.md has a top-level "Asking blind-mouse" section describing when to reach for this tool from inside Claude Code.
