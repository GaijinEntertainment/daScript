Attempts to acquire an exclusive file lock without waiting. Returns false for a
null file, a competing lock, an OS error, or an unsupported target. Closing the
file releases the lock. Uses flock on POSIX and LockFileEx on Windows; locks are
not supported on WebAssembly or in builds with file I/O disabled.
