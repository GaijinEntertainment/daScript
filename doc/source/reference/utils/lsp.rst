.. _utils_lsp:

.. index::
   single: Utils; LSP Server
   single: Utils; Language Server Protocol
   single: Utils; Diagnostics

===========================================
 LSP Server --- Editor & AI Diagnostics
===========================================

The daslang LSP server implements the
`Language Server Protocol <https://microsoft.github.io/language-server-protocol/>`_
for ``.das`` files.  What it buys over the :ref:`MCP tools <utils_mcp>`:
**push diagnostics** --- the compiler and lint report after *every* edit
with no explicit tool call --- plus native go-to-definition, references,
hover, document / workspace symbols, call hierarchy, and
go-to-implementation.

Zero setup beyond a daslang binary and Python 3: no sgconfig, no
tree-sitter, no MCP server.

.. contents::
   :local:
   :depth: 2


Quick start
===========

Claude Code sessions started at the daslang repository root (or at an
installed SDK root) load the server automatically --- the checked-in
``.claude/skills/daslang-lsp/.claude-plugin/plugin.json`` manifest
registers it on workspace trust.  From any other directory, wire it in
explicitly with::

   claude --plugin-dir /abs/path/to/daScript/utils/lsp/plugin

(The explicit form also takes precedence over the checked-in manifest
when both are present, which is what you want while developing the
server itself.)

Diagnostics then attach automatically to the next tool result after any
edit of a ``.das`` file, and the LSP tool exposes definition,
references, hover, documentSymbol, workspaceSymbol, implementation, and
call-hierarchy operations.

Any other stdio LSP client works too --- point it at::

   python3 utils/lsp/lsp_supervisor.py

.. note::

   The plugin manifest spawns ``python3``.  On Windows, python.org
   installs typically ship only ``python.exe`` --- either create a
   ``python3`` alias or edit the manifest's ``command`` locally.


Diagnostics
===========

One lint-profile compile serves both error classes:

- **Compile errors** publish with severity Error, the numeric daslang
  error code (e.g. ``30341``), and the exact source range.
- On a clean compile, the repo's paranoid / performance / style lint
  passes run on the same program: LINT and PERF rules publish as
  Warning, STYLE rules as Information.  Repo lint policy is honored
  (``.lint_config``, default-off rules, ``// nolint`` comments).

dastest expect-files (negative tests that declare intentional compile
errors via ``expect`` directives) get a single informational note
instead of their intentional errors.

Unsaved buffers are compiled as-is: the server shadows every open
document and overlays the buffer text over the on-disk content for both
diagnostics and navigation, so clients that validate while typing get
correct positions without saving.


Navigation
==========

Definition, references, hover, and document / workspace symbols behave
as in any LSP server.  The daslang-specific semantics:

- **Call hierarchy** covers direct calls (``foo()``, ``obj.method()``).
  Virtual invocations through ``obj->method()`` and lambda /
  function-pointer invokes are not statically resolvable and don't
  appear.  Generic instances collapse onto their generic; class-method
  items display the bare method name.
- **Go to implementation**: on a class method --- its overrides in
  derived classes; on a class or struct --- the types deriving from it.
- Search scope is the program of the file at the cursor (its
  ``require`` closure) --- callers or overrides in unrelated files that
  are not part of that program are not found.


Configuration
=============

``initializationOptions`` (all optional):

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Key
     - Description
   * - ``compiler``
     - Path to the daslang binary.
   * - ``project``
     - ``.das_project`` file passed to the compile (module roots /
       source-resolver options).
   * - ``project_root``
     - Passed as ``-project_root`` (daspkg-style dynamic native module
       resolution).
   * - ``load_module``
     - List; each entry passed as ``-load_module``.

Compiler lookup order: ``initializationOptions.compiler`` →
``$DASLANG_LSP_COMPILER`` → repo and installed-SDK layouts
(``bin/Release/daslang[.exe]``, ``bin/daslang[.exe]``,
``build/daslang``, ``build/bin/daslang``) under the workspace and the
supervisor's own tree → ``daslang`` on ``PATH``.  Relative paths are
absolutized at discovery.

Set ``DASLANG_LSP_LOG=<path>`` for a message-by-message debug log
(default: ``daslang_lsp.log`` in the system temp directory).


Architecture
============

Two processes, hard split:

- ``utils/lsp/lsp_supervisor.py`` --- the endpoint the client spawns.
  Owns all session state: Content-Length framing, the ``initialize``
  handshake, the document shadow, debounce, and request dispatch.
  Zero language knowledge.
- ``utils/lsp/subtools/*.das`` --- stateless batch tools
  (``validate.das``, ``nav.das``).  One fresh daslang process per
  request: argv in, LSP-shaped JSON out, exit.

No resident daslang process ever runs, by design: no macro-state leaks
across compiles, no binary or DLL locks while builds run, and a
compiler crash on a broken buffer costs one request instead of the
session.  Full rationale and wave history: ``utils/lsp/ROADMAP.md``.


Tests
=====

``tests/lsp/test_lsp_protocol.das`` drives the supervisor over a stdio
pipe: initialize → didOpen with broken buffer text against a clean disk
file (proving the unsaved-buffer overlay) → didChange back to clean →
definition → the call-hierarchy loop (prepare → incoming → outgoing) →
implementation → shutdown/exit::

   bin/daslang dastest/dastest.das -- --test tests/lsp

The test probes ``python3`` then ``python`` and skips with a log notice
when neither is on ``PATH``.


.. seealso::

   ``utils/lsp/README.md`` --- setup and configuration details

   :ref:`utils_mcp` --- the MCP server (pull-style tools: introspection,
   navigation, execution, live-reload control)

   `Language Server Protocol specification
   <https://microsoft.github.io/language-server-protocol/>`_
