The MATCH module implements pattern matching on variants, structs, tuples,
arrays, and scalar values. Supports variable capture (``$v(name)``), wildcards
(``_``), guard expressions (``&&``), and alternation (``||``).

``match`` is a **statement**, not an expression — write the arms so each one
assigns or returns, rather than expecting the ``match`` itself to produce a value.
Arms are tried in source order and the first one that matches wins; a pattern
that cannot apply to the subject type is a compile error. ``static_match``
drops such arms silently instead of erroring, which is what makes it usable in
generic code where only some arms apply per instantiation.
``multi_match`` / ``static_multi_match`` run **every** matching arm instead of
stopping at the first.

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
