.. _tutorial_regex:

========================
Regular Expressions
========================

.. index::
    single: Tutorial; Regular Expressions
    single: Tutorial; Regex
    single: Tutorial; Pattern Matching (Regex)
    single: Tutorial; regex_boost

This tutorial covers ``daslib/regex`` and ``daslib/regex_boost`` — compiling,
matching, and replacing text with regular expressions in daScript.

``regex`` provides the core compiler, matcher, and iterator APIs.
``regex_boost`` adds the ``%regex~`` reader macro for compile-time patterns.

Compiling and matching
======================

``regex_compile`` creates a ``Regex`` from a pattern string.
``regex_match`` returns the end position of the match (from position 0),
or ``-1`` on failure::

  var re <- regex_compile("hello")
  let pos = regex_match(re, "hello world")   // 5
  let no  = regex_match(re, "goodbye")       // -1

An optional third argument specifies a starting offset::

  var re2 <- regex_compile("world")
  let pos2 = regex_match(re2, "hello world", 6)   // 11

Character classes
=================

Built-in shorthand classes match common character categories:

========  ================================
Escape    Meaning
========  ================================
``\w``    Word chars ``[a-zA-Z0-9_]``
``\W``    Non-word chars
``\d``    Digits ``[0-9]``
``\D``    Non-digits
``\s``    Whitespace (space, tab, newline, CR, form-feed, vertical-tab)
``\S``    Non-whitespace
``\t``    Tab
``\n``    Newline
``\r``    Carriage return
========  ================================

Example::

  var re_num <- regex_compile("\\d+")
  regex_match(re_num, "12345")    // 5

  var re_ws <- regex_compile("\\s+")
  regex_match(re_ws, "   x")     // 3

Anchors
=======

``^`` anchors the match to the beginning of the string (or offset position).
``$`` anchors to the end::

  var re_start <- regex_compile("^hello")
  regex_match(re_start, "hello")       // 5
  regex_match(re_start, "say hello")   // -1

  var re_full <- regex_compile("^abc$")
  regex_match(re_full, "abc")          // 3
  regex_match(re_full, "abcd")         // -1

Quantifiers
===========

==========  ====================================
Syntax      Meaning
==========  ====================================
``+``       One or more (greedy)
``*``       Zero or more (greedy)
``?``       Zero or one
``{n}``     Exactly *n* repetitions
``{n,}``    *n* or more repetitions (greedy)
``{n,m}``   Between *n* and *m* repetitions (greedy)
==========  ====================================

::

  var re_plus <- regex_compile("a+")
  regex_match(re_plus, "aaa")       // 3

  var re_q <- regex_compile("colou?r")
  regex_match(re_q, "color")        // 5
  regex_match(re_q, "colour")       // 6

Counted quantifiers use braces (escaped as ``\{`` in daScript strings)::

  var re_exact <- regex_compile("\\d\{4}")
  regex_match(re_exact, "1234")     // 4

  var re_range <- regex_compile("a\{2,4}")
  regex_match(re_range, "a")        // -1
  regex_match(re_range, "aaa")      // 3
  regex_match(re_range, "aaaaa")    // 4

Groups and alternation
======================

Parentheses create capturing groups. ``|`` separates alternatives::

  var re_alt <- regex_compile("cat|dog")
  regex_match(re_alt, "cat")    // 3
  regex_match(re_alt, "dog")    // 3

``regex_group`` retrieves group captures after a successful match::

  var re_grp <- regex_compile("(\\w+)@(\\w+)")
  let inp = "user@host"
  regex_match(re_grp, inp)                  // 9
  print("{regex_group(re_grp, 1, inp)}\n")  // user
  print("{regex_group(re_grp, 2, inp)}\n")  // host

Character sets
==============

Square brackets define a set of characters to match:

- ``[abc]`` — matches ``a``, ``b``, or ``c``
- ``[a-z]`` — matches a range
- ``[^abc]`` — negated set (matches anything NOT listed)
- ``[\d_]`` — shorthand classes work inside sets

::

  var re_vowel <- regex_compile("[aeiou]+")
  regex_match(re_vowel, "aeiou")    // 5

  var re_neg <- regex_compile("[^0-9]+")
  regex_match(re_neg, "abc")        // 3

Word boundaries
===============

``\b`` matches at a word boundary — the transition between ``\w`` and ``\W``
characters, or at the start/end of the string.
``\B`` matches at a non-boundary position::

  var re_bnd <- regex_compile("\\bhello\\b")
  regex_match(re_bnd, "hello")       // 5

  var re_nb <- regex_compile("\\Bell")
  regex_match(re_nb, "hello", 1)     // 4

Foreach and replace
===================

``regex_foreach`` iterates over all non-overlapping matches, passing each
match range (as ``int2``) to a block. Return ``true`` to continue::

  var re_num <- regex_compile("\\d+")
  regex_foreach(re_num, "a12b34c56") <| $(at) {
      print("[{at.x},{at.y}] ")   // [1,3] [4,6] [7,9]
      return true
  }

``regex_replace`` replaces every match using a block that receives the
matched substring and returns the replacement::

  let result = regex_replace(re_num, "a12b34c56") <| $(match_str) {
      return "X"
  }
  print("{result}\n")   // aXbXcX

Escaped metacharacters
======================

Backslash escapes literal metacharacters: ``\.`` ``\+`` ``\*`` ``\(`` ``\)``
``\[`` ``\]`` ``\|`` ``\\`` ``\^`` ``\{`` ``\}``::

  var re_dot <- regex_compile("\\d+\\.\\d+")
  regex_match(re_dot, "3.14")       // 4

  var re_parens <- regex_compile("\\(\\w+\\)")
  regex_match(re_parens, "(hello)")   // 7

Hex escapes
===========

``\xHH`` matches a character by its hexadecimal code::

  var re_hex <- regex_compile("\\x41")
  regex_match(re_hex, "A")    // 1  (0x41 = 'A')

Reader macro
============

``regex_boost`` provides the ``%regex~`` reader macro which compiles
a pattern at compile time. No double-escaping is needed — backslashes
are literal in the macro body::

  require daslib/regex_boost

  var re <- %regex~\d+%%
  regex_match(re, "42abc")    // 2

  var re2 <- %regex~[a-z]+%%
  regex_match(re2, "hello")   // 5

Practical examples
==================

Phone number validation::

  var re_phone <- regex_compile("^\\d\{3}-\\d\{4}$")
  regex_match(re_phone, "555-1234") != -1   // true
  regex_match(re_phone, "55-1234")  != -1   // false

Strip non-word characters::

  var re_strip <- regex_compile("[^\\w]+")
  let cleaned = regex_replace(re_strip, "he!l@l#o") <| $(m) {
      return ""
  }
  // cleaned == "hello"

Extract email parts::

  var re_email <- regex_compile("([\\w.]+)@([\\w.]+)")
  let email = "user@example.com"
  regex_match(re_email, email)
  regex_group(re_email, 1, email)   // "user"
  regex_group(re_email, 2, email)   // "example.com"

IP address pattern::

  var re_ip <- regex_compile("\\d\{1,3}\\.\\d\{1,3}\\.\\d\{1,3}\\.\\d\{1,3}")
  regex_match(re_ip, "192.168.1.1")   // 11

.. seealso::

   Full source: :download:`tutorials/language/31_regex.das <../../../../tutorials/language/31_regex.das>`

   :ref:`JSON tutorial <tutorial_json>` (previous tutorial).

   :doc:`/stdlib/regex` — core regex module reference.

   :doc:`/stdlib/regex_boost` — regex boost module reference.
