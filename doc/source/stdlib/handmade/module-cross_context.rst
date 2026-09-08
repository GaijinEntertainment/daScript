The CROSS_CONTEXT module requires a ``shared`` module after the prerequisite walk -
from a macro, a simulate macro or a running script - and calls into its macro context
by function name, so a module that is expensive to bring up and only sometimes needed
comes in at the point that decides it needs it.

All functions and symbols are in "cross_context" module, use require to get access to it.

.. code-block:: das

    require daslib/cross_context
