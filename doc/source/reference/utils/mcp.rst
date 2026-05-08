.. _utils_mcp:

.. index::
   single: Utils; MCP Server
   single: Utils; AI Tool Integration
   single: Utils; Model Context Protocol

===========================================
 MCP Server --- AI Tool Integration
===========================================

The daslang MCP server exposes compiler-backed tools to AI coding
assistants via the `Model Context Protocol <https://modelcontextprotocol.io/>`_.
It provides compilation diagnostics, type inspection, go-to-definition,
find-references, AST dump, AOT generation, expression evaluation,
parse-aware grep, and more.

Uses stdio transport -- no extra build dependencies.

.. contents::
   :local:
   :depth: 2


Quick start
===========

Test the server manually::

   # Windows
   bin/Release/daslang.exe utils/mcp/main.das

   # Linux
   ./bin/daslang utils/mcp/main.das

Configure in ``.mcp.json`` (project root):

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

Or add via CLI::

   claude mcp add daslang -- bin/Release/daslang.exe utils/mcp/main.das

Claude Code starts and stops the server automatically with each
session.

The ``"defer_loading": false`` field requests that tool schemas load
at session start instead of being deferred (deferred = the assistant
must ``ToolSearch`` each tool by name before it can be called).  When
the harness honors the flag the per-call friction is removed; when it
doesn't (currently the upstream behavior --- see
`Issue #26844 <https://github.com/anthropics/claude-code/issues/26844>`_),
the flag is harmless and the tools fall back to the deferred path.


Tools
=====

Each tool is invoked via MCP's ``tools/call`` method.

Compilation and diagnostics
---------------------------

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Tool
     - Description
   * - ``compile_check``
     - Compile a ``.das`` file and return errors/warnings plus a
       categorized function listing on success.  Optional ``json``
       for structured output.
   * - ``list_functions``
     - Compile a ``.das`` file and list all user functions, class
       methods, and generic instances (after macro expansion).
   * - ``list_types``
     - Compile a ``.das`` file and list all structs, classes (with
       fields), enums (with values), and type aliases.
   * - ``list_requires``
     - Compile a ``.das`` file and list all ``require`` dependencies
       (direct and transitive), with source file paths.  Optional
       ``json`` for structured output.
   * - ``list_modules``
     - List all available daslang modules (builtin C++ modules and
       daslib).  Optional ``json`` for structured output.
   * - ``list_module_api``
     - List all functions, types, enums, and globals exported by a
       builtin or daslib module.  Optional ``compact`` mode for large
       modules, ``filter`` for substring matching, ``section`` to
       limit output (e.g. ``functions``, ``structs``).

Code navigation
---------------

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Tool
     - Description
   * - ``goto_definition``
     - Given a cursor position (file, line, column), resolve the
       definition of the symbol under the cursor.  Returns location,
       kind, and source snippet.
   * - ``type_of``
     - Given a cursor position, return the resolved type of the
       expression under the cursor (innermost to outermost).
   * - ``find_references``
     - Find all references to the symbol under the cursor.  Scope:
       ``file`` (default) or ``all`` (all loaded modules).
   * - ``find_symbol``
     - Cross-module symbol search (functions, generics, structs,
       handled types, enums, globals, typedefs, fields).
       Case-insensitive substring by default; ``=query`` for exact
       match.  Optional ``with_cpp_source`` to redirect builtins /
       handled types to their C++ source location (see below).

Cursor-based tools (``goto_definition``, ``type_of``,
``find_references``) support a ``no_opt`` parameter that disables
compiler optimizations to preserve the full AST -- useful when globals,
enum values, or bitfield constants get constant-folded away.

