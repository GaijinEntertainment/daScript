
.. _stdlib_regex_boost:

=======================
Boost package for REGEX
=======================

The REGEX_BOOST module extends regular expressions with the ``%regex~`` reader
macro for compile-time regex construction. Inside the reader macro, backslashes are
literal — no double-escaping is needed (e.g. ``%regex~\d{3}%%`` instead of
``"\\d\{3}"``).

Optional flags can be appended after a second ``~`` separator:

- ``%regex~pattern~i%%`` — case-insensitive matching
- ``%regex~pattern~s%%`` — dot-all mode (``.`` matches ``\n``)
- ``%regex~pattern~is%%`` — both flags combined

See :doc:`regex` for the full list of supported syntax.
See :ref:`tutorial_regex` for a hands-on tutorial.

All functions and symbols are in "regex_boost" module, use require to get access to it. ::

    require daslib/regex_boost

Example: ::

    require daslib/regex_boost
        require strings

        [export]
        def main() {
            var inscope re <- %regex~\d+%%
            let m = regex_match(re, "123abc")
            print("match length = {m}\n")
            let text = "age 25, height 180"
            regex_foreach(re, text) <| $(r) {
                print("found: {slice(text, r.x, r.y)}\n")
                return true
            }
        }
        // output:
        // match length = 3
        // found: 25
        // found: 180

+++++++++++++
Reader macros
+++++++++++++

.. _call-macro-regex_boost-regex:

.. das:attribute:: regex

Reader macro that converts ``%regex~`` literals into precompiled ``regex::Regex`` objects at compilation time. Optional flags can follow a second ``~``: ``%regex~pattern~i%%`` for case-insensitive, ``%regex~pattern~s%%`` for dot-all, ``%regex~pattern~is%%`` for both.

