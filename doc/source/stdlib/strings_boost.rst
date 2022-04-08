
.. _stdlib_strings_boost:

=============================================
Boost package for string manipulation library
=============================================

.. include:: detail/strings_boost.rst

The STRINGS boost module implements collection of helper macros and functions to accompany :ref:`STRINGS <stdlib_strings>`.

All functions and symbols are in "strings_boost" module, use require to get access to it. ::

    require daslib/strings_boost


++++++++++++++
Split and join
++++++++++++++

  *  :ref:`split (text:string const;delim:string const) : array\<string\> <function-_at_strings_boost_c__c_split_Cs_Cs>` 
  *  :ref:`split_by_chars (text:string const;delim:string const) : array\<string\> <function-_at_strings_boost_c__c_split_by_chars_Cs_Cs>` 
  *  :ref:`join (it:auto const;separator:string const) : auto <function-_at_strings_boost_c__c_join_C._Cs>` 
  *  :ref:`split (text:string const;delim:string const;blk:block\<(arg:array\<string\> const#):auto\> const) : auto <function-_at_strings_boost_c__c_split_Cs_Cs_CN_ls_arg_gr_0_ls_C_hh_1_ls_s_gr_A_gr_1_ls_._gr__builtin_>` 
  *  :ref:`split_by_chars (text:string const;delim:string const;blk:block\<(arg:array\<string\> const#):auto\> const) : auto <function-_at_strings_boost_c__c_split_by_chars_Cs_Cs_CN_ls_arg_gr_0_ls_C_hh_1_ls_s_gr_A_gr_1_ls_._gr__builtin_>` 

.. _function-_at_strings_boost_c__c_split_Cs_Cs:

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

.. _function-_at_strings_boost_c__c_split_by_chars_Cs_Cs:

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

.. _function-_at_strings_boost_c__c_join_C._Cs:

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

.. _function-_at_strings_boost_c__c_split_Cs_Cs_CN_ls_arg_gr_0_ls_C_hh_1_ls_s_gr_A_gr_1_ls_._gr__builtin_:

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

.. _function-_at_strings_boost_c__c_split_by_chars_Cs_Cs_CN_ls_arg_gr_0_ls_C_hh_1_ls_s_gr_A_gr_1_ls_._gr__builtin_:

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

++++++++++
Formatting
++++++++++

  *  :ref:`wide (text:string const;width:int const) : string <function-_at_strings_boost_c__c_wide_Cs_Ci>` 

.. _function-_at_strings_boost_c__c_wide_Cs_Ci:

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

+++++++++++++++++++++++
Queries and comparisons
+++++++++++++++++++++++

  *  :ref:`is_character_at (foo:array\<uint8\> const#;idx:int const;ch:int const) : auto <function-_at_strings_boost_c__c_is_character_at_C_hh_1_ls_u8_gr_A_Ci_Ci>` 
  *  :ref:`eq (a:string const;b:$::das_string const) : auto <function-_at_strings_boost_c__c_eq_Cs_CH_ls__builtin__c__c_das_string_gr_>` 
  *  :ref:`eq (b:$::das_string const;a:string const) : auto <function-_at_strings_boost_c__c_eq_CH_ls__builtin__c__c_das_string_gr__Cs>` 

.. _function-_at_strings_boost_c__c_is_character_at_C_hh_1_ls_u8_gr_A_Ci_Ci:

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

.. _function-_at_strings_boost_c__c_eq_Cs_CH_ls__builtin__c__c_das_string_gr_:

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

.. _function-_at_strings_boost_c__c_eq_CH_ls__builtin__c__c_das_string_gr__Cs:

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


