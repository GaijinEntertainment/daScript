# src/builtin - the job queue

Companion to `ARCHITECTURE.md`; each section is cited by its anchor.

### A job's panic is the caller's {#jobque-team-panic}

Every job body runs under the same catch (`runWithCatch`), whichever way it was dispatched, and its
panic reaches the caller as the caller's own panic, `JOB EXCEPTION: <what> at <where>` - recoverable
by a `try/recover` around the dispatch for a graceful shutdown, and an exit code of 1 where nothing
catches it. The re-raise always happens where the caller's frame holds nothing a longjmp would skip:
with exceptions off a panic is a longjmp that runs no destructor, so the clones, the guards and the
recorder live in an inner scope and the message crosses it in a stack buffer.

A team dispatch - `parallel_for` in team mode, its indexed form and the multi-stage chain - runs its
chunks on worker clones and on the caller's own slot; the first panic is recorded under a mutex, the
failed context's exception is cleared, since the same context - a fork, or the caller itself - serves
the op's next chunk on its slot and a stop flag left by the interrupted lambda would cut that chunk
short, the team runs to completion, the clones are cleaned up, and the caller re-raises. A panic on
the caller's slot never longjmps out of the team callback mid-run: that left the workers waiting on a
completion that never came, and the stage chain's dispatch mutex held.

A fifo job (`new_job`, `parallel_for` off team mode) panics on a worker with no caller frame under it:
the worker records the first panic and clears its context, notifies and releases every wait group the
job's lambda captured and had not released itself - the panic skipped the job's own
`notify_and_release`, and a join on that group would otherwise never return - and the job's siblings
run on. The caller re-raises when its enclosing `with_job_status` block ends - which is where
`parallel_for` returns - or when the `with_job_que` block ends, before the drain's own complaint. A
`join` never raises: it may run inside a block whose guard a longjmp would skip. A detached
`new_thread` has no join point to report at, so its panic reports on the thread and ends the process
there, as before.

### Idle channel consumers {#channel-idle-wait}

`Channel::pop` waits on a condition variable until an item arrives or all producers
complete. Pushes and producer completion notify that condition, so an idle consumer
needs no periodic timeout or host-clock query.

### Thread-loop collection boundaries {#thread-loop-collection}

`invoke_job_lambda` roots the repeated lambda's capture throughout the loop. Heap
collection runs only after a step returns true, when that step's stack has unwound
and persistent values reside in globals or the rooted capture. A false return is
the final step: it releases captured handles, skips collection, and proceeds to
capture-release verification.

### A panic that strands a job ends the process {#jobque-panic-teardown}

An uncaught panic in the entry function skips the program's own shutdown, so a job parked on a
channel pop, or a `new_thread` loop, is still running when the host tears the modules down - and
`~JobQue` joins its workers while the module destructor waits for every detached thread, both
forever. A hung process holds its port with nothing serving it, and a supervisor that restarts
on exit never sees one. So after printing the panic the host calls
`jobque_teardown_or_exit_after_panic`: one deadline, under which the detached-thread count must
reach zero - a live thread can still enqueue work - and the global que must then drain with the
count still zero - a drained job can still start a thread; the two are rechecked until both hold
in one pass. When that does not happen in time it prints that it is exiting without teardown and `_Exit`s
with `DAS_PANIC_EXIT_CODE` (-2, which a POSIX parent reads as 254) - the code both hosts also
return after a panic that did tear down, so an uncaught exception is told apart from a compile
error's 1; the panic was fatal already, and the leak and
heap reports a teardown would print describe a program that never reached its shutdown. Both
hosts call it: `daslang`
(`utils/daslang/main.cpp`) and the standalone exe's `jit_run_main_guarded`
(`src/builtin/jit_runtime.cpp`).
