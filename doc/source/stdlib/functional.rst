
.. _stdlib_functional:

==============================
Functional programming library
==============================

.. include:: detail/functional.rst

|module-functional|

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_functional_c__c_all__hh_auto_hh_const:

.. das:function:: all(it: auto const)

all returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+it      +auto const   +
+--------+-------------+


|function-functional-all|

.. _function-_at_functional_c__c_any__hh_auto_hh_const:

.. das:function:: any(it: auto const)

any returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+it      +auto const   +
+--------+-------------+


|function-functional-any|

.. _function-_at_functional_c__c_count_int_int:

.. das:function:: count(start: int; step: int)

count returns iterator<int>

+--------+-------------+
+argument+argument type+
+========+=============+
+start   +int          +
+--------+-------------+
+step    +int          +
+--------+-------------+


|function-functional-count|

.. _function-_at_functional_c__c_cycle__hh_iterator_hh__hh_auto:

.. das:function:: cycle(src: iterator<auto(TT)>)

cycle returns auto

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+


|function-functional-cycle|

.. _function-_at_functional_c__c_echo__hh_auto_string_hh_const:

.. das:function:: echo(x: auto; extra: string const)

echo returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +auto         +
+--------+-------------+
+extra   +string const +
+--------+-------------+


|function-functional-echo|

.. _function-_at_functional_c__c_filter__hh_iterator_hh__hh_auto__hh_function_hh__hh_alias_hh_TT_hh_const_hh__rq_what_hh__c_bool_hh_const:

.. das:function:: filter(src: iterator<auto(TT)>; blk: function<(what:TT const -&):bool> const)

filter returns auto

+--------+------------------------------------+
+argument+argument type                       +
+========+====================================+
+src     +iterator<auto(TT)>                  +
+--------+------------------------------------+
+blk     +function<(what:TT const):bool> const+
+--------+------------------------------------+


|function-functional-filter|

.. _function-_at_functional_c__c_filter__hh_iterator_hh__hh_auto__hh_lambda_hh__hh_alias_hh_TT_hh_const_hh__rq_what_hh__c_bool_hh_const:

.. das:function:: filter(src: iterator<auto(TT)>; blk: lambda<(what:TT const -&):bool> const)

filter returns auto

+--------+----------------------------------+
+argument+argument type                     +
+========+==================================+
+src     +iterator<auto(TT)>                +
+--------+----------------------------------+
+blk     +lambda<(what:TT const):bool> const+
+--------+----------------------------------+


|function-functional-filter|

.. _function-_at_functional_c__c_is_equal__hh_auto_hh_const__hh_auto_hh_const:

.. das:function:: is_equal(a: auto const; b: auto const)

is_equal returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto const   +
+--------+-------------+
+b       +auto const   +
+--------+-------------+


|function-functional-is_equal|

.. _function-_at_functional_c__c_is_not_equal__hh_auto_hh_const__hh_auto_hh_const:

.. das:function:: is_not_equal(a: auto const; b: auto const)

is_not_equal returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+a       +auto const   +
+--------+-------------+
+b       +auto const   +
+--------+-------------+


|function-functional-is_not_equal|

.. _function-_at_functional_c__c_islice__hh_iterator_hh__hh_auto_int_hh_const_int_hh_const:

.. das:function:: islice(src: iterator<auto(TT)>; start: int const; stop: int const)

islice returns auto

+--------+------------------+
+argument+argument type     +
+========+==================+
+src     +iterator<auto(TT)>+
+--------+------------------+
+start   +int const         +
+--------+------------------+
+stop    +int const         +
+--------+------------------+


|function-functional-islice|

.. _function-_at_functional_c__c_map__hh_iterator_hh__hh_auto__hh_function_hh__hh_alias_hh_TT_hh_const_hh__rq_what_hh__c__hh_auto_hh_const:

.. das:function:: map(src: iterator<auto(TT)>; blk: function<(what:TT const -&):auto(QQ)> const)

map returns auto

+--------+----------------------------------------+
+argument+argument type                           +
+========+========================================+
+src     +iterator<auto(TT)>                      +
+--------+----------------------------------------+
+blk     +function<(what:TT const):auto(QQ)> const+
+--------+----------------------------------------+


