
.. _stdlib_match:

================
Pattern matching
================

.. include:: detail/match.rst

.. _match:

The MATCH module implements pattern matching in daScript.

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

|structure_macro-match-match_as_is|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_match_c__c_match_type_C._C.:

.. das:function:: match_type(anytype: auto const; anything: auto const)

+--------+-------------+
+argument+argument type+
+========+=============+
+anytype +auto const   +
+--------+-------------+
+anything+auto const   +
+--------+-------------+


this is a macro, it will be replaced with a match expression of specified type

.. _function-_at_match_c__c_match_expr_C.:

.. das:function:: match_expr(anything: auto const)

+--------+-------------+
+argument+argument type+
+========+=============+
+anything+auto const   +
+--------+-------------+


this is a macro, it will be replaced with a match expression


