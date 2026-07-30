# Text viewer

A small Unicode plain-text and Markdown viewer built with the in-tree dasImgui module.

```text
daslang examples/text/main.das -- README.md
daslang examples/text/main.das -- --plain LICENSE
daslang-live examples/text/main.das -- README.md
```

`.md` and `.markdown` files start in rendered Markdown mode; other files start
in plain-text mode. Use `--markdown` or `--plain` before the path to override
that choice. The View menu and bottom mode buttons can switch at runtime.

Markdown source keeps every original byte visible while styling markers such
as **strong delimiters**, `code fences`, and link punctuation. Unicode remains
cell-aligned across combining text, CJK, and color emoji: é · 中文 · 👩‍💻.

Every document, including the initial welcome/file document, is an ordinary
closable docked window. New documents join the central dockspace and inherit
the active document's zoom. Zoom is a fixed 5% step from 50–300% at the
bottom-right, and Ctrl+mouse-wheel adjusts it while the document is hovered.
On macOS, the usual Command key is used in place of Ctrl.

**File / Open...** and Ctrl+O add the selected document as another docked view
inside the running application. **Save**/Ctrl+S writes the canonical UTF-8
source; **Save As...** uses Ctrl+Shift+S. Untouched saves preserve the original
UTF-8 BOM, line endings, whitespace, and unsupported syntax byte-for-byte.

Keyboard, menu, toolbar, and live-API input invoke stable semantic command IDs.
Use **Edit / Edit Bindings...** to record, clear, reset, load, and save keyboard
bindings. The explicit per-user JSON config is loaded at startup but is written
only when **Save** is pressed in that window. Selection components expose Copy,
Select All, and Clear Selection operations without owning fixed keyboard keys.

GFM task-list checkboxes are the first editing operation. Clicking one changes
only the marker byte, reparses the rich projection, marks the source dirty, and
persists through Save/reload without normalizing the rest of the file.

The application owns its presentation policy. `FontRoleSet` supplies faces and
the base size, `MarkdownTypography` supplies relative heading/code sizes, and
each document supplies its zoom. The viewer therefore hard-codes sensible
typography without adding those implementation details to user preferences.
