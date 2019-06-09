.. _stdlib_stdstringlib:

==================
The String library
==================

the string lib implements string formatting and regular expression matching routines.

++++++++++++++
Global Symbols
++++++++++++++

.. js:function:: endswith(str, cmp)

    returns `true` if the end of the string `str`  matches a the string `cmp` otherwise returns `false`

.. js:function:: startswith(str, cmp)

    returns `true` if the beginning of the string `str` matches the string `cmp`; otherwise returns `false`

.. js:function:: strip_left(str)

    Strips white-space-only characters that might appear at the beginning of the given string
    and returns the new stripped string.

.. js:function:: strip_right(str)

    Strips white-space-only characters that might appear at the end of the given string
    and returns the new stripped string.

.. js:function:: strip(str)

    Strips white-space-only characters that might appear at the beginning or end of the given string and returns the new stripped string.

.. js:function:: find(str, substr, [at])

    Return index where substr czn be found within str (starting from optional at), or -1 if not found

.. js:function:: length(str)

    Return length of string

.. js:function:: reverse(str)

    Return reversed string

.. js:function:: tolower(str)

    Return all lower case string

.. js:function:: toupper(str)

    Return all upper case string

.. js:function:: slice(str, start [, end])

    Return all part of the strings starting at start and ending by end.
    Start can be negative (-1 means "1 from the end").


++++++++++++++
String builder
++++++++++++++

