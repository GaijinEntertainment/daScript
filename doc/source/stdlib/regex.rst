
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

Bit array which represents an 8-bit character set.

.. _alias-ReGenRandom:

.. das:attribute:: ReGenRandom = iterator<uint>

|detail/typedef-regex-ReGenRandom|

.. _alias-MaybeReNode:

.. das:attribute:: MaybeReNode is a variant type

Single regular expression node or nothing.

:Variants: * **value** :  :ref:`ReNode <struct-regex-ReNode>` ?

           * **nothing** : void?

Single regular expression node or nothing.

++++++++++++
Enumerations
++++++++++++

.. _enum-regex-ReOp:

.. das:attribute:: ReOp

Type of regular expression operation.

:Values: * **Char** = 0

         * **Set** = 1

         * **Any** = 2

         * **Eos** = 3

         * **Group** = 4

         * **Plus** = 5

         * **Star** = 6

         * **Question** = 7

         * **Concat** = 8

         * **Union** = 9

Type of regular expression operation.

++++++++++
Structures
++++++++++

.. _struct-regex-ReNode:

.. das:attribute:: ReNode

Single node in regular expression parsing tree.

:Fields: * **op** :  :ref:`ReOp <enum-regex-ReOp>` 

         * **id** : int

         * **fun2** : function<(regex: :ref:`Regex <struct-regex-Regex>` ;node: :ref:`ReNode <struct-regex-ReNode>` ?;str:uint8?):uint8?>

         * **gen2** : function<(node: :ref:`ReNode <struct-regex-ReNode>` ?;rnd: :ref:`ReGenRandom <alias-ReGenRandom>` ;str: :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ):void>

         * **at** : range

         * **text** : string

         * **textLen** : int

         * **all** : array< :ref:`ReNode <struct-regex-ReNode>` ?>

         * **left** :  :ref:`ReNode <struct-regex-ReNode>` ?

         * **right** :  :ref:`ReNode <struct-regex-ReNode>` ?

         * **subexpr** :  :ref:`ReNode <struct-regex-ReNode>` ?

         * **next** :  :ref:`ReNode <struct-regex-ReNode>` ?

         * **cset** :  :ref:`CharSet <alias-CharSet>` 

         * **index** : int

         * **tail** : uint8?

Single node in regular expression parsing tree.

.. _struct-regex-Regex:

.. das:attribute:: Regex

Regular expression.

:Fields: * **root** :  :ref:`ReNode <struct-regex-ReNode>` ?

         * **match** : uint8?

         * **groups** : array<tuple<range;string>>

         * **earlyOut** :  :ref:`CharSet <alias-CharSet>` 

         * **canEarlyOut** : bool

Regular expression.

++++++++++++++++++++++++++
Compilation and validation
++++++++++++++++++++++++++

  *  :ref:`visit_top_down (var node: ReNode?; blk: block\<(var n:ReNode?):void\>) <function-_at_regex_c__c_visit_top_down_1_ls_S_ls_regex_c__c_ReNode_gr__gr__qm__CN_ls_n_gr_0_ls_1_ls_S_ls_regex_c__c_ReNode_gr__gr__qm__gr_1_ls_v_gr__builtin_>` 
  *  :ref:`is_valid (var re: Regex) : bool <function-_at_regex_c__c_is_valid_S_ls_regex_c__c_Regex_gr_>` 
  *  :ref:`regex_compile (var re: Regex; expr: string) : bool <function-_at_regex_c__c_regex_compile_S_ls_regex_c__c_Regex_gr__Cs>` 
  *  :ref:`regex_compile (expr: string) : Regex <function-_at_regex_c__c_regex_compile_Cs>` 
  *  :ref:`regex_compile (var re: Regex) : Regex <function-_at_regex_c__c_regex_compile_S_ls_regex_c__c_Regex_gr_>` 
  *  :ref:`regex_debug (regex: Regex) <function-_at_regex_c__c_regex_debug_CS_ls_regex_c__c_Regex_gr_>` 
  *  :ref:`debug_set (cset: CharSet) <function-_at_regex_c__c_debug_set_C_lb_8_rb_Y_ls_CharSet_gr_u>` 

.. _function-_at_regex_c__c_visit_top_down_1_ls_S_ls_regex_c__c_ReNode_gr__gr__qm__CN_ls_n_gr_0_ls_1_ls_S_ls_regex_c__c_ReNode_gr__gr__qm__gr_1_ls_v_gr__builtin_:

.. das:function:: visit_top_down(node: ReNode?; blk: block<(var n:ReNode?):void>)

|detail/function-regex-visit_top_down-0xdfa599a02332cf89|

:Arguments: * **node** :  :ref:`ReNode <struct-regex-ReNode>` ?

            * **blk** : block<(n: :ref:`ReNode <struct-regex-ReNode>` ?):void>

.. _function-_at_regex_c__c_is_valid_S_ls_regex_c__c_Regex_gr_:

.. das:function:: is_valid(re: Regex) : bool

