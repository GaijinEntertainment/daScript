
.. _stdlib_temp_strings:

==========================
Temporary string utilities
==========================

.. das:module:: temp_strings

The TEMP_STRINGS module provides temporary string construction that avoids heap
allocations. Temporary strings are allocated on the stack or in scratch memory
and are valid only within the current scope, offering fast string building
for formatting and output.

All functions and symbols are in "temp_strings" module, use require to get access to it.

.. code-block:: das

    require daslib/temp_strings



++++++++++++++++++++
Function annotations
++++++++++++++++++++

.. _handle-temp_strings-TempStringMacro:

.. das:attribute:: TempStringMacro

Function annotation that enables temporary string optimization.



+++++++++++++++++++++++++
Temporary string builders
+++++++++++++++++++++++++

  *  :ref:`build_temp_string (bldr: block\<(var writer:StringBuilderWriter):void\>; cb: block\<(res:string#):void\>) <function-temp_strings_build_temp_string_block_ls_var_writer_c_StringBuilderWriter_c_void_gr__block_ls_res_c_string_hh__c_void_gr_>`

.. _function-temp_strings_build_temp_string_block_ls_var_writer_c_StringBuilderWriter_c_void_gr__block_ls_res_c_string_hh__c_void_gr_:

.. das:function:: build_temp_string(bldr: block<(var writer:StringBuilderWriter):void>; cb: block<(res:string#):void>)

Same as build_string, but delete the string after the callback is called.
Intern strings are not deleted.


:Arguments: * **bldr** : block<(writer: :ref:`StringBuilderWriter <handle-strings-StringBuilderWriter>`):void>

            * **cb** : block<(res:string\ #):void>


+++++++++++++++++++++++++++
Temporary string conversion
+++++++++++++++++++++++++++

  *  :ref:`temp_string (arr: array\<uint8\>; cb: block\<(res:string#):void\>) <function-temp_strings_temp_string_array_ls_uint8_gr__block_ls_res_c_string_hh__c_void_gr_>`
  *  :ref:`temp_string (str: string; cb: block\<(res:string#):void\>) <function-temp_strings_temp_string_string_block_ls_res_c_string_hh__c_void_gr_>`


temp_string
^^^^^^^^^^^

.. _function-temp_strings_temp_string_array_ls_uint8_gr__block_ls_res_c_string_hh__c_void_gr_:

.. das:function:: temp_string(arr: array<uint8>; cb: block<(res:string#):void>)

Construct string from array of bytes and pass it to the callback.
Delete the string after the callback is called. Intern strings are not deleted.


:Arguments: * **arr** : array<uint8>

            * **cb** : block<(res:string\ #):void>

.. _function-temp_strings_temp_string_string_block_ls_res_c_string_hh__c_void_gr_:

.. das:function:: temp_string(str: string; cb: block<(res:string#):void>)


