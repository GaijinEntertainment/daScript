.. _tutorial_strings:

========================
Strings
========================

.. index::
    single: Tutorial; Strings

This tutorial covers string literals, escape sequences, string interpolation,
character access, slicing, type conversions, and utilities from the
``strings`` and ``daslib/strings_boost`` modules.

String literals
===============

String literals are enclosed in double quotes.  Standard escape sequences are
supported::

  print("tab:\there\n")
  print("newline:\nhere\n")
  print("backslash: \\\n")
  print("quote: \"\n")

String interpolation
====================

Expressions inside ``{ }`` are evaluated and inserted::

  let name = "daScript"
  let version = 0.5
  print("{name} version {version}\n")

Length and character access
===========================

``length`` returns the number of bytes.  ``character_at`` returns the integer
character code at a given index.  Convert back with ``to_char``::

  let text = "Hello"
  print("{length(text)}\n")     // 5

  let ch = character_at(text, 0)
  print("{ch}\n")               // 72  (ASCII for 'H')
  print("{to_char(ch)}\n")      // H

Slicing
=======

``slice`` extracts a substring by start and end indices::

  let phrase = "Hello, World!"
  print("{slice(phrase, 0, 5)}\n")   // Hello
  print("{slice(phrase, 7)}\n")      // World!

Comparison
==========

Strings support the usual comparison operators::

  "abc" == "abc"   // true
  "abc" < "xyz"    // true

Converting to and from strings
==============================

``string(value)`` converts numeric types to a string. For booleans, use string
interpolation::

  let numStr = string(42)      // "42"
  let fltStr = string(3.14)    // "3.14"

  // For booleans, use interpolation:
  let flag = true
  print("{flag}\n")            // true

To convert strings to numbers, ``require strings`` and use ``to_int`` /
``to_float``::

  require strings

  let n = to_int("123")       // 123
  let f = to_float("3.14")    // 3.14

Note: ``int("123")`` does **not** work — you must use ``to_int``.

strings_boost utilities
=======================

``require daslib/strings_boost`` provides higher-level helpers::

  starts_with("image.png", "image")   // true
  ends_with("image.png", ".png")      // true

  let parts = fixed_array("one", "two", "three")
  join(parts, ", ")                    // "one, two, three"

  var items <- split("apple,banana,cherry", ",")
  // ["apple", "banana", "cherry"]

  replace("aabbcc", "bb", "XX")       // "aaXXcc"
  strip("  hello  ")                  // "hello"

Running the tutorial
====================

::

  daslang.exe tutorials/language/07_strings.das

Full source: ``tutorials/language/07_strings.das``

See also
========

* **Next:** :ref:`tutorial_structs` — structs
* :ref:`String builder <string_builder>` — advanced string construction
