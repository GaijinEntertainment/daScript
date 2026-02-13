
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

Formats a daScript source code string and returns the formatted result.

:Arguments: * **file_data** : string& implicit

.. _function-das_source_formatter_format_source_array_ls_uint8_gr_:

.. das:function:: format_source(file_data: array<uint8>) : string

Formats daScript source code given as a byte array and returns the formatted result.

:Arguments: * **file_data** : array<uint8> implicit


