
.. _stdlib_strings:

===========================
String manipulation library
===========================

++++++++
Generics
++++++++

.. das:function:: join(it;separator)

join returns ::

 auto



function arguments are

+---------+------------+
+it       +auto const  +
+---------+------------+
+separator+string const+
+---------+------------+



to be documented


.. das:function:: split(text;delim;blk)

split returns ::

 auto



function arguments are

+-----+--------------------------------------------+
+text +string const                                +
+-----+--------------------------------------------+
+delim+string const                                +
+-----+--------------------------------------------+
+blk  +block<(arg:array<string> const#):auto> const+
+-----+--------------------------------------------+



to be documented


.. das:function:: split_by_chars(text;delim;blk)

split_by_chars returns ::

 auto



function arguments are

+-----+--------------------------------------------+
+text +string const                                +
+-----+--------------------------------------------+
+delim+string const                                +
+-----+--------------------------------------------+
+blk  +block<(arg:array<string> const#):auto> const+
+-----+--------------------------------------------+



to be documented


+++++++++
Functions
+++++++++

.. das:function:: split(text;delim)

split returns ::

 array<string>



function arguments are

+-----+------------+
+text +string const+
+-----+------------+
+delim+string const+
+-----+------------+



to be documented


.. das:function:: split_by_chars(text;delim)

split_by_chars returns ::

 array<string>



function arguments are

+-----+------------+
+text +string const+
+-----+------------+
+delim+string const+
+-----+------------+



to be documented


.. das:function:: wide(text;width)

wide returns ::

 string



function arguments are

+-----+------------+
+text +string const+
+-----+------------+
+width+int const   +
+-----+------------+



to be documented



