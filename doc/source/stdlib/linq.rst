
.. _stdlib_linq:

====
LINQ
====

.. include:: detail/linq.rst

The LINQ module implements LINQ (Language Integrated Query) capabilities in Daslang.

All functions and symbols are in "linq" module, use require to get access to it. ::

    require daslib/linq


++++++++++++
Sorting data
++++++++++++

  *  :ref:`reverse_inplace (buffer:array\<auto(TT)\> -const) : auto <function-_at_linq_c__c_reverse_inplace_1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`reverse (a:array\<auto(TT)\> const) : array\<TT -const -&\> <function-_at_linq_c__c_reverse_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`reverse (a:iterator\<auto(TT)\> -const) : iterator\<TT -const -&\> <function-_at_linq_c__c_reverse_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`reverse_to_array (a:iterator\<auto(TT)\> -const) : array\<TT -const -&\> <function-_at_linq_c__c_reverse_to_array_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`order_inplace (buffer:array\<auto(TT)\> -const) : auto <function-_at_linq_c__c_order_inplace_1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`order (a:iterator\<auto(TT)\> -const) : iterator\<TT -const -&\> <function-_at_linq_c__c_order_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`order (arr:array\<auto(TT)\> const) : array\<TT -const -&\> <function-_at_linq_c__c_order_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`order_to_array (a:iterator\<auto(TT)\> -const) : array\<TT -const -&\> <function-_at_linq_c__c_order_to_array_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`order_inplace (buffer:array\<auto(TT)\> -const;fun:block\<(v1:TT const -&;v2:TT const -&):bool\> const) : auto <function-_at_linq_c__c_order_inplace_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`order (a:iterator\<auto(TT)\> -const;fun:block\<(v1:TT const -&;v2:TT const -&):bool\> const) : iterator\<TT -const -&\> <function-_at_linq_c__c_order_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`order (a:array\<auto(TT)\> const;fun:block\<(v1:TT const -&;v2:TT const -&):bool\> const) : array\<TT -const -&\> <function-_at_linq_c__c_order_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`order_to_array (a:iterator\<auto(TT)\> -const;fun:block\<(v1:TT const -&;v2:TT const -&):bool\> const) : array\<TT -const -&\> <function-_at_linq_c__c_order_to_array_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`order_descending_inplace (buffer:array\<auto(TT)\> -const;fun:block\<(v1:TT const -&;v2:TT const -&):bool\> const) : auto <function-_at_linq_c__c_order_descending_inplace_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`order_descending (a:iterator\<auto(TT)\> -const;fun:block\<(v1:TT const -&;v2:TT const -&):bool\> const) : iterator\<TT -const -&\> <function-_at_linq_c__c_order_descending_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`order_descending (a:array\<auto(TT)\> const;fun:block\<(v1:TT const -&;v2:TT const -&):bool\> const) : array\<TT -const -&\> <function-_at_linq_c__c_order_descending_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`order_descending_to_array (a:iterator\<auto(TT)\> -const;fun:block\<(v1:TT const -&;v2:TT const -&):bool\> const) : array\<TT -const -&\> <function-_at_linq_c__c_order_descending_to_array_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`order_by_inplace (buffer:array\<auto(TT)\> -const;key:auto const) : auto <function-_at_linq_c__c_order_by_inplace_1_ls_Y_ls_TT_gr_._gr_A_C.>` 
  *  :ref:`order_by (a:iterator\<auto(TT)\> -const;key:auto const) : iterator\<TT -const -&\> <function-_at_linq_c__c_order_by_1_ls_Y_ls_TT_gr_._gr_G_C.>` 
  *  :ref:`order_by (a:array\<auto(TT)\> const;key:auto const) : array\<TT -const -&\> <function-_at_linq_c__c_order_by_C1_ls_Y_ls_TT_gr_._gr_A_C.>` 
  *  :ref:`order_by_to_array (a:iterator\<auto(TT)\> -const;key:auto const) : array\<TT -const -&\> <function-_at_linq_c__c_order_by_to_array_1_ls_Y_ls_TT_gr_._gr_G_C.>` 
  *  :ref:`order_by_descending_inplace (buffer:array\<auto(TT)\> -const;key:auto const) : auto <function-_at_linq_c__c_order_by_descending_inplace_1_ls_Y_ls_TT_gr_._gr_A_C.>` 
  *  :ref:`order_by_descending (a:iterator\<auto(TT)\> -const;key:auto const) : iterator\<TT -const -&\> <function-_at_linq_c__c_order_by_descending_1_ls_Y_ls_TT_gr_._gr_G_C.>` 
  *  :ref:`order_by_descending (a:array\<auto(TT)\> const;key:auto const) : array\<TT -const -&\> <function-_at_linq_c__c_order_by_descending_C1_ls_Y_ls_TT_gr_._gr_A_C.>` 
  *  :ref:`order_by_descending_to_array (a:iterator\<auto(TT)\> -const;key:auto const) : array\<TT -const -&\> <function-_at_linq_c__c_order_by_descending_to_array_1_ls_Y_ls_TT_gr_._gr_G_C.>` 
  *  :ref:`order_unique_folded (a:array\<auto(TT)\> -const) : array\<TT -const -&\> <function-_at_linq_c__c_order_unique_folded_1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`order_unique_folded (a:iterator\<auto(TT)\> -const) : array\<TT -const -&\> <function-_at_linq_c__c_order_unique_folded_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`order_unique_folded_inplace (a:array\<auto(TT)\> -const) : auto <function-_at_linq_c__c_order_unique_folded_inplace_1_ls_Y_ls_TT_gr_._gr_A>` 

.. _function-_at_linq_c__c_reverse_inplace_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: reverse_inplace(buffer: array<auto(TT)>)

reverse_inplace returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+buffer  +array<auto(TT)>+
+--------+---------------+


Reverses an array in place

.. _function-_at_linq_c__c_reverse_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: reverse(a: array<auto(TT)> const)

reverse returns array<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+


Reverses an iterator

.. _function-_at_linq_c__c_reverse_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: reverse(a: iterator<auto(TT)>)

reverse returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+


Reverses an iterator

.. _function-_at_linq_c__c_reverse_to_array_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: reverse_to_array(a: iterator<auto(TT)>)

reverse_to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+


Reverses an iterator and returns an array

.. _function-_at_linq_c__c_order_inplace_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: order_inplace(buffer: array<auto(TT)>)

order_inplace returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+buffer  +array<auto(TT)>+
+--------+---------------+


Sorts an array in place

.. _function-_at_linq_c__c_order_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: order(a: iterator<auto(TT)>)

order returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+


Sorts an array

.. _function-_at_linq_c__c_order_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: order(arr: array<auto(TT)> const)

order returns array<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arr     +array<auto(TT)> const+
+--------+---------------------+


Sorts an array

.. _function-_at_linq_c__c_order_to_array_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: order_to_array(a: iterator<auto(TT)>)

order_to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+


Sorts an iterator and returns an array

.. _function-_at_linq_c__c_order_inplace_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: order_inplace(buffer: array<auto(TT)>; fun: block<(v1:TT const;v2:TT const):bool> const)

order_inplace returns auto

+--------+-------------------------------------------+
+argument+argument type                              +
+========+===========================================+
+buffer  +array<auto(TT)>                            +
+--------+-------------------------------------------+
+fun     +block<(v1:TT const;v2:TT const):bool> const+
+--------+-------------------------------------------+


Sorts an array in place

.. _function-_at_linq_c__c_order_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: order(a: iterator<auto(TT)>; fun: block<(v1:TT const;v2:TT const):bool> const)

order returns iterator<TT>

+--------+-------------------------------------------+
+argument+argument type                              +
+========+===========================================+
+a       +iterator<auto(TT)>                         +
+--------+-------------------------------------------+
+fun     +block<(v1:TT const;v2:TT const):bool> const+
+--------+-------------------------------------------+


Sorts an array

.. _function-_at_linq_c__c_order_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: order(a: array<auto(TT)> const; fun: block<(v1:TT const;v2:TT const):bool> const)

order returns array<TT>

+--------+-------------------------------------------+
+argument+argument type                              +
+========+===========================================+
+a       +array<auto(TT)> const                      +
+--------+-------------------------------------------+
+fun     +block<(v1:TT const;v2:TT const):bool> const+
+--------+-------------------------------------------+


Sorts an array

.. _function-_at_linq_c__c_order_to_array_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: order_to_array(a: iterator<auto(TT)>; fun: block<(v1:TT const;v2:TT const):bool> const)

order_to_array returns array<TT>

+--------+-------------------------------------------+
+argument+argument type                              +
+========+===========================================+
+a       +iterator<auto(TT)>                         +
+--------+-------------------------------------------+
+fun     +block<(v1:TT const;v2:TT const):bool> const+
+--------+-------------------------------------------+


Sorts an iterator and returns an array

.. _function-_at_linq_c__c_order_descending_inplace_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: order_descending_inplace(buffer: array<auto(TT)>; fun: block<(v1:TT const;v2:TT const):bool> const)

order_descending_inplace returns auto

+--------+-------------------------------------------+
+argument+argument type                              +
+========+===========================================+
+buffer  +array<auto(TT)>                            +
+--------+-------------------------------------------+
+fun     +block<(v1:TT const;v2:TT const):bool> const+
+--------+-------------------------------------------+


Sorts an array in descending order in place

.. _function-_at_linq_c__c_order_descending_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: order_descending(a: iterator<auto(TT)>; fun: block<(v1:TT const;v2:TT const):bool> const)

order_descending returns iterator<TT>

+--------+-------------------------------------------+
+argument+argument type                              +
+========+===========================================+
+a       +iterator<auto(TT)>                         +
+--------+-------------------------------------------+
+fun     +block<(v1:TT const;v2:TT const):bool> const+
+--------+-------------------------------------------+


Sorts an array in descending order

.. _function-_at_linq_c__c_order_descending_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: order_descending(a: array<auto(TT)> const; fun: block<(v1:TT const;v2:TT const):bool> const)

order_descending returns array<TT>

+--------+-------------------------------------------+
+argument+argument type                              +
+========+===========================================+
+a       +array<auto(TT)> const                      +
+--------+-------------------------------------------+
+fun     +block<(v1:TT const;v2:TT const):bool> const+
+--------+-------------------------------------------+


Sorts an array in descending order

.. _function-_at_linq_c__c_order_descending_to_array_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_v1;v2_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: order_descending_to_array(a: iterator<auto(TT)>; fun: block<(v1:TT const;v2:TT const):bool> const)

order_descending_to_array returns array<TT>

+--------+-------------------------------------------+
+argument+argument type                              +
+========+===========================================+
+a       +iterator<auto(TT)>                         +
+--------+-------------------------------------------+
+fun     +block<(v1:TT const;v2:TT const):bool> const+
+--------+-------------------------------------------+


Sorts an iterator in descending order and returns an array

.. _function-_at_linq_c__c_order_by_inplace_1_ls_Y_ls_TT_gr_._gr_A_C.:

.. das:function:: order_by_inplace(buffer: array<auto(TT)>; key: auto const)

order_by_inplace returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+buffer  +array<auto(TT)>+
+--------+---------------+
+key     +auto const     +
+--------+---------------+


Sorts an array in place

.. _function-_at_linq_c__c_order_by_1_ls_Y_ls_TT_gr_._gr_G_C.:

.. das:function:: order_by(a: iterator<auto(TT)>; key: auto const)

order_by returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+
+key     +auto const        +
+--------+------------------+


Sorts an array

.. _function-_at_linq_c__c_order_by_C1_ls_Y_ls_TT_gr_._gr_A_C.:

.. das:function:: order_by(a: array<auto(TT)> const; key: auto const)

order_by returns array<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+
+key     +auto const           +
+--------+---------------------+


Sorts an array

.. _function-_at_linq_c__c_order_by_to_array_1_ls_Y_ls_TT_gr_._gr_G_C.:

.. das:function:: order_by_to_array(a: iterator<auto(TT)>; key: auto const)

order_by_to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+
+key     +auto const        +
+--------+------------------+


Sorts an iterator and returns an array

.. _function-_at_linq_c__c_order_by_descending_inplace_1_ls_Y_ls_TT_gr_._gr_A_C.:

.. das:function:: order_by_descending_inplace(buffer: array<auto(TT)>; key: auto const)

order_by_descending_inplace returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+buffer  +array<auto(TT)>+
+--------+---------------+
+key     +auto const     +
+--------+---------------+


Sorts an array in descending order in place

.. _function-_at_linq_c__c_order_by_descending_1_ls_Y_ls_TT_gr_._gr_G_C.:

.. das:function:: order_by_descending(a: iterator<auto(TT)>; key: auto const)

order_by_descending returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+
+key     +auto const        +
+--------+------------------+


Sorts an array in descending order

.. _function-_at_linq_c__c_order_by_descending_C1_ls_Y_ls_TT_gr_._gr_A_C.:

.. das:function:: order_by_descending(a: array<auto(TT)> const; key: auto const)

order_by_descending returns array<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+
+key     +auto const           +
+--------+---------------------+


Sorts an array in descending order

.. _function-_at_linq_c__c_order_by_descending_to_array_1_ls_Y_ls_TT_gr_._gr_G_C.:

.. das:function:: order_by_descending_to_array(a: iterator<auto(TT)>; key: auto const)

order_by_descending_to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+
+key     +auto const        +
+--------+------------------+


Sorts an iterator in descending order and returns an array

.. _function-_at_linq_c__c_order_unique_folded_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: order_unique_folded(a: array<auto(TT)>)

order_unique_folded returns array<TT>

+--------+---------------+
+argument+argument type  +
+========+===============+
+a       +array<auto(TT)>+
+--------+---------------+


sort and remove duplicate elements from an iterator

