The PEG module is a parser generator based on
`Parsing Expression Grammars <https://en.wikipedia.org/wiki/Parsing_expression_grammar>`_.
Define grammars directly in daslang using the ``parse`` macro --- the compiler
generates a packrat parser at compile time.  No external tools, no runtime code
generation.

See :ref:`tutorial_dasPEG_hello_parser` for a hands-on introduction.

Grammar basics
^^^^^^^^^^^^^^

A grammar lives inside a ``parse(input) { ... }`` block.  The first ``var``
declaration is the **entry rule**.  Each ``rule(...)`` adds an ordered alternative:

.. code-block:: das

    def my_parser(input : string; blk : block<(val : int; err : array<ParsingError>) : void>) {
        parse(input) {
            var expr : int
            rule(number as n, WS, "+", WS, number as m, EOF) {
                return n + m
            }
        }
    }

Rules try alternatives top to bottom.  The first match wins (ordered choice ---
no ambiguity).

Built-in terminals
^^^^^^^^^^^^^^^^^^

================================  =============================================
Terminal                          Description
================================  =============================================
``"literal"``                     Exact text match
``EOF``                           End of input
``EOL``                           End of line (``\n`` or ``\r\n``)
``WS``                            Zero or more whitespace (including newlines)
``TS``                            Zero or more tabs/spaces (no newlines)
``any``                           Any single character
``number``                        Decimal integer (returns ``int``)
``string_``                       Double-quoted string literal (returns content)
``double_``                       Floating-point number (returns ``double``)
``set(ranges...)``                Character from ranges: ``set('a'..'z', '0'..'9')``
``not_set(chars...)``             Any character NOT in the set
================================  =============================================

Operators and combinators
^^^^^^^^^^^^^^^^^^^^^^^^^

================================  =============================================
Syntax                            Description
================================  =============================================
``rule as name``                  Bind the rule result to ``name``
``"{rule}" as name``              Extract matched text as string
``+rule``                         One or more repetitions (array)
``*rule``                         Zero or more repetitions (array)
``MB(rule)``                      Optional (zero or one)
``!rule``                         Negative lookahead (no input consumed)
``PEEK(rule)``                    Positive lookahead (no input consumed)
``commit``                        Cut --- no backtracking past this point
``log("msg")``                    Print debug message during parsing
================================  =============================================

Options
^^^^^^^

Add inside the ``parse`` block:

- ``option(tracing)`` --- print rule-by-rule execution trace
- ``option(color)`` --- ANSI colors in trace output
- ``option(print_generated)`` --- show generated parser code at compile time

Return types
^^^^^^^^^^^^

Rules can return any daslang type: scalars, strings, structs, variants,
tuples, tables, and arrays.  Use ``void?`` for pattern-only rules that
match characters without producing a value.

Error handling
^^^^^^^^^^^^^^

``parse`` calls a callback ``$(val; err)`` where ``err`` is an
``array<ParsingError>``.  Place ``commit`` after unambiguous prefixes to
get meaningful error messages with position information.

Left recursion
^^^^^^^^^^^^^^

dasPEG supports left-recursive rules via packrat memoization.  This is the
standard technique for encoding left-associative operators (e.g. ``a + b + c``).

Performance
^^^^^^^^^^^

- Packrat memoization caches results per rule per position --- O(n) parsing
- Place common alternatives first (PEG tries in order)
- Use ``PEEK`` to quickly reject impossible alternatives
- Use ``commit`` after unambiguous prefixes for speed and error quality
- Set ``options stack = 1000000`` --- PEG parsers need more stack than default

All functions and symbols are in "peg" module, use require to get access to it.

.. code-block:: das

    require peg/peg

Example:

.. code-block:: das

    require peg/peg

        def parse_greeting(input : string;
                           blk : block<(val : string; err : array<ParsingError>) : void>) {
            parse(input) {
                var greeting : string
                rule("Hello, ", "{+letter}" as name, "!", EOF) {
                    return name
                }
                var letter : void?
                rule(set('a'..'z', 'A'..'Z')) {
                    return null
                }
            }
        }

        [export]
        def main() {
            parse_greeting("Hello, World!") $(val; err) {
                print("name = {val}\n")
            }
        }
        // output:
        // name = World
