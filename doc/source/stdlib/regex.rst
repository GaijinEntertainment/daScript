
.. _stdlib_regex:

==========================
Regular expression library
==========================

.. include:: detail/regex.rst

The `experimental` REGEX module implement regular expression parser and pattern matching functionality.

Currently its in very early stage and implements only very few basic regex operations.

All functions and symbols are in "regex" module, use require to get access to it. ::

    require daslib/regex


++++++++++++
Type aliases
++++++++++++

.. _alias-CharSet:

.. das:attribute:: CharSet = uint[8]

|typedef-regex-CharSet|

.. _alias-MaybeReNode:

.. das:attribute:: MaybeReNode is a variant type

+-------+---------------------------------------------+
+value  + :ref:`regex::ReNode <struct-regex-ReNode>` ?+
+-------+---------------------------------------------+
+nothing+void?                                        +
+-------+---------------------------------------------+


|typedef-regex-MaybeReNode|

+++++++++
Constants
+++++++++

.. _global-regex-log_parse_enabled:

.. das:attribute:: log_parse_enabled = false

|variable-regex-log_parse_enabled|

.. _global-regex-log_match_enabled:

.. das:attribute:: log_match_enabled = false

|variable-regex-log_match_enabled|

.. _global-regex-range_check_enabled:

.. das:attribute:: range_check_enabled = false

|variable-regex-range_check_enabled|

.. _global-regex-meta:

.. das:attribute:: meta = "\\+-*.()[]|^"

|variable-regex-meta|

.. _global-regex-meta_set:

.. das:attribute:: meta_set = "wWsSdD"

|variable-regex-meta_set|

++++++++++++
Enumerations
++++++++++++

.. _enum-regex-ReOp:

.. das:attribute:: ReOp

+--------+-+
+Char    +0+
+--------+-+
+Set     +1+
+--------+-+
+Any     +2+
+--------+-+
+Eos     +3+
+--------+-+
+Group   +4+
+--------+-+
+Plus    +5+
+--------+-+
+Star    +6+
+--------+-+
+Question+7+
+--------+-+
+Concat  +8+
+--------+-+
+Union   +9+
+--------+-+


|enumeration-regex-ReOp|

.. _struct-regex-ReNode:

.. das:attribute:: ReNode



ReNode fields are

+-------+---------------------------------------------------------------------------------------------------------------------------------------+
+op     + :ref:`regex::ReOp <enum-regex-ReOp>`                                                                                                  +
+-------+---------------------------------------------------------------------------------------------------------------------------------------+
+id     +int                                                                                                                                    +
+-------+---------------------------------------------------------------------------------------------------------------------------------------+
+fun2   +function<(regex: :ref:`regex::Regex <struct-regex-Regex>` ;node: :ref:`regex::ReNode <struct-regex-ReNode>` ?;str:uint8? const):uint8?>+
+-------+---------------------------------------------------------------------------------------------------------------------------------------+
+at     +range                                                                                                                                  +
+-------+---------------------------------------------------------------------------------------------------------------------------------------+
+text   +string                                                                                                                                 +
+-------+---------------------------------------------------------------------------------------------------------------------------------------+
+textLen+int                                                                                                                                    +
+-------+---------------------------------------------------------------------------------------------------------------------------------------+
+all    +array< :ref:`regex::ReNode <struct-regex-ReNode>` ?>                                                                                   +
+-------+---------------------------------------------------------------------------------------------------------------------------------------+
+left   + :ref:`regex::ReNode <struct-regex-ReNode>` ?                                                                                          +
+-------+---------------------------------------------------------------------------------------------------------------------------------------+
+right  + :ref:`regex::ReNode <struct-regex-ReNode>` ?                                                                                          +
+-------+---------------------------------------------------------------------------------------------------------------------------------------+
+subexpr+ :ref:`regex::ReNode <struct-regex-ReNode>` ?                                                                                          +
+-------+---------------------------------------------------------------------------------------------------------------------------------------+
+next   + :ref:`regex::ReNode <struct-regex-ReNode>` ?                                                                                          +
+-------+---------------------------------------------------------------------------------------------------------------------------------------+
+cset   + :ref:`CharSet <alias-CharSet>`                                                                                                        +
+-------+---------------------------------------------------------------------------------------------------------------------------------------+
+index  +int                                                                                                                                    +
+-------+---------------------------------------------------------------------------------------------------------------------------------------+
+tail   +uint8?                                                                                                                                 +
+-------+---------------------------------------------------------------------------------------------------------------------------------------+


|structure-regex-ReNode|

.. _struct-regex-Regex:

.. das:attribute:: Regex



Regex fields are

