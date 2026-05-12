Finalize the APNG file: drain the encoder thread, backpatch the ``acTL`` frame count, write ``IEND``, and free the writer. Returns 1 on success.
