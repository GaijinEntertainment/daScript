.. _tutorial_glob:

==========================================
Glob Pattern Matching
==========================================

.. index::
    single: Tutorial; Glob
    single: Tutorial; Pattern Matching (Glob)
    single: Tutorial; Wildcard
    single: Tutorial; Filename Matching

This tutorial covers shell-style glob pattern matching in daslang.  daslib
ships **two** glob flavors that look similar but treat ``/`` differently:

- ``daslib/fio.match_glob`` — pathname-aware: ``*`` stops at ``/``, ``**``
  crosses, and ``[abc]`` / ``[a-z]`` / ``[!abc]`` character classes are
  supported.  This is the POSIX ``fnmatch`` behavior with ``FNM_PATHNAME``.
- ``daslib/strings_boost.glob_match`` — filename-style: ``*`` matches any
  characters including ``/``, ``?`` matches one character including ``/``,
  no ``**``, no character classes.  This is plain POSIX ``fnmatch`` without
  ``FNM_PATHNAME``.

Use ``fio.match_glob`` for paths.  Use ``glob_match`` only when matching a
single name component or any text where ``/`` is incidental.

.. code-block:: das

    options gen2
    require daslib/fio
    require daslib/strings_boost


Pathname-aware matching with ``match_glob``
============================================

``match_glob(pattern, path)`` returns ``true`` when the path matches the
pattern.  It is pure — no filesystem access — and operates on slash-separated
paths.  ``*`` matches any characters except ``/``; ``**`` matches across ``/``;
``?`` matches one non-``/`` character; ``[...]`` is a character class.

.. code-block:: das

    match_glob("*.das",     "hello.das")        // true
    match_glob("*.das",     "sub/hello.das")    // false — * stops at /
    match_glob("**/*.das",  "sub/foo.das")      // true
    match_glob("**/*.das",  "a/b/c/x.das")      // true

    match_glob("[a-z][0-9].txt", "m7.txt")      // true — character class + range
    match_glob("[!abc].log",     "x.log")       // true — negated class
    match_glob("src/?ello.das",  "src/hello.das")  // true — ? matches one char

Empty patterns and paths follow the obvious rules: an empty pattern matches
only an empty path; a path is empty-matchable only by patterns made of
``*`` / ``**``.


Walking the filesystem with ``glob``
=====================================

``glob(root, pattern, blk)`` recursively walks ``root`` and invokes
``blk(filename, is_dir)`` for every entry whose path (relative to ``root``,
normalized to use ``/``) matches ``pattern``.  Order matches ``dir_rec``;
the same pattern works on Windows and POSIX.

.. code-block:: das

    var das_files : array<string>
    glob("src", "**/*.das") $(filename, is_dir) {
        if (!is_dir) {
            das_files |> push_clone(filename)
        }
    }

If you only need top-level entries, drop the ``**/``:

.. code-block:: das

    glob("src", "*.das") $(filename, is_dir) {
        // only matches src/foo.das, not src/sub/foo.das
    }


Filtering with ``glob_filtered``
=================================

``glob_filtered(root, includes, excludes, blk)`` is the multi-pattern form.
An entry is yielded if it matches at least one ``includes`` pattern and zero
``excludes`` patterns — excludes win on conflict.

.. code-block:: das

    var includes : array<string>
    var excludes : array<string>
    includes |> push_clone("**/*.das")
    excludes |> push_clone("tests/**")
    excludes |> push_clone("_build/**")

    glob_filtered("project", includes, excludes) $(filename, is_dir) {
        if (!is_dir) {
            // every .das file outside tests/ and _build/
        }
    }


Detecting glob vs literal with ``is_glob_pattern``
====================================================

