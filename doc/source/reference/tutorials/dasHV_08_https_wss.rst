.. _tutorial_dasHV_https_wss:

=================================
HV-08 — HTTPS and WSS (TLS)
=================================

.. index::
    single: Tutorial; HTTPS
    single: Tutorial; TLS
    single: Tutorial; WSS
    single: Tutorial; init_wss
    single: Tutorial; dasHV

This tutorial covers serving over TLS. A server created with ``init_wss``
listens on both a plain HTTP port and an HTTPS port, using a certificate and
private key on disk. The same route handlers serve both.

Prerequisites: :ref:`tutorial_dasHV_http_server`.

Starting a TLS Server
=====================

Where a plain server calls ``init(port)``, a TLS server calls
``init_wss(port, httpsPort)``. The server then accepts plain HTTP on ``port``
and TLS on ``httpsPort``; the route API is unchanged:

.. code-block:: das

   class TutorialTlsServer : HvWebServer {
       def override onInit {
           GET("/secure") <| @(var req : HttpRequest?; var resp : HttpResponse?) : http_status {
               return resp |> TEXT_PLAIN("secure hello over TLS")
           }
       }
   }

   var server = new TutorialTlsServer()
   server->init_wss(18091, 18443)   // HTTP on 18091, HTTPS on 18443
   server->start()

The Certificate
===============

``init_wss`` takes an optional third argument — the directory holding
``server.crt`` and ``server.key``. When omitted it defaults to
``modules/dasHV/cert``, a self-signed pair that ships with dasHV:

.. code-block:: das

   server->init_wss(18091, 18443)                 // default cert dir
   server->init_wss(18091, 18443, "/path/certs")  // your own crt/key

TLS requires that dasHV was built with OpenSSL (the build enables it by
default). If the certificate files cannot be loaded, ``init_wss`` reports an
error naming the expected ``server.crt`` / ``server.key`` paths.

Making an HTTPS Request
=======================

The client follows the ``https://`` scheme and negotiates TLS automatically.
The bundled certificate is self-signed, which the local client accepts:

.. code-block:: das

   GET("https://127.0.0.1:18443/secure") $(resp) {
       assert(resp.status_code == http_status.OK)
       // resp.body == "secure hello over TLS"
   }

The same threaded server-lifecycle helper from :ref:`tutorial_dasHV_http_server`
works unchanged — only ``init`` becomes ``init_wss`` and the client URL becomes
``https://``.

.. note::

   ``init_wss`` also enables secure WebSockets (``wss://``) on the TLS port for
   any WebSocket routes the server registers (see :ref:`tutorial_dasHV_websockets`).

.. seealso::

   Full source: :download:`tutorials/dasHV/08_https_wss.das <../../../../tutorials/dasHV/08_https_wss.das>`

   Previous tutorial: :ref:`tutorial_dasHV_sse_and_streaming`
