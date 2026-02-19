Runtime implementation for ``parallel_for_each``.
Partitions array indices ``[0..length(arr))`` into ``num_jobs`` chunks and invokes ``blk``
with ``(chunk_begin_idx, chunk_end_idx, wg)`` on the calling thread.
The block should dispatch ``new_job`` calls that process ``arr[i]`` for ``i`` in ``[chunk_begin_idx..chunk_end_idx)``,
then call ``wg |> notify_and_release``.
Blocks until all jobs finish. Requires ``with_job_que`` context.
