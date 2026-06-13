.. _tutorial_logger:

========================
Logger
========================

.. index::
    single: Tutorial; Logger
    single: Tutorial; logging
    single: Tutorial; logger_info
    single: Tutorial; JSON Lines

This tutorial covers ``daslib/logger`` — structured logging that writes one
JSON object per line ("JSON Lines" / ``ndjson``). Each record carries an
ISO-8601 UTC timestamp, a level, a free-form dotted category, the message,
and an optional ``fields`` object.

The module is built for tool back-ends — MCP servers, language servers —
where stdout is a wire protocol and a stray ``print()`` would corrupt the
stream. All public names carry the ``logger_`` prefix so short verbs like
``info`` / ``close`` don't collide with user code.

Basic logging
=============

Point the logger at a file with ``logger_set_path`` (or
``logger_set_name``, which writes to ``{get_das_root()}/logs/<name>.log``),
then call the level helpers. Each call is ``(category, message)``::

  logger_set_path("app.log")
  logger_info("app", "starting up")
  logger_warning("app.net", "connection retry")
  logger_error("app.db", "query failed")

Each line is a complete JSON object::

  {"ts":"2026-06-13T04:30:36.600Z","level":"info","cat":"app","msg":"starting up"}

Structured fields
=================

Pass a ``JsonValue?`` as the optional fourth argument to attach
machine-readable context. The ``json_boost`` named-tuple form
``JV((k=v, ...))`` is the shortest way to build it::

  logger_info("app.req", "handled request",
      JV((method = "GET", path = "/users", status = 200, ms = 12)))

  // {"ts":"...","level":"info","cat":"app.req","msg":"handled request",
  //  "fields":{"method":"GET","path":"/users","status":200,"ms":12}}

Level filtering
===============

Records below the minimum level are dropped before formatting. The default
minimum is ``LOG_INFO`` (so ``logger_debug`` is silent unless you lower the
bar). Raise it and info-level records disappear::

  logger_set_min_level(LOG_WARNING)
  logger_info("app", "dropped — below the bar")
  logger_warning("app", "kept")

The runtime levels are ``LOG_TRACE``, ``LOG_DEBUG``, ``LOG_INFO``,
``LOG_WARNING``, ``LOG_ERROR``, ``LOG_CRITICAL``.

Per-category overrides
======================

``logger_set_category_level`` lifts (or lowers) the bar for one category and
everything beneath it in the dotted hierarchy. An override on ``"app.trace"``
also governs ``"app.trace.sql"``, ``"app.trace.http"``, etc. Anything outside
that prefix still uses the global minimum::

  logger_set_category_level("app.trace", LOG_DEBUG)
  logger_debug("app.trace.sql", "kept — under the app.trace override")
  logger_debug("app.other", "dropped — global minimum is still INFO")
  logger_clear_category_levels()

The diversion hook
==================

``logger_install_hook`` plugs a global debug agent into the runtime so that
``print()`` and ``to_log()`` — from this context or any future thread — are
redirected into the log file instead of stdout/stderr. This is the whole
point of the module for stdio-transport servers::

  logger_set_min_level(LOG_DEBUG)   // print() arrives as a DEBUG record
  logger_install_hook()
  print("this print is now diverted into the log file\n")
  to_log(LOG_WARNING, "this to_log is diverted too\n")

``print()`` reaches the hook as a **debug**-level record, so lower the
minimum to ``LOG_DEBUG`` first or the default ``LOG_INFO`` bar drops it.
``to_log`` carries whatever level you pass.

Once the hook is installed, ``print()`` no longer reaches the terminal — to
display anything you must write straight to the stdout ``FILE`` with
``fwrite(fstdout(), ...)``, which bypasses the hook. ``logger_init(name)`` is
the one-line convenience that combines ``logger_set_name`` and
``logger_install_hook``.

.. seealso::

   Full source: :download:`tutorials/language/58_logger.das <../../../../tutorials/language/58_logger.das>`

   Previous tutorial: :ref:`TOML <tutorial_toml>`.

   :ref:`JSON tutorial <tutorial_json>` — the ``JV`` constructors used for the fields object.

   :doc:`/stdlib/generated/logger` — logger module reference.
