
.. _stdlib_functional:

==============================
Functional programming library
==============================

++++++++
Generics
++++++++

.. das:function:: all(it)

all returns ::

 auto



function arguments are

+--+----------+
+it+auto const+
+--+----------+



to be documented


.. das:function:: any(it)

any returns ::

 auto



function arguments are

+--+----------+
+it+auto const+
+--+----------+



to be documented


.. das:function:: count(start;step)

count returns ::

 iterator<int>



function arguments are

+-----+----------+
+start+int -const+
+-----+----------+
+step +int -const+
+-----+----------+



to be documented


.. das:function:: cycle(src)

cycle returns ::

 auto



function arguments are

+---+-------------------------+
+src+iterator<auto(TT)> -const+
+---+-------------------------+



to be documented


.. das:function:: echo(x;extra)

echo returns ::

 auto



function arguments are

+-----+------------+
+x    +auto -const +
+-----+------------+
+extra+string const+
+-----+------------+



to be documented


.. das:function:: filter(src;blk)

filter returns ::

 auto



function arguments are

+---+---------------------------------------+
+src+iterator<auto(TT)> -const              +
+---+---------------------------------------+
+blk+function<(what:TT const -&):bool> const+
+---+---------------------------------------+



to be documented


.. das:function:: filter(src;blk)

filter returns ::

 auto



function arguments are

+---+-------------------------------------+
+src+iterator<auto(TT)> -const            +
+---+-------------------------------------+
+blk+lambda<(what:TT const -&):bool> const+
+---+-------------------------------------+



to be documented


.. das:function:: is_equal(a;b)

is_equal returns ::

 auto



function arguments are

+-+----------+
+a+auto const+
+-+----------+
+b+auto const+
+-+----------+



to be documented


.. das:function:: is_not_equal(a;b)

is_not_equal returns ::

 auto



function arguments are

+-+----------+
+a+auto const+
+-+----------+
+b+auto const+
+-+----------+



to be documented


.. das:function:: islice(src;start;stop)

islice returns ::

 auto



function arguments are

+-----+-------------------------+
+src  +iterator<auto(TT)> -const+
+-----+-------------------------+
+start+int const                +
+-----+-------------------------+
+stop +int const                +
+-----+-------------------------+



to be documented


.. das:function:: map(src;blk)

map returns ::

 auto



function arguments are

+---+-------------------------------------------+
+src+iterator<auto(TT)> -const                  +
+---+-------------------------------------------+
+blk+function<(what:TT const -&):auto(QQ)> const+
+---+-------------------------------------------+



to be documented


.. das:function:: map(src;blk)

map returns ::

 auto



function arguments are

+---+-----------------------------------------+
+src+iterator<auto(TT)> -const                +
+---+-----------------------------------------+
+blk+lambda<(what:TT const -&):auto(QQ)> const+
+---+-----------------------------------------+



to be documented


.. das:function:: not(x)

not returns ::

 auto



function arguments are

+-+----------+
+x+auto const+
+-+----------+



to be documented


.. das:function:: reduce(it;blk)

reduce returns ::

 auto



function arguments are

+---+-------------------------------------------------------------+
+it +iterator<auto(TT)> const                                     +
+---+-------------------------------------------------------------+
+blk+block<(left:TT const -&;right:TT const -&):TT const -&> const+
+---+-------------------------------------------------------------+



to be documented


.. das:function:: reduce(it;blk)

reduce returns ::

 auto



function arguments are

+---+----------------------------------------------------------------+
+it +iterator<auto(TT)> const                                        +
+---+----------------------------------------------------------------+
+blk+function<(left:TT const -&;right:TT const -&):TT const -&> const+
+---+----------------------------------------------------------------+



to be documented


.. das:function:: reduce(it;blk)

reduce returns ::

 auto



function arguments are

+---+--------------------------------------------------------------+
+it +iterator<auto(TT)> const                                      +
+---+--------------------------------------------------------------+
+blk+lambda<(left:TT const -&;right:TT const -&):TT const -&> const+
+---+--------------------------------------------------------------+



to be documented


.. das:function:: repeat(value;count)

repeat returns ::

 auto



function arguments are

+-----+--------------+
+value+auto(TT) const+
+-----+--------------+
+count+int -const    +
+-----+--------------+



to be documented


.. das:function:: sum(it)

sum returns ::

 auto



function arguments are

+--+------------------------+
+it+iterator<auto(TT)> const+
+--+------------------------+



to be documented



