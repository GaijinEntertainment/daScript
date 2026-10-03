Returns the unsigned mode value supplied by stat or fstat. POSIX mode values
include file-type and permission bits. Windows values are CRT mode bits, not
Windows ACLs. Check is_valid before interpreting the result.