:Arguments: * **re** :  :ref:`Regex <struct-regex-Regex>` 


returns `true` if enumeration compiled correctly

.. _function-_at_regex_c__c_regex_compile_S_ls_regex_c__c_Regex_gr__Cs:

.. das:function:: regex_compile(re: Regex; expr: string) : bool

Compile regular expression.
Validity of the compiled expression is checked by `is_valid`.

:Arguments: * **re** :  :ref:`Regex <struct-regex-Regex>` 

            * **expr** : string

.. _function-_at_regex_c__c_regex_compile_Cs:

.. das:function:: regex_compile(expr: string) : Regex

|detail/function-regex-regex_compile-0x8d1fa7153512427c|

:Arguments: * **expr** : string

.. _function-_at_regex_c__c_regex_compile_S_ls_regex_c__c_Regex_gr_:

.. das:function:: regex_compile(re: Regex) : Regex

|detail/function-regex-regex_compile-0xff482d838b45f8a|

:Arguments: * **re** :  :ref:`Regex <struct-regex-Regex>` 

.. _function-_at_regex_c__c_regex_debug_CS_ls_regex_c__c_Regex_gr_:

.. das:function:: regex_debug(regex: Regex)

Prints regular expression and its related information in human readable form.

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

.. _function-_at_regex_c__c_debug_set_C_lb_8_rb_Y_ls_CharSet_gr_u:

.. das:function:: debug_set(cset: CharSet)

Prints character set in human readable form.

:Arguments: * **cset** :  :ref:`CharSet <alias-CharSet>` 

++++++
Access
++++++

  *  :ref:`regex_group (regex: Regex; index: int; match: string) : string <function-_at_regex_c__c_regex_group_CS_ls_regex_c__c_Regex_gr__Ci_Cs>` 
  *  :ref:`regex_foreach (var regex: Regex; str: string; blk: block\<(at:range const):bool\>) <function-_at_regex_c__c_regex_foreach_S_ls_regex_c__c_Regex_gr__Cs_CN_ls_at_gr_0_ls_Cr_gr_1_ls_b_gr__builtin_>` 

.. _function-_at_regex_c__c_regex_group_CS_ls_regex_c__c_Regex_gr__Ci_Cs:

.. das:function:: regex_group(regex: Regex; index: int; match: string) : string

Returns string for the given group index and match result.

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

            * **index** : int

            * **match** : string

.. _function-_at_regex_c__c_regex_foreach_S_ls_regex_c__c_Regex_gr__Cs_CN_ls_at_gr_0_ls_Cr_gr_1_ls_b_gr__builtin_:

.. das:function:: regex_foreach(regex: Regex; str: string; blk: block<(at:range const):bool>)

Iterates through all matches for the given regular expression in `str`.

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

            * **str** : string

            * **blk** : block<(at:range):bool>

+++++
Match
+++++

  *  :ref:`regex_match (var regex: Regex; str: string; offset: int = 0) : int <function-_at_regex_c__c_regex_match_S_ls_regex_c__c_Regex_gr__Cs_Ci>` 

.. _function-_at_regex_c__c_regex_match_S_ls_regex_c__c_Regex_gr__Cs_Ci:

.. das:function:: regex_match(regex: Regex; str: string; offset: int = 0) : int

Returns first match for the regular expression in `str`.
If `offset` is specified, first that many number of symbols will not be matched.

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

            * **str** : string

            * **offset** : int

++++++++++
Generation
++++++++++

  *  :ref:`re_gen_get_rep_limit () : uint <function-_at_regex_c__c_re_gen_get_rep_limit>` 
  *  :ref:`re_gen (var re: Regex; var rnd: ReGenRandom) : string <function-_at_regex_c__c_re_gen_S_ls_regex_c__c_Regex_gr__Y_ls_ReGenRandom_gr_1_ls_u_gr_G>` 

.. _function-_at_regex_c__c_re_gen_get_rep_limit:

.. das:function:: re_gen_get_rep_limit() : uint

|detail/function-regex-re_gen_get_rep_limit-0xf9842345edbf4128|

.. _function-_at_regex_c__c_re_gen_S_ls_regex_c__c_Regex_gr__Y_ls_ReGenRandom_gr_1_ls_u_gr_G:

.. das:function:: re_gen(re: Regex; rnd: ReGenRandom) : string

|detail/function-regex-re_gen-0x313e66971fa6c4f1|

:Arguments: * **re** :  :ref:`Regex <struct-regex-Regex>` 

            * **rnd** :  :ref:`ReGenRandom <alias-ReGenRandom>` 

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_regex_c__c_regex_replace_S_ls_regex_c__c_Regex_gr__Cs_CN_ls_at_gr_0_ls_Cs_gr_1_ls_s_gr__builtin_:

.. das:function:: regex_replace(regex: Regex; str: string; blk: block<(at:string const):string>) : string

Iterates through all matches for the given regular expression in `str`.

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

            * **str** : string

            * **blk** : block<(at:string):string>


