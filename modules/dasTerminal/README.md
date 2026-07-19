# dasTerminal

`dasTerminal` provides a renderless terminal state machine written in daScript
and a deliberately small native PTY transport boundary. The emulator consumes
raw `array<uint8>` streams and exposes inspectable daScript value state and
snapshots; neither layer draws pixels. Long-lived process ownership belongs to
the session host, and rendering belongs to consumers such as dasImgui.

The snapshot deliberately includes both screen buffers, scrollback, grapheme
width and style per cell, cursor state, terminal modes, title/CWD metadata, and
unknown-sequence diagnostics. That schema is also intended to become the
session-host-to-UI IPC contract.

Renderers use `terminal_viewport_snapshot` instead of the canonical full
snapshot. It returns only the active screen and the visible scrollback slice,
while retaining total history size and global row indices for selection. This
keeps per-frame projection bounded by the grid size; the full snapshot remains
available for explicit whole-history operations such as copying a selection.

The first conformance slice covers text controls, wrapping and scrolling,
cursor movement, erase operations, common SGR colors/attributes, OSC title/CWD
and hyperlinks, alternate screen, selected DEC modes, device replies, and
incremental UTF-8 decoding. It also encodes normalized key events and paste
payloads into mode-sensitive VT input bytes while keeping device replies on a
separate raw-byte drain path. `tests/terminal_semantics.das` asserts semantic
snapshots, byte-at-a-time chunk equivalence, bounded history/control strings,
input encoding, and reply/event behavior. The C++ test covers only the native
transport contract.

`src/pty.h` is the transport-only process boundary. On Windows it launches a
child through dynamically resolved ConPTY APIs, exposes nonblocking reads,
writes, resize, root-process wait/exit status, and explicit termination, and
does not couple process bytes to a particular emulator or renderer. The live
test prefers PowerShell 7 (`pwsh`) and falls back to Windows PowerShell when it
is not installed; readiness and echo completion are observed from raw PTY bytes
rather than sleeps.

The daScript `terminal_launch` convenience handle composes those separate
interfaces for local applications. `terminal_poll` feeds nonblocking PTY output
into the emulator and routes device replies back to the child; write, resize,
process-status, exit-code, and explicit-termination operations complete the
local process pump. The first ImGui consumer remains outside this module at
`modules/dasImgui/terminal/imgui_terminal.das`.

`benchmarks/terminal/snapshot_scaling.das` covers feed throughput plus full and
viewport snapshot scaling. The hot path reuses cached ASCII graphemes, shares
immutable strings, and moves scrolled row arrays into the history ring; viewport
projection remains bounded by visible geometry rather than total scrollback.
