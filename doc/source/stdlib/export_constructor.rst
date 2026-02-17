
.. _stdlib_export_constructor:

==================
Export constructor
==================

.. das:module:: export_constructor

The EXPORT_CONSTRUCTOR module provides the ``[export_constructor]`` annotation
for struct types. Annotated structs automatically generate an exported
constructor function that can be called from other modules or from C++ code.

All functions and symbols are in "export_constructor" module, use require to get access to it. ::

    require daslib/export_constructor



++++++++++++++++
Structure macros
++++++++++++++++

.. _handle-export_constructor-export_constructor:

.. das:attribute:: export_constructor

implements 'export_constructor' macro, adds function make`{StructureName} which makes a new instance of a class or structure



