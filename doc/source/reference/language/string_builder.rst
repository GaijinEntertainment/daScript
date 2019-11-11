.. _string_builder:


=================
String Builder
=================

.. index::
    single: String Builder

Instead of formatting strings with variant arguments count function (like printf), daScript provides String builder functionality out-of-box.
It is both more readable, more compact and more robust than printf-like syntax.
All strings in daScript can be either string literals, or *built strings*.
Both are written with "", but string builder strings also contains any expresion in curly brackets '{}'::

    let str1 = "String Literal"
    let str2 = "str1={str1}"  // str2 will be "str1=String Literal"

In the example above, str2 will actually be compile-time defined, as expression in {} is compile-time computable.
But generally, that can be run-time compiled as well.
Expression in {} can be of any type, including handled extern type, provided that said type implements DataWalker.
All PODs in daScript do have DataWalker 'to string' implementation.

In order to make string with {} inside, one has to escape it with '\\\\' ::

    print("Curly brackets=\{\}")  // prints Curly brackets={}
