---
slug: how-do-i-trigger-a-daslang-live-reload-programmatically-curl-reload-doesn-t-work
title: How do I trigger a daslang-live reload programmatically? `curl ... reload` doesn't work.
created: 2026-05-11
last_verified: 2026-05-11
links: []
---

Use the MCP tool `mcp__daslang__live_reload` (optional `port` and `full` flags). It hits an internal endpoint, NOT a `[live_command]`.

The live API HTTP server on port 9090 only dispatches `[live_command]`-annotated functions; `reload` isn't one of them, so:

```bash
curl -X POST -d '{"name":"reload"}' http://localhost:9090/command
# → "unknown command: reload"
```

The MCP tool talks to a separate control channel that bypasses the command queue. From script code, the equivalent is `daslang-live`'s built-in reload mechanism (file-watcher or explicit "reload" key in the dev UI) — there is no in-script `reload()` you can call.

Related MCP tools for live control: `live_status` (fps/uptime/has_error/paused), `live_pause`, `live_shutdown`, `live_command` (dispatch a `[live_command]`), `live_launch`. Full set at `utils/mcp/main.das` and the `skills/daslang_live.md` reference.

If you need a script-driven reload from inside a `[live_command]` handler, that's not currently supported — file an issue against `dasLiveHost/live/live_commands.das` if you need it.

## Questions
- How do I trigger a daslang-live reload programmatically? `curl ... reload` doesn't work.
