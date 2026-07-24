---
name: dasherder
description: Coordinate a dasHerd-managed agent session, including durable Inbox/Outbox requests, multi-repository review bundles, exact source focus, and ready-for-review handoff. Use whenever DASHERD_SESSION_ID is present or a dasHerd wake-up asks the agent to fetch a request.
---

# Coordinate with dasHerd

Treat the launch worktree as immutable session origin. Other repositories and
worktrees may participate, but declare them in the active review bundle. Do not
infer ownership from dirty files and do not include unrelated pre-existing work.

The watcher injects `DASHERD_URL`, `DASHERD_TOKEN`, `DASHERD_SESSION_ID`,
`DASHERD_SESSION_KIND`, and `DASHERD_CONTEXT_PATH`. Read the context artifact
first. Run the repository-owned CLI from any participating worktree by using its
absolute path when necessary:

```powershell
powershell.exe -NoProfile -File <origin>/utils/dasHerd/dasherd.ps1 whoami
```

## Session checkpoints

Keep dasHerd informed at these semantic checkpoints; do not stream every file
write:

1. When work crosses into another repository or worktree, register it if needed
   and add it as a bundle participant.
2. When intended changes become clear, sync the complete participant/file set.
3. When review points become clear, add exact focus targets and short captions.
4. Before saying the work is ready, reconcile the manifest against Git and sync
   the bundle with `status: ready`.
5. After publication, sync PR state into the same participant when that schema is
   available; do not invent a separate logical delivery.

`bundle sync` is declarative and retry-safe. Always send the complete desired
state. Reuse a stable bundle `id` chosen for the logical unit of work.

```powershell
powershell.exe -NoProfile -File <origin>/utils/dasHerd/dasherd.ps1 bundle sync `
  --manifest path/to/bundle.json
powershell.exe -NoProfile -File <origin>/utils/dasHerd/dasherd.ps1 bundle list
```

A bundle manifest has this shape:

```json
{
  "id": "renderer-crash-fix",
  "title": "Renderer crash fix",
  "kind": "debug",
  "status": "ready",
  "summary": "The ownership fix spans the runtime and Vulkan adapter.",
  "participants": [
    {
      "repository_id": "local:D:/Work/daScript",
      "worktree_path": "D:/Work/daScript/.codex/worktrees/renderer-fix",
      "role": "implementation",
      "base_revision": "origin/master",
      "head_revision": "abc123",
      "state": "modified",
      "files": [
        {"path": "src/runtime.das", "role": "primary"}
      ]
    },
    {
      "repository_id": "local:D:/Work/dasVulkan",
      "worktree_path": "D:/Work/dasVulkan/.codex/worktrees/renderer-fix",
      "role": "dependency",
      "state": "modified",
      "files": [
        {"path": "src/device.das", "role": "risk"}
      ]
    }
  ],
  "focus": {
    "summary": "Important code; the remaining changes are plumbing.",
    "targets": [
      {
        "repository_id": "local:D:/Work/daScript",
        "worktree_path": "D:/Work/daScript/.codex/worktrees/renderer-fix",
        "comparison": "working",
        "role": "primary",
        "file_path": "src/runtime.das",
        "whole_file": false,
        "ranges": [
          {"start_byte": 120, "end_byte": 380, "caption": "main loop"}
        ]
      },
      {
        "repository_id": "local:D:/Work/dasVulkan",
        "worktree_path": "D:/Work/dasVulkan/.codex/worktrees/renderer-fix",
        "comparison": "working",
        "role": "risk",
        "file_path": "src/device.das",
        "whole_file": false,
        "ranges": [
          {"start_byte": 44, "end_byte": 95, "caption": "bug was here"}
        ]
      }
    ]
  }
}
```

Every focus target must resolve to one declared participant. UTF-8 byte offsets
are half-open: `start_byte` is included and `end_byte` is excluded. A target with
`whole_file: true` may have no ranges. A ready bundle automatically projects one
non-modal Attention item; later syncs update that item.

Use these commands to inspect or register repositories:

```powershell
powershell.exe -NoProfile -File <origin>/utils/dasHerd/dasherd.ps1 repository list
powershell.exe -NoProfile -File <origin>/utils/dasHerd/dasherd.ps1 repository add --path D:/Work/dasVulkan
```

Use the exact `repository_id` and observed `worktrees[].path` returned by
`repository list`. If a worktree is not observed yet, register/refresh it and do
not substitute a similarly named checkout.

## Human requests and replies

When the terminal receives a wake-up, fetch that exact message and acknowledge
it after reading:

```powershell
powershell.exe -NoProfile -File <origin>/utils/dasHerd/dasherd.ps1 inbox list
powershell.exe -NoProfile -File <origin>/utils/dasHerd/dasherd.ps1 inbox get hf_42
powershell.exe -NoProfile -File <origin>/utils/dasHerd/dasherd.ps1 inbox ack hf_42
powershell.exe -NoProfile -File <origin>/utils/dasHerd/dasherd.ps1 inbox complete hf_42
```

Use `outbox reply <inbox-id> --subject ... --focus-json ...` for a focused reply.
Use `outbox send --bundle <bundle-id> --subject ... --focus-json ...` for notes
that belong to a bundle but are not its canonical ready-state projection.

Inbox is human/system to agent; Outbox is agent to human. The watcher assigns
provenance. The token prevents accidental local cross-talk; it is not a
permission boundary.
