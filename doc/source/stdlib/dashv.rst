
.. _stdlib_dashv:

==================================
HTTP and WebSocket library (libhv)
==================================

.. das:module:: dashv

The DASHV module provides HTTP and WebSocket networking built on top of the
`libhv <https://github.com/ithewei/libhv>`_ library. It exposes WebSocket
client/server types, HTTP request/response handling, route registration,
cookie and form-data helpers, and an HTTP client for making outbound requests.

Use ``dashv_boost`` for the high-level daScript wrappers (``HvWebServer``,
``HvWebSocketClient``). The low-level C++ bindings live in this module.

All functions and symbols are in "dashv" module, use require to get access to it.

.. code-block:: das

    require dashv

See also:

  * :ref:`dashv_boost <stdlib_dashv_boost>` — high-level wrapper classes
  * `Tutorial HV-01 — HTTP requests <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasHV/01_http_requests.das>`_
  * `Tutorial HV-02 — Advanced HTTP requests <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasHV/02_http_requests_advanced.das>`_
  * `Tutorial HV-03 — HTTP server <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasHV/03_http_server.das>`_
  * `Tutorial HV-04 — Advanced HTTP server <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasHV/04_http_server_advanced.das>`_
  * `Tutorial HV-05 — Cookies and forms <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasHV/05_cookies_and_forms.das>`_
  * `Tutorial HV-06 — WebSockets <https://github.com/GaijinEntertainment/daScript/blob/master/tutorials/dasHV/06_websockets.das>`_



++++++++++++
Enumerations
++++++++++++

.. _enum-dashv-http_method:

.. das:attribute:: http_method

HTTP request method.

:Values: * **DELETE** = 0 - Delete resource

         * **GET** = 1 - Get resource

         * **HEAD** = 2 - Head request (headers only)

         * **POST** = 3 - Post data

         * **PUT** = 4 - Put resource

         * **CONNECT** = 5 - Connect tunnel

         * **OPTIONS** = 6 - Options inspection

         * **TRACE** = 7 - Trace echo

         * **COPY** = 8 - WebDAV copy

         * **LOCK** = 9 - WebDAV lock

         * **MKCOL** = 10 - WebDAV create collection

         * **MOVE** = 11 - WebDAV move

         * **PROPFIND** = 12 - WebDAV property find

         * **PROPPATCH** = 13 - WebDAV property patch

         * **SEARCH** = 14 - WebDAV search

         * **UNLOCK** = 15 - WebDAV unlock

         * **BIND** = 16 - WebDAV bind

         * **REBIND** = 17 - WebDAV rebind

         * **UNBIND** = 18 - WebDAV unbind

         * **ACL** = 19 - WebDAV ACL

         * **REPORT** = 20 - WebDAV report

         * **MKACTIVITY** = 21 - WebDAV make activity

         * **CHECKOUT** = 22 - WebDAV checkout

         * **MERGE** = 23 - WebDAV merge

         * **MSEARCH** = 14 - UPnP multicast search

         * **NOTIFY** = 25 - UPnP notify

         * **SUBSCRIBE** = 26 - UPnP subscribe

         * **UNSUBSCRIBE** = 27 - UPnP unsubscribe

         * **PATCH** = 28 - Partial update

         * **PURGE** = 29 - Cache purge

         * **MKCALENDAR** = 30 - CalDAV create calendar

         * **LINK** = 31 - Link resources

         * **UNLINK** = 32 - Unlink resources

         * **SOURCE** = 33 - Retrieve source



.. _enum-dashv-http_status:

.. das:attribute:: http_status

HTTP response status code.

:Values: * **CONTINUE** = 100 - 100 Continue

         * **SWITCHING_PROTOCOLS** = 101 - 101 Switching Protocols

         * **PROCESSING** = 102 - 102 Processing

         * **OK** = 200 - 200 OK

         * **CREATED** = 201 - 201 Created

         * **ACCEPTED** = 202 - 202 Accepted

         * **NON_AUTHORITATIVE_INFORMATION** = 203 - 203 Non-Authoritative Information

         * **NO_CONTENT** = 204 - 204 No Content

         * **RESET_CONTENT** = 205 - 205 Reset Content

         * **PARTIAL_CONTENT** = 206 - 206 Partial Content

         * **MULTI_STATUS** = 207 - 207 Multi-Status

         * **ALREADY_REPORTED** = 208 - 208 Already Reported

         * **IM_USED** = 226 - 226 IM Used

         * **MULTIPLE_CHOICES** = 300 - 300 Multiple Choices

         * **MOVED_PERMANENTLY** = 301 - 301 Moved Permanently

         * **FOUND** = 302 - 302 Found

         * **SEE_OTHER** = 303 - 303 See Other

         * **NOT_MODIFIED** = 304 - 304 Not Modified

         * **USE_PROXY** = 305 - 305 Use Proxy

         * **TEMPORARY_REDIRECT** = 307 - 307 Temporary Redirect

         * **PERMANENT_REDIRECT** = 308 - 308 Permanent Redirect

         * **BAD_REQUEST** = 400 - 400 Bad Request

         * **UNAUTHORIZED** = 401 - 401 Unauthorized

         * **PAYMENT_REQUIRED** = 402 - 402 Payment Required

         * **FORBIDDEN** = 403 - 403 Forbidden

         * **NOT_FOUND** = 404 - 404 Not Found

         * **METHOD_NOT_ALLOWED** = 405 - 405 Method Not Allowed

         * **NOT_ACCEPTABLE** = 406 - 406 Not Acceptable

         * **PROXY_AUTHENTICATION_REQUIRED** = 407 - 407 Proxy Authentication Required

         * **REQUEST_TIMEOUT** = 408 - 408 Request Timeout

         * **CONFLICT** = 409 - 409 Conflict

         * **GONE** = 410 - 410 Gone

         * **LENGTH_REQUIRED** = 411 - 411 Length Required

         * **PRECONDITION_FAILED** = 412 - 412 Precondition Failed

         * **PAYLOAD_TOO_LARGE** = 413 - 413 Payload Too Large

         * **URI_TOO_LONG** = 414 - 414 URI Too Long

         * **UNSUPPORTED_MEDIA_TYPE** = 415 - 415 Unsupported Media Type

         * **RANGE_NOT_SATISFIABLE** = 416 - 416 Range Not Satisfiable

         * **EXPECTATION_FAILED** = 417 - 417 Expectation Failed

         * **MISDIRECTED_REQUEST** = 421 - 421 Misdirected Request

         * **UNPROCESSABLE_ENTITY** = 422 - 422 Unprocessable Entity

         * **LOCKED** = 423 - 423 Locked

         * **FAILED_DEPENDENCY** = 424 - 424 Failed Dependency

         * **UPGRADE_REQUIRED** = 426 - 426 Upgrade Required

         * **PRECONDITION_REQUIRED** = 428 - 428 Precondition Required

         * **TOO_MANY_REQUESTS** = 429 - 429 Too Many Requests

         * **REQUEST_HEADER_FIELDS_TOO_LARGE** = 431 - 431 Request Header Fields Too Large

         * **UNAVAILABLE_FOR_LEGAL_REASONS** = 451 - 451 Unavailable For Legal Reasons

         * **INTERNAL_SERVER_ERROR** = 500 - 500 Internal Server Error

         * **NOT_IMPLEMENTED** = 501 - 501 Not Implemented

         * **BAD_GATEWAY** = 502 - 502 Bad Gateway

         * **SERVICE_UNAVAILABLE** = 503 - 503 Service Unavailable

         * **GATEWAY_TIMEOUT** = 504 - 504 Gateway Timeout

         * **HTTP_VERSION_NOT_SUPPORTED** = 505 - 505 HTTP Version Not Supported

         * **VARIANT_ALSO_NEGOTIATES** = 506 - 506 Variant Also Negotiates

         * **INSUFFICIENT_STORAGE** = 507 - 507 Insufficient Storage

         * **LOOP_DETECTED** = 508 - 508 Loop Detected

         * **NOT_EXTENDED** = 510 - 510 Not Extended

         * **NETWORK_AUTHENTICATION_REQUIRED** = 511 - 511 Network Authentication Required



