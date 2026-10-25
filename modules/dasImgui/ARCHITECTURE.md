# dasImgui architecture

## 1. Browser callback routing {#browser-callback-routing}

On Emscripten, ImGui ignores the embedded GLFW scroll callback. Initialization
installs ImGui's browser callbacks against the persistent `#canvas` selector.
The wheel listener bypasses GLFW, so `das_imgui_set_real_input_callbacks` installs
and removes it alongside the GLFW callbacks to keep the real-input gate coherent.

The binding removes its resize and fullscreen listeners before deleting the backend.
Those listeners retain backend data; upstream shutdown only removes the wheel listener.
Ownership follows the context that installed the embedded-GLFW browser callbacks.
The contrib GLFW port retains its own callback lifecycle.

Embedded GLFW updates its polled cursor coordinates on touch-start without sending
its cursor-position callback. The browser mouse-button bridge queues that current
position before forwarding the button event to ImGui. This preserves a quick tap's
location even when press/release arrive before the next frame. It does not synthesize
additional GLFW cursor callbacks. Installation follows backend initialization and
real-input re-enabling; backend callback restoration removes the bridge again.

## 2. Touch {#touch}

On a touch device (`glfw_is_touch_device`, or `--imgui-touch` after the daslang `--`) the live
init bakes the font at 17 px instead of 14, sets `IsTouchScreen` (which the snapshot reports),
pads every hit box through `TouchExtraPadding`, widens the scrollbar and grab, and moves windows
only by their title bar, so a finger dragging over a window body never moves it. One finger stays
the mouse the backend already makes of it. Two fingers scroll: the harness feeds the frame's
finger list to `imgui_touch`, which on the frame the second finger lands releases the mouse
button (clearing that frame's whole event queue, so a press that landed with the second finger
never becomes a click) and hovers their centroid once, so the wheel lands on the window under
them; a hover every frame would hold each wheel a frame behind it under ImGui's trickle rule. It
then turns the centroid's motion into wheel events at the distance ImGui scrolls per unit - five
font heights vertically, two horizontally, at the in-frame font size - so content follows the
fingers one to one. A finger landing or lifting while two stay down moves the centroid but not
the content. When the fingers lift, the last delta decays over a few frames as a flick. The machine is a pure
function of the finger list, and `imgui_touch_inject` feeds a test's fingers into the same path;
injected fingers persist until the next injection, because a headless host runs thousands of
frames between two commands.

## 3. Modified Enter shortcuts

The pinned ImGui 1.92.6 input editor uses exact modifier shortcuts for Enter.
`patches/input_routing.cmake` adds Shift+Enter and Shift+KeypadEnter to that routing;
the existing multiline newline/validation policy still decides what they do. The
configure step is idempotent and refuses an unrecognized upstream implementation,
so an ImGui update requires reviewing the patch rather than silently dropping it.
The headless `test_io_synth_text.das` test drives both modified keys into a real
multiline widget and checks its resulting buffer. The same patch set preserves queued
text-before-mouse ordering so a mouse click cannot redirect an earlier text commit
into the newly focused editor.

## 4. Browser clipboard

The GLFW browser platform layer installs its own clipboard callbacks. The native
clipboard installer leaves them intact on Emscripten, where the desktop clipboard
module reports unsupported. Browser paste events supply bounded plain-text bytes;
`TextEncoder` converts DOM UTF-16 to well-formed UTF-8. ImGui consumes those bytes
through its normal paste operation, preserving selection replacement and undo.
Ordinary HTML inputs are excluded from the canvas clipboard listener.

The current backend owns the listener and buffer, gates them with real input and
text focus, and clears them on shutdown or owner replacement. Clipboard writes use
the asynchronous browser API without substituting an internal fake clipboard on
failure. Initial browser platform setup detects macOS so its Command shortcuts use
ImGui's logical Control mapping. Browser keyboard/paste coverage lives in
`tests/browser_input.cjs`; native key synthesis alone does not exercise this path.

## Browser composition ordering {#browser-composition-ordering}

The browser input bridge commits pending composition text in a capture-phase
`pointerdown` listener before GLFW queues the following `mousedown`. The committed
text therefore precedes the click that can focus another editor in ImGui's input
queue.

During composition, the browser textarea paints preedit at the published caret,
with the current ImGui text/background colors and line height. Its width grows
within the viewport. Commit/cancel, owner replacement and disabled input hide it.
Password fields use a native password input, keeping composition visually hidden
and protected in the browser accessibility tree. A native query checks the current
input flags whenever ownership synchronizes, including focus or flag changes which
leave the caret geometry unchanged. Changing protection mode replaces the host and
cancels its old composition.

`SetTextInputCaret` publishes platform IME data and records the requesting item ID
in context-owned hook data. A shutdown hook frees that data; only this frame or
the previous frame can supply an owner. The browser bridge prefers an active stock
item, otherwise the recorded custom owner. It never manufactures a persistent
ImGui ActiveId for a custom editor: doing so blocks rich-surface mouse handling and
ordinary controls. Component commands retain their normal shortcut routing.
Keyboard admission checks actual item ownership instead of WantTextInput, which
now also reflects the custom editor's own platform request. Caret publication
continues through blink-off frames and clipped primary segments.
On the desktop the same published caret feeds native composition (below).

