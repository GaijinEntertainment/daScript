.. _tutorial_jsonrpc_batch:

==================================================
JSONRPC-03 — §6 Batches: Many Messages in One Wire
==================================================

.. index::
    single: Tutorial; JSON-RPC; batch
    single: Tutorial; jsonrpc; §6 batch

This tutorial covers JSON-RPC 2.0 §6 batch requests — sending and
receiving multiple messages in a single wire payload. The semantics
are subtle (empty array is an error, all-notifications batch yields
nothing on the wire, per-entry errors are individual envelopes), but
``daslib/jsonrpc`` handles all the edge cases.

Mixed batch round-trip
======================

A client batches two requests plus one notification. The server
dispatches them. The response is an array of *two* entries:
the notification produces no response and input order is preserved.

.. code-block:: das

   let entries <- [
       make_request("ping", "null", 1),
       make_notification("log", "\{\"msg\":\"hello\"}"),
       make_request("echo", "[42]", 2)
   ]
   let wire = make_batch(entries)

   let resp = jsonrpc::dispatch_line(wire, false) $(m, p) {
       if (m == "ping") return "\"pong\""
       if (m == "echo") return jsonrpc::compact_json_whitespace(p)
       return "\"unknown\""
   }
   // resp = [{"id":1,"result":"pong"},{"id":2,"result":[42]}]

Per-entry errors: continue-on-error
====================================

Malformed entries — missing method, bad id type, etc. — get individual
error envelopes in their array slot. Valid entries still dispatch:

.. code-block:: das

   let wire = "[\{\"id\":1,\"method\":\"ping\"},\{\"id\":2},\{\"id\":3,\"method\":\"echo\",\"params\":[\"x\"]}]"
   let resp = jsonrpc::dispatch_line(wire, false) $(m, p) { return "ok" }
   // resp has three entries:
   //   id=1 result
   //   id=2 error -32600 (missing method)
   //   id=3 result

Empty batch array
=================

Per §6, an empty array ``[]`` is *itself* an invalid request. The server
responds with a SINGLE error envelope, not an array:

.. code-block:: das

   let resp = jsonrpc::dispatch_line("[]", false) $(m, p) { return "" }
   // resp = {"jsonrpc":"2.0","id":null,"error":{"code":-32600,"message":"invalid request: empty batch"}}

All-notifications batch
=======================

If every entry in a batch is a notification, the server processes them
(dispatcher runs for each) but emits nothing on the wire — the response
array would be empty, so per §6 the server returns nothing at all:

.. code-block:: das

   let wire = "[\{\"method\":\"a\"},\{\"method\":\"b\"}]"
   let resp = jsonrpc::dispatch_line(wire, false) $(m, p) { return "ignored" }
   // resp == ""

Manual batch handling
=====================

For custom error semantics (per-method ``INVALID_PARAMS``,
``METHOD_NOT_FOUND``, logging per entry, etc.), use ``parse_batch``
directly:

.. code-block:: das

   let pb = parse_batch(wire)
   if (!empty(pb.framing_error)) return pb.framing_error
   for (req in pb.requests) {
       if (!empty(req.error_envelope)) { /* per-entry error */ }
       else { /* req.method, req.id_str, req.params, req.params_json available */ }
   }

Running the tutorial
====================

::

   daslang.exe tutorials/jsonrpc/03_batch.das

Full source: :download:`tutorials/jsonrpc/03_batch.das <../../../../tutorials/jsonrpc/03_batch.das>`

See also
========

* :ref:`tutorial_jsonrpc_request_response` — building requests, parsing responses
* :ref:`tutorial_jsonrpc_dispatch_line` — server with ``dispatch_line``
* :ref:`stdlib_jsonrpc` — module reference
