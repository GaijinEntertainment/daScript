
.. _stdlib_dashv_boost:

======================================
High-level HTTP and WebSocket wrappers
======================================

.. das:module:: dashv_boost

The DASHV_BOOST module provides high-level daScript wrapper classes for the
low-level ``dashv`` C++ bindings.

``HvWebServer`` wraps the WebSocket/HTTP server with convenient route
registration (``GET``, ``POST``, ``PUT``, ``DELETE``, ``PATCH``, ``HEAD``,
``ANY``), static file serving, and WebSocket event callbacks.

``HvWebSocketClient`` wraps the WebSocket client with ``onOpen``,
``onClose``, and ``onMessage`` callbacks.

All functions and symbols are in "dashv_boost" module, use require to get access to it.

.. code-block:: das

    require dashv/dashv_boost

See also:

  * :ref:`dashv <stdlib_dashv>` — low-level C++ bindings
  * `Tutorial HV-01 — HTTP requests <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasHV/01_http_requests.das>`_
  * `Tutorial HV-02 — Advanced HTTP requests <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasHV/02_http_requests_advanced.das>`_
  * `Tutorial HV-03 — HTTP server <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasHV/03_http_server.das>`_
  * `Tutorial HV-04 — Advanced HTTP server <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasHV/04_http_server_advanced.das>`_
  * `Tutorial HV-05 — Cookies and forms <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasHV/05_cookies_and_forms.das>`_
  * `Tutorial HV-06 — WebSockets <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasHV/06_websockets.das>`_



+++++++
Classes
+++++++

.. _struct-dashv_boost-HvWebSocketClient:

.. das:attribute:: HvWebSocketClient

WebSocket client with event-driven callbacks.
Subclass and override ``onOpen``, ``onClose``, and ``onMessage``
to handle connection lifecycle and incoming messages. Call
``process_event_que`` periodically to dispatch events.
Called when the WebSocket connection is established.
Called when the WebSocket connection is closed.

:Fields: * **client** :  :ref:`WebSocketClient <handle-dashv-WebSocketClient>`? - Called when a text message is received from the server.



.. _function-dashv_boost_HvWebSocketClient_rq_init_HvWebSocketClient_string_0x1b:

.. das:function:: HvWebSocketClient.init(url: string) : int

Connects to the given WebSocket URL. Returns 0 on success.


:Arguments: * **url** : string

.. _function-dashv_boost_HvWebSocketClient_rq_is_connected_HvWebSocketClient_0x2d:

.. das:function:: HvWebSocketClient.is_connected() : bool

Returns ``true`` if the WebSocket connection is currently open.


.. _function-dashv_boost_HvWebSocketClient_rq_process_event_que_HvWebSocketClient_0x31:

.. das:function:: HvWebSocketClient.process_event_que()

Processes pending WebSocket events; must be called periodically.


.. _function-dashv_boost_HvWebSocketClient_rq_send_HvWebSocketClient_string_0x35:

.. das:function:: HvWebSocketClient.send(text: string)

Sends a text message to the server.


:Arguments: * **text** : string

.. _function-dashv_boost_HvWebSocketClient_rq_close_HvWebSocketClient_0x39:

.. das:function:: HvWebSocketClient.close() : int

Closes the WebSocket connection. Returns 0 on success.


.. _struct-dashv_boost-HvWebServer:

.. das:attribute:: HvWebServer

HTTP and WebSocket server with route registration and event callbacks.
Subclass and override ``onWsOpen``, ``onWsClose``, ``onWsMessage``,
``onTick``, and ``onInit`` as needed. Register HTTP routes with
``GET``, ``POST``, ``PUT``, ``DELETE``, ``PATCH``, ``HEAD``, or ``ANY``.
Called when a WebSocket client connects. Override to handle new connections.
Called when a WebSocket client disconnects. Override to handle disconnections.
Called when a WebSocket message is received. Override to handle messages.
Called each tick cycle. Override for periodic work.

:Fields: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? - Called during server initialization, after the low-level server is created. Override to register routes.



.. _function-dashv_boost_HvWebServer_rq_init_HvWebServer_int_0x45:

.. das:function:: HvWebServer.init(port: int)

Initializes an HTTP/WebSocket server on the specified port.


:Arguments: * **port** : int

.. _function-dashv_boost_HvWebServer_rq_init_wss_HvWebServer_int_int_string_0x4a:

.. das:function:: HvWebServer.init_wss(port: int; httpsPort: int; pathToCert: string = "")

Initializes an HTTPS/WSS server with TLS. Uses the default ``modules/dasHV/cert``
directory if ``pathToCert`` is empty.


:Arguments: * **port** : int

            * **httpsPort** : int

            * **pathToCert** : string

.. _function-dashv_boost_HvWebServer_rq_start_HvWebServer_0x51:

.. das:function:: HvWebServer.start() : int

Starts the server. Returns 0 on success.


.. _function-dashv_boost_HvWebServer_rq_stop_HvWebServer_0x55:

.. das:function:: HvWebServer.stop() : int

