Constructs a clock value from epoch seconds — the inverse of int64(clock). Useful for stamping a file time relative to another (e.g. set_mtime with an offset from a stat's mtime).