.. _function-_at_linq_c__c_order_unique_folded_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: order_unique_folded(a: iterator<auto(TT)>)

order_unique_folded returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+


sort and remove duplicate elements from an iterator

.. _function-_at_linq_c__c_order_unique_folded_inplace_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: order_unique_folded_inplace(a: array<auto(TT)>)

order_unique_folded_inplace returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+a       +array<auto(TT)>+
+--------+---------------+


sort and remove duplicate elements from an array

++++++++++++++
Set operations
++++++++++++++

  *  :ref:`unique_key (a:auto const) : auto <function-_at_linq_c__c_unique_key_C.>` 
  *  :ref:`distinct (a:array\<auto(TT)\> const) : array\<TT -const -&\> <function-_at_linq_c__c_distinct_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`distinct_to_array (a:iterator\<auto(TT)\> -const) : array\<TT -const -&\> <function-_at_linq_c__c_distinct_to_array_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`distinct_inplace (a:array\<auto(TT)\> -const) : auto <function-_at_linq_c__c_distinct_inplace_1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`distinct (a:iterator\<auto(TT)\> -const) : iterator\<TT -const -&\> <function-_at_linq_c__c_distinct_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`distinct_by (a:array\<auto(TT)\> const;key:block\<(arg:TT const -&):auto\> const) : array\<TT -const -&\> <function-_at_linq_c__c_distinct_by_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`distinct_by_to_array (a:iterator\<auto(TT)\> -const;key:block\<(arg:TT const -&):auto\> const) : array\<TT -const -&\> <function-_at_linq_c__c_distinct_by_to_array_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`distinct_by (a:iterator\<auto(TT)\> -const;key:block\<(arg:TT const -&):auto\> const) : iterator\<TT -const -&\> <function-_at_linq_c__c_distinct_by_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`distinct_by_inplace (a:array\<auto(TT)\> -const;key:block\<(arg:TT const -&):auto\> const) : auto <function-_at_linq_c__c_distinct_by_inplace_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`unique (a:iterator\<auto(TT)\> const) : iterator\<TT -const -&\> <function-_at_linq_c__c_unique_C1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`unique (a:array\<auto(TT)\> const) : array\<TT -const -&\> <function-_at_linq_c__c_unique_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`unique_to_array (a:iterator\<auto(TT)\> const) : array\<TT -const -&\> <function-_at_linq_c__c_unique_to_array_C1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`unique_inplace (a:array\<auto(TT)\> -const) : auto <function-_at_linq_c__c_unique_inplace_1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`unique_by (a:iterator\<auto(TT)\> const;key:block\<(arg:TT const -&):auto\> const) : iterator\<TT -const -&\> <function-_at_linq_c__c_unique_by_C1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`unique_by (a:array\<auto(TT)\> const;key:block\<(arg:TT const -&):auto\> const) : array\<TT -const -&\> <function-_at_linq_c__c_unique_by_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`unique_by_to_array (a:iterator\<auto(TT)\> const;key:block\<(arg:TT const -&):auto\> const) : array\<TT -const -&\> <function-_at_linq_c__c_unique_by_to_array_C1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`unique_by_inplace (a:array\<auto(TT)\> -const;key:block\<(arg:TT const -&):auto\> const) : auto <function-_at_linq_c__c_unique_by_inplace_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`union (srca:array\<auto(TT)\> -const;srcb:array\<auto(TT)\> -const) : array\<TT -const -&\> <function-_at_linq_c__c_union_1_ls_Y_ls_TT_gr_._gr_A_1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`union (srca:iterator\<auto(TT)\> -const;srcb:iterator\<auto(TT)\> -const) : iterator\<TT -const -&\> <function-_at_linq_c__c_union_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`union_to_array (srca:iterator\<auto(TT)\> -const;srcb:iterator\<auto(TT)\> -const) : array\<TT -const -&\> <function-_at_linq_c__c_union_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`union_by (srca:iterator\<auto(TT)\> -const;srcb:iterator\<auto(TT)\> -const;key:block\<(arg:TT const -&):auto\> const) : iterator\<TT -const -&\> <function-_at_linq_c__c_union_by_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`union_by (srca:array\<auto(TT)\> const;srcb:array\<auto(TT)\> const;key:block\<(arg:TT const -&):auto\> const) : array\<TT -const -&\> <function-_at_linq_c__c_union_by_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`union_by_to_array (srca:iterator\<auto(TT)\> -const;srcb:iterator\<auto(TT)\> -const;key:block\<(arg:TT const -&):auto\> const) : array\<TT -const -&\> <function-_at_linq_c__c_union_by_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`except (src:iterator\<auto(TT)\> -const;exclude:iterator\<auto(TT)\> -const) : iterator\<TT -const -&\> <function-_at_linq_c__c_except_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`except (src:array\<auto(TT)\> const;exclude:array\<auto(TT)\> const) : array\<TT -const -&\> <function-_at_linq_c__c_except_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`except_to_array (src:iterator\<auto(TT)\> -const;exclude:iterator\<auto(TT)\> -const) : array\<TT -const -&\> <function-_at_linq_c__c_except_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`except_by (src:iterator\<auto(TT)\> -const;exclude:iterator\<auto(TT)\> -const;key:block\<(arg:TT const -&):auto\> const) : iterator\<TT -const -&\> <function-_at_linq_c__c_except_by_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`except_by (src:array\<auto(TT)\> const;exclude:array\<auto(TT)\> const;key:block\<(arg:TT const -&):auto\> const) : array\<TT -const -&\> <function-_at_linq_c__c_except_by_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`except_by_to_array (src:iterator\<auto(TT)\> -const;exclude:iterator\<auto(TT)\> -const;key:block\<(arg:TT const -&):auto\> const) : array\<TT -const -&\> <function-_at_linq_c__c_except_by_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`intersect (srca:iterator\<auto(TT)\> -const;srcb:iterator\<auto(TT)\> -const) : iterator\<TT -const -&\> <function-_at_linq_c__c_intersect_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`intersect (srca:array\<auto(TT)\> const;srcb:array\<auto(TT)\> const) : array\<TT -const -&\> <function-_at_linq_c__c_intersect_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`intersect_to_array (srca:iterator\<auto(TT)\> -const;srcb:iterator\<auto(TT)\> -const) : array\<TT -const -&\> <function-_at_linq_c__c_intersect_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`intersect_by (srca:iterator\<auto(TT)\> -const;srcb:iterator\<auto(TT)\> -const;key:block\<(arg:TT const -&):auto\> const) : iterator\<TT -const -&\> <function-_at_linq_c__c_intersect_by_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`intersect_by (srca:array\<auto(TT)\> const;srcb:array\<auto(TT)\> const;key:block\<(arg:TT const -&):auto\> const) : array\<TT -const -&\> <function-_at_linq_c__c_intersect_by_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`intersect_by_to_array (srca:iterator\<auto(TT)\> -const;srcb:iterator\<auto(TT)\> -const;key:block\<(arg:TT const -&):auto\> const) : array\<TT -const -&\> <function-_at_linq_c__c_intersect_by_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 

.. _function-_at_linq_c__c_unique_key_C.:

.. das:function:: unique_key(a: auto const)

unique_key returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto const   +
+--------+-------------+


! generates unique key of workhorse type for the value

.. _function-_at_linq_c__c_distinct_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: distinct(a: array<auto(TT)> const)

distinct returns array<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+


Returns distinct elements from an iterator

.. _function-_at_linq_c__c_distinct_to_array_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: distinct_to_array(a: iterator<auto(TT)>)

distinct_to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+


Returns distinct elements from an iterator and returns an array

.. _function-_at_linq_c__c_distinct_inplace_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: distinct_inplace(a: array<auto(TT)>)

distinct_inplace returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+a       +array<auto(TT)>+
+--------+---------------+


Returns distinct elements from an array in place

.. _function-_at_linq_c__c_distinct_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: distinct(a: iterator<auto(TT)>)

distinct returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+


Returns distinct elements from an iterator

.. _function-_at_linq_c__c_distinct_by_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: distinct_by(a: array<auto(TT)> const; key: block<(arg:TT const):auto> const)

distinct_by returns array<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+a       +array<auto(TT)> const           +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Returns distinct elements from an iterator based on a key
TODO: specialize with const value vs var value of key, which can be captured.

.. _function-_at_linq_c__c_distinct_by_to_array_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: distinct_by_to_array(a: iterator<auto(TT)>; key: block<(arg:TT const):auto> const)

distinct_by_to_array returns array<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+a       +iterator<auto(TT)>              +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Returns distinct elements from an iterator based on a key and returns an array

.. _function-_at_linq_c__c_distinct_by_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: distinct_by(a: iterator<auto(TT)>; key: block<(arg:TT const):auto> const)

distinct_by returns iterator<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+a       +iterator<auto(TT)>              +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Returns distinct elements from an iterator based on a key
TODO: specialize with const value vs var value of key, which can be captured.

.. _function-_at_linq_c__c_distinct_by_inplace_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: distinct_by_inplace(a: array<auto(TT)>; key: block<(arg:TT const):auto> const)

distinct_by_inplace returns auto

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+a       +array<auto(TT)>                 +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Returns distinct elements from an array based on a key in place

.. _function-_at_linq_c__c_unique_C1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: unique(a: iterator<auto(TT)> const)

unique returns iterator<TT>

+--------+------------------------+
+argument+argument type           +
+========+========================+
+a       +iterator<auto(TT)> const+
+--------+------------------------+


sort and remove duplicate elements from an array

.. _function-_at_linq_c__c_unique_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: unique(a: array<auto(TT)> const)

unique returns array<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+


sort and remove duplicate elements from an array

.. _function-_at_linq_c__c_unique_to_array_C1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: unique_to_array(a: iterator<auto(TT)> const)

unique_to_array returns array<TT>

+--------+------------------------+
+argument+argument type           +
+========+========================+
+a       +iterator<auto(TT)> const+
+--------+------------------------+


sort and remove duplicate elements from an iterator and returns an array

.. _function-_at_linq_c__c_unique_inplace_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: unique_inplace(a: array<auto(TT)>)

unique_inplace returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+a       +array<auto(TT)>+
+--------+---------------+


remove duplicate elements from sorted array in place

.. _function-_at_linq_c__c_unique_by_C1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: unique_by(a: iterator<auto(TT)> const; key: block<(arg:TT const):auto> const)

unique_by returns iterator<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+a       +iterator<auto(TT)> const        +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


sort and remove duplicate elements from an array based on a key

.. _function-_at_linq_c__c_unique_by_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: unique_by(a: array<auto(TT)> const; key: block<(arg:TT const):auto> const)

unique_by returns array<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+a       +array<auto(TT)> const           +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


sort and remove duplicate elements from an array based on a key

.. _function-_at_linq_c__c_unique_by_to_array_C1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: unique_by_to_array(a: iterator<auto(TT)> const; key: block<(arg:TT const):auto> const)

unique_by_to_array returns array<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+a       +iterator<auto(TT)> const        +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


sort and remove duplicate elements from an iterator based on a key and returns an array

.. _function-_at_linq_c__c_unique_by_inplace_1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: unique_by_inplace(a: array<auto(TT)>; key: block<(arg:TT const):auto> const)

unique_by_inplace returns auto

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+a       +array<auto(TT)>                 +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


remove duplicate elements from an array based on a key in place

.. _function-_at_linq_c__c_union_1_ls_Y_ls_TT_gr_._gr_A_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: union(srca: array<auto(TT)>; srcb: array<auto(TT)>)

union returns array<TT>

+--------+---------------+
+argument+argument type  +
+========+===============+
+srca    +array<auto(TT)>+
+--------+---------------+
+srcb    +array<auto(TT)>+
+--------+---------------+


Returns distinct elements from the concatenation of two iterators

.. _function-_at_linq_c__c_union_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: union(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>)

union returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+srca    +iterator<auto(TT)>+
+--------+------------------+
+srcb    +iterator<auto(TT)>+
+--------+------------------+


Returns distinct elements from the concatenation of two iterators

.. _function-_at_linq_c__c_union_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: union_to_array(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>)

union_to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+srca    +iterator<auto(TT)>+
+--------+------------------+
+srcb    +iterator<auto(TT)>+
+--------+------------------+


Returns distinct elements from the concatenation of two iterators and returns an array

.. _function-_at_linq_c__c_union_by_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: union_by(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>; key: block<(arg:TT const):auto> const)

union_by returns iterator<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+srca    +iterator<auto(TT)>              +
+--------+--------------------------------+
+srcb    +iterator<auto(TT)>              +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Returns distinct elements from the concatenation of two arrays by key

.. _function-_at_linq_c__c_union_by_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: union_by(srca: array<auto(TT)> const; srcb: array<auto(TT)> const; key: block<(arg:TT const):auto> const)

union_by returns array<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+srca    +array<auto(TT)> const           +
+--------+--------------------------------+
+srcb    +array<auto(TT)> const           +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Returns distinct elements from the concatenation of two arrays by key

.. _function-_at_linq_c__c_union_by_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: union_by_to_array(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>; key: block<(arg:TT const):auto> const)

union_by_to_array returns array<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+srca    +iterator<auto(TT)>              +
+--------+--------------------------------+
+srcb    +iterator<auto(TT)>              +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Returns distinct elements from the concatenation of two iterators by key and returns an array

.. _function-_at_linq_c__c_except_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: except(src: iterator<auto(TT)>; exclude: iterator<auto(TT)>)

except returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+exclude +iterator<auto(TT)>+
+--------+------------------+


Returns elements from the first array that are not in the second array

.. _function-_at_linq_c__c_except_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: except(src: array<auto(TT)> const; exclude: array<auto(TT)> const)

except returns array<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+
+exclude +array<auto(TT)> const+
+--------+---------------------+


Returns elements from the first array that are not in the second array

