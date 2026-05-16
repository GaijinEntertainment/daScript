.. _tutorial_jsonrpc_dispatch_line:

=====================================================
JSONRPC-02 — Implementing a Server with dispatch_line
=====================================================

.. index::
    single: Tutorial; JSON-RPC; server
    single: Tutorial; jsonrpc; dispatch_line

This tutorial shows the server side of ``daslib/jsonrpc``: the
``dispatch_line`` convenience that wraps parsing, envelope building,
notification suppression, and batch fan-out in one call. The lower-level
``parse_request`` + envelope-builder path is also covered for cases
where ``dispatch_line`` doesn't fit.

The dispatcher block
====================

``dispatch_line(line, strict, dispatcher_block)`` parses a wire line and
invokes the dispatcher block for each non-notification request. The
block receives ``(method, params_json)`` and returns the raw JSON result
string; the library wraps the result in a JSON-RPC envelope.

.. code-block:: das

   require daslib/jsonrpc

   def dispatch(method, params_json : string) : string {
       if (method == "ping") return "\"pong\""
       if (method == "echo") return jsonrpc::compact_json_whitespace(params_json)
       return "\{\"error\":\"unknown: {method}\"}"
   }

   [export]
   def main() {
       let resp = jsonrpc::dispatch_line("\{\"id\":1,\"method\":\"ping\"}", false) $(m, p) {
           return dispatch(m, p)
       }
       print("{resp}\n")
       // → {"jsonrpc":"2.0","id":1,"result":"pong"}
   }

Notification semantics
======================

For a notification (no ``id``), the dispatcher still runs (so side
effects happen) but the return value is discarded and ``dispatch_line``
returns ``""``. Per JSON-RPC 2.0 §4.1, the wire MUST stay silent.

Top-level parse failure
=======================

If the wire line isn't valid JSON, ``dispatch_line`` returns a pre-built
``PARSE_ERROR`` envelope and the dispatcher is never called. Same for
top-level §6 violations such as the empty array ``[]``.

Strict vs permissive
====================

``dispatch_line(line, strict=false)`` is permissive: the ``jsonrpc``
field is optional and any string value is accepted. Pass ``strict=true``
to enforce §4 — a missing or non-``"2.0"`` ``jsonrpc`` field yields
``INVALID_REQUEST``.

When dispatch_line doesn't fit
==============================

``dispatch_line`` wraps all dispatcher results in a *success* envelope.
When you need to emit specific error codes per method
(``METHOD_NOT_FOUND``, ``INVALID_PARAMS``), drop down to
``parse_request`` + envelope builders directly:

.. code-block:: das

   let req = parse_request(line)
   if (!empty(req.error_envelope)) return req.is_notification ? "" : req.error_envelope
   if (req.method == "unknown") return error(req.id_str, METHOD_NOT_FOUND, "no such method")
   return response(req.id_str, dispatch(req.method, req.params_json))

Running the tutorial
====================

::

   daslang.exe tutorials/jsonrpc/02_dispatch_line.das

Full source: :download:`tutorials/jsonrpc/02_dispatch_line.das <../../../../tutorials/jsonrpc/02_dispatch_line.das>`

See also
========

* :ref:`tutorial_jsonrpc_request_response` — building requests, parsing responses (the client side)
* **Next:** :ref:`tutorial_jsonrpc_batch` — §6 batches end-to-end
* :ref:`stdlib_jsonrpc` — module reference
