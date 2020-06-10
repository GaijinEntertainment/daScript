
.. _stdlib_functional:

==============================
Functional programming library
==============================

++++++++
Generics
++++++++

.. das:function:: all(it:auto const)

all returns auto



function arguments are

+--+----------+
+it+auto const+
+--+----------+



to be documented


.. das:function:: any(it:auto const)

any returns auto



function arguments are

+--+----------+
+it+auto const+
+--+----------+



to be documented


.. das:function:: count(start:int; step:int)

count returns iterator<int>



function arguments are

+-----+---+
+start+int+
+-----+---+
+step +int+
+-----+---+



to be documented


.. das:function:: cycle(src:iterator<auto(TT)>)

cycle returns auto



function arguments are

+---+------------------+
+src+iterator<auto(TT)>+
+---+------------------+



to be documented


.. das:function:: echo(x:auto; extra:string const)

echo returns auto



function arguments are

+-----+------------+
+x    +auto        +
+-----+------------+
+extra+string const+
+-----+------------+



to be documented


.. das:function:: filter(src:iterator<auto(TT)>; blk:function<(what:TT const -&):bool> const)

filter returns auto



function arguments are

+---+---------------------------------------+
+src+iterator<auto(TT)>                     +
+---+---------------------------------------+
+blk+function<(what:TT const -&):bool> const+
+---+---------------------------------------+



to be documented


.. das:function:: filter(src:iterator<auto(TT)>; blk:lambda<(what:TT const -&):bool> const)

filter returns auto



function arguments are

+---+-------------------------------------+
+src+iterator<auto(TT)>                   +
+---+-------------------------------------+
+blk+lambda<(what:TT const -&):bool> const+
+---+-------------------------------------+



to be documented


.. das:function:: is_equal(a:auto const; b:auto const)

is_equal returns auto



function arguments are

+-+----------+
+a+auto const+
+-+----------+
+b+auto const+
+-+----------+



to be documented


.. das:function:: is_not_equal(a:auto const; b:auto const)

is_not_equal returns auto



function arguments are

+-+----------+
+a+auto const+
+-+----------+
+b+auto const+
+-+----------+



to be documented


.. das:function:: islice(src:iterator<auto(TT)>; start:int const; stop:int const)

islice returns auto



function arguments are

+-----+------------------+
+src  +iterator<auto(TT)>+
+-----+------------------+
+start+int const         +
+-----+------------------+
+stop +int const         +
+-----+------------------+



to be documented


.. das:function:: map(src:iterator<auto(TT)>; blk:function<(what:TT const -&):auto(QQ)> const)

map returns auto



function arguments are

+---+-------------------------------------------+
+src+iterator<auto(TT)>                         +
+---+-------------------------------------------+
+blk+function<(what:TT const -&):auto(QQ)> const+
+---+-------------------------------------------+



to be documented


.. das:function:: map(src:iterator<auto(TT)>; blk:lambda<(what:TT const -&):auto(QQ)> const)

map returns auto



function arguments are

+---+-----------------------------------------+
+src+iterator<auto(TT)>                       +
+---+-----------------------------------------+
+blk+lambda<(what:TT const -&):auto(QQ)> const+
+---+-----------------------------------------+



to be documented


.. das:function:: not(x:auto const)

not returns auto



function arguments are

+-+----------+
+x+auto const+
+-+----------+



to be documented


.. das:function:: reduce(it:iterator<auto(TT)> const; blk:block<(left:TT const -&;right:TT const -&):TT const -&> const)

reduce returns auto



function arguments are

+---+-------------------------------------------------------------+
+it +iterator<auto(TT)> const                                     +
+---+-------------------------------------------------------------+
+blk+block<(left:TT const -&;right:TT const -&):TT const -&> const+
+---+-------------------------------------------------------------+



to be documented


.. das:function:: reduce(it:iterator<auto(TT)> const; blk:function<(left:TT const -&;right:TT const -&):TT const -&> const)

reduce returns auto



function arguments are

+---+----------------------------------------------------------------+
+it +iterator<auto(TT)> const                                        +
+---+----------------------------------------------------------------+
+blk+function<(left:TT const -&;right:TT const -&):TT const -&> const+
+---+----------------------------------------------------------------+



to be documented


.. das:function:: reduce(it:iterator<auto(TT)> const; blk:lambda<(left:TT const -&;right:TT const -&):TT const -&> const)

reduce returns auto



function arguments are

+---+--------------------------------------------------------------+
+it +iterator<auto(TT)> const                                      +
+---+--------------------------------------------------------------+
+blk+lambda<(left:TT const -&;right:TT const -&):TT const -&> const+
+---+--------------------------------------------------------------+



to be documented


.. das:function:: repeat(value:auto(TT) const; count:int)

repeat returns auto



function arguments are

+-----+--------------+
+value+auto(TT) const+
+-----+--------------+
+count+int           +
+-----+--------------+



to be documented


.. das:function:: sum(it:iterator<auto(TT)> const)

sum returns auto



function arguments are

+--+------------------------+
+it+iterator<auto(TT)> const+
+--+------------------------+



to be documented