.. _enum-dashv-ws_opcode:

.. das:attribute:: ws_opcode

WebSocket frame opcode.

:Values: * **WS_OPCODE_CONTINUE** = 0 - Continuation frame

         * **WS_OPCODE_TEXT** = 1 - Text frame

         * **WS_OPCODE_BINARY** = 2 - Binary frame

         * **WS_OPCODE_CLOSE** = 8 - Close connection

         * **WS_OPCODE_PING** = 9 - Ping keepalive

         * **WS_OPCODE_PONG** = 10 - Pong reply



.. _enum-dashv-ws_session_type:

.. das:attribute:: ws_session_type

WebSocket session type.

:Values: * **WS_CLIENT** = 0 - Client session

         * **WS_SERVER** = 1 - Server session




++++++++++++++++++
Handled structures
++++++++++++++++++

.. _handle-dashv-WebSocketClient:

.. das:attribute:: WebSocketClient

Low-level WebSocket client handle. Use `HvWebSocketClient` for a higher-level API.



.. _handle-dashv-WebSocketServer:

.. das:attribute:: WebSocketServer

Low-level WebSocket/HTTP server handle. Use `HvWebServer` for a higher-level API.



.. _handle-dashv-WebSocketChannel:

.. das:attribute:: WebSocketChannel

A connected WebSocket channel for sending messages to a client.



.. _handle-dashv-HttpMessage:

.. das:attribute:: HttpMessage

Base type for HTTP messages, providing header access.



.. _handle-dashv-HttpRequest:

.. das:attribute:: HttpRequest

HTTP request with URL, method, headers, body, and parameters.

:Fields: * **body** :  :ref:`das_string <handle-builtin-das_string>` - Request body content.

         * **method** :  :ref:`http_method <enum-dashv-http_method>` - HTTP method (GET, POST, etc.).

         * **url** :  :ref:`das_string <handle-builtin-das_string>` - Full request URL.

         * **scheme** :  :ref:`das_string <handle-builtin-das_string>` - URL scheme (http or https).

         * **host** :  :ref:`das_string <handle-builtin-das_string>` - Host name.

         * **port** : int - Port number.

         * **path** :  :ref:`das_string <handle-builtin-das_string>` - URL path component.

         * **timeout** : uint16 - Overall request timeout in seconds.

         * **connect_timeout** : uint16 - Connection timeout in seconds.



.. _handle-dashv-HttpResponse:

.. das:attribute:: HttpResponse

HTTP response with status code, headers, and body.

:Fields: * **body** :  :ref:`das_string <handle-builtin-das_string>` - Response body content.

         * **content** : void? - Content string.

         * **status_code** :  :ref:`http_status <enum-dashv-http_status>` - HTTP status code.



.. _handle-dashv-HttpContext:

.. das:attribute:: HttpContext

HTTP request/response context passed to route handlers.



.. _handle-dashv-HttpResponseWriter:

.. das:attribute:: HttpResponseWriter

HTTP response writer for streaming responses. Supports chunked transfer encoding and SSE events. Obtained through async HTTP handlers registered with ``SSE``.




++++++++++++++++
WebSocket client
++++++++++++++++

  *  :ref:`close (self: WebSocketClient) : int <function-dashv_close_WebSocketClient>`
  *  :ref:`is_connected (self: WebSocketClient) : bool <function-dashv_is_connected_WebSocketClient>`
  *  :ref:`make_web_socket_client (class: void?; info: StructInfo const?) : WebSocketClient? <function-dashv_make_web_socket_client_void_q__StructInfo_const_q_>`
  *  :ref:`open (self: WebSocketClient; url: string) : int <function-dashv_open_WebSocketClient_string>`
  *  :ref:`send (channel: WebSocketChannel?; msg: string; opcode: ws_opcode; fin: bool) : int <function-dashv_send_WebSocketChannel_q__string_ws_opcode_bool>`
  *  :ref:`send (self: WebSocketClient; msg: string; len: int; opcode: ws_opcode) : int <function-dashv_send_WebSocketClient_string_int_ws_opcode>`
  *  :ref:`send (channel: WebSocketChannel?; msg: string; len: int; opcode: ws_opcode; fin: bool) : int <function-dashv_send_WebSocketChannel_q__string_int_ws_opcode_bool>`
  *  :ref:`send (self: WebSocketClient; msg: string) : int <function-dashv_send_WebSocketClient_string>`
  *  :ref:`send (channel: WebSocketChannel?; msg: string; len: int; fragment: int; opcode: ws_opcode) : int <function-dashv_send_WebSocketChannel_q__string_int_int_ws_opcode>`
  *  :ref:`tick (server: WebSocketServer?) <function-dashv_tick_WebSocketServer_q_>`
  *  :ref:`tick (self: WebSocketClient) <function-dashv_tick_WebSocketClient>`

.. _function-dashv_close_WebSocketClient:

