
.. _stdlib_lint:

==================
Paranoid lint pass
==================

The LINT module implements static analysis checks for daScript code.
It provides customizable lint rules that detect common mistakes, style
violations, and potential bugs at compile time.

All functions and symbols are in "lint" module, use require to get access to it. ::

    require daslib/lint

+++++++++++++++
Lint operations
+++++++++++++++

  *  :ref:`paranoid (prog: ProgramPtr; compile_time_errors: bool) <function-lint_paranoid_ProgramPtr_bool>` 

.. _function-lint_paranoid_ProgramPtr_bool:

.. das:function:: paranoid(prog: ProgramPtr; compile_time_errors: bool)

Runs the paranoid lint visitor on the program to check for common coding issues.

:Arguments: * **prog** :  :ref:`ProgramPtr <alias-ProgramPtr>` 

            * **compile_time_errors** : bool


