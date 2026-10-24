Compiles a file through ``FileAccess`` and invokes the block with a success
flag, program and diagnostics.

The overload with ``moduleCache`` uses an explicit AST module-cache path
and returns the cache read verdict: ``none``, ``clean``, ``partial``,
``fallback`` or ``unavailable``. An empty path disables caching. With this
overload, keep the module group alive while using the returned program;
it owns source locations restored from the cache. Use ``module_cache_path``
for a filename keyed by the host compile inputs.
