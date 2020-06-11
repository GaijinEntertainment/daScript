
.. _stdlib_functional:

==============================
Functional programming library
==============================

+++++++++++++
Uncategorized
+++++++++++++

.. _function-_at_functional::all__hh_auto_hh_const:

.. das:function:: all(it: auto const)

all returns auto

arguments are

+--+----------+
+it+auto const+
+--+----------+


function|functional|all to be documented

.. _function-_at_functional::any__hh_auto_hh_const:

.. das:function:: any(it: auto const)

any returns auto

arguments are

+--+----------+
+it+auto const+
+--+----------+


function|functional|any to be documented

.. _function-_at_functional::count_int_int:

.. das:function:: count(start: int; step: int)

count returns iterator<int>

arguments are

+-----+---+
+start+int+
+-----+---+
+step +int+
+-----+---+


function|functional|count to be documented

.. _function-_at_functional::cycle__hh_iterator_hh__hh_auto:

.. das:function:: cycle(src: iterator<auto(TT)>)

cycle returns auto

arguments are

+---+------------------+
+src+iterator<auto(TT)>+
+---+------------------+


function|functional|cycle to be documented

.. _function-_at_functional::echo__hh_auto_string_hh_const:

.. das:function:: echo(x: auto; extra: string const)

echo returns auto

arguments are

+-----+------------+
+x    +auto        +
+-----+------------+
+extra+string const+
+-----+------------+


function|functional|echo to be documented

.. _function-_at_functional::filter__hh_iterator_hh__hh_auto__hh_function_hh__hh_alias_hh_TT_hh_const_hh__rq_what_hh_:bool_hh_const:

.. das:function:: filter(src: iterator<auto(TT)>; blk: function<(what:TT const -&):bool> const)

filter returns auto

arguments are

+---+------------------------------------+
+src+iterator<auto(TT)>                  +
+---+------------------------------------+
+blk+function<(what:TT const):bool> const+
+---+------------------------------------+


function|functional|filter to be documented

.. _function-_at_functional::filter__hh_iterator_hh__hh_auto__hh_lambda_hh__hh_alias_hh_TT_hh_const_hh__rq_what_hh_:bool_hh_const:

.. das:function:: filter(src: iterator<auto(TT)>; blk: lambda<(what:TT const -&):bool> const)

filter returns auto

arguments are

+---+----------------------------------+
+src+iterator<auto(TT)>                +
+---+----------------------------------+
+blk+lambda<(what:TT const):bool> const+
+---+----------------------------------+


function|functional|filter to be documented

.. _function-_at_functional::is_equal__hh_auto_hh_const__hh_auto_hh_const:

.. das:function:: is_equal(a: auto const; b: auto const)

is_equal returns auto

arguments are

+-+----------+
+a+auto const+
+-+----------+
+b+auto const+
+-+----------+


function|functional|is_equal to be documented

.. _function-_at_functional::is_not_equal__hh_auto_hh_const__hh_auto_hh_const:

.. das:function:: is_not_equal(a: auto const; b: auto const)

is_not_equal returns auto

arguments are

+-+----------+
+a+auto const+
+-+----------+
+b+auto const+
+-+----------+


function|functional|is_not_equal to be documented

.. _function-_at_functional::islice__hh_iterator_hh__hh_auto_int_hh_const_int_hh_const:

.. das:function:: islice(src: iterator<auto(TT)>; start: int const; stop: int const)

islice returns auto

arguments are

+-----+------------------+
+src  +iterator<auto(TT)>+
+-----+------------------+
+start+int const         +
+-----+------------------+
+stop +int const         +
+-----+------------------+


function|functional|islice to be documented

.. _function-_at_functional::map__hh_iterator_hh__hh_auto__hh_function_hh__hh_alias_hh_TT_hh_const_hh__rq_what_hh_:_hh_auto_hh_const:

.. das:function:: map(src: iterator<auto(TT)>; blk: function<(what:TT const -&):auto(QQ)> const)

