The lint_fixups module holds the helpers the lint passes build their
``--fix`` edits from: the source text of a node or a range, the checks
a rewrite must pass, and the shared ``LintFix`` constructors.

All functions and symbols are in "lint_fixups" module, use require to get access to it.

.. code-block:: das

    require daslib/lint_fixups
