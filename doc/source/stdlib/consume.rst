
.. _stdlib_consume:

=============================
Consume argument optimization
=============================

.. das:module:: consume

The CONSUME module implements the ``consume`` pattern, which moves ownership
of containers and other moveable values while leaving the source in a
default-constructed state. This enables efficient ownership transfer.

All functions and symbols are in "consume" module, use require to get access to it.

.. code-block:: das

    require daslib/consume



++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-consume-consume:

.. das:attribute:: consume

This annotation ensures that all arguments to the function are passed as moved values.
For example [consume(a,b)] ensures that both a and b are passed as moved values.



