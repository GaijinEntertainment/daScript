Compiles a file with an explicit AST module-cache path and invokes the block
with its success flag, program and diagnostics. Returns the cache read
verdict: ``none``, ``clean``, ``partial``, ``fallback`` or ``unavailable``.
An empty path disables caching. Keep the module group alive while using the
returned program; it owns source locations restored from the cache.
