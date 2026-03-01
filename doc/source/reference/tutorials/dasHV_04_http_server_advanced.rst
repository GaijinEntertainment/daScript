.. _tutorial_dasHV_http_server_advanced:

===========================================
HV-04 — Advanced HTTP Server Features
===========================================

.. index::
    single: Tutorial; HTTP Server Advanced
    single: Tutorial; dasHV
    single: Tutorial; Static Files
    single: Tutorial; CORS
    single: Tutorial; Redirect
    single: Tutorial; Binary Response

This tutorial covers advanced server capabilities — static file serving,
CORS, HTTP redirects, binary responses, custom headers and caching,
content-type control, and status codes.

Prerequisites: :ref:`tutorial_dasHV_http_server` (basic routes and JSON).

Static File Serving
===================

``STATIC(server, path, dir)`` maps a URL prefix to a filesystem
directory.  All files under that directory are served with the correct
``Content-Type`` based on their extension.

.. code-block:: das

   // After server->init(port) but before server->start():
   STATIC(server.server, "/static", "/path/to/public")

.. important::

   ``STATIC`` must be called after ``init()`` (which registers the
   HTTP service with the router) and before ``start()``.

Clients can then fetch ``/static/index.html``, ``/static/style.css``,
etc.

CORS (Cross-Origin Resource Sharing)
=====================================

``allow_cors()`` enables CORS headers on all responses.  Call it in
``onInit``:

.. code-block:: das

   def override onInit {
       allow_cors()
       // ... routes ...
   }

The server will respond to ``OPTIONS`` preflight requests automatically
with ``Access-Control-Allow-Origin`` and related headers.

HTTP Redirects
==============

``REDIRECT(resp, location, status)`` sends a redirect response:

.. code-block:: das

   GET("/old-path") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       return resp |> REDIRECT("/new-path", http_status.MOVED_PERMANENTLY)
   }

Common redirect status codes:

- ``http_status.MOVED_PERMANENTLY`` (301) — permanent redirect, browsers cache it
- ``http_status.FOUND`` (302) — temporary redirect

Custom Response Headers and Caching
====================================

Use ``set_header`` on the response to add cache-control, ETags, and
custom application headers:

.. code-block:: das

   GET("/cached") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       set_header(resp, "Cache-Control", "max-age=3600, public")
       set_header(resp, "ETag", "\"v1.0\"")
       set_header(resp, "X-Server", "daslang")
       return resp |> TEXT_PLAIN("cached response")
   }

Binary Response
===============

``DATA(resp, data, length)`` sends raw bytes with
``Content-Type: application/octet-stream``:

.. code-block:: text

   GET("/binary") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       var payload = "BINARY\x00DATA1234"
       return resp |> DATA(payload, 16)
   }

Content-Type and Status Codes
=============================

``set_content_type(resp, type)`` and ``set_status(resp, status)``
give full control over the response:

.. code-block:: das

   // HTML response
   GET("/html") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       set_content_type(resp, "text/html")
       resp.body := "<h1>Hello from daslang!</h1>"
       return int(http_status.OK)
   }

Non-200 JSON Responses
''''''''''''''''''''''

``JSON()`` always sets status 200.  For error responses with JSON
bodies, set the content-type and status manually:

.. code-block:: das

   GET("/not-found") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       let payload : tuple<error:string; code:int> = ("resource not found", 404)
       set_content_type(resp, "application/json")
       resp.body := write_json(JV(payload))
       set_status(resp, http_status.NOT_FOUND)
       return int(http_status.NOT_FOUND)
   }

201 Created with Location Header
'''''''''''''''''''''''''''''''''

.. code-block:: das

   POST("/items") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       set_status(resp, http_status.CREATED)
       set_header(resp, "Location", "/items/42")
       set_content_type(resp, "application/json")
       let payload : tuple<id:int; body:string> = (42, string(req.body))
       resp.body := write_json(JV(payload))
       return int(http_status.CREATED)
   }

204 No Content
''''''''''''''

.. code-block:: das

   POST("/ack") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       set_status(resp, http_status.NO_CONTENT)
       return int(http_status.NO_CONTENT)
   }

Quick Reference
===============

==========================================  ===============================================
Function                                    Description
==========================================  ===============================================
``STATIC(server, path, dir)``               Serve static files from directory
``allow_cors()``                            Enable CORS headers on all routes
``REDIRECT(resp, location, status)``        Send 3xx redirect response
``JSON(resp, json_str)``                    200 JSON response
``TEXT_PLAIN(resp, text)``                  200 text response
``DATA(resp, data, length)``                Binary response (octet-stream)
``set_header(resp, key, value)``            Set response header
``set_status(resp, status)``                Set HTTP status code
``set_content_type(resp, type)``            Set Content-Type header
==========================================  ===============================================

.. seealso::

   Full source: :download:`tutorials/dasHV/04_http_server_advanced.das <../../../../tutorials/dasHV/04_http_server_advanced.das>`

   Previous tutorial: :ref:`tutorial_dasHV_http_server`