|function-functional-map|

.. _function-_at_functional_c__c_map__hh_iterator_hh__hh_auto__hh_lambda_hh__hh_alias_hh_TT_hh_const_hh__rq_what_hh__c__hh_auto_hh_const:

.. das:function:: map(src: iterator<auto(TT)>; blk: lambda<(what:TT const -&):auto(QQ)> const)

map returns auto

+--------+--------------------------------------+
+argument+argument type                         +
+========+======================================+
+src     +iterator<auto(TT)>                    +
+--------+--------------------------------------+
+blk     +lambda<(what:TT const):auto(QQ)> const+
+--------+--------------------------------------+


|function-functional-map|

.. _function-_at_functional_c__c_not__hh_auto_hh_const:

.. das:function:: not(x: auto const)

not returns auto

+--------+-------------+
+argument+argument type+
+========+=============+
+x       +auto const   +
+--------+-------------+


|function-functional-not|

.. _function-_at_functional_c__c_reduce__hh_iterator_hh__hh_auto_hh_const__hh_block_hh__hh_alias_hh_TT_hh_const_hh__hh_alias_hh_TT_hh_const_hh__rq_left_hh__rq_right_hh__c__hh_alias_hh_TT_hh_const_hh_const:

.. das:function:: reduce(it: iterator<auto(TT)> const; blk: block<(left:TT const -&;right:TT const -&):TT const -&> const)

reduce returns auto

+--------+----------------------------------------------------+
+argument+argument type                                       +
+========+====================================================+
+it      +iterator<auto(TT)> const                            +
+--------+----------------------------------------------------+
+blk     +block<(left:TT const;right:TT const):TT const> const+
+--------+----------------------------------------------------+


|function-functional-reduce|

.. _function-_at_functional_c__c_reduce__hh_iterator_hh__hh_auto_hh_const__hh_function_hh__hh_alias_hh_TT_hh_const_hh__hh_alias_hh_TT_hh_const_hh__rq_left_hh__rq_right_hh__c__hh_alias_hh_TT_hh_const_hh_const:

.. das:function:: reduce(it: iterator<auto(TT)> const; blk: function<(left:TT const -&;right:TT const -&):TT const -&> const)

reduce returns auto

+--------+-------------------------------------------------------+
+argument+argument type                                          +
+========+=======================================================+
+it      +iterator<auto(TT)> const                               +
+--------+-------------------------------------------------------+
+blk     +function<(left:TT const;right:TT const):TT const> const+
+--------+-------------------------------------------------------+


|function-functional-reduce|

.. _function-_at_functional_c__c_reduce__hh_iterator_hh__hh_auto_hh_const__hh_lambda_hh__hh_alias_hh_TT_hh_const_hh__hh_alias_hh_TT_hh_const_hh__rq_left_hh__rq_right_hh__c__hh_alias_hh_TT_hh_const_hh_const:

.. das:function:: reduce(it: iterator<auto(TT)> const; blk: lambda<(left:TT const -&;right:TT const -&):TT const -&> const)

reduce returns auto

+--------+-----------------------------------------------------+
+argument+argument type                                        +
+========+=====================================================+
+it      +iterator<auto(TT)> const                             +
+--------+-----------------------------------------------------+
+blk     +lambda<(left:TT const;right:TT const):TT const> const+
+--------+-----------------------------------------------------+


|function-functional-reduce|

.. _function-_at_functional_c__c_repeat__hh_auto_hh_const_int:

.. das:function:: repeat(value: auto(TT) const; count: int)

repeat returns auto

+--------+--------------+
+argument+argument type +
+========+==============+
+value   +auto(TT) const+
+--------+--------------+
+count   +int           +
+--------+--------------+


|function-functional-repeat|

.. _function-_at_functional_c__c_sum__hh_iterator_hh__hh_auto_hh_const:

.. das:function:: sum(it: iterator<auto(TT)> const)

sum returns auto

+--------+------------------------+
+argument+argument type           +
+========+========================+
+it      +iterator<auto(TT)> const+
+--------+------------------------+


|function-functional-sum|


