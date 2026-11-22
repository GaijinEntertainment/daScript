Returns the cache read verdict of the last ``compile_file`` that used this module group with an explicit
module-cache path: ``none``, ``clean``, ``partial`` or ``fallback``. It is set before that compile's block
runs, so the block can act on it; a group no such compile used answers ``none``. Builds without file I/O throw.
