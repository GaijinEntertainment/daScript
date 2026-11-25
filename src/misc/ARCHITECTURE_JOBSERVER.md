# src/misc Architecture - the jobserver client

A build that runs many compilers at once shares one budget of threads through the GNU make
jobserver. `ARCHITECTURE.md` beside this file holds the folder's other mechanisms.

## A compile's extra threads are tokens from the build's pool {#jobserver-client}

`jobserver.cpp` joins the pool a parent advertises in the environment - make, ninja's
`--jobserver-pool`, cargo - read from the last `--jobserver-auth=` (or the pre-4.2
`--jobserver-fds=`) in `CARGO_MAKEFLAGS`, `MAKEFLAGS` or `MFLAGS`. A process owns one implicit
token; every job that runs beside it holds a token from the pool while it runs, and gives back
the same byte it read when it ends. A job takes its token with `Jobserver::acquire`, which
returns the implicit token when no job of this `Jobserver` holds it, else a pool token, and
otherwise waits: a token released inside the process wakes it at once, and the pool is read
again every 10 ms for tokens other processes free. The `JobserverToken` gives the token back
when it dies. A positive limit caps the tokens one `Jobserver` holds at once, the implicit one
included, so a caller's thread count holds with or without a pool. With no pool and no limit
`acquire` returns at once, and a compile runs as it would with no build around it.

Each parallel job holds one token while it runs: a module of the compile DAG
(`src/ast/ast_compile_modules.cpp`), and a partition of the split JIT emit (`run_split_emit` in
`modules/dasLLVM/daslib/llvm_jit_run.das`, through the `jobque` module's `with_jobserver` and
`with_jobserver_token`). A job waits for its token on its own worker, so no worker gives up
while jobs remain, and a job never waits for another job while it holds one.

The pool's three forms: `fifo:PATH` (make 4.4) is opened with a descriptor of its own;
`R,W` is a pair of inherited pipe descriptors; a bare name is a Windows semaphore. The pipe's
read end is reopened through `/proc/self/fd`, never switched to non-blocking in place, because
make shares that open file description and a non-blocking flag set on it changes make's own
reads; with no `/proc`, the pipe form is off and the fifo form serves. Make leaves the pipe
descriptors open only for a recipe it treats as recursive - `$(MAKE)` or a `+` prefix - so a
descriptor that is closed or is not a pipe turns the client off rather than reading whatever
the number now names.

The library is the runtime's: the `jobque` module that binds it for das lives in the runtime,
and the compiler library reaches it through the exported `Jobserver` and `JobserverToken`.
