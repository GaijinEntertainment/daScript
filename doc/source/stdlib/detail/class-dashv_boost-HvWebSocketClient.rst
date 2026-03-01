WebSocket client with event-driven callbacks.
Subclass and override ``onOpen``, ``onClose``, and ``onMessage``
to handle connection lifecycle and incoming messages. Call
``process_event_que`` periodically to dispatch events.
Called when the WebSocket connection is established.
Called when the WebSocket connection is closed.
Called when a text message is received from the server.
