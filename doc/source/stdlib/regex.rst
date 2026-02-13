
.. _stdlib_regex:

==========================
Regular expression library
==========================

The REGEX module implements regular expression matching and searching.
It provides ``regex_compile`` for building patterns, ``regex_match`` for
full-string matching, and ``regex_foreach`` for finding all matches within text.

See also :doc:`regex_boost` for compile-time regex construction via the ``%regex~`` reader macro.

All functions and symbols are in "regex" module, use require to get access to it. ::

    require daslib/regex

Example: ::

    require daslib/regex
        require strings

        [export]
        def main() {
            var re <- regex_compile("[0-9]+")
            let m = regex_match(re, "123abc")
            print("match length = {m}\n")
            let text = "age 25, height 180"
            regex_foreach(re, text) <| $(r) {
                print("found: {slice(text, r.x, r.y)}\n")
                return true
            }
        }
        // output:
        // match length = 3
        // found: 25
        // found: 180

++++++++++++
Type aliases
++++++++++++

.. _alias-CharSet:

.. das:attribute:: CharSet = uint[8]

Bitfield character set used internally by the regex engine.
.. _alias-ReGenRandom:

.. das:attribute:: ReGenRandom = iterator<uint>

Random number generator callback used by ``re_gen`` for regex-based string generation.
# === MODULE: regex_boost ===
.. _alias-MaybeReNode:

.. das:attribute:: variant MaybeReNode

Regex node or nothing.

:Variants: * **value** :  :ref:`ReNode <struct-regex-ReNode>` ? - Node.

           * **nothing** : void? - Nothing.


++++++++++++
Enumerations
++++++++++++

.. _enum-regex-ReOp:

.. das:attribute:: ReOp

Type of regular expression operation.

:Values: * **Char** = 0 - Matching a character

         * **Set** = 1 - Matching a character set

         * **Any** = 2 - Matches any character

         * **Eos** = 3 - Matches end of string

         * **Group** = 4 - Matching a group

         * **Plus** = 5 - Repetition: one or more

         * **Star** = 6 - Repetition: zero or more

         * **Question** = 7 - Repetition: zero or one

         * **Concat** = 8 - First followed by second

         * **Union** = 9 - Either first or second


++++++++++
Structures
++++++++++

.. _struct-regex-ReNode:

.. das:attribute:: ReNode

Regular expression node.

