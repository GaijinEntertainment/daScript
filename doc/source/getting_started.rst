.. _getting_started:

.. index::
   single: Getting Started
   single: Installation

=================
 Getting Started
=================

This page takes you from a clean machine to a running daslang program,
with editor support and AI-assistant tooling wired in.

.. contents::
   :local:
   :depth: 1


Build the compiler
==================

You need git, CMake, and a C++ compiler for your platform (MSVC, clang,
or gcc)::

   git clone https://github.com/GaijinEntertainment/daScript
   cd daScript
   cmake -B build
   cmake --build build --config Release -j

The compiler lands at ``bin/Release/daslang.exe`` (Windows / MSVC
multi-config), or ``bin/daslang`` / ``build/daslang`` (Make and Ninja
single-config layouts).  The rest of this page writes ``daslang`` for
whichever of those you have; adding it to ``PATH`` or a shell alias
makes that literal.


Hello, world
============

.. code-block:: das

   options gen2

   [export]
   def main() {
       print("hello, world\n")
   }

Save as ``hello_world.das`` and run::

   daslang hello_world.das

The ``[export]`` annotation makes ``main`` callable from outside the
compiled context --- entry points need it.  ``options gen2`` selects the
current syntax generation; all documentation and new code use it.

From here the fastest tour of the language is the tutorial series ---
start at :ref:`tutorials` and read in order, or jump straight to the
:ref:`reference manual <reference>` and the :ref:`standard library
<stdlib>`.


Editor support: VSCode
======================

The best supported editor is VSCode with the
`daScript language support
<https://marketplace.visualstudio.com/items?itemName=profelis.dascript-plugin>`_
extension --- language server, linting, debugging, and snippets.
Install it from the marketplace and point
``dascript.compiler`` at your daslang binary.  If autocompletion seems
unresponsive, raise the ``dascript.server.connectTimeout`` setting
(10 s, or 20 s for a Debug-build compiler).


AI assistants: MCP server
=========================

The :ref:`daslang MCP server <utils_mcp>` gives AI coding assistants
(Claude Code and any other `Model Context Protocol
<https://modelcontextprotocol.io/>`_ client) compiler-backed tools:
compile checks, go-to-definition, find-references, parse-aware grep,
AST dumps, test running, live-reload control, and more.

Configure it in ``.mcp.json`` at the project root:

.. code-block:: json

   {
     "mcpServers": {
       "daslang": {
         "command": "bin/Release/daslang.exe",
         "args": ["utils/mcp/main.das"],
         "defer_loading": false
       }
     }
   }

(on Linux/macOS point ``command`` at your binary, e.g.
``bin/daslang``), or add it via the CLI::

   claude mcp add daslang -- bin/Release/daslang.exe utils/mcp/main.das

The assistant starts and stops the server automatically with each
session.  The full tool table, C++-side search tools, and Windows/MSVC
environment notes are in :ref:`utils_mcp`.


AI assistants: LSP server
=========================

The :ref:`daslang LSP server <utils_lsp>` complements the MCP tools
with **push diagnostics**: after every edit of a ``.das`` file the
compiler (and the lint passes) report errors and warnings into the
assistant's context automatically --- no explicit tool call --- plus
native definition / references / hover / symbols / call hierarchy /
implementation navigation.  It needs only the daslang binary and
Python 3.

Claude Code sessions started at the repository root (or an installed
SDK root) load it automatically via the checked-in
``.claude/skills/daslang-lsp/`` plugin manifest.  From any other
directory::

   claude --plugin-dir /abs/path/to/daScript/utils/lsp/plugin

Any other stdio LSP client can spawn it directly as
``python3 utils/lsp/lsp_supervisor.py``.  Configuration options and
architecture notes are in :ref:`utils_lsp`.


Next steps
==========

- :ref:`Tutorials <tutorials>` --- runnable, self-contained walkthroughs
  from hello-world to macros and C++ integration.
- :ref:`Reference manual <reference>` --- the language, embedding, and
  tooling documentation.
- :ref:`Standard library <stdlib>` --- every builtin and daslib module.
