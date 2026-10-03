Sets per-WebSocket pending message, pending byte, and write-buffer limits before
server start. All limits must be positive. Returns false for invalid values,
invalid server ownership, or a server that has already started.

Pending limits include messages currently draining on the owner thread. Exceeding
a limit closes that peer without consuming another peer's allowance. The write
limit covers libhv's unsent socket buffer, not bytes already accepted by the OS.