.. _function-_at_linq_c__c_except_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: except_to_array(src: iterator<auto(TT)>; exclude: iterator<auto(TT)>)

except_to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+exclude +iterator<auto(TT)>+
+--------+------------------+


Returns elements from the first iterator that are not in the second iterator and returns an array

.. _function-_at_linq_c__c_except_by_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: except_by(src: iterator<auto(TT)>; exclude: iterator<auto(TT)>; key: block<(arg:TT const):auto> const)

except_by returns iterator<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+src     +iterator<auto(TT)>              +
+--------+--------------------------------+
+exclude +iterator<auto(TT)>              +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Returns elements from the first array that are not in the second array by key

.. _function-_at_linq_c__c_except_by_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: except_by(src: array<auto(TT)> const; exclude: array<auto(TT)> const; key: block<(arg:TT const):auto> const)

except_by returns array<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+src     +array<auto(TT)> const           +
+--------+--------------------------------+
+exclude +array<auto(TT)> const           +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Returns elements from the first array that are not in the second array by key

.. _function-_at_linq_c__c_except_by_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: except_by_to_array(src: iterator<auto(TT)>; exclude: iterator<auto(TT)>; key: block<(arg:TT const):auto> const)

except_by_to_array returns array<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+src     +iterator<auto(TT)>              +
+--------+--------------------------------+
+exclude +iterator<auto(TT)>              +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Returns elements from the first iterator that are not in the second iterator by key and returns an array

.. _function-_at_linq_c__c_intersect_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: intersect(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>)

intersect returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+srca    +iterator<auto(TT)>+
+--------+------------------+
+srcb    +iterator<auto(TT)>+
+--------+------------------+


Returns elements that are present in both arrays

.. _function-_at_linq_c__c_intersect_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: intersect(srca: array<auto(TT)> const; srcb: array<auto(TT)> const)

intersect returns array<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+srca    +array<auto(TT)> const+
+--------+---------------------+
+srcb    +array<auto(TT)> const+
+--------+---------------------+


Returns elements that are present in both arrays

.. _function-_at_linq_c__c_intersect_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: intersect_to_array(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>)

intersect_to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+srca    +iterator<auto(TT)>+
+--------+------------------+
+srcb    +iterator<auto(TT)>+
+--------+------------------+


Returns elements that are present in both iterators and returns an array

.. _function-_at_linq_c__c_intersect_by_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: intersect_by(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>; key: block<(arg:TT const):auto> const)

intersect_by returns iterator<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+srca    +iterator<auto(TT)>              +
+--------+--------------------------------+
+srcb    +iterator<auto(TT)>              +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Returns elements that are present in both arrays by key

.. _function-_at_linq_c__c_intersect_by_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: intersect_by(srca: array<auto(TT)> const; srcb: array<auto(TT)> const; key: block<(arg:TT const):auto> const)

intersect_by returns array<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+srca    +array<auto(TT)> const           +
+--------+--------------------------------+
+srcb    +array<auto(TT)> const           +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Returns elements that are present in both arrays by key

.. _function-_at_linq_c__c_intersect_by_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: intersect_by_to_array(srca: iterator<auto(TT)>; srcb: iterator<auto(TT)>; key: block<(arg:TT const):auto> const)

intersect_by_to_array returns array<TT>

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+srca    +iterator<auto(TT)>              +
+--------+--------------------------------+
+srcb    +iterator<auto(TT)>              +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Returns elements that are present in both iterators by key and returns an array

++++++++++++++++++++++++
Concatenation operations
++++++++++++++++++++++++

  *  :ref:`concat (a:array\<auto(TT)\> const;b:array\<auto(TT)\> const) : array\<TT -const -&\> <function-_at_linq_c__c_concat_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`concat_to_array (a:iterator\<auto(TT)\> -const;b:iterator\<auto(TT)\> -const) : array\<TT -const -&\> <function-_at_linq_c__c_concat_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`concat (a:iterator\<auto(TT)\> -const;b:iterator\<auto(TT)\> -const) : iterator\<TT -const -&\> <function-_at_linq_c__c_concat_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`concat_inplace (a:array\<auto(TT)\> -const;b:array\<auto(TT)\> const) : auto <function-_at_linq_c__c_concat_inplace_1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`prepend (arr:array\<auto(TT)\> const;value:TT const -&) : array\<TT -const -&\> <function-_at_linq_c__c_prepend_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L>` 
  *  :ref:`prepend_to_array (it:iterator\<auto(TT)\> -const;value:TT const -&) : array\<TT -const -&\> <function-_at_linq_c__c_prepend_to_array_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L>` 
  *  :ref:`prepend (it:iterator\<auto(TT)\> -const;value:TT const -&) : iterator\<TT -const -&\> <function-_at_linq_c__c_prepend_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L>` 
  *  :ref:`prepend_inplace (arr:array\<auto(TT)\> -const;value:TT const -&) : auto <function-_at_linq_c__c_prepend_inplace_1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L>` 
  *  :ref:`append (arr:array\<auto(TT)\> const;value:TT const -&) : array\<TT -const -&\> <function-_at_linq_c__c_append_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L>` 
  *  :ref:`append_to_array (it:iterator\<auto(TT)\> -const;value:TT const -&) : array\<TT -const -&\> <function-_at_linq_c__c_append_to_array_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L>` 
  *  :ref:`append (it:iterator\<auto(TT)\> -const;value:TT const -&) : iterator\<TT -const -&\> <function-_at_linq_c__c_append_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L>` 
  *  :ref:`append_inplace (arr:array\<auto(TT)\> -const;value:TT const -&) : auto <function-_at_linq_c__c_append_inplace_1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L>` 

.. _function-_at_linq_c__c_concat_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: concat(a: array<auto(TT)> const; b: array<auto(TT)> const)

concat returns array<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+
+b       +array<auto(TT)> const+
+--------+---------------------+


Concatenates two iterators

.. _function-_at_linq_c__c_concat_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: concat_to_array(a: iterator<auto(TT)>; b: iterator<auto(TT)>)

concat_to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+
+b       +iterator<auto(TT)>+
+--------+------------------+


Concatenates two iterators and returns an array

.. _function-_at_linq_c__c_concat_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: concat(a: iterator<auto(TT)>; b: iterator<auto(TT)>)

concat returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+
+b       +iterator<auto(TT)>+
+--------+------------------+


Concatenates two iterators

.. _function-_at_linq_c__c_concat_inplace_1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: concat_inplace(a: array<auto(TT)>; b: array<auto(TT)> const)

concat_inplace returns auto

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)>      +
+--------+---------------------+
+b       +array<auto(TT)> const+
+--------+---------------------+


Concatenates two arrays in place

.. _function-_at_linq_c__c_prepend_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L:

.. das:function:: prepend(arr: array<auto(TT)> const; value: TT const)

prepend returns array<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arr     +array<auto(TT)> const+
+--------+---------------------+
+value   +TT const             +
+--------+---------------------+


Prepends a value to the beginning of an iterator

.. _function-_at_linq_c__c_prepend_to_array_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L:

.. das:function:: prepend_to_array(it: iterator<auto(TT)>; value: TT const)

prepend_to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+it      +iterator<auto(TT)>+
+--------+------------------+
+value   +TT const          +
+--------+------------------+


Prepends a value to the beginning of an iterator and returns an array

.. _function-_at_linq_c__c_prepend_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L:

.. das:function:: prepend(it: iterator<auto(TT)>; value: TT const)

prepend returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+it      +iterator<auto(TT)>+
+--------+------------------+
+value   +TT const          +
+--------+------------------+


Prepends a value to the beginning of an iterator

.. _function-_at_linq_c__c_prepend_inplace_1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L:

.. das:function:: prepend_inplace(arr: array<auto(TT)>; value: TT const)

prepend_inplace returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+arr     +array<auto(TT)>+
+--------+---------------+
+value   +TT const       +
+--------+---------------+


Prepends a value to the beginning of an array in place

.. _function-_at_linq_c__c_append_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L:

.. das:function:: append(arr: array<auto(TT)> const; value: TT const)

append returns array<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arr     +array<auto(TT)> const+
+--------+---------------------+
+value   +TT const             +
+--------+---------------------+


Appends a value to the end of an iterator

.. _function-_at_linq_c__c_append_to_array_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L:

.. das:function:: append_to_array(it: iterator<auto(TT)>; value: TT const)

append_to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+it      +iterator<auto(TT)>+
+--------+------------------+
+value   +TT const          +
+--------+------------------+


Appends a value to the end of an iterator and returns an array

.. _function-_at_linq_c__c_append_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L:

.. das:function:: append(it: iterator<auto(TT)>; value: TT const)

append returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+it      +iterator<auto(TT)>+
+--------+------------------+
+value   +TT const          +
+--------+------------------+


Appends a value to the end of an iterator

.. _function-_at_linq_c__c_append_inplace_1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L:

.. das:function:: append_inplace(arr: array<auto(TT)>; value: TT const)

append_inplace returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+arr     +array<auto(TT)>+
+--------+---------------+
+value   +TT const       +
+--------+---------------+


Appends a value to the end of an array in place

