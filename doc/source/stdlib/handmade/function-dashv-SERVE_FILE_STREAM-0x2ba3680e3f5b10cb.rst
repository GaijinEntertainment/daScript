Queues a bounded regular-file response through a live streaming writer. Returns
zero when queued, or -1 for invalid server/writer ownership, an empty path, or a
nonpositive size limit. The queued operation responds with 404 for an unavailable
or non-regular file, 413 above max_bytes, or 200 otherwise. HEAD sends no body.

The event loop reads at most one 64 KiB chunk after the previous write drains.
Disconnect and server shutdown release the transfer. Existing response headers
are retained; an unset content type defaults to application/octet-stream.
