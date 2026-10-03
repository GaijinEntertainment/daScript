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
