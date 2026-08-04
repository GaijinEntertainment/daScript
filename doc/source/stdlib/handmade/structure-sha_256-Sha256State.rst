Streaming hash state; create with sha256_init and feed through sha256_update / sha256_final.
Current hash values (eight 32-bit words).
Pending input block (64 bytes).
Bytes currently buffered in the pending block.
Total bytes absorbed so far.
