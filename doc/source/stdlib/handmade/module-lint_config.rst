The lint_config module loads ``{get_das_root()}/.lint_config`` (a TOML
file with a ``[rules]`` table of booleans) and folds it into a
``disabled_codes`` set consumed by the three lint pass-macros
(``daslib/lint``, ``daslib/perf_lint``, ``daslib/style_lint``), by the
standalone runner ``utils/lint/main.das``, and by the MCP ``lint`` tool.
``seed_default_disabled`` seeds the canonical default-off rule set
(currently STYLE005) before the file is read.

All functions and symbols are in "lint_config" module, use require to get access to it.

.. code-block:: das

    require daslib/lint_config
