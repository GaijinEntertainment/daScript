Creates a new ``SeqBox`` for wait-free snapshot exchange between threads.

Returns a box that starts out empty, carrying one reference which the caller owns and
must hand to ``seq_box_release``. Every other holder — a lambda capture, a thread the
pointer was passed to — takes its own reference, and whoever drops the last one deletes
the box, so the creator is under no obligation to release last.
