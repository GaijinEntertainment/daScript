# Browser live capture transport

`live_capture.js` connects an application's exported Emscripten `Module.FS` to the
transport-neutral daslang `live/live_capture` module. Snapshot/validation/restore
logic stays in daslang. The JavaScript layer supplies report UI, browser/GPU metadata,
console diagnostics, upload/download and a file picker. F8 requests a capture; the
application can also call `live_capture_request()` from its own UI.

Configure `/live-capture` as the WASM mailbox. Call `live_capture_poll()` before
simulation and `live_capture_flush(frame)` after rendering. The latter pairs provider
state with an application-supplied framebuffer PNG from that same frame. Do not call
it after a later simulation update. Requests and responses use monotonically unique
client IDs; only capture and validated restore operations cross this browser mailbox.

Wire formats:

- Snapshot: `daslang.live.capture/1`, with application/build/frame, versioned providers,
  opt-in diagnostics and a bounded event history.
- Report bundle: `daslang.live.report/1`, containing `snapshot`, optional scene PNG data
  URL, description, browser/GPU metadata, console and input history.

The optional backend implements POST/GET `/api/reports`, GET `/api/report?id=R…`
and GET `/api/report-image?id=R…`. Upload is an explicit user action. A failed upload
leaves the local bundle available through Download. Restore requires the matching
application/build and validates every restorable provider before applying any.

Load `live_capture.js` in the page body **before** the Emscripten launcher script.
It installs the report-dialog keyboard barrier before GLFW registers its window-level
capture listeners. Blocking only at the dialog's bubble phase is too late: GLFW
already prevents Backspace and Tab defaults. Key-up events still reach GLFW to clear
any modifiers held when entering the dialog.

The transport regression test runs without a browser: `node --test modules/dasLiveHost/tests/test_capture_transport.cjs`.
It covers request correlation, timeouts, native capture notifications and loading guards.
