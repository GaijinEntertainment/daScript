.. _generic_programming:


===================
Generic Programming
===================

.. index::
    single: Generic Programming

daScript allows ommision of types in statements, functions, and function declaration, making writing in it similar to dynamically typed languages, such as Python or Lua.
Said functions are *instantiated* for specific types of arguments on the first call.

There are also ways to inspect the types of the provided arguments, in order to change the behaviour of function, or to provide reasonable meaningful errors during the compilation phase.
Most of these ways are achieved with s

Unlike C++ with its SFINAE, you can use common conditionals (if) in order to change the instance of the function depending on the type info of its arguments.
Consider the following example::

    def setSomeField(var obj; val)
        if typeinfo(has_field<someField> obj)
            obj.someField = val

This function sets ``someField`` in the provided argument *if* it is a struct with a ``someField`` member.

We can do even more.  For example::

    def setSomeField(var obj; val: auto(valT))
        if typeinfo(has_field<someField> obj)
            if typeinfo(typename obj.someField) == typeinfo(typename type valT delete const)
                obj.someField = val

This function sets ``someField`` in the provided argument *if* it is a struct with a ``someField`` member, and only if ``someField`` is of the same type as ``val``!

^^^^^^^^^
typeinfo
^^^^^^^^^

Most type reflection mechanisms are implemented with the typeinfo operator. There are:

    * ``typeinfo(typename object)`` // returns typename of object
    * ``typeinfo(fulltypename object)`` // returns full typename of object, with contracts (like !const, or !&)
    * ``typeinfo(sizeof object)`` // returns sizeof
    * ``typeinfo(is_pod object)`` // returns true if object is POD type
    * ``typeinfo(is_raw object)`` // returns true if object is raw data, i.e., can be copied with memcpy
    * ``typeinfo(is_struct object)`` // returns true if object is struct
    * ``typeinfo(has_field<name_of_field> object)`` // returns true if object is struct with field name_of_field
    * ``typeinfo(is_ref object)`` // returns true if object is reference to something
    * ``typeinfo(is_ref_type object)`` // returns true if object is of reference type (such as array, table, das_string or other handled reference types)
    * ``typeinfo(is_const object)`` // returns true if object is of const type (i.e., can't be modified)
    * ``typeinfo(is_pointer object)`` // returns true if object is of pointer type, i.e., int?

All typeinfo can work with types, not objects, with the ``type`` keyword::

    typeinfo(typename type int) // returns "int"

^^^^^^^^^^^^^^^^^^^^^^^^^^^
auto and auto(named)
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Instead of ommitting the type name in a generic, it is possible to use an explicit ``auto`` type or ``auto(name)`` to type it::

    def fn(a: auto): auto
        return a

or ::

    def fn(a: auto(some_name)): some_name
        return a

This is the same as::

    def fn(a)
        return a

This is very helpful if the function accepts numerous arguments, and some of them have to be of the same type::

    def fn(a, b) // a and b can be of different types
        return a + b

This is not the same as::

    def fn(a, b: auto) // a and b are one type
        return a + b

Also, consider the following::

    def set0(a, b; index: int) // a is only supposed to be of array type, of same type as b
        return a[index] = b

If you call this function with an array of floats and an int, you would get a not-so-obvious compiler error message::

    def set0(a: array<auto(some>); b: some; index: int) // a is of array type, of same type as b
        return a[index] = b

Usage of named ``auto`` with ``typeinfo`` ::

    def fn(a: auto(some))
        print(typeinfo(typename type some))

    fn(1) // print "const int"

You can also modify the type with delete syntax::

    def fn(a: auto(some))
        print(typeinfo(typename type some delete const))

    fn(1) // print "int"
