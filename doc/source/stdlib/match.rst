
.. _stdlib_match:

================
Pattern matching
================

.. include:: detail/match.rst

.. _match:

The MATCH module implements pattern matching in daScript.
(See also the :ref:`pattern-matching` section.)

All functions and symbols are in "match" module, use require to get access to it. ::

    require daslib/match


+++++++++++
Call macros
+++++++++++

.. _call-macro-match-match:

.. das:attribute:: match

Implements `match` macro.

.. _call-macro-match-static_match:

.. das:attribute:: static_match

Implements `static_match` macro.

++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-match-match_as_is:

.. das:attribute:: match_as_is

Implements `match_as_is` annotation.
This annotation is used to mark that structure can be matched with different type via is and as machinery.


