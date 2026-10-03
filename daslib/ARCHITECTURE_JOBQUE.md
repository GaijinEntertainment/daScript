# Worker capture lifetime

## Thread-loop final release {#thread-loop-final-release}

The five `release_capture_jobque_*` helpers in `jobque_boost.das` defer their
release checks while their argument address lies inside the rooted repeated
lambda capture. Native `verify_loop_capture_released` in
`src/builtin/module_builtin_jobque.cpp` checks that capture after the final step.
Its handled-type list matches the five helpers: Channel, JobStatus, LockBox,
Stream and SeqBox. A nested lambda's capture lies outside that address range and
keeps the ordinary release checks.
