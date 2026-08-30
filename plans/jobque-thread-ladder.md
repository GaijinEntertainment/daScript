# jobque thread-ladder follow-ups

- Hybrid-topology detection for Windows and Linux (Intel P/E). `JobQue::get_num_perf_cores`
  answers 0 there, so every non-Apple hybrid box takes the homogeneous default and only
  `DAS_JOBQUE_THREADS` or `set_jobque_threads` can widen the pool onto the second tier.
