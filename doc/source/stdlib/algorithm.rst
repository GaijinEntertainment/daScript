
.. _stdlib_algorithm:

=======================
Miscelanious algorithms
=======================

.. include:: detail/algorithm.rst

|module-algorithm|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_algorithm_c__c_unique_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: unique(a: array<auto(TT)>)

unique returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+a       +array<auto(TT)>+
+--------+---------------+


|function-algorithm-unique|

.. _function-_at_algorithm_c__c_sort_unique_1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: sort_unique(a: array<auto(TT)>)

sort_unique returns auto

+--------+---------------+
+argument+argument type  +
+========+===============+
+a       +array<auto(TT)>+
+--------+---------------+


|function-algorithm-sort_unique|

.. _function-_at_algorithm_c__c_reverse_1_ls_._gr_A:

.. das:function:: reverse(a: array<auto>)

reverse returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +array<auto>  +
+--------+-------------+


|function-algorithm-reverse|

.. _function-_at_algorithm_c__c_combine_C1_ls_Y_ls_TT_gr_._gr_A_C1_ls_Y_ls_TT_gr_._gr_A:

.. das:function:: combine(a: array<auto(TT)> const; b: array<auto(TT)> const)

combine returns auto

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+
+b       +array<auto(TT)> const+
+--------+---------------------+


|function-algorithm-combine|

.. _function-_at_algorithm_c__c_lower_bound_C1_ls_Y_ls_TT_gr_._gr_A_Ci_Ci_CY_ls_TT_gr_L:

.. das:function:: lower_bound(a: array<auto(TT)> const; f: int const; l: int const; val: TT const)

lower_bound returns auto

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+
+f       +int const            +
+--------+---------------------+
+l       +int const            +
+--------+---------------------+
+val     +TT const             +
+--------+---------------------+


|function-algorithm-lower_bound|

.. _function-_at_algorithm_c__c_lower_bound_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L:

.. das:function:: lower_bound(a: array<auto(TT)> const; val: TT const)

lower_bound returns auto

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+
+val     +TT const             +
+--------+---------------------+


|function-algorithm-lower_bound|

.. _function-_at_algorithm_c__c_lower_bound_C1_ls_Y_ls_TT_gr_._gr_A_Ci_Ci_CY_ls_TT_gr_L_CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: lower_bound(a: array<auto(TT)> const; f: int const; l: int const; value: TT const; less: block<(a:TT const -&;b:TT const -&):bool> const)

lower_bound returns auto

+--------+-----------------------------------------+
+argument+argument type                            +
+========+=========================================+
+a       +array<auto(TT)> const                    +
+--------+-----------------------------------------+
+f       +int const                                +
+--------+-----------------------------------------+
+l       +int const                                +
+--------+-----------------------------------------+
+value   +TT const                                 +
+--------+-----------------------------------------+
+less    +block<(a:TT const;b:TT const):bool> const+
+--------+-----------------------------------------+


|function-algorithm-lower_bound|

.. _function-_at_algorithm_c__c_lower_bound_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L_CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: lower_bound(a: array<auto(TT)> const; value: TT const; less: block<(a:TT const -&;b:TT const -&):bool> const)

lower_bound returns auto

+--------+-----------------------------------------+
+argument+argument type                            +
+========+=========================================+
+a       +array<auto(TT)> const                    +
+--------+-----------------------------------------+
+value   +TT const                                 +
+--------+-----------------------------------------+
+less    +block<(a:TT const;b:TT const):bool> const+
+--------+-----------------------------------------+


|function-algorithm-lower_bound|

.. _function-_at_algorithm_c__c_binary_search_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L:

.. das:function:: binary_search(a: array<auto(TT)> const; val: TT const)

binary_search returns auto

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+
+val     +TT const             +
+--------+---------------------+


|function-algorithm-binary_search|

.. _function-_at_algorithm_c__c_binary_search_C1_ls_Y_ls_TT_gr_._gr_A_Ci_Ci_CY_ls_TT_gr_L:

.. das:function:: binary_search(a: array<auto(TT)> const; f: int const; last: int const; val: TT const)

binary_search returns auto

+--------+---------------------+
+argument+argument type        +
+========+=====================+
+a       +array<auto(TT)> const+
+--------+---------------------+
+f       +int const            +
+--------+---------------------+
+last    +int const            +
+--------+---------------------+
+val     +TT const             +
+--------+---------------------+


|function-algorithm-binary_search|

.. _function-_at_algorithm_c__c_binary_search_C1_ls_Y_ls_TT_gr_._gr_A_CY_ls_TT_gr_L_CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: binary_search(a: array<auto(TT)> const; val: TT const; less: block<(a:TT const -&;b:TT const -&):bool> const)

binary_search returns auto

+--------+-----------------------------------------+
+argument+argument type                            +
+========+=========================================+
+a       +array<auto(TT)> const                    +
+--------+-----------------------------------------+
+val     +TT const                                 +
+--------+-----------------------------------------+
+less    +block<(a:TT const;b:TT const):bool> const+
+--------+-----------------------------------------+


|function-algorithm-binary_search|

.. _function-_at_algorithm_c__c_binary_search_C1_ls_Y_ls_TT_gr_._gr_A_Ci_Ci_CY_ls_TT_gr_L_CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: binary_search(a: array<auto(TT)> const; f: int const; last: int const; val: TT const; less: block<(a:TT const -&;b:TT const -&):bool> const)

