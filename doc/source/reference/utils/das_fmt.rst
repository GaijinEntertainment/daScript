.. _utils_das_fmt:

.. index::
   single: Utils; das-fmt
   single: Utils; Formatter

===============================
 das-fmt --- Code Formatter
===============================

das-fmt formats daslang source files in place using
``daslib/das_source_formatter``.  It is the formatter behind the MCP
``format_file`` tool and the shipped ``pre-commit`` hook.  Run it as
``daslang -tool das-fmt``, or as ``das-fmt`` through the SDK's
``bin/das-fmt.exe`` (see :ref:`utils_tools`).

Not to be confused with ``gen1-to-gen2``, the gen1→gen2 syntax
*converter*.

Quick start
===========

Format a folder (or a single file)::

   daslang utils/das-fmt/dasfmt.das -- --path path/to/scripts

Verify without writing (CI mode — exits nonzero on unformatted files)::

   daslang utils/das-fmt/dasfmt.das -- --path path/to/scripts --verify

Wrap lines longer than 100 columns as well (off by default)::

   daslang utils/das-fmt/dasfmt.das -- --path path/to/scripts --max-line-length 100

A long line breaks only where a newline cannot end the statement: after ``=>``,
or inside ``(`` / ``[`` after ``(``, ``[``, ``,``, ``;`` or before ``||`` /
``&&``. The continuation lines up under the open bracket. Groups shorter than
40 columns are not split, lines with a comment are not touched, and a line
that cannot be made to fit stays as it is.

Other flags: ``--exclude-mask <mask>`` (skip paths containing the mask,
repeatable), ``--t <n>`` (thread cap), ``--verbose``, ``--color`` /
``--no-color``.

.. seealso::

   :ref:`utils_lint` -- the lint runner (style checks beyond formatting)