+++++++++++++++++++++
Generation operations
+++++++++++++++++++++

  *  :ref:`range_sequence (start:int const;count:int const) : iterator\<int\> <function-_at_linq_c__c_range_sequence_Ci_Ci>` 
  *  :ref:`empty (typ:auto(TT) -const) : iterator\<TT -const -&\> <function-_at_linq_c__c_empty_Y_ls_TT_gr_.>` 
  *  :ref:`default_empty (src:iterator\<auto(TT)\> -const) : iterator\<TT -const -&\> <function-_at_linq_c__c_default_empty_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`repeat (element:auto(TT) const;count:int const) : iterator\<TT -const -&\> <function-_at_linq_c__c_repeat_CY_ls_TT_gr_._Ci>` 

.. _function-_at_linq_c__c_range_sequence_Ci_Ci:

.. das:function:: range_sequence(start: int const; count: int const)

range_sequence returns iterator<int>

+--------+-------------+
+argument+argument type+
+========+=============+
+start   +int const    +
+--------+-------------+
+count   +int const    +
+--------+-------------+


Generates a sequence of integers within a specified range

.. _function-_at_linq_c__c_empty_Y_ls_TT_gr_.:

.. das:function:: empty(typ: auto(TT))

empty returns iterator<TT>

+--------+-------------+
+argument+argument type+
+========+=============+
+typ     +auto(TT)     +
+--------+-------------+


Returns an empty iterator of the specified type

.. _function-_at_linq_c__c_default_empty_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: default_empty(src: iterator<auto(TT)>)

default_empty returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+


Returns the elements of the iterator, or a default value if the iterator is empty

.. _function-_at_linq_c__c_repeat_CY_ls_TT_gr_._Ci:

.. das:function:: repeat(element: auto(TT) const; count: int const)

repeat returns iterator<TT>

+--------+--------------+
+argument+argument type +
+========+==============+
+element +auto(TT) const+
+--------+--------------+
+count   +int const     +
+--------+--------------+


Generates a sequence that contains one repeated value

++++++++++++++++++++++
Aggregation operations
++++++++++++++++++++++

  *  :ref:`count (a:iterator\<auto(TT)\> -const) : int <function-_at_linq_c__c_count_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`count (a:array\<auto(TT)\> const) : int <function-_at_linq_c__c_count_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`long_count (a:iterator\<auto(TT)\> -const) : int64 <function-_at_linq_c__c_long_count_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`long_count (a:array\<auto(TT)\> const) : int64 <function-_at_linq_c__c_long_count_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`min (src:iterator\<auto(TT)\> -const) : TT -const -& <function-_at_linq_c__c_min_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`min (src:array\<auto(TT)\> const) : TT -const -& <function-_at_linq_c__c_min_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`min_by (src:iterator\<auto(TT)\> -const;key:auto const) : TT -const -& <function-_at_linq_c__c_min_by_1_ls_Y_ls_TT_gr_._gr_G_C.>` 
  *  :ref:`min_by (src:array\<auto(TT)\> const;key:auto const) : TT -const -& <function-_at_linq_c__c_min_by_C1_ls_Y_ls_TT_gr_._gr_A_C.>` 
  *  :ref:`max (src:iterator\<auto(TT)\> -const) : TT -const -& <function-_at_linq_c__c_max_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`max (src:array\<auto(TT)\> const) : TT -const -& <function-_at_linq_c__c_max_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`max_by (src:iterator\<auto(TT)\> -const;key:auto const) : TT -const -& <function-_at_linq_c__c_max_by_1_ls_Y_ls_TT_gr_._gr_G_C.>` 
  *  :ref:`max_by (src:array\<auto(TT)\> const;key:auto const) : TT -const -& <function-_at_linq_c__c_max_by_C1_ls_Y_ls_TT_gr_._gr_A_C.>` 
  *  :ref:`min_max (src:iterator\<auto(TT)\> -const) : tuple\<TT -const -&;TT -const -&\> <function-_at_linq_c__c_min_max_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`min_max (src:array\<auto(TT)\> const) : tuple\<TT -const -&;TT -const -&\> <function-_at_linq_c__c_min_max_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`min_max_by (src:iterator\<auto(TT)\> -const;key:auto const) : tuple\<TT -const -&;TT -const -&\> <function-_at_linq_c__c_min_max_by_1_ls_Y_ls_TT_gr_._gr_G_C.>` 
  *  :ref:`min_max_by (src:array\<auto(TT)\> const;key:auto const) : tuple\<TT -const -&;TT -const -&\> <function-_at_linq_c__c_min_max_by_C1_ls_Y_ls_TT_gr_._gr_A_C.>` 
  *  :ref:`aggregate (src:iterator\<auto(TT)\> -const;seed:auto(AGG) const;func:block\<(acc:AGG const -&;x:TT const -&):AGG -&\> const) : AGG -const -& <function-_at_linq_c__c_aggregate_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_AGG_gr_._CN_ls_acc;x_gr_0_ls_CY_ls_AGG_gr_L;CY_ls_TT_gr_L_gr_1_ls_Y_ls_AGG_gr_L_gr__builtin_>` 
  *  :ref:`aggregate (src:array\<auto(TT)\> const;seed:auto(AGG) const;func:block\<(acc:AGG const -&;x:TT const -&):AGG -&\> const) : AGG -const -& <function-_at_linq_c__c_aggregate_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_AGG_gr_._CN_ls_acc;x_gr_0_ls_CY_ls_AGG_gr_L;CY_ls_TT_gr_L_gr_1_ls_Y_ls_AGG_gr_L_gr__builtin_>` 
  *  :ref:`sum (src:iterator\<auto(TT)\> -const) : TT -const -& <function-_at_linq_c__c_sum_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`sum (src:array\<auto(TT)\> const) : TT -const -& <function-_at_linq_c__c_sum_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`average (src:iterator\<auto(TT)\> -const) : TT -const -& <function-_at_linq_c__c_average_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`average (src:array\<auto(TT)\> const) : TT -const -& <function-_at_linq_c__c_average_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`min_max_average (src:iterator\<auto(TT)\> -const) : tuple\<TT -const -&;TT -const -&;TT -const -&\> <function-_at_linq_c__c_min_max_average_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`min_max_average (src:array\<auto(TT)\> const) : tuple\<TT -const -&;TT -const -&;TT -const -&\> <function-_at_linq_c__c_min_max_average_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`min_max_average_by (src:iterator\<auto(TT)\> -const;key:auto const) : tuple\<TT -const -&;TT -const -&;TT -const -&\> <function-_at_linq_c__c_min_max_average_by_1_ls_Y_ls_TT_gr_._gr_G_C.>` 
  *  :ref:`min_max_average_by (src:array\<auto(TT)\> const;key:auto const) : tuple\<TT -const -&;TT -const -&;TT -const -&\> <function-_at_linq_c__c_min_max_average_by_C1_ls_Y_ls_TT_gr_._gr_A_C.>` 

.. _function-_at_linq_c__c_count_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: count(a: iterator<auto(TT)>)

count returns int

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+


Counts elements in an array

.. _function-_at_linq_c__c_count_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: count(a: array<auto(TT)> const)

count returns int

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+


Counts elements in an array

.. _function-_at_linq_c__c_long_count_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: long_count(a: iterator<auto(TT)>)

long_count returns int64

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+


Counts elements in an array, using a long integer

.. _function-_at_linq_c__c_long_count_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: long_count(a: array<auto(TT)> const)

long_count returns int64

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+


Counts elements in an array, using a long integer

.. _function-_at_linq_c__c_min_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: min(src: iterator<auto(TT)>)

min returns TT

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+


Finds the minimum element in an array

.. _function-_at_linq_c__c_min_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: min(src: array<auto(TT)> const)

min returns TT

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+


Finds the minimum element in an array

.. _function-_at_linq_c__c_min_by_1_ls_Y_ls_TT_gr_._gr_G_C.:

.. das:function:: min_by(src: iterator<auto(TT)>; key: auto const)

min_by returns TT

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+key     +auto const        +
+--------+------------------+


Finds the minimum element in an array by key

.. _function-_at_linq_c__c_min_by_C1_ls_Y_ls_TT_gr_._gr_A_C.:

.. das:function:: min_by(src: array<auto(TT)> const; key: auto const)

min_by returns TT

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+
+key     +auto const           +
+--------+---------------------+


Finds the minimum element in an array by key

.. _function-_at_linq_c__c_max_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: max(src: iterator<auto(TT)>)

max returns TT

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+


Finds the maximum element in an array

.. _function-_at_linq_c__c_max_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: max(src: array<auto(TT)> const)

max returns TT

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+


Finds the maximum element in an array

.. _function-_at_linq_c__c_max_by_1_ls_Y_ls_TT_gr_._gr_G_C.:

.. das:function:: max_by(src: iterator<auto(TT)>; key: auto const)

max_by returns TT

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+key     +auto const        +
+--------+------------------+


Finds the maximum element in an array by key

.. _function-_at_linq_c__c_max_by_C1_ls_Y_ls_TT_gr_._gr_A_C.:

.. das:function:: max_by(src: array<auto(TT)> const; key: auto const)

max_by returns TT

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+
+key     +auto const           +
+--------+---------------------+


Finds the maximum element in an array by key

.. _function-_at_linq_c__c_min_max_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: min_max(src: iterator<auto(TT)>)

min_max returns tuple<TT;TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+


Finds the minimum and maximum elements in an array

.. _function-_at_linq_c__c_min_max_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: min_max(src: array<auto(TT)> const)

min_max returns tuple<TT;TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+


Finds the minimum and maximum elements in an array

.. _function-_at_linq_c__c_min_max_by_1_ls_Y_ls_TT_gr_._gr_G_C.:

.. das:function:: min_max_by(src: iterator<auto(TT)>; key: auto const)

min_max_by returns tuple<TT;TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+key     +auto const        +
+--------+------------------+


Finds the minimum and maximum elements in an array by key

.. _function-_at_linq_c__c_min_max_by_C1_ls_Y_ls_TT_gr_._gr_A_C.:

.. das:function:: min_max_by(src: array<auto(TT)> const; key: auto const)

min_max_by returns tuple<TT;TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+
+key     +auto const           +
+--------+---------------------+


Finds the minimum and maximum elements in an array by key

.. _function-_at_linq_c__c_aggregate_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_AGG_gr_._CN_ls_acc;x_gr_0_ls_CY_ls_AGG_gr_L;CY_ls_TT_gr_L_gr_1_ls_Y_ls_AGG_gr_L_gr__builtin_:

.. das:function:: aggregate(src: iterator<auto(TT)>; seed: auto(AGG) const; func: block<(acc:AGG const;x:TT const):AGG> const)

aggregate returns AGG

+--------+-------------------------------------------+
+argument+argument type                              +
+========+===========================================+
+src     +iterator<auto(TT)>                         +
+--------+-------------------------------------------+
+seed    +auto(AGG) const                            +
+--------+-------------------------------------------+
+func    +block<(acc:AGG const;x:TT const):AGG> const+
+--------+-------------------------------------------+


Aggregates elements in an array using a seed and a function

.. _function-_at_linq_c__c_aggregate_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_AGG_gr_._CN_ls_acc;x_gr_0_ls_CY_ls_AGG_gr_L;CY_ls_TT_gr_L_gr_1_ls_Y_ls_AGG_gr_L_gr__builtin_:

.. das:function:: aggregate(src: array<auto(TT)> const; seed: auto(AGG) const; func: block<(acc:AGG const;x:TT const):AGG> const)

aggregate returns AGG

+--------+-------------------------------------------+
+argument+argument type                              +
+========+===========================================+
+src     +array<auto(TT)> const                      +
+--------+-------------------------------------------+
+seed    +auto(AGG) const                            +
+--------+-------------------------------------------+
+func    +block<(acc:AGG const;x:TT const):AGG> const+
+--------+-------------------------------------------+


Aggregates elements in an array using a seed and a function

.. _function-_at_linq_c__c_sum_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: sum(src: iterator<auto(TT)>)

sum returns TT

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+


Sums elements in an array

.. _function-_at_linq_c__c_sum_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: sum(src: array<auto(TT)> const)

sum returns TT

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+


Sums elements in an array

.. _function-_at_linq_c__c_average_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: average(src: iterator<auto(TT)>)

average returns TT

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+


Averages elements in an array

.. _function-_at_linq_c__c_average_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: average(src: array<auto(TT)> const)

average returns TT

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+


Averages elements in an array

.. _function-_at_linq_c__c_min_max_average_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: min_max_average(src: iterator<auto(TT)>)

min_max_average returns tuple<TT;TT;TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+


Finds the minimum, maximum, and average elements in an array

.. _function-_at_linq_c__c_min_max_average_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: min_max_average(src: array<auto(TT)> const)

min_max_average returns tuple<TT;TT;TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+


Finds the minimum, maximum, and average elements in an array

.. _function-_at_linq_c__c_min_max_average_by_1_ls_Y_ls_TT_gr_._gr_G_C.:

.. das:function:: min_max_average_by(src: iterator<auto(TT)>; key: auto const)

min_max_average_by returns tuple<TT;TT;TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+key     +auto const        +
+--------+------------------+


Finds the minimum, maximum, and average elements in an array by key

.. _function-_at_linq_c__c_min_max_average_by_C1_ls_Y_ls_TT_gr_._gr_A_C.:

.. das:function:: min_max_average_by(src: array<auto(TT)> const; key: auto const)

min_max_average_by returns tuple<TT;TT;TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+
+key     +auto const           +
+--------+---------------------+


Finds the minimum, maximum, and average elements in an array by key

++++++++++++++
Filtering data
++++++++++++++

  *  :ref:`where_ (src:iterator\<auto(TT)\> -const;predicate:block\<(arg:TT const -&):bool\> const) : iterator\<TT -const -&\> <function-_at_linq_c__c_where__1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`where_ (src:array\<auto(TT)\> const;predicate:block\<(arg:TT const -&):bool\> const) : array\<TT -const -&\> <function-_at_linq_c__c_where__C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`where_to_array (src:iterator\<auto(TT)\> -const;predicate:block\<(arg:TT const -&):bool\> const) : array\<TT -const -&\> <function-_at_linq_c__c_where_to_array_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 

.. _function-_at_linq_c__c_where__1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: where_(src: iterator<auto(TT)>; predicate: block<(arg:TT const):bool> const)

where_ returns iterator<TT>

+---------+--------------------------------+
+argument +argument type                   +
+=========+================================+
+src      +iterator<auto(TT)>              +
+---------+--------------------------------+
+predicate+block<(arg:TT const):bool> const+
+---------+--------------------------------+


Filters elements in an array based on a predicate

.. _function-_at_linq_c__c_where__C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: where_(src: array<auto(TT)> const; predicate: block<(arg:TT const):bool> const)

where_ returns array<TT>

+---------+--------------------------------+
+argument +argument type                   +
+=========+================================+
+src      +array<auto(TT)> const           +
+---------+--------------------------------+
+predicate+block<(arg:TT const):bool> const+
+---------+--------------------------------+


Filters elements in an array based on a predicate

.. _function-_at_linq_c__c_where_to_array_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: where_to_array(src: iterator<auto(TT)>; predicate: block<(arg:TT const):bool> const)

where_to_array returns array<TT>

+---------+--------------------------------+
+argument +argument type                   +
+=========+================================+
+src      +iterator<auto(TT)>              +
+---------+--------------------------------+
+predicate+block<(arg:TT const):bool> const+
+---------+--------------------------------+


Filters elements in an iterator based on a predicate and returns an array

+++++++++++++++++
Partitioning data
+++++++++++++++++

  *  :ref:`skip (arr:array\<auto(TT)\> const;total:int -const) : array\<TT -const -&\> <function-_at_linq_c__c_skip_C1_ls_Y_ls_TT_gr_._gr_A_i>` 
  *  :ref:`skip_inplace (arr:array\<auto(TT)\> -const;total:int -const) : auto <function-_at_linq_c__c_skip_inplace_1_ls_Y_ls_TT_gr_._gr_A_i>` 
  *  :ref:`skip (src:iterator\<auto(TT)\> -const;total:int -const) : iterator\<TT -const -&\> <function-_at_linq_c__c_skip_1_ls_Y_ls_TT_gr_._gr_G_i>` 
  *  :ref:`skip_to_array (src:iterator\<auto(TT)\> -const;total:int -const) : array\<TT -const -&\> <function-_at_linq_c__c_skip_to_array_1_ls_Y_ls_TT_gr_._gr_G_i>` 
  *  :ref:`skip_while (src:iterator\<auto(TT)\> -const;predicate:block\<(arg:TT const -&):bool\> const) : iterator\<TT -const -&\> <function-_at_linq_c__c_skip_while_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`skip_while (src:array\<auto(TT)\> const;predicate:block\<(arg:TT const -&):bool\> const) : array\<TT -const -&\> <function-_at_linq_c__c_skip_while_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`skip_while_to_array (src:iterator\<auto(TT)\> -const;predicate:block\<(arg:TT const -&):bool\> const) : array\<TT -const -&\> <function-_at_linq_c__c_skip_while_to_array_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`take (arr:array\<auto(TT)\> const;total:int -const) : array\<TT -const -&\> <function-_at_linq_c__c_take_C1_ls_Y_ls_TT_gr_._gr_A_i>` 
  *  :ref:`take_inplace (arr:array\<auto(TT)\> -const;total:int -const) : auto <function-_at_linq_c__c_take_inplace_1_ls_Y_ls_TT_gr_._gr_A_i>` 
  *  :ref:`take (src:iterator\<auto(TT)\> -const;total:int -const) : iterator\<TT -const -&\> <function-_at_linq_c__c_take_1_ls_Y_ls_TT_gr_._gr_G_i>` 
  *  :ref:`take_to_array (src:iterator\<auto(TT)\> -const;total:int -const) : array\<TT -const -&\> <function-_at_linq_c__c_take_to_array_1_ls_Y_ls_TT_gr_._gr_G_i>` 
  *  :ref:`take (src:iterator\<auto(TT)\> -const;from:range const) : iterator\<TT -const -&\> <function-_at_linq_c__c_take_1_ls_Y_ls_TT_gr_._gr_G_Cr>` 
  *  :ref:`take (src:array\<auto(TT)\> const;from:range const) : array\<TT -const -&\> <function-_at_linq_c__c_take_C1_ls_Y_ls_TT_gr_._gr_A_Cr>` 
  *  :ref:`take_to_array (src:iterator\<auto(TT)\> -const;from:range const) : array\<TT -const -&\> <function-_at_linq_c__c_take_to_array_1_ls_Y_ls_TT_gr_._gr_G_Cr>` 
  *  :ref:`take_inplace (arr:array\<auto(TT)\> -const;from:range const) : auto <function-_at_linq_c__c_take_inplace_1_ls_Y_ls_TT_gr_._gr_A_Cr>` 
  *  :ref:`take_while (src:iterator\<auto(TT)\> -const;predicate:block\<(arg:TT const -&):bool\> const) : auto <function-_at_linq_c__c_take_while_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`take_while (src:array\<auto(TT)\> const;predicate:block\<(arg:TT const -&):bool\> const) : array\<TT -const -&\> <function-_at_linq_c__c_take_while_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`take_while_to_array (src:iterator\<auto(TT)\> -const;predicate:block\<(arg:TT const -&):bool\> const) : array\<TT -const -&\> <function-_at_linq_c__c_take_while_to_array_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`chunk (src:array\<auto(TT)\> const;size:int const) : array\<array\<TT -const -&\>\> <function-_at_linq_c__c_chunk_C1_ls_Y_ls_TT_gr_._gr_A_Ci>` 
  *  :ref:`chunk_to_array (src:iterator\<auto(TT)\> -const;size:int const) : array\<array\<TT -const -&\>\> <function-_at_linq_c__c_chunk_to_array_1_ls_Y_ls_TT_gr_._gr_G_Ci>` 
  *  :ref:`chunk (src:iterator\<auto(TT)\> -const;size:int const) : iterator\<array\<TT -const -&\>\> <function-_at_linq_c__c_chunk_1_ls_Y_ls_TT_gr_._gr_G_Ci>` 