+-----------+---------------------------------------------+
+root       + :ref:`regex::ReNode <struct-regex-ReNode>` ?+
+-----------+---------------------------------------------+
+match      +uint8?                                       +
+-----------+---------------------------------------------+
+groups     +array<tuple<range;string>>                   +
+-----------+---------------------------------------------+
+earlyOut   + :ref:`CharSet <alias-CharSet>`              +
+-----------+---------------------------------------------+
+canEarlyOut+bool                                         +
+-----------+---------------------------------------------+


|structure-regex-Regex|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_regex_c__c_re_early_out_[8]Y_ls_CharSet_gr_u_C1_ls_S_ls_ReNode_gr__gr_?:

.. das:function:: re_early_out(cset: CharSet; node: regex::ReNode? const)

re_early_out returns bool

+--------+---------------------------------------------------+
+argument+argument type                                      +
+========+===================================================+
+cset    + :ref:`CharSet <alias-CharSet>`                    +
+--------+---------------------------------------------------+
+node    + :ref:`regex::ReNode <struct-regex-ReNode>` ? const+
+--------+---------------------------------------------------+


|function-regex-re_early_out|

.. _function-_at_regex_c__c_is_valid_S_ls_Regex_gr_:

.. das:function:: is_valid(re: Regex)

is_valid returns bool

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+re      + :ref:`regex::Regex <struct-regex-Regex>` +
+--------+------------------------------------------+


|function-regex-is_valid|

.. _function-_at_regex_c__c_regex_compile_S_ls_Regex_gr__Cs:

.. das:function:: regex_compile(re: Regex; expr: string const)

regex_compile returns bool

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+re      + :ref:`regex::Regex <struct-regex-Regex>` +
+--------+------------------------------------------+
+expr    +string const                              +
+--------+------------------------------------------+


|function-regex-regex_compile|

.. _function-_at_regex_c__c_regex_compile_Cs:

.. das:function:: regex_compile(expr: string const)

regex_compile returns  :ref:`regex::Regex <struct-regex-Regex>` 

+--------+-------------+
+argument+argument type+
+========+=============+
+expr    +string const +
+--------+-------------+


|function-regex-regex_compile|

.. _function-_at_regex_c__c_regex_compile_S_ls_Regex_gr_:

.. das:function:: regex_compile(re: Regex)

regex_compile returns  :ref:`regex::Regex <struct-regex-Regex>` 

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+re      + :ref:`regex::Regex <struct-regex-Regex>` +
+--------+------------------------------------------+


|function-regex-regex_compile|

.. _function-_at_regex_c__c_regex_match_S_ls_Regex_gr__Cs_Ci:

.. das:function:: regex_match(regex: Regex; str: string const; offset: int const)

regex_match returns int

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+regex   + :ref:`regex::Regex <struct-regex-Regex>` +
+--------+------------------------------------------+
+str     +string const                              +
+--------+------------------------------------------+
+offset  +int const                                 +
+--------+------------------------------------------+


|function-regex-regex_match|

.. _function-_at_regex_c__c_regex_group_CS_ls_Regex_gr__Ci_Cs:

.. das:function:: regex_group(regex: Regex const; index: int const; match: string const)

regex_group returns string

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+regex   + :ref:`regex::Regex <struct-regex-Regex>`  const+
+--------+------------------------------------------------+
+index   +int const                                       +
+--------+------------------------------------------------+
+match   +string const                                    +
+--------+------------------------------------------------+


|function-regex-regex_group|

.. _function-_at_regex_c__c_regex_foreach_S_ls_Regex_gr__Cs_CN_ls_at_gr_0_ls_Cr_gr_1_ls_b_gr__builtin_:

.. das:function:: regex_foreach(regex: Regex; str: string const; blk: block<(at:range const):bool> const)

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+regex   + :ref:`regex::Regex <struct-regex-Regex>` +
+--------+------------------------------------------+
+str     +string const                              +
+--------+------------------------------------------+
+blk     +block<(at:range const):bool> const        +
+--------+------------------------------------------+


|function-regex-regex_foreach|

.. _function-_at_regex_c__c_regex_debug_CS_ls_Regex_gr_:

.. das:function:: regex_debug(regex: Regex const)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+regex   + :ref:`regex::Regex <struct-regex-Regex>`  const+
+--------+------------------------------------------------+


|function-regex-regex_debug|

.. _function-_at_regex_c__c_debug_set_C[8]Y_ls_CharSet_gr_u:

.. das:function:: debug_set(cset: CharSet)

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+cset    + :ref:`CharSet <alias-CharSet>` +
+--------+--------------------------------+


|function-regex-debug_set|