.. das:function:: close(self: WebSocketClient) : int

Closes the WebSocket client connection. Returns 0 on success.


:Arguments: * **self** :  :ref:`WebSocketClient <handle-dashv-WebSocketClient>` implicit

.. _function-dashv_is_connected_WebSocketClient:

.. das:function:: is_connected(self: WebSocketClient) : bool

Returns `true` if the WebSocket client is currently connected.


:Arguments: * **self** :  :ref:`WebSocketClient <handle-dashv-WebSocketClient>` implicit

.. _function-dashv_make_web_socket_client_void_q__StructInfo_const_q_:

.. das:function:: make_web_socket_client(class: void?; info: StructInfo const?) : WebSocketClient?

Creates a new low-level WebSocket client bound to a daScript class instance.


:Arguments: * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`? implicit

.. _function-dashv_open_WebSocketClient_string:

.. das:function:: open(self: WebSocketClient; url: string) : int

Opens a WebSocket connection to the specified URL. Returns 0 on success.


:Arguments: * **self** :  :ref:`WebSocketClient <handle-dashv-WebSocketClient>` implicit

            * **url** : string implicit


send
^^^^

.. _function-dashv_send_WebSocketChannel_q__string_ws_opcode_bool:

.. das:function:: send(channel: WebSocketChannel?; msg: string; opcode: ws_opcode; fin: bool) : int

Sends a WebSocket message through a channel with the specified opcode and fin flag.


:Arguments: * **channel** :  :ref:`WebSocketChannel <handle-dashv-WebSocketChannel>`? implicit

            * **msg** : string implicit

            * **opcode** :  :ref:`ws_opcode <enum-dashv-ws_opcode>`

            * **fin** : bool

.. _function-dashv_send_WebSocketClient_string_int_ws_opcode:

.. das:function:: send(self: WebSocketClient; msg: string; len: int; opcode: ws_opcode) : int

.. _function-dashv_send_WebSocketChannel_q__string_int_ws_opcode_bool:

.. das:function:: send(channel: WebSocketChannel?; msg: string; len: int; opcode: ws_opcode; fin: bool) : int

.. _function-dashv_send_WebSocketClient_string:

.. das:function:: send(self: WebSocketClient; msg: string) : int

.. _function-dashv_send_WebSocketChannel_q__string_int_int_ws_opcode:

.. das:function:: send(channel: WebSocketChannel?; msg: string; len: int; fragment: int; opcode: ws_opcode) : int

----


tick
^^^^

.. _function-dashv_tick_WebSocketServer_q_:

.. das:function:: tick(server: WebSocketServer?)

Processes pending events on the server or client; must be called periodically.


:Arguments: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? implicit

.. _function-dashv_tick_WebSocketClient:

.. das:function:: tick(self: WebSocketClient)


++++++++++++++++++++++++++
WebSocket server lifecycle
++++++++++++++++++++++++++

  *  :ref:`make_web_socket_server (port: int; https_port: int; pathToCert: string; class: void?; info: StructInfo const?) : WebSocketServer? <function-dashv_make_web_socket_server_int_int_string_void_q__StructInfo_const_q_>`
  *  :ref:`start (server: WebSocketServer?) : int <function-dashv_start_WebSocketServer_q_>`
  *  :ref:`stop (server: WebSocketServer?) : int <function-dashv_stop_WebSocketServer_q_>`

.. _function-dashv_make_web_socket_server_int_int_string_void_q__StructInfo_const_q_:

.. das:function:: make_web_socket_server(port: int; https_port: int; pathToCert: string; class: void?; info: StructInfo const?) : WebSocketServer?

Creates a new low-level WebSocket/HTTP server on the given port, optionally with TLS.


:Arguments: * **port** : int

            * **https_port** : int

            * **pathToCert** : string implicit

            * **class** : void? implicit

            * **info** :  :ref:`StructInfo <handle-rtti-StructInfo>`? implicit

.. _function-dashv_start_WebSocketServer_q_:

.. das:function:: start(server: WebSocketServer?) : int

Starts the server, spawning network threads. Returns 0 on success.


:Arguments: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? implicit

.. _function-dashv_stop_WebSocketServer_q_:

.. das:function:: stop(server: WebSocketServer?) : int

Stops the server. Returns 0 on success.


:Arguments: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? implicit


++++++++++++++++++++++
WebSocket channel send
++++++++++++++++++++++



+++++++++++++++++++++++
HTTP route registration
+++++++++++++++++++++++

  *  :ref:`ANY (server: WebSocketServer?; url: string; lambda: lambda\<():void\>) <function-dashv_ANY_WebSocketServer_q__string_lambda_ls__c_void_gr_>`
  *  :ref:`DELETE (url: string; block: block\<(HttpResponse?):void\>) <function-dashv_DELETE_string_block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`DELETE (server: WebSocketServer?; url: string; lambda: lambda\<():void\>) <function-dashv_DELETE_WebSocketServer_q__string_lambda_ls__c_void_gr_>`
  *  :ref:`DELETE (url: string; headers: table\<string, string\>; block: block\<(HttpResponse?):void\>) <function-dashv_DELETE_string_table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`GET (url: string; headers: table\<string, string\>; block: block\<(HttpResponse?):void\>) <function-dashv_GET_string_table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`GET (url: string; block: block\<(HttpResponse?):void\>) <function-dashv_GET_string_block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`GET (server: WebSocketServer?; url: string; lambda: lambda\<():void\>) <function-dashv_GET_WebSocketServer_q__string_lambda_ls__c_void_gr_>`
  *  :ref:`HEAD (server: WebSocketServer?; url: string; lambda: lambda\<():void\>) <function-dashv_HEAD_WebSocketServer_q__string_lambda_ls__c_void_gr_>`
  *  :ref:`HEAD (url: string; block: block\<(HttpResponse?):void\>) <function-dashv_HEAD_string_block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`HEAD (url: string; headers: table\<string, string\>; block: block\<(HttpResponse?):void\>) <function-dashv_HEAD_string_table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`PATCH (server: WebSocketServer?; url: string; lambda: lambda\<():void\>) <function-dashv_PATCH_WebSocketServer_q__string_lambda_ls__c_void_gr_>`
  *  :ref:`PATCH (url: string; text: string; block: block\<(HttpResponse?):void\>) <function-dashv_PATCH_string_string_block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`PATCH (url: string; text: string; headers: table\<string, string\>; block: block\<(HttpResponse?):void\>) <function-dashv_PATCH_string_string_table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`PATCH (url: string; text: string; headers: table\<string, string\>; from: table\<string, string\>; block: block\<(HttpResponse?):void\>) <function-dashv_PATCH_string_string_table_ls_string,_string_gr__table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`POST (url: string; text: string; headers: table\<string, string\>; block: block\<(HttpResponse?):void\>) <function-dashv_POST_string_string_table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`POST (url: string; text: string; block: block\<(HttpResponse?):void\>) <function-dashv_POST_string_string_block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`POST (url: string; text: string; headers: table\<string, string\>; from: table\<string, string\>; block: block\<(HttpResponse?):void\>) <function-dashv_POST_string_string_table_ls_string,_string_gr__table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`POST (server: WebSocketServer?; url: string; lambda: lambda\<():void\>) <function-dashv_POST_WebSocketServer_q__string_lambda_ls__c_void_gr_>`
  *  :ref:`PUT (url: string; text: string; block: block\<(HttpResponse?):void\>) <function-dashv_PUT_string_string_block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`PUT (url: string; text: string; headers: table\<string, string\>; block: block\<(HttpResponse?):void\>) <function-dashv_PUT_string_string_table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`PUT (server: WebSocketServer?; url: string; lambda: lambda\<():void\>) <function-dashv_PUT_WebSocketServer_q__string_lambda_ls__c_void_gr_>`
  *  :ref:`PUT (url: string; text: string; headers: table\<string, string\>; from: table\<string, string\>; block: block\<(HttpResponse?):void\>) <function-dashv_PUT_string_string_table_ls_string,_string_gr__table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`SSE (server: WebSocketServer?; url: string; lambda: lambda\<():void\>) <function-dashv_SSE_WebSocketServer_q__string_lambda_ls__c_void_gr_>`

