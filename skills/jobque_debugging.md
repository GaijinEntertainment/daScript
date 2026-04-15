# Debugging JobStatus / Channel / LockBox / Feature Leaks

## Overview

`JobStatus`, `Channel`, `LockBox`, and `Feature` are threading primitives in `job_que.h` / `aot_builtin_jobque.h`. They use manual refcounting (`addRef`/`releaseRef`) and are a common source of leaks. A permanent tracking system (intrusive linked lists + per-ID tracing) is always on — no debug macros or rebuild needed.

## Architecture

### Object hierarchy

- **JobStatus** — base refcounted sync primitive (`mRef`, `mRemaining`, condition variable)
- **Channel : JobStatus** — FIFO queue of `Feature` values (`mTrackMagic = TRACK_CHANNEL`)
- **LockBox : JobStatus** — single-slot fill/grab/join container (`mTrackMagic = TRACK_LOCKBOX`)
- **Feature** — value type carrying `void* data + TypeInfo + Context`. Lives in Channels and LockBoxes
- **AtomicTT** — bare `std::atomic<TT>`, does NOT inherit from JobStatus, no refcounting

### Tracking fields

Every `JobStatus` has:
- `mTrackId` — unique ID from shared counter `g_jobque_track_total` (shared with Feature)
- `mTrackNext/mTrackPrev` — intrusive doubly-linked list
- `mTrackMagic` — subtype tag: `TRACK_JOB_STATUS`, `TRACK_CHANNEL`, or `TRACK_LOCKBOX`
- `mCreatedAt` — string description of the daScript source location where created

Every `Feature` has:
- `fTrackId` — unique ID from same shared counter
- `fTrackNext/fTrackPrev` — separate intrusive doubly-linked list
- `fCreatedAt` — creation source location
- `fOwner` / `fOwnerTrackId` — which Channel/LockBox holds this Feature

### Key files

| File | What |
|---|---|
| `include/daScript/misc/job_que.h` | JobStatus tracking fields, addRef/releaseRef signatures |
| `include/daScript/simulate/aot_builtin_jobque.h` | Feature tracking fields, AtomicTT (standalone) |
| `src/misc/job_que.cpp` | Tracking implementation: linked list ops, DumpJobQueLeaks, trackEvent |
| `src/builtin/module_builtin_jobque.cpp` | daScript wrappers that pass LineInfo to addRef/releaseRef |
| `src/hal/debug_break.cpp` | Static variable definitions for tracking globals |

## Step 1: Get the Leak Dump

`DumpJobQueLeaks()` runs at exit in both `daslang.exe` and `daslang-live.exe`. It walks both linked lists and prints all surviving objects.

**With daslang.exe:**
```bash
bin/Debug/daslang.exe path/to/script.das
```

**With daslang-live.exe** — capture output to file:
```bash
cd path/to/script/dir
bin/Debug/daslang-live.exe main.das > /tmp/output.txt 2>&1 &
# ... interact, then shutdown via REST API or live_shutdown MCP tool
cat /tmp/output.txt
```

### Reading the dump

```
  JobStatus #4 (rc=2) LockBox created at strudel_player.das:280:35
total 1 leaked JobStatus objects
  Feature #5 owner=#4
total 1 leaked Feature objects
```

- **#4** — track ID (shared between JobStatus and Feature)
- **(rc=2)** — current refcount at dump time (should be 0 for cleanup)
- **LockBox** — subtype (Channel, LockBox, or JobStatus)
- **created at** — daScript source location
- **owner=#4** — Feature is inside JobStatus #4

## Step 2: Trace a Specific Object

Once you know the leaking ID, use `--track-job-status` to get every addRef/releaseRef with source locations:

```bash
bin/Debug/daslang.exe --track-job-status 4 path/to/script.das
```

This sets `g_jobque_track_id = 4`. Every `addRef`/`releaseRef` on that object prints immediately:

```
tracking JobStatus #4
JobStatus #4 (LockBox) addRef (rc=0) at strudel_player.das:280:35
JobStatus #4 (LockBox) addRef (rc=1) at strudel_player.das:281:29
JobStatus #4 (LockBox) addRef (rc=2) at audio_boost.das:1265:14
Shutting down...
JobStatus #4 (LockBox) releaseRef (rc=3) at audio_boost.das:215:22
```

