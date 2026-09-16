# dasAudio Architecture

**Who reads this: me.** Durable facts about why the audio system is shaped the way it is -
readable cold, no history, no PR numbers.

## Threaded WebAssembly output

The AudioWorklet only consumes float PCM from a preallocated single-producer,
single-consumer ring and zero-fills an underrun. A regular pthread runs `mix_audio`,
including command-stream draining, decoder allocation and daslang context locking.
The worklet never waits for it.

The ring's depth is the output LATENCY and there is no way to spend it twice: the producer
renders ahead, so in steady state the ring is full, and what is buffered is exactly what a
newly triggered sound waits behind. Depth is therefore a budget, not a safety margin to be
raised when underruns appear - 20 ms of the DEVICE's own rate (`ma_device.sampleRate`, which a
browser may resolve away from the rate that was asked for; floored 512 frames, capped 4096).

What that 20 ms buys is one producer turnaround: the worst time `mix_audio` takes for a block
plus the wake in front of it. That turnaround is BOUNDED, not measured - with the producer woken
on the drain rather than a clock, 10 ms of depth still corrupts and 20 ms plays clean on the box
this was fixed on, which puts the worst case above 10 ms and at or under 20. A measurement would
replace the bound with a number and is the right way to move this depth; another A/B is not. What
the depth no longer covers is any property of the host, which is what the drain-wake removed.

A full ring parks the producer on `emscripten_futex_wait` against `g_playback_drain_seq`, and the
worklet bumps that counter and wakes it on every block it frees. The wake, not a timeout, is what
ends the wait, and this is the whole reason the depth can be small: a producer that SLEEPS is
bounded below by the host's timer quantum, which `Atomics.wait`'s timeout is quantized to - about
1 ms on macOS but the 15.6 ms one on Windows, measured in a Worker there at 16.0 ms median and
16.6 ms worst for a 1 ms request. A sleeping producer therefore needs a ring deeper than that
quantum or it empties before it can wake, every cycle, whatever the core count or load; that is
why a 512-frame ring (10.7 ms at 48 kHz) played clean on a Mac and corrupted on Windows. Waiting
on the drain instead removes the floor, so the depth answers to the mixer's own work and the
latency a game will accept. The wait keeps a timeout only as a backstop against a lost wake, and
`emscripten_futex_wake` is a notify - it never blocks, so the worklet may call it.

The ring is FILLED before the device starts - a device started against an empty one
underruns on its first callback every time, and priming costs no latency that the steady
state was not going to hold anyway. `sound_playback_underrun_frames` counts the frames the
worklet filled with silence, the playback twin of `sound_record_overflow_frames`. Without it an
underrun has no symptom a caller can read: a zero-fill is heard as a click inside the clip,
not as a device error, so the count is how this failure is told from a bad mix.

This separation is required for correctness: an Emscripten AudioWorklet is a Wasm
Worker, and the hybrid runtime can initialize it with no pthread pointer. C++ mutex
ownership then sees thread ID zero, so a mutex acquired by that worklet need not
exclude a main-thread producer. Draining `Stream` there can race with `Stream::push`.
Do not put the command queue, allocator, context locks or script execution back in
that callback even when a particular browser/toolchain appears to tolerate it.

Start the ring/producer before starting the device. On teardown, stop/join the
producer, stop the device, then free the ring and mixer context. Native, null-device
and single-threaded WASM backends retain the direct callback path.

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

## audio_live owns the audio system across a reload

Under `daslang-live` the host runs the program's `shutdown()` on every reload and `init()`
after it, while `audio/audio_live.das` carries the audio thread's command stream and channels
across the reload by adopting the old context's reference - the stream itself keeps mixing.
`audio_live_system()` creates the system on the first `init()` and adopts it on every later one;
`audio_live_finalize()` finalizes only at the real shutdown. A live program that calls
`audio_system_create`, `audio_system_finalize` or `set_audio_thread_command_stream` itself
breaks that in one of two ways: a finalize on reload frees the stream the restore is about to
push through, and a `set_` on reload takes a reference per reload that the real finalize can
never drop. `REVIEW.das` reports those calls in any program that requires `live/audio_live`;
a program whose reload design tears audio down drops the require instead.

## The waits cascade

`strudel_init` spawns the worker; the worker runs the caller's function, which calls
`strudel_play`; `strudel_play` ends in the PCM wait; `done_status` is notified only after all of
that returns. So an ungrabbed buffer wedges the worker, and a wedged worker wedges
`strudel_shutdown` on the main thread. This is why the worker-exit budget must exceed the PCM one.
