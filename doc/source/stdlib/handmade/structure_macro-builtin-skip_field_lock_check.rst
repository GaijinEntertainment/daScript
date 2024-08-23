Optimization, which indicates that the structure does not need lock checks::

    [skip_field_lock_check]
    struct PerfContext
        events : array<PerfEvent> // when pushing to this array, lock-check won't be performed
        enabled : bool
