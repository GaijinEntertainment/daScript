Lambda-typed transport for dasImgui live commands. ``Transport`` is a
closure ``(action, payload) → JsonValue?``; ``live_api_transport(url)``
returns a transport that POSTs ``{"name": action, "args": payload}`` to
``<url>/command`` and parses the JSON response. Users substitute mocks,
in-process buses, or alternative transports without changing call sites.

Wait helpers — ``await_imgui``, ``await_imgui_frame``, ``send_with_await``
— drive client-side polling against the host's ``imgui_await`` endpoint.
Server-side blocking is incompatible with the GLFW-main-thread invariant
(the dispatch handler runs on the same thread that advances frames), so
waits are always client-side; ``send_with_await`` dispatches on
``await_frames`` / ``await_until: "quiescent"`` / ``fire_and_forget`` and
default-polls quiescence otherwise.
