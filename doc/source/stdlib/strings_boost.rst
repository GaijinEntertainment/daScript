
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

.. _function-_at_strings_boost_c__c_split__hh_const_hh_string__hh_const_hh_string:

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

.. _function-_at_strings_boost_c__c_split_by_chars__hh_const_hh_string__hh_const_hh_string:

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

.. _function-_at_strings_boost_c__c_wide__hh_const_hh_string__hh_const_hh_int:

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

.. _function-_at_strings_boost_c__c_eq__hh_const_hh__hh_handle_hh_das_string__hh_const_hh_string:

.. das:function:: eq(b: das_string const; a: string const)

eq returns auto

+--------+--------------------------------------------------------------+
+argument+argument type                                                 +
+========+==============================================================+
+b       + :ref:`builtin::das_string <handle-builtin-das_string>`  const+
+--------+--------------------------------------------------------------+
+a       +string const                                                  +
+--------+--------------------------------------------------------------+


|function-strings_boost-eq|

.. _function-_at_strings_boost_c__c_eq__hh_const_hh_string__hh_const_hh__hh_handle_hh_das_string:

.. das:function:: eq(a: string const; b: das_string const)

eq returns auto

+--------+--------------------------------------------------------------+
+argument+argument type                                                 +
+========+==============================================================+
+a       +string const                                                  +
+--------+--------------------------------------------------------------+
+b       + :ref:`builtin::das_string <handle-builtin-das_string>`  const+
+--------+--------------------------------------------------------------+


|function-strings_boost-eq|

.. _function-_at_strings_boost_c__c_is_character_at__hh_const_hh__hh_temporary_hh__hh_array_hh_uint8__hh_const_hh_int__hh_const_hh_int:

.. das:function:: is_character_at(foo: array<uint8> const#; idx: int const; ch: int const)

is_character_at returns auto

+--------+-------------------+
+argument+argument type      +
+========+===================+
+foo     +array<uint8> const#+
+--------+-------------------+
+idx     +int const          +
+--------+-------------------+
+ch      +int const          +
+--------+-------------------+


|function-strings_boost-is_character_at|

.. _function-_at_strings_boost_c__c_join__hh_const_hh__hh_auto__hh_const_hh_string:

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

.. _function-_at_strings_boost_c__c_split__hh_const_hh_string__hh_const_hh_string__hh_const_hh__hh_block_hh__hh_const_hh__hh_temporary_hh__hh_array_hh_string_hh__rq_arg_hh__c__hh_auto:

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

.. _function-_at_strings_boost_c__c_split_by_chars__hh_const_hh_string__hh_const_hh_string__hh_const_hh__hh_block_hh__hh_const_hh__hh_temporary_hh__hh_array_hh_string_hh__rq_arg_hh__c__hh_auto:

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


