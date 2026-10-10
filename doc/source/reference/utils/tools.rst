.. _utils_tools:

.. index::
   single: Utils; tools
   single: Utils; daslang -tool

===================================
 tools --- running the SDK's tools
===================================

Every tool under the SDK's ``utils/`` folder is a daslang program. ``daslang -tool``
runs one by name, from wherever the SDK is installed, so you never need to know
the install path.

.. contents::
   :local:
   :depth: 2


Running a tool
==============

Name the tool after ``-tool``. Every argument after the name goes to the tool::

    daslang -tool lint src/ --quiet
    daslang -tool ast-verify --file main.das
    daslang -tool daspkg list

``daslang -tool <name>`` runs ``<dasroot>/utils/<name>/main.das``. Two tools have
other entry files: ``dastest`` runs ``dastest/dastest.das`` and ``das-fmt`` runs
``utils/das-fmt/dasfmt.das``. In a source tree, a name that is not under ``utils/``
is looked up under ``utils/internal/``.

Options before ``-tool`` are daslang's own, so a tool can run under the JIT::

    daslang -jit -tool lint src/

A tool runs like any other script: it loads every module ``daslang`` would. The
project root is the directory you run the tool in, so the modules your project
installs under ``modules/`` are found. Pass ``-project_root <dir>`` before
``-tool`` to use another one.


Listing the tools
=================

``daslang -tool`` without a name runs this tool, which lists every tool of the
install::

    daslang -tool


Tools on PATH
=============

The SDK's ``bin/`` holds a copy of ``daslang`` under each of these names:
``lint``, ``dastest``, ``daspkg``, ``dascov``, ``detect-dupe``, ``benchctl`` and
``das-fmt`` (each with the ``.exe`` suffix, on every platform). Started under a
tool's name, ``daslang`` runs that tool, so ``lint src/`` is ``daslang -tool lint
src/``. The package managers put these names on PATH.

The supervisor ``watchdog`` is on PATH as ``daslang-watchdog``. Its ``-tool`` option
starts the servers an editor or an AI assistant connects to::

    daslang-watchdog -tool mcp
    daslang-watchdog -tool dap
    daslang-watchdog -tool lsp

See :ref:`utils_watchdog`.
