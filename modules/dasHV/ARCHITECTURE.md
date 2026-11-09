# dasHV architecture

This document follows the repository's `ARCHITECTURE_COMMON.md` contract.

## WebSocket admission capacity {#websocket-admission-capacity}

Pending admission tickets and registered WebSocket channels share the channel
admission limit. A closed channel remains registered until its queued close
callback runs, so it continues to occupy admission capacity during that interval.
Close callbacks use reserved cleanup capacity instead of ordinary event admission;
the bounded channel registry limits the number of pending close callbacks.

## Open-request snapshot ownership

The native `onopen` event copies its `HttpRequest` before enqueueing it to the
script owner. A queued shared pointer owns that copy through callback dispatch;
teardown drops it with the event. This prevents the callback from observing a
released or reused parser request. All incoming request snapshots use the shared
copy helper, which clears the transport callback and cached non-owning content
pointer so body access resolves against the copied body. Admission/channel handles retain their existing
generation checks and cleanup paths. The queued request's full measured size is
charged to the same event budget used by ordinary HTTP requests.

`onWsOpenRequest` is optional. Its presence selects request-aware dispatch; absence
selects the original URL-only callback. The generated class adapter is regenerated
from `dashv_boost.das`, with the new method appended after existing methods.

## Bounded file-transfer lifecycle {#bounded-file-transfer-lifecycle}

`BoundedFilePump` consumes at most one 64 KiB chunk per step after the previous
socket write drains. `WriterFileStream` owns event-loop scheduling and file/socket
lifetime. Writer callbacks keep the transfer alive; weak writer and server
references prevent ownership cycles.

Empty files use `WriteResponse` rather than `EndHeaders` because libhv strips
`Content-Length: 0` in `EndHeaders`; the full response preserves the keepalive body
boundary. Pump steps are queued on the event loop because socket `onwrite` can be
synchronous; queuing avoids recursive pumping.

Finish restores callbacks and releases the current admission token before `End`.
`End` can synchronously start the next request, which needs its own registration.

## Module shutdown stops libhv threads

The handle registries own every server and client adapter, and a registry is a
static, destroyed after `main` returns. libhv destroys its default logger from an
`atexit` handler registered on its first log line, after the registries exist, so
it runs before them, while a loop thread may still log.
`Module_HV`'s destructor runs inside `Module::Shutdown`, before `main` returns, so it
stops every live server and client there: `stop()` joins the loop threads, and the
adapters' own destructors find them stopped. The live adapters are collected first
and stopped outside the registry lock, because loop-thread callbacks look handles up
through that lock.

## WebSocket sends run on the channel's loop {#websocket-send-on-loop}

A das program calls `send` on a server channel or a client from its own thread, while
the channel's libhv loop runs on a loop thread, and libhv's `hio_write` reads the io's
state outside the lock that guards its write queue. `send_on_channel_loop` posts the send
to the io's loop as a custom event and waits for the result, so the write and its
backpressure verdict - a refusal past the connection's write-buffer budget - happen on the
loop thread and come back to the caller synchronously, as a direct call's did. A call
already on the loop thread runs the send inline. A loop that is not running answers -1
without posting, and a posted send the loop has not served within
`WS_SEND_ON_LOOP_DEADLINE_MS` is cancelled under the record's lock and answers -1, so a loop
stopped between the check and the post cannot hang the program and a send the caller saw
fail is never written later; a send the loop serves in the same instant wins the lock first
and its result is the answer. The record is shared between the caller and the posted event
and holds its own copy of the payload, so neither side reads memory the other has freed.
