options gen2
options indenting = 4
options no_unused_block_arguments = false
options no_unused_function_arguments = false

//! High-level WebSocket and HTTP wrappers for the ``dashv`` C++ module.
//!
//! Provides ``HvWebSocketClient`` for WebSocket client connections with
//! ``onOpen``, ``onClose``, ``onMessage``, and lossless ``onMessageFrame``
//! callbacks, and ``HvWebServer``
//! for HTTP/WebSocket servers with route registration and static file serving.
//! Also includes ``with_http_request`` and ``get_body_bytes`` helpers.

module dashv_boost shared public

require dashv public

require daslib/rtti


class HvWebSocketClient {
    //! WebSocket client with event-driven callbacks.
    //! Subclass and override ``onOpen``, ``onClose``, and ``onMessage``
    //! to handle connection lifecycle and incoming messages. Call
    //! ``process_event_que`` periodically to dispatch events.
    @safe_when_uninitialized
    client : WebSocketClient
    def init(url : string) : int {
        //! Connects to the given WebSocket URL. Returns 0 on success.
        client = make_web_socket_client(unsafe(addr(self)), class_info(self))
        return open(client, url)
    }
    def cleanup {
        //! Releases the underlying WebSocket client handle. Idempotent.
        if (is_alive(client)) {
            destroy_web_socket_client(client)
            client = default<WebSocketClient>
        }
    }
    def operator delete {
        cleanup()
    }
    def abstract onOpen : void
        //! Called when the WebSocket connection is established.
    def abstract onClose : void
        //! Called when the WebSocket connection is closed.
    def abstract onMessage(msg : string#) : void
        //! Called when a text message is received from the server.
    def onMessageFrame(msg : string#; byte_count : int; opcode : ws_opcode) {
        //! Called for every complete text or binary frame. ``byte_count`` is
        //! authoritative and preserves embedded zero bytes. The default keeps
        //! the original text-only contract by forwarding text frames.
        if (opcode == ws_opcode.WS_OPCODE_TEXT) onMessage(msg)
    }
    def is_connected : bool {
        //! Returns ``true`` if the WebSocket connection is currently open.
        return is_connected(client)
    }
    def process_event_que {
        //! Processes pending WebSocket events; must be called periodically.
        tick(client)
    }
    def send(text : string) {
        //! Sends a text message to the server.
        send(client, text)
    }
    def send_result(text : string) : int {
        //! Sends a text message and returns the native transport result.
        return send(client, text)
    }
    def close : int {
        //! Closes the WebSocket connection. Returns 0 on success.
        return close(client)
    }
}

class HvWebServer {
    //! HTTP and WebSocket server with route registration and event callbacks.
    //! Subclass and override ``onWsOpen``, ``onWsClose``, ``onWsMessage``,
    //! ``onTick``, and ``onInit`` as needed. Register HTTP routes with
    //! ``GET``, ``POST``, ``PUT``, ``DELETE``, ``PATCH``, ``HEAD``, or ``ANY``.
    @safe_when_uninitialized
    server : WebSocketServer
    private http_route_handlers : array<lambda<(var req : HttpRequest?; var resp : HttpResponse?) : http_status>>
    private stream_route_handlers : array<lambda<(var req : HttpRequest?; var writer : HttpResponseWriter?) : void>>
    def init(port : int) {
        //! Initializes an HTTP/WebSocket server on the specified port.
        server = make_web_socket_server(port, 0, "", unsafe(addr(self)), class_info(self))
        onInit()
    }
    def init_wss(port, httpsPort : int; pathToCert : string = "") {
        //! Initializes an HTTPS/WSS server with TLS. Uses the default ``modules/dasHV/cert``
        //! directory if ``pathToCert`` is empty.
        let certPath = !empty(pathToCert) ? pathToCert : get_das_root() + "/modules/dasHV/cert"
        server = make_web_socket_server(port, httpsPort, certPath, unsafe(addr(self)), class_info(self))
        onInit()
    }
    def cleanup {
        //! Releases the underlying WebSocket server handle. Idempotent.
        if (is_alive(server)) {
            destroy_web_socket_server(server)
            server = default<WebSocketServer>
        }
        http_route_handlers |> clear()
        stream_route_handlers |> clear()
    }
    def operator delete {
        cleanup()
    }
    def start : int {
        //! Starts the server. Returns 0 on success.
        return start(server)
    }
    def set_bind_host(host : string) : bool {
        //! Selects the interface to bind before ``start`` (for example, ``127.0.0.1``).
        return set_bind_host(server, host)
    }
    def stop : int {
        //! Stops the server. Returns 0 on success.
        return stop(server)
    }
    def tick : void {
        //! Processes pending HTTP and WebSocket events; must be called periodically.
        tick(server)
    }
    def abstract onWsOpen(channel : WebSocketChannel; url : string#) : void
        //! Called when a WebSocket client connects. Override to handle new connections.
    def abstract onWsClose(channel : WebSocketChannel) : void
        //! Called when a WebSocket client disconnects. Override to handle disconnections.
    def abstract onWsMessage(channel : WebSocketChannel; msg : string#) : void
        //! Called when a WebSocket message is received. Override to handle messages.
    def abstract onTick : void
        //! Called each tick cycle. Override for periodic work.
    def onInit : void {}
        //! Called during server initialization, after the low-level server is created. Override to register routes.
    def GET(uri : string; lmb : lambda<(var req : HttpRequest?; var resp : HttpResponse?) : http_status>) : void {
        //! Registers a handler for HTTP GET requests matching ``uri``.
        http_route_handlers |> push(lmb)
        unsafe(GET(server, uri, lmb))
    }
    def POST(uri : string; lmb : lambda<(var req : HttpRequest?; var resp : HttpResponse?) : http_status>) : void {
        //! Registers a handler for HTTP POST requests matching ``uri``.
        http_route_handlers |> push(lmb)
        unsafe(POST(server, uri, lmb))
    }
    def PUT(uri : string; lmb : lambda<(var req : HttpRequest?; var resp : HttpResponse?) : http_status>) : void {
        //! Registers a handler for HTTP PUT requests matching ``uri``.
        http_route_handlers |> push(lmb)
        unsafe(PUT(server, uri, lmb))
    }
    def DELETE(uri : string; lmb : lambda<(var req : HttpRequest?; var resp : HttpResponse?) : http_status>) : void {
        //! Registers a handler for HTTP DELETE requests matching ``uri``.
        http_route_handlers |> push(lmb)
        unsafe(DELETE(server, uri, lmb))
    }
    def PATCH(uri : string; lmb : lambda<(var req : HttpRequest?; var resp : HttpResponse?) : http_status>) : void {
        //! Registers a handler for HTTP PATCH requests matching ``uri``.
        http_route_handlers |> push(lmb)
        unsafe(PATCH(server, uri, lmb))
    }
    def HEAD(uri : string; lmb : lambda<(var req : HttpRequest?; var resp : HttpResponse?) : http_status>) : void {
        //! Registers a handler for HTTP HEAD requests matching ``uri``.
        http_route_handlers |> push(lmb)
        unsafe(HEAD(server, uri, lmb))
    }
    def ANY(uri : string; lmb : lambda<(var req : HttpRequest?; var resp : HttpResponse?) : http_status>) : void {
        //! Registers a handler for any HTTP method matching ``uri``.
        http_route_handlers |> push(lmb)
        unsafe(ANY(server, uri, lmb))
    }
    def STATIC(path, dir : string) : void {
        //! Serves static files from ``dir`` under the URL prefix ``path``.
        STATIC(server, path, dir)
    }
    def allow_cors() : void {
        //! Enables cross-origin resource sharing (CORS) on the server.
        allow_cors(server)
    }
    def set_document_root(dir : string) : void {
        //! Sets the document root directory for static file serving.
        set_document_root(server, dir)
    }
    def set_home_page(filename : string) : void {
        //! Sets the default home page file (e.g., ``index.html``).
        set_home_page(server, filename)
    }
    def set_index_of(dir : string) : void {
        //! Enables directory listing for the specified directory.
        set_index_of(server, dir)
    }
    def set_error_page(filename : string) : void {
        //! Sets a custom error page file.
        set_error_page(server, filename)
    }
    def SSE(uri : string; lmb : lambda<(var req : HttpRequest?; var resp : HttpResponse?) : http_status>) : void {
        //! Registers an SSE (Server-Sent Events) handler for ``uri``.
        //! Same signature as GET/POST — set SSE headers and write events to the response.
        //! Uses ``ANY`` method matching so any HTTP method reaches this handler.
        http_route_handlers |> push(lmb)
        unsafe(SSE(server, uri, lmb))
    }
    def STREAM(uri : string; lmb : lambda<(var req : HttpRequest?; var writer : HttpResponseWriter?) : void>) : void {
        //! Registers a streaming (incremental) handler for ``uri`` (any HTTP method, like ``ANY``).
        //! Unlike GET/POST, the handler receives a live ``HttpResponseWriter`` and runs on the tick
        //! thread; drive it with ``respond`` (whole body) or ``sse_event`` + ``close_writer`` (Server-
        //! Sent Events) — each call marshals the actual socket write onto the server's event loop, so
        //! tokens flush to the client as they are produced. Pass ``server`` (the field) to those ops.
        stream_route_handlers |> push(lmb)
        unsafe(STREAM(server, uri, lmb))
    }
}

def with_http_request(blk : block<(var req : HttpRequest?#) : void>) {
    //! Creates a temporary ``HttpRequest``, invokes the block, then cleans up.
    //! Use this to configure and send outbound HTTP requests.
    var req = new HttpRequest
    unsafe {
        invoke(blk, reinterpret<HttpRequest?#>(req))
        delete req
    }
}

def get_body_bytes(resp : HttpResponse?) : array<uint8> {
    //! Extracts the response body as an ``array<uint8>``.
    //! Returns an empty array if the response is null, non-OK, or has no content.
    var bytes : array<uint8>
    if (resp == null || resp.status_code != http_status.OK || resp.content_length == 0) return <- bytes
    bytes |> resize(resp.content_length)
    peek(resp.body) $(bodybytes) {
        unsafe {
            memcpy(addr(bytes[0]), reinterpret<void?>(bodybytes), resp.content_length)
        }
    }
    return <- bytes
}
