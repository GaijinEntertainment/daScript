The MATCH module implements pattern matching on variants, structs, tuples,
arrays, and scalar values. Each arm is ``pattern => body``. A bare name in a
pattern binds the matched value, ``_`` matches anything, ``&&`` adds a guard,
and ``|`` separates alternatives.

As a statement, a multi-statement body is a ``$ { }`` block whose ``return``
leaves the enclosing function. In value position ``match`` is an expression:
every body is an expression and the last arm must always match.
Arms are tried in source order and the first one that matches wins; a pattern
that cannot apply to the subject type is a compile error. ``static_match``
drops such arms silently instead of erroring, which is what makes it usable in
generic code where only some arms apply per instantiation.

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
        return match (c) {
            Color.red => "red"
            Color.green => "green"
            _ => "other"
        }
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
