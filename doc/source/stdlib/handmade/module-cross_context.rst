The CROSS_CONTEXT module requires a ``shared`` module after the prerequisite walk -
from a macro, a simulate macro or a running script - and calls into its macro context
by function name, so a module that is expensive to bring up and only sometimes needed
comes in at the point that decides it needs it.

The function reached by name is ``[export]``\ ed - a macro context keeps only exported,
``[init]``, ``[finalize]`` and macro-init functions - and it runs under the target
context's lock, so it must not call back into the calling context; everything the caller
needs comes back through the result pointer.

All functions and symbols are in "cross_context" module, use require to get access to it.

.. code-block:: das

    require daslib/cross_context
