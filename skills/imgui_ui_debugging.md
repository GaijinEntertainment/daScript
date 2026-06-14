# Fixing UI bugs in dasImgui — the discipline (CRITICAL UI SKILL)

UI is hard. The model can be correct while the pixels are wrong, and a fix that "looks right"
in the code routinely renders garbage. That gap is why **discipline here is paramount** — more
than anywhere else, you do not get to reason your way to "it works." You prove it.

This skill is the loop for diagnosing and fixing any dasImgui widget/interaction bug, and the
non-negotiables that keep you honest. Read it before touching imgui UI behavior.

## The loop — every step, in order

1. **Reproduce, and screenshot the bug.** Get it failing first — ideally a `dastest` (`with_imgui_app`
   for a GL test, or headless for pure logic). **Take a `screenshot` of the broken state regardless** —
   the "before." A bug you can't reproduce on demand isn't fixed when it merely stops happening.

2. **Make the bug observable in STRUCTURED state.** `imgui_snapshot` is ground truth — it dumps every
   registered widget's payload (`value`, `buffer`, `bbox`, `rendered`, `clicked`, …). Find the widget,
   read the exact field the bug is about. **If the field you need isn't in the snapshot — or the widget
   isn't even registered — that is a TOOLING gap: fix the inspection FIRST** (expose the field in the
   widget's snapshot payload, add a `cmd_*` accessor, register the tag) until the bug appears as a
   *wrong value* in structured data. You cannot fix what you cannot observe, and you cannot prove a fix
   you can't read.

3. **Fix it.** Now BOTH must hold: the test passes AND `imgui_snapshot` reports the correct value. A
   green test with a wrong snapshot value means your test asserts the wrong layer — fix the test too.

4. **Take the 'after' screenshot.** Show the pixels changed. Diff it against step 1 in your own head
   before you report.

5. **Leave the app demonstrating the fix.** Reload, drive it into the state that *shows* the fix (the
   item selected, the field populated, the menu behaving) and leave it there for the human to look at.

## Non-negotiables (this is why it's CRITICAL)

- **Model state ≠ rendered state.** A `cmd_*` model dump can be perfectly correct while the widget on
  screen shows the wrong thing. Verify the **rendered** layer (the widget's snapshot payload), not just
  the model. Canonical trap: an `input_text` whose `value` holds the right string while its `buffer` —
  the thing that actually renders — is empty.
- **A screenshot is for humans, not for proof.** Downscaled framebuffers can't resolve small text or
  numeric fields. Use screenshots for the before/after story; take the *verdict* from `imgui_snapshot`'s
  structured fields (or a test that asserts them).
- **"Probably works" = not verified = not done.** Never report a UI fix from the logic. If you did not
  read the corrected value out of the snapshot (or out of a test that asserts it), you do not know it
  works — and saying so anyway is how you lose trust on exactly the class of bug that hides best.
- **When two views disagree, find which layer each reads.** "The list marker says this item has data
  but the detail panel is blank" → one view reads the model, the other reads the rendered buffer. The
  disagreement *localizes* the bug — chase it, don't paper over one side.
- **Tests pass THROUGH UI bugs.** A smoke test that asserts "the widget exists / rendered" sails right
  past a widget rendering the *wrong* value. Add a regression assertion at the layer the bug lives in
  (assert the `buffer`/`value` the user actually sees), or it returns.

## Tooling (dasImgui live + playwright)

- `imgui_snapshot` — every widget + per-frame payload. **The ground truth.** Read the specific field.
- `screenshot {file}` — framebuffer PNG (before/after only).
- `live_reload` then `live_status` — hot-reload the fix; check `has_error == false` and `generation`
  bumped to confirm it took. **`live_status.paused` is the render-loop flag, not your app's logic pause**
  — never read app state from it; use a `cmd_*` query.
- `with_imgui_app` (imgui_playwright) — spawn a GL instance for a `[test]`. `find_widget(snap, path)`
  returns a widget's payload; drive with `imgui_click` / `cmd_*`; `wait_for_widget` / `wait_for_visible`
  before acting. Pass `--worker-index N` so the spawn lands on `9090+N` and doesn't collide with a
  running instance.
- The boost widget DSL registers each widget at a stable path — `CONTAINER/WIDGET`, indexed tables as
  `TABLE[key]` (string or int key). That path is what `find_widget` / `imgui_click` / snapshot keys use.

## Do NOT clobber a live session a human is using

Driving a human's running instance with synthetic input (`imgui_click`, `set_user_control(false)`) can
wedge it — a captured menu that won't close, detached real input, a held button. For path discovery and
snapshots, spawn a **throwaway worker** (`with_imgui_app`, or a second `--live-port`); reserve the human's
session for `live_reload` + verify. If you do wedge it, recover with:
`set_user_control {enabled:true}` (reattach real input) → `imgui_close {target}` (close a stuck menu) →
`imgui_mouse_stop` + `imgui_key_stop` (release held synthetic buttons/keys) → `cmd_pause` / your reset.

## ImGui state gotchas that LOOK like your bug

- **`input_text` renders its char BUFFER, not `state.value`.** `state.value = v` every frame never
  repaints. Reseed via the pending path (`pending_value` / `has_pending`), and gate the reseed on the
  **buffer content**, not `state.value` — value can desync from the buffer (a plain `.value=` write, or
  a hot reload, leaves them inconsistent) and a `value`-based guard then skips the reseed.
- **Context popups auto-open on ANY right-RELEASE.** `popup_context_window` / `popup_context_item` do
  not gate on drag distance, so a right-drag (e.g. a pan) that ends in a right-release pops the menu. For
  tap-vs-drag, drive a manual `popup_window` + `open_popup(...)` yourself on a clean release only.
- **`live_reload` preserves globals.** Stale widget state survives a reload, so a symptom can be a reload
  artifact — or be masked by one. When a result looks impossible, re-verify on a FRESH launch.
