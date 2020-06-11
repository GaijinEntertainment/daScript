
.. _stdlib_functional:

==============================
Functional programming library
==============================

.. das:function:: all (it:auto const )  : auto

all returns auto

arguments are

+--+----------+
+it+auto const+
+--+----------+


function|functional|all to be documented

.. das:function:: any (it:auto const )  : auto

any returns auto

arguments are

+--+----------+
+it+auto const+
+--+----------+


function|functional|any to be documented

.. das:function:: count (start:int; step:int )  : iterator<int>

count returns iterator<int>

arguments are

+-----+---+
+start+int+
+-----+---+
+step +int+
+-----+---+


function|functional|count to be documented

.. das:function:: cycle (src:iterator<auto(TT)> )  : auto

cycle returns auto

arguments are

+---+------------------+
+src+iterator<auto(TT)>+
+---+------------------+


function|functional|cycle to be documented

.. das:function:: echo (x:auto; extra:string const )  : auto

echo returns auto

arguments are

+-----+------------+
+x    +auto        +
+-----+------------+
+extra+string const+
+-----+------------+


function|functional|echo to be documented

.. das:function:: filter (src:iterator<auto(TT)>; blk:function<(what:TT const -&):bool> const )  : auto

filter returns auto

arguments are

+---+------------------------------------+
+src+iterator<auto(TT)>                  +
+---+------------------------------------+
+blk+function<(what:TT const):bool> const+
+---+------------------------------------+


function|functional|filter to be documented

.. das:function:: filter (src:iterator<auto(TT)>; blk:lambda<(what:TT const -&):bool> const )  : auto

filter returns auto

arguments are

+---+----------------------------------+
+src+iterator<auto(TT)>                +
+---+----------------------------------+
+blk+lambda<(what:TT const):bool> const+
+---+----------------------------------+


function|functional|filter to be documented

.. das:function:: is_equal (a:auto const; b:auto const )  : auto

is_equal returns auto

arguments are

+-+----------+
+a+auto const+
+-+----------+
+b+auto const+
+-+----------+


function|functional|is_equal to be documented

.. das:function:: is_not_equal (a:auto const; b:auto const )  : auto

is_not_equal returns auto

arguments are

+-+----------+
+a+auto const+
+-+----------+
+b+auto const+
+-+----------+


function|functional|is_not_equal to be documented

.. das:function:: islice (src:iterator<auto(TT)>; start:int const; stop:int const )  : auto

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

.. das:function:: map (src:iterator<auto(TT)>; blk:function<(what:TT const -&):auto(QQ)> const )  : auto

map returns auto

arguments are

+---+----------------------------------------+
+src+iterator<auto(TT)>                      +
+---+----------------------------------------+
+blk+function<(what:TT const):auto(QQ)> const+
+---+----------------------------------------+


function|functional|map to be documented

.. das:function:: map (src:iterator<auto(TT)>; blk:lambda<(what:TT const -&):auto(QQ)> const )  : auto

map returns auto

arguments are

+---+--------------------------------------+
+src+iterator<auto(TT)>                    +
+---+--------------------------------------+
+blk+lambda<(what:TT const):auto(QQ)> const+
+---+--------------------------------------+


function|functional|map to be documented

.. das:function:: not (x:auto const )  : auto

not returns auto

arguments are

+-+----------+
+x+auto const+
+-+----------+


function|functional|not to be documented

.. das:function:: reduce (it:iterator<auto(TT)> const; blk:block<(left:TT const -&;right:TT const -&):TT const -&> const )  : auto

reduce returns auto

arguments are

+---+----------------------------------------------------+
+it +iterator<auto(TT)> const                            +
+---+----------------------------------------------------+
+blk+block<(left:TT const;right:TT const):TT const> const+
+---+----------------------------------------------------+


function|functional|reduce to be documented

.. das:function:: reduce (it:iterator<auto(TT)> const; blk:function<(left:TT const -&;right:TT const -&):TT const -&> const )  : auto

reduce returns auto

arguments are

+---+-------------------------------------------------------+
+it +iterator<auto(TT)> const                               +
+---+-------------------------------------------------------+
+blk+function<(left:TT const;right:TT const):TT const> const+
+---+-------------------------------------------------------+


function|functional|reduce to be documented

.. das:function:: reduce (it:iterator<auto(TT)> const; blk:lambda<(left:TT const -&;right:TT const -&):TT const -&> const )  : auto

reduce returns auto

arguments are

+---+-----------------------------------------------------+
+it +iterator<auto(TT)> const                             +
+---+-----------------------------------------------------+
+blk+lambda<(left:TT const;right:TT const):TT const> const+
+---+-----------------------------------------------------+


function|functional|reduce to be documented

.. das:function:: repeat (value:auto(TT) const; count:int )  : auto

repeat returns auto

arguments are

+-----+--------------+
+value+auto(TT) const+
+-----+--------------+
+count+int           +
+-----+--------------+


function|functional|repeat to be documented

.. das:function:: sum (it:iterator<auto(TT)> const )  : auto

sum returns auto

arguments are

+--+------------------------+
+it+iterator<auto(TT)> const+
+--+------------------------+


function|functional|sum to be documented


