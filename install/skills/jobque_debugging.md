# Debugging JobStatus / Channel / LockBox / Feature Leaks

`JobStatus`, `Channel`, `LockBox`, and `Feature` are threading primitives in the daslang runtime. They use manual refcounting (`addRef`/`releaseRef`) and are a common source of leaks. A permanent tracking system (intrusive linked lists + per-ID tracing) is always on — no debug build or special flag needed to get the basic dump.

## Object hierarchy

- **JobStatus** — base refcounted sync primitive (`mRef`, `mRemaining`, condition variable)
- **Channel : JobStatus** — FIFO queue of `Feature` values
- **LockBox : JobStatus** — single-slot fill/grab/join container
- **Feature** — value type carrying `void* data + TypeInfo + Context`. Lives in Channels and LockBoxes.
- **AtomicTT** — bare `std::atomic<TT>`, NOT refcounted — does not appear in the leak dump.

Each `JobStatus` carries a `mTrackId`, intrusive `mTrackNext/Prev`, a subtype tag (`TRACK_JOB_STATUS`, `TRACK_CHANNEL`, `TRACK_LOCKBOX`), and `mCreatedAt` (the daslang source location where it was created). Every `Feature` carries the same plus an `fOwner` / `fOwnerTrackId` linking it back to the Channel/LockBox holding it.

## Step 1: get the leak dump

`DumpJobQueLeaks()` runs at exit in `bin/daslang` and `bin/daslang-live`.

```bash
bin/daslang path/to/script.das
```

For `daslang-live`, capture stdout/stderr to a file and shut down via the REST API (`POST /shutdown`) or the `live_shutdown` MCP tool:

```bash
bin/daslang-live main.das > /tmp/output.txt 2>&1 &
# ... interact, then shut down ...
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
- **created at** — daslang source location
- **owner=#4** — Feature is inside JobStatus #4

## Step 2: trace a specific object

Once you know the leaking ID, use `--track-job-status` to get every addRef/releaseRef with source locations:

```bash
bin/daslang --track-job-status 4 path/to/script.das
```

This prints every `addRef`/`releaseRef` on that object as it happens:

```
tracking JobStatus #4
JobStatus #4 (LockBox) addRef (rc=0) at strudel_player.das:280:35
JobStatus #4 (LockBox) addRef (rc=1) at strudel_player.das:281:29
JobStatus #4 (LockBox) addRef (rc=2) at audio_boost.das:1265:14
Shutting down...
JobStatus #4 (LockBox) releaseRef (rc=3) at audio_boost.das:215:22
```

**Read the trace:** each line shows the rc *before* the operation. Here rc goes 0→1→2→3, then only one releaseRef 3→2. Two refs were never released — that's the leak.

## Step 3: identify the leak pattern

### Common patterns

**Missing cleanup on early return**

```das
def my_shutdown() {
    if (g_cmd_channel == null) { return }   // ← early return skips status box cleanup
    ...
    // status box cleanup never reached
}
```

Fix: move cleanup outside the guard, or restructure the function so all paths run cleanup.

**`release()` nulls the pointer**

The daslang `release()` wrapper calls `releaseRef` then sets the pointer to `nullptr`. If you need the pointer after release, save it first:

```das
var saved = my_box
my_box |> release()       // my_box is now null
unsafe(lock_box_remove(saved))  // use the saved copy
```

**Async command on a dead worker thread**

If a function pushes an async command to a worker thread (e.g. `unset_status_update`) but the worker has already shut down, the command never gets processed and the ref it would have released leaks. Fix: drain async commands while the worker is still alive, then `join()` to wait for them to be processed.

**Hidden `add_ref` inside library calls**

Some library calls (e.g. `set_status_update(sid, box)`) call `box |> add_ref` internally. The release happens later when the matching `unset_status_update` is processed. The cleanup must account for this hidden ref.

**Capture macro implicit `add_ref`**

When a Channel/LockBox/JobStatus/Stream is captured in a lambda, `capture_macro` in `jobque_boost.das` auto-inserts `addRef` at capture and `releaseRef` at lambda destruction. These refs appear in the trace but not in the daslang source — if the lambda outlives expectations, the ref outlives it too.

### Refcount accounting example

For a typical LockBox with `set_status_update`:

```
lock_box_create()           rc: 0 → 1   (create)
add_ref()                   rc: 1 → 2   (explicit, in user code)
set_status_update() add_ref rc: 2 → 3   (hidden, inside library)
--- worker thread processes unset ---
releaseRef                  rc: 3 → 2   (worker releases set_status_update ref)
release()                   rc: 2 → 1   (matches explicit add_ref)
lock_box_remove()           rc: 1 → 0   (delete)
```

### LockBox lifecycle (fill/grab/join)

LockBox synchronization: `fill()` sets `mRemaining=1`, `grab()` sets `mRemaining=0` and notifies, `join()` waits for `mRemaining==0`. No extra `add_ref`/`release` needed for the fill/grab/join cycle itself — the refs track *ownership* (who holds a pointer), not the synchronization state.

## Step 4: add `--max-frames` for automated testing

For GUI / event-loop apps that don't naturally exit, add a `--max-frames N` argument so tests can exit deterministically after N frames:

```das
require daslib/clargs

