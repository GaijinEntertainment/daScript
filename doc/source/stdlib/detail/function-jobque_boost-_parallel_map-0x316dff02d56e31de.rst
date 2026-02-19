Runtime implementation for ``parallel_map``.
Partitions array indices ``[0..length(arr))`` into ``num_jobs`` chunks and invokes ``blk``
on the calling thread with ``(chunk_begin_idx, chunk_end_idx, results_channel, wg)``.
Blocks until all jobs finish. Results are available in ``results_channel`` after this call returns.
Requires ``with_job_que`` context.
