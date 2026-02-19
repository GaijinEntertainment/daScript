
.. _stdlib_stringify:

===========================
Long string embedding macro
===========================

.. das:module:: stringify

The STRINGIFY module provides the ``%stringify~`` reader macro for embedding
multi-line string literals verbatim. Text between ``%stringify~`` and ``%%``
is captured as-is without requiring escape sequences for quotes, braces,
or other special characters.

All functions and symbols are in "stringify" module, use require to get access to it.

.. code-block:: das

    require daslib/stringify



+++++++++++++
Reader macros
+++++++++++++

.. _call-macro-stringify-stringify:

.. das:attribute:: stringify

This macro implements embedding of the long string into the source code. ::

    var st = %stringify~
    This is a long string
    with multiple lines
    and special charactes like { this } and "that"
    %%