.. _function-dashv_ANY_WebSocketServer_q__string_lambda_ls__c_void_gr_:

.. das:function:: ANY(server: WebSocketServer?; url: string; lambda: lambda<():void>)

.. warning::
  This is unsafe operation.

Registers a route handler that matches any HTTP method.


:Arguments: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? implicit

            * **url** : string implicit

            * **lambda** : lambda<void>


DELETE
^^^^^^

.. _function-dashv_DELETE_string_block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: DELETE(url: string; block: block<(HttpResponse?):void>)

Registers a DELETE route handler, or performs an HTTP DELETE client request.


:Arguments: * **url** : string implicit

            * **block** : block<( :ref:`HttpResponse <handle-dashv-HttpResponse>`?):void> implicit

.. _function-dashv_DELETE_WebSocketServer_q__string_lambda_ls__c_void_gr_:

.. das:function:: DELETE(server: WebSocketServer?; url: string; lambda: lambda<():void>)

.. _function-dashv_DELETE_string_table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: DELETE(url: string; headers: table<string, string>; block: block<(HttpResponse?):void>)

----


GET
^^^

.. _function-dashv_GET_string_table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: GET(url: string; headers: table<string, string>; block: block<(HttpResponse?):void>)

Registers a GET route handler, or performs an HTTP GET client request.


:Arguments: * **url** : string implicit

            * **headers** : table<string;string> implicit

            * **block** : block<( :ref:`HttpResponse <handle-dashv-HttpResponse>`?):void> implicit

.. _function-dashv_GET_string_block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: GET(url: string; block: block<(HttpResponse?):void>)

.. _function-dashv_GET_WebSocketServer_q__string_lambda_ls__c_void_gr_:

.. das:function:: GET(server: WebSocketServer?; url: string; lambda: lambda<():void>)

----


HEAD
^^^^

.. _function-dashv_HEAD_WebSocketServer_q__string_lambda_ls__c_void_gr_:

.. das:function:: HEAD(server: WebSocketServer?; url: string; lambda: lambda<():void>)

.. warning::
  This is unsafe operation.

Registers a HEAD route handler, or performs an HTTP HEAD client request.


:Arguments: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? implicit

            * **url** : string implicit

            * **lambda** : lambda<void>

.. _function-dashv_HEAD_string_block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: HEAD(url: string; block: block<(HttpResponse?):void>)

.. _function-dashv_HEAD_string_table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: HEAD(url: string; headers: table<string, string>; block: block<(HttpResponse?):void>)

----


PATCH
^^^^^

.. _function-dashv_PATCH_WebSocketServer_q__string_lambda_ls__c_void_gr_:

.. das:function:: PATCH(server: WebSocketServer?; url: string; lambda: lambda<():void>)

.. warning::
  This is unsafe operation.

Registers a PATCH route handler, or performs an HTTP PATCH client request.


:Arguments: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? implicit

            * **url** : string implicit

            * **lambda** : lambda<void>

.. _function-dashv_PATCH_string_string_block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: PATCH(url: string; text: string; block: block<(HttpResponse?):void>)

.. _function-dashv_PATCH_string_string_table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: PATCH(url: string; text: string; headers: table<string, string>; block: block<(HttpResponse?):void>)

.. _function-dashv_PATCH_string_string_table_ls_string,_string_gr__table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: PATCH(url: string; text: string; headers: table<string, string>; from: table<string, string>; block: block<(HttpResponse?):void>)

----


POST
^^^^

.. _function-dashv_POST_string_string_table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: POST(url: string; text: string; headers: table<string, string>; block: block<(HttpResponse?):void>)

Registers a POST route handler, or performs an HTTP POST client request.


:Arguments: * **url** : string implicit

            * **text** : string implicit

            * **headers** : table<string;string> implicit

            * **block** : block<( :ref:`HttpResponse <handle-dashv-HttpResponse>`?):void> implicit

.. _function-dashv_POST_string_string_block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: POST(url: string; text: string; block: block<(HttpResponse?):void>)

.. _function-dashv_POST_string_string_table_ls_string,_string_gr__table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: POST(url: string; text: string; headers: table<string, string>; from: table<string, string>; block: block<(HttpResponse?):void>)

.. _function-dashv_POST_WebSocketServer_q__string_lambda_ls__c_void_gr_:

.. das:function:: POST(server: WebSocketServer?; url: string; lambda: lambda<():void>)

----


PUT
^^^

.. _function-dashv_PUT_string_string_block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: PUT(url: string; text: string; block: block<(HttpResponse?):void>)

Registers a PUT route handler, or performs an HTTP PUT client request.


:Arguments: * **url** : string implicit

            * **text** : string implicit

            * **block** : block<( :ref:`HttpResponse <handle-dashv-HttpResponse>`?):void> implicit

