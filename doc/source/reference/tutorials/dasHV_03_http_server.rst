.. _tutorial_dasHV_http_server:

===========================================
HV-03 — HTTP Server Basics
===========================================

.. index::
    single: Tutorial; HTTP Server
    single: Tutorial; dasHV
    single: Tutorial; Routes
    single: Tutorial; JSON Response
    single: Tutorial; Query Parameters

This tutorial covers building an HTTP server with the ``dashv`` module —
defining routes for all HTTP methods, reading path and query parameters,
setting response headers, and returning JSON responses.

Prerequisites: :ref:`tutorial_dasHV_http_requests` (client-side basics).

Server Class
============

Every server extends ``HvWebServer`` and overrides ``onInit`` to register
routes.  The four required WebSocket callbacks can be left empty:

.. code-block:: das

   class MyServer : HvWebServer {
       def override onInit {
           GET("/hello") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
               return resp |> TEXT_PLAIN("Hello, world!")
           }
       }
   }

Every handler receives the request and response by reference and must
return an ``int`` status code (typically from ``http_status``).

WebSocket callbacks (``onWsOpen``, ``onWsClose``, ``onWsMessage``) and
``onTick`` have empty defaults in the base class — override them only
when you need WebSocket support.

GET Route
=========

.. code-block:: das

   GET("/hello") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       return resp |> TEXT_PLAIN("Hello, world!")
   }

``TEXT_PLAIN`` sets ``Content-Type: text/plain`` and returns 200.

POST Route
==========

.. code-block:: das

   POST("/echo") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       return resp |> TEXT_PLAIN(string(req.body))
   }

``req.body`` contains the raw request body.  Cast to ``string`` when
passing to functions that expect ``string``.

All HTTP Methods
================

Use ``PUT``, ``PATCH``, ``DEL``, ``HEAD``, and ``ANY`` to register
handlers for additional methods:

.. code-block:: das

   PUT("/data") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       return resp |> TEXT_PLAIN("updated")
   }
   PATCH("/data") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       return resp |> TEXT_PLAIN("patched")
   }
   DEL("/data") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       return resp |> TEXT_PLAIN("deleted")
   }
   HEAD("/data") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       return int(http_status.OK)
   }
   // ANY registers a handler for all methods at once
   ANY("/universal") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       return resp |> TEXT_PLAIN("method was {req.method}")
   }

.. note::

   The server-side DELETE route method is ``DEL`` (not ``DELETE``) to
   avoid collision with the daslang ``delete`` keyword.

Path Parameters
===============

Use ``:name`` in the route to capture path segments.  Read them with
``get_param``:

.. code-block:: das

   GET("/users/:id") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       let id = get_param(unsafe(addr(req)), "id")
       return resp |> TEXT_PLAIN("user {id}")
   }

.. important::

   Inside a handler, ``req`` is a reference, not a pointer.  Functions
   like ``get_param`` and ``each_param`` take a pointer, so wrap the
   call in ``unsafe(addr(req))``.

Multiple path parameters work naturally:

.. code-block:: das

   GET("/users/:id/posts/:post_id") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       let user_id = get_param(unsafe(addr(req)), "id")
       let post_id = get_param(unsafe(addr(req)), "post_id")
       return resp |> TEXT_PLAIN("user {user_id}, post {post_id}")
   }

Query Parameters
================

Iterate all query parameters with ``each_param``:

.. code-block:: das

   GET("/search") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       var parts : array<string>
       each_param(unsafe(addr(req))) <| $(key, value : string) {
           parts |> push("{key}={value}")
       }
       return resp |> TEXT_PLAIN(join(parts, ", "))
   }

Individual query parameters can also be read with ``get_param`` by name.

Response Headers
================

``set_header`` on the response object adds custom headers:

.. code-block:: das

   GET("/api/info") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       set_header(resp, "X-Request-Id", "42")
       set_header(resp, "X-Server", "daslang")
       return resp |> TEXT_PLAIN("ok")
   }

JSON Responses
==============

Combine ``daslib/json_boost`` with the ``JSON`` response helper.  Build a
``JsonValue`` tree using ``JV()`` — which accepts structs, named tuples,
arrays, and scalars — serialize it with ``write_json``, and pass the
string to ``JSON(resp, ...)``:

.. code-block:: das

   require daslib/json_boost

   GET("/api/data") <| @(var req : HttpRequest; var resp : HttpResponse) : int {
       let payload : tuple<message:string; count:int> = ("hello", 42)
       return resp |> JSON(write_json(JV(payload)))
   }

``JSON`` sets ``Content-Type: application/json`` and returns 200.
``JV()`` converts named tuple fields into JSON object keys automatically.

.. note::

   ``JSON()`` always returns status 200.  For non-200 JSON responses,
   set the body and content-type manually — see
   :ref:`tutorial_dasHV_http_server_advanced`.

Server Lifecycle
================

Start the server on a background thread, run your test code, then stop:

.. code-block:: das

   require daslib/jobque_boost

   // Helper: run server, call block, stop
   def with_test_server(port : int; blk : block<(base_url : string) : void>) {
       with_job_que() {
           with_job_status(1) $(started) {
               with_job_status(1) $(finished) {
                   with_atomic32() $(stop_flag) {
                       new_thread() @() {
                           var server = new MyServer()
                           server->init(port)
                           server->start()
                           started |> notify_and_release
                           while (stop_flag |> get == 0) {
                               server->tick()
                               sleep(10u)
                           }
                           server->stop()
                           unsafe { delete server }
                           finished |> notify_and_release
                       }
                       started |> join
                       invoke(blk, "http://127.0.0.1:{port}")
                       stop_flag |> set(1)
                       finished |> join
                   }
               }
           }
       }
   }

Quick Reference
===============

====================================  ====================================================
Function                              Description
====================================  ====================================================
``GET(path) <| handler``              Register GET route
``POST(path) <| handler``             Register POST route
``PUT(path) <| handler``              Register PUT route
``PATCH(path) <| handler``            Register PATCH route
``DEL(path) <| handler``              Register DELETE route
``HEAD(path) <| handler``             Register HEAD route
``ANY(path) <| handler``              Register handler for all methods
``TEXT_PLAIN(resp, text)``            200 text/plain response
``JSON(resp, json_str)``              200 application/json response
``get_param(addr(req), name)``        Read path/query parameter
``each_param(addr(req)) <| ...``      Iterate all query parameters
``set_header(resp, key, value)``      Set response header
====================================  ====================================================

.. seealso::

   Full source: :download:`tutorials/dasHV/03_http_server.das <../../../../tutorials/dasHV/03_http_server.das>`

   Next tutorial: :ref:`tutorial_dasHV_http_server_advanced`

   Previous tutorial: :ref:`tutorial_dasHV_http_requests_advanced`
