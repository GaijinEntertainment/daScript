HTTP and WebSocket server with route registration and event callbacks.
Subclass and override ``onWsOpen``, ``onWsClose``, ``onWsMessage``,
``onTick``, and ``onInit`` as needed. Register HTTP routes with
``GET``, ``POST``, ``PUT``, ``DELETE``, ``PATCH``, ``HEAD``, or ``ANY``.
Called when a WebSocket client connects. Override to handle new connections.
Called when a WebSocket client disconnects. Override to handle disconnections.
Called when a WebSocket message is received. Override to handle messages.
Called each tick cycle. Override for periodic work.
Called during server initialization, after the low-level server is created. Override to register routes.