.. _function-_at_linq_c__c_skip_C1_ls_Y_ls_TT_gr_._gr_A_i:

.. das:function:: skip(arr: array<auto(TT)> const; total: int)

skip returns array<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arr     +array<auto(TT)> const+
+--------+---------------------+
+total   +int                  +
+--------+---------------------+


Yields all but the first `total` elements

.. _function-_at_linq_c__c_skip_inplace_1_ls_Y_ls_TT_gr_._gr_A_i:

.. das:function:: skip_inplace(arr: array<auto(TT)>; total: int)

skip_inplace returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+arr     +array<auto(TT)>+
+--------+---------------+
+total   +int            +
+--------+---------------+


Removes the first `total` elements from an array in place

.. _function-_at_linq_c__c_skip_1_ls_Y_ls_TT_gr_._gr_G_i:

.. das:function:: skip(src: iterator<auto(TT)>; total: int)

skip returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+total   +int               +
+--------+------------------+


Yields all but the first `total` elements

.. _function-_at_linq_c__c_skip_to_array_1_ls_Y_ls_TT_gr_._gr_G_i:

.. das:function:: skip_to_array(src: iterator<auto(TT)>; total: int)

skip_to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+total   +int               +
+--------+------------------+


Yields all but the first `total` elements and returns an array

.. _function-_at_linq_c__c_skip_while_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: skip_while(src: iterator<auto(TT)>; predicate: block<(arg:TT const):bool> const)

skip_while returns iterator<TT>

+---------+--------------------------------+
+argument +argument type                   +
+=========+================================+
+src      +iterator<auto(TT)>              +
+---------+--------------------------------+
+predicate+block<(arg:TT const):bool> const+
+---------+--------------------------------+


Skips all elements of an array while the predicate is true

.. _function-_at_linq_c__c_skip_while_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: skip_while(src: array<auto(TT)> const; predicate: block<(arg:TT const):bool> const)

skip_while returns array<TT>

+---------+--------------------------------+
+argument +argument type                   +
+=========+================================+
+src      +array<auto(TT)> const           +
+---------+--------------------------------+
+predicate+block<(arg:TT const):bool> const+
+---------+--------------------------------+


Skips all elements of an array while the predicate is true

.. _function-_at_linq_c__c_skip_while_to_array_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: skip_while_to_array(src: iterator<auto(TT)>; predicate: block<(arg:TT const):bool> const)

skip_while_to_array returns array<TT>

+---------+--------------------------------+
+argument +argument type                   +
+=========+================================+
+src      +iterator<auto(TT)>              +
+---------+--------------------------------+
+predicate+block<(arg:TT const):bool> const+
+---------+--------------------------------+


Skips all elements of an iterator while the predicate is true and returns an array

.. _function-_at_linq_c__c_take_C1_ls_Y_ls_TT_gr_._gr_A_i:

.. das:function:: take(arr: array<auto(TT)> const; total: int)

take returns array<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+arr     +array<auto(TT)> const+
+--------+---------------------+
+total   +int                  +
+--------+---------------------+


Yields a range of elements from an array

.. _function-_at_linq_c__c_take_inplace_1_ls_Y_ls_TT_gr_._gr_A_i:

.. das:function:: take_inplace(arr: array<auto(TT)>; total: int)

take_inplace returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+arr     +array<auto(TT)>+
+--------+---------------+
+total   +int            +
+--------+---------------+


Keeps only a range of elements in an array in place

.. _function-_at_linq_c__c_take_1_ls_Y_ls_TT_gr_._gr_G_i:

.. das:function:: take(src: iterator<auto(TT)>; total: int)

take returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+total   +int               +
+--------+------------------+


Yields a range of elements from an array

.. _function-_at_linq_c__c_take_to_array_1_ls_Y_ls_TT_gr_._gr_G_i:

.. das:function:: take_to_array(src: iterator<auto(TT)>; total: int)

take_to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+total   +int               +
+--------+------------------+


Yields a range of elements from an iterator and returns an array

.. _function-_at_linq_c__c_take_1_ls_Y_ls_TT_gr_._gr_G_Cr:

.. das:function:: take(src: iterator<auto(TT)>; from: range const)

take returns iterator<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+from    +range const       +
+--------+------------------+


Yields a range of elements from an array

.. _function-_at_linq_c__c_take_C1_ls_Y_ls_TT_gr_._gr_A_Cr:

.. das:function:: take(src: array<auto(TT)> const; from: range const)

take returns array<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+
+from    +range const          +
+--------+---------------------+


Yields a range of elements from an array

.. _function-_at_linq_c__c_take_to_array_1_ls_Y_ls_TT_gr_._gr_G_Cr:

.. das:function:: take_to_array(src: iterator<auto(TT)>; from: range const)

take_to_array returns array<TT>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+from    +range const       +
+--------+------------------+


Yields a range of elements from an iterator and returns an array

.. _function-_at_linq_c__c_take_inplace_1_ls_Y_ls_TT_gr_._gr_A_Cr:

.. das:function:: take_inplace(arr: array<auto(TT)>; from: range const)

take_inplace returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+arr     +array<auto(TT)>+
+--------+---------------+
+from    +range const    +
+--------+---------------+


Keeps only a range of elements in an array in place

.. _function-_at_linq_c__c_take_while_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: take_while(src: iterator<auto(TT)>; predicate: block<(arg:TT const):bool> const)

take_while returns auto

+---------+--------------------------------+
+argument +argument type                   +
+=========+================================+
+src      +iterator<auto(TT)>              +
+---------+--------------------------------+
+predicate+block<(arg:TT const):bool> const+
+---------+--------------------------------+


Yields only the elements of an array while the predicate is true

.. _function-_at_linq_c__c_take_while_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: take_while(src: array<auto(TT)> const; predicate: block<(arg:TT const):bool> const)

take_while returns array<TT>

+---------+--------------------------------+
+argument +argument type                   +
+=========+================================+
+src      +array<auto(TT)> const           +
+---------+--------------------------------+
+predicate+block<(arg:TT const):bool> const+
+---------+--------------------------------+


Yields only the elements of an array while the predicate is true

.. _function-_at_linq_c__c_take_while_to_array_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: take_while_to_array(src: iterator<auto(TT)>; predicate: block<(arg:TT const):bool> const)

take_while_to_array returns array<TT>

+---------+--------------------------------+
+argument +argument type                   +
+=========+================================+
+src      +iterator<auto(TT)>              +
+---------+--------------------------------+
+predicate+block<(arg:TT const):bool> const+
+---------+--------------------------------+


Yields only the elements of an iterator while the predicate is true and returns an array

.. _function-_at_linq_c__c_chunk_C1_ls_Y_ls_TT_gr_._gr_A_Ci:

.. das:function:: chunk(src: array<auto(TT)> const; size: int const)

chunk returns array<array<TT>>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+
+size    +int const            +
+--------+---------------------+


Splits an iterator into chunks of a specified size

.. _function-_at_linq_c__c_chunk_to_array_1_ls_Y_ls_TT_gr_._gr_G_Ci:

.. das:function:: chunk_to_array(src: iterator<auto(TT)>; size: int const)

chunk_to_array returns array<array<TT>>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+size    +int const         +
+--------+------------------+


Splits an iterator into chunks of a specified size and returns an array

.. _function-_at_linq_c__c_chunk_1_ls_Y_ls_TT_gr_._gr_G_Ci:

.. das:function:: chunk(src: iterator<auto(TT)>; size: int const)

chunk returns iterator<array<TT>>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+size    +int const         +
+--------+------------------+


Splits an iterator into chunks of a specified size

+++++++++++++++++++++++++
Join and group operations
+++++++++++++++++++++++++

  *  :ref:`join (srca:iterator\<auto(TA)\> -const;srcb:iterator\<auto(TB)\> -const;keya:auto const;keyb:auto const;result:auto const) : iterator\<typedecl(result(type\<TA\>,type\<TB\>)) -const -&\> <function-_at_linq_c__c_join_1_ls_Y_ls_TA_gr_._gr_G_1_ls_Y_ls_TB_gr_._gr_G_C._C._C.>` 
  *  :ref:`join (srca:array\<auto(TA)\> const;srcb:array\<auto(TB)\> const;keya:auto const;keyb:auto const;result:auto const) : array\<typedecl(result(type\<TA\>,type\<TB\>)) -const -&\> <function-_at_linq_c__c_join_C1_ls_Y_ls_TA_gr_._gr_A_C1_ls_Y_ls_TB_gr_._gr_A_C._C._C.>` 
  *  :ref:`join_to_array (srca:iterator\<auto(TA)\> -const;srcb:iterator\<auto(TB)\> -const;keya:auto const;keyb:auto const;result:auto const) : array\<typedecl(result(type\<TA\>,type\<TB\>)) -const -&\> <function-_at_linq_c__c_join_to_array_1_ls_Y_ls_TA_gr_._gr_G_1_ls_Y_ls_TB_gr_._gr_G_C._C._C.>` 
  *  :ref:`group_join (srca:iterator\<auto(TA)\> -const;srcb:iterator\<auto(TB)\> -const;keya:auto const;keyb:auto const;result:auto const) : iterator\<typedecl(result(type\<TA\>,type\<array\<TB -const -&\>\>)) -const -&\> <function-_at_linq_c__c_group_join_1_ls_Y_ls_TA_gr_._gr_G_1_ls_Y_ls_TB_gr_._gr_G_C._C._C.>` 
  *  :ref:`group_join (srca:array\<auto(TA)\> const;srcb:array\<auto(TB)\> const;keya:auto const;keyb:auto const;result:auto const) : array\<typedecl(result(type\<TA\>,type\<array\<TB -const -&\>\>)) -const -&\> <function-_at_linq_c__c_group_join_C1_ls_Y_ls_TA_gr_._gr_A_C1_ls_Y_ls_TB_gr_._gr_A_C._C._C.>` 
  *  :ref:`group_join_to_array (srca:iterator\<auto(TA)\> -const;srcb:iterator\<auto(TB)\> -const;keya:auto const;keyb:auto const;result:auto const) : array\<typedecl(result(type\<TA\>,type\<array\<TB -const -&\>\>)) -const -&\> <function-_at_linq_c__c_group_join_to_array_1_ls_Y_ls_TA_gr_._gr_G_1_ls_Y_ls_TB_gr_._gr_G_C._C._C.>` 
  *  :ref:`group_by (source:iterator\<auto(TT)\> -const;key:auto const;element_selector:auto const;result_selector:auto const) : auto <function-_at_linq_c__c_group_by_1_ls_Y_ls_TT_gr_._gr_G_C._C._C.>` 
  *  :ref:`group_by (source:array\<auto(TT)\> const;key:auto const;element_selector:auto const;result_selector:auto const) : auto <function-_at_linq_c__c_group_by_C1_ls_Y_ls_TT_gr_._gr_A_C._C._C.>` 
  *  :ref:`group_by_to_array (source:iterator\<auto(TT)\> -const;key:auto const;element_selector:auto const;result_selector:auto const) : auto <function-_at_linq_c__c_group_by_to_array_1_ls_Y_ls_TT_gr_._gr_G_C._C._C.>` 

.. _function-_at_linq_c__c_join_1_ls_Y_ls_TA_gr_._gr_G_1_ls_Y_ls_TB_gr_._gr_G_C._C._C.:

.. das:function:: join(srca: iterator<auto(TA)>; srcb: iterator<auto(TB)>; keya: auto const; keyb: auto const; result: auto const)

join returns iterator<typedecl>

+--------+------------------+
+argument+argument type     +
+========+==================+
+srca    +iterator<auto(TA)>+
+--------+------------------+
+srcb    +iterator<auto(TB)>+
+--------+------------------+
+keya    +auto const        +
+--------+------------------+
+keyb    +auto const        +
+--------+------------------+
+result  +auto const        +
+--------+------------------+


Joins two arrays based on matching keys (inner join)

.. _function-_at_linq_c__c_join_C1_ls_Y_ls_TA_gr_._gr_A_C1_ls_Y_ls_TB_gr_._gr_A_C._C._C.:

.. das:function:: join(srca: array<auto(TA)> const; srcb: array<auto(TB)> const; keya: auto const; keyb: auto const; result: auto const)