**Read the trace:** Each line shows the rc *before* the operation. Here rc goes 0→1→2→3, then only one releaseRef 3→2. Two refs were never released — that's the leak.

## Step 3: Identify the Leak Pattern

### Common patterns

**Missing cleanup on early return:**
```
strudel_shutdown() {
    if (g_cmd_channel == null) { return }  // ← early return skips status box cleanup
    ...
    // status box cleanup never reached in main-thread mode
}
```
Fix: move cleanup outside the guard, or restructure the function.

**`release()` nulls the pointer:**
The daScript `release()` wrapper calls `releaseRef` then sets the pointer to `nullptr`. If you need the pointer after release, save it first:
```das
var saved = my_box
my_box |> release()       // my_box is now null
unsafe(lock_box_remove(saved))  // use saved copy
```

**Async `unset_status_update` on dead audio thread:**
`unset_status_update` pushes an async command. If the audio system is already shut down, the command never gets processed and the ref is never released. Fix: call `unset_status_update` while audio is still alive, then `join()` to wait for the audio thread to process it.

**`set_status_update` adds a hidden `add_ref`:**
`set_status_update(sid, box)` calls `box |> add_ref` internally (audio_boost.das:1265). This ref is released when `unset_status_update` is processed by the audio thread. The cleanup must account for this ref.

**Capture macro implicit `add_ref`:**
When a Channel/LockBox/JobStatus is captured in a lambda, `capture_macro` in `jobque_boost.das` auto-inserts `addRef` at capture and `releaseRef` at lambda destruction. These refs appear in the trace but not in the daScript source.

### Refcount accounting

For a typical LockBox with `set_status_update`:
```
lock_box_create()           rc: 0 → 1  (create)
add_ref()                   rc: 1 → 2  (explicit, in user code)
set_status_update() add_ref rc: 2 → 3  (hidden, inside audio_boost)
--- audio thread processes unset ---
releaseRef                  rc: 3 → 2  (audio thread releases set_status_update ref)
release()                   rc: 2 → 1  (matches explicit add_ref)
lock_box_remove()           rc: 1 → 0  (delete)
```

### LockBox lifecycle (fill/grab/join)

LockBox synchronization: `fill()` sets `mRemaining=1`, `grab()` sets `mRemaining=0` and notifies, `join()` waits for `mRemaining==0`. No extra `add_ref`/`release` needed for the fill/grab/join cycle itself — the refs are for ownership tracking (who holds a pointer to the box).

## Step 4: Add `--max-frames` for Automated Testing

For GUI apps, add a `--max-frames N` argument to exit after N frames:
```das
var max_frame_limit = 0

def parse_args() {
    let args <- get_command_line_arguments()
    for (i in range(length(args) - 1)) {
        if (args[i] == "--max-frames") {
            max_frame_limit = to_int(args[i + 1])
        }
    }
}

[export]
def main() {
    parse_args()
    init()
    var frame_count = 0
    while (!exit_requested()) {
        update()
        frame_count ++
        if (max_frame_limit > 0 && frame_count >= max_frame_limit) {
            break
        }
    }
    shutdown()
}
```

Then test:
```bash
bin/Debug/daslang.exe --track-job-status 4 path/to/script.das -- --max-frames 30
```

Note: `--track-job-status` goes before the script path (daslang arg), `--max-frames` goes after `--` (script arg).

## Shutdown Order

Audio apps must shut down in the correct order:

1. `unset_status_update(sid)` — tell audio thread to release the status box ref
2. Wait for audio thread to process it (the box's `join()` will return once the audio thread's grab/release cycle completes)
3. `strudel_shutdown()` or equivalent — stop the strudel/midi thread
4. `audio_system_finalize(...)` — stop the audio thread
5. `lock_box_remove()` / `channel_remove()` — delete the objects (rc must be 1)

If `audio_system_finalize` runs before `unset_status_update` is processed, the async command is lost and the ref leaks.

## Quick Checklist

1. Build debug: `cmake --build build --config Debug --target daslang -- /nodeReuse:false`
2. Run script, check exit output for leak dump
3. If leaks found, rerun with `--track-job-status <id>` to get addRef/releaseRef trace
4. Count refs: every addRef must have a matching releaseRef
5. Check: early returns skipping cleanup? `release()` nulling pointer before further use? async unset on dead thread? missing `unset_status_update`?
6. Fix and verify: no leak lines in output, exit code 0
