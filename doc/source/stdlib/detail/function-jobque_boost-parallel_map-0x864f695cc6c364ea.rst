Partitions array indices ``[0..length(arr))`` into ``num_jobs`` chunks.
The block body is automatically wrapped in ``new_job``.
Blocks until all jobs finish. Results are available in ``results_channel`` after this call returns.
Requires ``with_job_que`` context.