When a tool accepts a user-supplied path, it is useful to dispatch on
"literal vs glob" — error if a literal file is missing, but warn if a glob
matches nothing.  ``is_glob_pattern`` returns ``true`` when the string
contains ``*``, ``?``, or ``[``:

.. code-block:: das

    is_glob_pattern("main.das")      // false — literal
    is_glob_pattern("*.das")         // true  — glob
    is_glob_pattern("[ab]c.txt")     // true  — character class
    is_glob_pattern("data/v?.json")  // true  — ?

A common convention used by build/release tools:

- A **literal** pattern that matches no file is an *error* (the user named a
  file that does not exist).
- A **glob** pattern that matches no file is a *warning* (the pattern was
  conditional and simply did not apply on this run).

Pattern to file list with ``expand_glob``
==========================================

``expand_glob(pattern, var result)`` is the higher-level helper for the common
"single glob argument from the command line" case.  It splits the literal
directory prefix from the glob remainder, picks recursive (``glob``) vs shallow
(``dir``) walk based on whether the remainder contains ``**`` or ``/``,
filters out directories so callers receive regular files only, sorts the
matches **locally**, and *appends* them to ``result``.  Backslash patterns
(``src\sub\*.das``) are normalized to ``/`` form internally.

.. code-block:: das

    var das_files : array<string>
    expand_glob("src/**/*.das", das_files)
    // das_files is sorted; only contains regular files; previously-existing
    // entries in das_files keep their position because the sort is local.


Comma/newline lists with ``parse_file_list``
=============================================

``parse_file_list(file, var result)`` is the canonical
"user-supplied paths argument → expanded file list" expander used by every
MCP tool with a ``paths`` / ``file`` argument.  Input is a comma- or
newline-separated list; whitespace is stripped; empty entries are skipped.
Literal entries pass through unchanged; glob entries (those containing
``*``, ``?``, or ``[``) go through ``expand_glob``.  Order is preserved
across plain entries — globs sort within their own slice.

.. code-block:: das

    var inputs : array<string>
    parse_file_list("README.md,daslib/*.das,tests/fio/*.das", inputs)
    // inputs:
    //   README.md           (plain, position preserved)
    //   daslib/<sorted>     (glob expansion, sorted within its slice)
    //   tests/fio/<sorted>  (glob expansion, sorted within its slice)

Reach for ``parse_file_list`` from any new CLI tool that accepts a
``--files`` / ``--paths`` argument.  Reach for ``expand_glob`` when the
caller already split its argument into individual glob patterns.


Choosing between the two flavors
=================================

The two flavors disagree the moment ``/`` enters the picture:

.. code-block:: das

    let pattern = "*.das"
    let path    = "sub/hello.das"

    match_glob(pattern, path)   // false — * does not cross /
    glob_match(pattern, path)   // true  — * matches any chars including /

Most user-facing patterns are pathname-style — ``release_include("assets/**/*.png")``,
``--exclude tests/**``, ``compile_check **/*.das`` — and want
``fio.match_glob``.  Reach for ``strings_boost.glob_match`` only for
single-name-component matching or text where ``/`` carries no structural
meaning (for example, channel-name matching).


Performance notes
==================

- ``match_glob`` accesses the pattern and path through ``peek_data``, so
  byte-level matching is O(1) per character — fine inside per-file loops.
- ``glob`` and ``glob_filtered`` pay the full ``dir_rec`` recursion cost.
  Build the file list once; do not call them per-file inside another walk.
- An idiom worth knowing: ``peek_data("")`` does **not** invoke its block.
  ``match_glob`` handles this with explicit empty-string handling at the
  top.  Any other code wrapping ``peek_data`` over potentially-empty input
  must do the same.


Summary
========

.. list-table::
   :header-rows: 1
   :widths: 40 60

   * - Function
     - Description
   * - ``match_glob(pattern, path)``
     - Pathname-aware glob match (true/false).
   * - ``glob(root, pattern, blk)``
     - Walk ``root`` recursively, yield matches.
   * - ``glob_filtered(root, in, ex, b)``
     - Walk + filter via include/exclude lists.
   * - ``is_glob_pattern(s)``
     - Detect whether a string is a glob or literal.
   * - ``to_generic_path(s)``
     - Convert a path to forward slashes regardless of host.
   * - ``expand_glob(pattern, var result)``
     - Pattern → sorted file list, appends.
   * - ``parse_file_list(file, var result)``
     - Comma/newline list of files / dirs / globs → flat list.
   * - ``glob_match(pattern, text)``
     - Filename-style match (``*`` crosses ``/``).

.. seealso::

   :ref:`tutorial_regex` — the regex equivalent for general text matching.

   Full source: :download:`tutorials/language/54_glob.das <../../../../tutorials/language/54_glob.das>`

   Previous tutorial: :ref:`tutorial_clargs`
