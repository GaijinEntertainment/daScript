.. _utils_das_fmt:

.. index::
   single: Utils; das-fmt
   single: Utils; Formatter

===============================
 das-fmt --- Code Formatter
===============================

das-fmt formats daslang source files in place using
``daslib/das_source_formatter``.  It is the formatter behind the MCP
``format_file`` tool and the shipped ``pre-commit`` hook.  The SDK
bundle also carries a prebuilt ``bin/das-fmt.exe``.

Not to be confused with ``gen1_to_gen2``, the gen1→gen2 syntax
*converter*.

Quick start
===========

Format a folder (or a single file)::

   daslang utils/das-fmt/dasfmt.das -- --path path/to/scripts

Verify without writing (CI mode — exits nonzero on unformatted files)::

   daslang utils/das-fmt/dasfmt.das -- --path path/to/scripts --verify

Other flags: ``--exclude-mask <mask>`` (skip paths containing the mask,
repeatable), ``--t <n>`` (thread cap), ``--verbose``, ``--color`` /
``--no-color``.

.. seealso::

   :ref:`utils_lint` -- the lint runner (style checks beyond formatting)
