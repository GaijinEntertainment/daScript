
.. _stdlib_match:

================
Pattern matching
================

.. das:module:: match

The MATCH module implements pattern matching on variants, structs, tuples,
arrays, and scalar values. Supports variable capture, wildcards, guard
expressions, and alternation. ``static_match`` enforces exhaustive matching
at compile time.

See :ref:`tutorial_pattern_matching` for a hands-on tutorial.

All functions and symbols are in "match" module, use require to get access to it.

.. code-block:: das

    require daslib/match

Example:

.. code-block:: das

    require daslib/match

        enum Color {
            red
            green
            blue
        }

        def describe(c : Color) : string {
            match (c) {
                if (Color.red) { return "red"; }
                if (Color.green) { return "green"; }
                if (_) { return "other"; }
            }
            return "?"
        }

        [export]
        def main() {
            print("{describe(Color.red)}\n")
            print("{describe(Color.green)}\n")
            print("{describe(Color.blue)}\n")
        }
        // output:
        // red
        // green
        // other



+++++++++++
Call macros
+++++++++++

.. _call-macro-match-match:

.. das:attribute:: match

Implements `match` macro.


.. _call-macro-match-static_match:

.. das:attribute:: static_match

Implements `static_match` macro.


.. _call-macro-match-multi_match:

.. das:attribute:: multi_match

Implements `multi_match` macro.


.. _call-macro-match-static_multi_match:

.. das:attribute:: static_multi_match

Implements `static_multi_match` macro.



++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-match-match_as_is:

.. das:attribute:: match_as_is

Implements `match_as_is` annotation.
This annotation is used to mark that structure can be matched with different type via is and as machinery.


.. _handle-match-match_copy:

.. das:attribute:: match_copy

Implements `match_copy` annotation.
This annotation is used to mark that structure can be matched with different type via match_copy machinery.



