.. _tutorial_dasPEG_hello_parser:

=======================================
PEG-01 --- Hello Parser
=======================================

.. index::
    single: Tutorial; PEG
    single: Tutorial; Parser
    single: Tutorial; dasPEG
    single: Tutorial; Literal Matching
    single: Tutorial; Character Sets

This tutorial introduces the ``peg`` module --- daslang's built-in PEG
(Parsing Expression Grammar) parser generator.  You will learn:

- The ``parse`` macro and grammar structure
- Literal string matching
- The ``EOF`` terminal
- Text extraction with ``"{rule}" as name``
- Handling parse results and errors
- Character sets with ``set()``

Setup
=====

Import the PEG module::

   require peg/peg

All parsers are defined inside a ``parse(input) { ... }`` macro block.
The macro generates a packrat parser at compile time --- no runtime code
generation or external tools needed.

Your First Parser
=================

A grammar is a set of **rules**.  The first ``var`` declaration is the
**entry rule** --- the parser starts matching there.  Each ``rule(...)``
defines an alternative tried in order:

.. code-block:: das

   def parse_hello(input : string;
                   blk : block<(val : bool; err : array<ParsingError>) : void>) {
       parse(input) {
           var greeting : bool
           rule("Hello", EOF) {
               return true
           }
       }
   }

``"Hello"`` matches the literal string.  ``EOF`` requires end-of-input.
The block after ``rule`` runs when the alternative succeeds --- its
``return`` value becomes the parse result.

Call the parser with a callback that receives the value and an error array:

.. code-block:: das

   parse_hello("Hello") $(val; err) {
       print("matched: {val}\n")   // matched: true
   }

Multiple Alternatives (Ordered Choice)
=======================================

PEG parsers try alternatives **in order** --- the first match wins.  This
eliminates ambiguity:

.. code-block:: das

   def parse_greeting(input : string;
                      blk : block<(val : string; err : array<ParsingError>) : void>) {
       parse(input) {
           var greeting : string
           rule("Hello", EOF) { return "hello" }
           rule("Hi", EOF)    { return "hi" }
           rule("Hey", EOF)   { return "hey" }
       }
   }

Text Extraction
===============

Wrap a rule reference in ``"{...}"`` and bind it with ``as`` to extract
the matched text.  ``+rule`` means "one or more" repetitions:

.. code-block:: das

   def parse_name(input : string;
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

Here ``letter`` is a helper rule using ``set()`` to match a character
range.  Its return type ``void?`` marks it as a pattern-only rule (no
value produced).  The ``"{+letter}" as name`` captures all matched text
into the ``name`` variable.

Character Sets
==============

``set()`` matches a single character from one or more ranges or
individual characters:

.. code-block:: das

   set('a'..'z', 'A'..'Z')         // letters
   set('0'..'9')                    // digits
   set('a'..'z', '0'..'9', '_')    // identifier chars

Multiple Rules and WS
=====================

Grammars can have any number of rules.  ``WS`` is a built-in terminal
that matches zero or more whitespace characters:

.. code-block:: das

   parse(input) {
       var entry : string
       rule("{+alpha}" as key, WS, "=", WS, string_ as value, EOF) {
           return "{key}: {value}"
       }

       var alpha : void?
       rule(set('a'..'z', 'A'..'Z', '_')) {
           return null
       }
   }

``string_`` is a built-in terminal that matches a double-quoted string
and returns its content (without quotes).

Error Handling
==============

The parse callback receives both the result and an ``array<ParsingError>``.
On failure, the result is the default value for its type:

.. code-block:: das

   parse_name("Hello, 123!") $(val; err) {
       if (!empty(err)) {
           for (e in err) {
               print("{e.text} (at position {e.index})\n")
           }
       }
   }

Each ``ParsingError`` has a ``text`` description and an ``index``
position in the input string.

Quick Reference
===============

========================  =============================================
Element                   Description
========================  =============================================
``parse(input) { ... }``  Define a grammar and run it on ``input``
``var name : Type``       Declare a rule with the given return type
``rule(...) { ... }``     One alternative for the enclosing rule
``"literal"``             Match exact text
``EOF``                   Match end of input
``WS``                    Match zero or more whitespace characters
``set(ranges...)``        Match one character from ranges
``"{+rule}" as name``     Extract matched text into ``name``
``+rule``                 One or more repetitions
``string_``               Match a double-quoted string literal
``number``                Match a decimal integer
========================  =============================================

.. seealso::

   Full source: :download:`tutorials/dasPEG/01_hello_parser.das <../../../../tutorials/dasPEG/01_hello_parser.das>`

   Next tutorial: :ref:`tutorial_dasPEG_calculator`
