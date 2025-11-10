
.. _stdlib_regex_boost:

=======================
Boost package for REGEX
=======================

The REGEX boost module implements collection of helper macros and functions to accompany :ref:`REGEX <stdlib_regex>`.

All functions and symbols are in "regex_boost" module, use require to get access to it. ::

    require daslib/regex_boost

+++++++++++++
Reader macros
+++++++++++++

.. _call-macro-regex_boost-regex:

.. das:attribute:: regex

Reader macro regex. This once comnversts %regex~ macros into regex::Regex objects during compilation time.