Stops the server. Returns 0 on success.


.. _function-dashv_boost_HvWebServer_rq_tick_HvWebServer_0x59:

.. das:function:: HvWebServer.tick()

Processes pending HTTP and WebSocket events; must be called periodically.


.. _function-dashv_boost_HvWebServer_rq_onWsOpen_HvWebServer_WebSocketChannel_q__string_hh__0x5d:

.. das:function:: HvWebServer.onWsOpen(channel: WebSocketChannel?; url: string#)

def onWsOpen : function<(var self:dashv_boost::HvWebServer;channel:dashv::WebSocketChannel? const;url:string const#):void>

:Arguments: * **channel** :  :ref:`WebSocketChannel <handle-dashv-WebSocketChannel>`?

            * **url** : string\ #

.. _function-dashv_boost_HvWebServer_rq_onWsClose_HvWebServer_WebSocketChannel_q__0x5f:

.. das:function:: HvWebServer.onWsClose(channel: WebSocketChannel?)

def onWsClose : function<(var self:dashv_boost::HvWebServer;channel:dashv::WebSocketChannel? const):void>

:Arguments: * **channel** :  :ref:`WebSocketChannel <handle-dashv-WebSocketChannel>`?

.. _function-dashv_boost_HvWebServer_rq_onWsMessage_HvWebServer_WebSocketChannel_q__string_hh__0x61:

.. das:function:: HvWebServer.onWsMessage(channel: WebSocketChannel?; msg: string#)

def onWsMessage : function<(var self:dashv_boost::HvWebServer;channel:dashv::WebSocketChannel? const;msg:string const#):void>

:Arguments: * **channel** :  :ref:`WebSocketChannel <handle-dashv-WebSocketChannel>`?

            * **msg** : string\ #

.. _function-dashv_boost_HvWebServer_rq_onTick_HvWebServer_0x63:

.. das:function:: HvWebServer.onTick()

def onTick : function<(var self:dashv_boost::HvWebServer):void>

.. _function-dashv_boost_HvWebServer_rq_onInit_HvWebServer_0x65:

.. das:function:: HvWebServer.onInit()

def onInit : function<(var self:dashv_boost::HvWebServer):void>

.. _function-dashv_boost_HvWebServer_rq_GET_HvWebServer_string_lambda_ls_var_req_c_HttpRequest_q_;var_resp_c_HttpResponse_q__c_http_status_gr__0x67:

.. das:function:: HvWebServer.GET(uri: string; lmb: lambda<(var req:HttpRequest?;var resp:HttpResponse?):http_status>)

Registers a handler for HTTP GET requests matching ``uri``.


:Arguments: * **uri** : string

            * **lmb** : lambda<(req: :ref:`HttpRequest <handle-dashv-HttpRequest>`?;resp: :ref:`HttpResponse <handle-dashv-HttpResponse>`?): :ref:`http_status <enum-dashv-http_status>`>

.. _function-dashv_boost_HvWebServer_rq_POST_HvWebServer_string_lambda_ls_var_req_c_HttpRequest_q_;var_resp_c_HttpResponse_q__c_http_status_gr__0x6b:

.. das:function:: HvWebServer.POST(uri: string; lmb: lambda<(var req:HttpRequest?;var resp:HttpResponse?):http_status>)

Registers a handler for HTTP POST requests matching ``uri``.


:Arguments: * **uri** : string

            * **lmb** : lambda<(req: :ref:`HttpRequest <handle-dashv-HttpRequest>`?;resp: :ref:`HttpResponse <handle-dashv-HttpResponse>`?): :ref:`http_status <enum-dashv-http_status>`>

.. _function-dashv_boost_HvWebServer_rq_PUT_HvWebServer_string_lambda_ls_var_req_c_HttpRequest_q_;var_resp_c_HttpResponse_q__c_http_status_gr__0x6f:

.. das:function:: HvWebServer.PUT(uri: string; lmb: lambda<(var req:HttpRequest?;var resp:HttpResponse?):http_status>)

Registers a handler for HTTP PUT requests matching ``uri``.


:Arguments: * **uri** : string

            * **lmb** : lambda<(req: :ref:`HttpRequest <handle-dashv-HttpRequest>`?;resp: :ref:`HttpResponse <handle-dashv-HttpResponse>`?): :ref:`http_status <enum-dashv-http_status>`>

.. _function-dashv_boost_HvWebServer_rq_DELETE_HvWebServer_string_lambda_ls_var_req_c_HttpRequest_q_;var_resp_c_HttpResponse_q__c_http_status_gr__0x73:

.. das:function:: HvWebServer.DELETE(uri: string; lmb: lambda<(var req:HttpRequest?;var resp:HttpResponse?):http_status>)

Registers a handler for HTTP DELETE requests matching ``uri``.


:Arguments: * **uri** : string

            * **lmb** : lambda<(req: :ref:`HttpRequest <handle-dashv-HttpRequest>`?;resp: :ref:`HttpResponse <handle-dashv-HttpResponse>`?): :ref:`http_status <enum-dashv-http_status>`>

.. _function-dashv_boost_HvWebServer_rq_PATCH_HvWebServer_string_lambda_ls_var_req_c_HttpRequest_q_;var_resp_c_HttpResponse_q__c_http_status_gr__0x77:

.. das:function:: HvWebServer.PATCH(uri: string; lmb: lambda<(var req:HttpRequest?;var resp:HttpResponse?):http_status>)

Registers a handler for HTTP PATCH requests matching ``uri``.


:Arguments: * **uri** : string

            * **lmb** : lambda<(req: :ref:`HttpRequest <handle-dashv-HttpRequest>`?;resp: :ref:`HttpResponse <handle-dashv-HttpResponse>`?): :ref:`http_status <enum-dashv-http_status>`>

.. _function-dashv_boost_HvWebServer_rq_HEAD_HvWebServer_string_lambda_ls_var_req_c_HttpRequest_q_;var_resp_c_HttpResponse_q__c_http_status_gr__0x7b:

.. das:function:: HvWebServer.HEAD(uri: string; lmb: lambda<(var req:HttpRequest?;var resp:HttpResponse?):http_status>)

Registers a handler for HTTP HEAD requests matching ``uri``.


:Arguments: * **uri** : string

            * **lmb** : lambda<(req: :ref:`HttpRequest <handle-dashv-HttpRequest>`?;resp: :ref:`HttpResponse <handle-dashv-HttpResponse>`?): :ref:`http_status <enum-dashv-http_status>`>

.. _function-dashv_boost_HvWebServer_rq_ANY_HvWebServer_string_lambda_ls_var_req_c_HttpRequest_q_;var_resp_c_HttpResponse_q__c_http_status_gr__0x7f:

.. das:function:: HvWebServer.ANY(uri: string; lmb: lambda<(var req:HttpRequest?;var resp:HttpResponse?):http_status>)

Registers a handler for any HTTP method matching ``uri``.


:Arguments: * **uri** : string

            * **lmb** : lambda<(req: :ref:`HttpRequest <handle-dashv-HttpRequest>`?;resp: :ref:`HttpResponse <handle-dashv-HttpResponse>`?): :ref:`http_status <enum-dashv-http_status>`>

.. _function-dashv_boost_HvWebServer_rq_STATIC_HvWebServer_string_string_0x83:

.. das:function:: HvWebServer.STATIC(path: string; dir: string)

Serves static files from ``dir`` under the URL prefix ``path``.


:Arguments: * **path** : string

            * **dir** : string

.. _function-dashv_boost_HvWebServer_rq_allow_cors_HvWebServer_0x87:

.. das:function:: HvWebServer.allow_cors()

Enables cross-origin resource sharing (CORS) on the server.


.. _function-dashv_boost_HvWebServer_rq_set_document_root_HvWebServer_string_0x8b:

.. das:function:: HvWebServer.set_document_root(dir: string)

Sets the document root directory for static file serving.


:Arguments: * **dir** : string

.. _function-dashv_boost_HvWebServer_rq_set_home_page_HvWebServer_string_0x8f:

.. das:function:: HvWebServer.set_home_page(filename: string)

Sets the default home page file (e.g., ``index.html``).


:Arguments: * **filename** : string

.. _function-dashv_boost_HvWebServer_rq_set_index_of_HvWebServer_string_0x93:

.. das:function:: HvWebServer.set_index_of(dir: string)

Enables directory listing for the specified directory.


:Arguments: * **dir** : string

.. _function-dashv_boost_HvWebServer_rq_set_error_page_HvWebServer_string_0x97:

.. das:function:: HvWebServer.set_error_page(filename: string)

Sets a custom error page file.


:Arguments: * **filename** : string


++++++++++++++++++++
HTTP request helpers
++++++++++++++++++++

  *  :ref:`get_body_bytes (resp: HttpResponse?) : array\<uint8\> <function-dashv_boost_get_body_bytes_HttpResponse_q_>`
  *  :ref:`with_http_request (blk: block\<(var req:HttpRequest?#):void\>) <function-dashv_boost_with_http_request_block_ls_var_req_c_HttpRequest_q__hh__c_void_gr_>`

.. _function-dashv_boost_get_body_bytes_HttpResponse_q_:

.. das:function:: get_body_bytes(resp: HttpResponse?) : array<uint8>

Extracts the response body as an ``array<uint8>``.
Returns an empty array if the response is null, non-OK, or has no content.


:Arguments: * **resp** :  :ref:`HttpResponse <handle-dashv-HttpResponse>`?

.. _function-dashv_boost_with_http_request_block_ls_var_req_c_HttpRequest_q__hh__c_void_gr_:

.. das:function:: with_http_request(blk: block<(var req:HttpRequest?#):void>)

Creates a temporary ``HttpRequest``, invokes the block, then cleans up.
Use this to configure and send outbound HTTP requests.


:Arguments: * **blk** : block<(req: :ref:`HttpRequest <handle-dashv-HttpRequest>`?\ #):void>


