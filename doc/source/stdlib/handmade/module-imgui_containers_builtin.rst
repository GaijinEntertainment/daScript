Built-in containers — block-arg wrappers around ImGui's ``Begin``/``End``
family. Annotated with ``[container]`` so the body is wrapped with
``container_path_push(widget_ident)`` / ``container_path_pop()`` automatically,
giving leaf widgets nested registry keys (e.g.
``MAIN_WIN/SETTINGS_TAB/RPS``). Covers windows (``window``, ``child``,
``group``), tab bars + items (``tab_bar``, ``tab_item``), trees
(``tree_node``, ``collapsing_header``), menus (``menu_bar``, ``menu``,
``menu_item``), and popups (``popup``, ``modal``, ``tooltip``).

The ``Begin``/``End`` pairing rules differ per kind — ``Begin``/``End`` and
``BeginChild``/``EndChild`` always pair; everything else is End-only-on-true
— so each container def authors its own pairing inline rather than the
macro guessing.