map returns auto

arguments are

+---+----------------------------------------+
+src+iterator<auto(TT)>                      +
+---+----------------------------------------+
+blk+function<(what:TT const):auto(QQ)> const+
+---+----------------------------------------+


function|functional|map to be documented

.. _function-_at_functional::map__hh_iterator_hh__hh_auto__hh_lambda_hh__hh_alias_hh_TT_hh_const_hh__rq_what_hh_:_hh_auto_hh_const:

.. das:function:: map(src: iterator<auto(TT)>; blk: lambda<(what:TT const -&):auto(QQ)> const)

map returns auto

arguments are

+---+--------------------------------------+
+src+iterator<auto(TT)>                    +
+---+--------------------------------------+
+blk+lambda<(what:TT const):auto(QQ)> const+
+---+--------------------------------------+


function|functional|map to be documented

.. _function-_at_functional::not__hh_auto_hh_const:

.. das:function:: not(x: auto const)

not returns auto

arguments are

+-+----------+
+x+auto const+
+-+----------+


function|functional|not to be documented

.. _function-_at_functional::reduce__hh_iterator_hh__hh_auto_hh_const__hh_block_hh__hh_alias_hh_TT_hh_const_hh__hh_alias_hh_TT_hh_const_hh__rq_left_hh__rq_right_hh_:_hh_alias_hh_TT_hh_const_hh_const:

.. das:function:: reduce(it: iterator<auto(TT)> const; blk: block<(left:TT const -&;right:TT const -&):TT const -&> const)

reduce returns auto

arguments are

+---+----------------------------------------------------+
+it +iterator<auto(TT)> const                            +
+---+----------------------------------------------------+
+blk+block<(left:TT const;right:TT const):TT const> const+
+---+----------------------------------------------------+


function|functional|reduce to be documented

.. _function-_at_functional::reduce__hh_iterator_hh__hh_auto_hh_const__hh_function_hh__hh_alias_hh_TT_hh_const_hh__hh_alias_hh_TT_hh_const_hh__rq_left_hh__rq_right_hh_:_hh_alias_hh_TT_hh_const_hh_const:

.. das:function:: reduce(it: iterator<auto(TT)> const; blk: function<(left:TT const -&;right:TT const -&):TT const -&> const)

reduce returns auto

arguments are

+---+-------------------------------------------------------+
+it +iterator<auto(TT)> const                               +
+---+-------------------------------------------------------+
+blk+function<(left:TT const;right:TT const):TT const> const+
+---+-------------------------------------------------------+


function|functional|reduce to be documented

.. _function-_at_functional::reduce__hh_iterator_hh__hh_auto_hh_const__hh_lambda_hh__hh_alias_hh_TT_hh_const_hh__hh_alias_hh_TT_hh_const_hh__rq_left_hh__rq_right_hh_:_hh_alias_hh_TT_hh_const_hh_const:

.. das:function:: reduce(it: iterator<auto(TT)> const; blk: lambda<(left:TT const -&;right:TT const -&):TT const -&> const)

reduce returns auto

arguments are

+---+-----------------------------------------------------+
+it +iterator<auto(TT)> const                             +
+---+-----------------------------------------------------+
+blk+lambda<(left:TT const;right:TT const):TT const> const+
+---+-----------------------------------------------------+


function|functional|reduce to be documented

.. _function-_at_functional::repeat__hh_auto_hh_const_int:

.. das:function:: repeat(value: auto(TT) const; count: int)

repeat returns auto

arguments are

+-----+--------------+
+value+auto(TT) const+
+-----+--------------+
+count+int           +
+-----+--------------+


function|functional|repeat to be documented

.. _function-_at_functional::sum__hh_iterator_hh__hh_auto_hh_const:

.. das:function:: sum(it: iterator<auto(TT)> const)

sum returns auto

arguments are

+--+------------------------+
+it+iterator<auto(TT)> const+
+--+------------------------+


function|functional|sum to be documented


