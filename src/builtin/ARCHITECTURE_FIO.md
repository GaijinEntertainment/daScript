# File input and child processes

### One spawn at a time holds inheritable pipe ends {#spawn-window}

A child inherits every inheritable handle its parent holds at the moment it is created
(`CreateProcess` with handle inheritance, `fork`), not only the ones meant for it. A spawn
creates the child's ends of its pipes, starts the child, and closes its own copies of those
ends; a second thread that starts a child inside that span hands it the first spawn's pipe
ends too. The first spawn's reader then sees no end of file until the second thread's whole
process tree has exited - two unrelated children end together, and a caller that reads to the
end of one waits for the other. `g_spawnWindow` (`module_builtin_fio.cpp`) is the lock these
spawns hold over that span - `popen`, `popen_timeout`, `popen_argv`, `popen_argv_pipe`,
`spawn_argv`, `spawn_process` - and release before they run a block or throw. `system` holds
no lock, since it returns only when its child has exited, and neither does the JIT linker's
`popen` (`module_jit.cpp`): a child either of them starts can still inherit another spawn's
ends. The write ends the parent keeps are not inheritable: Windows clears the flag on them,
and the POSIX write end of a child's stdin is close-on-exec. A read end that leaks into
another child delays no end of file. On Apple `popen` takes the `pclose` mutex before this
lock, so a `popen` that waits for a `pclose` in flight holds up no other spawn.

### The direct random reader {#direct-reader}

`das_dread_open`, `das_dread_at` and `das_dread_close` read positional spans off a file opened
cache-bypassing (`FILE_FLAG_NO_BUFFERING` on Windows, `O_DIRECT` on Linux, `F_NOCACHE` on macOS),
so a cache of on-demand blocks held in pinned memory is not doubled in the page cache. A direct
read wants the file offset, the count and the destination aligned to the sector, so a handle owns
an aligned bounce band: the sectors a span shares with its neighbours - the head before the offset
and the tail past the end - go through it, as does any span whose destination is not aligned, in
band-sized pieces, while a span aligned at both ends lands in the caller's buffer straight off the
device. The sector is never taken under 4096 bytes (`DAS_DIRECT_MIN_SECTOR`, the writer's too):
drives report 512 logical over 4096 physical. One handle serves one reader thread: on Windows the
read's position rides the `OVERLAPPED` of a synchronous handle, so there is no file position to
share; on Linux a filesystem that refuses `O_DIRECT` (tmpfs) serves the same handle buffered. A
read past the end of the file comes back short, a failed read as -1.

### Regular-file buffered reads {#regular-file-buffered-read}

`das_fopen_regular_read_utf8` validates the opened object and derives its size from
that object, so replacement of the pathname cannot substitute an unchecked file.
On POSIX it opens with `O_NONBLOCK` and checks `fstat` for a regular file before
creating the buffered stream; opening a FIFO therefore cannot wait for a producer.
On Windows it checks the opened handle's disk type and attributes before creating
the stream.
