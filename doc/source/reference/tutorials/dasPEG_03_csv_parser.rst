.. _tutorial_dasPEG_csv_parser:

=======================================
PEG-03 --- CSV Parser
=======================================

.. index::
    single: Tutorial; PEG
    single: Tutorial; CSV
    single: Tutorial; Repetition
    single: Tutorial; Arrays
    single: Tutorial; Comma-separated Lists

This tutorial builds a CSV parser that demonstrates collection-oriented
PEG features.  You will learn:

- ``*rule`` (zero-or-more) and ``+rule`` (one-or-more) repetition
- ``!rule`` (negative lookahead)
- ``any``, ``EOL``, ``TS`` terminals
- ``void?`` pattern rules
- ``string_`` and ``double_`` built-in terminals
- The comma-separated list idiom

Repetition Operators
====================

============================  =============================================
Syntax                        Description
============================  =============================================
``*rule``                     Zero or more --- collects into ``array``
``+rule``                     One or more --- collects into ``array``
``*rule as name``             Bind repeated matches to ``name``
``"{+rule}" as name``         Capture repeated text into a string
``MB(rule)``                  Optional (zero or one)
============================  =============================================

When ``*rule`` or ``+rule`` is bound with ``as``, the result is an
``array`` of the rule's return type.

Data Types
==========

The parser produces typed rows using a variant and typedef:

.. code-block:: das

   variant Cell {
       text : string
       number : double
   }

   typedef Row = array<Cell>

The Comma-Separated List Pattern
================================

The canonical PEG idiom for comma-separated lists is:

::

   list      ->  *comma_item  last_item
   comma_item -> item ","

This avoids ambiguity with trailing commas.  The last element has no
comma:

.. code-block:: das

   var row : Row
   rule(TS, *comma_cell as cells, cell as last) {
       cells |> emplace(last)
       return <- cells
   }

   var comma_cell : Cell
   rule(cell as c, TS, ",", TS) {
       return c
   }

The Grammar
===========

.. code-block:: das

   def parse_csv(input : string;
                 blk : block<(val : array<Row>; err : array<ParsingError>) : void>) {
       parse(input) {
           var csv : array<Row>
           rule(*newline_row as rows, last_row as last, MB(trailing_eol), EOF) {
               rows |> emplace(last)
               return <- rows
           }
           rule(EOF) {
               var empty_rows : array<Row>
               return <- empty_rows
           }

           var newline_row : Row
           rule(row as r, EOL) { return <- r }

           var last_row : Row
           rule(row as r) { return <- r }

           var row : Row
           rule(TS, *comma_cell as cells, cell as last) {
               cells |> emplace(last)
               return <- cells
           }

           var comma_cell : Cell
           rule(cell as c, TS, ",", TS) { return c }

           var cell : Cell
           rule(string_ as text, TS) { return Cell(text = text) }
           rule(double_ as value, TS) { return Cell(number = value) }

           var trailing_eol : bool
           rule(EOL) { return true }
       }
   }

Built-in Terminals
==================

============================  =============================================
Terminal                      Description
============================  =============================================
``string_``                   Matches ``"..."`` and returns content
``double_``                   Matches a floating-point number
``number``                    Matches a decimal integer
``any``                       Matches any single character
``EOF``                       End of input
``EOL``                       End of line (``\n`` or ``\r\n``)
``WS``                        Zero or more whitespace (including newlines)
``TS``                        Zero or more tabs/spaces (no newlines)
============================  =============================================

Negative Lookahead
==================

``!rule`` succeeds when ``rule`` does **not** match, without consuming
input.  Useful for "match anything except":

.. code-block:: das

   // Match any character that is not a newline
   var expr_text : void?
   rule(not_set('\n', '\r', ';')) {
       return null
   }

.. seealso::

   Full source: :download:`tutorials/dasPEG/03_csv_parser.das <../../../../tutorials/dasPEG/03_csv_parser.das>`

   Next tutorial: :ref:`tutorial_dasPEG_email_validator`
