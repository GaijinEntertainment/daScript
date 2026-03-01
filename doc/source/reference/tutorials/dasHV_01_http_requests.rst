.. _tutorial_dasHV_http_requests:

===========================================
HV-01 — Simple HTTP Requests
===========================================

.. index::
    single: Tutorial; HTTP
    single: Tutorial; HTTP Requests
    single: Tutorial; dasHV
    single: Tutorial; GET
    single: Tutorial; POST

This tutorial covers daslang's HTTP client bindings in the ``dashv`` module —
making GET, POST, PUT, PATCH, DELETE, and HEAD requests, passing custom
headers, and reading responses.

Setup
=====

Import the ``dashv`` module (via its boost helper)::

   require dashv/dashv_boost

All request functions create a fresh connection per call — ideal for scripts,
tools, and one-off requests. For persistent connections, custom timeouts, and
authentication, see :ref:`tutorial_dasHV_http_requests_advanced`.

GET Requests
============

The simplest call retrieves a resource with no body:

.. code-block:: das

   GET("http://example.com/api") <| $(resp) {
       print("status: {resp.status_code}, body: {resp.body}\n")
   }

Pass custom headers with a table literal:

.. code-block:: das

   GET(url, {"Accept" => "text/plain", "X-Request-Id" => "42"}) <| $(resp) {
       print("{resp.status_code}: {resp.body}\n")
   }

POST Requests
=============

POST sends a body string. An optional headers table is accepted as
the third argument:

.. code-block:: das

   POST(url, "hello") <| $(resp) {
       print("{resp.status_code}: {resp.body}\n")
   }

   // with headers
   POST(url, "\{\"msg\":\"hi\"\}", {"Content-Type" => "application/json"}) <| $(resp) {
       print("{resp.status_code}: {resp.body}\n")
   }

PUT and PATCH
=============

PUT and PATCH follow the same signature as POST — a URL, a body,
and an optional headers table:

.. code-block:: das

   PUT(url, "payload") <| $(resp) { ... }
   PATCH(url, "payload") <| $(resp) { ... }

DELETE and HEAD
===============

DELETE takes a URL and optional headers. HEAD is identical in
signature to GET but returns only the response headers (no body):

.. code-block:: das

   HTTP_DELETE(url) <| $(resp) { ... }
   HEAD(url) <| $(resp) { ... }

.. note::

   The DELETE function is named ``HTTP_DELETE`` to avoid clashing with
   the daslang ``delete`` keyword.

Response Headers
================

Read individual headers with ``get_header``, or iterate all with
``each_header``. Both accept an ``HttpResponse?`` pointer:

.. code-block:: das

   GET(url) <| $(resp) {
       let ct = get_header(resp, "Content-Type")
       print("Content-Type: {ct}\n")

       each_header(resp) <| $(key, value) {
           print("  {key}: {value}\n")
       }
   }

Status Message
==============

``status_message`` returns the human-readable reason phrase (e.g.
``"OK"``, ``"Not Found"``):

.. code-block:: das

   GET(url) <| $(resp) {
       let msg = status_message(resp)
       print("Status: {resp.status_code} {msg}\n")
   }

Binary Data
===========

``resp.body`` is a string, but ``resp.content_length`` tells you the
exact byte count for binary payloads:

.. code-block:: das

   GET(url) <| $(resp) {
       print("Received {resp.content_length} bytes\n")
   }

Quick Reference
===============

====================================  ==============================================
Function                              Description
====================================  ==============================================
``GET(url) <| $(resp) { ... }``       GET request
``GET(url, headers) <| ...``          GET with custom headers
``POST(url, body) <| ...``            POST request
``POST(url, body, headers) <| ...``   POST with custom headers
``PUT(url, body) <| ...``             PUT request
``PATCH(url, body) <| ...``           PATCH request
``HTTP_DELETE(url) <| ...``           DELETE request
``HEAD(url) <| ...``                  HEAD request
``get_header(resp, key)``             Read a single response header
``each_header(resp) <| $(k, v) {}``   Iterate all response headers
``status_message(resp)``              Human-readable status phrase
====================================  ==============================================

.. seealso::

   Full source: :download:`tutorials/dasHV/01_http_requests.das <../../../../tutorials/dasHV/01_http_requests.das>`

   Next tutorial: :ref:`tutorial_dasHV_http_requests_advanced`