join returns array<typedecl>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+srca    +array<auto(TA)> const+
+--------+---------------------+
+srcb    +array<auto(TB)> const+
+--------+---------------------+
+keya    +auto const           +
+--------+---------------------+
+keyb    +auto const           +
+--------+---------------------+
+result  +auto const           +
+--------+---------------------+


Joins two arrays based on matching keys (inner join)

.. _function-_at_linq_c__c_join_to_array_1_ls_Y_ls_TA_gr_._gr_G_1_ls_Y_ls_TB_gr_._gr_G_C._C._C.:

.. das:function:: join_to_array(srca: iterator<auto(TA)>; srcb: iterator<auto(TB)>; keya: auto const; keyb: auto const; result: auto const)

join_to_array returns array<typedecl>

+--------+------------------+
+argument+argument type     +
+========+==================+
+srca    +iterator<auto(TA)>+
+--------+------------------+
+srcb    +iterator<auto(TB)>+
+--------+------------------+
+keya    +auto const        +
+--------+------------------+
+keyb    +auto const        +
+--------+------------------+
+result  +auto const        +
+--------+------------------+


Joins two iterators based on matching keys (inner join) and returns an array

.. _function-_at_linq_c__c_group_join_1_ls_Y_ls_TA_gr_._gr_G_1_ls_Y_ls_TB_gr_._gr_G_C._C._C.:

.. das:function:: group_join(srca: iterator<auto(TA)>; srcb: iterator<auto(TB)>; keya: auto const; keyb: auto const; result: auto const)

group_join returns iterator<typedecl>

+--------+------------------+
+argument+argument type     +
+========+==================+
+srca    +iterator<auto(TA)>+
+--------+------------------+
+srcb    +iterator<auto(TB)>+
+--------+------------------+
+keya    +auto const        +
+--------+------------------+
+keyb    +auto const        +
+--------+------------------+
+result  +auto const        +
+--------+------------------+


we pass TA, and sequence of TB to 'result'

.. _function-_at_linq_c__c_group_join_C1_ls_Y_ls_TA_gr_._gr_A_C1_ls_Y_ls_TB_gr_._gr_A_C._C._C.:

.. das:function:: group_join(srca: array<auto(TA)> const; srcb: array<auto(TB)> const; keya: auto const; keyb: auto const; result: auto const)

group_join returns array<typedecl>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+srca    +array<auto(TA)> const+
+--------+---------------------+
+srcb    +array<auto(TB)> const+
+--------+---------------------+
+keya    +auto const           +
+--------+---------------------+
+keyb    +auto const           +
+--------+---------------------+
+result  +auto const           +
+--------+---------------------+


we pass TA, and sequence of TB to 'result'

.. _function-_at_linq_c__c_group_join_to_array_1_ls_Y_ls_TA_gr_._gr_G_1_ls_Y_ls_TB_gr_._gr_G_C._C._C.:

.. das:function:: group_join_to_array(srca: iterator<auto(TA)>; srcb: iterator<auto(TB)>; keya: auto const; keyb: auto const; result: auto const)

group_join_to_array returns array<typedecl>

+--------+------------------+
+argument+argument type     +
+========+==================+
+srca    +iterator<auto(TA)>+
+--------+------------------+
+srcb    +iterator<auto(TB)>+
+--------+------------------+
+keya    +auto const        +
+--------+------------------+
+keyb    +auto const        +
+--------+------------------+
+result  +auto const        +
+--------+------------------+


we pass TA, and sequence of TB to 'result'

.. _function-_at_linq_c__c_group_by_1_ls_Y_ls_TT_gr_._gr_G_C._C._C.:

.. das:function:: group_by(source: iterator<auto(TT)>; key: auto const; element_selector: auto const; result_selector: auto const)

group_by returns auto

+----------------+------------------+
+argument        +argument type     +
+================+==================+
+source          +iterator<auto(TT)>+
+----------------+------------------+
+key             +auto const        +
+----------------+------------------+
+element_selector+auto const        +
+----------------+------------------+
+result_selector +auto const        +
+----------------+------------------+


Groups the elements of an array according to a specified key selector function

.. _function-_at_linq_c__c_group_by_C1_ls_Y_ls_TT_gr_._gr_A_C._C._C.:

.. das:function:: group_by(source: array<auto(TT)> const; key: auto const; element_selector: auto const; result_selector: auto const)

group_by returns auto

+----------------+---------------------+
+argument        +argument type        +
+================+=====================+
+source          +array<auto(TT)> const+
+----------------+---------------------+
+key             +auto const           +
+----------------+---------------------+
+element_selector+auto const           +
+----------------+---------------------+
+result_selector +auto const           +
+----------------+---------------------+


Groups the elements of an array according to a specified key selector function

.. _function-_at_linq_c__c_group_by_to_array_1_ls_Y_ls_TT_gr_._gr_G_C._C._C.:

.. das:function:: group_by_to_array(source: iterator<auto(TT)>; key: auto const; element_selector: auto const; result_selector: auto const)

group_by_to_array returns auto

+----------------+------------------+
+argument        +argument type     +
+================+==================+
+source          +iterator<auto(TT)>+
+----------------+------------------+
+key             +auto const        +
+----------------+------------------+
+element_selector+auto const        +
+----------------+------------------+
+result_selector +auto const        +
+----------------+------------------+


Groups the elements of an iterator according to a specified key selector function and returns an array

+++++++++++++
Querying data
+++++++++++++

  *  :ref:`any (src:array\<auto(TT)\> const) : bool <function-_at_linq_c__c_any_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`any (src:iterator\<auto(TT)\> -const) : bool <function-_at_linq_c__c_any_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`any (src:iterator\<auto(TT)\> -const;predicate:block\<(arg:TT const -&):bool\> const) : bool <function-_at_linq_c__c_any_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`any (src:array\<auto(TT)\> const;predicate:block\<(arg:TT const -&):bool\> const) : bool <function-_at_linq_c__c_any_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`all (src:iterator\<auto(TT)\> -const;predicate:block\<(arg:TT const -&):bool\> const) : bool <function-_at_linq_c__c_all_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`all (src:array\<auto(TT)\> const;predicate:block\<(arg:TT const -&):bool\> const) : bool <function-_at_linq_c__c_all_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_>` 
  *  :ref:`contains (src:iterator\<auto(TT)\> -const;element:TT const -&) : bool <function-_at_linq_c__c_contains_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L>` 
  *  :ref:`contains (src:array\<auto(TT)\> const;element:TT const -&) : bool <function-_at_linq_c__c_contains_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L>` 

.. _function-_at_linq_c__c_any_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: any(src: array<auto(TT)> const)

any returns bool

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+


Returns true if any element in the array satisfies the predicate

.. _function-_at_linq_c__c_any_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: any(src: iterator<auto(TT)>)

any returns bool

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+


Returns true if any element in the array satisfies the predicate

.. _function-_at_linq_c__c_any_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: any(src: iterator<auto(TT)>; predicate: block<(arg:TT const):bool> const)

any returns bool

+---------+--------------------------------+
+argument +argument type                   +
+=========+================================+
+src      +iterator<auto(TT)>              +
+---------+--------------------------------+
+predicate+block<(arg:TT const):bool> const+
+---------+--------------------------------+


Returns true if any element in the array satisfies the predicate

.. _function-_at_linq_c__c_any_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: any(src: array<auto(TT)> const; predicate: block<(arg:TT const):bool> const)

any returns bool

+---------+--------------------------------+
+argument +argument type                   +
+=========+================================+
+src      +array<auto(TT)> const           +
+---------+--------------------------------+
+predicate+block<(arg:TT const):bool> const+
+---------+--------------------------------+


Returns true if any element in the array satisfies the predicate

.. _function-_at_linq_c__c_all_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: all(src: iterator<auto(TT)>; predicate: block<(arg:TT const):bool> const)

all returns bool

+---------+--------------------------------+
+argument +argument type                   +
+=========+================================+
+src      +iterator<auto(TT)>              +
+---------+--------------------------------+
+predicate+block<(arg:TT const):bool> const+
+---------+--------------------------------+


Returns true if all elements in the array satisfy the predicate

.. _function-_at_linq_c__c_all_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: all(src: array<auto(TT)> const; predicate: block<(arg:TT const):bool> const)

all returns bool

+---------+--------------------------------+
+argument +argument type                   +
+=========+================================+
+src      +array<auto(TT)> const           +
+---------+--------------------------------+
+predicate+block<(arg:TT const):bool> const+
+---------+--------------------------------+


Returns true if all elements in the array satisfy the predicate

.. _function-_at_linq_c__c_contains_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L:

.. das:function:: contains(src: iterator<auto(TT)>; element: TT const)

contains returns bool

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+element +TT const          +
+--------+------------------+


Returns true if the element is present in the array

.. _function-_at_linq_c__c_contains_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L:

.. das:function:: contains(src: array<auto(TT)> const; element: TT const)

contains returns bool

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+
+element +TT const             +
+--------+---------------------+


Returns true if the element is present in the array

++++++++++++++++++
Element operations
++++++++++++++++++

  *  :ref:`element_at (src:array\<auto(TT)\> const;index:int const) : TT -const -& <function-_at_linq_c__c_element_at_C1_ls_Y_ls_TT_gr_._gr_A_Ci>` 
  *  :ref:`element_at (src:iterator\<auto(TT)\> -const;index:int const) : TT -const -& <function-_at_linq_c__c_element_at_1_ls_Y_ls_TT_gr_._gr_G_Ci>` 
  *  :ref:`element_at_or_default (src:array\<auto(TT)\> const;index:int const) : TT -const -& <function-_at_linq_c__c_element_at_or_default_C1_ls_Y_ls_TT_gr_._gr_A_Ci>` 
  *  :ref:`element_at_or_default (src:iterator\<auto(TT)\> -const;index:int const) : TT -const -& <function-_at_linq_c__c_element_at_or_default_1_ls_Y_ls_TT_gr_._gr_G_Ci>` 
  *  :ref:`first (src:array\<auto(TT)\> const) : TT -const -& <function-_at_linq_c__c_first_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`first (src:iterator\<auto(TT)\> -const) : TT -const -& <function-_at_linq_c__c_first_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`first_or_default (src:array\<auto(TT)\> const;defaultValue:TT const -&) : TT -const -& <function-_at_linq_c__c_first_or_default_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L>` 
  *  :ref:`first_or_default (src:iterator\<auto(TT)\> -const;defaultValue:TT const -&) : TT -const -& <function-_at_linq_c__c_first_or_default_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L>` 
  *  :ref:`last (src:array\<auto(TT)\> const) : TT -const -& <function-_at_linq_c__c_last_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`last (src:iterator\<auto(TT)\> -const) : TT -const -& <function-_at_linq_c__c_last_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`last_or_default (src:array\<auto(TT)\> const;defaultValue:TT const -&) : TT -const -& <function-_at_linq_c__c_last_or_default_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L>` 
  *  :ref:`last_or_default (src:iterator\<auto(TT)\> -const;defaultValue:TT const -&) : TT -const -& <function-_at_linq_c__c_last_or_default_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L>` 
  *  :ref:`single (src:array\<auto(TT)\> const) : TT -const -& <function-_at_linq_c__c_single_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`single (src:iterator\<auto(TT)\> -const) : TT -const -& <function-_at_linq_c__c_single_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`single_or_default (src:array\<auto(TT)\> const;defaultValue:TT const -&) : TT -const -& <function-_at_linq_c__c_single_or_default_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L>` 
  *  :ref:`single_or_default (src:iterator\<auto(TT)\> -const;defaultValue:TT const -&) : TT -const -& <function-_at_linq_c__c_single_or_default_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L>` 

.. _function-_at_linq_c__c_element_at_C1_ls_Y_ls_TT_gr_._gr_A_Ci:

.. das:function:: element_at(src: array<auto(TT)> const; index: int const)

element_at returns TT

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+
+index   +int const            +
+--------+---------------------+


Returns the element at the specified index

.. _function-_at_linq_c__c_element_at_1_ls_Y_ls_TT_gr_._gr_G_Ci:

.. das:function:: element_at(src: iterator<auto(TT)>; index: int const)

element_at returns TT

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+index   +int const         +
+--------+------------------+


Returns the element at the specified index

.. _function-_at_linq_c__c_element_at_or_default_C1_ls_Y_ls_TT_gr_._gr_A_Ci:

.. das:function:: element_at_or_default(src: array<auto(TT)> const; index: int const)

element_at_or_default returns TT

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+
+index   +int const            +
+--------+---------------------+


Returns the element at the specified index, or a default value if the index is out of range

.. _function-_at_linq_c__c_element_at_or_default_1_ls_Y_ls_TT_gr_._gr_G_Ci:

.. das:function:: element_at_or_default(src: iterator<auto(TT)>; index: int const)

element_at_or_default returns TT

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+index   +int const         +
+--------+------------------+


Returns the element at the specified index, or a default value if the index is out of range

.. _function-_at_linq_c__c_first_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: first(src: array<auto(TT)> const)

first returns TT

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+


Returns the first element of an iterator

.. _function-_at_linq_c__c_first_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: first(src: iterator<auto(TT)>)

first returns TT

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+


Returns the first element of an iterator

.. _function-_at_linq_c__c_first_or_default_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L:

.. das:function:: first_or_default(src: array<auto(TT)> const; defaultValue: TT const)

first_or_default returns TT

+------------+---------------------+
+argument    +argument type        +
+============+=====================+
+src         +array<auto(TT)> const+
+------------+---------------------+
+defaultValue+TT const             +
+------------+---------------------+


Returns the first element of an iterator, or a default value if the iterator is empty

.. _function-_at_linq_c__c_first_or_default_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L:

.. das:function:: first_or_default(src: iterator<auto(TT)>; defaultValue: TT const)

first_or_default returns TT

+------------+------------------+
+argument    +argument type     +
+============+==================+
+src         +iterator<auto(TT)>+
+------------+------------------+
+defaultValue+TT const          +
+------------+------------------+


Returns the first element of an iterator, or a default value if the iterator is empty

.. _function-_at_linq_c__c_last_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: last(src: array<auto(TT)> const)

last returns TT

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+


Returns the last element of an iterator

.. _function-_at_linq_c__c_last_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: last(src: iterator<auto(TT)>)

last returns TT

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+


Returns the last element of an iterator

.. _function-_at_linq_c__c_last_or_default_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L:

.. das:function:: last_or_default(src: array<auto(TT)> const; defaultValue: TT const)

last_or_default returns TT

+------------+---------------------+
+argument    +argument type        +
+============+=====================+
+src         +array<auto(TT)> const+
+------------+---------------------+
+defaultValue+TT const             +
+------------+---------------------+


Returns the last element of an iterator, or a default value if the iterator is empty

.. _function-_at_linq_c__c_last_or_default_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L:

.. das:function:: last_or_default(src: iterator<auto(TT)>; defaultValue: TT const)

last_or_default returns TT

+------------+------------------+
+argument    +argument type     +
+============+==================+
+src         +iterator<auto(TT)>+
+------------+------------------+
+defaultValue+TT const          +
+------------+------------------+


Returns the last element of an iterator, or a default value if the iterator is empty

.. _function-_at_linq_c__c_single_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: single(src: array<auto(TT)> const)

single returns TT

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+


Returns the only element of an iterator, and throws if there is not exactly one element

.. _function-_at_linq_c__c_single_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: single(src: iterator<auto(TT)>)

single returns TT

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+


Returns the only element of an iterator, and throws if there is not exactly one element

.. _function-_at_linq_c__c_single_or_default_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L:

.. das:function:: single_or_default(src: array<auto(TT)> const; defaultValue: TT const)

single_or_default returns TT

+------------+---------------------+
+argument    +argument type        +
+============+=====================+
+src         +array<auto(TT)> const+
+------------+---------------------+
+defaultValue+TT const             +
+------------+---------------------+


Returns the only element of an iterator, or a default value if there is not exactly one element

.. _function-_at_linq_c__c_single_or_default_1_ls_Y_ls_TT_gr_._gr_G_CY_ls_TT_gr_L:

.. das:function:: single_or_default(src: iterator<auto(TT)>; defaultValue: TT const)

single_or_default returns TT

+------------+------------------+
+argument    +argument type     +
+============+==================+
+src         +iterator<auto(TT)>+
+------------+------------------+
+defaultValue+TT const          +
+------------+------------------+


Returns the only element of an iterator, or a default value if there is not exactly one element

++++++++++++++++++++
Transform operations
++++++++++++++++++++

  *  :ref:`select (src:iterator\<auto(TT)\> -const) : iterator\<tuple\<int;TT -const -&\>\> <function-_at_linq_c__c_select_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`select (src:array\<auto(TT)\> const) : array\<tuple\<int;TT -const -&\>\> <function-_at_linq_c__c_select_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`select_to_array (src:iterator\<auto(TT)\> -const) : array\<tuple\<int;TT -const -&\>\> <function-_at_linq_c__c_select_to_array_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`select (src:iterator\<auto(TT)\> -const;result_selector:auto const) : iterator\<typedecl(result_selector(type\<TT\>)) -const -&\> <function-_at_linq_c__c_select_1_ls_Y_ls_TT_gr_._gr_G_C.>` 
  *  :ref:`select (src:array\<auto(TT)\> const;result_selector:auto const) : array\<typedecl(result_selector(type\<TT\>)) -const -&\> <function-_at_linq_c__c_select_C1_ls_Y_ls_TT_gr_._gr_A_C.>` 
  *  :ref:`select_to_array (src:iterator\<auto(TT)\> -const;result_selector:auto const) : array\<typedecl(result_selector(type\<TT\>)) -const -&\> <function-_at_linq_c__c_select_to_array_1_ls_Y_ls_TT_gr_._gr_G_C.>` 
  *  :ref:`select_many (src:iterator\<auto(TT)\> -const;result_selector:auto const) : iterator\<typedecl(result_selector(iter_type(type\<TT\>))) -const -&\> <function-_at_linq_c__c_select_many_1_ls_Y_ls_TT_gr_._gr_G_C.>` 
  *  :ref:`select_many (src:array\<auto(TT)\> const;result_selector:auto const) : array\<typedecl(result_selector(iter_type(type\<TT\>))) -const -&\> <function-_at_linq_c__c_select_many_C1_ls_Y_ls_TT_gr_._gr_A_C.>` 
  *  :ref:`select_many_to_array (src:iterator\<auto(TT)\> -const;result_selector:auto const) : array\<typedecl(result_selector(iter_type(type\<TT\>))) -const -&\> <function-_at_linq_c__c_select_many_to_array_1_ls_Y_ls_TT_gr_._gr_G_C.>` 
  *  :ref:`select_many (src:iterator\<auto(TT)\> -const;collection_selector:auto const;result_selector:auto const) : iterator\<typedecl(result_selector(iter_type(collection_selector(type\<TT\>)))) -const -&\> <function-_at_linq_c__c_select_many_1_ls_Y_ls_TT_gr_._gr_G_C._C.>` 
  *  :ref:`select_many (src:array\<auto(TT)\> const;collection_selector:auto const;result_selector:auto const) : array\<typedecl(result_selector(iter_type(collection_selector(type\<TT\>)))) -const -&\> <function-_at_linq_c__c_select_many_C1_ls_Y_ls_TT_gr_._gr_A_C._C.>` 
  *  :ref:`select_many_to_array (src:iterator\<auto(TT)\> -const;collection_selector:auto const;result_selector:auto const) : array\<typedecl(result_selector(iter_type(collection_selector(type\<TT\>)))) -const -&\> <function-_at_linq_c__c_select_many_to_array_1_ls_Y_ls_TT_gr_._gr_G_C._C.>` 
  *  :ref:`zip (a:iterator\<auto(TT)\> -const;b:iterator\<auto(UU)\> -const) : iterator\<tuple\<TT -const -&;UU -const -&\>\> <function-_at_linq_c__c_zip_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_UU_gr_._gr_G>` 
  *  :ref:`zip (a:array\<auto(TT)\> const;b:array\<auto(UU)\> const) : array\<tuple\<TT -const -&;UU -const -&\>\> <function-_at_linq_c__c_zip_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_UU_gr_._gr_A>` 
  *  :ref:`zip_to_array (a:iterator\<auto(TT)\> -const;b:iterator\<auto(UU)\> -const) : array\<tuple\<TT -const -&;UU -const -&\>\> <function-_at_linq_c__c_zip_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_UU_gr_._gr_G>` 
  *  :ref:`zip (a:iterator\<auto(TT)\> -const;b:iterator\<auto(UU)\> -const;result_selector:block\<(l:TT const -&;r:UU const -&):auto\> const) : iterator\<typedecl(result_selector(type\<TT\>,type\<UU\>)) -const -&\> <function-_at_linq_c__c_zip_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_UU_gr_._gr_G_CN_ls_l;r_gr_0_ls_CY_ls_TT_gr_L;CY_ls_UU_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`zip (a:array\<auto(TT)\> const;b:array\<auto(UU)\> const;result_selector:block\<(l:TT const -&;r:UU const -&):auto\> const) : array\<typedecl(result_selector(type\<TT\>,type\<UU\>)) -const -&\> <function-_at_linq_c__c_zip_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_UU_gr_._gr_A_CN_ls_l;r_gr_0_ls_CY_ls_TT_gr_L;CY_ls_UU_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`zip_to_array (a:iterator\<auto(TT)\> -const;b:iterator\<auto(UU)\> -const;result_selector:block\<(l:TT const -&;r:UU const -&):auto\> const) : array\<typedecl(result_selector(type\<TT\>,type\<UU\>)) -const -&\> <function-_at_linq_c__c_zip_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_UU_gr_._gr_G_CN_ls_l;r_gr_0_ls_CY_ls_TT_gr_L;CY_ls_UU_gr_L_gr_1_ls_._gr__builtin_>` 