.. _function-dashv_PUT_string_string_table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: PUT(url: string; text: string; headers: table<string, string>; block: block<(HttpResponse?):void>)

.. _function-dashv_PUT_WebSocketServer_q__string_lambda_ls__c_void_gr_:

.. das:function:: PUT(server: WebSocketServer?; url: string; lambda: lambda<():void>)

.. _function-dashv_PUT_string_string_table_ls_string,_string_gr__table_ls_string,_string_gr__block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: PUT(url: string; text: string; headers: table<string, string>; from: table<string, string>; block: block<(HttpResponse?):void>)

----

.. _function-dashv_SSE_WebSocketServer_q__string_lambda_ls__c_void_gr_:

.. das:function:: SSE(server: WebSocketServer?; url: string; lambda: lambda<():void>)

.. warning::
  This is unsafe operation.

Registers an SSE (Server-Sent Events) handler on the server for the given URL path. Uses ``ANY`` method matching so both GET and POST requests reach the handler. The lambda receives the request and response and returns an HTTP status code.

:Arguments: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? implicit

            * **url** : string implicit

            * **lambda** : lambda<void>


+++++++++++++++++++++++++
HTTP server configuration
+++++++++++++++++++++++++

  *  :ref:`STATIC (server: WebSocketServer?; path: string; dir: string) <function-dashv_STATIC_WebSocketServer_q__string_string>`
  *  :ref:`allow_cors (server: WebSocketServer?) <function-dashv_allow_cors_WebSocketServer_q_>`
  *  :ref:`set_document_root (server: WebSocketServer?; dir: string) <function-dashv_set_document_root_WebSocketServer_q__string>`
  *  :ref:`set_error_page (server: WebSocketServer?; filename: string) <function-dashv_set_error_page_WebSocketServer_q__string>`
  *  :ref:`set_home_page (server: WebSocketServer?; filename: string) <function-dashv_set_home_page_WebSocketServer_q__string>`
  *  :ref:`set_index_of (server: WebSocketServer?; dir: string) <function-dashv_set_index_of_WebSocketServer_q__string>`

.. _function-dashv_STATIC_WebSocketServer_q__string_string:

.. das:function:: STATIC(server: WebSocketServer?; path: string; dir: string)

Serves static files from a directory under the given URL path prefix.


:Arguments: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? implicit

            * **path** : string implicit

            * **dir** : string implicit

.. _function-dashv_allow_cors_WebSocketServer_q_:

.. das:function:: allow_cors(server: WebSocketServer?)

Enables cross-origin resource sharing (CORS) on the server.


:Arguments: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? implicit

.. _function-dashv_set_document_root_WebSocketServer_q__string:

.. das:function:: set_document_root(server: WebSocketServer?; dir: string)

Sets the document root directory for static file serving.


:Arguments: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? implicit

            * **dir** : string implicit

.. _function-dashv_set_error_page_WebSocketServer_q__string:

.. das:function:: set_error_page(server: WebSocketServer?; filename: string)

Sets a custom error page filename.


:Arguments: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? implicit

            * **filename** : string implicit

.. _function-dashv_set_home_page_WebSocketServer_q__string:

.. das:function:: set_home_page(server: WebSocketServer?; filename: string)

Sets the default home page filename (e.g., `index.html`).


:Arguments: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? implicit

            * **filename** : string implicit

.. _function-dashv_set_index_of_WebSocketServer_q__string:

.. das:function:: set_index_of(server: WebSocketServer?; dir: string)

Enables directory listing for the specified directory.


:Arguments: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? implicit

            * **dir** : string implicit


+++++++++++++++++++++
HTTP response helpers
+++++++++++++++++++++

  *  :ref:`DATA (response: HttpResponse?; data: string; length: int; status: http_status = dashv::http_status.OK) : http_status <function-dashv_DATA_HttpResponse_q__string_int_http_status>`
  *  :ref:`JSON (response: HttpResponse?; json_string: string; status: http_status = dashv::http_status.OK) : http_status <function-dashv_JSON_HttpResponse_q__string_http_status>`
  *  :ref:`REDIRECT (response: HttpResponse?; location: string; status: http_status) : http_status <function-dashv_REDIRECT_HttpResponse_q__string_http_status>`
  *  :ref:`SERVE_FILE (response: HttpResponse?; filepath: string) : http_status <function-dashv_SERVE_FILE_HttpResponse_q__string>`
  *  :ref:`TEXT_PLAIN (response: HttpResponse?; text: string; status: http_status = dashv::http_status.OK) : http_status <function-dashv_TEXT_PLAIN_HttpResponse_q__string_http_status>`
  *  :ref:`set_content_type (response: HttpResponse?; content_type: string) <function-dashv_set_content_type_HttpResponse_q__string>`
  *  :ref:`set_content_type (request: HttpRequest?; content_type: string) <function-dashv_set_content_type_HttpRequest_q__string>`
  *  :ref:`set_header (response: HttpResponse?; key: string; value: string) <function-dashv_set_header_HttpResponse_q__string_string>`
  *  :ref:`set_header (request: HttpRequest?; key: string; value: string) <function-dashv_set_header_HttpRequest_q__string_string>`

.. _function-dashv_DATA_HttpResponse_q__string_int_http_status:

.. das:function:: DATA(response: HttpResponse?; data: string; length: int; status: http_status = dashv::http_status.OK) : http_status

Sends raw data as the response body with the given status code.


:Arguments: * **response** :  :ref:`HttpResponse <handle-dashv-HttpResponse>`? implicit

            * **data** : string implicit

            * **length** : int

            * **status** :  :ref:`http_status <enum-dashv-http_status>`

.. _function-dashv_JSON_HttpResponse_q__string_http_status:

.. das:function:: JSON(response: HttpResponse?; json_string: string; status: http_status = dashv::http_status.OK) : http_status

Sends a JSON response with the given status code.


:Arguments: * **response** :  :ref:`HttpResponse <handle-dashv-HttpResponse>`? implicit

            * **json_string** : string implicit

            * **status** :  :ref:`http_status <enum-dashv-http_status>`

.. _function-dashv_REDIRECT_HttpResponse_q__string_http_status:

.. das:function:: REDIRECT(response: HttpResponse?; location: string; status: http_status) : http_status

Sends an HTTP redirect response to the specified location.


:Arguments: * **response** :  :ref:`HttpResponse <handle-dashv-HttpResponse>`? implicit

            * **location** : string implicit

            * **status** :  :ref:`http_status <enum-dashv-http_status>`