[CommandLineArgs]
struct Config {
    @clarg_doc = "Exit after N frames (0 = unlimited)"
    max_frames : int
}

[export]
def main() {
    var cfg = Config()
    parse_args(cfg)
    init()
    var frame_count = 0
    while (!exit_requested()) {
        update()
        frame_count ++
        break if (cfg.max_frames > 0 && frame_count >= cfg.max_frames)
    }
    shutdown()
}
```

Then run:

```bash
bin/daslang --track-job-status 4 path/to/script.das -- --max-frames 30
```

Note: `--track-job-status` is a **daslang** flag, so it goes BEFORE the script path. `--max-frames` is a **script** arg, so it goes AFTER the `--`.

## Stream-capturing jobs MUST release the captured ref

When a `new_job() @() { ... }` lambda captures a `Stream?`, the jobque capture macro bumps the Stream's refcount on capture. The job body **must** drop that ref before it returns or the runtime panics:

```
Stream has not been released. missing stream|>release or stream|>notify_and_release
```

…and the Stream is then deleted while the outer scope still holds a handle, producing the secondary crash:

```
synch primitive deleted while being used (ref=1)
```

Two idioms balance the capture:

- **`s |> release`** — when completion is signalled separately (e.g. alongside a `with_wait_group` / JobStatus that the job also signals). Use this when the consumer doesn't care about the Stream's own count.
- **`s |> notify_and_release`** — when the Stream itself is the completion signal (`with_stream(count)` form, consumer checks `s.isReady`). Use this when the consumer is gated on the stream's ready state.

The symmetry with Channel / JobStatus / LockBox / Feature (all four primitives auto-refcount on lambda capture) means the rule is uniform — but the panic only fires at runtime from an inner lambda frame, so the crash message is not obviously tied to the forgotten release at the **top** of the job body. There is no RAII / `defer` wrapper that does it implicitly.

## Shutdown order

Apps with worker threads should shut down in the correct order:

1. Tell each worker to release any `add_ref`'d state it holds (e.g. `unset_status_update(sid)`).
2. Wait for the worker to process step 1 (`join()` on the relevant box, or the worker's own quit signal).
3. Stop the worker thread itself.
4. Delete the sync primitives (`lock_box_remove`, `channel_remove`) — refcount must be 1 at this point.

If step 3 runs before step 2 finishes, the async command is lost and the ref leaks.

## Quick checklist

1. Run the script normally; check exit output for the leak dump.
2. If leaks are reported, rerun with `--track-job-status <id>` to get the addRef/releaseRef trace.
3. Count refs: every addRef must have a matching releaseRef.
4. Check: early returns skipping cleanup? `release()` nulling the pointer before further use? async unset on a dead worker? missing `unset_status_update`? stream-capturing job missing `s |> release` or `s |> notify_and_release`?
5. Fix and verify: no leak lines in output, exit code 0.
