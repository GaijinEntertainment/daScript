.. _generic_programming:


===================
Generic Programming
===================

.. index::
    single: Generic Programming

daScript allows ommision of types in statements, functions, and function declaration, making writing in it similar to dynamically typed languages, such as Python or Lua.
Said functions will be *instantiated* for specific types of arguments on first call.

There are also ways to inspect types of provided arguments, in order to change behaviour of function, or provided reasonable meaningful errors during compilation phase.
Most of these ways are achieved with s

Unlike C++ with it's SFINAE, you can use comon conditions (if) in order to change instance of function depending on type info of arguments.
Consider the following example ::

    def setSomeField(var obj; val)
        if typeinfo(has_field<someField> obj)
            obj.someField = val

this function will set someField in provided argument *if* it is a struct with someField member.

We can do even more, for example ::

    def setSomeField(var obj; val: auto(valT))
        if typeinfo(has_field<someField> obj)
            if typeinfo(typename obj.someField) == typeinfo(typename type valT delete const)
                obj.someField = val

this function will set someField in provided argument *if* it is a struct with someField member, and more over, only if someField is of the same type as val!

^^^^^^^^^
typeinfo
^^^^^^^^^

Most of type reflection mechanisms are achieved with typeinfo operator. There are:

    * typeinfo(typename object) // returns typename of object 
    * typeinfo(fulltypename object) // returns full typename of object, with contracts (like !const, or !&)
    * typeinfo(sizeof object) // returns sizeof
    * typeinfo(is_pod object) // returns true if object is POD type
    * typeinfo(is_raw object) // returns true if object is raw data, i.e., can be copied with memcpy
    * typeinfo(is_struct object) // returns true if object is struct
    * typeinfo(has_field<name_of_field> object) // returns true if object is struct with field name_of_field
    * typeinfo(is_ref object) // returns true if object is reference to something
    * typeinfo(is_ref_type object) // returns true if object is of reference type (such as array, table, das_string or other handled reference types)
    * typeinfo(is_const object) // returns true if object is of const type (i.e., can't be modified)
    * typeinfo(is_pointer object) // returns true if object is of pointer type, i.e., int?

All typeinfo can wotk with types not objects, with type keyword, i.e. :: 

    * typeinfo(typename type int) // returns "int"

^^^^^^^^^^^^^^^^^^^^^^^^^^^
auto and auto(named)
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Instead of ommision of type name in generic, it is possible to use explicit 'auto' type or 'auto(name)' type it. ::

    def fn(a: auto): auto
        return a

or ::

    def fn(a: auto(some_name)): some_name
        return a

same as ::

    def fn(a)
        return a

this is very helpful, if function accept numerous arguments, and some of them has to be of the same type :: 

    def fn(a, b) // a and b can be of different types
        return a + b

Not the same as::

    def fn(a, b: auto) // a and b are one type
        return a + b

Also, consider the following::

    def set0(a, b; index: int) // a is only supposed to be of array type, of same type as b
        return a[index] = b

If you cann this function with array of floats and int, you would get not obvious compiler error message::

    def set0(a: array<auto(some>); b: some; index: int) // a is of array type, of same type as b
        return a[index] = b

Usage of named auto with typeinfo ::

    def fn(a: auto(some))
        print(typeinfo(typename type some))
    
    fn(1) // print "const int"

you can also modify type with delete syntax::

    def fn(a: auto(some))
        print(typeinfo(typename type some delete const))
    
    fn(1) // print "int"