## Native composition {#native-composition}

The desktop backend (`src/native_input.h`) is the native counterpart of the browser bridge,
on the input-method API of dasGlfw's patched GLFW (`modules/dasGlfw/ARCHITECTURE.md`, the
input-method patch). It attaches to the context that initializes the GLFW backend, through an
end-of-frame context hook, and replaces ImGui's `Platform_SetImeDataFn`: on Windows that
default talks to IMM directly and would fight GLFW for the same window.

Each frame the hook reads the frame's platform IME data and the text-input owner
(`GetTextInputOwner` - the active stock item, else the custom editor's recorded owner). With
multi-viewports on, the owner's viewport may be a detached window: the state follows that
viewport's GLFW window - preedit callback, text-input focus and caret rectangle move with it,
and fall back to the main window when the detached one closes.

- GLFW's text-input focus follows the owner. While no field wants text the input method is
  off, so keys reach shortcuts and games unconverted. Focus is released only once no key has
  been held for a tenth of a second: Enter, Escape and Tab end editing while still down, and an
  X11 input context that loses focus before IBus has handed a key's release back lets IBus pass
  the next key after refocus through unconverted.
- The caret rectangle goes to `glfwSetPreeditCursorRectangle` whenever it moves; the
  candidate window opens beside the caret.
- A click finishes the composition into the field it leaves, as the browser bridge does. A
  start-of-frame context hook finds a mouse press in ImGui's input queue before ImGui handles
  it. When the preedit is known (macOS, Windows) it
  inserts the preedit text ahead of the press and cancels the input method's composition. On
  X11 IBus reports no preedit in the over-the-spot style, so the hook resets the input context
  and holds the press, and everything queued after it, for one frame: an engine that commits on
  reset (Hangul, Mozc) sends its text within a millisecond, and the press goes back into the
  queue behind it. An engine that discards on reset (libpinyin) loses its composition, as it
  does in every X11 client. A press is finished once, by its event id. An owner change by any
  other route cancels the composition.
- The preedit GLFW reports is drawn on the foreground draw list at the caret: the field's
  background, the text, one underline per conversion block (thicker under the focused block),
  and the input method's caret. It is drawn with the font the field used: the `InputText`
  wrappers and `SetTextInputCaret` record the current font for the active owner. A field
  outside those paths falls back to the default font.

Committed text arrives as ordinary character events, so stock fields and the custom editor
insert it with no composition-specific code. The patched GLFW holds back the keys the input
method consumes - the Enter that confirms a conversion, Backspace and Escape inside the
composition - so they never reach ImGui. On X11 GLFW uses the over-the-spot style: the input
method draws the preedit itself at the published spot, and no preedit callback arrives.

When real input is detached (`imgui_set_real_input_callbacks(false)`), composition is cancelled
and text-input focus is released; owner tracking and drawing continue, so a test can inject
preedit (`imgui_ime_inject_preedit`, one block or two with the second focused) and read back
the owner, text, caret, text-input focus, published rectangle, drawn overlay, its marks and its
font (`imgui_ime_*`). The headless backend has no window; the same state and overlay
run there with no GLFW calls.

## 6. A texture reference crosses the native JIT by address {#texture-ref-abi}

The native JIT passes a managed struct argument by address. `ImTextureRef` is a managed
struct, so an ImGui entry point that takes it by value receives a pointer where it expects
the struct, and the texture handle does not survive the call. `ImDrawList::AddImage`,
`AddImageQuad`, `AddImageRounded`, `PushTexture` and `_SetTexture` are therefore excluded
from the generator (`imgui_skip_func` in `bind/bind_imgui.das`) and hand-bound in
`src/dasIMGUI.main.cpp` through wrappers taking `const ImTextureRef &`, which the
interpreter and the JIT both call with a pointer. `tests/texture_ref_abi.das` runs
interpreted and under `-jit` (CTest `imgui_texture_ref_abi_interpreted` /
`imgui_texture_ref_abi_jit`) and asserts the handle survives each of the five calls.

## Widget path cache {#widget-path-cache}

`widget_path_key` caches each constructed `<container path>/<ident>` string by
exact container path and leaf identifier. Repeated lookup of the same pair reuses
the string without allocating another one. Cache insertion owns copies of both
key components, so caller-owned buffers can be reused. Bare identifiers remain unchanged.

The cache retains at most 8192 paths. Once full, lookups still construct valid keys
but retain no new entries. The next `begin_frame` deletes the cache's tables so
changing identifiers cannot grow retained table storage without bound. Constructed
path strings remain managed: registry, dispatch and caller references keep them alive
through cache turnover and normal string collection. The path reuse tests exercise
identity, bounded storage, resumed caching and references across collection.

A working set of at least 8192 distinct paths each frame saturates the cache each
frame and gets no cross-frame reuse. The bounded reset policy trades that extra
work for limited retention; reuse within a frame is still possible.
