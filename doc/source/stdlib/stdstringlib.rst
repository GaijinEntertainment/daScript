.. _stdlib_stdstringlib:

==================
The String library
==================

the string lib implements string formatting and regular expression matching routines.

++++++++++++++
Global Symbols
++++++++++++++

.. das:function:: ends_with(str, cmp)

    returns `true` if the end of the string `str`  matches a the string `cmp` otherwise returns `false`

.. das:function:: starts_with(str, cmp)

    returns `true` if the beginning of the string `str` matches the string `cmp`; otherwise returns `false`

.. das:function:: strip_left(str)

    Strips white-space-only characters that might appear at the beginning of the given string
    and returns the new stripped string.

.. das:function:: strip_right(str)

    Strips white-space-only characters that might appear at the end of the given string
    and returns the new stripped string.

.. das:function:: strip(str)

    Strips white-space-only characters that might appear at the beginning or end of the given string and returns the new stripped string.

.. das:function:: find(str, substr, [at])

    Return index where substr czn be found within str (starting from optional at), or -1 if not found

.. das:function:: length(str)

    Return length of string

.. das:function:: reverse(str)

    Return reversed string

.. das:function:: to_lower(str)

    Return all lower case string

.. das:function:: to_upper(str)

    Return all upper case string

.. das:function:: slice(str, start [, end])

    Return all part of the strings starting at start and ending by end.
    Start can be negative (-1 means "1 from the end").


++++++++++++++
String builder
++++++++++++++

