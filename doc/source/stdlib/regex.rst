
.. _stdlib_regex:

==========================
Regular expression library
==========================

.. das:module:: regex

The REGEX module implements regular expression matching and searching.
It provides ``regex_compile`` for building patterns, ``regex_match`` for
full-string matching, ``regex_search`` for finding the first match anywhere,
``regex_foreach`` for iterating all matches, ``regex_replace`` for substitution
(both block-based and template-string forms), ``regex_split`` for splitting strings,
``regex_match_all`` for collecting all match ranges, ``regex_group`` for
capturing groups by index, and ``regex_group_by_name`` for named group lookup.

See :ref:`tutorial_regex` for a hands-on tutorial.

Supported syntax:

- ``.`` — any character except newline (use ``dot_all=true`` to also match ``\n``)
- ``^`` — beginning of string (or offset position)
- ``$`` — end of string
- ``+`` — one or more (greedy)
- ``*`` — zero or more (greedy)
- ``?`` — zero or one (greedy)
- ``+?`` — one or more (lazy)
- ``*?`` — zero or more (lazy)
- ``??`` — zero or one (lazy)
- ``{n}`` — exactly *n* repetitions
- ``{n,}`` — *n* or more (greedy)
- ``{n,m}`` — between *n* and *m* (greedy)
- ``{n}?`` ``{n,}?`` ``{n,m}?`` — counted repetitions (lazy)
- ``(...)`` — capturing group
- ``(?:...)`` — non-capturing group
- ``(?P<name>...)`` — named capturing group
- ``(?=...)`` — positive lookahead assertion
- ``(?!...)`` — negative lookahead assertion
- ``|`` — alternation
- ``[abc]``, ``[a-z]``, ``[^abc]`` — character sets (negated with ``^``)
- ``\w`` ``\W`` — word / non-word characters
- ``\d`` ``\D`` — digit / non-digit characters
- ``\s`` ``\S`` — whitespace / non-whitespace characters
- ``\b`` ``\B`` — word boundary / non-boundary assertions
- ``\t`` ``\n`` ``\r`` ``\f`` ``\v`` — whitespace escapes
- ``\xHH`` — hexadecimal character escape
- ``\.`` ``\+`` ``\*`` ``\(`` ``\)`` ``\[`` ``\]`` ``\|`` ``\\`` ``\^`` ``\{`` ``\}`` — escaped metacharacters

Flags:

- ``case_insensitive=true`` — ASCII case-insensitive matching (pass to ``regex_compile``)
- ``dot_all=true`` — ``.`` also matches ``\n`` (pass to ``regex_compile``)

Template-string replacement:

``regex_replace(re, str, replacement)`` replaces matches using a template string.
Supported references: ``$0`` or ``$&`` for the whole match, ``$1``–``$9`` for
numbered groups, ``${name}`` for named groups, ``$$`` for a literal ``$``.

The engine is ASCII-only (256-bit ``CharSet``). Matching is anchored — ``regex_match`` tests from
position 0 (or the given offset) and does NOT search; use ``regex_search`` to find the first
occurrence, or ``regex_foreach`` / ``regex_match_all`` to find all occurrences.

See also :doc:`regex_boost` for compile-time regex construction via the ``%regex~`` reader macro.

All functions and symbols are in "regex" module, use require to get access to it.

.. code-block:: das

    require daslib/regex

Example:

.. code-block:: das

    require daslib/regex
        require strings

        [export]
        def main() {
            var re <- regex_compile("[0-9]+")
            let m = regex_match(re, "123abc")
            print("match length = {m}\n")
            let text = "age 25, height 180"
            regex_foreach(re, text) $(r) {
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

.. _alias-MaybeReNode:

.. das:attribute:: variant MaybeReNode

Regex node or nothing.

:Variants: * **value** :  :ref:`ReNode <struct-regex-ReNode>`? - Node.

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

         * **Bos** = 4 - Matches beginning of string

         * **Group** = 5 - Matching a group

         * **Plus** = 6 - Repetition: one or more

         * **Star** = 7 - Repetition: zero or more

         * **Question** = 8 - Repetition: zero or one

         * **Concat** = 9 - First followed by second

         * **Union** = 10 - Either first or second

         * **Repeat** = 11 - Counted repetition: {n}, {n,}, {n,m}

         * **WordBoundary** = 12 - Matches at a word boundary

         * **NonWordBoundary** = 13 - Matches at a non-word boundary

         * **Lookahead** = 14 - Positive lookahead assertion (?=...)

         * **NegativeLookahead** = 15 - Negative lookahead assertion (?!...)




++++++++++
Structures
++++++++++

.. _struct-regex-ReNode:

.. das:attribute:: ReNode

Regular expression node.

:Fields: * **op** :  :ref:`ReOp <enum-regex-ReOp>` - Regex operation

         * **id** : int - Unique node identifier

         * **fun2** : function<(regex: :ref:`Regex <struct-regex-Regex>`;node: :ref:`ReNode <struct-regex-ReNode>`?;str:uint8?):uint8?> - Matchig function

         * **gen2** : function<(node: :ref:`ReNode <struct-regex-ReNode>`?;rnd: :ref:`ReGenRandom <alias-ReGenRandom>`;str: :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`):void> - Generator function

         * **at** : range - Source range

         * **text** : string - Text fragment

         * **textLen** : int - Length of text fragment

         * **all** : array< :ref:`ReNode <struct-regex-ReNode>`?> - All child nodes

         * **left** :  :ref:`ReNode <struct-regex-ReNode>`? - Left child node

         * **right** :  :ref:`ReNode <struct-regex-ReNode>`? - Right child node

         * **subexpr** :  :ref:`ReNode <struct-regex-ReNode>`? - Subexpression node

         * **next** :  :ref:`ReNode <struct-regex-ReNode>`? - Next node in the list

         * **cset** :  :ref:`CharSet <alias-CharSet>` - Character set for character class matching

         * **index** : int - Index for character class matching

         * **min_rep** : int - Minimum repetition count for counted quantifiers

         * **max_rep** : int - Maximum repetition count for counted quantifiers (-1 means unlimited)

         * **lazy** : bool - Whether this quantifier uses lazy matching (``*?``, ``+?``, ``??``, ``{n,m}?``)

         * **tail** : uint8? - Tail of the string



.. _struct-regex-Regex:

.. das:attribute:: Regex

Regular expression structure.

:Fields: * **root** :  :ref:`ReNode <struct-regex-ReNode>`? - Root node of the regex.

         * **match** : uint8? - Original source text.

         * **groups** : array<tuple<range;string>> - Captured groups.

         * **earlyOut** :  :ref:`CharSet <alias-CharSet>` - Character set for early out optimization.

         * **canEarlyOut** : bool - Whether early out optimization is enabled.

         * **caseInsensitive** : bool - When true, matching is case-insensitive (ASCII only).

         * **dotAll** : bool - When true, ``.`` matches newline characters as well.




++++++++++++++++++++++++++
Compilation and validation
++++++++++++++++++++++++++

  *  :ref:`debug_set (cset: CharSet) <function-regex_debug_set_CharSet>`
  *  :ref:`is_valid (var re: Regex) : bool <function-regex_is_valid_Regex>`
  *  :ref:`regex_compile (expr: string; case_insensitive: bool = false; dot_all: bool = false) : Regex <function-regex_regex_compile_string_bool_bool>`
  *  :ref:`regex_compile (var re: Regex; expr: string; case_insensitive: bool = false; dot_all: bool = false) : bool <function-regex_regex_compile_Regex_string_bool_bool>`
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

.. _function-regex_regex_compile_string_bool_bool:

.. das:function:: regex_compile(expr: string; case_insensitive: bool = false; dot_all: bool = false) : Regex

Compiles a regular expression pattern string into a ``Regex`` object. Panics if the pattern is invalid. An overload taking a ``var re : Regex`` out-parameter returns ``bool`` instead of panicking. Optional flags: ``case_insensitive=true`` for ASCII case-insensitive matching, ``dot_all=true`` for ``.`` to also match newline characters.


:Arguments: * **expr** : string

            * **case_insensitive** : bool

            * **dot_all** : bool

.. _function-regex_regex_compile_Regex_string_bool_bool:

.. das:function:: regex_compile(re: Regex; expr: string; case_insensitive: bool = false; dot_all: bool = false) : bool

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


:Arguments: * **node** :  :ref:`ReNode <struct-regex-ReNode>`?

            * **blk** : block<(n: :ref:`ReNode <struct-regex-ReNode>`?):void>


++++++
Access
++++++

  *  :ref:`Regex[] (regex: Regex; index: int) : range <function-regex__lb__rb__Regex_int>`
  *  :ref:`Regex[] (regex: Regex; name: string) : range <function-regex__lb__rb__Regex_string>`
  *  :ref:`regex_foreach (var regex: Regex; str: string; blk: block\<(at:range):bool\>) <function-regex_regex_foreach_Regex_string_block_ls_at_c_range_c_bool_gr_>`
  *  :ref:`regex_group (regex: Regex; index: int; match: string) : string <function-regex_regex_group_Regex_int_string>`
  *  :ref:`regex_group_by_name (regex: Regex; name: string; str: string) : string <function-regex_regex_group_by_name_Regex_string_string>`


Regex[]
^^^^^^^

.. _function-regex__lb__rb__Regex_int:

.. das:function:: Regex[](regex: Regex; index: int) : range

Returns the match range for the given group index.


:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>`

            * **index** : int

.. _function-regex__lb__rb__Regex_string:

.. das:function:: Regex[](regex: Regex; name: string) : range

----

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

.. _function-regex_regex_group_by_name_Regex_string_string:

.. das:function:: regex_group_by_name(regex: Regex; name: string; str: string) : string

Returns the matched substring for the named capturing group ``(?P<name>...)``. Returns empty string if the group name is not found.


:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>`

            * **name** : string

            * **str** : string


+++++++++++++++
Match & replace
+++++++++++++++

  *  :ref:`regex_match (var regex: Regex; str: string; offset: int = 0) : int <function-regex_regex_match_Regex_string_int>`
  *  :ref:`regex_match_all (var regex: Regex; str: string) : array\<range\> <function-regex_regex_match_all_Regex_string>`
  *  :ref:`regex_replace (var regex: Regex; str: string; blk: block\<(at:string):string\>) : string <function-regex_regex_replace_Regex_string_block_ls_at_c_string_c_string_gr_>`
  *  :ref:`regex_replace (var regex: Regex; str: string; replacement: string) : string <function-regex_regex_replace_Regex_string_string>`
  *  :ref:`regex_search (var regex: Regex; str: string; offset: int = 0) : int2 <function-regex_regex_search_Regex_string_int>`
  *  :ref:`regex_split (var regex: Regex; str: string) : array\<string\> <function-regex_regex_split_Regex_string>`

.. _function-regex_regex_match_Regex_string_int:

.. das:function:: regex_match(regex: Regex; str: string; offset: int = 0) : int

Matches a compiled regex against a string and returns the end position of the match, or ``-1`` on failure.


:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>`

            * **str** : string

            * **offset** : int

.. _function-regex_regex_match_all_Regex_string:

.. das:function:: regex_match_all(regex: Regex; str: string) : array<range>

Returns an array of all non-overlapping match ranges for the regular expression in ``str``.


:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>`

            * **str** : string


regex_replace
^^^^^^^^^^^^^

.. _function-regex_regex_replace_Regex_string_block_ls_at_c_string_c_string_gr_:

.. das:function:: regex_replace(regex: Regex; str: string; blk: block<(at:string):string>) : string

Replaces each substring matched by the regex with the result returned by the provided block. An overload accepting a template string is also available, supporting ``$0``/``$&`` for the whole match, ``$1``–``$9`` for numbered groups, ``${name}`` for named groups, and ``$$`` for a literal ``$``.


:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>`

            * **str** : string

            * **blk** : block<(at:string):string>

.. _function-regex_regex_replace_Regex_string_string:

.. das:function:: regex_replace(regex: Regex; str: string; replacement: string) : string

----

.. _function-regex_regex_search_Regex_string_int:

.. das:function:: regex_search(regex: Regex; str: string; offset: int = 0) : int2

Searches for the first occurrence of the regular expression anywhere in ``str``, starting from ``offset``. Returns ``int2(start, end)`` on success, or ``int2(-1, -1)`` if not found. Unlike ``regex_match``, this function scans the entire string.


:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>`

            * **str** : string

            * **offset** : int

.. _function-regex_regex_split_Regex_string:

.. das:function:: regex_split(regex: Regex; str: string) : array<string>

Splits ``str`` by all non-overlapping matches of the regular expression. Returns an array of substrings between matches.


:Arguments: * **regex** :  :ref:`Regex <struct-regex-Regex>`

            * **str** : string


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



