
.. _stdlib_strings_boost:

=============================================
Boost package for string manipulation library
=============================================

.. include:: detail/strings_boost.rst

The STRINGS boost module implements collection of helper macros and functions to accompany :ref:`STRINGS <stdlib_strings>`.

All functions and symbols are in "strings_boost" module, use require to get access to it. ::

    require daslib/strings_boost


+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_strings_boost_c__c_split_string_hh_const_string_hh_const:

.. das:function:: split(text: string const; delim: string const)

split returns array<string>

+--------+-------------+
+argument+argument type+
+========+=============+
+text    +string const +
+--------+-------------+
+delim   +string const +
+--------+-------------+


|function-strings_boost-split|

.. _function-_at_strings_boost_c__c_split_by_chars_string_hh_const_string_hh_const:

.. das:function:: split_by_chars(text: string const; delim: string const)

split_by_chars returns array<string>

+--------+-------------+
+argument+argument type+
+========+=============+
+text    +string const +
+--------+-------------+
+delim   +string const +
+--------+-------------+


|function-strings_boost-split_by_chars|

.. _function-_at_strings_boost_c__c_wide_string_hh_const_int_hh_const:

.. das:function:: wide(text: string const; width: int const)

wide returns string

+--------+-------------+
+argument+argument type+
+========+=============+
+text    +string const +
+--------+-------------+
+width   +int const    +
+--------+-------------+


|function-strings_boost-wide|

.. _function-_at_strings_boost_c__c_join__hh_auto_hh_const_string_hh_const:

.. das:function:: join(it: auto const; separator: string const)

join returns auto

+---------+-------------+
+argument +argument type+
+=========+=============+
+it       +auto const   +
+---------+-------------+
+separator+string const +
+---------+-------------+


|function-strings_boost-join|

.. _function-_at_strings_boost_c__c_split_string_hh_const_string_hh_const__hh_block_hh__hh_array_hh_string_hh_const_hh_temporary_hh__rq_arg_hh__c__hh_auto_hh_const:

.. das:function:: split(text: string const; delim: string const; blk: block<(arg:array<string> const#):auto> const)

split returns auto

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+text    +string const                                +
+--------+--------------------------------------------+
+delim   +string const                                +
+--------+--------------------------------------------+
+blk     +block<(arg:array<string> const#):auto> const+
+--------+--------------------------------------------+


|function-strings_boost-split|

.. _function-_at_strings_boost_c__c_split_by_chars_string_hh_const_string_hh_const__hh_block_hh__hh_array_hh_string_hh_const_hh_temporary_hh__rq_arg_hh__c__hh_auto_hh_const:

.. das:function:: split_by_chars(text: string const; delim: string const; blk: block<(arg:array<string> const#):auto> const)

split_by_chars returns auto

+--------+--------------------------------------------+
+argument+argument type                               +
+========+============================================+
+text    +string const                                +
+--------+--------------------------------------------+
+delim   +string const                                +
+--------+--------------------------------------------+
+blk     +block<(arg:array<string> const#):auto> const+
+--------+--------------------------------------------+


|function-strings_boost-split_by_chars|