.. _function-dashv_SERVE_FILE_HttpResponse_q__string:

.. das:function:: SERVE_FILE(response: HttpResponse?; filepath: string) : http_status

Sends a file as the response body, setting the content type automatically.


:Arguments: * **response** :  :ref:`HttpResponse <handle-dashv-HttpResponse>`? implicit

            * **filepath** : string implicit

.. _function-dashv_TEXT_PLAIN_HttpResponse_q__string_http_status:

.. das:function:: TEXT_PLAIN(response: HttpResponse?; text: string; status: http_status = dashv::http_status.OK) : http_status

Sends a plain-text response with the given status code.


:Arguments: * **response** :  :ref:`HttpResponse <handle-dashv-HttpResponse>`? implicit

            * **text** : string implicit

            * **status** :  :ref:`http_status <enum-dashv-http_status>`


set_content_type
^^^^^^^^^^^^^^^^

.. _function-dashv_set_content_type_HttpResponse_q__string:

.. das:function:: set_content_type(response: HttpResponse?; content_type: string)

Sets the Content-Type header on a response or request.


:Arguments: * **response** :  :ref:`HttpResponse <handle-dashv-HttpResponse>`? implicit

            * **content_type** : string implicit

.. _function-dashv_set_content_type_HttpRequest_q__string:

.. das:function:: set_content_type(request: HttpRequest?; content_type: string)

----


set_header
^^^^^^^^^^

.. _function-dashv_set_header_HttpResponse_q__string_string:

.. das:function:: set_header(response: HttpResponse?; key: string; value: string)

Sets a response or request header.


:Arguments: * **response** :  :ref:`HttpResponse <handle-dashv-HttpResponse>`? implicit

            * **key** : string implicit

            * **value** : string implicit

.. _function-dashv_set_header_HttpRequest_q__string_string:

.. das:function:: set_header(request: HttpRequest?; key: string; value: string)


++++++++++++++++++++
HTTP client requests
++++++++++++++++++++

  *  :ref:`request (request: HttpRequest?; block: block\<(HttpResponse?):void\>) <function-dashv_request_HttpRequest_q__block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`request_cb (request: HttpRequest?; on_body: block\<(uint8 const?;int):void\>; on_complete: block\<(HttpResponse?):void\>) <function-dashv_request_cb_HttpRequest_q__block_ls_uint8_const_q_;int_c_void_gr__block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`request_cb (request: HttpRequest?; on_body: block\<(string):void\>; on_complete: block\<(HttpResponse?):void\>) <function-dashv_request_cb_HttpRequest_q__block_ls_string_c_void_gr__block_ls_HttpResponse_q__c_void_gr_>`
  *  :ref:`status_message (response: HttpResponse?) : string <function-dashv_status_message_HttpResponse_q_>`

.. _function-dashv_request_HttpRequest_q__block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: request(request: HttpRequest?; block: block<(HttpResponse?):void>)

Sends an HTTP request configured via an `HttpRequest` object and invokes the block with the response.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **block** : block<( :ref:`HttpResponse <handle-dashv-HttpResponse>`?):void> implicit


request_cb
^^^^^^^^^^

.. _function-dashv_request_cb_HttpRequest_q__block_ls_uint8_const_q_;int_c_void_gr__block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: request_cb(request: HttpRequest?; on_body: block<(uint8 const?;int):void>; on_complete: block<(HttpResponse?):void>)

Sends an HTTP request and invokes ``on_body`` with raw bytes (``uint8?`` pointer and size) for each body chunk as it arrives, then calls ``on_complete`` with the final response. Use this for binary streaming or when exact byte counts are needed.

:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **on_body** : block<(uint8?;int):void> implicit

            * **on_complete** : block<( :ref:`HttpResponse <handle-dashv-HttpResponse>`?):void> implicit

.. _function-dashv_request_cb_HttpRequest_q__block_ls_string_c_void_gr__block_ls_HttpResponse_q__c_void_gr_:

.. das:function:: request_cb(request: HttpRequest?; on_body: block<(string):void>; on_complete: block<(HttpResponse?):void>)

----

.. _function-dashv_status_message_HttpResponse_q_:

.. das:function:: status_message(response: HttpResponse?) : string

Returns the status message string for the given HTTP response.


:Arguments: * **response** :  :ref:`HttpResponse <handle-dashv-HttpResponse>`? implicit


+++++++++++++++++++++++++
Message and header access
+++++++++++++++++++++++++

  *  :ref:`HttpResponse.content_length (self: HttpResponse) : int <function-dashv__dot__rq_content_length_HttpResponse>`
  *  :ref:`each_header (message: HttpMessage?; block: block\<(string;string):void\>) <function-dashv_each_header_HttpMessage_q__block_ls_string;string_c_void_gr_>`
  *  :ref:`get_header (message: HttpMessage?; key: string) : string <function-dashv_get_header_HttpMessage_q__string>`

.. _function-dashv__dot__rq_content_length_HttpResponse:

.. das:function:: HttpResponse.content_length(self: HttpResponse) : int

Returns the content length of the HTTP response body.


:Arguments: * **self** :  :ref:`HttpResponse <handle-dashv-HttpResponse>` implicit

.. _function-dashv_each_header_HttpMessage_q__block_ls_string;string_c_void_gr_:

.. das:function:: each_header(message: HttpMessage?; block: block<(string;string):void>)

Iterates over all headers of an HTTP message, invoking the block with each key-value pair.


:Arguments: * **message** :  :ref:`HttpMessage <handle-dashv-HttpMessage>`? implicit

            * **block** : block<(string;string):void> implicit

.. _function-dashv_get_header_HttpMessage_q__string:

.. das:function:: get_header(message: HttpMessage?; key: string) : string

Returns the value of a specific HTTP header from a message.


:Arguments: * **message** :  :ref:`HttpMessage <handle-dashv-HttpMessage>`? implicit

            * **key** : string implicit


