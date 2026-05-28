The logger module provides a structured, file-backed logging
facility for daslang tools (MCP server, daslang-live, dastest, etc).
Records are written as one JSON object per line ("JSON Lines" /
``ndjson``) to ``{get_das_root()}/logs/<name>.log``, with an
ISO 8601 UTC timestamp (millisecond precision), level name,
dotted category, message, and optional ``fields`` object.

The default level is ``LOG_INFO``; per-category overrides apply by
dotted prefix (an override on ``"mcp.live"`` covers ``"mcp.live"``,
``"mcp.live.foo"``, and so on). :func:`logger_install_hook` registers
a process-wide debug agent that diverts ``print`` and ``to_log`` into
the log file — essential for stdio-transport processes (MCP server,
language servers) where any stdout write corrupts the wire format.

All functions and symbols are in "logger" module, use require to get
access to it.

.. code-block:: das

    require daslib/logger
