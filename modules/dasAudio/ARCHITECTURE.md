# dasAudio Architecture

**Who reads this: me.** Durable facts about why the audio system is shaped the way it is -
readable cold, no history, no PR numbers.

## The audio callback must never wait on a game thread

`data_callback` (`src/dasAudio.cpp`) runs on miniaudio's realtime thread and has a buffer
deadline. Any primitive it touches must be wait-free on the writer side, because a game thread
that owns a lock and then gets descheduled turns into an audio dropout.

That splits the sharing primitives by role:

| role | primitive | why |
|---|---|---|
| status snapshots (per sound, system stats, playback time) | `SeqBox` | POD published every mix, read on demand; a reader can never make the mixer wait, and there is nothing to own |
| PCM buffer handoff (`append_box_to_pcm`) | `LockBox` | passes an `array<float>` by pointer and needs a *consumed* state, so the producer knows when the buffer is reusable - neither is something a snapshot box offers |
| thread-exit handshakes (`done_status`) | `JobStatus` | a genuine blocking wait is the point |

A snapshot must not be given a `LockBox`: every `LockBox` operation holds its mutex for the whole
duration of the das block it invokes, so a polling reader and the mixer contend directly.

## A teardown wait that expires leaks on purpose

Every wait in teardown is bounded (`PCM_GRAB_TIMEOUT_MS`, `WORKER_EXIT_TIMEOUT_MS` and their midi
twins). On expiry the handle is **leaked, not freed**: a queued grab may still reach the box and a
wedged worker still references its stream and status, so freeing after a timeout trades a hang for
a use-after-free. A leak is recoverable, the jobque leak dump names it, and the log line says which
wait gave up. Do not "fix" a timeout path by freeing there.

`SeqBox` is the exception and is released on both paths: its holders drop references in any order
and the last one deletes, so it needs no agreement with the audio thread.

## The waits cascade

`strudel_init` spawns the worker; the worker runs the caller's function, which calls
`strudel_play`; `strudel_play` ends in the PCM wait; `done_status` is notified only after all of
that returns. So an ungrabbed buffer wedges the worker, and a wedged worker wedges
`strudel_shutdown` on the main thread. This is why the worker-exit budget must exceed the PCM one.
