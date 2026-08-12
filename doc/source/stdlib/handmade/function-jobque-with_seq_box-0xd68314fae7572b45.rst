Creates a ``SeqBox`` scoped to the given block and releases it afterward.

The block receives the box as its only argument. The reference taken here is dropped when
the block returns, so anything that outlives the block — a job, a thread, an audio callback —
must take a reference of its own rather than borrow this pointer.
