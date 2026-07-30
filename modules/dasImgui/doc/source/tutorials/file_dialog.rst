.. _tutorial_file_dialog:

#######################
file_dialog
#######################

A native, reusable **New / Open / Save / Pick-Folder** file dialog, built entirely
from the boost DSL and living in ``imgui/imgui_file_dialog``. Because it is composed from
boost widgets (``popup_modal`` / ``data_table`` / ``selectable_label`` /
``input_text`` / ``combo`` / ``button``) rather than a bound C++ file-dialog
library, **every element registers on the snapshot registry** — the whole dialog
is :ref:`imgui_playwright <tutorial_driving_outside>`-drivable (a bound C++
dialog renders raw ImGui and would be invisible to it).

It mirrors the core UX of a desktop file picker: a clickable directory breadcrumb
plus ``..`` and an **editable path bar** (type a path or a different drive, press
*Enter*), a **New Folder** button, a sortable Name / Size / Type / **Modified**
table, a **multi-extension** filter dropdown, multi-select in open mode, a filename
field (with an optional pre-filled default) in save / create mode, an
overwrite-confirm step, a **directory-pick** mode, and keyboard shortcuts
(*Enter* confirms, *Backspace* goes up a directory).

Source: ``examples/tutorial/file_dialog.das``.

.. video:: file_dialog.mp4

The recording opens the dialog in Save mode: the name field is pre-filled from
``default_name``, the breadcrumb and editable path bar sit above a sortable
Name / Size / Type / Modified table, the Images filter matches several extensions
at once, and New Folder makes a directory in place.

************
Walkthrough
************

