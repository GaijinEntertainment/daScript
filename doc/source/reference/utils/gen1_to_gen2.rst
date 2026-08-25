.. _utils_gen1_to_gen2:

.. index::
   single: Utils; gen1-to-gen2
   single: Utils; v1 to gen2 converter

===========================================
 gen1-to-gen2 --- v1 to gen2 Converter
===========================================

gen1-to-gen2 rewrites a daslang file written in v1 (indentation-delimited)
syntax into gen2 (braces and parentheses).  It parses the file with type
inference off, so the tree mirrors the source one to one, collects every
edit as a byte range in the ORIGINAL text, and applies them in a single
pass.  Only delimiters move: the code between them is copied verbatim.

Every comment in the input must appear in the output, in the same order.
An edit that would delete or absorb one is refused, and a file with a
refused edit is reported and left untouched rather than half converted.

Quick start
===========

::

   daslang utils/gen1-to-gen2/main.das -- -p <file>        # print the result
   daslang utils/gen1-to-gen2/main.das -- -i <files...>    # rewrite in place

Flags: ``-p``/``--print`` (write to stdout), ``-i``/``--inplace``,
``--gen15`` (convert the constructor syntax but keep the indentation and
the ``options gen2 = false`` marker), ``--semicolon`` (terminate the
generated header), ``--verify-compile`` (compile the result before
accepting it), ``--help`` for the rest.

The exit code is 1 when a file does not parse as v1 or an edit is
refused; nothing is written in either case.  A file that is already gen2
is echoed unchanged, and a file using spoof templates is skipped, since
the template body is text the parser never sees as code.

.. seealso::

   :ref:`utils_das_fmt` -- the formatter; it does not change syntax generation
