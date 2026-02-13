
.. _stdlib_das_source_formatter:

=====================
Source code formatter
=====================

The DAS_SOURCE_FORMATTER module implements source code formatting for daScript.
It can parse and re-emit daScript source code with consistent indentation,
spacing, and line breaking rules. Used by editor integrations and code
quality tools.

All functions and symbols are in "das_source_formatter" module, use require to get access to it. ::

    require daslib/das_source_formatter

+++++++++++++
Uncategorized
+++++++++++++

.. _function-das_source_formatter_format_source_string_string_const_implicit:

.. das:function:: format_source_string(file_data: string const& implicit) : string

|detail/function-das_source_formatter-format_source_string-0x21a01e17e70ca4c2|

:Arguments: * **file_data** : string& implicit

.. _function-das_source_formatter_format_source_array_ls_uint8_gr_:

.. das:function:: format_source(file_data: array<uint8>) : string

|detail/function-das_source_formatter-format_source-0xfb066e19d23081ff|

:Arguments: * **file_data** : array<uint8> implicit


