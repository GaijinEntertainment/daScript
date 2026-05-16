.. _tutorial_jsonrpc_request_response:

=================================================
JSONRPC-01 — Building Requests, Parsing Responses
=================================================

.. index::
    single: Tutorial; JSON-RPC
    single: Tutorial; jsonrpc
    single: Tutorial; daslib/jsonrpc

This tutorial covers the client side of ``daslib/jsonrpc``: building
outgoing JSON-RPC 2.0 requests, notifications, and §6 batches, then
parsing the responses you get back.

Setup
=====

Import the module:

.. code-block:: das

   require daslib/jsonrpc

Building a request
==================

``make_request(method, params_json, id)`` builds a JSON-RPC 2.0 request
as a single-line string. The ``params_json`` argument is a *pre-serialized*
JSON value — pass ``"null"`` for no params, or any valid JSON literal,
object, or array. The id may be ``int`` or ``string``:

.. code-block:: das

   let req1 = make_request("echo", "[\"hello\"]", 1)
   // → {"jsonrpc":"2.0","id":1,"method":"echo","params":["hello"]}

   let req2 = make_request("status", "null", "call-7")
   // → {"jsonrpc":"2.0","id":"call-7","method":"status","params":null}

Notifications
=============

A notification omits the ``id`` field. Per JSON-RPC 2.0 §4.1, the server
MUST NOT send a response. Use these for fire-and-forget side effects:

.. code-block:: das

   let notif = make_notification("log", "\{\"msg\":\"ready\"}")
   // → {"jsonrpc":"2.0","method":"log","params":{"msg":"ready"}}

Building a §6 batch
====================

``make_batch(messages)`` wraps an array of pre-built request /
notification strings as a JSON array. The server returns an array of
responses, with notification entries suppressed:

.. code-block:: das

   let entries <- [
       make_request("status", "null", 10),
       make_request("echo", "[1, 2, 3]", 11),
       make_notification("log", "\{\"level\":\"info\"}")
   ]
   let batch = make_batch(entries)

Parsing a success response
===========================

``parse_response(line)`` returns a ``ParsedResponse`` struct. Check
``is_success`` to distinguish ``{"result":...}`` from ``{"error":...}``:

.. code-block:: das

   let wire = "\{\"jsonrpc\":\"2.0\",\"id\":1,\"result\":[\"hello\"]}"
   let r = parse_response(wire)
   // r.is_success  = true
   // r.id_str      = "1"
   // r.result_json = ["hello"]

Parsing an error response
==========================

Error responses populate ``error_code``, ``error_msg``, and optionally
``error_data``. Standard codes are constants: ``PARSE_ERROR``,
``INVALID_REQUEST``, ``METHOD_NOT_FOUND``, ``INVALID_PARAMS``,
``INTERNAL_ERROR``:

.. code-block:: das

   let wire = "\{\"jsonrpc\":\"2.0\",\"id\":1,\"error\":\{\"code\":-32601,\"message\":\"method not found\"}}"
   let r = parse_response(wire)
   // r.is_success = false
   // r.error_code = -32601 (matches METHOD_NOT_FOUND)
   // r.error_msg  = "method not found"

Parsing a batched response
===========================

``parse_response_batch`` detects whether the wire was a single response
or a JSON array. For batches, ``is_batch=true`` and ``responses[]``
holds one entry per array element:

.. code-block:: das

   let wire = "[\{\"id\":10,\"result\":\"ok\"},\{\"id\":11,\"error\":\{\"code\":-32602,\"message\":\"bad\"}}]"
   let pb = parse_response_batch(wire)
   for (entry in pb.responses) {
       if (entry.is_success) print("[{entry.id_str}] result: {entry.result_json}\n")
       else                  print("[{entry.id_str}] error {entry.error_code}: {entry.error_msg}\n")
   }

Running the tutorial
====================

::

   daslang.exe tutorials/jsonrpc/01_request_response.das

Full source: :download:`tutorials/jsonrpc/01_request_response.das <../../../../tutorials/jsonrpc/01_request_response.das>`

See also
========

* **Next:** :ref:`tutorial_jsonrpc_dispatch_line` — implementing a server with ``dispatch_line``
* :ref:`tutorial_jsonrpc_batch` — §6 batches end-to-end
* :ref:`stdlib_jsonrpc` — module reference
