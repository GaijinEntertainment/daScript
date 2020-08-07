.. _constants_and_enumerations:


========================
Constants & Enumerations
========================

.. index::
    single: Constants & Enumerations



daScript allows to bind constant values to a global variable identifier.
Whenever possible all constant global variables will be evaluated at compile time.
There are also Enumerations which are strong type constant collections similar to enum class in C++.

--------
Constant
--------

.. index::
    single: Constants

Constant binds a specific value to an identifier. Constants are exactly global variables. Their value cannot be changed.

Constants are declared with the following syntax::

    let
      foobar = 100
    let
      floatbar = 1.2
    let
      stringbar = "I'm a constant string"

constants are always globally scoped from the moment they are declared.
Any subsequential code can reference them.

You can not change such global variables.
Mutable global variables are defined as::

    var
      foobar = 100

and their usage can be switched on and off per-project basis via CodeOfPolicies.

.. _enumerations:

-----------
Enumeration
-----------

.. index::
    single: Enumerations

An enumeration binds a specific value to a name. Enumerations are also evaluated at compile time
and their value cannot be changed.

An enum declaration introduces a new enumeration to the program.
Enumeration values can only be compile time constant expressions.
It is not required to assign specific value to enum::

    enum Numbers
        zero     // will be 0
        one      // will be 1
        two      // will be 2
        ten = 9+1 // will be 10, since its explicitly specified

An enum name itself is strong type, and all enum values are of this type.
An enum value can be addressed as 'enum name' followed by exact enumeration ::

    let one: Numbers = Numbers one

An enum value can be converted to integer type with explicit cast ::

    let one: Numbers = Numbers one
    assert(int(one) == 1)

Enumeration can specify one of the following storage types: int,int8, in16, uint, uint8, uint16::

    enum Characters : uint8
        ch_a = 'A'
        ch_b = 'B'

Enumeration value will be truncated down to storage type.

each_enum iterator iterates over specific enumeration type values.
Any enum element needs to be provided to specify enumeration type::

	for x in each_enum(Characters ch_a)
		print("x = {x}\n")

