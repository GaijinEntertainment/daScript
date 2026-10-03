# Buffered file input

### Regular-file buffered reads {#regular-file-buffered-read}

`das_fopen_regular_read_utf8` validates the opened object and derives its size from
that object, so replacement of the pathname cannot substitute an unchecked file.
On POSIX it opens with `O_NONBLOCK` and checks `fstat` for a regular file before
creating the buffered stream; opening a FIFO therefore cannot wait for a producer.
On Windows it checks the opened handle's disk type and attributes before creating
the stream.
