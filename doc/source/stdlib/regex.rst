
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


.. _alias-MaybeReNode:

.. das:attribute:: MaybeReNode is a variant type

+-------+---------------------------------------------+
+value  + :ref:`regex::ReNode <struct-regex-ReNode>` ?+
+-------+---------------------------------------------+
+nothing+void?                                        +
+-------+---------------------------------------------+


Single regular expression node or nothing.

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


Type of regular expression operation.

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


Single node in regular expression parsing tree.

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


Regular expression.

++++++++++++++++++++++++++
Compilation and validation
++++++++++++++++++++++++++

  *  :ref:`is_valid (re:regex::Regex -const) : bool <function-_at_regex_c__c_is_valid_S_ls_Regex_gr_>` 
  *  :ref:`regex_compile (re:regex::Regex -const;expr:string const) : bool <function-_at_regex_c__c_regex_compile_S_ls_Regex_gr__Cs>` 
  *  :ref:`regex_compile (expr:string const) : regex::Regex <function-_at_regex_c__c_regex_compile_Cs>` 
  *  :ref:`regex_compile (re:regex::Regex -const) : regex::Regex <function-_at_regex_c__c_regex_compile_S_ls_Regex_gr_>` 
  *  :ref:`regex_debug (regex:regex::Regex const) : void <function-_at_regex_c__c_regex_debug_CS_ls_Regex_gr_>` 
  *  :ref:`debug_set (cset:uint const[8]) : void <function-_at_regex_c__c_debug_set_C[8]Y_ls_CharSet_gr_u>` 

.. _function-_at_regex_c__c_is_valid_S_ls_Regex_gr_:

.. das:function:: is_valid(re: Regex)

is_valid returns bool

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+re      + :ref:`regex::Regex <struct-regex-Regex>` +
+--------+------------------------------------------+


returns `true` if enumeration compiled correctly

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


Compile regular expression.
Validity of the compiled expression is checked by `is_valid`.

.. _function-_at_regex_c__c_regex_compile_Cs:

.. das:function:: regex_compile(expr: string const)

regex_compile returns  :ref:`regex::Regex <struct-regex-Regex>` 

+--------+-------------+
+argument+argument type+
+========+=============+
+expr    +string const +
+--------+-------------+


Compile regular expression.
Validity of the compiled expression is checked by `is_valid`.

.. _function-_at_regex_c__c_regex_compile_S_ls_Regex_gr_:

.. das:function:: regex_compile(re: Regex)

regex_compile returns  :ref:`regex::Regex <struct-regex-Regex>` 

+--------+------------------------------------------+
+argument+argument type                             +
+========+==========================================+
+re      + :ref:`regex::Regex <struct-regex-Regex>` +
+--------+------------------------------------------+


Compile regular expression.
Validity of the compiled expression is checked by `is_valid`.

.. _function-_at_regex_c__c_regex_debug_CS_ls_Regex_gr_:

.. das:function:: regex_debug(regex: Regex const)

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+regex   + :ref:`regex::Regex <struct-regex-Regex>`  const+
+--------+------------------------------------------------+


Prints regular expression and its related information in human readable form.

.. _function-_at_regex_c__c_debug_set_C[8]Y_ls_CharSet_gr_u:

.. das:function:: debug_set(cset: CharSet)

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+cset    + :ref:`CharSet <alias-CharSet>` +
+--------+--------------------------------+


Prints character set in human readable form.

++++++
Access
++++++

  *  :ref:`regex_group (regex:regex::Regex const;index:int const;match:string const) : string <function-_at_regex_c__c_regex_group_CS_ls_Regex_gr__Ci_Cs>` 
  *  :ref:`regex_foreach (regex:regex::Regex -const;str:string const;blk:block\<(at:range const):bool\> const) : void <function-_at_regex_c__c_regex_foreach_S_ls_Regex_gr__Cs_CN_ls_at_gr_0_ls_Cr_gr_1_ls_b_gr__builtin_>` 

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


Returns string for the given group index and match result.

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


Iterates through all matches for the given regular expression in `str`.

+++++
Match
+++++

  *  :ref:`regex_match (regex:regex::Regex -const;str:string const;offset:int const) : int <function-_at_regex_c__c_regex_match_S_ls_Regex_gr__Cs_Ci>` 

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


Returns first match for the regular expression in `str`.
If `offset` is specified, first that many number of symbols will not be matched.


