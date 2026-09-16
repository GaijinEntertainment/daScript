# Registered live captures

`require live/live_capture` adds reusable capture and restore alongside live commands.
Register `LiveCaptureProvider(name,version,read,validate,apply)` for logical state.
Read returns newly owned JSON. Validate must be side-effect-free and reject malformed
state. Apply runs only after all providers validate; it must not fail or retain borrowed
JSON pointers. Inspection-only providers may omit validate/apply. Providers rebuild
transient resources; they must not serialize GPU handles or process pointers for replay.

Use `live_capture_diagnostic(name)` to opt read-only live commands into a capture.
The registry intentionally does not invoke every registered command: many mutate state.
`live_capture_event(category,payload)` retains the newest 256 events, bounded to 4096
bytes each. Applications may attach their own input, navigation or event history.

Native live commands:

- `live_capture`: current provider state and registered diagnostics.
- `live_capture_restore`: validate and restore a snapshot or report bundle.
- `live_capture_inspect_file`: inspect saved JSON; optional `command` selects an existing
  captured diagnostic without executing it.
- `live_capture_restore_file`: restore a saved snapshot/report through the same providers.

For a frame-aligned screenshot, configure a mailbox and artifact callback, request capture,
and flush after rendering. Browser integration is described in `../web/README.md`.

A scene provider should validate dimensions, array/index bounds, asset references and
rendering ranges before applying a snapshot. It can freeze simulation and lock the captured
viewport for reproduction. Its source fingerprint must match the browser build. Historical
reports remain inspectable even when replay is refused.