.. _function-_at_linq_c__c_select_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: select(src: iterator<auto(TT)>)

select returns iterator<tuple<int;TT>>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+


Projects each element of an array into a new form

.. _function-_at_linq_c__c_select_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: select(src: array<auto(TT)> const)

select returns array<tuple<int;TT>>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+src     +array<auto(TT)> const+
+--------+---------------------+


Projects each element of an array into a new form

.. _function-_at_linq_c__c_select_to_array_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: select_to_array(src: iterator<auto(TT)>)

select_to_array returns array<tuple<int;TT>>

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+


Projects each element of an iterator into a new form and returns an array

.. _function-_at_linq_c__c_select_1_ls_Y_ls_TT_gr_._gr_G_C.:

.. das:function:: select(src: iterator<auto(TT)>; result_selector: auto const)

select returns iterator<typedecl>

+---------------+------------------+
+argument       +argument type     +
+===============+==================+
+src            +iterator<auto(TT)>+
+---------------+------------------+
+result_selector+auto const        +
+---------------+------------------+


Projects each element of an array into a new form

.. _function-_at_linq_c__c_select_C1_ls_Y_ls_TT_gr_._gr_A_C.:

.. das:function:: select(src: array<auto(TT)> const; result_selector: auto const)

select returns array<typedecl>

+---------------+---------------------+
+argument       +argument type        +
+===============+=====================+
+src            +array<auto(TT)> const+
+---------------+---------------------+
+result_selector+auto const           +
+---------------+---------------------+


Projects each element of an array into a new form

.. _function-_at_linq_c__c_select_to_array_1_ls_Y_ls_TT_gr_._gr_G_C.:

.. das:function:: select_to_array(src: iterator<auto(TT)>; result_selector: auto const)

select_to_array returns array<typedecl>

+---------------+------------------+
+argument       +argument type     +
+===============+==================+
+src            +iterator<auto(TT)>+
+---------------+------------------+
+result_selector+auto const        +
+---------------+------------------+


Projects each element of an iterator into a new form and returns an array

.. _function-_at_linq_c__c_select_many_1_ls_Y_ls_TT_gr_._gr_G_C.:

.. das:function:: select_many(src: iterator<auto(TT)>; result_selector: auto const)

select_many returns iterator<typedecl>

+---------------+------------------+
+argument       +argument type     +
+===============+==================+
+src            +iterator<auto(TT)>+
+---------------+------------------+
+result_selector+auto const        +
+---------------+------------------+


Projects each element of an array to an iterator and flattens the resulting iterators into one array

.. _function-_at_linq_c__c_select_many_C1_ls_Y_ls_TT_gr_._gr_A_C.:

.. das:function:: select_many(src: array<auto(TT)> const; result_selector: auto const)

select_many returns array<typedecl>

+---------------+---------------------+
+argument       +argument type        +
+===============+=====================+
+src            +array<auto(TT)> const+
+---------------+---------------------+
+result_selector+auto const           +
+---------------+---------------------+


Projects each element of an array to an iterator and flattens the resulting iterators into one array

.. _function-_at_linq_c__c_select_many_to_array_1_ls_Y_ls_TT_gr_._gr_G_C.:

.. das:function:: select_many_to_array(src: iterator<auto(TT)>; result_selector: auto const)

select_many_to_array returns array<typedecl>

+---------------+------------------+
+argument       +argument type     +
+===============+==================+
+src            +iterator<auto(TT)>+
+---------------+------------------+
+result_selector+auto const        +
+---------------+------------------+


Projects each element of an iterator to an iterator and flattens the resulting iterators into one array

.. _function-_at_linq_c__c_select_many_1_ls_Y_ls_TT_gr_._gr_G_C._C.:

.. das:function:: select_many(src: iterator<auto(TT)>; collection_selector: auto const; result_selector: auto const)

select_many returns iterator<typedecl>

+-------------------+------------------+
+argument           +argument type     +
+===================+==================+
+src                +iterator<auto(TT)>+
+-------------------+------------------+
+collection_selector+auto const        +
+-------------------+------------------+
+result_selector    +auto const        +
+-------------------+------------------+


Projects each element of an array to an iterator and flattens the resulting iterators into one array

.. _function-_at_linq_c__c_select_many_C1_ls_Y_ls_TT_gr_._gr_A_C._C.:

.. das:function:: select_many(src: array<auto(TT)> const; collection_selector: auto const; result_selector: auto const)

select_many returns array<typedecl>

+-------------------+---------------------+
+argument           +argument type        +
+===================+=====================+
+src                +array<auto(TT)> const+
+-------------------+---------------------+
+collection_selector+auto const           +
+-------------------+---------------------+
+result_selector    +auto const           +
+-------------------+---------------------+


Projects each element of an array to an iterator and flattens the resulting iterators into one array

.. _function-_at_linq_c__c_select_many_to_array_1_ls_Y_ls_TT_gr_._gr_G_C._C.:

.. das:function:: select_many_to_array(src: iterator<auto(TT)>; collection_selector: auto const; result_selector: auto const)

select_many_to_array returns array<typedecl>

+-------------------+------------------+
+argument           +argument type     +
+===================+==================+
+src                +iterator<auto(TT)>+
+-------------------+------------------+
+collection_selector+auto const        +
+-------------------+------------------+
+result_selector    +auto const        +
+-------------------+------------------+


Projects each element of an iterator to an iterator and flattens the resulting iterators into one array

.. _function-_at_linq_c__c_zip_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_UU_gr_._gr_G:

.. das:function:: zip(a: iterator<auto(TT)>; b: iterator<auto(UU)>)

zip returns iterator<tuple<TT;UU>>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+
+b       +iterator<auto(UU)>+
+--------+------------------+


Merges two arrays into an array by applying a specified function

