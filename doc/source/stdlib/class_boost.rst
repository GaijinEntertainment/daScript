
.. _stdlib_class_boost:

===================
Class method macros
===================

.. das:module:: class_boost

The CLASS_BOOST module provides macros for extending class functionality,
including the ``[serialize_as_class]`` annotation for automatic serialization
and common class patterns like abstract method enforcement.

All functions and symbols are in "class_boost" module, use require to get access to it. ::

    require daslib/class_boost



++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-class_boost-class_method:

.. das:attribute:: class_method

Turns a static method into a class method by adding a ``self`` argument
of the class type as the first argument, and wrapping the function body
in ``with (self) { ... }``. Applied via ``[class_method]`` annotation.


.. _handle-class_boost-explicit_const_class_method:

.. das:attribute:: explicit_const_class_method

Same as ``[class_method]`` but marks the ``self`` parameter with ``explicitConst``,
allowing overloading of const and non-const class methods.