binary_search returns auto

+--------+-----------------------------------------+
+argument+argument type                            +
+========+=========================================+
+a       +array<auto(TT)> const                    +
+--------+-----------------------------------------+
+f       +int const                                +
+--------+-----------------------------------------+
+last    +int const                                +
+--------+-----------------------------------------+
+val     +TT const                                 +
+--------+-----------------------------------------+
+less    +block<(a:TT const;b:TT const):bool> const+
+--------+-----------------------------------------+


|function-algorithm-binary_search|

.. _function-_at_algorithm_c__c_reverse_.:

.. das:function:: reverse(a: auto)

reverse returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto         +
+--------+-------------+


|function-algorithm-reverse|

.. _function-_at_algorithm_c__c_combine_C._C.:

.. das:function:: combine(a: auto const; b: auto const)

combine returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto const   +
+--------+-------------+
+b       +auto const   +
+--------+-------------+


|function-algorithm-combine|

.. _function-_at_algorithm_c__c_lower_bound_C._Ci_Ci_C.:

.. das:function:: lower_bound(a: auto const; f: int const; l: int const; val: auto const)

lower_bound returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto const   +
+--------+-------------+
+f       +int const    +
+--------+-------------+
+l       +int const    +
+--------+-------------+
+val     +auto const   +
+--------+-------------+


|function-algorithm-lower_bound|

.. _function-_at_algorithm_c__c_lower_bound_C._C.:

.. das:function:: lower_bound(a: auto const; val: auto const)

lower_bound returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto const   +
+--------+-------------+
+val     +auto const   +
+--------+-------------+


|function-algorithm-lower_bound|

.. _function-_at_algorithm_c__c_lower_bound_C._Ci_Ci_CY_ls_TT_gr_._CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: lower_bound(a: auto const; f: int const; l: int const; val: auto(TT) const; less: block<(a:TT const -&;b:TT const -&):bool> const)

lower_bound returns auto

+--------+-----------------------------------------+
+argument+argument type                            +
+========+=========================================+
+a       +auto const                               +
+--------+-----------------------------------------+
+f       +int const                                +
+--------+-----------------------------------------+
+l       +int const                                +
+--------+-----------------------------------------+
+val     +auto(TT) const                           +
+--------+-----------------------------------------+
+less    +block<(a:TT const;b:TT const):bool> const+
+--------+-----------------------------------------+


|function-algorithm-lower_bound|

.. _function-_at_algorithm_c__c_lower_bound_C._CY_ls_TT_gr_._CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: lower_bound(a: auto const; val: auto(TT) const; less: block<(a:TT const -&;b:TT const -&):bool> const)

lower_bound returns auto

+--------+-----------------------------------------+
+argument+argument type                            +
+========+=========================================+
+a       +auto const                               +
+--------+-----------------------------------------+
+val     +auto(TT) const                           +
+--------+-----------------------------------------+
+less    +block<(a:TT const;b:TT const):bool> const+
+--------+-----------------------------------------+


|function-algorithm-lower_bound|

.. _function-_at_algorithm_c__c_binary_search_C._C.:

.. das:function:: binary_search(a: auto const; val: auto const)

binary_search returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto const   +
+--------+-------------+
+val     +auto const   +
+--------+-------------+


|function-algorithm-binary_search|

.. _function-_at_algorithm_c__c_binary_search_C._Ci_Ci_C.:

.. das:function:: binary_search(a: auto const; f: int const; last: int const; val: auto const)

binary_search returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto const   +
+--------+-------------+
+f       +int const    +
+--------+-------------+
+last    +int const    +
+--------+-------------+
+val     +auto const   +
+--------+-------------+


|function-algorithm-binary_search|

.. _function-_at_algorithm_c__c_binary_search_C._CY_ls_TT_gr_._CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: binary_search(a: auto const; val: auto(TT) const; less: block<(a:TT const -&;b:TT const -&):bool> const)

binary_search returns auto

+--------+-----------------------------------------+
+argument+argument type                            +
+========+=========================================+
+a       +auto const                               +
+--------+-----------------------------------------+
+val     +auto(TT) const                           +
+--------+-----------------------------------------+
+less    +block<(a:TT const;b:TT const):bool> const+
+--------+-----------------------------------------+


|function-algorithm-binary_search|

.. _function-_at_algorithm_c__c_binary_search_C._Ci_Ci_CY_ls_TT_gr_._CN_ls_a;b_gr_0_ls_CY_ls_TT_gr_L;CY_ls_TT_gr_L_gr_1_ls_b_gr__builtin_:

.. das:function:: binary_search(a: auto const; f: int const; last: int const; val: auto(TT) const; less: block<(a:TT const -&;b:TT const -&):bool> const)

binary_search returns auto

+--------+-----------------------------------------+
+argument+argument type                            +
+========+=========================================+
+a       +auto const                               +
+--------+-----------------------------------------+
+f       +int const                                +
+--------+-----------------------------------------+
+last    +int const                                +
+--------+-----------------------------------------+
+val     +auto(TT) const                           +
+--------+-----------------------------------------+
+less    +block<(a:TT const;b:TT const):bool> const+
+--------+-----------------------------------------+


|function-algorithm-binary_search|