+++++++++++++++++++++
Request configuration
+++++++++++++++++++++

  *  :ref:`allow_redirect (request: HttpRequest?; on: bool) <function-dashv_allow_redirect_HttpRequest_q__bool>`
  *  :ref:`each_param (request: HttpRequest?; block: block\<(string;string):void\>) <function-dashv_each_param_HttpRequest_q__block_ls_string;string_c_void_gr_>`
  *  :ref:`get_param (request: HttpRequest?; key: string) : string <function-dashv_get_param_HttpRequest_q__string>`
  *  :ref:`set_basic_auth (request: HttpRequest?; username: string; password: string) <function-dashv_set_basic_auth_HttpRequest_q__string_string>`
  *  :ref:`set_bearer_token_auth (request: HttpRequest?; token: string) <function-dashv_set_bearer_token_auth_HttpRequest_q__string>`
  *  :ref:`set_connect_timeout (request: HttpRequest?; seconds: int) <function-dashv_set_connect_timeout_HttpRequest_q__int>`
  *  :ref:`set_param (request: HttpRequest?; key: string; value: string) <function-dashv_set_param_HttpRequest_q__string_string>`
  *  :ref:`set_timeout (request: HttpRequest?; seconds: int) <function-dashv_set_timeout_HttpRequest_q__int>`

.. _function-dashv_allow_redirect_HttpRequest_q__bool:

.. das:function:: allow_redirect(request: HttpRequest?; on: bool)

Enables or disables automatic following of HTTP redirects.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **on** : bool

.. _function-dashv_each_param_HttpRequest_q__block_ls_string;string_c_void_gr_:

.. das:function:: each_param(request: HttpRequest?; block: block<(string;string):void>)

Iterates over all query parameters of an HTTP request, invoking the block with each key-value pair.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **block** : block<(string;string):void> implicit

.. _function-dashv_get_param_HttpRequest_q__string:

.. das:function:: get_param(request: HttpRequest?; key: string) : string

Returns the value of a query parameter from an HTTP request.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **key** : string implicit

.. _function-dashv_set_basic_auth_HttpRequest_q__string_string:

.. das:function:: set_basic_auth(request: HttpRequest?; username: string; password: string)

Sets HTTP Basic authentication credentials on a request.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **username** : string implicit

            * **password** : string implicit

.. _function-dashv_set_bearer_token_auth_HttpRequest_q__string:

.. das:function:: set_bearer_token_auth(request: HttpRequest?; token: string)

Sets a Bearer token authentication header on a request.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **token** : string implicit

.. _function-dashv_set_connect_timeout_HttpRequest_q__int:

.. das:function:: set_connect_timeout(request: HttpRequest?; seconds: int)

Sets the connection timeout in seconds.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **seconds** : int

.. _function-dashv_set_param_HttpRequest_q__string_string:

.. das:function:: set_param(request: HttpRequest?; key: string; value: string)

Sets a query parameter on an HTTP request.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **key** : string implicit

            * **value** : string implicit

.. _function-dashv_set_timeout_HttpRequest_q__int:

.. das:function:: set_timeout(request: HttpRequest?; seconds: int)

Sets the overall request timeout in seconds.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **seconds** : int


+++++++
Cookies
+++++++

  *  :ref:`add_cookie (request: HttpRequest?; name: string; value: string; domain: string; path: string; max_age: int; secure: bool; httponly: bool) <function-dashv_add_cookie_HttpRequest_q__string_string_string_string_int_bool_bool>`
  *  :ref:`add_cookie (request: HttpRequest?; name: string; value: string) <function-dashv_add_cookie_HttpRequest_q__string_string>`
  *  :ref:`add_cookie (response: HttpResponse?; name: string; value: string; domain: string; path: string; max_age: int; secure: bool; httponly: bool) <function-dashv_add_cookie_HttpResponse_q__string_string_string_string_int_bool_bool>`
  *  :ref:`add_cookie (response: HttpResponse?; name: string; value: string) <function-dashv_add_cookie_HttpResponse_q__string_string>`
  *  :ref:`each_cookie (response: HttpResponse?; block: block\<(string;string):void\>) <function-dashv_each_cookie_HttpResponse_q__block_ls_string;string_c_void_gr_>`
  *  :ref:`each_cookie (request: HttpRequest?; block: block\<(string;string):void\>) <function-dashv_each_cookie_HttpRequest_q__block_ls_string;string_c_void_gr_>`
  *  :ref:`get_cookie (request: HttpRequest?; name: string) : string <function-dashv_get_cookie_HttpRequest_q__string>`
  *  :ref:`get_cookie (response: HttpResponse?; name: string) : string <function-dashv_get_cookie_HttpResponse_q__string>`


add_cookie
^^^^^^^^^^

.. _function-dashv_add_cookie_HttpRequest_q__string_string_string_string_int_bool_bool:

.. das:function:: add_cookie(request: HttpRequest?; name: string; value: string; domain: string; path: string; max_age: int; secure: bool; httponly: bool)

Adds a cookie to a request or response, optionally with domain, path, max-age, secure, and httponly flags.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **name** : string implicit

            * **value** : string implicit

            * **domain** : string implicit

            * **path** : string implicit

            * **max_age** : int

            * **secure** : bool

            * **httponly** : bool

.. _function-dashv_add_cookie_HttpRequest_q__string_string:

.. das:function:: add_cookie(request: HttpRequest?; name: string; value: string)

.. _function-dashv_add_cookie_HttpResponse_q__string_string_string_string_int_bool_bool:

.. das:function:: add_cookie(response: HttpResponse?; name: string; value: string; domain: string; path: string; max_age: int; secure: bool; httponly: bool)

.. _function-dashv_add_cookie_HttpResponse_q__string_string:

.. das:function:: add_cookie(response: HttpResponse?; name: string; value: string)

----


each_cookie
^^^^^^^^^^^

.. _function-dashv_each_cookie_HttpResponse_q__block_ls_string;string_c_void_gr_:

.. das:function:: each_cookie(response: HttpResponse?; block: block<(string;string):void>)

Iterates over all cookies, invoking the block with each name-value pair.


:Arguments: * **response** :  :ref:`HttpResponse <handle-dashv-HttpResponse>`? implicit

            * **block** : block<(string;string):void> implicit

.. _function-dashv_each_cookie_HttpRequest_q__block_ls_string;string_c_void_gr_:

.. das:function:: each_cookie(request: HttpRequest?; block: block<(string;string):void>)

----


get_cookie
^^^^^^^^^^

.. _function-dashv_get_cookie_HttpRequest_q__string:

.. das:function:: get_cookie(request: HttpRequest?; name: string) : string

Returns the value of a cookie by name from a request.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **name** : string implicit

.. _function-dashv_get_cookie_HttpResponse_q__string:

.. das:function:: get_cookie(response: HttpResponse?; name: string) : string