:Fields: * **op** :  :ref:`ReOp <enum-regex-ReOp>`  - Regex operation

         * **id** : int - Unique node identifier

         * **fun2** : function<(regex: :ref:`Regex <struct-regex-Regex>` ;node: :ref:`ReNode <struct-regex-ReNode>` ?;str:uint8?):uint8?> - Matchig function

         * **gen2** : function<(node: :ref:`ReNode <struct-regex-ReNode>` ?;rnd: :ref:`ReGenRandom <alias-ReGenRandom>` ;str: :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>` ):void> - Generator function

         * **at** : range - Source range

         * **text** : string - Text fragment

         * **textLen** : int - Length of text fragment

         * **all** : array< :ref:`ReNode <struct-regex-ReNode>` ?> - All child nodes

         * **left** :  :ref:`ReNode <struct-regex-ReNode>` ? - Left child node

         * **right** :  :ref:`ReNode <struct-regex-ReNode>` ? - Right child node

         * **subexpr** :  :ref:`ReNode <struct-regex-ReNode>` ? - Subexpression node

         * **next** :  :ref:`ReNode <struct-regex-ReNode>` ? - Next node in the list

         * **cset** :  :ref:`CharSet <alias-CharSet>`  - Character set for character class matching

         * **index** : int - Index for character class matching

         * **tail** : uint8? - Tail of the string


.. _struct-regex-Regex:

.. das:attribute:: Regex

Regular expression structure.

:Fields: * **root** :  :ref:`ReNode <struct-regex-ReNode>` ? - Root node of the regex.

         * **match** : uint8? - Original source text.

         * **groups** : array<tuple<range;string>> - Captured groups.

         * **earlyOut** :  :ref:`CharSet <alias-CharSet>`  - Character set for early out optimization.

         * **canEarlyOut** : bool - Whether early out optimization is enabled.


++++++++++++++++++++++++++
Compilation and validation
++++++++++++++++++++++++++

  *  :ref:`debug_set (cset: CharSet) <function-regex_debug_set_CharSet>` 
  *  :ref:`is_valid (var re: Regex) : bool <function-regex_is_valid_Regex>` 
  *  :ref:`regex_compile (expr: string) : Regex <function-regex_regex_compile_string>` 
  *  :ref:`regex_compile (var re: Regex; expr: string) : bool <function-regex_regex_compile_Regex_string>` 
  *  :ref:`regex_compile (var re: Regex) : Regex <function-regex_regex_compile_Regex>` 
  *  :ref:`regex_debug (regex: Regex) <function-regex_regex_debug_Regex>` 
  *  :ref:`visit_top_down (var node: ReNode?; blk: block\<(var n:ReNode?):void\>) <function-regex_visit_top_down_ReNode_q__block_ls_var_n_c_ReNode_q__c_void_gr_>` 

.. _function-regex_debug_set_CharSet:

.. das:function:: debug_set(cset: CharSet)

Prints all characters contained in a ``CharSet`` for debugging purposes.

:Arguments: * **cset** :  :ref:`CharSet <alias-CharSet>` 

.. _function-regex_is_valid_Regex:

.. das:function:: is_valid(re: Regex) : bool

Returns ``true`` if the compiled regex is valid and ready for matching.

:Arguments: * **re** :  :ref:`Regex <struct-regex-Regex>` 


regex_compile
^^^^^^^^^^^^^

.. _function-regex_regex_compile_string:

.. das:function:: regex_compile(expr: string) : Regex

Compiles a regular expression pattern string into a ``Regex`` object.

:Arguments: * **expr** : string

.. _function-regex_regex_compile_Regex_string:

.. das:function:: regex_compile(re: Regex; expr: string) : bool

.. _function-regex_regex_compile_Regex:

.. das:function:: regex_compile(re: Regex) : Regex

----

.. _function-regex_regex_debug_Regex:

.. das:function:: regex_debug(regex: Regex)

Prints the internal structure of a compiled regex for debugging purposes.

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

.. _function-regex_visit_top_down_ReNode_q__block_ls_var_n_c_ReNode_q__c_void_gr_:

.. das:function:: visit_top_down(node: ReNode?; blk: block<(var n:ReNode?):void>)

Visits all nodes of a compiled regex tree in top-down order, invoking a callback for each node.

:Arguments: * **node** :  :ref:`ReNode <struct-regex-ReNode>` ?

            * **blk** : block<(n: :ref:`ReNode <struct-regex-ReNode>` ?):void>

++++++
Access
++++++

  *  :ref:`regex_foreach (var regex: Regex; str: string; blk: block\<(at:range):bool\>) <function-regex_regex_foreach_Regex_string_block_ls_at_c_range_c_bool_gr_>` 
  *  :ref:`regex_group (regex: Regex; index: int; match: string) : string <function-regex_regex_group_Regex_int_string>` 

.. _function-regex_regex_foreach_Regex_string_block_ls_at_c_range_c_bool_gr_:

.. das:function:: regex_foreach(regex: Regex; str: string; blk: block<(at:range):bool>)

Iterates over all non-overlapping matches of a regex in a string, invoking a block for each match.

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

            * **str** : string

            * **blk** : block<(at:range):bool>

.. _function-regex_regex_group_Regex_int_string:

.. das:function:: regex_group(regex: Regex; index: int; match: string) : string

Returns the substring captured by the specified group index after a successful match.

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

            * **index** : int

            * **match** : string

+++++++++++++++
Match & replace
+++++++++++++++

  *  :ref:`regex_match (var regex: Regex; str: string; offset: int = 0) : int <function-regex_regex_match_Regex_string_int>` 
  *  :ref:`regex_replace (var regex: Regex; str: string; blk: block\<(at:string):string\>) : string <function-regex_regex_replace_Regex_string_block_ls_at_c_string_c_string_gr_>` 

.. _function-regex_regex_match_Regex_string_int:

.. das:function:: regex_match(regex: Regex; str: string; offset: int = 0) : int

Matches a compiled regex against a string and returns the end position of the match, or ``-1`` on failure.

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

            * **str** : string

            * **offset** : int

.. _function-regex_regex_replace_Regex_string_block_ls_at_c_string_c_string_gr_:

.. das:function:: regex_replace(regex: Regex; str: string; blk: block<(at:string):string>) : string

Replaces each substring matched by the regex with the result returned by the provided block.

:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>` 

            * **str** : string

            * **blk** : block<(at:string):string>

++++++++++
Generation
++++++++++

  *  :ref:`re_gen (var re: Regex; var rnd: ReGenRandom) : string <function-regex_re_gen_Regex_ReGenRandom>` 
  *  :ref:`re_gen_get_rep_limit () : uint <function-regex_re_gen_get_rep_limit>` 

.. _function-regex_re_gen_Regex_ReGenRandom:

.. das:function:: re_gen(re: Regex; rnd: ReGenRandom) : string

Generates a random string that matches the given compiled regex.

:Arguments: * **re** :  :ref:`Regex <struct-regex-Regex>` 

            * **rnd** :  :ref:`ReGenRandom <alias-ReGenRandom>` 

.. _function-regex_re_gen_get_rep_limit:

.. das:function:: re_gen_get_rep_limit() : uint

Returns the maximum repetition limit used by regex quantifiers during string generation.


