
.. _stdlib_coverage:

=======================
Code coverage profiling
=======================

The COVERAGE module provides code coverage instrumentation.
It automatically inserts coverage tracking calls at every block entry point,
allowing you to measure which parts of your code are executed during a run.

All functions and symbols are in "coverage" module, use require to get access to it. ::

    require daslib/coverage

.. note::

    This module uses ``[infer_macro]`` to instrument code at compile time.
    Coverage results can be collected and analyzed after program execution.