+++++++++
Form data
+++++++++

  *  :ref:`each_form_field (request: HttpRequest?; block: block\<(string;string;string):void\>) <function-dashv_each_form_field_HttpRequest_q__block_ls_string;string;string_c_void_gr_>`
  *  :ref:`get_form_data (request: HttpRequest?; name: string) : string <function-dashv_get_form_data_HttpRequest_q__string>`
  *  :ref:`get_url_encoded (request: HttpRequest?; key: string) : string <function-dashv_get_url_encoded_HttpRequest_q__string>`
  *  :ref:`save_form_file (request: HttpRequest?; name: string; path: string) : int <function-dashv_save_form_file_HttpRequest_q__string_string>`
  *  :ref:`set_form_data (request: HttpRequest?; name: string; value: string) <function-dashv_set_form_data_HttpRequest_q__string_string>`
  *  :ref:`set_form_file (request: HttpRequest?; name: string; filepath: string) <function-dashv_set_form_file_HttpRequest_q__string_string>`
  *  :ref:`set_url_encoded (request: HttpRequest?; key: string; value: string) <function-dashv_set_url_encoded_HttpRequest_q__string_string>`

.. _function-dashv_each_form_field_HttpRequest_q__block_ls_string;string;string_c_void_gr_:

.. das:function:: each_form_field(request: HttpRequest?; block: block<(string;string;string):void>)

Iterates over all form fields of a server-side request, invoking the block with name, filename, and content type.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **block** : block<(string;string;string):void> implicit

.. _function-dashv_get_form_data_HttpRequest_q__string:

.. das:function:: get_form_data(request: HttpRequest?; name: string) : string

Returns the value of a form field from a server-side request.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **name** : string implicit

.. _function-dashv_get_url_encoded_HttpRequest_q__string:

.. das:function:: get_url_encoded(request: HttpRequest?; key: string) : string

Returns the value of a URL-encoded form field from a server-side request.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **key** : string implicit

.. _function-dashv_save_form_file_HttpRequest_q__string_string:

.. das:function:: save_form_file(request: HttpRequest?; name: string; path: string) : int

Saves an uploaded form file to disk. Returns 0 on success.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **name** : string implicit

            * **path** : string implicit

.. _function-dashv_set_form_data_HttpRequest_q__string_string:

.. das:function:: set_form_data(request: HttpRequest?; name: string; value: string)

Sets a form field value on a multipart request.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **name** : string implicit

            * **value** : string implicit

.. _function-dashv_set_form_file_HttpRequest_q__string_string:

.. das:function:: set_form_file(request: HttpRequest?; name: string; filepath: string)

Attaches a file to a multipart request.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **name** : string implicit

            * **filepath** : string implicit

.. _function-dashv_set_url_encoded_HttpRequest_q__string_string:

.. das:function:: set_url_encoded(request: HttpRequest?; key: string; value: string)

Sets a URL-encoded form field on a request.


:Arguments: * **request** :  :ref:`HttpRequest <handle-dashv-HttpRequest>`? implicit

            * **key** : string implicit

            * **value** : string implicit


+++++++++++++++++++
SSE response writer
+++++++++++++++++++

  *  :ref:`close_writer (writer: HttpResponseWriter?) : int <function-dashv_close_writer_HttpResponseWriter_q_>`
  *  :ref:`end_headers (writer: HttpResponseWriter?; key: string; value: string) : int <function-dashv_end_headers_HttpResponseWriter_q__string_string>`
  *  :ref:`end_response (writer: HttpResponseWriter?) : int <function-dashv_end_response_HttpResponseWriter_q_>`
  *  :ref:`release_writer (server: WebSocketServer?; writer: HttpResponseWriter?) <function-dashv_release_writer_WebSocketServer_q__HttpResponseWriter_q_>`
  *  :ref:`sse_event (writer: HttpResponseWriter?; data: string; event: string) : int <function-dashv_sse_event_HttpResponseWriter_q__string_string>`
  *  :ref:`write_chunked (writer: HttpResponseWriter?; data: string; length: int) : int <function-dashv_write_chunked_HttpResponseWriter_q__string_int>`

.. _function-dashv_close_writer_HttpResponseWriter_q_:

.. das:function:: close_writer(writer: HttpResponseWriter?) : int

Closes the response writer's underlying connection. Use this to forcefully terminate a long-lived streaming connection. Returns 0 on success.

:Arguments: * **writer** :  :ref:`HttpResponseWriter <handle-dashv-HttpResponseWriter>`? implicit

.. _function-dashv_end_headers_HttpResponseWriter_q__string_string:

.. das:function:: end_headers(writer: HttpResponseWriter?; key: string; value: string) : int

Sends the response headers through the writer, adding one final header (key/value pair). Call this before writing body chunks or SSE events. Returns 0 on success.

:Arguments: * **writer** :  :ref:`HttpResponseWriter <handle-dashv-HttpResponseWriter>`? implicit

            * **key** : string implicit

            * **value** : string implicit

.. _function-dashv_end_response_HttpResponseWriter_q_:

.. das:function:: end_response(writer: HttpResponseWriter?) : int

Ends the chunked response by sending a zero-length terminating chunk. Call this after all data has been written to signal the end of the response. Returns 0 on success.

:Arguments: * **writer** :  :ref:`HttpResponseWriter <handle-dashv-HttpResponseWriter>`? implicit

.. _function-dashv_release_writer_WebSocketServer_q__HttpResponseWriter_q_:

.. das:function:: release_writer(server: WebSocketServer?; writer: HttpResponseWriter?)

Releases the server's reference to a response writer, allowing it to be destroyed. Call this after ``end_response`` or ``close_writer`` to free the writer's resources.

:Arguments: * **server** :  :ref:`WebSocketServer <handle-dashv-WebSocketServer>`? implicit

            * **writer** :  :ref:`HttpResponseWriter <handle-dashv-HttpResponseWriter>`? implicit

.. _function-dashv_sse_event_HttpResponseWriter_q__string_string:

.. das:function:: sse_event(writer: HttpResponseWriter?; data: string; event: string) : int

Sends an SSE event through the response writer. Formats the data and event type according to the SSE wire protocol. Returns 0 on success.

:Arguments: * **writer** :  :ref:`HttpResponseWriter <handle-dashv-HttpResponseWriter>`? implicit

            * **data** : string implicit

            * **event** : string implicit

.. _function-dashv_write_chunked_HttpResponseWriter_q__string_int:

.. das:function:: write_chunked(writer: HttpResponseWriter?; data: string; length: int) : int

Writes a chunk of data through the response writer using HTTP chunked transfer encoding. Returns 0 on success.

:Arguments: * **writer** :  :ref:`HttpResponseWriter <handle-dashv-HttpResponseWriter>`? implicit

            * **data** : string implicit

            * **length** : int


