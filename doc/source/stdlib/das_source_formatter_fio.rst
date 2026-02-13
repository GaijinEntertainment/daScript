
.. _stdlib_das_source_formatter_fio:

================================
File-based source code formatter
================================

The DAS_SOURCE_FORMATTER_FIO module extends the source formatter with file I/O
capabilities, enabling formatting of daScript source files on disk.
It reads, formats, and writes back source files in place or to new locations.

All functions and symbols are in "das_source_formatter_fio" module, use require to get access to it. ::

    require daslib/das_source_formatter_fio

+++++++++++++++
File formatting
+++++++++++++++

  *  :ref:`format_file (file_name: string) <function-das_source_formatter_fio_format_file_string>` 

.. _function-das_source_formatter_fio_format_file_string:

.. das:function:: format_file(file_name: string)

|detail/function-das_source_formatter_fio-format_file-0xee2279329a68061|

:Arguments: * **file_name** : string

+++++++++++++
Uncategorized
+++++++++++++

.. _function-das_source_formatter_fio_format_files_array_ls_string_gr_:

.. das:function:: format_files(file_names: array<string>)

|detail/function-das_source_formatter_fio-format_files-0x244ceb22c2b5f106|

:Arguments: * **file_names** : array<string>


