The TOML module parses `TOML 1.0 <https://toml.io/en/v1.0.0>`_ into the
same ``JsonValue?`` tree shape produced by ``daslib/json``, so existing
``json_boost`` accessors (``v ?? def``, ``from_JV``, etc.) work on TOML
inputs as-is. Date-time tokens are preserved as raw RFC-3339 strings
since JSON has no native date type.

All functions and symbols are in "toml" module, use require to get access to it.

.. code-block:: das

    require daslib/toml
