The EXPORT_CONSTRUCTOR module provides the ``[export_constructor]`` annotation
for struct types. Annotated structs automatically generate an exported
constructor function that can be called from other modules or from C++ code.

All functions and symbols are in "export_constructor" module, use require to get access to it.

.. code-block:: das

    require daslib/export_constructor