.. _function-_at_linq_c__c_zip_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_UU_gr_._gr_A:

.. das:function:: zip(a: array<auto(TT)> const; b: array<auto(UU)> const)

zip returns array<tuple<TT;UU>>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+
+b       +array<auto(UU)> const+
+--------+---------------------+


Merges two arrays into an array by applying a specified function

.. _function-_at_linq_c__c_zip_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_UU_gr_._gr_G:

.. das:function:: zip_to_array(a: iterator<auto(TT)>; b: iterator<auto(UU)>)

zip_to_array returns array<tuple<TT;UU>>

+--------+------------------+
+argument+argument type     +
+========+==================+
+a       +iterator<auto(TT)>+
+--------+------------------+
+b       +iterator<auto(UU)>+
+--------+------------------+


Merges two iterators into an array by applying a specified function

.. _function-_at_linq_c__c_zip_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_UU_gr_._gr_G_CN_ls_l;r_gr_0_ls_CY_ls_TT_gr_L;CY_ls_UU_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: zip(a: iterator<auto(TT)>; b: iterator<auto(UU)>; result_selector: block<(l:TT const;r:UU const):auto> const)

zip returns iterator<typedecl>

+---------------+-----------------------------------------+
+argument       +argument type                            +
+===============+=========================================+
+a              +iterator<auto(TT)>                       +
+---------------+-----------------------------------------+
+b              +iterator<auto(UU)>                       +
+---------------+-----------------------------------------+
+result_selector+block<(l:TT const;r:UU const):auto> const+
+---------------+-----------------------------------------+


Merges two arrays into an array by applying a specified function

.. _function-_at_linq_c__c_zip_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_UU_gr_._gr_A_CN_ls_l;r_gr_0_ls_CY_ls_TT_gr_L;CY_ls_UU_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: zip(a: array<auto(TT)> const; b: array<auto(UU)> const; result_selector: block<(l:TT const;r:UU const):auto> const)

zip returns array<typedecl>

+---------------+-----------------------------------------+
+argument       +argument type                            +
+===============+=========================================+
+a              +array<auto(TT)> const                    +
+---------------+-----------------------------------------+
+b              +array<auto(UU)> const                    +
+---------------+-----------------------------------------+
+result_selector+block<(l:TT const;r:UU const):auto> const+
+---------------+-----------------------------------------+


Merges two arrays into an array by applying a specified function

.. _function-_at_linq_c__c_zip_to_array_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_UU_gr_._gr_G_CN_ls_l;r_gr_0_ls_CY_ls_TT_gr_L;CY_ls_UU_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: zip_to_array(a: iterator<auto(TT)>; b: iterator<auto(UU)>; result_selector: block<(l:TT const;r:UU const):auto> const)

zip_to_array returns array<typedecl>

+---------------+-----------------------------------------+
+argument       +argument type                            +
+===============+=========================================+
+a              +iterator<auto(TT)>                       +
+---------------+-----------------------------------------+
+b              +iterator<auto(UU)>                       +
+---------------+-----------------------------------------+
+result_selector+block<(l:TT const;r:UU const):auto> const+
+---------------+-----------------------------------------+


Merges two iterators into an array by applying a specified function

+++++++++++++++++++++
Convertion operations
+++++++++++++++++++++

  *  :ref:`to_sequence (a:array\<auto(TT)\> const) : iterator\<TT -const -&\> <function-_at_linq_c__c_to_sequence_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`to_sequence_move (a:array\<auto(TT)\> -const) : iterator\<TT -const -&\> <function-_at_linq_c__c_to_sequence_move_1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`to_table (a:iterator\<auto(TT)\> -const;key:block\<(v:TT const -&):auto\> const;elementSelector:block\<(v:TT const -&):auto\> const) : table\<typedecl(_::unique_key(type\<TT\>)) -const -&;typedecl(elementSelector(type\<TT\>)) -const -&\> <function-_at_linq_c__c_to_table_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_v_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin__CN_ls_v_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`to_table (a:array\<auto(TT)\> const;key:block\<(v:TT const -&):auto\> const;elementSelector:block\<(v:TT const -&):auto\> const) : table\<typedecl(_::unique_key(type\<TT\>)) -const -&;typedecl(elementSelector(type\<TT\>)) -const -&\> <function-_at_linq_c__c_to_table_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_v_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin__CN_ls_v_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 

.. _function-_at_linq_c__c_to_sequence_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: to_sequence(a: array<auto(TT)> const)

to_sequence returns iterator<TT>

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+


Converts an array to an iterator

.. _function-_at_linq_c__c_to_sequence_move_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: to_sequence_move(a: array<auto(TT)>)

to_sequence_move returns iterator<TT>

+--------+---------------+
+argument+argument type  +
+========+===============+
+a       +array<auto(TT)>+
+--------+---------------+


Converts an array to an iterator, captures input

.. _function-_at_linq_c__c_to_table_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_v_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin__CN_ls_v_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: to_table(a: iterator<auto(TT)>; key: block<(v:TT const):auto> const; elementSelector: block<(v:TT const):auto> const)

to_table returns table<typedecl;typedecl>

+---------------+------------------------------+
+argument       +argument type                 +
+===============+==============================+
+a              +iterator<auto(TT)>            +
+---------------+------------------------------+
+key            +block<(v:TT const):auto> const+
+---------------+------------------------------+
+elementSelector+block<(v:TT const):auto> const+
+---------------+------------------------------+


Converts an array to a table

.. _function-_at_linq_c__c_to_table_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_v_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin__CN_ls_v_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: to_table(a: array<auto(TT)> const; key: block<(v:TT const):auto> const; elementSelector: block<(v:TT const):auto> const)

to_table returns table<typedecl;typedecl>

+---------------+------------------------------+
+argument       +argument type                 +
+===============+==============================+
+a              +array<auto(TT)> const         +
+---------------+------------------------------+
+key            +block<(v:TT const):auto> const+
+---------------+------------------------------+
+elementSelector+block<(v:TT const):auto> const+
+---------------+------------------------------+


Converts an array to a table

++++++++++++++++++++
Comparators and keys
++++++++++++++++++++

  *  :ref:`less (a:auto const;b:auto const) : bool <function-_at_linq_c__c_less_C._C.>` 
  *  :ref:`less (a:tuple\<auto(TT)\> const;b:tuple\<auto(TT)\> const) : bool <function-_at_linq_c__c_less_C0_ls_Y_ls_TT_gr_._gr_U_C0_ls_Y_ls_TT_gr_._gr_U>` 
  *  :ref:`less (a:tuple\<auto(TT);auto(UU)\> const;b:tuple\<auto(TT);auto(UU)\> const) : bool <function-_at_linq_c__c_less_C0_ls_Y_ls_TT_gr_.;Y_ls_UU_gr_._gr_U_C0_ls_Y_ls_TT_gr_.;Y_ls_UU_gr_._gr_U>` 
  *  :ref:`less (a:tuple\<auto(TT);auto(UU);auto(VV)\> const;b:tuple\<auto(TT);auto(UU);auto(VV)\> const) : bool <function-_at_linq_c__c_less_C0_ls_Y_ls_TT_gr_.;Y_ls_UU_gr_.;Y_ls_VV_gr_._gr_U_C0_ls_Y_ls_TT_gr_.;Y_ls_UU_gr_.;Y_ls_VV_gr_._gr_U>` 
  *  :ref:`less (a:tuple\<auto(TT);auto(UU);auto(VV);auto(WW)\> const;b:tuple\<auto(TT);auto(UU);auto(VV);auto(WW)\> const) : bool <function-_at_linq_c__c_less_C0_ls_Y_ls_TT_gr_.;Y_ls_UU_gr_.;Y_ls_VV_gr_.;Y_ls_WW_gr_._gr_U_C0_ls_Y_ls_TT_gr_.;Y_ls_UU_gr_.;Y_ls_VV_gr_.;Y_ls_WW_gr_._gr_U>` 
  *  :ref:`sequence_equal (first:iterator\<auto(TT)\> -const;second:iterator\<auto(TT)\> -const) : bool <function-_at_linq_c__c_sequence_equal_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G>` 
  *  :ref:`sequence_equal (first:array\<auto(TT)\> const;second:array\<auto(TT)\> const) : bool <function-_at_linq_c__c_sequence_equal_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A>` 
  *  :ref:`sequence_equal_by (first:iterator\<auto(TT)\> -const;second:iterator\<auto(TT)\> -const;key:block\<(arg:TT const -&):auto\> const) : bool <function-_at_linq_c__c_sequence_equal_by_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 
  *  :ref:`sequence_equal_by (first:array\<auto(TT)\> const;second:array\<auto(TT)\> const;key:block\<(arg:TT const -&):auto\> const) : bool <function-_at_linq_c__c_sequence_equal_by_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_>` 

.. _function-_at_linq_c__c_less_C._C.:

.. das:function:: less(a: auto const; b: auto const)

less returns bool

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto const   +
+--------+-------------+
+b       +auto const   +
+--------+-------------+


Compares two tuples, returns true if first is less than second

.. _function-_at_linq_c__c_less_C0_ls_Y_ls_TT_gr_._gr_U_C0_ls_Y_ls_TT_gr_._gr_U:

.. das:function:: less(a: tuple<auto(TT)> const; b: tuple<auto(TT)> const)

less returns bool

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +tuple<auto(TT)> const+
+--------+---------------------+
+b       +tuple<auto(TT)> const+
+--------+---------------------+


Compares two tuples, returns true if first is less than second

.. _function-_at_linq_c__c_less_C0_ls_Y_ls_TT_gr_.;Y_ls_UU_gr_._gr_U_C0_ls_Y_ls_TT_gr_.;Y_ls_UU_gr_._gr_U:

.. das:function:: less(a: tuple<auto(TT);auto(UU)> const; b: tuple<auto(TT);auto(UU)> const)

less returns bool

+--------+------------------------------+
+argument+argument type                 +
+========+==============================+
+a       +tuple<auto(TT);auto(UU)> const+
+--------+------------------------------+
+b       +tuple<auto(TT);auto(UU)> const+
+--------+------------------------------+


Compares two tuples, returns true if first is less than second

.. _function-_at_linq_c__c_less_C0_ls_Y_ls_TT_gr_.;Y_ls_UU_gr_.;Y_ls_VV_gr_._gr_U_C0_ls_Y_ls_TT_gr_.;Y_ls_UU_gr_.;Y_ls_VV_gr_._gr_U:

.. das:function:: less(a: tuple<auto(TT);auto(UU);auto(VV)> const; b: tuple<auto(TT);auto(UU);auto(VV)> const)

less returns bool

+--------+---------------------------------------+
+argument+argument type                          +
+========+=======================================+
+a       +tuple<auto(TT);auto(UU);auto(VV)> const+
+--------+---------------------------------------+
+b       +tuple<auto(TT);auto(UU);auto(VV)> const+
+--------+---------------------------------------+


Compares two tuples, returns true if first is less than second

.. _function-_at_linq_c__c_less_C0_ls_Y_ls_TT_gr_.;Y_ls_UU_gr_.;Y_ls_VV_gr_.;Y_ls_WW_gr_._gr_U_C0_ls_Y_ls_TT_gr_.;Y_ls_UU_gr_.;Y_ls_VV_gr_.;Y_ls_WW_gr_._gr_U:

.. das:function:: less(a: tuple<auto(TT);auto(UU);auto(VV);auto(WW)> const; b: tuple<auto(TT);auto(UU);auto(VV);auto(WW)> const)

less returns bool

+--------+------------------------------------------------+
+argument+argument type                                   +
+========+================================================+
+a       +tuple<auto(TT);auto(UU);auto(VV);auto(WW)> const+
+--------+------------------------------------------------+
+b       +tuple<auto(TT);auto(UU);auto(VV);auto(WW)> const+
+--------+------------------------------------------------+


Compares two tuples, returns true if first is less than second

.. _function-_at_linq_c__c_sequence_equal_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G:

.. das:function:: sequence_equal(first: iterator<auto(TT)>; second: iterator<auto(TT)>)

sequence_equal returns bool

+--------+------------------+
+argument+argument type     +
+========+==================+
+first   +iterator<auto(TT)>+
+--------+------------------+
+second  +iterator<auto(TT)>+
+--------+------------------+


Checks if two arrays are equal

.. _function-_at_linq_c__c_sequence_equal_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: sequence_equal(first: array<auto(TT)> const; second: array<auto(TT)> const)

sequence_equal returns bool

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+first   +array<auto(TT)> const+
+--------+---------------------+
+second  +array<auto(TT)> const+
+--------+---------------------+


Checks if two arrays are equal

.. _function-_at_linq_c__c_sequence_equal_by_1_ls_Y_ls_TT_gr_._gr_G_1_ls_Y_ls_TT_gr_._gr_G_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: sequence_equal_by(first: iterator<auto(TT)>; second: iterator<auto(TT)>; key: block<(arg:TT const):auto> const)

sequence_equal_by returns bool

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+first   +iterator<auto(TT)>              +
+--------+--------------------------------+
+second  +iterator<auto(TT)>              +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Checks if two arrays are equal by key

.. _function-_at_linq_c__c_sequence_equal_by_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A_CN_ls_arg_gr_0_ls_CY_ls_TT_gr_L_gr_1_ls_._gr__builtin_:

.. das:function:: sequence_equal_by(first: array<auto(TT)> const; second: array<auto(TT)> const; key: block<(arg:TT const):auto> const)

sequence_equal_by returns bool

+--------+--------------------------------+
+argument+argument type                   +
+========+================================+
+first   +array<auto(TT)> const           +
+--------+--------------------------------+
+second  +array<auto(TT)> const           +
+--------+--------------------------------+
+key     +block<(arg:TT const):auto> const+
+--------+--------------------------------+


Checks if two arrays are equal by key