``with_cpp_source`` redirect
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``find_symbol`` and ``goto_definition`` accept an optional
``with_cpp_source`` boolean.  When ``true``, results that have a C++
implementation (builtin functions, handled types via ``addExtern`` /
``MAKE_TYPE_FACTORY``) get a resolved C++ source location appended via
the lazily-built C++ index.  First call costs ~2 s (one full
``src/include/modules`` scan); subsequent calls cost ~150 ms (a
git-state staleness signature: ``git rev-parse HEAD`` + filtered
``git status`` + per-file mtimes + the search-config file's mtime).
The index rebuilds automatically when relevant ``.cpp/.h/.hpp`` files
in the search scope change, when ``HEAD`` moves, or when the search
config is edited.  Default off -- opt in when the question is "where
is X *actually* implemented", not when just enumerating symbols.

Program introspection
---------------------

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Tool
     - Description
   * - ``ast_dump``
     - Dump AST of an expression or compiled function.
       ``mode=ast`` returns S-expression, ``mode=source`` returns
       post-macro daslang code.  Optional ``lineinfo``.
   * - ``program_log``
     - Produce full post-compilation program text (like
       ``options log``).  Shows all types, globals, and functions after
       macro expansion.  Optional ``function`` filter.
   * - ``describe_type``
     - Describe a type's fields, methods, values, and base type.
       Supports structs, classes, handled types, enums, bitfields,
       variants, tuples, typedefs.

Execution
---------

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Tool
     - Description
   * - ``run_script``
     - Run a ``.das`` file or inline code snippet and return
       stdout/stderr.
   * - ``run_test``
     - Run dastest on a ``.das`` test file and return pass/fail
       results.  Optional ``json`` for structured output.
   * - ``eval_expression``
     - Evaluate a daslang expression and return its printed result.
       Supports module imports via ``require`` parameter.

Code generation and transformation
-----------------------------------

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Tool
     - Description
   * - ``format_file``
     - Format a ``.das`` file using the built-in formatter.
   * - ``convert_to_gen2``
     - Convert a ``.das`` file from gen1 syntax to gen2 using
       ``das-fmt``.  Optional ``inplace`` flag.
   * - ``aot``
     - Generate AOT (ahead-of-time) C++ code for a ``.das`` file or a
       single function.  Overloaded names return a disambiguation list.

Parse-aware search (tree-sitter)
---------------------------------

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Tool
     - Description
   * - ``grep_usage``
     - Parse-aware symbol search across ``.das`` files using ast-grep +
       tree-sitter.  Finds identifier occurrences excluding comments
       and strings.  Conditional on ``sg`` CLI.
   * - ``outline``
     - List all declarations in a file or set of files using
       tree-sitter.  Works on broken/incomplete code -- no compilation
       needed.  Conditional on ``sg`` CLI.

C++ source search (tree-sitter-cpp)
------------------------------------

Parallel parse-aware tools for the C++ side of the codebase, backed by
ast-grep with tree-sitter-cpp.  Search scope and exclusions are
configured in :ref:`utils_mcp_cpp_search_config` (defaults: ``src/``,
``include/``, ``modules/`` --- locked to ``.cpp``/``.h``/``.hpp`` ---
with ``build*/``, ``cmake-build-*/``, ``CMakeFiles/``, ``_deps/``,
``3rdparty/``, ``.git/`` always excluded plus an auto-exclude for any
folder that contains a ``.git`` file or directory).  All tools are
conditional on the ``sg`` CLI.

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Tool
     - Description
   * - ``cpp_grep_usage``
     - Parse-aware identifier search across ``.cpp``/``.h``/``.hpp``
       files via ``sg run -p``.  Skips comments and strings.
   * - ``cpp_find_symbol``
     - Search C++ symbol *declarations* by name + kind.  ``kind`` accepts
       ``function``, ``class``, ``struct``, ``enum``, ``union``,
       ``typedef``, ``namespace``, ``macro``.  Covers function
       definitions *and* header-only function declarations
       (e.g. ``void foo();``); ``typedef`` matches both legacy
       ``typedef X Y;`` and modern ``using X = Y;``.  Best-effort name
       extraction; complex templates and function-pointer typedefs may
       report partial names.  Specializations are not separately listed
       (primary template only).  Macro-expanded declarations like
       ``DAS_BIND_FN(foo)`` are invisible to ast-grep.
   * - ``cpp_outline``
     - Top-level declarations in a C++ file or glob via tree-sitter-cpp.
       Works on broken/incomplete code -- no compile DB needed.
   * - ``cpp_goto_definition``
     - Given a cursor position in a C++ file, return up to five
       plausible definition locations.  **Approximate** -- no scope
       resolution, no overload disambiguation, no template-specialization
       tracking.  For substring/usage searches prefer ``cpp_grep_usage``.
       A clangd-backed precise mode is on the v2 roadmap.

.. _utils_mcp_cpp_search_config:

Search-scope configuration
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Edit ``utils/mcp/cpp_search_config.das`` to change which folders the
C++ tools (and the ``with_cpp_source`` redirect) scan.  The file
declares four constants:

- ``CPP_SEARCH_DIRS`` --- root folders to scan, recursively.
  Defaults to ``["src", "include", "modules"]``.
- ``CPP_SEARCH_ALWAYS_EXCLUDE`` --- ``--globs`` exclusion patterns
  always applied (``build*/``, ``cmake-build-*/``, ``CMakeFiles/``,
  ``_deps/``, ``3rdparty/``, ``.git/``).
- ``CPP_SEARCH_INCLUDE_GLOBS`` --- file-extension lock; defaults to
  ``["*.cpp", "*.h", "*.hpp"]``.  Repo audit shows these cover 99.3%
  of the C++ surface; extending this is rarely needed.
- ``CPP_SEARCH_INCLUDE_OVERRIDES`` --- repo-relative paths to
  re-include even when the auto-``.git``-folder rule would have
  excluded them.  Empty by default.

Folders containing a ``.git`` file or directory at any depth are
auto-excluded.  This covers daspkg-installed packages (in
``modules/.daspkg_cache/``), git submodules, ``FetchContent``
destinations, and ad-hoc clones.  To force-include such a folder, add
its repo-relative path to ``CPP_SEARCH_INCLUDE_OVERRIDES``.

Edits to ``cpp_search_config.das`` trigger an index rebuild on the
next lookup automatically (the file's mtime is part of the staleness
signature; see the ``with_cpp_source`` redirect section above).

Duplicate detection
-------------------

All four tools shell out to the underlying CLIs --- daslang's
``require`` grammar can't take hyphenated path components, so the MCP
wrappers invoke ``daslang utils/detect-dupe/main.das`` and
``daslang utils/find-dupe/main.das`` as subprocesses.  ``export_corpus``
builds the corpus once over a body of code; ``detect_duplicates``
queries it; ``judge_duplicates`` and ``find_dupe`` invoke the
:ref:`utils_find_dupe` AI judge.

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Tool
     - Description
   * - ``export_corpus``
     - Scan one or more ``.das`` files / directories / globs and write
       a corpus JSON to ``out``.  Subprocesses
       ``detect-dupe --export-functions``.
   * - ``detect_duplicates``
     - Compare candidate file(s) against a pre-built corpus.  Returns
       a per-candidate JSON envelope with corpus stats, pattern-skip
       counts, and the top-N exact and fuzzy matches per candidate.
       Supports ``keep`` to override the default pattern filter.
   * - ``judge_duplicates``
     - Take a ``detect-dupe`` JSON report and ask Claude to partition
       each cluster into real / partial / false_positive verdicts.
       Shells out to ``daslang utils/find-dupe/main.das`` --- requires
       ``daspkg install --root utils/find-dupe`` first (the
       ``anthropic/anthropic`` package).
   * - ``find_dupe``
     - Convenience wrapper: run ``detect-dupe`` against ``paths`` and
       judge the resulting clusters in one call.  Same daspkg
       requirement as ``judge_duplicates``.


Live-reload control
-------------------

These tools interact with a running :ref:`daslang-live <utils_daslang_live>`
instance via its REST API.  All accept an optional ``port`` parameter
(default 9090).

.. list-table::
   :header-rows: 1
   :widths: 25 75

   * - Tool
     - Description
   * - ``live_launch``
     - Launch ``daslang-live.exe`` on a script if not already running.
       Sets working directory to the script's folder.
   * - ``live_status``
     - Query the running instance for fps, uptime, paused state, and
       error status.
   * - ``live_error``
     - Retrieve the last compilation error (plain text).
   * - ``live_reload``
     - Trigger an incremental or full reload.
   * - ``live_pause``
     - Pause or unpause execution.
   * - ``live_command``
     - Dispatch a ``[live_command]`` by name with JSON arguments.
   * - ``live_shutdown``
     - Gracefully shut down the running instance.


ast-grep / tree-sitter setup
=============================

The ``grep_usage``, ``outline``, and ``cpp_*`` tools use
`ast-grep <https://ast-grep.github.io/>`_ (``sg`` CLI) with a custom
tree-sitter grammar for daslang plus the built-in tree-sitter-cpp
grammar.  The ``sgconfig.yml`` config file is platform-specific (shared
library extension differs), so it is gitignored.

Copy the appropriate template to ``sgconfig.yml`` in the project root:

.. code-block:: bash

   # Windows
   cp sgconfig.yml.windows sgconfig.yml

   # Linux
   cp sgconfig.yml.linux sgconfig.yml

   # macOS
   cp sgconfig.yml.osx sgconfig.yml

All three templates include a ``languageGlobs: { cpp: ["*.h", "*.hpp"] }``
block.  Without this, ast-grep classifies ``.h`` files as C (not C++)
and the ``cpp_*`` tools silently produce zero matches on headers.


Architecture
============

- Each tool invocation runs in a **separate thread** with its own
  context/heap -- when the thread ends, its memory is freed without GC.
- Protocol logic lives in ``protocol.das``, the entry point is
  ``main.das``.
- Heap is collected after each request when over threshold (1 MB).
- Tool handlers are modular: each tool lives in ``tools/*.das``,
  shared utilities in ``tools/common.das``.


Protocol
========

The server implements `MCP <https://modelcontextprotocol.io/>`_ via
JSON-RPC 2.0 over stdio:

- Reads newline-delimited JSON (NDJSON) from stdin.
- Writes JSON-RPC responses to stdout (one line per message).
- Handles: ``initialize``, ``tools/list``, ``tools/call``, ``ping``.
- Logs to stderr and to ``utils/mcp/mcp_server.log``.
- File paths passed to tools are resolved relative to the server's
  working directory.


Configuring Claude Code permissions
=====================================

Optionally, allow all MCP tools without prompting by adding to
``.claude/settings.json``:

.. code-block:: json

   {
     "permissions": {
       "allow": [
         "mcp__daslang__compile_check",
         "mcp__daslang__list_functions",
         "mcp__daslang__list_types",
         "mcp__daslang__run_test",
         "mcp__daslang__format_file",
         "mcp__daslang__run_script",
         "mcp__daslang__ast_dump",
         "mcp__daslang__list_modules",
         "mcp__daslang__find_symbol",
         "mcp__daslang__list_requires",
         "mcp__daslang__list_module_api",
         "mcp__daslang__convert_to_gen2",
         "mcp__daslang__goto_definition",
         "mcp__daslang__type_of",
         "mcp__daslang__find_references",
         "mcp__daslang__program_log",
         "mcp__daslang__eval_expression",
         "mcp__daslang__describe_type",
         "mcp__daslang__grep_usage",
         "mcp__daslang__outline",
         "mcp__daslang__cpp_grep_usage",
         "mcp__daslang__cpp_find_symbol",
         "mcp__daslang__cpp_outline",
         "mcp__daslang__cpp_goto_definition",
         "mcp__daslang__aot"
       ]
     }
   }


.. seealso::

   ``utils/mcp/README.md`` -- setup and configuration details

   `Model Context Protocol specification <https://modelcontextprotocol.io/>`_

   :ref:`utils_daslang_live` -- the live-reload application host controlled by ``live_*`` tools
