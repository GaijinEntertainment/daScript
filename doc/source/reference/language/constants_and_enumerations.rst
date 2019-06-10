.. _constants_and_enumerations:


========================
Constants & Enumerations
========================

.. index::
    single: Constants & Enumerations



daScript allows to bind constant values to a global variable identifier, and all constant global variables will be evaulated compile time if possible.
There is also Enumerations, which are strong type constants (similar to enum class in c++).

---------------
Constants
---------------

.. index::
    single: Constants

Constants bind a specific value to an identifier. Constants are exactly global variables, their value cannot be changed.

are declared with the following syntax.::

    let
      foobar = 100
    let
      floatbar = 1.2
    let
      stringbar = "I'm a constant string"

constants are always globally scoped, from the moment they are declared, any following code
can reference them.

You can not change such global variables.
Mutable global variables are defined as:

var 
  foobar = 100

and their usage can be switched of per-project basis.

---------------
Enumerations
---------------

.. index::
    single: Enumerations

As Enumerations bind a specific value to a name. Enumerations are also evaluated at compile time
and their value cannot be changed.

An enum declaration introduces a new enumeration into the program.
Enumeration values can only be integers. No expression are allowed. It is not required to assign specific value to enum::

    enum Numbers
        zero     //will be 0
        one      //will be 1
        two      //will be 2
        ten = 10 //will be 10, as written

An enum name itself is strong type, and all enum values are of this type.
An enum value can be addressed as 'enum name' followed by exact enumeration ::

    let one : Numbers = Numbers one

An enum value can be converted to integer type with explicit cast ::

    let one : Numbers = Numbers one
    assert ( int(one)==1 )

