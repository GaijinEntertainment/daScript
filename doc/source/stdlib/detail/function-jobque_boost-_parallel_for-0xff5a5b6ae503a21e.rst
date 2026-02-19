Partitions ``[range_begin..range_end)`` into ``num_jobs`` chunks and invokes ``blk`` once per chunk
on the calling thread with ``(chunk_begin, chunk_end, wg)``. The block is expected to dispatch work
via ``new_job`` and call ``wg |> notify_and_release`` when each job finishes.
``parallel_for`` blocks until all notifications are received (via internal ``with_wait_group``).
Requires ``with_job_que`` context.