The dialog is **immediate-mode** — there are no stored callbacks (daslang blocks
can't be stored across frames). The flow is three calls:

1. a trigger (button / menu item) calls ``file_dialog_open(cfg)`` with a mode,
   a start directory and a list of extension filters;
2. ``file_dialog_draw()`` is called **every frame**, at the top level of
   ``update()`` (outside any window, so the registry path stays a clean
   ``FILEDLG/...``). It renders the modal when open and returns this frame's
   ``FileDialogResult``;
3. on ``confirmed``, the chosen path(s) are read with
   ``file_dialog_selection()`` / ``file_dialog_selected_path()``.

.. literalinclude:: ../../../examples/tutorial/file_dialog.das
   :language: das
   :linenos:

Requires
========

One module on top of the harness:

* ``imgui/imgui_file_dialog`` — the dialog component. It pulls in
  ``imgui_table_builtin`` (the sortable ``data_table``),
  ``imgui_widgets_builtin`` and ``imgui_containers_builtin`` (the leaf widgets +
  ``popup_modal``), and ``daslib/fio`` (directory listing) transitively.

Public API
==========

.. code-block:: das

   enum FileDialogResult { none; confirmed; cancelled }
   enum FileDialogMode   { open; save; create; pick_dir }   // 'new' is a reserved keyword

   struct FileDialogFilter { caption : string; ext : string }  // ext: ';'-separated ("png;jpg"); "" = all
   struct FileDialogConfig {
       title : string = "Select File"
       start_dir : string = ""                   // "" => current working directory
       filters : array<FileDialogFilter>         // empty => all files
       mode : FileDialogMode = FileDialogMode.open
       max_selection : int = 1                   // open mode; 0 = unlimited
       default_name : string = ""                // save/create: pre-filled filename
   }

   def public file_dialog_open(cfg : FileDialogConfig) : void
   def public file_dialog_draw() : FileDialogResult
   def public file_dialog_selection() : array<string>   // confirmed absolute path(s)
   def public file_dialog_selected_path() : string      // first selection (convenience)
   def public file_dialog_current_dir() : string
   def public file_dialog_is_open() : bool
   def public file_dialog_last_dir() : string                 // last confirmed folder this session
   def public file_dialog_set_last_dir(dir : string) : void   // seed it (persist across runs)
   // convenience: parse a C#-style "caption|ext;ext|caption|ext|..." pipe spec
   def public file_filters(spec : string) : array<FileDialogFilter>

The result is polled, not pushed: ``file_dialog_draw()`` returns ``confirmed``
exactly on the frame the user commits, ``cancelled`` on the frame they close /
press *Escape*, and ``none`` otherwise. The accessors stay valid from the
``confirmed`` frame until the dialog is reopened.

A blank ``start_dir`` opens the dialog at the last folder a selection was
confirmed in this session, then the working directory. ``file_dialog_set_last_dir``
seeds that folder (e.g. from saved app config on startup) and
``file_dialog_last_dir`` reads it back to persist across runs; an explicit
``start_dir`` always wins.

Modes
=====

* **open** — pick one or more existing files. Selection follows the usual
  modifiers: plain click selects, **Ctrl+click** toggles, **Shift+click** selects
  a range, clamped to ``max_selection`` (``0`` = unlimited). Double-clicking a
  file confirms it; double-clicking a directory enters it.
* **save** / **create** — type a name in the filename field (clicking a row fills
  it; ``default_name`` pre-fills it on open). The active filter's first extension
  is appended if missing. If the name already exists, an **overwrite-confirm**
  sub-modal (``FILEDLG/OVERWRITE``) gates the commit. (``create`` is the "New"
  flavor — same name-entry path; ``new`` itself is a reserved keyword, hence the
  enum name.)
* **pick_dir** — choose a directory. The list shows folders only, the filter and
  name field are hidden, and **Select Folder** confirms the highlighted folder
  (or, with nothing selected, the directory you are browsing).

A filter's ``ext`` is a ``;``-separated pattern list, so ``"png;jpg;jpeg"`` is one
"Images" filter matching all three. ``file_filters(...)`` builds the typed list
from the terse C# pipe form: ``"daslang|das|Images|png;jpg|All files|"``.

Keyboard: *Enter* / *Keypad-Enter* confirms (or, when the path bar has focus,
navigates to it); *Backspace* goes up a directory when no text field is being
edited; *Escape* cancels.

Navigation and sorting
=======================

The current directory is listed with ``daslib/fio``'s ``dir`` + per-entry
``stat`` (so directories are always shown for navigation, files are filtered by
the active extension). A clickable breadcrumb of ancestor directories plus a
``..`` button drive ``navigate_to``; directories always sort first. The
**editable path bar** (``FILEDLG/PATH_FIELD`` + ``FILEDLG/GO``) jumps to any typed
absolute path — including a different drive on Windows — validated via
``stat().is_dir``, and **New Folder** (``FILEDLG/NEWFOLDER`` → the
``FILEDLG/NEWDIR`` sub-modal) creates a directory with ``fio``'s ``mkdir_rec``.

The file list is a :ref:`data_table <tutorial_data_table>` with the
``Sortable`` flag and four ``table_setup_column`` calls tagged with stable
``user_id`` values (``COL_NAME`` / ``COL_SIZE`` / ``COL_TYPE`` / ``COL_DATE``). The
Modified column renders each entry's ``stat().mtime`` via the ``format_time``
builtin and sorts on the ``clock`` ``<`` / ``>`` operators. A ``sort_specs()``
block captures the active sort key and re-orders the view (directories first, then
the chosen column). Each row's first cell is a ``selectable_label`` with
``SpanAllColumns | AllowDoubleClick`` so a click anywhere on the row selects it and
a double-click enters / confirms.

Drivability
===========

Every interactive element is a registry-backed boost widget, so a driver can
operate the dialog by id:

.. list-table::
   :header-rows: 1

   * - Element
     - Path
     - Verb
   * - Modal
     - ``FILEDLG``
     - snapshot / ``imgui_open``
   * - Parent / breadcrumb
     - ``FILEDLG/UP`` · ``FILEDLG/CRUMB[i]``
     - ``imgui_click``
   * - Path bar / Go
     - ``FILEDLG/PATH_FIELD`` · ``FILEDLG/GO``
     - ``imgui_force_set {"value": "path"}`` then click ``GO``
   * - New Folder
     - ``FILEDLG/NEWFOLDER`` → ``FILEDLG/NEWDIR/NEWDIR_NAME`` · ``.../NEWDIR_OK`` · ``.../NEWDIR_CANCEL``
     - ``imgui_click`` / ``imgui_force_set``
   * - Filter dropdown
     - ``FILEDLG/FILTER_COMBO``
     - ``imgui_force_set {"value": N}``
   * - File table
     - ``FILEDLG/FILE_TABLE``
     - sort via header click
   * - File row *i*
     - ``FILEDLG/FILE_TABLE/ROW[i]``
     - ``imgui_click`` (select) · ``double_click`` (enter dir / open file)
   * - Filename field
     - ``FILEDLG/NAME_FIELD``
     - ``imgui_force_set {"value": "name"}``
   * - OK / Cancel
     - ``FILEDLG/BTN_OK`` · ``FILEDLG/BTN_CANCEL``
     - ``imgui_click``
   * - Overwrite confirm
     - ``FILEDLG/OVERWRITE/OW_YES`` · ``.../OW_NO``
     - ``imgui_click``

Standalone vs live
==================

Same convention as the other tutorials. ``daslang.exe`` runs the demo once and
exits at ``exit_requested()``; ``daslang-live`` keeps the window open and reloads
on source edits.

.. seealso::

   Full source: :download:`examples/tutorial/file_dialog.das <../../../examples/tutorial/file_dialog.das>`

   Integration test: ``tests/integration/test_file_dialog.das`` — drives the
   dialog through ``imgui_playwright`` (open → cancel, save → type → confirm).

   :ref:`data_table <tutorial_data_table>` — the sortable table the file list is
   built on. :ref:`popup_modal <tutorial_popup_modal>` — the modal container.
