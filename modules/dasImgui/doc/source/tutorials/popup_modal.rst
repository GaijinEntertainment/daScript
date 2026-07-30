.. _tutorial_popup_modal:

#######################
Modal popups
#######################

``popup_modal`` is the **blocking** sibling of ``popup``: ImGui dims the
parent window, absorbs every click outside the modal, and routes ESC to
the close path. The wrapper's signature:

.. code-block:: das

   popup_modal(IDENT, (text = "title",
                       closable = bool,
                       flags = ImGuiWindowFlags....)) {
       // body — runs only while ImGui has the modal open
   }

Lifecycle is identical to ``popup`` — ``OpenPopup`` + ``BeginPopupModal``
+ ``EndPopup``, driven by ``state.pending_open`` / ``pending_close``.
The differences are entirely visual + input-blocking, both handled by
ImGui.

``closable=true`` adds an X-button on the title bar wired to
``state.open``. The wrapper feeds ``&state.open`` to
``BeginPopupModal``'s ``p_open`` parameter, so an X-click sets
``state.open=false`` and ImGui closes the modal on the next frame.

Source: ``examples/tutorial/popup_modal.das``.

************
Walkthrough
************

.. video:: popup_modal.mp4

.. literalinclude:: ../../../examples/tutorial/popup_modal.das
   :language: das
   :linenos:

Requires
========

Already in the baseline boost layer:

* ``imgui/imgui_containers_builtin`` — ``popup_modal`` plus ``window``.
* ``imgui/imgui_widgets_builtin`` — ``button``, ``checkbox``, ``slider_int``,
  ``text``.

Confirm dialog (non-closable)
=============================

The classic "Yes/No before destructive action":

.. code-block:: das

   popup_modal(CONFIRM_MODAL, (text = "Confirm delete",
                               closable = false,
                               flags = ImGuiWindowFlags.AlwaysAutoResize)) {
       text("Are you sure you want to delete the selected file?")
       separator()
       if (button(CONFIRM_YES, (text = "Yes"))) {
           CONFIRM_RESULT = "Yes"
           CONFIRM_MODAL.pending_close = true
       }
       same_line()
       if (button(CONFIRM_NO, (text = "No"))) {
           CONFIRM_RESULT = "No"
           CONFIRM_MODAL.pending_close = true
       }
   }

``closable=false`` means there's no X-button — but ESC still closes the
modal (ImGui's built-in popup keybinding, unaffected by ``closable`` or
any ``ImGuiWindowFlags`` bit). The body's Yes/No buttons are the
explicit answer paths: both write to ``CONFIRM_RESULT`` and set
``pending_close``. To make the modal truly non-dismissable, the app must
intercept the ESC key before ``NewFrame()`` and avoid issuing any
``pending_close`` from app code — outside the scope of this tutorial.

The ``AlwaysAutoResize`` flag (passed via ``flags``) sizes the modal to
its content — saves the caller from picking width/height by hand.

Closable settings modal
=======================

When the user might want to close without "saving":

.. code-block:: das

   popup_modal(SETTINGS_MODAL, (text = "Settings",
                                closable = true,
                                flags = ImGuiWindowFlags.AlwaysAutoResize)) {
       checkbox(S_AUTOSAVE, (text = "Auto-save every 5 minutes"))
       checkbox(S_TELEMETRY, (text = "Send anonymous telemetry"))
       slider_int(S_MAX_FPS, (text = "Max FPS cap"))
       separator()
       if (button(S_APPLY, (text = "Apply"))) {
           SETTINGS_MODAL.pending_close = true
       }
   }

``closable=true`` activates the X-button. The wrapper passes
``&state.open`` to ImGui as ``p_open``; ImGui flips it false on
X-click or ESC, the next frame's ``BeginPopupModal`` returns false,
and the wrapper finalizes the close. ``SETTINGS_MODAL.open`` mirrors
ImGui's view so the snapshot reports whether the modal is currently up.

popup vs popup_modal
====================

Same machinery, different UX:

* ``popup`` — clicks outside the popup close it (auto-close behavior).
  Useful for dropdowns, context menus, transient widgets.
* ``popup_modal`` — clicks outside are **absorbed**. The user MUST
  resolve the modal before doing anything else. Use for destructive
  confirms, multi-step wizards, blocking error dialogs.

The two share ``PopupState`` exactly — same ``open`` / ``flags`` /
``pending_open`` / ``pending_close`` fields. Switching between forms
is a one-word edit.

Pending-flag channels
=====================

Three control surfaces converge on the same state:

* **App code** — ``MODAL.pending_open = true`` from a button handler
  or any event.
* **Live commands** — ``imgui_open`` / ``imgui_close`` against the
  registered path.
* **Close button / ESC** (closable only) — ImGui flips ``state.open``,
  the next frame's wrapper applies the close.

The wrapper bridges them: ``pending_open=true`` triggers ``OpenPopup``,
``pending_close=true`` triggers ``CloseCurrentPopup`` inside the active
modal body, X-click flips ``open=false`` and the wrapper closes on
next frame.

Standalone vs live
==================

Same convention as the other tutorials.

Driving from outside
====================

The walkthrough above opens each modal by clicking its trigger button and
dismisses it by clicking Yes / No / Apply — every gesture is a real click,
self-verified by the modal body appearing or vanishing. The live commands
below drive the same lifecycle directly, for remote or scripted control:

.. code-block:: bash

   curl -X POST -d '{"name":"imgui_open","args":{"target":"PM_WIN/CONFIRM_MODAL"}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_click","args":{"target":"PM_WIN/CONFIRM_MODAL/CONFIRM_YES"}}' \
        localhost:9090/command
   curl -X POST -d '{"name":"imgui_close","args":{"target":"PM_WIN/CONFIRM_MODAL"}}' \
        localhost:9090/command

The path for body widgets composes off the modal IDENT — clicks against
``PM_WIN/CONFIRM_MODAL/CONFIRM_YES`` resolve correctly only while the
modal is open (the body's widgets aren't in the registry otherwise).

.. seealso::

   Full source: :download:`examples/tutorial/popup_modal.das <../../../examples/tutorial/popup_modal.das>`

   Features-side demo: ``examples/features/containers_overlay.das`` —
   ``popup`` + ``popup_modal`` + ``tooltip`` overlay family showcase.

   Sibling: :ref:`tutorial_popup_window` — manual-trigger popup pattern
   for shared-str_id under PushID scopes.

   :ref:`Boost macros <stdlib_imgui_boost_section>` — the macro layer.
