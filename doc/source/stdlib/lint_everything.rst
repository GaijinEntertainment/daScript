
.. _stdlib_lint_everything:

===================================
Global lint pass (paranoid checks)
===================================

The LINT_EVERYTHING module applies paranoid lint diagnostics to all modules.
Like ``lint``, but applies paranoid diagnostics to every module in the program,
not just the one that requires it.

All functions and symbols are in "lint_everything" module, use require to get access to it. ::

    require daslib/lint_everything

.. note::

    This module uses ``[global_lint_macro]`` to enable paranoid checks across
    all modules in the compilation unit. Any lint warnings will be treated as
    compilation errors.
