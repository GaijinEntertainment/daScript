The boost DSL spine — ``[widget]`` and ``[container]`` function annotations.
Mark ``def myWidget(var state : ST, …)`` with ``[widget]`` and the macro
injects a ``widget_ident`` parameter, idempotently emits ``IDENT`` as a
module-scope state global at the first call site, parses the dotted-suffix
flag chain (``SAVE_BTN.PUBLIC``, ``RPS.NOTLIVE``), and rewrites the call to
``myWidget(IDENT, "IDENT", …)``. ``[container]`` is the block-arg sibling: same
identifier auto-emit, plus ``container_path_push(widget_ident)`` /
``container_path_pop()`` around ``invoke(blk)`` so leaf telemetry paths nest
into ``MAIN_WIN/SETTINGS_TAB/RPS``. UPPER_SNAKE_CASE identifiers by convention
(API_REWORK §4.5); auto-emit is module-scope only.

Begin/End pairing — which call paints always vs only-on-true — is NOT
abstracted; each container def authors its own ``Begin → invoke(blk) → End``
sequence so the imgui-irregular rules stay visible at the call site.
