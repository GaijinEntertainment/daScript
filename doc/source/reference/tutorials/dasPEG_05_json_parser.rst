.. _tutorial_dasPEG_json_parser:

=======================================
PEG-05 --- JSON Parser
=======================================

.. index::
    single: Tutorial; PEG
    single: Tutorial; JSON
    single: Tutorial; Recursive Grammar
    single: Tutorial; Tables
    single: Tutorial; Tuples

This tutorial builds a complete JSON parser that produces ``daslib/json``
``JsonValue?`` trees.  You will learn:

- ``MB()`` for optional matching
- Complex recursive grammars (arrays in arrays, objects in objects)
- Tuple and table return types
- Escaped characters in literals (``"\{"`` and ``"\}"``)
- The comma-separated list pattern in practice

Prerequisites: :ref:`tutorial_dasPEG_csv_parser`.

Recursive Grammars
==================

JSON is inherently recursive --- arrays contain values, values can be
arrays or objects, objects contain values.  In PEG, rules reference each
other freely:

::

   element  ->  array | object | value
   array    ->  "[" element_list "]"
   object   ->  "{" mapping_list "}"
   value    ->  true | false | null | number | string

dasPEG's packrat memoization handles the recursion efficiently.

Escaping Braces
===============

Since ``{`` and ``}`` have special meaning in daslang string builders,
literal braces in grammar strings must be escaped with a backslash:
``"\{"`` and ``"\}"``.

The Grammar (Simplified)
========================

.. code-block:: das

   def parse_json(input : string;
                  blk : block<(var val : JsonValue?# implicit;
                               err : array<ParsingError>) : void>) {
       parse(input) {
           var json_value : JsonValue?
           rule(WS, element as e, EOF) {
               return <- e
           }

           var element : JsonValue?
           rule(array_ as a) { return <- JV(a) }
           rule(object_ as o) { return <- JV(o) }
           rule(value as v) { return <- v }

           // Array: [ elements ]
           var array_ : array<JsonValue?>
           rule("[", WS, "]", WS) {
               return <- array<JsonValue?>()
           }
           rule("[", WS, element_list as list, "]", WS) {
               return <- list
           }

           // Comma-separated element list
           var element_list : array<JsonValue?>
           rule(*comma_element as els, element as last) {
               els |> push <| last
               return <- els
           }

           var comma_element : JsonValue?
           rule(element as e, ",", WS) { return <- e }

           // Object: { mappings }
           var object_ : table<string; JsonValue?>
           rule("\{", WS, "\}", WS) {
               var empty : table<string; JsonValue?>
               return <- empty
           }
           rule("\{", WS, mapping_list as list, "\}", WS) {
               return <- list |> into_table
           }

           // ... mapping rules, value rules (true/false/null/number/string)
       }
   }

Tuple Return Types
==================

Key-value pairs use ``tuple<string; JsonValue?>`` as the return type:

.. code-block:: das

   var mapping : tuple<string; JsonValue?>
   rule(string_ as s, WS, ":", WS, element as e) {
       return <- (s, e)
   }

Tuples are constructed with parenthesized comma expressions.

Verification
============

The tutorial verifies its output matches ``daslib/json``'s built-in
parser:

.. code-block:: das

   var discard_error : string
   var stdlib_json <- read_json(test_input, discard_error)
   parse_json(test_input) $(var peg_json; err) {
       let match_ = write_json(stdlib_json) == write_json(peg_json)
       print("stdlib vs PEG match: {match_}\n")   // true
   }

.. seealso::

   Full source: :download:`tutorials/dasPEG/05_json_parser.das <../../../../tutorials/dasPEG/05_json_parser.das>`

   Next tutorial: :ref:`tutorial_dasPEG_debugging`

   Related: :ref:`tutorial_json` (daslib/json module tutorial)
