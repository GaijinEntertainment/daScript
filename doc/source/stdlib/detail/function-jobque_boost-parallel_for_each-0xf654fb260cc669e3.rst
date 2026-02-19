Convenience wrapper around ``parallel_for`` for arrays.
Partitions array indices ``[0..length(arr))`` into ``num_jobs`` chunks.
The block body is automatically wrapped in ``new_job``.
Blocks until all jobs finish. Requires ``with_job_que`` context.
