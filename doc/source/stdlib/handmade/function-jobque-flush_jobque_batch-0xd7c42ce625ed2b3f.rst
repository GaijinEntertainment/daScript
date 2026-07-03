Publish this thread's pending batched fork jobs to the queue NOW instead of at join (a no-op when nothing is pending or batching is off — see set_jobque_batch_dispatch).
Pair with jobque_try_run_one when the dispatching thread participates in the work: the batch must be visible in the fifo before the caller starts popping from it.
