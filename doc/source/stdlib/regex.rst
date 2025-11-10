
.. _stdlib_regex:

==========================
Regular expression library
==========================

The `experimental` REGEX module implement regular expression parser and pattern matching functionality.

Currently its in very early stage and implements only very few basic regex operations.

All functions and symbols are in "regex" module, use require to get access to it. ::

    require daslib/regex

++++++++++++
Type aliases
++++++++++++

.. _alias-CharSet:

.. das:attribute:: CharSet = uint[8]

|detail/typedef-regex-CharSet|

.. _alias-ReGenRandom:

.. das:attribute:: ReGenRandom = iterator<uint>

|detail/typedef-regex-ReGenRandom|

.. _alias-MaybeReNode:

.. das:attribute:: variant MaybeReNode

|detail/typedef-regex-MaybeReNode|

++++++++++++
Enumerations
++++++++++++

.. _enum-regex-ReOp:

.. das:attribute:: ReOp

|detail/enumeration-regex-ReOp|

++++++++++
Structures
++++++++++

.. _struct-regex-ReNode:

.. das:attribute:: ReNode

|detail/structure-regex-ReNode|

.. _struct-regex-Regex:

.. das:attribute:: Regex

|detail/structure-regex-Regex|

++++++++++++++++++++++++++
Compilation and validation
++++++++++++++++++++++++++

  *  :ref:`visit_top_down (var node: ReNode?; blk: block\<(var n:ReNode?):void\>) <function-regex_visit_top_down_ReNode_q__block_ls_var_n_c_ReNode_q__c_void_gr_>` 
  *  :ref:`is_valid (var re: Regex) : bool <function-regex_is_valid_Regex>` 
  *  :ref:`regex_compile (var re: Regex; expr: string) : bool <function-regex_regex_compile_Regex_string>` 
  *  :ref:`regex_compile (expr: string) : Regex <function-regex_regex_compile_string>` 
  *  :ref:`regex_compile (var re: Regex) : Regex <function-regex_regex_compile_Regex>` 
  *  :ref:`regex_debug (regex: Regex) <function-regex_regex_debug_Regex>` 
  *  :ref:`debug_set (cset: CharSet) <function-regex_debug_set_CharSet>` 

.. _function-regex_visit_top_down_ReNode_q__block_ls_var_n_c_ReNode_q__c_void_gr_:

.. das:function:: visit_top_down(node: ReNode?; blk: block<(var n:ReNode?):void>)

|detail/function-regex-visit_top_down-0x867105c84f7707dd|

:Arguments: * **node** :  :ref:`ReNode <struct-regex-ReNode>` ?

            * **blk** : block<(n: :ref:`ReNode <struct-regex-ReNode>` ?):void>

.. _function-regex_is_valid_Regex:

.. das:function:: is_valid(re: Regex) : bool

|detail/function-regex-is_valid-0x48938b17eb85e468|

:Arguments: * **re** :  :ref:`Regex <struct-regex-Regex>` 

.. _function-regex_regex_compile_Regex_string:

.. das:function:: regex_compile(re: Regex; expr: string) : bool

|detail/function-regex-regex_compile-0xbee372cb413d18d4|

:Arguments: * **re** :  :ref:`Regex <struct-regex-Regex>` 

            * **expr** : string

.. _function-regex_regex_compile_string:

.. das:function:: regex_compile(expr: string) : Regex

|detail/function-regex-regex_compile-0x8d1fa7153512427c|

:Arguments: * **expr** : string

.. _function-regex_regex_compile_Regex:

.. das:function:: regex_compile(re: Regex) : Regex

|detail/function-regex-regex_compile-0xf92d1317941ea54f|

:Arguments: * **re** :  :ref:`Regex <struct-regex-Regex>` 

.. _function-regex_regex_debug_Regex:

.. das:function:: regex_debug(regex: Regex)

|detail/function-regex-regex_debug-0xb83173e5f53e7307|

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

.. _function-regex_debug_set_CharSet:

.. das:function:: debug_set(cset: CharSet)

|detail/function-regex-debug_set-0xcca0fc7d0b472fcc|

:Arguments: * **cset** :  :ref:`CharSet <alias-CharSet>` 

++++++
Access
++++++

  *  :ref:`regex_group (regex: Regex; index: int; match: string) : string <function-regex_regex_group_Regex_int_string>` 
  *  :ref:`regex_foreach (var regex: Regex; str: string; blk: block\<(at:range):bool\>) <function-regex_regex_foreach_Regex_string_block_ls_at_c_range_c_bool_gr_>` 

.. _function-regex_regex_group_Regex_int_string:

.. das:function:: regex_group(regex: Regex; index: int; match: string) : string

|detail/function-regex-regex_group-0xd24e0018b69956a1|

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

            * **index** : int

            * **match** : string

.. _function-regex_regex_foreach_Regex_string_block_ls_at_c_range_c_bool_gr_:

.. das:function:: regex_foreach(regex: Regex; str: string; blk: block<(at:range):bool>)

|detail/function-regex-regex_foreach-0xf58d1bd2afa1c1cf|

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

            * **str** : string

            * **blk** : block<(at:range):bool>

+++++
Match
+++++

  *  :ref:`regex_match (var regex: Regex; str: string; offset: int = 0) : int <function-regex_regex_match_Regex_string_int>` 

.. _function-regex_regex_match_Regex_string_int:

.. das:function:: regex_match(regex: Regex; str: string; offset: int = 0) : int

|detail/function-regex-regex_match-0x57eb2a8a877d853e|

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

            * **str** : string

            * **offset** : int

++++++++++
Generation
++++++++++

  *  :ref:`re_gen_get_rep_limit () : uint <function-regex_re_gen_get_rep_limit>` 
  *  :ref:`re_gen (var re: Regex; var rnd: ReGenRandom) : string <function-regex_re_gen_Regex_ReGenRandom>` 

.. _function-regex_re_gen_get_rep_limit:

.. das:function:: re_gen_get_rep_limit() : uint

|detail/function-regex-re_gen_get_rep_limit-0xf9842345edbf4128|

.. _function-regex_re_gen_Regex_ReGenRandom:

.. das:function:: re_gen(re: Regex; rnd: ReGenRandom) : string

|detail/function-regex-re_gen-0x43707a23b4974c5a|

:Arguments: * **re** :  :ref:`Regex <struct-regex-Regex>` 

            * **rnd** :  :ref:`ReGenRandom <alias-ReGenRandom>` 

+++++++++++++
Uncategorized
+++++++++++++

.. _function-regex_regex_replace_Regex_string_block_ls_at_c_string_c_string_gr_:

.. das:function:: regex_replace(regex: Regex; str: string; blk: block<(at:string):string>) : string

|detail/function-regex-regex_replace-0x1afe0b10a073cf4f|

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

            * **str** : string

            * **blk** : block<(at:string):string>


